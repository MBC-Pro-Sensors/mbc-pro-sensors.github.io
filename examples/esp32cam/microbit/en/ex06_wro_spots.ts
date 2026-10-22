// Vision sensor ESP32CAM (WRO edition, HUE12) × micro:bit example 6: all 12 colour spots at a glance
// What it does: the top 3 LED rows = spots S1..S12 (4 per row): colour → bright, white → dim, black or off → dark. The dot on row 5 = the selected spot. Press A to select the next spot, B to show its colour code and vividness.
// Wiring: camera SDA (IO3) → P20, SCL (IO1) → P19, VCC → external 5 V, GND → GND (common ground).
// Setup: on the WRO edition camera set the 12 spot positions first (see the product page).
// You see: spots over coloured bricks bright, over white dim, over black dark; press B for the colour code: 0..239 colour (red ≈ 0, yellow ≈ 40, green ≈ 80, blue ≈ 160), 240..245 white, 246..250 black, 255 = spot off.

let sel = 1                                               // selected spot 1..12
let busy = false

if (mbc.waitReady(mbc.ADDR_CAM, 5000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    basic.clearScreen()
    for (let n = 1; n <= 12; n++) {
        let x = (n - 1) % 4                               // 4 per row
        let y = Math.idiv(n - 1, 4)                       // rows 0..2
        if (mbc.camSpotIs(n, MbcCamSpotKind.Colour)) {    // code 0..239
            led.plotBrightness(x, y, 255)
        } else if (mbc.camSpotIs(n, MbcCamSpotKind.White)) {   // 240..245
            led.plotBrightness(x, y, 40)
        }
    }
    led.plot((sel - 1) % 4, 4)                            // row 5: column of the selected spot
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    sel = sel % 12 + 1                                    // 1 → 2 → … → 12 → 1
    busy = true
    basic.showString("S" + sel)
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.camSpotColor(sel))               // colour code 0..255
    basic.showString("V")
    basic.showNumber(mbc.camSpotVivid(sel))               // vividness 0..255
    busy = false
})
