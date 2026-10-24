<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/arduino/ide -->
# IMU 9-Axis Gyro × Arduino × Arduino IDE

This page covers **Arduino IDE (C++)** only: how to set it up for the IMU 9-Axis Gyro on Arduino / ESP32 / UNO R4 (I2C), how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** (4 pins: 5V, GND, SDA, SCL), factory I2C address `0x6A`. UNO / Nano: A4 (SDA), A5 (SCL); UNO R4 and ESP32: the pins marked SDA / SCL.

<figure class="ref-fig"><img src="/images/figures/imu/wiring-uno.svg" alt="IMU 9-Axis Gyro × Arduino / ESP32 / UNO R4 (I2C)" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/arduino-ide.en.md + _build/guides/product/imu/arduino-ide.en.md {"product":"IMU 9-Axis Gyro","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [IMU 9-Axis Gyro: Arduino IDE (C++)](/en/sensors/imu/arduino-i2c.md).
<!-- /include-fallback -->

## Reading values

Every snippet uses the `mbcRead()` helper (expand "Helper code" and paste it at the bottom of your sketch); address `0x6A`.

<details class="helper">
<summary>Helper code (paste at the bottom of your sketch)</summary>

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

</details>

<section id="v01" class="ref-item">

### gyro 3D heading <small>headingGyro3D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |

```cpp
int16_t headingGyro3D = mbcRead(0x6A, 0x01);   // gyro 3D heading：-32768~32767 °
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v02" class="ref-item">

### fused heading <small>headingFused</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |

```cpp
int16_t headingFused = mbcRead(0x6A, 0x02);   // fused heading：-32768~32767 °
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v03" class="ref-item">

### gyro 1D heading <small>headingGyro1D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |

```cpp
int16_t headingGyro1D = mbcRead(0x6A, 0x03);   // gyro 1D heading：-32768~32767 °
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v04" class="ref-item">

### magnetic heading <small>headingMag</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | tilt-compensated magnetometer heading, degrees, relative to the zero point |

```cpp
int16_t headingMag = mbcRead(0x6A, 0x04);   // magnetic heading：-32768~32767 °
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v05" class="ref-item">

### yaw rate <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="yaw rate" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | angular rate about the yaw axis (getYawAxis()), 0.1 deg/s, same sign as the heading |

```cpp
int16_t yawRate = mbcRead(0x6A, 0x05);   // yaw rate：-32768~32767 0.1 dps
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v06" class="ref-item">

### pitch angle <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="pitch angle" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-90~+90` ° | elevation of the board's printed X axis above horizontal: +90 pointing up, −90 pointing down, 0 level (from the gravity vector) |

```cpp
int16_t pitch = mbcRead(0x6A, 0x06);   // pitch angle：-90~+90 °
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v07" class="ref-item">

### pitch rate <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="pitch rate" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | angular rate about the board's printed X axis, 0.1 °/s (right-hand rule, CCW seen from +X is positive); this is gyro X, not the derivative of 06 |

```cpp
int16_t pitchRate = mbcRead(0x6A, 0x07);   // pitch rate：-32768~32767 0.1 dps
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v08" class="ref-item">

### magnetometer trust <small>magTrust</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magTrust.svg" alt="magnetometer trust" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | how much the fusion trusts the magnetometer, 0~255 (= 0~1 × 255) |

```cpp
int16_t magTrust = mbcRead(0x6A, 0x08);   // magnetometer trust：0~255
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v09" class="ref-item">

### magnetic disturbance <small>magDisturb</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magDisturb.svg" alt="magnetic disturbance" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` 0.01° | magnetic disturbance level 0~255 (degrees × 100, capped at 255) |

```cpp
int16_t magDisturb = mbcRead(0x6A, 0x09);   // magnetic disturbance：0~255 0.01°
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v0a" class="ref-item">

### face up <small>upFace</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-upFace.svg" alt="face up" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~5` | which axis points up: 0 +X, 1 +Y, 2 +Z, 3 −X, 4 −Y, 5 −Z (physical axes) |

```cpp
int16_t upFace = mbcRead(0x6A, 0x0A);   // face up：0~5
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v10" class="ref-item">

### gyro axis 1~3 <small>gyro</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-gyro.svg" alt="gyro axis 1~3" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 10 X, 11 Y, 12 Z angular rate, 0.1 deg/s (physical axes, sign-corrected) |

```cpp
int16_t gyro[3];
mbcReadMany(0x6A, 0x10, 3, gyro);   // gyro axis 1~3：-32768~32767 0.1 dps
for (uint8_t i = 0; i < 3; i++) { Serial.print(gyro[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">Ids 0x10~0x12 are consecutive: one <code>mbcReadMany</code> call returns them from the same 10 ms snapshot.</p>

</section>

<section id="v13" class="ref-item">

### accel axis 1~3 <small>accel</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-accel.svg" alt="accel axis 1~3" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` mg | 13 X, 14 Y, 15 Z acceleration, mg (1000 = 1 g) |

```cpp
int16_t accel[3];
mbcReadMany(0x6A, 0x13, 3, accel);   // accel axis 1~3：-32768~32767 mg
for (uint8_t i = 0; i < 3; i++) { Serial.print(accel[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">Ids 0x13~0x15 are consecutive: one <code>mbcReadMany</code> call returns them from the same 10 ms snapshot.</p>

</section>

<section id="v16" class="ref-item">

### mag axis 1~3 <small>mag</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-mag.svg" alt="mag axis 1~3" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` mG | 16 X, 17 Y, 18 Z magnetic field, mG (milligauss) |

```cpp
int16_t mag[3];
mbcReadMany(0x6A, 0x16, 3, mag);   // mag axis 1~3：-32768~32767 mG
for (uint8_t i = 0; i < 3; i++) { Serial.print(mag[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">Ids 0x16~0x18 are consecutive: one <code>mbcReadMany</code> call returns them from the same 10 ms snapshot.</p>

</section>

## Commands

<section id="ce0" class="ref-item ref-cmd">

### zero heading <small>zeroHeading</small>

zero all four headings (zeroHeadings()); EV3 also accepts the stock gyro-reset WRITE 0x11

```cpp
mbcWrite(0x6A, 0xE0);   // zero heading
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### start accelerometer calibration <small>accelCalStart</small>

start accelerometer calibration (= v1 0x20=0x01); F3 b4 = 1 meanwhile

```cpp
mbcWrite(0x6A, 0xE1);   // start accelerometer calibration
while (mbcRead(0x6A, 0xF3) & (1 << 4)) delay(10);   // wait until F3 bit4 clears = done
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### end accelerometer calibration <small>accelCalEnd</small>

finish accelerometer calibration and save to EEPROM (= v1 0x20=0x02); headings are zeroed once on success

```cpp
mbcWrite(0x6A, 0xE2);   // end accelerometer calibration
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### start magnetometer calibration <small>magCalStart</small>

start magnetometer calibration (= v1 0x20=0x03); F3 b5 = 1 meanwhile

```cpp
mbcWrite(0x6A, 0xE3);   // start magnetometer calibration
while (mbcRead(0x6A, 0xF3) & (1 << 5)) delay(10);   // wait until F3 bit5 clears = done
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### end magnetometer calibration <small>magCalEnd</small>

finish magnetometer calibration and save to EEPROM (= v1 0x20=0x04)

```cpp
mbcWrite(0x6A, 0xE4);   // end magnetometer calibration
```

</section>

## Examples

<!-- examples: en/sensors/imu/arduino-examples.md host=arduino sw=ide whole -->

## More questions

- **Always reading -32768?** The module is not answering: check SDA / SCL are not swapped, the address is right and 5V / GND are connected.
- **Can I connect several sensors?** Yes, each needs its own address; re-address identical sensors with command F8 (see the universal I2C protocol).

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [All examples imu_arduino_examples_en.zip](/examples/imu/arduino-en/imu_arduino_examples_en.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/imu.json` (single source of truth)
- Setup：`_build/guides/software/arduino-ide.en.md`、`_build/guides/product/imu/arduino-ide.en.md`
- Examples：`sensors/imu/arduino-examples.md`
