### Which side is left

**Left and right are as seen from behind the sensor, looking the way it faces (left = J5, right = J3).** Facing the sensor, they are the other way round, so when you test with your hand, stand behind the sensor and look the same way it does. "Left is closer" and "turn left" in the examples follow this direction.

### Test results (Pybricks 4.1.2)

The SPIKE edition **passed hardware testing on Pybricks 4.1.2 (2026-11-02)**: readings, the library and the beginner examples were all run on a hub.

- When you read it directly with `ColorSensor`, sensor firmware from 2026-11 on gives a whole number from `reflection()` on Pybricks 4.x (for example 31); older firmware gives one decimal place (for example 30.8). Writing `int(...)` works with both, so just keep it.
