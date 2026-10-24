<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/mblock/mblock5 -->
# IMU 9-Axis Gyro × mBlock × mBlock 5

This page covers **mBlock 5 (MBC Pro Sensors extension)** only: how to set it up for the IMU 9-Axis Gyro on Makeblock (mBot) + mBlock 5, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **MakeBlock edition** (RJ25 plug) and plug it into an RJ25 port of mBot / Ranger / Orion / MegaPi; usable ports per board are listed in [MakeBlock extension](/en/makeblock.md).

<figure class="ref-fig"><img src="/images/figures/imu/wiring-mbot.svg" alt="IMU 9-Axis Gyro × Makeblock (mBot) + mBlock 5" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/mblock-mblock5.en.md + _build/guides/product/imu/mblock-mblock5.en.md {"product":"IMU 9-Axis Gyro","edition":"MakeBlock edition","port":"1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [IMU 9-Axis Gyro: mBlock 5 (MBC Pro Sensors extension)](/en/sensors/imu/mblock.md).
<!-- /include-fallback -->

## Reading values

After adding the MBC Pro Sensors extension, find the blocks in the "IMU Gyro" category and pick the port the sensor is plugged into.

<section id="v01" class="ref-item">

### gyro 3D heading <small>headingGyro3D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |

<pre class="blocks">(IMU Gyro port [1 v] gyro 3D heading :: sensing)</pre>

<p class="ref-note">Block returns gyro 3D heading（-32768~32767 °）。Host sends <code>[01]</code> → reply <code>[lo][hi][01 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v02" class="ref-item">

### fused heading <small>headingFused</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |

<pre class="blocks">(IMU Gyro port [1 v] fused heading :: sensing)</pre>

<p class="ref-note">Block returns fused heading（-32768~32767 °）。Host sends <code>[02]</code> → reply <code>[lo][hi][02 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v03" class="ref-item">

### gyro 1D heading <small>headingGyro1D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |

<pre class="blocks">(IMU Gyro port [1 v] gyro 1D heading :: sensing)</pre>

<p class="ref-note">Block returns gyro 1D heading（-32768~32767 °）。Host sends <code>[03]</code> → reply <code>[lo][hi][03 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v04" class="ref-item">

### magnetic heading <small>headingMag</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | tilt-compensated magnetometer heading, degrees, relative to the zero point |

<pre class="blocks">(IMU Gyro port [1 v] magnetic heading :: sensing)</pre>

<p class="ref-note">Block returns magnetic heading（-32768~32767 °）。Host sends <code>[04]</code> → reply <code>[lo][hi][04 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v05" class="ref-item">

### yaw rate <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="yaw rate" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | angular rate about the yaw axis (getYawAxis()), 0.1 deg/s, same sign as the heading |

<pre class="blocks">(IMU Gyro port [1 v] yaw rate :: sensing)</pre>

<p class="ref-note">Block returns yaw rate（-32768~32767 0.1 dps）。Host sends <code>[05]</code> → reply <code>[lo][hi][05 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v06" class="ref-item">

### pitch angle <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="pitch angle" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-90~+90` ° | elevation of the board's printed X axis above horizontal: +90 pointing up, −90 pointing down, 0 level (from the gravity vector) |

<pre class="blocks">(IMU Gyro port [1 v] pitch angle :: sensing)</pre>

<p class="ref-note">Block returns pitch angle（-90~+90 °）。Host sends <code>[06]</code> → reply <code>[lo][hi][06 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v07" class="ref-item">

### pitch rate <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="pitch rate" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | angular rate about the board's printed X axis, 0.1 °/s (right-hand rule, CCW seen from +X is positive); this is gyro X, not the derivative of 06 |

<pre class="blocks">(IMU Gyro port [1 v] pitch rate :: sensing)</pre>

<p class="ref-note">Block returns pitch rate（-32768~32767 0.1 dps）。Host sends <code>[07]</code> → reply <code>[lo][hi][07 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v08" class="ref-item">

### magnetometer trust <small>magTrust</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magTrust.svg" alt="magnetometer trust" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | how much the fusion trusts the magnetometer, 0~255 (= 0~1 × 255) |

<pre class="blocks">(IMU Gyro port [1 v] magnetometer trust :: sensing)</pre>

<p class="ref-note">Block returns magnetometer trust（0~255）。Host sends <code>[08]</code> → reply <code>[lo][hi][08 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v09" class="ref-item">

### magnetic disturbance <small>magDisturb</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magDisturb.svg" alt="magnetic disturbance" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` 0.01° | magnetic disturbance level 0~255 (degrees × 100, capped at 255) |

<pre class="blocks">(IMU Gyro port [1 v] magnetic disturbance :: sensing)</pre>

<p class="ref-note">Block returns magnetic disturbance（0~255 0.01°）。Host sends <code>[09]</code> → reply <code>[lo][hi][09 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v0a" class="ref-item">

### face up <small>upFace</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-upFace.svg" alt="face up" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~5` | which axis points up: 0 +X, 1 +Y, 2 +Z, 3 −X, 4 −Y, 5 −Z (physical axes) |

<pre class="blocks">(IMU Gyro port [1 v] face up :: sensing)</pre>

<p class="ref-note">Block returns face up（0~5）。Host sends <code>[0A]</code> → reply <code>[lo][hi][0A ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v10" class="ref-item">

### gyro axis 1~3 <small>gyro</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-gyro.svg" alt="gyro axis 1~3" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 10 X, 11 Y, 12 Z angular rate, 0.1 deg/s (physical axes, sign-corrected) |

<pre class="blocks">(IMU Gyro port [1 v] gyro [axis] :: sensing)</pre>

<p class="ref-note">Block returns gyro axis 1~3（-32768~32767 0.1 dps）。Host sends <code>[10]</code> → reply <code>[lo][hi][10 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x10 + (N − 1).</p>

</section>

<section id="v13" class="ref-item">

### accel axis 1~3 <small>accel</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-accel.svg" alt="accel axis 1~3" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` mg | 13 X, 14 Y, 15 Z acceleration, mg (1000 = 1 g) |

<pre class="blocks">(IMU Gyro port [1 v] accel [axis] :: sensing)</pre>

<p class="ref-note">Block returns accel axis 1~3（-32768~32767 mg）。Host sends <code>[13]</code> → reply <code>[lo][hi][13 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x13 + (N − 1).</p>

</section>

<section id="v16" class="ref-item">

### mag axis 1~3 <small>mag</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-mag.svg" alt="mag axis 1~3" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` mG | 16 X, 17 Y, 18 Z magnetic field, mG (milligauss) |

<pre class="blocks">(IMU Gyro port [1 v] mag [axis] :: sensing)</pre>

<p class="ref-note">Block returns mag axis 1~3（-32768~32767 mG）。Host sends <code>[16]</code> → reply <code>[lo][hi][16 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x16 + (N − 1).</p>

</section>

## Commands

<section id="ce0" class="ref-item ref-cmd">

### zero heading <small>zeroHeading</small>

zero all four headings (zeroHeadings()); EV3 also accepts the stock gyro-reset WRITE 0x11

<pre class="blocks">IMU Gyro port [1 v] zero heading :: sensing</pre>

<p class="ref-note">Host sends <code>[E0]</code> → reply <code>[00][00][E0]</code> (ack), then it runs.</p>

</section>

<section id="ce1" class="ref-item ref-cmd">

### start accelerometer calibration <small>accelCalStart</small>

start accelerometer calibration (= v1 0x20=0x01); F3 b4 = 1 meanwhile

<pre class="blocks">IMU Gyro port [1 v] start accelerometer calibration :: sensing</pre>

<p class="ref-note">Host sends <code>[E1]</code> → reply <code>[00][00][E1]</code> (ack), then it runs; F3 bit4 = 1 while busy.</p>

</section>

<section id="ce2" class="ref-item ref-cmd">

### end accelerometer calibration <small>accelCalEnd</small>

finish accelerometer calibration and save to EEPROM (= v1 0x20=0x02); headings are zeroed once on success

<pre class="blocks">IMU Gyro port [1 v] end accelerometer calibration :: sensing</pre>

<p class="ref-note">Host sends <code>[E2]</code> → reply <code>[00][00][E2]</code> (ack), then it runs.</p>

</section>

<section id="ce3" class="ref-item ref-cmd">

### start magnetometer calibration <small>magCalStart</small>

start magnetometer calibration (= v1 0x20=0x03); F3 b5 = 1 meanwhile

<pre class="blocks">IMU Gyro port [1 v] start magnetometer calibration :: sensing</pre>

<p class="ref-note">Host sends <code>[E3]</code> → reply <code>[00][00][E3]</code> (ack), then it runs; F3 bit5 = 1 while busy.</p>

</section>

<section id="ce4" class="ref-item ref-cmd">

### end magnetometer calibration <small>magCalEnd</small>

finish magnetometer calibration and save to EEPROM (= v1 0x20=0x04)

<pre class="blocks">IMU Gyro port [1 v] end magnetometer calibration :: sensing</pre>

<p class="ref-note">Host sends <code>[E4]</code> → reply <code>[00][00][E4]</code> (ack), then it runs.</p>

</section>

## Examples

<!-- examples: en/sensors/imu/mblock-examples.md host=mblock sw=mblock5 whole -->

## More questions

- **Blocks keep returning 0?** Check the port in the block matches the real port and that the program was uploaded to the board.
- **Does the old extension still work?** Sensors shipped before 2026-10 use the legacy v1.0.0 extension; old and new blocks are not compatible.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext)
- [MBC_Pro_Sensors_mBlock5_v1.0.0.mext (legacy, units shipped before 2026-10)](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/imu.json` (single source of truth)
- Setup：`_build/guides/software/mblock-mblock5.en.md`、`_build/guides/product/imu/mblock-mblock5.en.md`
- Examples：`sensors/imu/mblock-examples.md`
