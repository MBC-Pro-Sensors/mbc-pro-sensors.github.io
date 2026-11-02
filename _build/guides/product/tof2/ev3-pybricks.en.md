### Which side is left

**Left and right are as seen from behind the sensor, looking the way it faces (left = J5, right = J3).** Facing the sensor, they are the other way round, so when you test with your hand, stand behind the sensor and look the same way it does. "Left is closer" and "turn left" in the examples follow this direction.

### EV3 test results and tips

**Passed hardware testing on the stock EV3 firmware and Pybricks v4 (2026-11-02)**: the EV3 sees the Ranger 2-Way as an **EV3 infrared sensor** (type 33), all 6 modes have the same structure as the LEGO infrared sensor, and it is recognised both when plugged in before power-on and when plugged in later. Both Pybricks v4 and EV3 MicroPython v2 can be used.

- **Push the plug all the way in, in one go.**
- **With the built-in `InfraredSensor` class, use `distance()` only** (average of left and right, 0 ~ 100, 100 = nothing ahead). **Do not use `buttons()`**: mode 2 holds the left and right distances, Pybricks treats them as remote button codes and raises `OSError` (error 5, EIO). `beacon()` returns numbers, but mode 1 also holds the left and right distances, so they mean nothing as a beacon. For left and right distances use the MBC library, or read mode 5 with `LUMPDevice`.
- **The EV3 needs about 1.1 s to change infrared mode** (the LEGO infrared sensor does too). Keep your program on one mode: mode 5 for left / right mm (work out the average yourself: (left + right) ÷ 2), mode 0 for the average distance.
- Do not use `InfraredSensor` and `LUMPDevice` on the same port at once; if you need both, create `InfraredSensor` first and then `LUMPDevice`, otherwise you may get ENODEV.
