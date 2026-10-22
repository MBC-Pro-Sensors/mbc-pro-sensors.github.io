// Distance sensor 8-ch × micro:bit example 5: auto explore (head for the most open direction, all together)
// What it does: the car keeps steering toward the most open direction; something inside 12 cm → back up and spin toward the open side; a skull flashes and a counter goes up each time. A = start, B = stop.
// Wiring: as example 4 (sensor P19 / P20, external 5 V; left wheel P1, right wheel P2, servos on their own battery, common ground).
// Setup: the two-wheel car and a few boxes arranged as a maze.
// You see: after A the car wanders between the boxes without hitting them; press B to stop and see how many times it dodged.

const STOP_CM = 12
let running = false
let dodges = 0

// left servo P1, right servo P2 (continuous rotation); left / right = −100..100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full reverse
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right servo is mounted mirrored, so subtract
}

mbc.onTof8Near(STOP_CM, function () {                     // event: once each time something comes inside 12 cm
    dodges += 1
    basic.showIcon(IconNames.Skull, 0)
})

input.onButtonPressed(Button.A, function () {
    dodges = 0
    running = true
    basic.showArrow(ArrowNames.North, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showNumber(dodges)
})

basic.forever(function () {
    if (!running) {
        basic.pause(100)
        return
    }
    let near = mbc.tof8Nearest(MbcDistUnit.Cm)
    let open = mbc.tof8OpenDirection()                    // most open direction −7..+7, negative = left
    if (near < STOP_CM) {
        drive(-40, -40)                                   // back up for 0.4 s
        basic.pause(400)
        if (open < 0) {
            drive(-40, 40)                                // open side on the left → spin left
        } else {
            drive(40, -40)
        }
        basic.pause(500)
    } else {
        let turn = Math.constrain(open * 8, -40, 40)      // open on the left (negative) → left wheel slower → veer left
        drive(50 + turn, 50 - turn)
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(30)
})
