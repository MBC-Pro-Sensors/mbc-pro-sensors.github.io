<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/mblock/mblock5 -->
# Pathfinder 8-Way (Line8) × mBlock × mBlock 5

This page covers **mBlock 5 (MBC Pro Sensors extension)** only: how to set it up for the Pathfinder 8-Way (Line8) on Makeblock (mBot) + mBlock 5, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **MakeBlock edition** (RJ25 plug) and plug it into an RJ25 port of mBot / Ranger / Orion / MegaPi; usable ports per board are listed in [MakeBlock extension](/en/makeblock.md).

<figure class="ref-fig"><img src="/images/figures/line8/wiring-mbot.svg" alt="Pathfinder 8-Way (Line8) × Makeblock (mBot) + mBlock 5" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/mblock-mblock5.en.md + _build/guides/product/line8/mblock-mblock5.en.md {"product":"Pathfinder 8-Way (Line8)","edition":"MakeBlock edition","port":"1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Pathfinder 8-Way (Line8): mBlock 5 (MBC Pro Sensors extension)](/en/sensors/line8/mblock.md).
<!-- /include-fallback -->

## Reading values

After adding the MBC Pro Sensors extension, find the blocks in the "Pathfinder" category and pick the port the sensor is plugged into.

<section id="v01" class="ref-item">

### line position <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="line position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | which channel the line is under: −8 (far left) to +8 (far right), 0 = centre; seen from behind the robot, negative = left |

<pre class="blocks">(Pathfinder port [1 v] line position :: sensing)</pre>

<p class="ref-note">Block returns line position（-8~+8）。Host sends <code>[01]</code> → reply <code>[lo][hi][01 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v02" class="ref-item">

### fine position <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="fine position" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | sub-channel position: −100 to +100, 0 = centre, negative = left (same sign as line position) |

<pre class="blocks">(Pathfinder port [1 v] fine line position :: sensing)</pre>

<p class="ref-note">Block returns fine position（-100~+100）。Host sends <code>[02]</code> → reply <code>[lo][hi][02 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v03" class="ref-item">

### line width <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="line width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~8` 路 | number of channels on the line, 0~8; 0 = no line |

<pre class="blocks">(Pathfinder port [1 v] line width :: sensing)</pre>

<p class="ref-note">Block returns line width（0~8 路）。Host sends <code>[03]</code> → reply <code>[lo][hi][03 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v04" class="ref-item">

### line segments <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="line segments" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~4` | number of separate line segments: 0 = lost, 1 = one line, ≥2 = fork / crossing |

<pre class="blocks">(Pathfinder port [1 v] line segments :: sensing)</pre>

<p class="ref-note">Block returns line segments（0~4）。Host sends <code>[04]</code> → reply <code>[lo][hi][04 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v05" class="ref-item">

### line bitmap <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="line bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = channel 1 (left) … bit7 = channel 8; 1 = that channel sees the line |

<pre class="blocks">(Pathfinder port [1 v] line bitmap :: sensing)</pre>

<p class="ref-note">Block returns line bitmap（0~255 bit）。Host sends <code>[05]</code> → reply <code>[lo][hi][05 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v06" class="ref-item">

### last exit direction <small>lastDir</small>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | where the line was before it was lost: −8 to +8 (negative = left); keeps the last value |

<pre class="blocks">(Pathfinder port [1 v] last exit direction :: sensing)</pre>

<p class="ref-note">Block returns last exit direction（-8~+8）。Host sends <code>[06]</code> → reply <code>[lo][hi][06 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v07" class="ref-item">

### threshold <small>threshold</small>

| Range | Meaning |
| :--- | :--- |
| `1~99` | reflect threshold for 'on line', 1~99, default 50; set with E4 |

<pre class="blocks">(Pathfinder port [1 v] threshold :: sensing)</pre>

<p class="ref-note">Block returns threshold（1~99）。Host sends <code>[07]</code> → reply <code>[lo][hi][07 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v08" class="ref-item">

### calibrations done <small>calibCount</small>

| Range | Meaning |
| :--- | :--- |
| `0~255` 次 | number of calibrations completed since power-on (button calibrations included) |

<pre class="blocks">(Pathfinder port [1 v] calibration count :: sensing)</pre>

<p class="ref-note">Block returns calibrations done（0~255 次）。Host sends <code>[08]</code> → reply <code>[lo][hi][08 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v10" class="ref-item">

### channel 1~8 reflect <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="channel 1~8 reflect" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | calibrated reflect of channel 1~8, 0~100 (channel 1 = left); ids 10~17 |

<pre class="blocks">(Pathfinder port [1 v] channel [1 v] reflect :: sensing)</pre>

<p class="ref-note">Block returns channel 1~8 reflect（0~100）。Host sends <code>[10]</code> → reply <code>[lo][hi][10 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x10 + (N − 1).</p>

</section>

<section id="vx-online" class="ref-item">

### channel N on line? <small>onLine</small>

| Range | Meaning |
| :--- | :--- |
| true/false | bit N−1 of line bitmap 05 (channel 1 = bit0, leftmost): true when that channel sees the line |

<pre class="blocks"><Pathfinder port [1 v] channel [1 v] on line? :: sensing></pre>

</section>

## Commands

<section id="ce0" class="ref-item ref-cmd">

### black line mode <small>blackLine</small>

follow a black line (default)

<pre class="blocks">Pathfinder port [1 v] black line mode :: sensing</pre>

<p class="ref-note">Host sends <code>[E0]</code> → reply <code>[00][00][E0]</code> (ack), then it runs.</p>

</section>

<section id="ce1" class="ref-item ref-cmd">

### white line mode <small>whiteLine</small>

follow a white line (F3 b1 = 1)

<pre class="blocks">Pathfinder port [1 v] white line mode :: sensing</pre>

<p class="ref-note">Host sends <code>[E1]</code> → reply <code>[00][00][E1]</code> (ack), then it runs.</p>

</section>

<section id="ce2" class="ref-item ref-cmd">

### calibrate black <small>calibBlack</small>

store the current reading as black (sensor over the black line), ~50 ms; saved to EEPROM

<pre class="blocks">Pathfinder port [1 v] calibrate black :: sensing</pre>

<p class="ref-note">Host sends <code>[E2]</code> → reply <code>[00][00][E2]</code> (ack), then it runs; F3 bit2 = 1 while busy.</p>

</section>

<section id="ce3" class="ref-item ref-cmd">

### calibrate white <small>calibWhite</small>

store the current reading as white (sensor over the white surface), ~50 ms; saved to EEPROM

<pre class="blocks">Pathfinder port [1 v] calibrate white :: sensing</pre>

<p class="ref-note">Host sends <code>[E3]</code> → reply <code>[00][00][E3]</code> (ack), then it runs; F3 bit2 = 1 while busy.</p>

</section>

<section id="ce4" class="ref-item ref-cmd">

### set threshold <small>setThreshold</small>

threshold 1~99 (RAM only, back to 50 after reboot)

<pre class="blocks">Pathfinder port [1 v] set threshold (50) :: sensing</pre>

<p class="ref-note">Host sends <code>[E4][params…]</code> → reply <code>[00][00][E4]</code> (ack), then it runs.</p>

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5-second auto calibration <small>calibAuto</small>

sweep the sensor across black line and white surface for 5 s; min/max become the references; F3 b2 = 1 meanwhile

<pre class="blocks">Pathfinder port [1 v] 5-second auto calibration :: sensing</pre>

<p class="ref-note">Host sends <code>[E7]</code> → reply <code>[00][00][E7]</code> (ack), then it runs; F3 bit2 = 1 while busy.</p>

</section>

<section id="cx-readall" class="ref-item ref-cmd">

### update all values <small>readAll</small>

reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one

<pre class="blocks">Pathfinder port [1 v] update all values :: sensing</pre>

</section>

## Examples

<!-- examples: en/sensors/line8/mblock-examples.md host=mblock sw=mblock5 whole -->

## More questions

- **Blocks keep returning 0?** Check the port in the block matches the real port and that the program was uploaded to the board.
- **Does the old extension still work?** Sensors shipped before 2026-10 use the legacy v1.0.0 extension; old and new blocks are not compatible.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext)
- [MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext)
- [MBC_Pro_Sensors_mBlock5_v1.0.0.mext (legacy, units shipped before 2026-10)](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext)
- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/line8.json` (single source of truth)
- Setup：`_build/guides/software/mblock-mblock5.en.md`、`_build/guides/product/line8/mblock-mblock5.en.md`
- Examples：`sensors/line8/mblock-examples.md`
