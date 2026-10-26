<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × microbit），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/microbit -->
<!-- noindex -->
# Controller PS4/PS5 × BBC micro:bit V1 / V2

Using the Controller PS4/PS5 with BBC micro:bit V1 / V2: check the edition and the wiring, then pick your software; its page covers that software only.

<figure class="guide-hero"><img src="/images/illust/host-microbit.webp" alt="BBC micro:bit V1 / V2"></figure>

## Which edition

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x42`. It **must use an external 5 V supply** (the ESP32 Bluetooth receiver draws current peaks), not the micro:bit 3V pin; share the supply GND with the micro:bit.

## Wiring

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-microbit.svg" alt="Controller PS4/PS5 × BBC micro:bit V1 / V2" loading="lazy"></figure>

## ② Pick your software

<!-- sw-grid -->

## More

- [Controller PS4/PS5: overview, specs and calibration](/en/sensors/ps4/index.md)
- [Download centre](/en/downloads.md)
