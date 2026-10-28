<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 📦 下載專區

MBC 全系列的函式庫、積木擴充、範例程式與型錄，集中在這一頁。先看清楚**版本與搭配的韌體**：2026-10 起出貨的產品是協定 v2 韌體，標「舊版」的檔案只搭配 2026-10 以前的韌體。

> [!TIP]
> 不確定手上感應器是哪一版？接上 Arduino 跑各產品的 `01_hello` 範例，或用 mBlock／micro:bit 的「協議版本」積木：回 **2** 就是新韌體。

---

## 📚 函式庫與擴充（打包下載）

<!-- gen:downloads-table -->
| 檔案 | 內容 | 版本 | 搭配韌體 | 大小 | 日期 |
| :--- | :--- | :---: | :--- | :---: | :---: |
| [MBC_EXP6_Pybricks_Lib_v2.zip](/downloads/MBC_EXP6_Pybricks_Lib_v2.zip) | SPIKE 6 路擴充器 EXP6：Pybricks 函式庫 v2＋範例（`MBC_exp6_obj_Lib_v2.py`、`Exp6_objMode_Demo.py`、指令說明書 PDF、`examples/` 11 支） | 2.0 | EXP6 v2 | 1.5 MB | 2026-10-25 |
| [MBC_EXP6_Official_App_Lib_v2.zip](/downloads/MBC_EXP6_Official_App_Lib_v2.zip) | SPIKE 6 路擴充器 EXP6：官方 SPIKE App 函式庫 v2＋範例（`MBC_exp6_SPIKE_App_Lib.py`、`SPIKE_Education/`、`Robot_Inventor/` 安裝／解除安裝腳本、`examples/` 9 支） | 2.0 | EXP6 v2 | 1.2 MB | 2026-10-21 |
| [MBC_EXP6_Pybricks_Lib.zip](/downloads/MBC_EXP6_Pybricks_Lib.zip) <span style="display:inline-block;background:#666;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">舊版</span> | EXP6 Pybricks 函式庫（舊版，v1 韌體）— 搭配 2026-10 以前韌體 | 1.x | EXP6 v1 | 1.5 MB | 2026-10-07 |
| [MBC_EXP6_Official_App_Lib.zip](/downloads/MBC_EXP6_Official_App_Lib.zip) <span style="display:inline-block;background:#666;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">舊版</span> | EXP6 官方 SPIKE App 函式庫（舊版，v1 韌體）— 搭配 2026-10 以前韌體 | 1.x | EXP6 v1 | 1.1 MB | 2026-10-21 |
| [MBC_Servo6_Pybricks_Lib.zip](/downloads/MBC_Servo6_Pybricks_Lib.zip) <span style="display:inline-block;background:#666;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">舊版</span> | SPIKE 6 路舵機擴充器 Servo6：Pybricks 函式庫 — 搭配 2026-10 以前韌體 | 1.0 | Servo6 | 8.7 KB | 2026-10-21 |
| [MBC_Servo6_Official_App_Lib.zip](/downloads/MBC_Servo6_Official_App_Lib.zip) <span style="display:inline-block;background:#666;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">舊版</span> | SPIKE 6 路舵機擴充器 Servo6：官方 SPIKE App 函式庫 — 搭配 2026-10 以前韌體 | 1.0 | Servo6 | 5.3 KB | 2026-10-21 |
| [line8_pybricks_spike_v4.x.zip](/examples/line8/line8_pybricks_spike_v4.x.zip) | 循行者 8 路：SPIKE Pybricks 函式庫與範例（`MBC_sensors.py`、`MBC_line8_obj_Lib.py`＋積木／Python 各 2 支） | 4.x | Pybricks 4.1 | 31 KB | 2026-10-25 |
| [MBC_TOF8_Pybricks_Lib.zip](/downloads/MBC_TOF8_Pybricks_Lib.zip) | 測距者 8 路 TOF8：SPIKE Pybricks 函式庫（`MBC_sensors.py`、`MBC_laser8_obj_Lib.py`） | 1.0 | Pybricks 4.1 | 21 KB | 2026-10-25 |
| [MBC_Pro_Sensors_mBlock5_v1.0.0.mext](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext) <span style="display:inline-block;background:#666;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">舊版</span> | mBlock 5 擴展 v1.0.0（舊版，15 顆積木）— 搭配 2026-10 以前韌體 | 1.0.0 | mBlock v1（2026-10 前） | 1.9 MB | 2026-10-21 |
<!-- /gen -->

> 這張表由 `_build/pack-downloads.mjs` 的清單產生（`_build/generated/downloads.json`），之後重新打包會自動覆寫。

---

## 🤖 mBlock 5（MakeBlock mBot 等）

| 檔案 | 狀態 | 說明 |
| :--- | :--- | :--- |
| [MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext) | **開發中：自行匯入（進階）** | v2.1.0＝v2.0.0 全部積木＋「神攝手通用版」分類 22 顆（搭配開發中的 [神攝手](/sensors/unicam/mblock/index.md)，韌體 v1 尚未上機驗證）。匯入方式同下一列 |
| [MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext) | **開發中：自行匯入（進階）** | v2.0.0，95 顆積木（循行者、測距者、掌控者、IMU、神攝手、MBC 共用），搭配協定 v2 韌體。這是「匯入檔」：到 [ext.mblock.cc](https://ext.mblock.cc) 登入 → 我的擴展 → 匯入資料 → 上傳 → 下載正式 `.mext`。正式上架後在 mBlock 擴展中心搜「MBC」即可，不用自己匯入 |
| [MBC_Pro_Sensors_mBlock5_v1.0.0.mext](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext) <span style="display:inline-block;background:#666;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">舊版</span> | 舊版 | v1.0.0，15 顆積木，搭配 2026-10 以前韌體；可直接拖進 mBlock |

安裝步驟與積木說明：[MakeBlock（mBot）積木擴展](/makeblock.md)。

## 🟠 micro:bit（MakeCode 擴充）

不用下載檔案：MakeCode → 擴充 → 貼 `https://github.com/contactmbcrobot/pxt-mbc-sensors`（v2.0.5，83 顆積木，MIT）。接線、硬體關卡與積木清單：[micro:bit 總覽](/microbit.md)。

## 🟢 MATRIX Mini R4

<!-- hide:hideMatrixBigChannel -->
| 檔案 | 說明 |
| :--- | :--- |
| [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip) | LauMo 大通道函式庫 v2（`begin()` 等待改 5 ms，支援 ESP32 產品）。解壓後覆蓋 MATRIXblock 安裝目錄的 `arduino\libraries\MatrixMiniR4\src\Modules\Sensors\MiniR4ColorSensorExt.cpp` 與 `.h`；要回原廠 0x22 用法就把原檔放回去 |

用法 A（原廠 0x22 積木）不用下載任何東西。兩種用法的對照表與每個產品的 byte 表：[MATRIX 總覽](/matrix.md)。
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
MATRIX 用原廠顏色感應器積木（0x22）讀，不用下載任何東西。每個產品的對照表：[MATRIX 總覽](/matrix.md)。
<!-- /show -->

## ∿ Arduino 範例（.zip，每款 5~6 支 .ino）

| 產品 | 中文註解版 | English |
| :--- | :--- | :--- |
| 循行者 8 路 | [line8_arduino_examples.zip](/examples/line8/arduino/line8_arduino_examples.zip) | [line8_arduino_examples_en.zip](/examples/line8/arduino-en/line8_arduino_examples_en.zip) |
| 循行者 16 路 | [line16_arduino_examples.zip](/examples/line16/arduino/line16_arduino_examples.zip) | [line16_arduino_examples_en.zip](/examples/line16/arduino-en/line16_arduino_examples_en.zip) |
| 測距者 2 路 | [tof2_arduino_examples.zip](/examples/tof2/arduino/tof2_arduino_examples.zip) | [tof2_arduino_examples_en.zip](/examples/tof2/arduino-en/tof2_arduino_examples_en.zip) |
| 測距者 8 路 | [tof8_arduino_examples.zip](/examples/tof8/arduino/tof8_arduino_examples.zip) | [tof8_arduino_examples_en.zip](/examples/tof8/arduino-en/tof8_arduino_examples_en.zip) |
| 掌控者 PS2 | [ps2_arduino_examples.zip](/examples/ps2/arduino/ps2_arduino_examples.zip) | [ps2_arduino_examples_en.zip](/examples/ps2/arduino-en/ps2_arduino_examples_en.zip) |
| 掌控者 PS4／PS5 | [ps4_arduino_examples.zip](/examples/ps4/arduino/ps4_arduino_examples.zip) | [ps4_arduino_examples_en.zip](/examples/ps4/arduino-en/ps4_arduino_examples_en.zip) |
| IMU 陀螺儀 | [imu_arduino_examples.zip](/examples/imu/arduino/imu_arduino_examples.zip) | [imu_arduino_examples_en.zip](/examples/imu/arduino-en/imu_arduino_examples_en.zip) |
| 神攝手（I2C，韌體 v1 尚未上機驗證） | [unicam_arduino_examples.zip](/examples/unicam/arduino/unicam_arduino_examples.zip) | [unicam_arduino_examples_en.zip](/examples/unicam/arduino-en/unicam_arduino_examples_en.zip) |
| SPIKE 6 路擴充器 EXP6（UART） | [exp6_arduino_examples.zip](/examples/exp6/arduino/exp6_arduino_examples.zip) | [exp6_arduino_examples_en.zip](/examples/exp6/arduino-en/exp6_arduino_examples_en.zip) |
| 通用 I2C 小工具（掃位址、讀版本） | [i2c_arduino_examples.zip](/examples/i2c/arduino/i2c_arduino_examples.zip) | — |

每支範例的逐行說明在各產品的「Arduino 範例」頁，從 [🔰 第一次用 Arduino](/sensors/arduino-start.md) 開始。[神攝手舊版（停止支援）](/sensors/esp32cam/index.md)的範例與函式庫集中在舊版總覽頁。

## 🧱 SPIKE／EV3 Pybricks 範例（.zip）

目前版本是 **Pybricks 4.1**（code.pybricks.com 正式版 v4.1.1；MBC SPIKE 積木網站燒的 v4.1.0 也可以）。每一包都附 `MBC_sensors.py`：**一個檔適用全部 MBC 感應器**（SPIKE、EV3 共用），新專案只要匯入它，寫 `from MBC_sensors import MBC_LINE8`（類別：`MBC_LINE8`、`MBC_LINE16`、`MBC_LASER2`、`MBC_LASER8`、`MBC_GAMEPAD`、`MBC_CAM_SCHOOL`、`MBC_HUE12`）。

> [!IMPORTANT]
> 包裡的舊檔名函式庫（例如 `MBC_line8_obj_Lib.py`）是給**舊專案直接換檔**用的：舊專案只換掉原本那一個檔，程式不用改。舊檔名每個都是整份共用檔，一個程式匯入 3 個以上會超過 SPIKE 程式槽約 51 KB 的上限；要混用多款感應器，請改成只匯入 `MBC_sensors.py`。

| 產品 | SPIKE Pybricks 4.1 | EV3 Pybricks |
| :--- | :--- | :--- |
| 循行者 8 路 | [line8_pybricks_spike_v4.x.zip](/examples/line8/line8_pybricks_spike_v4.x.zip) | [line8_pybricks_ev3.zip](/examples/line8/line8_pybricks_ev3.zip) |
| 循行者 16 路 | [line16_pybricks_spike_v4.x.zip](/examples/line16/line16_pybricks_spike_v4.x.zip) | [line16_pybricks_ev3.zip](/examples/line16/line16_pybricks_ev3.zip) |
| 測距者 2 路 | [tof2_pybricks_spike_v4.x.zip](/examples/tof2/tof2_pybricks_spike_v4.x.zip) | — |
| 測距者 8 路 | [tof8_pybricks_spike_v4.x.zip](/examples/tof8/tof8_pybricks_spike_v4.x.zip) | — |
| 掌控者 PS2 | [ps2_pybricks_spike_v4.x.zip](/examples/ps2/ps2_pybricks_spike_v4.x.zip) | — |
| 掌控者 PS4／PS5 | [ps4_pybricks_spike_v4.x.zip](/examples/ps4/ps4_pybricks_spike_v4.x.zip) | — |

### 舊版（不再更新）

Pybricks **3.6.1** 版的函式庫與範例：給還沒升級到 Pybricks 4 的主機；2026 新款 SPIKE 不能用。這些檔案不會再更新，能升級的話請改用上面的 4.1 版。

| 產品 | SPIKE Pybricks 3.6.1 |
| :--- | :--- |
| 循行者 8 路 | [line8_pybricks_spike_v3.6.1.zip](/examples/line8/line8_pybricks_spike_v3.6.1.zip) |
| 循行者 16 路 | [line16_pybricks_spike_v3.6.1.zip](/examples/line16/line16_pybricks_spike_v3.6.1.zip) |
| 測距者 8 路 | [tof8_pybricks_spike_v3.6.1.zip](/examples/tof8/tof8_pybricks_spike_v3.6.1.zip) |
| 掌控者 PS2 | [ps2_pybricks_spike_v3.6.1.zip](/examples/ps2/ps2_pybricks_spike_v3.6.1.zip) |
| 掌控者 PS4／PS5 | [ps4_pybricks_spike_v3.6.1.zip](/examples/ps4/ps4_pybricks_spike_v3.6.1.zip) |

### 新手範例（不另外打包）

SPIKE 官方 App（.llsp3）、MBC 積木網站（.abp）與 EV3 官方軟體（.ev3／.lmsp／clev3r .bp）的新手範例**不另外打包**，每個範例在頁面上逐一下載並附截圖：

| 產品 | SPIKE 新手範例 | EV3 新手範例 |
| :--- | :--- | :--- |
| 循行者 8 路 | [頁面](/sensors/line8/spike-examples.md) | [頁面](/sensors/line8/ev3-examples.md) |
| 循行者 16 路 | [頁面](/sensors/line16/spike-examples.md) | [頁面](/sensors/line16/ev3-examples.md) |
| 測距者 2 路 | [頁面](/sensors/tof2/spike-examples.md) | [頁面](/sensors/tof2/ev3-examples.md) |
| 測距者 8 路 | [頁面](/sensors/tof8/spike-examples.md) | [頁面](/sensors/tof8/ev3-examples.md) |
| 掌控者 PS2 | [頁面](/sensors/ps2/spike-examples.md) | [頁面](/sensors/ps2/ev3-examples.md) |
| 掌控者 PS4／PS5 | [頁面](/sensors/ps4/spike-examples.md) | [頁面](/sensors/ps4/ev3-examples.md) |
| IMU 陀螺儀 | — | [頁面](/sensors/imu/ev3-examples.md) |
| 神攝手 | [頁面](/sensors/unicam/spike-examples.md) | [頁面](/sensors/unicam/ev3-examples.md) |
| SPIKE 6 路擴充器 EXP6 | [頁面](/sensors/exp6/spike-examples.md) | — |

## 📄 型錄與說明書

| 檔案 | 說明 |
| :--- | :--- |
| [MBC-Pro-Sensors-Catalog-2026-EN.pdf](/downloads/MBC-Pro-Sensors-Catalog-2026-EN.pdf) | 2026 產品型錄（英文，2.1 MB） |
| [PS2_Controller_Command_Guide_ZH.pdf](/examples/ps2/PS2_Controller_Command_Guide_ZH.pdf) | 掌控者 PS2 指令說明（中文） |
| [PS2_Controller_Command_Guide_EN_Taiwan_only.pdf](/examples/ps2/PS2_Controller_Command_Guide_EN_Taiwan_only.pdf) | 掌控者 PS2 指令說明（英文；PS2 只在台灣販售） |

> 找不到你要的檔案，或想要韌體更新？[聯絡我們](/contact.md)。
