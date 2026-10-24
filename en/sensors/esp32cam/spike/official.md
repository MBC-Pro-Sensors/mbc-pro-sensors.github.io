<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/spike/official -->
# Sharpshooter Vision (ESP32CAM) × SPIKE × Official LEGO App

This page covers **Official LEGO App (SPIKE 3, Robot Inventor)** only: how to set it up for the Sharpshooter Vision (ESP32CAM) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-official.en.md + _build/guides/product/esp32cam/spike-official.en.md {"product":"Sharpshooter Vision (ESP32CAM)","edition":"SPIKE edition","port":"C","lib":"MBC_cam_school_Lib.py","libUrl":"/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Sharpshooter Vision (ESP32CAM): Official LEGO App (SPIKE 3, Robot Inventor)](/en/sensors/esp32cam/spike-examples.md).
<!-- /include-fallback -->

## Reading values (School edition)

The official app shows the sensor as a Color Sensor. Read it with the color, reflected light and raw red / green / blue blocks under More Sensors, then decode as shown.

<section id="v01" class="ref-item">

### hue <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="hue" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-1~100` | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | hue | 「color」 | the number it returns |
| Robot Inventor | hue | 「color」 | the number it returns |

<p class="ref-note">Firmware note: −1 = 0xFF; 0~10（÷10）</p>

</section>

<section id="v04" class="ref-item">

### motion <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="motion" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | whole-frame motion 0~100 (more change = larger) |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | motion | 「reflected light」 | the number it returns |
| Robot Inventor | motion | 「reflected light」 | the number it returns |

</section>

<section id="v05" class="ref-item">

### blob X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="blob X" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob centre X 0~100, left = 0; 0 when not found |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | blob X | 「raw red」 | the number mod 256 |
| Robot Inventor | blob X | 「raw red」 | the number it returns |

<p class="ref-note">Firmware note: byte 3 = 0; ×4 查表編碼（10-bit）</p>

</section>

<section id="v06" class="ref-item">

### blob Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="blob Y" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | blob Y | 「raw green」 | the number mod 256 |
| Robot Inventor | blob Y | 「raw green」 | the number it returns |

<p class="ref-note">Firmware note: byte 5 = 0; ×4 查表編碼</p>

</section>

<section id="v09" class="ref-item">

### blob area <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="blob area" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~7500` | W×H (0~7500); 0 when not found |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | blob area | 「raw blue」 | the number mod 256 |
| Robot Inventor | blob area | 「raw blue」 | the number it returns |

<p class="ref-note">Firmware note: 鎖住：這裡是 map(像素面積, 0~5000, 0~100)，不是 W×H；byte 7 = 0; 同上 0~100 再 ×4 查表編碼</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：saturation, brightness, blob width, blob height, blob found. Use <a href="/en/sensors/esp32cam/spike/mbcblocks.md">MBC Blocks website</a> / <a href="/en/sensors/esp32cam/spike/pybricks.md">Pybricks</a> for these.</p>

## Reading values (WRO edition)

The official app shows the sensor as a Color Sensor. Read it with the color, reflected light and raw red / green / blue blocks under More Sensors, then decode as shown.

<p class="ref-missing"><b>Not readable here</b>：spot 1~12 colour code, spot 1~12 vividness. Use <a href="/en/sensors/esp32cam/spike/pybricks.md">Pybricks</a> for these.</p>

## Examples (School edition)

<!-- examples: en/sensors/esp32cam/spike-examples.md host=spike sw=official -->

## Examples (WRO edition)

<!-- examples: en/sensors/esp32cam/spike-examples-wro.md host=spike sw=official -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json` (single source of truth)
- Setup：`_build/guides/software/spike-official.en.md`、`_build/guides/product/esp32cam/spike-official.en.md`
- Examples：`sensors/esp32cam/spike-examples(-wro).md`
