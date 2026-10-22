// 測距者 2 路 × micro:bit 範例 4：跟著我走的小車
// 做什麼：車子跟前面的東西（你的手或一塊板子）保持 20 cm：太遠就前進、太近就後退，東西偏左就左轉。按 A 開始、B 停。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，感應器裝在車頭朝前。
// 看到什麼：按 A 後把手放在車前 20 cm：手往後退車就跟上來，手靠近車就倒退；手拿開（超過 60 cm）車子停下、顯示問號臉。

const TARGET = 20                                         // 想保持的距離（cm）
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
    basic.showIcon(IconNames.Happy, 0)
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
    let d = Math.min(l, r)                                // 較近的那顆
    if (d >= 60) {
        drive(0, 0)                                       // 太遠：東西不見了，停
        basic.showIcon(IconNames.Confused, 0)
    } else {
        let speed = Math.constrain((d - TARGET) * 4, -60, 60)   // 比 20 cm 遠 → 正 = 前進；近 → 負 = 後退
        let turn = Math.constrain((r - l) * 2, -30, 30)         // 右邊那顆比較遠 → 東西偏左 → 左轉
        drive(speed - turn, speed + turn)
        if (turn < -5) {
            basic.showArrow(ArrowNames.East, 0)
        } else if (turn > 5) {
            basic.showArrow(ArrowNames.West, 0)
        } else {
            basic.showArrow(ArrowNames.North, 0)
        }
    }
    basic.pause(30)
})
