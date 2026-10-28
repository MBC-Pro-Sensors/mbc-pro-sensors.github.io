<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- Code blocks = original text of examples/unicam/microbit/en/*.ts (helper master: 上位機函式庫\microbit\MBC_UniCam_microbit.ts) -->
# 🟧 micro:bit examples: Sharpshooter Vision

[🔙 Back to Sharpshooter Vision](/en/sensors/unicam/index.md)　｜　[micro:bit reading guide](/en/sensors/unicam/microbit/index.md)　｜　[📋 Value reference (I2C)](/en/sensors/unicam/reference.md#host-i2c)

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passed computer tests; real-hardware numbers (fps, line thresholds, I2C pins) may change after bench testing.

This page is for people who **have never written a program**. Four examples: **which side is the blob**, **what colour does the spot see**, a **line-following car** and **which side is the red triangle (object detection)**. All are written in **MakeCode** JavaScript: switch to JavaScript, paste the whole example, done. You can switch back to Blocks to see what it looks like as blocks.

> [!IMPORTANT]
> - **No extension needed.** At the bottom of every example there is a "UniCam helpers" section that talks to the camera with the built-in `pins.i2c…` functions. The "camera" blocks in the **pxt-mbc-sensors** extension are for the **School edition** and cannot read the current Sharpshooter Vision's values — do not mix them.
> - Use a camera with the **common I2C** firmware. SPIKE, EV3 and MATRIX firmware cannot be read by a micro:bit.
> - The examples were compiled on a computer with the MakeCode CLI (V1 + V2) and their logic was run with fake I2C data; **they have not been run with a real camera yet**.

## Software

| Tool | micro:bit V1 | micro:bit V2 | How |
|---|---|---|---|
| **MakeCode web** ([makecode.microbit.org](https://makecode.microbit.org/), JavaScript) | ✔ | ✔ | New project → switch to **JavaScript** (top right) → select all, delete, paste the example → **Download**, drag the .hex onto the MICROBIT drive |
| MakeCode offline app (Windows / Mac) | ✔ | ✔ | Same |
| Python Editor (MicroPython) | — | — | No examples yet |

- **Simulator**: the MakeCode simulator has no real I2C and reads 0 (the examples show an X). Download to a real micro:bit to see real values.
- **Servos**: example 3 uses two **continuous-rotation servos** (FS90R or similar), left wheel P1, right wheel P2. Always power the servos from an **external 4.8~6 V battery box** and connect the battery minus to micro:bit GND (common ground).

## Before you start: hardware checks (bench test pending)

| Check | This product | Status |
|---|---|---|
| Voltage | The camera's I2C is 3.3 V, connect straight to P19/P20, no level shifter | ✔ |
| Power | ESP32 camera + screen peaks above 300 mA: **external 5 V is required**, never the micro:bit 3V pin | Pending |
| I2C pull-ups | The micro:bit already has 4.7 kΩ; if the camera board also has pull-ups, check the low level ≤ 0.3×VDD | Pending |
| Clock stretching | ESP32 cannot stretch the clock: the helpers wait 2 ms after writing the id, nothing to do | Pending |
| I2C pins | Camera side is SDA = IO3, SCL = IO1 for now (same pair as the LEGO serial port), **final after bench testing** | Pending |
| Ready time | `ucReady()` reads F3 bit0; the examples wait at start until the camera answers | Pending |

### Wiring (all examples)

| Camera | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| VCC | external 5 V (required) |
| GND | GND (servo battery minus here too) |

If you only ever see an X: check SDA/SCL are not swapped → the external power is on → the camera has the "common I2C" firmware.

## Values used on this page

- **Mode number** = camera mode × 10 + option: 13 = colour tracking ③ standard, 21 = spot colour ① hue, 41 = line following ① black line. The examples switch with the **E0** command at start; you can also set it in the camera menu (both must match).
- **Coordinates**: pixels, X 0~159 (left → right), Y 0~119 (top → bottom); **0 = not found**.
- **Hue code** 0~255: 0~240 hue (red 0/240, yellow 40, green 80, cyan 120, blue 160, purple 200); 241~248 white/grey; 249~255 black.
- **Line offset** −100~100: standing behind the car looking forward, **line on the left = negative**, on the right = positive.

### I2C value ids (address 0x4C)

| Id | Content |
|---|---|
| `0x01` | Mode number (e.g. 13; object detection 51 / 52) |
| `0x02` | Option |
| `0x03` | Colours taught |
| `0x04` | Values in this level |
| from `0x10` | Values of this mode (order as in the [value reference](/en/sensors/unicam/reference.md#host-i2c): value 1 = `0x10`, value 2 = `0x11` …) |
| `0xF0` | Device id (`0x4C` + 3×256, 3 = current Sharpshooter Vision) |
| `0xF2` | Firmware version (100 = 1.0) |
| `0xF3` | Status (bit0 = ready, bit1 = something found in the current mode) |
| write `0xE0` | [camera mode 1~5, option]: switch mode (saved in the camera; object detection [5, 1] / [5, 2]) |
| write `0xE1` | [spot 1~20, X 0~159, Y 0~119]: move a sample spot (saved in the camera) |

Reading (MBC common I2C protocol v2): write [id] → wait 2 ms → read 2×k bytes = k values from that id (int16, low byte first), at most 16 at a time; unknown ids read −32768.

### Helper functions

| Function | Returns | Meaning |
|---|---|---|
| `ucReadMany(id, k)` | number array | read k values (1~16) from id; all `UC_NONE` (−32768) when the camera does not answer |
| `ucWrite(id, params)` | — | write a command [id][params…] |
| `ucReady()` | true/false | is the camera ready (F3 bit0) |
| `ucSetMode(camMode, sub)` | — | E0: switch camera mode (colour order uses option 0) |
| `ucSetPoint(n, x, y)` | — | E1: move sample spot n to (x, y) |
| `ucModeNumber()` | number | the camera's current mode number |
| `ucReadBlob(mode, color)` | `[x, y, w, h, count, area%]` | colour tracking 11~14, colour 1~16; x = 0 = not found; −1 = not in this mode |
| `ucColorsFound(mode)` | number | how many of the 16 colours are found |
| `ucReadSpot(mode, point)` | `[hue, sat, val]` | spot colour 21~23, spot 1~16; −1 = not in this mode |
| `ucReadOrder()` | number array | colour order: hue codes from left to right (up to 16 blocks) |
| `ucOrderTotal()` | number | total blocks |
| `ucReadLine()` | 14 numbers | line: [near, mid, far offset, angle, width, line flags, junction, junction distance, left edge, right edge, far width, marker colour, marker count, lost side] |
| `ucColorCode(h)` | 1~9 | hue code → 1 red 2 orange 3 yellow 4 green 5 cyan 6 blue 7 purple 8 white/grey 9 black |
| `ucReadObject(mode, i)` | `[code, colour, shape, x, y, width, height, area%, angle, vertices, track id]` | object detection 51 / 52, objects 1~8 (1 = largest); code = colour × 10 + shape, 0 = none; −1 = not given in mode 51 |
| `ucObjectCount(mode)` | number | objects in view, 0~8 |
| `ucShapeName(s)` | text | shape 0~9 → `"other"` `"circle"` `"ellipse"` `"triangle"` `"square"` `"rectangle"` `"pentagon"` `"hexagon"` `"star"` `"trapezoid"` |

---

## Example 1: which side is the blob (arrow)

**What it does**: colour 1 (the first colour taught to the camera) on the left of the picture → left arrow; middle → up arrow; right → right arrow; not seen → question mark. Press A to show the area %.

**You need**
- micro:bit V1 or V2 + USB cable
- Sharpshooter Vision (common I2C firmware) + external 5 V power
- One brightly coloured block

**Camera setup**: teach "target 1" on the camera (live view on target 1 → press OK). Camera mode **1 colour tracking · option ③ standard** (mode number 13) — the program switches it with E0 at start.

**Wiring**: see the wiring table above.

**Download**: [ex01_track_arrow.ts](/examples/unicam/microbit/en/ex01_track_arrow.ts) (right-click, save) → MakeCode new project → **JavaScript** → select all, delete, paste → **Download**.

**Program**

```typescript
// Sharpshooter Vision × micro:bit example 1: which side is the blob (arrow)
// ★ Set the camera menu to camera mode 1 colour tracking · option ③ standard (mode number 13) (the program also switches it with E0 at start)
// What it does: colour 1 (the first colour taught to the camera) on the left → left arrow, middle → up arrow, right → right arrow; not found → question mark. Press A to show the area %.
// Wiring: camera SDA → P20, SCL → P19, VCC → external 5 V (do not power it from the micro:bit 3V pin), GND → GND (common ground).
// Before you start: teach "target 1" on the camera (stop on target 1, press OK to pick the colour). MakeCode new project → switch to JavaScript (top right) → paste this whole file → Download. No extension needed.
// What you see: a tick at start; move a block of the target colour left and right in front of the lens and the arrow follows; take it away → question mark.
// ⚠ v1 not yet hardware-verified: the value layout is final and passed computer tests; real-hardware numbers may change.

const MODE = 13                                   // mode number = camera mode 1 × 10 + option 3
let ready = false
let busy = false

basic.showIcon(IconNames.SmallDiamond)            // waiting for the camera

basic.forever(function () {
    if (!ready) {                                 // first time: wait for the camera, then switch to mode 13
        if (ucReady()) {
            ucSetMode(1, 3)                       // E0: camera mode 1, option 3
            ready = true
            basic.showIcon(IconNames.Yes)
            basic.pause(500)
        } else {
            basic.showIcon(IconNames.No)          // no answer: check P19/P20 and the external power
            basic.pause(500)
        }
        return
    }
    if (busy) {
        basic.pause(100)
        return
    }
    const b = ucReadBlob(MODE, 1)                 // [x, y, width, height, count, area%]
    const x = b[0]                                // 0 = not found; 1~159 left → right
    if (x <= 0) {
        basic.showString("?", 0)
    } else if (x < 60) {                          // 👉 You can change this: left range
        basic.showArrow(ArrowNames.West, 0)
    } else if (x > 100) {                         // 👉 You can change this: right range
        basic.showArrow(ArrowNames.East, 0)
    } else {
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(ucReadBlob(MODE, 1)[5])      // area: % of the picture (0 = not found)
    busy = false
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
// ===== UniCam helpers (MBC common I2C protocol v2): same in every example, no need to change =====
const UC_ADDR = 0x4C                     // 相機的 I2C 位址 / camera I2C address
const UC_NONE = -32768                   // 讀不到 / no value

// 從編號 id 起連續讀 k 個 int16（1~16）；讀不到全部是 UC_NONE
function ucReadMany(id: number, k: number): number[] {
    const cmd = pins.createBuffer(1)
    cmd[0] = id
    const r = pins.i2cWriteBuffer(UC_ADDR, cmd, false)
    basic.pause(2)                       // 相機（ESP32）要等 2 ms 再讀
    const out: number[] = []
    if (r) {                             // 實機：非 0 = 沒有回應
        for (let i = 0; i < k; i++) out.push(UC_NONE)
        return out
    }
    const b = pins.i2cReadBuffer(UC_ADDR, 2 * k, false)
    for (let i = 0; i < k; i++) out.push(b.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 寫命令 [id][參數…] / write a command
function ucWrite(id: number, params: number[]): void {
    const b = pins.createBuffer(1 + params.length)
    b[0] = id
    for (let i = 0; i < params.length; i++) b[1 + i] = params[i]
    pins.i2cWriteBuffer(UC_ADDR, b, false)
}

// 相機準備好了嗎（F3 bit0）/ is the camera ready
function ucReady(): boolean {
    const f = ucReadMany(0xF3, 1)[0]
    return f != UC_NONE && (f & 1) == 1
}

// 切相機模式（E0）：camMode 1~5、sub 子選項（照順序給色號填 0；物體偵測 1 清單、2 詳細）/ switch camera mode
function ucSetMode(camMode: number, sub: number): void {
    ucWrite(0xE0, [camMode, sub])
    basic.pause(50)
}

// 改取樣點位置（E1）：n 1~20、x 0~159、y 0~119 / move a sample spot
function ucSetPoint(n: number, x: number, y: number): void {
    ucWrite(0xE1, [n, x, y])
}

// 相機目前的模式編號（例 13）/ current mode number
function ucModeNumber(): number {
    return ucReadMany(0x01, 1)[0]
}

// 顏色追蹤每色幾個值 / values per colour (modes 11~14)
function ucStride(mode: number): number {
    if (mode == 11) return 2
    if (mode == 12) return 3
    if (mode == 13) return 4
    if (mode == 14) return 6
    return 0
}

// 色 color（1~16）的色塊：[x, y, 寬, 高, 塊數, 面積%]，x = 0 = 沒找到；這個模式沒給的是 -1
// blob of colour: [x, y, w, h, count, area%]; x = 0 = not found; -1 = not in this mode
function ucReadBlob(mode: number, color: number): number[] {
    const k = ucStride(mode)
    const d = ucReadMany(0x10 + (color - 1) * k, k)
    if (mode == 11) return [d[0], d[1], -1, -1, -1, -1]
    if (mode == 12) return [d[0], d[1], -1, -1, d[2], -1]
    if (mode == 13) return [d[0], d[1], -1, -1, d[3], d[2]]
    return [d[0], d[1], d[2], d[3], d[4], d[5]]
}

// 16 個目標色裡找到幾色 / colours found (modes 11~14)
function ucColorsFound(mode: number): number {
    return ucReadMany(0x10 + 16 * ucStride(mode), 1)[0]
}

// 取樣點 point（1~16）：[色相碼, 飽和度, 明度]；沒給的是 -1 / spot: [hue, sat, val]
function ucReadSpot(mode: number, point: number): number[] {
    const k = mode - 20
    const d = ucReadMany(0x10 + (point - 1) * k, k)
    return [d[0], k >= 2 ? d[1] : -1, k >= 3 ? d[2] : -1]
}

// 照順序給色號：由左到右每塊的色相碼（最多 16 塊）/ hue codes left to right
function ucReadOrder(): number[] {
    const total = ucReadMany(0x20, 1)[0]
    const d = ucReadMany(0x10, 16)
    if (total == UC_NONE) return []
    return d.slice(0, Math.min(16, total))
}

// 總塊數 / total blocks (mode 30)
function ucOrderTotal(): number {
    return ucReadMany(0x20, 1)[0]
}

// 循線 14 個值：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向]
// line values (near/mid/far offset: line on the left = negative)
function ucReadLine(): number[] {
    return ucReadMany(0x10, 14)
}

// 物體偵測每個物體幾個值：51 → 4（色號 形狀 X Y）、52 → 10 / values per object (modes 51, 52)
function ucObjStride(mode: number): number {
    if (mode == 51) return 4
    if (mode == 52) return 10
    return 0
}

// 物體 i（1~8，1 = 面積最大）：[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]
// 代碼 = 色號×10＋形狀，0 = 沒有；模式 51 沒給的是 -1
// object i: [code, colour, shape, x, y, w, h, area%, angle, vertices, trackId]; -1 = not in mode 51
function ucReadObject(mode: number, i: number): number[] {
    const k = ucObjStride(mode)
    const d = ucReadMany(0x10 + (i - 1) * k, k)
    const code = d[0] > 0 ? d[0] * 10 + d[1] : 0
    if (mode == 51) return [code, d[0], d[1], d[2], d[3], -1, -1, -1, -1, -1, -1]
    return [code, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]]
}

// 畫面裡有幾個物體 0~8 / number of objects in view (modes 51, 52)
function ucObjectCount(mode: number): number {
    return ucReadMany(0x10 + 8 * ucObjStride(mode), 1)[0]
}

// 形狀編號 → 英文名稱 / shape number -> name
function ucShapeName(s: number): string {
    const n = ["other", "circle", "ellipse", "triangle", "square", "rectangle", "pentagon", "hexagon", "star", "trapezoid"]
    return (s >= 0 && s <= 9) ? n[s] : "?"
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
function ucColorCode(h: number): number {
    if (h > 248) return 9
    if (h > 240) return 8
    if (h < 12 || h >= 220) return 1
    if (h < 28) return 2
    if (h < 56) return 3
    if (h < 100) return 4
    if (h < 140) return 5
    if (h < 180) return 6
    return 7
}
// ==== MBC_UNICAM_MB_HELPERS_END ====
```

**Step by step**
1. At start a small diamond shows; the first time `forever` runs it calls `ucReady()` until the camera answers, then `ucSetMode(1, 3)` switches to mode 13 and shows a tick.
2. `ucReadBlob(MODE, 1)` reads the 4 values of colour 1 (X, Y, area, count) at once and returns `[x, y, w, h, count, area%]`.
3. `x` = 0 → not seen → question mark; below 60 → left; above 100 → right; otherwise middle (the picture is 160 wide, the middle is 80).
4. Button A: `[5]` is the area %, shown as a number.

**What you should see**
- A tick about 3 seconds after power-on.
- Block on the left of the picture: ←; centre: ↑; right: →; taken away: ?.
- The closer the block, the bigger the number when you press A.

**Common problems**
- **Always X?** → The camera does not answer: check wiring, external power and that the firmware is the common I2C version. Power the camera first, wait for its picture, then press the micro:bit reset button.
- **Always a question mark?** → "Target 1" is not taught, or the light is too dark. Check that the X value in the camera's bottom info bar changes.
- **Left and right swapped?** → The camera faces you: swap the arrows for `x < 60` and `x > 100`.

---

## Example 2: what colour does the spot see

**What it does**: the LEDs show a letter for the colour seen by spot S1: **R** red, **O** orange, **Y** yellow, **G** green, **C** cyan, **B** blue, **P** purple, **W** white/grey, **K** black. Button A sends the **E1** command to move S1 to the centre of the picture; button B shows the hue code.

**You need**: the parts of example 1 + red, yellow, green, blue, white and black blocks or paper.

**Camera setup**: camera mode **2 spot colour · option ① hue** (mode number 21) — switched automatically at start.

**Wiring**: same as example 1.

**Download**: [ex02_spot_color.ts](/examples/unicam/microbit/en/ex02_spot_color.ts) (right-click, save) → paste into MakeCode JavaScript → Download.

**Program**

```typescript
// Sharpshooter Vision × micro:bit example 2: what colour does the spot see
// ★ Set the camera menu to camera mode 2 spot colour · option ① hue (mode number 21) (the program also switches it with E0 at start)
// What it does: the LEDs show a letter for the colour seen by spot S1: R red, O orange, Y yellow, G green, C cyan, B blue, P purple, W white/grey, K black.
//               Press A: the E1 command moves S1 to the centre of the picture (X 80, Y 60); press B: show the hue code number (0~255).
// Wiring: camera SDA → P20, SCL → P19, VCC → external 5 V, GND → GND (common ground).
// Before you start: red, yellow, green, blue, white and black blocks or paper. In MakeCode switch to JavaScript, paste this file → Download. No extension needed.
// What you see: press A first, then hold a block in the centre of the lens: red → R, green → G, blue → B, white paper → W.
// ⚠ v1 not yet hardware-verified: the colour borders (ucColorCode) are estimates from the hue code; lighting differs, so press B to see the number and adjust.

const MODE = 21                                   // mode number = camera mode 2 × 10 + option 1
const LETTERS = "?ROYGCBPWK"                      // letters for ucColorCode 1~9
let ready = false
let busy = false

basic.showIcon(IconNames.SmallDiamond)            // waiting for the camera

basic.forever(function () {
    if (!ready) {                                 // first time: wait for the camera, then switch to mode 21
        if (ucReady()) {
            ucSetMode(2, 1)                       // E0: camera mode 2, option 1
            ready = true
            basic.showIcon(IconNames.Yes)
            basic.pause(500)
        } else {
            basic.showIcon(IconNames.No)          // no answer: check P19/P20 and the external power
            basic.pause(500)
        }
        return
    }
    if (busy) {
        basic.pause(100)
        return
    }
    const h = ucReadSpot(MODE, 1)[0]              // hue code of S1, 0~255
    if (h == UC_NONE) {
        basic.showString("-", 0)                  // cannot read
    } else {
        basic.showString(LETTERS.charAt(ucColorCode(h)), 0)
    }
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    ucSetPoint(1, 80, 60)                         // E1: move spot 1 to X 80, Y 60 (centre; saved in the camera)
    basic.showIcon(IconNames.Target)
    basic.pause(500)
    busy = false
})

input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(ucReadSpot(MODE, 1)[0])      // hue code: red 0/240, yellow 40, green 80, cyan 120, blue 160, purple 200; 241~248 white, 249~255 black
    busy = false
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
// ===== UniCam helpers (MBC common I2C protocol v2): same in every example, no need to change =====
const UC_ADDR = 0x4C                     // 相機的 I2C 位址 / camera I2C address
const UC_NONE = -32768                   // 讀不到 / no value

// 從編號 id 起連續讀 k 個 int16（1~16）；讀不到全部是 UC_NONE
function ucReadMany(id: number, k: number): number[] {
    const cmd = pins.createBuffer(1)
    cmd[0] = id
    const r = pins.i2cWriteBuffer(UC_ADDR, cmd, false)
    basic.pause(2)                       // 相機（ESP32）要等 2 ms 再讀
    const out: number[] = []
    if (r) {                             // 實機：非 0 = 沒有回應
        for (let i = 0; i < k; i++) out.push(UC_NONE)
        return out
    }
    const b = pins.i2cReadBuffer(UC_ADDR, 2 * k, false)
    for (let i = 0; i < k; i++) out.push(b.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 寫命令 [id][參數…] / write a command
function ucWrite(id: number, params: number[]): void {
    const b = pins.createBuffer(1 + params.length)
    b[0] = id
    for (let i = 0; i < params.length; i++) b[1 + i] = params[i]
    pins.i2cWriteBuffer(UC_ADDR, b, false)
}

// 相機準備好了嗎（F3 bit0）/ is the camera ready
function ucReady(): boolean {
    const f = ucReadMany(0xF3, 1)[0]
    return f != UC_NONE && (f & 1) == 1
}

// 切相機模式（E0）：camMode 1~5、sub 子選項（照順序給色號填 0；物體偵測 1 清單、2 詳細）/ switch camera mode
function ucSetMode(camMode: number, sub: number): void {
    ucWrite(0xE0, [camMode, sub])
    basic.pause(50)
}

// 改取樣點位置（E1）：n 1~20、x 0~159、y 0~119 / move a sample spot
function ucSetPoint(n: number, x: number, y: number): void {
    ucWrite(0xE1, [n, x, y])
}

// 相機目前的模式編號（例 13）/ current mode number
function ucModeNumber(): number {
    return ucReadMany(0x01, 1)[0]
}

// 顏色追蹤每色幾個值 / values per colour (modes 11~14)
function ucStride(mode: number): number {
    if (mode == 11) return 2
    if (mode == 12) return 3
    if (mode == 13) return 4
    if (mode == 14) return 6
    return 0
}

// 色 color（1~16）的色塊：[x, y, 寬, 高, 塊數, 面積%]，x = 0 = 沒找到；這個模式沒給的是 -1
// blob of colour: [x, y, w, h, count, area%]; x = 0 = not found; -1 = not in this mode
function ucReadBlob(mode: number, color: number): number[] {
    const k = ucStride(mode)
    const d = ucReadMany(0x10 + (color - 1) * k, k)
    if (mode == 11) return [d[0], d[1], -1, -1, -1, -1]
    if (mode == 12) return [d[0], d[1], -1, -1, d[2], -1]
    if (mode == 13) return [d[0], d[1], -1, -1, d[3], d[2]]
    return [d[0], d[1], d[2], d[3], d[4], d[5]]
}

// 16 個目標色裡找到幾色 / colours found (modes 11~14)
function ucColorsFound(mode: number): number {
    return ucReadMany(0x10 + 16 * ucStride(mode), 1)[0]
}

// 取樣點 point（1~16）：[色相碼, 飽和度, 明度]；沒給的是 -1 / spot: [hue, sat, val]
function ucReadSpot(mode: number, point: number): number[] {
    const k = mode - 20
    const d = ucReadMany(0x10 + (point - 1) * k, k)
    return [d[0], k >= 2 ? d[1] : -1, k >= 3 ? d[2] : -1]
}

// 照順序給色號：由左到右每塊的色相碼（最多 16 塊）/ hue codes left to right
function ucReadOrder(): number[] {
    const total = ucReadMany(0x20, 1)[0]
    const d = ucReadMany(0x10, 16)
    if (total == UC_NONE) return []
    return d.slice(0, Math.min(16, total))
}

// 總塊數 / total blocks (mode 30)
function ucOrderTotal(): number {
    return ucReadMany(0x20, 1)[0]
}

// 循線 14 個值：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向]
// line values (near/mid/far offset: line on the left = negative)
function ucReadLine(): number[] {
    return ucReadMany(0x10, 14)
}

// 物體偵測每個物體幾個值：51 → 4（色號 形狀 X Y）、52 → 10 / values per object (modes 51, 52)
function ucObjStride(mode: number): number {
    if (mode == 51) return 4
    if (mode == 52) return 10
    return 0
}

// 物體 i（1~8，1 = 面積最大）：[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]
// 代碼 = 色號×10＋形狀，0 = 沒有；模式 51 沒給的是 -1
// object i: [code, colour, shape, x, y, w, h, area%, angle, vertices, trackId]; -1 = not in mode 51
function ucReadObject(mode: number, i: number): number[] {
    const k = ucObjStride(mode)
    const d = ucReadMany(0x10 + (i - 1) * k, k)
    const code = d[0] > 0 ? d[0] * 10 + d[1] : 0
    if (mode == 51) return [code, d[0], d[1], d[2], d[3], -1, -1, -1, -1, -1, -1]
    return [code, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]]
}

// 畫面裡有幾個物體 0~8 / number of objects in view (modes 51, 52)
function ucObjectCount(mode: number): number {
    return ucReadMany(0x10 + 8 * ucObjStride(mode), 1)[0]
}

// 形狀編號 → 英文名稱 / shape number -> name
function ucShapeName(s: number): string {
    const n = ["other", "circle", "ellipse", "triangle", "square", "rectangle", "pentagon", "hexagon", "star", "trapezoid"]
    return (s >= 0 && s <= 9) ? n[s] : "?"
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
function ucColorCode(h: number): number {
    if (h > 248) return 9
    if (h > 240) return 8
    if (h < 12 || h >= 220) return 1
    if (h < 28) return 2
    if (h < 56) return 3
    if (h < 100) return 4
    if (h < 140) return 5
    if (h < 180) return 6
    return 7
}
// ==== MBC_UNICAM_MB_HELPERS_END ====
```

**Step by step**
1. Wait for the camera, then `ucSetMode(2, 1)` switches to mode 21 (one value per spot: the hue code).
2. `ucReadSpot(MODE, 1)[0]`: hue code of S1, 0~255.
3. `ucColorCode(h)` sorts the hue code into 9 groups; `LETTERS.charAt(...)` picks the letter.
4. Button A: `ucSetPoint(1, 80, 60)` sends E1; S1 moves to the centre (saved in the camera). The cross on the camera screen moves too.
5. Button B: shows the hue code so you can tune your own colour borders.

**What you should see**
- After A, the S1 cross on the camera screen jumps to the centre and the micro:bit shows a target.
- Red block → R; yellow → Y; green → G; blue → B; white paper → W; black block → K.

**Common problems**
- **Red shows O, blue shows C?** → Lighting differs. Press B to see the number and add your own test outside the helpers (for example `if (h < 20) …`); do not change the helpers.
- **White paper shows a colour?** → White paper looks yellow under warm light. Run "white calibration" in the camera SET menu.

---

## Example 3: line-following car (PD + stop at a junction)

**What it does**: press A to follow a black line: line to the left → steer left, to the right → steer right (near and far offsets together, so it turns earlier on bends); at a **cross or T junction** it stops and shows a square; with no line in view it spins toward the side where it last saw the line. Press B to stop.

**You need**
- The parts of example 1
- A two-wheel car: one continuous-rotation servo per side + 4.8~6 V battery box
- A black line on white (about 2 cm wide) with a cross junction

**Camera setup**: camera on the front, looking forward and down at the floor. Camera mode **4 line following · option ① black line** (mode number 41) — switched automatically; for a white line change `ucSetMode(4, 1)` to `(4, 2)`, colour line `(4, 3)`.

**Wiring**: camera as in example 1; left servo signal → **P1**, right → **P2**; servo power from the battery box, battery minus to micro:bit GND.

**Download**: [ex03_line_follow.ts](/examples/unicam/microbit/en/ex03_line_follow.ts) (right-click, save) → paste into MakeCode JavaScript → Download.

**Program**

```typescript
// Sharpshooter Vision × micro:bit example 3: line-following car (near/far offset PD, stops at a junction)
// ★ Set the camera menu to camera mode 4 line following · option ① black line (mode number 41) (the program also switches it with E0 at start; white line = ②, colour line = ③)
// What it does: press A to follow a black line: line to the left → steer left, to the right → steer right (near + far offsets together, so it reacts to bends earlier);
//               at a cross or T junction it stops and shows a square; with no line in view it spins toward the side where it last saw the line. Press B to stop.
// Wiring: camera SDA → P20, SCL → P19, VCC → external 5 V, GND → GND. Left continuous-rotation servo signal → P1, right → P2;
//         servo power from a 4.8~6 V battery box, battery minus to micro:bit GND.
// Before you start: a two-wheel car, camera on the front looking forward and down at the floor; a black line on white (about 2 cm wide).
// Direction: standing behind the car looking forward, line on the left = negative, on the right = positive.
// ⚠ v1 not yet hardware-verified: BASE, KP and KD are suggestions; tune them for your car.

const BASE = 40                                   // 👉 You can change this: straight speed (-100~100)
const KP = 0.5                                    // 👉 You can change this: turn as much as the offset (P)
const KD = 0.3                                    // 👉 You can change this: extra turn when the offset changes fast (D)
let ready = false
let running = false
let lastErr = 0

// Left servo P1, right servo P2 (continuous rotation); left/right = −100~100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full backward
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right wheel is mounted the other way, so subtract
}

input.onButtonPressed(Button.A, function () {
    running = true
    lastErr = 0
    basic.showIcon(IconNames.Happy, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No, 0)
})

drive(0, 0)
basic.showIcon(IconNames.SmallDiamond)            // waiting for the camera

basic.forever(function () {
    if (!ready) {                                 // first time: wait for the camera, then switch to mode 41
        if (ucReady()) {
            ucSetMode(4, 1)                       // E0: camera mode 4, option 1 (black line)
            ready = true
            basic.showIcon(IconNames.Yes)
        } else {
            basic.showIcon(IconNames.No)          // no answer: check P19/P20 and the external power
            basic.pause(500)
        }
        return
    }
    if (!running) {
        basic.pause(100)
        return
    }
    const v = ucReadLine()                        // [near, mid, far, angle, width, line flags, junction, junction dist, …, lost side]
    if (v[0] == UC_NONE) {                        // cannot read the camera: safety first, stop
        drive(0, 0)
        basic.showIcon(IconNames.No, 0)
        return
    }
    const junction = v[6]                         // 0 none 1 cross 2 T 3 left 4 right 5 dead end
    if (junction == 1 || junction == 2) {         // cross or T junction → stop
        drive(0, 0)
        running = false
        basic.showIcon(IconNames.Square, 0)
        return
    }
    if (v[5] == 0) {                              // line flags = 0: no line near, middle or far
        if (v[13] == 1) {
            drive(-30, 30)                        // last seen on the left → spin left to find it
        } else {
            drive(30, -30)                        // last seen on the right (or unknown) → spin right
        }
        return
    }
    const err = v[0] * 0.7 + v[2] * 0.3           // near offset 70 %, far offset 30 % (line on the left = negative)
    const turn = KP * err + KD * (err - lastErr)  // PD: turn by the offset + by how fast it changes
    lastErr = err
    drive(BASE + turn, BASE - turn)               // line on the left (negative) → left wheel slower → steer left
    basic.pause(20)
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
// ===== UniCam helpers (MBC common I2C protocol v2): same in every example, no need to change =====
const UC_ADDR = 0x4C                     // 相機的 I2C 位址 / camera I2C address
const UC_NONE = -32768                   // 讀不到 / no value

// 從編號 id 起連續讀 k 個 int16（1~16）；讀不到全部是 UC_NONE
function ucReadMany(id: number, k: number): number[] {
    const cmd = pins.createBuffer(1)
    cmd[0] = id
    const r = pins.i2cWriteBuffer(UC_ADDR, cmd, false)
    basic.pause(2)                       // 相機（ESP32）要等 2 ms 再讀
    const out: number[] = []
    if (r) {                             // 實機：非 0 = 沒有回應
        for (let i = 0; i < k; i++) out.push(UC_NONE)
        return out
    }
    const b = pins.i2cReadBuffer(UC_ADDR, 2 * k, false)
    for (let i = 0; i < k; i++) out.push(b.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 寫命令 [id][參數…] / write a command
function ucWrite(id: number, params: number[]): void {
    const b = pins.createBuffer(1 + params.length)
    b[0] = id
    for (let i = 0; i < params.length; i++) b[1 + i] = params[i]
    pins.i2cWriteBuffer(UC_ADDR, b, false)
}

// 相機準備好了嗎（F3 bit0）/ is the camera ready
function ucReady(): boolean {
    const f = ucReadMany(0xF3, 1)[0]
    return f != UC_NONE && (f & 1) == 1
}

// 切相機模式（E0）：camMode 1~5、sub 子選項（照順序給色號填 0；物體偵測 1 清單、2 詳細）/ switch camera mode
function ucSetMode(camMode: number, sub: number): void {
    ucWrite(0xE0, [camMode, sub])
    basic.pause(50)
}

// 改取樣點位置（E1）：n 1~20、x 0~159、y 0~119 / move a sample spot
function ucSetPoint(n: number, x: number, y: number): void {
    ucWrite(0xE1, [n, x, y])
}

// 相機目前的模式編號（例 13）/ current mode number
function ucModeNumber(): number {
    return ucReadMany(0x01, 1)[0]
}

// 顏色追蹤每色幾個值 / values per colour (modes 11~14)
function ucStride(mode: number): number {
    if (mode == 11) return 2
    if (mode == 12) return 3
    if (mode == 13) return 4
    if (mode == 14) return 6
    return 0
}

// 色 color（1~16）的色塊：[x, y, 寬, 高, 塊數, 面積%]，x = 0 = 沒找到；這個模式沒給的是 -1
// blob of colour: [x, y, w, h, count, area%]; x = 0 = not found; -1 = not in this mode
function ucReadBlob(mode: number, color: number): number[] {
    const k = ucStride(mode)
    const d = ucReadMany(0x10 + (color - 1) * k, k)
    if (mode == 11) return [d[0], d[1], -1, -1, -1, -1]
    if (mode == 12) return [d[0], d[1], -1, -1, d[2], -1]
    if (mode == 13) return [d[0], d[1], -1, -1, d[3], d[2]]
    return [d[0], d[1], d[2], d[3], d[4], d[5]]
}

// 16 個目標色裡找到幾色 / colours found (modes 11~14)
function ucColorsFound(mode: number): number {
    return ucReadMany(0x10 + 16 * ucStride(mode), 1)[0]
}

// 取樣點 point（1~16）：[色相碼, 飽和度, 明度]；沒給的是 -1 / spot: [hue, sat, val]
function ucReadSpot(mode: number, point: number): number[] {
    const k = mode - 20
    const d = ucReadMany(0x10 + (point - 1) * k, k)
    return [d[0], k >= 2 ? d[1] : -1, k >= 3 ? d[2] : -1]
}

// 照順序給色號：由左到右每塊的色相碼（最多 16 塊）/ hue codes left to right
function ucReadOrder(): number[] {
    const total = ucReadMany(0x20, 1)[0]
    const d = ucReadMany(0x10, 16)
    if (total == UC_NONE) return []
    return d.slice(0, Math.min(16, total))
}

// 總塊數 / total blocks (mode 30)
function ucOrderTotal(): number {
    return ucReadMany(0x20, 1)[0]
}

// 循線 14 個值：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向]
// line values (near/mid/far offset: line on the left = negative)
function ucReadLine(): number[] {
    return ucReadMany(0x10, 14)
}

// 物體偵測每個物體幾個值：51 → 4（色號 形狀 X Y）、52 → 10 / values per object (modes 51, 52)
function ucObjStride(mode: number): number {
    if (mode == 51) return 4
    if (mode == 52) return 10
    return 0
}

// 物體 i（1~8，1 = 面積最大）：[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]
// 代碼 = 色號×10＋形狀，0 = 沒有；模式 51 沒給的是 -1
// object i: [code, colour, shape, x, y, w, h, area%, angle, vertices, trackId]; -1 = not in mode 51
function ucReadObject(mode: number, i: number): number[] {
    const k = ucObjStride(mode)
    const d = ucReadMany(0x10 + (i - 1) * k, k)
    const code = d[0] > 0 ? d[0] * 10 + d[1] : 0
    if (mode == 51) return [code, d[0], d[1], d[2], d[3], -1, -1, -1, -1, -1, -1]
    return [code, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]]
}

// 畫面裡有幾個物體 0~8 / number of objects in view (modes 51, 52)
function ucObjectCount(mode: number): number {
    return ucReadMany(0x10 + 8 * ucObjStride(mode), 1)[0]
}

// 形狀編號 → 英文名稱 / shape number -> name
function ucShapeName(s: number): string {
    const n = ["other", "circle", "ellipse", "triangle", "square", "rectangle", "pentagon", "hexagon", "star", "trapezoid"]
    return (s >= 0 && s <= 9) ? n[s] : "?"
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
function ucColorCode(h: number): number {
    if (h > 248) return 9
    if (h > 240) return 8
    if (h < 12 || h >= 220) return 1
    if (h < 28) return 2
    if (h < 56) return 3
    if (h < 100) return 4
    if (h < 140) return 5
    if (h < 180) return 6
    return 7
}
// ==== MBC_UNICAM_MB_HELPERS_END ====
```

**Step by step**
1. `drive(left, right)`: wheels −100~100 become servo angles (90 = stop); the right wheel is mirrored, so subtract.
2. `ucReadLine()` reads all 14 line values. `v[0]` = −32768 → the camera does not answer → stop.
3. `v[6]` junction: 1 cross, 2 T → stop, `running = false`.
4. `v[5]` line flags = 0 → no line near, middle or far → look at `v[13]` (lost side, 1 = left) and spin back.
5. Offset `err = near×0.7 + far×0.3`; `turn = KP×err + KD×(err − last)` (P = turn by the offset, D = turn more when it changes fast).
6. `drive(BASE + turn, BASE - turn)`: line on the left (negative) → left wheel slower, right faster → steer left.

**What you should see**: after A (smiley) the car follows the line and turns early on bends; at the cross it stops and shows a square. Lift the car off the line and it spins back to find it.

**Common problems**
- **Runs off and cannot come back?** → Increase `KP` (0.5 → 0.8); wobbles a lot → decrease `KP`, slow `BASE`.
- **Wrong direction (line on the left but it turns right)?** → Servos are swapped: swap the P1 and P2 wires, or swap `+`/`-` in the two lines of `drive`.
- **Stops before the junction?** → The camera looks far ahead and reports the junction while it is at the top of the picture. Check `v[7]` (junction distance %) and only stop above a number you choose.

---

## Example 4: which side is the red triangle (object detection)

**What it does**: the camera sorts the objects in the picture from largest to smallest (object 1 = largest) and gives each an "object code" = colour × 10 + shape. The program looks from the largest down for a "triangle of target 1" (code 13): on the left → left arrow; middle → up arrow; right → right arrow; not found → question mark. Press A to show how many objects are in view, B to scroll the shape name of object 1.

**You need**
- The parts of example 1
- A red triangle (card or bricks); add a red circle and a triangle of another colour to see that it is not fooled

**Camera setup**: teach "target 1" as red on the camera (the shape can stay unset; only targets 1~8 become objects). Camera mode **5 object detection · option ① object list** (mode number 51) — the program switches it with E0 at start.

**Wiring**: see the wiring table above.

**Download**: [ex04_object.ts](/examples/unicam/microbit/en/ex04_object.ts) (right-click, save) → paste into MakeCode JavaScript → Download.

**Program**

```typescript
// Sharpshooter Vision × micro:bit example 4: which side is the red triangle (object detection)
// ★ Set the camera menu to camera mode 5 object detection · option ① object list (mode number 51) (the program also switches it with E0 at start)
// What it does: the camera sorts objects from largest to smallest (object 1 = largest) and gives each an "object code" = colour × 10 + shape. It looks from the largest down for a "triangle of target 1" (code 13):
//   on the left → left arrow, middle → up arrow, right → right arrow; not found → question mark. Press A to show how many objects are in view, B to scroll the shape name of object 1.
// Shapes: 0 other 1 circle 2 ellipse 3 triangle 4 square 5 rectangle 6 pentagon 7 hexagon 8 star 9 trapezoid.
// Wiring: camera SDA → P20, SCL → P19, VCC → external 5 V (do not power it from the micro:bit 3V pin), GND → GND (common ground).
// Before you start: teach "target 1" as red on the camera (the shape can stay unset). MakeCode new project → switch to JavaScript (top right) → paste this whole file → Download. No extension needed.
// What you see: a tick at start; move a red triangle left and right in front of the lens and the arrow follows; a circle or another colour → question mark.
// ⚠ v1 not yet hardware-verified: the value layout is final and passed computer tests; real-hardware numbers may change.

const MODE = 51                                   // mode number = camera mode 5 × 10 + option 1
const WANT = 1 * 10 + 3                           // 👉 you can change: target 1 × 10 + shape 3 (triangle); square 14, circle 11
let ready = false
let busy = false

basic.showIcon(IconNames.SmallDiamond)            // waiting for the camera

// Look from the largest object down for code = WANT and return its X (0 = not found)
function findX(): number {
    const n = ucObjectCount(MODE)                 // objects in view, 0~8
    for (let i = 1; i <= n && i <= 8; i++) {
        const o = ucReadObject(MODE, i)           // [code, colour, shape, x, y, …]
        if (o[0] == WANT) return o[3]
    }
    return 0
}

basic.forever(function () {
    if (!ready) {                                 // first time: wait for the camera, then switch to mode 51
        if (ucReady()) {
            ucSetMode(5, 1)                       // E0: camera mode 5, option 1
            ready = true
            basic.showIcon(IconNames.Yes)
            basic.pause(500)
        } else {
            basic.showIcon(IconNames.No)          // no answer: check P19/P20 and the external power
            basic.pause(500)
        }
        return
    }
    if (busy) {
        basic.pause(100)
        return
    }
    const x = findX()                             // 0 = not found; 1~159 left → right
    if (x <= 0) {
        basic.showString("?", 0)
    } else if (x < 60) {                          // 👉 you can change: the left range
        basic.showArrow(ArrowNames.West, 0)
    } else if (x > 100) {                         // 👉 you can change: the right range
        basic.showArrow(ArrowNames.East, 0)
    } else {
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(ucObjectCount(MODE))         // objects in view, 0~8
    busy = false
})

input.onButtonPressed(Button.B, function () {
    busy = true
    const o = ucReadObject(MODE, 1)               // object 1 (the largest)
    basic.showString(o[0] > 0 ? ucShapeName(o[2]) : "-")   // shape name; "-" when there is no object
    busy = false
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
// ===== UniCam helpers (MBC common I2C protocol v2): same in every example, no need to change =====
const UC_ADDR = 0x4C                     // 相機的 I2C 位址 / camera I2C address
const UC_NONE = -32768                   // 讀不到 / no value

// 從編號 id 起連續讀 k 個 int16（1~16）；讀不到全部是 UC_NONE
function ucReadMany(id: number, k: number): number[] {
    const cmd = pins.createBuffer(1)
    cmd[0] = id
    const r = pins.i2cWriteBuffer(UC_ADDR, cmd, false)
    basic.pause(2)                       // 相機（ESP32）要等 2 ms 再讀
    const out: number[] = []
    if (r) {                             // 實機：非 0 = 沒有回應
        for (let i = 0; i < k; i++) out.push(UC_NONE)
        return out
    }
    const b = pins.i2cReadBuffer(UC_ADDR, 2 * k, false)
    for (let i = 0; i < k; i++) out.push(b.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// 寫命令 [id][參數…] / write a command
function ucWrite(id: number, params: number[]): void {
    const b = pins.createBuffer(1 + params.length)
    b[0] = id
    for (let i = 0; i < params.length; i++) b[1 + i] = params[i]
    pins.i2cWriteBuffer(UC_ADDR, b, false)
}

// 相機準備好了嗎（F3 bit0）/ is the camera ready
function ucReady(): boolean {
    const f = ucReadMany(0xF3, 1)[0]
    return f != UC_NONE && (f & 1) == 1
}

// 切相機模式（E0）：camMode 1~5、sub 子選項（照順序給色號填 0；物體偵測 1 清單、2 詳細）/ switch camera mode
function ucSetMode(camMode: number, sub: number): void {
    ucWrite(0xE0, [camMode, sub])
    basic.pause(50)
}

// 改取樣點位置（E1）：n 1~20、x 0~159、y 0~119 / move a sample spot
function ucSetPoint(n: number, x: number, y: number): void {
    ucWrite(0xE1, [n, x, y])
}

// 相機目前的模式編號（例 13）/ current mode number
function ucModeNumber(): number {
    return ucReadMany(0x01, 1)[0]
}

// 顏色追蹤每色幾個值 / values per colour (modes 11~14)
function ucStride(mode: number): number {
    if (mode == 11) return 2
    if (mode == 12) return 3
    if (mode == 13) return 4
    if (mode == 14) return 6
    return 0
}

// 色 color（1~16）的色塊：[x, y, 寬, 高, 塊數, 面積%]，x = 0 = 沒找到；這個模式沒給的是 -1
// blob of colour: [x, y, w, h, count, area%]; x = 0 = not found; -1 = not in this mode
function ucReadBlob(mode: number, color: number): number[] {
    const k = ucStride(mode)
    const d = ucReadMany(0x10 + (color - 1) * k, k)
    if (mode == 11) return [d[0], d[1], -1, -1, -1, -1]
    if (mode == 12) return [d[0], d[1], -1, -1, d[2], -1]
    if (mode == 13) return [d[0], d[1], -1, -1, d[3], d[2]]
    return [d[0], d[1], d[2], d[3], d[4], d[5]]
}

// 16 個目標色裡找到幾色 / colours found (modes 11~14)
function ucColorsFound(mode: number): number {
    return ucReadMany(0x10 + 16 * ucStride(mode), 1)[0]
}

// 取樣點 point（1~16）：[色相碼, 飽和度, 明度]；沒給的是 -1 / spot: [hue, sat, val]
function ucReadSpot(mode: number, point: number): number[] {
    const k = mode - 20
    const d = ucReadMany(0x10 + (point - 1) * k, k)
    return [d[0], k >= 2 ? d[1] : -1, k >= 3 ? d[2] : -1]
}

// 照順序給色號：由左到右每塊的色相碼（最多 16 塊）/ hue codes left to right
function ucReadOrder(): number[] {
    const total = ucReadMany(0x20, 1)[0]
    const d = ucReadMany(0x10, 16)
    if (total == UC_NONE) return []
    return d.slice(0, Math.min(16, total))
}

// 總塊數 / total blocks (mode 30)
function ucOrderTotal(): number {
    return ucReadMany(0x20, 1)[0]
}

// 循線 14 個值：[近, 中, 遠偏移, 角度, 線寬, 有線旗標, 路口類型, 路口距離, 左緣, 右緣, 遠線寬, 標記色號, 標記條數, 離線方向]
// line values (near/mid/far offset: line on the left = negative)
function ucReadLine(): number[] {
    return ucReadMany(0x10, 14)
}

// 物體偵測每個物體幾個值：51 → 4（色號 形狀 X Y）、52 → 10 / values per object (modes 51, 52)
function ucObjStride(mode: number): number {
    if (mode == 51) return 4
    if (mode == 52) return 10
    return 0
}

// 物體 i（1~8，1 = 面積最大）：[代碼, 色號, 形狀, x, y, 寬, 高, 面積%, 角度, 頂點數, 追蹤編號]
// 代碼 = 色號×10＋形狀，0 = 沒有；模式 51 沒給的是 -1
// object i: [code, colour, shape, x, y, w, h, area%, angle, vertices, trackId]; -1 = not in mode 51
function ucReadObject(mode: number, i: number): number[] {
    const k = ucObjStride(mode)
    const d = ucReadMany(0x10 + (i - 1) * k, k)
    const code = d[0] > 0 ? d[0] * 10 + d[1] : 0
    if (mode == 51) return [code, d[0], d[1], d[2], d[3], -1, -1, -1, -1, -1, -1]
    return [code, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]]
}

// 畫面裡有幾個物體 0~8 / number of objects in view (modes 51, 52)
function ucObjectCount(mode: number): number {
    return ucReadMany(0x10 + 8 * ucObjStride(mode), 1)[0]
}

// 形狀編號 → 英文名稱 / shape number -> name
function ucShapeName(s: number): string {
    const n = ["other", "circle", "ellipse", "triangle", "square", "rectangle", "pentagon", "hexagon", "star", "trapezoid"]
    return (s >= 0 && s <= 9) ? n[s] : "?"
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
function ucColorCode(h: number): number {
    if (h > 248) return 9
    if (h > 240) return 8
    if (h < 12 || h >= 220) return 1
    if (h < 28) return 2
    if (h < 56) return 3
    if (h < 100) return 4
    if (h < 140) return 5
    if (h < 180) return 6
    return 7
}
// ==== MBC_UNICAM_MB_HELPERS_END ====
```

**Step by step**
1. `WANT = 1 × 10 + 3`: object code = colour × 10 + shape, so a triangle of target 1 is 13 (square 14, circle 11).
2. `ucObjectCount(MODE)`: how many objects are in view (0~8).
3. `ucReadObject(MODE, i)` → `[code, colour, shape, x, y, …]`; object 1 is the largest, so the first match from 1 down is the biggest. Mode 51 only gives code, colour, shape, X, Y; the rest are −1.
4. `ucShapeName(o[2])`: shape number → English name (the LEDs can only scroll plain letters).

**What you should see**: a tick at start; red triangle on the left → left arrow, on the right → right arrow; a red circle or a blue triangle → question mark. B scrolls a word such as `triangle`.

**Common problems**
- **Always a question mark?** → The top right of the camera screen must show `5-1`; look at the "colour shape" label next to each object on the camera screen. If the triangle is read as another shape, set the shape of target 1 to Tri on the camera, or raise **ShapeTol** in the SET menu.
- **Want size, area, angle?** → Use `ucSetMode(5, 2)` and `MODE = 52`: `ucReadObject` then also gives width, height, area %, angle 0~179, vertices and track id.

---

## Next

- Every value in detail: [value reference (I2C)](/en/sensors/unicam/reference.md#host-i2c)
- Helper master files, Arduino version and other hosts: [downloads](/en/downloads.md)
- Product page and camera menu: [Sharpshooter Vision](/en/sensors/unicam/index.md)
