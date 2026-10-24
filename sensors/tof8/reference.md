<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof8/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof8/index.md">測距者 8 路</a></p>

# 測距者 8 路 指令參考

[🔙 回到測距者 8 路產品頁](/sensors/tof8/index.md)

這一頁列出測距者 8 路送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 `?host=spike`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/tof8/microbit.md">micro:bit</a></p>

| 項目 | 內容 |
| :--- | :--- |
| I2C 出廠位址 | `0x29` |
| hw[0] | `8`（F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte、大通道 byte 28） |
| 型號 | `LASER8` |
| 協議版本 | `2` |
| 支援平台 | SPIKE、EV3、Arduino I2C、mBlock、MATRIX、micro:bit |

## 值一覽

| 編號 | 值 | 範圍 | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [最近距離 cm](#v01) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [最近方向](#v02) | -8~+8 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [最空曠距離 cm](#v03) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [最空曠方向](#v04) | -7~+7 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`05`](#v05) | [正前方 cm](#v05) | 0~120 cm | — | ✓ | ✓ | ✓ | — | ✓ |
| [`06`](#v06) | [最近距離 mm](#v06) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`07`](#v07) | [初始化位元圖](#v07) | 0~255 bit | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| [`08`](#v08) | [故障位元圖](#v08) | 0~255 bit | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| [`09`](#v09) | [正前方距離 mm](#v09) | 0~1200 mm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`10~17`](#v10) | [第 1~8 顆距離 cm](#v10) | 0~120 cm | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`18~1F`](#v18) | [第 1~8 顆距離 mm](#v18) | 0~1200 mm | — | — | ✓ | ✓ | — | ✓ |

✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。

## 每個值怎麼讀

<section id="v01" class="ref-item">

### `01` 最近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="最近距離 cm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |
| 範圍 | `0~120` cm |
| I2C／mBlock 編號 | `01` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 2 byte 0；模式 3 byte 1；模式 4 v[0] hi；模式 7 v[2] 🔒；模式 9 v[4] hi 🔒。備註：[最近方向, 最近距離, 第 1 顆]（第 4 byte 第 8 顆被描述表砍掉）; Pybricks ambient() = int16[2]/100 = 最近 cm（INO:348-361）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
nearCm = ((w >> 8) & 0xFF)   # 最近距離 cm（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearCm = laser.min_distance()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 2 · int8 · IR-REMOTE [最近方向−8, 最近', 最空曠方向−8, 最空曠']（INO:154-159）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

nearCm = sensor.read(2)[1]   # 最近距離 cm：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
nearCm = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t nearCm = mbcRead(0x29, 0x01);   // 最近距離 cm：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 最近距離 cm（0~120 cm）。主機送 `[01]` → 回 `[值低][值高][01 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 最近距離 cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 9 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 9：最近距離 cm
int nearCm = bigU8(9);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 最近距離 cm :: #E8711E)</pre>

```typescript
let nearCm = MBC.readValue(0x29, 0x01)   // 最近距離 cm：0~120 cm
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` 最近方向 <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="最近方向" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |
| 範圍 | `-8~+8` |
| I2C／mBlock 編號 | `02` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 1 byte 0 −8；模式 3 byte 0 −8；模式 4 v[0] lo −8；模式 5 v[0] 🔒；模式 6 v[2]；模式 9 v[4] lo −8 🔒。備註：原始方位 0~16（8 = 正前）; Pybricks reflection() 編碼：x = 方位−8，負數 +6399，a = x/25、b = 10×(x mod 25)+9，3 個 int16 = a×256+b → reflection() 向零截斷 = −8~+8（INO:319-336）; #67 新：HSV 3×int16 = [最近 mm, 正前方 mm, 最近方向 −8~+8]

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
nearDir = (w & 0xFF) - 8   # 最近方向（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearDir = laser.min_direction()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 2 · int8 · int8 二補數（方位−8）；模式 5 · int16 · #67 新：IR-CAL 2×int16 = [最近 mm, 最近方向 −8~+8]（原本 = 前 4 顆兩兩合併）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

nearDir = sensor.read(2)[0]   # 最近方向：-8~+8
nearDir = sensor.read(5)[1]   # 最近方向：-8~+8
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
nearDir = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 5)
nearDir = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t nearDir = mbcRead(0x29, 0x02);   // 最近方向：-8~+8
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 最近方向（-8~+8）。主機送 `[02]` → 回 `[值低][值高][02 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 最近方向 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 8 u8 (+8)。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 8：最近方向 + 8（原始方位 0~16）
int nearDir = bigU8(8) - 8;
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 最近方向 :: #E8711E)</pre>

```typescript
let nearDir = MBC.readValue(0x29, 0x02)   // 最近方向：-8~+8
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` 最空曠距離 cm <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="最空曠距離 cm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |
| 範圍 | `0~120` cm |
| I2C／mBlock 編號 | `03` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[5] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
farCm = ((w >> 8) & 0xFF)   # 最空曠距離 cm（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
farCm = laser.max_distance()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 2 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

farCm = sensor.read(2)[3]   # 最空曠距離 cm：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
farCm = Sensor.ReadRawValue(1, 3)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t farCm = mbcRead(0x29, 0x03);   // 最空曠距離 cm：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 最空曠距離 cm（0~120 cm）。主機送 `[03]` → 回 `[值低][值高][03 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 最空曠距離 cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 11 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 11：最空曠距離 cm
int farCm = bigU8(11);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 最空曠距離 cm :: #E8711E)</pre>

```typescript
let farCm = MBC.readValue(0x29, 0x03)   // 最空曠距離 cm：0~120 cm
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` 最空曠方向 <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="最空曠方向" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |
| 範圍 | `-7~+7` |
| I2C／mBlock 編號 | `04` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[5] lo −8 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
farDir = (w & 0xFF) - 8   # 最空曠方向（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
farDir = laser.max_direction()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 2 · int8 · int8 二補數（方位−8）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

farDir = sensor.read(2)[2]   # 最空曠方向：-7~+7
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
farDir = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t farDir = mbcRead(0x29, 0x04);   // 最空曠方向：-7~+7
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 最空曠方向（-7~+7）。主機送 `[04]` → 回 `[值低][值高][04 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 最空曠方向 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 10 u8 (+8)。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 10：最空曠方向 + 8（原始方位 1~15）
int farDir = bigU8(10) - 8;
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 最空曠方向 :: #E8711E)</pre>

```typescript
let farDir = MBC.readValue(0x29, 0x04)   // 最空曠方向：-7~+7
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` 正前方 cm <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="正前方 cm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 第 4、5 顆（兩顆平行朝前）平均 0~120 cm；EV3 模式 3、SPIKE 模式 6、大通道 [16..17] 給的是同一個值的 mm；0x22 的 C 是 min(第 4, 第 5 顆) 不是平均（見 10） |
| 範圍 | `0~120` cm |
| I2C／mBlock 編號 | `05` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 模式沒有送這個值（已鎖住的模式不再改）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

模式 0 · int8 · IR-PROX = (第 4'+第 5')>>1（鎖住，INO:148-150）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

frontCm = sensor.read(0)[0]   # 正前方 cm：0~120 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
frontCm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t frontCm = mbcRead(0x29, 0x05);   // 正前方 cm：0~120 cm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 正前方 cm（0~120 cm）。主機送 `[05]` → 回 `[值低][值高][05 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 正前方距離 cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 正前方距離 cm :: #E8711E)</pre>

```typescript
let frontCm = MBC.readValue(0x29, 0x05)   // 正前方 cm：0~120 cm
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` 最近距離 mm <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="最近距離 mm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 8 顆 mm 的最小值 0~1200（失效那顆以 1200 參與），1200 = 四周沒東西 |
| 範圍 | `0~1200` mm |
| I2C／mBlock 編號 | `06` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 6 v[0]。備註：#67 新：[最近 mm, 正前方 mm, 方向]

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

nearMm = dev.read(6)[0]   # 最近距離 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearMm = laser.nearest_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 5 · int16 · #67 新：[最近 mm, 最近方向]

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

nearMm = sensor.read(5)[0]   # 最近距離 mm：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
nearMm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t nearMm = mbcRead(0x29, 0x06);   // 最近距離 mm：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 最近距離 mm（0~1200 mm）。主機送 `[06]` → 回 `[值低][值高][06 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 最近距離 mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 14~15 u16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 14~15：最近距離 mm（#67 追加（[14..15] 小端））
int nearMm = bigU16(14);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 最近距離 mm :: #E8711E)</pre>

```typescript
let nearMm = MBC.readValue(0x29, 0x06)   // 最近距離 mm：0~1200 mm
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | bit0 = 第 1 顆（最左）… bit7 = 第 8 顆，1 = 那顆開機初始化成功（8 顆約 2~4 秒，失敗不重試）；255 = 全部好 |
| 範圍 | `0~255` bit |
| I2C／mBlock 編號 | `07` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[6] lo 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[6]
initOk = (w & 0xFF)   # 初始化位元圖（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
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
int16_t initOk = mbcRead(0x29, 0x07);   // 初始化位元圖：0~255 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 初始化位元圖（0~255 bit）。主機送 `[07]` → 回 `[值低][值高][07 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 初始化位元圖 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 12 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 12：初始化位元圖（#67 補填（註解有寫、原本沒填））
int initOk = bigU8(12);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 初始化位元圖 :: #E8711E)</pre>

```typescript
let initOk = MBC.readValue(0x29, 0x07)   // 初始化位元圖：0~255 bit
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | bit0 = 第 1 顆 … bit7 = 第 8 顆，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |
| 範圍 | `0~255` bit |
| I2C／mBlock 編號 | `08` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[6] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[6]
fault = ((w >> 8) & 0xFF)   # 故障位元圖（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
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
int16_t fault = mbcRead(0x29, 0x08);   // 故障位元圖：0~255 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 故障位元圖（0~255 bit）。主機送 `[08]` → 回 `[值低][值高][08 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 故障位元圖 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 13 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 13：故障位元圖（#67 補填）
int fault = bigU8(13);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 故障位元圖 :: #E8711E)</pre>

```typescript
let fault = MBC.readValue(0x29, 0x08)   // 故障位元圖：0~255 bit
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` 正前方距離 mm <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="正前方距離 mm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 第 4、5 顆（正前方兩顆）平均，0~1200 mm；1200 = 前面沒東西。cm 版是 05 |
| 範圍 | `0~1200` mm |
| I2C／mBlock 編號 | `09` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 6 v[1]。備註：#67 新：正前方 mm

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

frontMm = dev.read(6)[1]   # 正前方距離 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
frontMm = laser.front_mm()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 3 · int16 · #67 新：IR-REM-A 1×int16 = 正前方（第 4、5 顆平均）mm（原本 = 第 1 顆 0~100）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

frontMm = sensor.read(3)[0]   # 正前方距離 mm：0~1200 mm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
frontMm = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t frontMm = mbcRead(0x29, 0x09);   // 正前方距離 mm：0~1200 mm
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 正前方距離 mm（0~1200 mm）。主機送 `[09]` → 回 `[值低][值高][09 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 正前方距離 mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 16~17 u16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 16~17：正前方距離 mm（#67 追加（MATRIX 定案 §4：[16..17] 正前方 mm 小端））
int frontMm = bigU16(16);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 正前方距離 mm :: #E8711E)</pre>

```typescript
let frontMm = MBC.readValue(0x29, 0x09)   // 正前方距離 mm：0~1200 mm
```

</details>

</section>

<section id="v10" class="ref-item">

### `10~17` 第 1~8 顆距離 cm <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="第 1~8 顆距離 cm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |
| 範圍 | `0~120` cm |
| I2C／mBlock 編號 | `10~17` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 0 🔒；模式 3 byte 2；模式 4 v[1]；模式 8 v[0]；模式 9 v[0] 🔒。備註：16 B = [第 1~8 顆, 0×8]：App 顏色 = 第 1 顆、反射光 = 第 2 顆、原始紅 = 第 3+256×第 4 …; byte0/1 = 第 1、2 顆 ×10/120（0~10）；byte2~7 = 第 3~8 顆兩顆一組 4-bit 再查表編碼（spikeEncodeU8，INO:259-284）; byte2 = 第 1 顆（byte3 = 第 8 顆被描述表砍掉）; v1 = 第 1 顆 + 256×第 8 顆; 只送 4 B：v0 = 第 1+256×第 2、v1 = 第 3+256×第 4; 每 int16 兩顆：lo = 第 2k+1 顆、hi = 第 2k+2 顆（v0~v3 = 第 1~8 顆）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

words = dev.read(9)[0:4]
distCm = []
for w in words:
    distCm.append(w & 0xFF)          # 奇數路
    distCm.append((w >> 8) & 0xFF)   # 偶數路
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
distCm = laser.distances()   # list，第 1 路 = [0]
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8 · IR-SEEK 8×int8 = 第 1~8 顆（EV3 視為頻道 1~4 的〈方位,距離〉）；模式 4 · int8 · IR-S-ALT = 前 4 顆（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

distCm = sensor.read(1)[0:8]   # 第 1~8 顆距離 cm
distCm = sensor.read(4)[0:4]   # 第 1~8 顆距離 cm
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
distCm1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
Sensor.SetMode(1, 4)
distCm1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x10~0x17 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t distCm[8];
mbcReadMany(0x29, 0x10, 8, distCm);   // 第 1~8 顆距離 cm：0~120 cm
for (uint8_t i = 0; i < 8; i++) { Serial.print(distCm[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 第 1~8 顆距離 cm（0~120 cm）。主機送 `[10]` → 回 `[值低][值高][10 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 第 [1 v] 顆距離 cm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R~K；大通道 byte 0~7 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// A. 原廠 0x22 積木：R~K = 第 1~8 顆距離 cm（#67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0）
int raw = MiniR4.I2C1.MXColor.getColor(R);   // 第 1 組
```

**B. 大通道（LauMo 函式庫）**

```cpp
// B. 大通道 byte 0~7：第 1~8 顆距離 cm
int distCm[8];
for (uint8_t i = 0; i < 8; i++) distCm[i] = bigU8(0 + i);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 第 [1 v] 顆距離 cm :: #E8711E)</pre>

```typescript
let distCm = MBC.readValues(0x29, 0x10, 8)   // 第 1~8 顆距離 cm
```

</details>

</section>

<section id="v18" class="ref-item">

### `18~1F` 第 1~8 顆距離 mm <small>distMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distMm.svg" alt="第 1~8 顆距離 mm" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 第 1~8 顆各自的距離 0~1200 mm，1200 = 那個方向沒東西／那顆失效；編號 18~1F |
| 範圍 | `0~1200` mm |
| I2C／mBlock 編號 | `18~1F` |

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

編號 0x18~0x1F 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t distMm[8];
mbcReadMany(0x29, 0x18, 8, distMm);   // 第 1~8 顆距離 mm：0~1200 mm
for (uint8_t i = 0; i < 8; i++) { Serial.print(distMm[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 第 1~8 顆距離 mm（0~1200 mm）。主機送 `[18]` → 回 `[值低][值高][18 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x18 + (N − 1)。

<pre class="blocks">(測距者 8 路 連接埠 [1 v] 第 [1 v] 顆距離 mm :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 第 [1 v] 顆距離 mm :: #E8711E)</pre>

```typescript
let distMm = MBC.readValues(0x29, 0x18, 8)   // 第 1~8 顆距離 mm
```

</details>

</section>

<section id="flags" class="ref-item">

## 狀態旗標 F3

`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀，MATRIX 大通道是 byte 24。

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 8 顆都初始化完（開機約 2~4 秒）、快照開始更新後恆 1 |
| b1 | 任一顆失效 (`fault`) | 初始化失敗、I2C 讀失敗或 200 ms 沒更新，任一顆就是 1（= 08 ≠ 0 或 07 ≠ 255） |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

id_word, proto, fw, seq = dev.read(8)   # 模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]
addr = id_word & 0xFF   # 0x29
hw0 = id_word >> 8      # 8
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
hw0 = id_word >> 8                # 8
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

<pre class="blocks"><測距者 8 路 連接埠 [1 v] 連線正常？ :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

```cpp
uint8_t st = MiniR4.I2C1.MXColor.getColorID();   // 大通道 byte 24 = F3 狀態旗標
bool ready = st & 1;
uint8_t id  = MiniR4.I2C1.MXColor.getColor(Y);    // byte 27 裝置 ID 0x29
uint8_t hw0 = MiniR4.I2C1.MXColor.getColor(K);    // byte 28 hw[0] = 8
```

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
| [`F3`](/i2c-protocol.md#rf3) | 狀態旗標 | b0 就緒，其餘各產品定義（見各產品 flags）；MATRIX 大通道 byte 24 同一個 byte |
| [`F4`](/i2c-protocol.md#rf4) | 更新序號 | 每 10 ms 快照 +1，0~255 循環（2.56 秒一圈，不能拿來偵測重開機；不動 = 模組忙） |
| [`F5`](/i2c-protocol.md#rf5) | 開機秒數 | 開機後秒數，到 32767 停住（約 9 小時）；數字變小 = 重開機過 |
| [`F8`](/i2c-protocol.md#rf8) | 換 I2C 位址 | [F8][新位址 0x08~0x77][新位址取反]，存 EEPROM，重開機後生效；[F8][0x00][0xFF] = 回出廠位址（共識 B-5） |
| [`EF`](/i2c-protocol.md#ref) | 重新開機 | [EF][0xA5]；mBlock 先回 ACK 再重開 |

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof8.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 正前方 mm（EV3 模式 3、SPIKE 模式 6 v[1]、大通道 [16..17]）在定案 §5 沒有獨立 I2C 編號 → 掛在 05 正前方 cm 底下，用 scale:mm 標示，不自己加編號。
- 0x22（使用者 2026-10-21 最終定案）：只用一個位址，R~K 由左到右 7 個 cm：第 1、2、3 顆、min(第 4, 第 5)、第 6、7、8 顆；11、12 不放資料；最近距離／方向由主機自己比較。此排法只用在 0x22，EV3／SPIKE／大通道不動。
- 大通道 [0..11] 不動（8 顆 cm、最近方向 0~16、最近 cm、最空曠方向、最空曠 cm）；補填 [12] 初始化、[13] 故障（原本註解有寫沒填）；追加 [14..15] 最近 mm、[16..17] 正前方 mm；尾段 [24] = F3、[27] = 0x29、[28] = 8（原本 [24] 沒填）。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改正前方 mm（原本 = 第 1 顆 0~100）、模式 5 改 [最近 mm, 最近方向 −8~+8]（原本 = 前 4 顆兩兩合併）。方向在 EV3／I2C／mBlock 一律 −8~+8 的 int8／int16，SPIKE 模式 1/3/4/9 與大通道維持原始 0~16（offset 8）。
- SPIKE：0/5/7/9 鎖住，1、2、3、4、8 維持不動；模式 6 改 [最近 mm, 正前方 mm, 最近方向 −8~+8]（原本 = 第 2~7 顆兩兩合併）；模式 8 v[0] 改 ID|hw[0]<<8。
- 最空曠方向 04 範圍 −7~+7（maxDirection 永遠不會是 0／16，盤點 INO:1086-1088）；最近方向 02 ±8 = 沒目標、沿用上次偏向側，開機預設 −8。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 主對話 2026-10-21：補 09 正前方 mm（定案 §5 只有 05 正前方 cm，但 EV3 模式 3、SPIKE 模式 6、大通道 [16..17] 都送 mm，I2C／mBlock 應有同一個值）。
