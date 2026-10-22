<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_wro.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
# 神攝手 WRO 版（HUE12） × MATRIX Mini R4

[🔙 回到神攝手 WRO 版（HUE12）產品頁](/sensors/esp32cam/index.md)

神攝手 WRO 版（HUE12）在 MATRIX 走串口，用 MATRIXblock 內建的 **mVision** 攝影機積木讀（不走 I2C）。

<p class="chips"><a class="chip chip-spike" href="/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off current">MATRIX</span><a class="chip chip-microbit" href="/sensors/esp32cam/microbit.md">micro:bit</a></p>

## mVision 資料格

<figure class="ref-fig"><img src="/images/figures/common/packet-mvision.svg" alt="mVision packet" loading="lazy"></figure>

| 資料 | 值 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| 0 | [色點 1~12 顏色碼](/sensors/esp32cam/reference-wro.md#v10) | 0~255 | 不動：byte 0~11 = S1~S12 顏色碼各 1 byte（idx 0 = S1 低 byte／S2 高 byte … idx 5 = S11／S12）；照 HUE12_AS_mVisionSensor_v1.0.8.mbr4 解碼；10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |
| 6 | [色點 1~12 鮮豔度](/sensors/esp32cam/reference-wro.md#v20) | 0~255 | 不動：byte 12~17 = 12 格各 4 bit（S>>4，1~15；黑 = 1；沒抓到 = 0），byte 12+2j = S(4j+1) \| S(4j+2)<<4、byte 13+2j = S(4j+3) \| S(4j+4)<<4；20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

[0xAA][N][N 個 uint16 小端][xor]，xor = AA ^ N ^ 所有資料 byte；N ≤ 10（MATRIXblock 只宣告 CamData[10]）；學校版每 20 ms 一包

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/cam_wro.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 2（共識 #6：神攝手 2 = WRO 版）；model = HUE12（樂高模式名）。
- I2C 0x4C 與學校版同一位址（主機用 F0 高 byte 1／2 分辨）；D4：SDA = IO3、SCL = IO1，與 EV3／SPIKE 串口模式二選一編譯；ESP32 從機 onReceive 預載 32 B；上機待測。mBlock 用 Serial.begin(38400, SERIAL_8N1, 3, 1)。
- 值表只有 10~1B、20~2B（mBlock 定案）；兩群各 12 個，I2C 一次連續讀 12 個值（24 B）剛好一群。
- EV3／SPIKE 只有一個模式 HUE12（type 95，動態描述表 DATA32×8），鎖住照盤點 §0-2 排法；沒有 ID 模式（type 95 只宣告 1 個模式，不加）。
- mVision 鎖住照盤點 §4：[0xAA][9][18 B][xor]，byte 0~11 顏色碼、12~17 鮮豔度 4 bit；WRO 隊伍的 HUE12_AS_mVisionSensor_v1.0.8.mbr4 在用，不動；B-6：不節流（每圈 loop 送）。
- mVision 的鮮豔度是 S>>4（0~15），值表 20~2B 是完整 0~255；兩者不同是既有設計，不改。
- 顏色碼 255 只代表「色點沒開」，不代表「沒找到」（沒抓到時仍回種子點顏色，盤點 §0-1）。
- 不做 0x22／大通道（hosts.matrix = false）；沒有寫入命令；F3 只有 b0。
