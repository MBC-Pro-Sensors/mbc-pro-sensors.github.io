// 循行者 8 路 × micro:bit 範例 1：線在哪一路（LED 光柱跟著線跑）
// 做什麼：把感應器在黑線上左右移動，LED 上的一根光柱會跟著線跑；按 A 顯示線位置的數字、按 B 顯示線寬。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。
// 準備：MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機先打勾（找到感應器）；線在左 → 光柱在左邊；線在正中間 → 光柱在中間；沒線 → 畫一個 X。

const CH = 8                                              // 路數：8 路版 8、16 路版 16
let busy = false                                          // 按鈕顯示數字時，暫停畫光柱

if (mbc.waitReady(mbc.ADDR_LINE, 3000)) {                 // 最多等 3 秒，看感應器有沒有接好
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和電源
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (mbc.lineSeen()) {
        let pos = mbc.linePosition()                      // −CH（最左）~ +CH（最右），0 = 正中間
        let x = Math.round(Math.map(pos, -CH, CH, 0, 4))  // 換算成 LED 的第 0~4 行
        basic.clearScreen()
        for (let y = 0; y < 5; y++) {
            led.plot(x, y)                                // 畫一整根光柱
        }
    } else {
        basic.showIcon(IconNames.No, 0)                   // 沒看到線
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.linePosition())                  // 線位置：負 = 線在左（站在車後往前看）
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.lineWidth())                     // 線寬：壓到線的路數，0 = 沒線
    busy = false
})
