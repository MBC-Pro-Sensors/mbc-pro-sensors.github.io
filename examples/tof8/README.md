# 測距者 8 路（TOF8）範例程式 / TOF8 Example Programs

## 繁體中文

第 1 顆 = 正左側（-90°）…第 8 顆 = 正右側（+90°），第 4、5 顆都朝正前方。

| 檔案 | 主機 | 韌體／軟體 | 說明 |
| :--- | :--- | :--- | :--- |
| `pybricks/For firmware 3.6.1/tof8_block_native_v361.py` | SPIKE Prime | Pybricks 3.6.1（舊版，不再更新） | 不用函式庫，以 `ColorSensor` 讀取：`reflection()` = 最近目標方位（-8 ~ +8）、`ambient()` = 最近距離（cm）。感應器接 Port C |
| `pybricks/For firmware 3.6.1/tof8_block_with_lib_v361.py` | SPIKE Prime | Pybricks 3.6.1（舊版，不再更新） | 積木版，使用函式庫 `MBC_laser8_Lib` 讀 8 顆距離。感應器接 Port C |
| `pybricks/For firmware 3.6.1/MBC_laser8_Lib.py` | SPIKE Prime | Pybricks 3.6.1（舊版，不再更新） | 函式庫（函式寫法）：8 顆距離（0 ~ 120 cm）、最近方位（0 ~ 16，8 = 正前方）與距離、初始化狀態 |
| `pybricks/For firmware 4.x/tof8_block_native.py` | SPIKE Prime | Pybricks 4.1 | 同 3.6.1 不用函式庫版本。感應器接 Port B |
| `pybricks/For firmware 4.x/tof8_block_with_lib.py` | SPIKE Prime | Pybricks 4.1 | 積木版，使用函式庫物件 `MBC_LASER8`，多工模式讀 8 顆距離。感應器接 Port B |
| `pybricks/For firmware 4.x/MBC_laser8_obj_Lib.py` | SPIKE Prime | Pybricks 4.1 | 函式庫（物件寫法）：8 顆距離、最近／最遠方位（-8 ~ +8，0 = 正前方）與距離、初始化與故障狀態 |
| `pybricks/For firmware 4.x/MBC_sensors.py` | SPIKE Prime／EV3 | Pybricks 4.1 | 共用函式庫：一個檔適用全部 MBC 感應器（`from MBC_sensors import MBC_LASER8`），新專案用這個 |
| `tof8_pybricks_spike_v4.x.zip` | SPIKE Prime | Pybricks 4.1 | 4.x 版全部檔案打包（含 `MBC_sensors.py`） |
| `tof8_pybricks_spike_v3.6.1.zip` | SPIKE Prime | Pybricks 3.6.1（舊版，不再更新） | 3.6.1 版全部檔案打包 |
| `arduino/tof8_01_hello/` … `arduino/tof8_06_avoid_car/` | Arduino Uno／Nano／ESP32／R4 | Arduino IDE | 通用 I2C 版（位址 `0x29`，I2C 協議 v2），6 個新手範例，說明見 [Arduino 範例頁](/sensors/tof8/arduino-examples.md) |
| `arduino/tof8_arduino_examples.zip`、`arduino-en/tof8_arduino_examples_en.zip` | 同上 | Arduino IDE | 6 個範例打包（中文註解／英文註解） |
| `tof8_arduino_examples.zip` | Arduino Uno／Nano | Arduino IDE | **舊版**：給 I2C 協議 v1 韌體（暫存器 `0x21`、`0x11`、`0x24`）用，v2 韌體請用上一列 |

**使用方式（Pybricks）**

1. 用 [Pybricks Code](https://code.pybricks.com) 開啟範例；主機請用 Pybricks 韌體 v4.1，開 `For firmware 4.x` 資料夾的檔案（`For firmware 3.6.1` 是舊版，已不再更新；兩版的函式庫不能混用）。
2. 使用函式庫的範例：把同資料夾的函式庫檔（`MBC_laser8_Lib.py` 或 `MBC_laser8_obj_Lib.py`）也匯入同一個 Pybricks 專案，**檔名不要改**。新專案可以改成只匯入 `MBC_sensors.py`（`from MBC_sensors import MBC_LASER8`）。
3. 依實際接線修改連接埠（函式庫的 port 參數：1 ~ 6 = A ~ F）。4.x 版 `MBC_LASER8(port, True)` 為多工模式，每個方法都要加 `await`。
4. 開機後約 2 ~ 4 秒完成 8 顆初始化，期間讀到的值可能是 120。

**使用方式（Arduino）**

1. 只適用**通用 I2C 版**硬體（SPIKE／EV3 版接頭不同、不能互換）。接線：A4 → SDA、A5 → SCL、5V → VCC、GND → GND。
2. 用 Arduino IDE 開啟資料夾內的 `.ino`，選 Arduino Uno（或 Nano）上傳，序列埠監控視窗設 115200 bps。
3. 編號 `18`～`1F` 是 8 顆距離（mm）：`1200` = 該方向沒有目標或該顆故障，要分辨故障就讀編號 `08`。

## English

Sensor 1 = far left (-90°) … sensor 8 = far right (+90°); sensors 4 and 5 both face straight ahead.

| File | Hub / Board | Firmware / Software | Description |
| :--- | :--- | :--- | :--- |
| `pybricks/For firmware 3.6.1/tof8_block_native_v361.py` | SPIKE Prime | Pybricks 3.6.1 (legacy, no longer updated) | No library. Reads through `ColorSensor`: `reflection()` = nearest target direction (-8 – +8), `ambient()` = nearest distance (cm). Sensor on Port C |
| `pybricks/For firmware 3.6.1/tof8_block_with_lib_v361.py` | SPIKE Prime | Pybricks 3.6.1 (legacy, no longer updated) | Block version using the `MBC_laser8_Lib` library to read all 8 distances. Sensor on Port C |
| `pybricks/For firmware 3.6.1/MBC_laser8_Lib.py` | SPIKE Prime | Pybricks 3.6.1 (legacy, no longer updated) | Library (function style): 8 distances (0 – 120 cm), nearest direction (0 – 16, 8 = front) and distance, init status |
| `pybricks/For firmware 4.x/tof8_block_native.py` | SPIKE Prime | Pybricks 4.1 | Same as the 3.6.1 no-library version. Sensor on Port B |
| `pybricks/For firmware 4.x/tof8_block_with_lib.py` | SPIKE Prime | Pybricks 4.1 | Block version using the `MBC_LASER8` library object in multitask mode. Sensor on Port B |
| `pybricks/For firmware 4.x/MBC_laser8_obj_Lib.py` | SPIKE Prime | Pybricks 4.1 | Library (object style): 8 distances, nearest / farthest direction (-8 – +8, 0 = front) and distance, init and fault status |
| `pybricks/For firmware 4.x/MBC_sensors.py` | SPIKE Prime / EV3 | Pybricks 4.1 | Shared library: one file for every MBC sensor (`from MBC_sensors import MBC_LASER8`); use it for new projects |
| `tof8_pybricks_spike_v4.x.zip` | SPIKE Prime | Pybricks 4.1 | All 4.x files in one archive (with `MBC_sensors.py`) |
| `tof8_pybricks_spike_v3.6.1.zip` | SPIKE Prime | Pybricks 3.6.1 (legacy, no longer updated) | All 3.6.1 files in one archive |
| `arduino/tof8_01_hello/` … `arduino/tof8_06_avoid_car/` | Arduino Uno / Nano / ESP32 / R4 | Arduino IDE | Universal I2C version (address `0x29`, I2C protocol v2). 6 beginner examples, see the [Arduino examples page](/en/sensors/tof8/arduino-examples.md) |
| `arduino/tof8_arduino_examples.zip`, `arduino-en/tof8_arduino_examples_en.zip` | Same | Arduino IDE | The 6 examples in one archive (Chinese / English comments) |
| `tof8_arduino_examples.zip` | Arduino Uno / Nano | Arduino IDE | **Legacy**: for I2C protocol v1 firmware (registers `0x21`, `0x11`, `0x24`). For v2 firmware use the row above |

**How to use (Pybricks)**

1. Open the example in [Pybricks Code](https://code.pybricks.com). Use Pybricks firmware v4.1 on the hub and the files in `For firmware 4.x` (`For firmware 3.6.1` is legacy and no longer updated; the two libraries are not interchangeable).
2. For examples that use a library, import the library file from the same folder (`MBC_laser8_Lib.py` or `MBC_laser8_obj_Lib.py`) into the same Pybricks project **without renaming it**. For a new project you can import only `MBC_sensors.py` instead (`from MBC_sensors import MBC_LASER8`).
3. Change the port to match your wiring (library port argument: 1 – 6 = A – F). In 4.x, `MBC_LASER8(port, True)` is multitask mode: put `await` before every method call.
4. The 8 sensors take about 2 – 4 s to initialize after power-up; values may read 120 during that time.

**How to use (Arduino)**

1. For the **Universal I2C version** hardware only (SPIKE / EV3 versions have different connectors and are not interchangeable). Wiring: A4 → SDA, A5 → SCL, 5V → VCC, GND → GND.
2. Open the `.ino` in the folder with the Arduino IDE, select Arduino Uno (or Nano), upload, and set the Serial Monitor to 115200 bps.
3. Ids `18`–`1F` are the 8 distances in mm: `1200` = no target that way or that sensor failed; read id `08` to tell a fault apart.
