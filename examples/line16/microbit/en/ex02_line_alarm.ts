// Line follower 16-ch × micro:bit example 2: lost-line alarm + one-key calibration
// What it does: line seen → happy face; line lost → sad face and a beep. Press A for the 5-second auto calibration (LED countdown), B to switch black / white line.
// Wiring: sensor SDA → P20, SCL → P19, VCC → 3V (or external 5 V), GND → GND. On a V1 connect a buzzer or headphones to P0 for sound (V2 has a speaker).
// Setup: a sheet of white paper with a strip of black tape. Extension and paste steps as in example 1.
// You see: sensor over the line → happy; lifted away → sad + beep; press A → 5 4 3 2 1, the calibration count, then a tick.

let white = false                                         // white-line mode on?

mbc.lineSetColor(MbcLineColor.Black)                      // start in black-line mode (the default)
basic.showIcon(IconNames.Happy)

mbc.onLineEvent(MbcLineEvent.Lost, function () {          // fires once when the segment count drops to 0
    basic.showIcon(IconNames.Sad, 0)
    music.playTone(Note.C, music.beat(BeatFraction.Half)) // low C for half a beat
})
mbc.onLineEvent(MbcLineEvent.Found, function () {         // line found again
    basic.showIcon(IconNames.Happy, 0)
})

input.onButtonPressed(Button.A, function () {
    mbc.lineCalibrate(MbcLineCalib.Auto)                  // sweep the sensor over black line and white paper for 5 s
    for (let i = 5; i > 0; i--) {
        basic.showNumber(i)                               // countdown
        basic.pause(1000)
    }
    while (mbc.lineCalibrating()) {                       // wait until the calibrating flag clears
        basic.pause(100)
    }
    basic.showNumber(mbc.lineCalibrationCount())          // calibrations done since power-on
    basic.showIcon(IconNames.Yes)
})

input.onButtonPressed(Button.B, function () {
    white = !white
    mbc.lineSetColor(white ? MbcLineColor.White : MbcLineColor.Black)   // black line on white ↔ white line on black
    basic.showString(white ? "W" : "B")
})
