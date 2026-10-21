/*
 * IMU Gyro x Arduino  Example 4: 90-degree turn alert
 * ------------------------------------------------------------
 * What does this example do?
 *   Turn the sensor on the table. Each time it turns 90 more degrees (1/4 of a circle),
 *   the built-in light on the Arduino board (the one marked L) blinks once,
 *   and the computer tells you "Turned right 90 degrees! Now at 1/4 turn".
 *   Robot contests often need a "90-degree turn". This example lets you feel how big 90 degrees is, by hand.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. We use the light built into the board.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put the sensor flat on the table and turn it slowly, like a steering wheel.
 *   Type z and press Enter to start again from 0.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address
const uint8_t LED = LED_BUILTIN;         // The light built into the board (pin 13 on UNO)

const int STEP_DEG = 90;                 // 👉 You can change this: alert after every this many degrees (for example 45 or 180)

long quarter = 0;                        // How many 90-degree steps we have turned (right is +, left is -)

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  Serial.println(F("Put the sensor on the table and do not move it..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}
  }
  zeroHeading();                         // Start counting from 0 degrees
  Serial.println(F("Ready! Turn the sensor slowly. The light blinks each time it turns 90 degrees."));
}

void loop() {
  // ---- 1. Start again from 0 when we get z ----
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'z' || c == 'Z') {
      zeroHeading();
      Serial.println(F("--- Reset! Counting again from 0 ---"));
    }
  }

  // ---- 2. Read the heading (box 0x11, 6 numbers), use numbers 2 and 3 (Gyro3D) ----
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {
    delay(100);
    return;
  }
  long heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);   // Turn right = bigger, turn left = smaller

  // ---- 3. Did it turn "one more" full 90 degrees? ----
  // It must turn a full 90 degrees "more" from the last 90-degree spot. Wobbling near the line will not keep blinking
  if (heading >= (quarter + 1) * STEP_DEG) {
    quarter = quarter + 1;
    Serial.print(F("Turned right "));
    Serial.print(STEP_DEG);
    Serial.print(F(" degrees! "));
    report();
  } else if (heading <= (quarter - 1) * STEP_DEG) {
    quarter = quarter - 1;
    Serial.print(F("Turned left "));
    Serial.print(STEP_DEG);
    Serial.print(F(" degrees! "));
    report();
  }

  delay(20);
}

// Show which 1/4 turn we are at now, and blink the light once
void report() {
  Serial.print(F("Now at "));
  Serial.print(quarter);
  Serial.print(F("/4 turn ("));
  Serial.print(quarter * STEP_DEG);
  Serial.print(F(" degrees)"));
  if (quarter != 0 && quarter % 4 == 0) {
    Serial.print(F("  That is exactly one full circle!"));
  }
  Serial.println();

  digitalWrite(LED, HIGH);               // Blink once
  delay(150);
  digitalWrite(LED, LOW);
}

// Reset the heading to 0
void zeroHeading() {
  mbcWrite(0x10, 0x01);                  // Write 0x01 into box 0x10 = "reset heading"
  delay(50);                             // Wait a little after the reset (at least 30 ms) before reading
  quarter = 0;
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
