// Line follower 16-ch × micro:bit example 4: line-following car (three-way)
// What it does: press A to drive: line on the left → turn left, on the right → turn right, centred → straight; line lost → stop. Press B to stop.
// Wiring: sensor SDA → P20, SCL → P19, VCC → 3V, GND → GND. Left continuous-rotation servo signal → P1, right → P2; both servos powered from a 4.8–6 V battery pack whose − is joined to micro:bit GND.
// Setup: a two-wheel car (two continuous-rotation servos as wheels), the sensor at the front 0.5–1 cm above the floor, a black-line track.
// You see: after A the arrow points up and the car follows the line; the arrow points the way it turns; off the line → sad face, stop.

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
    if (!mbc.lineSeen()) {
        drive(0, 0)                                       // stop when the line is lost
        basic.showIcon(IconNames.Sad, 0)
    } else {
        let pos = mbc.linePosition()                      // negative = line on the left
        if (pos < 0) {
            drive(20, 60)                                 // left slow, right fast = turn left
            basic.showArrow(ArrowNames.West, 0)
        } else if (pos > 0) {
            drive(60, 20)                                 // turn right
            basic.showArrow(ArrowNames.East, 0)
        } else {
            drive(50, 50)                                 // straight
            basic.showArrow(ArrowNames.North, 0)
        }
    }
    basic.pause(20)
})
