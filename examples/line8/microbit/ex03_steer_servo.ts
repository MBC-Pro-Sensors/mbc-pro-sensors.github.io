// 循行者 8 路 × micro:bit 範例 3：方向盤伺服跟著線轉
// 做什麼：一顆 9g 伺服（SG90）當方向盤：線在左 → 伺服往左轉，線越偏轉越多；丟線 → 回正中。按 A 顯示目前角度。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND（共地）。
// 準備：範例 2 的黑線紙＋先做過一次校準。
// 看到什麼：感應器往線的左邊移 → 伺服擺臂往一邊轉；移到右邊 → 往另一邊；拿開 → 回到 90°。

const CH = 8                                              // 路數：8 路版 8、16 路版 16
const GAIN = Math.idiv(80, CH)                            // 每偏一路轉幾度：最外側剛好 ±80°
let angle = 90

basic.forever(function () {
    if (mbc.lineSeen()) {
        angle = 90 + mbc.linePosition() * GAIN            // 線在左（負）→ 角度小於 90
    } else {
        angle = 90                                        // 沒線：回正
    }
    angle = Math.constrain(angle, 0, 180)
    pins.servoWritePin(AnalogPin.P1, angle)               // 0~180°
    basic.pause(30)
})

input.onButtonPressed(Button.A, function () {
    basic.showNumber(angle)
})
