// Distance sensor 8-ch × micro:bit example 2: left / centre / right alarm
// What it does: something inside 20 cm: on the left → left arrow + low note, ahead → up arrow + high note, on the right → right arrow + middle note; nothing → happy face. B toggles the sound.
// Wiring: sensor SDA → P20, SCL → P19, VCC → external 5 V, GND → GND (common ground). On a V1 connect a buzzer or headphones to P0 for sound (V2 has a speaker).
// Setup: extension and paste steps as in example 1.
// You see: move your hand around the front of the sensor and the arrow and pitch change side; hand far away → happy face, silence.

const ALARM_CM = 20
let sound = true

input.onButtonPressed(Button.B, function () {
    sound = !sound
    basic.showString(sound ? "S" : "M")                   // S = sound on, M = muted
})

basic.forever(function () {
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)
    let dir = mbc.tof8NearestDirection()
    if (d < ALARM_CM && Math.abs(dir) <= 7) {
        if (dir < -2) {                                   // the three left sensors
            basic.showArrow(ArrowNames.West, 0)
            if (sound) {
                music.playTone(Note.C, 200)
            }
        } else if (dir > 2) {                             // the three right sensors
            basic.showArrow(ArrowNames.East, 0)
            if (sound) {
                music.playTone(Note.G, 200)
            }
        } else {                                          // straight ahead (sensors 4 and 5)
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
