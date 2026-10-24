<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/matrix/matrixblock -->
# Controller 2.4G (PS2) × MATRIX × MATRIXblock stock blocks

This page covers **MATRIXblock stock colour-sensor blocks (0x22)** only: how to set it up for the Controller 2.4G (PS2) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **MATRIX edition** and connect it with the stock MATRIX 4-pin cable to I2C1 or I2C2 of the Mini R4 (address `0x22`); pick that port in the block.

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-matrix.svg" alt="Controller 2.4G (PS2) × MATRIX Mini R4" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/matrix-matrixblock.en.md + _build/guides/product/ps2/matrix-matrixblock.en.md {"product":"Controller 2.4G (PS2)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): MATRIXblock stock colour-sensor blocks (0x22)](/en/sensors/ps2/matrix.md).
<!-- /include-fallback -->

## Reading values

Use the stock MATRIXblock colour-sensor R / G / B / C / M / Y / K blocks (not H / S / V or colour ID); the Arduino-mode line is shown too.

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

Block "colour sensor → R"

```cpp
int dpad = MiniR4.I2C1.MXColor.getColor(R);   // d-pad
```

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

Block "colour sensor → G"

```cpp
int shoulder = MiniR4.I2C1.MXColor.getColor(G);   // shoulder buttons
```

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |

Block "colour sensor → K"

```cpp
int connected = MiniR4.I2C1.MXColor.getColor(K);   // connected
```

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

Blocks "colour sensor → B~Y": #67 新增：B、C、M、Y = LX、LY、RX、RY

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(B);   // group 1
```

</section>

<p class="ref-missing"><b>Not readable here</b>：16 buttons, shape buttons, function buttons, battery, L2 analog, R2 analog, system keys, gamepad type. Use <a href="/en/sensors/ps2/arduino/ide.md">Arduino</a> / <a href="/en/sensors/ps2/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/ps2/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

This software cannot send commands; use Arduino I2C, mBlock or micro:bit if you need them.

## Examples

<p class="no-examples">There is no full example program for this software yet. Each item under “Reading values” and “Commands” above comes with code you can use as is; put them together in order, or <a href="/en/contact.html">contact us</a> for a full example.</p>

## More questions



Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/matrix-matrixblock.en.md`、`_build/guides/product/ps2/matrix-matrixblock.en.md`
