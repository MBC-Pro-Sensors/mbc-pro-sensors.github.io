<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/microbit/makecode -->
# Controller PS4/PS5 × micro:bit × MakeCode

This page covers **MakeCode (blocks / JavaScript)** only: how to set it up for the Controller PS4/PS5 on BBC micro:bit V1 / V2, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **Universal I2C edition** and connect it to the I2C header of a micro:bit breakout (SDA = P20, SCL = P19), factory address `0x42`. It **must use an external 5 V supply** (the ESP32 Bluetooth receiver draws current peaks), not the micro:bit 3V pin; share the supply GND with the micro:bit.

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-microbit.svg" alt="Controller PS4/PS5 × BBC micro:bit V1 / V2" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/microbit-makecode.en.md + _build/guides/product/ps4/microbit-makecode.en.md {"product":"Controller PS4/PS5","edition":"Universal I2C edition","port":"I2C","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller PS4/PS5: MakeCode (blocks / JavaScript)](/en/sensors/ps4/microbit.md).
<!-- /include-fallback -->

## Reading values

After adding pxt-mbc-sensors, find the blocks in the MBC Sensors category; the JavaScript form is shown too, address `0x42`.

<section id="v01" class="ref-item">

### 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | all buttons in one 16-bit word (PSX layout): bit0 SELECT(SHARE), 1 L3, 2 R3, 3 START(OPTIONS), 4 up, 5 right, 6 down, 7 left, 8 L2, 9 R2, 10 L1, 11 R1, 12 triangle, 13 circle, 14 cross, 15 square; 1 = pressed; 0 when not connected |

<pre class="blocks">(gamepad buttons bit mask :: #00A4E4)</pre>

```typescript
let buttons16 = mbc.padButtons()   // 16 buttons：0~65535 bit
```

**Related block "gamepad button [cross] is pressed"**

```typescript
let padButtonPressed = mbc.padButtonPressed(MbcPadButton.Cross)
```

**Related block "on gamepad button [cross] [pressed]"**

```typescript
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, function () {
})
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v02" class="ref-item">

### D-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="D-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | up 1, right 2, down 4, left 8, added when pressed together (0~15) |

<pre class="blocks">(gamepad [d-pad v] buttons \(0-15\) :: #00A4E4)</pre>

```typescript
let dpad = mbc.padGroup(MbcPadGroup.Dpad)   // D-pad：0~15 bit
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | triangle 1, circle 2, cross 4, square 8, added when pressed together (0~15) |

<pre class="blocks">(gamepad [shapes v] buttons \(0-15\) :: #00A4E4)</pre>

```typescript
let shapeKeys = mbc.padGroup(MbcPadGroup.Shapes)   // shape buttons：0~15 bit
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8, added when pressed together (0~15) |

<pre class="blocks">(gamepad [shoulders v] buttons \(0-15\) :: #00A4E4)</pre>

```typescript
let shoulderKeys = mbc.padGroup(MbcPadGroup.Shoulders)   // shoulder buttons：0~15 bit
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1, L3 2, R3 4, START(OPTIONS) 8, added when pressed together (0~15) |

<pre class="blocks">(gamepad [functions v] buttons \(0-15\) :: #00A4E4)</pre>

```typescript
let funcKeys = mbc.padGroup(MbcPadGroup.Functions)   // function buttons：0~15 bit
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected and data seen within 300 ms, 0 = not connected (buttons 0, sticks centred while disconnected) |

<pre class="blocks"><gamepad is connected :: #00A4E4></pre>

```typescript
let connected = mbc.padConnected()   // connected：0~1
```

<p class="ref-note">Returns false when the sensor does not answer.</p>

</section>

<section id="v07" class="ref-item">

### battery <small>battery</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-battery.svg" alt="battery" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` % | gamepad battery 0~100 (always 0 on the PS2 receiver) |

<pre class="blocks">(gamepad battery percent :: #00A4E4)</pre>

```typescript
let battery = mbc.padBattery()   // battery：0~100 %
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v08" class="ref-item">

### L2 analog <small>l2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-l2Analog.svg" alt="L2 analog" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | L2 trigger depth 0~255 (0 when not connected) |

<pre class="blocks">(gamepad trigger [L2 v] analog :: #00A4E4)</pre>

```typescript
let l2Analog = mbc.padTrigger(MbcPadTrigger.L2)   // L2 analog：0~255
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v09" class="ref-item">

### R2 analog <small>r2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-r2Analog.svg" alt="R2 analog" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | R2 trigger depth 0~255 (0 when not connected) |

<pre class="blocks">(gamepad trigger [R2 v] analog :: #00A4E4)</pre>

```typescript
let r2Analog = mbc.padTrigger(MbcPadTrigger.R2)   // R2 analog：0~255
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v0a" class="ref-item">

### system buttons <small>sysKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-sysKeys.svg" alt="system buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~3` bit | b0 PS button, b1 capture (PS5 mute) button; 0 when not connected (always 0 on PS2) |

No dedicated block; in JavaScript read it with `mbc.readValue(address, id)` (−32768 when nothing answers). Related block: "gamepad system button [PS] is pressed".

```typescript
let sysKeys = mbc.readValue(0x42, 0x0A)   // system buttons：0~3 bit
```

</section>

<section id="v0b" class="ref-item">

### gamepad type <small>padType</small>

| Range | Meaning |
| :--- | :--- |
| `0~4` | 0 none, 1 PS2 digital, 2 PS2 analog, 3 PS4, 4 PS5 (or other) |

<pre class="blocks">(gamepad type :: #00A4E4)</pre>

```typescript
let padType = mbc.padType()   // gamepad type：0~4
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

<section id="v10" class="ref-item">

### stick axis 1~4 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="stick axis 1~4" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right/up positive; 0 when not connected |

<pre class="blocks">(gamepad joystick [left X v] :: #00A4E4)</pre>

```typescript
let stick = [mbc.padStick(MbcPadAxis.LeftX), mbc.padStick(MbcPadAxis.LeftY), mbc.padStick(MbcPadAxis.RightX), mbc.padStick(MbcPadAxis.RightY)]   // stick axis 1~4：-100~+100
```

<p class="ref-note">Returns 0 when the sensor does not answer; check with "device at … is ready" first.</p>

</section>

## Commands

<section id="ce5" class="ref-item ref-cmd">

### rumble <small>rumble</small>

[strength 0~255], 0 = stop; 200 ms per call (ignored by PS2)

<pre class="blocks">gamepad rumble strength (200) :: #00A4E4</pre>

```typescript
mbc.padRumble(200)   // rumble
```

</section>

<section id="ce6" class="ref-item ref-cmd">

### light bar colour <small>lightbar</small>

[R][G][B] each 0~255, exactly 3 bytes; remembered until connected; default blue (0,0,255) (ignored by PS2)

<pre class="blocks">gamepad light bar colour [#ff0000] :: #00A4E4</pre>

```typescript
mbc.padLight(0xff0000)   // light bar colour
```

</section>

## Examples

<!-- examples: en/sensors/ps4/microbit-examples.md host=microbit sw=makecode whole -->

## More questions

- **Cannot find the MBC blocks?** In MakeCode Extensions, paste https://github.com/contactmbcrobot/pxt-mbc-sensors.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MakeCode extension pxt-mbc-sensors (paste this URL in MakeCode Extensions)](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps4.json` (single source of truth)
- Setup：`_build/guides/software/microbit-makecode.en.md`、`_build/guides/product/ps4/microbit-makecode.en.md`
- Examples：`sensors/ps4/microbit-examples.md`
