<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line8.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# Line Follower 8-ch × Arduino I2C (universal I2C edition)

[🔙 Back to Line Follower 8-ch](/en/sensors/line8/index.md)

The universal I2C edition of the Line Follower 8-ch speaks **MBC I2C protocol v2**: write one id, read 2 bytes = one int16 little-endian value; consecutive ids can be read in one go. This page has the value table, commands and wiring for this product; the shared rules (F0~F5 version & status, F8 re-address, EF reboot) are on the [universal I2C protocol](/en/i2c-protocol.md) page.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/line8/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/line8/ev3-hub.md">EV3</a><a class="chip chip-arduino current" href="/en/sensors/line8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/line8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/line8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/line8/microbit.md">micro:bit</a></p>

> [!TIP]
> **New to coding?** Start with the [Arduino examples](/en/sensors/line8/arduino-examples.md); every sketch is copy-and-upload. The six-host reading of every value is in the [reference](/en/sensors/line8/reference.md).

## Basics

| Item | Detail |
| :--- | :--- |
| I2C address | `0x16`（change with F8） |
| F0 device ID | low byte `0x16`、high byte `8` |
| Model | `LINE8` |
| Protocol F1 | `2` |
| Speed | 100 kHz (recommended) / 400 kHz; wait 2 ms after writing the id |
| Read | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| Snapshot | updated every 10 ms; a multi-value read is atomic |

## Wiring

<figure class="ref-fig"><img src="/images/figures/line8/wiring-uno.svg" alt="Line Follower 8-ch × Arduino UNO" loading="lazy"></figure>

| Sensor | Arduino UNO / Nano | Arduino R4 | ESP32 |
| :--- | :--- | :--- | :--- |
| SDA | A4 | SDA | GPIO 21 |
| SCL | A5 | SCL | GPIO 22 |
| 5V | 5V | 5V | 5V |
| GND | GND | GND | GND |

Works directly with 5 V hosts, no level shifter; two products with the same address cannot share a bus (re-address one with F8 first).

## Value table

Each id returns one int16; click an id for the six-host reading and figure.

| Id | Value | Range | Meaning |
| :--- | :--- | :--- | :--- |
| [`01`](/en/sensors/line8/reference.md#v01) | line position `linePos` | -8~+8 | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |
| [`02`](/en/sensors/line8/reference.md#v02) | fine position `linePosFine` | -100~+100 | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |
| [`03`](/en/sensors/line8/reference.md#v03) | line width `lineWidth` | 0~8 路 | number of channels on the line, 0~8; 0 = no line |
| [`04`](/en/sensors/line8/reference.md#v04) | line segments `groupCount` | 0~4 | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |
| [`05`](/en/sensors/line8/reference.md#v05) | line bitmap `bitmap` | 0~255 bit | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |
| [`06`](/en/sensors/line8/reference.md#v06) | last exit direction `lastDir` | -8~+8 | where the line was before it was lost: −8 to +8 (negative = left); keeps the last value |
| [`07`](/en/sensors/line8/reference.md#v07) | threshold `threshold` | 1~99 | reflect threshold for 'on line', 1~99, default 50; set with E4 |
| [`08`](/en/sensors/line8/reference.md#v08) | calibrations done `calibCount` | 0~255 次 | number of calibrations completed since power-on (button calibrations included) |
| [`10~17`](/en/sensors/line8/reference.md#v10) | channel 1~8 reflect `light` | 0~100 | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |

## Write commands

| Id | Command | Parameters | Meaning |
| :--- | :--- | :--- | :--- |
| [`E0`](/en/sensors/line8/reference.md#ce0) | black line mode `blackLine` | — | follow a black line (default) |
| [`E1`](/en/sensors/line8/reference.md#ce1) | white line mode `whiteLine` | — | follow a white line (F3 b1 = 1) |
| [`E2`](/en/sensors/line8/reference.md#ce2) | calibrate black `calibBlack` | — | store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM（F3 bit2 = 1 while running） |
| [`E3`](/en/sensors/line8/reference.md#ce3) | calibrate white `calibWhite` | — | store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM（F3 bit2 = 1 while running） |
| [`E4`](/en/sensors/line8/reference.md#ce4) | set threshold `setThreshold` | threshold 1~99 | threshold 1~99 (RAM only, back to 50 after reboot) |
| [`E7`](/en/sensors/line8/reference.md#ce7) | 5-second auto calibration `calibAuto` | — | sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile（F3 bit2 = 1 while running） |

## Status flags F3

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 |
| b1 | white line mode (`whiteMode`) | 1 = following a white line |
| b2 | calibrating (`calibrating`) | E2/E3/E7 in progress; new calibration commands are ignored |
| b3 | line lost (`lost`) | segment count = 0 |
| b4 | fork (`fork`) | segment count ≥ 2 |

## Shared registers

| Id | Name | Meaning |
| :--- | :--- | :--- |
| [`F0`](/en/i2c-protocol.md#rf0) | device ID | low byte = factory I2C address (unchanged after re-addressing), high byte = hw[0] (channels / sensors / variant) |
| [`F1`](/en/i2c-protocol.md#rf1) | protocol version | 2 for this release (same number reported in SPIKE mode 8 v[1]) |
| [`F2`](/en/i2c-protocol.md#rf2) | firmware version | major×100 + minor (e.g. 102 = 1.2) |
| [`F3`](/en/i2c-protocol.md#rf3) | status flags | b0 = ready; other bits per product (see flags); same byte as MATRIX big-channel byte 24 |
| [`F4`](/en/i2c-protocol.md#rf4) | update counter | +1 every 10 ms snapshot, wraps 0~255 (2.56 s per cycle, not usable for reboot detection; frozen = module busy) |
| [`F5`](/en/i2c-protocol.md#rf5) | uptime seconds | seconds since boot, saturates at 32767 (~9 h); a drop means the module rebooted |
| [`F8`](/en/i2c-protocol.md#rf8) | change I2C address | [F8][new 0x08~0x77][~new], saved to EEPROM, takes effect after reboot; [F8][0x00][0xFF] = back to factory address |
| [`EF`](/en/i2c-protocol.md#ref) | reboot | [EF][0xA5]; mBlock sends ACK first, then reboots |

## Single-file helper functions

Paste this at the bottom of every sketch; no library to install:

```cpp
#include <Wire.h>

const int16_t MBC_NONE = -32768;   // no answer / undefined id

// read k consecutive values (k ≤ 16) starting at id, int16 little-endian
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                                        // always wait 2 ms (ESP32 products cannot clock-stretch)
  if (Wire.requestFrom(addr, (uint8_t)(k * 2)) != (uint8_t)(k * 2)) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint8_t lo = Wire.read(), hi = Wire.read();
    out[i] = (int16_t)(lo | (hi << 8));
  }
  return true;
}

// read one value; MBC_NONE when nothing answers
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// write a command (0~3 parameters)
void mbcWrite(uint8_t addr, uint8_t id, const uint8_t *args, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(args[i]);
  Wire.endTransmission();
}
void mbcWrite(uint8_t addr, uint8_t id) { mbcWrite(addr, id, nullptr, 0); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a) { mbcWrite(addr, id, &a, 1); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a, uint8_t b) { uint8_t p[2] = {a, b}; mbcWrite(addr, id, p, 2); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a, uint8_t b, uint8_t c) { uint8_t p[3] = {a, b, c}; mbcWrite(addr, id, p, 3); }

// wait until ready (F3 bit0 = 1), at most ms milliseconds
bool mbcWaitReady(uint8_t addr, unsigned long ms) {
  unsigned long t0 = millis();
  while (millis() - t0 < ms) {
    int16_t st = mbcRead(addr, 0xF3);
    if (st != MBC_NONE && (st & 1)) return true;
    delay(20);
  }
  return false;
}
```

## Example: print the whole value table

```cpp
const uint8_t ADDR = 0x16;

void setup() {
  Serial.begin(115200);
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);   // AVR: never hang when the module is silent
#endif
  if (!mbcWaitReady(ADDR, 2000)) Serial.println("module not found: check wiring and address");
  Serial.print("ID 0x"); Serial.println(mbcRead(ADDR, 0xF0), HEX);   // low byte address, high byte hw[0]
  Serial.print("FW ");    Serial.println(mbcRead(ADDR, 0xF2));
}

void loop() {
  Serial.print("linePos: "); Serial.println(mbcRead(ADDR, 0x01));   // line position
  Serial.print("linePosFine: "); Serial.println(mbcRead(ADDR, 0x02));   // fine position
  Serial.print("lineWidth: "); Serial.println(mbcRead(ADDR, 0x03));   // line width
  Serial.print("groupCount: "); Serial.println(mbcRead(ADDR, 0x04));   // line segments
  Serial.print("bitmap: "); Serial.println(mbcRead(ADDR, 0x05));   // line bitmap
  Serial.print("lastDir: "); Serial.println(mbcRead(ADDR, 0x06));   // last exit direction
  Serial.print("threshold: "); Serial.println(mbcRead(ADDR, 0x07));   // threshold
  Serial.print("calibCount: "); Serial.println(mbcRead(ADDR, 0x08));   // calibrations done
  { int16_t a[8]; mbcReadMany(ADDR, 0x10, 8, a); Serial.print("light: "); for (uint8_t i = 0; i < 8; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// paste the helper functions below
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/line8.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- B-11：細位置 02 與線位置 01 同號（負 = 左）；v1 韌體註解寫反，程式碼同號。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- SPIKE 教育版模式 0 的 byte 8~15 補 0（#65 已改，使用者保留）。
- EV3 模式 4 與模式 2 相同內容；模式 3 改壓線位元圖（#67）。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體，0x0B 回 1。
- Pin table uses each board's common defaults (ESP32: change with `Wire.begin(sda, scl)`).
