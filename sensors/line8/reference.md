<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line8.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/line8/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line8/index.md">循行者 8 路</a></p>

# 循行者 8 路 指令參考

[🔙 回到循行者 8 路產品頁](/sensors/line8/index.md)

這一頁列出循行者 8 路送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 `?host=spike`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。

<p class="chips"><a class="chip chip-spike" href="/sensors/line8/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/line8/ev3-hub.md">EV3</a><a class="chip chip-arduino" href="/sensors/line8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/line8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/line8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/line8/microbit.md">micro:bit</a></p>

| 項目 | 內容 |
| :--- | :--- |
| I2C 出廠位址 | `0x16` |
| hw[0] | `8`（F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte） |
| 型號 | `LINE8` |
| 協議版本 | `2` |
| 支援平台 | SPIKE、EV3、Arduino I2C、mBlock、MATRIX、micro:bit |

## 值一覽

| 編號 | 值 | 範圍 | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [線位置](#v01) | -8~+8 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [細位置](#v02) | -100~+100 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [線寬](#v03) | 0~8 路 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [線段數](#v04) | 0~4 | ✓ | — | ✓ | ✓ | — | ✓ |
| [`05`](#v05) | [壓線位元圖](#v05) | 0~255 bit | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`06`](#v06) | [最後離線方向](#v06) | -8~+8 | — | — | ✓ | ✓ | — | ✓ |
| [`07`](#v07) | [二值化門檻](#v07) | 1~99 | — | — | ✓ | ✓ | — | ✓ |
| [`08`](#v08) | [校準完成次數](#v08) | 0~255 次 | — | — | ✓ | ✓ | — | — |
| [`10~17`](#v10) | [第 1~8 路光值](#v10) | 0~100 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。

## 每個值怎麼讀

<section id="v01" class="ref-item">

### `01` 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 線在哪一路：−8（最左）~+8（最右），0 = 正中間；站在車後往前看，負 = 線在左 |
| 範圍 | `-8~+8` |
| I2C／mBlock 編號 | `01` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 1 −8 🔒；模式 0 (toy) byte 0 −8 🔒；模式 1 byte 0 −8；模式 9 v[4] lo −8 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
linePos = (w & 0xFF) - 8   # 線位置（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
linePos = line.pos8()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 0 · int8 · IR-PROX（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

linePos = sensor.read(0)[0]   # 線位置：-8~+8
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
linePos = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t linePos = mbcRead(0x16, 0x01);   // 線位置：-8~+8
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 線位置（-8~+8）。主機送 `[01]` → 回 `[值低][值高][01 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(循行者 連接埠 [1 v] 線的位置 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R (+8)。

```cpp
// A. 原廠 0x22 積木：R = 線位置 + 8
int linePos = MiniR4.I2C1.MXColor.getColor(R) - 8;
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 線的位置 :: #E8711E)</pre>

```typescript
let linePos = MBC.readValue(0x16, 0x01)   // 線位置：-8~+8
```

</details>

**相關範例**：[Arduino 範例 03](/sensors/line8/arduino-examples.md)、[EV3 範例 02](/sensors/line8/ev3-examples.md)、[SPIKE 範例 02](/sensors/line8/spike-examples.md)

</section>

<section id="v02" class="ref-item">

### `02` 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |
| 範圍 | `-100~+100` |
| I2C／mBlock 編號 | `02` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 2 −100 🔒；模式 3 byte 0 −100；模式 4 v[0] −100；模式 9 v[5] lo −100 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
linePosFine = (w & 0xFF) - 100   # 細位置（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
linePosFine = line.pos100()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 2 · int8 · IR-REMOTE byte0 = highRes−100 存成 int8（F030 ev3Uart case 2），主機直接讀（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

linePosFine = sensor.read(2)[0]   # 細位置：-100~+100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
linePosFine = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t linePosFine = mbcRead(0x16, 0x02);   // 細位置：-100~+100
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 細位置（-100~+100）。主機送 `[02]` → 回 `[值低][值高][02 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(循行者 連接埠 [1 v] 線的細位置 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B (+100)。

```cpp
// A. 原廠 0x22 積木：B = 細位置 + 100
int linePosFine = MiniR4.I2C1.MXColor.getColor(B) - 100;
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 線的細位置 :: #E8711E)</pre>

```typescript
let linePosFine = MBC.readValue(0x16, 0x02)   // 細位置：-100~+100
```

</details>

**相關範例**：[Arduino 範例 06](/sensors/line8/arduino-examples.md)、[EV3 範例 04](/sensors/line8/ev3-examples.md)

</section>

<section id="v03" class="ref-item">

### `03` 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 壓到線的路數 0~8，0 = 找不到線 |
| 範圍 | `0~8` 路 |
| I2C／mBlock 編號 | `03` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 1 🔒；模式 2 byte 0；模式 9 v[4] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
lineWidth = ((w >> 8) & 0xFF)   # 線寬（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
lineWidth = line.width()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 2 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

lineWidth = sensor.read(2)[1]   # 線寬：0~8 路
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
lineWidth = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t lineWidth = mbcRead(0x16, 0x03);   // 線寬：0~8 路
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 線寬（0~8 路）。主機送 `[03]` → 回 `[值低][值高][03 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(循行者 連接埠 [1 v] 線寬 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 G。

```cpp
// A. 原廠 0x22 積木：G = 線寬
int lineWidth = MiniR4.I2C1.MXColor.getColor(G);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 線寬 :: #E8711E)</pre>

```typescript
let lineWidth = MBC.readValue(0x16, 0x03)   // 線寬：0~8 路
```

</details>

**相關範例**：[Arduino 範例 03](/sensors/line8/arduino-examples.md)、[EV3 範例 03](/sensors/line8/ev3-examples.md)

</section>

<section id="v04" class="ref-item">

### `04` 線段數 <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="線段數" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |
| 範圍 | `0~4` |
| I2C／mBlock 編號 | `04` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 9 v[5] hi 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
groupCount = ((w >> 8) & 0xFF)   # 線段數（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
groupCount = line.junction()
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
int16_t groupCount = mbcRead(0x16, 0x04);   // 線段數：0~4
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 線段數（0~4）。主機送 `[04]` → 回 `[值低][值高][04 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(循行者 連接埠 [1 v] 線段數 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 線段數 :: #E8711E)</pre>

```typescript
let groupCount = MBC.readValue(0x16, 0x04)   // 線段數：0~4
```

</details>

**相關範例**：[Arduino 範例 05](/sensors/line8/arduino-examples.md)

</section>

<section id="v05" class="ref-item">

### `05` 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | bit0 = 第 1 路（最左）… bit7 = 第 8 路，1 = 那一路在線上 |
| 範圍 | `0~255` bit |
| I2C／mBlock 編號 | `05` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 4 v[1]；模式 9 v[6] lo 🔒

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[6]
bitmap = (w & 0xFF)   # 壓線位元圖（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
bitmap = line.bin_raw()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 3 · int16 · #67 新：IR-REM-A 1×int16

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

bitmap = sensor.read(3)[0]   # 壓線位元圖：0~255 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
bitmap = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t bitmap = mbcRead(0x16, 0x05);   // 壓線位元圖：0~255 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 壓線位元圖（0~255 bit）。主機送 `[05]` → 回 `[值低][值高][05 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(循行者 連接埠 [1 v] 壓線位元圖 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 壓線位元圖 :: #E8711E)</pre>

```typescript
let bitmap = MBC.readValue(0x16, 0x05)   // 壓線位元圖：0~255 bit
```

</details>

**相關範例**：[Arduino 範例 05](/sensors/line8/arduino-examples.md)、[EV3 範例 06](/sensors/line8/ev3-examples.md)

</section>

<section id="v06" class="ref-item">

### `06` 最後離線方向 <small>lastDir</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 丟線前線最後在哪一側：−8 ~ +8（負 = 左），找回線後維持最後一次 |
| 範圍 | `-8~+8` |
| I2C／mBlock 編號 | `06` |

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
int16_t lastDir = mbcRead(0x16, 0x06);   // 最後離線方向：-8~+8
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 最後離線方向（-8~+8）。主機送 `[06]` → 回 `[值低][值高][06 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(循行者 連接埠 [1 v] 最後離線方向 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 最後離線方向 :: #E8711E)</pre>

```typescript
let lastDir = MBC.readValue(0x16, 0x06)   // 最後離線方向：-8~+8
```

</details>

**相關範例**：[Arduino 範例 06](/sensors/line8/arduino-examples.md)

</section>

<section id="v07" class="ref-item">

### `07` 二值化門檻 <small>threshold</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 判斷「在線上」的光值門檻 1~99，預設 50；用 E4 改 |
| 範圍 | `1~99` |
| I2C／mBlock 編號 | `07` |

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
int16_t threshold = mbcRead(0x16, 0x07);   // 二值化門檻：1~99
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 二值化門檻（1~99）。主機送 `[07]` → 回 `[值低][值高][07 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(循行者 連接埠 [1 v] 門檻 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 門檻 :: #E8711E)</pre>

```typescript
let threshold = MBC.readValue(0x16, 0x07)   // 二值化門檻：1~99
```

</details>

**相關範例**：[Arduino 範例 07](/sensors/line8/arduino-examples.md)

</section>

<section id="v08" class="ref-item">

### `08` 校準完成次數 <small>calibCount</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 開機後完成校準的次數（按鈕校準也算） |
| 範圍 | `0~255` 次 |
| I2C／mBlock 編號 | `08` |

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
int16_t calibCount = mbcRead(0x16, 0x08);   // 校準完成次數：0~255 次
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 校準完成次數（0~255 次）。主機送 `[08]` → 回 `[值低][值高][08 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(循行者 連接埠 [1 v] 校準次數 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

micro:bit 擴充沒有這顆積木（可用 `MBC.readValue(位址, 編號)` 通用積木讀）。

</details>

**相關範例**：[Arduino 範例 04](/sensors/line8/arduino-examples.md)

</section>

<section id="v10" class="ref-item">

### `10~17` 第 1~8 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="第 1~8 路光值" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 第 1~8 路校準後光值 0~100（第 1 路最左）；編號 10~17 |
| 範圍 | `0~100` |
| I2C／mBlock 編號 | `10~17` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 4 🔒；模式 9 v[0] 🔒。備註：4-bit (v×10)>>6; 每 int16 兩路：lo = 奇數路、hi = 偶數路

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

words = dev.read(9)[0:4]
light = []
for w in words:
    light.append(w & 0xFF)          # 奇數路
    light.append((w >> 8) & 0xFF)   # 偶數路
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
light = line.ir_calibration()   # list，第 1 路 = [0]
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8 · IR-SEEK 8×int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

light = sensor.read(1)[0:8]   # 第 1~8 路光值
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
light1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x10~0x17 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t light[8];
mbcReadMany(0x16, 0x10, 8, light);   // 第 1~8 路光值：0~100
for (uint8_t i = 0; i < 8; i++) { Serial.print(light[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 第 1~8 路光值（0~100）。主機送 `[10]` → 回 `[值低][值高][10 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。

<pre class="blocks">(循行者 連接埠 [1 v] 第 [1 v] 路光值 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 C~K。

```cpp
// A. 原廠 0x22 積木：C~K = 第 1~8 路光值（C~K 兩路一組 convert9×10+convert9）
int raw = MiniR4.I2C1.MXColor.getColor(C);   // 第 1 組
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 第 [1 v] 路光值 :: #E8711E)</pre>

```typescript
let light = MBC.readValues(0x16, 0x10, 8)   // 第 1~8 路光值
```

</details>

**相關範例**：[Arduino 範例 02](/sensors/line8/arduino-examples.md)、[EV3 範例 01](/sensors/line8/ev3-examples.md)

</section>

## 寫入命令

只有 Arduino I2C、mBlock、micro:bit 能送命令；SPIKE／EV3 接頭是唯讀的。

<section id="ce0" class="ref-item ref-cmd">

### `E0` 切黑線 <small>blackLine</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 追黑線（預設） |
| 參數 | 無 |
| 忙碌旗標 | 無（立即生效） |

**怎麼送**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE0);   // 切黑線
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E0]` → 感應器回 `[00][00][E0]`（收到了）再執行。

<pre class="blocks">循行者 連接埠 [1 v] 切黑線 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個命令。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 切換為黑線 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE0)   // 切黑線
```

</details>

</section>

<section id="ce1" class="ref-item ref-cmd">

### `E1` 切白線 <small>whiteLine</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 追白線（F3 b1 = 1） |
| 參數 | 無 |
| 忙碌旗標 | 無（立即生效） |

**怎麼送**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE1);   // 切白線
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E1]` → 感應器回 `[00][00][E1]`（收到了）再執行。

<pre class="blocks">循行者 連接埠 [1 v] 切白線 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個命令。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 切換為白線 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE1)   // 切白線
```

</details>

</section>

<section id="ce2" class="ref-item ref-cmd">

### `E2` 黑色校準 <small>calibBlack</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM |
| 參數 | 無 |
| 忙碌旗標 | F3 bit2（執行中 = 1） |

**怎麼送**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE2);   // 黑色校準
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // 等 F3 bit2 歸零 = 做完
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E2]` → 感應器回 `[00][00][E2]`（收到了）再執行；執行期間 F3 bit2 = 1。

<pre class="blocks">循行者 連接埠 [1 v] 黑色校準 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個命令。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 黑色校準 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE2)   // 黑色校準
```

</details>

</section>

<section id="ce3" class="ref-item ref-cmd">

### `E3` 白色校準 <small>calibWhite</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM |
| 參數 | 無 |
| 忙碌旗標 | F3 bit2（執行中 = 1） |

**怎麼送**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE3);   // 白色校準
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // 等 F3 bit2 歸零 = 做完
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E3]` → 感應器回 `[00][00][E3]`（收到了）再執行；執行期間 F3 bit2 = 1。

<pre class="blocks">循行者 連接埠 [1 v] 白色校準 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個命令。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 白色校準 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE3)   // 白色校準
```

</details>

</section>

<section id="ce4" class="ref-item ref-cmd">

### `E4` 設定門檻 <small>setThreshold</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 二值化門檻 1~99（RAM，重開機回預設 50） |
| 參數 | 門檻 `1~99`（1 byte） |
| 忙碌旗標 | 無（立即生效） |

**怎麼送**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE4, 50);   // 設定門檻（門檻 1~99）
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E4][參數…]` → 感應器回 `[00][00][E4]`（收到了）再執行。

<pre class="blocks">循行者 連接埠 [1 v] 設定門檻 (50) :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個命令。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 門檻設為 (50) :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE4, [50])   // 設定門檻
```

</details>

</section>

<section id="ce7" class="ref-item ref-cmd">

### `E7` 5 秒動態校準 <small>calibAuto</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1 |
| 參數 | 無 |
| 忙碌旗標 | F3 bit2（執行中 = 1） |

**怎麼送**

<details class="plat plat-spike" data-host="spike">
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x16, 0xE7);   // 5 秒動態校準
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // 等 F3 bit2 歸零 = 做完
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E7]` → 感應器回 `[00][00][E7]`（收到了）再執行；執行期間 F3 bit2 = 1。

<pre class="blocks">循行者 連接埠 [1 v] 5 秒動態校準 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個命令。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 5 秒動態校準 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE7)   // 5 秒動態校準
```

</details>

</section>

<section id="flags" class="ref-item">

## 狀態旗標 F3

`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀。

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 白線模式 (`whiteMode`) | 1 = 正在追白線 |
| b2 | 校準中 (`calibrating`) | E2／E3／E7 執行中；此時拒收新的校準命令 |
| b3 | 丟線 (`lost`) | 線段數 = 0 |
| b4 | 分岔 (`fork`) | 線段數 ≥ 2 |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

id_word, proto, fw, seq = dev.read(8)   # 模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]
addr = id_word & 0xFF   # 0x16
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
addr = id_word & 0xFF             # 0x16
hw0 = id_word >> 8                # 8
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x16, 0xF3);
bool ready = st & (1 << 0);   // 就緒
bool whiteMode = st & (1 << 1);   // 白線模式
bool calibrating = st & (1 << 2);   // 校準中
bool lost = st & (1 << 3);   // 丟線
bool fork = st & (1 << 4);   // 分岔
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

狀態旗標 F3 的各 bit 由擴展拆成布林積木（就緒、校準中…）。

<pre class="blocks"><循行者 連接埠 [1 v] 連線正常？ :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木讀不到狀態旗標。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC 裝置就緒 :: #E8711E></pre>

```typescript
let st = MBC.status(0x16)   // F3
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

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/line8.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- B-11：細位置 02 與線位置 01 同號（負 = 左）；v1 韌體註解寫反，程式碼同號。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- SPIKE 教育版模式 0 的 byte 8~15 補 0（#65 已改，使用者保留）。
- EV3 模式 4 與模式 2 相同內容；模式 3 改壓線位元圖（#67）。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體，0x0B 回 1。
