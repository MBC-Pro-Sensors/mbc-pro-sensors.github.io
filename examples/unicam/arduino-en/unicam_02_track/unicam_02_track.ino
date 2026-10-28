/*
 * Sharpshooter Vision x Arduino  Example 2: Where is the blob? (colour tracking)
 * ------------------------------------------------------------
 * Camera menu: camera mode 1 Colour tracking, option 3 Standard (the sketch also switches it with E0)
 *
 * What does this example do?
 *   Every 0.2 s it reads the position and area of colours 1~4 and says left/middle/right, top/middle/bottom.
 *   X 0~159 (left -> right), Y 0~119 (top -> bottom), X = 0 means not found; area = % of the picture.
 *
 * Before you start: teach colours on the camera - stop on "Target 1" in the live view, press OK, aim at the colour;
 *   teach Target 2, 3, 4 for more colours.
 *
 * Wiring (the I2C pins are provisional until bench testing):
 *   camera 5V  -> external 5V (power bank or adapter, 2 A or more; do not draw it from the Arduino 5V pin)
 *   camera GND -> Arduino GND (the external supply GND too)
 *   camera IO3 (SDA) -> A4   camera IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   Note: use the "I2C" firmware. IO1/IO3 are shared with the LEGO serial version; the firmware is one or the other.
 *
 * How to see the result: open the Serial Monitor (115200) and move a taught colour in front of the lens.
 */

#include <Wire.h>

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
// one object (modes 51/52): code = colour x 10 + shape (0 = none), color 1~8, shape 0~9,
// x, y centroid pixels, w, h box pixels, area %, angle 0~179, vertices, trackId 1~255; -1 = not given in mode 51
struct UcObject { int16_t code, color, shape, x, y, w, h, area, angle, vertices, trackId; };
// ==== MBC_UNICAM_TOP_END ====

const uint8_t MODE = 13;                 // mode number 13 = colour tracking 3 standard (X, Y, area, count per colour)
const uint8_t COLORS = 4;                // 👉 You can change this: how many colours to show (1~16)
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
  ucSetMode(1, 3);                       // E0: camera mode 1, option 3
  Serial.println(F("Ready! Show a taught colour to the lens."));
}

void loop() {
  if (millis() - lastPrint < 200) return; // 👉 You can change this: print every 0.2 s
  lastPrint = millis();

  for (uint8_t c = 1; c <= COLORS; c++) {
    UcBlob b;
    if (!ucReadBlob(MODE, c, b)) { Serial.println(F("Cannot read the camera")); return; }
    Serial.print(F("Colour ")); Serial.print(c); Serial.print(F(": "));
    if (b.x == 0) {                      // X = 0: this colour is not found
      Serial.print(F("not seen      "));
      continue;
    }
    // X: 0~159 split into left/middle/right; Y: 0~119 into top/middle/bottom (top = small)
    if (b.x < 53)       Serial.print(F("left"));
    else if (b.x > 106) Serial.print(F("right"));
    else                Serial.print(F("middle"));
    if (b.y < 40)       Serial.print(F("-top"));
    else if (b.y > 80)  Serial.print(F("-bottom"));
    else                Serial.print(F("-middle"));
    Serial.print(F(" X=")); Serial.print(b.x);
    Serial.print(F(" Y=")); Serial.print(b.y);
    Serial.print(F(" area")); Serial.print(b.area); Serial.print(F("%"));
    Serial.print(F(" x")); Serial.print(b.count); Serial.print(F(" blobs  "));
  }
  Serial.println();
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

// ---------- Sharpshooter Vision ----------

// Switch camera mode (E0): camMode 1~5, sub = option (0 for colour order; object detection 1 list, 2 detail). Saved in the camera
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

// Object detection (modes 51/52): values per object: 51 -> 4 (colour shape X Y), 52 -> 10 (+ width height area angle vertices track id)
uint8_t ucObjStride(uint8_t mode) {
  if (mode == 51) return 4;
  if (mode == 52) return 10;
  return 0;
}

// Read object i (1~8, 1 = largest); mode = 51 or 52. true if read
bool ucReadObject(uint8_t mode, uint8_t i, UcObject &o) {
  uint8_t k = ucObjStride(mode);
  if (k == 0 || i < 1 || i > 8) return false;
  int16_t d[10];
  if (!mbcReadMany(ADDR, 0x10 + (i - 1) * k, k, d)) return false;
  o.color = d[0]; o.shape = d[1]; o.x = d[2]; o.y = d[3];
  o.code = (d[0] > 0) ? d[0] * 10 + d[1] : 0;
  o.w = -1; o.h = -1; o.area = -1; o.angle = -1; o.vertices = -1; o.trackId = -1;
  if (mode == 52) { o.w = d[4]; o.h = d[5]; o.area = d[6]; o.angle = d[7]; o.vertices = d[8]; o.trackId = d[9]; }
  return true;
}

// Number of objects in view, 0~8 (modes 51/52)
int16_t ucObjectCount(uint8_t mode) {
  uint8_t k = ucObjStride(mode);
  if (k == 0) return MBC_NONE;
  return mbcRead(ADDR, 0x10 + 8 * k);
}

// Shape number -> name: 0 other 1 circle 2 ellipse 3 triangle 4 square 5 rectangle 6 pentagon 7 hexagon 8 star 9 trapezoid
const char *ucShapeName(int16_t s) {
  switch (s) {
    case 0: return "other";    case 1: return "circle";    case 2: return "ellipse";  case 3: return "triangle";
    case 4: return "square";   case 5: return "rectangle"; case 6: return "pentagon"; case 7: return "hexagon";
    case 8: return "star";     case 9: return "trapezoid";
  }
  return "?";
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
