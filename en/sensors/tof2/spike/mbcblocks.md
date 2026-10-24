<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × spike × mbcblocks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/spike/mbcblocks -->
# Ranger 2-Way (TOF2) × SPIKE × MBC Blocks website

This page covers **MBC SPIKE Blocks website** only: how to set it up for the Ranger 2-Way (TOF2) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-mbcblocks.en.md + _build/guides/product/tof2/spike-mbcblocks.en.md {"product":"Ranger 2-Way (TOF2)","edition":"SPIKE edition","port":"C","lib":"MBC_laser2_obj_Lib.py","libUrl":"/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): MBC SPIKE Blocks website](/en/sensors/tof2/spike-examples.md).
<!-- /include-fallback -->

## Reading values

The hub needs Pybricks firmware. The MBC Blocks website reads mode 9 of the sensor (all values in one read).

<section id="v01" class="ref-item">

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

Add this sensor's blocks on the MBC Blocks website and use the "left distance (cm)" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
leftCm = (w & 0xFF)   # left distance (cm)（lo byte）
```

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

Add this sensor's blocks on the MBC Blocks website and use the "right distance (cm)" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
rightCm = ((w >> 8) & 0xFF)   # right distance (cm)（hi byte）
```

</section>

<section id="v06" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor initialised at power-on (no retry); 3 = both OK |

Add this sensor's blocks on the MBC Blocks website and use the "init bitmap" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
initOk = (w & 0xFF)   # init bitmap（lo byte）
```

</section>

<section id="v07" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor is currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

Add this sensor's blocks on the MBC Blocks website and use the "fault bitmap" block; on the hub it runs this Pybricks code (mode 9):

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
fault = ((w >> 8) & 0xFF)   # fault bitmap（hi byte）
```

</section>

<p class="ref-missing"><b>Not readable here</b>：nearer distance (cm), left distance (mm), right distance (mm), average distance (mm). Use <a href="/en/sensors/tof2/spike/official.md">Official LEGO App</a> / <a href="/en/sensors/tof2/spike/pybricks.md">Pybricks</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof2/spike-examples.md host=spike sw=mbcblocks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MBC SPIKE Blocks website (online, nothing to install)](https://mbc-spike-blocks.mbcrobot.workers.dev)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/spike-mbcblocks.en.md`、`_build/guides/product/tof2/spike-mbcblocks.en.md`
- Examples：`sensors/tof2/spike-examples.md`
