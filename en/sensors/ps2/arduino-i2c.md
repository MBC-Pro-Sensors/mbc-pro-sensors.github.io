<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps2.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/ps2/arduino/ide.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/ps2/arduino/ide.md">Controller 2.4G › Arduino › Arduino IDE</a></p>

# PS2 Gamepad Receiver × Arduino I2C (universal I2C edition)

[🔙 Back to PS2 Gamepad Receiver](/en/sensors/ps2/index.md)

The universal I2C edition of the PS2 Gamepad Receiver speaks **MBC I2C protocol v2**: write one id, read 2 bytes = one int16 little-endian value; consecutive ids can be read in one go. This page has the value table, commands and wiring for this product; the shared rules (F0~F5 version & status, F8 re-address, EF reboot) are on the [universal I2C protocol](/en/i2c-protocol.md) page.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/ps2/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/ps2/index.md">EV3</a><a class="chip chip-arduino current" href="/en/sensors/ps2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/ps2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/ps2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/ps2/microbit.md">micro:bit</a></p>

> [!TIP]
> **New to coding?** Start with the [Arduino examples](/en/sensors/ps2/arduino-examples.md); every sketch is copy-and-upload. The six-host reading of every value is in the [reference](/en/sensors/ps2/reference.md).

## Basics

| Item | Detail |
| :--- | :--- |
| I2C address | `0x42`（change with F8） |
| F0 device ID | low byte `0x42`、high byte `1` |
| Model | `PS2` |
| Protocol F1 | `2` |
| Speed | 100 kHz (recommended) / 400 kHz; wait 2 ms after writing the id |
| Read | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| Snapshot | updated every 10 ms; a multi-value read is atomic |

## Wiring

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-uno.svg" alt="PS2 Gamepad Receiver × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/en/sensors/ps2/reference.md#v01) | 16 buttons `buttons16` | 0~65535 bit | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |
| [`02`](/en/sensors/ps2/reference.md#v02) | d-pad `dpad` | 0~15 bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |
| [`03`](/en/sensors/ps2/reference.md#v03) | shape buttons `shape` | 0~15 bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |
| [`04`](/en/sensors/ps2/reference.md#v04) | shoulder buttons `shoulder` | 0~15 bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |
| [`05`](/en/sensors/ps2/reference.md#v05) | function buttons `func` | 0~15 bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |
| [`06`](/en/sensors/ps2/reference.md#v06) | connected `connected` | 0~1 | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |
| [`07`](/en/sensors/ps2/reference.md#v07) | battery `battery` | 0 % | 0~100; PS2 pads report no battery → always 0 (PS4 only) |
| [`08`](/en/sensors/ps2/reference.md#v08) | L2 analog `l2Analog` | 0~255 | L2 pressure 0~255 (only DualShock 2 in analog mode reports pressure; otherwise pressed = 255, released = 0); 0 when not connected |
| [`09`](/en/sensors/ps2/reference.md#v09) | R2 analog `r2Analog` | 0~255 | R2 pressure 0~255 (same rule as 08); 0 when not connected |
| [`0A`](/en/sensors/ps2/reference.md#v0a) | system keys `sysKeys` | 0 bit | b0 PS button, b1 capture button; PS2 pads have none → always 0 (PS4 only) |
| [`0B`](/en/sensors/ps2/reference.md#v0b) | gamepad type `padType` | 0~2 | 0 none, 1 PS2 digital mode, 2 PS2 analog mode (3 PS4 / 4 PS5 only appear on the PS4 receiver) |
| [`10~13`](/en/sensors/ps2/reference.md#v10) | stick LX / LY / RX / RY `stick` | -100~+100 | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

## Write commands

| Id | Command | Parameters | Meaning |
| :--- | :--- | :--- | :--- |
| [`E5`](/en/sensors/ps2/reference.md#ce5) | rumble `vibrate` | strength 0~255 | [strength 0~255], 0 = stop; the PS2 receiver ACKs and ignores it (pad rumble disabled since 250614), effective on PS4 only |
| [`E6`](/en/sensors/ps2/reference.md#ce6) | light bar colour `setLed` | red 0~255, green 0~255, blue 0~255 | [R][G][B] 0~255 each; PS2 pads have no light bar, the receiver ACKs and ignores it, effective on PS4 only |

## Status flags F3

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 |
| b1 | gamepad connected (`connected`) | same as value 06 |

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
  Serial.print("dpad: "); Serial.println(mbcRead(ADDR, 0x02));   // d-pad
  Serial.print("shape: "); Serial.println(mbcRead(ADDR, 0x03));   // shape buttons
  Serial.print("shoulder: "); Serial.println(mbcRead(ADDR, 0x04));   // shoulder buttons
  Serial.print("func: "); Serial.println(mbcRead(ADDR, 0x05));   // function buttons
  Serial.print("connected: "); Serial.println(mbcRead(ADDR, 0x06));   // connected
  Serial.print("battery: "); Serial.println(mbcRead(ADDR, 0x07));   // battery
  Serial.print("l2Analog: "); Serial.println(mbcRead(ADDR, 0x08));   // L2 analog
  Serial.print("r2Analog: "); Serial.println(mbcRead(ADDR, 0x09));   // R2 analog
  Serial.print("sysKeys: "); Serial.println(mbcRead(ADDR, 0x0A));   // system keys
  Serial.print("padType: "); Serial.println(mbcRead(ADDR, 0x0B));   // gamepad type
  { int16_t a[4]; mbcReadMany(ADDR, 0x10, 4, a); Serial.print("stick: "); for (uint8_t i = 0; i < 4; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// paste the helper functions below
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/ps2.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- hw[0] = 1（PS2）、電量 07 恆 0、系統鍵 0A 恆 0、手把類型 0B 只會出現 0／1／2（I2C 定案 §2、§4）。
- 08／09 L2／R2 類比照定案寫 0~255：DS2 類比模式有壓力值；沒有壓力值時按下 = 255、放開 = 0（沿用 v1 0x14 的做法，盤點 .ino:598-603）。
- E5 震動、E6 燈條照 I2C 定案 §3 列在 PS2／PS4 共用命令表；PS2 收到回 ACK 後忽略（跟 v1 0x10／0x23 一樣）。
- 0x22 新增（MATRIX 定案 §6）：1 = 0x43、4 = 方向|形狀<<4、5 = 肩|功能<<4、6~9 = LX LY RX RY 0~99、10 = 已連線、11~12 = 0；大通道新增 [0..3] 四組鍵、[4..7] 搖桿 0~99、[8..9] button16、[10] 已連線、[11] 電量，尾段 [24] F3、[27] 0x42、[28] 1；F1~F4 無動作。ch22 命令 4／5 各掛在 02／04 底下（03／05 在 note 說明高 4 bit），避免同一命令重複登記。
- 搖桿 10~13 在 I2C／mBlock 是 ±100（由 raw (v−128)×100/128、Y 上為正）；EV3／大通道／0x22 維持 0~99（50 中間）、SPIKE 維持 1~9（5 中間）照盤點，不改鎖住模式。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改 方向|形狀<<4|已連線<<8（原本沒有 bit8）；模式 5 改成 ID 格式 B（原本 = 按鍵／搖桿合併版）。
- SPIKE：0/5/7/9 鎖住，1、2、4、6 維持不動；模式 3 改 [已連線, 電量(0), 0]（原本 = 4 組按鍵）；模式 8 改 v[0] = ID|hw[0]<<8（原本 = 搖桿）。模式 9 v[6] button16 = PSX 排列（D3）。
- SPIKE 模式 5 只容得下 11 鍵（L2、R2、L3、R3、START 讀不到），鎖住不改。
- Pin table uses each board's common defaults (ESP32: change with `Wire.begin(sda, scl)`).
