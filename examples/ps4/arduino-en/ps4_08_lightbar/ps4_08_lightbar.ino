/*
 * Controller PS4/PS5 x Arduino   Example 8: Set the light bar color with the sticks
 * ------------------------------------------------------------
 * What does this example do?
 *   Push the sticks to change the color of the gamepad's light bar:
 *     left stick left/right (LX) -> red, left stick up/down (LY) -> green, right stick left/right (RX) -> blue
 *     -100 ~ +100 becomes 0 ~ 255; all released = medium-brightness white.
 *   It uses command E6: write [0xE6][R][G][B], exactly 3 bytes.
 *   It only sends when the color changes (at most every 100 ms), so it does not flood the I2C bus.
 *
 * You need: Controller PS4/PS5 receiver (Universal I2C edition) + a PS4 or PS5 gamepad (already paired), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *
 * How to see the result:
 *   After uploading, push the sticks and watch the light bar change color. The Serial Monitor (115200) prints R, G, B.
 *
 * Tip:
 *   It is fine to send a color before the gamepad is connected: the receiver remembers it and applies it once connected. The default color is blue (0,0,255).
 *   A PS2 gamepad has no light bar; the PS2 receiver replies ACK to E6 but ignores it.
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

uint8_t lastRGB[3] = {255, 255, 255};    // The color we sent last time (on purpose different from the start, so the first pass sends)
unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Push the sticks to change the light bar color."));
}

void loop() {
  if (millis() - lastSend >= 100) {      // Send at most every 0.1 s
    // Read numbers 10~12 (3 values) in one go: [0] LX [1] LY [2] RX (-100 ~ +100)
    int16_t s[3];
    if (!mbcReadMany(ADDR, 0x10, 3, s)) { delay(100); return; }

    uint8_t rgb[3];
    rgb[0] = map(s[0], -100, 100, 0, 255);   // Red: left stick left/right
    rgb[1] = map(s[1], -100, 100, 0, 255);   // Green: left stick up/down
    rgb[2] = map(s[2], -100, 100, 0, 255);   // Blue: right stick left/right

    // Only send when the color changed (a difference of 1 or less counts as no change, so a slightly shaky stick does not keep sending)
    if (abs(rgb[0] - lastRGB[0]) > 1 || abs(rgb[1] - lastRGB[1]) > 1 || abs(rgb[2] - lastRGB[2]) > 1) {
      mbcWrite(ADDR, 0xE6, rgb, 3);      // Command E6: [R][G][B], must be exactly 3 bytes
      lastSend = millis();
      memcpy(lastRGB, rgb, 3);
      Serial.print(F("Light bar -> R ")); Serial.print(rgb[0]);
      Serial.print(F("   G ")); Serial.print(rgb[1]);
      Serial.print(F("   B ")); Serial.println(rgb[2]);
    }
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
