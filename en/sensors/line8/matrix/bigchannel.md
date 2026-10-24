<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × matrix × bigchannel），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/matrix/bigchannel -->
# Pathfinder 8-Way (Line8) × MATRIX × Big-channel library

This page covers **LauMo big-channel library (MATRIXblock / Arduino)** only: how to set it up for the Pathfinder 8-Way (Line8) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-bigchannel.en.md + _build/guides/product/line8/matrix-bigchannel.en.md {"product":"Pathfinder 8-Way (Line8)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 8-Way (Line8): LauMo big-channel library (MATRIXblock / Arduino)](/en/sensors/line8/matrix.md).
<!-- /include-fallback -->

## Reading values

With the big-channel library installed, read byte n with the `bigU8()` / `bigU16()` helpers (expand "Helper code").

<details class="helper">
<summary>Helper code</summary>

```cpp
// big channel: read byte n (uint8) / 2 bytes from n (uint16 little-endian)
uint8_t bigU8(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();                 // pointer back to byte 0
  uint8_t v = MiniR4.I2C1.MXColor.getColor(B);      // read byte 0
  for (uint8_t i = 0; i < n; i++) { MiniR4.I2C1.MXColor.getColor(R); v = MiniR4.I2C1.MXColor.getColor(B); }   // Red = advance 1
  return v;
}
uint16_t bigU16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (uint16_t)MiniR4.I2C1.MXColor.getS();      // Saturation = read 2 bytes
}
int16_t bigI16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (int16_t)MiniR4.I2C1.MXColor.getH();       // Hue = read signed 2 bytes
}
```

</details>

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |

```cpp
int linePos = bigU8(0) - 8;   // line position
```

<p class="ref-note">Big channel byte 0（u8）</p>

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

```cpp
int linePosFine = bigU8(2) - 100;   // fine position
```

<p class="ref-note">Big channel byte 2（u8）</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` 路 | number of channels on the line, 0~8; 0 = no line |

```cpp
int lineWidth = bigU8(1);   // line width
```

<p class="ref-note">Big channel byte 1（u8）</p>

</section>

<section id="v04" class="ref-item">

### line segments <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="line segments" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~4` | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |

```cpp
int groupCount = bigU8(12);   // line segments
```

<p class="ref-note">Big channel byte 12（u8）</p>

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |

```cpp
int bitmap = bigU8(11);   // line bitmap
```

<p class="ref-note">Big channel byte 11（u8）</p>

</section>

<section id="v08" class="ref-item">

### calibrations done <small>calibCount</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` 次 | number of calibrations completed since power-on (button calibrations included) |

```cpp
int calibCount = bigU8(25);   // calibrations done
```

<p class="ref-note">Big channel byte 25（u8）; 尾段 F1/F2 完成次數</p>

</section>

<section id="v10" class="ref-item">

### channel 1~8 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="channel 1~8 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |

```cpp
int light[8];
for (uint8_t i = 0; i < 8; i++) light[i] = bigU8(3 + i);   // channel 1~8 reflect
```

<p class="ref-note">Big channel byte 3~10（u8）</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：last exit direction, threshold. Use <a href="/en/sensors/line8/arduino/ide.md">Arduino</a> / <a href="/en/sensors/line8/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/line8/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

<section id="ce2" class="ref-item ref-cmd">

### calibrate black <small>calibBlack</small>

store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM

```cpp
MiniR4.I2C1.MXColor.getColor(C);   // big channel F1（Cyan）= calibrate black, returns the completion counter from the previous packet
```

<p class="ref-note">Big-channel library only; the stock 0x22 library has no actions.</p>

</section>

<section id="ce3" class="ref-item ref-cmd">

### calibrate white <small>calibWhite</small>

store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM

```cpp
MiniR4.I2C1.MXColor.getColor(M);   // big channel F2（Magenta）= calibrate white, returns the completion counter from the previous packet
```

<p class="ref-note">Big-channel library only; the stock 0x22 library has no actions.</p>

</section>

<p class="ref-missing"><b>Not available here</b>：black line mode, white line mode, set threshold, 5-second auto calibration</p>

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line8.json` (single source of truth)
- Setup：`_build/guides/software/matrix-bigchannel.en.md`、`_build/guides/product/line8/matrix-bigchannel.en.md`
