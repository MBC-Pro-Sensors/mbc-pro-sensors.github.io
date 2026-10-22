/*
 * Ranger 8-Way × Arduino   Example 4: Which side is most open?
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads "03 most open distance cm" and "04 most open direction": the sensor finds the direction
 *   among the 8 that is farthest (most empty) for you. In a maze or when avoiding obstacles, use these two boxes to "go where it is most open".
 *   It also reads 01/02 (the nearest) and prints both, so you can compare "where to avoid" and "where to go".
 *
 * You need: Ranger 8-Way (universal I2C edition) + a few books as walls.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Use books to build different "walls" on the left, right and front of the sensor.
 *
 * Note:
 *   A failed laser always reads 120, so it looks like the "most open" direction. Run Example 1 before a match to check that all 8 lasers are OK.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Build a few walls with books next to the sensor."));
}

void loop() {
  int16_t v[4];                          // 01 nearest cm, 02 nearest direction, 03 most open cm, 04 most open direction
  if (!mbcReadMany(ADDR, 0x01, 4, v)) { Serial.println(F("Cannot read the sensor")); delay(500); return; }
  int16_t nearCm = v[0], nearDir = v[1], farCm = v[2], farDir = v[3];

  Serial.print(F("Most open → "));
  printDir(farDir);
  Serial.print(' '); Serial.print(farCm); Serial.print(F(" cm"));

  Serial.print(F("   |   Nearest → "));
  if (nearCm >= 120) Serial.println(F("nothing within 120 cm around"));
  else {
    printDir(nearDir);
    Serial.print(' '); Serial.print(nearCm); Serial.println(F(" cm"));
  }
  delay(50);
  delay(250);                            // Print one line every 0.3 s   👉 You can change this
}

// Print a direction -7 ~ +7 as words (each step ≈ 13°, ±7 ≈ ±90°)
void printDir(int16_t dir) {
  if      (dir <= -6) Serial.print(F("left"));
  else if (dir <= -2) Serial.print(F("front left"));
  else if (dir <=  1) Serial.print(F("ahead"));
  else if (dir <=  5) Serial.print(F("front right"));
  else                Serial.print(F("right"));
  Serial.print(F("(")); Serial.print(dir); Serial.print(')');
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
