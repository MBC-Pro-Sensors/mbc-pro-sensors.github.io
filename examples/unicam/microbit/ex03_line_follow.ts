// 神攝手通用版 v1 × micro:bit 範例 3：循線小車（近／遠偏移 PD 循線，遇到路口停車）
// ★ 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線（模式編號 41）（程式開頭也會用 E0 自動切；白線改 ②、色線改 ③）
// 做什麼：按 A 開始沿黑線走：線偏左就往左修、偏右就往右修（近處＋遠處偏移一起看，轉彎比較早反應）；
//         遇到十字路口或 T 字路口就停車、顯示方塊；看不到線就往最後看到線的那一邊原地轉。按 B 停。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；
//       伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，相機裝在車頭、斜斜往前下方看地面；白底黑線的場地（線寬約 2 cm）。
// 方向：站在車子後面往前看，線在左 = 負、線在右 = 正。
// ⚠ v1 尚未上機驗證：BASE、KP、KD 是建議值，請依你的車子調。

const BASE = 40                                   // 👉 可以改：直走速度（-100~100）
const KP = 0.5                                    // 👉 可以改：偏多少就轉多少（P）
const KD = 0.3                                    // 👉 可以改：偏移變化快就多修一點（D）
let ready = false
let running = false
let lastErr = 0

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
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
basic.showIcon(IconNames.SmallDiamond)            // 等相機

basic.forever(function () {
    if (!ready) {                                 // 第一次：等相機準備好，再切到模式 41
        if (ucReady()) {
            ucSetMode(4, 1)                       // E0：相機模式 4、子選項 1（黑線）
            ready = true
            basic.showIcon(IconNames.Yes)
        } else {
            basic.showIcon(IconNames.No)          // 沒回應：檢查 P19／P20、外接電源
            basic.pause(500)
        }
        return
    }
    if (!running) {
        basic.pause(100)
        return
    }
    const v = ucReadLine()                        // [近, 中, 遠, 角度, 線寬, 有線旗標, 路口類型, 路口距離, …, 離線方向]
    if (v[0] == UC_NONE) {                        // 讀不到相機：安全第一，停車
        drive(0, 0)
        basic.showIcon(IconNames.No, 0)
        return
    }
    const junction = v[6]                         // 0 無 1 十字 2 T 3 左 4 右 5 盡頭
    if (junction == 1 || junction == 2) {         // 十字或 T 字路口 → 停車
        drive(0, 0)
        running = false
        basic.showIcon(IconNames.Square, 0)
        return
    }
    if (v[5] == 0) {                              // 有線旗標 = 0：近中遠都沒看到線
        if (v[13] == 1) {
            drive(-30, 30)                        // 最後在左邊看到線 → 原地左轉找線
        } else {
            drive(30, -30)                        // 最後在右邊（或不知道）→ 原地右轉
        }
        return
    }
    const err = v[0] * 0.7 + v[2] * 0.3           // 近處偏移占 7 成、遠處占 3 成（線在左 = 負）
    const turn = KP * err + KD * (err - lastErr)  // PD：偏多少修多少＋偏移變多快
    lastErr = err
    drive(BASE + turn, BASE - turn)               // 線在左（負）→ 左輪慢、右輪快 → 往左修
    basic.pause(20)
})

// ==== MBC_UNICAM_MB_HELPERS_BEGIN ====
// ===== 神攝手通用版 小幫手（MBC 通用 I2C 協議 v2）：所有範例都一樣，不用改 =====
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
