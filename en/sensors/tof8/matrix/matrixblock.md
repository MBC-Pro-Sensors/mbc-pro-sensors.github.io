<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/matrix/matrixblock -->
# Ranger 8-Way (TOF8) × MATRIX × MATRIXblock stock blocks

This page covers **MATRIXblock stock colour-sensor blocks (0x22)** only: how to set it up for the Ranger 8-Way (TOF8) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-matrixblock.en.md + _build/guides/product/tof8/matrix-matrixblock.en.md {"product":"Ranger 8-Way (TOF8)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): MATRIXblock stock colour-sensor blocks (0x22)](/en/sensors/tof8/matrix.md).
<!-- /include-fallback -->

## Reading values

Use the stock MATRIXblock colour-sensor R / G / B / C / M / Y / K blocks (not H / S / V or colour ID); the Arduino-mode line is shown too.

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

Blocks "colour sensor → R~K": #67 改（使用者最終定案）：R、G、B = 第 1、2、3 顆；C = min(第 4, 第 5 顆)（正前方取較近）；M、Y、K = 第 6、7、8 顆；全部 cm 0~120；11、12 = 0

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(R);   // group 1
```

</section>

<p class="ref-missing"><b>Not readable here</b>：nearest distance (cm), nearest direction, farthest distance (cm), farthest direction, front distance (cm), nearest distance (mm), init bitmap, fault bitmap, front distance (mm), sensor 1~8 distance (mm). Use <a href="/en/sensors/tof8/matrix/bigchannel.md">Big-channel library</a> for these.</p>

## Examples

<p class="no-examples">There is no full example program for this software yet. Each item under “Reading values” above comes with code you can use as is; put them together in order, or <a href="/en/contact.html">contact us</a> for a full example.</p>

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/matrix-matrixblock.en.md`、`_build/guides/product/tof8/matrix-matrixblock.en.md`
