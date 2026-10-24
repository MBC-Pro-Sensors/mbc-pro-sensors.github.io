<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × matrix × bigchannel），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/matrix/bigchannel -->
# Ranger 8-Way (TOF8) × MATRIX × Big-channel library

This page covers **LauMo big-channel library (MATRIXblock / Arduino)** only: how to set it up for the Ranger 8-Way (TOF8) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-bigchannel.en.md + _build/guides/product/tof8/matrix-bigchannel.en.md {"product":"Ranger 8-Way (TOF8)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): LauMo big-channel library (MATRIXblock / Arduino)](/en/sensors/tof8/matrix.md).
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

### nearest distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="nearest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |

```cpp
int nearCm = bigU8(9);   // nearest distance (cm)
```

<p class="ref-note">Big channel byte 9（u8）</p>

</section>

<section id="v02" class="ref-item">

### nearest direction <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="nearest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |

```cpp
int nearDir = bigU8(8) - 8;   // nearest direction
```

<p class="ref-note">Big channel byte 8（u8）; 原始方位 0~16</p>

</section>

<section id="v03" class="ref-item">

### farthest distance (cm) <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="farthest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |

```cpp
int farCm = bigU8(11);   // farthest distance (cm)
```

<p class="ref-note">Big channel byte 11（u8）</p>

</section>

<section id="v04" class="ref-item">

### farthest direction <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="farthest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-7~+7` | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |

```cpp
int farDir = bigU8(10) - 8;   // farthest direction
```

<p class="ref-note">Big channel byte 10（u8）; 原始方位 1~15</p>

</section>

<section id="v06" class="ref-item">

### nearest distance (mm) <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="nearest distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |

```cpp
int nearMm = bigU16(14);   // nearest distance (mm)
```

<p class="ref-note">Big channel byte 14~15（u16）; #67 追加（[14..15] 小端）</p>

</section>

<section id="v07" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |

```cpp
int initOk = bigU8(12);   // init bitmap
```

<p class="ref-note">Big channel byte 12（u8）; #67 補填（註解有寫、原本沒填）</p>

</section>

<section id="v08" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

```cpp
int fault = bigU8(13);   // fault bitmap
```

<p class="ref-note">Big channel byte 13（u8）; #67 補填</p>

</section>

<section id="v09" class="ref-item">

### front distance (mm) <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="front distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |

```cpp
int frontMm = bigU16(16);   // front distance (mm)
```

<p class="ref-note">Big channel byte 16~17（u16）; #67 追加（MATRIX 定案 §4：[16..17] 正前方 mm 小端）</p>

</section>

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

```cpp
int distCm[8];
for (uint8_t i = 0; i < 8; i++) distCm[i] = bigU8(0 + i);   // sensor 1~8 distance (cm)
```

<p class="ref-note">Big channel byte 0~7（u8）</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：front distance (cm), sensor 1~8 distance (mm). Use <a href="/en/sensors/tof8/arduino/ide.md">Arduino</a> / <a href="/en/sensors/tof8/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/tof8/microbit/makecode.md">micro:bit</a> for these.</p>

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/matrix-bigchannel.en.md`、`_build/guides/product/tof8/matrix-bigchannel.en.md`
