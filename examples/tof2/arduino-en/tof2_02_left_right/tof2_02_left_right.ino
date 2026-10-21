/*
 * Ranger 2-Way x Arduino  Example 2: How far is each side?
 * ------------------------------------------------------------
 * What does this example do?
 *   The Ranger 2-Way has two lasers, left and right. Both look straight ahead.
 *   This example prints the distance each laser measures (unit: cm) and draws it as a bar:
 *     number        = how far away the thing in front is
 *     nothing there = nothing within 120 cm on this side
 *     FAULT         = this laser is broken
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Cover only the left laser with your hand, then only the right one. Which number changes?
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing on this side
const uint16_t FAULT = 0xFFFF;           // 0xFFFF (65535) = this laser is broken

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // While the lasers start, distances are 0, so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Cover the left or the right laser with your hand."));
}

void loop() {
  // Box 0x11: 4 numbers
  //   Numbers 0, 1 together = left distance (mm)
  //   Numbers 2, 3 together = right distance (mm)
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];   // Join 2 numbers into one distance
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  Serial.print(F("Left  "));
  printSide(left);
  Serial.print(F("   Right "));
  printSide(right);
  Serial.println();

  delay(300);                            // 👉 You can change this: check every 0.3 seconds
}

// Print one side's distance: a short bar + cm. Order of checks: broken first, then nothing there, and only then the distance
void printSide(uint16_t mm) {
  if (mm == FAULT) {
    Serial.print(F("[   FAULT    ] This laser is broken"));
    return;
  }
  if (mm >= NO_TARGET) {
    Serial.print(F("[############] nothing there"));
    return;
  }
  // Draw the bar: one block per 100 mm (10 cm), 12 blocks at most. Longer = farther
  Serial.print('[');
  uint8_t blocks = mm / 100;
  for (uint8_t k = 0; k < 12; k++) Serial.print(k < blocks ? '#' : ' ');
  Serial.print(F("] "));
  Serial.print(mm / 10);                 // mm divided by 10 = cm
  Serial.print(F(" cm"));
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
