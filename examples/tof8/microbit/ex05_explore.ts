// 測距者 8 路 × micro:bit 範例 5：自動探索（往最空曠的方向走，綜合）
// 做什麼：車子一直往「最空曠的方向」偏過去走；12 cm 內有東西 → 倒退、往空曠那邊原地轉；每次閃骷髏並計數。A 開始、B 停。
// 接線：同範例 4（感應器 P19／P20、外接 5 V；左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：兩輪小車＋幾個箱子圍成迷宮。
// 看到什麼：按 A 後車子自己在箱子間繞，不會撞上去；按 B 停下來顯示躲了幾次。

const STOP_CM = 12
let running = false
let dodges = 0

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

mbc.onTof8Near(STOP_CM, function () {                     // 事件：每次進到 12 cm 內觸發一次
    dodges += 1
    basic.showIcon(IconNames.Skull, 0)
})

input.onButtonPressed(Button.A, function () {
    dodges = 0
    running = true
    basic.showArrow(ArrowNames.North, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showNumber(dodges)
})

basic.forever(function () {
    if (!running) {
        basic.pause(100)
        return
    }
    let near = mbc.tof8Nearest(MbcDistUnit.Cm)
    let open = mbc.tof8OpenDirection()                    // 最空曠的方向 −7~+7，負 = 左
    if (near < STOP_CM) {
        drive(-40, -40)                                   // 倒退 0.4 秒
        basic.pause(400)
        if (open < 0) {
            drive(-40, 40)                                // 空曠在左 → 原地左轉
        } else {
            drive(40, -40)
        }
        basic.pause(500)
    } else {
        let turn = Math.constrain(open * 8, -40, 40)      // 空曠在左（負）→ 左輪慢 → 往左偏
        drive(50 + turn, 50 - turn)
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(30)
})
