<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- Code blocks come from examples/unicam/arduino-en/*/*.ino and are filled in by node _build/sync-arduino-examples.mjs; edit the .ino and re-run -->

# 🧪 Sharpshooter Vision × Arduino examples

[🔙 Back to Sharpshooter Vision](/en/sensors/unicam/index.md)　｜　[📡 MBC common I2C protocol v2 (advanced)](/en/i2c-protocol.md)

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passed computer tests; real-hardware numbers (fps, line thresholds, I2C pins) may change after bench testing.

This page has **5 examples**: check the connection, colour tracking, spot colour, colour order (WRO colour sequence) and a line-following car. Every example can be copied and uploaded as is, and every important line has a comment. **No coding experience needed** — just go through them in order.

> [!TIP]
> New to Arduino? Start with [🔰 Arduino from zero](/en/sensors/arduino-start.md): install the software, upload a program and see the result in 15 minutes.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/unicam/arduino-en/unicam_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
</div>

---

## 🔌 Wiring (same for every example)

| Camera (product side) | Arduino UNO / Nano | Arduino UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: |
| 5V | **external 5V** (power bank / adapter, 2 A or more) | external 5V | external 5V |
| GND | GND (external supply GND too) | GND | GND |
| IO3 (= SDA, provisional) | A4 | SDA | GPIO 21 |
| IO1 (= SCL, provisional) | A5 | SCL | GPIO 22 |

> [!IMPORTANT]
> - The camera needs the **I2C firmware**; I2C address `0x4C`. IO1/IO3 are the same pins as the LEGO (EV3/SPIKE) serial version, so **the firmware is one or the other**.
> - **SDA = IO3, SCL = IO1 is provisional** until bench testing; if nothing is found, try swapping the two wires.
> - The camera (ESP32 + screen) draws a lot of peak current: **use an external 5V** and always join its GND to the Arduino GND.
> - The camera is an ESP32 slave, so the `delay(2)` in the helpers **must stay**. It takes a few seconds to boot; the examples wait up to 8 s.

---

## 🎛️ Two things to do on the camera first

1. **Teach colours**: in the live view use left/right to stop on "Target 1" → press **OK** → move the cross onto the colour → **OK** to take it (hold OK 0.8 s to finish at once). Teach Target 2, 3… for more colours (up to 16). Examples 2, 4 and 5 (colour line) use them.
2. **Choose the camera mode** (optional): stop on **MODE** in the live view, press OK → choose the camera mode → choose the option. Every Arduino example **switches it automatically with the E0 command**, so you can skip this; the top right of the camera screen shows "mode-option" (e.g. `1-3`).

| Mode number | Camera mode / option | Per colour / spot (I2C always gives 16) | Example |
| :---: | :--- | :--- | :---: |
| 11 | 1 Colour tracking ① Max | X, Y | — |
| 12 | 1 Colour tracking ② Multi | X, Y, count | — |
| 13 | 1 Colour tracking ③ Standard | X, Y, area, count | 1, 2 |
| 14 | 1 Colour tracking ④ Fine | X, Y, width, height, count, area | — |
| 21 / 22 / 23 | 2 Spot colour ① / ② / ③ | hue / + saturation / + value | 3 |
| 30 | 3 Colour order | hue code of each block left to right + total | 4 |
| 41 / 42 / 43 | 4 Line following black / white / colour | near/mid/far offset, angle, junction… 14 values | 5 |

---

<h2 id="cmds">📖 Value ids (read and write)</h2>

The camera is like a row of numbered "boxes", each holding one number (int16). Write `[id]`, wait 2 ms, then read to get consecutive values starting at that box (up to 16 at a time).

| Id | Name | Range | Notes |
| :---: | :--- | :--- | :--- |
| `01` | Mode number | 11–43 | camera mode × 10 + option (colour order = 30) |
| `02` | Option | 0–4 | |
| `03` | Colours taught | 0–16 | |
| `04` | Values in this layer | | e.g. mode 13 gives 65 (16 colours × 4 + colours found) |
| `10` up | Values of this mode | | layout per mode (table below); undefined ids read `-32768` |
| `F0` | Device ID | | low byte = `0x4C`, high byte **hw0 = 3 (current Sharpshooter Vision)**; School version 1, WRO version 2 |
| `F1` | Protocol version | 2 | |
| `F2` | Firmware version | 100 | major × 100 + minor (100 = 1.0) |
| `F3` | Status flags | bit | bit0 = ready (used by `mbcWaitReady`) |

**Layout from `10`** (colour c and spot p start at 1):

| Mode | Layout | Last box |
| :---: | :--- | :--- |
| 11 | 2 boxes per colour: `10+2(c−1)` = X, +1 = Y | `30` = colours found |
| 12 | 3 per colour: X, Y, count | `40` = colours found |
| 13 | 4 per colour: X, Y, area %, count | `50` = colours found |
| 14 | 6 per colour: X, Y, width, height, count, area % | `70` = colours found |
| 21 / 22 / 23 | 1 / 2 / 3 per spot: hue code (, saturation, value) | — |
| 30 | `10`–`1F` = hue code of blocks 1–16 from the left | `20` = total blocks |
| 41–43 | `10` near offset, `11` mid, `12` far, `13` angle, `14` near width %, `15` line seen flags (near 1 + mid 2 + far 4), `16` junction (0 none 1 cross 2 T 3 left 4 right 5 dead end), `17` junction distance %, `18` left edge, `19` right edge, `1A` far width %, `1B` marker colour, `1C` marker count, `1D` last lost side (0 on line 1 left 2 right) | — |

- **Coordinates**: X 0–159 (left → right), Y 0–119 (top → bottom), **0 = not found** (at least 1 when found).
- **Hue code** 0–255: 0–240 colours (red 0/240, yellow 40, green 80, cyan 120, blue 160, purple 200), 241–248 white/grey, 249–255 black.
- **Offsets, angle and edges** are signed: **line on the left = negative** (standing behind the car, looking forward).

**Write commands:**

| Command | Parameters | What it does |
| :---: | :--- | :--- |
| `E0` | [camera mode 1–4, option] | switch mode (saved in the camera) |
| `E1` | [spot 1–20, X 0–159, Y 0–119] | move a spot-colour sample spot (saved in the camera) |

> [!NOTE]
> Target colours can **only be taught on the camera** (v1 has no command to write colours).

---

## 🧰 Helpers (the block at the bottom of every example)

Every example has the same code at the top and at the bottom (between `MBC_UNICAM_…_BEGIN` and `END`) that talks to the camera. You do not need to change it — just use these:

| Helper | What it does | Mode |
| :--- | :--- | :---: |
| `ucSetMode(cameraMode, option)` | E0 switch mode (colour order: `ucSetMode(3, 0)`) | all |
| `ucSetPoint(spot, X, Y)` | E1 move a sample spot | 21–23 |
| `ucModeNumber()` | read the current mode number (e.g. 13) | all |
| `ucReadBlob(mode, colour, b)` | read one colour → `UcBlob b`: `b.x`, `b.y`, `b.w`, `b.h`, `b.count`, `b.area` (-1 = not in this mode) | 11–14 |
| `ucColorsFound(mode)` | how many of the 16 colours are found | 11–14 |
| `ucReadSpot(mode, spot, s)` | read one spot → `UcSpot s`: `s.h`, `s.s`, `s.v` (-1 = not given) | 21–23 |
| `ucReadOrder(array, max, total)` | hue codes from left to right into the array, returns how many; `total` = all blocks | 30 |
| `ucReadLine(l)` | read all 14 → `UcLine l`: `nearOff`, `midOff`, `farOff`, `angle`, `width`, `flags`, `junction`, `junctionDist`, `leftEdge`, `rightEdge`, `farWidth`, `marker`, `markerCount`, `lost` | 41–43 |
| `ucColorCode(hue)` | hue code → colour number: 1 red 2 orange 3 yellow 4 green 5 cyan 6 blue 7 purple 8 white/grey 9 black | any |
| `mbcRead` / `mbcReadMany` / `mbcWaitReady` | read ids directly, wait for the camera to boot | any |

> [!TIP]
> **Writing your own sketch?** Download [MBC_UniCam.h](/examples/unicam/arduino-en/MBC_UniCam.h), put it in your sketch folder (next to the .ino) and write `#include "MBC_UniCam.h"` at the top — all the helpers above are ready, no need to paste the block.

---

## 📋 Examples

| # | Example | Camera mode / option | What you learn | Extra parts |
| :---: | :--- | :--- | :--- | :--- |
| 1 | [Is it connected?](#ex1) | 1 Colour tracking ③ (13) | device ID check, firmware version, switch mode with E0 | none |
| 2 | [Where is the blob?](#ex2) | 1 Colour tracking ③ (13) | X / Y / area / count of 4 colours | something in a taught colour |
| 3 | [What colour is this?](#ex3) | 2 Spot colour ③ (23) | move spots with E1, hue code → colour name | a few coloured bricks |
| 4 | [Colour order (WRO)](#ex4) | 3 Colour order (30) | colour of each block from left to right + total | a row of coloured blocks |
| 5 | [Line follower that looks ahead (advanced)](#ex5) | 4 Line following ① black (41) | near/far offset PD line following, stop at junctions | 2-wheel car, L298N |

> [!TIP]
> **Suggested order: 1 → 2 → 3 → 4 → 5.** Each example switches the camera to the mode it needs at the start.

---

<h2 id="ex1">1️⃣ Is it connected?</h2>

**Camera mode needed:** camera mode 1 Colour tracking, option ③ Standard (the sketch switches it with E0)

**What it does:** reads the `F0` device ID and checks that the low byte is `0x4C` and the high byte (hw0) is **3 = current Sharpshooter Vision**; prints the protocol and firmware versions; switches the camera to mode 13 with `ucSetMode(1, 3)` and reads `01` to confirm. Then it prints the current mode number every second — change the mode in the camera menu and it follows. Run this first every time you connect a camera.

**Steps:**
1. Connect the 4 wires as in the table, switch on the external 5V, plug USB into the computer.
2. Copy the program below into the Arduino IDE and press **→ Upload**.
3. Open the Serial Monitor (magnifier at the top right) at **115200**.

<a href="/examples/unicam/arduino-en/unicam_01_hello/unicam_01_hello.ino" download data-ignore="true">📄 Download unicam_01_hello.ino</a>

<!-- ino: examples/unicam/arduino-en/unicam_01_hello/unicam_01_hello.ino -->
```cpp
/*
 * Sharpshooter Vision x Arduino  Example 1: Is it connected?
 * ------------------------------------------------------------
 * Camera menu: camera mode 1 Colour tracking, option 3 Standard (the sketch also switches it with E0)
 *
 * What does this example do?
 *   1. Read F0 device ID: low byte = 0x4C (address), high byte hw0 = 3 means "current Sharpshooter Vision"
 *      (hw0 = 1 is the School version, 2 is the WRO version; they read differently)
 *   2. Read F2 firmware version (100 = 1.0)
 *   3. Switch the camera to "mode 13" with the E0 command, then read 0x01 to confirm
 *   4. Then print the current mode number every second (it follows the camera menu too)
 *
 * Mode number = camera mode x 10 + option:
 *   11~14 colour tracking, 21~23 spot colour, 30 colour order, 41~43 line following
 *
 * You need: Sharpshooter Vision (I2C firmware), Arduino UNO / Nano / UNO R4 or ESP32.
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
  uint8_t hw0 = (id[0] >> 8) & 0xFF;     // high byte: which version (3 = current Sharpshooter Vision)
  Serial.print(F("Address 0x")); Serial.print(addrLow, HEX);
  Serial.print(F("  hw0=")); Serial.print(hw0);
  if (addrLow == 0x4C && hw0 == 3) Serial.println(F("  -> Sharpshooter Vision OK"));
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

// ---------- Sharpshooter Vision ----------

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
```

**You will see:**

```text
Address 0x4C  hw0=3  -> Sharpshooter Vision OK
Protocol version 2  Firmware 1.0
Mode number after switching: 13
Mode number 13  option 3  taught 2 colours  values in this layer 65
Mode number 13  option 3  taught 2 colours  values in this layer 65
```

**Try it:** switch the camera menu to "4 Line following ①" and the monitor should show `Mode number 41`. Change `ucSetMode(1, 3)` to `ucSetMode(2, 1)` and upload again: it should print `21` after start-up.

---

<h2 id="ex2">2️⃣ Where is the blob?</h2>

**Camera mode needed:** camera mode 1 Colour tracking, option ③ Standard (the sketch switches it with E0)

**What it does:** every 0.2 s reads the position, area and count of colours 1–4 and says left/middle/right and top/middle/bottom. `b.x = 0` means the colour is not found.

**Steps:**
1. Teach 1–4 colours on the camera (see "Two things to do on the camera first").
2. Upload, open the Serial Monitor (115200).
3. Move a taught colour left/right, up/down, near/far in front of the lens.

<a href="/examples/unicam/arduino-en/unicam_02_track/unicam_02_track.ino" download data-ignore="true">📄 Download unicam_02_track.ino</a>

<!-- ino: examples/unicam/arduino-en/unicam_02_track/unicam_02_track.ino -->
```cpp
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
```

**You will see:** (2 colours taught, only colour 1 in front of the lens)

```text
Colour 1: left-top X=30 Y=25 area4% x1 blobs  Colour 2: not seen      Colour 3: not seen      Colour 4: not seen      
```

**Try it:** set `COLORS` to `8` to see 8 colours (I2C gives 16 at once). For width and height use mode 14: `ucSetMode(1, 4)`, `MODE = 14`, and print `b.w`, `b.h`.

---

<h2 id="ex3">3️⃣ What colour is this?</h2>

**Camera mode needed:** camera mode 2 Spot colour, option ③ Hue + saturation + value (the sketch switches it with E0)

**What it does:** uses `E1` to put sample spots 1–3 in the middle row (left X=40, middle X=80, right X=120, all Y=60), then every 0.3 s reads hue code, saturation and value of the 3 spots and turns them into colour names with `ucColorCode`. The spot positions are saved in the camera; the camera screen shows a cross and number for each.

**Steps:**
1. Upload, open the Serial Monitor (115200).
2. Put a coloured brick at left, middle and right; check that the crosses on the camera screen land on the bricks.

<a href="/examples/unicam/arduino-en/unicam_03_spot_color/unicam_03_spot_color.ino" download data-ignore="true">📄 Download unicam_03_spot_color.ino</a>

<!-- ino: examples/unicam/arduino-en/unicam_03_spot_color/unicam_03_spot_color.ino -->
```cpp
/*
 * Sharpshooter Vision x Arduino  Example 3: What colour is this? (spot colour)
 * ------------------------------------------------------------
 * Camera menu: camera mode 2 Spot colour, option 3 Hue + saturation + value (the sketch also switches it with E0)
 *
 * What does this example do?
 *   1. Use the E1 command to put sample spots 1~3 in the middle row: left (40, 60), middle (80, 60), right (120, 60)
 *      (the spot positions are saved in the camera; the screen shows a cross and number)
 *   2. Every 0.3 s read hue code, saturation and value of the 3 spots and print the colour name.
 *   Hue code 0~240 = colours (red 0/240, yellow 40, green 80, cyan 120, blue 160, purple 200), 241~248 white/grey, 249~255 black.
 *
 * Wiring (the I2C pins are provisional until bench testing):
 *   camera 5V  -> external 5V (power bank or adapter, 2 A or more; do not draw it from the Arduino 5V pin)
 *   camera GND -> Arduino GND (the external supply GND too)
 *   camera IO3 (SDA) -> A4   camera IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   Note: use the "I2C" firmware. IO1/IO3 are shared with the LEGO serial version; the firmware is one or the other.
 *
 * How to see the result: open the Serial Monitor (115200) and put a coloured brick at left, middle and right.
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
// ==== MBC_UNICAM_TOP_END ====

const uint8_t MODE = 23;                 // mode number 23 = spot colour 3 (hue + saturation + value)
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
  ucSetMode(2, 3);                       // E0: camera mode 2, option 3
  // E1: spot number, X, Y (X 0~159, Y 0~119)
  ucSetPoint(1, 40, 60);                 // 👉 You can change this: spot 1 on the left
  ucSetPoint(2, 80, 60);                 // spot 2 in the middle
  ucSetPoint(3, 120, 60);                // spot 3 on the right
  Serial.println(F("Ready! Put a coloured brick at left, middle and right."));
}

void loop() {
  if (millis() - lastPrint < 300) return;
  lastPrint = millis();

  for (uint8_t p = 1; p <= 3; p++) {
    UcSpot s;
    if (!ucReadSpot(MODE, p, s)) { Serial.println(F("Cannot read the camera")); return; }
    Serial.print(F("Spot ")); Serial.print(p); Serial.print(F(": "));
    printColorName(ucColorCode(s.h));    // hue code -> colour name
    Serial.print(F(" (H")); Serial.print(s.h);
    Serial.print(F(" S")); Serial.print(s.s);
    Serial.print(F(" V")); Serial.print(s.v);
    Serial.print(F(")   "));
  }
  Serial.println();
}

// Print the colour name (code from ucColorCode)
void printColorName(uint8_t code) {
  switch (code) {
    case 1: Serial.print(F("red")); break;
    case 2: Serial.print(F("orange")); break;
    case 3: Serial.print(F("yellow")); break;
    case 4: Serial.print(F("green")); break;
    case 5: Serial.print(F("cyan")); break;
    case 6: Serial.print(F("blue")); break;
    case 7: Serial.print(F("purple")); break;
    case 8: Serial.print(F("white/grey")); break;
    default: Serial.print(F("black")); break;
  }
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
```

**You will see:** (red left, green middle, white right)

```text
Spot 1: red (H3 S78 V65)   Spot 2: green (H84 S60 V52)   Spot 3: white/grey (H246 S5 V90)   
```

**Try it:** change the X, Y in `ucSetPoint(…)` to move the spots (X 0–159, Y 0–119). The colour borders are in the helper `ucColorCode` (e.g. hue code 56–99 = green); if your lights shift the colours, change the limits to the numbers you measure.

---

<h2 id="ex4">4️⃣ Colour order (WRO mission)</h2>

**Camera mode needed:** camera mode 3 Colour order (no option; the sketch switches it with E0)

**What it does:** a common WRO task is "a row of coloured blocks — which colours, from left to right?". The camera sorts the blobs of **taught colours** from left to right and gives each hue code plus the total; this sketch prints them every 0.5 s. The number is the hue code of the taught colour, so the same colour always gives the same number — easy to compare.

**Steps:**
1. Teach every colour that can appear on the field (Target 1, 2, 3…).
2. Upload, open the Serial Monitor (115200).
3. Line up coloured blocks in front of the lens.

<a href="/examples/unicam/arduino-en/unicam_04_color_order/unicam_04_color_order.ino" download data-ignore="true">📄 Download unicam_04_color_order.ino</a>

<!-- ino: examples/unicam/arduino-en/unicam_04_color_order/unicam_04_color_order.ino -->
```cpp
/*
 * Sharpshooter Vision x Arduino  Example 4: Colour order (WRO mission)
 * ------------------------------------------------------------
 * Camera menu: camera mode 3 Colour order (no option; the sketch also switches it with E0)
 *
 * What does this example do?
 *   A common WRO task: a row of coloured blocks; find the colours from left to right.
 *   The camera sorts the taught-colour blobs from left to right and gives each hue code and the total;
 *   this sketch prints "N blocks: 1 red, 2 green..." every 0.5 s.
 *
 * Before you start: teach every colour that can appear on the camera (Target 1, 2, 3...).
 *   The number is the hue code of the taught colour, so the same colour always gives the same number.
 *
 * Wiring (the I2C pins are provisional until bench testing):
 *   camera 5V  -> external 5V (power bank or adapter, 2 A or more; do not draw it from the Arduino 5V pin)
 *   camera GND -> Arduino GND (the external supply GND too)
 *   camera IO3 (SDA) -> A4   camera IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   Note: use the "I2C" firmware. IO1/IO3 are shared with the LEGO serial version; the firmware is one or the other.
 *
 * How to see the result: open the Serial Monitor (115200) and line up coloured blocks in front of the lens.
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
// ==== MBC_UNICAM_TOP_END ====

const uint8_t MAX_BLOCKS = 6;            // 👉 You can change this: how many blocks to print at most (1~16)
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
  ucSetMode(3, 0);                       // E0: camera mode 3 (colour order has no option, use 0)
  Serial.println(F("Ready! Line up coloured blocks in front of the lens."));
}

void loop() {
  if (millis() - lastPrint < 500) return;
  lastPrint = millis();

  int16_t hues[16];                      // hue code of each block from left to right
  int16_t total;                         // total blocks in the picture
  uint8_t n = ucReadOrder(hues, MAX_BLOCKS, total);
  if (total == MBC_NONE) { Serial.println(F("Cannot read the camera")); return; }

  Serial.print(F("")); Serial.print(total); Serial.print(F(" blocks: "));
  if (total == 0) { Serial.println(F("(no taught colour seen)")); return; }
  for (uint8_t i = 0; i < n; i++) {
    Serial.print(i + 1); Serial.print(' ');           // block number (from the left)
    printColorName(ucColorCode(hues[i]));
    Serial.print(F("(")); Serial.print(hues[i]); Serial.print(F(")  "));
  }
  Serial.println();
}

// Print the colour name (code from ucColorCode)
void printColorName(uint8_t code) {
  switch (code) {
    case 1: Serial.print(F("red")); break;
    case 2: Serial.print(F("orange")); break;
    case 3: Serial.print(F("yellow")); break;
    case 4: Serial.print(F("green")); break;
    case 5: Serial.print(F("cyan")); break;
    case 6: Serial.print(F("blue")); break;
    case 7: Serial.print(F("purple")); break;
    case 8: Serial.print(F("white/grey")); break;
    default: Serial.print(F("black")); break;
  }
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
```

**You will see:** (left to right: red, yellow, green, blue)

```text
4 blocks: 1 red(2)  2 yellow(41)  3 green(83)  4 blue(158)  
```

**Try it:** if WRO wants "colour numbers", map the result of `ucColorCode(hues[i])` to the official numbers in the loop. I2C gives up to 16 blocks; raise `MAX_BLOCKS` to print more.

---

<h2 id="ex5">5️⃣ Line follower that looks ahead (advanced)</h2>

**Camera mode needed:** camera mode 4 Line following, option ① Black line (the sketch switches it with E0; white line: `ucSetMode(4, 2)`)

**What it does:** the camera looks down and ahead and gives 14 line values at once. Error = near offset × 0.7 + far offset × 0.3 (seeing the curve ahead turns earlier), corrected with PD control. At a cross (1) or T (2) junction whose distance is ≤ 30 (already close), it stops for 3 s, then goes on. If none of the 3 scan rows sees the line (`flags = 0`), it spins toward the last lost side; if the camera cannot be read, it stops.

**Wiring (in addition to the table above):**

| L298N | UNO / Nano / R4 | ESP32 |
| :---: | :---: | :---: |
| ENA (left wheel) | 5 | 25 |
| IN1 / IN2 | 7 / 8 | 26 / 27 |
| ENB (right wheel) | 6 | 14 |
| IN3 / IN4 | 9 / 10 | 32 / 33 |
| GND | GND | GND |

**Steps:**
1. Mount the camera at the front of the car, looking down and ahead (the bottom of the picture is just in front of the car).
2. Lift the car (wheels in the air), upload, and check both wheels turn forward; if one turns backward, set `LEFT_REVERSE` or `RIGHT_REVERSE` to `true`.
3. Put it on the black line; it starts after 3 s.

<a href="/examples/unicam/arduino-en/unicam_05_line_follow/unicam_05_line_follow.ino" download data-ignore="true">📄 Download unicam_05_line_follow.ino</a>

<!-- ino: examples/unicam/arduino-en/unicam_05_line_follow/unicam_05_line_follow.ino -->
```cpp
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
```

**You will see:** the car follows the black line and corrects early before curves; at a cross or T junction it stops for 3 s, prints `Junction! Stop for 3 s`, then drives straight across.

**Try it:**
- If it cannot get back onto the line, raise `KP`; if the wobble keeps growing, raise `KD` and lower `KP`.
- Stops before the junction → lower `JUNCTION_NEAR`; stops too late → raise it.
- v1 is not bench-tested yet: all these numbers are starting values — tune them on your field.

---

## ❓ FAQ

**Q: It keeps saying "Camera not found"?**
1. SDA/SCL wiring is provisional: try **swapping** the IO3 and IO1 wires.
2. Is the external 5V on? Is the camera screen lit? Is its GND joined to the Arduino GND?
3. Does the camera run the **I2C firmware**? The EV3/SPIKE/MATRIX firmware uses serial and cannot be read by Arduino.
4. The camera takes a few seconds to boot; the examples wait 8 s. If it still fails, press the Arduino RESET button.

**Q: Example 1 says School version or WRO version?**
That is a legacy firmware (the School / WRO editions are no longer supported and read differently); the legacy examples stay online for reference in [Sharpshooter Vision legacy editions (no longer supported)](/en/sensors/esp32cam/index.md). To upgrade to the current Sharpshooter Vision, contact the dealer you bought it from and send it back: **the firmware update is free**, and the buyer pays shipping both ways.

**Q: A colour is always "not seen"?**
The colour has not been taught yet, or the taught colour is too different from what the camera sees now (different light). Stop on that target on the camera and teach it again; a minimum area (Size) that is too big also hides small blobs.

**Q: What does `-32768` mean?**
"No value" (called `MBC_NONE` in the examples): you read an id this mode does not have, or the camera cannot be read (loose wire).

**Q: Can I remove the `delay(2)` in the helpers?**
No. The camera is an ESP32 slave and cannot hold the I2C clock while it prepares data; after the master writes the id it needs 2 ms.

**Q: Why only 16 values at a time?**
The Arduino UNO Wire buffer is 32 bytes (16 int16 values). The helpers already split it for you; when you use `mbcReadMany` yourself, keep k at 16 or less.

**Q: What is in every box?**
See the [value ids](#cmds) above, or the full spec on the [Sharpshooter Vision page](/en/sensors/unicam/index.md).
