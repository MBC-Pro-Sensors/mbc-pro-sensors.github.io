/*
 * IMU Gyro x Arduino   Example 8: Calibrate the accelerometer and magnetometer
 * ------------------------------------------------------------
 * What does this example do?
 *   Type commands in the Serial Monitor to calibrate the gyro (the result is saved in its EEPROM):
 *     1 -> E1 start accelerometer calibration   2 -> E2 finish accelerometer calibration (save, and heading is set to zero automatically)
 *     3 -> E3 start magnetometer calibration    4 -> E4 finish magnetometer calibration (save)
 *   While calibrating, bit4 (accelerometer) or bit5 (magnetometer) of status flags F3 is 1. The program prints them every 0.5 s.
 *   Accelerometer calibration: press 1, then lay the gyro flat on each of its six sides for a few seconds, then press 2.
 *   Magnetometer calibration: press 3, then slowly turn the gyro in all directions (draw a figure 8) for about 20 s, then press 4.
 *
 * You need: IMU Gyro (Universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Gyro VCC -> 5V      Gyro GND -> GND
 *   Gyro SDA -> A4      Gyro SCL -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor, choose 115200 baud, type 1~4 in the input box at the top and press Enter.
 *
 * Tip:
 *   Not sure whether you need to calibrate? It is calibrated at the factory. Only do it when the heading drifts fast,
 *   or the "side facing up" is wrong.
 *   Keep away from motors, speakers and iron desks while calibrating.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x6A;               // The IMU Gyro's "house number" (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("Gyro not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("=== IMU calibration ==="));
  Serial.println(F("Type 1 = start accel calibration, 2 = finish; 3 = start mag calibration, 4 = finish"));
}

void loop() {
  // ---- 1. Did the computer send a character? ----
  if (Serial.available()) {
    char ch = Serial.read();
    switch (ch) {
      case '1': mbcWrite(ADDR, 0xE1); Serial.println(F(">>> E1 accel calibration started: lay it flat on each of the six sides for a few seconds")); break;
      case '2': mbcWrite(ADDR, 0xE2); Serial.println(F(">>> E2 accel calibration finished (saved, heading set to zero)")); break;
      case '3': mbcWrite(ADDR, 0xE3); Serial.println(F(">>> E3 mag calibration started: slowly draw a figure 8 for about 20 s")); break;
      case '4': mbcWrite(ADDR, 0xE4); Serial.println(F(">>> E4 mag calibration finished (saved)")); break;
      default: break;                    // Ignore other characters (for example the newline)
    }
  }

  // ---- 2. Every 0.5 s print the calibration status (bit4, bit5 of F3) ----
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    int16_t st = mbcRead(ADDR, 0xF3);
    if (st == MBC_NONE) { Serial.println(F("Cannot read the gyro")); return; }
    Serial.print(F("Accel calibrating ")); Serial.print((st & 0x10) ? F("yes") : F("no"));   // bit4
    Serial.print(F(" | Mag calibrating ")); Serial.print((st & 0x20) ? F("yes") : F("no"));  // bit5
    Serial.print(F(" | Mag fusion initialized ")); Serial.print((st & 0x40) ? F("yes") : F("no"));   // bit6
    Serial.print(F(" | Mag auto-cal converged ")); Serial.println((st & 0x80) ? F("yes") : F("no"));  // bit7
  }
  delay(50);
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] -> wait 2 ms -> read 2 x k bytes = k int16 values (little-endian) starting at that number
//   If the number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// From the sensor at address addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting at number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (ESP32-based products need this)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), up to ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
