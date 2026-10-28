/*
 * Sharpshooter Vision x Arduino  Example 5 (advanced): line follower that looks ahead
 * ------------------------------------------------------------
 * Camera menu: camera mode 4 Line following, option 1 Black line (the sketch also switches it with E0)
 *
 * What does this example do?
 *   The camera looks down and ahead and gives a near offset and a far offset (-100 ~ +100, line on the left = negative).
 *   Error = near x 0.7 + far x 0.3 (seeing the curve ahead turns earlier), corrected with PD control.
 *   At a cross or T junction that is already close, the car stops for 3 s, then goes on.
 *   If none of the 3 scan rows sees the line, it turns toward the last lost side; if the camera cannot be read, it stops.
 *
 * You need: a 2-wheel car, an L298N motor driver, batteries. Mount the camera at the front, looking down and ahead.
 *
 * Wiring (the I2C pins are provisional until bench testing):
 *   camera 5V  -> external 5V (power bank or adapter, 2 A or more; do not draw it from the Arduino 5V pin)
 *   camera GND -> Arduino GND (the external supply GND too)
 *   camera IO3 (SDA) -> A4   camera IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   Note: use the "I2C" firmware. IO1/IO3 are shared with the LEGO serial version; the firmware is one or the other.
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   (ESP32: ENA -> 25, IN1 -> 26, IN2 -> 27, ENB -> 14, IN3 -> 32, IN4 -> 33)
 *   Join the L298N GND to the Arduino GND!
 *
 * Before the first run: lift the car (wheels in the air), upload, check both wheels turn forward;
 *   if one turns backward, set LEFT_REVERSE or RIGHT_REVERSE to true.
 *   Note: v1 is not bench-tested yet: KP, KD and the junction threshold are starting values; tune them on your field.
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

// ---- Motor pins ----
#if defined(ESP32)
// ESP32 pins 6 ~ 11 are wired to the flash chip and cannot be used, so use these
const int ENA = 25, IN1 = 26, IN2 = 27;  // left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // right wheel
#endif

// ---- 👉 You can change these ----
const int   BASE_SPEED    = 110;         // straight speed: 0 ~ 255, start slow
const float KP            = 1.2;         // correction strength: bigger if it cannot get back, smaller if it wobbles
const float KD            = 3.0;         // damping: increase if the wobble grows
const int   SEARCH_SPEED  = 100;         // spin speed when searching for the line
const int   JUNCTION_NEAR = 30;          // stop when junction distance <= this (0 = right in front, 100 = far end of the picture)
const bool  LEFT_REVERSE  = false;       // set to true if the left wheel turns backward
const bool  RIGHT_REVERSE = false;       // set to true if the right wheel turns backward

float lastError = 0;                     // previous error (for D control)

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {        // wait up to 8 s for the camera to boot
    Serial.println(F("Camera not found! Please check:"));
    Serial.println(F(" 1. The camera has external 5V and its GND is joined to the Arduino GND"));
    Serial.println(F(" 2. IO3 goes to SDA, IO1 goes to SCL (swap them if reversed)"));
    Serial.println(F(" 3. The camera runs the I2C firmware"));
    while (true) {}                      // stop here
  }
  ucSetMode(4, 1);                       // E0: camera mode 4 line, option 1 black line (white line: ucSetMode(4, 2))
  Serial.println(F("Starting in 3 s!"));
  delay(3000);
}

void loop() {
  UcLine l;
  if (!ucReadLine(l)) {                  // cannot read the camera -> stop for safety
    drive(0, 0);
    delay(50);
    return;
  }

  if (l.flags == 0) {
    // no scan row sees the line: spin toward the side where it was lost
    if (l.lost == 1) drive(-SEARCH_SPEED, SEARCH_SPEED);       // line was last on the left -> spin left
    else             drive(SEARCH_SPEED, -SEARCH_SPEED);       // line was last on the right -> spin right
    delay(20);
    return;
  }

  // cross (1) or T (2) junction that is already close -> stop for 3 s
  if ((l.junction == 1 || l.junction == 2) && l.junctionDist <= JUNCTION_NEAR) {
    drive(0, 0);
    Serial.println(F("Junction! Stop for 3 s"));
    delay(3000);
    drive(BASE_SPEED, BASE_SPEED);       // drive straight across
    delay(400);
    return;
  }

  // PD control: error = 70% near + 30% far
  float error = 0.7 * l.nearOff + 0.3 * l.farOff;
  float turn = KP * error + KD * (error - lastError);
  lastError = error;
  // line on the right (positive) -> turn > 0 -> left faster, right slower -> turn right
  drive(BASE_SPEED + (int)turn, BASE_SPEED - (int)turn);
  delay(20);
}

// Drive both wheels: speed -255 (full reverse) ~ 255 (full forward)
void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);   // keep the speed in range
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
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
