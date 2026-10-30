# 掌控者 PS4/PS5 藍牙遙控接收器 範例程式

## 中文

| 檔案 | 說明 |
| :--- | :--- |
| `spike/ps4_demo_spike_prime_v3.4.3.llsp3` | SPIKE 教育版 App（SPIKE Prime）範例：解出 16 個按鍵與 4 軸搖桿變數，左搖桿 Y 軸控制 A、B 馬達（接收器 D 埠） |
| `spike/ps4_demo_robot_inventor_v10.5.0.lms` | MINDSTORMS Robot Inventor App 範例：同上，方向鍵上／下控制 A、B 馬達（接收器 D 埠） |
| `pybricks/MBC_gamepad_Lib.py` | Pybricks 4.1 函式庫（`from MBC_gamepad_Lib import MBC_GAMEPAD`），新手範例用這個 |
| `pybricks/MBC_sensors.py` | 共用函式庫：一個檔適用全部 MBC 感應器（`from MBC_sensors import MBC_GAMEPAD`），新專案用這個 |
| `spike_beginner/pybricks/ex01_button_number.py` … `ex06_connected_battery.py` | Pybricks 4.1 新手範例，說明見 [SPIKE 新手範例頁](/sensors/ps4/spike-examples.md) |
| `ps4_pybricks_spike_v4.x.zip` | 上面兩個函式庫＋6 個新手範例打包 |
| `pybricks/For firmware 3.6.1/ps4_block_native_v361.py` | Pybricks 積木檔：用「顏色感應器」積木讀按鍵與搖桿，印出 15 個值 |
| `pybricks/For firmware 3.6.1/ps4_python_native_v361.py` | 同上的 Python 文字版 |
| `pybricks/For firmware 3.6.1/ps4_python_mode9_v361.py` | Python 完整版：`PUPDevice.read(9)` 一次讀到全部 16 個按鍵與 4 軸搖桿 |
| `ps4_pybricks_spike_v3.6.1.zip` | 上面三個 Pybricks 3.6.1 檔打包（舊版，不再更新） |

**適用**
- SPIKE 教育版 App 3.4.3 以上（`.llsp3`）、Robot Inventor App 10.5.0 以上（`.lms`）。
- Pybricks 韌體 v4.1：`MBC_gamepad_Lib.py`、`MBC_sensors.py`、新手範例，SPIKE Prime／Robot Inventor 主機。`For firmware 3.6.1` 資料夾是舊版（Pybricks 3.6.1，積木檔格式 1.3.2），已不再更新。

**怎麼用**
1. 先完成手把配對（見產品頁「配對方式」）。
2. 官方 App 範例：接收器插 **D 孔**、馬達插 A、B 孔；Pybricks 範例：接收器插 **C 孔**。
3. 開啟檔案後執行。官方 App 範例的「PS4搖桿_資料更新」積木要放在迴圈開頭，每圈呼叫一次。
4. Pybricks 積木版／native 版只能讀 11 個鍵（方向鍵、▲●✕■、L1、R1、SHARE）；要讀全部按鍵請用 `mode9` 版。
5. 搖桿值：SPIKE／Pybricks 為 `1`~`9`（`5` = 中心）；EV3 為 `0`~`99`（`50` = 中心）。

## English

| File | Description |
| :--- | :--- |
| `spike/ps4_demo_spike_prime_v3.4.3.llsp3` | SPIKE (Education) App example for SPIKE Prime: decodes 16 buttons and 4 stick axes into variables; left stick Y drives motors A and B (receiver on port D) |
| `spike/ps4_demo_robot_inventor_v10.5.0.lms` | MINDSTORMS Robot Inventor App example: same decoding; D-pad up/down drives motors A and B (receiver on port D) |
| `pybricks/MBC_gamepad_Lib.py` | Pybricks 4.1 library (`from MBC_gamepad_Lib import MBC_GAMEPAD`), used by the beginner examples |
| `pybricks/MBC_sensors.py` | Shared library: one file for every MBC sensor (`from MBC_sensors import MBC_GAMEPAD`); use it for new projects |
| `spike_beginner/pybricks/ex01_button_number.py` … `ex06_connected_battery.py` | Pybricks 4.1 beginner examples, see the [SPIKE beginner examples page](/en/sensors/ps4/spike-examples.md) |
| `ps4_pybricks_spike_v4.x.zip` | The two libraries above plus the 6 beginner examples, zipped |
| `pybricks/For firmware 3.6.1/ps4_block_native_v361.py` | Pybricks block file: reads buttons and sticks through the Color Sensor blocks and prints 15 values |
| `pybricks/For firmware 3.6.1/ps4_python_native_v361.py` | Python text version of the same program |
| `pybricks/For firmware 3.6.1/ps4_python_mode9_v361.py` | Full Python version: `PUPDevice.read(9)` returns all 16 buttons and 4 stick axes |
| `ps4_pybricks_spike_v3.6.1.zip` | The three Pybricks 3.6.1 files above, zipped (legacy, no longer updated) |

**Works with**
- SPIKE App 3.4.3 or later (`.llsp3`), Robot Inventor App 10.5.0 or later (`.lms`).
- Pybricks firmware v4.1 on SPIKE Prime / Robot Inventor hubs: `MBC_gamepad_Lib.py`, `MBC_sensors.py`, beginner examples. The `For firmware 3.6.1` folder is legacy (Pybricks 3.6.1, block format 1.3.2) and no longer updated.

**How to use**
1. Pair the controller first (see "Pairing" on the product page).
2. Official App examples: receiver on **port D**, motors on A and B. Pybricks examples: receiver on **port C**.
3. Open the file and run it. In the official App examples, call the "data update" block once at the top of every loop.
4. The Pybricks block / native versions read 11 buttons (D-pad, ▲●✕■, L1, R1, SHARE). Use the `mode9` version to read every button.
5. Stick values: `1`-`9` on SPIKE / Pybricks (`5` = center); `0`-`99` on EV3 (`50` = center).
