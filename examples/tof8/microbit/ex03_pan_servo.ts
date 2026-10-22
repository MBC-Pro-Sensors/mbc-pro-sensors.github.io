// 測距者 8 路 × micro:bit 範例 3：雷達轉頭（伺服對準最近的東西）
// 做什麼：伺服上貼一個箭頭，箭頭一直指著最近的東西；四周沒東西就回正中。按 A 輪流顯示 8 顆的公分數（S1 最左 → S8 最右）。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：9g 伺服（SG90）＋紙箭頭。
// 看到什麼：手從左邊繞到右邊，箭頭跟著轉；手拿開 → 回到 90°。

let busy = false

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let dir = mbc.tof8NearestDirection()                  // −7~+7，負 = 左；±8 = 沒東西
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)
    let angle = 90
    if (Math.abs(dir) <= 7 && d < 120) {
        angle = 90 + dir * 12                             // 每一格 12°：±7 → 6°~174°
    }
    pins.servoWritePin(AnalogPin.P1, angle)
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    for (let n = 1; n <= 8; n++) {
        basic.showString("S" + n)                         // 第 n 顆
        basic.showNumber(mbc.tof8Sensor(n, MbcDistUnit.Cm))
    }
    busy = false
})
