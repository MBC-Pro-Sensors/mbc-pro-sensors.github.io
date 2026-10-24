<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × arduino），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/arduino -->
<!-- noindex -->
# 掌控者 PS2 (PS2) × Arduino／ESP32／UNO R4（I2C）

掌控者 PS2 (PS2)用在 Arduino／ESP32／UNO R4（I2C）：先確認要買哪個版本、怎麼接，再選你用的軟體，之後整頁都只講那個軟體。

<figure class="guide-hero"><img src="/images/illust/host-arduino.webp" alt="Arduino／ESP32／UNO R4（I2C）"></figure>

## 要買哪個版本

買 **通用 I2C 版**（4 pin：5V、GND、SDA、SCL），出廠 I2C 位址 `0x42`。UNO／Nano 接 A4（SDA）、A5（SCL）；UNO R4、ESP32 接板子上標 SDA／SCL 的腳。

## 怎麼接

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-uno.svg" alt="掌控者 PS2 (PS2) × Arduino／ESP32／UNO R4（I2C）" loading="lazy"></figure>

## ② 選你用的軟體

<!-- sw-grid -->

## 其他資料

- [掌控者 PS2 (PS2)產品介紹、規格與校準](/sensors/ps2/index.md)
- [下載中心](/downloads.md)
