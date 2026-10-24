<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/arduino/ide -->
# Controller 2.4G (PS2) × Arduino × Arduino IDE

This page covers **Arduino IDE (C++)** only: how to set it up for the Controller 2.4G (PS2) on Arduino / ESP32 / UNO R4 (I2C), how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** (4 pins: 5V, GND, SDA, SCL), factory I2C address `0x42`. UNO / Nano: A4 (SDA), A5 (SCL); UNO R4 and ESP32: the pins marked SDA / SCL.

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-uno.svg" alt="Controller 2.4G (PS2) × Arduino / ESP32 / UNO R4 (I2C)" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/arduino-ide.en.md + _build/guides/product/ps2/arduino-ide.en.md {"product":"Controller 2.4G (PS2)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): Arduino IDE (C++)](/en/sensors/ps2/arduino-i2c.md).
<!-- /include-fallback -->

## Reading values

Every snippet uses the `mbcRead()` helper (expand "Helper code" and paste it at the bottom of your sketch); address `0x42`.

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

### 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |

```cpp
int16_t buttons16 = mbcRead(0x42, 0x01);   // 16 buttons：0~65535 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

```cpp
int16_t dpad = mbcRead(0x42, 0x02);   // d-pad：0~15 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |

```cpp
int16_t shape = mbcRead(0x42, 0x03);   // shape buttons：0~15 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

```cpp
int16_t shoulder = mbcRead(0x42, 0x04);   // shoulder buttons：0~15 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |

```cpp
int16_t func = mbcRead(0x42, 0x05);   // function buttons：0~15 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |

```cpp
int16_t connected = mbcRead(0x42, 0x06);   // connected：0~1
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v07" class="ref-item">

### battery <small>battery</small>

| Range | Meaning |
| :--- | :--- |
| `0` % | 0~100; PS2 pads report no battery → always 0 (PS4 only) |

```cpp
int16_t battery = mbcRead(0x42, 0x07);   // battery：0 %
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v08" class="ref-item">

### L2 analog <small>l2Analog</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` | L2 pressure 0~255 (only DualShock 2 in analog mode reports pressure; otherwise pressed = 255, released = 0); 0 when not connected |

```cpp
int16_t l2Analog = mbcRead(0x42, 0x08);   // L2 analog：0~255
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v09" class="ref-item">

### R2 analog <small>r2Analog</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` | R2 pressure 0~255 (same rule as 08); 0 when not connected |

```cpp
int16_t r2Analog = mbcRead(0x42, 0x09);   // R2 analog：0~255
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v0a" class="ref-item">

### system keys <small>sysKeys</small>

| Range | Meaning |
| :--- | :--- |
| `0` bit | b0 PS button, b1 capture button; PS2 pads have none → always 0 (PS4 only) |

```cpp
int16_t sysKeys = mbcRead(0x42, 0x0A);   // system keys：0 bit
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v0b" class="ref-item">

### gamepad type <small>padType</small>

| Range | Meaning |
| :--- | :--- |
| `0~2` | 0 none, 1 PS2 digital mode, 2 PS2 analog mode (3 PS4 / 4 PS5 only appear on the PS4 receiver) |

```cpp
int16_t padType = mbcRead(0x42, 0x0B);   // gamepad type：0~2
```

<p class="ref-note">Returns <code>MBC_NONE</code> (−32768) when the module does not answer.</p>

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

```cpp
int16_t stick[4];
mbcReadMany(0x42, 0x10, 4, stick);   // stick LX / LY / RX / RY：-100~+100
for (uint8_t i = 0; i < 4; i++) { Serial.print(stick[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">Ids 0x10~0x13 are consecutive: one <code>mbcReadMany</code> call returns them from the same 10 ms snapshot.</p>

</section>

## Commands

<section id="ce5" class="ref-item ref-cmd">

### rumble <small>vibrate</small>

[strength 0~255], 0 = stop; the PS2 receiver ACKs and ignores it (pad rumble disabled since 250614), effective on PS4 only

```cpp
mbcWrite(0x42, 0xE5, 128);   // rumble（strength 0~255）
```

</section>

<section id="ce6" class="ref-item ref-cmd">

### light bar colour <small>setLed</small>

[R][G][B] 0~255 each; PS2 pads have no light bar, the receiver ACKs and ignores it, effective on PS4 only

```cpp
mbcWrite(0x42, 0xE6, 128, 128, 128);   // light bar colour（red 0~255, green 0~255, blue 0~255）
```

</section>

## Examples

<!-- examples: en/sensors/ps2/arduino-examples.md host=arduino sw=ide whole -->

## More questions

- **Always reading -32768?** The module is not answering: check SDA / SCL are not swapped, the address is right and 5V / GND are connected.
- **Can I connect several sensors?** Yes, each needs its own address; re-address identical sensors with command F8 (see the universal I2C protocol).

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [All examples ps2_arduino_examples_en.zip](/examples/ps2/arduino-en/ps2_arduino_examples_en.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/arduino-ide.en.md`、`_build/guides/product/ps2/arduino-ide.en.md`
- Examples：`sensors/ps2/arduino-examples.md`
