// 神攝手 ESP32CAM（學校版）× micro:bit 範例 2：看到指定顏色就嗶
// 做什麼：LED 顯示要找的顏色（R 紅、Y 黃、G 綠、B 藍）；鏡頭中央看到那個顏色 → 愛心＋嗶。按 A／B 換下一個／上一個顏色。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND 共地。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：相機做過「設定 1：COLOR」；紅、黃、綠、藍四塊積木。
// 看到什麼：LED 顯示 R，紅積木放到鏡頭前 → 愛心＋嗶；綠積木 → 沒反應；按 A 變成 Y，換黃積木才會響。

const NAMES = ["R", "Y", "G", "B"]
const HUES = [0, 17, 33, 67]                              // 紅、黃、綠、藍的色相（0~100）
const TOL = 8                                             // 差 8 以內算同一色
let sel = 0

// 色相是一個圓：0 和 100 都是紅，所以差距超過 50 要繞另一邊算
function sameHue(h: number, target: number): boolean {
    let d = Math.abs(h - target)
    if (d > 50) {
        d = 100 - d
    }
    return d <= TOL
}

input.onButtonPressed(Button.A, function () {
    sel = (sel + 1) % 4
})
input.onButtonPressed(Button.B, function () {
    sel = (sel + 3) % 4
})

basic.forever(function () {
    let h = mbc.camColor(MbcCamColor.Hue)                 // −1 = 黑白灰
    if (!mbc.camIsGrey() && sameHue(h, HUES[sel])) {
        basic.showIcon(IconNames.Heart, 0)
        music.playTone(Note.E, 150)
    } else {
        basic.showString(NAMES[sel])                      // 單一個字母直接顯示，不捲動
    }
    basic.pause(100)
})
