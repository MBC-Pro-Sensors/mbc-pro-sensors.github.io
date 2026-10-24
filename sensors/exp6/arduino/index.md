<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（exp6 × arduino），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: exp6/arduino -->
<!-- noindex -->
# SPIKE 6 路擴充器 (EXP6) × Arduino／ESP32／UNO R4（I2C）

SPIKE 6 路擴充器 (EXP6)用在 Arduino／ESP32／UNO R4（I2C）：先確認要買哪個版本、怎麼接，再選你用的軟體，之後整頁都只講那個軟體。

<figure class="guide-hero"><img src="/images/illust/host-arduino.webp" alt="Arduino／ESP32／UNO R4（I2C）"></figure>

## 要買哪個版本

EXP6 **不能用 I2C** 接 Arduino（主機接頭的隔離晶片只能單向傳送）；改用 **序列埠（UART）**，建議 Arduino UNO R4、Mega 或 ESP32。

## ② 選你用的軟體

<!-- sw-grid -->

## 其他資料

- [SPIKE 6 路擴充器 (EXP6)產品介紹、規格與校準](/sensors/exp6/index.md)
- [下載中心](/downloads.md)
