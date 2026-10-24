<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × ev3 × clev3r），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/ev3/clev3r -->
# Pathfinder 8-Way (Line8) × EV3 × clev3r

This page covers **clev3r (EV3 BASIC)** only: how to set it up for the Pathfinder 8-Way (Line8) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-clev3r.en.md + _build/guides/product/line8/ev3-clev3r.en.md {"product":"Pathfinder 8-Way (Line8)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 8-Way (Line8): clev3r (EV3 BASIC)](/en/sensors/line8/ev3-clev3r.md).
<!-- /include-fallback -->

## Reading values

Code assumes port 1: `Sensor.SetMode` picks the mode, `Sensor.ReadRawValue` reads the n-th value.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
linePos = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">mode 0 · int8 · IR-PROX (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
linePosFine = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">mode 2 · int8 · IR-REMOTE byte0 = highRes−100 存成 int8（F030 ev3Uart case 2），主機直接讀 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` 路 | number of channels on the line, 0~8; 0 = no line |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
lineWidth = Sensor.ReadRawValue(1, 1)
```

<p class="ref-note">mode 2 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
bitmap = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">mode 3 · int16 · #67 新：IR-REM-A 1×int16</p>

</section>

<section id="v10" class="ref-item">

### channel 1~8 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="channel 1~8 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
light1 = Sensor.ReadRawValue(1, 0)   ' first; for the N-th use 0 + N - 1
```

<p class="ref-note">mode 1 · int8 · IR-SEEK 8×int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：line segments, last exit direction, threshold, calibrations done. Use <a href="/en/sensors/line8/arduino/ide.md">Arduino</a> / <a href="/en/sensors/line8/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/line8/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

The EV3 port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/line8/ev3-examples.md host=ev3 sw=clev3r -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line8.json` (single source of truth)
- Setup：`_build/guides/software/ev3-clev3r.en.md`、`_build/guides/product/line8/ev3-clev3r.en.md`
- Examples：`sensors/line8/ev3-examples.md`
