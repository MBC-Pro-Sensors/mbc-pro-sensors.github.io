<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/microbit/makecode -->
# Sharpshooter Vision (ESP32CAM) × micro:bit × MakeCode

This page covers **MakeCode (blocks / JavaScript)** only: how to set it up for the Sharpshooter Vision (ESP32CAM) on BBC micro:bit V1 / V2, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x4C`. The sensor needs 5 V; use a breakout board with a 5 V output.

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-microbit.svg" alt="Sharpshooter Vision (ESP32CAM) × BBC micro:bit V1 / V2" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/microbit-makecode.en.md + _build/guides/product/esp32cam/microbit-makecode.en.md {"product":"Sharpshooter Vision (ESP32CAM)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Sharpshooter Vision (ESP32CAM): MakeCode (blocks / JavaScript)](/en/sensors/esp32cam/microbit.md).
<!-- /include-fallback -->

## Reading values (School edition)

After adding pxt-mbc-sensors, find the blocks in the MBC category; the JavaScript form is shown too, address `0x4C`.

<section id="v01" class="ref-item">

### hue <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="hue" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-1~100` | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |

<pre class="blocks">(MBC hue :: #E8711E)</pre>

```typescript
let hue = MBC.readValue(0x4C, 0x01)   // hue：-1~100
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v02" class="ref-item">

### saturation <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="saturation" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | saturation of sample point S1, 0~100 |

<pre class="blocks">(MBC saturation :: #E8711E)</pre>

```typescript
let saturation = MBC.readValue(0x4C, 0x02)   // saturation：0~100
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v03" class="ref-item">

### brightness <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="brightness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | brightness (HSV value) of sample point S1, 0~100 |

<pre class="blocks">(MBC brightness :: #E8711E)</pre>

```typescript
let value = MBC.readValue(0x4C, 0x03)   // brightness：0~100
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v04" class="ref-item">

### motion <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="motion" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | whole-frame motion 0~100 (more change = larger) |

<pre class="blocks">(MBC motion :: #E8711E)</pre>

```typescript
let motion = MBC.readValue(0x4C, 0x04)   // motion：0~100
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v05" class="ref-item">

### blob X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="blob X" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob centre X 0~100, left = 0; 0 when not found |

<pre class="blocks">(MBC blob X :: #E8711E)</pre>

```typescript
let blobX = MBC.readValue(0x4C, 0x05)   // blob X：0~100
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v06" class="ref-item">

### blob Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="blob Y" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |

<pre class="blocks">(MBC blob Y :: #E8711E)</pre>

```typescript
let blobY = MBC.readValue(0x4C, 0x06)   // blob Y：0~75
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v07" class="ref-item">

### blob width <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="blob width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob width W 0~100; 0 = not found |

<pre class="blocks">(MBC blob width :: #E8711E)</pre>

```typescript
let blobW = MBC.readValue(0x4C, 0x07)   // blob width：0~100
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v08" class="ref-item">

### blob height <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="blob height" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob height H 0~75; 0 when not found |

<pre class="blocks">(MBC blob height :: #E8711E)</pre>

```typescript
let blobH = MBC.readValue(0x4C, 0x08)   // blob height：0~75
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v09" class="ref-item">

### blob area <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="blob area" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~7500` | W×H (0~7500); 0 when not found |

<pre class="blocks">(MBC blob area :: #E8711E)</pre>

```typescript
let blobArea = MBC.readValue(0x4C, 0x09)   // blob area：0~7500
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v0a" class="ref-item">

### blob found <small>found</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = a blob of the target colour is in view (W > 0), 0 = none |

<pre class="blocks">(MBC blob found? :: #E8711E)</pre>

```typescript
let found = MBC.readValue(0x4C, 0x0A)   // blob found：0~1
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

## Reading values (WRO edition)

After adding pxt-mbc-sensors, find the blocks in the MBC category; the JavaScript form is shown too, address `0x4C`.

<section id="w10" class="ref-item">

### spot 1~12 colour code <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="spot 1~12 colour code" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 10~1B = colour code of spots S1~S12: 0~239 colour (hue 0~255 scaled to 0~239), 240~245 white (saturation < 30, lower = larger), 246~250 black (value < 30, darker = larger), 255 = spot not enabled |

<pre class="blocks">(MBC spot [1 v] colour code :: #E8711E)</pre>

```typescript
let spotColor = MBC.readValues(0x4C, 0x10, 12)   // spot 1~12 colour code
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="w20" class="ref-item">

### spot 1~12 vividness <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="spot 1~12 vividness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 20~2B = vividness (saturation) of spots S1~S12, 0~255: black = 1, white/colour = actual saturation; 0 when the spot is not enabled |

<pre class="blocks">(MBC spot [1 v] vividness :: #E8711E)</pre>

```typescript
let spotVivid = MBC.readValues(0x4C, 0x20, 12)   // spot 1~12 vividness
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

## Examples (School edition)

<!-- examples: en/sensors/esp32cam/microbit-examples.md host=microbit sw=makecode whole -->

## More questions

- **Cannot find the MBC blocks?** In MakeCode Extensions, paste https://github.com/contactmbcrobot/pxt-mbc-sensors.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MakeCode extension pxt-mbc-sensors (paste this URL in MakeCode Extensions)](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json` (single source of truth)
- Setup：`_build/guides/software/microbit-makecode.en.md`、`_build/guides/product/esp32cam/microbit-makecode.en.md`
- Examples：`sensors/esp32cam/microbit-examples(-wro).md`
