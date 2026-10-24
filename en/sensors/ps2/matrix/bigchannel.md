<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × matrix × bigchannel），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/matrix/bigchannel -->
# Controller 2.4G (PS2) × MATRIX × Big-channel library

This page covers **LauMo big-channel library (MATRIXblock / Arduino)** only: how to set it up for the Controller 2.4G (PS2) on MATRIX Mini R4, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/matrix-bigchannel.en.md + _build/guides/product/ps2/matrix-bigchannel.en.md {"product":"Controller 2.4G (PS2)","edition":"MATRIX edition","port":"I2C1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): LauMo big-channel library (MATRIXblock / Arduino)](/en/sensors/ps2/matrix.md).
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

### 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |

```cpp
int buttons16 = bigU16(8);   // 16 buttons
```

<p class="ref-note">Big channel byte 8~9（u16）; #67 新增（[8..9] 小端）</p>

</section>

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

```cpp
int dpad = bigU8(0);   // d-pad
```

<p class="ref-note">Big channel byte 0（u8）; #67 新增</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |

```cpp
int shape = bigU8(1);   // shape buttons
```

<p class="ref-note">Big channel byte 1（u8）; #67 新增</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

```cpp
int shoulder = bigU8(2);   // shoulder buttons
```

<p class="ref-note">Big channel byte 2（u8）; #67 新增</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |

```cpp
int func = bigU8(3);   // function buttons
```

<p class="ref-note">Big channel byte 3（u8）; #67 新增</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |

```cpp
int connected = bigU8(10);   // connected
```

<p class="ref-note">Big channel byte 10（u8）; #67 新增</p>

</section>

<section id="v07" class="ref-item">

### battery <small>battery</small>

| Range | Meaning |
| :--- | :--- |
| `0` % | 0~100; PS2 pads report no battery → always 0 (PS4 only) |

```cpp
int battery = bigU8(11);   // battery
```

<p class="ref-note">Big channel byte 11（u8）; #67 新增；PS2 = 0</p>

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

```cpp
int stick[4];
for (uint8_t i = 0; i < 4; i++) stick[i] = bigU8(4 + i);   // stick LX / LY / RX / RY
```

<p class="ref-note">Big channel byte 4~7（u8）; #67 新增：[4..7] = LX、LY、RX、RY</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：L2 analog, R2 analog, system keys, gamepad type. Use <a href="/en/sensors/ps2/arduino/ide.md">Arduino</a> / <a href="/en/sensors/ps2/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/ps2/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

This software cannot send commands; use Arduino I2C, mBlock or micro:bit if you need them.

## More questions

- **Can I mix stock blocks and the big channel?** No: they need different library files; pick one per program.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/matrix-bigchannel.en.md`、`_build/guides/product/ps2/matrix-bigchannel.en.md`
