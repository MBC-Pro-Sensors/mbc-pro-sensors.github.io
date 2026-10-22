// Distance sensor 8-ch × micro:bit example 3: radar head (a servo points at the nearest object)
// What it does: a servo with an arrow keeps pointing at the nearest object; nothing around → back to centre. Press A to show the 8 sensors in cm one by one (S1 = far left .. S8 = far right).
// Wiring: sensor SDA → P20, SCL → P19, VCC → external 5 V, GND → GND. Servo orange (signal) → P1, red → external 4.8–6 V battery +, brown → battery − joined to micro:bit GND.
// Setup: a 9 g servo (SG90) with a paper arrow.
// You see: move your hand from left to right and the arrow follows; take it away → back to 90°.

let busy = false

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let dir = mbc.tof8NearestDirection()                  // −7..+7, negative = left; ±8 = nothing
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)
    let angle = 90
    if (Math.abs(dir) <= 7 && d < 120) {
        angle = 90 + dir * 12                             // 12° per step: ±7 → 6°..174°
    }
    pins.servoWritePin(AnalogPin.P1, angle)
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    for (let n = 1; n <= 8; n++) {
        basic.showString("S" + n)                         // sensor n
        basic.showNumber(mbc.tof8Sensor(n, MbcDistUnit.Cm))
    }
    busy = false
})
