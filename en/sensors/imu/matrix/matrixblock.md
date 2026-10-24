<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/matrix/matrixblock -->
# IMU 9-Axis Gyro × MATRIX × MATRIXblock stock blocks

This page covers **MATRIXblock stock colour-sensor blocks (0x22)** only: how to set it up for the IMU 9-Axis Gyro on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-matrixblock.en.md + _build/guides/product/imu/matrix-matrixblock.en.md {"product":"IMU 9-Axis Gyro","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [IMU 9-Axis Gyro: MATRIXblock stock colour-sensor blocks (0x22)](/en/sensors/imu/matrix.md).
<!-- /include-fallback -->

## Reading values

Use the stock MATRIXblock colour-sensor R / G / B / C / M / Y / K blocks (not H / S / V or colour ID); the Arduino-mode line is shown too.

<section id="v01" class="ref-item">

### gyro 3D heading <small>headingGyro3D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |

Blocks "colour sensor → R~G": R = (航向+32768) 低 byte、G = 高 byte（R4 大通道版：低在前）；主機 hi×256+lo−32768

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(R);   // group 1
```

</section>

<section id="v02" class="ref-item">

### fused heading <small>headingFused</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |

Blocks "colour sensor → B~C": B = 低 byte、C = 高 byte

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(B);   // group 1
```

</section>

<section id="v03" class="ref-item">

### gyro 1D heading <small>headingGyro1D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |

Blocks "colour sensor → M~Y": M = 低 byte、Y = 高 byte；K（10）= 0

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(M);   // group 1
```

</section>

<p class="ref-missing"><b>Not readable here</b>：magnetic heading, yaw rate, pitch angle, pitch rate, magnetometer trust, magnetic disturbance, face up, gyro axis 1~3, accel axis 1~3, mag axis 1~3. Use <a href="/en/sensors/imu/matrix/bigchannel.md">Big-channel library</a> for these.</p>

## Commands

This software cannot send commands; use Arduino I2C, mBlock or micro:bit if you need them.

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/imu.json` (single source of truth)
- Setup：`_build/guides/software/matrix-matrixblock.en.md`、`_build/guides/product/imu/matrix-matrixblock.en.md`
