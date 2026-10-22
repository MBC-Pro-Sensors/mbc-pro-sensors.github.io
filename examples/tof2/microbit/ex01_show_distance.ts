// 測距者 2 路 × micro:bit 範例 1：前面多遠（LED 長條＋數字）
// 做什麼：手越靠近感應器，LED 亮的格子越多；按 A 顯示左邊那顆的公分數、按 B 顯示右邊那顆。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。
// 準備：MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機先打勾；前面沒東西 → 全暗；手放 60 cm → 亮一半；貼到 10 cm 內 → 幾乎全亮。

let busy = false

if (mbc.waitReady(mbc.ADDR_TOF, 3000)) {                  // 兩顆雷射都開好了才算就緒
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和電源
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let d = mbc.tof2NearestCm()                           // 左右兩顆取較近的，0~120 cm，120 = 前面沒東西
    led.plotBarGraph(120 - d, 120)                        // 越近長條越長
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm))    // 左邊那顆（站在車後往前看）
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm))   // 右邊那顆
    busy = false
})
