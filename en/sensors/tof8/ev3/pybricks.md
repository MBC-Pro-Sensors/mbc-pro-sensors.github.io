<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/ev3/pybricks -->
# Ranger 8-Way (TOF8) × EV3 × Pybricks

This page covers **Pybricks v4 / EV3 MicroPython v2** only: how to set it up for the Ranger 8-Way (TOF8) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-pybricks.en.md + _build/guides/product/tof8/ev3-pybricks.en.md {"product":"Ranger 8-Way (TOF8)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): Pybricks v4 / EV3 MicroPython v2](/en/sensors/tof8/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes port 1 and reads raw modes with `LUMPDevice`; EV3 MicroPython v2 works the same way.

<section id="v01" class="ref-item">

### nearest distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="nearest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

nearCm = sensor.read(2)[1]   # nearest distance (cm)：0~120 cm
```

<p class="ref-note">mode 2 · int8 · IR-REMOTE [最近方向−8, 最近', 最空曠方向−8, 最空曠']（INO:154-159） (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v02" class="ref-item">

### nearest direction <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="nearest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

nearDir = sensor.read(2)[0]   # nearest direction：-8~+8
nearDir = sensor.read(5)[1]   # nearest direction：-8~+8
```

<p class="ref-note">mode 2 · int8 · int8 二補數（方位−8）; mode 5 · int16 · #67 新：IR-CAL 2×int16 = [最近 mm, 最近方向 −8~+8]（原本 = 前 4 顆兩兩合併） (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v03" class="ref-item">

### farthest distance (cm) <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="farthest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

farCm = sensor.read(2)[3]   # farthest distance (cm)：0~120 cm
```

<p class="ref-note">mode 2 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v04" class="ref-item">

### farthest direction <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="farthest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-7~+7` | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

farDir = sensor.read(2)[2]   # farthest direction：-7~+7
```

<p class="ref-note">mode 2 · int8 · int8 二補數（方位−8） (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v05" class="ref-item">

### front distance (cm) <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="front distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3, SPIKE mode 6 and big-channel [16..17] carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

frontCm = sensor.read(0)[0]   # front distance (cm)：0~120 cm
```

<p class="ref-note">mode 0 · int8 · IR-PROX = (第 4'+第 5')>>1（鎖住，INO:148-150） (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v06" class="ref-item">

### nearest distance (mm) <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="nearest distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

nearMm = sensor.read(5)[0]   # nearest distance (mm)：0~1200 mm
```

<p class="ref-note">mode 5 · int16 · #67 新：[最近 mm, 最近方向]</p>

</section>

<section id="v09" class="ref-item">

### front distance (mm) <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="front distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

frontMm = sensor.read(3)[0]   # front distance (mm)：0~1200 mm
```

<p class="ref-note">mode 3 · int16 · #67 新：IR-REM-A 1×int16 = 正前方（第 4、5 顆平均）mm（原本 = 第 1 顆 0~100）</p>

</section>

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

distCm = sensor.read(1)[0:8]   # sensor 1~8 distance (cm)
distCm = sensor.read(4)[0:4]   # sensor 1~8 distance (cm)
```

<p class="ref-note">mode 1 · int8 · IR-SEEK 8×int8 = 第 1~8 顆（EV3 視為頻道 1~4 的〈方位,距離〉）; mode 4 · int8 · IR-S-ALT = 前 4 顆 (locked mode: firmware and hosts will not change it)</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：init bitmap, fault bitmap, sensor 1~8 distance (mm). Use <a href="/en/sensors/tof8/arduino/ide.md">Arduino</a> / <a href="/en/sensors/tof8/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/tof8/microbit/makecode.md">micro:bit</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof8/ev3-examples.md host=ev3 sw=pybricks -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/ev3-pybricks.en.md`、`_build/guides/product/tof8/ev3-pybricks.en.md`
- Examples：`sensors/tof8/ev3-examples.md`
