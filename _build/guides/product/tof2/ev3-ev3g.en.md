### Which side is left

**Left and right are as seen from behind the sensor, looking the way it faces (left = J5, right = J3).** Facing the sensor, they are the other way round, so when you test with your hand, stand behind the sensor and look the same way it does. "Left is closer" and "turn left" in the examples follow this direction.

### EV3 test results and tips

**Passed hardware testing on the stock EV3 firmware (2026-11-02)**: the EV3 sees the Ranger 2-Way as an **EV3 infrared sensor** (type 33), all 6 modes have the same structure as the LEGO infrared sensor, and it is recognised both when plugged in before power-on and when plugged in later (about 1.4 s after plugging in). The infrared sensor blocks in EV3-G work as they are.

- **Push the plug all the way in, in one go.**
- **The EV3 needs about 1.1 s to change infrared mode** (the LEGO infrared sensor does too). Keep to blocks of one mode in a program: use "Proximity" for the average distance and "Beacon" for the left and right distances.
- On the stock firmware the "Remote" mode sometimes reads 0 right after switching to it; use Beacon for the left and right distances.
