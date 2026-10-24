<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps4.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/ps4/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/ps4/index.md">掌控者 PS4/PS5</a></p>

# 掌控者 PS4／PS5 接收器 指令參考

[🔙 回到掌控者 PS4／PS5 接收器產品頁](/sensors/ps4/index.md)

這一頁列出掌控者 PS4／PS5 接收器送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 `?host=spike`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。

<p class="chips"><a class="chip chip-spike" href="/sensors/ps4/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/ps4/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/ps4/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/ps4/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/ps4/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/ps4/microbit.md">micro:bit</a></p>

| 項目 | 內容 |
| :--- | :--- |
| I2C 出廠位址 | `0x42` |
| hw[0] | `2`（F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte、大通道 byte 28） |
| 型號 | `PS4` |
| 協議版本 | `2` |
| 支援平台 | SPIKE、EV3、Arduino I2C、mBlock、MATRIX、micro:bit |

## 值一覽

| 編號 | 值 | 範圍 | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [十六鍵](#v01) | 0~65535 bit | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| [`02`](#v02) | [方向鍵](#v02) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [形狀鍵](#v03) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`04`](#v04) | [肩鍵](#v04) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`05`](#v05) | [功能鍵](#v05) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`06`](#v06) | [已連線](#v06) | 0~1 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`07`](#v07) | [電量](#v07) | 0~100 % | ✓ | — | ✓ | ✓ | ✓ | ✓ |
| [`08`](#v08) | [L2 類比](#v08) | 0~255 | — | — | ✓ | ✓ | — | ✓ |
| [`09`](#v09) | [R2 類比](#v09) | 0~255 | — | — | ✓ | ✓ | — | ✓ |
| [`0A`](#v0a) | [系統鍵](#v0a) | 0~3 bit | — | — | ✓ | ✓ | — | ✓ |
| [`0B`](#v0b) | [手把類型](#v0b) | 0~4 | — | — | ✓ | ✓ | — | ✓ |
| [`10~13`](#v10) | [搖桿 1~4 軸](#v10) | -100~+100 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。

## 每個值怎麼讀

<section id="v01" class="ref-item">

### `01` 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 全部按鍵一個 16 bit（PSX 排列）：bit0 SELECT(SHARE)、1 L3、2 R3、3 START(OPTIONS)、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；1 = 按下；未連線 = 0 |
| 範圍 | `0~65535` bit |
| I2C／mBlock 編號 | `01` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 6 🔒；模式 6 v[2]；模式 9 v[6] 🔒。備註：PSX 排列（#65 已改，原本是 Bluepad32 排列）; PSX 排列; PSX 排列（D3，PS2／PS4 相同；函式庫 buttons_psx()）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

buttons16 = dev.read(9)[6]   # 十六鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
buttons16 = pad.buttons_psx()
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
int16_t buttons16 = mbcRead(0x42, 0x01);   // 十六鍵：0~65535 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 十六鍵（0~65535 bit）。主機送 `[01]` → 回 `[值低][值高][01 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 十六鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 8~9 u16。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 8~9：十六鍵（PSX 排列（Saturation 積木讀 2 byte））
int buttons16 = bigU16(8);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 十六鍵 :: #E8711E)</pre>

```typescript
let buttons16 = MBC.readValue(0x42, 0x01)   // 十六鍵：0~65535 bit
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 上 1、右 2、下 4、左 8，同時按相加（0~15） |
| 範圍 | `0~15` bit |
| I2C／mBlock 編號 | `02` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 0 🔒；模式 1 byte 0；模式 6 v[0] lo；模式 7 v[0] lo 🔒；模式 9 v[0] lo 🔒。備註：低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵; 低 nibble；高 nibble = 形狀鍵

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
dpad = (w & 0xFF)   # 方向鍵（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
dpad = pad.pressed('UP')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 0 · int8 · IR-PROX 1×int8；模式 1 · int8 · IR-SEEK [方向鍵, LX, 形狀鍵, LY, 肩鍵, RX, 功能鍵, RY]；模式 3 · int16 · 低 nibble；高 nibble = 形狀鍵；bit8 = 已連線（+256，#67 新增）；模式 4 · int8 · IR-S-ALT [方向鍵, LX, 形狀鍵, LY]（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

dpad = sensor.read(0)[0]   # 方向鍵：0~15 bit
dpad = sensor.read(1)[0]   # 方向鍵：0~15 bit
dpad = sensor.read(3)[0]   # 方向鍵：0~15 bit
dpad = sensor.read(4)[0]   # 方向鍵：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 1)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
dpad = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t dpad = mbcRead(0x42, 0x02);   // 方向鍵：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 方向鍵（0~15 bit）。主機送 `[02]` → 回 `[值低][值高][02 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 方向鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 R；大通道 byte 0 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// A. 原廠 0x22 積木：R = 方向鍵（R = 方向鍵 | 形狀鍵<<4（上 1 右 2 下 4 左 8、▲16 ●32 ✕64 ■128））
int dpad = MiniR4.I2C1.MXColor.getColor(R);
```

**B. 大通道（LauMo 函式庫）**

```cpp
// B. 大通道 byte 0：方向鍵
int dpad = bigU8(0);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 方向鍵 :: #E8711E)</pre>

```typescript
let dpad = MBC.readValue(0x42, 0x02)   // 方向鍵：0~15 bit
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` 形狀鍵 <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="形狀鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | ▲ 1、● 2、✕ 4、■ 8，同時按相加（0~15） |
| 範圍 | `0~15` bit |
| I2C／mBlock 編號 | `03` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 0 🔒；模式 1 byte 0；模式 6 v[0] hi；模式 7 v[0] hi 🔒；模式 9 v[0] lo 🔒。備註：高 nibble; 高 nibble; 高 nibble; 高 nibble

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
shapeKeys = (w & 0xFF)   # 形狀鍵（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
shapeKeys = pad.pressed('TRIANGLE')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 3 · int16 · 高 nibble（÷16 取整數再 mod 16）；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

shapeKeys = sensor.read(1)[2]   # 形狀鍵：0~15 bit
shapeKeys = sensor.read(3)[0]   # 形狀鍵：0~15 bit
shapeKeys = sensor.read(4)[2]   # 形狀鍵：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shapeKeys = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 3)
shapeKeys = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
shapeKeys = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t shapeKeys = mbcRead(0x42, 0x03);   // 形狀鍵：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 形狀鍵（0~15 bit）。主機送 `[03]` → 回 `[值低][值高][03 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 形狀鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 1 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 1：形狀鍵
int shapeKeys = bigU8(1);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 形狀鍵 :: #E8711E)</pre>

```typescript
let shapeKeys = MBC.readValue(0x42, 0x03)   // 形狀鍵：0~15 bit
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` 肩鍵 <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="肩鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | L2 1、R2 2、L1 4、R1 8，同時按相加（0~15） |
| 範圍 | `0~15` bit |
| I2C／mBlock 編號 | `04` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒；模式 2 byte 0；模式 6 v[1] lo；模式 7 v[1] lo 🔒；模式 9 v[1] lo 🔒。備註：低 nibble；高 nibble = 功能鍵; 低 nibble；高 nibble = 功能鍵; 低 nibble；高 nibble = 功能鍵

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
shoulderKeys = (w & 0xFF)   # 肩鍵（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
shoulderKeys = pad.pressed('L1')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

shoulderKeys = sensor.read(1)[4]   # 肩鍵：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shoulderKeys = Sensor.ReadRawValue(1, 4)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t shoulderKeys = mbcRead(0x42, 0x04);   // 肩鍵：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 肩鍵（0~15 bit）。主機送 `[04]` → 回 `[值低][值高][04 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 肩鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 G；大通道 byte 2 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// A. 原廠 0x22 積木：G = 肩鍵（G = 肩鍵 | 功能鍵<<4（L2 1 R2 2 L1 4 R1 8、SELECT 16 L3 32 R3 64 START 128））
int shoulderKeys = MiniR4.I2C1.MXColor.getColor(G);
```

**B. 大通道（LauMo 函式庫）**

```cpp
// B. 大通道 byte 2：肩鍵
int shoulderKeys = bigU8(2);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 肩鍵 :: #E8711E)</pre>

```typescript
let shoulderKeys = MBC.readValue(0x42, 0x04)   // 肩鍵：0~15 bit
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` 功能鍵 <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="功能鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | SELECT(SHARE) 1、L3 2、R3 4、START(OPTIONS) 8，同時按相加（0~15） |
| 範圍 | `0~15` bit |
| I2C／mBlock 編號 | `05` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 1 🔒；模式 2 byte 0；模式 6 v[1] hi；模式 7 v[1] hi 🔒；模式 9 v[1] lo 🔒。備註：高 nibble; 單鍵編號 1~8（掃 功能鍵<<4|肩鍵 的 bit0→7：1 L2 2 R2 3 L1 4 R1 5 SELECT 6 L3 7 R3 8 START）; 高 nibble; 高 nibble

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
funcKeys = (w & 0xFF)   # 功能鍵（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
funcKeys = pad.pressed('START')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

funcKeys = sensor.read(1)[6]   # 功能鍵：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
funcKeys = Sensor.ReadRawValue(1, 6)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t funcKeys = mbcRead(0x42, 0x05);   // 功能鍵：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 功能鍵（0~15 bit）。主機送 `[05]` → 回 `[值低][值高][05 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 功能鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 3 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 3：功能鍵
int funcKeys = bigU8(3);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 功能鍵 :: #E8711E)</pre>

```typescript
let funcKeys = MBC.readValue(0x42, 0x05)   // 功能鍵：0~15 bit
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` 已連線 <small>connected</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 1 = 手把已連線且 300 ms 內有資料，0 = 未連線（斷線時按鍵 0、搖桿中間） |
| 範圍 | `0~1` |
| I2C／mBlock 編號 | `06` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 3 byte 0。備註：#67 改：模式 3 = [已連線, 電量, 0]（3×int8）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

connected = dev.read(3)[0]   # 已連線
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
connected = pad.connected()
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 3 · int16 · bit8（值 ≥ 256 = 已連線，#67 新增）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

connected = sensor.read(3)[0]   # 已連線：0~1
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
connected = Sensor.ReadRawValue(1, 0)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t connected = mbcRead(0x42, 0x06);   // 已連線：0~1
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 已連線（0~1）。主機送 `[06]` → 回 `[值低][值高][06 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 手把已連線？ :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 K；大通道 byte 10 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// A. 原廠 0x22 積木：K = 已連線（K = 1／0）
int connected = MiniR4.I2C1.MXColor.getColor(K);
```

**B. 大通道（LauMo 函式庫）**

```cpp
// B. 大通道 byte 10：已連線
int connected = bigU8(10);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 手把已連線？ :: #E8711E)</pre>

```typescript
let connected = MBC.readValue(0x42, 0x06)   // 已連線：0~1
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` 電量 <small>battery</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-battery.svg" alt="電量" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 手把電量 0~100（PS2 接收器恆 0） |
| 範圍 | `0~100` % |
| I2C／mBlock 編號 | `07` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 3 byte 1

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

battery = dev.read(3)[1]   # 電量
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
battery = pad.battery()
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
int16_t battery = mbcRead(0x42, 0x07);   // 電量：0~100 %
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 電量（0~100 %）。主機送 `[07]` → 回 `[值低][值高][07 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 電量 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 沒有；大通道 byte 11 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// B. 大通道 byte 11：電量
int battery = bigU8(11);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 電量 :: #E8711E)</pre>

```typescript
let battery = MBC.readValue(0x42, 0x07)   // 電量：0~100 %
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` L2 類比 <small>l2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-l2Analog.svg" alt="L2 類比" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | L2 扳機壓多深 0~255（未連線 0） |
| 範圍 | `0~255` |
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
int16_t l2Analog = mbcRead(0x42, 0x08);   // L2 類比：0~255
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 L2 類比（0~255）。主機送 `[08]` → 回 `[值低][值高][08 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] L2 類比 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC L2 類比 :: #E8711E)</pre>

```typescript
let l2Analog = MBC.readValue(0x42, 0x08)   // L2 類比：0~255
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` R2 類比 <small>r2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-r2Analog.svg" alt="R2 類比" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | R2 扳機壓多深 0~255（未連線 0） |
| 範圍 | `0~255` |
| I2C／mBlock 編號 | `09` |

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
int16_t r2Analog = mbcRead(0x42, 0x09);   // R2 類比：0~255
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 R2 類比（0~255）。主機送 `[09]` → 回 `[值低][值高][09 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] R2 類比 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC R2 類比 :: #E8711E)</pre>

```typescript
let r2Analog = MBC.readValue(0x42, 0x09)   // R2 類比：0~255
```

</details>

</section>

<section id="v0a" class="ref-item">

### `0A` 系統鍵 <small>sysKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-sysKeys.svg" alt="系統鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | b0 PS 鍵、b1 擷取（PS5 靜音）鍵；未連線 0（PS2 恆 0） |
| 範圍 | `0~3` bit |
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
int16_t sysKeys = mbcRead(0x42, 0x0A);   // 系統鍵：0~3 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 系統鍵（0~3 bit）。主機送 `[0A]` → 回 `[值低][值高][0A ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 系統鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 系統鍵 :: #E8711E)</pre>

```typescript
let sysKeys = MBC.readValue(0x42, 0x0A)   // 系統鍵：0~3 bit
```

</details>

</section>

<section id="v0b" class="ref-item">

### `0B` 手把類型 <small>padType</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 0 無、1 PS2 數位、2 PS2 類比、3 PS4、4 PS5（或其他） |
| 範圍 | `0~4` |
| I2C／mBlock 編號 | `0B` |

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
int16_t padType = mbcRead(0x42, 0x0B);   // 手把類型：0~4
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 手把類型（0~4）。主機送 `[0B]` → 回 `[值低][值高][0B ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 手把類型 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 0x22 與大通道都沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 手把類型 :: #E8711E)</pre>

```typescript
let padType = MBC.readValue(0x42, 0x0B)   // 手把類型：0~4
```

</details>

</section>

<section id="v10" class="ref-item">

### `10~13` 搖桿 1~4 軸 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="搖桿 1~4 軸" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正；未連線 0 |
| 範圍 | `-100~+100` |
| I2C／mBlock 編號 | `10~13` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 2 🔒；模式 0 (toy) byte 2 🔒；模式 4 byte 0；模式 7 v[2] 🔒；模式 9 v[2] 🔒。備註：LX×10+LY（各 1~9，5 = 中間）；byte 4~5 = RX×10+RY; LX×10+LY 查表編碼；byte 4~5 = RX×10+RY；byte 6~7 = 肩鍵 byte 查表; [LX, LY, RX, RY] 0~99（宣告 2×int16，送 4 B）; LX×1000+LY×100+RX×10+RY（各 1~9）; v[2..5] = LX LY RX RY，1~9，5 = 中間

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

words = dev.read(9)[2:6]
stick = []
for w in words:
    stick.append(w & 0xFF)          # 奇數路
    stick.append((w >> 8) & 0xFF)   # 偶數路
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
stick = pad.stick('LX')   # list，第 1 路 = [0]
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8 · LX；LY 在 index 3、RX 5、RY 7（與按鍵交錯）；0~99，50 = 中間（未連線 50）；模式 2 · int8 · IR-REMOTE [LX, LY, RX, RY] 0~99，50 = 中間；模式 4 · int8 · LX；LY 在 index 3（RX、RY 不在模式 4）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

stick = sensor.read(1)[1]   # 搖桿 1~4 軸：-100~+100
stick = sensor.read(2)[0:4]   # 搖桿 1~4 軸
stick = sensor.read(4)[1]   # 搖桿 1~4 軸：-100~+100
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
stick = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 2)
stick1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
Sensor.SetMode(1, 4)
stick = Sensor.ReadRawValue(1, 1)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x10~0x13 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t stick[4];
mbcReadMany(0x42, 0x10, 4, stick);   // 搖桿 1~4 軸：-100~+100
for (uint8_t i = 0; i < 4; i++) { Serial.print(stick[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 搖桿 1~4 軸（-100~+100）。主機送 `[10]` → 回 `[值低][值高][10 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。

<pre class="blocks">(掌控者 連接埠 [1 v] 搖桿 [軸] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B~Y；大通道 byte 4~7 u8。A、B 要裝不同函式庫檔，不能同時用；`bigU8()` 等小幫手見本產品的 MATRIX 頁。

```cpp
// A. 原廠 0x22 積木：B~Y = 搖桿 1~4 軸（B C M Y = LX LY RX RY，0~99，50 = 中間）
int raw = MiniR4.I2C1.MXColor.getColor(B);   // 第 1 組
```

**B. 大通道（LauMo 函式庫）**

```cpp
// B. 大通道 byte 4~7：搖桿 1~4 軸
int stick[4];
for (uint8_t i = 0; i < 4; i++) stick[i] = bigU8(4 + i);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充內部用 `Int16LE` 讀 2 byte；寫完編號 `basic.pause(2)` 再讀。

<pre class="blocks">(MBC 搖桿 [軸] :: #E8711E)</pre>

```typescript
let stick = MBC.readValues(0x42, 0x10, 4)   // 搖桿 1~4 軸
```

</details>

</section>

## 寫入命令

只有 Arduino I2C、mBlock、micro:bit（與 MATRIX 大通道的 F1／F2）能送命令；SPIKE／EV3 接頭是唯讀的。

<section id="ce5" class="ref-item ref-cmd">

### `E5` 震動 <small>rumble</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | [強度 0~255]，0 = 停；每次 200 ms（PS2 收到忽略） |
| 參數 | 強度 `0~255`（1 byte） |
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
mbcWrite(0x42, 0xE5, 128);   // 震動（強度 0~255）
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E5][參數…]` → 感應器回 `[00][00][E5]`（收到了）再執行。

<pre class="blocks">掌控者 連接埠 [1 v] 震動 (128) :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 沒有這個命令（大通道只有 F1／F2 兩個動作）。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 震動 [強度] :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE5, [50])   // 震動
```

</details>

</section>

<section id="ce6" class="ref-item ref-cmd">

### `E6` 燈條顏色 <small>lightbar</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | [R][G][B] 各 0~255，要剛好 3 byte；未連線先記住，連上再套用；預設藍 (0,0,255)（PS2 收到忽略） |
| 參數 | 紅 `0~255`（1 byte）、綠 `0~255`（1 byte）、藍 `0~255`（1 byte） |
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
mbcWrite(0x42, 0xE6, 128, 128, 128);   // 燈條顏色（紅 0~255、綠 0~255、藍 0~255）
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

主機送 `[E6][參數…]` → 感應器回 `[00][00][E6]`（收到了）再執行。

<pre class="blocks">掌控者 連接埠 [1 v] 燈條顏色 (128) (128) (128) :: sensing</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 沒有這個命令（大通道只有 F1／F2 兩個動作）。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">MBC 燈條 R[r] G[g] B[b] :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE6, [50, 50, 50])   // 燈條顏色
```

</details>

</section>

<section id="flags" class="ref-item">

## 狀態旗標 F3

`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀，MATRIX 大通道是 byte 24。

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 手把已連線 (`connected`) | = 值 06（linkActive） |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

id_word, proto, fw, seq = dev.read(8)   # 模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]
addr = id_word & 0xFF   # 0x42
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
addr = id_word & 0xFF             # 0x42
hw0 = id_word >> 8                # 2
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x42, 0xF3);
bool ready = st & (1 << 0);   // 就緒
bool connected = st & (1 << 1);   // 手把已連線
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

狀態旗標 F3 的各 bit 由擴展拆成布林積木（就緒、校準中…）。

<pre class="blocks"><掌控者 連接埠 [1 v] 連線正常？ :: sensing></pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

```cpp
uint8_t st = MiniR4.I2C1.MXColor.getColorID();   // 大通道 byte 24 = F3 狀態旗標
bool ready = st & 1;
uint8_t id  = MiniR4.I2C1.MXColor.getColor(Y);    // byte 27 裝置 ID 0x42
uint8_t hw0 = MiniR4.I2C1.MXColor.getColor(K);    // byte 28 hw[0] = 2
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><MBC 裝置就緒 :: #E8711E></pre>

```typescript
let st = MBC.status(0x42)   // F3
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

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/ps4.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 值表、命令、旗標與 PS2 完全相同（mBlock 定案「兩款相同」＋I2C 定案 §3／§4）；只有 hw0 = 2、model = PS4、E5／E6 有作用。
- D3：SPIKE 模式 0 教育版 byte 6~7、模式 6 v[2]、模式 9 v[6] 的 button16 都是 PSX 排列（#65 已把 PS4 改成 PSX，盤點檔寫的 Bluepad32 排列已過時）。
- 0x22 命令 4 同時裝方向鍵（低 nibble）與形狀鍵（高 nibble）、命令 5 裝肩鍵（低）與功能鍵（高）；JSON 只能把一個命令掛在一個值上，所以掛在 02／04，03／05 的 ch22 = null（內容寫在 02／04 的 note）。SPIKE 模式 0／1／2／9 的 nibble 也用同樣方式在 note 說明。
- 搖桿：I2C／mBlock 送 ±100；EV3／0x22／大通道送 0~99（50 = 中間）、SPIKE 送 1~9（5 = 中間），不是線性 offset，主機端各自換算（網站範例已是這樣）。
- EV3 模式 3 = 方向鍵 | 形狀鍵<<4 | 已連線<<8（#67 新增 bit8）；模式 4 內容不變（[方向鍵, LX, 形狀鍵, LY]）；模式 5 改送裝置 ID 格式 B [0x42 | 2<<8, 韌體版]。
- SPIKE 模式 3 改 [已連線, 電量, 0]（3×int8）、模式 8 改 [0x42 | 2<<8, 協議版 2, 韌體版, 更新序號]；其餘空模式 1、2、4、6 維持現狀。
- 大通道尾段：byte 24 = F3、25~26 = 0（無 F1／F2 動作）、27 = 0x42、28 = 2；F1~F4 收到不做任何動作。
- ESP32 從機不能時脈延展：I2C v2 在 onReceive 就預載從該編號起 32 B；0x22／大通道照 MATRIX 定案 §5 #2 預載最新一包，待上機。
- I2C 0A 系統鍵 b1 = 擷取鍵（v1 0x15 放在 b3，v2 改 b1，照 I2C 定案 §4）。
