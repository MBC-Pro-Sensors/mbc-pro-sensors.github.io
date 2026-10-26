<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/arduino/ide -->
# Ranger 2-Way (TOF2) × Arduino × Arduino IDE

This page covers **Arduino IDE (C++)** only: how to set it up for the Ranger 2-Way (TOF2) on Arduino / ESP32 / UNO R4 (I2C), how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** (4 pins: 5V, GND, SDA, SCL), factory I2C address `0x29`. UNO / Nano: A4 (SDA), A5 (SCL); UNO R4 and ESP32: the pins marked SDA / SCL.

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-uno.svg" alt="Ranger 2-Way (TOF2) × Arduino / ESP32 / UNO R4 (I2C)" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/arduino-ide.en.md + _build/guides/product/tof2/arduino-ide.en.md {"product":"Ranger 2-Way (TOF2)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): Arduino IDE (C++)](/en/sensors/tof2/arduino-i2c.md).
<!-- /include-fallback -->

## Reading values

Every snippet uses the `mbcRead()` helper: expand "Helper code", put part ① (2 lines) at the top of your sketch and paste part ② at the bottom; address `0x29`.

<details class="helper">
<summary>Helper code (① at the top, ② at the bottom)</summary>

**① At the top of your sketch** (before `setup()`)

```cpp
#include <Wire.h>

const int16_t MBC_NONE = -32768;   // no answer / undefined id
```

**② At the bottom of your sketch** (after `loop()`)

```cpp
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
// with 2 parameters whose first is 0, write (uint8_t)0, e.g. mbcWrite(ADDR, 0xF8, (uint8_t)0x00, 0xFF); otherwise it clashes with the array form below and will not compile
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

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

```cpp
int16_t leftCm = mbcRead(0x29, 0x01);   // left distance (cm)：0~120 cm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

```cpp
int16_t rightCm = mbcRead(0x29, 0x02);   // right distance (cm)：0~120 cm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v03" class="ref-item">

### nearer distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="nearer distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | the smaller of left and right, 0~120 cm |

```cpp
int16_t nearCm = mbcRead(0x29, 0x03);   // nearer distance (cm)：0~120 cm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v04" class="ref-item">

### left distance (mm) <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="left distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | left sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

```cpp
int16_t leftMm = mbcRead(0x29, 0x04);   // left distance (mm)：0~1200 mm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v05" class="ref-item">

### right distance (mm) <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="right distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | right sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

```cpp
int16_t rightMm = mbcRead(0x29, 0x05);   // right distance (mm)：0~1200 mm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v06" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor initialised at power-on (no retry); 3 = both OK |

```cpp
int16_t initOk = mbcRead(0x29, 0x06);   // init bitmap：0~3 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v07" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | bit0 = left, bit1 = right; 1 = that sensor is currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

```cpp
int16_t fault = mbcRead(0x29, 0x07);   // fault bitmap：0~3 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v08" class="ref-item">

### average distance (mm) <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="average distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | (left mm + right mm) / 2, 0~1200; same number as EV3 mode 3 and the 3rd value of SPIKE mode 6 |

```cpp
int16_t avgMm = mbcRead(0x29, 0x08);   // average distance (mm)：0~1200 mm
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

## Examples

<!-- examples: en/sensors/tof2/arduino-examples.md host=arduino sw=ide whole -->

## More questions

- **Always reading -32768?** The module is not answering: check SDA / SCL are not swapped, the address is right and 5V / GND are connected.
- **Can I connect several sensors?** Yes, each needs its own address; re-address identical sensors with command F8 (see the universal I2C protocol).

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [All examples tof2_arduino_examples_en.zip](/examples/tof2/arduino-en/tof2_arduino_examples_en.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/arduino-ide.en.md`、`_build/guides/product/tof2/arduino-ide.en.md`
- Examples：`sensors/tof2/arduino-examples.md`
