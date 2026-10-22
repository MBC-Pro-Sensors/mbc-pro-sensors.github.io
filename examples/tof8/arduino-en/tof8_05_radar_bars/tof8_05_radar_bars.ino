/*
 * Ranger 8-Way × Arduino   Example 5: Radar bar chart
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads "10~17 lasers 1~8 distance cm" in one go and draws the 8 directions as 8 bars:
 *   a longer bar = that direction is more open, a very short bar = something is blocking it. One look tells you where you can go.
 *   It also reads "08 fault bitmap" and marks a failed laser with XX.
 *
 * You need: Ranger 8-Way (universal I2C edition) + a few books as obstacles.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200.
 *
 * Where do the 8 lasers look? (standing behind the robot, looking forward)
 *   Laser 1 left 90°, laser 2 left 60°, laser 3 left 30°, lasers 4 and 5 straight ahead,
 *   laser 6 right 30°, laser 7 right 60°, laser 8 right 90°
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int8_t ANGLE[8] = {-90, -60, -30, 0, 0, 30, 60, 90};   // Angle of each laser (negative = left)

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Put a few books next to the sensor."));
}

void loop() {
  int16_t cm[8];
  int16_t fault = mbcRead(ADDR, 0x08);   // 08 fault bitmap: bit0 = laser 1
  if (!mbcReadMany(ADDR, 0x10, 8, cm) || fault == MBC_NONE) { Serial.println(F("Cannot read the sensor")); delay(500); return; }

  Serial.println(F("====== Radar view (longer bar = more open) ======"));
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(i + 1);                 // Laser number (1~8)
    Serial.print(' ');
    if (ANGLE[i] >= 0) Serial.print('+');
    if (ANGLE[i] > -10 && ANGLE[i] < 10) Serial.print(' ');
    Serial.print(ANGLE[i]); Serial.print(F("° |"));

    if ((fault >> i) & 1) {              // This laser failed
      Serial.println(F("XX fault"));
      continue;
    }
    int8_t n = cm[i] / 5;                // One cell per 5 cm, at most 24 cells
    if (n > 24) n = 24;
    for (int8_t k = 0; k < n; k++) Serial.print('#');
    if (cm[i] >= 120) Serial.println(F("> nothing"));
    else { Serial.print(' '); Serial.print(cm[i]); Serial.println(F(" cm")); }
  }
  delay(50);
  delay(450);                            // Redraw every 0.5 s   👉 You can change this
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
