// 陀螺儀 × micro:bit 範例 1：電子羅盤（箭頭一直指著開機時的正前方）
// 做什麼：把 micro:bit 和陀螺儀一起轉，LED 箭頭會反方向轉，一直指著原本的正前方。按 A 顯示航向角度、按 B 把現在的方向設成 0。
// 接線：陀螺儀 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。
// 準備：MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。陀螺儀平放、開機後 2 秒不要動。
// 看到什麼：開機打勾、箭頭朝上；往右轉 90° → 箭頭指左；轉 180° → 箭頭朝下；轉回來 → 箭頭朝上。

const ARROWS = [ArrowNames.North, ArrowNames.NorthEast, ArrowNames.East, ArrowNames.SouthEast,
    ArrowNames.South, ArrowNames.SouthWest, ArrowNames.West, ArrowNames.NorthWest]   // 順時針 8 個方向
let busy = false

if (mbc.waitReady(mbc.ADDR_IMU, 3000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和電源
}
mbc.imuZeroHeading()                                      // 現在的方向 = 0°

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // 度，往右轉變大、往左轉變小，可超過 360
    let a = ((h % 360) + 360) % 360                       // 整理成 0~359
    let idx = Math.round(a / 45) % 8                      // 轉了幾個 45°
    basic.showArrow(ARROWS[(8 - idx) % 8], 0)             // 箭頭要往反方向指回原本的正前方
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.imuHeading(MbcImuHeading.Fused))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    mbc.imuZeroHeading()
})
