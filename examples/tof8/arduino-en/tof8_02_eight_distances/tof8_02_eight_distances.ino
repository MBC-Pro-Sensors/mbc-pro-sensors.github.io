/*
 * Ranger 8-Way x Arduino  Example 2: How far does each of the 8 lasers see?
 * ------------------------------------------------------------
 * What does this example do?
 *   The Ranger 8-Way has 8 lasers in a half circle. They look from the left all the way to the right.
 *   This example prints the distance each laser measures (unit: millimeters mm, 10 mm = 1 cm):
 *     number = how far away the thing in front is
 *     ----   = nothing in this direction (farther than 120 cm)
 *     XX     = this laser is broken
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Move your hand or a book left and right in front of the sensor. Which number changes?
 *
 * Good to know:
 *   Laser 0 is on the far left, laser 7 is on the far right (left and right as seen
 *   from behind the sensor, looking the same way it looks).
 *   Lasers 3 and 4 both point straight ahead.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint8_t LASERS = 8;                // The Ranger 8-Way has 8 lasers
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing in this direction
const uint16_t FAULT = 0xFFFF;           // 0xFFFF (65535) = this laser is broken

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then, so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand in front of the sensor."));
  Serial.println(F("(Unit: mm. ---- = nothing there, XX = broken)"));
  Serial.println(F("  Left                                     Right"));
  Serial.println(F("     0     1     2     3     4     5     6     7"));
}

void loop() {
  // Box 0x21: 16 numbers. Every 2 numbers make one laser's distance (laser 0 -> laser 7)
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  for (uint8_t i = 0; i < LASERS; i++) {
    // Join 2 numbers into one distance: the first one is the "big half"
    uint16_t mm = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];

    // The order of checks matters: first broken?, then nothing there?, and only then the distance
    if (mm == FAULT) {
      Serial.print(F("    XX"));          // This laser is broken
    } else if (mm >= NO_TARGET) {
      Serial.print(F("  ----"));          // Nothing in this direction
    } else {
      printRight(mm, 6);                  // Print the distance, right-aligned in 6 spaces
    }
  }
  Serial.println();

  delay(300);                             // 👉 You can change this: check every 0.3 seconds
}

// Print a number right-aligned, using width spaces in total (blank spaces in front)
void printRight(uint16_t value, uint8_t width) {
  uint8_t digits = 1;
  for (uint16_t v = value; v >= 10; v /= 10) digits++;   // Count how many digits the number has
  for (uint8_t k = digits; k < width; k++) Serial.print(' ');
  Serial.print(value);
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
