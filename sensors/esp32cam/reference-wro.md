<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_wro.json 產生（reference），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/esp32cam/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/esp32cam/index.md">神攝手 ESP32CAM</a></p>

# 神攝手 WRO 版（HUE12） 指令參考

[🔙 回到神攝手 WRO 版（HUE12）產品頁](/sensors/esp32cam/index.md)

這一頁列出神攝手 WRO 版（HUE12）送給每個上位機的**每一個值、每一個命令**：它是什麼意思、範圍多少、在 SPIKE／EV3／Arduino I2C／mBlock／MATRIX／micro:bit 各要怎麼讀。網址加 `?host=spike`（或 ev3、arduino、mblock、matrix、microbit）會直接展開那個平台。

<p class="chips"><a class="chip chip-spike" href="/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off">MATRIX</span><a class="chip chip-microbit" href="/sensors/esp32cam/microbit.md">micro:bit</a></p>

| 項目 | 內容 |
| :--- | :--- |
| I2C 出廠位址 | `0x4C` |
| hw[0] | `2`（F0 高 byte、SPIKE 模式 8／EV3 模式 5 ID 高 byte） |
| 型號 | `HUE12` |
| 協議版本 | `2` |
| 支援平台 | SPIKE、EV3、Arduino I2C、mBlock、micro:bit |

## 值一覽

| 編號 | 值 | 範圍 | SPIKE | EV3 | Arduino I2C | mBlock | micro:bit |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: |
| [`10~1B`](#v10) | [色點 1~12 顏色碼](#v10) | 0~255 | ✓ | ✓ | ✓ | ✓ | ✓ |
| [`20~2B`](#v20) | [色點 1~12 鮮豔度](#v20) | 0~255 | ✓ | ✓ | ✓ | ✓ | ✓ |

✓ = 這個平台有這個值；— = 沒有（表格裡的平台欄位只列產品支援的平台）。

## 每個值怎麼讀

<section id="v10" class="ref-item">

### `10~1B` 色點 1~12 顏色碼 <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="色點 1~12 顏色碼" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |
| 範圍 | `0~255` |
| I2C／mBlock 編號 | `10~1B` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 0 🔒。備註：同 EV3：Pybricks read(0) 得 8 個 int32，每個拆 3 byte（積木網站 MBC_HUE12 設定檔）；玩具版不送；鎖住

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

spotColor = dev.read(0)[0]   # 色點 1~12 顏色碼
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_hue12_Lib import HUE12Sensor

cam = HUE12Sensor(Port.A)
cam.update()   # 每圈先更新一次
spotColor = cam.get_slot(1)[1]   # list，第 1 路 = [0]
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 0 · int32×8 · HUE12 8×int32 LE（32 B）：24 byte = [色相, 鮮豔度]×12，每 3 byte 塞一個 int32（byte3 = 0）；格 n 色相 = byte 2(n−1) → int32 k = idx/3、byte = idx%3；鎖住（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

spotColor = sensor.read(0)[0:12]   # 色點 1~12 顏色碼
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
spotColor1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x10~0x1B 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t spotColor[12];
mbcReadMany(0x4C, 0x10, 12, spotColor);   // 色點 1~12 顏色碼：0~255
for (uint8_t i = 0; i < 12; i++) { Serial.print(spotColor[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 色點 1~12 顏色碼（0~255）。主機送 `[10]` → 回 `[值低][值高][10 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。

<pre class="blocks">(神攝手舊版 WRO 連接埠 [1 v] 色點 [1 v] 顏色碼 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(神攝手 色點 (1) 顏色碼 :: #00A4E4)</pre>

```typescript
let spotColor = mbc.camSpotColor(1)   // 色點 1~12 顏色碼：0~255（N = 1~12）
```

**相關積木「神攝手 色點 ［1~12］ 是 ［彩色］？」**

```typescript
let camSpotIs = mbc.camSpotIs(1, MbcCamSpotKind.Colour)
```

</details>

</section>

<section id="v20" class="ref-item">

### `20~2B` 色點 1~12 鮮豔度 <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="色點 1~12 鮮豔度" loading="lazy"></figure>

| 項目 | 內容 |
| :--- | :--- |
| 用途 | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |
| 範圍 | `0~255` |
| I2C／mBlock 編號 | `20~2B` |
| 鎖住 | 🔒 已鎖住的樂高模式排法不再改 |

**怎麼讀**

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

模式 0 (edu) byte 1 🔒。備註：同 EV3；鎖住

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

spotVivid = dev.read(0)[1]   # 色點 1~12 鮮豔度
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_hue12_Lib import HUE12Sensor

cam = HUE12Sensor(Port.A)
cam.update()   # 每圈先更新一次
spotVivid = cam.get_slot(1)[2]   # list，第 1 路 = [0]
```

</details>

<details class="plat plat-ev3" data-host="ev3">
<summary><span class="plat-dot"></span>EV3</summary>

模式 0 · int32×8 · 同 10~1B：格 n 鮮豔度 = byte 2(n−1)+1；鎖住（已鎖住的模式，韌體與主機端都不再改）

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

spotVivid = sensor.read(0)[0:12]   # 色點 1~12 鮮豔度
```

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
spotVivid1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

編號 0x20~0x2B 連號，一次 `mbcReadMany` 讀完保證同一瞬間。

```cpp
int16_t spotVivid[12];
mbcReadMany(0x4C, 0x20, 12, spotVivid);   // 色點 1~12 鮮豔度：0~255
for (uint8_t i = 0; i < 12; i++) { Serial.print(spotVivid[i]); Serial.print(' '); }
Serial.println();
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

積木傳回 色點 1~12 鮮豔度（0~255）。主機送 `[20]` → 回 `[值低][值高][20 ^ 值低 ^ 值高]`，值 = int16 小端；第 N 路的編號 = 0x20 + (N − 1)。

<pre class="blocks">(神攝手舊版 WRO 連接埠 [1 v] 色點 [1 v] 鮮豔度 :: sensing)</pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。

<pre class="blocks">(神攝手 色點 (1) 鮮豔度 :: #00A4E4)</pre>

```typescript
let spotVivid = mbc.camSpotVivid(1)   // 色點 1~12 鮮豔度：0~255（N = 1~12）
```

</details>

</section>

<section id="flags" class="ref-item">

## 狀態旗標 F3

`F3` 每個 bit 一個狀態；I2C、mBlock、micro:bit 直接讀。

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1；b1 = 0（WRO 版沒有「有找到色塊」） |

<details class="plat plat-spike" data-host="spike" open>
<summary><span class="plat-dot"></span>SPIKE</summary>

SPIKE 沒有 F3 旗標；模式 8 回裝置 ID／版本。官方 App 讀不到模式 8。

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

id_word, proto, fw, seq = dev.read(8)   # 模式 8：[ID | hw0<<8, 協議版 2, 主版×100+次版, 更新序號]
addr = id_word & 0xFF   # 0x4C
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
addr = id_word & 0xFF             # 0x4C
hw0 = id_word >> 8                # 2
```

</details>

<details class="plat plat-arduino" data-host="arduino">
<summary><span class="plat-dot"></span>Arduino I2C</summary>

```cpp
int16_t st = mbcRead(0x4C, 0xF3);
bool ready = st & (1 << 0);   // 就緒
```

</details>

<details class="plat plat-mblock" data-host="mblock">
<summary><span class="plat-dot"></span>mBlock</summary>

狀態旗標 F3 的各 bit 由擴展拆成布林積木（就緒、校準中…）。

<pre class="blocks"><神攝手舊版 WRO 連接埠 [1 v] 連線正常？ :: sensing></pre>

</details>

<details class="plat plat-microbit" data-host="microbit">
<summary><span class="plat-dot"></span>micro:bit</summary>

<pre class="blocks"><位址 (0x4C v) 的裝置就緒？ :: #00A4E4></pre>

```typescript
let ready = mbc.deviceReady(0x4C)   // F3 bit0；沒接 = false
let st = mbc.status(0x4C)   // F3
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

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/cam_wro.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 2（共識 #6：神攝手 2 = WRO 版）；model = HUE12（樂高模式名）。
- I2C 0x4C 與學校版同一位址（主機用 F0 高 byte 1／2 分辨）；D4：SDA = IO3、SCL = IO1，與 EV3／SPIKE 串口模式二選一編譯；ESP32 從機 onReceive 預載 32 B；上機待測。mBlock 用 Serial.begin(38400, SERIAL_8N1, 3, 1)。
- 值表只有 10~1B、20~2B（mBlock 定案）；兩群各 12 個，I2C 一次連續讀 12 個值（24 B）剛好一群。
- EV3／SPIKE 只有一個模式 HUE12（type 95，動態描述表 DATA32×8），鎖住照盤點 §0-2 排法；沒有 ID 模式（type 95 只宣告 1 個模式，不加）。
- mVision 鎖住照盤點 §4：[0xAA][9][18 B][xor]，byte 0~11 顏色碼、12~17 鮮豔度 4 bit；WRO 隊伍的 HUE12_AS_mVisionSensor_v1.0.8.mbr4 在用，不動；B-6：不節流（每圈 loop 送）。
- mVision 的鮮豔度是 S>>4（0~15），值表 20~2B 是完整 0~255；兩者不同是既有設計，不改。
- 顏色碼 255 只代表「色點沒開」，不代表「沒找到」（沒抓到時仍回種子點顏色，盤點 §0-1）。
- 不做 0x22（hosts.matrix = false）；沒有寫入命令；F3 只有 b0。
