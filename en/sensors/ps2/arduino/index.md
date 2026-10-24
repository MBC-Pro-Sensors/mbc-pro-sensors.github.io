<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × arduino），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/arduino -->
<!-- noindex -->
# Controller 2.4G (PS2) × Arduino / ESP32 / UNO R4 (I2C)

Using the Controller 2.4G (PS2) with Arduino / ESP32 / UNO R4 (I2C): check the edition and the wiring, then pick your software; its page covers that software only.

<figure class="guide-hero"><img src="/images/illust/host-arduino.webp" alt="Arduino / ESP32 / UNO R4 (I2C)"></figure>

## Which edition

Get the **Universal I2C edition** (4 pins: 5V, GND, SDA, SCL), factory I2C address `0x42`. UNO / Nano: A4 (SDA), A5 (SCL); UNO R4 and ESP32: the pins marked SDA / SCL.

## Wiring

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-uno.svg" alt="Controller 2.4G (PS2) × Arduino / ESP32 / UNO R4 (I2C)" loading="lazy"></figure>

## ② Pick your software

<!-- sw-grid -->

## More

- [Controller 2.4G (PS2): overview, specs and calibration](/en/sensors/ps2/index.md)
- [Download centre](/en/downloads.md)
