<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/microbit/makecode -->
# Pathfinder 8-Way (Line8) × micro:bit × MakeCode

This page covers **MakeCode (blocks / JavaScript)** only: how to set it up for the Pathfinder 8-Way (Line8) on BBC micro:bit V1 / V2, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x16`. It draws little current, so VCC can go to the micro:bit **3V** pin (about 90 mA on V1, 190 mA on V2); on batteries the rail can sag below 3 V, so switch to external 5 V if readings are unstable (share GND with the micro:bit).

<figure class="ref-fig"><img src="/images/figures/line8/wiring-microbit.svg" alt="Pathfinder 8-Way (Line8) × BBC micro:bit V1 / V2" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/microbit-makecode.en.md + _build/guides/product/line8/microbit-makecode.en.md {"product":"Pathfinder 8-Way (Line8)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 8-Way (Line8): MakeCode (blocks / JavaScript)](/en/sensors/line8/microbit.md).
<!-- /include-fallback -->

## Reading values

After adding pxt-mbc-sensors, find the blocks in the MBC Sensors category; the JavaScript form is shown too, address `0x16`.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |

<pre class="blocks">(line position :: #00A4E4)</pre>

```typescript
let linePos = mbc.linePosition()   // line position：-8~+8
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

<pre class="blocks">(fine line position :: #00A4E4)</pre>

```typescript
let linePosFine = mbc.linePositionFine()   // fine position：-100~+100
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` 路 | number of channels on the line, 0~8; 0 = no line |

<pre class="blocks">(line width :: #00A4E4)</pre>

```typescript
let lineWidth = mbc.lineWidth()   // line width：0~8 路
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v04" class="ref-item">

### line segments <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="line segments" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~4` | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |

<pre class="blocks">(line segments :: #00A4E4)</pre>

```typescript
let groupCount = mbc.lineSegments()   // line segments：0~4
```

**Related block "line is seen"**

```typescript
let lineSeen = mbc.lineSeen()
```

**Related block "on line [lost]"**

```typescript
mbc.onLineEvent(MbcLineEvent.Lost, function () {
})
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |

<pre class="blocks">(line bitmap :: #00A4E4)</pre>

```typescript
let bitmap = mbc.lineBitmap()   // line bitmap：0~255 bit
```

**Related block "channel 1 is on the line"**

```typescript
let lineChannelOnLine = mbc.lineChannelOnLine(1)
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v06" class="ref-item">

### last exit direction <small>lastDir</small>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | where the line was before it was lost: −8 to +8 (negative = left); keeps the last value |

<pre class="blocks">(last exit direction :: #00A4E4)</pre>

```typescript
let lastDir = mbc.lineLastDirection()   // last exit direction：-8~+8
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v07" class="ref-item">

### threshold <small>threshold</small>

| Range | Meaning |
| :--- | :--- |
| `1~99` | reflect threshold for 'on line', 1~99, default 50; set with E4 |

<pre class="blocks">(line threshold :: #00A4E4)</pre>

```typescript
let threshold = mbc.lineThreshold()   // threshold：1~99
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v08" class="ref-item">

### calibrations done <small>calibCount</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` 次 | number of calibrations completed since power-on (button calibrations included) |

<pre class="blocks">(line calibration count :: #00A4E4)</pre>

```typescript
let calibCount = mbc.lineCalibrationCount()   // calibrations done：0~255 次
```

**Related block "line follower is calibrating"**

```typescript
let lineCalibrating = mbc.lineCalibrating()
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v10" class="ref-item">

### channel 1~8 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="channel 1~8 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |

<pre class="blocks">(channel (1) reflect :: #00A4E4)</pre>

```typescript
let light = mbc.lineChannel(1)   // channel 1~8 reflect：0~100 (N = 1~8)
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

## Commands

<section id="ce0" class="ref-item ref-cmd">

### black line mode <small>blackLine</small>

follow a black line (default)

<pre class="blocks">line follower follow [black v] line :: #00A4E4</pre>

```typescript
mbc.lineSetColor(MbcLineColor.Black)   // black line mode
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### white line mode <small>whiteLine</small>

follow a white line (F3 b1 = 1)

<pre class="blocks">line follower follow [white v] line :: #00A4E4</pre>

```typescript
mbc.lineSetColor(MbcLineColor.White)   // white line mode
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### calibrate black <small>calibBlack</small>

store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM

<pre class="blocks">line follower calibrate [current reading is black v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.Black)   // calibrate black
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### calibrate white <small>calibWhite</small>

store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM

<pre class="blocks">line follower calibrate [current reading is white v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.White)   // calibrate white
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### set threshold <small>setThreshold</small>

threshold 1~99 (RAM only, back to 50 after reboot)

<pre class="blocks">line follower set threshold (50) :: #00A4E4</pre>

```typescript
mbc.lineSetThreshold(50)   // set threshold
```

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5-second auto calibration <small>calibAuto</small>

sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile

<pre class="blocks">line follower calibrate [auto \(sweep for 5 s\) v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.Auto)   // 5-second auto calibration
```

</section>

## Examples

<!-- examples: en/sensors/line8/microbit-examples.md host=microbit sw=makecode whole -->

## More questions

- **Cannot find the MBC blocks?** In MakeCode Extensions, paste https://github.com/contactmbcrobot/pxt-mbc-sensors.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MakeCode extension pxt-mbc-sensors (paste this URL in MakeCode Extensions)](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line8.json` (single source of truth)
- Setup：`_build/guides/software/microbit-makecode.en.md`、`_build/guides/product/line8/microbit-makecode.en.md`
- Examples：`sensors/line8/microbit-examples.md`
