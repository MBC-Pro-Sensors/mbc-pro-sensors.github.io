<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × ev3 × ev3g），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/ev3/ev3g -->
# Controller PS4/PS5 × EV3 × EV3-G

This page covers **EV3-G official software (EV3 Lab)** only: how to set it up for the Controller PS4/PS5 on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-ev3g.en.md + _build/guides/product/ps4/ev3-ev3g.en.md {"product":"Controller PS4/PS5","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller PS4/PS5: EV3-G official software (EV3 Lab)](/en/sensors/ps4/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

The EV3 sees a stock LEGO sensor, so read it with the stock sensor block.

<section id="v02" class="ref-item">

### D-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="D-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | up 1, right 2, down 4, left 8, added when pressed together (0~15) |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| D-pad | Infrared Sensor → Proximity | the value | mode 0 |

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | triangle 1, circle 2, cross 4, square 8, added when pressed together (0~15) |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| shape buttons | Infrared Sensor → Beacon heading, channel 2 | the value | mode 1 |

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8, added when pressed together (0~15) |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| shoulder buttons | Infrared Sensor → Beacon heading, channel 3 | the value | mode 1 |

</section>

<section id="v05" class="ref-item">

### function buttons <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1, L3 2, R3 4, START(OPTIONS) 8, added when pressed together (0~15) |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| function buttons | Infrared Sensor → Beacon heading, channel 4 | the value | mode 1 |

</section>

<section id="v10" class="ref-item">

### stick axis 1~4 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="stick axis 1~4" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right/up positive; 0 when not connected |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| stick axis 1~4 | Infrared Sensor → Beacon proximity, channel 1 | the value | mode 1 |

</section>

<p class="ref-missing"><b>Not readable here</b>：16 buttons, connected, battery, L2 analog, R2 analog, system buttons, gamepad type. Use <a href="/en/sensors/ps4/ev3/pybricks.md">Pybricks</a> / <a href="/en/sensors/ps4/ev3/clev3r.md">clev3r</a> for these.</p>

## Commands

The EV3 port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/ps4/ev3-examples.md host=ev3 sw=ev3g -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps4.json` (single source of truth)
- Setup：`_build/guides/software/ev3-ev3g.en.md`、`_build/guides/product/ps4/ev3-ev3g.en.md`
- Examples：`sensors/ps4/ev3-examples.md`
