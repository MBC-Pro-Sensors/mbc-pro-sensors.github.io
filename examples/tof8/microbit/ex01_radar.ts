// 測距者 8 路 × micro:bit 範例 1：雷達畫面（最近的東西在哪個方向、多近）
// 做什麼：LED 上一根光柱：左右位置 = 最近的東西在哪個方向，高度 = 多近（越近越高）。按 A 顯示最近距離（cm）、按 B 顯示方向（−7~+7）。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 外接 5 V（8 顆雷射耗電大，不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機等 2~4 秒打勾（8 顆雷射開好）；手放在左前方 → 光柱在左；正前方 → 中間；越靠近光柱越高；四周沒東西 → 全暗。

let busy = false

if (mbc.waitReady(mbc.ADDR_TOF, 5000)) {                  // 8 顆雷射開機約 2~4 秒
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和外接電源
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let dir = mbc.tof8NearestDirection()                  // −7（正左）~ +7（正右），0 = 正前方；±8 = 四周沒東西
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)               // 8 顆裡最近的，0~120 cm
    basic.clearScreen()
    if (dir >= -7 && dir <= 7 && d < 120) {
        let x = Math.round(Math.map(dir, -7, 7, 0, 4))    // 方向 → 第 0~4 行
        let h = Math.round(Math.map(d, 0, 120, 5, 1))     // 距離 → 高度 1~5 格
        for (let y = 5 - h; y < 5; y++) {
            led.plot(x, y)
        }
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.tof8Nearest(MbcDistUnit.Cm))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.tof8NearestDirection())
    busy = false
})
