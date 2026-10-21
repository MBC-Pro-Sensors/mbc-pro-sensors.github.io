# 測距者 8 路（TOF8）範例程式 / TOF8 Example Programs

## 繁體中文

第 1 顆 = 正左側（-90°）…第 8 顆 = 正右側（+90°），第 4、5 顆都朝正前方。

| 檔案 | 主機 | 韌體／軟體 | 說明 |
| :--- | :--- | :--- | :--- |
| `pybricks/For firmware 3.6.1/tof8_block_native_v361.py` | SPIKE Prime | Pybricks 3.6.1 | 不用函式庫，以 `ColorSensor` 讀取：`reflection()` = 最近目標方位（-8 ~ +8）、`ambient()` = 最近距離（cm）。感應器接 Port C |
| `pybricks/For firmware 3.6.1/tof8_block_with_lib_v361.py` | SPIKE Prime | Pybricks 3.6.1 | 積木版，使用函式庫 `MBC_laser8_Lib` 讀 8 顆距離。感應器接 Port C |
| `pybricks/For firmware 3.6.1/MBC_laser8_Lib.py` | SPIKE Prime | Pybricks 3.6.1 | 函式庫（函式寫法）：8 顆距離（0 ~ 120 cm）、最近方位（0 ~ 16，8 = 正前方）與距離、初始化狀態 |
| `pybricks/For firmware 4.0.0/tof8_block_native_v400.py` | SPIKE Prime | Pybricks 4.0.0 | 同 3.6.1 不用函式庫版本。感應器接 Port B |
| `pybricks/For firmware 4.0.0/tof8_block_with_lib_v400.py` | SPIKE Prime | Pybricks 4.0.0 | 積木版，使用函式庫物件 `MBC_LASER8`，多工模式讀 8 顆距離。感應器接 Port B |
| `pybricks/For firmware 4.0.0/MBC_laser8_obj_Lib.py` | SPIKE Prime | Pybricks 4.0.0 | 函式庫（物件寫法）：8 顆距離、最近／最遠方位（-8 ~ +8，0 = 正前方）與距離、初始化與故障狀態 |
| `tof8_pybricks_spike_v3.6.1.zip`、`tof8_pybricks_spike_v4.0.0.zip` | SPIKE Prime | 3.6.1／4.0.0 | 各版本全部檔案打包 |
| `arduino/TOF8_Arduino_Library_Example/` | Arduino Uno／Nano | Arduino IDE | 通用 I2C 版（位址 `0x29`），用同資料夾的 `MBC_TOF8.h` 讀 8 顆距離（mm）與最近／最遠方位 |
| `arduino/TOF8_Arduino_Wire_Example/` | Arduino Uno／Nano | Arduino IDE | 同上，直接用 `Wire.h` 讀暫存器 `0x21`、`0x11`、`0x24` |
| `tof8_arduino_examples.zip` | Arduino Uno／Nano | Arduino IDE | 兩個 Arduino 範例打包 |

**使用方式（Pybricks）**

1. 用 [Pybricks Code](https://code.pybricks.com) 開啟範例；請先確認主機韌體版本，選對應資料夾的檔案（3.6.1 與 4.0.0 的函式庫不能混用）。
2. 使用函式庫的範例：把同資料夾的函式庫檔（`MBC_laser8_Lib.py` 或 `MBC_laser8_obj_Lib.py`）也匯入同一個 Pybricks 專案，**檔名不要改**。
3. 依實際接線修改連接埠（函式庫的 port 參數：1 ~ 6 = A ~ F）。4.0.0 版 `MBC_LASER8(port, True)` 為多工模式，每個方法都要加 `await`。
4. 開機後約 2 ~ 4 秒完成 8 顆初始化，期間讀到的值可能是 120。

**使用方式（Arduino）**

1. 只適用**通用 I2C 版**硬體（SPIKE／EV3 版接頭不同、不能互換）。接線：A4 → SDA、A5 → SCL、5V → VCC、GND → GND。
2. 用 Arduino IDE 開啟資料夾內的 `.ino`，選 Arduino Uno（或 Nano）上傳，序列埠監控視窗設 115200 bps。
3. 距離單位 mm：`1200` = 該方向沒有目標、`65535`（`0xFFFF`）= 該顆故障。

## English

Sensor 1 = far left (-90°) … sensor 8 = far right (+90°); sensors 4 and 5 both face straight ahead.

| File | Hub / Board | Firmware / Software | Description |
| :--- | :--- | :--- | :--- |
| `pybricks/For firmware 3.6.1/tof8_block_native_v361.py` | SPIKE Prime | Pybricks 3.6.1 | No library. Reads through `ColorSensor`: `reflection()` = nearest target direction (-8 – +8), `ambient()` = nearest distance (cm). Sensor on Port C |
| `pybricks/For firmware 3.6.1/tof8_block_with_lib_v361.py` | SPIKE Prime | Pybricks 3.6.1 | Block version using the `MBC_laser8_Lib` library to read all 8 distances. Sensor on Port C |
| `pybricks/For firmware 3.6.1/MBC_laser8_Lib.py` | SPIKE Prime | Pybricks 3.6.1 | Library (function style): 8 distances (0 – 120 cm), nearest direction (0 – 16, 8 = front) and distance, init status |
| `pybricks/For firmware 4.0.0/tof8_block_native_v400.py` | SPIKE Prime | Pybricks 4.0.0 | Same as the 3.6.1 no-library version. Sensor on Port B |
| `pybricks/For firmware 4.0.0/tof8_block_with_lib_v400.py` | SPIKE Prime | Pybricks 4.0.0 | Block version using the `MBC_LASER8` library object in multitask mode. Sensor on Port B |
| `pybricks/For firmware 4.0.0/MBC_laser8_obj_Lib.py` | SPIKE Prime | Pybricks 4.0.0 | Library (object style): 8 distances, nearest / farthest direction (-8 – +8, 0 = front) and distance, init and fault status |
| `tof8_pybricks_spike_v3.6.1.zip`, `tof8_pybricks_spike_v4.0.0.zip` | SPIKE Prime | 3.6.1 / 4.0.0 | All files of each version in one archive |
| `arduino/TOF8_Arduino_Library_Example/` | Arduino Uno / Nano | Arduino IDE | Universal I2C version (address `0x29`). Uses `MBC_TOF8.h` in the same folder to read 8 distances (mm) and nearest / farthest direction |
| `arduino/TOF8_Arduino_Wire_Example/` | Arduino Uno / Nano | Arduino IDE | Same, reading registers `0x21`, `0x11` and `0x24` directly with `Wire.h` |
| `tof8_arduino_examples.zip` | Arduino Uno / Nano | Arduino IDE | Both Arduino examples in one archive |

**How to use (Pybricks)**

1. Open the example in [Pybricks Code](https://code.pybricks.com). Check your hub firmware version first and use the matching folder (the 3.6.1 and 4.0.0 libraries are not interchangeable).
2. For examples that use a library, import the library file from the same folder (`MBC_laser8_Lib.py` or `MBC_laser8_obj_Lib.py`) into the same Pybricks project **without renaming it**.
3. Change the port to match your wiring (library port argument: 1 – 6 = A – F). In 4.0.0, `MBC_LASER8(port, True)` is multitask mode: put `await` before every method call.
4. The 8 sensors take about 2 – 4 s to initialize after power-up; values may read 120 during that time.

**How to use (Arduino)**

1. For the **Universal I2C version** hardware only (SPIKE / EV3 versions have different connectors and are not interchangeable). Wiring: A4 → SDA, A5 → SCL, 5V → VCC, GND → GND.
2. Open the `.ino` in the folder with the Arduino IDE, select Arduino Uno (or Nano), upload, and set the Serial Monitor to 115200 bps.
3. Distances are in mm: `1200` = no target in that direction, `65535` (`0xFFFF`) = that sensor is faulty.
