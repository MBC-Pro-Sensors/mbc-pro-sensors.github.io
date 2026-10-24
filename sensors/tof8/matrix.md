<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof8/matrix/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof8/matrix/index.md">測距者 8 路 › MATRIX</a></p>

# 測距者 8 路 × MATRIX Mini R4

[🔙 回到測距者 8 路產品頁](/sensors/tof8/index.md)

測距者 8 路 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，有兩種用法：**A. 原廠顏色感應器積木**（不用換函式庫，R~K 七個 0~255 的數）與 **B. LauMo 大通道**（換成大通道函式庫，一包 30 byte 什麼都有）。兩種要裝不同的函式庫檔，不能同時用。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/tof8/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-matrix.svg" alt="測距者 8 路 × MATRIX Mini R4" loading="lazy"></figure>

用 MATRIX 原廠 4 pin 感應器線接 I2C1 或 I2C2，積木裡選對應的孔。

## A. 原廠積木（0x22）

積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數。⚠️ 不要用原廠的 H／S／V、顏色辨識積木（那是主機拿 R、G、B 自己算的，對本產品沒有意義）；灰階、顏色編號積木已被 MATRIX 拿掉。

| 積木 | 內容 | 解法 |
| :--- | :--- | :--- |
| R | [第 1~8 顆距離 cm](/sensors/tof8/reference.md#v10) 第 1~2 路 | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| G | [第 1~8 顆距離 cm](/sensors/tof8/reference.md#v10) 第 3~4 路 | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| B | [第 1~8 顆距離 cm](/sensors/tof8/reference.md#v10) 第 5~6 路 | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| C | [第 1~8 顆距離 cm](/sensors/tof8/reference.md#v10) 第 7~8 路 | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| M | [第 1~8 顆距離 cm](/sensors/tof8/reference.md#v10) 第 9~10 路 | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| Y | [第 1~8 顆距離 cm](/sensors/tof8/reference.md#v10) 第 11~12 路 | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |
| K | [第 1~8 顆距離 cm](/sensors/tof8/reference.md#v10) 第 13~14 路 | #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0 |

```cpp
// MATRIXblock → Arduino：原廠函式庫
int r = MiniR4.I2C1.MXColor.getColor(R);   // 第 1~8 顆距離 cm 第 1~2 路
```

## B. 大通道（LauMo 函式庫）

函式庫 `MiniR4ColorSensorExt大通道版`（[下載](/downloads.md)）換掉原廠檔後，同一組積木變成「指標讀法」：ColorID 把指標歸零並回傳狀態、Blue 讀 1 byte、Green 讀 1 byte 有正負、Saturation 讀 2 byte、Hue 讀 2 byte 有正負、Value 讀 4 byte 小數、Red 把指標往後移「上一個讀取的大小」。

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | 積木 | 內容 | 備註 |
| :--- | :--- | :--- | :--- |
| 9 | Blue | [最近距離 cm](/sensors/tof8/reference.md#v01) |  |
| 8 | Blue | [最近方向](/sensors/tof8/reference.md#v02) + 8 | 原始方位 0~16 |
| 11 | Blue | [最空曠距離 cm](/sensors/tof8/reference.md#v03) |  |
| 10 | Blue | [最空曠方向](/sensors/tof8/reference.md#v04) + 8 | 原始方位 1~15 |
| 14~15 | Saturation | [最近距離 mm](/sensors/tof8/reference.md#v06) | #67 追加（[14..15] 小端） |
| 12 | Blue | [初始化位元圖](/sensors/tof8/reference.md#v07) | #67 補填（註解有寫、原本沒填） |
| 13 | Blue | [故障位元圖](/sensors/tof8/reference.md#v08) | #67 補填 |
| 16~17 | Saturation | [正前方距離 mm](/sensors/tof8/reference.md#v09) | #67 追加（MATRIX 定案 §4：[16..17] 正前方 mm 小端） |
| 0~7 | Blue | [第 1~8 顆距離 cm](/sensors/tof8/reference.md#v10)（每路 1 byte） |  |
| 24 | ColorID | [F3 狀態旗標](/sensors/tof8/reference.md#flags) |  |
| 25 | Cyan | F1 動作完成次數 | 0 |
| 26 | Magenta | F2 動作完成次數 | 0 |
| 27 | Yellow | 裝置 ID | 0x29 |
| 28 | Key | hw[0] | 8 |
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
int nearCm = bigU8(9);   // 最近距離 cm
```

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
- 函式庫規則（命令 1 回 0x43、CRC-8、最快 5 ms 讀一次）以 `80_上位機研究\MatrixMiniR4\MiniR4ColorSensorExt大通道版` 為準。
