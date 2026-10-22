// Gamepad receiver PS4/PS5 × micro:bit example 2: buttons make pictures and sounds (light bar, rumble)
// What it does: triangle / circle / cross / square each show a picture and play a note; the d-pad shows arrows; cross rumbles the gamepad and the light bar changes colour with the button.
// Wiring: receiver SDA → P20, SCL → P19, VCC → external 5 V, GND → GND (common ground). On a V1 connect a buzzer or headphones to P0 for sound (V2 has a speaker).
// Setup: gamepad paired; extension and paste steps as in example 1.
// You see: triangle → a triangle + C; circle → a target + E; cross → an X + G and a rumble; square → a square + high C; d-pad → arrows in four directions.

mbc.onPadButton(MbcPadButton.Triangle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Triangle, 0)
    mbc.padLight(0x00FF00)                                // light bar green (like the triangle on the pad)
    music.playTone(Note.C, 200)
})
mbc.onPadButton(MbcPadButton.Circle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Target, 0)
    mbc.padLight(0xFF0000)                                // red
    music.playTone(Note.E, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.No, 0)
    mbc.padLight(0x0000FF)                                // blue
    mbc.padRumble(200)                                    // rumble (0..255)
    music.playTone(Note.G, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Released, function () {
    mbc.padRumble(0)                                      // stop rumbling on release
    basic.clearScreen()
})
mbc.onPadButton(MbcPadButton.Square, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Square, 0)
    mbc.padLight(0xFF00FF)                                // pink
    music.playTone(Note.C5, 200)
})
mbc.onPadButton(MbcPadButton.Up, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.North, 0)
})
mbc.onPadButton(MbcPadButton.Down, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.South, 0)
})
mbc.onPadButton(MbcPadButton.Left, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.West, 0)
})
mbc.onPadButton(MbcPadButton.Right, MbcPadButtonEvent.Pressed, function () {
    basic.showArrow(ArrowNames.East, 0)
})

basic.forever(function () {
    if (!mbc.padConnected()) {                            // no gamepad: show a small heart as a reminder
        basic.showIcon(IconNames.SmallHeart, 0)
    }
    basic.pause(500)
})
