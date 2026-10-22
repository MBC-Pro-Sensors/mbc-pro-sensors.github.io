// 掌控者 PS2 × micro:bit 範例 4：方向鍵開車
// 做什麼：按住方向鍵車子就往那個方向走，放開就停：上 = 前進、下 = 後退、左 = 原地左轉、右 = 原地右轉。手把斷線車子自動停。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。左輪連續旋轉伺服訊號 → P1、右輪 → P2；伺服電源 4.8~6 V 電池盒，電池負極接 micro:bit GND。
// 準備：兩輪小車（兩顆連續旋轉伺服當輪子）。
// 看到什麼：按住上 → 車子前進、LED 朝上箭頭；放開 → 停；手把關掉 → X、車停。

// 左輪伺服 P1、右輪伺服 P2（連續旋轉伺服）；left／right = −100~100，正 = 前進
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = 停、180 = 全速前進、0 = 全速後退
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // 右輪裝的方向相反，所以用減的
}

basic.forever(function () {
    if (!mbc.padConnected()) {                            // 斷線：停車
        drive(0, 0)
        basic.showIcon(IconNames.No, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Up)) {
        drive(50, 50)                                     // 前進
        basic.showArrow(ArrowNames.North, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Down)) {
        drive(-50, -50)                                   // 後退
        basic.showArrow(ArrowNames.South, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Left)) {
        drive(-30, 30)                                    // 左輪往後、右輪往前 = 原地左轉
        basic.showArrow(ArrowNames.West, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Right)) {
        drive(30, -30)                                    // 原地右轉
        basic.showArrow(ArrowNames.East, 0)
    } else {
        drive(0, 0)                                       // 都沒按：停
        basic.clearScreen()
    }
    basic.pause(50)
})
