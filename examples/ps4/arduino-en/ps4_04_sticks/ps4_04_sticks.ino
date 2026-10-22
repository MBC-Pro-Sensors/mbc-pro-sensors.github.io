/*
 * Controller PS4/PS5 x Arduino   Example 4: How far is the stick pushed?
 * ------------------------------------------------------------
 * What does this example do?
 *   Turns the position of the two sticks into numbers (number 10 LX, 11 LY, 12 RX, 13 RY):
 *     released (in the middle) = 0; pushed right or up = positive, all the way = +100;
 *     pushed left or down = negative, all the way = -100.
 *   Prints the numbers every 0.3 s and draws two "cross charts": O is where the stick is now.
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
 *   After uploading, open the Serial Monitor and choose 115200 baud. Push the stick slowly and watch the O move.
 *
 * Tip:
 *   When the stick wobbles a little near the middle, the number stays at 0. The receiver does this on purpose
 *   (it is called a "dead zone"), so a car does not creep when you let go of the stick.
 *   The first time, you must pair: within 30 s after the receiver powers on (or after you press Reset),
 *   hold SHARE + PS on a PS4 gamepad, or Create + PS on a PS5 gamepad, until the light bar blinks fast.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Push the sticks."));
}

void loop() {
  if (millis() - lastPrint >= 300) {     // 👉 You can change this: print every 0.3 s
    lastPrint = millis();

    // Read numbers 10~13 (4 values) in one go: [0] LX [1] LY [2] RX [3] RY (-100 ~ +100)
    int16_t s[4];
    if (!mbcReadMany(ADDR, 0x10, 4, s)) { Serial.println(F("Cannot read the receiver")); return; }

    Serial.print(F("Left stick X ")); Serial.print(s[0]);
    Serial.print(F("  Y ")); Serial.print(s[1]);
    Serial.print(F("   |   Right stick X ")); Serial.print(s[2]);
    Serial.print(F("  Y ")); Serial.println(s[3]);
    drawCross(s[0], s[1], s[2], s[3]);   // Draw the two cross charts
  }
  delay(50);
}

// Draw two 7x7 cross charts (left stick and right stick side by side). The center is +, the stick position is O
//   X: -100 ~ +100 -> column 0 ~ 6 (left -> right); Y: up is positive, so +100 is the top row
void drawCross(int lx, int ly, int rx, int ry) {
  int lcol = map(lx, -100, 100, 0, 6), lrow = map(ly, 100, -100, 0, 6);
  int rcol = map(rx, -100, 100, 0, 6), rrow = map(ry, 100, -100, 0, 6);
  for (int row = 0; row < 7; row++) {
    for (int col = 0; col < 7; col++) Serial.print(cell(row, col, lrow, lcol));
    Serial.print(F("     "));            // 5 spaces between the two charts
    for (int col = 0; col < 7; col++) Serial.print(cell(row, col, rrow, rcol));
    Serial.println();
  }
  Serial.println();
}

// Decide which character to print in one cell: O = stick position, + = center, | and - = cross lines, . = empty
char cell(int row, int col, int posRow, int posCol) {
  if (row == posRow && col == posCol) return 'O';
  if (row == 3 && col == 3) return '+';
  if (col == 3) return '|';
  if (row == 3) return '-';
  return '.';
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
