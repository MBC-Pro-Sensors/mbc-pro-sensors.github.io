<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/matrix/matrixblock -->
# Pathfinder 16-Way (Line16) × MATRIX × MATRIXblock stock blocks

This page covers **MATRIXblock stock colour-sensor blocks (0x22)** only: how to set it up for the Pathfinder 16-Way (Line16) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-matrixblock.en.md + _build/guides/product/line16/matrix-matrixblock.en.md {"product":"Pathfinder 16-Way (Line16)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 16-Way (Line16): MATRIXblock stock colour-sensor blocks (0x22)](/en/sensors/line16/matrix.md).
<!-- /include-fallback -->

## Reading values

Use the stock MATRIXblock colour-sensor R / G / B / C / M / Y / K blocks (not H / S / V or colour ID); the Arduino-mode line is shown too.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-16~+16` | which channel the line is under: −16 (far left) to +16 (far right), 0 = centre; seen from behind the robot, negative = left (normally ±15; ±16 only for 200 ms after the line leaves the outermost channel) |

Block "colour sensor → R", subtract 16

```cpp
int linePos = MiniR4.I2C1.MXColor.getColor(R) - 16;   // line position
```

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

Block "colour sensor → B", subtract 100

```cpp
int linePosFine = MiniR4.I2C1.MXColor.getColor(B) - 100;   // fine position
```

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~16` 路 | number of channels on the line, 0~16; 0 = no line |

Block "colour sensor → G"

```cpp
int lineWidth = MiniR4.I2C1.MXColor.getColor(G);   // line width
```

</section>

<section id="v10" class="ref-item">

### channel 1~16 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="channel 1~16 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~16, 0~100 (channel 1 = left); ids 10~1F |

Blocks "colour sensor → C~K": C~K 四路一組，每路 2 bit（convert3：0~24→0、25~49→1、50~74→2、75~100→3）：第 1 路在 bit7-6 … 第 4 路在 bit1-0；C = 第 1~4 路、M = 5~8、Y = 9~12、K = 13~16

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(C);   // group 1
```

</section>

<p class="ref-missing"><b>Not readable here</b>：line segments, line bitmap, last exit direction, threshold, calibrations done. Use <a href="/en/sensors/line16/matrix/bigchannel.md">Big-channel library</a> for these.</p>

## Commands

This software cannot send commands; use Arduino I2C, mBlock or micro:bit if you need them.

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line16.json` (single source of truth)
- Setup：`_build/guides/software/matrix-matrixblock.en.md`、`_build/guides/product/line16/matrix-matrixblock.en.md`
