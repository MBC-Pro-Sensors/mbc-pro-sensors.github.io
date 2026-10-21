# 掌控者 2.4G 遙控接收器（PS2）範例程式

## 中文

> PS2 版只在台灣販售。

| 檔案 | 說明 |
| :--- | :--- |
| `pybricks/For firmware 3.6.1/ps2_block_native_v361.py` | Pybricks 積木檔：用「顏色感應器」積木讀按鍵與搖桿，印出 15 個值 |
| `pybricks/For firmware 3.6.1/ps2_python_native_v361.py` | 同上的 Python 文字版（`reflection()` 讀按鍵、`ambient()` 讀搖桿） |
| `pybricks/For firmware 3.6.1/ps2_python_mode9_v361.py` | Python 完整版：`PUPDevice.read(9)` 一次讀到全部 16 個按鍵與 4 軸搖桿 |
| `ps2_pybricks_spike_v3.6.1.zip` | 上面三個 Pybricks 檔打包 |
| `PS2_Controller_Command_Guide_ZH.pdf` | 指令說明（EV3 LabVIEW、EV3 Classroom、SPIKE App、Pybricks） |
| `PS2_Controller_Command_Guide_EN_Taiwan_only.pdf` | 指令說明英文版（產品只在台灣販售） |

**適用**：SPIKE Prime／Robot Inventor 主機，Pybricks 韌體 3.6.1（Pybricks Code 積木檔格式 1.3.2）。

**怎麼用**
1. 接收器插 SPIKE 的 C 孔（範例預設；改孔位就改 `Port.C`）。
2. 到 Pybricks Code 開啟 `.py` 檔，按執行，看終端機輸出。
3. 積木版／native 版只能讀 11 個鍵（方向鍵、▲●✕■、L1、R1、SELECT）；要讀全部按鍵請用 `mode9` 版。
4. 搖桿值：SPIKE／Pybricks 為 `1`~`9`（`5` = 中心）；EV3 為 `0`~`99`（`50` = 中心）。

## English

> **Taiwan only** — the PS2 version is sold in Taiwan only.

| File | Description |
| :--- | :--- |
| `pybricks/For firmware 3.6.1/ps2_block_native_v361.py` | Pybricks block file: reads buttons and sticks through the Color Sensor blocks and prints 15 values |
| `pybricks/For firmware 3.6.1/ps2_python_native_v361.py` | Python text version of the same program (`reflection()` = buttons, `ambient()` = sticks) |
| `pybricks/For firmware 3.6.1/ps2_python_mode9_v361.py` | Full Python version: `PUPDevice.read(9)` returns all 16 buttons and 4 stick axes |
| `ps2_pybricks_spike_v3.6.1.zip` | The three Pybricks files above, zipped |
| `PS2_Controller_Command_Guide_ZH.pdf` | Command guide in Chinese (EV3 LabVIEW, EV3 Classroom, SPIKE App, Pybricks) |
| `PS2_Controller_Command_Guide_EN_Taiwan_only.pdf` | Command guide in English (product sold in Taiwan only) |

**Works with**: SPIKE Prime / Robot Inventor hub, Pybricks firmware 3.6.1 (Pybricks Code block format 1.3.2).

**How to use**
1. Plug the receiver into hub port C (change `Port.C` if you use another port).
2. Open the `.py` file in Pybricks Code, run it and watch the terminal.
3. The block / native versions read 11 buttons (D-pad, ▲●✕■, L1, R1, SELECT). Use the `mode9` version to read every button.
4. Stick values: `1`-`9` on SPIKE / Pybricks (`5` = center); `0`-`99` on EV3 (`50` = center).
