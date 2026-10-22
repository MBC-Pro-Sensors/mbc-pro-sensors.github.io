// 神攝手 ESP32CAM（學校版）× micro:bit 範例 5：動態偵測警報＋追蹤雲台（綜合）
// 做什麼：畫面有動靜（動態 > 30）→ 警報音、次數 +1（2 秒內不重複）；有找到色塊時伺服轉向色塊（同範例 3）；色塊出現／消失畫打勾／X。A 清零、B 開關警報。
// 接線：相機 SDA → P20、SCL → P19、VCC → 外接 5 V、GND → GND。伺服訊號 → P1、伺服電源外接 4.8~6 V、共地。V1 聲音要在 P0 接蜂鳴器。
// 準備：相機做過「設定 2：TRACK」；相機固定在伺服擺臂上。
// 看到什麼：有人從鏡頭前走過 → 嗶、數字 +1；拿目標色的積木進來 → 打勾、伺服轉過去跟著它；按 B → M（靜音）。

const MOTION_LEVEL = 30                                   // 動態 0~100，超過就算有人
let armed = true
let alarms = 0
let lastAlarm = -5000
let angle = 90

mbc.onCamBlob(MbcCamEvent.Found, function () {            // 色塊出現
    basic.showIcon(IconNames.Yes, 0)
})
mbc.onCamBlob(MbcCamEvent.Lost, function () {             // 色塊消失
    basic.showIcon(IconNames.No, 0)
})

input.onButtonPressed(Button.A, function () {
    alarms = 0
    basic.showNumber(0)
})
input.onButtonPressed(Button.B, function () {
    armed = !armed
    basic.showString(armed ? "S" : "M")                   // S = 警報開、M = 靜音
})

basic.forever(function () {
    let m = mbc.camMotion()                               // 全畫面動態量 0~100
    if (armed && m > MOTION_LEVEL && control.millis() - lastAlarm > 2000) {
        alarms += 1
        lastAlarm = control.millis()
        music.playTone(Note.A, 300)                       // 警報響時伺服會抖一下，正常（PWM 共用週期）
        basic.showNumber(alarms)
    }
    if (mbc.camFound()) {                                 // 有色塊：雲台跟著它
        let err = mbc.camBlob(MbcCamBlob.X) - 50
        if (Math.abs(err) > 8) {
            angle = Math.constrain(angle - err / 10, 0, 180)
        }
        pins.servoWritePin(AnalogPin.P1, Math.round(angle))
    }
    basic.pause(50)
})
