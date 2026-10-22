<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line8.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# 循行者 8 路 × MATRIX Mini R4

[🔙 回到循行者 8 路產品頁](/sensors/line8/index.md)

循行者 8 路 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，有兩種用法：**A. 原廠顏色感應器積木**（不用換函式庫，R~K 七個 0~255 的數）與 **B. LauMo 大通道**（換成大通道函式庫，一包 30 byte 什麼都有）。兩種要裝不同的函式庫檔，不能同時用。

<p class="chips"><a class="chip chip-spike" href="/sensors/line8/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/line8/ev3-hub.md">EV3</a><a class="chip chip-arduino" href="/sensors/line8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/line8/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/sensors/line8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/line8/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/line8/wiring-matrix.svg" alt="循行者 8 路 × MATRIX Mini R4" loading="lazy"></figure>

用 MATRIX 原廠 4 pin 感應器線接 I2C1 或 I2C2，積木裡選對應的孔。

## A. 原廠積木（0x22）

積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數。⚠️ 不要用原廠的 H／S／V、顏色辨識積木（那是主機拿 R、G、B 自己算的，對本產品沒有意義）；灰階、顏色編號積木已被 MATRIX 拿掉。

| 積木 | 內容 | 解法 |
| :--- | :--- | :--- |
| R | [線位置](/sensors/line8/reference.md#v01) + 8 | 讀到的數減 8 |
| B | [細位置](/sensors/line8/reference.md#v02) + 100 | 讀到的數減 100 |
| G | [線寬](/sensors/line8/reference.md#v03) |  |
| C | [第 1~8 路光值](/sensors/line8/reference.md#v10) 第 1~2 路 | C~K 兩路一組 convert9×10+convert9 |
| M | [第 1~8 路光值](/sensors/line8/reference.md#v10) 第 3~4 路 | C~K 兩路一組 convert9×10+convert9 |
| Y | [第 1~8 路光值](/sensors/line8/reference.md#v10) 第 5~6 路 | C~K 兩路一組 convert9×10+convert9 |
| K | [第 1~8 路光值](/sensors/line8/reference.md#v10) 第 7~8 路 | C~K 兩路一組 convert9×10+convert9 |

```cpp
// MATRIXblock → Arduino：原廠函式庫
int r = MiniR4.I2C1.MXColor.getColor(R);   // 線位置 + 8
```

## B. 大通道（LauMo 函式庫）

函式庫 `MiniR4ColorSensorExt大通道版`（[下載](/downloads.md)）換掉原廠檔後，同一組積木變成「指標讀法」：ColorID 把指標歸零並回傳狀態、Blue 讀 1 byte、Green 讀 1 byte 有正負、Saturation 讀 2 byte、Hue 讀 2 byte 有正負、Value 讀 4 byte 小數、Red 把指標往後移「上一個讀取的大小」。

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | 積木 | 內容 | 備註 |
| :--- | :--- | :--- | :--- |
| 0 | Blue | [線位置](/sensors/line8/reference.md#v01) + 8 | 讀到的數減 8 |
| 2 | Blue | [細位置](/sensors/line8/reference.md#v02) + 100 | 讀到的數減 100 |
| 1 | Blue | [線寬](/sensors/line8/reference.md#v03) |  |
| 12 | Blue | [線段數](/sensors/line8/reference.md#v04) |  |
| 11 | Blue | [壓線位元圖](/sensors/line8/reference.md#v05) |  |
| 25 | Blue | [校準完成次數](/sensors/line8/reference.md#v08) | 尾段 F1/F2 完成次數 |
| 3~10 | Blue | [第 1~8 路光值](/sensors/line8/reference.md#v10)（每路 1 byte） |  |
| 24 | ColorID | [F3 狀態旗標](/sensors/line8/reference.md#flags) |  |
| 25 | Cyan | F1 動作完成次數 | 黑色校準 |
| 26 | Magenta | F2 動作完成次數 | 白色校準 |
| 27 | Yellow | 裝置 ID | 0x16 |
| 28 | Key | hw[0] | 8 |
| 29 | — | CRC-8 | 多項式 0x07，涵蓋 0~28；函式庫自己檢查 |

**動作**：Cyan = 黑色校準、Magenta = 白色校準

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
int linePos = bigU8(0) - 8;   // 線位置
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/line8.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- B-11：細位置 02 與線位置 01 同號（負 = 左）；v1 韌體註解寫反，程式碼同號。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- SPIKE 教育版模式 0 的 byte 8~15 補 0（#65 已改，使用者保留）。
- EV3 模式 4 與模式 2 相同內容；模式 3 改壓線位元圖（#67）。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體，0x0B 回 1。
- 函式庫規則（命令 1 回 0x43、CRC-8、最快 5 ms 讀一次）以 `80_上位機研究\MatrixMiniR4\MiniR4ColorSensorExt大通道版` 為準。
