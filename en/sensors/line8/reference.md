<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line8.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/line8/index.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/line8/index.md">Pathfinder 8-Way</a></p>

# Line Follower 8-ch: value & command reference

[🔙 Back to Line Follower 8-ch](/en/sensors/line8/index.md)

Every value and command the Line Follower 8-ch reports to each host: what it means, its range, and how to read it on SPIKE / EV3 / Arduino I2C / mBlock / MATRIX / micro:bit. Append `?host=spike` (or ev3, arduino, mblock, matrix, microbit) to the URL to expand that host by default.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/line8/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/line8/ev3-hub.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/line8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/line8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/line8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/line8/microbit.md">micro:bit</a></p>

| Item | Detail |
| :--- | :--- |
| Factory I2C address | `0x16` |
| hw[0] | `8`（F0 high byte; ID high byte in SPIKE mode 8 / EV3 mode 5） |
| Model | `LINE8` |
| Protocol version | `2` |
| Hosts | SPIKE, EV3, Arduino I2C, mBlock, MATRIX, micro:bit |

## Values at a glance

| Id | Value | Range | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [line position](#v01) | -8~+8 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [fine position](#v02) | -100~+100 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [line width](#v03) | 0~8 路 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [line segments](#v04) | 0~4 | ✓ | — | ✓ | ✓ | — | ✓ |
| [`05`](#v05) | [line bitmap](#v05) | 0~255 bit | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`06`](#v06) | [last exit direction](#v06) | -8~+8 | — | — | ✓ | ✓ | — | ✓ |
| [`07`](#v07) | [threshold](#v07) | 1~99 | — | — | ✓ | ✓ | — | ✓ |
| [`08`](#v08) | [calibrations done](#v08) | 0~255 次 | — | — | ✓ | ✓ | — | ✓ |
| [`10~17`](#v10) | [channel 1~8 reflect](#v10) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

✓ = available on that host; — = not sent.

## Reading each value

<section id="v01" class="ref-item">

### `01` line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |
| Range | `-8~+8` |
| I2C / mBlock id | `01` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 1 −8 🔒; mode 0 (toy) byte 0 −8 🔒; mode 1 byte 0 −8; mode 9 v[4] lo −8 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
linePos = (w & 0xFF) - 8   # line position（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
linePos = line.pos8()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 0 · int8 · IR-PROX (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

linePos = sensor.read(0)[0]   # line position：-8~+8
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
linePos = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t linePos = mbcRead(0x16, 0x01);   // line position：-8~+8
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns line position（-8~+8）。Host sends `[01]` → reply `[lo][hi][01 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Pathfinder port [1 v] line position :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R (+8).

```cpp
// A. stock 0x22 blocks：R = line position + 8
int linePos = MiniR4.I2C1.MXColor.getColor(R) - 8;
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(line position :: #00A4E4)</pre>

```typescript
let linePos = mbc.linePosition()   // line position：-8~+8
```

</details>

**Related examples**：[Arduino example 03](/en/sensors/line8/arduino-examples.md)、[EV3 example 02](/en/sensors/line8/ev3-examples.md)、[SPIKE example 02](/en/sensors/line8/spike-examples.md)

</section>

<section id="v02" class="ref-item">

### `02` fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |
| Range | `-100~+100` |
| I2C / mBlock id | `02` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 2 −100 🔒; mode 3 byte 0 −100; mode 4 v[0] −100; mode 9 v[5] lo −100 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
linePosFine = (w & 0xFF) - 100   # fine position（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
linePosFine = line.pos100()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 2 · int8 · IR-REMOTE byte0 = highRes−100 存成 int8（F030 ev3Uart case 2），主機直接讀 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

linePosFine = sensor.read(2)[0]   # fine position：-100~+100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
linePosFine = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t linePosFine = mbcRead(0x16, 0x02);   // fine position：-100~+100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns fine position（-100~+100）。Host sends `[02]` → reply `[lo][hi][02 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Pathfinder port [1 v] fine line position :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B (+100).

```cpp
// A. stock 0x22 blocks：B = fine position + 100
int linePosFine = MiniR4.I2C1.MXColor.getColor(B) - 100;
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(fine line position :: #00A4E4)</pre>

```typescript
let linePosFine = mbc.linePositionFine()   // fine position：-100~+100
```

</details>

**Related examples**：[Arduino example 06](/en/sensors/line8/arduino-examples.md)、[EV3 example 04](/en/sensors/line8/ev3-examples.md)

</section>

<section id="v03" class="ref-item">

### `03` line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | number of channels on the line, 0~8; 0 = no line |
| Range | `0~8` 路 |
| I2C / mBlock id | `03` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 1 🔒; mode 2 byte 0; mode 9 v[4] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
lineWidth = ((w >> 8) & 0xFF)   # line width（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
lineWidth = line.width()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 2 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

lineWidth = sensor.read(2)[1]   # line width：0~8 路
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
lineWidth = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t lineWidth = mbcRead(0x16, 0x03);   // line width：0~8 路
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns line width（0~8 路）。Host sends `[03]` → reply `[lo][hi][03 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Pathfinder port [1 v] line width :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 G.

```cpp
// A. stock 0x22 blocks：G = line width
int lineWidth = MiniR4.I2C1.MXColor.getColor(G);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(line width :: #00A4E4)</pre>

```typescript
let lineWidth = mbc.lineWidth()   // line width：0~8 路
```

</details>

**Related examples**：[Arduino example 03](/en/sensors/line8/arduino-examples.md)、[EV3 example 03](/en/sensors/line8/ev3-examples.md)

</section>

<section id="v04" class="ref-item">

### `04` line segments <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="line segments" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |
| Range | `0~4` |
| I2C / mBlock id | `04` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 9 v[5] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
groupCount = ((w >> 8) & 0xFF)   # line segments（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
groupCount = line.junction()
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
int16_t groupCount = mbcRead(0x16, 0x04);   // line segments：0~4
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns line segments（0~4）。Host sends `[04]` → reply `[lo][hi][04 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Pathfinder port [1 v] line segments :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(line segments :: #00A4E4)</pre>

```typescript
let groupCount = mbc.lineSegments()   // line segments：0~4
```

**Related block "line is seen"**

```typescript
let lineSeen = mbc.lineSeen()
```

**Related block "on line [lost]"**

```typescript
mbc.onLineEvent(MbcLineEvent.Lost, function () {
})
```

</details>

**Related examples**：[Arduino example 05](/en/sensors/line8/arduino-examples.md)

</section>

<section id="v05" class="ref-item">

### `05` line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |
| Range | `0~255` bit |
| I2C / mBlock id | `05` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 4 v[1]; mode 9 v[6] lo 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[6]
bitmap = (w & 0xFF)   # line bitmap（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
bitmap = line.bin_raw()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 3 · int16 · #67 新：IR-REM-A 1×int16

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

bitmap = sensor.read(3)[0]   # line bitmap：0~255 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
bitmap = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t bitmap = mbcRead(0x16, 0x05);   // line bitmap：0~255 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns line bitmap（0~255 bit）。Host sends `[05]` → reply `[lo][hi][05 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Pathfinder port [1 v] line bitmap :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(line bitmap :: #00A4E4)</pre>

```typescript
let bitmap = mbc.lineBitmap()   // line bitmap：0~255 bit
```

**Related block "channel 1 is on the line"**

```typescript
let lineChannelOnLine = mbc.lineChannelOnLine(1)
```

</details>

**Related examples**：[Arduino example 05](/en/sensors/line8/arduino-examples.md)、[EV3 example 06](/en/sensors/line8/ev3-examples.md)

</section>

<section id="v06" class="ref-item">

### `06` last exit direction <small>lastDir</small>

| Item | Detail |
| :--- | :--- |
| Meaning | where the line was before it was lost: −8 to +8 (negative = left); keeps the last value |
| Range | `-8~+8` |
| I2C / mBlock id | `06` |

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
int16_t lastDir = mbcRead(0x16, 0x06);   // last exit direction：-8~+8
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns last exit direction（-8~+8）。Host sends `[06]` → reply `[lo][hi][06 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Pathfinder port [1 v] last exit direction :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(last exit direction :: #00A4E4)</pre>

```typescript
let lastDir = mbc.lineLastDirection()   // last exit direction：-8~+8
```

</details>

**Related examples**：[Arduino example 06](/en/sensors/line8/arduino-examples.md)

</section>

<section id="v07" class="ref-item">

### `07` threshold <small>threshold</small>

| Item | Detail |
| :--- | :--- |
| Meaning | reflect threshold for 'on line', 1~99, default 50; set with E4 |
| Range | `1~99` |
| I2C / mBlock id | `07` |

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
int16_t threshold = mbcRead(0x16, 0x07);   // threshold：1~99
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns threshold（1~99）。Host sends `[07]` → reply `[lo][hi][07 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Pathfinder port [1 v] threshold :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(line threshold :: #00A4E4)</pre>

```typescript
let threshold = mbc.lineThreshold()   // threshold：1~99
```

</details>

**Related examples**：[Arduino example 07](/en/sensors/line8/arduino-examples.md)

</section>

<section id="v08" class="ref-item">

### `08` calibrations done <small>calibCount</small>

| Item | Detail |
| :--- | :--- |
| Meaning | number of calibrations completed since power-on (button calibrations included) |
| Range | `0~255` 次 |
| I2C / mBlock id | `08` |

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
int16_t calibCount = mbcRead(0x16, 0x08);   // calibrations done：0~255 次
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns calibrations done（0~255 次）。Host sends `[08]` → reply `[lo][hi][08 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Pathfinder port [1 v] calibration count :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(line calibration count :: #00A4E4)</pre>

```typescript
let calibCount = mbc.lineCalibrationCount()   // calibrations done：0~255 次
```

**Related block "line follower is calibrating"**

```typescript
let lineCalibrating = mbc.lineCalibrating()
```

</details>

**Related examples**：[Arduino example 04](/en/sensors/line8/arduino-examples.md)

</section>

<section id="v10" class="ref-item">

### `10~17` channel 1~8 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="channel 1~8 reflect" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |
| Range | `0~100` |
| I2C / mBlock id | `10~17` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 4 🔒; mode 9 v[0] 🔒. Note: 4-bit (v×10)>>6; 每 int16 兩路：lo = 奇數路、hi = 偶數路

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

words = dev.read(9)[0:4]
light = []
for w in words:
    light.append(w & 0xFF)          # odd channel
    light.append((w >> 8) & 0xFF)   # even channel
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
light = line.ir_calibration()   # list, channel 1 = [0]
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 · IR-SEEK 8×int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

light = sensor.read(1)[0:8]   # channel 1~8 reflect
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
light1 = Sensor.ReadRawValue(1, 0)   ' first; for the N-th use 0 + N - 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Ids 0x10~0x17 are consecutive: one `mbcReadMany` call returns them from the same 10 ms snapshot.

```cpp
int16_t light[8];
mbcReadMany(0x16, 0x10, 8, light);   // channel 1~8 reflect：0~100
for (uint8_t i = 0; i < 8; i++) { Serial.print(light[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns channel 1~8 reflect（0~100）。Host sends `[10]` → reply `[lo][hi][10 ^ lo ^ hi]`, value = int16 little-endian; channel N uses id 0x10 + (N − 1).

<pre class="blocks">(Pathfinder port [1 v] channel [1 v] reflect :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 C~K.

```cpp
// A. stock 0x22 blocks：C~K = channel 1~8 reflect（C~K 兩路一組 convert9×10+convert9）
int raw = MiniR4.I2C1.MXColor.getColor(C);   // group 1
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

Returns 0 when the sensor does not answer; check with "device at … is ready" first.

<pre class="blocks">(channel (1) reflect :: #00A4E4)</pre>

```typescript
let light = mbc.lineChannel(1)   // channel 1~8 reflect：0~100 (N = 1~8)
```

</details>

**Related examples**：[Arduino example 02](/en/sensors/line8/arduino-examples.md)、[EV3 example 01](/en/sensors/line8/ev3-examples.md)

</section>

## Write commands

Only Arduino I2C, mBlock and micro:bit can send commands; the SPIKE / EV3 ports are read-only.

<section id="ce0" class="ref-item ref-cmd">

### `E0` black line mode <small>blackLine</small>

| Item | Detail |
| :--- | :--- |
| Action | follow a black line (default) |
| Parameters | none |
| Busy flag | none (immediate) |

**How to send it**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

The SPIKE port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE0);   // black line mode
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E0]` → reply `[00][00][E0]` (ack), then it runs.

<pre class="blocks">Pathfinder port [1 v] black line mode :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">line follower follow [black v] line :: #00A4E4</pre>

```typescript
mbc.lineSetColor(MbcLineColor.Black)   // black line mode
```

</details>

</section>

<section id="ce1" class="ref-item ref-cmd">

### `E1` white line mode <small>whiteLine</small>

| Item | Detail |
| :--- | :--- |
| Action | follow a white line (F3 b1 = 1) |
| Parameters | none |
| Busy flag | none (immediate) |

**How to send it**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

The SPIKE port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE1);   // white line mode
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E1]` → reply `[00][00][E1]` (ack), then it runs.

<pre class="blocks">Pathfinder port [1 v] white line mode :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">line follower follow [white v] line :: #00A4E4</pre>

```typescript
mbc.lineSetColor(MbcLineColor.White)   // white line mode
```

</details>

</section>

<section id="ce2" class="ref-item ref-cmd">

### `E2` calibrate black <small>calibBlack</small>

| Item | Detail |
| :--- | :--- |
| Action | store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM |
| Parameters | none |
| Busy flag | F3 bit2（1 while running） |

**How to send it**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

The SPIKE port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE2);   // calibrate black
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // wait until F3 bit2 clears = done
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E2]` → reply `[00][00][E2]` (ack), then it runs; F3 bit2 = 1 while busy.

<pre class="blocks">Pathfinder port [1 v] calibrate black :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">line follower calibrate [current reading is black v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.Black)   // calibrate black
```

</details>

</section>

<section id="ce3" class="ref-item ref-cmd">

### `E3` calibrate white <small>calibWhite</small>

| Item | Detail |
| :--- | :--- |
| Action | store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM |
| Parameters | none |
| Busy flag | F3 bit2（1 while running） |

**How to send it**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

The SPIKE port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE3);   // calibrate white
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // wait until F3 bit2 clears = done
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E3]` → reply `[00][00][E3]` (ack), then it runs; F3 bit2 = 1 while busy.

<pre class="blocks">Pathfinder port [1 v] calibrate white :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">line follower calibrate [current reading is white v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.White)   // calibrate white
```

</details>

</section>

<section id="ce4" class="ref-item ref-cmd">

### `E4` set threshold <small>setThreshold</small>

| Item | Detail |
| :--- | :--- |
| Action | threshold 1~99 (RAM only, back to 50 after reboot) |
| Parameters | threshold `1~99`（1 byte） |
| Busy flag | none (immediate) |

**How to send it**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

The SPIKE port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE4, 50);   // set threshold（threshold 1~99）
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E4][params…]` → reply `[00][00][E4]` (ack), then it runs.

<pre class="blocks">Pathfinder port [1 v] set threshold (50) :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">line follower set threshold (50) :: #00A4E4</pre>

```typescript
mbc.lineSetThreshold(50)   // set threshold
```

</details>

</section>

<section id="ce7" class="ref-item ref-cmd">

### `E7` 5-second auto calibration <small>calibAuto</small>

| Item | Detail |
| :--- | :--- |
| Action | sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile |
| Parameters | none |
| Busy flag | F3 bit2（1 while running） |

**How to send it**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

The SPIKE port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

The EV3 port carries no write commands: calibrate with the button on the sensor, change settings over Arduino I2C / mBlock.

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE7);   // 5-second auto calibration
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // wait until F3 bit2 clears = done
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E7]` → reply `[00][00][E7]` (ack), then it runs; F3 bit2 = 1 while busy.

<pre class="blocks">Pathfinder port [1 v] 5-second auto calibration :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">line follower calibrate [auto \(sweep for 5 s\) v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.Auto)   // 5-second auto calibration
```

</details>

</section>

<section id="flags" class="ref-item">

## Status flags F3

One status per bit of `F3`; read directly over I2C, mBlock, micro:bit.

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 |
| b1 | white line mode (`whiteMode`) | 1 = following a white line |
| b2 | calibrating (`calibrating`) | E2/E3/E7 in progress; new calibration commands are ignored |
| b3 | line lost (`lost`) | segment count = 0 |
| b4 | fork (`fork`) | segment count ≥ 2 |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE has no F3 flags; mode 8 returns ID / version. The official app cannot read mode 8.

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

id_word, proto, fw, seq = dev.read(8)   # mode 8: [ID | hw0<<8, protocol 2, major×100+minor, counter]
addr = id_word & 0xFF   # 0x16
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
addr = id_word & 0xFF             # 0x16
hw0 = id_word >> 8                # 8
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x16, 0xF3);
bool ready = st & (1 << 0);   // ready
bool whiteMode = st & (1 << 1);   // white line mode
bool calibrating = st & (1 << 2);   // calibrating
bool lost = st & (1 << 3);   // line lost
bool fork = st & (1 << 4);   // fork
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

The extension splits status flags F3 into boolean blocks (ready, calibrating, …).

<pre class="blocks"><Pathfinder port [1 v] connected? :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

The status flags are not available with the stock MATRIX 0x22 blocks.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><device at (0x16 v) is ready :: #00A4E4></pre>

```typescript
let ready = mbc.deviceReady(0x16)   // F3 bit0; false when nothing answers
let st = mbc.status(0x16)   // F3
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

Generated by `_build/gen-reference.mjs` from `_build/protocol/line8.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- B-11：細位置 02 與線位置 01 同號（負 = 左）；v1 韌體註解寫反，程式碼同號。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- SPIKE 教育版模式 0 的 byte 8~15 補 0（#65 已改，使用者保留）。
- EV3 模式 4 與模式 2 相同內容；模式 3 改壓線位元圖（#67）。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體，0x0B 回 1。
