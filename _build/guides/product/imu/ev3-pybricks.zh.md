> [!NOTE]
> 陀螺儀 IMU 在 EV3 上會被當成原廠陀螺儀，範例程式用 `GyroSensor` 讀。
>
> **Pybricks v4 實測通過（2026-11-02，11/11 項）**：`angle()`、`reset_angle(0)` 正常。`speed()` 讀到的是**磁力計航向**（不是每秒幾度）；要角速度請用 `LUMPDevice` 讀模式 2（見 [EV3 新手範例](/sensors/imu/ev3-examples.md) 範例 7）。`calibrate()` 之後約 3.5 秒才讀得到值（原廠陀螺儀也一樣）。
