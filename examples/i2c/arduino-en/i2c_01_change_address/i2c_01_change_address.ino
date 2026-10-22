/*
 * MBC universal I2C × Arduino   Shared example 1: Change the I2C address (F8)
 * ------------------------------------------------------------
 * What does this example do?
 *   When two MBC sensors with the same "house number" must share one I2C bus (for example two Ranger 2-Way,
 *   both at 0x29), change one of them to another address first. How:
 *     write [F8][new address][inverted new address]  → saved in the sensor's EEPROM, takes effect after reboot
 *     write [F8][0x00][0xFF]                         → back to the factory address
 *   After the change it automatically sends [EF][0xA5] to reboot, then scans again to confirm the new address shows up.
 *
 * You need: any MBC universal I2C edition sensor (connect only one at a time!).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to use:
 *   1. Below, change OLD_ADDR to the sensor's current address and NEW_ADDR to the address you want (0x08~0x77).
 *   2. Upload, open the Serial Monitor at 115200, type y and Enter to change; type r and Enter to go back to factory.
 *   3. From now on, change ADDR in every other example to the new address.
 *
 * Tip:
 *   The low byte of F0 "device ID" is always the factory address, even after a change, so a scan can still tell which product it is.
 */

#include <Wire.h>

const uint8_t OLD_ADDR = 0x29;           // The sensor's current address   👉 Change this (Pathfinder 0x16, Ranger 0x29, Controller 0x42, IMU 0x6A, Sharpshooter 0x4C)
const uint8_t NEW_ADDR = 0x2A;           // The address you want           👉 Change this (0x08 ~ 0x77, must not clash with other devices)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC universal I2C: Change address ==="));
  Serial.print(F("MBC devices on the bus now:")); scan();
  if (!mbcWaitReady(OLD_ADDR, 6000)) {
    Serial.print(F("No sensor found at 0x")); Serial.print(OLD_ADDR, HEX); Serial.println(F(". Please check OLD_ADDR and the wiring."));
    while (true) {}
  }
  Serial.print(F("Found 0x")); Serial.print(OLD_ADDR, HEX);
  Serial.print(F(" (factory address 0x")); Serial.print(mbcRead(OLD_ADDR, 0xF0) & 0xFF, HEX); Serial.println(')');
  Serial.print(F("Type y to change to 0x")); Serial.print(NEW_ADDR, HEX); Serial.println(F("; type r to go back to the factory address. Then press Enter"));
}

void loop() {
  if (!Serial.available()) { delay(50); return; }
  char c = Serial.read();
  if (c == 'y') {
    if (NEW_ADDR < 0x08 || NEW_ADDR > 0x77) { Serial.println(F("NEW_ADDR must be between 0x08 and 0x77")); return; }
    uint8_t p[2] = { NEW_ADDR, (uint8_t)~NEW_ADDR };   // [new address][inverted new address]: both must match, so a mistake cannot change it
    Serial.print(F("Changing address → 0x")); Serial.println(NEW_ADDR, HEX);
    if (!mbcWrite(OLD_ADDR, 0xF8, p, 2)) { Serial.println(F("Write failed")); return; }
    rebootAndCheck(NEW_ADDR);
  } else if (c == 'r') {
    uint8_t p[2] = { 0x00, 0xFF };       // [F8][00][FF] = back to the factory address
    Serial.println(F("Back to the factory address"));
    if (!mbcWrite(OLD_ADDR, 0xF8, p, 2)) { Serial.println(F("Write failed")); return; }
    rebootAndCheck(0);                   // 0 = factory address unknown, just scan and see
  }
}

// Send the reboot command, wait for it to come back, then scan again
void rebootAndCheck(uint8_t expect) {
  delay(50);                             // Give the EEPROM a moment to finish writing
  uint8_t magic = 0xA5;
  mbcWrite(OLD_ADDR, 0xEF, &magic, 1);   // [EF][A5] = reboot (the new address only takes effect after reboot)
  Serial.println(F("Rebooting..."));
  delay(3000);                           // Ranger 8-Way needs 2~4 s to boot
  Serial.print(F("Scan result:")); scan();
  if (expect && mbcRead(expect, 0xF3) != MBC_NONE) {
    Serial.print(F("Success! The sensor is now at 0x")); Serial.println(expect, HEX);
    Serial.println(F("Remember to change ADDR in the other examples to the new address."));
  } else if (expect) {
    Serial.println(F("No answer at the new address. Power off and on, then scan again (some boards only update I2C after a power cycle)."));
  }
  Serial.println(F("(Change OLD_ADDR to the new address and upload again to change once more or go back to factory)"));
}

// Scan 0x08~0x77; for every device that answers, read F0 to see if it is an MBC device
void scan() {
  uint8_t found = 0;
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() != 0) continue;
    int16_t id = mbcRead(a, 0xF0);
    Serial.print(F(" 0x")); Serial.print(a, HEX);
    if (id != MBC_NONE) { Serial.print(F("(factory 0x")); Serial.print(id & 0xFF, HEX); Serial.print(')'); }
    found++;
  }
  if (!found) Serial.print(F(" (none)"));
  Serial.println();
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
