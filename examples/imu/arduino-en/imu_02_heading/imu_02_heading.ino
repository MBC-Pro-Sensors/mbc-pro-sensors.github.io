/*
 * IMU Gyro x Arduino  Example 2: How many degrees did I turn? (heading)
 * ------------------------------------------------------------
 * What does this example do?
 *   Turn the sensor slowly on the table. The computer keeps telling you
 *   "how many degrees it has turned now".
 *     Seen from above, turn "clockwise" (to the right)        -> the number gets bigger
 *     Seen from above, turn "counterclockwise" (to the left)  -> the number gets smaller (it can go below 0)
 *   The number keeps adding up: two turns to the right is 720 degrees. It does not jump back to 0.
 *   It also shows the "0 to 359 degrees" style, which goes back to 0 after one turn, like a clock.
 *   Type z in the Serial Monitor and press Enter to reset the angle (the direction now = 0 degrees).
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Wait until you see "Ready" before you start turning.
 *
 * Good to know:
 *   When the gyro powers on, it takes "the direction at that moment" as 0 degrees.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  Serial.println(F("Put the sensor on the table and do not move it..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}
  }
  Serial.println(F("Ready! Turn the sensor slowly and watch. Type z and press Enter to reset to 0."));
}

void loop() {
  // ---- 1. Reset to 0 when we get z ----
  if (Serial.available()) {
    char c = Serial.read();              // Read one letter you typed
    if (c == 'z' || c == 'Z') {
      mbcWrite(0x10, 0x01);              // Write 0x01 into box 0x10 = "reset heading"
      delay(50);                         // Wait a little after the reset (at least 30 ms) before reading, so you get the new 0
      Serial.println(F("--- Reset! This direction is now 0 degrees ---"));
    }
  }

  // ---- 2. Read the heading: box 0x11, 6 numbers (3 kinds of heading, 2 numbers each) ----
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {            // If reading fails, wait a little and try again
    delay(100);
    return;
  }

  // Numbers 2 and 3 are the "3D-compensated heading (Gyro3D)". This is the one we recommend most
  // The two numbers make one number that "can be negative": the first is the high part, the second is the low part
  int16_t heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);

  // Change it to 0 to 359 degrees (like a clock: after one turn it goes back to 0)
  int angle360 = ((heading % 360) + 360) % 360;

  Serial.print(F("Total angle: "));
  Serial.print(heading);
  Serial.print(F(" deg  |  0-359 style: "));
  Serial.print(angle360);
  Serial.println(F(" deg"));

  delay(200);                            // 👉 You can change this: print every 0.2 seconds
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
