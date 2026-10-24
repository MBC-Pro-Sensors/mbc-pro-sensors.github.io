<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/microbit/makecode -->
# Ranger 8-Way (TOF8) × micro:bit × MakeCode

This page covers **MakeCode (blocks / JavaScript)** only: how to set it up for the Ranger 8-Way (TOF8) on BBC micro:bit V1 / V2, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x29`. The sensor needs 5 V; use a breakout board with a 5 V output.

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-microbit.svg" alt="Ranger 8-Way (TOF8) × BBC micro:bit V1 / V2" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/microbit-makecode.en.md + _build/guides/product/tof8/microbit-makecode.en.md {"product":"Ranger 8-Way (TOF8)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): MakeCode (blocks / JavaScript)](/en/sensors/tof8/microbit.md).
<!-- /include-fallback -->

## Reading values

After adding pxt-mbc-sensors, find the blocks in the MBC category; the JavaScript form is shown too, address `0x29`.

<section id="v01" class="ref-item">

### nearest distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="nearest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |

<pre class="blocks">(MBC nearest distance cm :: #E8711E)</pre>

```typescript
let nearCm = MBC.readValue(0x29, 0x01)   // nearest distance (cm)：0~120 cm
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v02" class="ref-item">

### nearest direction <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="nearest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |

<pre class="blocks">(MBC nearest direction :: #E8711E)</pre>

```typescript
let nearDir = MBC.readValue(0x29, 0x02)   // nearest direction：-8~+8
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v03" class="ref-item">

### farthest distance (cm) <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="farthest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |

<pre class="blocks">(MBC farthest distance cm :: #E8711E)</pre>

```typescript
let farCm = MBC.readValue(0x29, 0x03)   // farthest distance (cm)：0~120 cm
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v04" class="ref-item">

### farthest direction <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="farthest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-7~+7` | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |

<pre class="blocks">(MBC farthest direction :: #E8711E)</pre>

```typescript
let farDir = MBC.readValue(0x29, 0x04)   // farthest direction：-7~+7
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v05" class="ref-item">

### front distance (cm) <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="front distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3, SPIKE mode 6 and big-channel [16..17] carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |

<pre class="blocks">(MBC front distance cm :: #E8711E)</pre>

```typescript
let frontCm = MBC.readValue(0x29, 0x05)   // front distance (cm)：0~120 cm
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v06" class="ref-item">

### nearest distance (mm) <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="nearest distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |

<pre class="blocks">(MBC nearest distance mm :: #E8711E)</pre>

```typescript
let nearMm = MBC.readValue(0x29, 0x06)   // nearest distance (mm)：0~1200 mm
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v07" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |

<pre class="blocks">(MBC init bitmap :: #E8711E)</pre>

```typescript
let initOk = MBC.readValue(0x29, 0x07)   // init bitmap：0~255 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v08" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

<pre class="blocks">(MBC fault bitmap :: #E8711E)</pre>

```typescript
let fault = MBC.readValue(0x29, 0x08)   // fault bitmap：0~255 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v09" class="ref-item">

### front distance (mm) <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="front distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |

<pre class="blocks">(MBC front distance mm :: #E8711E)</pre>

```typescript
let frontMm = MBC.readValue(0x29, 0x09)   // front distance (mm)：0~1200 mm
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

<pre class="blocks">(MBC sensor [1 v] distance cm :: #E8711E)</pre>

```typescript
let distCm = MBC.readValues(0x29, 0x10, 8)   // sensor 1~8 distance (cm)
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v18" class="ref-item">

### sensor 1~8 distance (mm) <small>distMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distMm.svg" alt="sensor 1~8 distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | distance of each sensor 1~8, 0~1200 mm; 1200 = nothing that way / sensor failed; ids 18~1F |

<pre class="blocks">(MBC sensor [1 v] distance mm :: #E8711E)</pre>

```typescript
let distMm = MBC.readValues(0x29, 0x18, 8)   // sensor 1~8 distance (mm)
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

## Examples

<!-- examples: en/sensors/tof8/microbit-examples.md host=microbit sw=makecode whole -->

## More questions

- **Cannot find the MBC blocks?** In MakeCode Extensions, paste https://github.com/contactmbcrobot/pxt-mbc-sensors.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MakeCode extension pxt-mbc-sensors (paste this URL in MakeCode Extensions)](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/microbit-makecode.en.md`、`_build/guides/product/tof8/microbit-makecode.en.md`
- Examples：`sensors/tof8/microbit-examples.md`
