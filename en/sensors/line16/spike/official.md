<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/spike/official -->
# Pathfinder 16-Way (Line16) × SPIKE × Official LEGO App

This page covers **Official LEGO App (SPIKE 3, Robot Inventor)** only: how to set it up for the Pathfinder 16-Way (Line16) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-official.en.md + _build/guides/product/line16/spike-official.en.md {"product":"Pathfinder 16-Way (Line16)","edition":"SPIKE edition","port":"C","lib":"MBC_line16_obj_Lib.py","libUrl":"/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 16-Way (Line16): Official LEGO App (SPIKE 3, Robot Inventor)](/en/sensors/line16/spike-education.md).
<!-- /include-fallback -->

## Reading values

The official app shows the sensor as a Color Sensor. Read it with the color, reflected light and raw red / green / blue blocks under More Sensors, then decode as shown.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-16~+16` | which channel the line is under: −16 (far left) to +16 (far right), 0 = centre; seen from behind the robot, negative = left (normally ±15; ±16 only for 200 ms after the line leaves the outermost channel) |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | line position | 「reflected light」 | the number it returns, minus 16 |
| Robot Inventor | line position | 「reflected light」 | the number it returns, minus 16 |

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | fine position | 「raw red」 | the number it returns, minus 100 |
| Robot Inventor | fine position | 「raw red」 | the number mod 256, minus 100 (Robot Inventor look-up encoding, see setup above) |

<p class="ref-note">Firmware note: byte2 = 細位置+100（0~200）、byte3 = 0；App 原始紅讀成 int16; 細位置+100 經玩具版查表編碼成 byte2~3</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~16` 路 | number of channels on the line, 0~16; 0 = no line |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | line width | 「color」 | the number it returns |
| Robot Inventor | line width | 「color」 | the number it returns |

<p class="ref-note">Firmware note: min(線寬, 10)</p>

</section>

<section id="v10" class="ref-item">

### channel 1~16 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="channel 1~16 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~16, 0~100 (channel 1 = left); ids 10~1F |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | channel 1~16 reflect | 「raw green」 | the number it returns |
| Robot Inventor | channel 1~16 reflect | 「raw green」 | the number mod 256 (Robot Inventor look-up encoding, see setup above) |

<p class="ref-note">Firmware note: 每路 2 bit（(v×10)>>8：0~25→0、26~51→1、52~76→2、77~100→3）；byte4 = 第 5~8 路（第 5 路在 bit7-6）、byte5 = 第 1~4 路、byte6 = 第 13~16 路、byte7 = 第 9~12 路 → App 原始綠 = 第 1~8 路（bit15-14 = 第 1 路）、原始藍 = 第 9~16 路；byte 8~15 = 0; 每路 1 bit（光值 > 50 = 1 亮）：第 1~8 路合成 1 byte（第 1 路在 bit7）經玩具版查表編碼成 byte4~5；第 9~16 路 → byte6~7</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：line segments, line bitmap, last exit direction, threshold, calibrations done. Use <a href="/en/sensors/line16/spike/mbcblocks.md">MBC Blocks website</a> / <a href="/en/sensors/line16/spike/pybricks.md">Pybricks</a> for these.</p>

## Commands

The SPIKE port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/line16/spike-examples.md host=spike sw=official -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [line16-demo.llsp3](/examples/line16/spike/line16-demo.llsp3)
- [line16-demo.lms](/examples/line16/spike/line16-demo.lms)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line16.json` (single source of truth)
- Setup：`_build/guides/software/spike-official.en.md`、`_build/guides/product/line16/spike-official.en.md`
- Examples：`sensors/line16/spike-examples.md`
