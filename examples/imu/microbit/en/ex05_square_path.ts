// IMU × micro:bit example 5: drive a square (all together)
// What it does: press A: drive straight for 1.5 s (gyro keeps it straight) → spin right exactly 90° → repeat 4 times back to the start; the LEDs show which side it is on. Press B to abort.
// Wiring: as example 4 (IMU P19 / P20; left wheel P1, right wheel P2, servos on their own battery, common ground).
// Setup: the two-wheel car and about 1 m square of clear floor.
// You see: after A the car drives a square showing 1 2 3 4, then stops with a tick. Under- or overshooting corners → lower TURN_SPEED.

const SIDE_MS = 1500                                      // how long each side takes
const TURN_SPEED = 35
let running = false

// left servo P1, right servo P2 (continuous rotation); left / right = −100..100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full reverse
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right servo is mounted mirrored, so subtract
}

// drive straight for ms milliseconds, holding the heading target with the gyro
function straight(target: number, ms: number): void {
    let t0 = control.millis()
    while (running && control.millis() - t0 < ms) {
        let err = mbc.imuHeading(MbcImuHeading.Fused) - target
        let turn = Math.constrain(err * 2, -40, 40)
        drive(50 - turn, 50 + turn)
        basic.pause(20)
    }
}

// spin on the spot until the heading is within 3° of target
function turnTo(target: number): void {
    while (running) {
        let err = target - mbc.imuHeading(MbcImuHeading.Fused)   // positive = still need to turn right
        if (Math.abs(err) < 3) {
            break
        }
        let s = err > 0 ? TURN_SPEED : -TURN_SPEED
        drive(s, -s)                                      // left forward, right back = turn right
        basic.pause(20)
    }
    drive(0, 0)
    basic.pause(300)                                      // settle
}

input.onButtonPressed(Button.A, function () {
    if (running) {
        return
    }
    running = true
    mbc.imuZeroHeading()
    let target = 0
    for (let side = 1; side <= 4; side++) {
        basic.showNumber(side)
        straight(target, SIDE_MS)
        target += 90                                      // heading of the next side
        turnTo(target)
    }
    drive(0, 0)
    if (running) {
        basic.showIcon(IconNames.Yes)
    }
    running = false
})
input.onButtonPressed(Button.B, function () {
    running = false
    drive(0, 0)
    basic.showIcon(IconNames.No)
})
