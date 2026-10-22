// Gamepad receiver PS4/PS5 × micro:bit example 5: two-stick tank drive + safe start (all together)
// What it does: left stick drives the left wheel, right stick the right wheel, further = faster. Both sticks must be released (centred) once before the car is 'armed'; it locks again on disconnect. Press A to show the battery; below 20% the light bar turns red.
// Wiring: as example 4 (receiver P19 / P20, external 5 V; left wheel P1, right wheel P2, servos on their own battery, common ground).
// Setup: the two-wheel car.
// You see: release both sticks → tick (armed), green light bar; push both forward → forward; one forward one back → spin; release → stop; gamepad off → X and locked.

const DEAD = 15                                           // dead zone: within ±15 counts as 0
let armed = false
let lowBattery = false

// left servo P1, right servo P2 (continuous rotation); left / right = −100..100, positive = forward
function drive(left: number, right: number): void {
    left = Math.constrain(left, -100, 100)
    right = Math.constrain(right, -100, 100)
    pins.servoWritePin(AnalogPin.P1, Math.round(90 + left * 0.9))    // 90 = stop, 180 = full forward, 0 = full reverse
    pins.servoWritePin(AnalogPin.P2, Math.round(90 - right * 0.9))   // the right servo is mounted mirrored, so subtract
}

function deadZone(v: number): number {                    // treat the stick near centre as 0 so the car really stops
    if (Math.abs(v) < DEAD) {
        return 0
    }
    return v
}

input.onButtonPressed(Button.A, function () {
    basic.showNumber(mbc.padBattery())                    // gamepad battery 0..100 (PS2 pads always 0)
})

basic.forever(function () {
    if (!mbc.padConnected()) {
        drive(0, 0)
        armed = false                                     // must re-arm after a disconnect
        basic.showIcon(IconNames.No, 0)
        basic.pause(200)
        return
    }
    let ly = deadZone(mbc.padStick(MbcPadAxis.LeftY))     // left stick up / down −100..+100
    let ry = deadZone(mbc.padStick(MbcPadAxis.RightY))    // right stick up / down
    if (!armed) {
        if (ly == 0 && ry == 0) {                         // arm only when both sticks are released
            armed = true
            basic.showIcon(IconNames.Yes, 0)
            mbc.padLight(0x00FF00)                        // green light bar = ready to drive
        }
        basic.pause(50)
        return
    }
    drive(ly, ry)                                         // left stick → left wheel, right stick → right wheel
    let low = mbc.padBattery() < 20
    if (low != lowBattery) {                              // change the light bar only when the battery state changes
        lowBattery = low
        mbc.padLight(low ? 0xFF0000 : 0x00FF00)           // red when low, green otherwise
    }
    basic.pause(20)
})
