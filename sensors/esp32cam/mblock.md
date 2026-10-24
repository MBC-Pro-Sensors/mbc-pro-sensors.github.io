<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_school.json 產生（mblock），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/esp32cam/mblock/mblock5.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/esp32cam/mblock/mblock5.md">神攝手 ESP32CAM › mBlock › mBlock 5</a></p>

# 神攝手 學校版 × mBlock 5（MakeBlock 版）

[🔙 回到神攝手 學校版產品頁](/sensors/esp32cam/index.md)

神攝手 學校版 MakeBlock 版接在 mBot、Ranger、Orion、MegaPi 的 RJ25 連接埠，用 **MBC Pro Sensors 擴展 v2** 的積木讀值。擴展安裝與各主板可用連接埠請看 [MakeBlock 積木擴展](/makeblock.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock current" href="/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off">MATRIX</span><a class="chip chip-microbit" href="/sensors/esp32cam/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-mbot.svg" alt="神攝手 學校版 × mBot" loading="lazy"></figure>

感應器 → mBot 插座片 → 主板 RJ25 連接埠（S1 = 主機送、S2 = 主機收）。UART 38400 8N1。

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| 神攝手 連接埠 ［埠］ 色相 | -1~100 | [`01`](/sensors/esp32cam/reference.md#v01) | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |
| 神攝手 連接埠 ［埠］ 飽和度 | 0~100 | [`02`](/sensors/esp32cam/reference.md#v02) | S1 取樣點飽和度 0~100 |
| 神攝手 連接埠 ［埠］ 明度 | 0~100 | [`03`](/sensors/esp32cam/reference.md#v03) | S1 取樣點明度 0~100 |
| 神攝手 連接埠 ［埠］ 動態 | 0~100 | [`04`](/sensors/esp32cam/reference.md#v04) | 全畫面動態量 0~100（畫面變化越多越大） |
| 神攝手 連接埠 ［埠］ 色塊 X | 0~100 | [`05`](/sensors/esp32cam/reference.md#v05) | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |
| 神攝手 連接埠 ［埠］ 色塊 Y | 0~75 | [`06`](/sensors/esp32cam/reference.md#v06) | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |
| 神攝手 連接埠 ［埠］ 色塊寬 | 0~100 | [`07`](/sensors/esp32cam/reference.md#v07) | 色塊寬 W 0~100；0 = 沒找到 |
| 神攝手 連接埠 ［埠］ 色塊高 | 0~75 | [`08`](/sensors/esp32cam/reference.md#v08) | 色塊高 H 0~75；沒找到 = 0 |
| 神攝手 連接埠 ［埠］ 色塊面積 | 0~7500 | [`09`](/sensors/esp32cam/reference.md#v09) | W×H（0~7500）；沒找到 = 0 |
| 神攝手 連接埠 ［埠］ 有找到色塊？ | 0~1 | [`0A`](/sensors/esp32cam/reference.md#v0a) | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |

## 狀態積木

擴展把 `F3` 各 bit 拆成布林積木：

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1（相機進選單時值不更新，但仍 1） |
| b1 | 有找到色塊 (`found`) | = 值 0A |

## 封包格式（給自己寫解碼器的人）

<figure class="ref-fig"><img src="/images/figures/common/packet-mblock.svg" alt="mBlock 3-byte packet" loading="lazy"></figure>

| 動作 | 格式 |
| :--- | :--- |
| 讀一個值 | 主機送 [編號] → 回 [值低][值高][編號^值低^值高] |
| 寫入命令 | 寫入命令 E0~EE：收齊參數後回 [00][00][編號] 再執行；20 ms 內沒收齊就丟棄 |
| 不認得的編號 | [00][80][編號^0x80]（= -32768） |
| 整包 FD | 送 `[FD]` → 回 `[AA][0A][10 個 int16 小端][xor]`，xor = AA ^ N ^ 所有資料 byte；內容 = 本頁值表由小到大排（不含共用編號與命令）。擴展用它一次抓全部、10 ms 內的積木直接讀快取。 |
| 共用編號 | `F0` 裝置 ID、`F1` 協議版本、`F2` 韌體版本、`F3` 狀態旗標 |

<figure class="ref-fig"><img src="/images/figures/common/packet-fd.svg" alt="FD pack-all packet" loading="lazy"></figure>

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/cam_school.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 1（共識 #6：神攝手 1 = 學校版）；model 字串 CAMSCH 照任務指定（韌體原本沒有型號欄位）。
- D4：I2C 0x4C 用 SDA = IO3、SCL = IO1（沿 PS4 Wire.setPins 慣例），與 EV3／SPIKE 串口模式二選一編譯；ESP32 從機不能時脈延展，onReceive 預載 32 B；頁面標「硬體已確認，上機待測」。mBlock 用同一組腳 Serial.begin(38400, SERIAL_8N1, 3, 1)。
- EV3 六個模式全部鎖住照盤點（模式 3／5 的 −1 已在 #65 修成符號延伸）；EV3 定案最後的 golden 備註把 cam_school 列進「模式 5 改 ID」是筆誤（定案表本身寫「—」；模式 5 仍是 [色相, 飽和]），所以 ev3IdMode = null。
- SPIKE 模式 9 鎖住，只有 byte 8~9（v[4]）從 0 改成面積 W×H；模式 6 改 [X, Y, 面積]；模式 8 = [0x4C | 1<<8, 2, 韌體版, 更新序號]；模式 1~4 維持 0。
- SPIKE 模式 0 的「面積」是 map(像素面積 0~5000 → 0~100)，跟值表 09 的 W×H 不同，鎖住不改（照盤點）。
- mVision：N = 10（idx 0~9），色相 −1 送 255；每 20 ms 一包（B-6）；不做 0x22／大通道（hosts.matrix = false）。
- 0A 有找到色塊 = W > 0（盤點：W 0 = 沒找到）；F3 b1 同這個值。
- 值 05~08 的「沒找到 = 0」照盤點 §0；色塊 Y 下 = 0、上 = 75 照盤點（map(bestY,120,0,0,75)）。
- 沒有寫入命令（韌體無法從程式改目標顏色／補光燈）。
- 積木文字以擴展 v2.0.0 產生器 `build_mext.js` 的表為準；本頁由同一份 JSON 產生。
