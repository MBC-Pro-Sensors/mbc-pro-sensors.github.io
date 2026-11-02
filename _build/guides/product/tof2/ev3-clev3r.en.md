### Which side is left

**Left and right are as seen from behind the sensor, looking the way it faces (left = J5, right = J3).** Facing the sensor, they are the other way round, so when you test with your hand, stand behind the sensor and look the same way it does. "Left is closer" and "turn left" in the examples follow this direction.

### EV3 test results and tips

**Passed hardware testing on the stock EV3 firmware (2026-11-02)**: clev3r programs run on the stock EV3 firmware, which sees the Ranger 2-Way as an **EV3 infrared sensor** (`Sensor.GetType` = 33). All 6 modes have the same structure as the LEGO infrared sensor, and it is recognised both when plugged in before power-on and when plugged in later. clev3r can be used as it is.

- **Push the plug all the way in, in one go.**
- **The EV3 needs about 1.1 s to change infrared mode** (the LEGO infrared sensor does too). Call `Sensor.SetMode` once at the start and keep reading the same mode: mode 5 for left / right mm, mode 0 for the average distance.
- On the stock firmware, switching to mode 3 (average of left and right in mm) sometimes does not take (you get mode 0 numbers until you switch again). For the average in millimetres, read mode 5 and work out (left + right) ÷ 2 yourself.
