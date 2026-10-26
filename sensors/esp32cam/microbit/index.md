<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × microbit），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/microbit -->
<!-- noindex -->
# 神攝手 ESP32CAM × BBC micro:bit V1／V2

神攝手 ESP32CAM用在 BBC micro:bit V1／V2：先確認要買哪個版本、怎麼接，再選你用的軟體，之後整頁都只講那個軟體。

<figure class="guide-hero"><img src="/images/illust/host-microbit.webp" alt="BBC micro:bit V1／V2"></figure>

## 要買哪個版本

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x4C`。**一定要外接 5 V 供電**（ESP32 相機＋螢幕＋補光燈），不能吃 micro:bit 的 3V 腳；外部電源的 GND 要和 micro:bit 共地。

## 怎麼接

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-microbit.svg" alt="神攝手 ESP32CAM × BBC micro:bit V1／V2" loading="lazy"></figure>

## ② 選你用的軟體

<!-- sw-grid -->

## 其他資料

- [神攝手 ESP32CAM產品介紹、規格與校準](/sensors/esp32cam/index.md)
- [下載中心](/downloads.md)
