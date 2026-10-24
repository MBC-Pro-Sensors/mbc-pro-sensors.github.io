<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/arduino/ide -->
# Ranger 8-Way (TOF8) × Arduino × Arduino IDE

This page covers **Arduino IDE (C++)** only: how to set it up for the Ranger 8-Way (TOF8) on Arduino / ESP32 / UNO R4 (I2C), how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** (4 pins: 5V, GND, SDA, SCL), factory I2C address `0x29`. UNO / Nano: A4 (SDA), A5 (SCL); UNO R4 and ESP32: the pins marked SDA / SCL.

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-uno.svg" alt="Ranger 8-Way (TOF8) × Arduino / ESP32 / UNO R4 (I2C)" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/arduino-ide.en.md + _build/guides/product/tof8/arduino-ide.en.md {"product":"Ranger 8-Way (TOF8)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): Arduino IDE (C++)](/en/sensors/tof8/arduino-i2c.md).
<!-- /include-fallback -->

## Reading values

Every snippet uses the `mbcRead()` helper (expand "Helper code" and paste it at the bottom of your sketch); address `0x29`.

<details class="helper">
<summary>Helper code (paste at the bottom of your sketch)</summary>

```cpp
#include <Wire.h>

const int16_t MBC_NONE = -32768;   // no answer / undefined id

// read k consecutive values (k ≤ 16) starting at id, int16 little-endian
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                                        // always wait 2 ms (ESP32 products cannot clock-stretch)
  if (Wire.requestFrom(addr, (uint8_t)(k * 2)) != (uint8_t)(k * 2)) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint8_t lo = Wire.read(), hi = Wire.read();
    out[i] = (int16_t)(lo | (hi << 8));
  }
  return true;
}

// read one value; MBC_NONE when nothing answers
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// write a command (0~3 parameters)
void mbcWrite(uint8_t addr, uint8_t id, const uint8_t *args, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(args[i]);
  Wire.endTransmission();
}
void mbcWrite(uint8_t addr, uint8_t id) { mbcWrite(addr, id, nullptr, 0); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a) { mbcWrite(addr, id, &a, 1); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a, uint8_t b) { uint8_t p[2] = {a, b}; mbcWrite(addr, id, p, 2); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a, uint8_t b, uint8_t c) { uint8_t p[3] = {a, b, c}; mbcWrite(addr, id, p, 3); }

// wait until ready (F3 bit0 = 1), at most ms milliseconds
bool mbcWaitReady(uint8_t addr, unsigned long ms) {
  unsigned long t0 = millis();
  while (millis() - t0 < ms) {
    int16_t st = mbcRead(addr, 0xF3);
    if (st != MBC_NONE && (st & 1)) return true;
    delay(20);
  }
  return false;
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
int16_t nearCm = mbcRead(0x29, 0x01);   // nearest distance (cm)：0~120 cm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v02" class="ref-item">

### nearest direction <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="nearest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |

```cpp
int16_t nearDir = mbcRead(0x29, 0x02);   // nearest direction：-8~+8
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v03" class="ref-item">

### farthest distance (cm) <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="farthest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |

```cpp
int16_t farCm = mbcRead(0x29, 0x03);   // farthest distance (cm)：0~120 cm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v04" class="ref-item">

### farthest direction <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="farthest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-7~+7` | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |

```cpp
int16_t farDir = mbcRead(0x29, 0x04);   // farthest direction：-7~+7
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v05" class="ref-item">

### front distance (cm) <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="front distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3, SPIKE mode 6 and big-channel [16..17] carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |

```cpp
int16_t frontCm = mbcRead(0x29, 0x05);   // front distance (cm)：0~120 cm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v06" class="ref-item">

### nearest distance (mm) <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="nearest distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |

```cpp
int16_t nearMm = mbcRead(0x29, 0x06);   // nearest distance (mm)：0~1200 mm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v07" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |

```cpp
int16_t initOk = mbcRead(0x29, 0x07);   // init bitmap：0~255 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v08" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

```cpp
int16_t fault = mbcRead(0x29, 0x08);   // fault bitmap：0~255 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v09" class="ref-item">

### front distance (mm) <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="front distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |

```cpp
int16_t frontMm = mbcRead(0x29, 0x09);   // front distance (mm)：0~1200 mm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

```cpp
int16_t distCm[8];
mbcReadMany(0x29, 0x10, 8, distCm);   // sensor 1~8 distance (cm)：0~120 cm
for (uint8_t i = 0; i < 8; i++) { Serial.print(distCm[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">Ids 0x10~0x17 are consecutive: one <code>mbcReadMany</code> call returns them from the same 10 ms snapshot.</p>

</section>

<section id="v18" class="ref-item">

### sensor 1~8 distance (mm) <small>distMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distMm.svg" alt="sensor 1~8 distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | distance of each sensor 1~8, 0~1200 mm; 1200 = nothing that way / sensor failed; ids 18~1F |

```cpp
int16_t distMm[8];
mbcReadMany(0x29, 0x18, 8, distMm);   // sensor 1~8 distance (mm)：0~1200 mm
for (uint8_t i = 0; i < 8; i++) { Serial.print(distMm[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">Ids 0x18~0x1F are consecutive: one <code>mbcReadMany</code> call returns them from the same 10 ms snapshot.</p>

</section>

## Examples

<!-- examples: en/sensors/tof8/arduino-examples.md host=arduino sw=ide whole -->

## More questions

- **Always reading -32768?** The module is not answering: check SDA / SCL are not swapped, the address is right and 5V / GND are connected.
- **Can I connect several sensors?** Yes, each needs its own address; re-address identical sensors with command F8 (see the universal I2C protocol).

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [All examples tof8_arduino_examples_en.zip](/examples/tof8/arduino-en/tof8_arduino_examples_en.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/arduino-ide.en.md`、`_build/guides/product/tof8/arduino-ide.en.md`
- Examples：`sensors/tof8/arduino-examples.md`
