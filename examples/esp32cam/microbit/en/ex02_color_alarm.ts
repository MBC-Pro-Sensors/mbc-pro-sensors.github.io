// Vision sensor ESP32CAM (school edition) × micro:bit example 2: beep when the chosen colour is seen
// What it does: the LEDs show the colour to look for (R red, Y yellow, G green, B blue); when the picture centre shows it → heart + beep. A / B select the next / previous colour.
// Wiring: camera SDA → P20, SCL → P19, VCC → external 5 V, GND → GND (common ground). On a V1 connect a buzzer or headphones to P0 for sound (V2 has a speaker).
// Setup: camera with 'Setting 1: COLOR' done; red, yellow, green and blue bricks.
// You see: the LEDs show R; a red brick in front → heart + beep; a green brick → nothing; press A → Y, now only the yellow brick triggers it.

const NAMES = ["R", "Y", "G", "B"]
const HUES = [0, 17, 33, 67]                              // hues of red, yellow, green, blue (0..100)
const TOL = 8                                             // within 8 counts as the same colour
let sel = 0

// hue is a circle: 0 and 100 are both red, so a difference over 50 wraps the other way
function sameHue(h: number, target: number): boolean {
    let d = Math.abs(h - target)
    if (d > 50) {
        d = 100 - d
    }
    return d <= TOL
}

input.onButtonPressed(Button.A, function () {
    sel = (sel + 1) % 4
})
input.onButtonPressed(Button.B, function () {
    sel = (sel + 3) % 4
})

basic.forever(function () {
    let h = mbc.camColor(MbcCamColor.Hue)                 // −1 = black / white / grey
    if (!mbc.camIsGrey() && sameHue(h, HUES[sel])) {
        basic.showIcon(IconNames.Heart, 0)
        music.playTone(Note.E, 150)
    } else {
        basic.showString(NAMES[sel])                      // a single letter shows without scrolling
    }
    basic.pause(100)
})
