<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × spike × mbcblocks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/spike/mbcblocks -->
# Pathfinder 16-Way (Line16) × SPIKE × MBC Blocks website

This page covers **MBC SPIKE Blocks website** only: how to set it up for the Pathfinder 16-Way (Line16) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-mbcblocks.en.md + _build/guides/product/line16/spike-mbcblocks.en.md {"product":"Pathfinder 16-Way (Line16)","edition":"SPIKE edition","port":"C","lib":"MBC_line16_obj_Lib.py","libUrl":"/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 16-Way (Line16): MBC SPIKE Blocks website](/en/sensors/line16/spike-examples.md).
<!-- /include-fallback -->

## Reading values

The hub needs Pybricks firmware. The MBC Blocks website reads mode 9 of the sensor (all values in one read).

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-16~+16` | which channel the line is under: −16 (far left) to +16 (far right), 0 = centre; seen from behind the robot, negative = left (normally ±15; ±16 only for 200 ms after the line leaves the outermost channel) |

Add this sensor's blocks on the MBC Blocks website and use the "line position" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
linePos = (w & 0xFF) - 16   # line position（lo byte）
```

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

Add this sensor's blocks on the MBC Blocks website and use the "fine position" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
linePosFine = (w & 0xFF) - 100   # fine position（lo byte）
```

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~16` 路 | number of channels on the line, 0~16; 0 = no line |

Add this sensor's blocks on the MBC Blocks website and use the "line width" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
lineWidth = ((w >> 8) & 0xFF)   # line width（hi byte）
```

</section>

<section id="v04" class="ref-item">

### line segments <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-groupCount.svg" alt="line segments" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |

Add this sensor's blocks on the MBC Blocks website and use the "line segments" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
groupCount = ((w >> 8) & 0xFF)   # line segments（hi byte）
```

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | bit0 = channel 1 (left) … bit15 = channel 16; 1 = that channel sees the line |

Add this sensor's blocks on the MBC Blocks website and use the "line bitmap" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

bitmap = dev.read(9)[6]   # line bitmap
```

</section>

<section id="v10" class="ref-item">

### channel 1~16 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="channel 1~16 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~16, 0~100 (channel 1 = left); ids 10~1F |

Add this sensor's blocks on the MBC Blocks website and use the "channel 1~16 reflect" block; on the hub it runs this Pybricks code (mode 9):

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

</section>

<p class="ref-missing"><b>Not readable here</b>：last exit direction, threshold, calibrations done. Use <a href="/en/sensors/line16/arduino/ide.md">Arduino</a> / <a href="/en/sensors/line16/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/line16/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

This software cannot send commands (neither the official SPIKE App 3 nor Pybricks' built-in ColorSensor blocks can). To send commands use [Pybricks + the MBC library](/en/sensors/line16/spike/pybricks.md); you can also calibrate with the button on the sensor.

## Examples

<!-- examples: en/sensors/line16/spike-examples.md host=spike sw=mbcblocks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MBC SPIKE Blocks website (online, nothing to install)](https://mbc-spike-blocks.mbcrobot.workers.dev)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line16.json` (single source of truth)
- Setup：`_build/guides/software/spike-mbcblocks.en.md`、`_build/guides/product/line16/spike-mbcblocks.en.md`
- Examples：`sensors/line16/spike-examples.md`
