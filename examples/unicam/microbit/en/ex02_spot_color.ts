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
