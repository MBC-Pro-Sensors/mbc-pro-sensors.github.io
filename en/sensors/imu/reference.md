<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/imu.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/imu/index.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/imu/index.md">IMU 9-Axis Gyro</a></p>

# IMU Gyro Sensor: value & command reference

[🔙 Back to IMU Gyro Sensor](/en/sensors/imu/index.md)

Every value and command the IMU Gyro Sensor reports to each host: what it means, its range, and how to read it on SPIKE / EV3 / Arduino I2C / mBlock / MATRIX / micro:bit. Append `?host=spike` (or ev3, arduino, mblock, matrix, microbit) to the URL to expand that host by default.

<p class="chips"><span class="chip chip-spike off">SPIKE</span><a class="chip chip-ev3" href="/en/sensors/imu/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/imu/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/imu/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/imu/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/imu/microbit.md">micro:bit</a></p>

| Item | Detail |
| :--- | :--- |
| Factory I2C address | `0x6A` |
| hw[0] | `1`（F0 high byte; ID high byte in SPIKE mode 8 / EV3 mode 5） |
| Model | `IMU` |
| Protocol version | `2` |
| Hosts | EV3, Arduino I2C, mBlock, MATRIX, micro:bit |

## Values at a glance

| Id | Value | Range | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [gyro 3D heading](#v01) | -32768~32767 ° | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [fused heading](#v02) | -32768~32767 ° | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [gyro 1D heading](#v03) | -32768~32767 ° | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [magnetic heading](#v04) | -32768~32767 ° | ✓ | ✓ | ✓ | — | ✓ |
| [`05`](#v05) | [yaw rate](#v05) | -32768~32767 0.1 dps | ✓ | ✓ | ✓ | — | ✓ |
| [`06`](#v06) | [pitch angle](#v06) | -90~+90 ° | ✓ | ✓ | ✓ | — | ✓ |
| [`07`](#v07) | [pitch rate](#v07) | -32768~32767 0.1 dps | ✓ | ✓ | ✓ | — | ✓ |
| [`08`](#v08) | [magnetometer trust](#v08) | 0~255 | — | ✓ | ✓ | — | ✓ |
| [`09`](#v09) | [magnetic disturbance](#v09) | 0~255 0.01° | — | ✓ | ✓ | — | ✓ |
| [`0A`](#v0a) | [face up](#v0a) | 0~5 | — | ✓ | ✓ | — | ✓ |
| [`10~12`](#v10) | [gyro axis 1~3](#v10) | -32768~32767 0.1 dps | — | ✓ | ✓ | — | ✓ |
| [`13~15`](#v13) | [accel axis 1~3](#v13) | -32768~32767 mg | — | ✓ | ✓ | — | ✓ |
| [`16~18`](#v16) | [mag axis 1~3](#v16) | -32768~32767 mG | — | ✓ | ✓ | — | ✓ |

✓ = available on that host; — = not sent.

## Reading each value

<section id="v01" class="ref-item">

### `01` gyro 3D heading <small>headingGyro3D</small>

| Item | Detail |
| :--- | :--- |
| Meaning | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |
| Range | `-32768~32767` ° |
| I2C / mBlock id | `01` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

mode 0 · int16 · GYRO-ANG 1×int16（盤點：mode0 = 陀螺 3D，INO:232-234） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

headingGyro3D = sensor.read(0)[0]   # gyro 3D heading：-32768~32767 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
headingGyro3D = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t headingGyro3D = mbcRead(0x6A, 0x01);   // gyro 3D heading：-32768~32767 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns gyro 3D heading（-32768~32767 °）。Host sends `[01]` → reply `[lo][hi][01 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] gyro 3D heading :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R~G (+32768).

```cpp
// A. stock 0x22 blocks：R~G = gyro 3D heading（R = (航向+32768) 低 byte、G = 高 byte（R4 版：低在前）；主機 hi×256+lo−32768）
int raw = MiniR4.I2C1.MXColor.getColor(R);   // group 1
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC gyro 3D heading :: #E8711E)</pre>

```typescript
let headingGyro3D = MBC.readValue(0x6A, 0x01)   // gyro 3D heading：-32768~32767 °
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` fused heading <small>headingFused</small>

| Item | Detail |
| :--- | :--- |
| Meaning | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |
| Range | `-32768~32767` ° |
| I2C / mBlock id | `02` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int16 · GYRO-RATE 1×int16 = 融合航向（EV3 定案：不改） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

headingFused = sensor.read(1)[0]   # fused heading：-32768~32767 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
headingFused = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t headingFused = mbcRead(0x6A, 0x02);   // fused heading：-32768~32767 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns fused heading（-32768~32767 °）。Host sends `[02]` → reply `[lo][hi][02 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] fused heading :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B~C (+32768).

```cpp
// A. stock 0x22 blocks：B~C = fused heading（B = 低 byte、C = 高 byte）
int raw = MiniR4.I2C1.MXColor.getColor(B);   // group 1
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC fused heading :: #E8711E)</pre>

```typescript
let headingFused = MBC.readValue(0x6A, 0x02)   // fused heading：-32768~32767 °
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` gyro 1D heading <small>headingGyro1D</small>

| Item | Detail |
| :--- | :--- |
| Meaning | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |
| Range | `-32768~32767` ° |
| I2C / mBlock id | `03` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

mode 3 · int16 · GYRO-G&A 2×int16 [陀螺 1D, 磁力航向] (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

headingGyro1D = sensor.read(3)[0]   # gyro 1D heading：-32768~32767 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
headingGyro1D = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t headingGyro1D = mbcRead(0x6A, 0x03);   // gyro 1D heading：-32768~32767 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns gyro 1D heading（-32768~32767 °）。Host sends `[03]` → reply `[lo][hi][03 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] gyro 1D heading :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 M~Y (+32768).

```cpp
// A. stock 0x22 blocks：M~Y = gyro 1D heading（M = 低 byte、Y = 高 byte；K（10）= 0）
int raw = MiniR4.I2C1.MXColor.getColor(M);   // group 1
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC gyro 1D heading :: #E8711E)</pre>

```typescript
let headingGyro1D = MBC.readValue(0x6A, 0x03)   // gyro 1D heading：-32768~32767 °
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` magnetic heading <small>headingMag</small>

| Item | Detail |
| :--- | :--- |
| Meaning | tilt-compensated magnetometer heading, degrees, relative to the zero point |
| Range | `-32768~32767` ° |
| I2C / mBlock id | `04` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

mode 3 · int16 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

headingMag = sensor.read(3)[1]   # magnetic heading：-32768~32767 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
headingMag = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t headingMag = mbcRead(0x6A, 0x04);   // magnetic heading：-32768~32767 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns magnetic heading（-32768~32767 °）。Host sends `[04]` → reply `[lo][hi][04 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] magnetic heading :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC magnetic heading :: #E8711E)</pre>

```typescript
let headingMag = MBC.readValue(0x6A, 0x04)   // magnetic heading：-32768~32767 °
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` yaw rate <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="yaw rate" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | angular rate about the yaw axis (getYawAxis()), 0.1 deg/s, same sign as the heading |
| Range | `-32768~32767` 0.1 dps |
| I2C / mBlock id | `05` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

mode 2 · int16 · GYRO-FAS 1×int16（#67 新增，原本 0）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

yawRate = sensor.read(2)[0]   # yaw rate：-32768~32767 0.1 dps
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
yawRate = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t yawRate = mbcRead(0x6A, 0x05);   // yaw rate：-32768~32767 0.1 dps
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns yaw rate（-32768~32767 0.1 dps）。Host sends `[05]` → reply `[lo][hi][05 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] yaw rate :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC yaw rate :: #E8711E)</pre>

```typescript
let yawRate = MBC.readValue(0x6A, 0x05)   // yaw rate：-32768~32767 0.1 dps
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` pitch angle <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="pitch angle" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | elevation of the board's printed X axis above horizontal: +90 pointing up, −90 pointing down, 0 level (from the gravity vector) |
| Range | `-90~+90` ° |
| I2C / mBlock id | `06` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

mode 6 · int16 · TILT-ANG 1×int16（#67 新增，原本 0）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

pitch = sensor.read(6)[0]   # pitch angle：-90~+90 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 6)
pitch = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t pitch = mbcRead(0x6A, 0x06);   // pitch angle：-90~+90 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns pitch angle（-90~+90 °）。Host sends `[06]` → reply `[lo][hi][06 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] pitch angle :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC pitch angle :: #E8711E)</pre>

```typescript
let pitch = MBC.readValue(0x6A, 0x06)   // pitch angle：-90~+90 °
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` pitch rate <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="pitch rate" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | angular rate about the board's printed X axis, 0.1 °/s (right-hand rule, CCW seen from +X is positive); this is gyro X, not the derivative of 06 |
| Range | `-32768~32767` 0.1 dps |
| I2C / mBlock id | `07` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

mode 5 · int16 · TILT-RATE 1×int16（#67：取代 #65 統一時放的 ID，ID 移到模式 4）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

pitchRate = sensor.read(5)[0]   # pitch rate：-32768~32767 0.1 dps
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
pitchRate = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t pitchRate = mbcRead(0x6A, 0x07);   // pitch rate：-32768~32767 0.1 dps
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns pitch rate（-32768~32767 0.1 dps）。Host sends `[07]` → reply `[lo][hi][07 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] pitch rate :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC pitch rate :: #E8711E)</pre>

```typescript
let pitchRate = MBC.readValue(0x6A, 0x07)   // pitch rate：-32768~32767 0.1 dps
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` magnetometer trust <small>magTrust</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magTrust.svg" alt="magnetometer trust" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | how much the fusion trusts the magnetometer, 0~255 (= 0~1 × 255) |
| Range | `0~255` |
| I2C / mBlock id | `08` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t magTrust = mbcRead(0x6A, 0x08);   // magnetometer trust：0~255
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns magnetometer trust（0~255）。Host sends `[08]` → reply `[lo][hi][08 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] magnetometer trust :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC magnetometer trust :: #E8711E)</pre>

```typescript
let magTrust = MBC.readValue(0x6A, 0x08)   // magnetometer trust：0~255
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` magnetic disturbance <small>magDisturb</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magDisturb.svg" alt="magnetic disturbance" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | magnetic disturbance level 0~255 (degrees × 100, capped at 255) |
| Range | `0~255` 0.01° |
| I2C / mBlock id | `09` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t magDisturb = mbcRead(0x6A, 0x09);   // magnetic disturbance：0~255 0.01°
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns magnetic disturbance（0~255 0.01°）。Host sends `[09]` → reply `[lo][hi][09 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] magnetic disturbance :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC magnetic disturbance :: #E8711E)</pre>

```typescript
let magDisturb = MBC.readValue(0x6A, 0x09)   // magnetic disturbance：0~255 0.01°
```

</details>

</section>

<section id="v0a" class="ref-item">

### `0A` face up <small>upFace</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-upFace.svg" alt="face up" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | which axis points up: 0 +X, 1 +Y, 2 +Z, 3 −X, 4 −Y, 5 −Z (physical axes) |
| Range | `0~5` |
| I2C / mBlock id | `0A` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t upFace = mbcRead(0x6A, 0x0A);   // face up：0~5
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns face up（0~5）。Host sends `[0A]` → reply `[lo][hi][0A ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(IMU Gyro port [1 v] face up :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC face up :: #E8711E)</pre>

```typescript
let upFace = MBC.readValue(0x6A, 0x0A)   // face up：0~5
```

</details>

</section>

<section id="v10" class="ref-item">

### `10~12` gyro axis 1~3 <small>gyro</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-gyro.svg" alt="gyro axis 1~3" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | 10 X, 11 Y, 12 Z angular rate, 0.1 deg/s (physical axes, sign-corrected) |
| Range | `-32768~32767` 0.1 dps |
| I2C / mBlock id | `10~12` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Ids 0x10~0x12 are consecutive: one `mbcReadMany` call returns them from the same 10 ms snapshot.

```cpp
int16_t gyro[3];
mbcReadMany(0x6A, 0x10, 3, gyro);   // gyro axis 1~3：-32768~32767 0.1 dps
for (uint8_t i = 0; i < 3; i++) { Serial.print(gyro[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns gyro axis 1~3（-32768~32767 0.1 dps）。Host sends `[10]` → reply `[lo][hi][10 ^ lo ^ hi]`, value = int16 little-endian; channel N uses id 0x10 + (N − 1).

<pre class="blocks">(IMU Gyro port [1 v] gyro [axis] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC gyro [axis] :: #E8711E)</pre>

```typescript
let gyro = MBC.readValues(0x6A, 0x10, 3)   // gyro axis 1~3
```

</details>

</section>

<section id="v13" class="ref-item">

### `13~15` accel axis 1~3 <small>accel</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-accel.svg" alt="accel axis 1~3" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | 13 X, 14 Y, 15 Z acceleration, mg (1000 = 1 g) |
| Range | `-32768~32767` mg |
| I2C / mBlock id | `13~15` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Ids 0x13~0x15 are consecutive: one `mbcReadMany` call returns them from the same 10 ms snapshot.

```cpp
int16_t accel[3];
mbcReadMany(0x6A, 0x13, 3, accel);   // accel axis 1~3：-32768~32767 mg
for (uint8_t i = 0; i < 3; i++) { Serial.print(accel[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns accel axis 1~3（-32768~32767 mg）。Host sends `[13]` → reply `[lo][hi][13 ^ lo ^ hi]`, value = int16 little-endian; channel N uses id 0x13 + (N − 1).

<pre class="blocks">(IMU Gyro port [1 v] accel [axis] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC accel [axis] :: #E8711E)</pre>

```typescript
let accel = MBC.readValues(0x6A, 0x13, 3)   // accel axis 1~3
```

</details>

</section>

<section id="v16" class="ref-item">

### `16~18` mag axis 1~3 <small>mag</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-mag.svg" alt="mag axis 1~3" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | 16 X, 17 Y, 18 Z magnetic field, mG (milligauss) |
| Range | `-32768~32767` mG |
| I2C / mBlock id | `16~18` |

**How to read it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Ids 0x16~0x18 are consecutive: one `mbcReadMany` call returns them from the same 10 ms snapshot.

```cpp
int16_t mag[3];
mbcReadMany(0x6A, 0x16, 3, mag);   // mag axis 1~3：-32768~32767 mG
for (uint8_t i = 0; i < 3; i++) { Serial.print(mag[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns mag axis 1~3（-32768~32767 mG）。Host sends `[16]` → reply `[lo][hi][16 ^ lo ^ hi]`, value = int16 little-endian; channel N uses id 0x16 + (N − 1).

<pre class="blocks">(IMU Gyro port [1 v] mag [axis] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC mag [axis] :: #E8711E)</pre>

```typescript
let mag = MBC.readValues(0x6A, 0x16, 3)   // mag axis 1~3
```

</details>

</section>

## Write commands

Only Arduino I2C, mBlock and micro:bit can send commands; the SPIKE / EV3 ports are read-only.

<section id="ce0" class="ref-item ref-cmd">

### `E0` zero heading <small>zeroHeading</small>

| Item | Detail |
| :--- | :--- |
| Action | zero all four headings (zeroHeadings()); EV3 also accepts the stock gyro-reset WRITE 0x11 |
| Parameters | none |
| Busy flag | none (immediate) |

**How to send it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE0);   // zero heading
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E0]` → reply `[00][00][E0]` (ack), then it runs.

<pre class="blocks">IMU Gyro port [1 v] zero heading :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC zero heading :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE0)   // zero heading
```

</details>

</section>

<section id="ce1" class="ref-item ref-cmd">

### `E1` start accelerometer calibration <small>accelCalStart</small>

| Item | Detail |
| :--- | :--- |
| Action | start accelerometer calibration (= v1 0x20=0x01); F3 b4 = 1 meanwhile |
| Parameters | none |
| Busy flag | F3 bit4（1 while running） |

**How to send it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE1);   // start accelerometer calibration
while (mbcRead(0x6A, 0xF3) & (1 << 4)) delay(10);   // wait until F3 bit4 clears = done
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E1]` → reply `[00][00][E1]` (ack), then it runs; F3 bit4 = 1 while busy.

<pre class="blocks">IMU Gyro port [1 v] start accelerometer calibration :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC start accel calibration :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE1)   // start accelerometer calibration
```

</details>

</section>

<section id="ce2" class="ref-item ref-cmd">

### `E2` end accelerometer calibration <small>accelCalEnd</small>

| Item | Detail |
| :--- | :--- |
| Action | finish accelerometer calibration and save to EEPROM (= v1 0x20=0x02); headings are zeroed once on success |
| Parameters | none |
| Busy flag | none (immediate) |

**How to send it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE2);   // end accelerometer calibration
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E2]` → reply `[00][00][E2]` (ack), then it runs.

<pre class="blocks">IMU Gyro port [1 v] end accelerometer calibration :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC end accel calibration :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE2)   // end accelerometer calibration
```

</details>

</section>

<section id="ce3" class="ref-item ref-cmd">

### `E3` start magnetometer calibration <small>magCalStart</small>

| Item | Detail |
| :--- | :--- |
| Action | start magnetometer calibration (= v1 0x20=0x03); F3 b5 = 1 meanwhile |
| Parameters | none |
| Busy flag | F3 bit5（1 while running） |

**How to send it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE3);   // start magnetometer calibration
while (mbcRead(0x6A, 0xF3) & (1 << 5)) delay(10);   // wait until F3 bit5 clears = done
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E3]` → reply `[00][00][E3]` (ack), then it runs; F3 bit5 = 1 while busy.

<pre class="blocks">IMU Gyro port [1 v] start magnetometer calibration :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC start mag calibration :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE3)   // start magnetometer calibration
```

</details>

</section>

<section id="ce4" class="ref-item ref-cmd">

### `E4` end magnetometer calibration <small>magCalEnd</small>

| Item | Detail |
| :--- | :--- |
| Action | finish magnetometer calibration and save to EEPROM (= v1 0x20=0x04) |
| Parameters | none |
| Busy flag | none (immediate) |

**How to send it**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE4);   // end magnetometer calibration
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E4]` → reply `[00][00][E4]` (ack), then it runs.

<pre class="blocks">IMU Gyro port [1 v] end magnetometer calibration :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC end mag calibration :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE4)   // end magnetometer calibration
```

</details>

</section>

<section id="flags" class="ref-item">

## Status flags F3

One status per bit of `F3`; read directly over I2C, mBlock, micro:bit.

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

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

EV3 has no F3 flags; mode 5 returns the device ID and firmware version (ID format B).

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

id_word, fw = sensor.read(5)   # mode 5: [ID | hw0<<8, major×100+minor]
addr = id_word & 0xFF             # 0x6A
hw0 = id_word >> 8                # 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x6A, 0xF3);
bool ready = st & (1 << 0);   // ready
bool still = st & (1 << 1);   // stationary
bool axisCaptured = st & (1 << 2);   // mount axis captured
bool mountTilted = st & (1 << 3);   // mount too tilted
bool accelCalibrating = st & (1 << 4);   // accel calibrating
bool magCalibrating = st & (1 << 5);   // mag calibrating
bool fusionInit = st & (1 << 6);   // fusion initialised
bool magAutoCalDone = st & (1 << 7);   // mag auto-cal converged
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

The extension splits status flags F3 into boolean blocks (ready, calibrating, …).

<pre class="blocks"><IMU Gyro port [1 v] connected? :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

The status flags are not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC device ready :: #E8711E></pre>

```typescript
let st = MBC.status(0x6A)   // F3
let ready = (st & 1) != 0
```

</details>

</section>

## Shared registers and commands

Identical on every product, documented once in the [universal I2C protocol](/en/i2c-protocol.md):

| Id | Name | Meaning |
| :--- | :--- | :--- |
| [`F0`](/en/i2c-protocol.md#rf0) | device ID | low byte = factory I2C address (unchanged after re-addressing), high byte = hw[0] (channels / sensors / variant) |
| [`F1`](/en/i2c-protocol.md#rf1) | protocol version | 2 for this release (same number reported in SPIKE mode 8 v[1]) |
| [`F2`](/en/i2c-protocol.md#rf2) | firmware version | major×100 + minor (e.g. 102 = 1.2) |
| [`F3`](/en/i2c-protocol.md#rf3) | status flags | b0 = ready; other bits per product (see flags) |
| [`F4`](/en/i2c-protocol.md#rf4) | update counter | +1 every 10 ms snapshot, wraps 0~255 (2.56 s per cycle, not usable for reboot detection; frozen = module busy) |
| [`F5`](/en/i2c-protocol.md#rf5) | uptime seconds | seconds since boot, saturates at 32767 (~9 h); a drop means the module rebooted |
| [`F8`](/en/i2c-protocol.md#rf8) | change I2C address | [F8][new 0x08~0x77][~new], saved to EEPROM, takes effect after reboot; [F8][0x00][0xFF] = back to factory address |
| [`EF`](/en/i2c-protocol.md#ref) | reboot | [EF][0xA5]; mBlock sends ACK first, then reboots |

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
