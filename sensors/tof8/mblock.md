<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（mblock），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# 測距者 8 路 × mBlock 5（MakeBlock 版）

[🔙 回到測距者 8 路產品頁](/sensors/tof8/index.md)

測距者 8 路 MakeBlock 版接在 mBot、Ranger、Orion、MegaPi 的 RJ25 連接埠，用 **MBC Pro Sensors 擴展 v2** 的積木讀值。擴展安裝與各主板可用連接埠請看 [MakeBlock 積木擴展](/makeblock.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock current" href="/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/tof8/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-mbot.svg" alt="測距者 8 路 × mBot" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收）。UART 38400 8N1。

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 測距者 8 路 連接埠 ［埠］ 最近距離 cm | 0~120 cm | [`01`](/sensors/tof8/reference.md#v01) | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |
| 測距者 8 路 連接埠 ［埠］ 最近方向 | -8~+8 | [`02`](/sensors/tof8/reference.md#v02) | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |
| 測距者 8 路 連接埠 ［埠］ 最空曠距離 cm | 0~120 cm | [`03`](/sensors/tof8/reference.md#v03) | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |
| 測距者 8 路 連接埠 ［埠］ 最空曠方向 | -7~+7 | [`04`](/sensors/tof8/reference.md#v04) | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |
| 測距者 8 路 連接埠 ［埠］ 正前方距離 cm | 0~120 cm | [`05`](/sensors/tof8/reference.md#v05) | 第 4、5 顆（兩顆平行朝前）平均 0~120 cm；EV3 模式 3、SPIKE 模式 6、大通道 [16..17] 給的是同一個值的 mm；0x22 的 C 是 min(第 4, 第 5 顆) 不是平均（見 10） |
| 測距者 8 路 連接埠 ［埠］ 最近距離 mm | 0~1200 mm | [`06`](/sensors/tof8/reference.md#v06) | 8 顆 mm 的最小值 0~1200（失效那顆以 1200 參與），1200 = 四周沒東西 |
| 測距者 8 路 連接埠 ［埠］ 初始化位元圖 | 0~255 bit | [`07`](/sensors/tof8/reference.md#v07) | bit0 = 第 1 顆（最左）… bit7 = 第 8 顆，1 = 那顆開機初始化成功（8 顆約 2~4 秒，失敗不重試）；255 = 全部好 |
| 測距者 8 路 連接埠 ［埠］ 故障位元圖 | 0~255 bit | [`08`](/sensors/tof8/reference.md#v08) | bit0 = 第 1 顆 … bit7 = 第 8 顆，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |
| 測距者 8 路 連接埠 ［埠］ 正前方距離 mm | 0~1200 mm | [`09`](/sensors/tof8/reference.md#v09) | 第 4、5 顆（正前方兩顆）平均，0~1200 mm；1200 = 前面沒東西。cm 版是 05 |
| 測距者 8 路 連接埠 ［埠］ 第 ［1~8］ 顆距離 cm | 0~120 cm | [`10~17`](/sensors/tof8/reference.md#v10) | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |
| 測距者 8 路 連接埠 ［埠］ 第 ［1~8］ 顆距離 mm | 0~1200 mm | [`18~1F`](/sensors/tof8/reference.md#v18) | 第 1~8 顆各自的距離 0~1200 mm，1200 = 那個方向沒東西／那顆失效；編號 18~1F |

## 狀態積木

擴展把 `F3` 各 bit 拆成布林積木：

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 8 顆都初始化完（開機約 2~4 秒）、快照開始更新後恆 1 |
| b1 | 任一顆失效 (`fault`) | 初始化失敗、I2C 讀失敗或 200 ms 沒更新，任一顆就是 1（= 08 ≠ 0 或 07 ≠ 255） |

## 封包格式（給自己寫解碼器的人）

<figure class="ref-fig"><img src="/images/figures/common/packet-mblock.svg" alt="mBlock 3-byte packet" loading="lazy"></figure>

| 動作 | 格式 |
| :--- | :--- |
| 讀一個值 | 主機送 [編號] → 回 [值低][值高][編號^值低^值高] |
| 寫入命令 | 寫入命令 E0~EE：收齊參數後回 [00][00][編號] 再執行；20 ms 內沒收齊就丟棄 |
| 不認得的編號 | [00][80][編號^0x80]（= -32768） |
| 整包 FD | 送 `[FD]` → 回 `[AA][19][25 個 int16 小端][xor]`，xor = AA ^ N ^ 所有資料 byte；內容 = 本頁值表由小到大排（不含共用編號與命令）。擴展用它一次抓全部、10 ms 內的積木直接讀快取。 |
| 共用編號 | `F0` 裝置 ID、`F1` 協議版本、`F2` 韌體版本、`F3` 狀態旗標 |

<figure class="ref-fig"><img src="/images/figures/common/packet-fd.svg" alt="FD pack-all packet" loading="lazy"></figure>

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
- 積木文字以擴展 v2.0.0 產生器 `build_mext.js` 的表為準；本頁由同一份 JSON 產生。
