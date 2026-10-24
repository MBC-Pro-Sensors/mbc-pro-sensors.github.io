<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/mblock/mblock5 -->
# Ranger 8-Way (TOF8) × mBlock × mBlock 5

This page covers **mBlock 5 (MBC Pro Sensors extension)** only: how to set it up for the Ranger 8-Way (TOF8) on Makeblock (mBot) + mBlock 5, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Which edition and how to connect

Get the **MakeBlock edition** (RJ25 plug) and plug it into an RJ25 port of mBot / Ranger / Orion / MegaPi; usable ports per board are listed in [MakeBlock extension](/en/makeblock.md).

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-mbot.svg" alt="Ranger 8-Way (TOF8) × Makeblock (mBot) + mBlock 5" loading="lazy"></figure>

## Setup

<!-- setup: _build/guides/software/mblock-mblock5.en.md + _build/guides/product/tof8/mblock-mblock5.en.md {"product":"Ranger 8-Way (TOF8)","edition":"MakeBlock edition","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 8-Way (TOF8): mBlock 5 (MBC Pro Sensors extension)](/en/sensors/tof8/mblock.md).
<!-- /include-fallback -->

## Reading values

After adding the MBC Pro Sensors extension, find the blocks in the "Ranger 8" category and pick the port the sensor is plugged into.

<section id="v01" class="ref-item">

### nearest distance (cm) <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="nearest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | smallest of the 8 distances, 0~120 cm (a failed sensor counts as 120); 120 = nothing around |

<pre class="blocks">(Ranger 8 port [1 v] nearest distance cm :: sensing)</pre>

<p class="ref-note">Block returns nearest distance (cm)（0~120 cm）。Host sends <code>[01]</code> → reply <code>[lo][hi][01 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v02" class="ref-item">

### nearest direction <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="nearest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-8~+8` | direction of the nearest object: −7 (left) to +7 (right), 0 = straight ahead, negative = left; ±8 = nothing around (keeps the last side) |

<pre class="blocks">(Ranger 8 port [1 v] nearest direction :: sensing)</pre>

<p class="ref-note">Block returns nearest direction（-8~+8）。Host sends <code>[02]</code> → reply <code>[lo][hi][02 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v03" class="ref-item">

### farthest distance (cm) <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="farthest distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | largest of the 8 distances, 0~120 cm (a failed sensor always reads 120 and looks 'farthest') |

<pre class="blocks">(Ranger 8 port [1 v] farthest distance cm :: sensing)</pre>

<p class="ref-note">Block returns farthest distance (cm)（0~120 cm）。Host sends <code>[03]</code> → reply <code>[lo][hi][03 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v04" class="ref-item">

### farthest direction <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="farthest direction" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-7~+7` | direction of the most open side: −7 to +7, 0 = straight ahead, negative = left; 0 when all 8 read 120 |

<pre class="blocks">(Ranger 8 port [1 v] farthest direction :: sensing)</pre>

<p class="ref-note">Block returns farthest direction（-7~+7）。Host sends <code>[04]</code> → reply <code>[lo][hi][04 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v05" class="ref-item">

### front distance (cm) <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="front distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | average of sensors 4 and 5 (both facing forward), 0~120 cm; EV3 mode 3 and SPIKE mode 6 carry the same thing in mm; 0x22 'C' is min(4, 5), not the average (see 10) |

<pre class="blocks">(Ranger 8 port [1 v] front distance cm :: sensing)</pre>

<p class="ref-note">Block returns front distance (cm)（0~120 cm）。Host sends <code>[05]</code> → reply <code>[lo][hi][05 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v06" class="ref-item">

### nearest distance (mm) <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="nearest distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | smallest of the 8 mm readings, 0~1200 (failed sensor counts as 1200); 1200 = nothing around |

<pre class="blocks">(Ranger 8 port [1 v] nearest distance mm :: sensing)</pre>

<p class="ref-note">Block returns nearest distance (mm)（0~1200 mm）。Host sends <code>[06]</code> → reply <code>[lo][hi][06 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v07" class="ref-item">

### init bitmap <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="init bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 (left) … bit7 = sensor 8; 1 = initialised at power-on (8 sensors take ~2~4 s, no retry); 255 = all OK |

<pre class="blocks">(Ranger 8 port [1 v] init bitmap :: sensing)</pre>

<p class="ref-note">Block returns init bitmap（0~255 bit）。Host sends <code>[07]</code> → reply <code>[lo][hi][07 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v08" class="ref-item">

### fault bitmap <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="fault bitmap" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~255` bit | bit0 = sensor 1 … bit7 = sensor 8; 1 = currently failing (I2C error or no update for 200 ms); its distance reads 120 / 1200 |

<pre class="blocks">(Ranger 8 port [1 v] fault bitmap :: sensing)</pre>

<p class="ref-note">Block returns fault bitmap（0~255 bit）。Host sends <code>[08]</code> → reply <code>[lo][hi][08 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v09" class="ref-item">

### front distance (mm) <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="front distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | average of sensors 4 and 5 (the two facing forward), 0~1200 mm; 1200 = nothing ahead. cm version is 05 |

<pre class="blocks">(Ranger 8 port [1 v] front distance mm :: sensing)</pre>

<p class="ref-note">Block returns front distance (mm)（0~1200 mm）。Host sends <code>[09]</code> → reply <code>[lo][hi][09 ^ lo ^ hi]</code>, value = int16 little-endian.</p>

</section>

<section id="v10" class="ref-item">

### sensor 1~8 distance (cm) <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="sensor 1~8 distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm | distance of each sensor 1~8, 0~120 cm (1 = far left, 4 and 5 = front, 8 = far right, 30° apart); 120 = nothing that way / sensor failed; ids 10~17 |

<pre class="blocks">(Ranger 8 port [1 v] sensor [1 v] distance cm :: sensing)</pre>

<p class="ref-note">Block returns sensor 1~8 distance (cm)（0~120 cm）。Host sends <code>[10]</code> → reply <code>[lo][hi][10 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x10 + (N − 1).</p>

</section>

<section id="v18" class="ref-item">

### sensor 1~8 distance (mm) <small>distMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distMm.svg" alt="sensor 1~8 distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | distance of each sensor 1~8, 0~1200 mm; 1200 = nothing that way / sensor failed; ids 18~1F |

<pre class="blocks">(Ranger 8 port [1 v] sensor [1 v] distance mm :: sensing)</pre>

<p class="ref-note">Block returns sensor 1~8 distance (mm)（0~1200 mm）。Host sends <code>[18]</code> → reply <code>[lo][hi][18 ^ lo ^ hi]</code>, value = int16 little-endian; channel N uses id 0x18 + (N − 1).</p>

</section>

## Examples

<!-- examples: en/sensors/tof8/mblock-examples.md host=mblock sw=mblock5 whole -->

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
- Values and commands：`_build/protocol/tof8.json` (single source of truth)
- Setup：`_build/guides/software/mblock-mblock5.en.md`、`_build/guides/product/tof8/mblock-mblock5.en.md`
- Examples：`sensors/tof8/mblock-examples.md`
