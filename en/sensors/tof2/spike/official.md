<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/spike/official -->
# Ranger 2-Way (TOF2) × SPIKE × Official LEGO App

This page covers **Official LEGO App (SPIKE 3, Robot Inventor)** only: how to set it up for the Ranger 2-Way (TOF2) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-official.en.md + _build/guides/product/tof2/spike-official.en.md {"product":"Ranger 2-Way (TOF2)","edition":"SPIKE edition","port":"C","lib":"MBC_laser2_obj_Lib.py","libUrl":"/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): Official LEGO App (SPIKE 3, Robot Inventor)](/en/sensors/tof2/spike-examples.md).
<!-- /include-fallback -->

## Reading values

The official app shows the sensor as a Color Sensor. Read it with the color, reflected light and raw red / green / blue blocks under More Sensors, then decode as shown.

<section id="v01" class="ref-item">

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | left distance (cm) | 「color」 | the number it returns |
| Robot Inventor | left distance (cm) | 「reflected light」 | the number it returns |

<p class="ref-note">Firmware note: 16 B = [左, 右, 0, 左, 0, 右, 0, 平均, 0×8]：App 顏色 = 左、原始紅 = 256×左; 8 B 原樣（不壓縮）</p>

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | right distance (cm) | 「reflected light」 | the number it returns |
| Robot Inventor | right distance (cm) | 「color」 | the number it returns |

<p class="ref-note">Firmware note: App 反射光 = 右、原始綠 = 256×右</p>

</section>

<section id="v08" class="ref-item">

### average distance (mm) <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="average distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | (left mm + right mm) / 2, 0~1200; same number as EV3 mode 3 and the 3rd value of SPIKE mode 6 |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | average distance (mm) | 「raw blue」 | floor(the number ÷ 256) |

<p class="ref-note">Firmware note: byte7 = (左 cm + 右 cm)>>1；App 原始藍 = 256×平均</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：nearer distance (cm), left distance (mm), right distance (mm), init bitmap, fault bitmap. Use <a href="/en/sensors/tof2/spike/mbcblocks.md">MBC Blocks website</a> / <a href="/en/sensors/tof2/spike/pybricks.md">Pybricks</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof2/spike-examples.md host=spike sw=official -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/spike-official.en.md`、`_build/guides/product/tof2/spike-official.en.md`
- Examples：`sensors/tof2/spike-examples.md`
