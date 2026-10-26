<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_wro.json 產生（mblock），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/esp32cam/mblock/mblock5.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/esp32cam/mblock/mblock5.md">神攝手 ESP32CAM › mBlock › mBlock 5</a></p>

# 神攝手 WRO 版（HUE12） × mBlock 5（MakeBlock 版）

[🔙 回到神攝手 WRO 版（HUE12）產品頁](/sensors/esp32cam/index.md)

神攝手 WRO 版（HUE12） MakeBlock 版接在 mBot、Ranger、Orion、MegaPi 的 RJ25 連接埠，用 **MBC Pro Sensors 擴展 v2** 的積木讀值。擴展安裝與各主板可用連接埠請看 [MakeBlock 積木擴展](/makeblock.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock current" href="/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off">MATRIX</span><a class="chip chip-microbit" href="/sensors/esp32cam/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-mbot.svg" alt="神攝手 WRO 版（HUE12） × mBot" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收）。UART 38400 8N1。

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 神攝手 WRO 連接埠 ［埠］ 色點 ［1~12］ 顏色碼 | 0~255 | [`10~1B`](/sensors/esp32cam/reference-wro.md#v10) | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |
| 神攝手 WRO 連接埠 ［埠］ 色點 ［1~12］ 鮮豔度 | 0~255 | [`20~2B`](/sensors/esp32cam/reference-wro.md#v20) | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

## 命令積木

| 積木 | 編號 | 說明 |
| :--- | :--- | :--- |
| 神攝手 WRO 連接埠 ［埠］ 更新全部數值 | `FD` | 一次讀整張值表進快取（FD 整包）；接下來 10 ms 內的讀值積木直接用快取，不再逐一詢問 |

## 狀態積木

擴展把 `F3` 各 bit 拆成布林積木：

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1；b1 = 0（WRO 版沒有「有找到色塊」） |

## 封包格式（給自己寫解碼器的人）

<figure class="ref-fig"><img src="/images/figures/common/packet-mblock.svg" alt="mBlock 3-byte packet" loading="lazy"></figure>

| 動作 | 格式 |
| :--- | :--- |
| 讀一個值 | 主機送 [編號] → 回 [值低][值高][編號^值低^值高] |
| 寫入命令 | 寫入命令 E0~EE：收齊參數後回 [00][00][編號] 再執行；20 ms 內沒收齊就丟棄 |
| 不認得的編號 | [00][80][編號^0x80]（= -32768） |
| 整包 FD | 送 `[FD]` → 回 `[AA][18][24 個 int16 小端][xor]`，xor = AA ^ N ^ 所有資料 byte；內容 = 本頁值表由小到大排（不含共用編號與命令）。擴展用它一次抓全部、10 ms 內的積木直接讀快取。 |
| 共用編號 | `F0` 裝置 ID、`F1` 協議版本、`F2` 韌體版本、`F3` 狀態旗標 |

<figure class="ref-fig"><img src="/images/figures/common/packet-fd.svg" alt="FD pack-all packet" loading="lazy"></figure>

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/cam_wro.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 2（共識 #6：神攝手 2 = WRO 版）；model = HUE12（樂高模式名）。
- I2C 0x4C 與學校版同一位址（主機用 F0 高 byte 1／2 分辨）；D4：SDA = IO3、SCL = IO1，與 EV3／SPIKE 串口模式二選一編譯；ESP32 從機 onReceive 預載 32 B；上機待測。mBlock 用 Serial.begin(38400, SERIAL_8N1, 3, 1)。
- 值表只有 10~1B、20~2B（mBlock 定案）；兩群各 12 個，I2C 一次連續讀 12 個值（24 B）剛好一群。
- EV3／SPIKE 只有一個模式 HUE12（type 95，動態描述表 DATA32×8），鎖住照盤點 §0-2 排法；沒有 ID 模式（type 95 只宣告 1 個模式，不加）。
- mVision 鎖住照盤點 §4：[0xAA][9][18 B][xor]，byte 0~11 顏色碼、12~17 鮮豔度 4 bit；WRO 隊伍的 HUE12_AS_mVisionSensor_v1.0.8.mbr4 在用，不動；B-6：不節流（每圈 loop 送）。
- mVision 的鮮豔度是 S>>4（0~15），值表 20~2B 是完整 0~255；兩者不同是既有設計，不改。
- 顏色碼 255 只代表「色點沒開」，不代表「沒找到」（沒抓到時仍回種子點顏色，盤點 §0-1）。
- 不做 0x22（hosts.matrix = false）；沒有寫入命令；F3 只有 b0。
- 積木文字以擴展 v2.0.0 產生器 `build_mext.js` 的表為準；本頁由同一份 JSON 產生。
