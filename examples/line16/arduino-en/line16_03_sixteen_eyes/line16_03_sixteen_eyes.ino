/*
 * Pathfinder 16-Way x Arduino  Example 3: What do the 16 eyes see?
 * ------------------------------------------------------------
 * What does this example do?
 *   Under the Pathfinder there are 16 light sensors, like 16 eyes in a row.
 *   This example shows what each eye sees:
 *     ● = this eye sees the line      ○ = this eye sees the background
 *   Below that, it shows each eye's "color score": 0 = very dark, 100 = very bright.
 *   16 scores in one row is too long, so they are split into two rows: "left 8 eyes" and "right 8 eyes".
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   The printed order is the real order with the sensor on the robot, viewed from behind the robot,
 *   looking forward: the left eye is printed on the left.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address
const uint8_t EYES = 16;                 // The Pathfinder 16-Way has 16 eyes

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  mbcWrite(0x10, 0);                     // Look for a black line (● = sees black). For a white line, change this to 1
  Serial.println(F("Ready! Move the sensor over the line and see which eyes turn into ●"));
}

void loop() {
  // Box 0x11: 7 numbers of line results. Numbers 5 and 6 together tell "which eyes see the line"
  uint8_t d[7];
  // Box 0x50: 16 numbers, the color score of each eye, 0 ~ 100
  uint8_t score[EYES];

  if (!mbcRead(0x11, d, 7) || !mbcRead(0x50, score, EYES)) {
    Serial.println(F("Read failed. Check that no wire is loose."));
    delay(500);
    return;
  }

  // Join the 2 numbers into one: it has 16 "bits", one bit for each eye
  uint16_t onLine = ((uint16_t)d[4] << 8) | d[5];

  // Eye numbers: eye 0 is on the far left, eye 15 on the far right.
  // So printing from eye 0 to eye 15 gives the same left and right as the real sensor.
  Serial.print(F("Left "));
  for (int8_t i = 0; i < EYES; i++) {
    if (onLine & (1u << i)) Serial.print(F("● "));
    else                    Serial.print(F("○ "));
    if (i == 7) Serial.print(F("| "));   // Draw a divider in the middle: left 8 eyes, right 8 eyes
  }
  Serial.println(F("Right"));

  // Print the scores in two rows: eyes 0 ~ 7 (left) and eyes 8 ~ 15 (right)
  Serial.print(F("Left 8 eyes:  "));
  printScores(score, 0, 7);
  Serial.print(F("Right 8 eyes: "));
  printScores(score, 8, 15);
  Serial.println();

  delay(300);                            // 👉 You can change this: look every 0.3 seconds
}

// Print the scores from eye "from" to eye "to". Each score takes 4 spaces, so they line up neatly
void printScores(uint8_t *score, int8_t from, int8_t to) {
  for (int8_t i = from; i <= to; i++) {
    Serial.print(' ');
    if (score[i] < 100) Serial.print(' ');   // For lining up: add spaces in front of numbers with fewer than 3 digits
    if (score[i] < 10)  Serial.print(' ');
    Serial.print(score[i]);
  }
  Serial.println();
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
