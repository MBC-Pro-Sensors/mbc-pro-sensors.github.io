// Distance sensor 8-ch × micro:bit example 4: wall following (left wall)
// What it does: the car follows the wall on its left, using the leftmost sensor (S1) to stay 15 cm from it; a wall within 15 cm ahead → spin right. A = start, B = stop.
// Wiring: sensor SDA → P20, SCL → P19, VCC → external 5 V, GND → GND. Left continuous-rotation servo signal → P1, right → P2; servos on a 4.8–6 V battery pack whose − is joined to micro:bit GND.
// Setup: a two-wheel car with the sensor at the front (S1 facing left, S4 / S5 facing forward); a wall or a row of boxes.
// You see: after A the car runs along the left wall, drifting back when too far and away when too close; at a corner ahead it turns right and carries on.

const WALL_CM = 15
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
    basic.showArrow(ArrowNames.North, 0)
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
    let side = mbc.tof8Sensor(1, MbcDistUnit.Cm)          // sensor 1 = far left
    let front = mbc.tof8Front(MbcDistUnit.Cm)             // straight ahead (average of sensors 4 and 5)
    if (front < WALL_CM) {
        drive(40, -40)                                    // wall ahead: spin right
        basic.showArrow(ArrowNames.East, 0)
    } else {
        let err = side - WALL_CM                          // positive = too far from the wall
        let turn = Math.constrain(err * 3, -30, 30)
        drive(50 - turn, 50 + turn)                       // too far → left wheel slower → drift left
        if (turn > 5) {
            basic.showArrow(ArrowNames.NorthWest, 0)
        } else if (turn < -5) {
            basic.showArrow(ArrowNames.NorthEast, 0)
        } else {
            basic.showArrow(ArrowNames.North, 0)
        }
    }
    basic.pause(30)
})
