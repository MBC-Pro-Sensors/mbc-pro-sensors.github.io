<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/imu.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/imu/arduino/ide.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/imu/arduino/ide.md">IMU 9-Axis Gyro › Arduino › Arduino IDE</a></p>

# IMU Gyro Sensor × Arduino I2C (universal I2C edition)

[🔙 Back to IMU Gyro Sensor](/en/sensors/imu/index.md)

The universal I2C edition of the IMU Gyro Sensor speaks **MBC I2C protocol v2**: write one id, read 2 bytes = one int16 little-endian value; consecutive ids can be read in one go. This page has the value table, commands and wiring for this product; the shared rules (F0~F5 version & status, F8 re-address, EF reboot) are on the [universal I2C protocol](/en/i2c-protocol.md) page.

<p class="chips"><span class="chip chip-spike off">SPIKE</span><a class="chip chip-ev3" href="/en/sensors/imu/index.md">EV3</a><a class="chip chip-arduino current" href="/en/sensors/imu/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/imu/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/imu/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/imu/microbit.md">micro:bit</a></p>

> [!TIP]
> **New to coding?** Start with the [Arduino examples](/en/sensors/imu/arduino-examples.md); every sketch is copy-and-upload. The six-host reading of every value is in the [reference](/en/sensors/imu/reference.md).

## Basics

| Item | Detail |
| :--- | :--- |
| I2C address | `0x6A`（change with F8） |
| F0 device ID | low byte `0x6A`、high byte `1` |
| Model | `IMU` |
| Protocol F1 | `2` |
| Speed | 100 kHz (recommended) / 400 kHz; wait 2 ms after writing the id |
| Read | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| Snapshot | updated every 10 ms; a multi-value read is atomic |

## Wiring

<figure class="ref-fig"><img src="/images/figures/imu/wiring-uno.svg" alt="IMU Gyro Sensor × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/en/sensors/imu/reference.md#v01) | gyro 3D heading `headingGyro3D` | -32768~32767 ° | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |
| [`02`](/en/sensors/imu/reference.md#v02) | fused heading `headingFused` | -32768~32767 ° | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |
| [`03`](/en/sensors/imu/reference.md#v03) | gyro 1D heading `headingGyro1D` | -32768~32767 ° | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |
| [`04`](/en/sensors/imu/reference.md#v04) | magnetic heading `headingMag` | -32768~32767 ° | tilt-compensated magnetometer heading, degrees, relative to the zero point |
| [`05`](/en/sensors/imu/reference.md#v05) | yaw rate `yawRate` | -32768~32767 0.1 dps | angular rate about the yaw axis (getYawAxis()), 0.1 deg/s, same sign as the heading |
| [`06`](/en/sensors/imu/reference.md#v06) | pitch angle `pitch` | -90~+90 ° | elevation of the board's printed X axis above horizontal: +90 pointing up, −90 pointing down, 0 level (from the gravity vector) |
| [`07`](/en/sensors/imu/reference.md#v07) | pitch rate `pitchRate` | -32768~32767 0.1 dps | angular rate about the board's printed X axis, 0.1 °/s (right-hand rule, CCW seen from +X is positive); this is gyro X, not the derivative of 06 |
| [`08`](/en/sensors/imu/reference.md#v08) | magnetometer trust `magTrust` | 0~255 | how much the fusion trusts the magnetometer, 0~255 (= 0~1 × 255) |
| [`09`](/en/sensors/imu/reference.md#v09) | magnetic disturbance `magDisturb` | 0~255 0.01° | magnetic disturbance level 0~255 (degrees × 100, capped at 255) |
| [`0A`](/en/sensors/imu/reference.md#v0a) | face up `upFace` | 0~5 | which axis points up: 0 +X, 1 +Y, 2 +Z, 3 −X, 4 −Y, 5 −Z (physical axes) |
| [`10~12`](/en/sensors/imu/reference.md#v10) | gyro axis 1~3 `gyro` | -32768~32767 0.1 dps | 10 X, 11 Y, 12 Z angular rate, 0.1 deg/s (physical axes, sign-corrected) |
| [`13~15`](/en/sensors/imu/reference.md#v13) | accel axis 1~3 `accel` | -32768~32767 mg | 13 X, 14 Y, 15 Z acceleration, mg (1000 = 1 g) |
| [`16~18`](/en/sensors/imu/reference.md#v16) | mag axis 1~3 `mag` | -32768~32767 mG | 16 X, 17 Y, 18 Z magnetic field, mG (milligauss) |

## Write commands

| Id | Command | Parameters | Meaning |
| :--- | :--- | :--- | :--- |
| [`E0`](/en/sensors/imu/reference.md#ce0) | zero heading `zeroHeading` | — | zero all four headings (zeroHeadings()); EV3 also accepts the stock gyro-reset WRITE 0x11 |
| [`E1`](/en/sensors/imu/reference.md#ce1) | start accelerometer calibration `accelCalStart` | — | start accelerometer calibration (= v1 0x20=0x01); F3 b4 = 1 meanwhile（F3 bit4 = 1 while running） |
| [`E2`](/en/sensors/imu/reference.md#ce2) | end accelerometer calibration `accelCalEnd` | — | finish accelerometer calibration and save to EEPROM (= v1 0x20=0x02); headings are zeroed once on success |
| [`E3`](/en/sensors/imu/reference.md#ce3) | start magnetometer calibration `magCalStart` | — | start magnetometer calibration (= v1 0x20=0x03); F3 b5 = 1 meanwhile（F3 bit5 = 1 while running） |
| [`E4`](/en/sensors/imu/reference.md#ce4) | end magnetometer calibration `magCalEnd` | — | finish magnetometer calibration and save to EEPROM (= v1 0x20=0x04) |

## Status flags F3

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 |
| b1 | stationary (`still`) | 1 = sensor is stationary |
| b2 | mount axis captured (`axisCaptured`) | 1 = mounting orientation determined (headings zeroed once after first capture) |
| b3 | mount too tilted (`mountTilted`) | 1 = mounted too far from level |
| b4 | accel calibrating (`accelCalibrating`) | 1 between E1 and E2 |
| b5 | mag calibrating (`magCalibrating`) | 1 between E3 and E4 |
| b6 | fusion initialised (`fusionInit`) | 1 = fused heading is valid |
| b7 | mag auto-cal converged (`magAutoCalDone`) | 1 = magnetometer auto-calibration offsets have converged |

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
const uint8_t ADDR = 0x6A;

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
  Serial.print("headingGyro3D: "); Serial.println(mbcRead(ADDR, 0x01));   // gyro 3D heading
  Serial.print("headingFused: "); Serial.println(mbcRead(ADDR, 0x02));   // fused heading
  Serial.print("headingGyro1D: "); Serial.println(mbcRead(ADDR, 0x03));   // gyro 1D heading
  Serial.print("headingMag: "); Serial.println(mbcRead(ADDR, 0x04));   // magnetic heading
  Serial.print("yawRate: "); Serial.println(mbcRead(ADDR, 0x05));   // yaw rate
  Serial.print("pitch: "); Serial.println(mbcRead(ADDR, 0x06));   // pitch angle
  Serial.print("pitchRate: "); Serial.println(mbcRead(ADDR, 0x07));   // pitch rate
  Serial.print("magTrust: "); Serial.println(mbcRead(ADDR, 0x08));   // magnetometer trust
  Serial.print("magDisturb: "); Serial.println(mbcRead(ADDR, 0x09));   // magnetic disturbance
  Serial.print("upFace: "); Serial.println(mbcRead(ADDR, 0x0A));   // face up
  { int16_t a[3]; mbcReadMany(ADDR, 0x10, 3, a); Serial.print("gyro: "); for (uint8_t i = 0; i < 3; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  { int16_t a[3]; mbcReadMany(ADDR, 0x13, 3, a); Serial.print("accel: "); for (uint8_t i = 0; i < 3; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  { int16_t a[3]; mbcReadMany(ADDR, 0x16, 3, a); Serial.print("mag: "); for (uint8_t i = 0; i < 3; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// paste the helper functions below
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/imu.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- hw0 = 1（共識 #6）；EV3 模式 4 = [0x6A | 1<<8, 韌體主×100+次, 0, 0]（格式 B 再補 2 個 0），模式 5 改送俯仰角速度（取代 #65 放的 ID）。
- EV3 鎖住模式 0／1／3 照盤點程式碼：0 = 陀螺 3D、1 = 融合航向、3 = [陀螺 1D, 磁力航向]（INO:213 註解寫反，以程式為準）。
- 0x22 照 R4 大通道版（出貨版）鎖住：命令 4~9 = 三個航向 +32768 的「低 byte 在前、高 byte 在後」；盤點檔分析的是主韌體（高在前），主韌體 #67 改成跟 R4 版逐 byte 相同。命令 10~12 = 0；0x01、0x0A、0x0F、F1~F4 都觸發航向歸零（刻意，不動）。
- 大通道 [0..7] 四個航向 int16 鎖住；[8..9] 轉向軸角速度、[10..11] 俯仰角、[12..13] 俯仰角速度為 #67 追加；[14..23] 與尾段 24~28 維持 0（教練程式可能讀 ColorID／Cyan 回傳值，不放 F3／ID）。
- SPIKE 不做（hosts.spike = false，使用者先前決定），所有值 spike = []。
- B-13：E1~E4 加速度計／磁力計校準開始／結束補回（v1 的 0x20 = 01/02/03/04），I2C 定案 §3 只列 E0。busyFlag：E1 → b4、E3 → b5。
- 俯仰角範圍定案沒寫（韌體尚無現成值，#67 用 atan2 新算），range 先填「°」不給 min/max，韌體做好後再補。
- 磁力信任度 08 = round(trust×255)，磁場干擾度 09 = round(disturbance×100) 上限 255，朝上的面 0A = 軸號 + 3×負向：公式照盤點 I2C v1 0x34~0x36。
- 角速度 10~12 = round(dps×SIGN×10)、加速度 13~15 = mm/s²×1000/9806.65、磁力 16~18 = raw×SIGN×4/15：公式照盤點 I2C v1 0x22~0x24；mBlock 一併有（I2C 定案 §4 共用值表）。
- 主對話 2026-10-22：06 俯仰角範圍 −90~+90、07 = gyro X（照字面），取自 F5 韌體實作。
- Pin table uses each board's common defaults (ESP32: change with `Wire.begin(sda, scl)`).
