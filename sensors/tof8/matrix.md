<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof8/matrix/matrixblock.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof8/matrix/matrixblock.md">測距者 8 路 › MATRIX › MATRIXblock 原廠積木</a></p>

# 測距者 8 路 × MATRIX Mini R4

[🔙 回到測距者 8 路產品頁](/sensors/tof8/index.md)

測距者 8 路 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，用 **原廠顏色感應器積木** 讀：不用換函式庫，R~K 七個 0~255 的數。

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

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof8.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 正前方 mm（EV3 模式 3、SPIKE 模式 6 v[1]）在定案 §5 沒有獨立 I2C 編號 → 掛在 05 正前方 cm 底下，用 scale:mm 標示，不自己加編號。
- 0x22（使用者 2026-10-21 最終定案）：只用一個位址，R~K 由左到右 7 個 cm：第 1、2、3 顆、min(第 4, 第 5)、第 6、7、8 顆；11、12 不放資料；最近距離／方向由主機自己比較。此排法只用在 0x22，EV3／SPIKE不動。
- SPIKE：0/5/7/9 鎖住，1、2、3、4、8 維持不動；模式 6 改 [最近 mm, 正前方 mm, 最近方向 −8~+8]（原本 = 第 2~7 顆兩兩合併）；模式 8 v[0] 改 ID|hw[0]<<8。
- 最空曠方向 04 範圍 −7~+7（maxDirection 永遠不會是 0／16，盤點 INO:1086-1088）；最近方向 02 ±8 = 沒目標、沿用上次偏向側，開機預設 −8。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 主對話 2026-10-21：補 09 正前方 mm（定案 §5 只有 05 正前方 cm，但 EV3 模式 3、SPIKE 模式 6 都送 mm，I2C／mBlock 應有同一個值）。
