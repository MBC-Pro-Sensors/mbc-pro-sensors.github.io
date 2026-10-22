// 陀螺儀 × micro:bit 範例 4：走直線（陀螺儀修正）
// 做什麼：按 A 車子往前走，偏掉就用陀螺儀的航向修回來，所以能走得很直；按 B 停。LED 顯示往哪邊修。
// 接線：陀螺儀 SDA → P20、SCL → P19、VCC → 3V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，陀螺儀平放在車上（絲印箭頭朝前）。
// 看到什麼：按 A 車子直直走；用手把車頭撥歪，它會自己轉回原本的方向繼續走。

const KP = 2                                              // 偏 1° 修正 2% 速度差
let running = false

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

input.onButtonPressed(Button.A, function () {
    mbc.imuZeroHeading()                                  // 出發方向 = 0°
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
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // 正 = 車頭偏右了
    let turn = Math.constrain(h * KP, -40, 40)
    drive(50 - turn, 50 + turn)                           // 偏右 → 左輪慢、右輪快 → 轉回左
    if (turn > 5) {
        basic.showArrow(ArrowNames.NorthWest, 0)
    } else if (turn < -5) {
        basic.showArrow(ArrowNames.NorthEast, 0)
    } else {
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(20)
})
