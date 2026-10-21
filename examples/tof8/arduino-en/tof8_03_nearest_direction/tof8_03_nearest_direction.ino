/*
 * Ranger 8-Way x Arduino  Example 3: Which way is the nearest thing?
 * ------------------------------------------------------------
 * What does this example do?
 *   The sensor compares the 8 lasers by itself and finds which direction the nearest thing is in,
 *   and how far away it is. The computer draws a small ruler with a * where the thing is,
 *   and an arrow that tells you the direction:
 *     ←  left    ↖  front-left    ↑  straight ahead    ↗  front-right    →  right
 *   Sumo robots use this to "find the opponent", and cars use it to "follow your hand".
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Hold your hand about 30 cm in front of the sensor and move it slowly from left to right.
 *
 * What is the bearing number?
 *   -7 (far left) ~ 0 (straight ahead) ~ +7 (far right). Negative = left, positive = right.
 *   When the thing is right between two lasers, you get the number in between (for example -6, -4).
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing there

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then, so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Hold your hand in front of the sensor and move it slowly from left to right."));
}

void loop() {
  // Box 0x11: 6 numbers
  //   Number 0: which direction the nearest thing is in (-7 left ~ +7 right)
  //   Numbers 1, 2: how far the nearest thing is (mm)
  //   Numbers 3 ~ 5: direction and distance of the farthest (most open) way. Not used here
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  int8_t   dir = (int8_t)d[0];                         // Direction: turn it into a number with a +/- sign
  uint16_t mm  = ((uint16_t)d[1] << 8) | d[2];         // Join 2 numbers into the distance

  // First make sure something is really there, then look at the direction. With nothing there, the direction means nothing
  if (mm >= NO_TARGET) {
    Serial.println(F("Left [...............] Right   Nothing within 120 cm in front"));
    delay(300);
    return;
  }

  // ---- 1. Draw the small ruler: 15 spots for -7 ~ +7. Put a * where the thing is ----
  Serial.print(F("Left ["));
  for (int8_t pos = -7; pos <= 7; pos++) {
    if (pos == dir)      Serial.print('*');            // The thing is here
    else if (pos == 0)   Serial.print('+');            // Mark the middle
    else                 Serial.print('.');
  }
  Serial.print(F("] Right   "));

  // ---- 2. Show the direction with an arrow ----
  if (dir <= -6)      Serial.print(F("← Left       "));  // 👉 You can change this: where each zone starts and ends
  else if (dir <= -2) Serial.print(F("↖ Front-left "));
  else if (dir <= 1)  Serial.print(F("↑ Ahead      "));
  else if (dir <= 5)  Serial.print(F("↗ Front-right"));
  else                Serial.print(F("→ Right      "));

  // ---- 3. Print the distance (cm) and the bearing number ----
  Serial.print(F("  "));
  Serial.print(mm / 10);                               // mm divided by 10 = cm
  Serial.print(F(" cm (bearing "));
  Serial.print((int)dir);                              // Change to int before printing, so it prints as a number
  Serial.println(F(")"));

  delay(300);                                          // 👉 You can change this: check every 0.3 seconds
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
