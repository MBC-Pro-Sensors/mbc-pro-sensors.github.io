<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 產生，請勿手改 -->
# 指令參考索引：產品 × 平台

每款產品一頁「指令參考」，列出每個值在六個平台各怎麼讀；表格裡每一格連到那個平台的說明頁。

| 產品 | I2C | SPIKE | EV3 | Arduino I2C | mBlock | MATRIX | micro:bit |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| [循行者 8 路 (Line8)](/sensors/line8/reference.md) | `0x16` | [SPIKE](/sensors/line8/reference.md) | [EV3](/sensors/line8/reference.md) | [Arduino I2C](/sensors/line8/arduino-i2c.md) | [mBlock](/sensors/line8/mblock.md) | [MATRIX](/sensors/line8/matrix.md) | [micro:bit](/sensors/line8/microbit.md) |
| [循行者 16 路 (Line16)](/sensors/line16/reference.md) | `0x16` | [SPIKE](/sensors/line16/reference.md) | [EV3](/sensors/line16/reference.md) | [Arduino I2C](/sensors/line16/arduino-i2c.md) | [mBlock](/sensors/line16/mblock.md) | [MATRIX](/sensors/line16/matrix.md) | [micro:bit](/sensors/line16/microbit.md) |
| [測距者 2 路 (TOF2)](/sensors/tof2/reference.md) | `0x29` | [SPIKE](/sensors/tof2/reference.md) | [EV3](/sensors/tof2/reference.md) | [Arduino I2C](/sensors/tof2/arduino-i2c.md) | [mBlock](/sensors/tof2/mblock.md) | [MATRIX](/sensors/tof2/matrix.md) | [micro:bit](/sensors/tof2/microbit.md) |
| [測距者 8 路 (TOF8)](/sensors/tof8/reference.md) | `0x29` | [SPIKE](/sensors/tof8/reference.md) | [EV3](/sensors/tof8/reference.md) | [Arduino I2C](/sensors/tof8/arduino-i2c.md) | [mBlock](/sensors/tof8/mblock.md) | [MATRIX](/sensors/tof8/matrix.md) | [micro:bit](/sensors/tof8/microbit.md) |
| [掌控者 PS2 (PS2)](/sensors/ps2/reference.md) | `0x42` | [SPIKE](/sensors/ps2/reference.md) | [EV3](/sensors/ps2/reference.md) | [Arduino I2C](/sensors/ps2/arduino-i2c.md) | [mBlock](/sensors/ps2/mblock.md) | [MATRIX](/sensors/ps2/matrix.md) | [micro:bit](/sensors/ps2/microbit.md) |
| [掌控者 PS4/PS5](/sensors/ps4/reference.md) | `0x42` | [SPIKE](/sensors/ps4/reference.md) | [EV3](/sensors/ps4/reference.md) | [Arduino I2C](/sensors/ps4/arduino-i2c.md) | [mBlock](/sensors/ps4/mblock.md) | [MATRIX](/sensors/ps4/matrix.md) | [micro:bit](/sensors/ps4/microbit.md) |
| [神攝手 ESP32CAM](/sensors/esp32cam/reference.md) | `0x4C` | [SPIKE](/sensors/esp32cam/reference.md) | [EV3](/sensors/esp32cam/reference.md) | [Arduino I2C](/sensors/esp32cam/arduino-i2c.md) | [mBlock](/sensors/esp32cam/mblock.md) | [MATRIX](/sensors/esp32cam/matrix.md) | [micro:bit](/sensors/esp32cam/microbit.md) |
| [神攝手通用版 v1](/sensors/unicam/index.md) | — | [SPIKE](/sensors/unicam/reference.md) | [EV3](/sensors/unicam/reference.md) | [Arduino I2C](/sensors/unicam/reference.md) | [mBlock](/sensors/unicam/reference.md) | [MATRIX](/sensors/unicam/reference.md) | [micro:bit](/sensors/unicam/reference.md) |
| [SPIKE 6 路擴充器 (EXP6)](/sensors/exp6/index.md) | — | [SPIKE](/sensors/exp6/spike-pybricks.md) | — | [Arduino I2C](/sensors/exp6/arduino-examples.md) | — | — | — |
| [陀螺儀 IMU 九軸](/sensors/imu/reference.md) | `0x6A` | — | [EV3](/sensors/imu/reference.md) | [Arduino I2C](/sensors/imu/arduino-i2c.md) | [mBlock](/sensors/imu/mblock.md) | [MATRIX](/sensors/imu/matrix.md) | [micro:bit](/sensors/imu/microbit.md) |
| [SPIKE 6 路舵機擴充器 (Servo6)](/sensors/servo6/index.md) | — | [SPIKE](/sensors/servo6/spike-pybricks.md) | — | — | — | — | — |
| [遠距離顏色感應器 OV3660](/sensors/ov3660/index.md) | — | — | — | — | — | — | — |

## 共用規則

- [通用 I2C 協議 v2](/i2c-protocol.md)
- [mBlock 擴展](/makeblock.md)
- [協定純文字版（給 AI）](/llms-protocol.txt)

## 已產生指令參考的產品

- [神攝手 學校版](/sensors/esp32cam/reference.md)：10 個值、0 個命令
- [神攝手 WRO 版（HUE12）](/sensors/esp32cam/reference-wro.md)：2 個值、0 個命令
- [IMU 陀螺儀](/sensors/imu/reference.md)：13 個值、5 個命令
- [循行者 16 路](/sensors/line16/reference.md)：9 個值、6 個命令
- [循行者 8 路](/sensors/line8/reference.md)：9 個值、6 個命令
- [掌控者 PS2](/sensors/ps2/reference.md)：12 個值、2 個命令
- [掌控者 PS4／PS5 接收器](/sensors/ps4/reference.md)：12 個值、2 個命令
- [測距者 2 路](/sensors/tof2/reference.md)：8 個值、0 個命令
- [測距者 8 路](/sensors/tof8/reference.md)：11 個值、0 個命令
