// Gamepad receiver PS2 × micro:bit example 2: buttons make pictures and sounds
// What it does: triangle / circle / cross / square each show a picture and play a note; the d-pad shows arrows.
// Wiring: receiver SDA → P20, SCL → P19, VCC → 3V (or external 5 V), GND → GND (common ground). On a V1 connect a buzzer or headphones to P0 for sound (V2 has a speaker).
// Setup: gamepad paired; extension and paste steps as in example 1.
// You see: triangle → a triangle + C; circle → a target + E; cross → an X + G; square → a square + high C; d-pad → arrows in four directions.

mbc.onPadButton(MbcPadButton.Triangle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Triangle, 0)
    music.playTone(Note.C, 200)
})
mbc.onPadButton(MbcPadButton.Circle, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Target, 0)
    music.playTone(Note.E, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.No, 0)
    music.playTone(Note.G, 200)
})
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Released, function () {
    basic.clearScreen()
})
mbc.onPadButton(MbcPadButton.Square, MbcPadButtonEvent.Pressed, function () {
    basic.showIcon(IconNames.Square, 0)
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
