// Distance sensor 8-ch × micro:bit example 1: radar view (where the nearest object is and how close)
// What it does: one LED column: its left / right position = direction of the nearest object, its height = how close (closer = taller). Press A to show the nearest distance (cm), B the direction (−7..+7).
// Wiring: sensor SDA → P20, SCL → P19, VCC → external 5 V (8 lasers draw too much for the micro:bit 3V pin), GND → GND (common ground).
// Setup: MakeCode new project → Extensions → paste https://github.com/contactmbcrobot/pxt-mbc-sensors → switch to JavaScript, paste this whole file → download to the micro:bit.
// You see: a tick after 2–4 s (all 8 lasers up); hand front-left → column on the left; straight ahead → middle; closer = taller; nothing around → all dark.

let busy = false

if (mbc.waitReady(mbc.ADDR_TOF, 5000)) {                  // the 8 lasers take about 2–4 s to start
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // no answer: check P19 / P20 and the external power
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let dir = mbc.tof8NearestDirection()                  // −7 (far left) .. +7 (far right), 0 = straight ahead; ±8 = nothing around
    let d = mbc.tof8Nearest(MbcDistUnit.Cm)               // the nearest of the 8, 0..120 cm
    basic.clearScreen()
    if (dir >= -7 && dir <= 7 && d < 120) {
        let x = Math.round(Math.map(dir, -7, 7, 0, 4))    // direction → column 0..4
        let h = Math.round(Math.map(d, 0, 120, 5, 1))     // distance → height 1..5
        for (let y = 5 - h; y < 5; y++) {
            led.plot(x, y)
        }
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.tof8Nearest(MbcDistUnit.Cm))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.tof8NearestDirection())
    busy = false
})
