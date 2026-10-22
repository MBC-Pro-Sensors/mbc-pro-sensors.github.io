// 循行者 8 路 × micro:bit 範例 5：平滑循線＋數路口停車（綜合）
// 做什麼：用「細位置」做比例控制，線越偏修正越多，走起來不抖；遇到十字路口就計數，第 3 個路口停車；丟線時用「最後離線方向」原地轉回去找線。A 開始、B 停。
// 接線：同範例 4（感應器 P19／P20、左輪 P1、右輪 P2、伺服外接電池、共地）。
// 準備：畫了 3 個十字路口的黑線跑道。
// 看到什麼：按 A 車子平順沿線走，每過一個路口 LED 顯示 1、2、3，到 3 停車打勾；把線拿開，車子會往線消失的那一邊轉，找到線再繼續。

const SPEED = 50
const KP = 0.5                                            // 細位置 ±100 → 修正 ±50
let running = false
let crossings = 0

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

mbc.onLineEvent(MbcLineEvent.Fork, function () {          // 線段數 ≥ 2：十字或分岔
    if (!running) {
        return
    }
    crossings += 1
    basic.showNumber(crossings)
    if (crossings >= 3) {
        running = false
        drive(0, 0)
        basic.showIcon(IconNames.Yes)
    }
})
mbc.onLineEvent(MbcLineEvent.Lost, function () {
    if (!running) {
        return
    }
    if (mbc.lineLastDirection() < 0) {                    // 線從左邊跑掉 → 原地左轉找線
        drive(-30, 30)
    } else {
        drive(30, -30)
    }
    basic.showIcon(IconNames.Confused, 0)
})

input.onButtonPressed(Button.A, function () {
    crossings = 0
    running = true
    basic.showArrow(ArrowNames.North, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    if (running && mbc.lineSeen()) {
        let err = mbc.linePositionFine()                  // −100~+100，負 = 線在左
        let turn = err * KP                               // 線在左 → turn 是負的 → 左輪減速 = 左轉
        drive(SPEED + turn, SPEED - turn)
    }
    basic.pause(20)
})
