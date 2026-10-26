<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × ev3 × ev3g），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/ev3/ev3g -->
# Ranger 8-Way (TOF8) × EV3 × EV3-G

This page covers **EV3-G official software (EV3 Lab)** only: how to set it up for the Ranger 8-Way (TOF8) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-ev3g.en.md + _build/guides/product/tof8/ev3-ev3g.en.md {"product":"Ranger 8-Way (TOF8)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): EV3-G official software (EV3 Lab)](/en/sensors/tof8/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

The EV3 sees a stock LEGO sensor, so read it with the stock sensor block.

<section id="v01" class="ref-item">

### nearest distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="nearest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm (EV3 mode 2 gives a scaled value: cm×100/120 (0~100)) | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| nearest distance (cm) | Infrared Sensor → Remote, channel 2 | the value | mode 2 |

</section>

<section id="v02" class="ref-item">

### nearest direction <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="nearest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| nearest direction | Infrared Sensor → Remote, channel 1 | the value | mode 2 |

</section>

<section id="v03" class="ref-item">

### farthest distance (cm) <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="farthest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm (EV3 mode 2 gives a scaled value: cm×100/120 (0~100)) | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| farthest distance (cm) | Infrared Sensor → Remote, channel 4 | the value | mode 2 |

</section>

<section id="v04" class="ref-item">

### farthest direction <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="farthest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-7~+7` | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| farthest direction | Infrared Sensor → Remote, channel 3 | the value | mode 2 |

</section>

<section id="v05" class="ref-item">

### front distance (cm) <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="front distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm (EV3 mode 0 gives a scaled value: cm×100/120 (0~100)) | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3 and SPIKE mode 6 carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| front distance (cm) | Infrared Sensor → Proximity | the value | mode 0 |

</section>

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm (EV3 mode 1, mode 4 give a scaled value: cm×100/120 (0~100)) | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

| Value | Block | Decode | Mode |
| :--- | :--- | :--- | :--- |
| #1 | Infrared Sensor → Beacon heading, channel 1 | the value | mode 1 |
| #2 | Infrared Sensor → Beacon proximity, channel 1 | the value | mode 1 |
| #3 | Infrared Sensor → Beacon heading, channel 2 | the value | mode 1 |
| #4 | Infrared Sensor → Beacon proximity, channel 2 | the value | mode 1 |
| #5 | Infrared Sensor → Beacon heading, channel 3 | the value | mode 1 |
| #6 | Infrared Sensor → Beacon proximity, channel 3 | the value | mode 1 |
| #7 | Infrared Sensor → Beacon heading, channel 4 | the value | mode 1 |
| #8 | Infrared Sensor → Beacon proximity, channel 4 | the value | mode 1 |

</section>

<p class="ref-missing"><b>Not readable here</b>：nearest distance (mm), init bitmap, fault bitmap, front distance (mm), sensor 1~8 distance (mm). Use <a href="/en/sensors/tof8/ev3/pybricks.md">Pybricks</a> / <a href="/en/sensors/tof8/ev3/clev3r.md">clev3r</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof8/ev3-examples.md host=ev3 sw=ev3g -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/ev3-ev3g.en.md`、`_build/guides/product/tof8/ev3-ev3g.en.md`
- Examples：`sensors/tof8/ev3-examples.md`
