<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_school.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/esp32cam/matrix/mvision.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/esp32cam/matrix/mvision.md">神攝手 ESP32CAM › MATRIX › mVision 積木</a></p>

# 神攝手 學校版 × MATRIX Mini R4

[🔙 回到神攝手 學校版產品頁](/sensors/esp32cam/index.md)

神攝手 學校版在 MATRIX 走串口，用 MATRIXblock 內建的 **mVision** 攝影機積木讀（不走 I2C）。

<p class="chips"><a class="chip chip-spike" href="/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off current">MATRIX</span><a class="chip chip-microbit" href="/sensors/esp32cam/microbit.md">micro:bit</a></p>

## mVision 資料格

<figure class="ref-fig"><img src="/images/figures/common/packet-mvision.svg" alt="mVision packet" loading="lazy"></figure>

| 資料 | 值 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| 0 | [色相](/sensors/esp32cam/reference.md#v01) | -1~100 | 0~100；255 = 黑白灰（−1 不能放 uint16，MATRIX 定案 §7）；S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |
| 1 | [飽和度](/sensors/esp32cam/reference.md#v02) | 0~100 | S1 取樣點飽和度 0~100 |
| 2 | [明度](/sensors/esp32cam/reference.md#v03) | 0~100 | S1 取樣點明度 0~100 |
| 3 | [動態](/sensors/esp32cam/reference.md#v04) | 0~100 | 全畫面動態量 0~100（畫面變化越多越大） |
| 4 | [色塊 X](/sensors/esp32cam/reference.md#v05) | 0~100 | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |
| 5 | [色塊 Y](/sensors/esp32cam/reference.md#v06) | 0~75 | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |
| 6 | [色塊寬](/sensors/esp32cam/reference.md#v07) | 0~100 | 色塊寬 W 0~100；0 = 沒找到 |
| 7 | [色塊高](/sensors/esp32cam/reference.md#v08) | 0~75 | 色塊高 H 0~75；沒找到 = 0 |
| 8 | [色塊面積](/sensors/esp32cam/reference.md#v09) | 0~7500 | W×H；W×H（0~7500）；沒找到 = 0 |
| 9 | [有找到色塊](/sensors/esp32cam/reference.md#v0a) | 0~1 | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |

[0xAA][N][N 個 uint16 小端][xor]，xor = AA ^ N ^ 所有資料 byte；N ≤ 10（MATRIXblock 只宣告 CamData[10]）；學校版每 20 ms 一包

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
