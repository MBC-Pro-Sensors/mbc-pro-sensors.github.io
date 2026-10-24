<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/microbit/makecode -->
# Controller 2.4G (PS2) × micro:bit × MakeCode

This page covers **MakeCode (blocks / JavaScript)** only: how to set it up for the Controller 2.4G (PS2) on BBC micro:bit V1 / V2, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x42`. The sensor needs 5 V; use a breakout board with a 5 V output.

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-microbit.svg" alt="Controller 2.4G (PS2) × BBC micro:bit V1 / V2" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/microbit-makecode.en.md + _build/guides/product/ps2/microbit-makecode.en.md {"product":"Controller 2.4G (PS2)","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): MakeCode (blocks / JavaScript)](/en/sensors/ps2/microbit.md).
<!-- /include-fallback -->

## Reading values

After adding pxt-mbc-sensors, find the blocks in the MBC category; the JavaScript form is shown too, address `0x42`.

<section id="v01" class="ref-item">

### 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |

<pre class="blocks">(MBC 16 buttons :: #E8711E)</pre>

```typescript
let buttons16 = MBC.readValue(0x42, 0x01)   // 16 buttons：0~65535 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

<pre class="blocks">(MBC d-pad :: #E8711E)</pre>

```typescript
let dpad = MBC.readValue(0x42, 0x02)   // d-pad：0~15 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |

<pre class="blocks">(MBC shape buttons :: #E8711E)</pre>

```typescript
let shape = MBC.readValue(0x42, 0x03)   // shape buttons：0~15 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

<pre class="blocks">(MBC shoulder buttons :: #E8711E)</pre>

```typescript
let shoulder = MBC.readValue(0x42, 0x04)   // shoulder buttons：0~15 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |

<pre class="blocks">(MBC function buttons :: #E8711E)</pre>

```typescript
let func = MBC.readValue(0x42, 0x05)   // function buttons：0~15 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |

<pre class="blocks">(MBC gamepad connected? :: #E8711E)</pre>

```typescript
let connected = MBC.readValue(0x42, 0x06)   // connected：0~1
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v07" class="ref-item">

### battery <small>battery</small>

| Range | Meaning |
| :--- | :--- |
| `0` % | 0~100; PS2 pads report no battery → always 0 (PS4 only) |

<pre class="blocks">(MBC battery :: #E8711E)</pre>

```typescript
let battery = MBC.readValue(0x42, 0x07)   // battery：0 %
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v08" class="ref-item">

### L2 analog <small>l2Analog</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` | L2 pressure 0~255 (only DualShock 2 in analog mode reports pressure; otherwise pressed = 255, released = 0); 0 when not connected |

<pre class="blocks">(MBC L2 analog :: #E8711E)</pre>

```typescript
let l2Analog = MBC.readValue(0x42, 0x08)   // L2 analog：0~255
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v09" class="ref-item">

### R2 analog <small>r2Analog</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` | R2 pressure 0~255 (same rule as 08); 0 when not connected |

<pre class="blocks">(MBC R2 analog :: #E8711E)</pre>

```typescript
let r2Analog = MBC.readValue(0x42, 0x09)   // R2 analog：0~255
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v0a" class="ref-item">

### system keys <small>sysKeys</small>

| Range | Meaning |
| :--- | :--- |
| `0` bit | b0 PS button, b1 capture button; PS2 pads have none → always 0 (PS4 only) |

<pre class="blocks">(MBC system keys :: #E8711E)</pre>

```typescript
let sysKeys = MBC.readValue(0x42, 0x0A)   // system keys：0 bit
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v0b" class="ref-item">

### gamepad type <small>padType</small>

| Range | Meaning |
| :--- | :--- |
| `0~2` | 0 none, 1 PS2 digital mode, 2 PS2 analog mode (3 PS4 / 4 PS5 only appear on the PS4 receiver) |

<pre class="blocks">(MBC gamepad type :: #E8711E)</pre>

```typescript
let padType = MBC.readValue(0x42, 0x0B)   // gamepad type：0~2
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

<pre class="blocks">(MBC stick [LX/LY/RX/RY] :: #E8711E)</pre>

```typescript
let stick = MBC.readValues(0x42, 0x10, 4)   // stick LX / LY / RX / RY
```

<p class="ref-note">The extension reads 2 bytes as <code>Int16LE</code> and pauses 2 ms after writing the id.</p>

</section>

## Commands

<section id="ce5" class="ref-item ref-cmd">

### rumble <small>vibrate</small>

[strength 0~255], 0 = stop; the PS2 receiver ACKs and ignores it (pad rumble disabled since 250614), effective on PS4 only

<pre class="blocks">MBC rumble (50) :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE5, [50])   // rumble
```

</section>

<section id="ce6" class="ref-item ref-cmd">

### light bar colour <small>setLed</small>

[R][G][B] 0~255 each; PS2 pads have no light bar, the receiver ACKs and ignores it, effective on PS4 only

<pre class="blocks">MBC light bar R[r] G[g] B[b] :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE6, [50, 50, 50])   // light bar colour
```

</section>

## Examples

<!-- examples: en/sensors/ps2/microbit-examples.md host=microbit sw=makecode whole -->

## More questions

- **Cannot find the MBC blocks?** In MakeCode Extensions, paste https://github.com/contactmbcrobot/pxt-mbc-sensors.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MakeCode extension pxt-mbc-sensors (paste this URL in MakeCode Extensions)](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/microbit-makecode.en.md`、`_build/guides/product/ps2/microbit-makecode.en.md`
- Examples：`sensors/ps2/microbit-examples.md`
