<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × ev3 × ev3g），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/ev3/ev3g -->
# IMU 9-Axis Gyro × EV3 × EV3-G

This page covers **EV3-G official software (EV3 Lab)** only: how to set it up for the IMU 9-Axis Gyro on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-ev3g.en.md + _build/guides/product/imu/ev3-ev3g.en.md {"product":"IMU 9-Axis Gyro","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [IMU 9-Axis Gyro: EV3-G official software (EV3 Lab)](/en/sensors/imu/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

The EV3 sees a stock LEGO sensor, so read it with the stock sensor block.

<section id="v01" class="ref-item">

### gyro 3D heading <small>headingGyro3D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| gyro 3D heading | Gyro Sensor → Angle | the value | mode 0 |

</section>

<section id="v02" class="ref-item">

### fused heading <small>headingFused</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| fused heading | Gyro Sensor → Rate | the value | mode 1 |

</section>

<section id="v03" class="ref-item">

### gyro 1D heading <small>headingGyro1D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| gyro 1D heading | Gyro Sensor → Angle and Rate (angle) | the value | mode 3 |

</section>

<section id="v04" class="ref-item">

### magnetic heading <small>headingMag</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | tilt-compensated magnetometer heading, degrees, relative to the zero point |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| magnetic heading | Gyro Sensor → Angle and Rate (rate) | the value | mode 3 |

</section>

<p class="ref-missing"><b>Not readable here</b>：yaw rate, pitch angle, pitch rate, magnetometer trust, magnetic disturbance, face up, gyro axis 1~3, accel axis 1~3, mag axis 1~3. Use <a href="/en/sensors/imu/ev3/pybricks.md">Pybricks</a> / <a href="/en/sensors/imu/ev3/clev3r.md">clev3r</a> for these.</p>

## Commands

The EV3 port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/imu/ev3-examples.md host=ev3 sw=ev3g -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/imu.json` (single source of truth)
- Setup：`_build/guides/software/ev3-ev3g.en.md`、`_build/guides/product/imu/ev3-ev3g.en.md`
- Examples：`sensors/imu/ev3-examples.md`
