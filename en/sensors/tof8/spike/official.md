<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/spike/official -->
# Ranger 8-Way (TOF8) × SPIKE × Official LEGO App

This page covers **Official LEGO App (SPIKE 3, Robot Inventor)** only: how to set it up for the Ranger 8-Way (TOF8) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-official.en.md + _build/guides/product/tof8/spike-official.en.md {"product":"Ranger 8-Way (TOF8)","edition":"SPIKE edition","port":"C","lib":"MBC_laser8_obj_Lib.py","libUrl":"/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): Official LEGO App (SPIKE 3, Robot Inventor)](/en/sensors/tof8/spike-examples.md).
<!-- /include-fallback -->

## Reading values

The official app shows the sensor as a Color Sensor. Read it with the color, reflected light and raw red / green / blue blocks under More Sensors, then decode as shown.

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | group 1 | 「color」 | the number it returns |
| SPIKE Prime | group 2 | 「reflected light」 | the number it returns |
| SPIKE Prime | group 3 | 「raw red」 | the number mod 256 |
| SPIKE Prime | group 4 | 「raw red」 | floor(the number ÷ 256) |
| SPIKE Prime | group 5 | 「raw green」 | the number mod 256 |
| SPIKE Prime | group 6 | 「raw green」 | floor(the number ÷ 256) |
| SPIKE Prime | group 7 | 「raw blue」 | the number mod 256 |
| SPIKE Prime | group 8 | 「raw blue」 | floor(the number ÷ 256) |
| Robot Inventor | group 1 | 「reflected light」 | the number it returns |
| Robot Inventor | group 2 | 「color」 | the number it returns |

<p class="ref-note">Firmware note: 16 B = [第 1~8 顆, 0×8]：App 顏色 = 第 1 顆、反射光 = 第 2 顆、原始紅 = 第 3+256×第 4 …; byte0/1 = 第 1、2 顆 ×10/120（0~10）；byte2~7 = 第 3~8 顆兩顆一組 4-bit 再查表編碼（spikeEncodeU8，INO:259-284）</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：nearest distance (cm), nearest direction, farthest distance (cm), farthest direction, front distance (cm), nearest distance (mm), init bitmap, fault bitmap, front distance (mm), sensor 1~8 distance (mm). Use <a href="/en/sensors/tof8/spike/mbcblocks.md">MBC Blocks website</a> / <a href="/en/sensors/tof8/spike/pybricks.md">Pybricks</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof8/spike-examples.md host=spike sw=official -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/spike-official.en.md`、`_build/guides/product/tof8/spike-official.en.md`
- Examples：`sensors/tof8/spike-examples.md`
