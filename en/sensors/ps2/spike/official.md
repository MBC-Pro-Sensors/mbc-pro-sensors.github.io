<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × spike × official），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/spike/official -->
# Controller 2.4G (PS2) × SPIKE × Official LEGO App

This page covers **Official LEGO App (SPIKE 3, Robot Inventor)** only: how to set it up for the Controller 2.4G (PS2) on SPIKE Prime / Robot Inventor, how to read every value, the commands it supports and examples. Use the switcher above to change product, host or software.

## Setup

<!-- setup: _build/guides/software/spike-official.en.md + _build/guides/product/ps2/spike-official.en.md {"product":"Controller 2.4G (PS2)","edition":"SPIKE edition","port":"C","lib":"MBC_gamepad_Lib.py","libUrl":"/examples/ps2/spike_beginner/pybricks/MBC_gamepad_Lib.py","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> Setup steps for this software are being moved onto this page. Meanwhile see the previous guide: [Controller 2.4G (PS2): Official LEGO App (SPIKE 3, Robot Inventor)](/en/sensors/ps2/spike-examples.md).
<!-- /include-fallback -->

## Reading values

The official app shows the sensor as a Color Sensor. Read it with the color, reflected light and raw red / green / blue blocks under More Sensors, then decode as shown.

<section id="v01" class="ref-item">

### 16 buttons <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="16 buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~65535` bit | all 16 buttons in one word (PSX order, 1 = pressed): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 Up, 5 Right, 6 Down, 7 Left, 8 L2, 9 R2, 10 L1, 11 R1, 12 Triangle, 13 Circle, 14 Cross, 15 Square; 0 when not connected |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | 16 buttons | 「raw blue」 | the number it returns |

<p class="ref-note">Firmware note: App 原始藍 = button16（.ino:201-211）</p>

</section>

<section id="v02" class="ref-item">

### d-pad <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="d-pad" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Up 1, Right 2, Down 4, Left 8 (added when pressed together), 0~15; 0 when not connected |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | d-pad | 「reflected light」 | the number it returns |
| Robot Inventor | d-pad | 「reflected light」 | the number it returns |

<p class="ref-note">Firmware note: byte1 = 方向 | 形狀<<4（App 反射光）：低 4 bit; byte0 = 方向 | 形狀<<4</p>

</section>

<section id="v03" class="ref-item">

### shape buttons <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="shape buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | Triangle 1, Circle 2, Cross 4, Square 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'R' |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | shape buttons | 「reflected light」 | the number it returns |
| Robot Inventor | shape buttons | 「reflected light」 | the number it returns |

<p class="ref-note">Firmware note: 高 4 bit; 高 4 bit</p>

</section>

<section id="v04" class="ref-item">

### shoulder buttons <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="shoulder buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | L2 1, R2 2, L1 4, R1 8 (added when pressed together), 0~15; 0 when not connected |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | shoulder buttons | 「color」 | the number it returns |
| Robot Inventor | shoulder buttons | 「raw blue」 | the number mod 256 (Robot Inventor look-up encoding, see setup above) |

<p class="ref-note">Firmware note: byte0 = 肩 | 功能<<4（App 顏色，START = −128）：低 4 bit; byte6~7 = (肩 | 功能<<4) 經玩具版查表編碼；byte1 = 單鍵編號（L2=1 R2=2 L1=3 R1=4 SELECT=5 L3=6 R3=7 START=8，沒按 0）</p>

</section>

<section id="v05" class="ref-item">

### function buttons <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="function buttons" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `0~15` bit | SELECT 1, L3 2, R3 4, START 8 (added when pressed together), 0~15; 0 when not connected; high nibble of 0x22 'G' |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | function buttons | 「color」 | the number it returns |

<p class="ref-note">Firmware note: 高 4 bit</p>

</section>

<section id="v10" class="ref-item">

### stick LX / LY / RX / RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="stick LX / LY / RX / RY" loading="lazy"></figure>

| Range | Meaning |
| :--- | :--- |
| `-100~+100` | 10 LX, 11 LY, 12 RX, 13 RY: −100 to +100, 0 = centre, right / up positive (raw 0~255 with ±7 dead zone); 0 when not connected. EV3 / SPIKE / MATRIX carry 0~99 (50 = centre) or 1~9 (5 = centre), see each platform |

| Hub | Value | Block (More Sensors → Color Sensor) | Decode |
| :--- | :--- | :--- | :--- |
| SPIKE Prime | group 1 | 「raw red」 | the number it returns |
| SPIKE Prime | group 2 | 「raw green」 | the number it returns |
| Robot Inventor | group 1 | 「raw red」 | the number mod 256 (Robot Inventor look-up encoding, see setup above) |
| Robot Inventor | group 2 | 「raw red」 | floor(the number ÷ 256) (Robot Inventor look-up encoding, see setup above) |

<p class="ref-note">Firmware note: byte2~3 = LX×10+LY、byte4~5 = RX×10+RY（11~99，中心 55；App 原始紅／綠）; LX×10+LY、RX×10+RY 經玩具版查表編碼成 byte2~3、byte4~5</p>

</section>

<p class="ref-missing"><b>Not readable here</b>：connected, battery, L2 analog, R2 analog, system keys, gamepad type. Use <a href="/en/sensors/ps2/spike/pybricks.md">Pybricks</a> for these.</p>

## Commands

The SPIKE port is read-only, so this software cannot send commands: calibrate with the button on the sensor and change settings over Arduino I2C, mBlock or micro:bit.

## Examples

<!-- examples: en/sensors/ps2/spike-examples.md host=spike sw=official -->

## More questions

- **The hub does not detect the sensor?** Unplug and replug, wait 2 s, try another port. The official app shows it as a colour sensor — that is expected.
- **Can I use the official app and Pybricks on the same hub?** One firmware at a time: Pybricks and the MBC Blocks website need Pybricks firmware, the official app needs LEGO firmware; you can switch back any time with the SPIKE app.

Still stuck? [Contact us](/en/contact.md) with your software and a screenshot of the program.

## Downloads

- [Download centre (libraries, firmware, catalogue for every product)](/en/downloads.md)

## Sources

- Host and software list：`_build/guides.json`
- Values and commands：`_build/protocol/ps2.json` (single source of truth)
- Setup：`_build/guides/software/spike-official.en.md`、`_build/guides/product/ps2/spike-official.en.md`
- Examples：`sensors/ps2/spike-examples.md`
