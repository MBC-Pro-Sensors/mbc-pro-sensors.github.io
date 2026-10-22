// Distance sensor 2-ch × micro:bit example 5: obstacle-avoiding car (all together)
// What it does: the car drives forward; something inside 15 cm → back up, spin toward the more open side, carry on; the LEDs count the detours. A = start, B = stop.
// Wiring: as example 4 (sensor P19 / P20, left wheel P1, right wheel P2, servos on their own battery, common ground).
// Setup: the two-wheel car and a few boxes as obstacles.
// You see: after A the car drives, backs up before an obstacle, turns and continues; the number goes up by 1 each detour; a skull flashes the moment something comes inside 15 cm.

const STOP_CM = 15
let running = false
let detours = 0

// left servo P1, right servo P2 (continuous rotation); left / right = −100..100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full reverse
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right servo is mounted mirrored, so subtract
}

mbc.onTof2Near(STOP_CM, function () {                     // event: flash once each time something comes inside 15 cm
    basic.showIcon(IconNames.Skull, 0)
})

input.onButtonPressed(Button.A, function () {
    detours = 0
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
    let l = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)
    let r = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)
    if (Math.min(l, r) < STOP_CM) {
        detours += 1
        drive(-50, -50)                                   // back up for 0.4 s
        basic.pause(400)
        if (l > r) {
            drive(-40, 40)                                // left is more open → spin left
        } else {
            drive(40, -40)                                // spin right
        }
        basic.pause(500)
        basic.showNumber(detours)
    } else {
        drive(50, 50)                                     // clear ahead: straight
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(30)
})
