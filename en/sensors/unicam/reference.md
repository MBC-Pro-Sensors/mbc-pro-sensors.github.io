<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-unicam.mjs 從 _build/protocol/unicam/cam_universal_layout.json 產生，請勿手改；規格改了跑 node _build/gen-unicam.mjs --sync -->
# Value reference: slot by slot

> [!WARNING]
> **Firmware v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

What the Sharpshooter Vision sends on each host for each "camera mode · sub-option", and in which slot. Pick the mode on the camera first (see the [product page](/en/sensors/unicam/index.md)), then find your host below and open the matching table. For code on each host see [SPIKE](/en/sensors/unicam/spike/index.md), [EV3](/en/sensors/unicam/ev3/index.md), [MATRIX](/en/sensors/unicam/matrix/mvision.md), [Arduino](/en/sensors/unicam/arduino/index.md) and [micro:bit](/en/sensors/unicam/microbit/index.md).

## How to read the tables

- **0 = no target**: a coordinate, cell number or area of 0 means nothing was found (when something is found X and Y are at least 1). Hue code 0 is red, so check the area or the total blocks as well.
- **"split ÷100"**: two values share one slot as A×100 + B; A = reading ÷ 100 (integer part), B = remainder of ÷100. "split ÷10" works the same way.
- **"mod 256, ÷256"**: slot = A + B×256; A = reading mod 256, B = reading ÷ 256 (integer part).
- **"A.BB"**: Pybricks `ambient()` shows a decimal: integer part = A, two decimals = B.
- **"(+100)"**: values that can be negative are sent +100; subtract 100 after reading.
- Coordinates: pixel X 0~159 (left → right), Y 0~119 (top → bottom); on EV3, Y is small at the bottom. Grid conversions (16×12, 8×6, 40×30) are on the [product page](/en/sensors/unicam/index.md).
- **Object code (camera mode 5)**: color number × 10 + shape, e.g. 13 = a triangle in color 1, 24 = a square in color 2, 0 = no object. Shapes: 0 other / unknown, 1 circle, 2 ellipse, 3 triangle, 4 square, 5 rectangle, 6 pentagon, 7 hexagon, 8 star, 9 trapezoid. Object 1 = the largest one; only target colors 1~8 count.

<a id="counts"></a>

## Colors / points per host

Click a number to jump to its table. Sub-option = level: the finer the level, the more values per color and the fewer colors.

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
| 5 Object detection ① Object list | [4 objects](/en/sensors/unicam/reference.md#pyb-51) | [7 objects](/en/sensors/unicam/reference.md#edu-51) | [2 objects](/en/sensors/unicam/reference.md#toy-51) | [1 object](/en/sensors/unicam/reference.md#pyblk-51) | [5 objects](/en/sensors/unicam/reference.md#ev3-51) | [4 objects](/en/sensors/unicam/reference.md#mx-51) | [8 objects: color, shape, XY](/en/sensors/unicam/reference.md#i2c-51) |
| 5 Object detection ② Object details | [2 objects + size, area](/en/sensors/unicam/reference.md#pyb-52) | [2 objects + size, area, angle](/en/sensors/unicam/reference.md#edu-52) | [1 object + area](/en/sensors/unicam/reference.md#toy-52) | [1 object + area](/en/sensors/unicam/reference.md#pyblk-52) | [2 objects + area, angle](/en/sensors/unicam/reference.md#ev3-52) | [3 objects + size, area](/en/sensors/unicam/reference.md#mx-52) | [8 objects + size, area, angle](/en/sensors/unicam/reference.md#i2c-52) |

<a id="host-pyb"></a>

## Pybricks library (SPIKE, modes 9 / 6)

With the MBC Pybricks library the hub reads mode 9: 7 int16 = 14 bytes, one value per byte. Coordinates are 160×120 pixels (Y: small at top), 0 = no target; values that can be negative (line offsets, angle, edges) are signed bytes (two's complement). In the Max-colors level each color is one byte = cell number 1~192 on a 16×12 grid, split into X and Y by the library; counts for the Max and Multi levels are in mode 6 (read only when needed); mode 8 is the device ID. This generation's mode 9 layout differs from the Kito edition, so the Kito camLibForKitoMode9 cannot be reused.

<details class="plat pg-spike" id="pyb-11">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ① Max colors: 14 colors</summary>

Mode number 11.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | Color 1 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 1 | Color 2 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 2 | Color 3 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 3 | Color 4 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 4 | Color 5 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 5 | Color 6 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 6 | Color 7 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 7 | Color 8 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 8 | Color 9 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 9 | Color 10 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 10 | Color 11 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 11 | Color 12 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 12 | Color 13 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| Byte 13 | Color 14 cell number (16×12 grid, small at top; the library splits it into X, Y) | 0, 1~192 |
| **SPIKE read(6): only when you need counts (switching modes adds 30 ms; mode 8 is the device ID)** | | |
| Bytes 0~5 | Counts of colors 1~14, 3 bits each (0~7, 7 = 7 or more) + colors found, 4 bits: 46 bits in 6 B (little-endian), split by the library | |
| Decoded | Color 1 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 2 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 3 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 4 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 5 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 6 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 7 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 8 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 9 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 10 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 11 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 12 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 13 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Color 14 count (0~7, 7 = 7 or more) | 0~7 |
| Decoded | Colors found | 0~16 |

</div>

</details>

<details class="plat pg-spike" id="pyb-12">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ② Multi-color: 7 colors</summary>

Mode number 12.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | Color 1 X (px) | 0, 1~159 |
| Byte 1 | Color 1 Y (px, small at top) | 0, 1~119 |
| Byte 2 | Color 2 X (px) | 0, 1~159 |
| Byte 3 | Color 2 Y (px, small at top) | 0, 1~119 |
| Byte 4 | Color 3 X (px) | 0, 1~159 |
| Byte 5 | Color 3 Y (px, small at top) | 0, 1~119 |
| Byte 6 | Color 4 X (px) | 0, 1~159 |
| Byte 7 | Color 4 Y (px, small at top) | 0, 1~119 |
| Byte 8 | Color 5 X (px) | 0, 1~159 |
| Byte 9 | Color 5 Y (px, small at top) | 0, 1~119 |
| Byte 10 | Color 6 X (px) | 0, 1~159 |
| Byte 11 | Color 6 Y (px, small at top) | 0, 1~119 |
| Byte 12 | Color 7 X (px) | 0, 1~159 |
| Byte 13 | Color 7 Y (px, small at top) | 0, 1~119 |
| **SPIKE read(6): only when you need counts (switching modes adds 30 ms; mode 8 is the device ID)** | | |
| Byte 0 | Color 1 count×16 + color 2 count | 0~255 |
| Byte 1 | Color 3 count×16 + color 4 count | 0~255 |
| Byte 2 | Color 5 count×16 + color 6 count | 0~255 |
| Byte 3 | Color 7 count×16 + colors found | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="pyb-13">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ③ Standard (XY + area): 4 colors XY + area</summary>

Mode number 13.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | Color 1 X (px) | 0, 1~159 |
| Byte 1 | Color 1 Y (px, small at top) | 0, 1~119 |
| Byte 2 | Color 1 area (% of frame) | 0, 1~100 % |
| Byte 3 | Color 2 X (px) | 0, 1~159 |
| Byte 4 | Color 2 Y (px, small at top) | 0, 1~119 |
| Byte 5 | Color 2 area (% of frame) | 0, 1~100 % |
| Byte 6 | Color 3 X (px) | 0, 1~159 |
| Byte 7 | Color 3 Y (px, small at top) | 0, 1~119 |
| Byte 8 | Color 3 area (% of frame) | 0, 1~100 % |
| Byte 9 | Color 4 X (px) | 0, 1~159 |
| Byte 10 | Color 4 Y (px, small at top) | 0, 1~119 |
| Byte 11 | Color 4 area (% of frame) | 0, 1~100 % |
| Byte 12 | Color 1 count×16 + color 2 count | 0~255 |
| Byte 13 | Color 3 count×16 + color 4 count | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="pyb-14">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ④ Fine (XYWH): 2 colors XYWH</summary>

Mode number 14.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | Color 1 X (px) | 0, 1~159 |
| Byte 1 | Color 1 Y (px, small at top) | 0, 1~119 |
| Byte 2 | Color 1 width (px) | 0~160 |
| Byte 3 | Color 1 height (px) | 0~120 |
| Byte 4 | Color 1 count | 0~15 |
| Byte 5 | Color 1 area (% of frame) | 0, 1~100 % |
| Byte 6 | Color 2 X (px) | 0, 1~159 |
| Byte 7 | Color 2 Y (px, small at top) | 0, 1~119 |
| Byte 8 | Color 2 width (px) | 0~160 |
| Byte 9 | Color 2 height (px) | 0~120 |
| Byte 10 | Color 2 count | 0~15 |
| Byte 11 | Color 2 area (% of frame) | 0, 1~100 % |
| Byte 12 | Colors found | 0~16 |

</div>

</details>

<details class="plat pg-spike" id="pyb-21">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ① Hue: 14 points hue</summary>

Mode number 21.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | S1 hue | 0~255 |
| Byte 1 | S2 hue | 0~255 |
| Byte 2 | S3 hue | 0~255 |
| Byte 3 | S4 hue | 0~255 |
| Byte 4 | S5 hue | 0~255 |
| Byte 5 | S6 hue | 0~255 |
| Byte 6 | S7 hue | 0~255 |
| Byte 7 | S8 hue | 0~255 |
| Byte 8 | S9 hue | 0~255 |
| Byte 9 | S10 hue | 0~255 |
| Byte 10 | S11 hue | 0~255 |
| Byte 11 | S12 hue | 0~255 |
| Byte 12 | S13 hue | 0~255 |
| Byte 13 | S14 hue | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="pyb-22">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ② Hue + saturation: 7 points hue + sat</summary>

Mode number 22.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | S1 hue | 0~255 |
| Byte 1 | S1 saturation | 0~100 |
| Byte 2 | S2 hue | 0~255 |
| Byte 3 | S2 saturation | 0~100 |
| Byte 4 | S3 hue | 0~255 |
| Byte 5 | S3 saturation | 0~100 |
| Byte 6 | S4 hue | 0~255 |
| Byte 7 | S4 saturation | 0~100 |
| Byte 8 | S5 hue | 0~255 |
| Byte 9 | S5 saturation | 0~100 |
| Byte 10 | S6 hue | 0~255 |
| Byte 11 | S6 saturation | 0~100 |
| Byte 12 | S7 hue | 0~255 |
| Byte 13 | S7 saturation | 0~100 |

</div>

</details>

<details class="plat pg-spike" id="pyb-23">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ③ Hue + saturation + value: 4 points hue + sat + value</summary>

Mode number 23.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | S1 hue | 0~255 |
| Byte 1 | S1 saturation | 0~100 |
| Byte 2 | S1 value | 0~100 |
| Byte 3 | S2 hue | 0~255 |
| Byte 4 | S2 saturation | 0~100 |
| Byte 5 | S2 value | 0~100 |
| Byte 6 | S3 hue | 0~255 |
| Byte 7 | S3 saturation | 0~100 |
| Byte 8 | S3 value | 0~100 |
| Byte 9 | S4 hue | 0~255 |
| Byte 10 | S4 saturation | 0~100 |
| Byte 11 | S4 value | 0~100 |

</div>

</details>

<details class="plat pg-spike" id="pyb-30">
<summary><span class="plat-dot"></span>Camera mode 3 Color order: 13 blocks hue code</summary>

Mode number 30.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | Hue of block 1 from the left | 0~255 |
| Byte 1 | Hue of block 2 from the left | 0~255 |
| Byte 2 | Hue of block 3 from the left | 0~255 |
| Byte 3 | Hue of block 4 from the left | 0~255 |
| Byte 4 | Hue of block 5 from the left | 0~255 |
| Byte 5 | Hue of block 6 from the left | 0~255 |
| Byte 6 | Hue of block 7 from the left | 0~255 |
| Byte 7 | Hue of block 8 from the left | 0~255 |
| Byte 8 | Hue of block 9 from the left | 0~255 |
| Byte 9 | Hue of block 10 from the left | 0~255 |
| Byte 10 | Hue of block 11 from the left | 0~255 |
| Byte 11 | Hue of block 12 from the left | 0~255 |
| Byte 12 | Hue of block 13 from the left | 0~255 |
| Byte 13 | Total blocks | 0~18 |

</div>

</details>

<details class="plat pg-spike" id="pyb-41">
<summary><span class="plat-dot"></span>Camera mode 4 Line following ①②③ black / white / colored line: 14 values</summary>

Mode number 41 / 42 / 43.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | Near offset | −100~100 |
| Byte 1 | Middle offset | −100~100 |
| Byte 2 | Far offset | −100~100 |
| Byte 3 | Line angle | −90~90 |
| Byte 4 | Near line width % | 0~100 |
| Byte 5 | Line-found flags (near 1 + middle 2 + far 4) | 0~7 |
| Byte 6 | Junction type (0 none, 1 cross, 2 T, 3 left, 4 right, 5 dead end) | 0~5 |
| Byte 7 | Junction distance % | 0~100 |
| Byte 8 | Near line left edge | −100~100 |
| Byte 9 | Near line right edge | −100~100 |
| Byte 10 | Far line width % | 0~100 |
| Byte 11 | Color number of the marker line crossed | 0, 1~16 |
| Byte 12 | Marker lines crossed so far | 0~99 |
| Byte 13 | Side the line was last lost (0 on the line, 1 left, 2 right) | 0~2 |

</div>

</details>

<details class="plat pg-spike" id="pyb-51">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ① Object list: 4 objects</summary>

Mode number 51.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | Object count | 0~8 |
| Byte 1 | Object 1 code (color × 10 + shape) | 0, 10~89 |
| Byte 2 | Object 1 X (px) | 0, 1~159 |
| Byte 3 | Object 1 Y (px, small at top) | 0, 1~119 |
| Byte 4 | Object 2 code (color × 10 + shape) | 0, 10~89 |
| Byte 5 | Object 2 X (px) | 0, 1~159 |
| Byte 6 | Object 2 Y (px, small at top) | 0, 1~119 |
| Byte 7 | Object 3 code (color × 10 + shape) | 0, 10~89 |
| Byte 8 | Object 3 X (px) | 0, 1~159 |
| Byte 9 | Object 3 Y (px, small at top) | 0, 1~119 |
| Byte 10 | Object 4 code (color × 10 + shape) | 0, 10~89 |
| Byte 11 | Object 4 X (px) | 0, 1~159 |
| Byte 12 | Object 4 Y (px, small at top) | 0, 1~119 |

</div>

</details>

<details class="plat pg-spike" id="pyb-52">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ② Object details: 2 objects + size, area</summary>

Mode number 52.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **SPIKE read(9)** | | |
| Byte 0 | Object 1 color number | 0, 1~8 |
| Byte 1 | Object 1 shape | 0~9 |
| Byte 2 | Object 1 X (px) | 0, 1~159 |
| Byte 3 | Object 1 Y (px, small at top) | 0, 1~119 |
| Byte 4 | Object 1 width (px) | 0~160 |
| Byte 5 | Object 1 height (px) | 0~120 |
| Byte 6 | Object 1 area (% of frame) | 0, 1~100 % |
| Byte 7 | Object 2 color number | 0, 1~8 |
| Byte 8 | Object 2 shape | 0~9 |
| Byte 9 | Object 2 X (px) | 0, 1~159 |
| Byte 10 | Object 2 Y (px, small at top) | 0, 1~119 |
| Byte 11 | Object 2 width (px) | 0~160 |
| Byte 12 | Object 2 height (px) | 0~120 |
| Byte 13 | Object 2 area (% of frame) | 0, 1~100 % |

</div>

</details>

<a id="host-edu"></a>

## SPIKE App (SPIKE Prime, yellow hub)

The SPIKE App (3.x) sees the camera as a Color Sensor and reads mode 0: "color" and "reflected light" are one byte each, "raw red / green / blue" are int16 each; the blocks can read these 8 bytes. The Standard and Fine levels follow Kito (16×12 grid, X + Y×256: mod 256, ÷256); the Max and Multi levels use an 8×6 grid with two colors per slot (X×10+Y, merged ×100). Y: small at top. Bytes 8~15 are not readable from blocks, only from the app's Python (to be confirmed on hardware).

<details class="plat pg-spike" id="edu-11">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ① Max colors: 14 colors</summary>

Mode number 11.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Colors found | 0~16 |
| Reflected light | Color 1 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Raw red | Color 2 X×10 + Y (8×6 grid, small at top) ×100 + color 3 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Raw green | Color 4 X×10 + Y (8×6 grid, small at top) ×100 + color 5 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Raw blue | Color 6 X×10 + Y (8×6 grid, small at top) ×100 + color 7 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | Color 8 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 9 | Color 9 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 10 | Color 10 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 11 | Color 11 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 12 | Color 12 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 13 | Color 13 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 14 | Color 14 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |

</div>

</details>

<details class="plat pg-spike" id="edu-12">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ② Multi-color: 7 colors</summary>

Mode number 12.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Colors found | 0~16 |
| Reflected light | Color 1 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Raw red | Color 2 X×10 + Y (8×6 grid, small at top) ×100 + color 3 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Raw green | Color 4 X×10 + Y (8×6 grid, small at top) ×100 + color 5 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Raw blue | Color 6 X×10 + Y (8×6 grid, small at top) ×100 + color 7 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | Color 1 count | 0~15 |
| Byte 9 | Color 2 count | 0~15 |
| Byte 10 | Color 3 count | 0~15 |
| Byte 11 | Color 4 count | 0~15 |
| Byte 12 | Color 5 count | 0~15 |
| Byte 13 | Color 6 count | 0~15 |
| Byte 14 | Color 7 count | 0~15 |

</div>

</details>

<details class="plat pg-spike" id="edu-13">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ③ Standard (XY + area): 4 colors XY + area</summary>

Mode number 13.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Color 1 X (16 cells) | 0, 1~16 |
| Reflected light | Color 1 Y (12 cells, small at top) | 0, 1~12 |
| Raw red | Color 2 X (16 cells) + color 2 Y (12 cells, small at top)×256 | Mod 256, ÷256 |
| Raw green | Color 3 X (16 cells) + color 3 Y (12 cells, small at top)×256 | Mod 256, ÷256 |
| Raw blue | Color 4 X (16 cells) + color 4 Y (12 cells, small at top)×256 | Mod 256, ÷256 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | Color 1 count | 0~15 |
| Byte 9 | Color 2 count | 0~15 |
| Byte 10 | Color 3 count | 0~15 |
| Byte 11 | Color 4 count | 0~15 |
| Byte 12 | Color 1 area (% of frame) | 0, 1~100 % |
| Byte 13 | Color 2 area (% of frame) | 0, 1~100 % |
| Byte 14 | Color 3 area (% of frame) | 0, 1~100 % |
| Byte 15 | Color 4 area (% of frame) | 0, 1~100 % |

</div>

</details>

<details class="plat pg-spike" id="edu-14">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ④ Fine (XYWH): 2 colors XYWH</summary>

Mode number 14.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Color 1 X (16 cells) | 0, 1~16 |
| Reflected light | Color 1 Y (12 cells, small at top) | 0, 1~12 |
| Raw red | Color 2 X (16 cells) + color 2 Y (12 cells, small at top)×256 | Mod 256, ÷256 |
| Raw green | Color 1 width (16 cells) + color 1 height (12 cells)×256 | Mod 256, ÷256 |
| Raw blue | Color 2 width (16 cells) + color 2 height (12 cells)×256 | Mod 256, ÷256 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | Color 1 count | 0~15 |
| Byte 9 | Color 2 count | 0~15 |
| Byte 10 | Color 1 area (% of frame) | 0, 1~100 % |
| Byte 11 | Color 2 area (% of frame) | 0, 1~100 % |

</div>

</details>

<details class="plat pg-spike" id="edu-21">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ① Hue: 14 points hue</summary>

Mode number 21.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | (unused, sends 0) |  |
| Reflected light | (unused, sends 0) |  |
| Raw red | S1 hue + S2 hue×256 | Mod 256, ÷256 |
| Raw green | S3 hue + S4 hue×256 | Mod 256, ÷256 |
| Raw blue | S5 hue + S6 hue×256 | Mod 256, ÷256 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | S7 hue | 0~255 |
| Byte 9 | S8 hue | 0~255 |
| Byte 10 | S9 hue | 0~255 |
| Byte 11 | S10 hue | 0~255 |
| Byte 12 | S11 hue | 0~255 |
| Byte 13 | S12 hue | 0~255 |
| Byte 14 | S13 hue | 0~255 |
| Byte 15 | S14 hue | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="edu-22">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ② Hue + saturation: 7 points hue + sat</summary>

Mode number 22.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | (unused, sends 0) |  |
| Reflected light | S4 saturation | 0~100 |
| Raw red | S1 hue + S1 saturation×256 | Mod 256, ÷256 |
| Raw green | S2 hue + S2 saturation×256 | Mod 256, ÷256 |
| Raw blue | S3 hue + S3 saturation×256 | Mod 256, ÷256 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | S4 hue | 0~255 |
| Byte 9 | S5 hue | 0~255 |
| Byte 10 | S5 saturation | 0~100 |
| Byte 11 | S6 hue | 0~255 |
| Byte 12 | S6 saturation | 0~100 |
| Byte 13 | S7 hue | 0~255 |
| Byte 14 | S7 saturation | 0~100 |

</div>

</details>

<details class="plat pg-spike" id="edu-23">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ③ Hue + saturation + value: 4 points hue + sat + value</summary>

Mode number 23.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | (unused, sends 0) |  |
| Reflected light | S1 value | 0~100 |
| Raw red | S1 hue + S1 saturation×256 | Mod 256, ÷256 |
| Raw green | S2 hue + S2 saturation×256 | Mod 256, ÷256 |
| Raw blue | S3 hue + S3 saturation×256 | Mod 256, ÷256 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | S2 value | 0~100 |
| Byte 9 | S3 value | 0~100 |
| Byte 10 | S4 hue | 0~255 |
| Byte 11 | S4 saturation | 0~100 |
| Byte 12 | S4 value | 0~100 |

</div>

</details>

<details class="plat pg-spike" id="edu-30">
<summary><span class="plat-dot"></span>Camera mode 3 Color order: 13 blocks hue code</summary>

Mode number 30.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Total blocks | 0~18 |
| Reflected light | (unused, sends 0) |  |
| Raw red | Hue of block 1 from the left + hue of block 2 from the left×256 | Mod 256, ÷256 |
| Raw green | Hue of block 3 from the left + hue of block 4 from the left×256 | Mod 256, ÷256 |
| Raw blue | Hue of block 5 from the left + hue of block 6 from the left×256 | Mod 256, ÷256 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | Hue of block 7 from the left | 0~255 |
| Byte 9 | Hue of block 8 from the left | 0~255 |
| Byte 10 | Hue of block 9 from the left | 0~255 |
| Byte 11 | Hue of block 10 from the left | 0~255 |
| Byte 12 | Hue of block 11 from the left | 0~255 |
| Byte 13 | Hue of block 12 from the left | 0~255 |
| Byte 14 | Hue of block 13 from the left | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="edu-41">
<summary><span class="plat-dot"></span>Camera mode 4 Line following ①②③ black / white / colored line: 13 values</summary>

Mode number 41 / 42 / 43.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Junction type (0 none, 1 cross, 2 T, 3 left, 4 right, 5 dead end) | 0~5 |
| Reflected light | Near line width % | 0~100 |
| Raw red | Near offset (+100) | 0~200 |
| Raw green | Far offset (+100) | 0~200 |
| Raw blue | Line angle (+100) | 0~200 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | Middle offset (+100) | 0~200 |
| Byte 9 | Line-found flags (near 1 + middle 2 + far 4) | 0~7 |
| Byte 10 | Junction distance % | 0~100 |
| Byte 11 | Color number of the marker line crossed | 0, 1~16 |
| Byte 12 | Marker lines crossed so far | 0~99 |
| Byte 13 | Far line width % | 0~100 |
| Byte 14 | Near line left edge (+100) | 0~200 |
| Byte 15 | Near line right edge (+100) | 0~200 |

</div>

</details>

<details class="plat pg-spike" id="edu-51">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ① Object list: 7 objects</summary>

Mode number 51.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Object count | 0~8 |
| Reflected light | Object 1 code (color × 10 + shape) | 0, 10~89 |
| Raw red | Object 1 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Raw green | Object 2 code (color × 10 + shape) ×100 + object 2 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Raw blue | Object 3 code (color × 10 + shape) ×100 + object 3 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | Object 4 code (color × 10 + shape) | 0, 10~89 |
| Byte 9 | Object 4 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 10 | Object 5 code (color × 10 + shape) | 0, 10~89 |
| Byte 11 | Object 5 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 12 | Object 6 code (color × 10 + shape) | 0, 10~89 |
| Byte 13 | Object 6 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Byte 14 | Object 7 code (color × 10 + shape) | 0, 10~89 |
| Byte 15 | Object 7 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |

</div>

</details>

<details class="plat pg-spike" id="edu-52">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ② Object details: 2 objects + size, area, angle</summary>

Mode number 52.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Object 1 code (color × 10 + shape) | 0, 10~89 |
| Reflected light | Object 1 area (% of frame) | 0, 1~100 % |
| Raw red | Object 1 X (px) + object 1 Y (px, small at top)×256 | Mod 256, ÷256 |
| Raw green | Object 1 width (px) + object 1 height (px)×256 | Mod 256, ÷256 |
| Raw blue | Object 2 code (color × 10 + shape) + object 2 area (% of frame)×256 | Mod 256, ÷256 |
| **App Python device.data(): bytes 8~15** | | |
| Byte 8 | Object 1 angle | 0~179 |
| Byte 9 | Object 2 X (px) | 0, 1~159 |
| Byte 10 | Object 2 Y (px, small at top) | 0, 1~119 |
| Byte 11 | Object 2 width (px) | 0~160 |
| Byte 12 | Object 2 height (px) | 0~120 |
| Byte 13 | Object 2 angle | 0~179 |
| Byte 14 | Object 1 vertices | 0~10 |
| Byte 15 | Object count | 0~8 |

</div>

</details>

<a id="host-toy"></a>

## Robot Inventor App (home edition, teal hub)

The Robot Inventor App reads mode 0, 8 bytes in total; the camera switches to the home-edition layout by itself when the hub sends 0x25. "Color" only goes to 0~10, "reflected light" up to 126 and "raw red / green / blue" 0~255, one value per slot.

<details class="plat pg-spike" id="toy-11">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ① Max colors: 4 colors</summary>

Mode number 11.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Color 1 X (8 cells) | 0, 1~8 |
| Reflected light | Color 1 Y (6 cells, small at top) | 0, 1~6 |
| Raw red | Color 2 X (8 cells) ×10 + color 2 Y (6 cells, small at top) | Split ÷10 (same as Kito) |
| Raw green | Color 3 X (8 cells) ×10 + color 3 Y (6 cells, small at top) | Split ÷10 (same as Kito) |
| Raw blue | Color 4 X (8 cells) ×10 + color 4 Y (6 cells, small at top) | Split ÷10 (same as Kito) |

</div>

</details>

<details class="plat pg-spike" id="toy-12">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ② Multi-color: 4 colors</summary>

Mode number 12.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Color 1 X (8 cells) | 0, 1~8 |
| Reflected light | Color 1 Y (6 cells, small at top) | 0, 1~6 |
| Raw red | Color 2 X (8 cells) ×10 + color 2 Y (6 cells, small at top) | Split ÷10 (same as Kito) |
| Raw green | Color 3 X (8 cells) ×10 + color 3 Y (6 cells, small at top) | Split ÷10 (same as Kito) |
| Raw blue | Color 4 X (8 cells) ×10 + color 4 Y (6 cells, small at top) | Split ÷10 (same as Kito) |

</div>

</details>

<details class="plat pg-spike" id="toy-13">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ③ Standard (XY + area): 4 colors XY + area</summary>

Mode number 13.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Color 1 X (8 cells) | 0, 1~8 |
| Reflected light | Color 1 Y (6 cells, small at top) | 0, 1~6 |
| Raw red | Color 2 X (8 cells) ×10 + color 2 Y (6 cells, small at top) | Split ÷10 (same as Kito) |
| Raw green | Color 3 X (8 cells) ×10 + color 3 Y (6 cells, small at top) | Split ÷10 (same as Kito) |
| Raw blue | Color 4 X (8 cells) ×10 + color 4 Y (6 cells, small at top) | Split ÷10 (same as Kito) |

</div>

</details>

<details class="plat pg-spike" id="toy-14">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ④ Fine (XYWH): 2 colors XYWH</summary>

Mode number 14.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Color 1 X (8 cells) | 0, 1~8 |
| Reflected light | Color 1 Y (6 cells, small at top) | 0, 1~6 |
| Raw red | Color 2 X (8 cells) ×10 + color 2 Y (6 cells, small at top) | Split ÷10 (same as Kito) |
| Raw green | Color 1 width (8 cells) ×10 + color 1 height (6 cells) | Split ÷10 (same as Kito) |
| Raw blue | Color 2 width (8 cells) ×10 + color 2 height (6 cells) | Split ÷10 (same as Kito) |

</div>

</details>

<details class="plat pg-spike" id="toy-21">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ① Hue: 4 points hue</summary>

Mode number 21.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | (unused, sends 0) |  |
| Reflected light | S4 hue (÷2) | 0~127 |
| Raw red | S1 hue | 0~255 |
| Raw green | S2 hue | 0~255 |
| Raw blue | S3 hue | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="toy-22">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ② Hue + saturation: 2 points hue + sat</summary>

Mode number 22.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | (unused, sends 0) |  |
| Reflected light | S2 saturation | 0~100 |
| Raw red | S1 hue | 0~255 |
| Raw green | S1 saturation | 0~100 |
| Raw blue | S2 hue | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="toy-23">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ③ Hue + saturation + value: 2 points hue + sat/value</summary>

Mode number 23.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | (unused, sends 0) |  |
| Reflected light | S2 sat÷10 ×10 + value÷10 | 0~99 |
| Raw red | S1 hue | 0~255 |
| Raw green | S1 sat÷10 ×10 + value÷10 | 0~99 |
| Raw blue | S2 hue | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="toy-30">
<summary><span class="plat-dot"></span>Camera mode 3 Color order: 3 blocks hue code</summary>

Mode number 30.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | (unused, sends 0) |  |
| Reflected light | Total blocks | 0~18 |
| Raw red | Hue of block 1 from the left | 0~255 |
| Raw green | Hue of block 2 from the left | 0~255 |
| Raw blue | Hue of block 3 from the left | 0~255 |

</div>

</details>

<details class="plat pg-spike" id="toy-41">
<summary><span class="plat-dot"></span>Camera mode 4 Line following ①②③ black / white / colored line: 5 values</summary>

Mode number 41 / 42 / 43.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Junction type (0 none, 1 cross, 2 T, 3 left, 4 right, 5 dead end) | 0~5 |
| Reflected light | Near line width % | 0~100 |
| Raw red | Near offset (+100) | 0~200 |
| Raw green | Far offset (+100) | 0~200 |
| Raw blue | Line angle (+100) | 0~200 |

</div>

</details>

<details class="plat pg-spike" id="toy-51">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ① Object list: 2 objects</summary>

Mode number 51.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Object count | 0~8 |
| Reflected light | Object 1 code (color × 10 + shape) | 0, 10~89 |
| Raw red | Object 1 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |
| Raw green | Object 2 code (color × 10 + shape) | 0, 10~89 |
| Raw blue | Object 2 X×10 + Y (8×6 grid, small at top) | 0, 11~86 |

</div>

</details>

<details class="plat pg-spike" id="toy-52">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ② Object details: 1 object + area</summary>

Mode number 52.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Stock blocks (mode 0)** | | |
| Color | Object count | 0~8 |
| Reflected light | Object 1 code (color × 10 + shape) | 0, 10~89 |
| Raw red | Object 1 X (px) | 0, 1~159 |
| Raw green | Object 1 Y (px, small at top) | 0, 1~119 |
| Raw blue | Object 1 area (% of frame) | 0, 1~100 % |

</div>

</details>

<a id="host-pyblk"></a>

## Pybricks built-in blocks (no library)

No MBC library, only the Pybricks built-in color sensor blocks: reflection() (mode 5) and ambient() (mode 7). Color tracking works like the Kito edition: reflection() = color 1 as X×100+Y, ambient() = color 2 as X.Y (40×30 grid). reflection() above 3199 shows up negative; add 6399 to fix it.

<details class="plat pg-spike" id="pyblk-11">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ① Max colors: 2 colors</summary>

Mode number 11.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | Color 1 X (40 cells) ×100 + color 1 Y (30 cells, small at top) | Split ÷100 (same as Kito) |
| **Mode 7 ambient()** | | |
| ambient() | Color 2 X (40 cells) . color 2 Y (30 cells, small at top) | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-12">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ② Multi-color: 2 colors</summary>

Mode number 12.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | Color 1 X (40 cells) ×100 + color 1 Y (30 cells, small at top) | Split ÷100 (same as Kito) |
| **Mode 7 ambient()** | | |
| ambient() | Color 2 X (40 cells) . color 2 Y (30 cells, small at top) | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-13">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ③ Standard (XY + area): 2 colors XY</summary>

Mode number 13.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | Color 1 X (40 cells) ×100 + color 1 Y (30 cells, small at top) | Split ÷100 (same as Kito) |
| **Mode 7 ambient()** | | |
| ambient() | Color 2 X (40 cells) . color 2 Y (30 cells, small at top) | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-14">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ④ Fine (XYWH): 1 color XYWH</summary>

Mode number 14.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | Color 1 X (40 cells) ×100 + color 1 Y (30 cells, small at top) | Split ÷100 (same as Kito) |
| **Mode 7 ambient()** | | |
| ambient() | Color 1 width (40 cells) . color 1 height (30 cells) | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-21">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ① Hue: 2 points hue</summary>

Mode number 21.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | S1 hue ×10 + S1 saturation÷10 | Split ÷10 |
| **Mode 7 ambient()** | | |
| ambient() | S2 hue . S2 saturation | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-22">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ② Hue + saturation: 2 points hue + sat</summary>

Mode number 22.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | S1 hue ×10 + S1 saturation÷10 | Split ÷10 |
| **Mode 7 ambient()** | | |
| ambient() | S2 hue . S2 saturation | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-23">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ③ Hue + saturation + value: 1 point(s) raw hue + sat + value</summary>

Mode number 23.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | S1 hue | 0~255 |
| **Mode 7 ambient()** | | |
| ambient() | S1 saturation . S1 value | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-30">
<summary><span class="plat-dot"></span>Camera mode 3 Color order: 2 blocks hue code</summary>

Mode number 30.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | Hue of block 1 from the left | 0~255 |
| **Mode 7 ambient()** | | |
| ambient() | Hue of block 2 from the left . total blocks | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-41">
<summary><span class="plat-dot"></span>Camera mode 4 Line following ①②③ black / white / colored line: 4 values</summary>

Mode number 41 / 42 / 43.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | Near offset (+100) ×10 + junction type (0 none, 1 cross, 2 T, 3 left, 4 right, 5 dead end) | Split ÷10 (same as Kito) |
| **Mode 7 ambient()** | | |
| ambient() | Far offset (+100) . line-found flags (near 1 + middle 2 + far 4) | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-51">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ① Object list: 1 object</summary>

Mode number 51.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | Object 1 X (40 cells) ×100 + object 1 Y (30 cells, small at top) | Split ÷100 (same as Kito) |
| **Mode 7 ambient()** | | |
| ambient() | Object 1 code (color × 10 + shape) . object count | A.BB |

</div>

</details>

<details class="plat pg-spike" id="pyblk-52">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ② Object details: 1 object + area</summary>

Mode number 52.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 5 reflection()** | | |
| reflection() | Object 1 X (40 cells) ×100 + object 1 Y (30 cells, small at top) | Split ÷100 (same as Kito) |
| **Mode 7 ambient()** | | |
| ambient() | Object 1 code (color × 10 + shape) . object 1 area (% of frame) | A.BB |

</div>

</details>

<a id="host-ev3"></a>

## EV3 (EV3-G / EV3 Classroom / EV3 MicroPython)

On the EV3 the camera shows up as a stock Infrared Sensor (type 33). Mode 0 proximity = camera mode number, mode 1 beacon = 8 values, mode 2 remote = 4 values, each an int8 (−127~100). Sub-option ①, Color order and Line following also use the remote mode (mode 2); the other levels only need the beacon mode. Switching modes takes about 0.5 s. Positions use an 8×6 grid (X×10+Y, Y: small at the bottom). Mode 5 = device ID.

<details class="plat pg-ev3" id="ev3-11">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ① Max colors: 12 colors</summary>

Mode number 11.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | Color 1 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 1 distance | Color 2 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 2 heading | Color 3 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 2 distance | Color 4 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 heading | Color 5 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 distance | Color 6 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 4 heading | Color 7 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 4 distance | Color 8 X×10 + Y (8×6 grid) | 0, 11~86 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | Color 9 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 2 | Color 10 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 | Color 11 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 4 | Color 12 X×10 + Y (8×6 grid) | 0, 11~86 |

</div>

</details>

<details class="plat pg-ev3" id="ev3-12">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ② Multi-color: 8 colors</summary>

Mode number 12.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | Color 1 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 1 distance | Color 2 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 2 heading | Color 3 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 2 distance | Color 4 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 heading | Color 5 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 distance | Color 6 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 4 heading | Color 7 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 4 distance | Color 8 X×10 + Y (8×6 grid) | 0, 11~86 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | (unused, sends 0) |  |
| Channel 2 | (unused, sends 0) |  |
| Channel 3 | (unused, sends 0) |  |
| Channel 4 | (unused, sends 0) |  |

</div>

</details>

<details class="plat pg-ev3" id="ev3-13">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ③ Standard (XY + area): 4 colors XY + area</summary>

Mode number 13.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | Color 1 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 1 distance | Color 1 area (% of frame) | 0, 1~100 % |
| Channel 2 heading | Color 2 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 2 distance | Color 2 area (% of frame) | 0, 1~100 % |
| Channel 3 heading | Color 3 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 distance | Color 3 area (% of frame) | 0, 1~100 % |
| Channel 4 heading | Color 4 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 4 distance | Color 4 area (% of frame) | 0, 1~100 % |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | (unused, sends 0) |  |
| Channel 2 | (unused, sends 0) |  |
| Channel 3 | (unused, sends 0) |  |
| Channel 4 | (unused, sends 0) |  |

</div>

</details>

<details class="plat pg-ev3" id="ev3-14">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ④ Fine (XYWH): 2 colors XYWH</summary>

Mode number 14.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | Color 1 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 1 distance | Color 1 W×10 + H (8×6 grid) | 0, 11~86 |
| Channel 2 heading | Color 1 count | 0~15 |
| Channel 2 distance | Color 1 area (% of frame) | 0, 1~100 % |
| Channel 3 heading | Color 2 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 distance | Color 2 W×10 + H (8×6 grid) | 0, 11~86 |
| Channel 4 heading | Color 2 count | 0~15 |
| Channel 4 distance | Color 2 area (% of frame) | 0, 1~100 % |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | (unused, sends 0) |  |
| Channel 2 | (unused, sends 0) |  |
| Channel 3 | (unused, sends 0) |  |
| Channel 4 | (unused, sends 0) |  |

</div>

</details>

<details class="plat pg-ev3" id="ev3-21">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ① Hue: 12 points hue</summary>

Mode number 21.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | S1 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 1 distance | S2 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 2 heading | S3 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 2 distance | S4 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 3 heading | S5 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 3 distance | S6 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 4 heading | S7 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 4 distance | S8 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | S9 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 2 | S10 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 3 | S11 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 4 | S12 hue (EV3: positive = hue, negative = black / white) | −15~100 |

</div>

</details>

<details class="plat pg-ev3" id="ev3-22">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ② Hue + saturation: 4 points hue + sat/value</summary>

Mode number 22.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | S1 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 1 distance | S1 sat÷10 ×10 + value÷10 | 0~99 |
| Channel 2 heading | S2 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 2 distance | S2 sat÷10 ×10 + value÷10 | 0~99 |
| Channel 3 heading | S3 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 3 distance | S3 sat÷10 ×10 + value÷10 | 0~99 |
| Channel 4 heading | S4 hue (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 4 distance | S4 sat÷10 ×10 + value÷10 | 0~99 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | (unused, sends 0) |  |
| Channel 2 | (unused, sends 0) |  |
| Channel 3 | (unused, sends 0) |  |
| Channel 4 | (unused, sends 0) |  |

</div>

</details>

<details class="plat pg-ev3" id="ev3-23">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ③ Hue + saturation + value: 2 point(s) raw hue + sat + value</summary>

Mode number 23.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | S1 hue code ÷100 (hundreds) | 0~2 |
| Channel 1 distance | S1 hue code mod 100 | 0~99 |
| Channel 2 heading | S1 saturation | 0~100 |
| Channel 2 distance | S1 value | 0~100 |
| Channel 3 heading | S2 hue code ÷100 (hundreds) | 0~2 |
| Channel 3 distance | S2 hue code mod 100 | 0~99 |
| Channel 4 heading | S2 saturation | 0~100 |
| Channel 4 distance | S2 value | 0~100 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | (unused, sends 0) |  |
| Channel 2 | (unused, sends 0) |  |
| Channel 3 | (unused, sends 0) |  |
| Channel 4 | (unused, sends 0) |  |

</div>

</details>

<details class="plat pg-ev3" id="ev3-30">
<summary><span class="plat-dot"></span>Camera mode 3 Color order: 11 blocks hue code</summary>

Mode number 30.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | Hue of block 1 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 1 distance | Hue of block 2 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 2 heading | Hue of block 3 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 2 distance | Hue of block 4 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 3 heading | Hue of block 5 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 3 distance | Hue of block 6 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 4 heading | Hue of block 7 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 4 distance | Hue of block 8 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | Hue of block 9 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 2 | Hue of block 10 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 3 | Hue of block 11 from the left (EV3: positive = hue, negative = black / white) | −15~100 |
| Channel 4 | Total blocks | 0~18 |

</div>

</details>

<details class="plat pg-ev3" id="ev3-41">
<summary><span class="plat-dot"></span>Camera mode 4 Line following ①②③ black / white / colored line: 12 values</summary>

Mode number 41 / 42 / 43.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | Near offset | −100~100 |
| Channel 1 distance | Middle offset | −100~100 |
| Channel 2 heading | Far offset | −100~100 |
| Channel 2 distance | Line angle | −90~90 |
| Channel 3 heading | Near line width % | 0~100 |
| Channel 3 distance | Junction type×10 + line-found flags | 0~57 |
| Channel 4 heading | Junction distance % | 0~100 |
| Channel 4 distance | Color number of the marker line crossed | 0, 1~16 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | Near line left edge | −100~100 |
| Channel 2 | Near line right edge | −100~100 |
| Channel 3 | Far line width % | 0~100 |
| Channel 4 | Marker lines crossed so far | 0~99 |

</div>

</details>

<details class="plat pg-ev3" id="ev3-51">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ① Object list: 5 objects</summary>

Mode number 51.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | Object 1 code (color × 10 + shape) | 0, 10~89 |
| Channel 1 distance | Object 1 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 2 heading | Object 2 code (color × 10 + shape) | 0, 10~89 |
| Channel 2 distance | Object 2 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 heading | Object 3 code (color × 10 + shape) | 0, 10~89 |
| Channel 3 distance | Object 3 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 4 heading | Object 4 code (color × 10 + shape) | 0, 10~89 |
| Channel 4 distance | Object 4 X×10 + Y (8×6 grid) | 0, 11~86 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | Object 5 code (color × 10 + shape) | 0, 10~89 |
| Channel 2 | Object 5 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 3 | (unused, sends 0) |  |
| Channel 4 | Object count | 0~8 |

</div>

</details>

<details class="plat pg-ev3" id="ev3-52">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ② Object details: 2 objects + area, angle</summary>

Mode number 52.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Mode 0 proximity (1 value)** | | |
| Value | Camera mode number (e.g. 13 = Color tracking ③ Standard, 51 = Object detection ① list) | 11~52 |
| **Mode 1 beacon (8 values)** | | |
| Channel 1 heading | Object 1 code (color × 10 + shape) | 0, 10~89 |
| Channel 1 distance | Object 1 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 2 heading | Object 1 area (% of frame) | 0, 1~100 % |
| Channel 2 distance | Object 1 angle ((angle + 1) ÷ 2) | 0~90 |
| Channel 3 heading | Object 2 code (color × 10 + shape) | 0, 10~89 |
| Channel 3 distance | Object 2 X×10 + Y (8×6 grid) | 0, 11~86 |
| Channel 4 heading | Object 2 area (% of frame) | 0, 1~100 % |
| Channel 4 distance | Object 2 angle ((angle + 1) ÷ 2) | 0~90 |
| **Mode 2 remote (4 values)** | | |
| Channel 1 | (unused, sends 0) |  |
| Channel 2 | (unused, sends 0) |  |
| Channel 3 | (unused, sends 0) |  |
| Channel 4 | (unused, sends 0) |  |

</div>

</details>

<a id="host-mx"></a>

## MATRIX Mini R4 (mVision, 10 slots)

The MATRIX Mini R4 reads the camera with the MATRIXblock mVision blocks: 10 uint16 slots ("slot 1" below = "data 0" in the block). A slot holds two byte values (A + B×256: mod 256, ÷256) or one pixel coordinate; coordinates are 160×120 pixels (Y: small at top), 0 = no target; values that can be negative are sent +100.

<details class="plat pg-matrix" id="mx-11">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ① Max colors: 16 colors</summary>

Mode number 11.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | Color 1 X×10 + Y (8×6 grid, small at top) ×100 + color 2 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Slot 2 | Color 3 X×10 + Y (8×6 grid, small at top) ×100 + color 4 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Slot 3 | Color 5 X×10 + Y (8×6 grid, small at top) ×100 + color 6 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Slot 4 | Color 7 X×10 + Y (8×6 grid, small at top) ×100 + color 8 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Slot 5 | Color 9 X×10 + Y (8×6 grid, small at top) ×100 + color 10 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Slot 6 | Color 11 X×10 + Y (8×6 grid, small at top) ×100 + color 12 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Slot 7 | Color 13 X×10 + Y (8×6 grid, small at top) ×100 + color 14 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Slot 8 | Color 15 X×10 + Y (8×6 grid, small at top) ×100 + color 16 X×10 + Y (8×6 grid, small at top) | Split ÷100 |
| Slot 9 | Colors found | 0~16 |
| Slot 10 | (unused, sends 0) |  |

</div>

</details>

<details class="plat pg-matrix" id="mx-12">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ② Multi-color: 10 colors</summary>

Mode number 12.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | Color 1 X (px) + color 1 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 2 | Color 2 X (px) + color 2 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 3 | Color 3 X (px) + color 3 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 4 | Color 4 X (px) + color 4 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 5 | Color 5 X (px) + color 5 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 6 | Color 6 X (px) + color 6 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 7 | Color 7 X (px) + color 7 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 8 | Color 8 X (px) + color 8 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 9 | Color 9 X (px) + color 9 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 10 | Color 10 X (px) + color 10 Y (px, small at top)×256 | Mod 256, ÷256 |

</div>

</details>

<details class="plat pg-matrix" id="mx-13">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ③ Standard (XY + area): 5 colors XY + area</summary>

Mode number 13.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | Color 1 X (px) + color 1 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 2 | Color 1 area (% of frame) + color 1 count×256 | Mod 256, ÷256 |
| Slot 3 | Color 2 X (px) + color 2 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 4 | Color 2 area (% of frame) + color 2 count×256 | Mod 256, ÷256 |
| Slot 5 | Color 3 X (px) + color 3 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 6 | Color 3 area (% of frame) + color 3 count×256 | Mod 256, ÷256 |
| Slot 7 | Color 4 X (px) + color 4 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 8 | Color 4 area (% of frame) + color 4 count×256 | Mod 256, ÷256 |
| Slot 9 | Color 5 X (px) + color 5 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 10 | Color 5 area (% of frame) + color 5 count×256 | Mod 256, ÷256 |

</div>

</details>

<details class="plat pg-matrix" id="mx-14">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ④ Fine (XYWH): 3 colors XYWH</summary>

Mode number 14.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | Color 1 X (px) + color 1 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 2 | Color 1 width (px) + color 1 height (px)×256 | Mod 256, ÷256 |
| Slot 3 | Color 1 area (% of frame) + color 1 count×256 | Mod 256, ÷256 |
| Slot 4 | Color 2 X (px) + color 2 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 5 | Color 2 width (px) + color 2 height (px)×256 | Mod 256, ÷256 |
| Slot 6 | Color 2 area (% of frame) + color 2 count×256 | Mod 256, ÷256 |
| Slot 7 | Color 3 X (px) + color 3 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 8 | Color 3 width (px) + color 3 height (px)×256 | Mod 256, ÷256 |
| Slot 9 | Color 3 area (% of frame) + color 3 count×256 | Mod 256, ÷256 |
| Slot 10 | Colors found | 0~16 |

</div>

</details>

<details class="plat pg-matrix" id="mx-21">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ① Hue: 20 points hue</summary>

Mode number 21.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | S1 hue + S2 hue×256 | Mod 256, ÷256 |
| Slot 2 | S3 hue + S4 hue×256 | Mod 256, ÷256 |
| Slot 3 | S5 hue + S6 hue×256 | Mod 256, ÷256 |
| Slot 4 | S7 hue + S8 hue×256 | Mod 256, ÷256 |
| Slot 5 | S9 hue + S10 hue×256 | Mod 256, ÷256 |
| Slot 6 | S11 hue + S12 hue×256 | Mod 256, ÷256 |
| Slot 7 | S13 hue + S14 hue×256 | Mod 256, ÷256 |
| Slot 8 | S15 hue + S16 hue×256 | Mod 256, ÷256 |
| Slot 9 | S17 hue + S18 hue×256 | Mod 256, ÷256 |
| Slot 10 | S19 hue + S20 hue×256 | Mod 256, ÷256 |

</div>

</details>

<details class="plat pg-matrix" id="mx-22">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ② Hue + saturation: 10 points hue + sat</summary>

Mode number 22.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | S1 hue + S1 saturation×256 | Mod 256, ÷256 |
| Slot 2 | S2 hue + S2 saturation×256 | Mod 256, ÷256 |
| Slot 3 | S3 hue + S3 saturation×256 | Mod 256, ÷256 |
| Slot 4 | S4 hue + S4 saturation×256 | Mod 256, ÷256 |
| Slot 5 | S5 hue + S5 saturation×256 | Mod 256, ÷256 |
| Slot 6 | S6 hue + S6 saturation×256 | Mod 256, ÷256 |
| Slot 7 | S7 hue + S7 saturation×256 | Mod 256, ÷256 |
| Slot 8 | S8 hue + S8 saturation×256 | Mod 256, ÷256 |
| Slot 9 | S9 hue + S9 saturation×256 | Mod 256, ÷256 |
| Slot 10 | S10 hue + S10 saturation×256 | Mod 256, ÷256 |

</div>

</details>

<details class="plat pg-matrix" id="mx-23">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ③ Hue + saturation + value: 6 points hue + sat + value</summary>

Mode number 23.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | S1 hue + S1 saturation×256 | Mod 256, ÷256 |
| Slot 2 | S1 value + S2 hue×256 | Mod 256, ÷256 |
| Slot 3 | S2 saturation + S2 value×256 | Mod 256, ÷256 |
| Slot 4 | S3 hue + S3 saturation×256 | Mod 256, ÷256 |
| Slot 5 | S3 value + S4 hue×256 | Mod 256, ÷256 |
| Slot 6 | S4 saturation + S4 value×256 | Mod 256, ÷256 |
| Slot 7 | S5 hue + S5 saturation×256 | Mod 256, ÷256 |
| Slot 8 | S5 value + S6 hue×256 | Mod 256, ÷256 |
| Slot 9 | S6 saturation + S6 value×256 | Mod 256, ÷256 |
| Slot 10 | (unused, sends 0) |  |

</div>

</details>

<details class="plat pg-matrix" id="mx-30">
<summary><span class="plat-dot"></span>Camera mode 3 Color order: 18 blocks hue code</summary>

Mode number 30.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | Hue of block 1 from the left + hue of block 2 from the left×256 | Mod 256, ÷256 |
| Slot 2 | Hue of block 3 from the left + hue of block 4 from the left×256 | Mod 256, ÷256 |
| Slot 3 | Hue of block 5 from the left + hue of block 6 from the left×256 | Mod 256, ÷256 |
| Slot 4 | Hue of block 7 from the left + hue of block 8 from the left×256 | Mod 256, ÷256 |
| Slot 5 | Hue of block 9 from the left + hue of block 10 from the left×256 | Mod 256, ÷256 |
| Slot 6 | Hue of block 11 from the left + hue of block 12 from the left×256 | Mod 256, ÷256 |
| Slot 7 | Hue of block 13 from the left + hue of block 14 from the left×256 | Mod 256, ÷256 |
| Slot 8 | Hue of block 15 from the left + hue of block 16 from the left×256 | Mod 256, ÷256 |
| Slot 9 | Hue of block 17 from the left + hue of block 18 from the left×256 | Mod 256, ÷256 |
| Slot 10 | Total blocks | 0~18 |

</div>

</details>

<details class="plat pg-matrix" id="mx-41">
<summary><span class="plat-dot"></span>Camera mode 4 Line following ①②③ black / white / colored line: 14 values</summary>

Mode number 41 / 42 / 43.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | Near offset (+100) | 0~200 |
| Slot 2 | Middle offset (+100) | 0~200 |
| Slot 3 | Far offset (+100) | 0~200 |
| Slot 4 | Line angle (+100) | 0~200 |
| Slot 5 | Junction type (0 none, 1 cross, 2 T, 3 left, 4 right, 5 dead end) | 0~5 |
| Slot 6 | Line-found flags (near 1 + middle 2 + far 4) | 0~7 |
| Slot 7 | Near line width % + junction distance %×256 | Mod 256, ÷256 |
| Slot 8 | Near line left edge (+100) + near line right edge (+100)×256 | Mod 256, ÷256 |
| Slot 9 | Far line width % + color number of the marker line crossed×256 | Mod 256, ÷256 |
| Slot 10 | Marker lines crossed so far + side the line was last lost (0 on the line, 1 left, 2 right)×256 | Mod 256, ÷256 |

</div>

</details>

<details class="plat pg-matrix" id="mx-51">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ① Object list: 4 objects</summary>

Mode number 51.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | Object count | 0~8 |
| Slot 2 | Object 1 code (color × 10 + shape) + object 1 area (% of frame)×256 | Mod 256, ÷256 |
| Slot 3 | Object 1 X (px) + object 1 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 4 | Object 2 code (color × 10 + shape) + object 2 area (% of frame)×256 | Mod 256, ÷256 |
| Slot 5 | Object 2 X (px) + object 2 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 6 | Object 3 code (color × 10 + shape) + object 3 area (% of frame)×256 | Mod 256, ÷256 |
| Slot 7 | Object 3 X (px) + object 3 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 8 | Object 4 code (color × 10 + shape) + object 4 area (% of frame)×256 | Mod 256, ÷256 |
| Slot 9 | Object 4 X (px) + object 4 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 10 | (unused, sends 0) |  |

</div>

</details>

<details class="plat pg-matrix" id="mx-52">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ② Object details: 3 objects + size, area</summary>

Mode number 52.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **mVision, 10 slots** | | |
| Slot 1 | Object count | 0~8 |
| Slot 2 | Object 1 code (color × 10 + shape) + object 1 area (% of frame)×256 | Mod 256, ÷256 |
| Slot 3 | Object 1 X (px) + object 1 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 4 | Object 1 width (px) + object 1 height (px)×256 | Mod 256, ÷256 |
| Slot 5 | Object 2 code (color × 10 + shape) + object 2 area (% of frame)×256 | Mod 256, ÷256 |
| Slot 6 | Object 2 X (px) + object 2 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 7 | Object 2 width (px) + object 2 height (px)×256 | Mod 256, ÷256 |
| Slot 8 | Object 3 code (color × 10 + shape) + object 3 area (% of frame)×256 | Mod 256, ÷256 |
| Slot 9 | Object 3 X (px) + object 3 Y (px, small at top)×256 | Mod 256, ÷256 |
| Slot 10 | Object 3 width (px) + object 3 height (px)×256 | Mod 256, ÷256 |

</div>

</details>

<a id="host-i2c"></a>

## I2C 0x4C (Arduino / micro:bit)

I2C address 0x4C (Arduino, ESP32, UNO R4, micro:bit). Registers are read in bursts with no slot limit: write the value number → wait 2 ms → read 2×k bytes (int16, little-endian), up to 16 values per read. "Value 1" below = register 0x10, "value 2" = 0x11 and so on. Coordinates are 160×120 pixels (Y: small at top), 0 = no target; negative values are sent as they are. Every level gives 16 colors / 16 points; the level only decides which values. mBlock (mBot and other Makeblock boards) reads the same value table over UART 38400: send 1 byte [id] → reply [low][high][id^low^high].

<details class="plat pg-arduino" id="i2c-11">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ① Max colors: 16 colors XY</summary>

Mode number 11.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | Color 1 X (px) | 0, 1~159 |
| Value 2 · `0x11` | Color 1 Y (px, small at top) | 0, 1~119 |
| Value 3 · `0x12` | Color 2 X (px) | 0, 1~159 |
| Value 4 · `0x13` | Color 2 Y (px, small at top) | 0, 1~119 |
| Value 5 · `0x14` | Color 3 X (px) | 0, 1~159 |
| Value 6 · `0x15` | Color 3 Y (px, small at top) | 0, 1~119 |
| Value 7 · `0x16` | Color 4 X (px) | 0, 1~159 |
| Value 8 · `0x17` | Color 4 Y (px, small at top) | 0, 1~119 |
| Value 9 · `0x18` | Color 5 X (px) | 0, 1~159 |
| Value 10 · `0x19` | Color 5 Y (px, small at top) | 0, 1~119 |
| Value 11 · `0x1A` | Color 6 X (px) | 0, 1~159 |
| Value 12 · `0x1B` | Color 6 Y (px, small at top) | 0, 1~119 |
| Value 13 · `0x1C` | Color 7 X (px) | 0, 1~159 |
| Value 14 · `0x1D` | Color 7 Y (px, small at top) | 0, 1~119 |
| Value 15 · `0x1E` | Color 8 X (px) | 0, 1~159 |
| Value 16 · `0x1F` | Color 8 Y (px, small at top) | 0, 1~119 |
| Value 17 · `0x20` | Color 9 X (px) | 0, 1~159 |
| Value 18 · `0x21` | Color 9 Y (px, small at top) | 0, 1~119 |
| Value 19 · `0x22` | Color 10 X (px) | 0, 1~159 |
| Value 20 · `0x23` | Color 10 Y (px, small at top) | 0, 1~119 |
| Value 21 · `0x24` | Color 11 X (px) | 0, 1~159 |
| Value 22 · `0x25` | Color 11 Y (px, small at top) | 0, 1~119 |
| Value 23 · `0x26` | Color 12 X (px) | 0, 1~159 |
| Value 24 · `0x27` | Color 12 Y (px, small at top) | 0, 1~119 |
| Value 25 · `0x28` | Color 13 X (px) | 0, 1~159 |
| Value 26 · `0x29` | Color 13 Y (px, small at top) | 0, 1~119 |
| Value 27 · `0x2A` | Color 14 X (px) | 0, 1~159 |
| Value 28 · `0x2B` | Color 14 Y (px, small at top) | 0, 1~119 |
| Value 29 · `0x2C` | Color 15 X (px) | 0, 1~159 |
| Value 30 · `0x2D` | Color 15 Y (px, small at top) | 0, 1~119 |
| Value 31 · `0x2E` | Color 16 X (px) | 0, 1~159 |
| Value 32 · `0x2F` | Color 16 Y (px, small at top) | 0, 1~119 |
| Value 33 · `0x30` | Colors found | 0~16 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-12">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ② Multi-color: 16 colors XY + count</summary>

Mode number 12.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | Color 1 X (px) | 0, 1~159 |
| Value 2 · `0x11` | Color 1 Y (px, small at top) | 0, 1~119 |
| Value 3 · `0x12` | Color 1 count | 0~15 |
| Value 4 · `0x13` | Color 2 X (px) | 0, 1~159 |
| Value 5 · `0x14` | Color 2 Y (px, small at top) | 0, 1~119 |
| Value 6 · `0x15` | Color 2 count | 0~15 |
| Value 7 · `0x16` | Color 3 X (px) | 0, 1~159 |
| Value 8 · `0x17` | Color 3 Y (px, small at top) | 0, 1~119 |
| Value 9 · `0x18` | Color 3 count | 0~15 |
| Value 10 · `0x19` | Color 4 X (px) | 0, 1~159 |
| Value 11 · `0x1A` | Color 4 Y (px, small at top) | 0, 1~119 |
| Value 12 · `0x1B` | Color 4 count | 0~15 |
| Value 13 · `0x1C` | Color 5 X (px) | 0, 1~159 |
| Value 14 · `0x1D` | Color 5 Y (px, small at top) | 0, 1~119 |
| Value 15 · `0x1E` | Color 5 count | 0~15 |
| Value 16 · `0x1F` | Color 6 X (px) | 0, 1~159 |
| Value 17 · `0x20` | Color 6 Y (px, small at top) | 0, 1~119 |
| Value 18 · `0x21` | Color 6 count | 0~15 |
| Value 19 · `0x22` | Color 7 X (px) | 0, 1~159 |
| Value 20 · `0x23` | Color 7 Y (px, small at top) | 0, 1~119 |
| Value 21 · `0x24` | Color 7 count | 0~15 |
| Value 22 · `0x25` | Color 8 X (px) | 0, 1~159 |
| Value 23 · `0x26` | Color 8 Y (px, small at top) | 0, 1~119 |
| Value 24 · `0x27` | Color 8 count | 0~15 |
| Value 25 · `0x28` | Color 9 X (px) | 0, 1~159 |
| Value 26 · `0x29` | Color 9 Y (px, small at top) | 0, 1~119 |
| Value 27 · `0x2A` | Color 9 count | 0~15 |
| Value 28 · `0x2B` | Color 10 X (px) | 0, 1~159 |
| Value 29 · `0x2C` | Color 10 Y (px, small at top) | 0, 1~119 |
| Value 30 · `0x2D` | Color 10 count | 0~15 |
| Value 31 · `0x2E` | Color 11 X (px) | 0, 1~159 |
| Value 32 · `0x2F` | Color 11 Y (px, small at top) | 0, 1~119 |
| Value 33 · `0x30` | Color 11 count | 0~15 |
| Value 34 · `0x31` | Color 12 X (px) | 0, 1~159 |
| Value 35 · `0x32` | Color 12 Y (px, small at top) | 0, 1~119 |
| Value 36 · `0x33` | Color 12 count | 0~15 |
| Value 37 · `0x34` | Color 13 X (px) | 0, 1~159 |
| Value 38 · `0x35` | Color 13 Y (px, small at top) | 0, 1~119 |
| Value 39 · `0x36` | Color 13 count | 0~15 |
| Value 40 · `0x37` | Color 14 X (px) | 0, 1~159 |
| Value 41 · `0x38` | Color 14 Y (px, small at top) | 0, 1~119 |
| Value 42 · `0x39` | Color 14 count | 0~15 |
| Value 43 · `0x3A` | Color 15 X (px) | 0, 1~159 |
| Value 44 · `0x3B` | Color 15 Y (px, small at top) | 0, 1~119 |
| Value 45 · `0x3C` | Color 15 count | 0~15 |
| Value 46 · `0x3D` | Color 16 X (px) | 0, 1~159 |
| Value 47 · `0x3E` | Color 16 Y (px, small at top) | 0, 1~119 |
| Value 48 · `0x3F` | Color 16 count | 0~15 |
| Value 49 · `0x40` | Colors found | 0~16 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-13">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ③ Standard (XY + area): 16 colors XY + area + count</summary>

Mode number 13.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | Color 1 X (px) | 0, 1~159 |
| Value 2 · `0x11` | Color 1 Y (px, small at top) | 0, 1~119 |
| Value 3 · `0x12` | Color 1 area (% of frame) | 0, 1~100 % |
| Value 4 · `0x13` | Color 1 count | 0~15 |
| Value 5 · `0x14` | Color 2 X (px) | 0, 1~159 |
| Value 6 · `0x15` | Color 2 Y (px, small at top) | 0, 1~119 |
| Value 7 · `0x16` | Color 2 area (% of frame) | 0, 1~100 % |
| Value 8 · `0x17` | Color 2 count | 0~15 |
| Value 9 · `0x18` | Color 3 X (px) | 0, 1~159 |
| Value 10 · `0x19` | Color 3 Y (px, small at top) | 0, 1~119 |
| Value 11 · `0x1A` | Color 3 area (% of frame) | 0, 1~100 % |
| Value 12 · `0x1B` | Color 3 count | 0~15 |
| Value 13 · `0x1C` | Color 4 X (px) | 0, 1~159 |
| Value 14 · `0x1D` | Color 4 Y (px, small at top) | 0, 1~119 |
| Value 15 · `0x1E` | Color 4 area (% of frame) | 0, 1~100 % |
| Value 16 · `0x1F` | Color 4 count | 0~15 |
| Value 17 · `0x20` | Color 5 X (px) | 0, 1~159 |
| Value 18 · `0x21` | Color 5 Y (px, small at top) | 0, 1~119 |
| Value 19 · `0x22` | Color 5 area (% of frame) | 0, 1~100 % |
| Value 20 · `0x23` | Color 5 count | 0~15 |
| Value 21 · `0x24` | Color 6 X (px) | 0, 1~159 |
| Value 22 · `0x25` | Color 6 Y (px, small at top) | 0, 1~119 |
| Value 23 · `0x26` | Color 6 area (% of frame) | 0, 1~100 % |
| Value 24 · `0x27` | Color 6 count | 0~15 |
| Value 25 · `0x28` | Color 7 X (px) | 0, 1~159 |
| Value 26 · `0x29` | Color 7 Y (px, small at top) | 0, 1~119 |
| Value 27 · `0x2A` | Color 7 area (% of frame) | 0, 1~100 % |
| Value 28 · `0x2B` | Color 7 count | 0~15 |
| Value 29 · `0x2C` | Color 8 X (px) | 0, 1~159 |
| Value 30 · `0x2D` | Color 8 Y (px, small at top) | 0, 1~119 |
| Value 31 · `0x2E` | Color 8 area (% of frame) | 0, 1~100 % |
| Value 32 · `0x2F` | Color 8 count | 0~15 |
| Value 33 · `0x30` | Color 9 X (px) | 0, 1~159 |
| Value 34 · `0x31` | Color 9 Y (px, small at top) | 0, 1~119 |
| Value 35 · `0x32` | Color 9 area (% of frame) | 0, 1~100 % |
| Value 36 · `0x33` | Color 9 count | 0~15 |
| Value 37 · `0x34` | Color 10 X (px) | 0, 1~159 |
| Value 38 · `0x35` | Color 10 Y (px, small at top) | 0, 1~119 |
| Value 39 · `0x36` | Color 10 area (% of frame) | 0, 1~100 % |
| Value 40 · `0x37` | Color 10 count | 0~15 |
| Value 41 · `0x38` | Color 11 X (px) | 0, 1~159 |
| Value 42 · `0x39` | Color 11 Y (px, small at top) | 0, 1~119 |
| Value 43 · `0x3A` | Color 11 area (% of frame) | 0, 1~100 % |
| Value 44 · `0x3B` | Color 11 count | 0~15 |
| Value 45 · `0x3C` | Color 12 X (px) | 0, 1~159 |
| Value 46 · `0x3D` | Color 12 Y (px, small at top) | 0, 1~119 |
| Value 47 · `0x3E` | Color 12 area (% of frame) | 0, 1~100 % |
| Value 48 · `0x3F` | Color 12 count | 0~15 |
| Value 49 · `0x40` | Color 13 X (px) | 0, 1~159 |
| Value 50 · `0x41` | Color 13 Y (px, small at top) | 0, 1~119 |
| Value 51 · `0x42` | Color 13 area (% of frame) | 0, 1~100 % |
| Value 52 · `0x43` | Color 13 count | 0~15 |
| Value 53 · `0x44` | Color 14 X (px) | 0, 1~159 |
| Value 54 · `0x45` | Color 14 Y (px, small at top) | 0, 1~119 |
| Value 55 · `0x46` | Color 14 area (% of frame) | 0, 1~100 % |
| Value 56 · `0x47` | Color 14 count | 0~15 |
| Value 57 · `0x48` | Color 15 X (px) | 0, 1~159 |
| Value 58 · `0x49` | Color 15 Y (px, small at top) | 0, 1~119 |
| Value 59 · `0x4A` | Color 15 area (% of frame) | 0, 1~100 % |
| Value 60 · `0x4B` | Color 15 count | 0~15 |
| Value 61 · `0x4C` | Color 16 X (px) | 0, 1~159 |
| Value 62 · `0x4D` | Color 16 Y (px, small at top) | 0, 1~119 |
| Value 63 · `0x4E` | Color 16 area (% of frame) | 0, 1~100 % |
| Value 64 · `0x4F` | Color 16 count | 0~15 |
| Value 65 · `0x50` | Colors found | 0~16 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-14">
<summary><span class="plat-dot"></span>Camera mode 1 Color tracking ④ Fine (XYWH): 16 colors XYWH + count + area</summary>

Mode number 14.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | Color 1 X (px) | 0, 1~159 |
| Value 2 · `0x11` | Color 1 Y (px, small at top) | 0, 1~119 |
| Value 3 · `0x12` | Color 1 width (px) | 0~160 |
| Value 4 · `0x13` | Color 1 height (px) | 0~120 |
| Value 5 · `0x14` | Color 1 count | 0~15 |
| Value 6 · `0x15` | Color 1 area (% of frame) | 0, 1~100 % |
| Value 7 · `0x16` | Color 2 X (px) | 0, 1~159 |
| Value 8 · `0x17` | Color 2 Y (px, small at top) | 0, 1~119 |
| Value 9 · `0x18` | Color 2 width (px) | 0~160 |
| Value 10 · `0x19` | Color 2 height (px) | 0~120 |
| Value 11 · `0x1A` | Color 2 count | 0~15 |
| Value 12 · `0x1B` | Color 2 area (% of frame) | 0, 1~100 % |
| Value 13 · `0x1C` | Color 3 X (px) | 0, 1~159 |
| Value 14 · `0x1D` | Color 3 Y (px, small at top) | 0, 1~119 |
| Value 15 · `0x1E` | Color 3 width (px) | 0~160 |
| Value 16 · `0x1F` | Color 3 height (px) | 0~120 |
| Value 17 · `0x20` | Color 3 count | 0~15 |
| Value 18 · `0x21` | Color 3 area (% of frame) | 0, 1~100 % |
| Value 19 · `0x22` | Color 4 X (px) | 0, 1~159 |
| Value 20 · `0x23` | Color 4 Y (px, small at top) | 0, 1~119 |
| Value 21 · `0x24` | Color 4 width (px) | 0~160 |
| Value 22 · `0x25` | Color 4 height (px) | 0~120 |
| Value 23 · `0x26` | Color 4 count | 0~15 |
| Value 24 · `0x27` | Color 4 area (% of frame) | 0, 1~100 % |
| Value 25 · `0x28` | Color 5 X (px) | 0, 1~159 |
| Value 26 · `0x29` | Color 5 Y (px, small at top) | 0, 1~119 |
| Value 27 · `0x2A` | Color 5 width (px) | 0~160 |
| Value 28 · `0x2B` | Color 5 height (px) | 0~120 |
| Value 29 · `0x2C` | Color 5 count | 0~15 |
| Value 30 · `0x2D` | Color 5 area (% of frame) | 0, 1~100 % |
| Value 31 · `0x2E` | Color 6 X (px) | 0, 1~159 |
| Value 32 · `0x2F` | Color 6 Y (px, small at top) | 0, 1~119 |
| Value 33 · `0x30` | Color 6 width (px) | 0~160 |
| Value 34 · `0x31` | Color 6 height (px) | 0~120 |
| Value 35 · `0x32` | Color 6 count | 0~15 |
| Value 36 · `0x33` | Color 6 area (% of frame) | 0, 1~100 % |
| Value 37 · `0x34` | Color 7 X (px) | 0, 1~159 |
| Value 38 · `0x35` | Color 7 Y (px, small at top) | 0, 1~119 |
| Value 39 · `0x36` | Color 7 width (px) | 0~160 |
| Value 40 · `0x37` | Color 7 height (px) | 0~120 |
| Value 41 · `0x38` | Color 7 count | 0~15 |
| Value 42 · `0x39` | Color 7 area (% of frame) | 0, 1~100 % |
| Value 43 · `0x3A` | Color 8 X (px) | 0, 1~159 |
| Value 44 · `0x3B` | Color 8 Y (px, small at top) | 0, 1~119 |
| Value 45 · `0x3C` | Color 8 width (px) | 0~160 |
| Value 46 · `0x3D` | Color 8 height (px) | 0~120 |
| Value 47 · `0x3E` | Color 8 count | 0~15 |
| Value 48 · `0x3F` | Color 8 area (% of frame) | 0, 1~100 % |
| Value 49 · `0x40` | Color 9 X (px) | 0, 1~159 |
| Value 50 · `0x41` | Color 9 Y (px, small at top) | 0, 1~119 |
| Value 51 · `0x42` | Color 9 width (px) | 0~160 |
| Value 52 · `0x43` | Color 9 height (px) | 0~120 |
| Value 53 · `0x44` | Color 9 count | 0~15 |
| Value 54 · `0x45` | Color 9 area (% of frame) | 0, 1~100 % |
| Value 55 · `0x46` | Color 10 X (px) | 0, 1~159 |
| Value 56 · `0x47` | Color 10 Y (px, small at top) | 0, 1~119 |
| Value 57 · `0x48` | Color 10 width (px) | 0~160 |
| Value 58 · `0x49` | Color 10 height (px) | 0~120 |
| Value 59 · `0x4A` | Color 10 count | 0~15 |
| Value 60 · `0x4B` | Color 10 area (% of frame) | 0, 1~100 % |
| Value 61 · `0x4C` | Color 11 X (px) | 0, 1~159 |
| Value 62 · `0x4D` | Color 11 Y (px, small at top) | 0, 1~119 |
| Value 63 · `0x4E` | Color 11 width (px) | 0~160 |
| Value 64 · `0x4F` | Color 11 height (px) | 0~120 |
| Value 65 · `0x50` | Color 11 count | 0~15 |
| Value 66 · `0x51` | Color 11 area (% of frame) | 0, 1~100 % |
| Value 67 · `0x52` | Color 12 X (px) | 0, 1~159 |
| Value 68 · `0x53` | Color 12 Y (px, small at top) | 0, 1~119 |
| Value 69 · `0x54` | Color 12 width (px) | 0~160 |
| Value 70 · `0x55` | Color 12 height (px) | 0~120 |
| Value 71 · `0x56` | Color 12 count | 0~15 |
| Value 72 · `0x57` | Color 12 area (% of frame) | 0, 1~100 % |
| Value 73 · `0x58` | Color 13 X (px) | 0, 1~159 |
| Value 74 · `0x59` | Color 13 Y (px, small at top) | 0, 1~119 |
| Value 75 · `0x5A` | Color 13 width (px) | 0~160 |
| Value 76 · `0x5B` | Color 13 height (px) | 0~120 |
| Value 77 · `0x5C` | Color 13 count | 0~15 |
| Value 78 · `0x5D` | Color 13 area (% of frame) | 0, 1~100 % |
| Value 79 · `0x5E` | Color 14 X (px) | 0, 1~159 |
| Value 80 · `0x5F` | Color 14 Y (px, small at top) | 0, 1~119 |
| Value 81 · `0x60` | Color 14 width (px) | 0~160 |
| Value 82 · `0x61` | Color 14 height (px) | 0~120 |
| Value 83 · `0x62` | Color 14 count | 0~15 |
| Value 84 · `0x63` | Color 14 area (% of frame) | 0, 1~100 % |
| Value 85 · `0x64` | Color 15 X (px) | 0, 1~159 |
| Value 86 · `0x65` | Color 15 Y (px, small at top) | 0, 1~119 |
| Value 87 · `0x66` | Color 15 width (px) | 0~160 |
| Value 88 · `0x67` | Color 15 height (px) | 0~120 |
| Value 89 · `0x68` | Color 15 count | 0~15 |
| Value 90 · `0x69` | Color 15 area (% of frame) | 0, 1~100 % |
| Value 91 · `0x6A` | Color 16 X (px) | 0, 1~159 |
| Value 92 · `0x6B` | Color 16 Y (px, small at top) | 0, 1~119 |
| Value 93 · `0x6C` | Color 16 width (px) | 0~160 |
| Value 94 · `0x6D` | Color 16 height (px) | 0~120 |
| Value 95 · `0x6E` | Color 16 count | 0~15 |
| Value 96 · `0x6F` | Color 16 area (% of frame) | 0, 1~100 % |
| Value 97 · `0x70` | Colors found | 0~16 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-21">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ① Hue: 16 points hue</summary>

Mode number 21.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | S1 hue | 0~255 |
| Value 2 · `0x11` | S2 hue | 0~255 |
| Value 3 · `0x12` | S3 hue | 0~255 |
| Value 4 · `0x13` | S4 hue | 0~255 |
| Value 5 · `0x14` | S5 hue | 0~255 |
| Value 6 · `0x15` | S6 hue | 0~255 |
| Value 7 · `0x16` | S7 hue | 0~255 |
| Value 8 · `0x17` | S8 hue | 0~255 |
| Value 9 · `0x18` | S9 hue | 0~255 |
| Value 10 · `0x19` | S10 hue | 0~255 |
| Value 11 · `0x1A` | S11 hue | 0~255 |
| Value 12 · `0x1B` | S12 hue | 0~255 |
| Value 13 · `0x1C` | S13 hue | 0~255 |
| Value 14 · `0x1D` | S14 hue | 0~255 |
| Value 15 · `0x1E` | S15 hue | 0~255 |
| Value 16 · `0x1F` | S16 hue | 0~255 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-22">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ② Hue + saturation: 16 points hue + sat</summary>

Mode number 22.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | S1 hue | 0~255 |
| Value 2 · `0x11` | S1 saturation | 0~100 |
| Value 3 · `0x12` | S2 hue | 0~255 |
| Value 4 · `0x13` | S2 saturation | 0~100 |
| Value 5 · `0x14` | S3 hue | 0~255 |
| Value 6 · `0x15` | S3 saturation | 0~100 |
| Value 7 · `0x16` | S4 hue | 0~255 |
| Value 8 · `0x17` | S4 saturation | 0~100 |
| Value 9 · `0x18` | S5 hue | 0~255 |
| Value 10 · `0x19` | S5 saturation | 0~100 |
| Value 11 · `0x1A` | S6 hue | 0~255 |
| Value 12 · `0x1B` | S6 saturation | 0~100 |
| Value 13 · `0x1C` | S7 hue | 0~255 |
| Value 14 · `0x1D` | S7 saturation | 0~100 |
| Value 15 · `0x1E` | S8 hue | 0~255 |
| Value 16 · `0x1F` | S8 saturation | 0~100 |
| Value 17 · `0x20` | S9 hue | 0~255 |
| Value 18 · `0x21` | S9 saturation | 0~100 |
| Value 19 · `0x22` | S10 hue | 0~255 |
| Value 20 · `0x23` | S10 saturation | 0~100 |
| Value 21 · `0x24` | S11 hue | 0~255 |
| Value 22 · `0x25` | S11 saturation | 0~100 |
| Value 23 · `0x26` | S12 hue | 0~255 |
| Value 24 · `0x27` | S12 saturation | 0~100 |
| Value 25 · `0x28` | S13 hue | 0~255 |
| Value 26 · `0x29` | S13 saturation | 0~100 |
| Value 27 · `0x2A` | S14 hue | 0~255 |
| Value 28 · `0x2B` | S14 saturation | 0~100 |
| Value 29 · `0x2C` | S15 hue | 0~255 |
| Value 30 · `0x2D` | S15 saturation | 0~100 |
| Value 31 · `0x2E` | S16 hue | 0~255 |
| Value 32 · `0x2F` | S16 saturation | 0~100 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-23">
<summary><span class="plat-dot"></span>Camera mode 2 Color at points ③ Hue + saturation + value: 16 points hue + sat + value</summary>

Mode number 23.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | S1 hue | 0~255 |
| Value 2 · `0x11` | S1 saturation | 0~100 |
| Value 3 · `0x12` | S1 value | 0~100 |
| Value 4 · `0x13` | S2 hue | 0~255 |
| Value 5 · `0x14` | S2 saturation | 0~100 |
| Value 6 · `0x15` | S2 value | 0~100 |
| Value 7 · `0x16` | S3 hue | 0~255 |
| Value 8 · `0x17` | S3 saturation | 0~100 |
| Value 9 · `0x18` | S3 value | 0~100 |
| Value 10 · `0x19` | S4 hue | 0~255 |
| Value 11 · `0x1A` | S4 saturation | 0~100 |
| Value 12 · `0x1B` | S4 value | 0~100 |
| Value 13 · `0x1C` | S5 hue | 0~255 |
| Value 14 · `0x1D` | S5 saturation | 0~100 |
| Value 15 · `0x1E` | S5 value | 0~100 |
| Value 16 · `0x1F` | S6 hue | 0~255 |
| Value 17 · `0x20` | S6 saturation | 0~100 |
| Value 18 · `0x21` | S6 value | 0~100 |
| Value 19 · `0x22` | S7 hue | 0~255 |
| Value 20 · `0x23` | S7 saturation | 0~100 |
| Value 21 · `0x24` | S7 value | 0~100 |
| Value 22 · `0x25` | S8 hue | 0~255 |
| Value 23 · `0x26` | S8 saturation | 0~100 |
| Value 24 · `0x27` | S8 value | 0~100 |
| Value 25 · `0x28` | S9 hue | 0~255 |
| Value 26 · `0x29` | S9 saturation | 0~100 |
| Value 27 · `0x2A` | S9 value | 0~100 |
| Value 28 · `0x2B` | S10 hue | 0~255 |
| Value 29 · `0x2C` | S10 saturation | 0~100 |
| Value 30 · `0x2D` | S10 value | 0~100 |
| Value 31 · `0x2E` | S11 hue | 0~255 |
| Value 32 · `0x2F` | S11 saturation | 0~100 |
| Value 33 · `0x30` | S11 value | 0~100 |
| Value 34 · `0x31` | S12 hue | 0~255 |
| Value 35 · `0x32` | S12 saturation | 0~100 |
| Value 36 · `0x33` | S12 value | 0~100 |
| Value 37 · `0x34` | S13 hue | 0~255 |
| Value 38 · `0x35` | S13 saturation | 0~100 |
| Value 39 · `0x36` | S13 value | 0~100 |
| Value 40 · `0x37` | S14 hue | 0~255 |
| Value 41 · `0x38` | S14 saturation | 0~100 |
| Value 42 · `0x39` | S14 value | 0~100 |
| Value 43 · `0x3A` | S15 hue | 0~255 |
| Value 44 · `0x3B` | S15 saturation | 0~100 |
| Value 45 · `0x3C` | S15 value | 0~100 |
| Value 46 · `0x3D` | S16 hue | 0~255 |
| Value 47 · `0x3E` | S16 saturation | 0~100 |
| Value 48 · `0x3F` | S16 value | 0~100 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-30">
<summary><span class="plat-dot"></span>Camera mode 3 Color order: 16 blocks hue code</summary>

Mode number 30.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | Hue of block 1 from the left | 0~255 |
| Value 2 · `0x11` | Hue of block 2 from the left | 0~255 |
| Value 3 · `0x12` | Hue of block 3 from the left | 0~255 |
| Value 4 · `0x13` | Hue of block 4 from the left | 0~255 |
| Value 5 · `0x14` | Hue of block 5 from the left | 0~255 |
| Value 6 · `0x15` | Hue of block 6 from the left | 0~255 |
| Value 7 · `0x16` | Hue of block 7 from the left | 0~255 |
| Value 8 · `0x17` | Hue of block 8 from the left | 0~255 |
| Value 9 · `0x18` | Hue of block 9 from the left | 0~255 |
| Value 10 · `0x19` | Hue of block 10 from the left | 0~255 |
| Value 11 · `0x1A` | Hue of block 11 from the left | 0~255 |
| Value 12 · `0x1B` | Hue of block 12 from the left | 0~255 |
| Value 13 · `0x1C` | Hue of block 13 from the left | 0~255 |
| Value 14 · `0x1D` | Hue of block 14 from the left | 0~255 |
| Value 15 · `0x1E` | Hue of block 15 from the left | 0~255 |
| Value 16 · `0x1F` | Hue of block 16 from the left | 0~255 |
| Value 17 · `0x20` | Total blocks | 0~18 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-41">
<summary><span class="plat-dot"></span>Camera mode 4 Line following ①②③ black / white / colored line: 14 values</summary>

Mode number 41 / 42 / 43.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | Near offset | −100~100 |
| Value 2 · `0x11` | Middle offset | −100~100 |
| Value 3 · `0x12` | Far offset | −100~100 |
| Value 4 · `0x13` | Line angle | −90~90 |
| Value 5 · `0x14` | Near line width % | 0~100 |
| Value 6 · `0x15` | Line-found flags (near 1 + middle 2 + far 4) | 0~7 |
| Value 7 · `0x16` | Junction type (0 none, 1 cross, 2 T, 3 left, 4 right, 5 dead end) | 0~5 |
| Value 8 · `0x17` | Junction distance % | 0~100 |
| Value 9 · `0x18` | Near line left edge | −100~100 |
| Value 10 · `0x19` | Near line right edge | −100~100 |
| Value 11 · `0x1A` | Far line width % | 0~100 |
| Value 12 · `0x1B` | Color number of the marker line crossed | 0, 1~16 |
| Value 13 · `0x1C` | Marker lines crossed so far | 0~99 |
| Value 14 · `0x1D` | Side the line was last lost (0 on the line, 1 left, 2 right) | 0~2 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-51">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ① Object list: 8 objects: color, shape, XY</summary>

Mode number 51.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | Object 1 color number | 0, 1~8 |
| Value 2 · `0x11` | Object 1 shape | 0~9 |
| Value 3 · `0x12` | Object 1 X (px) | 0, 1~159 |
| Value 4 · `0x13` | Object 1 Y (px, small at top) | 0, 1~119 |
| Value 5 · `0x14` | Object 2 color number | 0, 1~8 |
| Value 6 · `0x15` | Object 2 shape | 0~9 |
| Value 7 · `0x16` | Object 2 X (px) | 0, 1~159 |
| Value 8 · `0x17` | Object 2 Y (px, small at top) | 0, 1~119 |
| Value 9 · `0x18` | Object 3 color number | 0, 1~8 |
| Value 10 · `0x19` | Object 3 shape | 0~9 |
| Value 11 · `0x1A` | Object 3 X (px) | 0, 1~159 |
| Value 12 · `0x1B` | Object 3 Y (px, small at top) | 0, 1~119 |
| Value 13 · `0x1C` | Object 4 color number | 0, 1~8 |
| Value 14 · `0x1D` | Object 4 shape | 0~9 |
| Value 15 · `0x1E` | Object 4 X (px) | 0, 1~159 |
| Value 16 · `0x1F` | Object 4 Y (px, small at top) | 0, 1~119 |
| Value 17 · `0x20` | Object 5 color number | 0, 1~8 |
| Value 18 · `0x21` | Object 5 shape | 0~9 |
| Value 19 · `0x22` | Object 5 X (px) | 0, 1~159 |
| Value 20 · `0x23` | Object 5 Y (px, small at top) | 0, 1~119 |
| Value 21 · `0x24` | Object 6 color number | 0, 1~8 |
| Value 22 · `0x25` | Object 6 shape | 0~9 |
| Value 23 · `0x26` | Object 6 X (px) | 0, 1~159 |
| Value 24 · `0x27` | Object 6 Y (px, small at top) | 0, 1~119 |
| Value 25 · `0x28` | Object 7 color number | 0, 1~8 |
| Value 26 · `0x29` | Object 7 shape | 0~9 |
| Value 27 · `0x2A` | Object 7 X (px) | 0, 1~159 |
| Value 28 · `0x2B` | Object 7 Y (px, small at top) | 0, 1~119 |
| Value 29 · `0x2C` | Object 8 color number | 0, 1~8 |
| Value 30 · `0x2D` | Object 8 shape | 0~9 |
| Value 31 · `0x2E` | Object 8 X (px) | 0, 1~159 |
| Value 32 · `0x2F` | Object 8 Y (px, small at top) | 0, 1~119 |
| Value 33 · `0x30` | Object count | 0~8 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

<details class="plat pg-arduino" id="i2c-52">
<summary><span class="plat-dot"></span>Camera mode 5 Object detection ② Object details: 8 objects + size, area, angle</summary>

Mode number 52.

<div>

| Position | Content | Range |
| :--- | :--- | :--- |
| **Read (value numbers in order)** | | |
| Value 1 · `0x10` | Object 1 color number | 0, 1~8 |
| Value 2 · `0x11` | Object 1 shape | 0~9 |
| Value 3 · `0x12` | Object 1 X (px) | 0, 1~159 |
| Value 4 · `0x13` | Object 1 Y (px, small at top) | 0, 1~119 |
| Value 5 · `0x14` | Object 1 width (px) | 0~160 |
| Value 6 · `0x15` | Object 1 height (px) | 0~120 |
| Value 7 · `0x16` | Object 1 area (% of frame) | 0, 1~100 % |
| Value 8 · `0x17` | Object 1 angle | 0~179 |
| Value 9 · `0x18` | Object 1 vertices | 0~10 |
| Value 10 · `0x19` | Object 1 tracking ID | 0~255 |
| Value 11 · `0x1A` | Object 2 color number | 0, 1~8 |
| Value 12 · `0x1B` | Object 2 shape | 0~9 |
| Value 13 · `0x1C` | Object 2 X (px) | 0, 1~159 |
| Value 14 · `0x1D` | Object 2 Y (px, small at top) | 0, 1~119 |
| Value 15 · `0x1E` | Object 2 width (px) | 0~160 |
| Value 16 · `0x1F` | Object 2 height (px) | 0~120 |
| Value 17 · `0x20` | Object 2 area (% of frame) | 0, 1~100 % |
| Value 18 · `0x21` | Object 2 angle | 0~179 |
| Value 19 · `0x22` | Object 2 vertices | 0~10 |
| Value 20 · `0x23` | Object 2 tracking ID | 0~255 |
| Value 21 · `0x24` | Object 3 color number | 0, 1~8 |
| Value 22 · `0x25` | Object 3 shape | 0~9 |
| Value 23 · `0x26` | Object 3 X (px) | 0, 1~159 |
| Value 24 · `0x27` | Object 3 Y (px, small at top) | 0, 1~119 |
| Value 25 · `0x28` | Object 3 width (px) | 0~160 |
| Value 26 · `0x29` | Object 3 height (px) | 0~120 |
| Value 27 · `0x2A` | Object 3 area (% of frame) | 0, 1~100 % |
| Value 28 · `0x2B` | Object 3 angle | 0~179 |
| Value 29 · `0x2C` | Object 3 vertices | 0~10 |
| Value 30 · `0x2D` | Object 3 tracking ID | 0~255 |
| Value 31 · `0x2E` | Object 4 color number | 0, 1~8 |
| Value 32 · `0x2F` | Object 4 shape | 0~9 |
| Value 33 · `0x30` | Object 4 X (px) | 0, 1~159 |
| Value 34 · `0x31` | Object 4 Y (px, small at top) | 0, 1~119 |
| Value 35 · `0x32` | Object 4 width (px) | 0~160 |
| Value 36 · `0x33` | Object 4 height (px) | 0~120 |
| Value 37 · `0x34` | Object 4 area (% of frame) | 0, 1~100 % |
| Value 38 · `0x35` | Object 4 angle | 0~179 |
| Value 39 · `0x36` | Object 4 vertices | 0~10 |
| Value 40 · `0x37` | Object 4 tracking ID | 0~255 |
| Value 41 · `0x38` | Object 5 color number | 0, 1~8 |
| Value 42 · `0x39` | Object 5 shape | 0~9 |
| Value 43 · `0x3A` | Object 5 X (px) | 0, 1~159 |
| Value 44 · `0x3B` | Object 5 Y (px, small at top) | 0, 1~119 |
| Value 45 · `0x3C` | Object 5 width (px) | 0~160 |
| Value 46 · `0x3D` | Object 5 height (px) | 0~120 |
| Value 47 · `0x3E` | Object 5 area (% of frame) | 0, 1~100 % |
| Value 48 · `0x3F` | Object 5 angle | 0~179 |
| Value 49 · `0x40` | Object 5 vertices | 0~10 |
| Value 50 · `0x41` | Object 5 tracking ID | 0~255 |
| Value 51 · `0x42` | Object 6 color number | 0, 1~8 |
| Value 52 · `0x43` | Object 6 shape | 0~9 |
| Value 53 · `0x44` | Object 6 X (px) | 0, 1~159 |
| Value 54 · `0x45` | Object 6 Y (px, small at top) | 0, 1~119 |
| Value 55 · `0x46` | Object 6 width (px) | 0~160 |
| Value 56 · `0x47` | Object 6 height (px) | 0~120 |
| Value 57 · `0x48` | Object 6 area (% of frame) | 0, 1~100 % |
| Value 58 · `0x49` | Object 6 angle | 0~179 |
| Value 59 · `0x4A` | Object 6 vertices | 0~10 |
| Value 60 · `0x4B` | Object 6 tracking ID | 0~255 |
| Value 61 · `0x4C` | Object 7 color number | 0, 1~8 |
| Value 62 · `0x4D` | Object 7 shape | 0~9 |
| Value 63 · `0x4E` | Object 7 X (px) | 0, 1~159 |
| Value 64 · `0x4F` | Object 7 Y (px, small at top) | 0, 1~119 |
| Value 65 · `0x50` | Object 7 width (px) | 0~160 |
| Value 66 · `0x51` | Object 7 height (px) | 0~120 |
| Value 67 · `0x52` | Object 7 area (% of frame) | 0, 1~100 % |
| Value 68 · `0x53` | Object 7 angle | 0~179 |
| Value 69 · `0x54` | Object 7 vertices | 0~10 |
| Value 70 · `0x55` | Object 7 tracking ID | 0~255 |
| Value 71 · `0x56` | Object 8 color number | 0, 1~8 |
| Value 72 · `0x57` | Object 8 shape | 0~9 |
| Value 73 · `0x58` | Object 8 X (px) | 0, 1~159 |
| Value 74 · `0x59` | Object 8 Y (px, small at top) | 0, 1~119 |
| Value 75 · `0x5A` | Object 8 width (px) | 0~160 |
| Value 76 · `0x5B` | Object 8 height (px) | 0~120 |
| Value 77 · `0x5C` | Object 8 area (% of frame) | 0, 1~100 % |
| Value 78 · `0x5D` | Object 8 angle | 0~179 |
| Value 79 · `0x5E` | Object 8 vertices | 0~10 |
| Value 80 · `0x5F` | Object 8 tracking ID | 0~255 |
| Value 81 · `0x60` | Object count | 0~8 |
| **Write** | | |
|  | Switch camera mode and sub-option; set target colors, minimum area and sample-point positions | |

</div>

</details>

## Sources

- Slot layout: `cam_universal_layout.json` (in `_build/protocol/unicam/`, copied from `tests\spec_layout.json` in the camera firmware folder, spec frozen 2026-10-28; the firmware is checked against it byte by byte on every build)
- Generator: `_build/gen-unicam.mjs`
