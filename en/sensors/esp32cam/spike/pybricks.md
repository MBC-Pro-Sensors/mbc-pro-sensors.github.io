<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/spike/pybricks -->
# Sharpshooter Vision (ESP32CAM) × SPIKE × Pybricks

This page covers **Pybricks (Python)** only: how to set it up for the Sharpshooter Vision (ESP32CAM) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-pybricks.en.md + _build/guides/product/esp32cam/spike-pybricks.en.md {"product":"Sharpshooter Vision (ESP32CAM)","edition":"SPIKE edition","port":"C","lib":"MBC_cam_school_Lib.py","libUrl":"/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Sharpshooter Vision (ESP32CAM): Pybricks (Python)](/en/sensors/esp32cam/spike-examples.md).
<!-- /include-fallback -->

## Reading values (School edition)

Code assumes the sensor is on port A. For the MBC library form, import the library .py into Pybricks too.

<section id="v01" class="ref-item">

### hue <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="hue" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-1~100` | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
hue = (w & 0xFF)   # hue（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
hue = cam.hue()
```

<p class="ref-note">mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 1 🔒; mode 5 v[0] 🔒; mode 9 v[0] lo 🔒. Note: −1 = 0xFF; 0~10（÷10）; raw = (色相×256 ± 128)/25 ≈ 色相×10.24，v[0]=v[1]=v[2]；Pybricks reflection() ÷10.24 還原</p>

</section>

<section id="v02" class="ref-item">

### saturation <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="saturation" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | saturation of sample point S1, 0~100 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[0]
saturation = ((w >> 8) & 0xFF)   # saturation（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
saturation = cam.saturation()
```

<p class="ref-note">mode 9 v[0] hi 🔒</p>

</section>

<section id="v03" class="ref-item">

### brightness <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="brightness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | brightness (HSV value) of sample point S1, 0~100 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
value = (w & 0xFF)   # brightness（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
value = cam.brightness()
```

<p class="ref-note">mode 9 v[1] lo 🔒</p>

</section>

<section id="v04" class="ref-item">

### motion <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="motion" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | whole-frame motion 0~100 (more change = larger) |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[1]
motion = ((w >> 8) & 0xFF)   # motion（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
motion = cam.motion()
```

<p class="ref-note">mode 0 (edu) byte 1 🔒; mode 0 (toy) byte 0 🔒; mode 9 v[1] hi 🔒</p>

</section>

<section id="v05" class="ref-item">

### blob X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="blob X" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob centre X 0~100, left = 0; 0 when not found |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[2]
blobX = (w & 0xFF)   # blob X（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobX = cam.blob_x()
```

<p class="ref-note">mode 0 (edu) byte 2 🔒; mode 0 (toy) byte 2 🔒; mode 6 v[0]; mode 7 v[2] 🔒; mode 9 v[2] lo 🔒. Note: byte 3 = 0; ×4 查表編碼（10-bit）; #67 改：模式 6 = [X, Y, 面積]（3×int16）; X×100+Y → Pybricks ambient() 顯示 X.Y</p>

</section>

<section id="v06" class="ref-item">

### blob Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="blob Y" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[2]
blobY = ((w >> 8) & 0xFF)   # blob Y（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobY = cam.blob_y()
```

<p class="ref-note">mode 0 (edu) byte 4 🔒; mode 0 (toy) byte 4 🔒; mode 6 v[1]; mode 7 v[2] 🔒; mode 9 v[2] hi 🔒. Note: byte 5 = 0; ×4 查表編碼; X×100+Y 的個位／十位</p>

</section>

<section id="v07" class="ref-item">

### blob width <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="blob width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob width W 0~100; 0 = not found |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[3]
blobW = (w & 0xFF)   # blob width（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobW = cam.blob_w()
```

<p class="ref-note">mode 9 v[3] lo 🔒</p>

</section>

<section id="v08" class="ref-item">

### blob height <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="blob height" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob height H 0~75; 0 when not found |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[3]
blobH = ((w >> 8) & 0xFF)   # blob height（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobH = cam.blob_h()
```

<p class="ref-note">mode 9 v[3] hi 🔒</p>

</section>

<section id="v09" class="ref-item">

### blob area <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="blob area" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~7500` | W×H (0~7500); 0 when not found |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

blobArea = dev.read(9)[4]   # blob area
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobArea = cam.area()
```

<p class="ref-note">mode 0 (edu) byte 6 🔒; mode 0 (toy) byte 6 🔒; mode 6 v[2]; mode 9 v[4] 🔒. Note: 鎖住：這裡是 map(像素面積, 0~5000, 0~100)，不是 W×H；byte 7 = 0; 同上 0~100 再 ×4 查表編碼; W×H; #67 新增：byte 8~9 = W×H（原本 0）；其餘 byte 不動</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：blob found. Use <a href="/en/sensors/esp32cam/arduino/ide.md">Arduino</a> / <a href="/en/sensors/esp32cam/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/esp32cam/microbit/makecode.md">micro:bit</a> for these.</p>

## Reading values (WRO edition)

Code assumes the sensor is on port A. For the MBC library form, import the library .py into Pybricks too.

<section id="w10" class="ref-item">

### spot 1~12 colour code <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="spot 1~12 colour code" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 10~1B = colour code of spots S1~S12: 0~239 colour (hue 0~255 scaled to 0~239), 240~245 white (saturation < 30, lower = larger), 246~250 black (value < 30, darker = larger), 255 = spot not enabled |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

spotColor = dev.read(0)[0]   # spot 1~12 colour code
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_hue12_Lib import HUE12Sensor

cam = HUE12Sensor(Port.A)
cam.update()   # refresh once per loop
spotColor = cam.get_slot(1)[1]   # list, channel 1 = [0]
```

<p class="ref-note">mode 0 (edu) byte 0 🔒. Note: 同 EV3：Pybricks read(0) 得 8 個 int32，每個拆 3 byte（積木網站 MBC_HUE12 設定檔）；玩具版不送；鎖住</p>

</section>

<section id="w20" class="ref-item">

### spot 1~12 vividness <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="spot 1~12 vividness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 20~2B = vividness (saturation) of spots S1~S12, 0~255: black = 1, white/colour = actual saturation; 0 when the spot is not enabled |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

spotVivid = dev.read(0)[1]   # spot 1~12 vividness
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_hue12_Lib import HUE12Sensor

cam = HUE12Sensor(Port.A)
cam.update()   # refresh once per loop
spotVivid = cam.get_slot(1)[2]   # list, channel 1 = [0]
```

<p class="ref-note">mode 0 (edu) byte 1 🔒. Note: 同 EV3；鎖住</p>

</section>

## Examples (School edition)

<!-- examples: en/sensors/esp32cam/spike-examples.md host=spike sw=pybricks -->

## Examples (WRO edition)

<!-- examples: en/sensors/esp32cam/spike-examples-wro.md host=spike sw=pybricks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Pybricks library MBC_cam_school_Lib.py](/examples/esp32cam/pybricks/MBC_cam_school_Lib.py)
- [Pybricks library MBC_hue12_Lib.py](/examples/esp32cam/pybricks/MBC_hue12_Lib.py)
- [Shared library (for new projects, one file for every MBC sensor) MBC_sensors.py](/examples/esp32cam/pybricks/MBC_sensors.py)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json` (single source of truth)
- Setup：`_build/guides/software/spike-pybricks.en.md`、`_build/guides/product/esp32cam/spike-pybricks.en.md`
- Examples：`sensors/esp32cam/spike-examples(-wro).md`
