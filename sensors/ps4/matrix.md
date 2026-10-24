<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps4.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/ps4/matrix/matrixblock.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/ps4/matrix/matrixblock.md">掌控者 PS4/PS5 › MATRIX › MATRIXblock 原廠積木</a></p>

# 掌控者 PS4／PS5 接收器 × MATRIX Mini R4

[🔙 回到掌控者 PS4／PS5 接收器產品頁](/sensors/ps4/index.md)

掌控者 PS4／PS5 接收器 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，用 **原廠顏色感應器積木** 讀：不用換函式庫，R~K 七個 0~255 的數。

<p class="chips"><a class="chip chip-spike" href="/sensors/ps4/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/ps4/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/ps4/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/ps4/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/sensors/ps4/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/ps4/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-matrix.svg" alt="掌控者 PS4／PS5 接收器 × MATRIX Mini R4" loading="lazy"></figure>

用 MATRIX 原廠 4 pin 感應器線接 I2C1 或 I2C2，積木裡選對應的孔。

## A. 原廠積木（0x22）

積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數。⚠️ 不要用原廠的 H／S／V、顏色辨識積木（那是主機拿 R、G、B 自己算的，對本產品沒有意義）；灰階、顏色編號積木已被 MATRIX 拿掉。

| 積木 | 內容 | 解法 |
| :--- | :--- | :--- |
| R | [方向鍵](/sensors/ps4/reference.md#v02) | R = 方向鍵 \| 形狀鍵<<4（上 1 右 2 下 4 左 8、▲16 ●32 ✕64 ■128） |
| G | [肩鍵](/sensors/ps4/reference.md#v04) | G = 肩鍵 \| 功能鍵<<4（L2 1 R2 2 L1 4 R1 8、SELECT 16 L3 32 R3 64 START 128） |
| K | [已連線](/sensors/ps4/reference.md#v06) | K = 1／0 |
| B | [搖桿 1~4 軸](/sensors/ps4/reference.md#v10) 第 1~2 路 | B C M Y = LX LY RX RY，0~99，50 = 中間 |
| C | [搖桿 1~4 軸](/sensors/ps4/reference.md#v10) 第 3~4 路 | B C M Y = LX LY RX RY，0~99，50 = 中間 |
| M | [搖桿 1~4 軸](/sensors/ps4/reference.md#v10) 第 5~6 路 | B C M Y = LX LY RX RY，0~99，50 = 中間 |
| Y | [搖桿 1~4 軸](/sensors/ps4/reference.md#v10) 第 7~8 路 | B C M Y = LX LY RX RY，0~99，50 = 中間 |

```cpp
// MATRIXblock → Arduino：原廠函式庫
int r = MiniR4.I2C1.MXColor.getColor(R);   // 方向鍵
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/ps4.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 值表、命令、旗標與 PS2 完全相同（mBlock 定案「兩款相同」＋I2C 定案 §3／§4）；只有 hw0 = 2、model = PS4、E5／E6 有作用。
- D3：SPIKE 模式 0 教育版 byte 6~7、模式 6 v[2]、模式 9 v[6] 的 button16 都是 PSX 排列（#65 已把 PS4 改成 PSX，盤點檔寫的 Bluepad32 排列已過時）。
- 0x22 命令 4 同時裝方向鍵（低 nibble）與形狀鍵（高 nibble）、命令 5 裝肩鍵（低）與功能鍵（高）；JSON 只能把一個命令掛在一個值上，所以掛在 02／04，03／05 的 ch22 = null（內容寫在 02／04 的 note）。SPIKE 模式 0／1／2／9 的 nibble 也用同樣方式在 note 說明。
- 搖桿：I2C／mBlock 送 ±100；EV3／0x22送 0~99（50 = 中間）、SPIKE 送 1~9（5 = 中間），不是線性 offset，主機端各自換算（網站範例已是這樣）。
- EV3 模式 3 = 方向鍵 | 形狀鍵<<4 | 已連線<<8（#67 新增 bit8）；模式 4 內容不變（[方向鍵, LX, 形狀鍵, LY]）；模式 5 改送裝置 ID 格式 B [0x42 | 2<<8, 韌體版]。
- SPIKE 模式 3 改 [已連線, 電量, 0]（3×int8）、模式 8 改 [0x42 | 2<<8, 協議版 2, 韌體版, 更新序號]；其餘空模式 1、2、4、6 維持現狀。
- ESP32 從機不能時脈延展：I2C v2 在 onReceive 就預載從該編號起 32 B；0x22照 MATRIX 定案 §5 #2 預載最新一包，待上機。
- I2C 0A 系統鍵 b1 = 擷取鍵（v1 0x15 放在 b3，v2 改 b1，照 I2C 定案 §4）。
