// Gamepad receiver PS4/PS5 × micro:bit example 1: joystick dot (an LED dot follows the left stick)
// What it does: push the left stick and the lit dot follows; no gamepad connected → an X. Press A to show the left stick X number, B the Y.
// Wiring: receiver SDA → P20, SCL → P19, VCC → external 5 V (the ESP32 receiver draws too much for the micro:bit 3V pin), GND → GND (common ground).
// Setup: pair the gamepad with the receiver first (see the product page). MakeCode new project → Extensions → paste https://github.com/contactmbcrobot/pxt-mbc-sensors → switch to JavaScript, paste this whole file → download to the micro:bit.
// You see: a tick at power-on (receiver found); gamepad off → X; stick released → dot in the centre; pushed up-right → dot in the top-right corner.

let busy = false

if (mbc.waitReady(mbc.ADDR_PAD, 3000)) {
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // no answer: check P19 / P20 and the external power
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (!mbc.padConnected()) {                            // no gamepad connected
        basic.showIcon(IconNames.No, 0)
        basic.pause(200)
        return
    }
    let sx = mbc.padStick(MbcPadAxis.LeftX)               // −100 (left) .. +100 (right), 0 = centre
    let sy = mbc.padStick(MbcPadAxis.LeftY)               // −100 (down) .. +100 (up)
    let x = Math.round(Math.map(sx, -100, 100, 0, 4))
    let y = Math.round(Math.map(sy, -100, 100, 4, 0))     // up = positive → row 0
    basic.clearScreen()
    led.plot(x, y)
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.padStick(MbcPadAxis.LeftX))
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.padStick(MbcPadAxis.LeftY))
    busy = false
})
