/*
 * IMU Gyro x Arduino   Example 1: Say hello to the gyro
 * ------------------------------------------------------------
 * What does this example do?
 *   Checks that the gyro is connected. If it is, it prints the gyro's "ID card"
 *   (device ID, protocol version, firmware version) and the 8 bits of status
 *   flags F3 (still? calibrating? mounted crooked?).
 *   Run this example first every time you connect a new gyro.
 *
 * You need: IMU Gyro (Universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Gyro VCC -> 5V      Gyro GND -> GND
 *   Gyro SDA -> A4      Gyro SCL -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Tip:
 *   Keep the gyro still for about 0.5 s when it powers on. It learns "which side is up"
 *   by itself and sets the heading to zero.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the gyro

const uint8_t ADDR = 0x6A;               // The IMU Gyro's "house number" (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // Open the channel to the computer at 115200 baud
  mbcBegin();                            // Get I2C ready (helper at the bottom of the file)
  delay(500);                            // Give the gyro 0.5 s to boot

  Serial.println(F("=== IMU Gyro: Say hello to the gyro ==="));

  // Wait until the gyro says "I am ready" (bit0 of status flags F3), up to 3 seconds
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("Gyro not found! Please check:"));
    Serial.println(F("  1. SDA -> A4, SCL -> A5 (ESP32: 21 / 22). Are they swapped?"));
    Serial.println(F("  2. VCC -> 5V, GND -> GND. Plugged in firmly?"));
    Serial.println(F("  3. Is your gyro the Universal I2C edition? (SPIKE and EV3 editions cannot connect to Arduino)"));
    while (true) {}                      // Stop here if not found
  }

  // ---- F0 device ID: low byte = factory address (0x6A), high byte = hw[0] (IMU = 1) ----
  int16_t id = mbcRead(ADDR, 0xF0);
  Serial.print(F("Found it! Factory address 0x"));
  Serial.print((uint8_t)(id & 0xFF), HEX);
  Serial.print(F(", type code "));
  Serial.println((uint8_t)((id >> 8) & 0xFF));   // IMU = 1

  // ---- F1 protocol version (these examples are written for version 2) ----
  Serial.print(F("Protocol version: "));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 firmware version: major x 100 + minor (for example 102 = 1.2) ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("Firmware version: "));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 status flags: 8 bits, each bit means one thing ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("Current status:"));
  printFlags(st);

  // ---- F5 seconds since power-on ----
  Serial.print(F("Seconds since power-on: "));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("Wiring is correct. You can try the other examples now!"));
}

void loop() {
  // This example only checks once at power-on, so loop does nothing
}

// Print the 8 bits of F3, one line each
void printFlags(int16_t st) {
  Serial.print(F("  bit0 ready                   ")); Serial.println((st & 0x01) ? F("yes") : F("no"));
  Serial.print(F("  bit1 still                   ")); Serial.println((st & 0x02) ? F("yes") : F("no (moving)"));
  Serial.print(F("  bit2 mounting axis captured  ")); Serial.println((st & 0x04) ? F("yes (knows which side is up)") : F("no (keep it still and wait)"));
  Serial.print(F("  bit3 mounted crooked         ")); Serial.println((st & 0x08) ? F("yes (please mount it flatter)") : F("no"));
  Serial.print(F("  bit4 accel calibrating       ")); Serial.println((st & 0x10) ? F("yes") : F("no"));
  Serial.print(F("  bit5 mag calibrating         ")); Serial.println((st & 0x20) ? F("yes") : F("no"));
  Serial.print(F("  bit6 mag fusion initialized  ")); Serial.println((st & 0x40) ? F("yes (fused heading available)") : F("no"));
  Serial.print(F("  bit7 mag auto-cal converged  ")); Serial.println((st & 0x80) ? F("yes") : F("no"));
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
