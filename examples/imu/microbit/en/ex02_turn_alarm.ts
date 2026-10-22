// IMU × micro:bit example 2: turn counter + tilt alarm
// What it does: every 90° turned → a beep and the count goes up by 1; tilted more than 20° nose up / down → sad face + low alarm; still → asleep face, moving → happy. Press B to reset.
// Wiring: IMU SDA → P20, SCL → P19, VCC → 3V, GND → GND. On a V1 connect a buzzer or headphones to P0 for sound (V2 has a speaker).
// Setup: extension and paste steps as in example 1.
// You see: turn it slowly through a full circle on the table → 4 beeps and 1 2 3 4; lift the front edge → sad face + alarm; leave it still → asleep face.

let turns = 0

mbc.imuZeroHeading()

mbc.onImuTurned(90, function () {                         // fires each time the fused heading has changed by another 90°
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
    let p = mbc.imuPitch()                                // pitch in degrees: nose up or down
    if (Math.abs(p) > 20) {
        basic.showIcon(IconNames.Sad, 0)
        music.playTone(Note.C, 200)                       // low alarm
    } else if (mbc.imuFlag(MbcImuFlag.Still)) {           // status bit 1: still
        basic.showIcon(IconNames.Asleep, 0)
    } else {
        basic.showIcon(IconNames.Happy, 0)
    }
    basic.pause(100)
})
