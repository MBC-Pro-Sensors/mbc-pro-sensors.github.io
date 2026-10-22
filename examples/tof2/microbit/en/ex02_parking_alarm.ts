// Distance sensor 2-ch × micro:bit example 2: parking sensor (faster beeps when closer)
// What it does: starts beeping inside 40 cm, faster as you get closer; inside 10 cm it shows an X. Press A to see whether the object is more to the left or the right.
// Wiring: sensor SDA → P20, SCL → P19, VCC → 3V, GND → GND. On a V1 connect a buzzer or headphones to P0 for sound (V2 has a speaker).
// Setup: extension and paste steps as in example 1.
// You see: hand approaching → beep … beep … beepbeepbeep; inside 10 cm → X; press A → an arrow toward the nearer side.

mbc.onTof2Near(10, function () {                          // fires once each time the nearer distance drops below 10 cm
    basic.showIcon(IconNames.No, 0)
})

basic.forever(function () {
    let d = mbc.tof2NearestCm()                           // 0..120 cm
    if (d < 40) {
        led.plotBarGraph(40 - d, 40)
        music.playTone(Note.A, 100)                       // beep for 0.1 s
        basic.pause(d * 10)                               // 30 cm waits 0.3 s, 10 cm waits 0.1 s: closer = faster
    } else {
        basic.showIcon(IconNames.Happy, 0)                // all clear
        basic.pause(100)
    }
})

input.onButtonPressed(Button.A, function () {
    let l = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)
    let r = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)
    if (l < r) {
        basic.showArrow(ArrowNames.West)                  // the left sensor is nearer
    } else if (r < l) {
        basic.showArrow(ArrowNames.East)
    } else {
        basic.showArrow(ArrowNames.North)                 // equal: straight ahead
    }
})
