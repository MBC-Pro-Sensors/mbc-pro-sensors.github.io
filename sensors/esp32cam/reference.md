<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_school.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# 神攝手 學校版 指令參考

[🔙 回到神攝手 學校版產品頁](/sensors/esp32cam/index.md)

這一頁列出神攝手 學校版送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 `?host=spike`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。

<p class="chips"><a class="chip chip-spike" href="/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off">MATRIX</span><a class="chip chip-microbit" href="/sensors/esp32cam/microbit.md">micro:bit</a></p>

| 項目 | 內容 |
| :--- | :--- |
| I2C 出廠位址 | `0x4C` |
| hw[0] | `1`（F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte、大通道 byte 28） |
| 型號 | `CAMSCH` |
| 協議版本 | `2` |
| 支援平台 | SPIKE、EV3、Arduino I2C、mBlock、micro:bit |

## 值一覽

| 編號 | 值 | 範圍 | SPIKE | EV3 | Arduino I2C | mBlock | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [色相](#v01) | -1~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [飽和度](#v02) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [明度](#v03) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [動態](#v04) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`05`](#v05) | [色塊 X](#v05) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`06`](#v06) | [色塊 Y](#v06) | 0~75 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`07`](#v07) | [色塊寬](#v07) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`08`](#v08) | [色塊高](#v08) | 0~75 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`09`](#v09) | [色塊面積](#v09) | 0~7500 | ✓ | — | ✓ | ✓ | ✓ |
| [`0A`](#v0a) | [有找到色塊](#v0a) | 0~1 | — | — | ✓ | ✓ | ✓ |

✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。

## 每個值怎麼讀

<section id="v01" class="ref-item">

### `01` 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |
| 範圍 | `-1~100` |
| I2C／mBlock 編號 | `01` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 1 🔒；模式 5 v[0] 🔒；模式 9 v[0] lo 🔒。備註：−1 = 0xFF; 0~10（÷10）; raw = (色相×256 ± 128)/25 ≈ 色相×10.24，v[0]=v[1]=v[2]；Pybricks reflection() ÷10.24 還原

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
hue = (w & 0xFF)   # 色相（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
hue = cam.hue()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 0 · int8 · IR-PROX 1×int8；模式 1 · int8 · IR-SEEK [色相, 飽和, 明度, 動態, X, Y, W, H]；模式 2 · int8 · IR-REMOTE [色相, 飽和, 明度, 動態]；模式 3 · int16 · IR-REM-A 1×int16（#65 已修成正確符號延伸，−1 = 0xFFFF）；模式 5 · int16 · IR-CAL [色相, 飽和] 2×int16（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

hue = sensor.read(0)[0]   # 色相：-1~100
hue = sensor.read(1)[0]   # 色相：-1~100
hue = sensor.read(2)[0]   # 色相：-1~100
hue = sensor.read(3)[0]   # 色相：-1~100
hue = sensor.read(5)[0]   # 色相：-1~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
hue = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 1)
hue = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 2)
hue = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
hue = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 5)
hue = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t hue = mbcRead(0x4C, 0x01);   // 色相：-1~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 色相（-1~100）。主機送 `[01]` → 回 `[值低][值高][01 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 色相 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 色相 :: #E8711E)</pre>

```typescript
let hue = MBC.readValue(0x4C, 0x01)   // 色相：-1~100
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | S1 取樣點飽和度 0~100 |
| 範圍 | `0~100` |
| I2C／mBlock 編號 | `02` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[0] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
saturation = ((w >> 8) & 0xFF)   # 飽和度（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
saturation = cam.saturation()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 2 · int8；模式 5 · int16（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

saturation = sensor.read(1)[1]   # 飽和度：0~100
saturation = sensor.read(2)[1]   # 飽和度：0~100
saturation = sensor.read(5)[1]   # 飽和度：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
saturation = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 2)
saturation = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 5)
saturation = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t saturation = mbcRead(0x4C, 0x02);   // 飽和度：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 飽和度（0~100）。主機送 `[02]` → 回 `[值低][值高][02 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 飽和度 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 飽和度 :: #E8711E)</pre>

```typescript
let saturation = MBC.readValue(0x4C, 0x02)   // 飽和度：0~100
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | S1 取樣點明度 0~100 |
| 範圍 | `0~100` |
| I2C／mBlock 編號 | `03` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[1] lo 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
value = (w & 0xFF)   # 明度（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
value = cam.brightness()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 2 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

value = sensor.read(1)[2]   # 明度：0~100
value = sensor.read(2)[2]   # 明度：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
value = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 2)
value = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t value = mbcRead(0x4C, 0x03);   // 明度：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 明度（0~100）。主機送 `[03]` → 回 `[值低][值高][03 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 明度 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 明度 :: #E8711E)</pre>

```typescript
let value = MBC.readValue(0x4C, 0x03)   // 明度：0~100
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 全畫面動態量 0~100（畫面變化越多越大） |
| 範圍 | `0~100` |
| I2C／mBlock 編號 | `04` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 0 🔒；模式 9 v[1] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
motion = ((w >> 8) & 0xFF)   # 動態（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
motion = cam.motion()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 2 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

motion = sensor.read(1)[3]   # 動態：0~100
motion = sensor.read(2)[3]   # 動態：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
motion = Sensor.ReadRawValue(1, 3)
Sensor.SetMode(1, 2)
motion = Sensor.ReadRawValue(1, 3)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t motion = mbcRead(0x4C, 0x04);   // 動態：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 動態（0~100）。主機送 `[04]` → 回 `[值低][值高][04 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 動態 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 動態 :: #E8711E)</pre>

```typescript
let motion = MBC.readValue(0x4C, 0x04)   // 動態：0~100
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |
| 範圍 | `0~100` |
| I2C／mBlock 編號 | `05` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 2 🔒；模式 0 (toy) byte 2 🔒；模式 6 v[0]；模式 7 v[2] 🔒；模式 9 v[2] lo 🔒。備註：byte 3 = 0; ×4 查表編碼（10-bit）; #67 改：模式 6 = [X, Y, 面積]（3×int16）; X×100+Y → Pybricks ambient() 顯示 X.Y

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[2]
blobX = (w & 0xFF)   # 色塊 X（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobX = cam.blob_x()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 4 · int8 · IR-S-ALT [X, Y, W, H]（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

blobX = sensor.read(1)[4]   # 色塊 X：0~100
blobX = sensor.read(4)[0]   # 色塊 X：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
blobX = Sensor.ReadRawValue(1, 4)
Sensor.SetMode(1, 4)
blobX = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t blobX = mbcRead(0x4C, 0x05);   // 色塊 X：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 色塊 X（0~100）。主機送 `[05]` → 回 `[值低][值高][05 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊 X :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 色塊 X :: #E8711E)</pre>

```typescript
let blobX = MBC.readValue(0x4C, 0x05)   // 色塊 X：0~100
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |
| 範圍 | `0~75` |
| I2C／mBlock 編號 | `06` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 4 🔒；模式 0 (toy) byte 4 🔒；模式 6 v[1]；模式 7 v[2] 🔒；模式 9 v[2] hi 🔒。備註：byte 5 = 0; ×4 查表編碼; X×100+Y 的個位／十位

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[2]
blobY = ((w >> 8) & 0xFF)   # 色塊 Y（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobY = cam.blob_y()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

blobY = sensor.read(1)[5]   # 色塊 Y：0~75
blobY = sensor.read(4)[1]   # 色塊 Y：0~75
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
blobY = Sensor.ReadRawValue(1, 5)
Sensor.SetMode(1, 4)
blobY = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t blobY = mbcRead(0x4C, 0x06);   // 色塊 Y：0~75
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 色塊 Y（0~75）。主機送 `[06]` → 回 `[值低][值高][06 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊 Y :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 色塊 Y :: #E8711E)</pre>

```typescript
let blobY = MBC.readValue(0x4C, 0x06)   // 色塊 Y：0~75
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 色塊寬 W 0~100；0 = 沒找到 |
| 範圍 | `0~100` |
| I2C／mBlock 編號 | `07` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[3] lo 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[3]
blobW = (w & 0xFF)   # 色塊寬（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobW = cam.blob_w()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

blobW = sensor.read(1)[6]   # 色塊寬：0~100
blobW = sensor.read(4)[2]   # 色塊寬：0~100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
blobW = Sensor.ReadRawValue(1, 6)
Sensor.SetMode(1, 4)
blobW = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t blobW = mbcRead(0x4C, 0x07);   // 色塊寬：0~100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 色塊寬（0~100）。主機送 `[07]` → 回 `[值低][值高][07 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊寬 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 色塊寬 :: #E8711E)</pre>

```typescript
let blobW = MBC.readValue(0x4C, 0x07)   // 色塊寬：0~100
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 色塊高 H 0~75；沒找到 = 0 |
| 範圍 | `0~75` |
| I2C／mBlock 編號 | `08` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[3] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[3]
blobH = ((w >> 8) & 0xFF)   # 色塊高（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobH = cam.blob_h()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

blobH = sensor.read(1)[7]   # 色塊高：0~75
blobH = sensor.read(4)[3]   # 色塊高：0~75
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
blobH = Sensor.ReadRawValue(1, 7)
Sensor.SetMode(1, 4)
blobH = Sensor.ReadRawValue(1, 3)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t blobH = mbcRead(0x4C, 0x08);   // 色塊高：0~75
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 色塊高（0~75）。主機送 `[08]` → 回 `[值低][值高][08 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊高 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 色塊高 :: #E8711E)</pre>

```typescript
let blobH = MBC.readValue(0x4C, 0x08)   // 色塊高：0~75
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` 色塊面積 <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="色塊面積" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | W×H（0~7500）；沒找到 = 0 |
| 範圍 | `0~7500` |
| I2C／mBlock 編號 | `09` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 6 🔒；模式 0 (toy) byte 6 🔒；模式 6 v[2]；模式 9 v[4] 🔒。備註：鎖住：這裡是 map(像素面積, 0~5000, 0~100)，不是 W×H；byte 7 = 0; 同上 0~100 再 ×4 查表編碼; W×H; #67 新增：byte 8~9 = W×H（原本 0）；其餘 byte 不動

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

blobArea = dev.read(9)[4]   # 色塊面積
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobArea = cam.area()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t blobArea = mbcRead(0x4C, 0x09);   // 色塊面積：0~7500
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 色塊面積（0~7500）。主機送 `[09]` → 回 `[值低][值高][09 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊面積 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 色塊面積 :: #E8711E)</pre>

```typescript
let blobArea = MBC.readValue(0x4C, 0x09)   // 色塊面積：0~7500
```

</details>

</section>

<section id="v0a" class="ref-item">

### `0A` 有找到色塊 <small>found</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |
| 範圍 | `0~1` |
| I2C／mBlock 編號 | `0A` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 模式沒有送這個值（已鎖住的模式不再改）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t found = mbcRead(0x4C, 0x0A);   // 有找到色塊：0~1
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 有找到色塊（0~1）。主機送 `[0A]` → 回 `[值低][值高][0A ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(神攝手 連接埠 [1 v] 有找到色塊？ :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 有找到色塊？ :: #E8711E)</pre>

```typescript
let found = MBC.readValue(0x4C, 0x0A)   // 有找到色塊：0~1
```

</details>

</section>

<section id="flags" class="ref-item">

## 狀態旗標 F3

`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀，MATRIX 大通道是 byte 24。

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1（相機進選單時值不更新，但仍 1） |
| b1 | 有找到色塊 (`found`) | = 值 0A |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

id_word, proto, fw, seq = dev.read(8)   # 模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]
addr = id_word & 0xFF   # 0x4C
hw0 = id_word >> 8      # 1
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 沒有 F3 旗標；模式 5 回裝置 ID 與韌體版本（ID 格式 B）。

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

id_word, fw = sensor.read(5)   # 模式 5：[ID | hw0<<8, 主版×100+次版]
addr = id_word & 0xFF             # 0x4C
hw0 = id_word >> 8                # 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x4C, 0xF3);
bool ready = st & (1 << 0);   // 就緒
bool found = st & (1 << 1);   // 有找到色塊
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

狀態旗標 F3 的各 bit 由擴展拆成布林積木（就緒、校準中…）。

<pre class="blocks"><神攝手 連接埠 [1 v] 連線正常？ :: sensing></pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC 裝置就緒 :: #E8711E></pre>

```typescript
let st = MBC.status(0x4C)   // F3
let ready = (st & 1) != 0
```

</details>

</section>

## 共用暫存器與命令

所有產品相同，只在 [通用 I2C 協議](/i2c-protocol.md) 寫一次：

| 編號 | 名稱 | 說明 |
| :--- | :--- | :--- |
| [`F0`](/i2c-protocol.md#rf0) | 裝置 ID | 低 byte = 出廠 I2C 位址（換位址後不變），高 byte = hw[0]（通道數／顆數／類別） |
| [`F1`](/i2c-protocol.md#rf1) | 協議版本 | 本版 = 2（SPIKE 模式 8 v[1]、EV3 ID 封包同一個數字） |
| [`F2`](/i2c-protocol.md#rf2) | 韌體版本 | 主版×100 + 次版（例 102 = 1.2） |
| [`F3`](/i2c-protocol.md#rf3) | 狀態旗標 | b0 就緒，其餘各產品定義（見各產品 flags）；MATRIX 大通道 byte 24 同一個 byte |
| [`F4`](/i2c-protocol.md#rf4) | 更新序號 | 每 10 ms 快照 +1，0~255 循環（2.56 秒一圈，不能拿來偵測重開機；不動 = 模組忙） |
| [`F5`](/i2c-protocol.md#rf5) | 開機秒數 | 開機後秒數，到 32767 停住（約 9 小時）；數字變小 = 重開機過 |
| [`F8`](/i2c-protocol.md#rf8) | 換 I2C 位址 | [F8][新位址 0x08~0x77][新位址取反]，存 EEPROM，重開機後生效；[F8][0x00][0xFF] = 回出廠位址（共識 B-5） |
| [`EF`](/i2c-protocol.md#ref) | 重新開機 | [EF][0xA5]；mBlock 先回 ACK 再重開 |

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/cam_school.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 1（共識 #6：神攝手 1 = 學校版）；model 字串 CAMSCH 照任務指定（韌體原本沒有型號欄位）。
- D4：I2C 0x4C 用 SDA = IO3、SCL = IO1（沿 PS4 Wire.setPins 慣例），與 EV3／SPIKE 串口模式二選一編譯；ESP32 從機不能時脈延展，onReceive 預載 32 B；頁面標「硬體已確認，上機待測」。mBlock 用同一組腳 Serial.begin(38400, SERIAL_8N1, 3, 1)。
- EV3 六個模式全部鎖住照盤點（模式 3／5 的 −1 已在 #65 修成符號延伸）；EV3 定案最後的 golden 備註把 cam_school 列進「模式 5 改 ID」是筆誤（定案表本身寫「—」；模式 5 仍是 [色相, 飽和]），所以 ev3IdMode = null。
- SPIKE 模式 9 鎖住，只有 byte 8~9（v[4]）從 0 改成面積 W×H；模式 6 改 [X, Y, 面積]；模式 8 = [0x4C | 1<<8, 2, 韌體版, 更新序號]；模式 1~4 維持 0。
- SPIKE 模式 0 的「面積」是 map(像素面積 0~5000 → 0~100)，跟值表 09 的 W×H 不同，鎖住不改（照盤點）。
- mVision：N = 10（idx 0~9），色相 −1 送 255；每 20 ms 一包（B-6）；不做 0x22／大通道（hosts.matrix = false）。
- 0A 有找到色塊 = W > 0（盤點：W 0 = 沒找到）；F3 b1 同這個值。
- 值 05~08 的「沒找到 = 0」照盤點 §0；色塊 Y 下 = 0、上 = 75 照盤點（map(bestY,120,0,0,75)）。
- 沒有寫入命令（韌體無法從程式改目標顏色／補光燈）。
