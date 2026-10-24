<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/spike/pybricks -->
# Controller 2.4G (PS2) × SPIKE × Pybricks

This page covers **Pybricks (Python)** only: how to set it up for the Controller 2.4G (PS2) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-pybricks.en.md + _build/guides/product/ps2/spike-pybricks.en.md {"product":"Controller 2.4G (PS2)","edition":"SPIKE edition","port":"C","lib":"MBC_gamepad_Lib.py","libUrl":"/examples/ps2/spike_beginner/pybricks/MBC_gamepad_Lib.py","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): Pybricks (Python)](/en/sensors/ps2/spike-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes the sensor is on port A. For the MBC library form, import the library .py into Pybricks too.

<section id="v01" class="ref-item">

### 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |

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

<p class="ref-note">mode 0 (edu) byte 6 🔒; mode 5 v[0] 🔒; mode 6 v[2]; mode 9 v[6] 🔒. Note: App 原始藍 = button16（.ino:201-211）; Pybricks reflection() 編碼：11 鍵 complexXY = SELECT<<10 | R1<<9 | L1<<8 | ■<<7 | ✕<<6 | ●<<5 | ▲<<4 | 左<<3 | 下<<2 | 右<<1 | 上，a = /25、b = (餘+1)×10−1，3 個 int16 = a×256+b → reflection() = complexXY；L2、R2、L3、R3、START 讀不到（.ino:334-358）; v2 = button16（PSX）; v6 = button16（PSX）</p>

</section>

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

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

<p class="ref-note">mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 6 v[0] lo; mode 9 v[0] 🔒. Note: byte1 = 方向 | 形狀<<4（App 反射光）：低 4 bit; byte0 = 方向 | 形狀<<4; 低 4 bit（高 4 bit = 形狀）; v0 = 方向 | 形狀<<4（bit0-3）</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |

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

<p class="ref-note">mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 6 v[0] hi; mode 9 v[0] 🔒. Note: 高 4 bit; 高 4 bit; 高 4 bit; v0 bit4-7</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

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

<p class="ref-note">mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 6 🔒; mode 2 byte 0; mode 6 v[1] lo; mode 9 v[1] 🔒. Note: byte0 = 肩 | 功能<<4（App 顏色，START = −128）：低 4 bit; byte6~7 = (肩 | 功能<<4) 經玩具版查表編碼；byte1 = 單鍵編號（L2=1 R2=2 L1=3 R1=4 SELECT=5 L3=6 R3=7 START=8，沒按 0）; 低 4 bit（高 4 bit = 功能）; v1 = 肩 | 功能<<4（bit0-3）</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |

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

<p class="ref-note">mode 0 (edu) byte 0 🔒; mode 2 byte 0; mode 6 v[1] hi; mode 9 v[1] 🔒. Note: 高 4 bit; 高 4 bit; v1 bit4-7</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |

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

<p class="ref-note">mode 3 byte 0. Note: #67 改：LIGHT 3×int8 = [已連線, 電量, 0]（原本 = 4 組按鍵）</p>

</section>

<section id="v07" class="ref-item">

### battery <small>battery</small>

| Range | Meaning |
| :--- | :--- |
| `0` % | 0~100; PS2 pads report no battery → always 0 (PS4 only) |

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

<p class="ref-note">mode 3 byte 1. Note: #67 改：PS2 送 0</p>

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

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

<p class="ref-note">mode 0 (edu) byte 2 🔒; mode 0 (toy) byte 2 🔒; mode 4 v[0]; mode 7 v[2] 🔒; mode 8 v[0]; mode 9 v[2] 🔒. Note: byte2~3 = LX×10+LY、byte4~5 = RX×10+RY（11~99，中心 55；App 原始紅／綠）; LX×10+LY、RX×10+RY 經玩具版查表編碼成 byte2~3、byte4~5; v0 = LX | LY<<8、v1 = RX | RY<<8; Pybricks ambient() 用：v2 = LX×1000 + LY×100 + RX×10 + RY（1111~9999，中心 5555）；v0、v1 同模式 6; 只送 4 B：v0 = LX | LY<<8、v1 = RX | RY<<8; v2~v5 = LX、LY、RX、RY（0~99 ÷10，0 補 1）</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：L2 analog, R2 analog, system keys, gamepad type. Use <a href="/en/sensors/ps2/arduino/ide.md">Arduino</a> / <a href="/en/sensors/ps2/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/ps2/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

The SPIKE port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/ps2/spike-examples.md host=spike sw=pybricks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Pybricks library MBC_gamepad_Lib.py](/examples/ps2/pybricks/MBC_gamepad_Lib.py)
- [Shared library (for new projects, one file for every MBC sensor) MBC_sensors.py](/examples/ps2/pybricks/MBC_sensors.py)
- [ps2_pybricks_spike_v4.x.zip](/examples/ps2/ps2_pybricks_spike_v4.x.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/spike-pybricks.en.md`、`_build/guides/product/ps2/spike-pybricks.en.md`
- Examples：`sensors/ps2/spike-examples.md`
