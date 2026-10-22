/*
 * Ranger 8-Way × Arduino   Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   Checks that the sensor is connected. If it is, it prints the sensor's ID card
 *   (device ID, protocol version, firmware version) and its current status
 *   (are all 8 lasers working?).
 *   Run this example first every time you connect a new sensor.
 *
 * You need: Ranger 8-Way (universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Note:
 *   Waking up the 8 lasers one by one takes 2~4 s. Before the sensor is ready every distance is 0, so always wait for F3 bit0 first.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the sensor

const uint8_t ADDR = 0x29;               // The sensor's "house number" (I2C address). Both 2-Way and 8-Way use 0x29
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // Open the channel to the computer at 115200 baud
  mbcBegin();                            // Get I2C ready (helper at the bottom of the file)
  delay(500);                            // Wait 0.5 s for the sensor to boot

  Serial.println(F("=== Ranger 8-Way: Say hello to the sensor ==="));
  Serial.println(F("Waiting for the 8 lasers to boot (at most 6 s)..."));

  // Wait for the sensor to say "I'm ready" (bit0 of status flags F3 = all 8 lasers initialized), at most 6 s
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found! Please check:"));
    Serial.println(F("  1. SDA to A4, SCL to A5 (ESP32: 21 / 22). Are they swapped?"));
    Serial.println(F("  2. VCC to 5V, GND to GND. Are they plugged in firmly?"));
    Serial.println(F("  3. Is your sensor the universal I2C edition? (SPIKE and EV3 editions do not work with Arduino)"));
    while (true) {}                      // Not found: stop here
  }

  // ---- F0 device ID: low byte = factory address, high byte = number of lasers ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t factoryAddr = (uint8_t)(id & 0xFF);        // Factory address (stays the same even if the address was changed)
  uint8_t sensors     = (uint8_t)((id >> 8) & 0xFF); // hw[0] = how many lasers
  Serial.print(F("Found it! Factory address 0x"));
  Serial.print(factoryAddr, HEX);
  Serial.print(F(", "));
  Serial.print(sensors);                 // Ranger 8-Way = 8
  Serial.println(F(" lasers"));

  // ---- F1 protocol version (this example is written for version 2) ----
  Serial.print(F("Protocol version: "));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 firmware version: major×100 + minor (e.g. 102 = 1.2) ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("Firmware version: "));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 status flags: each bit means one thing ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("Current status:"));
  Serial.print(F("  bit0 ready            ")); Serial.println((st & 0x01) ? F("yes (all 8 lasers initialized)") : F("no"));
  Serial.print(F("  bit1 any laser failed ")); Serial.println((st & 0x02) ? F("yes! (run Example 2 to see which one)") : F("no (all 8 lasers OK)"));

  // ---- 07 init bitmap: bit0 = laser 1 (far left) ... bit7 = laser 8 ----
  int16_t initOk = mbcRead(ADDR, 0x07);
  Serial.print(F("Lasers that booted OK (left→right): "));
  for (uint8_t i = 0; i < 8; i++) Serial.print((initOk >> i) & 1 ? 'O' : 'X');
  Serial.println(initOk == 255 ? F("  all OK") : F("  a laser marked X failed to boot, please power off and on"));

  // ---- F5 seconds since boot ----
  Serial.print(F("Seconds since boot: "));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("Wiring is correct. You can try the other examples now!"));
}

void loop() {
  // This example only checks once at startup, so loop does nothing
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
