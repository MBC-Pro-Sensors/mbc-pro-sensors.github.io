<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps2.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# 掌控者 PS2 × MATRIX Mini R4

[🔙 回到掌控者 PS2產品頁](/sensors/ps2/index.md)

掌控者 PS2 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，有兩種用法：**A. 原廠顏色感應器積木**（不用換函式庫，R~K 七個 0~255 的數）與 **B. LauMo 大通道**（換成大通道函式庫，一包 30 byte 什麼都有）。兩種要裝不同的函式庫檔，不能同時用。

<p class="chips"><a class="chip chip-spike" href="/sensors/ps2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/ps2/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/ps2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/ps2/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/sensors/ps2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/ps2/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-matrix.svg" alt="掌控者 PS2 × MATRIX Mini R4" loading="lazy"></figure>

用 MATRIX 原廠 4 pin 感應器線接 I2C1 或 I2C2，積木裡選對應的孔。

## A. 原廠積木（0x22）

積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數。⚠️ 不要用原廠的 H／S／V、顏色辨識積木（那是主機拿 R、G、B 自己算的，對本產品沒有意義）；灰階、顏色編號積木已被 MATRIX 拿掉。

| 積木 | 內容 | 解法 |
| :--- | :--- | :--- |
| R | [方向鍵](/sensors/ps2/reference.md#v02) | #67 新增：R = 方向鍵 \| 形狀鍵<<4（上 1、右 2、下 4、左 8、▲16、●32、✕64、■128） |
| G | [肩鍵](/sensors/ps2/reference.md#v04) | #67 新增：G = 肩鍵 \| 功能鍵<<4（L2 1、R2 2、L1 4、R1 8、SELECT 16、L3 32、R3 64、START 128） |
| K | [已連線](/sensors/ps2/reference.md#v06) | #67 新增：K = 已連線 1／0 |
| B | [搖桿 LX／LY／RX／RY](/sensors/ps2/reference.md#v10) 第 1~2 路 | #67 新增：B、C、M、Y = LX、LY、RX、RY |
| C | [搖桿 LX／LY／RX／RY](/sensors/ps2/reference.md#v10) 第 3~4 路 | #67 新增：B、C、M、Y = LX、LY、RX、RY |
| M | [搖桿 LX／LY／RX／RY](/sensors/ps2/reference.md#v10) 第 5~6 路 | #67 新增：B、C、M、Y = LX、LY、RX、RY |
| Y | [搖桿 LX／LY／RX／RY](/sensors/ps2/reference.md#v10) 第 7~8 路 | #67 新增：B、C、M、Y = LX、LY、RX、RY |

```cpp
// MATRIXblock → Arduino：原廠函式庫
int r = MiniR4.I2C1.MXColor.getColor(R);   // 方向鍵
```

## B. 大通道（LauMo 函式庫）

函式庫 `MiniR4ColorSensorExt大通道版`（[下載](/downloads.md)）換掉原廠檔後，同一組積木變成「指標讀法」：ColorID 把指標歸零並回傳狀態、Blue 讀 1 byte、Green 讀 1 byte 有正負、Saturation 讀 2 byte、Hue 讀 2 byte 有正負、Value 讀 4 byte 小數、Red 把指標往後移「上一個讀取的大小」。

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | 積木 | 內容 | 備註 |
| :--- | :--- | :--- | :--- |
| 8~9 | Saturation | [十六鍵](/sensors/ps2/reference.md#v01) | #67 新增（[8..9] 小端） |
| 0 | Blue | [方向鍵](/sensors/ps2/reference.md#v02) | #67 新增 |
| 1 | Blue | [形狀鍵](/sensors/ps2/reference.md#v03) | #67 新增 |
| 2 | Blue | [肩鍵](/sensors/ps2/reference.md#v04) | #67 新增 |
| 3 | Blue | [功能鍵](/sensors/ps2/reference.md#v05) | #67 新增 |
| 10 | Blue | [已連線](/sensors/ps2/reference.md#v06) | #67 新增 |
| 11 | Blue | [電量](/sensors/ps2/reference.md#v07) | #67 新增；PS2 = 0 |
| 4~7 | Blue | [搖桿 LX／LY／RX／RY](/sensors/ps2/reference.md#v10)（每路 1 byte） | #67 新增：[4..7] = LX、LY、RX、RY |
| 24 | ColorID | [F3 狀態旗標](/sensors/ps2/reference.md#flags) |  |
| 25 | Cyan | F1 動作完成次數 | 0 |
| 26 | Magenta | F2 動作完成次數 | 0 |
| 27 | Yellow | 裝置 ID | 0x42 |
| 28 | Key | hw[0] | 1 |
| 29 | — | CRC-8 | 多項式 0x07，涵蓋 0~28；函式庫自己檢查 |

**動作**：無（Cyan／Magenta／Yellow／Key 只回傳 byte 25~28，不做事）

### Arduino 寫法（MATRIXblock 的 Arduino 模式）

```cpp
// 大通道：讀第 n 個 byte（uint8）／第 n 個起的 2 byte（uint16 小端）
uint8_t bigU8(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();                 // 指標回到 byte 0
  uint8_t v = MiniR4.I2C1.MXColor.getColor(B);      // 讀 byte 0
  for (uint8_t i = 0; i < n; i++) { MiniR4.I2C1.MXColor.getColor(R); v = MiniR4.I2C1.MXColor.getColor(B); }   // Red = 前進 1
  return v;
}
uint16_t bigU16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (uint16_t)MiniR4.I2C1.MXColor.getS();      // Saturation = 讀 2 byte
}
int16_t bigI16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (int16_t)MiniR4.I2C1.MXColor.getH();       // Hue = 讀 2 byte 有正負
}
```

```cpp
int buttons16 = bigU8(8);   // 十六鍵
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/ps2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw[0] = 1（PS2）、電量 07 恆 0、系統鍵 0A 恆 0、手把類型 0B 只會出現 0／1／2（I2C 定案 §2、§4）。
- 08／09 L2／R2 類比照定案寫 0~255：DS2 類比模式有壓力值；沒有壓力值時按下 = 255、放開 = 0（沿用 v1 0x14 的做法，盤點 .ino:598-603）。
- E5 震動、E6 燈條照 I2C 定案 §3 列在 PS2／PS4 共用命令表；PS2 收到回 ACK 後忽略（跟 v1 0x10／0x23 一樣）。
- 0x22 新增（MATRIX 定案 §6）：1 = 0x43、4 = 方向|形狀<<4、5 = 肩|功能<<4、6~9 = LX LY RX RY 0~99、10 = 已連線、11~12 = 0；大通道新增 [0..3] 四組鍵、[4..7] 搖桿 0~99、[8..9] button16、[10] 已連線、[11] 電量，尾段 [24] F3、[27] 0x42、[28] 1；F1~F4 無動作。ch22 命令 4／5 各掛在 02／04 底下（03／05 在 note 說明高 4 bit），避免同一命令重複登記。
- 搖桿 10~13 在 I2C／mBlock 是 ±100（由 raw (v−128)×100/128、Y 上為正）；EV3／大通道／0x22 維持 0~99（50 中間）、SPIKE 維持 1~9（5 中間）照盤點，不改鎖住模式。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改 方向|形狀<<4|已連線<<8（原本沒有 bit8）；模式 5 改成 ID 格式 B（原本 = 按鍵／搖桿合併版）。
- SPIKE：0/5/7/9 鎖住，1、2、4、6 維持不動；模式 3 改 [已連線, 電量(0), 0]（原本 = 4 組按鍵）；模式 8 改 v[0] = ID|hw[0]<<8（原本 = 搖桿）。模式 9 v[6] button16 = PSX 排列（D3）。
- SPIKE 模式 5 只容得下 11 鍵（L2、R2、L3、R3、START 讀不到），鎖住不改。
- 函式庫規則（命令 1 回 0x43、CRC-8、最快 5 ms 讀一次）以 `80_上位機研究\MatrixMiniR4\MiniR4ColorSensorExt大通道版` 為準。
