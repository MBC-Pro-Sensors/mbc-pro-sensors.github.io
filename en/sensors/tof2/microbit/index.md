<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × microbit），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/microbit -->
<!-- noindex -->
# Ranger 2-Way (TOF2) × BBC micro:bit V1 / V2

Using the Ranger 2-Way (TOF2) with BBC micro:bit V1 / V2: check the edition and the wiring, then pick your software; its page covers that software only.

<figure class="guide-hero"><img src="/images/illust/host-microbit.webp" alt="BBC micro:bit V1 / V2"></figure>

## Which edition

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x29`. It draws little current, so VCC can go to the micro:bit **3V** pin (about 90 mA on V1, 190 mA on V2); on batteries the rail can sag below 3 V, so switch to external 5 V if readings are unstable (share GND with the micro:bit).

## Wiring

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-microbit.svg" alt="Ranger 2-Way (TOF2) × BBC micro:bit V1 / V2" loading="lazy"></figure>

## ② Pick your software

<!-- sw-grid -->

## More

- [Ranger 2-Way (TOF2): overview, specs and calibration](/en/sensors/tof2/index.md)
- [Download centre](/en/downloads.md)
