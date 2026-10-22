// Distance sensor 2-ch × micro:bit example 3: automatic barrier (servo)
// What it does: something inside 15 cm → the barrier opens (servo to 90°); 2 s after it leaves the barrier closes (0°). A / B raise / lower the threshold by 5.
// Wiring: sensor SDA → P20, SCL → P19, VCC → 3V, GND → GND. Servo orange (signal) → P1, red → external 4.8–6 V battery +, brown → battery − joined to micro:bit GND.
// Setup: a 9 g servo (SG90) with a lolly stick as the barrier.
// You see: hand in front → barrier lifts, tick; hand away for 2 s → barrier drops, X.

let limit = 15                                            // inside this many cm counts as 'something there'
let lastSeen = -10000                                     // time (ms) something was last seen
let open = false

pins.servoWritePin(AnalogPin.P1, 0)                       // start closed
basic.showIcon(IconNames.No)

basic.forever(function () {
    let d = mbc.tof2NearestCm()
    if (d > 0 && d < limit) {
        lastSeen = control.millis()                       // remember the time
    }
    let wantOpen = control.millis() - lastSeen < 2000     // keep open for 2 s after the last sighting
    if (wantOpen != open) {                               // move the servo only when the state changes
        open = wantOpen
        pins.servoWritePin(AnalogPin.P1, open ? 90 : 0)
        basic.showIcon(open ? IconNames.Yes : IconNames.No, 0)
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    limit = Math.constrain(limit + 5, 5, 100)
    basic.showNumber(limit)
})
input.onButtonPressed(Button.B, function () {
    limit = Math.constrain(limit - 5, 5, 100)
    basic.showNumber(limit)
})
