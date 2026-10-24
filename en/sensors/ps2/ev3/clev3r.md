<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × ev3 × clev3r），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/ev3/clev3r -->
# Controller 2.4G (PS2) × EV3 × clev3r

This page covers **clev3r (EV3 BASIC)** only: how to set it up for the Controller 2.4G (PS2) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-clev3r.en.md + _build/guides/product/ps2/ev3-clev3r.en.md {"product":"Controller 2.4G (PS2)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): clev3r (EV3 BASIC)](/en/sensors/ps2/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes port 1: `Sensor.SetMode` picks the mode, `Sensor.ReadRawValue` reads the n-th value.

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 1)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
dpad = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">mode 0 · int8 · IR-PROX = 方向鍵（.ino:130-132）; mode 1 · int8 · IR-SEEK [方向, LX, 形狀, LY, 肩, RX, 功能, RY]（按鍵與搖桿交錯）; mode 3 · int16 · #67 改：IR-REM-A 1×int16 = 方向 | 形狀<<4 | 已連線<<8（bit0-3 = 方向）; mode 4 · int8 · IR-S-ALT [方向, LX, 形狀, LY] (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shape = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 3)
shape = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
shape = Sensor.ReadRawValue(1, 2)
```

<p class="ref-note">mode 1 · int8; mode 3 · int16 · #67 改：bit4-7 = 形狀（同一個 int16）; mode 4 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shoulder = Sensor.ReadRawValue(1, 4)
```

<p class="ref-note">mode 1 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
func = Sensor.ReadRawValue(1, 6)
```

<p class="ref-note">mode 1 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
connected = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">mode 3 · int16 · #67 改：bit8 = 已連線（+256）</p>

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
stick1 = Sensor.ReadRawValue(1, 1)   ' first; for the N-th use 1 + N - 1
Sensor.SetMode(1, 2)
stick1 = Sensor.ReadRawValue(1, 0)   ' first; for the N-th use 0 + N - 1
Sensor.SetMode(1, 4)
stick1 = Sensor.ReadRawValue(1, 1)   ' first; for the N-th use 1 + N - 1
```

<p class="ref-note">mode 1 · int8 · 交錯：index 1 = LX、3 = LY、5 = RX、7 = RY（偶數 index 是按鍵）; mode 2 · int8 · IR-REMOTE [LX, LY, RX, RY]（描述表宣告 0~10，實送 0~99）; mode 4 · int8 · index 1 = LX、3 = LY（只到左搖桿） (locked mode: firmware and hosts will not change it)</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：16 buttons, battery, L2 analog, R2 analog, system keys, gamepad type. Use <a href="/en/sensors/ps2/arduino/ide.md">Arduino</a> / <a href="/en/sensors/ps2/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/ps2/microbit/makecode.md">micro:bit</a> for these.</p>

## Commands

The EV3 port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/ps2/ev3-examples.md host=ev3 sw=clev3r -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/ev3-clev3r.en.md`、`_build/guides/product/ps2/ev3-clev3r.en.md`
- Examples：`sensors/ps2/ev3-examples.md`
