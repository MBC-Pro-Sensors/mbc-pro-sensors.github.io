/*
 * Ranger 8-Way x Arduino  Example 5: Radar bar chart
 * ------------------------------------------------------------
 * What does this example do?
 *   It draws the distances of the 8 lasers as 8 bars, one bar per laser:
 *     Long bar  = that direction is open (things are far away)
 *     Short bar = something is blocking that direction
 *   One look tells you "where the obstacles are and which way is free", like a small radar.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put the sensor on a table. Block the front, the left and the right with a book.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint8_t LASERS = 8;                // 8 lasers
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing there
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

// About which angle each laser looks at (negative = left, positive = right, 0 = straight ahead)
const int ANGLE[LASERS] = {-90, -60, -30, 0, 0, 30, 60, 90};

const uint16_t MM_PER_BLOCK = 50;        // 👉 You can change this: one # = 50 mm (5 cm). 1200 mm = exactly 24 #

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then, so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Block the front, the left and the right of the sensor with a book."));
}

void loop() {
  // Box 0x21: the distances of the 8 lasers, 2 numbers each, 16 in total
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  Serial.println();
  Serial.println(F("====== Radar (longer bar = more open) ======"));

  for (uint8_t i = 0; i < LASERS; i++) {
    uint16_t mm = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];   // Join 2 numbers into the distance

    // ---- 1. Print "which laser, which angle" ----
    Serial.print(i);
    if (ANGLE[i] > 0)       Serial.print(F("  +"));            // Put a + in front of right-side angles
    else if (ANGLE[i] == 0) Serial.print(F("   "));
    else                    Serial.print(F("  "));
    if (abs(ANGLE[i]) < 10) Serial.print(' ');                 // To line things up
    Serial.print(ANGLE[i]);
    Serial.print(F(" deg |"));

    // ---- 2. Draw the bar ----
    if (mm == FAULT) {                                         // First check if it is broken
      Serial.println(F("XXXXXXXX  This laser is broken"));
      continue;                                                // Done with this laser, go to the next one
    }
    uint16_t blocks = mm / MM_PER_BLOCK;                       // How many # to draw
    for (uint16_t k = 0; k < blocks; k++) Serial.print('#');

    // ---- 3. Write the distance after the bar ----
    if (mm >= NO_TARGET) {
      Serial.println(F("> nothing there"));
    } else {
      Serial.print(F(" "));
      Serial.print(mm / 10);                                   // mm divided by 10 = cm
      Serial.println(F(" cm"));
    }
  }

  delay(500);                                                  // 👉 You can change this: draw every 0.5 seconds
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
