/*
 * MBC universal I2C × Arduino   Shared example 3: Scan the devices on the bus
 * ------------------------------------------------------------
 * What does this example do?
 *   Knocks on every I2C address 0x08~0x77. For every device that answers, it reads F0 "device ID" to tell which
 *   MBC product it is (a non-MBC device is printed as "other device"). Is the wiring OK, what address was it changed to,
 *   how many are on the bus: one run tells you. Scans every 3 seconds.
 *
 * You need: any MBC universal I2C edition sensor (several at once is fine, as long as the addresses differ).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200.
 */

#include <Wire.h>

const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC universal I2C: Scan ==="));
}

void loop() {
  uint8_t found = 0;
  Serial.println(F("Scanning 0x08 ~ 0x77 ..."));
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    Wire.beginTransmission(a);           // Knock on the door
    if (Wire.endTransmission() != 0) continue;   // Nobody answers → next one
    found++;
    Serial.print(F("  0x"));
    if (a < 0x10) Serial.print('0');
    Serial.print(a, HEX);
    Serial.print(F("  "));

    int16_t id = mbcRead(a, 0xF0);       // Read the device ID
    if (id == MBC_NONE) { Serial.println(F("other device (not MBC, or not the universal I2C edition)")); continue; }
    uint8_t factory = (uint8_t)(id & 0xFF);
    uint8_t hw0     = (uint8_t)((id >> 8) & 0xFF);
    printProduct(factory, hw0);
    int16_t fw = mbcRead(a, 0xF2);
    Serial.print(F("  firmware ")); Serial.print(fw / 100); Serial.print('.'); Serial.print(fw % 100);
    Serial.print(F("  protocol v")); Serial.print(mbcRead(a, 0xF1));
    if (factory != a) { Serial.print(F("  (address was changed, factory is 0x")); Serial.print(factory, HEX); Serial.print(')'); }
    Serial.println();
  }
  if (!found) Serial.println(F("  No device at all: check that SDA/SCL are not swapped and VCC/GND are plugged in firmly"));
  Serial.print(F("Total: ")); Serial.print(found); Serial.println(F(" device(s)"));
  delay(50);
  delay(2950);
}

// Tell the product from the factory address + hw[0]
void printProduct(uint8_t factory, uint8_t hw0) {
  if      (factory == 0x16 && hw0 == 8)  Serial.print(F("Pathfinder 8-Way"));
  else if (factory == 0x16 && hw0 == 16) Serial.print(F("Pathfinder 16-Way"));
  else if (factory == 0x29 && hw0 == 2)  Serial.print(F("Ranger 2-Way"));
  else if (factory == 0x29 && hw0 == 8)  Serial.print(F("Ranger 8-Way"));
  else if (factory == 0x42 && hw0 == 1)  Serial.print(F("Controller PS2"));
  else if (factory == 0x42 && hw0 == 2)  Serial.print(F("Controller PS4"));
  else if (factory == 0x6A)              Serial.print(F("IMU 9-Axis Gyro"));
  else if (factory == 0x4C && hw0 == 1)  Serial.print(F("Sharpshooter (School edition)"));
  else if (factory == 0x4C && hw0 == 2)  Serial.print(F("Sharpshooter (WRO edition)"));
  else                                   Serial.print(F("Unknown MBC device"));
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
