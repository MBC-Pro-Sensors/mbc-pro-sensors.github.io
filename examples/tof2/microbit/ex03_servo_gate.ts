// 測距者 2 路 × micro:bit 範例 3：自動柵欄（伺服）
// 做什麼：有東西進到 15 cm 內 → 柵欄打開（伺服轉到 90°）；離開 2 秒後關上（0°）。按 A／B 把距離門檻 +5／−5。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：9g 伺服（SG90）＋冰棒棍當柵欄。
// 看到什麼：手伸到感應器前 → 柵欄抬起、打勾；手拿開 2 秒 → 柵欄放下、X。

let limit = 15                                            // 幾公分以內算「有東西」
let lastSeen = -10000                                     // 上次看到東西的時間（毫秒）
let open = false

pins.servoWritePin(AnalogPin.P1, 0)                       // 開機先關上
basic.showIcon(IconNames.No)

basic.forever(function () {
    let d = mbc.tof2NearestCm()
    if (d > 0 && d < limit) {
        lastSeen = control.millis()                       // 記下現在的時間
    }
    let wantOpen = control.millis() - lastSeen < 2000     // 2 秒內看過東西就要開著
    if (wantOpen != open) {                               // 狀態變了才動伺服
        open = wantOpen
        pins.servoWritePin(AnalogPin.P1, open ? 90 : 0)
        basic.showIcon(open ? IconNames.Yes : IconNames.No, 0)
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    limit = Math.constrain(limit + 5, 5, 100)
    basic.showNumber(limit)
})
input.onButtonPressed(Button.B, function () {
    limit = Math.constrain(limit - 5, 5, 100)
    basic.showNumber(limit)
})
