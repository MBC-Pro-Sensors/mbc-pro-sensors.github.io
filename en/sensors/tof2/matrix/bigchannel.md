<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × matrix × bigchannel），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/matrix/bigchannel -->
# Ranger 2-Way (TOF2) × MATRIX × Big-channel library

This page covers **LauMo big-channel library (MATRIXblock / Arduino)** only: how to set it up for the Ranger 2-Way (TOF2) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-bigchannel.en.md + _build/guides/product/tof2/matrix-bigchannel.en.md {"product":"Ranger 2-Way (TOF2)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): LauMo big-channel library (MATRIXblock / Arduino)](/en/sensors/tof2/matrix.md).
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

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

```cpp
int leftCm = bigU8(0);   // left distance (cm)
```

<p class="ref-note">Big channel byte 0（u8）</p>

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

```cpp
int rightCm = bigU8(1);   // right distance (cm)
```

<p class="ref-note">Big channel byte 1（u8）</p>

</section>

<section id="v03" class="ref-item">

### nearer distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="nearer distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | the smaller of left and right, 0~120 cm |

```cpp
int nearCm = bigU8(2);   // nearer distance (cm)
```

<p class="ref-note">Big channel byte 2（u8）</p>

</section>

<section id="v04" class="ref-item">

### left distance (mm) <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="left distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | left sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

```cpp
int leftMm = bigU16(5);   // left distance (mm)
```

<p class="ref-note">Big channel byte 5~6（u16）; #67 追加（MATRIX 定案 §4：[5..6] 小端）</p>

</section>

<section id="v05" class="ref-item">

### right distance (mm) <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="right distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | right sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

```cpp
int rightMm = bigU16(7);   // right distance (mm)
```

<p class="ref-note">Big channel byte 7~8（u16）; #67 追加（[7..8] 小端）</p>

</section>

<section id="v06" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor initialised at power-on (no retry); 3 = both OK |

```cpp
int initOk = bigU8(3);   // init bitmap
```

<p class="ref-note">Big channel byte 3（u8）</p>

</section>

<section id="v07" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor is currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

```cpp
int fault = bigU8(4);   // fault bitmap
```

<p class="ref-note">Big channel byte 4（u8）</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：average distance (mm). Use <a href="/en/sensors/tof2/arduino/ide.md">Arduino</a> / <a href="/en/sensors/tof2/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/tof2/microbit/makecode.md">micro:bit</a> for these.</p>

## Examples

<p class="no-examples">There is no full example program for this software yet. Each item under “Reading values” above comes with code you can use as is; put them together in order, or <a href="/en/contact.html">contact us</a> for a full example.</p>

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/matrix-bigchannel.en.md`、`_build/guides/product/tof2/matrix-bigchannel.en.md`
