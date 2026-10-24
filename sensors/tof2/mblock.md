<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof2.json 產生（mblock），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof2/mblock/mblock5.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof2/mblock/mblock5.md">測距者 2 路 › mBlock › mBlock 5</a></p>

# 測距者 2 路 × mBlock 5（MakeBlock 版）

[🔙 回到測距者 2 路產品頁](/sensors/tof2/index.md)

測距者 2 路 MakeBlock 版接在 mBot、Ranger、Orion、MegaPi 的 RJ25 連接埠，用 **MBC Pro Sensors 擴展 v2** 的積木讀值。擴展安裝與各主板可用連接埠請看 [MakeBlock 積木擴展](/makeblock.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof2/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock current" href="/sensors/tof2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/tof2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/tof2/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-mbot.svg" alt="測距者 2 路 × mBot" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收）。UART 38400 8N1。

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 測距者 2 路 連接埠 ［埠］ 左距離 cm | 0~120 cm | [`01`](/sensors/tof2/reference.md#v01) | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |
| 測距者 2 路 連接埠 ［埠］ 右距離 cm | 0~120 cm | [`02`](/sensors/tof2/reference.md#v02) | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |
| 測距者 2 路 連接埠 ［埠］ 較近距離 cm | 0~120 cm | [`03`](/sensors/tof2/reference.md#v03) | 左右兩顆取較近的 0~120 cm |
| 測距者 2 路 連接埠 ［埠］ 左距離 mm | 0~1200 mm | [`04`](/sensors/tof2/reference.md#v04) | 左邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |
| 測距者 2 路 連接埠 ［埠］ 右距離 mm | 0~1200 mm | [`05`](/sensors/tof2/reference.md#v05) | 右邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |
| 測距者 2 路 連接埠 ［埠］ 初始化位元圖 | 0~3 bit | [`06`](/sensors/tof2/reference.md#v06) | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不重試）；3 = 兩顆都好 |
| 測距者 2 路 連接埠 ［埠］ 故障位元圖 | 0~3 bit | [`07`](/sensors/tof2/reference.md#v07) | bit0 = 左、bit1 = 右，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |
| 測距者 2 路 連接埠 ［埠］ 左右平均 mm | 0~1200 mm | [`08`](/sensors/tof2/reference.md#v08) | (左 mm + 右 mm) / 2，0~1200；跟 EV3 模式 3、SPIKE 模式 6 第 3 個值一致（共識 #8 補上） |

## 狀態積木

擴展把 `F3` 各 bit 拆成布林積木：

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 2 顆都初始化完、快照開始更新後恆 1 |
| b1 | 任一顆失效 (`fault`) | 初始化失敗、I2C 讀失敗或 200 ms 沒更新，任一顆就是 1（= 07 ≠ 0 或 06 ≠ 3） |

## 封包格式（給自己寫解碼器的人）

<figure class="ref-fig"><img src="/images/figures/common/packet-mblock.svg" alt="mBlock 3-byte packet" loading="lazy"></figure>

| 動作 | 格式 |
| :--- | :--- |
| 讀一個值 | 主機送 [編號] → 回 [值低][值高][編號^值低^值高] |
| 寫入命令 | 寫入命令 E0~EE：收齊參數後回 [00][00][編號] 再執行；20 ms 內沒收齊就丟棄 |
| 不認得的編號 | [00][80][編號^0x80]（= -32768） |
| 整包 FD | 送 `[FD]` → 回 `[AA][08][8 個 int16 小端][xor]`，xor = AA ^ N ^ 所有資料 byte；內容 = 本頁值表由小到大排（不含共用編號與命令）。擴展用它一次抓全部、10 ms 內的積木直接讀快取。 |
| 共用編號 | `F0` 裝置 ID、`F1` 協議版本、`F2` 韌體版本、`F3` 狀態旗標 |

<figure class="ref-fig"><img src="/images/figures/common/packet-fd.svg" alt="FD pack-all packet" loading="lazy"></figure>

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 共識 #8：08 左右平均 mm 補進 I2C／mBlock 值表（定案 §5 已列）。
- 0x22 命令 4~12 不動（4 左、5 右、6 最近、7~11 = 0、12 = 最近）；schema 不收 11/12，所以 12 只寫在 03 的 note。
- EV3：模式 0、2、4 鎖住（照盤點）；模式 1 改 [左,右]×4（index 0/1 各代表左右）、模式 3 改平均 mm、模式 5 改 [左 mm, 右 mm]；模式 1 描述表宣告 int8 −100~100，cm×100/120 在範圍內。
- SPIKE：0/5/7/9 鎖住，另加模式 3 鎖住（網站 sensors/tof2/index.md read(3) = 左／右 cm，共識 #11）；模式 6 改 [左 mm, 右 mm, 平均 mm]；模式 1、2、4、8 維持不動；模式 8 v[0] 改 ID|hw[0]<<8（統一後共用函式）。
- 「左」= 站在車後往前看的左（第 0 顆，XSHUT PA6）；韌體只有 L/R 巨集名，盤點檔標為推測。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 積木文字以擴展 v2.0.0 產生器 `build_mext.js` 的表為準；本頁由同一份 JSON 產生。
