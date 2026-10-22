// 循行者 16 路 × micro:bit 範例 2：丟線警報＋一鍵校準
// 做什麼：看到線 → 笑臉；丟線 → 哭臉並嗶一聲。按 A 做 5 秒自動校準（LED 倒數），按 B 切換黑線／白線。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 3V（或外接 5 V）、GND → GND。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：一張白紙＋黑色電工膠帶貼一條線。擴充與貼程式的方法同範例 1。
// 看到什麼：感應器在線上 → 笑臉；拿開 → 哭臉＋嗶；按 A → 5 4 3 2 1 倒數、顯示校準次數、打勾。

let white = false                                         // 目前是不是白線模式

mbc.lineSetColor(MbcLineColor.Black)                      // 開機先設成追黑線（預設）
basic.showIcon(IconNames.Happy)

mbc.onLineEvent(MbcLineEvent.Lost, function () {          // 線段數從 >0 變 0 時觸發一次
    basic.showIcon(IconNames.Sad, 0)
    music.playTone(Note.C, music.beat(BeatFraction.Half)) // 低音 Do 響半拍
})
mbc.onLineEvent(MbcLineEvent.Found, function () {         // 重新看到線
    basic.showIcon(IconNames.Happy, 0)
})

input.onButtonPressed(Button.A, function () {
    mbc.lineCalibrate(MbcLineCalib.Auto)                  // 5 秒內把感應器在黑線和白底上來回掃
    for (let i = 5; i > 0; i--) {
        basic.showNumber(i)                               // 倒數
        basic.pause(1000)
    }
    while (mbc.lineCalibrating()) {                       // 等感應器的「校準中」旗標歸零
        basic.pause(100)
    }
    basic.showNumber(mbc.lineCalibrationCount())          // 開機後完成校準的次數
    basic.showIcon(IconNames.Yes)
})

input.onButtonPressed(Button.B, function () {
    white = !white
    mbc.lineSetColor(white ? MbcLineColor.White : MbcLineColor.Black)   // 白底黑線 ↔ 黑底白線
    basic.showString(white ? "W" : "B")
})
