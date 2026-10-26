<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps4.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/ps4/arduino/ide.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/ps4/arduino/ide.md">Controller PS4/PS5 › Arduino › Arduino IDE</a></p>

# Gamepad Receiver PS4/PS5 × Arduino I2C (universal I2C edition)

[🔙 Back to Gamepad Receiver PS4/PS5](/en/sensors/ps4/index.md)

The universal I2C edition of the Gamepad Receiver PS4/PS5 speaks **MBC I2C protocol v2**: write one id, read 2 bytes = one int16 little-endian value; consecutive ids can be read in one go. This page has the value table, commands and wiring for this product; the shared rules (F0~F5 version & status, F8 re-address, EF reboot) are on the [universal I2C protocol](/en/i2c-protocol.md) page.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/ps4/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/ps4/index.md">EV3</a><a class="chip chip-arduino current" href="/en/sensors/ps4/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/ps4/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/ps4/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/ps4/microbit.md">micro:bit</a></p>

> [!TIP]
> **New to coding?** Start with the [Arduino examples](/en/sensors/ps4/arduino-examples.md); every sketch is copy-and-upload. The six-host reading of every value is in the [reference](/en/sensors/ps4/reference.md).

## Basics

| Item | Detail |
| :--- | :--- |
| I2C address | `0x42`（change with F8） |
| F0 device ID | low byte `0x42`、high byte `2` |
| Model | `PS4` |
| Protocol F1 | `2` |
| Speed | 100 kHz (recommended) / 400 kHz; wait 2 ms after writing the id |
| Read | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| Snapshot | updated every 10 ms; a multi-value read is atomic |

## Wiring

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-uno.svg" alt="Gamepad Receiver PS4/PS5 × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/en/sensors/ps4/reference.md#v01) | 16 buttons `buttons16` | 0~65535 bit | all buttons in one 16-bit word (PSX layout): bit0 SELECT(SHARE), 1 L3, 2 R3, 3 START(OPTIONS), 4 up, 5 right, 6 down, 7 left, 8 L2, 9 R2, 10 L1, 11 R1, 12 triangle, 13 circle, 14 cross, 15 square; 1 = pressed; 0 when not connected |
| [`02`](/en/sensors/ps4/reference.md#v02) | D-pad `dpad` | 0~15 bit | up 1, right 2, down 4, left 8, added when pressed together (0~15) |
| [`03`](/en/sensors/ps4/reference.md#v03) | shape buttons `shapeKeys` | 0~15 bit | triangle 1, circle 2, cross 4, square 8, added when pressed together (0~15) |
| [`04`](/en/sensors/ps4/reference.md#v04) | shoulder buttons `shoulderKeys` | 0~15 bit | L2 1, R2 2, L1 4, R1 8, added when pressed together (0~15) |
| [`05`](/en/sensors/ps4/reference.md#v05) | function buttons `funcKeys` | 0~15 bit | SELECT(SHARE) 1, L3 2, R3 4, START(OPTIONS) 8, added when pressed together (0~15) |
| [`06`](/en/sensors/ps4/reference.md#v06) | connected `connected` | 0~1 | 1 = gamepad connected and data seen within 300 ms, 0 = not connected (buttons 0, sticks centred while disconnected) |
| [`07`](/en/sensors/ps4/reference.md#v07) | battery `battery` | 0~100 % | gamepad battery 0~100 (always 0 on the PS2 receiver) |
| [`08`](/en/sensors/ps4/reference.md#v08) | L2 analog `l2Analog` | 0~255 | L2 trigger depth 0~255 (0 when not connected) |
| [`09`](/en/sensors/ps4/reference.md#v09) | R2 analog `r2Analog` | 0~255 | R2 trigger depth 0~255 (0 when not connected) |
| [`0A`](/en/sensors/ps4/reference.md#v0a) | system buttons `sysKeys` | 0~3 bit | b0 PS button, b1 capture (PS5 mute) button; 0 when not connected (always 0 on PS2) |
| [`0B`](/en/sensors/ps4/reference.md#v0b) | gamepad type `padType` | 0~4 | 0 none, 1 PS2 digital, 2 PS2 analog, 3 PS4, 4 PS5 (or other) |
| [`10~13`](/en/sensors/ps4/reference.md#v10) | stick axis 1~4 `stick` | -100~+100 | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right/up positive; 0 when not connected |

## Write commands

| Id | Command | Parameters | Meaning |
| :--- | :--- | :--- | :--- |
| [`E5`](/en/sensors/ps4/reference.md#ce5) | rumble `rumble` | strength 0~255 | [strength 0~255], 0 = stop; 200 ms per call (ignored by PS2) |
| [`E6`](/en/sensors/ps4/reference.md#ce6) | light bar colour `lightbar` | R 0~255, G 0~255, B 0~255 | [R][G][B] each 0~255, exactly 3 bytes; remembered until connected; default blue (0,0,255) (ignored by PS2) |

## Status flags F3

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 |
| b1 | gamepad connected (`connected`) | same as value 06 (linkActive) |

## Shared registers

| Id | Name | Meaning |
| :--- | :--- | :--- |
| [`F0`](/en/i2c-protocol.md#rf0) | device ID | low byte = factory I2C address (unchanged after re-addressing), high byte = hw[0] (channels / sensors / variant) |
| [`F1`](/en/i2c-protocol.md#rf1) | protocol version | 2 for this release (same number reported in SPIKE mode 8 v[1]) |
| [`F2`](/en/i2c-protocol.md#rf2) | firmware version | major×100 + minor (e.g. 102 = 1.2) |
| [`F3`](/en/i2c-protocol.md#rf3) | status flags | b0 = ready; other bits per product (see flags) |
| [`F4`](/en/i2c-protocol.md#rf4) | update counter | +1 every 10 ms snapshot, wraps 0~255 (2.56 s per cycle, not usable for reboot detection; frozen = module busy) |
| [`F5`](/en/i2c-protocol.md#rf5) | uptime seconds | seconds since boot, saturates at 32767 (~9 h); a drop means the module rebooted |
| [`F8`](/en/i2c-protocol.md#rf8) | change I2C address | [F8][new 0x08~0x77][~new], saved to EEPROM, takes effect after reboot; [F8][0x00][0xFF] = back to factory address |
| [`EF`](/en/i2c-protocol.md#ref) | reboot | [EF][0xA5]; mBlock sends ACK first, then reboots |

## Single-file helper functions

No library to install. Paste it in two parts: part ① (2 lines) at the top of the sketch, part ② at the bottom. `#include <Wire.h>` and `MBC_NONE` must appear before `setup()` uses them; pasting everything at the bottom will not compile.

**① At the top of your sketch** (before `setup()`)

```cpp
#include <Wire.h>

const int16_t MBC_NONE = -32768;   // no answer / undefined id
```

**② At the bottom of your sketch** (after `loop()`)

```cpp
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
// with 2 parameters whose first is 0, write (uint8_t)0, e.g. mbcWrite(ADDR, 0xF8, (uint8_t)0x00, 0xFF); otherwise it clashes with the array form below and will not compile
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
// helper part ①
#include <Wire.h>

const int16_t MBC_NONE = -32768;   // no answer / undefined id

const uint8_t ADDR = 0x42;

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
  Serial.print("buttons16: "); Serial.println(mbcRead(ADDR, 0x01));   // 16 buttons
  Serial.print("dpad: "); Serial.println(mbcRead(ADDR, 0x02));   // D-pad
  Serial.print("shapeKeys: "); Serial.println(mbcRead(ADDR, 0x03));   // shape buttons
  Serial.print("shoulderKeys: "); Serial.println(mbcRead(ADDR, 0x04));   // shoulder buttons
  Serial.print("funcKeys: "); Serial.println(mbcRead(ADDR, 0x05));   // function buttons
  Serial.print("connected: "); Serial.println(mbcRead(ADDR, 0x06));   // connected
  Serial.print("battery: "); Serial.println(mbcRead(ADDR, 0x07));   // battery
  Serial.print("l2Analog: "); Serial.println(mbcRead(ADDR, 0x08));   // L2 analog
  Serial.print("r2Analog: "); Serial.println(mbcRead(ADDR, 0x09));   // R2 analog
  Serial.print("sysKeys: "); Serial.println(mbcRead(ADDR, 0x0A));   // system buttons
  Serial.print("padType: "); Serial.println(mbcRead(ADDR, 0x0B));   // gamepad type
  { int16_t a[4]; mbcReadMany(ADDR, 0x10, 4, a); Serial.print("stick: "); for (uint8_t i = 0; i < 4; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// paste part ② of the helper functions below
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/ps4.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- 值表、命令、旗標與 PS2 完全相同（mBlock 定案「兩款相同」＋I2C 定案 §3／§4）；只有 hw0 = 2、model = PS4、E5／E6 有作用。
- D3：SPIKE 模式 0 教育版 byte 6~7、模式 6 v[2]、模式 9 v[6] 的 button16 都是 PSX 排列（#65 已把 PS4 改成 PSX，盤點檔寫的 Bluepad32 排列已過時）。
- 0x22 命令 4 同時裝方向鍵（低 nibble）與形狀鍵（高 nibble）、命令 5 裝肩鍵（低）與功能鍵（高）；JSON 只能把一個命令掛在一個值上，所以掛在 02／04，03／05 的 ch22 = null（內容寫在 02／04 的 note）。SPIKE 模式 0／1／2／9 的 nibble 也用同樣方式在 note 說明。
- 搖桿：I2C／mBlock 送 ±100；EV3／0x22送 0~99（50 = 中間）、SPIKE 送 1~9（5 = 中間），不是線性 offset，主機端各自換算（網站範例已是這樣）。
- EV3 模式 3 = 方向鍵 | 形狀鍵<<4 | 已連線<<8（#67 新增 bit8）；模式 4 內容不變（[方向鍵, LX, 形狀鍵, LY]）；模式 5 改送裝置 ID 格式 B [0x42 | 2<<8, 韌體版]。
- SPIKE 模式 3 改 [已連線, 電量, 0]（3×int8）、模式 8 改 [0x42 | 2<<8, 協議版 2, 韌體版, 更新序號]；其餘空模式 1、2、4、6 維持現狀。
- ESP32 從機不能時脈延展：I2C v2 在 onReceive 就預載從該編號起 32 B；0x22照 MATRIX 定案 §5 #2 預載最新一包，待上機。
- I2C 0A 系統鍵 b1 = 擷取鍵（v1 0x15 放在 b3，v2 改 b1，照 I2C 定案 §4）。
- Pin table uses each board's common defaults (ESP32: change with `Wire.begin(sda, scl)`).
