<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × ev3 × clev3r），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/ev3/clev3r -->
# Ranger 2-Way (TOF2) × EV3 × clev3r

This page covers **clev3r (EV3 BASIC)** only: how to set it up for the Ranger 2-Way (TOF2) on MINDSTORMS EV3, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/ev3-clev3r.en.md + _build/guides/product/tof2/ev3-clev3r.en.md {"product":"Ranger 2-Way (TOF2)","edition":"EV3 edition","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Ranger 2-Way (TOF2): clev3r (EV3 BASIC)](/en/sensors/tof2/ev3-examples.md).
<!-- /include-fallback -->

## Reading values

Code assumes port 1: `Sensor.SetMode` picks the mode, `Sensor.ReadRawValue` reads the n-th value.

<section id="v01" class="ref-item">

### left distance (cm) <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="left distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm (EV3 mode 1, mode 2, mode 4 give a scaled value: cm×100/120 (0~100)) | distance from the left sensor, 0~120 cm (left = sensor 0 seen from behind the robot); 120 = nothing in range / sensor failed |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
leftCm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 2)
leftCm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
leftCm = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">mode 1 · int8 · #67 新：IR-SEEK 8×int8 = [左, 右, 左, 右, 左, 右, 左, 右]（index 0/2/4/6 = 左；原本 index 2~7 = 0）; mode 2 · int8 · IR-REMOTE [左, 右, 0, 0]（INO:137-139）; mode 4 · int8 · IR-S-ALT 同模式 2 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v02" class="ref-item">

### right distance (cm) <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="right distance (cm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~120` cm (EV3 mode 1, mode 2, mode 4 give a scaled value: cm×100/120 (0~100)) | distance from the right sensor, 0~120 cm (right = sensor 1); 120 = nothing in range / sensor failed |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
rightCm = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 2)
rightCm = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 4)
rightCm = Sensor.ReadRawValue(1, 1)
```

<p class="ref-note">mode 1 · int8 · #67 新：index 1/3/5/7 = 右; mode 2 · int8; mode 4 · int8 (locked mode: firmware and hosts will not change it)</p>

</section>

<section id="v04" class="ref-item">

### left distance (mm) <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="left distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | left sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
leftMm = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">mode 5 · int16 · #67 新：IR-CAL 2×int16 = [左 mm, 右 mm]（原本 word0 = 左'+256×右'）</p>

</section>

<section id="v05" class="ref-item">

### right distance (mm) <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="right distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm | right sensor distance 0~1200 mm; 1200 = nothing in range / sensor failed |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
rightMm = Sensor.ReadRawValue(1, 1)
```

<p class="ref-note">mode 5 · int16 · #67 新</p>

</section>

<section id="v08" class="ref-item">

### average distance (mm) <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="average distance (mm)" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~1200` mm (EV3 mode 0 gives a scaled value: cm average ×100/120 (0~100)) | (left mm + right mm) / 2, 0~1200; same number as EV3 mode 3 and the 3rd value of SPIKE mode 6 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
avgMm = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
avgMm = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">mode 0 · int8 · IR-PROX = (左'+右')>>1，cm 縮放後平均（鎖住，INO:131-133）; mode 3 · int16 · #67 新：IR-REM-A 1×int16 = 左右平均 mm（原本 = 左 0~100） (locked mode: firmware and hosts will not change it)</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：nearer distance (cm), init bitmap, fault bitmap. Use <a href="/en/sensors/tof2/arduino/ide.md">Arduino</a> / <a href="/en/sensors/tof2/mblock/mblock5.md">mBlock</a> / <a href="/en/sensors/tof2/microbit/makecode.md">micro:bit</a> for these.</p>

## Examples

<!-- examples: en/sensors/tof2/ev3-examples.md host=ev3 sw=clev3r -->

## More questions

- **The EV3 shows an infrared sensor?** Expected: the sensor emulates a LEGO sensor so the stock blocks can read it.
- **A short freeze when switching modes?** The EV3 needs about 0.5 s per mode change. Stick to one mode during a run.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/tof2.json` (single source of truth)
- Setup：`_build/guides/software/ev3-clev3r.en.md`、`_build/guides/product/tof2/ev3-clev3r.en.md`
- Examples：`sensors/tof2/ev3-examples.md`
