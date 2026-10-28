<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/mblock/mblock5 -->
# Sharpshooter Vision (ESP32CAM) × mBlock × mBlock 5

This page covers **mBlock 5 (MBC Pro Sensors extension)** only: how to set it up for the Sharpshooter Vision (ESP32CAM) on Makeblock (mBot) + mBlock 5, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **MakeBlock edition** (RJ25 plug) and plug it into an RJ25 port of mBot / Ranger / Orion / MegaPi; usable ports per board are listed in [MakeBlock extension](/en/makeblock.md).

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-mbot.svg" alt="Sharpshooter Vision (ESP32CAM) × Makeblock (mBot) + mBlock 5" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/mblock-mblock5.en.md + _build/guides/product/esp32cam/mblock-mblock5.en.md {"product":"Sharpshooter Vision (ESP32CAM)","edition":"MakeBlock edition","port":"1","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Sharpshooter Vision (ESP32CAM): mBlock 5 (MBC Pro Sensors extension)](/en/sensors/esp32cam/mblock.md).
<!-- /include-fallback -->

## Reading values (School edition)

After adding the MBC Pro Sensors extension, find the blocks in the "Vision" category and pick the port the sensor is plugged into.

<section id="v01" class="ref-item">

### hue <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="hue" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-1~100` | hue of sample point S1, 0~100 (hue ring 0~240 scaled); −1 = black/white/grey (saturation < 10); 0 when S1 is not set |

<pre class="blocks">(Vision port [1 v] hue :: sensing)</pre>

<p class="ref-note">Block returns hue（-1~100）。Host sends <code>[01]</code> → reply <code>[lo][hi][01 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v02" class="ref-item">

### saturation <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="saturation" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | saturation of sample point S1, 0~100 |

<pre class="blocks">(Vision port [1 v] saturation :: sensing)</pre>

<p class="ref-note">Block returns saturation（0~100）。Host sends <code>[02]</code> → reply <code>[lo][hi][02 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v03" class="ref-item">

### brightness <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="brightness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | brightness (HSV value) of sample point S1, 0~100 |

<pre class="blocks">(Vision port [1 v] brightness :: sensing)</pre>

<p class="ref-note">Block returns brightness（0~100）。Host sends <code>[03]</code> → reply <code>[lo][hi][03 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v04" class="ref-item">

### motion <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="motion" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | whole-frame motion 0~100 (more change = larger) |

<pre class="blocks">(Vision port [1 v] motion :: sensing)</pre>

<p class="ref-note">Block returns motion（0~100）。Host sends <code>[04]</code> → reply <code>[lo][hi][04 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v05" class="ref-item">

### blob X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="blob X" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob centre X 0~100, left = 0; 0 when not found |

<pre class="blocks">(Vision port [1 v] blob X :: sensing)</pre>

<p class="ref-note">Block returns blob X（0~100）。Host sends <code>[05]</code> → reply <code>[lo][hi][05 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v06" class="ref-item">

### blob Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="blob Y" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob centre Y 0~75, bottom = 0, top = 75; 0 when not found |

<pre class="blocks">(Vision port [1 v] blob Y :: sensing)</pre>

<p class="ref-note">Block returns blob Y（0~75）。Host sends <code>[06]</code> → reply <code>[lo][hi][06 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v07" class="ref-item">

### blob width <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="blob width" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~100` | blob width W 0~100; 0 = not found |

<pre class="blocks">(Vision port [1 v] blob width :: sensing)</pre>

<p class="ref-note">Block returns blob width（0~100）。Host sends <code>[07]</code> → reply <code>[lo][hi][07 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v08" class="ref-item">

### blob height <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="blob height" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~75` | blob height H 0~75; 0 when not found |

<pre class="blocks">(Vision port [1 v] blob height :: sensing)</pre>

<p class="ref-note">Block returns blob height（0~75）。Host sends <code>[08]</code> → reply <code>[lo][hi][08 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v09" class="ref-item">

### blob area <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="blob area" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~7500` | W×H (0~7500); 0 when not found |

<pre class="blocks">(Vision port [1 v] blob area :: sensing)</pre>

<p class="ref-note">Block returns blob area（0~7500）。Host sends <code>[09]</code> → reply <code>[lo][hi][09 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v0a" class="ref-item">

### blob found <small>found</small>

| Range | Meaning |
| :--- | :--- |
| `0~1` | 1 = a blob of the target colour is in view (W > 0), 0 = none |

<pre class="blocks">(Vision port [1 v] blob found? :: sensing)</pre>

<p class="ref-note">Block returns blob found（0~1）。Host sends <code>[0A]</code> → reply <code>[lo][hi][0A ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

## Commands

<section id="cx-readall" class="ref-item ref-cmd">

### update all values <small>readAll</small>

reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one

<pre class="blocks">Vision port [1 v] update all values :: sensing</pre>

</section>

## Reading values (WRO edition)

After adding the MBC Pro Sensors extension, find the blocks in the "Vision WRO" category and pick the port the sensor is plugged into.

<section id="w10" class="ref-item">

### spot 1~12 colour code <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="spot 1~12 colour code" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 10~1B = colour code of spots S1~S12: 0~239 colour (hue 0~255 scaled to 0~239), 240~245 white (saturation < 30, lower = larger), 246~250 black (value < 30, darker = larger), 255 = spot not enabled |

<pre class="blocks">(Vision WRO port [1 v] spot [1 v] colour code :: sensing)</pre>

<p class="ref-note">Block returns spot 1~12 colour code（0~255）。Host sends <code>[10]</code> → reply <code>[lo][hi][10 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x10 + (N − 1).</p>

</section>

<section id="w20" class="ref-item">

### spot 1~12 vividness <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="spot 1~12 vividness" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` | 20~2B = vividness (saturation) of spots S1~S12, 0~255: black = 1, white/colour = actual saturation; 0 when the spot is not enabled |

<pre class="blocks">(Vision WRO port [1 v] spot [1 v] vividness :: sensing)</pre>

<p class="ref-note">Block returns spot 1~12 vividness（0~255）。Host sends <code>[20]</code> → reply <code>[lo][hi][20 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x20 + (N − 1).</p>

</section>

## Commands (WRO edition)

<section id="wcx-readall" class="ref-item ref-cmd">

### update all values <small>readAll</small>

reads the whole value table into a cache (FD pack-all); reporter blocks within the next 10 ms use the cache instead of asking one by one

<pre class="blocks">Vision WRO port [1 v] update all values :: sensing</pre>

</section>

## Examples (School edition)

<!-- examples: en/sensors/esp32cam/mblock-examples.md host=mblock sw=mblock5 whole -->

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
- Values and commands：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json` (single source of truth)
- Setup：`_build/guides/software/mblock-mblock5.en.md`、`_build/guides/product/esp32cam/mblock-mblock5.en.md`
- Examples：`sensors/esp32cam/mblock-examples(-wro).md`
