// 陀螺儀 × micro:bit 範例 5：走正方形（綜合）
// 做什麼：按 A：直走 1.5 秒（用陀螺儀保持直線）→ 原地右轉剛好 90° → 重複 4 次回到原點，LED 顯示第幾邊；按 B 中途停。
// 接線：同範例 4（陀螺儀 P19／P20；左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：兩輪小車＋約 1 公尺見方的空地。
// 看到什麼：按 A 車子走出一個正方形、顯示 1 2 3 4，最後停下來打勾。轉彎不足或過頭 → 把 TURN_SPEED 調小。

const SIDE_MS = 1500                                      // 每邊走多久
const TURN_SPEED = 35
let running = false

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

// 直走 ms 毫秒，用陀螺儀保持在 target 這個航向
function straight(target: number, ms: number): void {
    let t0 = control.millis()
    while (running && control.millis() - t0 < ms) {
        let err = mbc.imuHeading(MbcImuHeading.Fused) - target
        let turn = Math.constrain(err * 2, -40, 40)
        drive(50 - turn, 50 + turn)
        basic.pause(20)
    }
}

// 原地轉到 target 航向（誤差 3° 內就停）
function turnTo(target: number): void {
    while (running) {
        let err = target - mbc.imuHeading(MbcImuHeading.Fused)   // 正 = 還要往右轉
        if (Math.abs(err) < 3) {
            break
        }
        let s = err > 0 ? TURN_SPEED : -TURN_SPEED
        drive(s, -s)                                      // 左輪前、右輪後 = 右轉
        basic.pause(20)
    }
    drive(0, 0)
    basic.pause(300)                                      // 停一下讓車身穩定
}

input.onButtonPressed(Button.A, function () {
    if (running) {
        return
    }
    running = true
    mbc.imuZeroHeading()
    let target = 0
    for (let side = 1; side <= 4; side++) {
        basic.showNumber(side)
        straight(target, SIDE_MS)
        target += 90                                      // 下一邊的航向
        turnTo(target)
    }
    drive(0, 0)
    if (running) {
        basic.showIcon(IconNames.Yes)
    }
    running = false
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No)
})
