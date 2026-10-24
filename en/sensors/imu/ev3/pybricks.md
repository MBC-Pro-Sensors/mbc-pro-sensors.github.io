<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/ev3/pybricks -->
# IMU 9-Axis Gyro × EV3 × Pybricks

This page covers **Pybricks v4 / EV3 MicroPython v2** only: how to set it up for the IMU 9-Axis Gyro on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-pybricks.en.md + _build/guides/product/imu/ev3-pybricks.en.md {"product":"IMU 9-Axis Gyro","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [IMU 9-Axis Gyro: Pybricks v4 / EV3 MicroPython v2](/en/sensors/imu/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes port 1 and reads raw modes with `LUMPDevice`; EV3 MicroPython v2 works the same way.

<section id="v01" class="ref-item">

### gyro 3D heading <small>headingGyro3D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading from the 3D quaternion, degrees, 0 at boot/zero, accumulates past 360 (wraps at ±32767, ~91 turns) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

headingGyro3D = sensor.read(0)[0]   # gyro 3D heading：-32768~32767 °
```

<p class="ref-note">mode 0 · int16 · GYRO-ANG 1×int16（盤點：mode0 = 陀螺 3D，INO:232-234） (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v02" class="ref-item">

### fused heading <small>headingFused</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | 3D gyro heading slowly corrected by the magnetometer, degrees, relative to the zero point; may exceed 360 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

headingFused = sensor.read(1)[0]   # fused heading：-32768~32767 °
```

<p class="ref-note">mode 1 · int16 · GYRO-RATE 1×int16 = 融合航向（EV3 定案：不改） (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v03" class="ref-item">

### gyro 1D heading <small>headingGyro1D</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | heading integrated about the yaw axis only, degrees, 0 at boot/zero; may exceed 360 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

headingGyro1D = sensor.read(3)[0]   # gyro 1D heading：-32768~32767 °
```

<p class="ref-note">mode 3 · int16 · GYRO-G&A 2×int16 [陀螺 1D, 磁力航向] (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v04" class="ref-item">

### magnetic heading <small>headingMag</small>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` ° | tilt-compensated magnetometer heading, degrees, relative to the zero point |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

headingMag = sensor.read(3)[1]   # magnetic heading：-32768~32767 °
```

<p class="ref-note">mode 3 · int16 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v05" class="ref-item">

### yaw rate <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="yaw rate" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | angular rate about the yaw axis (getYawAxis()), 0.1 deg/s, same sign as the heading |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

yawRate = sensor.read(2)[0]   # yaw rate：-32768~32767 0.1 dps
```

<p class="ref-note">mode 2 · int16 · GYRO-FAS 1×int16（#67 新增，原本 0）</p>

</section>

<section id="v06" class="ref-item">

### pitch angle <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="pitch angle" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-90~+90` ° | elevation of the board's printed X axis above horizontal: +90 pointing up, −90 pointing down, 0 level (from the gravity vector) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

pitch = sensor.read(6)[0]   # pitch angle：-90~+90 °
```

<p class="ref-note">mode 6 · int16 · TILT-ANG 1×int16（#67 新增，原本 0）</p>

</section>

<section id="v07" class="ref-item">

### pitch rate <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="pitch rate" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-32768~32767` 0.1 dps | angular rate about the board's printed X axis, 0.1 °/s (right-hand rule, CCW seen from +X is positive); this is gyro X, not the derivative of 06 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

pitchRate = sensor.read(5)[0]   # pitch rate：-32768~32767 0.1 dps
```

<p class="ref-note">mode 5 · int16 · TILT-RATE 1×int16（#67：取代 #65 統一時放的 ID，ID 移到模式 4）</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：magnetometer trust, magnetic disturbance, face up, gyro axis 1~3, accel axis 1~3, mag axis 1~3. Use <a href="/en/sensors/imu/arduino/ide.md">Arduino</a> / <a href="/en/sensors/imu/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/imu/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

The EV3 port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/imu/ev3-examples.md host=ev3 sw=pybricks -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/imu.json` (single source of truth)
- Setup：`_build/guides/software/ev3-pybricks.en.md`、`_build/guides/product/imu/ev3-pybricks.en.md`
- Examples：`sensors/imu/ev3-examples.md`
