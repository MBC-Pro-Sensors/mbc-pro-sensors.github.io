> [!NOTE]
> On the EV3 the IMU shows up as a "**Gyro Sensor**" (not an infrared sensor); read it with the original gyro sensor blocks.
>
> **Tested (2026-11-02, stock EV3 firmware + EV3-G):** the Angle block and the Reset block both work. In the "Angle and Rate" block, the **rate is the magnetometer heading** (relative, not degrees per second); for turning speed, subtract two Angle readings.
