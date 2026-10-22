// 掌控者 PS2 × micro:bit 範例 3：扳機控制伺服
// 做什麼：R2 壓多深，伺服就往右轉多少；L2 往左；兩邊都放開回正中。LED 長條顯示 R2 的壓力。按 A 顯示目前角度。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：9g 伺服（SG90）。
// 看到什麼：慢慢壓 R2 → 長條變長、伺服慢慢轉到 180°；壓 L2 → 轉到 0°；放開 → 90°。

let angle = 90

basic.forever(function () {
    let r2 = mbc.padTrigger(MbcPadTrigger.R2)             // 0（放開）~ 255（壓到底）
    let l2 = mbc.padTrigger(MbcPadTrigger.L2)
    angle = Math.round(90 + (r2 - l2) * 90 / 255)         // R2 → 90~180、L2 → 90~0
    pins.servoWritePin(AnalogPin.P1, angle)
    led.plotBarGraph(r2, 255)
    basic.pause(30)
})

input.onButtonPressed(Button.A, function () {
    basic.showNumber(angle)
})
