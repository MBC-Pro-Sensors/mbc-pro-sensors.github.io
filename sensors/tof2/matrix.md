<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof2.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof2/matrix/matrixblock.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof2/matrix/matrixblock.md">測距者 2 路 › MATRIX › MATRIXblock 原廠積木</a></p>

# 測距者 2 路 × MATRIX Mini R4

[🔙 回到測距者 2 路產品頁](/sensors/tof2/index.md)

測距者 2 路 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，用 **原廠顏色感應器積木** 讀：不用換函式庫，R~K 七個 0~255 的數。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof2/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/tof2/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/sensors/tof2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/tof2/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-matrix.svg" alt="測距者 2 路 × MATRIX Mini R4" loading="lazy"></figure>

用 MATRIX 原廠 4 pin 感應器線接 I2C1 或 I2C2，積木裡選對應的孔。

## A. 原廠積木（0x22）

積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數。⚠️ 不要用原廠的 H／S／V、顏色辨識積木（那是主機拿 R、G、B 自己算的，對本產品沒有意義）；灰階、顏色編號積木已被 MATRIX 拿掉。

| 積木 | 內容 | 解法 |
| :--- | :--- | :--- |
| R | [左距離 cm](/sensors/tof2/reference.md#v01) |  |
| G | [右距離 cm](/sensors/tof2/reference.md#v02) |  |
| B | [較近距離 cm](/sensors/tof2/reference.md#v03) | B（命令 12 也回較近，schema 不收 12） |

```cpp
// MATRIXblock → Arduino：原廠函式庫
int r = MiniR4.I2C1.MXColor.getColor(R);   // 左距離 cm
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 共識 #8：08 左右平均 mm 補進 I2C／mBlock 值表（定案 §5 已列）。
- 0x22 命令 4~12 不動（4 左、5 右、6 最近、7~11 = 0、12 = 最近）；schema 不收 11/12，所以 12 只寫在 03 的 note。
- EV3：模式 0、2、4 鎖住（照盤點）；模式 1 改 [左,右]×4（index 0/1 各代表左右）、模式 3 改平均 mm、模式 5 改 [左 mm, 右 mm]；模式 1 描述表宣告 int8 −100~100，cm×100/120 在範圍內。
- SPIKE：0/5/7/9 鎖住，另加模式 3 鎖住（網站 sensors/tof2/index.md read(3) = 左／右 cm，共識 #11）；模式 6 改 [左 mm, 右 mm, 平均 mm]；模式 1、2、4、8 維持不動；模式 8 v[0] 改 ID|hw[0]<<8（統一後共用函式）。
- 「左」= 站在車後往前看的左（第 0 顆，XSHUT PA6）；韌體只有 L/R 巨集名，盤點檔標為推測。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
