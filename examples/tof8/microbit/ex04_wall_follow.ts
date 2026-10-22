// 測距者 8 路 × micro:bit 範例 4：沿牆走（左牆）
// 做什麼：車子靠左邊的牆走，用最左那顆（S1）保持離牆 15 cm；正前方 15 cm 內有牆就原地右轉。A 開始、B 停。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，感應器裝在車頭（S1 朝正左、S4／S5 朝正前）；一面牆或一排箱子。
// 看到什麼：按 A 車子沿著左牆走，離牆太遠就往左靠、太近就往右閃；前面遇到牆角就右轉繼續。

const WALL_CM = 15
let running = false

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

input.onButtonPressed(Button.A, function () {
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
    let side = mbc.tof8Sensor(1, MbcDistUnit.Cm)          // 第 1 顆 = 正左
    let front = mbc.tof8Front(MbcDistUnit.Cm)             // 正前方（第 4、5 顆平均）
    if (front < WALL_CM) {
        drive(40, -40)                                    // 前面有牆：原地右轉
        basic.showArrow(ArrowNames.East, 0)
    } else {
        let err = side - WALL_CM                          // 正 = 離牆太遠
        let turn = Math.constrain(err * 3, -30, 30)
        drive(50 - turn, 50 + turn)                       // 太遠 → 左輪慢 → 往左靠
        if (turn > 5) {
            basic.showArrow(ArrowNames.NorthWest, 0)
        } else if (turn < -5) {
            basic.showArrow(ArrowNames.NorthEast, 0)
        } else {
            basic.showArrow(ArrowNames.North, 0)
        }
    }
    basic.pause(30)
})
