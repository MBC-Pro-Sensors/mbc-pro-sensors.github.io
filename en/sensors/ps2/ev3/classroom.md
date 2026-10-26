<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × ev3 × classroom），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/ev3/classroom -->
# Controller 2.4G (PS2) × EV3 × EV3 Classroom

This page covers **EV3 Classroom (Scratch blocks)** only: how to set it up for the Controller 2.4G (PS2) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-classroom.en.md + _build/guides/product/ps2/ev3-classroom.en.md {"product":"Controller 2.4G (PS2)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): EV3 Classroom (Scratch blocks)](/en/sensors/ps2/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

The EV3 sees a stock LEGO sensor; read it with the stock Classroom sensor blocks.

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| d-pad | Infrared Sensor → Proximity | the value | mode 0 |

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| shape buttons | Infrared Sensor → Beacon heading, channel 2 | the value | mode 1 |

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| shoulder buttons | Infrared Sensor → Beacon heading, channel 3 | the value | mode 1 |

</section>

<section id="v05" class="ref-item">

### function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| function buttons | Infrared Sensor → Beacon heading, channel 4 | the value | mode 1 |

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` (EV3 mode 1, mode 2, mode 4 give a scaled value: 0~99 (50 = 中間)) | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| #1 | Infrared Sensor → Beacon proximity, channel 1 | the value | mode 1 |
| #2 | Infrared Sensor → Beacon heading, channel 2 | the value | mode 1 |
| #3 | Infrared Sensor → Beacon proximity, channel 2 | the value | mode 1 |
| #4 | Infrared Sensor → Beacon heading, channel 3 | the value | mode 1 |

</section>

<p class="ref-missing"><b>Not readable here</b>：16 buttons, connected, battery, L2 analog, R2 analog, system keys, gamepad type. Use <a href="/en/sensors/ps2/ev3/pybricks.md">Pybricks</a> / <a href="/en/sensors/ps2/ev3/clev3r.md">clev3r</a> for these.</p>

## Commands

The EV3 port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/ps2/ev3-examples.md host=ev3 sw=classroom -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/ev3-classroom.en.md`、`_build/guides/product/ps2/ev3-classroom.en.md`
- Examples：`sensors/ps2/ev3-examples.md`
