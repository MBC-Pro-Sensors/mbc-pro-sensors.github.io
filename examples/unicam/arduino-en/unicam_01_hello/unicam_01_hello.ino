/*
 * Sharpshooter Vision Universal v1 x Arduino  Example 1: Is it connected?
 * ------------------------------------------------------------
 * Camera menu: camera mode 1 Colour tracking, option 3 Standard (the sketch also switches it with E0)
 *
 * What does this example do?
 *   1. Read F0 device ID: low byte = 0x4C (address), high byte hw0 = 3 means "Universal"
 *      (hw0 = 1 is the School version, 2 is the WRO version; they read differently)
 *   2. Read F2 firmware version (100 = 1.0)
 *   3. Switch the camera to "mode 13" with the E0 command, then read 0x01 to confirm
 *   4. Then print the current mode number every second (it follows the camera menu too)
 *
 * Mode number = camera mode x 10 + option:
 *   11~14 colour tracking, 21~23 spot colour, 30 colour order, 41~43 line following
 *
 * You need: Sharpshooter Vision Universal v1 (I2C firmware), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring (the I2C pins are provisional until bench testing):
 *   camera 5V  -> external 5V (power bank or adapter, 2 A or more; do not draw it from the Arduino 5V pin)
 *   camera GND -> Arduino GND (the external supply GND too)
 *   camera IO3 (SDA) -> A4   camera IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   Note: use the "I2C" firmware. IO1/IO3 are shared with the LEGO serial version; the firmware is one or the other.
 *
 * How to see the result: after uploading, open the Serial Monitor at 115200 baud.
 */

#include <Wire.h>                        // built-in Arduino I2C library

// ==== MBC_UNICAM_TOP_BEGIN ====
const uint8_t ADDR = 0x4C;               // the camera's I2C address ("house number")
const int16_t MBC_NONE = -32768;         // code for "no value / cannot read"
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);   // helper declaration (the code is at the bottom)
// one blob: x, y pixels (0 = not found), w, h pixels, count = number of blobs, area %; -1 = not given in this mode
struct UcBlob { int16_t x, y, w, h, count, area; };
// one sample spot: h hue code 0~255, s saturation, v value 0~100; -1 = not given
struct UcSpot { int16_t h, s, v; };
// 14 line values (near/mid/far offset -100~100: line on the left = negative)
struct UcLine { int16_t nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_TOP_END ====

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {        // wait up to 8 s for the camera to boot
    Serial.println(F("Camera not found! Please check:"));
    Serial.println(F(" 1. The camera has external 5V and its GND is joined to the Arduino GND"));
    Serial.println(F(" 2. IO3 goes to SDA, IO1 goes to SCL (swap them if reversed)"));
    Serial.println(F(" 3. The camera runs the I2C firmware"));
    while (true) {}                      // stop here
  }

  // Read F0~F3 at once: [0] device ID [1] protocol version [2] firmware version [3] status flags
  int16_t id[4];
  if (!mbcReadMany(ADDR, 0xF0, 4, id)) { Serial.println(F("Cannot read F0~F3")); while (true) {} }
  uint8_t addrLow = id[0] & 0xFF;        // low byte: factory address
  uint8_t hw0 = (id[0] >> 8) & 0xFF;     // high byte: which version (3 = Universal)
  Serial.print(F("Address 0x")); Serial.print(addrLow, HEX);
  Serial.print(F("  hw0=")); Serial.print(hw0);
  if (addrLow == 0x4C && hw0 == 3) Serial.println(F("  -> Sharpshooter Vision Universal OK"));
  else if (hw0 == 1) Serial.println(F("  -> this is the School version, use the School examples"));
  else if (hw0 == 2) Serial.println(F("  -> this is the WRO version (HUE12), use the WRO examples"));
  else Serial.println(F("  -> unknown version"));
  Serial.print(F("Protocol version ")); Serial.print(id[1]);
  Serial.print(F("  Firmware ")); Serial.print(id[2] / 100);
  Serial.print('.'); Serial.println(id[2] % 100);

  // E0 switch: camera mode 1 (colour tracking), option 3 (standard XY + area) -> mode number 13
  ucSetMode(1, 3);                       // 👉 You can change this: e.g. ucSetMode(4, 1) = black line
  delay(200);
  Serial.print(F("Mode number after switching: "));
  Serial.println(ucModeNumber());        // should print 13
}

void loop() {
  if (millis() - lastPrint >= 1000) {    // print once per second
    lastPrint = millis();
    // 0x01~0x04: [0] mode number [1] option [2] colours taught [3] values in this layer
    int16_t m[4];
    if (!mbcReadMany(ADDR, 0x01, 4, m)) { Serial.println(F("Cannot read the camera")); return; }
    Serial.print(F("Mode number ")); Serial.print(m[0]);
    Serial.print(F("  option ")); Serial.print(m[1]);
    Serial.print(F("  taught ")); Serial.print(m[2]);
    Serial.print(F(" colours  values in this layer ")); Serial.println(m[3]);
  }
  delay(20);
}

// ==== MBC_UNICAM_HELPERS_BEGIN ====
// ============================================================
// Communication helpers (MBC I2C protocol v2) + UniCam helpers. Same in every example, no need to change or understand.
//   Read: write [id] -> wait 2 ms -> read 2*k bytes = k int16 values from that id (little-endian)
// ============================================================

// Start I2C (call once in setup). UNO/Nano: SDA=A4, SCL=A5; ESP32: GPIO21/22
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // does not hang if a wire is loose
#endif
}

// Read k values (1~16) starting at id into out; true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // at most 16 values (32 bytes) at a time
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // the camera (ESP32) needs a 2 ms wait before reading
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();
    uint16_t hi = Wire.read();
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value; MBC_NONE if it cannot be read
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][params...]; true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (F3 bit0 = 1), at most ms milliseconds
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}

// ---------- Sharpshooter Vision Universal ----------

// Switch camera mode (E0): camMode 1~4, sub = option (0 for colour order). Saved in the camera
bool ucSetMode(uint8_t camMode, uint8_t sub) {
  uint8_t p[2] = { camMode, sub };
  bool ok = mbcWrite(ADDR, 0xE0, p, 2);
  delay(50);                             // give the camera time to switch
  return ok;
}

// Move a sample spot (E1): n = spot 1~20, x 0~159, y 0~119. Saved in the camera
bool ucSetPoint(uint8_t n, uint8_t x, uint8_t y) {
  uint8_t p[3] = { n, x, y };
  return mbcWrite(ADDR, 0xE1, p, 3);
}

// The camera's current mode number (e.g. 13 = colour tracking 3); MBC_NONE if it cannot be read
int16_t ucModeNumber() {
  return mbcRead(ADDR, 0x01);
}

// values per colour in colour tracking (modes 11~14)
uint8_t ucStride(uint8_t mode) {
  if (mode == 11) return 2;              // X Y
  if (mode == 12) return 3;              // X Y count
  if (mode == 13) return 4;              // X Y area count
  if (mode == 14) return 6;              // X Y width height count area
  return 0;
}

// Read the blob of colour (1~16); mode = the mode number you chose, 11~14. true if read
bool ucReadBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  uint8_t k = ucStride(mode);
  if (k == 0 || color < 1 || color > 16) return false;
  int16_t d[6];
  if (!mbcReadMany(ADDR, 0x10 + (color - 1) * k, k, d)) return false;
  b.x = d[0]; b.y = d[1];
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 12) b.count = d[2];
  if (mode == 13) { b.area = d[2]; b.count = d[3]; }
  if (mode == 14) { b.w = d[2]; b.h = d[3]; b.count = d[4]; b.area = d[5]; }
  return true;
}

// How many of the 16 target colours are found (modes 11~14)
int16_t ucColorsFound(uint8_t mode) {
  uint8_t k = ucStride(mode);
  if (k == 0) return MBC_NONE;
  return mbcRead(ADDR, 0x10 + 16 * k);
}

// Read sample spot point (1~16); mode = 21, 22, 23
bool ucReadSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (mode < 21 || mode > 23 || point < 1 || point > 16) return false;
  uint8_t k = mode - 20;                 // 21 -> 1 value, 22 -> 2, 23 -> 3
  int16_t d[3];
  if (!mbcReadMany(ADDR, 0x10 + (point - 1) * k, k, d)) return false;
  p.h = d[0];
  p.s = (k >= 2) ? d[1] : -1;
  p.v = (k >= 3) ? d[2] : -1;
  return true;
}

// Colour order (mode 30): hues gets the hue code of each block from left to right (up to maxN, <= 16); returns how many; total = all blocks
uint8_t ucReadOrder(int16_t *hues, uint8_t maxN, int16_t &total) {
  int16_t d[16];
  total = mbcRead(ADDR, 0x20);
  if (total == MBC_NONE || !mbcReadMany(ADDR, 0x10, 16, d)) { total = MBC_NONE; return 0; }
  uint8_t n = (total < 16) ? (uint8_t)total : 16;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = d[i];
  return n;
}

// Line following (modes 41~43): read 14 values at once
bool ucReadLine(UcLine &l) {
  int16_t d[14];
  if (!mbcReadMany(ADDR, 0x10, 14, d)) return false;
  l.nearOff = d[0]; l.midOff = d[1]; l.farOff = d[2]; l.angle = d[3];
  l.width = d[4]; l.flags = d[5]; l.junction = d[6]; l.junctionDist = d[7];
  l.leftEdge = d[8]; l.rightEdge = d[9]; l.farWidth = d[10];
  l.marker = d[11]; l.markerCount = d[12]; l.lost = d[13];
  return true;
}

// Hue code -> colour number: 1 red 2 orange 3 yellow 4 green 5 cyan 6 blue 7 purple 8 white/grey 9 black
uint8_t ucColorCode(int16_t h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_HELPERS_END ====
