<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/ev3/pybricks -->
# Controller PS4/PS5 × EV3 × Pybricks

This page covers **Pybricks v4 / EV3 MicroPython v2** only: how to set it up for the Controller PS4/PS5 on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-pybricks.en.md + _build/guides/product/ps4/ev3-pybricks.en.md {"product":"Controller PS4/PS5","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller PS4/PS5: Pybricks v4 / EV3 MicroPython v2](/en/sensors/ps4/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes port 1 and reads raw modes with `LUMPDevice`; EV3 MicroPython v2 works the same way.

<section id="v02" class="ref-item">

### D-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="D-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | up 1, right 2, down 4, left 8, added when pressed together (0~15) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

dpad = sensor.read(0)[0]   # D-pad：0~15 bit
dpad = sensor.read(1)[0]   # D-pad：0~15 bit
dpad = sensor.read(3)[0]   # D-pad：0~15 bit
dpad = sensor.read(4)[0]   # D-pad：0~15 bit
```

<p class="ref-note">mode 0 · int8 · IR-PROX 1×int8; mode 1 · int8 · IR-SEEK [方向鍵, LX, 形狀鍵, LY, 肩鍵, RX, 功能鍵, RY]; mode 3 · int16 · 低 nibble；高 nibble = 形狀鍵；bit8 = 已連線（+256，#67 新增）; mode 4 · int8 · IR-S-ALT [方向鍵, LX, 形狀鍵, LY] (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | triangle 1, circle 2, cross 4, square 8, added when pressed together (0~15) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

shapeKeys = sensor.read(1)[2]   # shape buttons：0~15 bit
shapeKeys = sensor.read(3)[0]   # shape buttons：0~15 bit
shapeKeys = sensor.read(4)[2]   # shape buttons：0~15 bit
```

<p class="ref-note">mode 1 · int8; mode 3 · int16 · 高 nibble（÷16 取整數再 mod 16）; mode 4 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8, added when pressed together (0~15) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

shoulderKeys = sensor.read(1)[4]   # shoulder buttons：0~15 bit
```

<p class="ref-note">mode 1 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1, L3 2, R3 4, START(OPTIONS) 8, added when pressed together (0~15) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

funcKeys = sensor.read(1)[6]   # function buttons：0~15 bit
```

<p class="ref-note">mode 1 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected and data seen within 300 ms, 0 = not connected (buttons 0, sticks centred while disconnected) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

connected = sensor.read(3)[0]   # connected：0~1
```

<p class="ref-note">mode 3 · int16 · bit8（值 ≥ 256 = 已連線，#67 新增）</p>

</section>

<section id="v10" class="ref-item">

### stick axis 1~4 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="stick axis 1~4" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right/up positive; 0 when not connected |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

stick = sensor.read(1)[1]   # stick axis 1~4：-100~+100
stick = sensor.read(2)[0:4]   # stick axis 1~4
stick = sensor.read(4)[1]   # stick axis 1~4：-100~+100
```

<p class="ref-note">mode 1 · int8 · LX；LY 在 index 3、RX 5、RY 7（與按鍵交錯）；0~99，50 = 中間（未連線 50）; mode 2 · int8 · IR-REMOTE [LX, LY, RX, RY] 0~99，50 = 中間; mode 4 · int8 · LX；LY 在 index 3（RX、RY 不在模式 4） (locked mode: firmware and hosts will not change it)</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：16 buttons, battery, L2 analog, R2 analog, system buttons, gamepad type. Use <a href="/en/sensors/ps4/arduino/ide.md">Arduino</a> / <a href="/en/sensors/ps4/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/ps4/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

The EV3 port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/ps4/ev3-examples.md host=ev3 sw=pybricks -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps4.json` (single source of truth)
- Setup：`_build/guides/software/ev3-pybricks.en.md`、`_build/guides/product/ps4/ev3-pybricks.en.md`
- Examples：`sensors/ps4/ev3-examples.md`
