<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps4.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# Gamepad Receiver PS4/PS5: value & command reference

[🔙 Back to Gamepad Receiver PS4/PS5](/en/sensors/ps4/index.md)

Every value and command the Gamepad Receiver PS4/PS5 reports to each host: what it means, its range, and how to read it on SPIKE / EV3 / Arduino I2C / mBlock / MATRIX / micro:bit. Append `?host=spike` (or ev3, arduino, mblock, matrix, microbit) to the URL to expand that host by default.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/ps4/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/ps4/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/ps4/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/ps4/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/ps4/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/ps4/microbit.md">micro:bit</a></p>

| Item | Detail |
| :--- | :--- |
| Factory I2C address | `0x42` |
| hw[0] | `2`（F0 high byte; ID high byte in SPIKE mode 8 / EV3 mode 5; big-channel byte 28） |
| Model | `PS4` |
| Protocol version | `2` |
| Hosts | SPIKE, EV3, Arduino I2C, mBlock, MATRIX, micro:bit |

## Values at a glance

| Id | Value | Range | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [16 buttons](#v01) | 0~65535 bit | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [D-pad](#v02) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [shape buttons](#v03) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [shoulder buttons](#v04) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`05`](#v05) | [function buttons](#v05) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`06`](#v06) | [connected](#v06) | 0~1 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`07`](#v07) | [battery](#v07) | 0~100 % | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| [`08`](#v08) | [L2 analog](#v08) | 0~255 | — | — | ✓ | ✓ | — | ✓ |
| [`09`](#v09) | [R2 analog](#v09) | 0~255 | — | — | ✓ | ✓ | — | ✓ |
| [`0A`](#v0a) | [system buttons](#v0a) | 0~3 bit | — | — | ✓ | ✓ | — | ✓ |
| [`0B`](#v0b) | [gamepad type](#v0b) | 0~4 | — | — | ✓ | ✓ | — | ✓ |
| [`10~13`](#v10) | [stick axis 1~4](#v10) | -100~+100 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

✓ = available on that host; — = not sent.

## Reading each value

<section id="v01" class="ref-item">

### `01` 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | all buttons in one 16-bit word (PSX layout): bit0 SELECT(SHARE), 1 L3, 2 R3, 3 START(OPTIONS), 4 up, 5 right, 6 down, 7 left, 8 L2, 9 R2, 10 L1, 11 R1, 12 triangle, 13 circle, 14 cross, 15 square; 1 = pressed; 0 when not connected |
| Range | `0~65535` bit |
| I2C / mBlock id | `01` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 6 🔒; mode 6 v[2]; mode 9 v[6] 🔒. Note: PSX 排列（#65 已改，原本是 Bluepad32 排列）; PSX 排列; PSX 排列（D3，PS2／PS4 相同；函式庫 buttons_psx()）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

buttons16 = dev.read(9)[6]   # 16 buttons
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
buttons16 = pad.buttons_psx()
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
int16_t buttons16 = mbcRead(0x42, 0x01);   // 16 buttons：0~65535 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns 16 buttons（0~65535 bit）。Host sends `[01]` → reply `[lo][hi][01 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] 16 buttons :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

no 0x22; big byte 8~9 u16. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// B. big channel byte 8~9：16 buttons（PSX 排列（Saturation 積木讀 2 byte））
int buttons16 = bigU16(8);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC 16 buttons :: #E8711E)</pre>

```typescript
let buttons16 = MBC.readValue(0x42, 0x01)   // 16 buttons：0~65535 bit
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` D-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="D-pad" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | up 1, right 2, down 4, left 8, added when pressed together (0~15) |
| Range | `0~15` bit |
| I2C / mBlock id | `02` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 6 v[0] lo; mode 7 v[0] lo 🔒; mode 9 v[0] lo 🔒. Note: 低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
dpad = (w & 0xFF)   # D-pad（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
dpad = pad.pressed('UP')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 0 · int8 · IR-PROX 1×int8; mode 1 · int8 · IR-SEEK [方向鍵, LX, 形狀鍵, LY, 肩鍵, RX, 功能鍵, RY]; mode 3 · int16 · 低 nibble；高 nibble = 形狀鍵；bit8 = 已連線（+256，#67 新增）; mode 4 · int8 · IR-S-ALT [方向鍵, LX, 形狀鍵, LY] (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

dpad = sensor.read(0)[0]   # D-pad：0~15 bit
dpad = sensor.read(1)[0]   # D-pad：0~15 bit
dpad = sensor.read(3)[0]   # D-pad：0~15 bit
dpad = sensor.read(4)[0]   # D-pad：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 1)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
dpad = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t dpad = mbcRead(0x42, 0x02);   // D-pad：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns D-pad（0~15 bit）。Host sends `[02]` → reply `[lo][hi][02 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] D-pad :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R; big byte 0 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// A. stock 0x22 blocks：R = D-pad（R = 方向鍵 | 形狀鍵<<4（上 1 右 2 下 4 左 8、▲16 ●32 ✕64 ■128））
int dpad = MiniR4.I2C1.MXColor.getColor(R);
```

**B. big channel (LauMo library)**

```cpp
// B. big channel byte 0：D-pad
int dpad = bigU8(0);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC D-pad :: #E8711E)</pre>

```typescript
let dpad = MBC.readValue(0x42, 0x02)   // D-pad：0~15 bit
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` shape buttons <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="shape buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | triangle 1, circle 2, cross 4, square 8, added when pressed together (0~15) |
| Range | `0~15` bit |
| I2C / mBlock id | `03` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 6 v[0] hi; mode 7 v[0] hi 🔒; mode 9 v[0] lo 🔒. Note: 高 nibble; 高 nibble; 高 nibble; 高 nibble

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
shapeKeys = (w & 0xFF)   # shape buttons（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
shapeKeys = pad.pressed('TRIANGLE')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 3 · int16 · 高 nibble（÷16 取整數再 mod 16）; mode 4 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

shapeKeys = sensor.read(1)[2]   # shape buttons：0~15 bit
shapeKeys = sensor.read(3)[0]   # shape buttons：0~15 bit
shapeKeys = sensor.read(4)[2]   # shape buttons：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shapeKeys = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 3)
shapeKeys = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
shapeKeys = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t shapeKeys = mbcRead(0x42, 0x03);   // shape buttons：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns shape buttons（0~15 bit）。Host sends `[03]` → reply `[lo][hi][03 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] shape buttons :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

no 0x22; big byte 1 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// B. big channel byte 1：shape buttons
int shapeKeys = bigU8(1);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC shape buttons :: #E8711E)</pre>

```typescript
let shapeKeys = MBC.readValue(0x42, 0x03)   // shape buttons：0~15 bit
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` shoulder buttons <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="shoulder buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | L2 1, R2 2, L1 4, R1 8, added when pressed together (0~15) |
| Range | `0~15` bit |
| I2C / mBlock id | `04` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 0 🔒; mode 2 byte 0; mode 6 v[1] lo; mode 7 v[1] lo 🔒; mode 9 v[1] lo 🔒. Note: 低 nibble；高 nibble = 功能鍵; 低 nibble；高 nibble = 功能鍵; 低 nibble；高 nibble = 功能鍵

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
shoulderKeys = (w & 0xFF)   # shoulder buttons（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
shoulderKeys = pad.pressed('L1')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

shoulderKeys = sensor.read(1)[4]   # shoulder buttons：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shoulderKeys = Sensor.ReadRawValue(1, 4)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t shoulderKeys = mbcRead(0x42, 0x04);   // shoulder buttons：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns shoulder buttons（0~15 bit）。Host sends `[04]` → reply `[lo][hi][04 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] shoulder buttons :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 G; big byte 2 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// A. stock 0x22 blocks：G = shoulder buttons（G = 肩鍵 | 功能鍵<<4（L2 1 R2 2 L1 4 R1 8、SELECT 16 L3 32 R3 64 START 128））
int shoulderKeys = MiniR4.I2C1.MXColor.getColor(G);
```

**B. big channel (LauMo library)**

```cpp
// B. big channel byte 2：shoulder buttons
int shoulderKeys = bigU8(2);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC shoulder buttons :: #E8711E)</pre>

```typescript
let shoulderKeys = MBC.readValue(0x42, 0x04)   // shoulder buttons：0~15 bit
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` function buttons <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="function buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | SELECT(SHARE) 1, L3 2, R3 4, START(OPTIONS) 8, added when pressed together (0~15) |
| Range | `0~15` bit |
| I2C / mBlock id | `05` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 1 🔒; mode 2 byte 0; mode 6 v[1] hi; mode 7 v[1] hi 🔒; mode 9 v[1] lo 🔒. Note: 高 nibble; 單鍵編號 1~8（掃 功能鍵<<4|肩鍵 的 bit0→7：1 L2 2 R2 3 L1 4 R1 5 SELECT 6 L3 7 R3 8 START）; 高 nibble; 高 nibble

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
funcKeys = (w & 0xFF)   # function buttons（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
funcKeys = pad.pressed('START')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

funcKeys = sensor.read(1)[6]   # function buttons：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
funcKeys = Sensor.ReadRawValue(1, 6)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t funcKeys = mbcRead(0x42, 0x05);   // function buttons：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns function buttons（0~15 bit）。Host sends `[05]` → reply `[lo][hi][05 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] function buttons :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

no 0x22; big byte 3 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// B. big channel byte 3：function buttons
int funcKeys = bigU8(3);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC function buttons :: #E8711E)</pre>

```typescript
let funcKeys = MBC.readValue(0x42, 0x05)   // function buttons：0~15 bit
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` connected <small>connected</small>

| Item | Detail |
| :--- | :--- |
| Meaning | 1 = gamepad connected and data seen within 300 ms, 0 = not connected (buttons 0, sticks centred while disconnected) |
| Range | `0~1` |
| I2C / mBlock id | `06` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 3 byte 0. Note: #67 改：模式 3 = [已連線, 電量, 0]（3×int8）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

connected = dev.read(3)[0]   # connected
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
connected = pad.connected()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 3 · int16 · bit8（值 ≥ 256 = 已連線，#67 新增）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

connected = sensor.read(3)[0]   # connected：0~1
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
connected = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t connected = mbcRead(0x42, 0x06);   // connected：0~1
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns connected（0~1）。Host sends `[06]` → reply `[lo][hi][06 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] gamepad connected? :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 K; big byte 10 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// A. stock 0x22 blocks：K = connected（K = 1／0）
int connected = MiniR4.I2C1.MXColor.getColor(K);
```

**B. big channel (LauMo library)**

```cpp
// B. big channel byte 10：connected
int connected = bigU8(10);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC gamepad connected? :: #E8711E)</pre>

```typescript
let connected = MBC.readValue(0x42, 0x06)   // connected：0~1
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` battery <small>battery</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-battery.svg" alt="battery" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | gamepad battery 0~100 (always 0 on the PS2 receiver) |
| Range | `0~100` % |
| I2C / mBlock id | `07` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 3 byte 1

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

battery = dev.read(3)[1]   # battery
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
battery = pad.battery()
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
int16_t battery = mbcRead(0x42, 0x07);   // battery：0~100 %
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns battery（0~100 %）。Host sends `[07]` → reply `[lo][hi][07 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] battery :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

no 0x22; big byte 11 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// B. big channel byte 11：battery
int battery = bigU8(11);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC battery :: #E8711E)</pre>

```typescript
let battery = MBC.readValue(0x42, 0x07)   // battery：0~100 %
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` L2 analog <small>l2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-l2Analog.svg" alt="L2 analog" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | L2 trigger depth 0~255 (0 when not connected) |
| Range | `0~255` |
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
int16_t l2Analog = mbcRead(0x42, 0x08);   // L2 analog：0~255
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns L2 analog（0~255）。Host sends `[08]` → reply `[lo][hi][08 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] L2 analog :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available over MATRIX 0x22 or the big channel.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC L2 analog :: #E8711E)</pre>

```typescript
let l2Analog = MBC.readValue(0x42, 0x08)   // L2 analog：0~255
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` R2 analog <small>r2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-r2Analog.svg" alt="R2 analog" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | R2 trigger depth 0~255 (0 when not connected) |
| Range | `0~255` |
| I2C / mBlock id | `09` |

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
int16_t r2Analog = mbcRead(0x42, 0x09);   // R2 analog：0~255
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns R2 analog（0~255）。Host sends `[09]` → reply `[lo][hi][09 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] R2 analog :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available over MATRIX 0x22 or the big channel.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC R2 analog :: #E8711E)</pre>

```typescript
let r2Analog = MBC.readValue(0x42, 0x09)   // R2 analog：0~255
```

</details>

</section>

<section id="v0a" class="ref-item">

### `0A` system buttons <small>sysKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-sysKeys.svg" alt="system buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | b0 PS button, b1 capture (PS5 mute) button; 0 when not connected (always 0 on PS2) |
| Range | `0~3` bit |
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
int16_t sysKeys = mbcRead(0x42, 0x0A);   // system buttons：0~3 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns system buttons（0~3 bit）。Host sends `[0A]` → reply `[lo][hi][0A ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] system buttons :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available over MATRIX 0x22 or the big channel.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC system buttons :: #E8711E)</pre>

```typescript
let sysKeys = MBC.readValue(0x42, 0x0A)   // system buttons：0~3 bit
```

</details>

</section>

<section id="v0b" class="ref-item">

### `0B` gamepad type <small>padType</small>

| Item | Detail |
| :--- | :--- |
| Meaning | 0 none, 1 PS2 digital, 2 PS2 analog, 3 PS4, 4 PS5 (or other) |
| Range | `0~4` |
| I2C / mBlock id | `0B` |

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
int16_t padType = mbcRead(0x42, 0x0B);   // gamepad type：0~4
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns gamepad type（0~4）。Host sends `[0B]` → reply `[lo][hi][0B ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] gamepad type :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available over MATRIX 0x22 or the big channel.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC gamepad type :: #E8711E)</pre>

```typescript
let padType = MBC.readValue(0x42, 0x0B)   // gamepad type：0~4
```

</details>

</section>

<section id="v10" class="ref-item">

### `10~13` stick axis 1~4 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="stick axis 1~4" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right/up positive; 0 when not connected |
| Range | `-100~+100` |
| I2C / mBlock id | `10~13` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 2 🔒; mode 0 (toy) byte 2 🔒; mode 4 byte 0; mode 7 v[2] 🔒; mode 9 v[2] 🔒. Note: LX×10+LY（各 1~9，5 = 中間）；byte 4~5 = RX×10+RY; LX×10+LY 查表編碼；byte 4~5 = RX×10+RY；byte 6~7 = 肩鍵 byte 查表; [LX, LY, RX, RY] 0~99（宣告 2×int16，送 4 B）; LX×1000+LY×100+RX×10+RY（各 1~9）; v[2..5] = LX LY RX RY，1~9，5 = 中間

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

words = dev.read(9)[2:6]
stick = []
for w in words:
    stick.append(w & 0xFF)          # odd channel
    stick.append((w >> 8) & 0xFF)   # even channel
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
stick = pad.stick('LX')   # list, channel 1 = [0]
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 · LX；LY 在 index 3、RX 5、RY 7（與按鍵交錯）；0~99，50 = 中間（未連線 50）; mode 2 · int8 · IR-REMOTE [LX, LY, RX, RY] 0~99，50 = 中間; mode 4 · int8 · LX；LY 在 index 3（RX、RY 不在模式 4） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

stick = sensor.read(1)[1]   # stick axis 1~4：-100~+100
stick = sensor.read(2)[0:4]   # stick axis 1~4
stick = sensor.read(4)[1]   # stick axis 1~4：-100~+100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
stick = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 2)
stick1 = Sensor.ReadRawValue(1, 0)   ' first; for the N-th use 0 + N - 1
Sensor.SetMode(1, 4)
stick = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Ids 0x10~0x13 are consecutive: one `mbcReadMany` call returns them from the same 10 ms snapshot.

```cpp
int16_t stick[4];
mbcReadMany(0x42, 0x10, 4, stick);   // stick axis 1~4：-100~+100
for (uint8_t i = 0; i < 4; i++) { Serial.print(stick[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns stick axis 1~4（-100~+100）。Host sends `[10]` → reply `[lo][hi][10 ^ lo ^ hi]`, value = int16 little-endian; channel N uses id 0x10 + (N − 1).

<pre class="blocks">(Controller port [1 v] stick [axis] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B~Y; big byte 4~7 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// A. stock 0x22 blocks：B~Y = stick axis 1~4（B C M Y = LX LY RX RY，0~99，50 = 中間）
int raw = MiniR4.I2C1.MXColor.getColor(B);   // group 1
```

**B. big channel (LauMo library)**

```cpp
// B. big channel byte 4~7：stick axis 1~4
int stick[4];
for (uint8_t i = 0; i < 4; i++) stick[i] = bigU8(4 + i);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC stick [axis] :: #E8711E)</pre>

```typescript
let stick = MBC.readValues(0x42, 0x10, 4)   // stick axis 1~4
```

</details>

</section>

## Write commands

Only Arduino I2C, mBlock, micro:bit (and MATRIX big-channel F1 / F2) can send commands; the SPIKE / EV3 ports are read-only.

<section id="ce5" class="ref-item ref-cmd">

### `E5` rumble <small>rumble</small>

| Item | Detail |
| :--- | :--- |
| Action | [strength 0~255], 0 = stop; 200 ms per call (ignored by PS2) |
| Parameters | strength `0~255`（1 byte） |
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
mbcWrite(0x42, 0xE5, 128);   // rumble（strength 0~255）
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E5][params…]` → reply `[00][00][E5]` (ack), then it runs.

<pre class="blocks">Controller port [1 v] rumble (128) :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available on MATRIX (the big channel only carries the F1 / F2 actions).

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC rumble [strength] :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE5, [50])   // rumble
```

</details>

</section>

<section id="ce6" class="ref-item ref-cmd">

### `E6` light bar colour <small>lightbar</small>

| Item | Detail |
| :--- | :--- |
| Action | [R][G][B] each 0~255, exactly 3 bytes; remembered until connected; default blue (0,0,255) (ignored by PS2) |
| Parameters | R `0~255`（1 byte）, G `0~255`（1 byte）, B `0~255`（1 byte） |
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
mbcWrite(0x42, 0xE6, 128, 128, 128);   // light bar colour（R 0~255, G 0~255, B 0~255）
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Host sends `[E6][params…]` → reply `[00][00][E6]` (ack), then it runs.

<pre class="blocks">Controller port [1 v] light bar colour (128) (128) (128) :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available on MATRIX (the big channel only carries the F1 / F2 actions).

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC light bar R[r] G[g] B[b] :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE6, [50, 50, 50])   // light bar colour
```

</details>

</section>

<section id="flags" class="ref-item">

## Status flags F3

One status per bit of `F3`; read directly over I2C, mBlock, micro:bit; MATRIX big-channel byte 24.

| bit | Name | Meaning |
| :--- | :--- | :--- |
| b0 | ready (`ready`) | always 1 |
| b1 | gamepad connected (`connected`) | same as value 06 (linkActive) |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE has no F3 flags; mode 8 returns ID / version. The official app cannot read mode 8.

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

id_word, proto, fw, seq = dev.read(8)   # mode 8: [ID | hw0<<8, protocol 2, major×100+minor, counter]
addr = id_word & 0xFF   # 0x42
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
addr = id_word & 0xFF             # 0x42
hw0 = id_word >> 8                # 2
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x42, 0xF3);
bool ready = st & (1 << 0);   // ready
bool connected = st & (1 << 1);   // gamepad connected
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

The extension splits status flags F3 into boolean blocks (ready, calibrating, …).

<pre class="blocks"><Controller port [1 v] connected? :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

```cpp
uint8_t st = MiniR4.I2C1.MXColor.getColorID();   // big channel byte 24 = F3 status flags
bool ready = st & 1;
uint8_t id  = MiniR4.I2C1.MXColor.getColor(Y);    // byte 27 device ID 0x42
uint8_t hw0 = MiniR4.I2C1.MXColor.getColor(K);    // byte 28 hw[0] = 2
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC device ready :: #E8711E></pre>

```typescript
let st = MBC.status(0x42)   // F3
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
| [`F3`](/en/i2c-protocol.md#rf3) | status flags | b0 = ready; other bits per product (see flags); same byte as MATRIX big-channel byte 24 |
| [`F4`](/en/i2c-protocol.md#rf4) | update counter | +1 every 10 ms snapshot, wraps 0~255 (2.56 s per cycle, not usable for reboot detection; frozen = module busy) |
| [`F5`](/en/i2c-protocol.md#rf5) | uptime seconds | seconds since boot, saturates at 32767 (~9 h); a drop means the module rebooted |
| [`F8`](/en/i2c-protocol.md#rf8) | change I2C address | [F8][new 0x08~0x77][~new], saved to EEPROM, takes effect after reboot; [F8][0x00][0xFF] = back to factory address |
| [`EF`](/en/i2c-protocol.md#ref) | reboot | [EF][0xA5]; mBlock sends ACK first, then reboots |

## Sources and assumptions

Generated by `_build/gen-reference.mjs` from `_build/protocol/ps4.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- 值表、命令、旗標與 PS2 完全相同（mBlock 定案「兩款相同」＋I2C 定案 §3／§4）；只有 hw0 = 2、model = PS4、E5／E6 有作用。
- D3：SPIKE 模式 0 教育版 byte 6~7、模式 6 v[2]、模式 9 v[6] 的 button16 都是 PSX 排列（#65 已把 PS4 改成 PSX，盤點檔寫的 Bluepad32 排列已過時）。
- 0x22 命令 4 同時裝方向鍵（低 nibble）與形狀鍵（高 nibble）、命令 5 裝肩鍵（低）與功能鍵（高）；JSON 只能把一個命令掛在一個值上，所以掛在 02／04，03／05 的 ch22 = null（內容寫在 02／04 的 note）。SPIKE 模式 0／1／2／9 的 nibble 也用同樣方式在 note 說明。
- 搖桿：I2C／mBlock 送 ±100；EV3／0x22／大通道送 0~99（50 = 中間）、SPIKE 送 1~9（5 = 中間），不是線性 offset，主機端各自換算（網站範例已是這樣）。
- EV3 模式 3 = 方向鍵 | 形狀鍵<<4 | 已連線<<8（#67 新增 bit8）；模式 4 內容不變（[方向鍵, LX, 形狀鍵, LY]）；模式 5 改送裝置 ID 格式 B [0x42 | 2<<8, 韌體版]。
- SPIKE 模式 3 改 [已連線, 電量, 0]（3×int8）、模式 8 改 [0x42 | 2<<8, 協議版 2, 韌體版, 更新序號]；其餘空模式 1、2、4、6 維持現狀。
- 大通道尾段：byte 24 = F3、25~26 = 0（無 F1／F2 動作）、27 = 0x42、28 = 2；F1~F4 收到不做任何動作。
- ESP32 從機不能時脈延展：I2C v2 在 onReceive 就預載從該編號起 32 B；0x22／大通道照 MATRIX 定案 §5 #2 預載最新一包，待上機。
- I2C 0A 系統鍵 b1 = 擷取鍵（v1 0x15 放在 b3，v2 改 b1，照 I2C 定案 §4）。
