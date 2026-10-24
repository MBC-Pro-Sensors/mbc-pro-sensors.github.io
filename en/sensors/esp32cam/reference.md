<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_school.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/esp32cam/index.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/esp32cam/index.md">Sharpshooter Vision</a></p>

# Vision Sensor (School Edition): value & command reference

[🔙 Back to Vision Sensor (School Edition)](/en/sensors/esp32cam/index.md)

Every value and command the Vision Sensor (School Edition) reports to each host: what it means, its range, and how to read it on SPIKE / EV3 / Arduino I2C / mBlock / MATRIX / micro:bit. Append `?host=spike` (or ev3, arduino, mblock, matrix, microbit) to the URL to expand that host by default.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off">MATRIX</span><a class="chip chip-microbit" href="/en/sensors/esp32cam/microbit.md">micro:bit</a></p>

| Item | Detail |
| :--- | :--- |
| Factory I2C address | `0x4C` |
| hw[0] | `1`（F0 high byte; ID high byte in SPIKE mode 8 / EV3 mode 5） |
| Model | `CAMSCH` |
| Protocol version | `2` |
| Hosts | SPIKE, EV3, Arduino I2C, mBlock, micro:bit |

## Values at a glance

| Id | Value | Range | SPIKE | EV3 | Arduino I2C | mBlock | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [hue](#v01) | -1~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [saturation](#v02) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [brightness](#v03) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [motion](#v04) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`05`](#v05) | [blob X](#v05) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`06`](#v06) | [blob Y](#v06) | 0~75 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`07`](#v07) | [blob width](#v07) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`08`](#v08) | [blob height](#v08) | 0~75 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`09`](#v09) | [blob area](#v09) | 0~7500 | ✓ | — | ✓ | ✓ | ✓ |
| [`0A`](#v0a) | [blob found](#v0a) | 0~1 | — | — | ✓ | ✓ | ✓ |

✓ = available on that host; — = not sent.

## Reading each value

<section id="v01" class="ref-item">

### `01` hue <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="hue" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |
| Range | `-1~100` |
| I2C / mBlock id | `01` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 1 🔒; mode 5 v[0] 🔒; mode 9 v[0] lo 🔒. Note: −1 = 0xFF; 0~10（÷10）; raw = (色相×256 ± 128)/25 ≈ 色相×10.24，v[0]=v[1]=v[2]；Pybricks reflection() ÷10.24 還原

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
hue = (w & 0xFF)   # hue（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
hue = cam.hue()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 0 · int8 · IR-PROX 1×int8; mode 1 · int8 · IR-SEEK [色相, 飽和, 明度, 動態, X, Y, W, H]; mode 2 · int8 · IR-REMOTE [色相, 飽和, 明度, 動態]; mode 3 · int16 · IR-REM-A 1×int16（#65 已修成正確符號延伸，−1 = 0xFFFF）; mode 5 · int16 · IR-CAL [色相, 飽和] 2×int16 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

hue = sensor.read(0)[0]   # hue：-1~100
hue = sensor.read(1)[0]   # hue：-1~100
hue = sensor.read(2)[0]   # hue：-1~100
hue = sensor.read(3)[0]   # hue：-1~100
hue = sensor.read(5)[0]   # hue：-1~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
hue = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 1)
hue = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 2)
hue = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
hue = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 5)
hue = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t hue = mbcRead(0x4C, 0x01);   // hue：-1~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns hue（-1~100）。Host sends `[01]` → reply `[lo][hi][01 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] hue :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC hue :: #E8711E)</pre>

```typescript
let hue = MBC.readValue(0x4C, 0x01)   // hue：-1~100
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` saturation <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="saturation" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | saturation of sample point S1, 0~100 |
| Range | `0~100` |
| I2C / mBlock id | `02` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[0] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
saturation = ((w >> 8) & 0xFF)   # saturation（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
saturation = cam.saturation()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 2 · int8; mode 5 · int16 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

saturation = sensor.read(1)[1]   # saturation：0~100
saturation = sensor.read(2)[1]   # saturation：0~100
saturation = sensor.read(5)[1]   # saturation：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
saturation = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 2)
saturation = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 5)
saturation = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t saturation = mbcRead(0x4C, 0x02);   // saturation：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns saturation（0~100）。Host sends `[02]` → reply `[lo][hi][02 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] saturation :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC saturation :: #E8711E)</pre>

```typescript
let saturation = MBC.readValue(0x4C, 0x02)   // saturation：0~100
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` brightness <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="brightness" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | brightness (HSV value) of sample point S1, 0~100 |
| Range | `0~100` |
| I2C / mBlock id | `03` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[1] lo 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
value = (w & 0xFF)   # brightness（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
value = cam.brightness()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 2 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

value = sensor.read(1)[2]   # brightness：0~100
value = sensor.read(2)[2]   # brightness：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
value = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 2)
value = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t value = mbcRead(0x4C, 0x03);   // brightness：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns brightness（0~100）。Host sends `[03]` → reply `[lo][hi][03 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] brightness :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC brightness :: #E8711E)</pre>

```typescript
let value = MBC.readValue(0x4C, 0x03)   // brightness：0~100
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` motion <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="motion" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | whole-frame motion 0~100 (more change = larger) |
| Range | `0~100` |
| I2C / mBlock id | `04` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 9 v[1] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
motion = ((w >> 8) & 0xFF)   # motion（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
motion = cam.motion()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 2 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

motion = sensor.read(1)[3]   # motion：0~100
motion = sensor.read(2)[3]   # motion：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
motion = Sensor.ReadRawValue(1, 3)
Sensor.SetMode(1, 2)
motion = Sensor.ReadRawValue(1, 3)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t motion = mbcRead(0x4C, 0x04);   // motion：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns motion（0~100）。Host sends `[04]` → reply `[lo][hi][04 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] motion :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC motion :: #E8711E)</pre>

```typescript
let motion = MBC.readValue(0x4C, 0x04)   // motion：0~100
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` blob X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="blob X" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | blob centre X 0~100, left = 0; 0 when not found |
| Range | `0~100` |
| I2C / mBlock id | `05` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 2 🔒; mode 0 (toy) byte 2 🔒; mode 6 v[0]; mode 7 v[2] 🔒; mode 9 v[2] lo 🔒. Note: byte 3 = 0; ×4 查表編碼（10-bit）; #67 改：模式 6 = [X, Y, 面積]（3×int16）; X×100+Y → Pybricks ambient() 顯示 X.Y

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[2]
blobX = (w & 0xFF)   # blob X（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobX = cam.blob_x()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 4 · int8 · IR-S-ALT [X, Y, W, H] (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

blobX = sensor.read(1)[4]   # blob X：0~100
blobX = sensor.read(4)[0]   # blob X：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
blobX = Sensor.ReadRawValue(1, 4)
Sensor.SetMode(1, 4)
blobX = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t blobX = mbcRead(0x4C, 0x05);   // blob X：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns blob X（0~100）。Host sends `[05]` → reply `[lo][hi][05 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] blob X :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC blob X :: #E8711E)</pre>

```typescript
let blobX = MBC.readValue(0x4C, 0x05)   // blob X：0~100
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` blob Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="blob Y" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |
| Range | `0~75` |
| I2C / mBlock id | `06` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 4 🔒; mode 0 (toy) byte 4 🔒; mode 6 v[1]; mode 7 v[2] 🔒; mode 9 v[2] hi 🔒. Note: byte 5 = 0; ×4 查表編碼; X×100+Y 的個位／十位

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[2]
blobY = ((w >> 8) & 0xFF)   # blob Y（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobY = cam.blob_y()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 4 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

blobY = sensor.read(1)[5]   # blob Y：0~75
blobY = sensor.read(4)[1]   # blob Y：0~75
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
blobY = Sensor.ReadRawValue(1, 5)
Sensor.SetMode(1, 4)
blobY = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t blobY = mbcRead(0x4C, 0x06);   // blob Y：0~75
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns blob Y（0~75）。Host sends `[06]` → reply `[lo][hi][06 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] blob Y :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC blob Y :: #E8711E)</pre>

```typescript
let blobY = MBC.readValue(0x4C, 0x06)   // blob Y：0~75
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` blob width <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="blob width" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | blob width W 0~100; 0 = not found |
| Range | `0~100` |
| I2C / mBlock id | `07` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[3] lo 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[3]
blobW = (w & 0xFF)   # blob width（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobW = cam.blob_w()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 4 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

blobW = sensor.read(1)[6]   # blob width：0~100
blobW = sensor.read(4)[2]   # blob width：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
blobW = Sensor.ReadRawValue(1, 6)
Sensor.SetMode(1, 4)
blobW = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t blobW = mbcRead(0x4C, 0x07);   // blob width：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns blob width（0~100）。Host sends `[07]` → reply `[lo][hi][07 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] blob width :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC blob width :: #E8711E)</pre>

```typescript
let blobW = MBC.readValue(0x4C, 0x07)   // blob width：0~100
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` blob height <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="blob height" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | blob height H 0~75; 0 when not found |
| Range | `0~75` |
| I2C / mBlock id | `08` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[3] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[3]
blobH = ((w >> 8) & 0xFF)   # blob height（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobH = cam.blob_h()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 4 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

blobH = sensor.read(1)[7]   # blob height：0~75
blobH = sensor.read(4)[3]   # blob height：0~75
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
blobH = Sensor.ReadRawValue(1, 7)
Sensor.SetMode(1, 4)
blobH = Sensor.ReadRawValue(1, 3)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t blobH = mbcRead(0x4C, 0x08);   // blob height：0~75
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns blob height（0~75）。Host sends `[08]` → reply `[lo][hi][08 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] blob height :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC blob height :: #E8711E)</pre>

```typescript
let blobH = MBC.readValue(0x4C, 0x08)   // blob height：0~75
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` blob area <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="blob area" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | W×H (0~7500); 0 when not found |
| Range | `0~7500` |
| I2C / mBlock id | `09` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 6 🔒; mode 0 (toy) byte 6 🔒; mode 6 v[2]; mode 9 v[4] 🔒. Note: 鎖住：這裡是 map(像素面積, 0~5000, 0~100)，不是 W×H；byte 7 = 0; 同上 0~100 再 ×4 查表編碼; W×H; #67 新增：byte 8~9 = W×H（原本 0）；其餘 byte 不動

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

blobArea = dev.read(9)[4]   # blob area
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobArea = cam.area()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t blobArea = mbcRead(0x4C, 0x09);   // blob area：0~7500
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns blob area（0~7500）。Host sends `[09]` → reply `[lo][hi][09 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] blob area :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC blob area :: #E8711E)</pre>

```typescript
let blobArea = MBC.readValue(0x4C, 0x09)   // blob area：0~7500
```

</details>

</section>

<section id="v0a" class="ref-item">

### `0A` blob found <small>found</small>

| Item | Detail |
| :--- | :--- |
| Meaning | 1 = a blob of the target colour is in view (W > 0), 0 = none |
| Range | `0~1` |
| I2C / mBlock id | `0A` |

**How to read it**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

Not sent in any SPIKE mode (locked modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

Not sent in any EV3 mode (the locked EV3 modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t found = mbcRead(0x4C, 0x0A);   // blob found：0~1
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns blob found（0~1）。Host sends `[0A]` → reply `[lo][hi][0A ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Vision port [1 v] blob found? :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC blob found? :: #E8711E)</pre>

```typescript
let found = MBC.readValue(0x4C, 0x0A)   // blob found：0~1
```

</details>

</section>

<section id="flags" class="ref-item">

## Status flags F3

One status per bit of `F3`; read directly over I2C, mBlock, micro:bit.

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 (values freeze while the camera menu is open) |
| b1 | blob found (`found`) | same as value 0A |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE has no F3 flags; mode 8 returns ID / version. The official app cannot read mode 8.

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

id_word, proto, fw, seq = dev.read(8)   # mode 8: [ID | hw0<<8, protocol 2, major×100+minor, counter]
addr = id_word & 0xFF   # 0x4C
hw0 = id_word >> 8      # 1
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 has no F3 flags; mode 5 returns the device ID and firmware version (ID format B).

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

id_word, fw = sensor.read(5)   # mode 5: [ID | hw0<<8, major×100+minor]
addr = id_word & 0xFF             # 0x4C
hw0 = id_word >> 8                # 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x4C, 0xF3);
bool ready = st & (1 << 0);   // ready
bool found = st & (1 << 1);   // blob found
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

The extension splits status flags F3 into boolean blocks (ready, calibrating, …).

<pre class="blocks"><Vision port [1 v] connected? :: sensing></pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC device ready :: #E8711E></pre>

```typescript
let st = MBC.status(0x4C)   // F3
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

Generated by `_build/gen-reference.mjs` from `_build/protocol/cam_school.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- hw0 = 1（共識 #6：神攝手 1 = 學校版）；model 字串 CAMSCH 照任務指定（韌體原本沒有型號欄位）。
- D4：I2C 0x4C 用 SDA = IO3、SCL = IO1（沿 PS4 Wire.setPins 慣例），與 EV3／SPIKE 串口模式二選一編譯；ESP32 從機不能時脈延展，onReceive 預載 32 B；頁面標「硬體已確認，上機待測」。mBlock 用同一組腳 Serial.begin(38400, SERIAL_8N1, 3, 1)。
- EV3 六個模式全部鎖住照盤點（模式 3／5 的 −1 已在 #65 修成符號延伸）；EV3 定案最後的 golden 備註把 cam_school 列進「模式 5 改 ID」是筆誤（定案表本身寫「—」；模式 5 仍是 [色相, 飽和]），所以 ev3IdMode = null。
- SPIKE 模式 9 鎖住，只有 byte 8~9（v[4]）從 0 改成面積 W×H；模式 6 改 [X, Y, 面積]；模式 8 = [0x4C | 1<<8, 2, 韌體版, 更新序號]；模式 1~4 維持 0。
- SPIKE 模式 0 的「面積」是 map(像素面積 0~5000 → 0~100)，跟值表 09 的 W×H 不同，鎖住不改（照盤點）。
- mVision：N = 10（idx 0~9），色相 −1 送 255；每 20 ms 一包（B-6）；不做 0x22（hosts.matrix = false）。
- 0A 有找到色塊 = W > 0（盤點：W 0 = 沒找到）；F3 b1 同這個值。
- 值 05~08 的「沒找到 = 0」照盤點 §0；色塊 Y 下 = 0、上 = 75 照盤點（map(bestY,120,0,0,75)）。
- 沒有寫入命令（韌體無法從程式改目標顏色／補光燈）。
