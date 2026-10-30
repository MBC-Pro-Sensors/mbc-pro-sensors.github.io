<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/spike/official -->
# Pathfinder 8-Way (Line8) × SPIKE × Official LEGO App

This page covers **Official LEGO App (SPIKE 3, Robot Inventor)** only: how to set it up for the Pathfinder 8-Way (Line8) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-official.en.md + _build/guides/product/line8/spike-official.en.md {"product":"Pathfinder 8-Way (Line8)","edition":"SPIKE edition","port":"C","lib":"MBC_line8_obj_Lib.py","libUrl":"/examples/line8/spike_beginner/pybricks/MBC_line8_obj_Lib.py","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 8-Way (Line8): Official LEGO App (SPIKE 3, Robot Inventor)](/en/sensors/line8/spike-education.md).
<!-- /include-fallback -->

## Reading values

The official app shows the sensor as a Color Sensor. Read it with the color, reflected light and raw red / green / blue blocks under More Sensors, then decode as shown.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | line position | 「reflected light」 | the number it returns, minus 8 |
| Robot Inventor | line position | 「reflected light」 | the number it returns, minus 8 |

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | fine position | 「raw red」 | the number it returns, minus 100 |

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` 路 | number of channels on the line, 0~8; 0 = no line |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | line width | 「color」 | the number it returns |
| Robot Inventor | line width | 「color」 | the number it returns |

</section>

<section id="v10" class="ref-item">

### channel 1~8 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="channel 1~8 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | channel 1~8 reflect | 「raw green」 | each number packs 4 channels of 4 bits |

<p class="ref-note">Firmware note: 4-bit (v×10)>>6</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：line segments, line bitmap, last exit direction, threshold, calibrations done. Use <a href="/en/sensors/line8/spike/mbcblocks.md">MBC Blocks website</a> / <a href="/en/sensors/line8/spike/pybricks.md">Pybricks</a> for these.</p>

## Commands

This software cannot send commands (neither the official SPIKE App 3 nor Pybricks' built-in ColorSensor blocks can). To send commands use [Pybricks + the MBC library](/en/sensors/line8/spike/pybricks.md); you can also calibrate with the button on the sensor.

## Examples

<!-- examples: en/sensors/line8/spike-examples.md host=spike sw=official -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [line8-demo.llsp3](/examples/line8/spike/line8-demo.llsp3)
- [line8-ri-demo.lms](/examples/line8/spike/line8-ri-demo.lms)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line8.json` (single source of truth)
- Setup：`_build/guides/software/spike-official.en.md`、`_build/guides/product/line8/spike-official.en.md`
- Examples：`sensors/line8/spike-examples.md`
