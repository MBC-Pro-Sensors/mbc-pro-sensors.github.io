# 測距者 2 路（TOF2）範例程式 / TOF2 Example Programs

## 繁體中文

距離單位為公分（0 ~ 120），讀到 120 代表該側沒有目標。第 1 路 = 左雷射、第 2 路 = 右雷射。

| 檔案 | 主機 | 韌體／軟體 | 說明 |
| :--- | :--- | :--- | :--- |
| `pybricks/For firmware 3.6.1/tof2_python_native_v361.py` | SPIKE Prime | Pybricks 3.6.1（舊版，不再更新） | 不用函式庫，以 `ColorSensor` 讀取：`reflection()` = 左距離、`ambient()` = 右距離 |
| `pybricks/For firmware 4.x/tof2_python_native.py` | SPIKE Prime | Pybricks 4.1 | 同上（文字程式） |
| `pybricks/For firmware 4.x/tof2_block_native.py` | SPIKE Prime | Pybricks 4.1 | 同上，積木版（可在 Pybricks 積木編輯器開啟） |
| `pybricks/For firmware 4.x/tof2_block_with_lib.py` | SPIKE Prime | Pybricks 4.1 | 積木版，使用函式庫物件 `MBC_LASER2`，多工模式讀取左右距離 |
| `pybricks/For firmware 4.x/MBC_laser2_obj_Lib.py` | SPIKE Prime | Pybricks 4.1 | 函式庫：左／右／平均／較近距離、較近側、左右差、通道狀態 |
| `pybricks/For firmware 4.x/MBC_sensors.py` | SPIKE Prime／EV3 | Pybricks 4.1 | 共用函式庫：一個檔適用全部 MBC 感應器（`from MBC_sensors import MBC_LASER2`），新專案用這個 |
| `tof2_pybricks_spike_v4.x.zip` | SPIKE Prime | Pybricks 4.1 | 上面 4.x 版全部檔案打包（含 `MBC_sensors.py`） |

**使用方式（Pybricks）**

1. 用 [Pybricks Code](https://code.pybricks.com) 開啟範例；主機請用 Pybricks 韌體 v4.1，開 `For firmware 4.x` 資料夾的檔案（`For firmware 3.6.1` 是舊版，已不再更新）。
2. 使用函式庫的範例：把 `MBC_laser2_obj_Lib.py` 也匯入同一個 Pybricks 專案，**檔名不要改**（範例用 `from MBC_laser2_obj_Lib import MBC_LASER2` 匯入）。新專案可以改成只匯入 `MBC_sensors.py`（`from MBC_sensors import MBC_LASER2`）。
3. 範例預設感應器接在 **Port B**，請依實際接線修改（函式庫的 port 參數：1 ~ 6 = A ~ F）。
4. `MBC_LASER2(port, True)` 為多工模式，每個方法都要加 `await`；`MBC_LASER2(port, False)` 為一般模式，直接呼叫。

## English

Distances are in centimeters (0 – 120); 120 means no target on that side. Channel 1 = left laser, channel 2 = right laser.

| File | Hub | Firmware / Software | Description |
| :--- | :--- | :--- | :--- |
| `pybricks/For firmware 3.6.1/tof2_python_native_v361.py` | SPIKE Prime | Pybricks 3.6.1 (legacy, no longer updated) | No library. Reads through `ColorSensor`: `reflection()` = left distance, `ambient()` = right distance |
| `pybricks/For firmware 4.x/tof2_python_native.py` | SPIKE Prime | Pybricks 4.1 | Same as above (text program) |
| `pybricks/For firmware 4.x/tof2_block_native.py` | SPIKE Prime | Pybricks 4.1 | Same as above, block version (opens in the Pybricks block editor) |
| `pybricks/For firmware 4.x/tof2_block_with_lib.py` | SPIKE Prime | Pybricks 4.1 | Block version using the `MBC_LASER2` library object in multitask mode |
| `pybricks/For firmware 4.x/MBC_laser2_obj_Lib.py` | SPIKE Prime | Pybricks 4.1 | Library: left / right / average / nearest distance, nearer side, left-right difference, channel status |
| `pybricks/For firmware 4.x/MBC_sensors.py` | SPIKE Prime / EV3 | Pybricks 4.1 | Shared library: one file for every MBC sensor (`from MBC_sensors import MBC_LASER2`); use it for new projects |
| `tof2_pybricks_spike_v4.x.zip` | SPIKE Prime | Pybricks 4.1 | All 4.x files above in one archive (with `MBC_sensors.py`) |

**How to use (Pybricks)**

1. Open the example in [Pybricks Code](https://code.pybricks.com). Use Pybricks firmware v4.1 on the hub and the files in `For firmware 4.x` (`For firmware 3.6.1` is legacy and no longer updated).
2. For examples that use the library, import `MBC_laser2_obj_Lib.py` into the same Pybricks project **without renaming it** (the example imports it with `from MBC_laser2_obj_Lib import MBC_LASER2`). For a new project you can import only `MBC_sensors.py` instead (`from MBC_sensors import MBC_LASER2`).
3. The examples assume the sensor is on **Port B**. Change it to match your wiring (library port argument: 1 – 6 = A – F).
4. `MBC_LASER2(port, True)` is multitask mode: put `await` before every method call. `MBC_LASER2(port, False)` is normal mode: call the methods directly.
