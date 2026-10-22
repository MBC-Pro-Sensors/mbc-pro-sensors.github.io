// 測距者 2 路 × micro:bit 範例 2：倒車雷達（越近嗶越急）
// 做什麼：40 cm 內開始嗶，越近嗶得越急；進到 10 cm 內畫一個 X。按 A 看東西偏左還是偏右。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V、GND → GND。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：擴充與貼程式的方法同範例 1。
// 看到什麼：手慢慢靠近 → 嗶…嗶…嗶嗶嗶；10 cm 內 → X；按 A → 箭頭指向比較近的那一邊。

mbc.onTof2Near(10, function () {                          // 較近距離第一次小於 10 cm 時觸發（每次靠近只觸發一次）
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    let d = mbc.tof2NearestCm()                           // 0~120 cm
    if (d < 40) {
        led.plotBarGraph(40 - d, 40)
        music.playTone(Note.A, 100)                       // 嗶 0.1 秒
        basic.pause(d * 10)                               // 30 cm 等 0.3 秒、10 cm 等 0.1 秒：越近越急
    } else {
        basic.showIcon(IconNames.Happy, 0)                // 前面很空
        basic.pause(100)
    }
})

input.onButtonPressed(Button.A, function () {
    let l = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)
    let r = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)
    if (l < r) {
        basic.showArrow(ArrowNames.West)                  // 左邊那顆比較近
    } else if (r < l) {
        basic.showArrow(ArrowNames.East)
    } else {
        basic.showArrow(ArrowNames.North)                 // 一樣近：正前方
    }
})
