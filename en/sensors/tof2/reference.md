<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof2.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/tof2/index.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/tof2/index.md">Ranger 2-Way</a></p>

# Laser Ranger 2-ch: value & command reference

[🔙 Back to Laser Ranger 2-ch](/en/sensors/tof2/index.md)

Every value and command the Laser Ranger 2-ch reports to each host: what it means, its range, and how to read it on SPIKE / EV3 / Arduino I2C / mBlock / MATRIX / micro:bit. Append `?host=spike` (or ev3, arduino, mblock, matrix, microbit) to the URL to expand that host by default.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/tof2/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/tof2/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/tof2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/tof2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/tof2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/tof2/microbit.md">micro:bit</a></p>

| Item | Detail |
| :--- | :--- |
| Factory I2C address | `0x29` |
| hw[0] | `2`（F0 high byte; ID high byte in SPIKE mode 8 / EV3 mode 5） |
| Model | `LASER2` |
| Protocol version | `2` |
| Hosts | SPIKE, EV3, Arduino I2C, mBlock, MATRIX, micro:bit |

## Values at a glance

| Id | Value | Range | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [left distance (cm)](#v01) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [right distance (cm)](#v02) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [nearer distance (cm)](#v03) | 0~120 cm | — | — | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [left distance (mm)](#v04) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`05`](#v05) | [right distance (mm)](#v05) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`06`](#v06) | [init bitmap](#v06) | 0~3 bit | ✓ | — | ✓ | ✓ | — | ✓ |
| [`07`](#v07) | [fault bitmap](#v07) | 0~3 bit | ✓ | — | ✓ | ✓ | — | ✓ |
| [`08`](#v08) | [average distance (mm)](#v08) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | — | ✓ |

✓ = available on that host; — = not sent.

## Reading each value

<section id="v01" class="ref-item">

### `01` left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |
| Range | `0~120` cm |
| I2C / mBlock id | `01` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 2 byte 0; mode 3 byte 0 🔒; mode 4 v[0] lo; mode 5 v[0] 🔒; mode 8 v[0] lo; mode 9 v[0] lo 🔒. Note: 16 B = [左, 右, 0, 左, 0, 右, 0, 平均, 0×8]：App 顏色 = 左、原始紅 = 256×左; 8 B 原樣（不壓縮）; 韌體註解寫「最近」，實際送左（INO:261-263）; 網站 read(3) 在用：[左, 右, 0]; Pybricks reflection() 編碼：L = 左 cm，a = L/25、b = 10×(L mod 25)+9，3 個 int16 = a×256+b → reflection() = L; 只送 4 B，v0 = 左 + 256×右

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
leftCm = (w & 0xFF)   # left distance (cm)（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
leftCm = laser.left()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 · #67 新：IR-SEEK 8×int8 = [左, 右, 左, 右, 左, 右, 左, 右]（index 0/2/4/6 = 左；原本 index 2~7 = 0）; mode 2 · int8 · IR-REMOTE [左, 右, 0, 0]（INO:137-139）; mode 4 · int8 · IR-S-ALT 同模式 2 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

leftCm = sensor.read(1)[0]   # left distance (cm)：0~120 cm
leftCm = sensor.read(2)[0]   # left distance (cm)：0~120 cm
leftCm = sensor.read(4)[0]   # left distance (cm)：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
leftCm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 2)
leftCm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
leftCm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t leftCm = mbcRead(0x29, 0x01);   // left distance (cm)：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns left distance (cm)（0~120 cm）。Host sends `[01]` → reply `[lo][hi][01 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 2 port [1 v] left distance cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R.

```cpp
// A. stock 0x22 blocks：R = left distance (cm)
int leftCm = MiniR4.I2C1.MXColor.getColor(R);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC left distance cm :: #E8711E)</pre>

```typescript
let leftCm = MBC.readValue(0x29, 0x01)   // left distance (cm)：0~120 cm
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |
| Range | `0~120` cm |
| I2C / mBlock id | `02` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 1 🔒; mode 3 byte 1 🔒; mode 4 v[0] hi; mode 7 v[2] 🔒; mode 8 v[0] hi; mode 9 v[0] hi 🔒. Note: App 反射光 = 右、原始綠 = 256×右; 網站 read(3) 在用; Pybricks ambient() = int16[2]/100 = 右 cm（INO:300-312）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
rightCm = ((w >> 8) & 0xFF)   # right distance (cm)（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
rightCm = laser.right()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 · #67 新：index 1/3/5/7 = 右; mode 2 · int8; mode 4 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

rightCm = sensor.read(1)[1]   # right distance (cm)：0~120 cm
rightCm = sensor.read(2)[1]   # right distance (cm)：0~120 cm
rightCm = sensor.read(4)[1]   # right distance (cm)：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
rightCm = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 2)
rightCm = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 4)
rightCm = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t rightCm = mbcRead(0x29, 0x02);   // right distance (cm)：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns right distance (cm)（0~120 cm）。Host sends `[02]` → reply `[lo][hi][02 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 2 port [1 v] right distance cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 G.

```cpp
// A. stock 0x22 blocks：G = right distance (cm)
int rightCm = MiniR4.I2C1.MXColor.getColor(G);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC right distance cm :: #E8711E)</pre>

```typescript
let rightCm = MBC.readValue(0x29, 0x02)   // right distance (cm)：0~120 cm
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` nearer distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="nearer distance (cm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | the smaller of left and right, 0~120 cm |
| Range | `0~120` cm |
| I2C / mBlock id | `03` |
| Locked | 🔒 LEGO mode layout will not change |

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
int16_t nearCm = mbcRead(0x29, 0x03);   // nearer distance (cm)：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns nearer distance (cm)（0~120 cm）。Host sends `[03]` → reply `[lo][hi][03 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 2 port [1 v] nearer distance cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B.

```cpp
// A. stock 0x22 blocks：B = nearer distance (cm)（B（命令 12 也回較近，schema 不收 12））
int nearCm = MiniR4.I2C1.MXColor.getColor(B);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC nearer distance cm :: #E8711E)</pre>

```typescript
let nearCm = MBC.readValue(0x29, 0x03)   // nearer distance (cm)：0~120 cm
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` left distance (mm) <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="left distance (mm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | left sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |
| Range | `0~1200` mm |
| I2C / mBlock id | `04` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 6 v[0]. Note: #67 新：HSV 3×int16 = [左 mm, 右 mm, 平均 mm]（原本是基底緩衝合併值）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

leftMm = dev.read(6)[0]   # left distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
leftMm = laser.left_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 5 · int16 · #67 新：IR-CAL 2×int16 = [左 mm, 右 mm]（原本 word0 = 左'+256×右'）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

leftMm = sensor.read(5)[0]   # left distance (mm)：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
leftMm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t leftMm = mbcRead(0x29, 0x04);   // left distance (mm)：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns left distance (mm)（0~1200 mm）。Host sends `[04]` → reply `[lo][hi][04 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 2 port [1 v] left distance mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC left distance mm :: #E8711E)</pre>

```typescript
let leftMm = MBC.readValue(0x29, 0x04)   // left distance (mm)：0~1200 mm
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` right distance (mm) <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="right distance (mm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | right sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |
| Range | `0~1200` mm |
| I2C / mBlock id | `05` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 6 v[1]. Note: #67 新

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

rightMm = dev.read(6)[1]   # right distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
rightMm = laser.right_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 5 · int16 · #67 新

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

rightMm = sensor.read(5)[1]   # right distance (mm)：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
rightMm = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t rightMm = mbcRead(0x29, 0x05);   // right distance (mm)：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns right distance (mm)（0~1200 mm）。Host sends `[05]` → reply `[lo][hi][05 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 2 port [1 v] right distance mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC right distance mm :: #E8711E)</pre>

```typescript
let rightMm = MBC.readValue(0x29, 0x05)   // right distance (mm)：0~1200 mm
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | bit0 = left, bit1 = right; 1 = that sensor initialised at power-on (no retry); 3 = both OK |
| Range | `0~3` bit |
| I2C / mBlock id | `06` |
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
initOk = (w & 0xFF)   # init bitmap（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
initOk = laser.is_ok(1)
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
int16_t initOk = mbcRead(0x29, 0x06);   // init bitmap：0~3 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns init bitmap（0~3 bit）。Host sends `[06]` → reply `[lo][hi][06 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 2 port [1 v] init bitmap :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC init bitmap :: #E8711E)</pre>

```typescript
let initOk = MBC.readValue(0x29, 0x06)   // init bitmap：0~3 bit
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | bit0 = left, bit1 = right; 1 = that sensor is currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |
| Range | `0~3` bit |
| I2C / mBlock id | `07` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[1] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
fault = ((w >> 8) & 0xFF)   # fault bitmap（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
fault = laser.is_ok(1)
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
int16_t fault = mbcRead(0x29, 0x07);   // fault bitmap：0~3 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns fault bitmap（0~3 bit）。Host sends `[07]` → reply `[lo][hi][07 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 2 port [1 v] fault bitmap :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC fault bitmap :: #E8711E)</pre>

```typescript
let fault = MBC.readValue(0x29, 0x07)   // fault bitmap：0~3 bit
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` average distance (mm) <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="average distance (mm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | (left mm + right mm) / 2, 0~1200; same number as EV3 mode 3 and the 3rd value of SPIKE mode 6 |
| Range | `0~1200` mm |
| I2C / mBlock id | `08` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 7 🔒; mode 6 v[2]. Note: byte7 = (左 cm + 右 cm)>>1；App 原始藍 = 256×平均; #67 新：平均 mm

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

avgMm = dev.read(0)[7]   # average distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
avgMm = laser.average_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 0 · int8 · IR-PROX = (左'+右')>>1，cm 縮放後平均（鎖住，INO:131-133）; mode 3 · int16 · #67 新：IR-REM-A 1×int16 = 左右平均 mm（原本 = 左 0~100） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

avgMm = sensor.read(0)[0]   # average distance (mm)：0~1200 mm
avgMm = sensor.read(3)[0]   # average distance (mm)：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
avgMm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
avgMm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t avgMm = mbcRead(0x29, 0x08);   // average distance (mm)：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns average distance (mm)（0~1200 mm）。Host sends `[08]` → reply `[lo][hi][08 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 2 port [1 v] average distance mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC average distance mm :: #E8711E)</pre>

```typescript
let avgMm = MBC.readValue(0x29, 0x08)   // average distance (mm)：0~1200 mm
```

</details>

</section>

<section id="flags" class="ref-item">

## Status flags F3

One status per bit of `F3`; read directly over I2C, mBlock, micro:bit.

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | 1 once both sensors are initialised and snapshots are running |
| b1 | any sensor failed (`fault`) | 1 if any sensor failed to initialise, has an I2C error or no update for 200 ms (07 ≠ 0 or 06 ≠ 3) |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE has no F3 flags; mode 8 returns ID / version. The official app cannot read mode 8.

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

id_word, proto, fw, seq = dev.read(8)   # mode 8: [ID | hw0<<8, protocol 2, major×100+minor, counter]
addr = id_word & 0xFF   # 0x29
hw0 = id_word >> 8      # 2
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
addr = id_word & 0xFF             # 0x29
hw0 = id_word >> 8                # 2
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x29, 0xF3);
bool ready = st & (1 << 0);   // ready
bool fault = st & (1 << 1);   // any sensor failed
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

The extension splits status flags F3 into boolean blocks (ready, calibrating, …).

<pre class="blocks"><Ranger 2 port [1 v] connected? :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

The status flags are not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC device ready :: #E8711E></pre>

```typescript
let st = MBC.status(0x29)   // F3
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

Generated by `_build/gen-reference.mjs` from `_build/protocol/tof2.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- 共識 #8：08 左右平均 mm 補進 I2C／mBlock 值表（定案 §5 已列）。
- 0x22 命令 4~12 不動（4 左、5 右、6 最近、7~11 = 0、12 = 最近）；schema 不收 11/12，所以 12 只寫在 03 的 note。
- EV3：模式 0、2、4 鎖住（照盤點）；模式 1 改 [左,右]×4（index 0/1 各代表左右）、模式 3 改平均 mm、模式 5 改 [左 mm, 右 mm]；模式 1 描述表宣告 int8 −100~100，cm×100/120 在範圍內。
- SPIKE：0/5/7/9 鎖住，另加模式 3 鎖住（網站 sensors/tof2/index.md read(3) = 左／右 cm，共識 #11）；模式 6 改 [左 mm, 右 mm, 平均 mm]；模式 1、2、4、8 維持不動；模式 8 v[0] 改 ID|hw[0]<<8（統一後共用函式）。
- 「左」= 站在車後往前看的左（第 0 顆，XSHUT PA6）；韌體只有 L/R 巨集名，盤點檔標為推測。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
