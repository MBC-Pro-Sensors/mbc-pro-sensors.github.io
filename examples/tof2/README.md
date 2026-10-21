# 測距者 2 路（TOF2）範例程式 / TOF2 Example Programs

## 繁體中文

距離單位為公分（0 ~ 120），讀到 120 代表該側沒有目標。第 1 路 = 左雷射、第 2 路 = 右雷射。

| 檔案 | 主機 | 韌體／軟體 | 說明 |
| :--- | :--- | :--- | :--- |
| `pybricks/For firmware 3.6.1/tof2_python_native_v361.py` | SPIKE Prime | Pybricks 3.6.1 | 不用函式庫，以 `ColorSensor` 讀取：`reflection()` = 左距離、`ambient()` = 右距離 |
| `pybricks/For firmware 4.0.0/tof2_python_native_v400.py` | SPIKE Prime | Pybricks 4.0.0 | 同上（文字程式） |
| `pybricks/For firmware 4.0.0/tof2_block_native_v400.py` | SPIKE Prime | Pybricks 4.0.0 | 同上，積木版（可在 Pybricks 積木編輯器開啟） |
| `pybricks/For firmware 4.0.0/tof2_block_with_lib_v400.py` | SPIKE Prime | Pybricks 4.0.0 | 積木版，使用函式庫物件 `MBC_LASER2`，多工模式讀取左右距離 |
| `pybricks/For firmware 4.0.0/MBC_laser2_obj_Lib.py` | SPIKE Prime | Pybricks 4.0.0 | 函式庫：左／右／平均／較近距離、較近側、左右差、通道狀態 |
| `tof2_pybricks_spike_v4.0.0.zip` | SPIKE Prime | Pybricks 4.0.0 | 上面 4.0.0 版全部檔案打包 |

**使用方式（Pybricks）**

1. 用 [Pybricks Code](https://code.pybricks.com) 開啟範例；請先確認主機韌體版本，選對應資料夾的檔案。
2. 使用函式庫的範例：把 `MBC_laser2_obj_Lib.py` 也匯入同一個 Pybricks 專案，**檔名不要改**（範例用 `from MBC_laser2_obj_Lib import MBC_LASER2` 匯入）。
3. 範例預設感應器接在 **Port B**，請依實際接線修改（函式庫的 port 參數：1 ~ 6 = A ~ F）。
4. `MBC_LASER2(port, True)` 為多工模式，每個方法都要加 `await`；`MBC_LASER2(port, False)` 為一般模式，直接呼叫。

## English

Distances are in centimeters (0 – 120); 120 means no target on that side. Channel 1 = left laser, channel 2 = right laser.

| File | Hub | Firmware / Software | Description |
| :--- | :--- | :--- | :--- |
| `pybricks/For firmware 3.6.1/tof2_python_native_v361.py` | SPIKE Prime | Pybricks 3.6.1 | No library. Reads through `ColorSensor`: `reflection()` = left distance, `ambient()` = right distance |
| `pybricks/For firmware 4.0.0/tof2_python_native_v400.py` | SPIKE Prime | Pybricks 4.0.0 | Same as above (text program) |
| `pybricks/For firmware 4.0.0/tof2_block_native_v400.py` | SPIKE Prime | Pybricks 4.0.0 | Same as above, block version (opens in the Pybricks block editor) |
| `pybricks/For firmware 4.0.0/tof2_block_with_lib_v400.py` | SPIKE Prime | Pybricks 4.0.0 | Block version using the `MBC_LASER2` library object in multitask mode |
| `pybricks/For firmware 4.0.0/MBC_laser2_obj_Lib.py` | SPIKE Prime | Pybricks 4.0.0 | Library: left / right / average / nearest distance, nearer side, left-right difference, channel status |
| `tof2_pybricks_spike_v4.0.0.zip` | SPIKE Prime | Pybricks 4.0.0 | All 4.0.0 files above in one archive |

**How to use (Pybricks)**

1. Open the example in [Pybricks Code](https://code.pybricks.com). Check your hub firmware version first and use the matching folder.
2. For examples that use the library, import `MBC_laser2_obj_Lib.py` into the same Pybricks project **without renaming it** (the example imports it with `from MBC_laser2_obj_Lib import MBC_LASER2`).
3. The examples assume the sensor is on **Port B**. Change it to match your wiring (library port argument: 1 – 6 = A – F).
4. `MBC_LASER2(port, True)` is multitask mode: put `await` before every method call. `MBC_LASER2(port, False)` is normal mode: call the methods directly.
