// 循行者 16 路 × micro:bit 範例 4：循線小車（三段式）
// 做什麼：按 A 開車：線在左 → 左轉、線在右 → 右轉、正中間 → 直走；丟線 → 停。按 B 停車。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；兩顆伺服的電源接 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車（兩顆連續旋轉伺服當輪子）、感應器裝在車頭離地 0.5~1 cm、黑線跑道。
// 看到什麼：按 A 後箭頭朝上、車子沿線走；轉彎時箭頭指向轉彎方向；拿開線 → 哭臉、停車。

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
    if (!mbc.lineSeen()) {
        drive(0, 0)                                       // 丟線就停，才不會跑出跑道
        basic.showIcon(IconNames.Sad, 0)
    } else {
        let pos = mbc.linePosition()                      // 負 = 線在左
        if (pos < 0) {
            drive(20, 60)                                 // 左輪慢、右輪快 = 左轉
            basic.showArrow(ArrowNames.West, 0)
        } else if (pos > 0) {
            drive(60, 20)                                 // 右轉
            basic.showArrow(ArrowNames.East, 0)
        } else {
            drive(50, 50)                                 // 直走
            basic.showArrow(ArrowNames.North, 0)
        }
    }
    basic.pause(20)
})
