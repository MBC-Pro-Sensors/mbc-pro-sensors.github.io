// 神攝手 × micro:bit 範例 2：取樣點看到什麼顏色
// ★ 需要把相機選單切到 相機模式 2 座標取色 ・子選項 ① 色相（模式編號 21）（程式開頭也會用 E0 自動切）
// 做什麼：LED 顯示取樣點 S1 看到的顏色字母：R 紅、O 橙、Y 黃、G 綠、C 青、B 藍、P 紫、W 白／灰、K 黑。
//         按 A：用 E1 命令把 S1 移到畫面正中間（X 80、Y 60）；按 B：顯示色相碼數字（0~255）。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND 共地。
// 準備：紅、黃、綠、藍、白、黑的積木或色紙。MakeCode 切到 JavaScript 貼上本檔 → 下載。不用加擴充。
// 看到什麼：先按 A，把積木放在鏡頭正中間：紅 → R、綠 → G、藍 → B、白紙 → W。
// ⚠ v1 尚未上機驗證：顏色的分界（ucColorCode）是依色相碼估的，每個場地光線不同，請按 B 看數字自己調。

const MODE = 21                                   // 模式編號 = 相機模式 2 × 10 + 子選項 1
const LETTERS = "?ROYGCBPWK"                      // ucColorCode 1~9 對應的字母
let ready = false
let busy = false

basic.showIcon(IconNames.SmallDiamond)            // 等相機

basic.forever(function () {
    if (!ready) {                                 // 第一次：等相機準備好，再切到模式 21
        if (ucReady()) {
            ucSetMode(2, 1)                       // E0：相機模式 2、子選項 1
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
    const h = ucReadSpot(MODE, 1)[0]              // S1 的色相碼 0~255
    if (h == UC_NONE) {
        basic.showString("-", 0)                  // 讀不到
    } else {
        basic.showString(LETTERS.charAt(ucColorCode(h)), 0)
    }
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    ucSetPoint(1, 80, 60)                         // E1：第 1 點移到 X 80、Y 60（畫面中間，會存進相機）
    basic.showIcon(IconNames.Target)
    basic.pause(500)
    busy = false
})

input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(ucReadSpot(MODE, 1)[0])      // 色相碼：紅 0/240、黃 40、綠 80、青 120、藍 160、紫 200；241~248 白、249~255 黑
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

// 切相機模式（E0）：camMode 1~4、sub 子選項（照順序給色號填 0）/ switch camera mode
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
