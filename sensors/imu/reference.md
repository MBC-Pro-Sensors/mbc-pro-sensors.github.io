<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/imu.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/imu/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/imu/index.md">陀螺儀 IMU 九軸</a></p>

# IMU 陀螺儀 指令參考

[🔙 回到IMU 陀螺儀產品頁](/sensors/imu/index.md)

這一頁列出IMU 陀螺儀送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 `?host=spike`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。

<p class="chips"><span class="chip chip-spike off">SPIKE</span><a class="chip chip-ev3" href="/sensors/imu/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/imu/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/imu/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/imu/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/imu/microbit.md">micro:bit</a></p>

| 項目 | 內容 |
| :--- | :--- |
| I2C 出廠位址 | `0x6A` |
| hw[0] | `1`（F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte、大通道 byte 28） |
| 型號 | `IMU` |
| 協議版本 | `2` |
| 支援平台 | EV3、Arduino I2C、mBlock、MATRIX、micro:bit |

## 值一覽

| 編號 | 值 | 範圍 | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [陀螺 3D 航向](#v01) | -32768~32767 ° | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [融合航向](#v02) | -32768~32767 ° | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [陀螺 1D 航向](#v03) | -32768~32767 ° | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [磁力航向](#v04) | -32768~32767 ° | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`05`](#v05) | [轉向軸角速度](#v05) | -32768~32767 0.1 dps | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`06`](#v06) | [俯仰角](#v06) | -90~+90 ° | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`07`](#v07) | [俯仰角速度](#v07) | -32768~32767 0.1 dps | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`08`](#v08) | [磁力信任度](#v08) | 0~255 | — | ✓ | ✓ | — | ✓ |
| [`09`](#v09) | [磁場干擾度](#v09) | 0~255 0.01° | — | ✓ | ✓ | — | ✓ |
| [`0A`](#v0a) | [朝上的面](#v0a) | 0~5 | — | ✓ | ✓ | — | ✓ |
| [`10~12`](#v10) | [角速度 1~3 軸](#v10) | -32768~32767 0.1 dps | — | ✓ | ✓ | — | ✓ |
| [`13~15`](#v13) | [加速度 1~3 軸](#v13) | -32768~32767 mg | — | ✓ | ✓ | — | ✓ |
| [`16~18`](#v16) | [磁力 1~3 軸](#v16) | -32768~32767 mG | — | ✓ | ✓ | — | ✓ |

✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。

## 每個值怎麼讀

<section id="v01" class="ref-item">

### `01` 陀螺 3D 航向 <small>headingGyro3D</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |
| 範圍 | `-32768~32767` ° |
| I2C／mBlock 編號 | `01` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

模式 0 · int16 · GYRO-ANG 1×int16（盤點：mode0 = 陀螺 3D，INO:232-234）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

headingGyro3D = sensor.read(0)[0]   # 陀螺 3D 航向：-32768~32767 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
headingGyro3D = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t headingGyro3D = mbcRead(0x6A, 0x01);   // 陀螺 3D 航向：-32768~32767 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 陀螺 3D 航向（-32768~32767 °）。主機送 `[01]` → 回 `[值低][值高][01 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 陀螺 3D 航向 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R~G (+32768)；大通道 byte 0~1 i16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// A. 原廠 0x22 積木：R~G = 陀螺 3D 航向（R = (航向+32768) 低 byte、G = 高 byte（R4 大通道版：低在前）；主機 hi×256+lo−32768）
int raw = MiniR4.I2C1.MXColor.getColor(R);   // 第 1 組
```

**B. 大通道（LauMo 函式庫）**

```cpp
// B. 大通道 byte 0~1：陀螺 3D 航向（鎖住（R4 大通道版已流出））
int headingGyro3D = bigI16(0);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 陀螺 3D 航向 :: #E8711E)</pre>

```typescript
let headingGyro3D = MBC.readValue(0x6A, 0x01)   // 陀螺 3D 航向：-32768~32767 °
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` 融合航向 <small>headingFused</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |
| 範圍 | `-32768~32767` ° |
| I2C／mBlock 編號 | `02` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int16 · GYRO-RATE 1×int16 = 融合航向（EV3 定案：不改）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

headingFused = sensor.read(1)[0]   # 融合航向：-32768~32767 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
headingFused = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t headingFused = mbcRead(0x6A, 0x02);   // 融合航向：-32768~32767 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 融合航向（-32768~32767 °）。主機送 `[02]` → 回 `[值低][值高][02 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 融合航向 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B~C (+32768)；大通道 byte 2~3 i16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// A. 原廠 0x22 積木：B~C = 融合航向（B = 低 byte、C = 高 byte）
int raw = MiniR4.I2C1.MXColor.getColor(B);   // 第 1 組
```

**B. 大通道（LauMo 函式庫）**

```cpp
// B. 大通道 byte 2~3：融合航向（鎖住）
int headingFused = bigI16(2);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 融合航向 :: #E8711E)</pre>

```typescript
let headingFused = MBC.readValue(0x6A, 0x02)   // 融合航向：-32768~32767 °
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` 陀螺 1D 航向 <small>headingGyro1D</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |
| 範圍 | `-32768~32767` ° |
| I2C／mBlock 編號 | `03` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

模式 3 · int16 · GYRO-G&A 2×int16 [陀螺 1D, 磁力航向]（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

headingGyro1D = sensor.read(3)[0]   # 陀螺 1D 航向：-32768~32767 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
headingGyro1D = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t headingGyro1D = mbcRead(0x6A, 0x03);   // 陀螺 1D 航向：-32768~32767 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 陀螺 1D 航向（-32768~32767 °）。主機送 `[03]` → 回 `[值低][值高][03 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 陀螺 1D 航向 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 M~Y (+32768)；大通道 byte 4~5 i16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// A. 原廠 0x22 積木：M~Y = 陀螺 1D 航向（M = 低 byte、Y = 高 byte；K（10）= 0）
int raw = MiniR4.I2C1.MXColor.getColor(M);   // 第 1 組
```

**B. 大通道（LauMo 函式庫）**

```cpp
// B. 大通道 byte 4~5：陀螺 1D 航向（鎖住）
int headingGyro1D = bigI16(4);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 陀螺 1D 航向 :: #E8711E)</pre>

```typescript
let headingGyro1D = MBC.readValue(0x6A, 0x03)   // 陀螺 1D 航向：-32768~32767 °
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` 磁力航向 <small>headingMag</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 磁力計傾斜補償航向，度，減去相對零點 |
| 範圍 | `-32768~32767` ° |
| I2C／mBlock 編號 | `04` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

模式 3 · int16（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

headingMag = sensor.read(3)[1]   # 磁力航向：-32768~32767 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
headingMag = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t headingMag = mbcRead(0x6A, 0x04);   // 磁力航向：-32768~32767 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 磁力航向（-32768~32767 °）。主機送 `[04]` → 回 `[值低][值高][04 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 磁力航向 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 6~7 i16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 6~7：磁力航向（鎖住）
int headingMag = bigI16(6);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 磁力航向 :: #E8711E)</pre>

```typescript
let headingMag = MBC.readValue(0x6A, 0x04)   // 磁力航向：-32768~32767 °
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` 轉向軸角速度 <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="轉向軸角速度" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 繞轉向軸（getYawAxis()）的角速度，0.1 度／秒，正負跟航向同向 |
| 範圍 | `-32768~32767` 0.1 dps |
| I2C／mBlock 編號 | `05` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

模式 2 · int16 · GYRO-FAS 1×int16（#67 新增，原本 0）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

yawRate = sensor.read(2)[0]   # 轉向軸角速度：-32768~32767 0.1 dps
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
yawRate = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t yawRate = mbcRead(0x6A, 0x05);   // 轉向軸角速度：-32768~32767 0.1 dps
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 轉向軸角速度（-32768~32767 0.1 dps）。主機送 `[05]` → 回 `[值低][值高][05 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 轉向角速度 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 8~9 i16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 8~9：轉向軸角速度（#67 追加（Hue 積木））
int yawRate = bigI16(8);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 轉向角速度 :: #E8711E)</pre>

```typescript
let yawRate = MBC.readValue(0x6A, 0x05)   // 轉向軸角速度：-32768~32767 0.1 dps
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` 俯仰角 <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="俯仰角" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 板子絲印 X 軸相對水平面的仰角：X 軸朝上 +90、朝下 −90、水平 0（用重力向量 atan2 算） |
| 範圍 | `-90~+90` ° |
| I2C／mBlock 編號 | `06` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

模式 6 · int16 · TILT-ANG 1×int16（#67 新增，原本 0）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

pitch = sensor.read(6)[0]   # 俯仰角：-90~+90 °
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 6)
pitch = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t pitch = mbcRead(0x6A, 0x06);   // 俯仰角：-90~+90 °
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 俯仰角（-90~+90 °）。主機送 `[06]` → 回 `[值低][值高][06 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 俯仰角 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 10~11 i16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 10~11：俯仰角（#67 追加）
int pitch = bigI16(10);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 俯仰角 :: #E8711E)</pre>

```typescript
let pitch = MBC.readValue(0x6A, 0x06)   // 俯仰角：-90~+90 °
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` 俯仰角速度 <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="俯仰角速度" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 繞板子絲印 X 軸的角速度，0.1 度／秒（右手定則，從 +X 端看逆時針為正）；注意這是 gyro X，不是 06 的微分 |
| 範圍 | `-32768~32767` 0.1 dps |
| I2C／mBlock 編號 | `07` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

模式 5 · int16 · TILT-RATE 1×int16（#67：取代 #65 統一時放的 ID，ID 移到模式 4）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

pitchRate = sensor.read(5)[0]   # 俯仰角速度：-32768~32767 0.1 dps
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
pitchRate = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t pitchRate = mbcRead(0x6A, 0x07);   // 俯仰角速度：-32768~32767 0.1 dps
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 俯仰角速度（-32768~32767 0.1 dps）。主機送 `[07]` → 回 `[值低][值高][07 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 俯仰角速度 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 12~13 i16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 12~13：俯仰角速度（#67 追加）
int pitchRate = bigI16(12);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 俯仰角速度 :: #E8711E)</pre>

```typescript
let pitchRate = MBC.readValue(0x6A, 0x07)   // 俯仰角速度：-32768~32767 0.1 dps
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` 磁力信任度 <small>magTrust</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magTrust.svg" alt="磁力信任度" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 融合演算法對磁力計的信任度 0~255（= 0~1 × 255） |
| 範圍 | `0~255` |
| I2C／mBlock 編號 | `08` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t magTrust = mbcRead(0x6A, 0x08);   // 磁力信任度：0~255
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 磁力信任度（0~255）。主機送 `[08]` → 回 `[值低][值高][08 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 磁力信任度 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 磁力信任度 :: #E8711E)</pre>

```typescript
let magTrust = MBC.readValue(0x6A, 0x08)   // 磁力信任度：0~255
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` 磁場干擾度 <small>magDisturb</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magDisturb.svg" alt="磁場干擾度" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 磁場干擾程度 0~255（度 × 100，上限 255） |
| 範圍 | `0~255` 0.01° |
| I2C／mBlock 編號 | `09` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t magDisturb = mbcRead(0x6A, 0x09);   // 磁場干擾度：0~255 0.01°
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 磁場干擾度（0~255 0.01°）。主機送 `[09]` → 回 `[值低][值高][09 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 磁場干擾度 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 磁場干擾度 :: #E8711E)</pre>

```typescript
let magDisturb = MBC.readValue(0x6A, 0x09)   // 磁場干擾度：0~255 0.01°
```

</details>

</section>

<section id="v0a" class="ref-item">

### `0A` 朝上的面 <small>upFace</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-upFace.svg" alt="朝上的面" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 哪個軸朝上：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z（實體軸） |
| 範圍 | `0~5` |
| I2C／mBlock 編號 | `0A` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t upFace = mbcRead(0x6A, 0x0A);   // 朝上的面：0~5
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 朝上的面（0~5）。主機送 `[0A]` → 回 `[值低][值高][0A ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 朝上的面 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 朝上的面 :: #E8711E)</pre>

```typescript
let upFace = MBC.readValue(0x6A, 0x0A)   // 朝上的面：0~5
```

</details>

</section>

<section id="v10" class="ref-item">

### `10~12` 角速度 1~3 軸 <small>gyro</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-gyro.svg" alt="角速度 1~3 軸" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 10 X、11 Y、12 Z 角速度，0.1 度／秒（實體軸，已套軸向號誌） |
| 範圍 | `-32768~32767` 0.1 dps |
| I2C／mBlock 編號 | `10~12` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x10~0x12 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t gyro[3];
mbcReadMany(0x6A, 0x10, 3, gyro);   // 角速度 1~3 軸：-32768~32767 0.1 dps
for (uint8_t i = 0; i < 3; i++) { Serial.print(gyro[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 角速度 1~3 軸（-32768~32767 0.1 dps）。主機送 `[10]` → 回 `[值低][值高][10 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 角速度 [軸] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 角速度 [軸] :: #E8711E)</pre>

```typescript
let gyro = MBC.readValues(0x6A, 0x10, 3)   // 角速度 1~3 軸
```

</details>

</section>

<section id="v13" class="ref-item">

### `13~15` 加速度 1~3 軸 <small>accel</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-accel.svg" alt="加速度 1~3 軸" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 13 X、14 Y、15 Z 加速度，mg（1000 = 1 g） |
| 範圍 | `-32768~32767` mg |
| I2C／mBlock 編號 | `13~15` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x13~0x15 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t accel[3];
mbcReadMany(0x6A, 0x13, 3, accel);   // 加速度 1~3 軸：-32768~32767 mg
for (uint8_t i = 0; i < 3; i++) { Serial.print(accel[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 加速度 1~3 軸（-32768~32767 mg）。主機送 `[13]` → 回 `[值低][值高][13 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x13 + (N − 1)。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 加速度 [軸] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 加速度 [軸] :: #E8711E)</pre>

```typescript
let accel = MBC.readValues(0x6A, 0x13, 3)   // 加速度 1~3 軸
```

</details>

</section>

<section id="v16" class="ref-item">

### `16~18` 磁力 1~3 軸 <small>mag</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-mag.svg" alt="磁力 1~3 軸" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 16 X、17 Y、18 Z 磁場強度，mG（毫高斯） |
| 範圍 | `-32768~32767` mG |
| I2C／mBlock 編號 | `16~18` |

**怎麼讀**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 模式沒有送這個值（EV3 封包以已鎖住的模式為準）。需要時改用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x16~0x18 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t mag[3];
mbcReadMany(0x6A, 0x16, 3, mag);   // 磁力 1~3 軸：-32768~32767 mG
for (uint8_t i = 0; i < 3; i++) { Serial.print(mag[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 磁力 1~3 軸（-32768~32767 mG）。主機送 `[16]` → 回 `[值低][值高][16 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x16 + (N − 1)。

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 磁力 [軸] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 磁力 [軸] :: #E8711E)</pre>

```typescript
let mag = MBC.readValues(0x6A, 0x16, 3)   // 磁力 1~3 軸
```

</details>

</section>

## 寫入命令

只有 Arduino I2C、mBlock、micro:bit（與 MATRIX 大通道的 F1／F2）能送命令；SPIKE／EV3 接頭是唯讀的。

<section id="ce0" class="ref-item ref-cmd">

### `E0` 航向歸零 <small>zeroHeading</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 四個航向都歸零（zeroHeadings()）；EV3 另有原廠 WRITE 0x11 陀螺歸零 |
| 參數 | 無 |
| 忙碌旗標 | 無（立即生效） |

**怎麼送**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE0);   // 航向歸零
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E0]` → 感應器回 `[00][00][E0]`（收到了）再執行。

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 航向歸零 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

只有大通道函式庫有這個動作；0x22 原廠函式庫沒有。

```cpp
MiniR4.I2C1.MXColor.getColor(C);   // 大通道 F1~F4（任一個都可以，這裡用 F1 = Cyan）= 航向歸零，回傳上一包的完成次數
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 航向歸零 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE0)   // 航向歸零
```

</details>

</section>

<section id="ce1" class="ref-item ref-cmd">

### `E1` 加速度計校準開始 <small>accelCalStart</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 開始加速度計校準（= v1 的 0x20=0x01）；期間 F3 b4 = 1 |
| 參數 | 無 |
| 忙碌旗標 | F3 bit4（執行中 = 1） |

**怎麼送**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE1);   // 加速度計校準開始
while (mbcRead(0x6A, 0xF3) & (1 << 4)) delay(10);   // 等 F3 bit4 歸零 = 做完
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E1]` → 感應器回 `[00][00][E1]`（收到了）再執行；執行期間 F3 bit4 = 1。

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 加速度計校準開始 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 沒有這個命令（大通道只有 F1／F2 兩個動作）。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 加速度計校準開始 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE1)   // 加速度計校準開始
```

</details>

</section>

<section id="ce2" class="ref-item ref-cmd">

### `E2` 加速度計校準結束 <small>accelCalEnd</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 結束加速度計校準並存 EEPROM（= v1 的 0x20=0x02）；通過後自動航向歸零一次 |
| 參數 | 無 |
| 忙碌旗標 | 無（立即生效） |

**怎麼送**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE2);   // 加速度計校準結束
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E2]` → 感應器回 `[00][00][E2]`（收到了）再執行。

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 加速度計校準結束 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 沒有這個命令（大通道只有 F1／F2 兩個動作）。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 加速度計校準結束 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE2)   // 加速度計校準結束
```

</details>

</section>

<section id="ce3" class="ref-item ref-cmd">

### `E3` 磁力計校準開始 <small>magCalStart</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 開始磁力計校準（= v1 的 0x20=0x03）；期間 F3 b5 = 1 |
| 參數 | 無 |
| 忙碌旗標 | F3 bit5（執行中 = 1） |

**怎麼送**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE3);   // 磁力計校準開始
while (mbcRead(0x6A, 0xF3) & (1 << 5)) delay(10);   // 等 F3 bit5 歸零 = 做完
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E3]` → 感應器回 `[00][00][E3]`（收到了）再執行；執行期間 F3 bit5 = 1。

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 磁力計校準開始 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 沒有這個命令（大通道只有 F1／F2 兩個動作）。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 磁力計校準開始 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE3)   // 磁力計校準開始
```

</details>

</section>

<section id="ce4" class="ref-item ref-cmd">

### `E4` 磁力計校準結束 <small>magCalEnd</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | 結束磁力計校準並存 EEPROM（= v1 的 0x20=0x04） |
| 參數 | 無 |
| 忙碌旗標 | 無（立即生效） |

**怎麼送**

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

EV3 接頭沒有寫入命令；校準用感應器上的按鈕，設定用 Arduino I2C／mBlock。

</details>

<details class="plat plat-arduino" data-host="arduino" open>
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
mbcWrite(0x6A, 0xE4);   // 磁力計校準結束
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E4]` → 感應器回 `[00][00][E4]`（收到了）再執行。

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 磁力計校準結束 :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 沒有這個命令（大通道只有 F1／F2 兩個動作）。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 磁力計校準結束 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x6A, 0xE4)   // 磁力計校準結束
```

</details>

</section>

<section id="flags" class="ref-item">

## 狀態旗標 F3

`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀，MATRIX 大通道是 byte 24。

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 靜止 (`still`) | 1 = 感應器靜止中 |
| b2 | 已擷取安裝軸 (`axisCaptured`) | 1 = 已判定安裝方向（首次擷取後自動歸零一次） |
| b3 | 安裝過歪 (`mountTilted`) | 1 = 安裝角度偏離太多 |
| b4 | 加速度計校準中 (`accelCalibrating`) | E1 之後、E2 之前 = 1 |
| b5 | 磁力計校準中 (`magCalibrating`) | E3 之後、E4 之前 = 1 |
| b6 | 磁力融合已初始化 (`fusionInit`) | 1 = 融合航向可用 |
| b7 | 磁力自動校準已收斂 (`magAutoCalDone`) | 1 = 磁力計自動校準偏移已收斂 |

<details class="plat plat-ev3" data-host="ev3" open>
<summary><span class="plat-dot"></span>EV3</summary>

EV3 沒有 F3 旗標；模式 5 回裝置 ID 與韌體版本（ID 格式 B）。

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

id_word, fw = sensor.read(5)   # 模式 5：[ID | hw0<<8, 主版×100+次版]
addr = id_word & 0xFF             # 0x6A
hw0 = id_word >> 8                # 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x6A, 0xF3);
bool ready = st & (1 << 0);   // 就緒
bool still = st & (1 << 1);   // 靜止
bool axisCaptured = st & (1 << 2);   // 已擷取安裝軸
bool mountTilted = st & (1 << 3);   // 安裝過歪
bool accelCalibrating = st & (1 << 4);   // 加速度計校準中
bool magCalibrating = st & (1 << 5);   // 磁力計校準中
bool fusionInit = st & (1 << 6);   // 磁力融合已初始化
bool magAutoCalDone = st & (1 << 7);   // 磁力自動校準已收斂
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

狀態旗標 F3 的各 bit 由擴展拆成布林積木（就緒、校準中…）。

<pre class="blocks"><IMU 陀螺儀 連接埠 [1 v] 連線正常？ :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

已流出的大通道格式鎖住，狀態 byte 維持 0。

```cpp
uint8_t st = MiniR4.I2C1.MXColor.getColorID();   // 這款已流出：byte 24 維持 0
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC 裝置就緒 :: #E8711E></pre>

```typescript
let st = MBC.status(0x6A)   // F3
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

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/imu.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 1（共識 #6）；EV3 模式 4 = [0x6A | 1<<8, 韌體主×100+次, 0, 0]（格式 B 再補 2 個 0），模式 5 改送俯仰角速度（取代 #65 放的 ID）。
- EV3 鎖住模式 0／1／3 照盤點程式碼：0 = 陀螺 3D、1 = 融合航向、3 = [陀螺 1D, 磁力航向]（INO:213 註解寫反，以程式為準）。
- 0x22 照 R4 大通道版（出貨版）鎖住：命令 4~9 = 三個航向 +32768 的「低 byte 在前、高 byte 在後」；盤點檔分析的是主韌體（高在前），主韌體 #67 改成跟 R4 版逐 byte 相同。命令 10~12 = 0；0x01、0x0A、0x0F、F1~F4 都觸發航向歸零（刻意，不動）。
- 大通道 [0..7] 四個航向 int16 鎖住；[8..9] 轉向軸角速度、[10..11] 俯仰角、[12..13] 俯仰角速度為 #67 追加；[14..23] 與尾段 24~28 維持 0（教練程式可能讀 ColorID／Cyan 回傳值，不放 F3／ID）。
- SPIKE 不做（hosts.spike = false，使用者先前決定），所有值 spike = []。
- B-13：E1~E4 加速度計／磁力計校準開始／結束補回（v1 的 0x20 = 01/02/03/04），I2C 定案 §3 只列 E0。busyFlag：E1 → b4、E3 → b5。
- 俯仰角範圍定案沒寫（韌體尚無現成值，#67 用 atan2 新算），range 先填「°」不給 min/max，韌體做好後再補。
- 磁力信任度 08 = round(trust×255)，磁場干擾度 09 = round(disturbance×100) 上限 255，朝上的面 0A = 軸號 + 3×負向：公式照盤點 I2C v1 0x34~0x36。
- 角速度 10~12 = round(dps×SIGN×10)、加速度 13~15 = mm/s²×1000/9806.65、磁力 16~18 = raw×SIGN×4/15：公式照盤點 I2C v1 0x22~0x24；mBlock 一併有（I2C 定案 §4 共用值表）。
- 主對話 2026-10-22：06 俯仰角範圍 −90~+90、07 = gyro X（照字面），取自 F5 韌體實作。
