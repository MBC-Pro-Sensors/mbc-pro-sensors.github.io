<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/ev3/pybricks -->
# Pathfinder 16-Way (Line16) × EV3 × Pybricks

This page covers **Pybricks v4 / EV3 MicroPython v2** only: how to set it up for the Pathfinder 16-Way (Line16) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-pybricks.en.md + _build/guides/product/line16/ev3-pybricks.en.md {"product":"Pathfinder 16-Way (Line16)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 16-Way (Line16): Pybricks v4 / EV3 MicroPython v2](/en/sensors/line16/ev3-pybricks.md).
<!-- /include-fallback -->

## Reading values

Code assumes port 1 and reads raw modes with `LUMPDevice`; EV3 MicroPython v2 works the same way.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-16~+16` | which channel the line is under: −16 (far left) to +16 (far right), 0 = centre; seen from behind the robot, negative = left (normally ±15; ±16 only for 200 ms after the line leaves the outermost channel) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

linePos = sensor.read(0)[0]   # line position：-16~+16
```

<p class="ref-note">mode 0 · int8 · IR-PROX（F16:739-741） (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

linePosFine = sensor.read(2)[0]   # fine position：-100~+100
linePosFine = sensor.read(4)[0]   # fine position：-100~+100
```

<p class="ref-note">mode 2 · int8 · IR-REMOTE byte0 = highRes−100 存成 int8（F16:745-751）; mode 4 · int8 · #67 新：IR-S-ALT 內容同模式 2（原本不送） (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~16` 路 | number of channels on the line, 0~16; 0 = no line |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

lineWidth = sensor.read(2)[1]   # line width：0~16 路
lineWidth = sensor.read(4)[1]   # line width：0~16 路
```

<p class="ref-note">mode 2 · int8; mode 4 · int8 · #67 新：同模式 2 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | bit0 = channel 1 (left) … bit15 = channel 16; 1 = that channel sees the line |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

bitmap = sensor.read(3)[0]   # line bitmap：0~65535 bit
```

<p class="ref-note">mode 3 · int16 · #67 新：IR-REM-A 1×int16 = 16 bit 位元圖（原本不送）</p>

</section>

<section id="v10" class="ref-item">

### channel 1~16 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="channel 1~16 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~16, 0~100 (channel 1 = left); ids 10~1F |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

light = sensor.read(1)[0:8]   # channel 1~16 reflect
light = sensor.read(2)[2:4]   # channel 1~16 reflect
light = sensor.read(4)[2:4]   # channel 1~16 reflect
```

<p class="ref-note">mode 1 · int8 · IR-SEEK 8×int8 兩路壓一 byte：byte i = convert9(第 2i+1 路)×10 + convert9(第 2i+2 路)（11~99；convert9：0~11→1、12~22→2 … 89~100→9，F16:716-723）; mode 2 · int8 · index 2 = 第 1 路、index 3 = 第 16 路（0~100）; mode 4 · int8 · #67 新：同模式 2 (locked mode: firmware and hosts will not change it)</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：line segments, last exit direction, threshold, calibrations done. Use <a href="/en/sensors/line16/arduino/ide.md">Arduino</a> / <a href="/en/sensors/line16/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/line16/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

The EV3 port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/line16/ev3-examples.md host=ev3 sw=pybricks -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [line16_pybricks_ev3.zip](/examples/line16/line16_pybricks_ev3.zip)
- [MBC_line16_EV3_Lib.py](/examples/line16/ev3/MBC_line16_EV3_Lib.py)
- [line16_ev3_block_with_lib.py](/examples/line16/ev3/line16_ev3_block_with_lib.py)
- [line16_ev3_python_with_lib.py](/examples/line16/ev3/line16_ev3_python_with_lib.py)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line16.json` (single source of truth)
- Setup：`_build/guides/software/ev3-pybricks.en.md`、`_build/guides/product/line16/ev3-pybricks.en.md`
- Examples：`sensors/line16/ev3-examples.md`
