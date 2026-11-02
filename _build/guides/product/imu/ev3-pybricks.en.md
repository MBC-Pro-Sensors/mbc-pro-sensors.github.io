> [!NOTE]
> The EV3 treats the IMU as an original gyro sensor; the examples read it with `GyroSensor`.
>
> **Tested on Pybricks v4 (2026-11-02, 11/11 checks):** `angle()` and `reset_angle(0)` work normally. `speed()` returns the **magnetometer heading** (not degrees per second); for angular rate, read mode 2 with `LUMPDevice` (see example 7 in the [EV3 beginner examples](/en/sensors/imu/ev3-examples.md)). After `calibrate()` it takes about 3.5 s before readings come back (same as the LEGO gyro).
