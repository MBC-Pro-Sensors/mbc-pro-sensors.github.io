<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps2.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /en/sensors/ps2/index.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/ps2/index.md">Controller 2.4G</a></p>

# PS2 Gamepad Receiver: value & command reference

[🔙 Back to PS2 Gamepad Receiver](/en/sensors/ps2/index.md)

Every value and command the PS2 Gamepad Receiver reports to each host: what it means, its range, and how to read it on SPIKE / EV3 / Arduino I2C / mBlock / MATRIX / micro:bit. Append `?host=spike` (or ev3, arduino, mblock, matrix, microbit) to the URL to expand that host by default.

<p class="chips"><a class="chip chip-spike" href="/en/sensors/ps2/index.md">SPIKE</a><a class="chip chip-ev3" href="/en/sensors/ps2/index.md">EV3</a><a class="chip chip-arduino" href="/en/sensors/ps2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/en/sensors/ps2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/en/sensors/ps2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/en/sensors/ps2/microbit.md">micro:bit</a></p>

| Item | Detail |
| :--- | :--- |
| Factory I2C address | `0x42` |
| hw[0] | `1`（F0 high byte; ID high byte in SPIKE mode 8 / EV3 mode 5; big-channel byte 28） |
| Model | `PS2` |
| Protocol version | `2` |
| Hosts | SPIKE, EV3, Arduino I2C, mBlock, MATRIX, micro:bit |

## Values at a glance

| Id | Value | Range | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [16 buttons](#v01) | 0~65535 bit | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [d-pad](#v02) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [shape buttons](#v03) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [shoulder buttons](#v04) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`05`](#v05) | [function buttons](#v05) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`06`](#v06) | [connected](#v06) | 0~1 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`07`](#v07) | [battery](#v07) | 0 % | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| [`08`](#v08) | [L2 analog](#v08) | 0~255 | — | — | ✓ | ✓ | — | ✓ |
| [`09`](#v09) | [R2 analog](#v09) | 0~255 | — | — | ✓ | ✓ | — | ✓ |
| [`0A`](#v0a) | [system keys](#v0a) | 0 bit | — | — | ✓ | ✓ | — | ✓ |
| [`0B`](#v0b) | [gamepad type](#v0b) | 0~2 | — | — | ✓ | ✓ | — | ✓ |
| [`10~13`](#v10) | [stick LX / LY / RX / RY](#v10) | -100~+100 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

✓ = available on that host; — = not sent.

## Reading each value

<section id="v01" class="ref-item">

### `01` 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |
| Range | `0~65535` bit |
| I2C / mBlock id | `01` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 6 🔒; mode 5 v[0] 🔒; mode 6 v[2]; mode 9 v[6] 🔒. Note: App 原始藍 = button16（.ino:201-211）; Pybricks reflection() 編碼：11 鍵 complexXY = SELECT<<10 | R1<<9 | L1<<8 | ■<<7 | ✕<<6 | ●<<5 | ▲<<4 | 左<<3 | 下<<2 | 右<<1 | 上，a = /25、b = (餘+1)×10−1，3 個 int16 = a×256+b → reflection() = complexXY；L2、R2、L3、R3、START 讀不到（.ino:334-358）; v2 = button16（PSX）; v6 = button16（PSX）

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
// B. big channel byte 8~9：16 buttons（#67 新增（[8..9] 小端））
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

### `02` d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |
| Range | `0~15` bit |
| I2C / mBlock id | `02` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 6 v[0] lo; mode 9 v[0] 🔒. Note: byte1 = 方向 | 形狀<<4（App 反射光）：低 4 bit; byte0 = 方向 | 形狀<<4; 低 4 bit（高 4 bit = 形狀）; v0 = 方向 | 形狀<<4（bit0-3）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

dpad = dev.read(9)[0]   # d-pad
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

mode 0 · int8 · IR-PROX = 方向鍵（.ino:130-132）; mode 1 · int8 · IR-SEEK [方向, LX, 形狀, LY, 肩, RX, 功能, RY]（按鍵與搖桿交錯）; mode 3 · int16 · #67 改：IR-REM-A 1×int16 = 方向 | 形狀<<4 | 已連線<<8（bit0-3 = 方向）; mode 4 · int8 · IR-S-ALT [方向, LX, 形狀, LY] (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

dpad = sensor.read(0)[0]   # d-pad：0~15 bit
dpad = sensor.read(1)[0]   # d-pad：0~15 bit
dpad = sensor.read(3)[0]   # d-pad：0~15 bit
dpad = sensor.read(4)[0]   # d-pad：0~15 bit
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
int16_t dpad = mbcRead(0x42, 0x02);   // d-pad：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns d-pad（0~15 bit）。Host sends `[02]` → reply `[lo][hi][02 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] d-pad :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R; big byte 0 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// A. stock 0x22 blocks：R = d-pad（#67 新增：R = 方向鍵 | 形狀鍵<<4（上 1、右 2、下 4、左 8、▲16、●32、✕64、■128））
int dpad = MiniR4.I2C1.MXColor.getColor(R);
```

**B. big channel (LauMo library)**

```cpp
// B. big channel byte 0：d-pad（#67 新增）
int dpad = bigU8(0);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC d-pad :: #E8711E)</pre>

```typescript
let dpad = MBC.readValue(0x42, 0x02)   // d-pad：0~15 bit
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |
| Range | `0~15` bit |
| I2C / mBlock id | `03` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 6 v[0] hi; mode 9 v[0] 🔒. Note: 高 4 bit; 高 4 bit; 高 4 bit; v0 bit4-7

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

shape = dev.read(9)[0]   # shape buttons
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
shape = pad.pressed('TRIANGLE')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8; mode 3 · int16 · #67 改：bit4-7 = 形狀（同一個 int16）; mode 4 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

shape = sensor.read(1)[2]   # shape buttons：0~15 bit
shape = sensor.read(3)[0]   # shape buttons：0~15 bit
shape = sensor.read(4)[2]   # shape buttons：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shape = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 3)
shape = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
shape = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t shape = mbcRead(0x42, 0x03);   // shape buttons：0~15 bit
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
// B. big channel byte 1：shape buttons（#67 新增）
int shape = bigU8(1);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC shape buttons :: #E8711E)</pre>

```typescript
let shape = MBC.readValue(0x42, 0x03)   // shape buttons：0~15 bit
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |
| Range | `0~15` bit |
| I2C / mBlock id | `04` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 6 🔒; mode 2 byte 0; mode 6 v[1] lo; mode 9 v[1] 🔒. Note: byte0 = 肩 | 功能<<4（App 顏色，START = −128）：低 4 bit; byte6~7 = (肩 | 功能<<4) 經玩具版查表編碼；byte1 = 單鍵編號（L2=1 R2=2 L1=3 R1=4 SELECT=5 L3=6 R3=7 START=8，沒按 0）; 低 4 bit（高 4 bit = 功能）; v1 = 肩 | 功能<<4（bit0-3）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

shoulder = dev.read(9)[1]   # shoulder buttons
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
shoulder = pad.pressed('L1')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

shoulder = sensor.read(1)[4]   # shoulder buttons：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shoulder = Sensor.ReadRawValue(1, 4)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t shoulder = mbcRead(0x42, 0x04);   // shoulder buttons：0~15 bit
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
// A. stock 0x22 blocks：G = shoulder buttons（#67 新增：G = 肩鍵 | 功能鍵<<4（L2 1、R2 2、L1 4、R1 8、SELECT 16、L3 32、R3 64、START 128））
int shoulder = MiniR4.I2C1.MXColor.getColor(G);
```

**B. big channel (LauMo library)**

```cpp
// B. big channel byte 2：shoulder buttons（#67 新增）
int shoulder = bigU8(2);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC shoulder buttons :: #E8711E)</pre>

```typescript
let shoulder = MBC.readValue(0x42, 0x04)   // shoulder buttons：0~15 bit
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |
| Range | `0~15` bit |
| I2C / mBlock id | `05` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 0 🔒; mode 2 byte 0; mode 6 v[1] hi; mode 9 v[1] 🔒. Note: 高 4 bit; 高 4 bit; v1 bit4-7

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

func = dev.read(9)[1]   # function buttons
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # refresh once per loop
func = pad.pressed('START')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

mode 1 · int8 (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

func = sensor.read(1)[6]   # function buttons：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
func = Sensor.ReadRawValue(1, 6)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Returns `MBC_NONE` (−32768) when the module does not answer.

```cpp
int16_t func = mbcRead(0x42, 0x05);   // function buttons：0~15 bit
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
// B. big channel byte 3：function buttons（#67 新增）
int func = bigU8(3);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC function buttons :: #E8711E)</pre>

```typescript
let func = MBC.readValue(0x42, 0x05)   // function buttons：0~15 bit
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` connected <small>connected</small>

| Item | Detail |
| :--- | :--- |
| Meaning | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |
| Range | `0~1` |
| I2C / mBlock id | `06` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 3 byte 0. Note: #67 改：LIGHT 3×int8 = [已連線, 電量, 0]（原本 = 4 組按鍵）

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

mode 3 · int16 · #67 改：bit8 = 已連線（+256）

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
// A. stock 0x22 blocks：K = connected（#67 新增：K = 已連線 1／0）
int connected = MiniR4.I2C1.MXColor.getColor(K);
```

**B. big channel (LauMo library)**

```cpp
// B. big channel byte 10：connected（#67 新增）
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

| Item | Detail |
| :--- | :--- |
| Meaning | 0~100; PS2 pads report no battery → always 0 (PS4 only) |
| Range | `0` % |
| I2C / mBlock id | `07` |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 3 byte 1. Note: #67 改：PS2 送 0

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
int16_t battery = mbcRead(0x42, 0x07);   // battery：0 %
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns battery（0 %）。Host sends `[07]` → reply `[lo][hi][07 ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] battery :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

no 0x22; big byte 11 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// B. big channel byte 11：battery（#67 新增；PS2 = 0）
int battery = bigU8(11);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC battery :: #E8711E)</pre>

```typescript
let battery = MBC.readValue(0x42, 0x07)   // battery：0 %
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` L2 analog <small>l2Analog</small>

| Item | Detail |
| :--- | :--- |
| Meaning | L2 pressure 0~255 (only DualShock 2 in analog mode reports pressure; otherwise pressed = 255, released = 0); 0 when not connected |
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

| Item | Detail |
| :--- | :--- |
| Meaning | R2 pressure 0~255 (same rule as 08); 0 when not connected |
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

### `0A` system keys <small>sysKeys</small>

| Item | Detail |
| :--- | :--- |
| Meaning | b0 PS button, b1 capture button; PS2 pads have none → always 0 (PS4 only) |
| Range | `0` bit |
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
int16_t sysKeys = mbcRead(0x42, 0x0A);   // system keys：0 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns system keys（0 bit）。Host sends `[0A]` → reply `[lo][hi][0A ^ lo ^ hi]`, value = int16 little-endian.

<pre class="blocks">(Controller port [1 v] system keys :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

Not available over MATRIX 0x22 or the big channel.

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC system keys :: #E8711E)</pre>

```typescript
let sysKeys = MBC.readValue(0x42, 0x0A)   // system keys：0 bit
```

</details>

</section>

<section id="v0b" class="ref-item">

### `0B` gamepad type <small>padType</small>

| Item | Detail |
| :--- | :--- |
| Meaning | 0 none, 1 PS2 digital mode, 2 PS2 analog mode (3 PS4 / 4 PS5 only appear on the PS4 receiver) |
| Range | `0~2` |
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
int16_t padType = mbcRead(0x42, 0x0B);   // gamepad type：0~2
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns gamepad type（0~2）。Host sends `[0B]` → reply `[lo][hi][0B ^ lo ^ hi]`, value = int16 little-endian.

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
let padType = MBC.readValue(0x42, 0x0B)   // gamepad type：0~2
```

</details>

</section>

<section id="v10" class="ref-item">

### `10~13` stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Item | Detail |
| :--- | :--- |
| Meaning | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |
| Range | `-100~+100` |
| I2C / mBlock id | `10~13` |
| Locked | 🔒 LEGO mode layout will not change |

**How to read it**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

mode 0 (edu) byte 2 🔒; mode 0 (toy) byte 2 🔒; mode 4 v[0]; mode 7 v[2] 🔒; mode 8 v[0]; mode 9 v[2] 🔒. Note: byte2~3 = LX×10+LY、byte4~5 = RX×10+RY（11~99，中心 55；App 原始紅／綠）; LX×10+LY、RX×10+RY 經玩具版查表編碼成 byte2~3、byte4~5; v0 = LX | LY<<8、v1 = RX | RY<<8; Pybricks ambient() 用：v2 = LX×1000 + LY×100 + RX×10 + RY（1111~9999，中心 5555）；v0、v1 同模式 6; 只送 4 B：v0 = LX | LY<<8、v1 = RX | RY<<8; v2~v5 = LX、LY、RX、RY（0~99 ÷10，0 補 1）

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

mode 1 · int8 · 交錯：index 1 = LX、3 = LY、5 = RX、7 = RY（偶數 index 是按鍵）; mode 2 · int8 · IR-REMOTE [LX, LY, RX, RY]（描述表宣告 0~10，實送 0~99）; mode 4 · int8 · index 1 = LX、3 = LY（只到左搖桿） (locked mode: firmware and hosts will not change it)

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

stick = sensor.read(1)[1:5]   # stick LX / LY / RX / RY
stick = sensor.read(2)[0:4]   # stick LX / LY / RX / RY
stick = sensor.read(4)[1:3]   # stick LX / LY / RX / RY
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
stick1 = Sensor.ReadRawValue(1, 1)   ' first; for the N-th use 1 + N - 1
Sensor.SetMode(1, 2)
stick1 = Sensor.ReadRawValue(1, 0)   ' first; for the N-th use 0 + N - 1
Sensor.SetMode(1, 4)
stick1 = Sensor.ReadRawValue(1, 1)   ' first; for the N-th use 1 + N - 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

Ids 0x10~0x13 are consecutive: one `mbcReadMany` call returns them from the same 10 ms snapshot.

```cpp
int16_t stick[4];
mbcReadMany(0x42, 0x10, 4, stick);   // stick LX / LY / RX / RY：-100~+100
for (uint8_t i = 0; i < 4; i++) { Serial.print(stick[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

Block returns stick LX / LY / RX / RY（-100~+100）。Host sends `[10]` → reply `[lo][hi][10 ^ lo ^ hi]`, value = int16 little-endian; channel N uses id 0x10 + (N − 1).

<pre class="blocks">(Controller port [1 v] stick [LX/LY/RX/RY] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B~Y; big byte 4~7 u8. A and B need different library files and cannot be mixed; `bigU8()` helpers are on this product's MATRIX page.

```cpp
// A. stock 0x22 blocks：B~Y = stick LX / LY / RX / RY（#67 新增：B、C、M、Y = LX、LY、RX、RY）
int raw = MiniR4.I2C1.MXColor.getColor(B);   // group 1
```

**B. big channel (LauMo library)**

```cpp
// B. big channel byte 4~7：stick LX / LY / RX / RY
int stick[4];
for (uint8_t i = 0; i < 4; i++) stick[i] = bigU8(4 + i);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

The extension reads 2 bytes as `Int16LE` and pauses 2 ms after writing the id.

<pre class="blocks">(MBC stick [LX/LY/RX/RY] :: #E8711E)</pre>

```typescript
let stick = MBC.readValues(0x42, 0x10, 4)   // stick LX / LY / RX / RY
```

</details>

</section>

## Write commands

Only Arduino I2C, mBlock, micro:bit (and MATRIX big-channel F1 / F2) can send commands; the SPIKE / EV3 ports are read-only.

<section id="ce5" class="ref-item ref-cmd">

### `E5` rumble <small>vibrate</small>

| Item | Detail |
| :--- | :--- |
| Action | [strength 0~255], 0 = stop; the PS2 receiver ACKs and ignores it (pad rumble disabled since 250614), effective on PS4 only |
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

<pre class="blocks">MBC rumble (50) :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE5, [50])   // rumble
```

</details>

</section>

<section id="ce6" class="ref-item ref-cmd">

### `E6` light bar colour <small>setLed</small>

| Item | Detail |
| :--- | :--- |
| Action | [R][G][B] 0~255 each; PS2 pads have no light bar, the receiver ACKs and ignores it, effective on PS4 only |
| Parameters | red `0~255`（1 byte）, green `0~255`（1 byte）, blue `0~255`（1 byte） |
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
mbcWrite(0x42, 0xE6, 128, 128, 128);   // light bar colour（red 0~255, green 0~255, blue 0~255）
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
| b1 | gamepad connected (`connected`) | same as value 06 |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE has no F3 flags; mode 8 returns ID / version. The official app cannot read mode 8.

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

id_word, proto, fw, seq = dev.read(8)   # mode 8: [ID | hw0<<8, protocol 2, major×100+minor, counter]
addr = id_word & 0xFF   # 0x42
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
addr = id_word & 0xFF             # 0x42
hw0 = id_word >> 8                # 1
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
uint8_t hw0 = MiniR4.I2C1.MXColor.getColor(K);    // byte 28 hw[0] = 1
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

Generated by `_build/gen-reference.mjs` from `_build/protocol/ps2.json` (single source of truth, transcribed from the 2026-10-21 protocol decisions).

- hw[0] = 1（PS2）、電量 07 恆 0、系統鍵 0A 恆 0、手把類型 0B 只會出現 0／1／2（I2C 定案 §2、§4）。
- 08／09 L2／R2 類比照定案寫 0~255：DS2 類比模式有壓力值；沒有壓力值時按下 = 255、放開 = 0（沿用 v1 0x14 的做法，盤點 .ino:598-603）。
- E5 震動、E6 燈條照 I2C 定案 §3 列在 PS2／PS4 共用命令表；PS2 收到回 ACK 後忽略（跟 v1 0x10／0x23 一樣）。
- 0x22 新增（MATRIX 定案 §6）：1 = 0x43、4 = 方向|形狀<<4、5 = 肩|功能<<4、6~9 = LX LY RX RY 0~99、10 = 已連線、11~12 = 0；大通道新增 [0..3] 四組鍵、[4..7] 搖桿 0~99、[8..9] button16、[10] 已連線、[11] 電量，尾段 [24] F3、[27] 0x42、[28] 1；F1~F4 無動作。ch22 命令 4／5 各掛在 02／04 底下（03／05 在 note 說明高 4 bit），避免同一命令重複登記。
- 搖桿 10~13 在 I2C／mBlock 是 ±100（由 raw (v−128)×100/128、Y 上為正）；EV3／大通道／0x22 維持 0~99（50 中間）、SPIKE 維持 1~9（5 中間）照盤點，不改鎖住模式。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改 方向|形狀<<4|已連線<<8（原本沒有 bit8）；模式 5 改成 ID 格式 B（原本 = 按鍵／搖桿合併版）。
- SPIKE：0/5/7/9 鎖住，1、2、4、6 維持不動；模式 3 改 [已連線, 電量(0), 0]（原本 = 4 組按鍵）；模式 8 改 v[0] = ID|hw[0]<<8（原本 = 搖桿）。模式 9 v[6] button16 = PSX 排列（D3）。
- SPIKE 模式 5 只容得下 11 鍵（L2、R2、L3、R3、START 讀不到），鎖住不改。
