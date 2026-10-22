// Distance sensor 2-ch × micro:bit example 4: follow-me car
// What it does: the car keeps 20 cm from the object in front (your hand or a board): too far → forward, too close → back, object to the left → turn left. A = start, B = stop.
// Wiring: sensor SDA → P20, SCL → P19, VCC → 3V, GND → GND. Left continuous-rotation servo signal → P1, right → P2; servos on a 4.8–6 V battery pack whose − is joined to micro:bit GND.
// Setup: a two-wheel car with the sensor at the front facing forward.
// You see: after A hold your hand 20 cm in front: move it back and the car follows, move it closer and the car reverses; take it away (over 60 cm) and the car stops with a puzzled face.

const TARGET = 20                                         // distance to keep (cm)
let running = false

// left servo P1, right servo P2 (continuous rotation); left / right = −100..100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full reverse
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right servo is mounted mirrored, so subtract
}

input.onButtonPressed(Button.A, function () {
    running = true
    basic.showIcon(IconNames.Happy, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    if (!running) {
        basic.pause(100)
        return
    }
    let l = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)
    let r = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)
    let d = Math.min(l, r)                                // the nearer one
    if (d >= 60) {
        drive(0, 0)                                       // too far: object gone, stop
        basic.showIcon(IconNames.Confused, 0)
    } else {
        let speed = Math.constrain((d - TARGET) * 4, -60, 60)   // farther than 20 cm → positive = forward; nearer → negative = back
        let turn = Math.constrain((r - l) * 2, -30, 30)         // right sensor farther → object is to the left → turn left
        drive(speed - turn, speed + turn)
        if (turn < -5) {
            basic.showArrow(ArrowNames.East, 0)
        } else if (turn > 5) {
            basic.showArrow(ArrowNames.West, 0)
        } else {
            basic.showArrow(ArrowNames.North, 0)
        }
    }
    basic.pause(30)
})
