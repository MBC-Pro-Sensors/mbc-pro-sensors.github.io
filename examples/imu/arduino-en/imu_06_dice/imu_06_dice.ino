/*
 * IMU Gyro x Arduino  Example 6: Electronic dice (which face is up?)
 * ------------------------------------------------------------
 * What does this example do?
 *   It uses the sensor as a dice: the sensor has 6 faces (+X, +Y, +Z, -X, -Y, -Z),
 *   and each face means 1 to 6 dots. Flip the sensor over and put it down.
 *   When it is still, the computer says "You rolled a 3!",
 *   and the built-in light on the Arduino board (the one marked L) blinks 3 times.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. We use the light built into the board.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Pick up the sensor, flip it, put it back on the table and do not touch it. You will see the dots in about 0.5 seconds.
 *   (Do not pull the jumper wires too tight, so they do not come loose when you flip it)
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address
const uint8_t LED = LED_BUILTIN;         // The light built into the board (pin 13 on UNO)

// How many dots each face means (like a real dice, two opposite faces add up to 7)
//                         +X  +Y  +Z  -X  -Y  -Z
const uint8_t DOTS[6] = {  2,  3,  1,  5,  4,  6 };   // 👉 You can change this: use the dots you like

bool rolling = true;                     // Is the dice still moving?

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
  Serial.println(F("Ready! Flip the sensor over, put it down, and wait until it is still."));
}

void loop() {
  // ---- 1. Read the status: box 0x31, 9 numbers ----
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {           // If reading fails, wait a little and try again
    delay(100);
    return;
  }
  uint8_t flags  = st[0];                // A row of "switches"
  uint8_t upFace = st[4];                // Which face is up: 0 to 5
  bool still = flags & (1u << 1);        // Switch 1 = not moving (it is still)

  // ---- 2. It started moving -> the dice is rolling ----
  if (!still) {
    if (!rolling) {                      // It just started moving; say it only once
      rolling = true;
      Serial.println(F("The dice is rolling..."));
    }
  }

  // ---- 3. It is still -> show the dots ----
  if (still && rolling && upFace < 6) {
    rolling = false;
    uint8_t n = DOTS[upFace];
    Serial.print(F("It stopped! You rolled a "));
    Serial.print(n);
    Serial.print(F(" (face up: "));
    printFace(upFace);
    Serial.println(F(")"));

    for (uint8_t i = 0; i < n; i++) {    // Blink once for each dot
      digitalWrite(LED, HIGH); delay(200);
      digitalWrite(LED, LOW);  delay(200);
    }
  }

  delay(50);
}

// Print the "face pointing up" number as words you can read
void printFace(uint8_t f) {
  switch (f) {
    case 0:  Serial.print(F("+X")); break;
    case 1:  Serial.print(F("+Y")); break;
    case 2:  Serial.print(F("+Z")); break;
    case 3:  Serial.print(F("-X")); break;
    case 4:  Serial.print(F("-Y")); break;
    case 5:  Serial.print(F("-Z")); break;
    default: Serial.print(F("?"));  break;
  }
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
