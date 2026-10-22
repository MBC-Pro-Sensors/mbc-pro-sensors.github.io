// 神攝手 ESP32CAM（學校版）× micro:bit 範例 3：鏡頭雲台追色塊（伺服）
// 做什麼：相機裝在伺服上：色塊在畫面左邊 → 伺服往左轉，直到色塊回到畫面中間；沒找到色塊 → 問號臉、伺服不動。按 A 回正中。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。伺服 橘（訊號）→ P1、紅 → 外接 4.8~6 V 電池正極、棕 → 電池負極並接 micro:bit GND。
// 準備：相機做過「設定 2：TRACK」（追蹤框會框住色塊）；9g 伺服（SG90）＋把相機固定在伺服擺臂上。
// 看到什麼：把色塊慢慢往左移，伺服跟著轉過去讓色塊回到中間；拿走色塊 → 問號臉。

const CENTRE = 50                                         // 色塊 X 0~100，50 = 畫面中間
const DEAD = 8                                            // 離中間 8 以內就不動
let angle = 90

basic.forever(function () {
    if (mbc.camFound()) {                                 // 有找到目標色的色塊
        let err = mbc.camBlob(MbcCamBlob.X) - CENTRE      // 正 = 色塊在右邊
        if (Math.abs(err) > DEAD) {
            angle = Math.constrain(angle - err / 10, 0, 180)   // 色塊在右 → 角度變小 = 往右轉（裝反就把減改成加）
            basic.showArrow(err > 0 ? ArrowNames.East : ArrowNames.West, 0)
        } else {
            basic.showIcon(IconNames.Target, 0)           // 對準了
        }
        pins.servoWritePin(AnalogPin.P1, Math.round(angle))
    } else {
        basic.showIcon(IconNames.Confused, 0)
    }
    basic.pause(40)
})

input.onButtonPressed(Button.A, function () {
    angle = 90
    pins.servoWritePin(AnalogPin.P1, 90)
})
