<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × microbit），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/microbit -->
<!-- noindex -->
# 測距者 8 路 (TOF8) × BBC micro:bit V1／V2

測距者 8 路 (TOF8)用在 BBC micro:bit V1／V2：先確認要買哪個版本、怎麼接，再選你用的軟體，之後整頁都只講那個軟體。

<figure class="guide-hero"><img src="/images/illust/host-microbit.webp" alt="BBC micro:bit V1／V2"></figure>

## 要買哪個版本

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x29`。感應器要 5 V 供電，micro:bit 本身只有 3.3 V，請用有 5 V 輸出的擴充板。

## 怎麼接

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-microbit.svg" alt="測距者 8 路 (TOF8) × BBC micro:bit V1／V2" loading="lazy"></figure>

## ② 選你用的軟體

<!-- sw-grid -->

## 其他資料

- [測距者 8 路 (TOF8)產品介紹、規格與校準](/sensors/tof8/index.md)
- [下載中心](/downloads.md)
