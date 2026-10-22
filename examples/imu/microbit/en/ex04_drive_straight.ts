// IMU × micro:bit example 4: drive straight (gyro correction)
// What it does: press A and the car drives forward; whenever it drifts, the IMU heading steers it back, so it runs straight. Press B to stop. The LEDs show which way it is correcting.
// Wiring: IMU SDA → P20, SCL → P19, VCC → 3V, GND → GND. Left continuous-rotation servo signal → P1, right → P2; servos on a 4.8–6 V battery pack whose − is joined to micro:bit GND.
// Setup: a two-wheel car with the IMU flat on it (printed arrow facing forward).
// You see: after A the car drives straight; nudge the front sideways and it turns back to the original heading and carries on.

const KP = 2                                              // 1° off → 2% speed difference
let running = false

// left servo P1, right servo P2 (continuous rotation); left / right = −100..100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full reverse
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right servo is mounted mirrored, so subtract
}

input.onButtonPressed(Button.A, function () {
    mbc.imuZeroHeading()                                  // the starting direction = 0°
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
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // positive = the nose has drifted right
    let turn = Math.constrain(h * KP, -40, 40)
    drive(50 - turn, 50 + turn)                           // drifted right → left slower, right faster → steer back left
    if (turn > 5) {
        basic.showArrow(ArrowNames.NorthWest, 0)
    } else if (turn < -5) {
        basic.showArrow(ArrowNames.NorthEast, 0)
    } else {
        basic.showArrow(ArrowNames.North, 0)
    }
    basic.pause(20)
})
