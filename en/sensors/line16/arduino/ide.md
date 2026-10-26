<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/arduino/ide -->
# Pathfinder 16-Way (Line16) × Arduino × Arduino IDE

This page covers **Arduino IDE (C++)** only: how to set it up for the Pathfinder 16-Way (Line16) on Arduino / ESP32 / UNO R4 (I2C), how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** (4 pins: 5V, GND, SDA, SCL), factory I2C address `0x16`. UNO / Nano: A4 (SDA), A5 (SCL); UNO R4 and ESP32: the pins marked SDA / SCL.

<figure class="ref-fig"><img src="/images/figures/line16/wiring-uno.svg" alt="Pathfinder 16-Way (Line16) × Arduino / ESP32 / UNO R4 (I2C)" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/arduino-ide.en.md + _build/guides/product/line16/arduino-ide.en.md {"product":"Pathfinder 16-Way (Line16)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 16-Way (Line16): Arduino IDE (C++)](/en/sensors/line16/arduino-i2c.md).
<!-- /include-fallback -->

## Reading values

Every snippet uses the `mbcRead()` helper: expand "Helper code", put part ① (2 lines) at the top of your sketch and paste part ② at the bottom; address `0x16`.

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

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-16~+16` | which channel the line is under: −16 (far left) to +16 (far right), 0 = centre; seen from behind the robot, negative = left (normally ±15; ±16 only for 200 ms after the line leaves the outermost channel) |

```cpp
int16_t linePos = mbcRead(0x16, 0x01);   // line position：-16~+16
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

```cpp
int16_t linePosFine = mbcRead(0x16, 0x02);   // fine position：-100~+100
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~16` 路 | number of channels on the line, 0~16; 0 = no line |

```cpp
int16_t lineWidth = mbcRead(0x16, 0x03);   // line width：0~16 路
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v04" class="ref-item">

### line segments <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-groupCount.svg" alt="line segments" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |

```cpp
int16_t groupCount = mbcRead(0x16, 0x04);   // line segments：0~8
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | bit0 = channel 1 (left) … bit15 = channel 16; 1 = that channel sees the line |

```cpp
int16_t bitmap = mbcRead(0x16, 0x05);   // line bitmap：0~65535 bit
```

<p class="ref-note">This value can itself be −32768 (top bit set, e.g. <code>0x8000</code>), the same as <code>MBC_NONE</code> for no answer, so do not test for −32768: use <code>mbcReadMany(0x16, 0x05, 1, &bitmap)</code> instead; only a <code>false</code> return means no answer.</p>

</section>

<section id="v06" class="ref-item">

### last exit direction <small>lastDir</small>

| Range | Meaning |
| :--- | :--- |
| `-16~+16` | where the line was before it was lost: −16 to +16 (negative = left); keeps the last value |

```cpp
int16_t lastDir = mbcRead(0x16, 0x06);   // last exit direction：-16~+16
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v07" class="ref-item">

### threshold <small>threshold</small>

| Range | Meaning |
| :--- | :--- |
| `1~99` | reflect threshold for 'on line', 1~99, default 50; set with E4 |

```cpp
int16_t threshold = mbcRead(0x16, 0x07);   // threshold：1~99
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v08" class="ref-item">

### calibrations done <small>calibCount</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` 次 | number of calibrations completed since power-on (button calibrations included) |

```cpp
int16_t calibCount = mbcRead(0x16, 0x08);   // calibrations done：0~255 次
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v10" class="ref-item">

### channel 1~16 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="channel 1~16 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~16, 0~100 (channel 1 = left); ids 10~1F |

```cpp
int16_t light[16];
mbcReadMany(0x16, 0x10, 16, light);   // channel 1~16 reflect：0~100
for (uint8_t i = 0; i < 16; i++) { Serial.print(light[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">Ids 0x10~0x1F are consecutive: one <code>mbcReadMany</code> call returns them from the same 10 ms snapshot.</p>

</section>

## Commands

<section id="ce0" class="ref-item ref-cmd">

### black line mode <small>blackLine</small>

follow a black line (default)

```cpp
mbcWrite(0x16, 0xE0);   // black line mode
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### white line mode <small>whiteLine</small>

follow a white line (F3 b1 = 1)

```cpp
mbcWrite(0x16, 0xE1);   // white line mode
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### calibrate black <small>calibBlack</small>

store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM

```cpp
mbcWrite(0x16, 0xE2);   // calibrate black
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // wait until F3 bit2 clears = done
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### calibrate white <small>calibWhite</small>

store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM

```cpp
mbcWrite(0x16, 0xE3);   // calibrate white
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // wait until F3 bit2 clears = done
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### set threshold <small>setThreshold</small>

threshold 1~99 (RAM only, back to 50 after reboot)

```cpp
mbcWrite(0x16, 0xE4, 50);   // set threshold（threshold 1~99）
```

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5-second auto calibration <small>calibAuto</small>

sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile

```cpp
mbcWrite(0x16, 0xE7);   // 5-second auto calibration
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // wait until F3 bit2 clears = done
```

</section>

## Examples

<!-- examples: en/sensors/line16/arduino-examples.md host=arduino sw=ide whole -->

## More questions

- **Always reading -32768?** The module is not answering: check SDA / SCL are not swapped, the address is right and 5V / GND are connected.
- **Can I connect several sensors?** Yes, each needs its own address; re-address identical sensors with command F8 (see the universal I2C protocol).

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [All examples line16_arduino_examples_en.zip](/examples/line16/arduino-en/line16_arduino_examples_en.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line16.json` (single source of truth)
- Setup：`_build/guides/software/arduino-ide.en.md`、`_build/guides/product/line16/arduino-ide.en.md`
- Examples：`sensors/line16/arduino-examples.md`
