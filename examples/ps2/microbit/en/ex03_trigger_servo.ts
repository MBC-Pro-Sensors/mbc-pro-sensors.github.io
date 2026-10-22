// Gamepad receiver PS2 × micro:bit example 3: triggers control a servo
// What it does: the harder you squeeze R2, the further the servo turns right; L2 turns it left; release both and it centres. The LED bar shows the R2 pressure. Press A to show the angle.
// Wiring: receiver SDA → P20, SCL → P19, VCC → 3V (or external 5 V), GND → GND. Servo orange (signal) → P1, red → external 4.8–6 V battery +, brown → battery − joined to micro:bit GND.
// Setup: a 9 g servo (SG90).
// You see: squeeze R2 slowly → the bar grows and the servo turns toward 180°; squeeze L2 → toward 0°; release → 90°.

let angle = 90

basic.forever(function () {
    let r2 = mbc.padTrigger(MbcPadTrigger.R2)             // 0 (released) .. 255 (fully squeezed)
    let l2 = mbc.padTrigger(MbcPadTrigger.L2)
    angle = Math.round(90 + (r2 - l2) * 90 / 255)         // R2 → 90..180, L2 → 90..0
    pins.servoWritePin(AnalogPin.P1, angle)
    led.plotBarGraph(r2, 255)
    basic.pause(30)
})

input.onButtonPressed(Button.A, function () {
    basic.showNumber(angle)
})
