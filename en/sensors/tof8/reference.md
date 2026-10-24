<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/tof8/index.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/tof8/index.md">Ranger 8-Way</a></p>

# Laser Ranger 8-ch: value & command reference

[🔙 Back to Laser Ranger 8-ch](/en/sensors/tof8/index.md)

Every value and command the Laser Ranger 8-ch reports to each host: what it means, its range, and how to read it on SPIKE / EV3 / Arduino I2C / mBlock / MATRIX / micro:bit. Append `?host=spike` (or ev3, arduino, mblock, matrix, microbit) to the URL to expand that host by default.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/tof8/microbit.md">micro:bit</a></p>

| Item | Detail |
| :--- | :--- |
| Factory I2C address | `0x29` |
| hw[0] | `8`（F0 high byte; ID high byte in SPIKE mode 8 / EV3 mode 5） |
| Model | `LASER8` |
| Protocol version | `2` |
| Hosts | SPIKE, EV3, Arduino I2C, mBlock, MATRIX, micro:bit |

## Values at a glance

| Id | Value | Range | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [nearest distance (cm)](#v01) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`02`](#v02) | [nearest direction](#v02) | -8~+8 | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`03`](#v03) | [farthest distance (cm)](#v03) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`04`](#v04) | [farthest direction](#v04) | -7~+7 | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`05`](#v05) | [front distance (cm)](#v05) | 0~120 cm | — | ✓ | ✓ | ✓ | — | ✓ |
| [`06`](#v06) | [nearest distance (mm)](#v06) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`07`](#v07) | [init bitmap](#v07) | 0~255 bit | ✓ | — | ✓ | ✓ | — | ✓ |
| [`08`](#v08) | [fault bitmap](#v08) | 0~255 bit | ✓ | — | ✓ | ✓ | — | ✓ |
| [`09`](#v09) | [front distance (mm)](#v09) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`10~17`](#v10) | [sensor 1~8 distance (cm)](#v10) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`18~1F`](#v18) | [sensor 1~8 distance (mm)](#v18) | 0~1200 mm | — | — | ✓ | ✓ | — | ✓ |

✓ = available on that host; — = not sent.

## Reading each value

<section id="v01" class="ref-item">

### `01` nearest distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="nearest distance (cm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |
| Range | `0~120` cm |
| I2C / mBlock id | `01` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 2 byte 0; mode 3 byte 1; mode 4 v[0] hi; mode 7 v[2] 🔒; mode 9 v[4] hi 🔒. Note: [最近方向, 最近距離, 第 1 顆]（第 4 byte 第 8 顆被描述表砍掉）; Pybricks ambient() = int16[2]/100 = 最近 cm（INO:348-361）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
nearCm = ((w >> 8) & 0xFF)   # nearest distance (cm)（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearCm = laser.min_distance()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 2 · int8 · IR-REMOTE [最近方向−8, 最近', 最空曠方向−8, 最空曠']（INO:154-159） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

nearCm = sensor.read(2)[1]   # nearest distance (cm)：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
nearCm = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t nearCm = mbcRead(0x29, 0x01);   // nearest distance (cm)：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns nearest distance (cm)（0~120 cm）。Host sends `[01]` → reply `[lo][hi][01 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] nearest distance cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC nearest distance cm :: #E8711E)</pre>

```typescript
let nearCm = MBC.readValue(0x29, 0x01)   // nearest distance (cm)：0~120 cm
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` nearest direction <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="nearest direction" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |
| Range | `-8~+8` |
| I2C / mBlock id | `02` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 1 byte 0 −8; mode 3 byte 0 −8; mode 4 v[0] lo −8; mode 5 v[0] 🔒; mode 6 v[2]; mode 9 v[4] lo −8 🔒. Note: 原始方位 0~16（8 = 正前）; Pybricks reflection() 編碼：x = 方位−8，負數 +6399，a = x/25、b = 10×(x mod 25)+9，3 個 int16 = a×256+b → reflection() 向零截斷 = −8~+8（INO:319-336）; #67 新：HSV 3×int16 = [最近 mm, 正前方 mm, 最近方向 −8~+8]

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
nearDir = (w & 0xFF) - 8   # nearest direction（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearDir = laser.min_direction()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 2 · int8 · int8 二補數（方位−8）; mode 5 · int16 · #67 新：IR-CAL 2×int16 = [最近 mm, 最近方向 −8~+8]（原本 = 前 4 顆兩兩合併） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

nearDir = sensor.read(2)[0]   # nearest direction：-8~+8
nearDir = sensor.read(5)[1]   # nearest direction：-8~+8
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
nearDir = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 5)
nearDir = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t nearDir = mbcRead(0x29, 0x02);   // nearest direction：-8~+8
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns nearest direction（-8~+8）。Host sends `[02]` → reply `[lo][hi][02 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] nearest direction :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC nearest direction :: #E8711E)</pre>

```typescript
let nearDir = MBC.readValue(0x29, 0x02)   // nearest direction：-8~+8
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` farthest distance (cm) <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="farthest distance (cm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |
| Range | `0~120` cm |
| I2C / mBlock id | `03` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[5] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
farCm = ((w >> 8) & 0xFF)   # farthest distance (cm)（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
farCm = laser.max_distance()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 2 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

farCm = sensor.read(2)[3]   # farthest distance (cm)：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
farCm = Sensor.ReadRawValue(1, 3)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t farCm = mbcRead(0x29, 0x03);   // farthest distance (cm)：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns farthest distance (cm)（0~120 cm）。Host sends `[03]` → reply `[lo][hi][03 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] farthest distance cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC farthest distance cm :: #E8711E)</pre>

```typescript
let farCm = MBC.readValue(0x29, 0x03)   // farthest distance (cm)：0~120 cm
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` farthest direction <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="farthest direction" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |
| Range | `-7~+7` |
| I2C / mBlock id | `04` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[5] lo −8 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
farDir = (w & 0xFF) - 8   # farthest direction（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
farDir = laser.max_direction()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 2 · int8 · int8 二補數（方位−8） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

farDir = sensor.read(2)[2]   # farthest direction：-7~+7
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
farDir = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t farDir = mbcRead(0x29, 0x04);   // farthest direction：-7~+7
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns farthest direction（-7~+7）。Host sends `[04]` → reply `[lo][hi][04 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] farthest direction :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC farthest direction :: #E8711E)</pre>

```typescript
let farDir = MBC.readValue(0x29, 0x04)   // farthest direction：-7~+7
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` front distance (cm) <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="front distance (cm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3 and SPIKE mode 6 carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |
| Range | `0~120` cm |
| I2C / mBlock id | `05` |

**How to read it**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

Not sent in any SPIKE mode (locked modes stay as they are). Use Arduino I2C / mBlock if you need it.

</details>

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

mode 0 · int8 · IR-PROX = (第 4'+第 5')>>1（鎖住，INO:148-150） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

frontCm = sensor.read(0)[0]   # front distance (cm)：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
frontCm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t frontCm = mbcRead(0x29, 0x05);   // front distance (cm)：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns front distance (cm)（0~120 cm）。Host sends `[05]` → reply `[lo][hi][05 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] front distance cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC front distance cm :: #E8711E)</pre>

```typescript
let frontCm = MBC.readValue(0x29, 0x05)   // front distance (cm)：0~120 cm
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` nearest distance (mm) <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="nearest distance (mm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |
| Range | `0~1200` mm |
| I2C / mBlock id | `06` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 6 v[0]. Note: #67 新：[最近 mm, 正前方 mm, 方向]

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

nearMm = dev.read(6)[0]   # nearest distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearMm = laser.nearest_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 5 · int16 · #67 新：[最近 mm, 最近方向]

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

nearMm = sensor.read(5)[0]   # nearest distance (mm)：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
nearMm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t nearMm = mbcRead(0x29, 0x06);   // nearest distance (mm)：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns nearest distance (mm)（0~1200 mm）。Host sends `[06]` → reply `[lo][hi][06 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] nearest distance mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC nearest distance mm :: #E8711E)</pre>

```typescript
let nearMm = MBC.readValue(0x29, 0x06)   // nearest distance (mm)：0~1200 mm
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |
| Range | `0~255` bit |
| I2C / mBlock id | `07` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[6] lo 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[6]
initOk = (w & 0xFF)   # init bitmap（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
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
int16_t initOk = mbcRead(0x29, 0x07);   // init bitmap：0~255 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns init bitmap（0~255 bit）。Host sends `[07]` → reply `[lo][hi][07 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] init bitmap :: sensing)</pre>

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
let initOk = MBC.readValue(0x29, 0x07)   // init bitmap：0~255 bit
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |
| Range | `0~255` bit |
| I2C / mBlock id | `08` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[6] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[6]
fault = ((w >> 8) & 0xFF)   # fault bitmap（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
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
int16_t fault = mbcRead(0x29, 0x08);   // fault bitmap：0~255 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns fault bitmap（0~255 bit）。Host sends `[08]` → reply `[lo][hi][08 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] fault bitmap :: sensing)</pre>

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
let fault = MBC.readValue(0x29, 0x08)   // fault bitmap：0~255 bit
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` front distance (mm) <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="front distance (mm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |
| Range | `0~1200` mm |
| I2C / mBlock id | `09` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 6 v[1]. Note: #67 新：正前方 mm

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

frontMm = dev.read(6)[1]   # front distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
frontMm = laser.front_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 3 · int16 · #67 新：IR-REM-A 1×int16 = 正前方（第 4、5 顆平均）mm（原本 = 第 1 顆 0~100）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

frontMm = sensor.read(3)[0]   # front distance (mm)：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
frontMm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t frontMm = mbcRead(0x29, 0x09);   // front distance (mm)：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns front distance (mm)（0~1200 mm）。Host sends `[09]` → reply `[lo][hi][09 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Ranger 8 port [1 v] front distance mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC front distance mm :: #E8711E)</pre>

```typescript
let frontMm = MBC.readValue(0x29, 0x09)   // front distance (mm)：0~1200 mm
```

</details>

</section>

<section id="v10" class="ref-item">

### `10~17` sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |
| Range | `0~120` cm |
| I2C / mBlock id | `10~17` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 0 🔒; mode 3 byte 2; mode 4 v[1]; mode 8 v[0]; mode 9 v[0] 🔒. Note: 16 B = [第 1~8 顆, 0×8]：App 顏色 = 第 1 顆、反射光 = 第 2 顆、原始紅 = 第 3+256×第 4 …; byte0/1 = 第 1、2 顆 ×10/120（0~10）；byte2~7 = 第 3~8 顆兩顆一組 4-bit 再查表編碼（spikeEncodeU8，INO:259-284）; byte2 = 第 1 顆（byte3 = 第 8 顆被描述表砍掉）; v1 = 第 1 顆 + 256×第 8 顆; 只送 4 B：v0 = 第 1+256×第 2、v1 = 第 3+256×第 4; 每 int16 兩顆：lo = 第 2k+1 顆、hi = 第 2k+2 顆（v0~v3 = 第 1~8 顆）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

words = dev.read(9)[0:4]
distCm = []
for w in words:
    distCm.append(w & 0xFF)          # odd channel
    distCm.append((w >> 8) & 0xFF)   # even channel
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
distCm = laser.distances()   # list, channel 1 = [0]
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 · IR-SEEK 8×int8 = 第 1~8 顆（EV3 視為頻道 1~4 的〈方位,距離〉）; mode 4 · int8 · IR-S-ALT = 前 4 顆 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

distCm = sensor.read(1)[0:8]   # sensor 1~8 distance (cm)
distCm = sensor.read(4)[0:4]   # sensor 1~8 distance (cm)
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
distCm1 = Sensor.ReadRawValue(1, 0)   ' first; for the N-th use 0 + N - 1
Sensor.SetMode(1, 4)
distCm1 = Sensor.ReadRawValue(1, 0)   ' first; for the N-th use 0 + N - 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Ids 0x10~0x17 are consecutive: one `mbcReadMany` call returns them from the same 10 ms snapshot.

```cpp
int16_t distCm[8];
mbcReadMany(0x29, 0x10, 8, distCm);   // sensor 1~8 distance (cm)：0~120 cm
for (uint8_t i = 0; i < 8; i++) { Serial.print(distCm[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns sensor 1~8 distance (cm)（0~120 cm）。Host sends `[10]` → reply `[lo][hi][10 ^ lo ^ hi]`, value = int16 little-endian; channel N uses id 0x10 + (N − 1).

<pre class="blocks">(Ranger 8 port [1 v] sensor [1 v] distance cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R~K.

```cpp
// A. stock 0x22 blocks：R~K = sensor 1~8 distance (cm)（#67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0）
int raw = MiniR4.I2C1.MXColor.getColor(R);   // group 1
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC sensor [1 v] distance cm :: #E8711E)</pre>

```typescript
let distCm = MBC.readValues(0x29, 0x10, 8)   // sensor 1~8 distance (cm)
```

</details>

</section>

<section id="v18" class="ref-item">

### `18~1F` sensor 1~8 distance (mm) <small>distMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distMm.svg" alt="sensor 1~8 distance (mm)" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | distance of each sensor 1~8, 0~1200 mm; 1200 = nothing that way / sensor failed; ids 18~1F |
| Range | `0~1200` mm |
| I2C / mBlock id | `18~1F` |

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

Ids 0x18~0x1F are consecutive: one `mbcReadMany` call returns them from the same 10 ms snapshot.

```cpp
int16_t distMm[8];
mbcReadMany(0x29, 0x18, 8, distMm);   // sensor 1~8 distance (mm)：0~1200 mm
for (uint8_t i = 0; i < 8; i++) { Serial.print(distMm[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns sensor 1~8 distance (mm)（0~1200 mm）。Host sends `[18]` → reply `[lo][hi][18 ^ lo ^ hi]`, value = int16 little-endian; channel N uses id 0x18 + (N − 1).

<pre class="blocks">(Ranger 8 port [1 v] sensor [1 v] distance mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC sensor [1 v] distance mm :: #E8711E)</pre>

```typescript
let distMm = MBC.readValues(0x29, 0x18, 8)   // sensor 1~8 distance (mm)
```

</details>

</section>

<section id="flags" class="ref-item">

## Status flags F3

One status per bit of `F3`; read directly over I2C, mBlock, micro:bit.

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | 1 once all 8 sensors are initialised (~2~4 s after power-on) and snapshots are running |
| b1 | any sensor failed (`fault`) | 1 if any sensor failed to initialise, has an I2C error or no update for 200 ms (08 ≠ 0 or 07 ≠ 255) |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE has no F3 flags; mode 8 returns ID / version. The official app cannot read mode 8.

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

id_word, proto, fw, seq = dev.read(8)   # mode 8: [ID | hw0<<8, protocol 2, major×100+minor, counter]
addr = id_word & 0xFF   # 0x29
hw0 = id_word >> 8      # 8
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
hw0 = id_word >> 8                # 8
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

<pre class="blocks"><Ranger 8 port [1 v] connected? :: sensing></pre>

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

Generated by `_build/gen-reference.mjs` from `_build/protocol/tof8.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- 正前方 mm（EV3 模式 3、SPIKE 模式 6 v[1]）在定案 §5 沒有獨立 I2C 編號 → 掛在 05 正前方 cm 底下，用 scale:mm 標示，不自己加編號。
- 0x22（使用者 2026-10-21 最終定案）：只用一個位址，R~K 由左到右 7 個 cm：第 1、2、3 顆、min(第 4, 第 5)、第 6、7、8 顆；11、12 不放資料；最近距離／方向由主機自己比較。此排法只用在 0x22，EV3／SPIKE不動。
- SPIKE：0/5/7/9 鎖住，1、2、3、4、8 維持不動；模式 6 改 [最近 mm, 正前方 mm, 最近方向 −8~+8]（原本 = 第 2~7 顆兩兩合併）；模式 8 v[0] 改 ID|hw[0]<<8。
- 最空曠方向 04 範圍 −7~+7（maxDirection 永遠不會是 0／16，盤點 INO:1086-1088）；最近方向 02 ±8 = 沒目標、沿用上次偏向側，開機預設 −8。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 主對話 2026-10-21：補 09 正前方 mm（定案 §5 只有 05 正前方 cm，但 EV3 模式 3、SPIKE 模式 6 都送 mm，I2C／mBlock 應有同一個值）。
