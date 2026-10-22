// 神攝手 ESP32CAM（學校版）× micro:bit 範例 4：追著色塊跑的小車
// 做什麼：色塊在畫面右邊 → 右轉；色塊很小（很遠）→ 前進；很大（很近）→ 後退；沒看到 → 停。A 開始、B 停。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車，相機朝前；相機做過「設定 2：TRACK」；一顆目標色的球或積木。
// 看到什麼：按 A 後把球放在車前面：球往後退車就追上去，球靠近車就倒退，球移到旁邊車就轉過去；球拿走 → 停、問號臉。

const FAR_AREA = 800                                      // 面積比這小 = 太遠 → 前進（依你的球大小調整）
const NEAR_AREA = 2500                                    // 面積比這大 = 太近 → 後退
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
    if (!mbc.camFound()) {
        drive(0, 0)
        basic.showIcon(IconNames.Confused, 0)
    } else {
        let x = mbc.camBlob(MbcCamBlob.X)                 // 0（左）~ 100（右）
        let area = mbc.camBlob(MbcCamBlob.Area)           // 寬 × 高，0~7500：越大越近
        let turn = Math.constrain((x - 50), -40, 40)      // 色塊在右 → 正 → 左輪快 = 右轉
        let speed = 0
        if (area < FAR_AREA) {
            speed = 50                                    // 太遠：前進
        } else if (area > NEAR_AREA) {
            speed = -40                                   // 太近：後退
        }
        drive(speed + turn, speed - turn)
        if (turn > 10) {
            basic.showArrow(ArrowNames.East, 0)
        } else if (turn < -10) {
            basic.showArrow(ArrowNames.West, 0)
        } else if (speed > 0) {
            basic.showArrow(ArrowNames.North, 0)
        } else if (speed < 0) {
            basic.showArrow(ArrowNames.South, 0)
        } else {
            basic.showIcon(IconNames.Target, 0)           // 距離剛好
        }
    }
    basic.pause(40)
})
