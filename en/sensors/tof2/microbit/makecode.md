<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/microbit/makecode -->
# Ranger 2-Way (TOF2) × micro:bit × MakeCode

This page covers **MakeCode (blocks / JavaScript)** only: how to set it up for the Ranger 2-Way (TOF2) on BBC micro:bit V1 / V2, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x29`. It draws little current, so VCC can go to the micro:bit **3V** pin (about 90 mA on V1, 190 mA on V2); on batteries the rail can sag below 3 V, so switch to external 5 V if readings are unstable (share GND with the micro:bit).

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-microbit.svg" alt="Ranger 2-Way (TOF2) × BBC micro:bit V1 / V2" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/microbit-makecode.en.md + _build/guides/product/tof2/microbit-makecode.en.md {"product":"Ranger 2-Way (TOF2)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): MakeCode (blocks / JavaScript)](/en/sensors/tof2/microbit.md).
<!-- /include-fallback -->

## Reading values

After adding pxt-mbc-sensors, find the blocks in the MBC Sensors category; the JavaScript form is shown too, address `0x29`.

<section id="v01" class="ref-item">

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

<pre class="blocks">(distance 2-ch [left v] \([cm v]\) :: #00A4E4)</pre>

```typescript
let leftCm = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)   // left distance (cm)：0~120 cm
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

<pre class="blocks">(distance 2-ch [right v] \([cm v]\) :: #00A4E4)</pre>

```typescript
let rightCm = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)   // right distance (cm)：0~120 cm
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v03" class="ref-item">

### nearer distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="nearer distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | the smaller of left and right, 0~120 cm |

<pre class="blocks">(distance 2-ch nearest \(cm\) :: #00A4E4)</pre>

```typescript
let nearCm = mbc.tof2NearestCm()   // nearer distance (cm)：0~120 cm
```

**Related block "on distance 2-ch nearer than 10 cm"**

```typescript
mbc.onTof2Near(10, function () {
})
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v04" class="ref-item">

### left distance (mm) <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="left distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | left sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

<pre class="blocks">(distance 2-ch [left v] \([mm v]\) :: #00A4E4)</pre>

```typescript
let leftMm = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Mm)   // left distance (mm)：0~1200 mm
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v05" class="ref-item">

### right distance (mm) <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="right distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | right sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

<pre class="blocks">(distance 2-ch [right v] \([mm v]\) :: #00A4E4)</pre>

```typescript
let rightMm = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Mm)   // right distance (mm)：0~1200 mm
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v06" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor initialised at power-on (no retry); 3 = both OK |

No dedicated block; in JavaScript read it with `mbc.readValue(address, id)` (−32768 when nothing answers). Related block: "distance 2-ch [left] sensor is OK".

```typescript
let initOk = mbc.readValue(0x29, 0x06)   // init bitmap：0~3 bit
```

</section>

<section id="v07" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor is currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

No dedicated block; in JavaScript read it with `mbc.readValue(address, id)` (−32768 when nothing answers). Related block: "distance 2-ch [left] sensor is OK".

```typescript
let fault = mbc.readValue(0x29, 0x07)   // fault bitmap：0~3 bit
```

</section>

<section id="v08" class="ref-item">

### average distance (mm) <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="average distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | (left mm + right mm) / 2, 0~1200; same number as EV3 mode 3 and the 3rd value of SPIKE mode 6 |

<pre class="blocks">(distance 2-ch average \(mm\) :: #00A4E4)</pre>

```typescript
let avgMm = mbc.tof2AverageMm()   // average distance (mm)：0~1200 mm
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

## Examples

<!-- examples: en/sensors/tof2/microbit-examples.md host=microbit sw=makecode whole -->

## More questions

- **Cannot find the MBC blocks?** In MakeCode Extensions, paste https://github.com/contactmbcrobot/pxt-mbc-sensors.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MakeCode extension pxt-mbc-sensors (paste this URL in MakeCode Extensions)](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/microbit-makecode.en.md`、`_build/guides/product/tof2/microbit-makecode.en.md`
- Examples：`sensors/tof2/microbit-examples.md`
