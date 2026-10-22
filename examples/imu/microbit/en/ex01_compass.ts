// IMU × micro:bit example 1: electronic compass (the arrow keeps pointing where 'front' was at power-on)
// What it does: turn the micro:bit and the IMU together and the LED arrow turns the other way, always pointing at the original front. Press A to show the heading, B to make the current direction 0.
// Wiring: IMU SDA → P20, SCL → P19, VCC → 3V (or external 5 V), GND → GND.
// Setup: MakeCode new project → Extensions → paste https://github.com/contactmbcrobot/pxt-mbc-sensors → switch to JavaScript, paste this whole file → download to the micro:bit. Keep the IMU flat and still for 2 s after power-on.
// You see: tick at power-on, arrow up; turn right 90° → arrow points left; 180° → arrow down; turn back → arrow up.

const ARROWS = [ArrowNames.North, ArrowNames.NorthEast, ArrowNames.East, ArrowNames.SouthEast,
    ArrowNames.South, ArrowNames.SouthWest, ArrowNames.West, ArrowNames.NorthWest]   // 8 directions clockwise
let busy = false

if (mbc.waitReady(mbc.ADDR_IMU, 3000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // no answer: check P19 / P20 and power
}
mbc.imuZeroHeading()                                      // current direction = 0°

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // degrees, grows turning right, shrinks turning left, may exceed 360
    let a = ((h % 360) + 360) % 360                       // fold to 0..359
    let idx = Math.round(a / 45) % 8                      // how many 45° steps turned
    basic.showArrow(ARROWS[(8 - idx) % 8], 0)             // the arrow points the opposite way, back to the original front
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.imuHeading(MbcImuHeading.Fused))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    mbc.imuZeroHeading()
})
