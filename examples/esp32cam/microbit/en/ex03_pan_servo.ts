// Vision sensor ESP32CAM (school edition) × micro:bit example 3: camera pan servo tracks the blob
// What it does: the camera sits on a servo: blob on the left of the picture → servo turns left until the blob is back in the middle; no blob → puzzled face, servo holds. Press A to centre.
// Wiring: camera SDA → P20, SCL → P19, VCC → external 5 V, GND → GND. Servo orange (signal) → P1, red → external 4.8–6 V battery +, brown → battery − joined to micro:bit GND.
// Setup: camera with 'Setting 2: TRACK' done (the tracking box follows the blob); a 9 g servo (SG90) with the camera fixed to its arm.
// You see: move the blob slowly left and the servo follows so the blob stays centred; take the blob away → puzzled face.

const CENTRE = 50                                         // blob X 0..100, 50 = picture centre
const DEAD = 8                                            // hold still within 8 of centre
let angle = 90

basic.forever(function () {
    if (mbc.camFound()) {                                 // a blob of the target colour is found
        let err = mbc.camBlob(MbcCamBlob.X) - CENTRE      // positive = blob on the right
        if (Math.abs(err) > DEAD) {
            angle = Math.constrain(angle - err / 10, 0, 180)   // blob on the right → smaller angle = turn right (swap − for + if mounted mirrored)
            basic.showArrow(err > 0 ? ArrowNames.East : ArrowNames.West, 0)
        } else {
            basic.showIcon(IconNames.Target, 0)           // locked on
        }
        pins.servoWritePin(AnalogPin.P1, Math.round(angle))
    } else {
        basic.showIcon(IconNames.Confused, 0)
    }
    basic.pause(40)
})

input.onButtonPressed(Button.A, function () {
    angle = 90
    pins.servoWritePin(AnalogPin.P1, 90)
})
