// Line follower 8-ch × micro:bit example 1: where is the line (an LED column follows the line)
// What it does: slide the sensor across a black line and a column of LEDs follows it; press A to show the line position number, B to show the line width.
// Wiring: sensor SDA → P20, SCL → P19, VCC → 3V (or external 5 V), GND → GND.
// Setup: MakeCode new project → Extensions → paste https://github.com/contactmbcrobot/pxt-mbc-sensors → switch to JavaScript, paste this whole file → download to the micro:bit.
// You see: a tick at power-on (sensor found); line on the left → column on the left; centred → middle column; no line → an X.

const CH = 8                                              // channels: 8 on the 8-ch, 16 on the 16-ch
let busy = false                                          // pause the column while a button shows a number

if (mbc.waitReady(mbc.ADDR_LINE, 3000)) {                 // wait up to 3 s for the sensor to answer
    basic.showIcon(IconNames.Yes)
} else {
    basic.showIcon(IconNames.No)                          // no answer: check P19 / P20 and power
}

basic.forever(function () {
    if (busy) {
        basic.pause(100)
        return
    }
    if (mbc.lineSeen()) {
        let pos = mbc.linePosition()                      // −CH (far left) .. +CH (far right), 0 = centre
        let x = Math.round(Math.map(pos, -CH, CH, 0, 4))  // map to LED column 0..4
        basic.clearScreen()
        for (let y = 0; y < 5; y++) {
            led.plot(x, y)                                // draw the whole column
        }
    } else {
        basic.showIcon(IconNames.No, 0)                   // no line seen
    }
    basic.pause(50)
})

input.onButtonPressed(Button.A, function () {
    busy = true
    basic.showNumber(mbc.linePosition())                  // line position: negative = line on the left (seen from behind the robot)
    busy = false
})
input.onButtonPressed(Button.B, function () {
    busy = true
    basic.showNumber(mbc.lineWidth())                     // line width: channels on the line, 0 = no line
    busy = false
})
