<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps2.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/ps2/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/ps2/index.md">掌控者 PS2</a></p>

# 掌控者 PS2 指令參考

[🔙 回到掌控者 PS2產品頁](/sensors/ps2/index.md)

這一頁列出掌控者 PS2送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 `?host=spike`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。

<p class="chips"><a class="chip chip-spike" href="/sensors/ps2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/ps2/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/ps2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/ps2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/ps2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/ps2/microbit.md">micro:bit</a></p>

| 項目 | 內容 |
| :--- | :--- |
| I2C 出廠位址 | `0x42` |
| hw[0] | `1`（F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte） |
| 型號 | `PS2` |
| 協議版本 | `2` |
| 支援平台 | SPIKE、EV3、Arduino I2C、mBlock、MATRIX、micro:bit |

## 值一覽

| 編號 | 值 | 範圍 | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| [`01`](#v01) | [十六鍵](#v01) | 0~65535 bit | ✓ | — | ✓ | ✓ | — | ✓ |
| [`02`](#v02) | [方向鍵](#v02) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`03`](#v03) | [形狀鍵](#v03) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`04`](#v04) | [肩鍵](#v04) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`05`](#v05) | [功能鍵](#v05) | 0~15 bit | ✓ | ✓ | ✓ | ✓ | — | ✓ |
| [`06`](#v06) | [已連線](#v06) | 0~1 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`07`](#v07) | [電量](#v07) | 0 % | ✓ | — | ✓ | ✓ | — | ✓ |
| [`08`](#v08) | [L2 類比](#v08) | 0~255 | — | — | ✓ | ✓ | — | ✓ |
| [`09`](#v09) | [R2 類比](#v09) | 0~255 | — | — | ✓ | ✓ | — | ✓ |
| [`0A`](#v0a) | [系統鍵](#v0a) | 0 bit | — | — | ✓ | ✓ | — | ✓ |
| [`0B`](#v0b) | [手把類型](#v0b) | 0~2 | — | — | ✓ | ✓ | — | ✓ |
| [`10~13`](#v10) | [搖桿 LX／LY／RX／RY](#v10) | -100~+100 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |

✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。

## 每個值怎麼讀

<section id="v01" class="ref-item">

### `01` 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 16 個按鍵合成一個數（PSX 排列，1 = 按下）：bit0 SELECT、1 L3、2 R3、3 START、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；未連線 = 0 |
| 範圍 | `0~65535` bit |
| I2C／mBlock 編號 | `01` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 6 🔒；模式 5 v[0] 🔒；模式 6 v[2]；模式 9 v[6] 🔒。備註：App 原始藍 = button16（.ino:201-211）; Pybricks reflection() 編碼：11 鍵 complexXY = SELECT<<10 | R1<<9 | L1<<8 | ■<<7 | ✕<<6 | ●<<5 | ▲<<4 | 左<<3 | 下<<2 | 右<<1 | 上，a = /25、b = (餘+1)×10−1，3 個 int16 = a×256+b → reflection() = complexXY；L2、R2、L3、R3、START 讀不到（.ino:334-358）; v2 = button16（PSX）; v6 = button16（PSX）

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

這個值本身就可能是 −32768（最高位元 = 1，例如 `0x8000`），跟讀不到時回的 `MBC_NONE` 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 `mbcReadMany(0x42, 0x01, 1, &buttons16)`，回傳 `false` 才是讀不到。

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

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 十六鍵位元圖 :: #00A4E4)</pre>

```typescript
let buttons16 = mbc.padButtons()   // 十六鍵：0~65535 bit
```

**相關積木「掌控者 按鈕 ［✕］ 被按住？」**

```typescript
let padButtonPressed = mbc.padButtonPressed(MbcPadButton.Cross)
```

**相關積木「當掌控者 按鈕 ［✕］ ［按下］」**

```typescript
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, function () {
})
```

</details>

</section>

<section id="v02" class="ref-item">

### `02` 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |
| 範圍 | `0~15` bit |
| I2C／mBlock 編號 | `02` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 0 🔒；模式 1 byte 0；模式 6 v[0] lo；模式 9 v[0] 🔒。備註：byte1 = 方向 | 形狀<<4（App 反射光）：低 4 bit; byte0 = 方向 | 形狀<<4; 低 4 bit（高 4 bit = 形狀）; v0 = 方向 | 形狀<<4（bit0-3）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

dpad = dev.read(9)[0]   # 方向鍵
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

模式 0 · int8 · IR-PROX = 方向鍵（.ino:130-132）；模式 1 · int8 · IR-SEEK [方向, LX, 形狀, LY, 肩, RX, 功能, RY]（按鍵與搖桿交錯）；模式 3 · int16 · #67 改：IR-REM-A 1×int16 = 方向 | 形狀<<4 | 已連線<<8（bit0-3 = 方向）；模式 4 · int8 · IR-S-ALT [方向, LX, 形狀, LY]（已鎖住的模式，韌體與主機端都不再改）

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

0x22 R。

```cpp
// A. 原廠 0x22 積木：R = 方向鍵（#67 新增：R = 方向鍵 | 形狀鍵<<4（上 1、右 2、下 4、左 8、▲16、●32、✕64、■128））
int dpad = MiniR4.I2C1.MXColor.getColor(R);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 [方向鍵 v]（0~15） :: #00A4E4)</pre>

```typescript
let dpad = mbc.padGroup(MbcPadGroup.Dpad)   // 方向鍵：0~15 bit
```

</details>

</section>

<section id="v03" class="ref-item">

### `03` 形狀鍵 <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="形狀鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |
| 範圍 | `0~15` bit |
| I2C／mBlock 編號 | `03` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 0 🔒；模式 1 byte 0；模式 6 v[0] hi；模式 9 v[0] 🔒。備註：高 4 bit; 高 4 bit; 高 4 bit; v0 bit4-7

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

shape = dev.read(9)[0]   # 形狀鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
shape = pad.pressed('TRIANGLE')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8；模式 3 · int16 · #67 改：bit4-7 = 形狀（同一個 int16）；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

shape = sensor.read(1)[2]   # 形狀鍵：0~15 bit
shape = sensor.read(3)[0]   # 形狀鍵：0~15 bit
shape = sensor.read(4)[2]   # 形狀鍵：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shape = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 3)
shape = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
shape = Sensor.ReadRawValue(1, 2)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t shape = mbcRead(0x42, 0x03);   // 形狀鍵：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 形狀鍵（0~15 bit）。主機送 `[03]` → 回 `[值低][值高][03 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 形狀鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 [形狀鍵 v]（0~15） :: #00A4E4)</pre>

```typescript
let shape = mbc.padGroup(MbcPadGroup.Shapes)   // 形狀鍵：0~15 bit
```

</details>

</section>

<section id="v04" class="ref-item">

### `04` 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |
| 範圍 | `0~15` bit |
| I2C／mBlock 編號 | `04` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 6 🔒；模式 2 byte 0；模式 6 v[1] lo；模式 9 v[1] 🔒。備註：byte0 = 肩 | 功能<<4（App 顏色，START = −128）：低 4 bit; byte6~7 = (肩 | 功能<<4) 經玩具版查表編碼；byte1 = 單鍵編號（L2=1 R2=2 L1=3 R1=4 SELECT=5 L3=6 R3=7 START=8，沒按 0）; 低 4 bit（高 4 bit = 功能）; v1 = 肩 | 功能<<4（bit0-3）

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

shoulder = dev.read(9)[1]   # 肩鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
shoulder = pad.pressed('L1')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

shoulder = sensor.read(1)[4]   # 肩鍵：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shoulder = Sensor.ReadRawValue(1, 4)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t shoulder = mbcRead(0x42, 0x04);   // 肩鍵：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 肩鍵（0~15 bit）。主機送 `[04]` → 回 `[值低][值高][04 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 肩鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 G。

```cpp
// A. 原廠 0x22 積木：G = 肩鍵（#67 新增：G = 肩鍵 | 功能鍵<<4（L2 1、R2 2、L1 4、R1 8、SELECT 16、L3 32、R3 64、START 128））
int shoulder = MiniR4.I2C1.MXColor.getColor(G);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 [肩鍵 v]（0~15） :: #00A4E4)</pre>

```typescript
let shoulder = mbc.padGroup(MbcPadGroup.Shoulders)   // 肩鍵：0~15 bit
```

</details>

</section>

<section id="v05" class="ref-item">

### `05` 功能鍵 <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="功能鍵" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |
| 範圍 | `0~15` bit |
| I2C／mBlock 編號 | `05` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒；模式 2 byte 0；模式 6 v[1] hi；模式 9 v[1] 🔒。備註：高 4 bit; 高 4 bit; v1 bit4-7

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

func = dev.read(9)[1]   # 功能鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
func = pad.pressed('START')
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 1 · int8（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

func = sensor.read(1)[6]   # 功能鍵：0~15 bit
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
func = Sensor.ReadRawValue(1, 6)
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

讀不到（沒接、位址錯）時回 `MBC_NONE`（−32768）。

```cpp
int16_t func = mbcRead(0x42, 0x05);   // 功能鍵：0~15 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 功能鍵（0~15 bit）。主機送 `[05]` → 回 `[值低][值高][05 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 功能鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 [功能鍵 v]（0~15） :: #00A4E4)</pre>

```typescript
let func = mbc.padGroup(MbcPadGroup.Functions)   // 功能鍵：0~15 bit
```

</details>

</section>

<section id="v06" class="ref-item">

### `06` 已連線 <small>connected</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |
| 範圍 | `0~1` |
| I2C／mBlock 編號 | `06` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 3 byte 0。備註：#67 改：LIGHT 3×int8 = [已連線, 電量, 0]（原本 = 4 組按鍵）

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

模式 3 · int16 · #67 改：bit8 = 已連線（+256）

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

0x22 K。

```cpp
// A. 原廠 0x22 積木：K = 已連線（#67 新增：K = 已連線 1／0）
int connected = MiniR4.I2C1.MXColor.getColor(K);
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回「否」。

<pre class="blocks"><掌控者 手把已連線？ :: #00A4E4></pre>

```typescript
let connected = mbc.padConnected()   // 已連線：0~1
```

</details>

</section>

<section id="v07" class="ref-item">

### `07` 電量 <small>battery</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 0~100；PS2 手把沒有電量 → 恆 0（PS4 才有） |
| 範圍 | `0` % |
| I2C／mBlock 編號 | `07` |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 3 byte 1。備註：#67 改：PS2 送 0

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
int16_t battery = mbcRead(0x42, 0x07);   // 電量：0 %
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 電量（0 %）。主機送 `[07]` → 回 `[值低][值高][07 ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 電量 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 電量（百分比） :: #00A4E4)</pre>

```typescript
let battery = mbc.padBattery()   // 電量：0 %
```

</details>

</section>

<section id="v08" class="ref-item">

### `08` L2 類比 <small>l2Analog</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | L2 壓力 0~255（DualShock 2 類比模式才有壓力值；手把沒有壓力值時按下 = 255、放開 = 0）；未連線 = 0 |
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

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 扳機 [L2 v] 類比值 :: #00A4E4)</pre>

```typescript
let l2Analog = mbc.padTrigger(MbcPadTrigger.L2)   // L2 類比：0~255
```

</details>

</section>

<section id="v09" class="ref-item">

### `09` R2 類比 <small>r2Analog</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | R2 壓力 0~255（同 08）；未連線 = 0 |
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

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 扳機 [R2 v] 類比值 :: #00A4E4)</pre>

```typescript
let r2Analog = mbc.padTrigger(MbcPadTrigger.R2)   // R2 類比：0~255
```

</details>

</section>

<section id="v0a" class="ref-item">

### `0A` 系統鍵 <small>sysKeys</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | b0 PS 鍵、b1 擷取鍵；PS2 手把沒有 → 恆 0（PS4 才有） |
| 範圍 | `0` bit |
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
int16_t sysKeys = mbcRead(0x42, 0x0A);   // 系統鍵：0 bit
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 系統鍵（0 bit）。主機送 `[0A]` → 回 `[值低][值高][0A ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 系統鍵 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

擴充沒有這顆積木；在 JavaScript 用 `mbc.readValue(位址, 編號)` 讀（讀不到 = −32768）。相關積木：「掌控者 系統鍵 ［PS］ 被按住？」。

```typescript
let sysKeys = mbc.readValue(0x42, 0x0A)   // 系統鍵：0 bit
```

</details>

</section>

<section id="v0b" class="ref-item">

### `0B` 手把類型 <small>padType</small>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 0 沒有手把、1 PS2 數位模式、2 PS2 類比模式（3 PS4、4 PS5 只有 PS4 接收器會出現） |
| 範圍 | `0~2` |
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
int16_t padType = mbcRead(0x42, 0x0B);   // 手把類型：0~2
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 手把類型（0~2）。主機送 `[0B]` → 回 `[值低][值高][0B ^ 值低 ^ 值高]`，值 = int16 小端。

<pre class="blocks">(掌控者 連接埠 [1 v] 手把類型 :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

MATRIX 原廠 0x22 積木沒有這個值。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 手把類型 :: #00A4E4)</pre>

```typescript
let padType = mbc.padType()   // 手把類型：0~2
```

</details>

</section>

<section id="v10" class="ref-item">

### `10~13` 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |
| 範圍 | `-100~+100` |
| I2C／mBlock 編號 | `10~13` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 2 🔒；模式 0 (toy) byte 2 🔒；模式 4 v[0]；模式 7 v[2] 🔒；模式 8 v[0]；模式 9 v[2] 🔒。備註：byte2~3 = LX×10+LY、byte4~5 = RX×10+RY（11~99，中心 55；App 原始紅／綠）; LX×10+LY、RX×10+RY 經玩具版查表編碼成 byte2~3、byte4~5; v0 = LX | LY<<8、v1 = RX | RY<<8; Pybricks ambient() 用：v2 = LX×1000 + LY×100 + RX×10 + RY（1111~9999，中心 5555）；v0、v1 同模式 6; 只送 4 B：v0 = LX | LY<<8、v1 = RX | RY<<8; v2~v5 = LX、LY、RX、RY（0~99 ÷10，0 補 1）

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

模式 1 · int8 · 交錯：index 1 = LX、3 = LY、5 = RX、7 = RY（偶數 index 是按鍵）；模式 2 · int8 · IR-REMOTE [LX, LY, RX, RY]（描述表宣告 0~10，實送 0~99）；模式 4 · int8 · index 1 = LX、3 = LY（只到左搖桿）（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

stick = sensor.read(1)[1:5]   # 搖桿 LX／LY／RX／RY
stick = sensor.read(2)[0:4]   # 搖桿 LX／LY／RX／RY
stick = sensor.read(4)[1:3]   # 搖桿 LX／LY／RX／RY
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
stick1 = Sensor.ReadRawValue(1, 1)   ' 第 1 筆；第 N 筆改成 1 + N - 1
Sensor.SetMode(1, 2)
stick1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
Sensor.SetMode(1, 4)
stick1 = Sensor.ReadRawValue(1, 1)   ' 第 1 筆；第 N 筆改成 1 + N - 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x10~0x13 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t stick[4];
mbcReadMany(0x42, 0x10, 4, stick);   // 搖桿 LX／LY／RX／RY：-100~+100
for (uint8_t i = 0; i < 4; i++) { Serial.print(stick[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 搖桿 LX／LY／RX／RY（-100~+100）。主機送 `[10]` → 回 `[值低][值高][10 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。

<pre class="blocks">(掌控者 連接埠 [1 v] 搖桿 [LX/LY/RX/RY] :: sensing)</pre>

</details>

<details class="plat plat-matrix" data-host="matrix">
<summary><span class="plat-dot"></span>MATRIX</summary>

0x22 B~Y。

```cpp
// A. 原廠 0x22 積木：B~Y = 搖桿 LX／LY／RX／RY（#67 新增：B、C、M、Y = LX、LY、RX、RY）
int raw = MiniR4.I2C1.MXColor.getColor(B);   // 第 1 組
```

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(掌控者 搖桿 [左搖桿 X v] :: #00A4E4)</pre>

```typescript
let stick = [mbc.padStick(MbcPadAxis.LeftX), mbc.padStick(MbcPadAxis.LeftY), mbc.padStick(MbcPadAxis.RightX), mbc.padStick(MbcPadAxis.RightY)]   // 搖桿 LX／LY／RX／RY：-100~+100
```

</details>

</section>

## 寫入命令

只有 Arduino I2C、mBlock、micro:bit 能送命令；SPIKE／EV3 接頭是唯讀的。

<section id="ce5" class="ref-item ref-cmd">

### `E5` 震動 <small>vibrate</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | [強度 0~255]，0 = 停；PS2 接收器收到後回 ACK 但忽略（250614 已關手把震動），PS4 才有作用 |
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

MATRIX 原廠 0x22 積木沒有這個命令。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">掌控者 震動 強度 (200) :: #00A4E4</pre>

```typescript
mbc.padRumble(200)   // 震動
```

</details>

</section>

<section id="ce6" class="ref-item ref-cmd">

### `E6` 燈條顏色 <small>setLed</small>

| 項目 | 內容 |
| :--- | :--- |
| 作用 | [R][G][B] 各 0~255；PS2 手把沒有燈條，接收器收到後回 ACK 但忽略，PS4 才有作用 |
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

MATRIX 原廠 0x22 積木沒有這個命令。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks">掌控者 燈條顏色 [#ff0000] :: #00A4E4</pre>

```typescript
mbc.padLight(0xff0000)   // 燈條顏色
```

</details>

</section>

<section id="flags" class="ref-item">

## 狀態旗標 F3

`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀。

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 手把已連線 (`connected`) | = 06 已連線 |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

id_word, proto, fw, seq = dev.read(8)   # 模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]
addr = id_word & 0xFF   # 0x42
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
addr = id_word & 0xFF             # 0x42
hw0 = id_word >> 8                # 1
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

MATRIX 原廠 0x22 積木讀不到狀態旗標。

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><位址 (0x42 v) 的裝置就緒？ :: #00A4E4></pre>

```typescript
let ready = mbc.deviceReady(0x42)   // F3 bit0；沒接 = false
let st = mbc.status(0x42)   // F3
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

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/ps2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw[0] = 1（PS2）、電量 07 恆 0、系統鍵 0A 恆 0、手把類型 0B 只會出現 0／1／2（I2C 定案 §2、§4）。
- 08／09 L2／R2 類比照定案寫 0~255：DS2 類比模式有壓力值；沒有壓力值時按下 = 255、放開 = 0（沿用 v1 0x14 的做法，盤點 .ino:598-603）。
- E5 震動、E6 燈條照 I2C 定案 §3 列在 PS2／PS4 共用命令表；PS2 收到回 ACK 後忽略（跟 v1 0x10／0x23 一樣）。
- 搖桿 10~13 在 I2C／mBlock 是 ±100（由 raw (v−128)×100/128、Y 上為正）；EV3／0x22 維持 0~99（50 中間）、SPIKE 維持 1~9（5 中間）照盤點，不改鎖住模式。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改 方向|形狀<<4|已連線<<8（原本沒有 bit8）；模式 5 改成 ID 格式 B（原本 = 按鍵／搖桿合併版）。
- SPIKE：0/5/7/9 鎖住，1、2、4、6 維持不動；模式 3 改 [已連線, 電量(0), 0]（原本 = 4 組按鍵）；模式 8 改 v[0] = ID|hw[0]<<8（原本 = 搖桿）。模式 9 v[6] button16 = PSX 排列（D3）。
- SPIKE 模式 5 只容得下 11 鍵（L2、R2、L3、R3、START 讀不到），鎖住不改。
