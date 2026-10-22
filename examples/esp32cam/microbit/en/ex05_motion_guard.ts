// Vision sensor ESP32CAM (school edition) × micro:bit example 5: motion alarm + tracking turret (all together)
// What it does: movement in the picture (motion > 30) → alarm and the count goes up (at most once per 2 s); when a blob is found the servo turns toward it (as in example 3); blob found / lost shows a tick / X. A resets, B toggles the alarm.
// Wiring: camera SDA → P20, SCL → P19, VCC → external 5 V, GND → GND. Servo signal → P1, servo on its own 4.8–6 V supply, common ground. On a V1 put a buzzer on P0 for sound.
// Setup: camera with 'Setting 2: TRACK' done; camera fixed to the servo arm.
// You see: someone walks past → beep, number +1; bring a target-colour brick in → tick and the servo follows it; press B → M (muted).

const MOTION_LEVEL = 30                                   // motion 0..100, above this counts as movement
let armed = true
let alarms = 0
let lastAlarm = -5000
let angle = 90

mbc.onCamBlob(MbcCamEvent.Found, function () {            // blob appears
    basic.showIcon(IconNames.Yes, 0)
})
mbc.onCamBlob(MbcCamEvent.Lost, function () {             // blob disappears
    basic.showIcon(IconNames.No, 0)
})

input.onButtonPressed(Button.A, function () {
    alarms = 0
    basic.showNumber(0)
})
input.onButtonPressed(Button.B, function () {
    armed = !armed
    basic.showString(armed ? "S" : "M")                   // S = alarm on, M = muted
})

basic.forever(function () {
    let m = mbc.camMotion()                               // whole-picture motion 0..100
    if (armed && m > MOTION_LEVEL && control.millis() - lastAlarm > 2000) {
        alarms += 1
        lastAlarm = control.millis()
        music.playTone(Note.A, 300)                       // the servo twitches during the beep; normal (shared PWM period)
        basic.showNumber(alarms)
    }
    if (mbc.camFound()) {                                 // blob present: the turret follows it
        let err = mbc.camBlob(MbcCamBlob.X) - 50
        if (Math.abs(err) > 8) {
            angle = Math.constrain(angle - err / 10, 0, 180)
        }
        pins.servoWritePin(AnalogPin.P1, Math.round(angle))
    }
    basic.pause(50)
})
