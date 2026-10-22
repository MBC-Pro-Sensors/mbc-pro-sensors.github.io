<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 產生，請勿手改 -->
# Value reference index: product × host

Each product has one reference page listing how every value is read on the six hosts; every cell links to that host's page.

| Product | I2C | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| [Pathfinder 8-Way (Line8)](/en/sensors/line8/reference.md) | `0x16` | [SPIKE](/en/sensors/line8/reference.md) | [EV3](/en/sensors/line8/reference.md) | [Arduino I2C](/en/sensors/line8/arduino-i2c.md) | [mBlock](/en/sensors/line8/mblock.md) | [MATRIX](/en/sensors/line8/matrix.md) | [micro:bit](/en/sensors/line8/microbit.md) |
| [Pathfinder 16-Way (Line16)](/en/sensors/line16/reference.md) | `0x16` | [SPIKE](/en/sensors/line16/reference.md) | [EV3](/en/sensors/line16/reference.md) | [Arduino I2C](/en/sensors/line16/arduino-i2c.md) | [mBlock](/en/sensors/line16/mblock.md) | [MATRIX](/en/sensors/line16/matrix.md) | [micro:bit](/en/sensors/line16/microbit.md) |
| [Ranger 2-Way (TOF2)](/en/sensors/tof2/reference.md) | `0x29` | [SPIKE](/en/sensors/tof2/reference.md) | [EV3](/en/sensors/tof2/reference.md) | [Arduino I2C](/en/sensors/tof2/arduino-i2c.md) | [mBlock](/en/sensors/tof2/mblock.md) | [MATRIX](/en/sensors/tof2/matrix.md) | [micro:bit](/en/sensors/tof2/microbit.md) |
| [Ranger 8-Way (TOF8)](/en/sensors/tof8/reference.md) | `0x29` | [SPIKE](/en/sensors/tof8/reference.md) | [EV3](/en/sensors/tof8/reference.md) | [Arduino I2C](/en/sensors/tof8/arduino-i2c.md) | [mBlock](/en/sensors/tof8/mblock.md) | [MATRIX](/en/sensors/tof8/matrix.md) | [micro:bit](/en/sensors/tof8/microbit.md) |
| [Controller 2.4G (PS2)](/en/sensors/ps2/reference.md) | `0x42` | [SPIKE](/en/sensors/ps2/reference.md) | [EV3](/en/sensors/ps2/reference.md) | [Arduino I2C](/en/sensors/ps2/arduino-i2c.md) | [mBlock](/en/sensors/ps2/mblock.md) | [MATRIX](/en/sensors/ps2/matrix.md) | [micro:bit](/en/sensors/ps2/microbit.md) |
| [Controller PS4/PS5](/en/sensors/ps4/reference.md) | `0x42` | [SPIKE](/en/sensors/ps4/reference.md) | [EV3](/en/sensors/ps4/reference.md) | [Arduino I2C](/en/sensors/ps4/arduino-i2c.md) | [mBlock](/en/sensors/ps4/mblock.md) | [MATRIX](/en/sensors/ps4/matrix.md) | [micro:bit](/en/sensors/ps4/microbit.md) |
| [Sharpshooter Vision (ESP32CAM)](/en/sensors/esp32cam/reference.md) | `0x4C` | [SPIKE](/en/sensors/esp32cam/reference.md) | [EV3](/en/sensors/esp32cam/reference.md) | [Arduino I2C](/en/sensors/esp32cam/arduino-i2c.md) | [mBlock](/en/sensors/esp32cam/mblock.md) | [MATRIX](/en/sensors/esp32cam/matrix.md) | [micro:bit](/en/sensors/esp32cam/microbit.md) |
| [SPIKE 6-Way Expander (EXP6)](/en/sensors/exp6/index.md) | — | [SPIKE](/en/sensors/exp6/spike-pybricks.md) | — | [Arduino I2C](/en/sensors/exp6/arduino-examples.md) | — | — | — |
| [IMU 9-Axis Gyro](/en/sensors/imu/reference.md) | `0x6A` | — | [EV3](/en/sensors/imu/reference.md) | [Arduino I2C](/en/sensors/imu/arduino-i2c.md) | [mBlock](/en/sensors/imu/mblock.md) | [MATRIX](/en/sensors/imu/matrix.md) | [micro:bit](/en/sensors/imu/microbit.md) |
| [SPIKE 6-Port Servo Expander (Servo6)](/en/sensors/servo6/index.md) | — | [SPIKE](/en/sensors/servo6/spike-pybricks.md) | — | — | — | — | — |
| [Long-Range Colour Sensor OV3660](/en/sensors/ov3660/index.md) | — | — | — | — | — | — | — |

## Shared rules

- [Universal I2C protocol v2](/en/i2c-protocol.md)
- [mBlock extension](/en/makeblock.md)
- [Plain-text protocol (for AI)](/llms-protocol.txt)

## Products with a generated reference

- [Vision Sensor (School Edition)](/en/sensors/esp32cam/reference.md)：10 values、0 commands
- [Vision Sensor (WRO Edition, HUE12)](/en/sensors/esp32cam/reference-wro.md)：2 values、0 commands
- [IMU Gyro Sensor](/en/sensors/imu/reference.md)：13 values、5 commands
- [Line Follower 16-ch](/en/sensors/line16/reference.md)：9 values、6 commands
- [Line Follower 8-ch](/en/sensors/line8/reference.md)：9 values、6 commands
- [PS2 Gamepad Receiver](/en/sensors/ps2/reference.md)：12 values、2 commands
- [Gamepad Receiver PS4/PS5](/en/sensors/ps4/reference.md)：12 values、2 commands
- [Laser Ranger 2-ch](/en/sensors/tof2/reference.md)：8 values、0 commands
- [Laser Ranger 8-ch](/en/sensors/tof8/reference.md)：11 values、0 commands
