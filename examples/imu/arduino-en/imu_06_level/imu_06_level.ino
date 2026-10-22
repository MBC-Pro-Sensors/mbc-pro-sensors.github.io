/*
 * IMU Gyro x Arduino   Example 6: Which side is up? Can we trust the magnetometer?
 * ------------------------------------------------------------
 * What does this example do?
 *   Every 0.3 s it reads numbers 08~0A (3 values in one read):
 *     0A Side facing up: 0 +X, 1 +Y, 2 +Z, 3 -X, 4 -Y, 5 -Z (the axes printed on the board)
 *     08 Magnetic trust 0~255: how much the fusion algorithm trusts the magnetometer (higher is better)
 *     09 Magnetic disturbance 0~255: is there iron or a motor nearby? (lower is better)
 *   When the side facing up changes it prints one line; lying flat (+Z up) the board's built-in LED turns on.
 *
 * You need: IMU Gyro (Universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Gyro VCC -> 5V      Gyro GND -> GND
 *   Gyro SDA -> A4      Gyro SCL -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Flip the gyro around and see which side is up; bring a magnet close and watch the disturbance.
 *
 * Tip:
 *   Keep the gyro still for about 0.5 s when it powers on. It learns "which side is up"
 *   by itself and sets the heading to zero.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in LED is, so use pin 2
#endif

const uint8_t ADDR = 0x6A;               // The IMU Gyro's "house number" (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int LED = LED_BUILTIN;             // The board's built-in LED (pin 13 on the UNO)

unsigned long lastPrint = 0;
int16_t lastFace = -1;                   // The side facing up last time (-1 = not seen yet)

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("Gyro not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Flip the gyro around."));
}

void loop() {
  if (millis() - lastPrint >= 300) {     // 👉 You can change this: check every 0.3 s
    lastPrint = millis();

    // Read numbers 08~0A (3 values) in one go: [0] magnetic trust [1] magnetic disturbance [2] side facing up
    int16_t v[3];
    if (!mbcReadMany(ADDR, 0x08, 3, v)) { Serial.println(F("Cannot read the gyro")); return; }

    digitalWrite(LED, v[2] == 2 ? HIGH : LOW);    // +Z up (lying flat) -> LED on

    if (v[2] != lastFace) {              // Only print when the side facing up changes
      lastFace = v[2];
      Serial.print(F("Side facing up: "));
      printFace(v[2]);
      Serial.print(F(" | Magnetic trust ")); Serial.print(v[0]);
      Serial.print(F(" | Magnetic disturbance ")); Serial.print(v[1]);
      if (v[1] > 100) Serial.print(F(" (big disturbance, iron or a motor nearby?)"));
      Serial.println();
    }
  }
  delay(50);
}

// 0A side facing up: 0 +X, 1 +Y, 2 +Z, 3 -X, 4 -Y, 5 -Z
void printFace(int16_t f) {
  switch (f) {
    case 0: Serial.print(F("+X (X axis up)")); break;
    case 1: Serial.print(F("+Y (Y axis up)")); break;
    case 2: Serial.print(F("+Z (flat, top side up)")); break;
    case 3: Serial.print(F("-X")); break;
    case 4: Serial.print(F("-Y")); break;
    case 5: Serial.print(F("-Z (flipped, bottom side up)")); break;
    default: Serial.print(F("?")); break;
  }
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
