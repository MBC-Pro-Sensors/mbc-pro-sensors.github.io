<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/mblock/mblock5 -->
# Controller 2.4G (PS2) × mBlock × mBlock 5

This page covers **mBlock 5 (MBC Pro Sensors extension)** only: how to set it up for the Controller 2.4G (PS2) on Makeblock (mBot) + mBlock 5, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **MakeBlock edition** (RJ25 plug) and plug it into an RJ25 port of mBot / Ranger / Orion / MegaPi; usable ports per board are listed in [MakeBlock extension](/en/makeblock.md).

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-mbot.svg" alt="Controller 2.4G (PS2) × Makeblock (mBot) + mBlock 5" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/mblock-mblock5.en.md + _build/guides/product/ps2/mblock-mblock5.en.md {"product":"Controller 2.4G (PS2)","edition":"MakeBlock edition","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): mBlock 5 (MBC Pro Sensors extension)](/en/sensors/ps2/mblock.md).
<!-- /include-fallback -->

## Reading values

After adding the MBC Pro Sensors extension, find the blocks in the "Controller" category and pick the port the sensor is plugged into.

<section id="v01" class="ref-item">

### 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |

<pre class="blocks">(Controller port [1 v] 16 buttons :: sensing)</pre>

<p class="ref-note">Block returns 16 buttons（0~65535 bit）。Host sends <code>[01]</code> → reply <code>[lo][hi][01 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

<pre class="blocks">(Controller port [1 v] d-pad :: sensing)</pre>

<p class="ref-note">Block returns d-pad（0~15 bit）。Host sends <code>[02]</code> → reply <code>[lo][hi][02 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |

<pre class="blocks">(Controller port [1 v] shape buttons :: sensing)</pre>

<p class="ref-note">Block returns shape buttons（0~15 bit）。Host sends <code>[03]</code> → reply <code>[lo][hi][03 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

<pre class="blocks">(Controller port [1 v] shoulder buttons :: sensing)</pre>

<p class="ref-note">Block returns shoulder buttons（0~15 bit）。Host sends <code>[04]</code> → reply <code>[lo][hi][04 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |

<pre class="blocks">(Controller port [1 v] function buttons :: sensing)</pre>

<p class="ref-note">Block returns function buttons（0~15 bit）。Host sends <code>[05]</code> → reply <code>[lo][hi][05 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v06" class="ref-item">

### connected <small>connected</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = gamepad connected (psx.read() succeeds), 0 = not; while disconnected all buttons read 0 and sticks are centred |

<pre class="blocks">(Controller port [1 v] gamepad connected? :: sensing)</pre>

<p class="ref-note">Block returns connected（0~1）。Host sends <code>[06]</code> → reply <code>[lo][hi][06 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v07" class="ref-item">

### battery <small>battery</small>

| Range | Meaning |
| :--- | :--- |
| `0` % | 0~100; PS2 pads report no battery → always 0 (PS4 only) |

<pre class="blocks">(Controller port [1 v] battery :: sensing)</pre>

<p class="ref-note">Block returns battery（0 %）。Host sends <code>[07]</code> → reply <code>[lo][hi][07 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v08" class="ref-item">

### L2 analog <small>l2Analog</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` | L2 pressure 0~255 (only DualShock 2 in analog mode reports pressure; otherwise pressed = 255, released = 0); 0 when not connected |

<pre class="blocks">(Controller port [1 v] L2 analog :: sensing)</pre>

<p class="ref-note">Block returns L2 analog（0~255）。Host sends <code>[08]</code> → reply <code>[lo][hi][08 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v09" class="ref-item">

### R2 analog <small>r2Analog</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` | R2 pressure 0~255 (same rule as 08); 0 when not connected |

<pre class="blocks">(Controller port [1 v] R2 analog :: sensing)</pre>

<p class="ref-note">Block returns R2 analog（0~255）。Host sends <code>[09]</code> → reply <code>[lo][hi][09 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v0a" class="ref-item">

### system keys <small>sysKeys</small>

| Range | Meaning |
| :--- | :--- |
| `0` bit | b0 PS button, b1 capture button; PS2 pads have none → always 0 (PS4 only) |

<pre class="blocks">(Controller port [1 v] system keys :: sensing)</pre>

<p class="ref-note">Block returns system keys（0 bit）。Host sends <code>[0A]</code> → reply <code>[lo][hi][0A ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v0b" class="ref-item">

### gamepad type <small>padType</small>

| Range | Meaning |
| :--- | :--- |
| `0~2` | 0 none, 1 PS2 digital mode, 2 PS2 analog mode (3 PS4 / 4 PS5 only appear on the PS4 receiver) |

<pre class="blocks">(Controller port [1 v] gamepad type :: sensing)</pre>

<p class="ref-note">Block returns gamepad type（0~2）。Host sends <code>[0B]</code> → reply <code>[lo][hi][0B ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

<pre class="blocks">(Controller port [1 v] stick [LX/LY/RX/RY] :: sensing)</pre>

<p class="ref-note">Block returns stick LX / LY / RX / RY（-100~+100）。Host sends <code>[10]</code> → reply <code>[lo][hi][10 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x10 + (N − 1).</p>

</section>

<section id="vx-pressed" class="ref-item">

### [button] pressed? <small>pressed</small>

| Range | Meaning |
| :--- | :--- |
| true/false | one button split out of 16 buttons 01 (PSX order); menu: ✕ ● ▲ ■ up down left right L1 R1 L2 R2 L3 R3 SELECT START |

<pre class="blocks"><Controller port [1 v] [■ v] pressed? :: sensing></pre>

</section>

## Commands

<section id="ce5" class="ref-item ref-cmd">

### rumble <small>vibrate</small>

[strength 0~255], 0 = stop; the PS2 receiver ACKs and ignores it (pad rumble disabled since 250614), effective on PS4 only

<pre class="blocks">Controller port [1 v] rumble (128) :: sensing</pre>

<p class="ref-note">Host sends <code>[E5][params…]</code> → reply <code>[00][00][E5]</code> (ack), then it runs.</p>

</section>

<section id="ce6" class="ref-item ref-cmd">

### light bar colour <small>setLed</small>

[R][G][B] 0~255 each; PS2 pads have no light bar, the receiver ACKs and ignores it, effective on PS4 only

<pre class="blocks">Controller port [1 v] light bar colour (128) (128) (128) :: sensing</pre>

<p class="ref-note">Host sends <code>[E6][params…]</code> → reply <code>[00][00][E6]</code> (ack), then it runs.</p>

</section>

<section id="cx-readall" class="ref-item ref-cmd">

### update all values <small>readAll</small>

reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one

<pre class="blocks">Controller port [1 v] update all values :: sensing</pre>

</section>

## Examples

<!-- examples: en/sensors/ps2/mblock-examples.md host=mblock sw=mblock5 whole -->

## More questions

- **Blocks keep returning 0?** Check the port in the block matches the real port and that the program was uploaded to the board.
- **Does the old extension still work?** Sensors shipped before 2026-10 use the legacy v1.0.0 extension; old and new blocks are not compatible.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext)
- [MBC_Pro_Sensors_mBlock5_v1.0.0.mext (legacy, units shipped before 2026-10)](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/mblock-mblock5.en.md`、`_build/guides/product/ps2/mblock-mblock5.en.md`
- Examples：`sensors/ps2/mblock-examples.md`
