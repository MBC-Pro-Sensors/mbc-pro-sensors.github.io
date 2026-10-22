// 陀螺儀 × micro:bit 範例 2：轉彎計數＋傾斜警報
// 做什麼：每轉 90° 嗶一聲並把次數 +1；前後傾斜超過 20° → 哭臉＋低音警報；靜止不動 → 睡臉、有在動 → 笑臉。按 B 歸零。
// 接線：陀螺儀 SDA → P20、SCL → P19、VCC → 3V、GND → GND。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：擴充與貼程式的方法同範例 1。
// 看到什麼：桌上慢慢轉一圈 → 嗶 4 聲、顯示 1 2 3 4；把板子前端抬高 → 哭臉＋警報；放著不動 → 睡臉。

let turns = 0

mbc.imuZeroHeading()

mbc.onImuTurned(90, function () {                         // 融合航向每累積變化 90° 觸發一次
    turns += 1
    music.playTone(Note.C5, 150)
    basic.showNumber(turns)
})

input.onButtonPressed(Button.B, function () {
    turns = 0
    mbc.imuZeroHeading()
    basic.showNumber(0)
})

basic.forever(function () {
    let p = mbc.imuPitch()                                // 俯仰角，度：前端抬高或壓低
    if (Math.abs(p) > 20) {
        basic.showIcon(IconNames.Sad, 0)
        music.playTone(Note.C, 200)                       // 低音警報
    } else if (mbc.imuFlag(MbcImuFlag.Still)) {           // 狀態旗標 b1：靜止
        basic.showIcon(IconNames.Asleep, 0)
    } else {
        basic.showIcon(IconNames.Happy, 0)
    }
    basic.pause(100)
})
