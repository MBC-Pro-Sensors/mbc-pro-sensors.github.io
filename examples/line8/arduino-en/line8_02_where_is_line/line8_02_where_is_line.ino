/*
 * Pathfinder 8-Way x Arduino  Example 2: Where is the line?
 * ------------------------------------------------------------
 * What does this example do?
 *   Hold the sensor over a black line on white paper and move it left and right.
 *   The computer tells you "the line is on the left, in the middle, or on the right",
 *   and draws a small ruler that marks where the line is.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Good to know:
 *   The first time you use it, or on a new competition field, run "Example 4: One-key calibration" first. The readings will be more accurate.
 *   "Left" and "right" here mean: the sensor is mounted on the robot (sensing side down),
 *   viewed from behind the robot, looking forward.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address

// 👉 You can change this: 0 = follow a black line (black line on white), 1 = follow a white line (white line on black)
const uint8_t LINE_COLOR = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}                      // Stop here
  }

  mbcWrite(0x10, LINE_COLOR);            // Tell the sensor to look for a black line or a white line
  Serial.println(F("Ready! Put the sensor over the line and move it left and right."));
}

void loop() {
  // Read 7 numbers at once: the sensor puts all the "line results" starting at box 0x11
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {
    Serial.println(F("Read failed. Check that no wire is loose."));
    delay(500);
    return;
  }

  int8_t  position = (int8_t)d[0];       // Line position: negative = line on the left, positive = line on the right, 0 = right in the middle
  uint8_t groups   = d[3];               // How many lines it sees: 0 = no line

  if (groups == 0) {
    Serial.println(F("|         |  No line (the line is lost)"));
  } else {
    // Draw a small ruler: -8 on the far left, +8 on the far right, # marks where the line is
    Serial.print('|');
    for (int8_t p = -8; p <= 8; p += 2) {
      // Each cell is 2 units wide. Find the cell the line is in
      if (position >= p && position <= p + 1) Serial.print('#');
      else Serial.print(' ');
    }
    Serial.print(F("|  Position "));
    Serial.print(position);

    if (position > 1)       Serial.println(F("  → The line is on the right, turn right"));
    else if (position < -1) Serial.println(F("  → The line is on the left, turn left"));
    else                    Serial.println(F("  → The line is in the middle, go straight!"));
  }

  delay(200);                            // 👉 You can change this: look every 0.2 seconds. Smaller number = faster updates
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
