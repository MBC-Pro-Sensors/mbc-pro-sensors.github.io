<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/imu.json 產生（microbit），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/imu/microbit/makecode.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/imu/microbit/makecode.md">IMU 9-Axis Gyro › micro:bit › MakeCode</a></p>

# IMU Gyro Sensor × micro:bit (MakeCode extension)

[🔙 Back to IMU Gyro Sensor](/en/sensors/imu/index.md)

The universal I2C edition of the IMU Gyro Sensor works with BBC micro:bit V1 / V2 through the MakeCode extension **pxt-mbc-sensors v2** (which speaks [MBC I2C protocol v2](/en/i2c-protocol.md)). Installation and hardware notes (3.3 V, power): [micro:bit overview](/en/microbit.md).

<p class="chips"><span class="chip chip-spike off">SPIKE</span><a class="chip chip-ev3" href="/en/sensors/imu/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/imu/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/imu/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/imu/matrix.md">MATRIX</a><a class="chip chip-microbit current" href="/en/sensors/imu/microbit.md">micro:bit</a></p>

> [!IMPORTANT]
> Extension v2 is not yet verified on hardware; block names follow the published release: <https://github.com/contactmbcrobot/pxt-mbc-sensors>

## Wiring

<figure class="ref-fig"><img src="/images/figures/imu/wiring-microbit.svg" alt="IMU Gyro Sensor × micro:bit" loading="lazy"></figure>

| Sensor | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| 5V | external 5 V (or the V2 3V pin, check current) |
| GND | GND |

## Reporter blocks

| Block | Returns | Id | Meaning |
| :--- | :--- | :--- | :--- |
| MBC gyro 3D heading | -32768~32767 ° | [`01`](/en/sensors/imu/reference.md#v01) | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |
| MBC fused heading | -32768~32767 ° | [`02`](/en/sensors/imu/reference.md#v02) | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |
| MBC gyro 1D heading | -32768~32767 ° | [`03`](/en/sensors/imu/reference.md#v03) | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |
| MBC magnetic heading | -32768~32767 ° | [`04`](/en/sensors/imu/reference.md#v04) | tilt-compensated magnetometer heading, degrees, relative to the zero point |
| MBC yaw rate | -32768~32767 0.1 dps | [`05`](/en/sensors/imu/reference.md#v05) | angular rate about the yaw axis (getYawAxis()), 0.1 deg/s, same sign as the heading |
| MBC pitch angle | -90~+90 ° | [`06`](/en/sensors/imu/reference.md#v06) | elevation of the board's printed X axis above horizontal: +90 pointing up, −90 pointing down, 0 level (from the gravity vector) |
| MBC pitch rate | -32768~32767 0.1 dps | [`07`](/en/sensors/imu/reference.md#v07) | angular rate about the board's printed X axis, 0.1 °/s (right-hand rule, CCW seen from +X is positive); this is gyro X, not the derivative of 06 |
| MBC magnetometer trust | 0~255 | [`08`](/en/sensors/imu/reference.md#v08) | how much the fusion trusts the magnetometer, 0~255 (= 0~1 × 255) |
| MBC magnetic disturbance | 0~255 0.01° | [`09`](/en/sensors/imu/reference.md#v09) | magnetic disturbance level 0~255 (degrees × 100, capped at 255) |
| MBC face up | 0~5 | [`0A`](/en/sensors/imu/reference.md#v0a) | which axis points up: 0 +X, 1 +Y, 2 +Z, 3 −X, 4 −Y, 5 −Z (physical axes) |
| MBC gyro [axis] | -32768~32767 0.1 dps | [`10~12`](/en/sensors/imu/reference.md#v10) | 10 X, 11 Y, 12 Z angular rate, 0.1 deg/s (physical axes, sign-corrected) |
| MBC accel [axis] | -32768~32767 mg | [`13~15`](/en/sensors/imu/reference.md#v13) | 13 X, 14 Y, 15 Z acceleration, mg (1000 = 1 g) |
| MBC mag [axis] | -32768~32767 mG | [`16~18`](/en/sensors/imu/reference.md#v16) | 16 X, 17 Y, 18 Z magnetic field, mG (milligauss) |

## Command blocks

| Block | Id | Meaning |
| :--- | :--- | :--- |
| MBC zero heading | [`E0`](/en/sensors/imu/reference.md#ce0) | zero all four headings (zeroHeadings()); EV3 also accepts the stock gyro-reset WRITE 0x11 |
| MBC start accel calibration | [`E1`](/en/sensors/imu/reference.md#ce1) | start accelerometer calibration (= v1 0x20=0x01); F3 b4 = 1 meanwhile |
| MBC end accel calibration | [`E2`](/en/sensors/imu/reference.md#ce2) | finish accelerometer calibration and save to EEPROM (= v1 0x20=0x02); headings are zeroed once on success |
| MBC start mag calibration | [`E3`](/en/sensors/imu/reference.md#ce3) | start magnetometer calibration (= v1 0x20=0x03); F3 b5 = 1 meanwhile |
| MBC end mag calibration | [`E4`](/en/sensors/imu/reference.md#ce4) | finish magnetometer calibration and save to EEPROM (= v1 0x20=0x04) |

## Status

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

## JavaScript

```typescript
basic.forever(function () {
    let headingGyro3D = MBC.readValue(0x6A, 0x01)   // gyro 3D heading
    basic.showNumber(headingGyro3D)
    basic.pause(100)
})
```

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/imu.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- hw0 = 1（共識 #6）；EV3 模式 4 = [0x6A | 1<<8, 韌體主×100+次, 0, 0]（格式 B 再補 2 個 0），模式 5 改送俯仰角速度（取代 #65 放的 ID）。
- EV3 鎖住模式 0／1／3 照盤點程式碼：0 = 陀螺 3D、1 = 融合航向、3 = [陀螺 1D, 磁力航向]（INO:213 註解寫反，以程式為準）。
- 0x22 照 R4 版（出貨版）鎖住：命令 4~9 = 三個航向 +32768 的「低 byte 在前、高 byte 在後」；盤點檔分析的是主韌體（高在前），主韌體 #67 改成跟 R4 版逐 byte 相同。命令 10~12 = 0；0x01、0x0A、0x0F、F1~F4 都觸發航向歸零（刻意，不動）。
- SPIKE 不做（hosts.spike = false，使用者先前決定），所有值 spike = []。
- B-13：E1~E4 加速度計／磁力計校準開始／結束補回（v1 的 0x20 = 01/02/03/04），I2C 定案 §3 只列 E0。busyFlag：E1 → b4、E3 → b5。
- 俯仰角範圍定案沒寫（韌體尚無現成值，#67 用 atan2 新算），range 先填「°」不給 min/max，韌體做好後再補。
- 磁力信任度 08 = round(trust×255)，磁場干擾度 09 = round(disturbance×100) 上限 255，朝上的面 0A = 軸號 + 3×負向：公式照盤點 I2C v1 0x34~0x36。
- 角速度 10~12 = round(dps×SIGN×10)、加速度 13~15 = mm/s²×1000/9806.65、磁力 16~18 = raw×SIGN×4/15：公式照盤點 I2C v1 0x22~0x24；mBlock 一併有（I2C 定案 §4 共用值表）。
- 主對話 2026-10-22：06 俯仰角範圍 −90~+90、07 = gyro X（照字面），取自 F5 韌體實作。
- The extension reads `Int16LE` and pauses 2 ms after writing the id (D5).
