<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/spike/pybricks -->
# Ranger 2-Way (TOF2) × SPIKE × Pybricks

This page covers **Pybricks (Python)** only: how to set it up for the Ranger 2-Way (TOF2) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-pybricks.en.md + _build/guides/product/tof2/spike-pybricks.en.md {"product":"Ranger 2-Way (TOF2)","edition":"SPIKE edition","port":"C","lib":"MBC_laser2_obj_Lib.py","libUrl":"/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): Pybricks (Python)](/en/sensors/tof2/spike-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes the sensor is on port A. For the MBC library form, import the library .py into Pybricks too.

<section id="v01" class="ref-item">

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
leftCm = (w & 0xFF)   # left distance (cm)（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
leftCm = laser.left()
```

<p class="ref-note">mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 0 🔒; mode 1 byte 0; mode 2 byte 0; mode 3 byte 0 🔒; mode 4 v[0] lo; mode 5 v[0] 🔒; mode 8 v[0] lo; mode 9 v[0] lo 🔒. Note: 16 B = [左, 右, 0, 左, 0, 右, 0, 平均, 0×8]：App 顏色 = 左、原始紅 = 256×左; 8 B 原樣（不壓縮）; 韌體註解寫「最近」，實際送左（INO:261-263）; 網站 read(3) 在用：[左, 右, 0]; Pybricks reflection() 編碼：L = 左 cm，a = L/25、b = 10×(L mod 25)+9，3 個 int16 = a×256+b → reflection() = L; 只送 4 B，v0 = 左 + 256×右</p>

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
rightCm = ((w >> 8) & 0xFF)   # right distance (cm)（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
rightCm = laser.right()
```

<p class="ref-note">mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 1 🔒; mode 3 byte 1 🔒; mode 4 v[0] hi; mode 7 v[2] 🔒; mode 8 v[0] hi; mode 9 v[0] hi 🔒. Note: App 反射光 = 右、原始綠 = 256×右; 網站 read(3) 在用; Pybricks ambient() = int16[2]/100 = 右 cm（INO:300-312）</p>

</section>

<section id="v04" class="ref-item">

### left distance (mm) <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="left distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | left sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

leftMm = dev.read(6)[0]   # left distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
leftMm = laser.left_mm()
```

<p class="ref-note">mode 6 v[0]. Note: #67 新：HSV 3×int16 = [左 mm, 右 mm, 平均 mm]（原本是基底緩衝合併值）</p>

</section>

<section id="v05" class="ref-item">

### right distance (mm) <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="right distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | right sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

rightMm = dev.read(6)[1]   # right distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
rightMm = laser.right_mm()
```

<p class="ref-note">mode 6 v[1]. Note: #67 新</p>

</section>

<section id="v06" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor initialised at power-on (no retry); 3 = both OK |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
initOk = (w & 0xFF)   # init bitmap（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
initOk = laser.is_ok(1)
```

<p class="ref-note">mode 9 v[1] lo 🔒</p>

</section>

<section id="v07" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor is currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
fault = ((w >> 8) & 0xFF)   # fault bitmap（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
fault = laser.is_ok(1)
```

<p class="ref-note">mode 9 v[1] hi 🔒</p>

</section>

<section id="v08" class="ref-item">

### average distance (mm) <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="average distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | (left mm + right mm) / 2, 0~1200; same number as EV3 mode 3 and the 3rd value of SPIKE mode 6 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

avgMm = dev.read(0)[7]   # average distance (mm)
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
avgMm = laser.average_mm()
```

<p class="ref-note">mode 0 (edu) byte 7 🔒; mode 6 v[2]. Note: byte7 = (左 cm + 右 cm)>>1；App 原始藍 = 256×平均; #67 新：平均 mm</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：nearer distance (cm). Use <a href="/en/sensors/tof2/arduino/ide.md">Arduino</a> / <a href="/en/sensors/tof2/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/tof2/microbit/makecode.md">micro:bit</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof2/spike-examples.md host=spike sw=pybricks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Pybricks library MBC_laser2_obj_Lib.py](/examples/tof2/pybricks/For%20firmware%204.x/MBC_laser2_obj_Lib.py)
- [Shared library (for new projects, one file for every MBC sensor) MBC_sensors.py](/examples/tof2/pybricks/For%20firmware%204.x/MBC_sensors.py)
- [tof2_pybricks_spike_v4.x.zip](/examples/tof2/tof2_pybricks_spike_v4.x.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/spike-pybricks.en.md`、`_build/guides/product/tof2/spike-pybricks.en.md`
- Examples：`sensors/tof2/spike-examples.md`
