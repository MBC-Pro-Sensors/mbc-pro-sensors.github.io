/*
 * IMU Gyro x Arduino  Example 3: Spirit level
 * ------------------------------------------------------------
 * What does this example do?
 *   It turns the sensor into a "spirit level":
 *     Sensor is level  -> the built-in light on the Arduino board (the one marked L) turns on
 *     Sensor is tilted -> the light turns off, and the computer tells you which side is lower and by how many degrees
 *   How it works: gravity always pulls down. The "accelerometer" inside the sensor
 *   can feel which way this pull goes, so it knows if it is level or not.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. We use the light built into the board.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put the sensor on a book, then slowly lift one side of the book and watch the numbers change.
 *
 * Good to know:
 *   The sensor can lie flat, stand up, or lie on its side. The program finds which face is up by itself.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address
const uint8_t LED = LED_BUILTIN;         // The light built into the board (pin 13 on UNO)

const float LEVEL_DEG = 2.0;             // 👉 You can change this: less than 2 degrees of tilt counts as "level"

const char AXIS_NAME[3] = {'X', 'Y', 'Z'};   // The names of the three directions

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // Set the light pin to "output" so we can turn it on or off
  mbcBegin();
  delay(500);

  Serial.println(F("Put the sensor on the table and do not move it..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}
  }
  Serial.println(F("Ready! Put the sensor on a book and lift one side of the book."));
}

void loop() {
  // ---- 1. Read the acceleration: box 0x23, 6 numbers (2 each for X, Y, Z) ----
  uint8_t d[6];
  if (!mbcRead(0x23, d, 6)) {            // If reading fails, wait a little and try again
    delay(100);
    return;
  }

  // Make 3 numbers. The unit is mg (1000 mg = 1 times gravity)
  // When it lies still, the direction pointing "up" reads about +1000
  int16_t a[3];
  for (uint8_t i = 0; i < 3; i++) {
    a[i] = (int16_t)(((uint16_t)d[2 * i] << 8) | d[2 * i + 1]);
  }

  // ---- 2. Find which direction points up (the one with the biggest "size") ----
  uint8_t up = 0;
  for (uint8_t i = 1; i < 3; i++) {
    if (abs(a[i]) > abs(a[up])) up = i;
  }

  // ---- 3. See how many degrees the other two directions are tilted ----
  bool isLevel = true;                   // First, guess that it is level
  for (uint8_t i = 0; i < 3; i++) {
    if (i == up) continue;               // Skip the direction that points up

    // Use "trigonometry" to change the force into an angle (atan2 is built into Arduino)
    float deg = atan2((float)a[i], (float)abs(a[up])) * 180.0 / PI;

    Serial.print(AXIS_NAME[i]);
    Serial.print(F(" axis: "));
    if (fabs(deg) <= LEVEL_DEG) {
      Serial.print(F("level                            "));
    } else {
      isLevel = false;
      // Negative number -> the "+" side is lower; positive number -> the "-" side is lower
      Serial.print(F("the "));
      Serial.print(deg < 0 ? '+' : '-');
      Serial.print(AXIS_NAME[i]);
      Serial.print(F(" side is lower by "));
      Serial.print(fabs(deg), 1);        // Show 1 digit after the decimal point
      Serial.print(F(" deg  "));
    }
  }

  // ---- 4. Turn on the light if it is level ----
  if (isLevel) {
    digitalWrite(LED, HIGH);
    Serial.println(F("-> Level!"));
  } else {
    digitalWrite(LED, LOW);
    Serial.println(F("-> Not level yet"));
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
