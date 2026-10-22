// IMU × micro:bit example 3: stabilised turret (the servo keeps pointing the same way)
// What it does: a servo mounted next to the IMU with an arrow on top: rotate the base and the servo counter-rotates so the arrow keeps pointing the original way (within ±90°). Press A to reset the reference direction.
// Wiring: IMU SDA → P20, SCL → P19, VCC → 3V, GND → GND. Servo orange (signal) → P1, red → external 4.8–6 V battery +, brown → battery − joined to micro:bit GND.
// Setup: a 9 g servo (SG90) and a board that holds the micro:bit, the IMU and the servo together.
// You see: turn the base right by 45° → the servo turns left 45°, the arrow still points the same way; beyond 90° the servo hits its end stop.

mbc.imuZeroHeading()
basic.showIcon(IconNames.Yes)

basic.forever(function () {
    let h = mbc.imuHeading(MbcImuHeading.Fused)           // positive when turned right
    let angle = Math.constrain(90 - h, 0, 180)            // base turns right → servo turns left
    pins.servoWritePin(AnalogPin.P1, angle)
    basic.pause(20)
})

input.onButtonPressed(Button.A, function () {
    mbc.imuZeroHeading()                                  // the current direction becomes the new 0°
    basic.showIcon(IconNames.Yes, 0)
})
