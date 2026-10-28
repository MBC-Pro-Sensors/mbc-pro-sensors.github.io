// 神攝手 × micro:bit 範例 4：紅色三角形在哪邊（物體偵測）
// ★ 需要把相機選單切到 相機模式 5 物體偵測 ・子選項 ① 物體清單（模式編號 51）（程式開頭也會用 E0 自動切）
// 做什麼：相機把物體由大到小排好（物體 1 最大），每個物體給「物體代碼」＝色號 × 10 ＋ 形狀。從最大的往下找「目標 1 的三角形」（代碼 13）：
//   在左邊 → 往左箭頭、中間 → 向上箭頭、右邊 → 往右箭頭；沒找到 → 問號。按 A 顯示畫面裡有幾個物體，按 B 捲動物體 1 的形狀名稱。
// 形狀：0 其他 1 圓 2 橢圓 3 三角 4 正方 5 長方 6 五邊 7 六邊 8 星 9 梯形。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V（不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：在相機機身把「目標 1」教成紅色（形狀可以不指定）。MakeCode 新專案 → 右上切到 JavaScript → 把本檔全部貼上 → 下載。不用加擴充。
// 看到什麼：開機打勾；拿紅色三角形在鏡頭前左右移動，箭頭跟著換方向；換成圓形或別的顏色 → 問號。
// ⚠ v1 尚未上機驗證：數值排法已定案並通過電腦端測試，實機數值上機後可能調整。

const MODE = 51                                   // 模式編號 = 相機模式 5 × 10 + 子選項 1
const WANT = 1 * 10 + 3                           // 👉 可以改：目標 1 × 10 ＋ 形狀 3（三角）；正方形 14、圓 11
let ready = false
let busy = false

basic.showIcon(IconNames.SmallDiamond)            // 等相機

// 從最大的物體往下找代碼 = WANT 的，回傳它的 X（0 = 沒找到）
function findX(): number {
    const n = ucObjectCount(MODE)                 // 畫面裡有幾個物體 0~8
    for (let i = 1; i <= n && i <= 8; i++) {
        const o = ucReadObject(MODE, i)           // [代碼, 色號, 形狀, x, y, …]
        if (o[0] == WANT) return o[3]
    }
    return 0
}

basic.forever(function () {
    if (!ready) {                                 // 第一次：等相機準備好，再切到模式 51
        if (ucReady()) {
            ucSetMode(5, 1)                       // E0：相機模式 5、子選項 1
            ready = true
            basic.showIcon(IconNames.Yes)
            basic.pause(500)
        } else {
            basic.showIcon(IconNames.No)          // 沒回應：檢查 P19／P20、外接電源
            basic.pause(500)
        }
        return
    }
    if (busy) {
        basic.pause(100)
        return
    }
    const x = findX()                             // 0 = 沒找到；1~159 左 → 右
    if (x <= 0) {
        basic.showString("?", 0)
    } else if (x < 60) {                          // 👉 可以改：左邊的範圍
        basic.showArrow(ArrowNames.West, 0)
    } else if (x > 100) {                         // 👉 可以改：右邊的範圍
        basic.showArrow(ArrowNames.East, 0)
    } else {
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(ucObjectCount(MODE))         // 畫面裡有幾個物體 0~8
    busy = false
})

input.onButtonPressed(Button.B, function () {
    busy = true
    const o = ucReadObject(MODE, 1)               // 物體 1（最大的）
    basic.showString(o[0] > 0 ? ucShapeName(o[2]) : "-")   // 形狀名稱（英文）；沒有物體顯示 -
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
