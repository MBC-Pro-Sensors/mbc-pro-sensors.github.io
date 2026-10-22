// Gamepad receiver PS4/PS5 × micro:bit example 4: drive with the d-pad
// What it does: hold a d-pad button and the car moves that way, release to stop: up = forward, down = back, left = spin left, right = spin right. The car stops by itself if the gamepad disconnects.
// Wiring: receiver SDA → P20, SCL → P19, VCC → external 5 V, GND → GND. Left continuous-rotation servo signal → P1, right → P2; servos on a 4.8–6 V battery pack whose − is joined to micro:bit GND.
// Setup: a two-wheel car (two continuous-rotation servos as wheels).
// You see: hold up → the car drives forward with an up arrow; release → stop; switch the gamepad off → X, car stops.

// left servo P1, right servo P2 (continuous rotation); left / right = −100..100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full reverse
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right servo is mounted mirrored, so subtract
}

basic.forever(function () {
    if (!mbc.padConnected()) {                            // disconnected: stop
        drive(0, 0)
        basic.showIcon(IconNames.No, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Up)) {
        drive(50, 50)                                     // forward
        basic.showArrow(ArrowNames.North, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Down)) {
        drive(-50, -50)                                   // back
        basic.showArrow(ArrowNames.South, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Left)) {
        drive(-30, 30)                                    // left back, right forward = spin left
        basic.showArrow(ArrowNames.West, 0)
    } else if (mbc.padButtonPressed(MbcPadButton.Right)) {
        drive(30, -30)                                    // spin right
        basic.showArrow(ArrowNames.East, 0)
    } else {
        drive(0, 0)                                       // nothing pressed: stop
        basic.clearScreen()
    }
    basic.pause(50)
})
