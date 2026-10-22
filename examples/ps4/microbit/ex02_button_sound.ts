// 掌控者 PS4／PS5 × micro:bit 範例 2：按鍵變圖案和聲音（燈條、震動）
// 做什麼：△ ○ ✕ □ 各畫一個圖案＋一個音；方向鍵畫箭頭；按 ✕ 手把震動、燈條跟著按鍵換顏色。
// 接線：接收器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND 共地。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：手把配對好；擴充與貼程式的方法同範例 1。
// 看到什麼：按 △ → 三角形＋Do；○ → 靶心＋Mi；✕ → X＋Sol、手把震一下；□ → 方形＋高音 Do；方向鍵 → 四個方向的箭頭。

mbc.onPadButton(MbcPadButton.Triangle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Triangle, 0)
    mbc.padLight(0x00FF00)                                // 燈條綠色（跟手把上的 △ 一樣）
    music.playTone(Note.C, 200)
})
mbc.onPadButton(MbcPadButton.Circle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Target, 0)
    mbc.padLight(0xFF0000)                                // 紅色
    music.playTone(Note.E, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.No, 0)
    mbc.padLight(0x0000FF)                                // 藍色
    mbc.padRumble(200)                                    // 震動（0~255）
    music.playTone(Note.G, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Released, function () {
    mbc.padRumble(0)                                      // 放開就停止震動
    basic.clearScreen()
})
mbc.onPadButton(MbcPadButton.Square, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Square, 0)
    mbc.padLight(0xFF00FF)                                // 粉紅
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
