// 掌控者 PS4／PS5 × micro:bit 範例 1：搖桿點點（LED 上的點跟著左搖桿動）
// 做什麼：推左搖桿，LED 上的亮點跟著跑；手把沒連線畫 X。按 A 顯示左搖桿 X 的數字、按 B 顯示 Y。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 外接 5 V（ESP32 接收器耗電大，不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：手把先跟接收器配對好（看產品頁）。MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機打勾（找到接收器）；手把沒開 → X；放開搖桿 → 點在正中間；推到右上 → 點跑到右上角。

let busy = false

if (mbc.waitReady(mbc.ADDR_PAD, 3000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20 和外接電源
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (!mbc.padConnected()) {                            // 手把沒連上
        basic.showIcon(IconNames.No, 0)
        basic.pause(200)
        return
    }
    let sx = mbc.padStick(MbcPadAxis.LeftX)               // −100（左）~ +100（右），0 = 中間
    let sy = mbc.padStick(MbcPadAxis.LeftY)               // −100（下）~ +100（上）
    let x = Math.round(Math.map(sx, -100, 100, 0, 4))
    let y = Math.round(Math.map(sy, -100, 100, 4, 0))     // 上 = 正 → 第 0 列
    basic.clearScreen()
    led.plot(x, y)
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.padStick(MbcPadAxis.LeftX))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.padStick(MbcPadAxis.LeftY))
    busy = false
})
