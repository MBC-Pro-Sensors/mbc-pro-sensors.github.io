<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/spike/pybricks -->
# Pathfinder 8-Way (Line8) × SPIKE × Pybricks

This page covers **Pybricks (Python)** only: how to set it up for the Pathfinder 8-Way (Line8) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-pybricks.en.md + _build/guides/product/line8/spike-pybricks.en.md {"product":"Pathfinder 8-Way (Line8)","edition":"SPIKE edition","port":"C","lib":"MBC_line8_obj_Lib.py","libUrl":"/examples/line8/spike_beginner/pybricks/MBC_line8_obj_Lib.py","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 8-Way (Line8): Pybricks (Python)](/en/sensors/line8/spike-pybricks.md).
<!-- /include-fallback -->

## Reading values

Code assumes the sensor is on port A. For the MBC library form, import the library .py into Pybricks too.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
linePos = (w & 0xFF) - 8   # line position（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
linePos = line.pos8()
```

<p class="ref-note">mode 0 (edu) byte 1 −8 🔒; mode 0 (toy) byte 0 −8 🔒; mode 1 byte 0 −8; mode 5 v[0] 🔒; mode 9 v[4] lo −8 🔒. Note: Pybricks reflection() 編碼：X = (線位置+8)×10 + 線寬（0~168），a = X/25、b = (X−a×25+1)×10−1，3 個 int16 = a×256+b → reflection() ≈ X，主機 ÷10 取整−8 = 線位置、取餘數 = 線寬（INO:947-950）</p>

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
linePosFine = (w & 0xFF) - 100   # fine position（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
linePosFine = line.pos100()
```

<p class="ref-note">mode 0 (edu) byte 2 −100 🔒; mode 3 byte 0 −100; mode 4 v[0] −100; mode 7 v[0] 🔒; mode 9 v[5] lo −100 🔒. Note: 4 個 int16：第 1、2、4 格 = (細位置+100)×100 + 線寬（0~20008；Pybricks hsv(surface=False).h 讀第 1 格，原封不動）；第 3 格 = (細位置+100)×100 + min(線寬,9)×10（Pybricks ambient() 讀這格：4.1.2 先 ÷10 去尾再 ÷10、3.6.1／4.0.0 ÷100，都得到 細位置+100 . 線寬）。2026-10-30 起第 3 格改這個編碼</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` 路 | number of channels on the line, 0~8; 0 = no line |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[4]
lineWidth = ((w >> 8) & 0xFF)   # line width（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
lineWidth = line.width()
```

<p class="ref-note">mode 0 (edu) byte 0 🔒; mode 0 (toy) byte 1 🔒; mode 2 byte 0; mode 5 v[0] 🔒; mode 7 v[0] 🔒; mode 9 v[4] hi 🔒. Note: 跟線位置合在 reflection() 的個位數（見線位置）; 跟細位置合在一起（見細位置）：第 1、2、4 格是個位數（hsv(surface=False).h），第 3 格是十位數、最多 9（ambient() 的小數第 1 位）</p>

</section>

<section id="v04" class="ref-item">

### line segments <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="line segments" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~4` | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[5]
groupCount = ((w >> 8) & 0xFF)   # line segments（hi byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
groupCount = line.junction()
```

<p class="ref-note">mode 9 v[5] hi 🔒</p>

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # sensor on port A

w = dev.read(9)[6]
bitmap = (w & 0xFF)   # line bitmap（lo byte）
```

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
bitmap = line.bin_raw()
```

<p class="ref-note">mode 4 v[1]; mode 9 v[6] lo 🔒</p>

</section>

<section id="v10" class="ref-item">

### channel 1~8 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="channel 1~8 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |

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

**MBC library**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
light = line.ir_calibration()   # list, channel 1 = [0]
```

<p class="ref-note">mode 0 (edu) byte 4 🔒; mode 9 v[0] 🔒. Note: 4-bit (v×10)>>6; 每 int16 兩路：lo = 奇數路、hi = 偶數路</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：last exit direction, threshold, calibrations done. Use <a href="/en/sensors/line8/arduino/ide.md">Arduino</a> / <a href="/en/sensors/line8/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/line8/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

Commands need the **MBC library** (2026-10 or later) and **sensor firmware from 2026-10 or later**: send with `cmd(code, params…)`, then `wait_done()` waits for it (True = done; False = failed, rejected or timed out). Sensor states: 0 done, 1 busy, 2 failed (e.g. parameter out of range), 3 rejected (unknown command, or another command sent during calibration).

- On older firmware `cmd()` reports "firmware too old"; readings keep working.
- The same command with the same parameters is not repeated (even inside a loop); pass `cmd(…, again=True)` to run it again.
- `wait_done()` costs 40–60 ms each time: use it for stop-and-do commands such as calibration, not inside a line-following loop.
- The official SPIKE App 3 and Pybricks' built-in ColorSensor blocks cannot send commands.

<section id="ce0" class="ref-item ref-cmd">

### black line mode <small>blackLine</small>

follow a black line (default); RAM only, back to the default after reset

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE0)   # black line mode
ok = line.wait_done()   # True = done; False = failed / rejected / timed out
```

<p class="ref-note">Pybricks + the MBC library only (not the official app or built-in ColorSensor blocks); needs sensor firmware from 2026-10 or later.</p>

</section>

<section id="ce1" class="ref-item ref-cmd">

### white line mode <small>whiteLine</small>

follow a white line (F3 b1 = 1); RAM only, back to the default after reset

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE1)   # white line mode
ok = line.wait_done()   # True = done; False = failed / rejected / timed out
```

<p class="ref-note">Pybricks + the MBC library only (not the official app or built-in ColorSensor blocks); needs sensor firmware from 2026-10 or later.</p>

</section>

<section id="ce2" class="ref-item ref-cmd">

### calibrate black <small>calibBlack</small>

store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE2)   # calibrate black
ok = line.wait_done()   # True = done; False = failed / rejected / timed out
```

<p class="ref-note">Pybricks + the MBC library only (not the official app or built-in ColorSensor blocks); needs sensor firmware from 2026-10 or later.</p>

</section>

<section id="ce3" class="ref-item ref-cmd">

### calibrate white <small>calibWhite</small>

store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE3)   # calibrate white
ok = line.wait_done()   # True = done; False = failed / rejected / timed out
```

<p class="ref-note">Pybricks + the MBC library only (not the official app or built-in ColorSensor blocks); needs sensor firmware from 2026-10 or later.</p>

</section>

<section id="ce4" class="ref-item ref-cmd">

### set threshold <small>setThreshold</small>

threshold 1~99 (RAM only, back to 50 after reboot); on SPIKE an out-of-range value returns failed (state 2)

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE4, 50)   # set threshold（threshold 1~99）
ok = line.wait_done()   # True = done; False = failed / rejected / timed out
```

<p class="ref-note">Pybricks + the MBC library only (not the official app or built-in ColorSensor blocks); needs sensor firmware from 2026-10 or later.</p>

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5-second auto calibration <small>calibAuto</small>

sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE7)   # 5-second auto calibration
ok = line.wait_done(8000)   # True = done; False = failed / rejected / timed out
```

<p class="ref-note">Pybricks + the MBC library only (not the official app or built-in ColorSensor blocks); needs sensor firmware from 2026-10 or later.</p>

</section>

<section id="ce9" class="ref-item ref-cmd">

### mirror left/right <small>reverse</small>

[0 normal / 1 mirrored]: for a sensor mounted backwards — channel order and line position are mirrored; calibration stays per physical channel; RAM only, 0 after reset (not in legacy mBlock); on SPIKE an out-of-range value returns failed (state 2)

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE9, 1)   # mirror left/right（mirrored 0~1）
ok = line.wait_done()   # True = done; False = failed / rejected / timed out
```

<p class="ref-note">Pybricks + the MBC library only (not the official app or built-in ColorSensor blocks); needs sensor firmware from 2026-10 or later.</p>

</section>

## Examples

<!-- examples: en/sensors/line8/spike-examples.md host=spike sw=pybricks -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Pybricks library MBC_line8_obj_Lib.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_line8_obj_Lib.py)
- [Shared library (for new projects, one file for every MBC sensor) MBC_sensors.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_sensors.py)
- [line8_pybricks_spike_v4.x.zip](/examples/line8/line8_pybricks_spike_v4.x.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line8.json` (single source of truth)
- Setup：`_build/guides/software/spike-pybricks.en.md`、`_build/guides/product/line8/spike-pybricks.en.md`
- Examples：`sensors/line8/spike-examples.md`
