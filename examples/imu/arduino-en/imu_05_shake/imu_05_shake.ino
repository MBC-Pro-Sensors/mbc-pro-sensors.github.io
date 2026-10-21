/*
 * IMU Gyro x Arduino  Example 5: Shake detector
 * ------------------------------------------------------------
 * What does this example do?
 *   Pick up the sensor and shake it hard (or twist it quickly).
 *   The built-in light on the Arduino board (the one marked L) turns on,
 *   the computer counts "how many times you shook it", and tells you how fast it just turned.
 *   How it works: the gyro measures "how fast it is turning". The faster it turns, the bigger the number.
 *   When the number goes over a limit, we count it as "one shake".
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. We use the light built into the board.
 *   Push the jumper wires in firmly so they do not come loose when you shake.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Wait until you see "Ready", then pick up the sensor and shake it.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address
const uint8_t LED = LED_BUILTIN;         // The light built into the board (pin 13 on UNO)

const float SHAKE_SPEED = 300.0;         // 👉 You can change this: turning faster than "300 degrees per second" counts as one shake
                                         //    (smaller = easier to trigger, bigger = you must shake harder)

bool shaking = false;                    // Are we in the middle of "one shake" now?
int  shakeCount = 0;                     // How many shakes in total
float peak = 0;                          // The fastest speed during this shake

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
  Serial.println(F("Ready! Pick up the sensor and shake it."));
}

void loop() {
  // ---- 1. Read the turning speed: box 0x22, 6 numbers (2 each for X, Y, Z) ----
  uint8_t d[6];
  if (!mbcRead(0x22, d, 6)) {            // If reading fails, wait a little and try again
    delay(100);
    return;
  }

  // Make 3 numbers. The unit is "0.1 degree per second", so divide by 10 to get "degrees per second"
  float g[3];
  for (uint8_t i = 0; i < 3; i++) {
    int16_t raw = (int16_t)(((uint16_t)d[2 * i] << 8) | d[2 * i + 1]);
    g[i] = raw / 10.0;
  }

  // The "turning speed" of all three directions together (always positive, no matter which way it turns)
  float speed = sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

  // ---- 2. Was that "one shake"? ----
  if (!shaking && speed > SHAKE_SPEED) {
    shaking = true;                      // The shake has started
    peak = speed;
    digitalWrite(LED, HIGH);             // Light on
  }
  if (shaking) {
    if (speed > peak) peak = speed;      // Remember the fastest speed of this shake
    if (speed < SHAKE_SPEED / 2) {       // It slowed down (below half the limit) -> this shake is over
      shaking = false;
      shakeCount = shakeCount + 1;
      digitalWrite(LED, LOW);
      Serial.print(F("Shake! Number "));
      Serial.print(shakeCount);
      Serial.print(F(" (fastest: "));
      Serial.print((int)peak);
      Serial.println(F(" degrees per second)"));
    }
  }

  delay(10);                             // Check very often so we do not miss a very fast shake
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
