<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × matrix × bigchannel），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/matrix/bigchannel -->
# IMU 9-Axis Gyro × MATRIX × Big-channel library

This page covers **LauMo big-channel library (MATRIXblock / Arduino)** only: how to set it up for the IMU 9-Axis Gyro on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-bigchannel.en.md + _build/guides/product/imu/matrix-bigchannel.en.md {"product":"IMU 9-Axis Gyro","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [IMU 9-Axis Gyro: LauMo big-channel library (MATRIXblock / Arduino)](/en/sensors/imu/matrix.md).
<!-- /include-fallback -->

## Reading values

With the big-channel library installed, read byte n with the `bigU8()` / `bigU16()` helpers (expand "Helper code").

<details class="helper">
<summary>Helper code</summary>

```cpp
// big channel: read byte n (uint8) / 2 bytes from n (uint16 little-endian)
uint8_t bigU8(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();                 // pointer back to byte 0
  uint8_t v = MiniR4.I2C1.MXColor.getColor(B);      // read byte 0
  for (uint8_t i = 0; i < n; i++) { MiniR4.I2C1.MXColor.getColor(R); v = MiniR4.I2C1.MXColor.getColor(B); }   // Red = advance 1
  return v;
}
uint16_t bigU16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (uint16_t)MiniR4.I2C1.MXColor.getS();      // Saturation = read 2 bytes
}
int16_t bigI16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (int16_t)MiniR4.I2C1.MXColor.getH();       // Hue = read signed 2 bytes
}
```

</details>

<section id="v01" class="ref-item">

### gyro 3D heading <small>headingGyro3D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |

```cpp
int headingGyro3D = bigI16(0);   // gyro 3D heading
```

<p class="ref-note">Big channel byte 0~1（i16）; 鎖住（R4 大通道版已流出）</p>

</section>

<section id="v02" class="ref-item">

### fused heading <small>headingFused</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |

```cpp
int headingFused = bigI16(2);   // fused heading
```

<p class="ref-note">Big channel byte 2~3（i16）; 鎖住</p>

</section>

<section id="v03" class="ref-item">

### gyro 1D heading <small>headingGyro1D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |

```cpp
int headingGyro1D = bigI16(4);   // gyro 1D heading
```

<p class="ref-note">Big channel byte 4~5（i16）; 鎖住</p>

</section>

<section id="v04" class="ref-item">

### magnetic heading <small>headingMag</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | tilt-compensated magnetometer heading, degrees, relative to the zero point |

```cpp
int headingMag = bigI16(6);   // magnetic heading
```

<p class="ref-note">Big channel byte 6~7（i16）; 鎖住</p>

</section>

<section id="v05" class="ref-item">

### yaw rate <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="yaw rate" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | angular rate about the yaw axis (getYawAxis()), 0.1 deg/s, same sign as the heading |

```cpp
int yawRate = bigI16(8);   // yaw rate
```

<p class="ref-note">Big channel byte 8~9（i16）; #67 追加（Hue 積木）</p>

</section>

<section id="v06" class="ref-item">

### pitch angle <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="pitch angle" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-90~+90` ° | elevation of the board's printed X axis above horizontal: +90 pointing up, −90 pointing down, 0 level (from the gravity vector) |

```cpp
int pitch = bigI16(10);   // pitch angle
```

<p class="ref-note">Big channel byte 10~11（i16）; #67 追加</p>

</section>

<section id="v07" class="ref-item">

### pitch rate <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="pitch rate" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | angular rate about the board's printed X axis, 0.1 °/s (right-hand rule, CCW seen from +X is positive); this is gyro X, not the derivative of 06 |

```cpp
int pitchRate = bigI16(12);   // pitch rate
```

<p class="ref-note">Big channel byte 12~13（i16）; #67 追加</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：magnetometer trust, magnetic disturbance, face up, gyro axis 1~3, accel axis 1~3, mag axis 1~3. Use <a href="/en/sensors/imu/arduino/ide.md">Arduino</a> / <a href="/en/sensors/imu/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/imu/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

<section id="ce0" class="ref-item ref-cmd">

### zero heading <small>zeroHeading</small>

zero all four headings (zeroHeadings()); EV3 also accepts the stock gyro-reset WRITE 0x11

```cpp
MiniR4.I2C1.MXColor.getColor(undefined);   // big channel F1~F4（undefined）= zero heading, returns the completion counter from the previous packet
```

<p class="ref-note">Big-channel library only; the stock 0x22 library has no actions.</p>

</section>

<p class="ref-missing"><b>Not available here</b>：start accelerometer calibration, end accelerometer calibration, start magnetometer calibration, end magnetometer calibration</p>

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/imu.json` (single source of truth)
- Setup：`_build/guides/software/matrix-bigchannel.en.md`、`_build/guides/product/imu/matrix-bigchannel.en.md`
