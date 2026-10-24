<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof2.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof2/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof2/index.md">測距者 2 路</a></p>

# 測距者 2 路 指令參考

[🔙 回到測距者 2 路產品頁](/sensors/tof2/index.md)

這一頁列出測距者 2 路送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 `?host=spike`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof2/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/tof2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/tof2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/tof2/microbit.md">micro:bit</a></p>

| 項目 | 內容 |
| :--- | :--- |
| I2C 出廠位址 | `0x29` |
| hw[0] | `2`（F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte） |
| 型號 | `LASER2` |
| 協議版本 | `2` |
| 支援平台 | SPIKE、EV3、Arduino I2C、mBlock、MATRIX、micro:bit |

## 值一覽

| 編號 | 值 | 範圍 | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [左距離 cm](#v01) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [右距離 cm](#v02) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [較近距離 cm](#v03) | 0~120 cm | — | — | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [左距離 mm](#v04) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`05`](#v05) | [右距離 mm](#v05) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`06`](#v06) | [初始化位元圖](#v06) | 0~3 bit | ✓ | — | ✓ | ✓ | — | ✓ |
| [`07`](#v07) | [故障位元圖](#v07) | 0~3 bit | ✓ | — | ✓ | ✓ | — | ✓ |
| [`08`](#v08) | [左右平均 mm](#v08) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | — | ✓ |

✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。

## 每個值怎麼讀

<section id="v01" class="ref-item">

### `01` 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |
| 範圍 | `0~120` cm |
| I2C／mBlock 編號 | `01` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 0 🔒；模式 1 byte 0；模式 2 byte 0；模式 3 byte 0 🔒；模式 4 v[0] lo；模式 5 v[0] 🔒；模式 8 v[0] lo；模式 9 v[0] lo 🔒。備註：16 B = [左, 右, 0, 左, 0, 右, 0, 平均, 0×8]：App 顏色 = 左、原始紅 = 256×左; 8 B 原樣（不壓縮）; 韌體註解寫「最近」，實際送左（INO:261-263）; 網站 read(3) 在用：[左, 右, 0]; Pybricks reflection() 編碼：L = 左 cm，a = L/25、b = 10×(L mod 25)+9，3 個 int16 = a×256+b → reflection() = L; 只送 4 B，v0 = 左 + 256×右

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
leftCm = (w & 0xFF)   # 左距離 cm（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
leftCm = laser.left()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8 · #67 新：IR-SEEK 8×int8 = [左, 右, 左, 右, 左, 右, 左, 右]（index 0/2/4/6 = 左；原本 index 2~7 = 0）；模式 2 · int8 · IR-REMOTE [左, 右, 0, 0]（INO:137-139）；模式 4 · int8 · IR-S-ALT 同模式 2（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

leftCm = sensor.read(1)[0]   # 左距離 cm：0~120 cm
leftCm = sensor.read(2)[0]   # 左距離 cm：0~120 cm
leftCm = sensor.read(4)[0]   # 左距離 cm：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
leftCm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 2)
leftCm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
leftCm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t leftCm = mbcRead(0x29, 0x01);   // 左距離 cm：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 左距離 cm（0~120 cm）。主機送 `[01]` → 回 `[值低][值高][01 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 左距離 cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R。

```cpp
// A. 原廠 0x22 積木：R = 左距離 cm
int leftCm = MiniR4.I2C1.MXColor.getColor(R);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 左距離 cm :: #E8711E)</pre>

```typescript
let leftCm = MBC.readValue(0x29, 0x01)   // 左距離 cm：0~120 cm
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |
| 範圍 | `0~120` cm |
| I2C／mBlock 編號 | `02` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 1 🔒；模式 3 byte 1 🔒；模式 4 v[0] hi；模式 7 v[2] 🔒；模式 8 v[0] hi；模式 9 v[0] hi 🔒。備註：App 反射光 = 右、原始綠 = 256×右; 網站 read(3) 在用; Pybricks ambient() = int16[2]/100 = 右 cm（INO:300-312）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
rightCm = ((w >> 8) & 0xFF)   # 右距離 cm（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
rightCm = laser.right()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8 · #67 新：index 1/3/5/7 = 右；模式 2 · int8；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

rightCm = sensor.read(1)[1]   # 右距離 cm：0~120 cm
rightCm = sensor.read(2)[1]   # 右距離 cm：0~120 cm
rightCm = sensor.read(4)[1]   # 右距離 cm：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
rightCm = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 2)
rightCm = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 4)
rightCm = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t rightCm = mbcRead(0x29, 0x02);   // 右距離 cm：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 右距離 cm（0~120 cm）。主機送 `[02]` → 回 `[值低][值高][02 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 右距離 cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 G。

```cpp
// A. 原廠 0x22 積木：G = 右距離 cm
int rightCm = MiniR4.I2C1.MXColor.getColor(G);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 右距離 cm :: #E8711E)</pre>

```typescript
let rightCm = MBC.readValue(0x29, 0x02)   // 右距離 cm：0~120 cm
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` 較近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="較近距離 cm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 左右兩顆取較近的 0~120 cm |
| 範圍 | `0~120` cm |
| I2C／mBlock 編號 | `03` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

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
int16_t nearCm = mbcRead(0x29, 0x03);   // 較近距離 cm：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 較近距離 cm（0~120 cm）。主機送 `[03]` → 回 `[值低][值高][03 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 較近距離 cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B。

```cpp
// A. 原廠 0x22 積木：B = 較近距離 cm（B（命令 12 也回較近，schema 不收 12））
int nearCm = MiniR4.I2C1.MXColor.getColor(B);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 較近距離 cm :: #E8711E)</pre>

```typescript
let nearCm = MBC.readValue(0x29, 0x03)   // 較近距離 cm：0~120 cm
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` 左距離 mm <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="左距離 mm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 左邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |
| 範圍 | `0~1200` mm |
| I2C／mBlock 編號 | `04` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 6 v[0]。備註：#67 新：HSV 3×int16 = [左 mm, 右 mm, 平均 mm]（原本是基底緩衝合併值）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

leftMm = dev.read(6)[0]   # 左距離 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
leftMm = laser.left_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 5 · int16 · #67 新：IR-CAL 2×int16 = [左 mm, 右 mm]（原本 word0 = 左'+256×右'）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

leftMm = sensor.read(5)[0]   # 左距離 mm：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
leftMm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t leftMm = mbcRead(0x29, 0x04);   // 左距離 mm：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 左距離 mm（0~1200 mm）。主機送 `[04]` → 回 `[值低][值高][04 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 左距離 mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 左距離 mm :: #E8711E)</pre>

```typescript
let leftMm = MBC.readValue(0x29, 0x04)   // 左距離 mm：0~1200 mm
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` 右距離 mm <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="右距離 mm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 右邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |
| 範圍 | `0~1200` mm |
| I2C／mBlock 編號 | `05` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 6 v[1]。備註：#67 新

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

rightMm = dev.read(6)[1]   # 右距離 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
rightMm = laser.right_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 5 · int16 · #67 新

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

rightMm = sensor.read(5)[1]   # 右距離 mm：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
rightMm = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t rightMm = mbcRead(0x29, 0x05);   // 右距離 mm：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 右距離 mm（0~1200 mm）。主機送 `[05]` → 回 `[值低][值高][05 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 右距離 mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 右距離 mm :: #E8711E)</pre>

```typescript
let rightMm = MBC.readValue(0x29, 0x05)   // 右距離 mm：0~1200 mm
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不重試）；3 = 兩顆都好 |
| 範圍 | `0~3` bit |
| I2C／mBlock 編號 | `06` |
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
initOk = (w & 0xFF)   # 初始化位元圖（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
initOk = laser.is_ok(1)
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
int16_t initOk = mbcRead(0x29, 0x06);   // 初始化位元圖：0~3 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 初始化位元圖（0~3 bit）。主機送 `[06]` → 回 `[值低][值高][06 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 初始化位元圖 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 初始化位元圖 :: #E8711E)</pre>

```typescript
let initOk = MBC.readValue(0x29, 0x06)   // 初始化位元圖：0~3 bit
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | bit0 = 左、bit1 = 右，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |
| 範圍 | `0~3` bit |
| I2C／mBlock 編號 | `07` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[1] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
fault = ((w >> 8) & 0xFF)   # 故障位元圖（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
fault = laser.is_ok(1)
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
int16_t fault = mbcRead(0x29, 0x07);   // 故障位元圖：0~3 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 故障位元圖（0~3 bit）。主機送 `[07]` → 回 `[值低][值高][07 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 故障位元圖 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 故障位元圖 :: #E8711E)</pre>

```typescript
let fault = MBC.readValue(0x29, 0x07)   // 故障位元圖：0~3 bit
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` 左右平均 mm <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="左右平均 mm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | (左 mm + 右 mm) / 2，0~1200；跟 EV3 模式 3、SPIKE 模式 6 第 3 個值一致（共識 #8 補上） |
| 範圍 | `0~1200` mm |
| I2C／mBlock 編號 | `08` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 7 🔒；模式 6 v[2]。備註：byte7 = (左 cm + 右 cm)>>1；App 原始藍 = 256×平均; #67 新：平均 mm

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

avgMm = dev.read(0)[7]   # 左右平均 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
avgMm = laser.average_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 0 · int8 · IR-PROX = (左'+右')>>1，cm 縮放後平均（鎖住，INO:131-133）；模式 3 · int16 · #67 新：IR-REM-A 1×int16 = 左右平均 mm（原本 = 左 0~100）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

avgMm = sensor.read(0)[0]   # 左右平均 mm：0~1200 mm
avgMm = sensor.read(3)[0]   # 左右平均 mm：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
avgMm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
avgMm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t avgMm = mbcRead(0x29, 0x08);   // 左右平均 mm：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 左右平均 mm（0~1200 mm）。主機送 `[08]` → 回 `[值低][值高][08 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 左右平均 mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 左右平均 mm :: #E8711E)</pre>

```typescript
let avgMm = MBC.readValue(0x29, 0x08)   // 左右平均 mm：0~1200 mm
```

</details>

</section>

<section id="flags" class="ref-item">

## 狀態旗標 F3

`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀。

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 2 顆都初始化完、快照開始更新後恆 1 |
| b1 | 任一顆失效 (`fault`) | 初始化失敗、I2C 讀失敗或 200 ms 沒更新，任一顆就是 1（= 07 ≠ 0 或 06 ≠ 3） |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

id_word, proto, fw, seq = dev.read(8)   # 模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]
addr = id_word & 0xFF   # 0x29
hw0 = id_word >> 8      # 2
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
addr = id_word & 0xFF             # 0x29
hw0 = id_word >> 8                # 2
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x29, 0xF3);
bool ready = st & (1 << 0);   // 就緒
bool fault = st & (1 << 1);   // 任一顆失效
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

狀態旗標 F3 的各 bit 由擴展拆成布林積木（就緒、校準中…）。

<pre class="blocks"><測距者 2 路 連接埠 [1 v] 連線正常？ :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木讀不到狀態旗標。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC 裝置就緒 :: #E8711E></pre>

```typescript
let st = MBC.status(0x29)   // F3
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
| [`F3`](/i2c-protocol.md#rf3) | 狀態旗標 | b0 就緒，其餘各產品定義（見各產品 flags） |
| [`F4`](/i2c-protocol.md#rf4) | 更新序號 | 每 10 ms 快照 +1，0~255 循環（2.56 秒一圈，不能拿來偵測重開機；不動 = 模組忙） |
| [`F5`](/i2c-protocol.md#rf5) | 開機秒數 | 開機後秒數，到 32767 停住（約 9 小時）；數字變小 = 重開機過 |
| [`F8`](/i2c-protocol.md#rf8) | 換 I2C 位址 | [F8][新位址 0x08~0x77][新位址取反]，存 EEPROM，重開機後生效；[F8][0x00][0xFF] = 回出廠位址（共識 B-5） |
| [`EF`](/i2c-protocol.md#ref) | 重新開機 | [EF][0xA5]；mBlock 先回 ACK 再重開 |

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 共識 #8：08 左右平均 mm 補進 I2C／mBlock 值表（定案 §5 已列）。
- 0x22 命令 4~12 不動（4 左、5 右、6 最近、7~11 = 0、12 = 最近）；schema 不收 11/12，所以 12 只寫在 03 的 note。
- EV3：模式 0、2、4 鎖住（照盤點）；模式 1 改 [左,右]×4（index 0/1 各代表左右）、模式 3 改平均 mm、模式 5 改 [左 mm, 右 mm]；模式 1 描述表宣告 int8 −100~100，cm×100/120 在範圍內。
- SPIKE：0/5/7/9 鎖住，另加模式 3 鎖住（網站 sensors/tof2/index.md read(3) = 左／右 cm，共識 #11）；模式 6 改 [左 mm, 右 mm, 平均 mm]；模式 1、2、4、8 維持不動；模式 8 v[0] 改 ID|hw[0]<<8（統一後共用函式）。
- 「左」= 站在車後往前看的左（第 0 顆，XSHUT PA6）；韌體只有 L/R 巨集名，盤點檔標為推測。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
