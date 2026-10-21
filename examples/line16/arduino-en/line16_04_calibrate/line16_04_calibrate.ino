/*
 * Pathfinder 16-Way x Arduino  Example 4: One-key calibration
 * ------------------------------------------------------------
 * What does this example do?
 *   "Calibration" means the sensor learns what black and white look like on your competition field.
 *   On a new field, or when the lights are different, calibrate again. The result is saved in the sensor,
 *   so it remembers even after you turn it off.
 *
 * How to calibrate:
 *   1. Hold the sensor over the line (about 0.5 ~ 1 cm above the floor, the same height as on the robot).
 *   2. Type c in the input box at the top of the Serial Monitor and press Enter.
 *   3. In the next 5 seconds, slowly move the sensor left and right so all 16 eyes "sweep" over the black line and the white background.
 *   4. When you see "Calibration done", you are finished.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
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
  Serial.println(F("Ready! Type c in the input box at the top and press Enter to start calibrating."));
}

void loop() {
  // Did someone type something in the Serial Monitor?
  if (Serial.available()) {
    char key = Serial.read();            // Read one letter
    if (key == 'c' || key == 'C') {
      calibrate();
    }
  }
}

// All the calibration steps are here
void calibrate() {
  // Box 0x21 has 2 numbers. The 2nd one is "how many times it has been calibrated"
  uint8_t before[2];
  if (!mbcRead(0x21, before, 2)) {
    Serial.println(F("Read failed. Check that no wire is loose."));
    return;
  }

  // Write 1 into box 0x20 = start the 5-second calibration
  mbcWrite(0x20, 1);
  Serial.println(F("Calibration started! In the next 5 seconds, move the sensor left and right over the black line and the white background..."));

  // Keep checking if the "calibration count" went up by 1. If it did, calibration is done
  unsigned long start = millis();
  while (millis() - start < 7000) {      // Wait up to 7 seconds
    delay(250);
    Serial.print('.');                   // Print a dot to show it is still waiting

    uint8_t now[2];
    if (mbcRead(0x21, now, 2) && now[1] != before[1]) {
      Serial.println();
      Serial.println(F("Calibration done! The result is saved in the sensor and stays there even after power off."));
      showResult();
      return;
    }
  }

  Serial.println();
  Serial.println(F("Waited too long. Calibration did not finish. Type c again and try once more."));
}

// Show the "white" and "black" that each eye has learned
void showResult() {
  uint8_t white[EYES], black[EYES];
  if (!mbcRead(0x51, white, EYES) ||     // Box 0x51: the white each eye has learned (16 numbers)
      !mbcRead(0x52, black, EYES)) {     // Box 0x52: the black each eye has learned (16 numbers)
    Serial.println(F("Could not read the calibration result. Check that no wire is loose."));
    return;
  }

  Serial.println(F("Eye (left → right)  White Black   Gap"));
  for (int8_t i = 0; i < EYES; i++) {      // Eye 0 is on the far left, eye 15 on the far right (viewed from behind the robot, looking forward)
    Serial.print(F("  Eye "));
    if (i < 10) Serial.print(' ');       // For lining up: add a space in front of 1-digit numbers
    Serial.print(i);
    Serial.print(F("             "));
    printNum(white[i]);
    Serial.print(F("  "));
    printNum(black[i]);
    Serial.print(F("  "));
    printNum(white[i] - black[i]);
    Serial.println();
  }
  Serial.println(F("A bigger 'Gap' is better. A very small gap (less than 20) means that eye did not sweep over the black line. Please calibrate again."));
}

// Print a number that always takes 4 spaces, so the table lines up
void printNum(int v) {
  if (v < 1000 && v > -100) Serial.print(' ');
  if (v < 100 && v > -10)   Serial.print(' ');
  if (v < 10 && v >= 0)     Serial.print(' ');
  Serial.print(v);
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
