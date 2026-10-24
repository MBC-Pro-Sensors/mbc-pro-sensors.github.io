<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/matrix/matrixblock -->
# Ranger 2-Way (TOF2) × MATRIX × MATRIXblock stock blocks

This page covers **MATRIXblock stock colour-sensor blocks (0x22)** only: how to set it up for the Ranger 2-Way (TOF2) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-matrixblock.en.md + _build/guides/product/tof2/matrix-matrixblock.en.md {"product":"Ranger 2-Way (TOF2)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): MATRIXblock stock colour-sensor blocks (0x22)](/en/sensors/tof2/matrix.md).
<!-- /include-fallback -->

## Reading values

Use the stock MATRIXblock colour-sensor R / G / B / C / M / Y / K blocks (not H / S / V or colour ID); the Arduino-mode line is shown too.

<section id="v01" class="ref-item">

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

Block "colour sensor → R"

```cpp
int leftCm = MiniR4.I2C1.MXColor.getColor(R);   // left distance (cm)
```

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

Block "colour sensor → G"

```cpp
int rightCm = MiniR4.I2C1.MXColor.getColor(G);   // right distance (cm)
```

</section>

<section id="v03" class="ref-item">

### nearer distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="nearer distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | the smaller of left and right, 0~120 cm |

Block "colour sensor → B"

```cpp
int nearCm = MiniR4.I2C1.MXColor.getColor(B);   // nearer distance (cm)
```

</section>

<p class="ref-missing"><b>Not readable here</b>：left distance (mm), right distance (mm), init bitmap, fault bitmap, average distance (mm). Use <a href="/en/sensors/tof2/matrix/bigchannel.md">Big-channel library</a> for these.</p>

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/matrix-matrixblock.en.md`、`_build/guides/product/tof2/matrix-matrixblock.en.md`
