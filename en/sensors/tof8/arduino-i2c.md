<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/tof8/arduino/ide.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/tof8/arduino/ide.md">Ranger 8-Way › Arduino › Arduino IDE</a></p>

# Laser Ranger 8-ch × Arduino I2C (universal I2C edition)

[🔙 Back to Laser Ranger 8-ch](/en/sensors/tof8/index.md)

The universal I2C edition of the Laser Ranger 8-ch speaks **MBC I2C protocol v2**: write one id, read 2 bytes = one int16 little-endian value; consecutive ids can be read in one go. This page has the value table, commands and wiring for this product; the shared rules (F0~F5 version & status, F8 re-address, EF reboot) are on the [universal I2C protocol](/en/i2c-protocol.md) page.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino current" href="/en/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/tof8/microbit.md">micro:bit</a></p>

> [!TIP]
> **New to coding?** Start with the [Arduino examples](/en/sensors/tof8/arduino-examples.md); every sketch is copy-and-upload. The six-host reading of every value is in the [reference](/en/sensors/tof8/reference.md).

## Basics

| Item | Detail |
| :--- | :--- |
| I2C address | `0x29`（change with F8） |
| F0 device ID | low byte `0x29`、high byte `8` |
| Model | `LASER8` |
| Protocol F1 | `2` |
| Speed | 100 kHz (recommended) / 400 kHz; wait 2 ms after writing the id |
| Read | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| Snapshot | updated every 10 ms; a multi-value read is atomic |

## Wiring

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-uno.svg" alt="Laser Ranger 8-ch × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/en/sensors/tof8/reference.md#v01) | nearest distance (cm) `nearCm` | 0~120 cm | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |
| [`02`](/en/sensors/tof8/reference.md#v02) | nearest direction `nearDir` | -8~+8 | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |
| [`03`](/en/sensors/tof8/reference.md#v03) | farthest distance (cm) `farCm` | 0~120 cm | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |
| [`04`](/en/sensors/tof8/reference.md#v04) | farthest direction `farDir` | -7~+7 | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |
| [`05`](/en/sensors/tof8/reference.md#v05) | front distance (cm) `frontCm` | 0~120 cm | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3, SPIKE mode 6 and big-channel [16..17] carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |
| [`06`](/en/sensors/tof8/reference.md#v06) | nearest distance (mm) `nearMm` | 0~1200 mm | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |
| [`07`](/en/sensors/tof8/reference.md#v07) | init bitmap `initOk` | 0~255 bit | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |
| [`08`](/en/sensors/tof8/reference.md#v08) | fault bitmap `fault` | 0~255 bit | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |
| [`09`](/en/sensors/tof8/reference.md#v09) | front distance (mm) `frontMm` | 0~1200 mm | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |
| [`10~17`](/en/sensors/tof8/reference.md#v10) | sensor 1~8 distance (cm) `distCm` | 0~120 cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |
| [`18~1F`](/en/sensors/tof8/reference.md#v18) | sensor 1~8 distance (mm) `distMm` | 0~1200 mm | distance of each sensor 1~8, 0~1200 mm; 1200 = nothing that way / sensor failed; ids 18~1F |

## Status flags F3

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | 1 once all 8 sensors are initialised (~2~4 s after power-on) and snapshots are running |
| b1 | any sensor failed (`fault`) | 1 if any sensor failed to initialise, has an I2C error or no update for 200 ms (08 ≠ 0 or 07 ≠ 255) |

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
const uint8_t ADDR = 0x29;

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
  Serial.print("nearCm: "); Serial.println(mbcRead(ADDR, 0x01));   // nearest distance (cm)
  Serial.print("nearDir: "); Serial.println(mbcRead(ADDR, 0x02));   // nearest direction
  Serial.print("farCm: "); Serial.println(mbcRead(ADDR, 0x03));   // farthest distance (cm)
  Serial.print("farDir: "); Serial.println(mbcRead(ADDR, 0x04));   // farthest direction
  Serial.print("frontCm: "); Serial.println(mbcRead(ADDR, 0x05));   // front distance (cm)
  Serial.print("nearMm: "); Serial.println(mbcRead(ADDR, 0x06));   // nearest distance (mm)
  Serial.print("initOk: "); Serial.println(mbcRead(ADDR, 0x07));   // init bitmap
  Serial.print("fault: "); Serial.println(mbcRead(ADDR, 0x08));   // fault bitmap
  Serial.print("frontMm: "); Serial.println(mbcRead(ADDR, 0x09));   // front distance (mm)
  { int16_t a[8]; mbcReadMany(ADDR, 0x10, 8, a); Serial.print("distCm: "); for (uint8_t i = 0; i < 8; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  { int16_t a[8]; mbcReadMany(ADDR, 0x18, 8, a); Serial.print("distMm: "); for (uint8_t i = 0; i < 8; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// paste the helper functions below
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/tof8.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- 正前方 mm（EV3 模式 3、SPIKE 模式 6 v[1]、大通道 [16..17]）在定案 §5 沒有獨立 I2C 編號 → 掛在 05 正前方 cm 底下，用 scale:mm 標示，不自己加編號。
- 0x22（使用者 2026-10-21 最終定案）：只用一個位址，R~K 由左到右 7 個 cm：第 1、2、3 顆、min(第 4, 第 5)、第 6、7、8 顆；11、12 不放資料；最近距離／方向由主機自己比較。此排法只用在 0x22，EV3／SPIKE／大通道不動。
- 大通道 [0..11] 不動（8 顆 cm、最近方向 0~16、最近 cm、最空曠方向、最空曠 cm）；補填 [12] 初始化、[13] 故障（原本註解有寫沒填）；追加 [14..15] 最近 mm、[16..17] 正前方 mm；尾段 [24] = F3、[27] = 0x29、[28] = 8（原本 [24] 沒填）。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改正前方 mm（原本 = 第 1 顆 0~100）、模式 5 改 [最近 mm, 最近方向 −8~+8]（原本 = 前 4 顆兩兩合併）。方向在 EV3／I2C／mBlock 一律 −8~+8 的 int8／int16，SPIKE 模式 1/3/4/9 與大通道維持原始 0~16（offset 8）。
- SPIKE：0/5/7/9 鎖住，1、2、3、4、8 維持不動；模式 6 改 [最近 mm, 正前方 mm, 最近方向 −8~+8]（原本 = 第 2~7 顆兩兩合併）；模式 8 v[0] 改 ID|hw[0]<<8。
- 最空曠方向 04 範圍 −7~+7（maxDirection 永遠不會是 0／16，盤點 INO:1086-1088）；最近方向 02 ±8 = 沒目標、沿用上次偏向側，開機預設 −8。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 主對話 2026-10-21：補 09 正前方 mm（定案 §5 只有 05 正前方 cm，但 EV3 模式 3、SPIKE 模式 6、大通道 [16..17] 都送 mm，I2C／mBlock 應有同一個值）。
- Pin table uses each board's common defaults (ESP32: change with `Wire.begin(sda, scl)`).
