// 測距者 8 路 × micro:bit 範例 2：左／中／右警報
// 做什麼：20 cm 內有東西：在左邊 → 左箭頭＋低音、正前方 → 上箭頭＋高音、右邊 → 右箭頭＋中音；沒東西 → 笑臉。按 B 開關聲音。
// 接線：感應器 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND 共地。V1 要在 P0 接蜂鳴器或耳機才有聲音（V2 有內建喇叭）。
// 準備：擴充與貼程式的方法同範例 1。
// 看到什麼：手繞著感應器前方移動，箭頭和音高跟著換邊；手拿遠 → 笑臉、安靜。

const ALARM_CM = 20
let sound = true

input.onButtonPressed(Button.B, function () {
    sound = !sound
    basic.showString(sound ? "S" : "M")                   // S = 有聲音、M = 靜音
})

basic.forever(function () {
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)
    let dir = mbc.tof8NearestDirection()
    if (d < ALARM_CM && Math.abs(dir) <= 7) {
        if (dir < -2) {                                   // 左邊三顆
            basic.showArrow(ArrowNames.West, 0)
            if (sound) {
                music.playTone(Note.C, 200)
            }
        } else if (dir > 2) {                             // 右邊三顆
            basic.showArrow(ArrowNames.East, 0)
            if (sound) {
                music.playTone(Note.G, 200)
            }
        } else {                                          // 正前方（第 4、5 顆）
            basic.showArrow(ArrowNames.North, 0)
            if (sound) {
                music.playTone(Note.C5, 200)
            }
        }
    } else {
        basic.showIcon(IconNames.Happy, 0)
    }
    basic.pause(100)
})
