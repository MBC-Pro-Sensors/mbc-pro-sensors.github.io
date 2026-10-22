/*
 * Ranger 2-Way × Arduino   Example 3: Is it nearer on the left or the right?
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads three boxes, "01 left cm", "02 right cm" and "03 nearest cm", draws the two
 *   lasers' distances as two bars, and tells you on which side the object is nearer.
 *   These are the three boxes used most for avoiding obstacles and finding an opponent.
 *
 * You need: Ranger 2-Way (universal I2C edition) + a box or your hand.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move your hand from left to right and watch the bars change.
 *
 * Which side is left and right?
 *   The sensor is mounted at the front of the robot (lasers pointing forward), and you stand behind the robot looking forward.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand from left to right. (Longer bar = farther away)"));
}

void loop() {
  int16_t v[3];                          // v[0] = 01 left cm, v[1] = 02 right cm, v[2] = 03 nearest cm
  if (!mbcReadMany(ADDR, 0x01, 3, v)) { Serial.println(F("Cannot read the sensor")); delay(500); return; }
  int16_t left = v[0], right = v[1], nearest = v[2];

  Serial.print(F("L "));
  printBar(left);
  Serial.print(F("  R "));
  printBar(right);

  if (nearest >= 120)        Serial.println(F("  → Nothing within 120 cm"));
  else if (left < right)     { Serial.print(F("  → Nearer on the left: ")); Serial.print(nearest); Serial.println(F(" cm")); }
  else if (right < left)     { Serial.print(F("  → Nearer on the right: ")); Serial.print(nearest); Serial.println(F(" cm")); }
  else                       { Serial.print(F("  → Straight ahead: "));     Serial.print(nearest); Serial.println(F(" cm")); }

  delay(50);
  delay(150);                            // Print one line every 0.2 s   👉 You can change this
}

// Draw 0~120 cm as a bar of 0~12 blocks
void printBar(int16_t cm) {
  int8_t n = cm / 10;
  if (n > 12) n = 12;
  Serial.print('[');
  for (int8_t i = 0; i < 12; i++) Serial.print(i < n ? '#' : '.');
  Serial.print(']');
  if (cm < 10)  Serial.print(' ');
  if (cm < 100) Serial.print(' ');
  Serial.print(cm);
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
