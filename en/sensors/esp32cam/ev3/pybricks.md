<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/ev3/pybricks -->
# Sharpshooter Vision (ESP32CAM) × EV3 × Pybricks

This page covers **Pybricks v4 / EV3 MicroPython v2** only: how to set it up for the Sharpshooter Vision (ESP32CAM) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-pybricks.en.md + _build/guides/product/esp32cam/ev3-pybricks.en.md {"product":"Sharpshooter Vision (ESP32CAM)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Sharpshooter Vision (ESP32CAM): Pybricks v4 / EV3 MicroPython v2](/en/sensors/esp32cam/ev3-examples.md).
<!-- /include-fallback -->

## Reading values (School edition)

Code assumes port 1 and reads raw modes with `LUMPDevice`; EV3 MicroPython v2 works the same way.

<section id="v01" class="ref-item">

### hue <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="hue" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-1~100` | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

hue = sensor.read(0)[0]   # hue：-1~100
hue = sensor.read(1)[0]   # hue：-1~100
hue = sensor.read(2)[0]   # hue：-1~100
hue = sensor.read(3)[0]   # hue：-1~100
hue = sensor.read(5)[0]   # hue：-1~100
```

<p class="ref-note">mode 0 · int8 · IR-PROX 1×int8; mode 1 · int8 · IR-SEEK [色相, 飽和, 明度, 動態, X, Y, W, H]; mode 2 · int8 · IR-REMOTE [色相, 飽和, 明度, 動態]; mode 3 · int16 · IR-REM-A 1×int16（#65 已修成正確符號延伸，−1 = 0xFFFF）; mode 5 · int16 · IR-CAL [色相, 飽和] 2×int16 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v02" class="ref-item">

### saturation <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="saturation" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | saturation of sample point S1, 0~100 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

saturation = sensor.read(1)[1]   # saturation：0~100
saturation = sensor.read(2)[1]   # saturation：0~100
saturation = sensor.read(5)[1]   # saturation：0~100
```

<p class="ref-note">mode 1 · int8; mode 2 · int8; mode 5 · int16 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v03" class="ref-item">

### brightness <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="brightness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | brightness (HSV value) of sample point S1, 0~100 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

value = sensor.read(1)[2]   # brightness：0~100
value = sensor.read(2)[2]   # brightness：0~100
```

<p class="ref-note">mode 1 · int8; mode 2 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v04" class="ref-item">

### motion <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="motion" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | whole-frame motion 0~100 (more change = larger) |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

motion = sensor.read(1)[3]   # motion：0~100
motion = sensor.read(2)[3]   # motion：0~100
```

<p class="ref-note">mode 1 · int8; mode 2 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v05" class="ref-item">

### blob X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="blob X" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob centre X 0~100, left = 0; 0 when not found |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

blobX = sensor.read(1)[4]   # blob X：0~100
blobX = sensor.read(4)[0]   # blob X：0~100
```

<p class="ref-note">mode 1 · int8; mode 4 · int8 · IR-S-ALT [X, Y, W, H] (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v06" class="ref-item">

### blob Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="blob Y" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

blobY = sensor.read(1)[5]   # blob Y：0~75
blobY = sensor.read(4)[1]   # blob Y：0~75
```

<p class="ref-note">mode 1 · int8; mode 4 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v07" class="ref-item">

### blob width <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="blob width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob width W 0~100; 0 = not found |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

blobW = sensor.read(1)[6]   # blob width：0~100
blobW = sensor.read(4)[2]   # blob width：0~100
```

<p class="ref-note">mode 1 · int8; mode 4 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v08" class="ref-item">

### blob height <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="blob height" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob height H 0~75; 0 when not found |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

blobH = sensor.read(1)[7]   # blob height：0~75
blobH = sensor.read(4)[3]   # blob height：0~75
```

<p class="ref-note">mode 1 · int8; mode 4 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：blob area, blob found. Use <a href="/en/sensors/esp32cam/arduino/ide.md">Arduino</a> / <a href="/en/sensors/esp32cam/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/esp32cam/microbit/makecode.md">micro:bit</a> for these.</p>

## Reading values (WRO edition)

Code assumes port 1 and reads raw modes with `LUMPDevice`; EV3 MicroPython v2 works the same way.

<section id="w10" class="ref-item">

### spot 1~12 colour code <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="spot 1~12 colour code" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 10~1B = colour code of spots S1~S12: 0~239 colour (hue 0~255 scaled to 0~239), 240~245 white (saturation < 30, lower = larger), 246~250 black (value < 30, darker = larger), 255 = spot not enabled |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

spotColor = sensor.read(0)[0:12]   # spot 1~12 colour code
```

<p class="ref-note">mode 0 · int32×8 · HUE12 8×int32 LE（32 B）：24 byte = [色相, 鮮豔度]×12，每 3 byte 塞一個 int32（byte3 = 0）；格 n 色相 = byte 2(n−1) → int32 k = idx/3、byte = idx%3；鎖住 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="w20" class="ref-item">

### spot 1~12 vividness <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="spot 1~12 vividness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 20~2B = vividness (saturation) of spots S1~S12, 0~255: black = 1, white/colour = actual saturation; 0 when the spot is not enabled |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # sensor on port 1

spotVivid = sensor.read(0)[0:12]   # spot 1~12 vividness
```

<p class="ref-note">mode 0 · int32×8 · 同 10~1B：格 n 鮮豔度 = byte 2(n−1)+1；鎖住 (locked mode: firmware and hosts will not change it)</p>

</section>

## Examples (School edition)

<!-- examples: en/sensors/esp32cam/ev3-examples.md host=ev3 sw=pybricks -->

## Examples (WRO edition)

<!-- examples: en/sensors/esp32cam/ev3-examples-wro.md host=ev3 sw=pybricks -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json` (single source of truth)
- Setup：`_build/guides/software/ev3-pybricks.en.md`、`_build/guides/product/esp32cam/ev3-pybricks.en.md`
- Examples：`sensors/esp32cam/ev3-examples(-wro).md`
