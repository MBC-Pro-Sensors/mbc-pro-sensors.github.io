<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（exp6 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: exp6/arduino/ide -->
# SPIKE 6 路擴充器 (EXP6) × Arduino × Arduino IDE

這一頁只講 **Arduino IDE（C++）**：在 Arduino／ESP32／UNO R4（I2C） 上用它讀SPIKE 6 路擴充器 (EXP6)的安裝步驟、每個值怎麼讀與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

EXP6 **不能用 I2C** 接 Arduino（主機接頭的隔離晶片只能單向傳送）；改用 **序列埠（UART）**，建議 Arduino UNO R4、Mega 或 ESP32。

## 安裝與設定

<!-- setup: _build/guides/software/arduino-ide.zh.md + _build/guides/product/exp6/arduino-ide.zh.md {"product":"SPIKE 6 路擴充器 (EXP6)","edition":"序列埠（UART）連接","port":"I2C","lib":"","libUrl":"","addr":"","skipWiring":true} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[SPIKE 6 路擴充器 (EXP6)：Arduino IDE（C++） 說明](/sensors/exp6/arduino-i2c.md)。
<!-- /include-fallback -->

## 範例

<!-- examples: sensors/exp6/arduino-examples.md host=arduino sw=ide whole -->

## 其他問題

- **一直讀到 -32768？** 代表模組沒回應：檢查 SDA／SCL 有沒有接反、位址對不對、5V／GND 有沒有接好。
- **可以接很多顆嗎？** 可以，每顆位址要不同；同型號用命令 F8 換位址（見通用 I2C 協議）。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [全部範例 exp6_arduino_examples.zip](/examples/exp6/arduino/exp6_arduino_examples.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 安裝與設定：`_build/guides/software/arduino-ide.zh.md`、`_build/guides/product/exp6/arduino-ide.zh.md`
- 範例：`sensors/exp6/arduino-examples.md`
