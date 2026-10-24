<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/spike/pybricks -->
# Controller PS4/PS5 × SPIKE × Pybricks

This page covers **Pybricks (Python)** only: how to set it up for the Controller PS4/PS5 on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-pybricks.en.md + _build/guides/product/ps4/spike-pybricks.en.md {"product":"Controller PS4/PS5","edition":"SPIKE edition","port":"C","lib":"MBC_gamepad_Lib.py","libUrl":"/examples/ps4/spike_beginner/pybricks/MBC_gamepad_Lib.py","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller PS4/PS5: Pybricks (Python)](/en/sensors/ps4/spike-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes the sensor is on port A. For the MBC library form, import the library .py into Pybricks too.

<section id="v01" class="ref-item">

### 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | all buttons in one 16-bit word (PSX layout): bit0 SELECT(SHARE), 1 L3, 2 R3, 3 START(OPTIONS), 4 up, 5 right, 6 down, 7 left, 8 L2, 9 R2, 10 L1, 11 R1, 12 triangle, 13 circle, 14 cross, 15 square; 1 = pressed; 0 when not connected |

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

<p class="ref-note">mode 0 (edu) byte 6 🔒; mode 6 v[2]; mode 9 v[6] 🔒. Note: PSX 排列（#65 已改，原本是 Bluepad32 排列）; PSX 排列; PSX 排列（D3，PS2／PS4 相同；函式庫 buttons_psx()）</p>

</section>

<section id="v02" class="ref-item">

### D-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="D-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | up 1, right 2, down 4, left 8, added when pressed together (0~15) |

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

<p class="ref-note">mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 6 v[0] lo; mode 7 v[0] lo 🔒; mode 9 v[0] lo 🔒. Note: 低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | triangle 1, circle 2, cross 4, square 8, added when pressed together (0~15) |

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

<p class="ref-note">mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 6 v[0] hi; mode 7 v[0] hi 🔒; mode 9 v[0] lo 🔒. Note: 高 nibble; 高 nibble; 高 nibble; 高 nibble</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8, added when pressed together (0~15) |

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

<p class="ref-note">mode 0 (edu) byte 0 🔒; mode 2 byte 0; mode 6 v[1] lo; mode 7 v[1] lo 🔒; mode 9 v[1] lo 🔒. Note: 低 nibble；高 nibble = 功能鍵; 低 nibble；高 nibble = 功能鍵; 低 nibble；高 nibble = 功能鍵</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1, L3 2, R3 4, START(OPTIONS) 8, added when pressed together (0~15) |

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

<p class="ref-note">mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 1 🔒; mode 2 byte 0; mode 6 v[1] hi; mode 7 v[1] hi 🔒; mode 9 v[1] lo 🔒. Note: 高 nibble; 單鍵編號 1~8（掃 功能鍵<<4|肩鍵 的 bit0→7：1 L2 2 R2 3 L1 4 R1 5 SELECT 6 L3 7 R3 8 START）; 高 nibble; 高 nibble</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected and data seen within 300 ms, 0 = not connected (buttons 0, sticks centred while disconnected) |

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

<p class="ref-note">mode 3 byte 0. Note: #67 改：模式 3 = [已連線, 電量, 0]（3×int8）</p>

</section>

<section id="v07" class="ref-item">

### battery <small>battery</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-battery.svg" alt="battery" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` % | gamepad battery 0~100 (always 0 on the PS2 receiver) |

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

<p class="ref-note">mode 3 byte 1</p>

</section>

<section id="v10" class="ref-item">

### stick axis 1~4 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="stick axis 1~4" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right/up positive; 0 when not connected |

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

<p class="ref-note">mode 0 (edu) byte 2 🔒; mode 0 (toy) byte 2 🔒; mode 4 byte 0; mode 7 v[2] 🔒; mode 9 v[2] 🔒. Note: LX×10+LY（各 1~9，5 = 中間）；byte 4~5 = RX×10+RY; LX×10+LY 查表編碼；byte 4~5 = RX×10+RY；byte 6~7 = 肩鍵 byte 查表; [LX, LY, RX, RY] 0~99（宣告 2×int16，送 4 B）; LX×1000+LY×100+RX×10+RY（各 1~9）; v[2..5] = LX LY RX RY，1~9，5 = 中間</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：L2 analog, R2 analog, system buttons, gamepad type. Use <a href="/en/sensors/ps4/arduino/ide.md">Arduino</a> / <a href="/en/sensors/ps4/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/ps4/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

The SPIKE port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/ps4/spike-examples.md host=spike sw=pybricks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Pybricks library MBC_gamepad_Lib.py](/examples/ps4/pybricks/MBC_gamepad_Lib.py)
- [ps4_pybricks_spike_v3.6.1.zip](/examples/ps4/ps4_pybricks_spike_v3.6.1.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps4.json` (single source of truth)
- Setup：`_build/guides/software/spike-pybricks.en.md`、`_build/guides/product/ps4/spike-pybricks.en.md`
- Examples：`sensors/ps4/spike-examples.md`
