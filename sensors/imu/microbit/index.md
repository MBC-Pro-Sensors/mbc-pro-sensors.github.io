<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × microbit），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/microbit -->
<!-- noindex -->
# 陀螺儀 IMU 九軸 × BBC micro:bit V1／V2

陀螺儀 IMU 九軸用在 BBC micro:bit V1／V2：先確認要買哪個版本、怎麼接，再選你用的軟體，之後整頁都只講那個軟體。

<figure class="guide-hero"><img src="/images/illust/host-microbit.webp" alt="BBC micro:bit V1／V2"></figure>

## 要買哪個版本

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x6A`。這款耗電小，VCC 可以先接 micro:bit 的 **3V** 腳（V1 約 90 mA、V2 約 190 mA）；用電池供電時電壓可能掉到 3 V 以下，不穩就改外接 5 V（GND 要和 micro:bit 共地）。

## 怎麼接

<figure class="ref-fig"><img src="/images/figures/imu/wiring-microbit.svg" alt="陀螺儀 IMU 九軸 × BBC micro:bit V1／V2" loading="lazy"></figure>

## ② 選你用的軟體

<!-- sw-grid -->

## 其他資料

- [陀螺儀 IMU 九軸產品介紹、規格與校準](/sensors/imu/index.md)
- [下載中心](/downloads.md)
