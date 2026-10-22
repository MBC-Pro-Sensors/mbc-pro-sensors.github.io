/*
 * Ranger 2-Way × Arduino   Example 6: Laser health check
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads "06 init bitmap", "07 fault bitmap" and "F3 status flags", and reports
 *   every second whether both lasers are healthy. Run it before a match to know if the sensor is broken.
 *     06: bit0 = left, bit1 = right, 1 = that laser initialized OK at boot (if it failed, there is no retry; power off and on again)
 *     07: bit0 = left, bit1 = right, 1 = that laser has failed now (cannot be read, or not updated for 200 ms)
 *   A failed laser always returns 120 cm / 1200 mm, which looks like "nothing there". These two boxes tell the difference.
 *
 * You need: Ranger 2-Way (universal I2C edition).
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

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== Ranger 2-Way: Laser health check ==="));
  // This example does not wait for "ready" on purpose, so you can still see in 06 which laser did not boot
  delay(3000);                           // Both lasers take at most about 3 s to boot
}

void loop() {
  int16_t v[2];                          // v[0] = 06 init, v[1] = 07 fault
  int16_t st = mbcRead(ADDR, 0xF3);
  if (!mbcReadMany(ADDR, 0x06, 2, v) || st == MBC_NONE) {
    Serial.println(F("Cannot read the sensor at all: check the wiring, or is it the SPIKE / EV3 edition?"));
    delay(1000);
    return;
  }
  int16_t initOk = v[0], fault = v[1];

  Serial.println(F("---------------------------"));
  Serial.print(F("F3 ready "));       Serial.print((st & 0x01) ? F("yes") : F("no"));
  Serial.print(F("  a laser failed "));  Serial.println((st & 0x02) ? F("yes") : F("no"));

  reportOne(F("Left"), initOk & 0x01, fault & 0x01);
  reportOne(F("Right"), initOk & 0x02, fault & 0x02);

  if (initOk == 3 && fault == 0) Serial.println(F("Both lasers are healthy!"));
  else Serial.println(F("Problem: power off and on first; if it is still the same, check whether that laser is covered or its connector is loose"));

  delay(50);
  delay(950);
}

// Print the status of one laser
void reportOne(const __FlashStringHelper *name, int16_t ok, int16_t bad) {
  Serial.print(F("  "));
  Serial.print(name);
  Serial.print(F(" laser: init "));
  Serial.print(ok ? F("OK") : F("failed"));
  Serial.print(F(", now "));
  Serial.println(bad ? F("FAILED!") : F("OK"));
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
