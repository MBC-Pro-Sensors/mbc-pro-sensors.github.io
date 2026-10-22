// 陀螺儀 × micro:bit 範例 3：穩定雲台（伺服永遠指著同一個方向）
// 做什麼：伺服裝在陀螺儀旁邊、上面貼箭頭：轉動底座，伺服會反方向轉回去，箭頭一直指著原本的方向（±90° 內）。按 A 重設「原本的方向」。
// 接線：陀螺儀 SDA → P20、SCL → P19、VCC → 3V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：9g 伺服（SG90）、一塊板子把 micro:bit、陀螺儀、伺服固定在一起。
// 看到什麼：底座往右轉 45° → 伺服往左轉 45°，箭頭還是指著原方向；轉超過 90° 伺服到底就跟不上了。

mbc.imuZeroHeading()
basic.showIcon(IconNames.Yes)

basic.forever(function () {
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // 往右轉為正
    let angle = Math.constrain(90 - h, 0, 180)            // 底座右轉 → 伺服左轉
    pins.servoWritePin(AnalogPin.P1, angle)
    basic.pause(20)
})

input.onButtonPressed(Button.A, function () {
    mbc.imuZeroHeading()                                  // 現在的方向當成新的 0°
    basic.showIcon(IconNames.Yes, 0)
})
