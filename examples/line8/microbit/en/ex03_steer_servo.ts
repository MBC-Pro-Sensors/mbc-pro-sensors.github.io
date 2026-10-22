// Line follower 8-ch × micro:bit example 3: a steering servo follows the line
// What it does: a 9 g servo (SG90) acts as a steering wheel: line on the left → servo turns left, further off-centre → bigger turn; line lost → back to centre. Press A to show the angle.
// Wiring: sensor SDA → P20, SCL → P19, VCC → 3V, GND → GND. Servo orange (signal) → P1, red → external 4.8–6 V battery +, brown → battery − joined to micro:bit GND (common ground).
// Setup: the black-line sheet from example 2, calibrated once.
// You see: move the sensor so the line is on its left → the servo arm swings one way; line on the right → the other way; lift it away → back to 90°.

const CH = 8                                              // channels: 8 on the 8-ch, 16 on the 16-ch
const GAIN = Math.idiv(80, CH)                            // degrees per channel: the outermost channel gives ±80°
let angle = 90

basic.forever(function () {
    if (mbc.lineSeen()) {
        angle = 90 + mbc.linePosition() * GAIN            // line on the left (negative) → angle below 90
    } else {
        angle = 90                                        // no line: centre
    }
    angle = Math.constrain(angle, 0, 180)
    pins.servoWritePin(AnalogPin.P1, angle)               // 0..180°
    basic.pause(30)
})

input.onButtonPressed(Button.A, function () {
    basic.showNumber(angle)
})
