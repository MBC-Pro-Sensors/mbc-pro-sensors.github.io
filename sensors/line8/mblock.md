<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line8.json 產生（mblock），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/line8/mblock/mblock5.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line8/mblock/mblock5.md">循行者 8 路 › mBlock › mBlock 5</a></p>

# 循行者 8 路 × mBlock 5（MakeBlock 版）

[🔙 回到循行者 8 路產品頁](/sensors/line8/index.md)

循行者 8 路 MakeBlock 版接在 mBot、Ranger、Orion、MegaPi 的 RJ25 連接埠，用 **MBC Pro Sensors 擴展 v2** 的積木讀值。擴展安裝與各主板可用連接埠請看 [MakeBlock 積木擴展](/makeblock.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/line8/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/line8/ev3-hub.md">EV3</a><a class="chip chip-arduino" href="/sensors/line8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock current" href="/sensors/line8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/line8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/line8/microbit.md">micro:bit</a></p>

> [!NOTE]
> 2026-10 以前出貨的 MakeBlock 版韌體用的是舊版擴展 v1.0.0（下載頁標「舊版」）；新舊積木不相容，請依感應器韌體版本選擇。

## 接線

<figure class="ref-fig"><img src="/images/figures/line8/wiring-mbot.svg" alt="循行者 8 路 × mBot" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收）。UART 38400 8N1。

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 循行者 連接埠 ［埠］ 線的位置 | -8~+8 | [`01`](/sensors/line8/reference.md#v01) | 線在哪一路：−8（最左）~+8（最右），0 = 正中間；站在車後往前看，負 = 線在左 |
| 循行者 連接埠 ［埠］ 線的細位置 | -100~+100 | [`02`](/sensors/line8/reference.md#v02) | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |
| 循行者 連接埠 ［埠］ 線寬 | 0~8 路 | [`03`](/sensors/line8/reference.md#v03) | 壓到線的路數 0~8，0 = 找不到線 |
| 循行者 連接埠 ［埠］ 線段數 | 0~4 | [`04`](/sensors/line8/reference.md#v04) | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |
| 循行者 連接埠 ［埠］ 壓線位元圖 | 0~255 bit | [`05`](/sensors/line8/reference.md#v05) | bit0 = 第 1 路（最左）… bit7 = 第 8 路，1 = 那一路在線上 |
| 循行者 連接埠 ［埠］ 最後離線方向 | -8~+8 | [`06`](/sensors/line8/reference.md#v06) | 丟線前線最後在哪一側：−8 ~ +8（負 = 左），找回線後維持最後一次 |
| 循行者 連接埠 ［埠］ 門檻 | 1~99 | [`07`](/sensors/line8/reference.md#v07) | 判斷「在線上」的光值門檻 1~99，預設 50；用 E4 改 |
| 循行者 連接埠 ［埠］ 校準次數 | 0~255 次 | [`08`](/sensors/line8/reference.md#v08) | 開機後完成校準的次數（按鈕校準也算） |
| 循行者 連接埠 ［埠］ 第 ［1~8］ 路光值 | 0~100 | [`10~17`](/sensors/line8/reference.md#v10) | 第 1~8 路校準後光值 0~100（第 1 路最左）；編號 10~17 |

## 命令積木

| 積木 | 編號 | 說明 |
| :--- | :--- | :--- |
| 循行者 連接埠 ［埠］ 切黑線 | [`E0`](/sensors/line8/reference.md#ce0) | 追黑線（預設） |
| 循行者 連接埠 ［埠］ 切白線 | [`E1`](/sensors/line8/reference.md#ce1) | 追白線（F3 b1 = 1） |
| 循行者 連接埠 ［埠］ 黑色校準 | [`E2`](/sensors/line8/reference.md#ce2) | 把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM（執行中 F3 bit2 = 1） |
| 循行者 連接埠 ［埠］ 白色校準 | [`E3`](/sensors/line8/reference.md#ce3) | 把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM（執行中 F3 bit2 = 1） |
| 循行者 連接埠 ［埠］ 設定門檻 ［門檻］ | [`E4`](/sensors/line8/reference.md#ce4) | 二值化門檻 1~99（RAM，重開機回預設 50） |
| 循行者 連接埠 ［埠］ 5 秒動態校準 | [`E7`](/sensors/line8/reference.md#ce7) | 5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1（執行中 F3 bit2 = 1） |

## 狀態積木

擴展把 `F3` 各 bit 拆成布林積木：

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 白線模式 (`whiteMode`) | 1 = 正在追白線 |
| b2 | 校準中 (`calibrating`) | E2／E3／E7 執行中；此時拒收新的校準命令 |
| b3 | 丟線 (`lost`) | 線段數 = 0 |
| b4 | 分岔 (`fork`) | 線段數 ≥ 2 |

## 封包格式（給自己寫解碼器的人）

<figure class="ref-fig"><img src="/images/figures/common/packet-mblock.svg" alt="mBlock 3-byte packet" loading="lazy"></figure>

| 動作 | 格式 |
| :--- | :--- |
| 讀一個值 | 主機送 [編號] → 回 [值低][值高][編號^值低^值高] |
| 寫入命令 | 寫入命令 E0~EE：收齊參數後回 [00][00][編號] 再執行；20 ms 內沒收齊就丟棄 |
| 不認得的編號 | [00][80][編號^0x80]（= -32768） |
| 整包 FD | 送 `[FD]` → 回 `[AA][10][16 個 int16 小端][xor]`，xor = AA ^ N ^ 所有資料 byte；內容 = 本頁值表由小到大排（不含共用編號與命令）。擴展用它一次抓全部、10 ms 內的積木直接讀快取。 |
| 共用編號 | `F0` 裝置 ID、`F1` 協議版本、`F2` 韌體版本、`F3` 狀態旗標 |

<figure class="ref-fig"><img src="/images/figures/common/packet-fd.svg" alt="FD pack-all packet" loading="lazy"></figure>

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/line8.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- B-11：細位置 02 與線位置 01 同號（負 = 左）；v1 韌體註解寫反，程式碼同號。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- SPIKE 教育版模式 0 的 byte 8~15 補 0（#65 已改，使用者保留）。
- EV3 模式 4 與模式 2 相同內容；模式 3 改壓線位元圖（#67）。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體，0x0B 回 1。
- 積木文字以擴展 v2.0.0 產生器 `build_mext.js` 的表為準；本頁由同一份 JSON 產生。
