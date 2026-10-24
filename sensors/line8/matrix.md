<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line8.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/line8/matrix/matrixblock.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line8/matrix/matrixblock.md">循行者 8 路 › MATRIX › MATRIXblock 原廠積木</a></p>

# 循行者 8 路 × MATRIX Mini R4

[🔙 回到循行者 8 路產品頁](/sensors/line8/index.md)

循行者 8 路 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，用 **原廠顏色感應器積木** 讀：不用換函式庫，R~K 七個 0~255 的數。

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

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/line8.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- B-11：細位置 02 與線位置 01 同號（負 = 左）；v1 韌體註解寫反，程式碼同號。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- SPIKE 教育版模式 0 的 byte 8~15 補 0（#65 已改，使用者保留）。
- EV3 模式 4 與模式 2 相同內容；模式 3 改壓線位元圖（#67）。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體，0x0B 回 1。
