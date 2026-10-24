<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（exp6 × arduino），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: exp6/arduino -->
<!-- noindex -->
# SPIKE 6-Way Expander (EXP6) × Arduino / ESP32 / UNO R4 (I2C)

Using the SPIKE 6-Way Expander (EXP6) with Arduino / ESP32 / UNO R4 (I2C): check the edition and the wiring, then pick your software; its page covers that software only.

<figure class="guide-hero"><img src="/images/illust/host-arduino.webp" alt="Arduino / ESP32 / UNO R4 (I2C)"></figure>

## Which edition

The EXP6 **cannot use I2C** with Arduino (the host-port isolator is one-way); use **serial (UART)** instead, ideally with an Arduino UNO R4, Mega or ESP32.

## ② Pick your software

<!-- sw-grid -->

## More

- [SPIKE 6-Way Expander (EXP6): overview, specs and calibration](/en/sensors/exp6/index.md)
- [Download centre](/en/downloads.md)
