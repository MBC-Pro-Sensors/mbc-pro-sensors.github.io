// 神攝手 ESP32CAM（WRO 版 HUE12）× micro:bit 範例 6：12 個色點一眼看完
// 做什麼：LED 前 3 列 = 色點 S1~S12（每列 4 個）：彩色 → 亮、白 → 微亮、黑或未啟用 → 暗。第 5 列的亮點 = 目前選的色點。按 A 選下一個色點、按 B 顯示它的顏色碼和鮮豔度。
// 接線：相機 SDA（IO3）→ P20、SCL（IO1）→ P19、VCC → 外接 5 V、GND → GND 共地。
// 準備：WRO 版相機先在機身上設定好 12 個色點的位置（看產品頁）。
// 看到什麼：場地上有彩色積木的色點亮、白底的微亮、黑的暗；按 B 看顏色碼：0~239 彩色（紅約 0、黃約 40、綠約 80、藍約 160）、240~245 白、246~250 黑、255 = 色點沒開。

let sel = 1                                               // 目前選的色點 1~12
let busy = false

if (mbc.waitReady(mbc.ADDR_CAM, 5000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    basic.clearScreen()
    for (let n = 1; n <= 12; n++) {
        let x = (n - 1) % 4                               // 每列 4 個
        let y = Math.idiv(n - 1, 4)                       // 第 0~2 列
        if (mbc.camSpotIs(n, MbcCamSpotKind.Colour)) {    // 顏色碼 0~239
            led.plotBrightness(x, y, 255)
        } else if (mbc.camSpotIs(n, MbcCamSpotKind.White)) {   // 240~245
            led.plotBrightness(x, y, 40)
        }
    }
    led.plot((sel - 1) % 4, 4)                            // 第 5 列：目前選的色點在哪一行
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    sel = sel % 12 + 1                                    // 1 → 2 → … → 12 → 1
    busy = true
    basic.showString("S" + sel)
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.camSpotColor(sel))               // 顏色碼 0~255
    basic.showString("V")
    basic.showNumber(mbc.camSpotVivid(sel))               // 鮮豔度 0~255
    busy = false
})
