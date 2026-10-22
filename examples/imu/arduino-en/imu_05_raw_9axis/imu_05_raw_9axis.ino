/*
 * IMU Gyro x Arduino   Example 5: Raw 9-axis values
 * ------------------------------------------------------------
 * What does this example do?
 *   Every 0.2 s it prints the nine raw values in numbers 10~18 (9 values in one read):
 *     10~12 gyro rate X/Y/Z, in 0.1 deg/s
 *     13~15 acceleration X/Y/Z, in mg (1000 = 1 g; lying flat, Z is about 1000)
 *     16~18 magnetic field X/Y/Z, in mG (milligauss)
 *   These are the "physical axis" values (the X, Y, Z printed on the board), for your own algorithms.
 *
 * You need: IMU Gyro (Universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Gyro VCC -> 5V      Gyro GND -> GND
 *   Gyro SDA -> A4      Gyro SCL -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Lying flat, accel Z is about 1000 and X/Y about 0; bring a magnet close and watch the magnetic values change.
 *
 * Tip:
 *   Keep the gyro still for about 0.5 s when it powers on. It learns "which side is up"
 *   by itself and sets the heading to zero.
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
  Serial.println(F("Ready!"));
  Serial.println(F("Gyro X/Y/Z(0.1deg/s) | Accel X/Y/Z(mg) | Mag X/Y/Z(mG)"));
}

void loop() {
  if (millis() - lastPrint >= 200) {     // 👉 You can change this: print every 0.2 s
    lastPrint = millis();

    // Read numbers 10~18 (9 values) in one go: [0..2] gyro rate [3..5] accel [6..8] magnetic
    int16_t r[9];
    if (!mbcReadMany(ADDR, 0x10, 9, r)) { Serial.println(F("Cannot read the gyro")); return; }

    char line[96];
    snprintf(line, sizeof(line), "%6d %6d %6d | %6d %6d %6d | %6d %6d %6d",
             r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8]);   // Each number takes 6 columns
    Serial.println(line);
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
