// Line follower 16-ch × micro:bit example 5: smooth line following + stop at the 3rd crossing (all together)
// What it does: proportional control on the fine position, so the car steers smoothly; counts crossings and stops at the 3rd; when the line is lost it spins back using the last exit direction. A = start, B = stop.
// Wiring: as example 4 (sensor P19 / P20, left wheel P1, right wheel P2, servos on their own battery, common ground).
// Setup: a black-line track with 3 crossings.
// You see: after A the car follows smoothly, the LEDs show 1, 2, 3 at each crossing and the car stops with a tick at 3; lift the track away and the car turns toward where the line went, then carries on when it finds it.

const SPEED = 50
const KP = 0.5                                            // fine position ±100 → correction ±50
let running = false
let crossings = 0

// left servo P1, right servo P2 (continuous rotation); left / right = −100..100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full reverse
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right servo is mounted mirrored, so subtract
}

mbc.onLineEvent(MbcLineEvent.Fork, function () {          // segment count ≥ 2: crossing or fork
    if (!running) {
        return
    }
    crossings += 1
    basic.showNumber(crossings)
    if (crossings >= 3) {
        running = false
        drive(0, 0)
        basic.showIcon(IconNames.Yes)
    }
})
mbc.onLineEvent(MbcLineEvent.Lost, function () {
    if (!running) {
        return
    }
    if (mbc.lineLastDirection() < 0) {                    // the line left on the left → spin left to find it
        drive(-30, 30)
    } else {
        drive(30, -30)
    }
    basic.showIcon(IconNames.Confused, 0)
})

input.onButtonPressed(Button.A, function () {
    crossings = 0
    running = true
    basic.showArrow(ArrowNames.North, 0)
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    if (running && mbc.lineSeen()) {
        let err = mbc.linePositionFine()                  // −100..+100, negative = line on the left
        let turn = err * KP                               // line on the left → negative turn → left wheel slows = turn left
        drive(SPEED + turn, SPEED - turn)
    }
    basic.pause(20)
})
