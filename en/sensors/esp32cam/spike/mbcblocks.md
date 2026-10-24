<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × spike × mbcblocks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/spike/mbcblocks -->
# Sharpshooter Vision (ESP32CAM) × SPIKE × MBC Blocks website

This page covers **MBC SPIKE Blocks website** only: how to set it up for the Sharpshooter Vision (ESP32CAM) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-mbcblocks.en.md + _build/guides/product/esp32cam/spike-mbcblocks.en.md {"product":"Sharpshooter Vision (ESP32CAM)","edition":"SPIKE edition","port":"C","lib":"MBC_cam_school_Lib.py","libUrl":"/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Sharpshooter Vision (ESP32CAM): MBC SPIKE Blocks website](/en/sensors/esp32cam/spike-examples.md).
<!-- /include-fallback -->

## Reading values (School edition)

The hub needs Pybricks firmware. The MBC Blocks website reads mode 9 of the sensor (all values in one read).

<section id="v01" class="ref-item">

### hue <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="hue" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-1~100` | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |

Add this sensor's blocks on the MBC Blocks website and use the "hue" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
hue = (w & 0xFF)   # hue（lo byte）
```

</section>

<section id="v02" class="ref-item">

### saturation <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="saturation" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | saturation of sample point S1, 0~100 |

Add this sensor's blocks on the MBC Blocks website and use the "saturation" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
saturation = ((w >> 8) & 0xFF)   # saturation（hi byte）
```

</section>

<section id="v03" class="ref-item">

### brightness <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="brightness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | brightness (HSV value) of sample point S1, 0~100 |

Add this sensor's blocks on the MBC Blocks website and use the "brightness" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
value = (w & 0xFF)   # brightness（lo byte）
```

</section>

<section id="v04" class="ref-item">

### motion <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="motion" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | whole-frame motion 0~100 (more change = larger) |

Add this sensor's blocks on the MBC Blocks website and use the "motion" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
motion = ((w >> 8) & 0xFF)   # motion（hi byte）
```

</section>

<section id="v05" class="ref-item">

### blob X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="blob X" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob centre X 0~100, left = 0; 0 when not found |

Add this sensor's blocks on the MBC Blocks website and use the "blob X" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[2]
blobX = (w & 0xFF)   # blob X（lo byte）
```

</section>

<section id="v06" class="ref-item">

### blob Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="blob Y" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |

Add this sensor's blocks on the MBC Blocks website and use the "blob Y" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[2]
blobY = ((w >> 8) & 0xFF)   # blob Y（hi byte）
```

</section>

<section id="v07" class="ref-item">

### blob width <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="blob width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob width W 0~100; 0 = not found |

Add this sensor's blocks on the MBC Blocks website and use the "blob width" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[3]
blobW = (w & 0xFF)   # blob width（lo byte）
```

</section>

<section id="v08" class="ref-item">

### blob height <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="blob height" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob height H 0~75; 0 when not found |

Add this sensor's blocks on the MBC Blocks website and use the "blob height" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[3]
blobH = ((w >> 8) & 0xFF)   # blob height（hi byte）
```

</section>

<section id="v09" class="ref-item">

### blob area <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="blob area" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~7500` | W×H (0~7500); 0 when not found |

Add this sensor's blocks on the MBC Blocks website and use the "blob area" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

blobArea = dev.read(9)[4]   # blob area
```

</section>

<p class="ref-missing"><b>Not readable here</b>：blob found. Use <a href="/en/sensors/esp32cam/arduino/ide.md">Arduino</a> / <a href="/en/sensors/esp32cam/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/esp32cam/microbit/makecode.md">micro:bit</a> for these.</p>

## Reading values (WRO edition)

The hub needs Pybricks firmware. The MBC Blocks website reads mode 9 of the sensor (all values in one read).

<p class="ref-missing"><b>Not readable here</b>：spot 1~12 colour code, spot 1~12 vividness. Use <a href="/en/sensors/esp32cam/spike/pybricks.md">Pybricks</a> for these.</p>

## Examples (School edition)

<!-- examples: en/sensors/esp32cam/spike-examples.md host=spike sw=mbcblocks -->

## Examples (WRO edition)

<!-- examples: en/sensors/esp32cam/spike-examples-wro.md host=spike sw=mbcblocks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MBC SPIKE Blocks website (online, nothing to install)](https://mbc-spike-blocks.mbcrobot.workers.dev)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json` (single source of truth)
- Setup：`_build/guides/software/spike-mbcblocks.en.md`、`_build/guides/product/esp32cam/spike-mbcblocks.en.md`
- Examples：`sensors/esp32cam/spike-examples(-wro).md`
