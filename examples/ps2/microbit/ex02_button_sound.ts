// 掌控者 PS2 × micro:bit 範例 2：按鍵變圖案和聲音
// 做什麼：△ ○ ✕ □ 各畫一個圖案＋一個音；方向鍵畫箭頭。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND 共地。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：手把配對好；擴充與貼程式的方法同範例 1。
// 看到什麼：按 △ → 三角形＋Do；○ → 靶心＋Mi；✕ → X＋Sol；□ → 方形＋高音 Do；方向鍵 → 四個方向的箭頭。

mbc.onPadButton(MbcPadButton.Triangle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Triangle, 0)
    music.playTone(Note.C, 200)
})
mbc.onPadButton(MbcPadButton.Circle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Target, 0)
    music.playTone(Note.E, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.No, 0)
    music.playTone(Note.G, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Released, function () {
    basic.clearScreen()
})
mbc.onPadButton(MbcPadButton.Square, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Square, 0)
    music.playTone(Note.C5, 200)
})
mbc.onPadButton(MbcPadButton.Up, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.North, 0)
})
mbc.onPadButton(MbcPadButton.Down, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.South, 0)
})
mbc.onPadButton(MbcPadButton.Left, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.West, 0)
})
mbc.onPadButton(MbcPadButton.Right, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.East, 0)
})

basic.forever(function () {
    if (!mbc.padConnected()) {                            // 手把沒連線就一直顯示小愛心提醒
        basic.showIcon(IconNames.SmallHeart, 0)
    }
    basic.pause(500)
})
