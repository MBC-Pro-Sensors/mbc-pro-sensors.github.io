/*
 * Ranger 2-Way x Arduino  Example 3: Is it on the left, on the right, or straight ahead?
 * ------------------------------------------------------------
 * What does this example do?
 *   The left and right lasers sit side by side and look forward, like two eyes:
 *     Both see something   -> it is straight ahead
 *     Only the left sees   -> it is to the left
 *     Only the right sees  -> it is to the right
 *     Neither sees         -> nothing in front
 *   When a robot "follows something" or "finds a target", step one is knowing which side it is on.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put a box (or your hand) about 20 cm in front of the sensor and move it slowly left and right.
 *
 * Good to know:
 *   Only things within 50 cm count (SEE_DIST), so a wall far behind is not taken as the target.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

const uint16_t SEE_DIST = 500;           // 👉 You can change this: how close counts as "seeing" (mm). 500 = 50 cm, 1199 at most

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // While the lasers start, distances are 0 (that would look like "something touching both sides"), so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Put something in front of the sensor and move it slowly left and right."));
}

void loop() {
  // Box 0x11: left distance (2 numbers) + right distance (2 numbers)
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  // "Sees it" = not broken, and close enough. A broken side counts as not seeing
  bool seeL = (left  != FAULT) && (left  < SEE_DIST);
  bool seeR = (right != FAULT) && (right < SEE_DIST);

  if (seeL && seeR) {
    Serial.print(F("↑ Straight ahead   distance "));
    Serial.print(min(left, right) / 10);           // The closer of the two sides, in cm
    Serial.println(F(" cm"));
  } else if (seeL) {
    Serial.print(F("↖ To the left      distance "));
    Serial.print(left / 10);
    Serial.println(F(" cm   -> the robot should turn left"));
  } else if (seeR) {
    Serial.print(F("↗ To the right     distance "));
    Serial.print(right / 10);
    Serial.println(F(" cm   -> the robot should turn right"));
  } else {
    Serial.println(F("   Nothing in front"));
  }

  if (left == FAULT || right == FAULT) {
    Serial.println(F("(Warning: one laser is broken. Run example 1 to check.)"));
  }

  delay(200);                            // 👉 You can change this: check every 0.2 seconds
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
