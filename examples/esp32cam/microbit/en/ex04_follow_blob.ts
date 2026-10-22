// Vision sensor ESP32CAM (school edition) × micro:bit example 4: car chases the blob
// What it does: blob on the right of the picture → turn right; blob small (far) → forward; big (near) → back; not seen → stop. A = start, B = stop.
// Wiring: camera SDA → P20, SCL → P19, VCC → external 5 V, GND → GND. Left continuous-rotation servo signal → P1, right → P2; servos on a 4.8–6 V battery pack whose − is joined to micro:bit GND.
// Setup: a two-wheel car with the camera facing forward; camera with 'Setting 2: TRACK' done; a ball or brick of the target colour.
// You see: after A put the ball in front: move it back and the car chases, bring it close and the car reverses, move it sideways and the car turns; take it away → stop, puzzled face.

const FAR_AREA = 800                                      // area below this = too far → forward (tune for your ball)
const NEAR_AREA = 2500                                    // area above this = too close → back
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
    if (!mbc.camFound()) {
        drive(0, 0)
        basic.showIcon(IconNames.Confused, 0)
    } else {
        let x = mbc.camBlob(MbcCamBlob.X)                 // 0 (left) .. 100 (right)
        let area = mbc.camBlob(MbcCamBlob.Area)           // width × height, 0..7500: bigger = nearer
        let turn = Math.constrain((x - 50), -40, 40)      // blob on the right → positive → left faster = turn right
        let speed = 0
        if (area < FAR_AREA) {
            speed = 50                                    // too far: forward
        } else if (area > NEAR_AREA) {
            speed = -40                                   // too close: back
        }
        drive(speed + turn, speed - turn)
        if (turn > 10) {
            basic.showArrow(ArrowNames.East, 0)
        } else if (turn < -10) {
            basic.showArrow(ArrowNames.West, 0)
        } else if (speed > 0) {
            basic.showArrow(ArrowNames.North, 0)
        } else if (speed < 0) {
            basic.showArrow(ArrowNames.South, 0)
        } else {
            basic.showIcon(IconNames.Target, 0)           // distance just right
        }
    }
    basic.pause(40)
})
