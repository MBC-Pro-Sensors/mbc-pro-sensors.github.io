<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/microbit/makecode -->
# Pathfinder 16-Way (Line16) × micro:bit × MakeCode

This page covers **MakeCode (blocks / JavaScript)** only: how to set it up for the Pathfinder 16-Way (Line16) on BBC micro:bit V1 / V2, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x16`. The sensor needs 5 V; use a breakout board with a 5 V output.

<figure class="ref-fig"><img src="/images/figures/line16/wiring-microbit.svg" alt="Pathfinder 16-Way (Line16) × BBC micro:bit V1 / V2" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/microbit-makecode.en.md + _build/guides/product/line16/microbit-makecode.en.md {"product":"Pathfinder 16-Way (Line16)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 16-Way (Line16): MakeCode (blocks / JavaScript)](/en/sensors/line16/microbit.md).
<!-- /include-fallback -->

## Reading values

After adding pxt-mbc-sensors, find the blocks in the MBC category; the JavaScript form is shown too, address `0x16`.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-16~+16` | which channel the line is under: −16 (far left) to +16 (far right), 0 = centre; seen from behind the robot, negative = left (normally ±15; ±16 only for 200 ms after the line leaves the outermost channel) |

<pre class="blocks">(MBC line position :: #E8711E)</pre>

```typescript
let linePos = MBC.readValue(0x16, 0x01)   // line position：-16~+16
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

<pre class="blocks">(MBC fine line position :: #E8711E)</pre>

```typescript
let linePosFine = MBC.readValue(0x16, 0x02)   // fine position：-100~+100
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~16` 路 | number of channels on the line, 0~16; 0 = no line |

<pre class="blocks">(MBC line width :: #E8711E)</pre>

```typescript
let lineWidth = MBC.readValue(0x16, 0x03)   // line width：0~16 路
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v04" class="ref-item">

### line segments <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-groupCount.svg" alt="line segments" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |

<pre class="blocks">(MBC line segments :: #E8711E)</pre>

```typescript
let groupCount = MBC.readValue(0x16, 0x04)   // line segments：0~8
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | bit0 = channel 1 (left) … bit15 = channel 16; 1 = that channel sees the line |

<pre class="blocks">(MBC line bitmap :: #E8711E)</pre>

```typescript
let bitmap = MBC.readValue(0x16, 0x05)   // line bitmap：0~65535 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v06" class="ref-item">

### last exit direction <small>lastDir</small>

| Range | Meaning |
| :--- | :--- |
| `-16~+16` | where the line was before it was lost: −16 to +16 (negative = left); keeps the last value |

<pre class="blocks">(MBC last exit direction :: #E8711E)</pre>

```typescript
let lastDir = MBC.readValue(0x16, 0x06)   // last exit direction：-16~+16
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v07" class="ref-item">

### threshold <small>threshold</small>

| Range | Meaning |
| :--- | :--- |
| `1~99` | reflect threshold for 'on line', 1~99, default 50; set with E4 |

<pre class="blocks">(MBC threshold :: #E8711E)</pre>

```typescript
let threshold = MBC.readValue(0x16, 0x07)   // threshold：1~99
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v08" class="ref-item">

### calibrations done <small>calibCount</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` 次 | number of calibrations completed since power-on (button calibrations included) |

No dedicated block (use the generic `MBC.readValue(address, id)` block).

</section>

<section id="v10" class="ref-item">

### channel 1~16 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="channel 1~16 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~16, 0~100 (channel 1 = left); ids 10~1F |

<pre class="blocks">(MBC channel [1 v] reflect :: #E8711E)</pre>

```typescript
let light = MBC.readValues(0x16, 0x10, 16)   // channel 1~16 reflect
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

## Commands

<section id="ce0" class="ref-item ref-cmd">

### black line mode <small>blackLine</small>

follow a black line (default)

<pre class="blocks">MBC set black line :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE0)   // black line mode
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### white line mode <small>whiteLine</small>

follow a white line (F3 b1 = 1)

<pre class="blocks">MBC set white line :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE1)   // white line mode
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### calibrate black <small>calibBlack</small>

store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM

<pre class="blocks">MBC calibrate black :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE2)   // calibrate black
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### calibrate white <small>calibWhite</small>

store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM

<pre class="blocks">MBC calibrate white :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE3)   // calibrate white
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### set threshold <small>setThreshold</small>

threshold 1~99 (RAM only, back to 50 after reboot)

<pre class="blocks">MBC set threshold (50) :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE4, [50])   // set threshold
```

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5-second auto calibration <small>calibAuto</small>

sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile

<pre class="blocks">MBC auto calibrate (5 s) :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE7)   // 5-second auto calibration
```

</section>

## Examples

<!-- examples: en/sensors/line16/microbit-examples.md host=microbit sw=makecode whole -->

## More questions

- **Cannot find the MBC blocks?** In MakeCode Extensions, paste https://github.com/contactmbcrobot/pxt-mbc-sensors.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MakeCode extension pxt-mbc-sensors (paste this URL in MakeCode Extensions)](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line16.json` (single source of truth)
- Setup：`_build/guides/software/microbit-makecode.en.md`、`_build/guides/product/line16/microbit-makecode.en.md`
- Examples：`sensors/line16/microbit-examples.md`
