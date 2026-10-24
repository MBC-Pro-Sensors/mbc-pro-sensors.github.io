<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_school.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/esp32cam/arduino/ide.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/esp32cam/arduino/ide.md">Sharpshooter Vision › Arduino › Arduino IDE</a></p>

# Vision Sensor (School Edition) × Arduino I2C (universal I2C edition)

[🔙 Back to Vision Sensor (School Edition)](/en/sensors/esp32cam/index.md)

The universal I2C edition of the Vision Sensor (School Edition) speaks **MBC I2C protocol v2**: write one id, read 2 bytes = one int16 little-endian value; consecutive ids can be read in one go. This page has the value table, commands and wiring for this product; the shared rules (F0~F5 version & status, F8 re-address, EF reboot) are on the [universal I2C protocol](/en/i2c-protocol.md) page.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino current" href="/en/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off">MATRIX</span><a class="chip chip-microbit" href="/en/sensors/esp32cam/microbit.md">micro:bit</a></p>

> [!TIP]
> **New to coding?** Start with the [Arduino examples](/en/sensors/esp32cam/arduino-examples.md); every sketch is copy-and-upload. The six-host reading of every value is in the [reference](/en/sensors/esp32cam/reference.md).

## Basics

| Item | Detail |
| :--- | :--- |
| I2C address | `0x4C`（change with F8） |
| F0 device ID | low byte `0x4C`、high byte `1` |
| Model | `CAMSCH` |
| Protocol F1 | `2` |
| Speed | 100 kHz (recommended) / 400 kHz; wait 2 ms after writing the id |
| Read | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| Snapshot | updated every 10 ms; a multi-value read is atomic |

## Wiring

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-uno.svg" alt="Vision Sensor (School Edition) × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/en/sensors/esp32cam/reference.md#v01) | hue `hue` | -1~100 | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |
| [`02`](/en/sensors/esp32cam/reference.md#v02) | saturation `saturation` | 0~100 | saturation of sample point S1, 0~100 |
| [`03`](/en/sensors/esp32cam/reference.md#v03) | brightness `value` | 0~100 | brightness (HSV value) of sample point S1, 0~100 |
| [`04`](/en/sensors/esp32cam/reference.md#v04) | motion `motion` | 0~100 | whole-frame motion 0~100 (more change = larger) |
| [`05`](/en/sensors/esp32cam/reference.md#v05) | blob X `blobX` | 0~100 | blob centre X 0~100, left = 0; 0 when not found |
| [`06`](/en/sensors/esp32cam/reference.md#v06) | blob Y `blobY` | 0~75 | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |
| [`07`](/en/sensors/esp32cam/reference.md#v07) | blob width `blobW` | 0~100 | blob width W 0~100; 0 = not found |
| [`08`](/en/sensors/esp32cam/reference.md#v08) | blob height `blobH` | 0~75 | blob height H 0~75; 0 when not found |
| [`09`](/en/sensors/esp32cam/reference.md#v09) | blob area `blobArea` | 0~7500 | W×H (0~7500); 0 when not found |
| [`0A`](/en/sensors/esp32cam/reference.md#v0a) | blob found `found` | 0~1 | 1 = a blob of the target colour is in view (W > 0), 0 = none |

## Status flags F3

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 (values freeze while the camera menu is open) |
| b1 | blob found (`found`) | same as value 0A |

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
const uint8_t ADDR = 0x4C;

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
  Serial.print("hue: "); Serial.println(mbcRead(ADDR, 0x01));   // hue
  Serial.print("saturation: "); Serial.println(mbcRead(ADDR, 0x02));   // saturation
  Serial.print("value: "); Serial.println(mbcRead(ADDR, 0x03));   // brightness
  Serial.print("motion: "); Serial.println(mbcRead(ADDR, 0x04));   // motion
  Serial.print("blobX: "); Serial.println(mbcRead(ADDR, 0x05));   // blob X
  Serial.print("blobY: "); Serial.println(mbcRead(ADDR, 0x06));   // blob Y
  Serial.print("blobW: "); Serial.println(mbcRead(ADDR, 0x07));   // blob width
  Serial.print("blobH: "); Serial.println(mbcRead(ADDR, 0x08));   // blob height
  Serial.print("blobArea: "); Serial.println(mbcRead(ADDR, 0x09));   // blob area
  Serial.print("found: "); Serial.println(mbcRead(ADDR, 0x0A));   // blob found
  Serial.println();
  delay(200);
}

// paste the helper functions below
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/cam_school.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- hw0 = 1（共識 #6：神攝手 1 = 學校版）；model 字串 CAMSCH 照任務指定（韌體原本沒有型號欄位）。
- D4：I2C 0x4C 用 SDA = IO3、SCL = IO1（沿 PS4 Wire.setPins 慣例），與 EV3／SPIKE 串口模式二選一編譯；ESP32 從機不能時脈延展，onReceive 預載 32 B；頁面標「硬體已確認，上機待測」。mBlock 用同一組腳 Serial.begin(38400, SERIAL_8N1, 3, 1)。
- EV3 六個模式全部鎖住照盤點（模式 3／5 的 −1 已在 #65 修成符號延伸）；EV3 定案最後的 golden 備註把 cam_school 列進「模式 5 改 ID」是筆誤（定案表本身寫「—」；模式 5 仍是 [色相, 飽和]），所以 ev3IdMode = null。
- SPIKE 模式 9 鎖住，只有 byte 8~9（v[4]）從 0 改成面積 W×H；模式 6 改 [X, Y, 面積]；模式 8 = [0x4C | 1<<8, 2, 韌體版, 更新序號]；模式 1~4 維持 0。
- SPIKE 模式 0 的「面積」是 map(像素面積 0~5000 → 0~100)，跟值表 09 的 W×H 不同，鎖住不改（照盤點）。
- mVision：N = 10（idx 0~9），色相 −1 送 255；每 20 ms 一包（B-6）；不做 0x22／大通道（hosts.matrix = false）。
- 0A 有找到色塊 = W > 0（盤點：W 0 = 沒找到）；F3 b1 同這個值。
- 值 05~08 的「沒找到 = 0」照盤點 §0；色塊 Y 下 = 0、上 = 75 照盤點（map(bestY,120,0,0,75)）。
- 沒有寫入命令（韌體無法從程式改目標顏色／補光燈）。
- Pin table uses each board's common defaults (ESP32: change with `Wire.begin(sda, scl)`).
