// 測距者 2 路 × micro:bit 範例 5：避障小車（綜合）
// 做什麼：車子一直往前走；15 cm 內有東西 → 倒退一下，往比較空的那一邊原地轉，再繼續走；LED 顯示躲了幾次。A 開始、B 停。
// 接線：同範例 4（感應器 P19／P20、左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：兩輪小車＋幾個箱子當障礙物。
// 看到什麼：按 A 車子前進，碰到障礙物前先倒退、轉彎、繼續；每躲一次 LED 數字 +1；進到 15 cm 內瞬間會閃一個骷髏。

const STOP_CM = 15
let running = false
let detours = 0

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

mbc.onTof2Near(STOP_CM, function () {                     // 事件：每次進到 15 cm 內閃一下
    basic.showIcon(IconNames.Skull, 0)
})

input.onButtonPressed(Button.A, function () {
    detours = 0
    running = true
    basic.showArrow(ArrowNames.North, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    if (!running) {
        basic.pause(100)
        return
    }
    let l = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)
    let r = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)
    if (Math.min(l, r) < STOP_CM) {
        detours += 1
        drive(-50, -50)                                   // 先倒退 0.4 秒
        basic.pause(400)
        if (l > r) {
            drive(-40, 40)                                // 左邊比較空 → 原地左轉
        } else {
            drive(40, -40)                                // 原地右轉
        }
        basic.pause(500)
        basic.showNumber(detours)
    } else {
        drive(50, 50)                                     // 前面沒東西：直走
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(30)
})
