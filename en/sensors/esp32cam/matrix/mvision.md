<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × matrix × mvision），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/matrix/mvision -->
# Sharpshooter Vision (ESP32CAM) × MATRIX × mVision blocks

This page covers **MATRIXblock mVision serial blocks** only: how to set it up for the Sharpshooter Vision (ESP32CAM) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **MATRIX edition** and connect it with the stock MATRIX 4-pin cable to I2C1 or I2C2 of the Mini R4 (address `0x22`); pick that port in the block.

## Setup

<!-- setup: _build/guides/software/matrix-mvision.en.md + _build/guides/product/esp32cam/matrix-mvision.en.md {"product":"Sharpshooter Vision (ESP32CAM)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Sharpshooter Vision (ESP32CAM): MATRIXblock mVision serial blocks](/en/sensors/esp32cam/matrix.md).
<!-- /include-fallback -->

## Reading values (School edition)

On MATRIX the camera talks serial; read it with the built-in mVision camera blocks, not I2C.

<section id="v01" class="ref-item">

### hue <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="hue" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-1~100` | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |

The MATRIXblock mVision block reads data slot 0: 0~100；255 = 黑白灰（−1 不能放 uint16，MATRIX 定案 §7）.

</section>

<section id="v02" class="ref-item">

### saturation <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="saturation" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | saturation of sample point S1, 0~100 |

The MATRIXblock mVision block reads data slot 1.

</section>

<section id="v03" class="ref-item">

### brightness <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="brightness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | brightness (HSV value) of sample point S1, 0~100 |

The MATRIXblock mVision block reads data slot 2.

</section>

<section id="v04" class="ref-item">

### motion <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="motion" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | whole-frame motion 0~100 (more change = larger) |

The MATRIXblock mVision block reads data slot 3.

</section>

<section id="v05" class="ref-item">

### blob X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="blob X" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob centre X 0~100, left = 0; 0 when not found |

The MATRIXblock mVision block reads data slot 4.

</section>

<section id="v06" class="ref-item">

### blob Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="blob Y" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |

The MATRIXblock mVision block reads data slot 5.

</section>

<section id="v07" class="ref-item">

### blob width <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="blob width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob width W 0~100; 0 = not found |

The MATRIXblock mVision block reads data slot 6.

</section>

<section id="v08" class="ref-item">

### blob height <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="blob height" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob height H 0~75; 0 when not found |

The MATRIXblock mVision block reads data slot 7.

</section>

<section id="v09" class="ref-item">

### blob area <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="blob area" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~7500` | W×H (0~7500); 0 when not found |

The MATRIXblock mVision block reads data slot 8: W×H.

</section>

<section id="v0a" class="ref-item">

### blob found <small>found</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = a blob of the target colour is in view (W > 0), 0 = none |

The MATRIXblock mVision block reads data slot 9.

</section>

## Reading values (WRO edition)

On MATRIX the camera talks serial; read it with the built-in mVision camera blocks, not I2C.

<section id="w10" class="ref-item">

### spot 1~12 colour code <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="spot 1~12 colour code" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 10~1B = colour code of spots S1~S12: 0~239 colour (hue 0~255 scaled to 0~239), 240~245 white (saturation < 30, lower = larger), 246~250 black (value < 30, darker = larger), 255 = spot not enabled |

The MATRIXblock mVision block reads data slot 0~5: 不動：byte 0~11 = S1~S12 顏色碼各 1 byte（idx 0 = S1 低 byte／S2 高 byte … idx 5 = S11／S12）；照 HUE12_AS_mVisionSensor_v1.0.8.mbr4 解碼.

</section>

<section id="w20" class="ref-item">

### spot 1~12 vividness <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="spot 1~12 vividness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 20~2B = vividness (saturation) of spots S1~S12, 0~255: black = 1, white/colour = actual saturation; 0 when the spot is not enabled |

The MATRIXblock mVision block reads data slot 6~8: 不動：byte 12~17 = 12 格各 4 bit（S>>4，1~15；黑 = 1；沒抓到 = 0），byte 12+2j = S(4j+1) | S(4j+2)<<4、byte 13+2j = S(4j+3) | S(4j+4)<<4.

</section>

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json` (single source of truth)
- Setup：`_build/guides/software/matrix-mvision.en.md`、`_build/guides/product/esp32cam/matrix-mvision.en.md`
