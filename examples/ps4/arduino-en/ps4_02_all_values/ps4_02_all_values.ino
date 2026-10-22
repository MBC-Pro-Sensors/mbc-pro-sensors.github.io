/*
 * Controller PS4/PS5 x Arduino   Example 2: Print all the values at once
 * ------------------------------------------------------------
 * What does this example do?
 *   Every 0.5 s it prints every value in the receiver's "value table", so you can
 *   see what each number holds:
 *     01~0B: buttons, connection, battery, triggers, system buttons, gamepad type (11 values in one read)
 *     10~13: the four stick axes LX, LY, RX, RY (4 values in one read)
 *   Numbers that are next to each other can be read "in one row" (mbcReadMany),
 *   and you get the values from the same instant.
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
 *   After uploading, open the Serial Monitor and choose 115200 baud. Press buttons, push the sticks, and watch which number changes.
 *
 * Tip:
 *   The first time, you must pair: within 30 s after the receiver powers on (or after you press Reset),
 *   hold SHARE + PS on a PS4 gamepad, or Create + PS on a PS5 gamepad, until the light bar blinks fast.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;             // When we last printed

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Printing all values every 0.5 s."));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 You can change this: print every 0.5 s
    lastPrint = millis();

    // ---- Part 1: numbers 01~0B, 11 values in one read ----
    int16_t a[11];
    if (!mbcReadMany(ADDR, 0x01, 11, a)) { Serial.println(F("Cannot read (part 1)")); return; }
    // ---- Part 2: numbers 10~13, 4 values in one read ----
    int16_t s[4];
    if (!mbcReadMany(ADDR, 0x10, 4, s)) { Serial.println(F("Cannot read (part 2)")); return; }

    Serial.println(F("------------------------------"));
    Serial.print(F("01 16 buttons (PSX order, binary): ")); printBits16((uint16_t)a[0]); Serial.println();
    Serial.print(F("02 D-pad (up1 right2 down4 left8)     : ")); Serial.println(a[1]);
    Serial.print(F("03 Shape buttons (▲1 ●2 ✕4 ■8)        : ")); Serial.println(a[2]);
    Serial.print(F("04 Shoulder (L2 1 R2 2 L1 4 R1 8)      : ")); Serial.println(a[3]);
    Serial.print(F("05 Function (SELECT1 L3 2 R3 4 START8) : ")); Serial.println(a[4]);
    Serial.print(F("06 Connected (1 = yes)                 : ")); Serial.println(a[5]);
    Serial.print(F("07 Battery 0~100        : ")); Serial.println(a[6]);
    Serial.print(F("08 L2 analog 0~255                     : ")); Serial.println(a[7]);
    Serial.print(F("09 R2 analog 0~255                     : ")); Serial.println(a[8]);
    Serial.print(F("0A System buttons (b0 PS, b1 capture)  : ")); Serial.println(a[9]);
    Serial.print(F("0B Gamepad type 0~4                    : ")); Serial.println(a[10]);
    Serial.print(F("10 LX  11 LY  12 RX  13 RY (±100): "));
    for (uint8_t i = 0; i < 4; i++) { Serial.print(s[i]); Serial.print(i < 3 ? F("  ") : F("")); }
    Serial.println();
  }
  delay(50);
}

// Print a 16-bit number as 16 zeros and ones (left = bit15 ■, right = bit0 SELECT)
void printBits16(uint16_t v) {
  for (int8_t i = 15; i >= 0; i--) {
    Serial.print((v >> i) & 1);
    if (i % 4 == 0 && i > 0) Serial.print(' ');   // A space every 4 bits, easier to read
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
