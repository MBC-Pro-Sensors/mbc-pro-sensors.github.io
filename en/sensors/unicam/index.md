<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Sharpshooter Vision Camera Sensor (In Development)

> [!WARNING]
> **Firmware v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

> [!TIP]
> **New to it? Try the interactive tutorial first**: no hub needed. Drag blobs, sample points and lines in your browser and watch the code build up. Six ways to write it: Pybricks (SPIKE), EV3, Arduino, mBlock, MATRIX, micro:bit → [Start the tutorial](/en/learn/index.md#unicam)

<!-- product-hero -->

<!-- platform-grid -->
<!-- 神攝手（unicam）還沒進 _build/guides.json 三層導覽（gen-guides 會改寫 reference.md 與範例頁），上面的 platform-grid 目前是空的；主機格子由 _build/gen-unicam.mjs 產生在下面。以後整合進三層導覽時，刪掉 gen:unicam-hosts 這一段。 -->
<!-- gen:unicam-hosts -->
<div class="pick" id="pick-host"><h2 class="pick-title">① Pick your host</h2><p class="pick-sub">One page per host: wiring, reading and the smallest working code. The slot-by-slot layout is on the value reference page.</p>
<div class="pick-grid pick-hosts">
<a class="pick-tile pg-spike" href="/en/sensors/unicam/spike/index.md"><span class="pt-img"><img src="/images/illust/host-spike.webp" alt="" loading="lazy"></span><span class="pt-name">SPIKE</span><span class="pt-tag">SPIKE Prime (yellow hub) and Robot Inventor (teal hub)</span><span class="pt-sw">Pybricks library · Pybricks blocks · LEGO app</span></a>
<a class="pick-tile pg-ev3" href="/en/sensors/unicam/ev3/index.md"><span class="pt-img"><img src="/images/illust/host-ev3.webp" alt="" loading="lazy"></span><span class="pt-name">EV3</span><span class="pt-tag">MINDSTORMS EV3, shows up as an Infrared Sensor</span><span class="pt-sw">EV3-G · EV3 Classroom · EV3 MicroPython</span></a>
<a class="pick-tile pg-matrix" href="/en/sensors/unicam/matrix/mvision.md"><span class="pt-img"><img src="/images/illust/host-matrix.webp" alt="" loading="lazy"></span><span class="pt-name">MATRIX</span><span class="pt-tag">MATRIX Mini R4, mVision camera blocks</span><span class="pt-sw">mVision blocks</span></a>
<a class="pick-tile pg-arduino" href="/en/sensors/unicam/arduino/index.md"><span class="pt-img"><img src="/images/illust/host-arduino.webp" alt="" loading="lazy"></span><span class="pt-name">Arduino</span><span class="pt-tag">UNO, Nano, UNO R4, ESP32 over I2C (0x4C)</span><span class="pt-sw">Arduino IDE</span></a>
<a class="pick-tile pg-microbit" href="/en/sensors/unicam/microbit/index.md"><span class="pt-img"><img src="/images/illust/host-microbit.webp" alt="" loading="lazy"></span><span class="pt-name">micro:bit</span><span class="pt-tag">micro:bit V1 / V2 over I2C (0x4C)</span><span class="pt-sw">MakeCode</span></a>
<a class="pick-tile pg-mblock" href="/en/sensors/unicam/mblock/index.md"><span class="pt-img"><img src="/images/illust/host-mblock.webp" alt="" loading="lazy"></span><span class="pt-name">mBlock</span><span class="pt-tag">mBot, Ranger, Orion, MegaPi over RJ25 serial 38400</span><span class="pt-sw">mBlock 5 (MBC Pro Sensors extension v2.1.0)</span></a>
</div></div>
<!-- /gen -->

> **One camera, four jobs: color tracking, color at points, color order and line following, switched in the camera menu. SPIKE, EV3, MATRIX, Arduino and micro:bit can all read it, and each host gets as much data as it can hold.**

## 📸 The 4 camera modes

Pick a camera mode in the camera's **MODE** menu, then a sub-option. **Sub-option = level**: the finer the level, the more values per target and the fewer targets; the table in the next section shows how many each host gets. Mode number = camera mode × 10 + sub-option (e.g. 13 = Color tracking ③ Standard; Color order = 30).

### 1 Color tracking

Teach colors on the camera first (up to 16). The camera finds the blob of each color and gives its position X, Y, plus width, height, count and area share depending on the level.

<figure class="ref-fig"><img src="/images/sensors/unicam/color-track.svg" alt="Color tracking: X, Y, width W and height H of each color" loading="lazy"></figure>

| Sub-option | What you get | Good for |
| :--- | :--- | :--- |
| ① Max colors | Position only (and count), the most colors | Many colors on the field, you only need where they are |
| ② Multi-color | Fewer colors, more precise position | Several colors with a better position |
| ③ Standard (XY + area) | Position + area (+ count) per color | The common "red, yellow, green, blue blocks" task |
| ④ Fine (XYWH) | Position, width, height, count, area | Few colors, use the size to estimate distance |

### 2 Color at points

A few fixed sample points on the frame (S1, S2 …) each give a hue; higher levels add saturation (how vivid) and value (how bright). Set the points on the camera or move them with I2C command E1. Use it for WRO tasks like "which color is in this slot".

<figure class="ref-fig"><img src="/images/sensors/unicam/sample-points.svg" alt="Color at points: sample points 1 to 20 on the frame" loading="lazy"></figure>

Default layout: up to 8 points in one row, more than 8 in two rows (20 points above). Each point averages the 3×3 pixels around it.

| Sub-option | Per point |
| :--- | :--- |
| ① Hue | Hue code |
| ② Hue + saturation | Hue code, saturation (tells light from dark shades) |
| ③ Hue + saturation + value | Hue code, saturation, value (H, S, V) |

### 3 Color order

Blobs in the frame are ordered **left to right** and you get the hue code of each one (the hue code of the color you taught), plus the total number of blocks. Made for WRO sequence tasks. No sub-options.

<figure class="ref-fig"><img src="/images/sensors/unicam/color-order.svg" alt="Color order: blocks numbered 1 to 5 from left to right, hue code on each block" loading="lazy"></figure>

> [!TIP]
> Red has hue code **0**, the same as "nothing". Read the **total blocks** first to know how many slots are valid.

### 4 Line following

The camera looks ahead at an angle and finds the line on **three scan rows: near, middle and far**. Use the near offset for PID, the far offset and angle to see bends coming, and the junction type tells you cross, T or left / right turn. It also reports colored marker lines it crosses (JBC advanced).

<figure class="ref-fig"><img src="/images/sensors/unicam/line-follow.svg" alt="Line following: near, middle and far scan rows; arrows are the offsets, the dotted line is the angle" loading="lazy"></figure>

| Sub-option | Notes |
| :--- | :--- |
| ① Black line | Black line on a white floor, threshold set automatically |
| ② White line | White line on a dark floor |
| ③ Colored line | Aim at the line and press OK to pick its color (line color = target color 1) |

Line-following values: near / middle / far offset (−100~100, negative = line on the left), line angle (−90~90), near and far line width %, line-found flags (near 1 + middle 2 + far 4), junction type (0 none, 1 cross, 2 T, 3 left, 4 right, 5 dead end) and junction distance %, near line left / right edge, colored marker line number and count, side the line was last lost (0 on the line, 1 left, 2 right). All three line types use the same layout; each host gets the values it can hold.

## 📊 Colors per host

The same sub-option carries different amounts of data on each host, and the camera **fills each host to its capacity**: you get as many colors as you taught, minus what does not fit. Click a number for its slot table.

<!-- gen:unicam-counts -->
| Camera mode · sub-option | Pybricks library | SPIKE App | Robot Inventor App | Pybricks blocks | EV3 | MATRIX | I2C |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 1 Color tracking ① Max colors | [14 colors](/en/sensors/unicam/reference.md#pyb-11) | [14 colors](/en/sensors/unicam/reference.md#edu-11) | [4 colors](/en/sensors/unicam/reference.md#toy-11) | [2 colors](/en/sensors/unicam/reference.md#pyblk-11) | [12 colors](/en/sensors/unicam/reference.md#ev3-11) | [16 colors](/en/sensors/unicam/reference.md#mx-11) | [16 colors XY](/en/sensors/unicam/reference.md#i2c-11) |
| 1 Color tracking ② Multi-color | [7 colors](/en/sensors/unicam/reference.md#pyb-12) | [7 colors](/en/sensors/unicam/reference.md#edu-12) | [4 colors](/en/sensors/unicam/reference.md#toy-12) | [2 colors](/en/sensors/unicam/reference.md#pyblk-12) | [8 colors](/en/sensors/unicam/reference.md#ev3-12) | [10 colors](/en/sensors/unicam/reference.md#mx-12) | [16 colors XY + count](/en/sensors/unicam/reference.md#i2c-12) |
| 1 Color tracking ③ Standard (XY + area) | [4 colors XY + area](/en/sensors/unicam/reference.md#pyb-13) | [4 colors XY + area](/en/sensors/unicam/reference.md#edu-13) | [4 colors XY + area](/en/sensors/unicam/reference.md#toy-13) | [2 colors XY](/en/sensors/unicam/reference.md#pyblk-13) | [4 colors XY + area](/en/sensors/unicam/reference.md#ev3-13) | [5 colors XY + area](/en/sensors/unicam/reference.md#mx-13) | [16 colors XY + area + count](/en/sensors/unicam/reference.md#i2c-13) |
| 1 Color tracking ④ Fine (XYWH) | [2 colors XYWH](/en/sensors/unicam/reference.md#pyb-14) | [2 colors XYWH](/en/sensors/unicam/reference.md#edu-14) | [2 colors XYWH](/en/sensors/unicam/reference.md#toy-14) | [1 color XYWH](/en/sensors/unicam/reference.md#pyblk-14) | [2 colors XYWH](/en/sensors/unicam/reference.md#ev3-14) | [3 colors XYWH](/en/sensors/unicam/reference.md#mx-14) | [16 colors XYWH + count + area](/en/sensors/unicam/reference.md#i2c-14) |
| 2 Color at points ① Hue | [14 points hue](/en/sensors/unicam/reference.md#pyb-21) | [14 points hue](/en/sensors/unicam/reference.md#edu-21) | [4 points hue](/en/sensors/unicam/reference.md#toy-21) | [2 points hue](/en/sensors/unicam/reference.md#pyblk-21) | [12 points hue](/en/sensors/unicam/reference.md#ev3-21) | [20 points hue](/en/sensors/unicam/reference.md#mx-21) | [16 points hue](/en/sensors/unicam/reference.md#i2c-21) |
| 2 Color at points ② Hue + saturation | [7 points hue + sat](/en/sensors/unicam/reference.md#pyb-22) | [7 points hue + sat](/en/sensors/unicam/reference.md#edu-22) | [2 points hue + sat](/en/sensors/unicam/reference.md#toy-22) | [2 points hue + sat](/en/sensors/unicam/reference.md#pyblk-22) | [4 points hue + sat/value](/en/sensors/unicam/reference.md#ev3-22) | [10 points hue + sat](/en/sensors/unicam/reference.md#mx-22) | [16 points hue + sat](/en/sensors/unicam/reference.md#i2c-22) |
| 2 Color at points ③ Hue + saturation + value | [4 points hue + sat + value](/en/sensors/unicam/reference.md#pyb-23) | [4 points hue + sat + value](/en/sensors/unicam/reference.md#edu-23) | [2 points hue + sat/value](/en/sensors/unicam/reference.md#toy-23) | [1 point(s) raw hue + sat + value](/en/sensors/unicam/reference.md#pyblk-23) | [2 point(s) raw hue + sat + value](/en/sensors/unicam/reference.md#ev3-23) | [6 points hue + sat + value](/en/sensors/unicam/reference.md#mx-23) | [16 points hue + sat + value](/en/sensors/unicam/reference.md#i2c-23) |
| 3 Color order | [13 blocks hue code](/en/sensors/unicam/reference.md#pyb-30) | [13 blocks hue code](/en/sensors/unicam/reference.md#edu-30) | [3 blocks hue code](/en/sensors/unicam/reference.md#toy-30) | [2 blocks hue code](/en/sensors/unicam/reference.md#pyblk-30) | [11 blocks hue code](/en/sensors/unicam/reference.md#ev3-30) | [18 blocks hue code](/en/sensors/unicam/reference.md#mx-30) | [16 blocks hue code](/en/sensors/unicam/reference.md#i2c-30) |
| 4 Line following ①②③ black / white / colored line | [14 values](/en/sensors/unicam/reference.md#pyb-41) | [13 values](/en/sensors/unicam/reference.md#edu-41) | [5 values](/en/sensors/unicam/reference.md#toy-41) | [4 values](/en/sensors/unicam/reference.md#pyblk-41) | [12 values](/en/sensors/unicam/reference.md#ev3-41) | [14 values](/en/sensors/unicam/reference.md#mx-41) | [14 values](/en/sensors/unicam/reference.md#i2c-41) |
<!-- /gen -->

- Full slot-by-slot layout (what is in each slot, range, how to split): [Value reference](/en/sensors/unicam/reference.md).
- One firmware edition per host: **SPIKE edition** (official app and Pybricks detected automatically), **EV3 edition**, **MATRIX edition**, **Universal I2C edition** (Arduino, micro:bit).

## 🎛️ On the camera

The back of the camera has a 1.8" color screen and 4 buttons; everything is set on the camera, no computer needed.

1. **Live view**: left / right cycles target 1 … target N → **MODE** → **SET**. N = the last taught color + 1 empty slot (up to 16).
2. **OK on a target: teach a color** (AxisX → AxisY → OK to pick; hold OK for 0.8 s to finish at once; then adjust Hue / Sta / Size / Locked · MaxArea). **Side button** on a target: clear it.
3. **OK on MODE**: left / right picks camera mode 1~4 → OK → pick a sub-option → OK. Sub-option names show **what the current host actually gets** (e.g. EV3 "8 XY", Pybricks library "7 XY"); before a SPIKE hub has read the camera they show the level names (Max / Multi / Std / Fine, H / HS / HSV). Modes with one sub-option skip that step. In menus the side button goes back one level.
4. **OK on SET**: camera menu (brightness, contrast, saturation, white balance, white calibration, self-test, defaults).
5. **Screen**: top left is the host name (green = connected, red = disconnected); top right shows "mode-sub-option" (e.g. 1-3) and the target number (from 1, the same color number the host receives); the bottom bar shows, per mode: color tracking = X Y W H count of the current target, color at points = H S V of S1, color order = number of blocks, line following = near / far offset, angle, junction type. Color at points draws the sample crosses and numbers, color order labels the blocks, line following draws the three scan rows and the line it found.
6. **Kept after power-off**: target colors (16), camera settings, camera mode and each mode's sub-option, sample-point positions changed over I2C. The first time this generation's firmware is flashed everything resets to defaults (Color tracking ③ Standard).

> [!NOTE]
> A SPIKE hub cannot read which mode the camera is in, so your program must match the camera setting (pass the mode number when you create the Pybricks library object, e.g. `mode=13`). On EV3 read mode 0; over I2C read `0x01`, or switch from your program with command `E0`.

## 📐 Common value rules

- **Coordinates**: pixel X 0~159 (left → right), Y 0~119 (top → bottom). **0 = no target**; when there is one, X and Y are at least 1.
- **Width, height**: pixels (right − left, bottom − top).
- **Area share**: blob pixels ÷ 19200 × 100, rounded; found but under 1% sends 1.
- **Count**: number of blobs larger than the minimum area (Size).
- **Values that can be negative** (line offsets, angle, edges): EV3, Pybricks library (signed byte) and I2C (int16) send them as they are; SPIKE App, MATRIX and Pybricks blocks send them **+100**, so subtract 100.

### Hue code 0~255

<figure class="ref-fig"><img src="/images/sensors/unicam/hue-code.svg" alt="Hue code ring: 0 to 240 is hue, 241 to 248 white to gray, 249 to 255 black" loading="lazy"></figure>

| Hue code | Color |
| :--- | :--- |
| 0~240 | Hue: red 0 (also 240), yellow 40, green 80, cyan 120, blue 160, purple 200 |
| 241~248 | White → gray (248 = white) |
| 249~255 | Black (255 = darkest) |

A pixel counts as black / white / gray when chroma (max − min) ≤ 24 or saturation ≤ 12%, and as black when its value is < 64. Saturation and value are both 0~100.

- **EV3 hue**: an EV3 slot only holds −127~100, so hue codes 0~240 are sent as "hue code × 100 ÷ 240" (rounded, so 0~100) and 241~255 as negatives −1~−15 (−(hue code − 240)). EV3 color at points ③ sends the raw hue code in two slots instead (hundreds, remainder of ÷100).

### Grids per host (all 4:3)

Hosts with little room shrink pixel coordinates into grid cells:

<figure class="ref-fig"><img src="/images/sensors/unicam/grid-8x6.svg" alt="8×6 grid: X×10+Y; EV3 counts Y from the bottom" loading="lazy"></figure>

| Grid | Used by | Conversion |
| :--- | :--- | :--- |
| 16×12 | SPIKE App (Standard, Fine) | X÷10+1, Y÷10+1 |
| 16×12 cell number | Pybricks library, 14 colors (1 byte per color) | (Y÷10)×16 + X÷10 + 1, 1~192; the library splits it back into X, Y |
| 8×6 | Robot Inventor App; "X×10+Y" in the multi-color levels of EV3, SPIKE App and MATRIX | X÷20+1, Y÷20+1 |
| 40×30 | Pybricks built-in blocks | X÷4+1, Y÷4+1 |

On EV3, Y is **small at the bottom**: (119−Y)÷20+1; the SPIKE family follows Kito (small at the top). Width / height: 8×6 is W÷20+1, H÷20+1; 16×12 is W÷10+1, H÷10+1; 40×30 is W÷4, H÷4.

## 🔀 How it differs from the legacy editions (School / WRO)

Sharpshooter Vision units shipped before October 2026 (School and WRO editions) are no longer supported; their guides and examples stay online for reference in [Sharpshooter Vision legacy editions (no longer supported)](/en/sensors/esp32cam/index.md). To upgrade one to the current Sharpshooter Vision, contact the dealer you bought it from and send it back: **the firmware update is free**, and the buyer pays shipping both ways.

| | Legacy: School edition | Legacy: WRO edition (HUE12) | Current Sharpshooter Vision |
| :--- | :--- | :--- | :--- |
| Main features | Pick point + blob tracking + motion | Color code and vividness of 12 spots | Color tracking (up to 16 colors), color at points, color order, line following |
| Switch jobs on the camera | — | — | 4 camera modes + sub-options |
| Hosts | SPIKE, EV3, MATRIX, Arduino and more | SPIKE, EV3, MATRIX | SPIKE, EV3, MATRIX, Arduino, micro:bit, mBlock |
| Status | No longer supported | No longer supported | **Firmware v1 in development, not yet hardware-verified** |
| Guide | [Legacy overview](/en/sensors/esp32cam/index.md) | [Legacy overview](/en/sensors/esp32cam/index.md) | This page |

- Programs written for a legacy edition do not run as-is on the current Sharpshooter Vision: the value layout is different, so rewrite them with the host guides on this page.
- To try the current Sharpshooter Vision early, [contact us](/en/contact.md).

## 🧩 Supported hosts

| Host | How it is read | Guide |
| :--- | :--- | :--- |
| SPIKE Prime / Robot Inventor | Pybricks library (modes 9 / 6), Pybricks built-in blocks (modes 5 / 7), official app Education / home edition (mode 0) | [Reading it on SPIKE](/en/sensors/unicam/spike/index.md) |
| EV3 | EV3-G, EV3 Classroom, EV3 MicroPython: Infrared Sensor modes 0 / 1 / 2 | [Reading it on EV3](/en/sensors/unicam/ev3/index.md) |
| MATRIX Mini R4 | MATRIXblock mVision blocks (10 slots) | [Reading it on MATRIX](/en/sensors/unicam/matrix/mvision.md) |
| Arduino / ESP32 / UNO R4 | I2C 0x4C | [Reading it on Arduino](/en/sensors/unicam/arduino/index.md) |
| micro:bit | I2C 0x4C (MakeCode) | [Reading it on micro:bit](/en/sensors/unicam/microbit/index.md) |
| mBlock (mBot, Ranger, Orion, MegaPi) | RJ25 serial 38400, MBC Pro Sensors extension v2.1.0 (in development; download the import file to try it before it is listed) | [Reading it in mBlock](/en/sensors/unicam/mblock/index.md) |

Example programs: [SPIKE](/en/sensors/unicam/spike-examples.md) · [EV3](/en/sensors/unicam/ev3-examples.md) · [Arduino](/en/sensors/unicam/arduino-examples.md) · [micro:bit](/en/sensors/unicam/microbit-examples.md) · [MATRIX](/en/sensors/unicam/matrix-examples.md) · [mBlock](/en/sensors/unicam/mblock-examples.md).

## ⚠️ Known limits, to be tested on hardware

Firmware v1 has not been run on real hardware yet; these may change after testing:

1. **fps**: the 16-color lookup and blob labelling do more work than 4 colors; real fps still has to be measured (Kito with 4 colors runs about 50 fps).
2. **I2C pins**: SDA = IO3, SCL = IO1 is provisional; whether the ESP32 can be an I2C target on IO1 / IO3 has to be tested with an Arduino. The **mBlock edition** still has to be tried on an mBot (only the transport layer was tested on a PC).
3. **Line following** has only been tested on synthetic frames (straight, offset, slanted, cross / T / left / right / dead end, white line, colored line, marker lines, lost side); classroom lighting, shadows and the perspective of looking ahead need thresholds tuned on hardware.
4. Whether **SPIKE App Python** can read bytes 8~15 of mode 0 is not confirmed yet; the 8 bytes the blocks read are not affected.
5. **Sample-point positions**: default layout + I2C E1; the camera menu cannot move sample points yet.
6. **Color order** takes up to 32 blocks (hosts actually get 2~18); the engine handles 16 colors, LEGO hosts use up to 14.
