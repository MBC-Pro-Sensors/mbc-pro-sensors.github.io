<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × ev3 × classroom），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/ev3/classroom -->
# Ranger 2-Way (TOF2) × EV3 × EV3 Classroom

This page covers **EV3 Classroom (Scratch blocks)** only: how to set it up for the Ranger 2-Way (TOF2) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-classroom.en.md + _build/guides/product/tof2/ev3-classroom.en.md {"product":"Ranger 2-Way (TOF2)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): EV3 Classroom (Scratch blocks)](/en/sensors/tof2/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

The EV3 sees a stock LEGO sensor; read it with the stock Classroom sensor blocks.

<section id="v01" class="ref-item">

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| left distance (cm) | Infrared Sensor → Beacon heading, channel 1 | the value | mode 1 |

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| right distance (cm) | Infrared Sensor → Beacon proximity, channel 1 | the value | mode 1 |

</section>

<section id="v08" class="ref-item">

### average distance (mm) <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="average distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | (left mm + right mm) / 2, 0~1200; same number as EV3 mode 3 and the 3rd value of SPIKE mode 6 |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| average distance (mm) | Infrared Sensor → Proximity | the value | mode 0 |

</section>

<p class="ref-missing"><b>Not readable here</b>：nearer distance (cm), left distance (mm), right distance (mm), init bitmap, fault bitmap. Use <a href="/en/sensors/tof2/ev3/pybricks.md">Pybricks</a> / <a href="/en/sensors/tof2/ev3/clev3r.md">clev3r</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof2/ev3-examples.md host=ev3 sw=classroom -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/ev3-classroom.en.md`、`_build/guides/product/tof2/ev3-classroom.en.md`
- Examples：`sensors/tof2/ev3-examples.md`
