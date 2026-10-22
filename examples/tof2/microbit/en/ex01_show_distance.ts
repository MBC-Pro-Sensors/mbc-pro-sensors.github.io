// Distance sensor 2-ch × micro:bit example 1: how far is it (LED bar + number)
// What it does: the closer your hand, the more LEDs light; press A to show the left sensor in cm, B the right one.
// Wiring: sensor SDA → P20, SCL → P19, VCC → 3V (or external 5 V), GND → GND.
// Setup: MakeCode new project → Extensions → paste https://github.com/contactmbcrobot/pxt-mbc-sensors → switch to JavaScript, paste this whole file → download to the micro:bit.
// You see: a tick at power-on; nothing in front → all dark; hand at 60 cm → half lit; within 10 cm → almost all lit.

let busy = false

if (mbc.waitReady(mbc.ADDR_TOF, 3000)) {                  // ready once both laser sensors are up
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // no answer: check P19 / P20 and power
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    let d = mbc.tof2NearestCm()                           // the nearer of the two, 0..120 cm, 120 = nothing in front
    led.plotBarGraph(120 - d, 120)                        // the closer, the longer the bar
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm))    // the left sensor (seen from behind the robot)
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm))   // the right sensor
    busy = false
})
