<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/spike/pybricks -->
# Ranger 8-Way (TOF8) × SPIKE × Pybricks

This page covers **Pybricks (Python)** only: how to set it up for the Ranger 8-Way (TOF8) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-pybricks.en.md + _build/guides/product/tof8/spike-pybricks.en.md {"product":"Ranger 8-Way (TOF8)","edition":"SPIKE edition","port":"C","lib":"MBC_laser8_obj_Lib.py","libUrl":"/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): Pybricks (Python)](/en/sensors/tof8/spike-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes the sensor is on port A. For the MBC library form, import the library .py into Pybricks too.

<section id="v01" class="ref-item">

### nearest distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="nearest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
nearCm = ((w >> 8) & 0xFF)   # nearest distance (cm)（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearCm = laser.min_distance()
```

<p class="ref-note">mode 2 byte 0; mode 3 byte 1; mode 4 v[0] hi; mode 7 v[2] 🔒; mode 9 v[4] hi 🔒. Note: [最近方向, 最近距離, 第 1 顆]（第 4 byte 第 8 顆被描述表砍掉）; Pybricks ambient() = int16[2]/100 = 最近 cm（INO:348-361）</p>

</section>

<section id="v02" class="ref-item">

### nearest direction <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="nearest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
nearDir = (w & 0xFF) - 8   # nearest direction（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearDir = laser.min_direction()
```

<p class="ref-note">mode 1 byte 0 −8; mode 3 byte 0 −8; mode 4 v[0] lo −8; mode 5 v[0] 🔒; mode 6 v[2]; mode 9 v[4] lo −8 🔒. Note: 原始方位 0~16（8 = 正前）; Pybricks reflection() 編碼：x = 方位−8，負數 +6399，a = x/25、b = 10×(x mod 25)+9，3 個 int16 = a×256+b → reflection() 向零截斷 = −8~+8（INO:319-336）; #67 新：HSV 3×int16 = [最近 mm, 正前方 mm, 最近方向 −8~+8]</p>

</section>

<section id="v03" class="ref-item">

### farthest distance (cm) <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="farthest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
farCm = ((w >> 8) & 0xFF)   # farthest distance (cm)（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
farCm = laser.max_distance()
```

<p class="ref-note">mode 9 v[5] hi 🔒</p>

</section>

<section id="v04" class="ref-item">

### farthest direction <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="farthest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-7~+7` | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
farDir = (w & 0xFF) - 8   # farthest direction（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
farDir = laser.max_direction()
```

<p class="ref-note">mode 9 v[5] lo −8 🔒</p>

</section>

<section id="v06" class="ref-item">

### nearest distance (mm) <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="nearest distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

nearMm = dev.read(6)[0]   # nearest distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearMm = laser.nearest_mm()
```

<p class="ref-note">mode 6 v[0]. Note: #67 新：[最近 mm, 正前方 mm, 方向]</p>

</section>

<section id="v07" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[6]
initOk = (w & 0xFF)   # init bitmap（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
initOk = laser.is_ok(1)
```

<p class="ref-note">mode 9 v[6] lo 🔒</p>

</section>

<section id="v08" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[6]
fault = ((w >> 8) & 0xFF)   # fault bitmap（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
fault = laser.is_ok(1)
```

<p class="ref-note">mode 9 v[6] hi 🔒</p>

</section>

<section id="v09" class="ref-item">

### front distance (mm) <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="front distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

frontMm = dev.read(6)[1]   # front distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
frontMm = laser.front_mm()
```

<p class="ref-note">mode 6 v[1]. Note: #67 新：正前方 mm</p>

</section>

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

words = dev.read(9)[0:4]
distCm = []
for w in words:
    distCm.append(w & 0xFF)          # odd channel
    distCm.append((w >> 8) & 0xFF)   # even channel
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
distCm = laser.distances()   # list, channel 1 = [0]
```

<p class="ref-note">mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 0 🔒; mode 3 byte 2; mode 4 v[1]; mode 8 v[0]; mode 9 v[0] 🔒. Note: 16 B = [第 1~8 顆, 0×8]：App 顏色 = 第 1 顆、反射光 = 第 2 顆、原始紅 = 第 3+256×第 4 …; byte0/1 = 第 1、2 顆 ×10/120（0~10）；byte2~7 = 第 3~8 顆兩顆一組 4-bit 再查表編碼（spikeEncodeU8，INO:259-284）; byte2 = 第 1 顆（byte3 = 第 8 顆被描述表砍掉）; v1 = 第 1 顆 + 256×第 8 顆; 只送 4 B：v0 = 第 1+256×第 2、v1 = 第 3+256×第 4; 每 int16 兩顆：lo = 第 2k+1 顆、hi = 第 2k+2 顆（v0~v3 = 第 1~8 顆）</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：front distance (cm), sensor 1~8 distance (mm). Use <a href="/en/sensors/tof8/arduino/ide.md">Arduino</a> / <a href="/en/sensors/tof8/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/tof8/microbit/makecode.md">micro:bit</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof8/spike-examples.md host=spike sw=pybricks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Pybricks library MBC_laser8_obj_Lib.py](/examples/tof8/pybricks/For%20firmware%204.0.0/MBC_laser8_obj_Lib.py)
- [tof8_pybricks_spike_v3.6.1.zip](/examples/tof8/tof8_pybricks_spike_v3.6.1.zip)
- [tof8_pybricks_spike_v4.0.0.zip](/examples/tof8/tof8_pybricks_spike_v4.0.0.zip)
- [MBC_TOF8_Pybricks_Lib.zip](/downloads/MBC_TOF8_Pybricks_Lib.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/spike-pybricks.en.md`、`_build/guides/product/tof8/spike-pybricks.en.md`
- Examples：`sensors/tof8/spike-examples.md`
