<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × microbit），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/microbit -->
<!-- noindex -->
# Sharpshooter Vision (ESP32CAM) × BBC micro:bit V1 / V2

Using the Sharpshooter Vision (ESP32CAM) with BBC micro:bit V1 / V2: check the edition and the wiring, then pick your software; its page covers that software only.

<figure class="guide-hero"><img src="/images/illust/host-microbit.webp" alt="BBC micro:bit V1 / V2"></figure>

## Which edition

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x4C`. The sensor needs 5 V; use a breakout board with a 5 V output.

## Wiring

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-microbit.svg" alt="Sharpshooter Vision (ESP32CAM) × BBC micro:bit V1 / V2" loading="lazy"></figure>

## ② Pick your software

<!-- sw-grid -->

## More

- [Sharpshooter Vision (ESP32CAM): overview, specs and calibration](/en/sensors/esp32cam/index.md)
- [Download centre](/en/downloads.md)
