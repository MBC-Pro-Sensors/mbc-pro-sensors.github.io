// Vision sensor ESP32CAM (school edition) × micro:bit example 1: what colour is it (hue bar)
// What it does: the LED bar shows the hue at the centre of the picture (red 0 → yellow 17 → green 33 → blue 67 → red 100); black / white / grey shows a small square. Press A to show the hue number, B the saturation.
// Wiring: camera SDA (IO3) → P20, SCL (IO1) → P19, VCC → external 5 V (the ESP32 draws too much for the micro:bit 3V pin), GND → GND (common ground).
// Setup: do 'Setting 1: COLOR' on the camera first (see the product page). MakeCode new project → Extensions → paste https://github.com/contactmbcrobot/pxt-mbc-sensors → switch to JavaScript, paste this whole file → download to the micro:bit.
// You see: a tick at power-on (camera found); a red brick → bar almost empty (0); green → 1/3 lit; blue → 2/3 lit; white paper → a small square.

let busy = false

if (mbc.waitReady(mbc.ADDR_CAM, 5000)) {                  // the camera takes about 3 s to boot
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // no answer: check P19 / P20 and the external power; power the camera before the micro:bit
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (mbc.camIsGrey()) {                                // hue −1: black, white or grey
        basic.showIcon(IconNames.SmallSquare, 0)
    } else {
        led.plotBarGraph(mbc.camColor(MbcCamColor.Hue), 100)   // 0..100
    }
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.camColor(MbcCamColor.Hue))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.camColor(MbcCamColor.Saturation))   // 0..100, higher = more vivid
    busy = false
})
