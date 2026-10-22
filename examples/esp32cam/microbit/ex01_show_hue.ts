// 神攝手 ESP32CAM（學校版）× micro:bit 範例 1：看到什麼顏色（色相長條）
// 做什麼：LED 長條顯示鏡頭中央看到的色相（紅 0 → 黃 17 → 綠 33 → 藍 67 → 紅 100）；黑白灰畫一個小方塊。按 A 顯示色相數字、按 B 顯示飽和度。
// 接線：相機 SDA（IO3）→ P20、SCL（IO1）→ P19、VCC → 外接 5 V（ESP32 耗電大，不要從 micro:bit 3V 取電）、GND → GND 共地。
// 準備：先在相機機身做「設定 1：COLOR」（看產品頁）。MakeCode 新專案 → 擴充 → 貼 https://github.com/contactmbcrobot/pxt-mbc-sensors → 切到 JavaScript 把本檔全部貼上 → 下載到 micro:bit。
// 看到什麼：開機打勾（找到相機）；紅色積木 → 長條幾乎全暗（0）；綠色 → 亮 1/3；藍色 → 亮 2/3；白紙 → 小方塊。

let busy = false

if (mbc.waitReady(mbc.ADDR_CAM, 5000)) {                  // 相機開機約 3 秒
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // 沒回應：檢查 P19／P20、外接電源，相機要先接好再開 micro:bit
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (mbc.camIsGrey()) {                                // 色相 −1：黑、白、灰
        basic.showIcon(IconNames.SmallSquare, 0)
    } else {
        led.plotBarGraph(mbc.camColor(MbcCamColor.Hue), 100)   // 0~100
    }
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.camColor(MbcCamColor.Hue))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.camColor(MbcCamColor.Saturation))   // 0~100，越大顏色越鮮
    busy = false
})
