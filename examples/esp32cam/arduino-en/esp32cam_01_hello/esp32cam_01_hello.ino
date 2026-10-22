/*
 * Sharpshooter ESP32CAM x Arduino   Example 1: Say hello to the Sharpshooter
 * ------------------------------------------------------------
 * What does this example do?
 *   Checks that the Sharpshooter is connected. If it is, it prints the sensor's
 *   "ID card" (device ID, School edition or WRO edition, protocol version,
 *   firmware version) and the status flags F3 (ready? is a color blob found?).
 *   Run this example first every time you connect a new Sharpshooter.
 *
 * You need: Sharpshooter ESP32CAM (I2C firmware), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Sharpshooter 5V  -> 5V       Sharpshooter GND -> GND
 *   Sharpshooter IO3 (SDA) -> A4   Sharpshooter IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   * Hardware confirmed; the I2C firmware bench test is still pending. IO1/IO3 are shared with the EV3 / SPIKE serial editions, so only one firmware at a time.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Tip:
 *   The Sharpshooter needs a few seconds to boot (camera init + screen), so the example waits up to 8 s.
 *   This example runs on both the School and the WRO edition; the high byte of F0 tells you which one you have.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the Sharpshooter

const uint8_t ADDR = 0x4C;               // The Sharpshooter's "house number" (I2C address). School and WRO editions are both 0x4C
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // Open the channel to the computer at 115200 baud
  mbcBegin();                            // Get I2C ready (helper at the bottom of the file)
  delay(500);

  Serial.println(F("=== Sharpshooter ESP32CAM: Say hello to the Sharpshooter ==="));

  // Wait until the Sharpshooter says "I am ready" (bit0 of status flags F3), up to 8 seconds
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("Sharpshooter not found! Please check:"));
    Serial.println(F("  1. IO3 (SDA) -> A4, IO1 (SCL) -> A5 (ESP32: 21 / 22). Are they swapped?"));
    Serial.println(F("  2. 5V and GND plugged in firmly? Is the Sharpshooter's screen on?"));
    Serial.println(F("  3. Is the Sharpshooter flashed with the I2C firmware? (EV3 and SPIKE editions use the same pins and cannot connect to Arduino)"));
    while (true) {}                      // Stop here if not found
  }

  // ---- F0 device ID: low byte = factory address (0x4C), high byte = edition (1 = School, 2 = WRO) ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t edition = (uint8_t)((id >> 8) & 0xFF);
  Serial.print(F("Found it! Factory address 0x"));
  Serial.println((uint8_t)(id & 0xFF), HEX);   // Should be 4C
  Serial.print(F("Edition: "));
  if (edition == 1)      Serial.println(F("School edition (color pick + blob tracking + motion, numbers 01~0A)"));
  else if (edition == 2) Serial.println(F("WRO edition HUE12 (12 color spots, numbers 10~1B, 20~2B)"));
  else                   Serial.println(F("Unknown edition"));

  // ---- F1 protocol version (these examples are written for version 2) ----
  Serial.print(F("Protocol version: "));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 firmware version: major x 100 + minor (for example 102 = 1.2) ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("Firmware version: "));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 status flags ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("Current status:"));
  Serial.print(F("  bit0 ready        ")); Serial.println((st & 0x01) ? F("yes") : F("no"));
  Serial.print(F("  bit1 blob found   ")); Serial.println((st & 0x02) ? F("yes") : F("no (School edition only; WRO edition always 0)"));

  // ---- F5 seconds since power-on ----
  Serial.print(F("Seconds since power-on: "));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("Wiring is correct. You can try the other examples now!"));
}

void loop() {
  // This example only checks once at power-on, so loop does nothing
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
