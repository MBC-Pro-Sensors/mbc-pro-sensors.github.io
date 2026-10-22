/*
 * Pathfinder 8-Way × Arduino   Example 3: Where is the line?
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads three boxes: "01 line position", "02 fine position" and "03 line width".
 *   It draws a small ruler, marks the line with #, and tells you which way to turn.
 *   These three boxes are the data used most often for line following.
 *
 * You need: Pathfinder 8-Way (universal I2C edition) + white paper + black electrical tape (about 2 cm wide).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move the black line slowly from the left of the sensor to the right.
 *
 * What do "left" and "right" mean?
 *   The sensor is mounted on the robot (sensing side down). You stand behind the robot and look forward:
 *   line on the left = negative, line on the right = positive, 0 = center. Channel 1 is on the far left.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  mbcWrite(ADDR, 0xE0);                  // E0 = follow a black line (for a white line change to 0xE1)  👉 You can change this
  Serial.println(F("Ready! Move the black line slowly from left to right."));
}

void loop() {
  int16_t v[3];                          // v[0] = 01 line position, v[1] = 02 fine position, v[2] = 03 line width
  if (!mbcReadMany(ADDR, 0x01, 3, v)) {  // Read 3 boxes in one go starting from 01
    Serial.println(F("Cannot read the sensor"));
    delay(500);
    return;
  }
  int16_t pos   = v[0];                  // -8 (far left) ~ +8 (far right)
  int16_t fine  = v[1];                  // -100 ~ +100, finer than the line position
  int16_t width = v[2];                  // How many channels are on the line, 0 = no line in sight

  // Draw a small 9-cell ruler: one cell for each position -4 ~ +4 (-8~+8 divided by 2), # marks the line
  Serial.print('|');
  for (int8_t i = -4; i <= 4; i++) Serial.print((width > 0 && i == pos / 2) ? '#' : ' ');
  Serial.print(F("|  "));

  if (width == 0) {
    Serial.println(F("No line (the line is lost)"));
  } else {
    Serial.print(F("Position ")); Serial.print(pos);
    Serial.print(F("  Fine ")); Serial.print(fine);
    Serial.print(F("  Width ")); Serial.print(width);
    if      (pos < -1) Serial.println(F("  → The line is on the left, turn left"));
    else if (pos >  1) Serial.println(F("  → The line is on the right, turn right"));
    else               Serial.println(F("  → The line is in the middle, go straight!"));
  }
  delay(50);
  delay(150);                            // Print one line every 0.2 s so it is easy to read   👉 You can change this
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
