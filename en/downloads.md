<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 📦 Downloads

Every MBC library, block extension, example pack and catalog on one page. Check the **version and the firmware it matches** first: products shipped from October 2026 run protocol v2 firmware, and files marked "legacy" only work with firmware shipped before 2026-10.

> [!TIP]
> Not sure which firmware your sensor has? Run the product's `01_hello` Arduino example, or use the "protocol version" block in mBlock / micro:bit: **2** means new firmware.

---

## 📚 Libraries and extensions (packed downloads)

<!-- gen:downloads-table -->
| File | Contents | Version | Firmware | Size | Date |
| :--- | :--- | :---: | :--- | :---: | :---: |
| [MBC_EXP6_Pybricks_Lib_v2.zip](/downloads/MBC_EXP6_Pybricks_Lib_v2.zip) | EXP6 expander: Pybricks library v2 + examples (`MBC_exp6_obj_Lib_v2.py`, `Exp6_objMode_Demo.py`, command manual PDF, 11 programs in `examples/`) | 2.0 | EXP6 v2 | 1.5 MB | 2026-10-21 |
| [MBC_EXP6_Official_App_Lib_v2.zip](/downloads/MBC_EXP6_Official_App_Lib_v2.zip) | EXP6 expander: official SPIKE App library v2 + examples (`MBC_exp6_SPIKE_App_Lib.py`, install / uninstall scripts for `SPIKE_Education/` and `Robot_Inventor/`, 9 programs in `examples/`) | 2.0 | EXP6 v2 | 1.2 MB | 2026-10-21 |
| [MBC_EXP6_Pybricks_Lib.zip](/downloads/MBC_EXP6_Pybricks_Lib.zip) <span style="display:inline-block;background:#777;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">legacy</span> | EXP6 Pybricks library (legacy, v1 firmware) — for firmware shipped before 2026-10 | 1.x | EXP6 v1 | 1.5 MB | 2026-10-07 |
| [MBC_EXP6_Official_App_Lib.zip](/downloads/MBC_EXP6_Official_App_Lib.zip) <span style="display:inline-block;background:#777;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">legacy</span> | EXP6 official SPIKE App library (legacy, v1 firmware) — for firmware shipped before 2026-10 | 1.x | EXP6 v1 | 1.1 MB | 2026-10-21 |
| [MBC_Servo6_Pybricks_Lib.zip](/downloads/MBC_Servo6_Pybricks_Lib.zip) <span style="display:inline-block;background:#777;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">legacy</span> | Servo6 expander: Pybricks library — for firmware shipped before 2026-10 | 1.0 | Servo6 | 8.7 KB | 2026-10-21 |
| [MBC_Servo6_Official_App_Lib.zip](/downloads/MBC_Servo6_Official_App_Lib.zip) <span style="display:inline-block;background:#777;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">legacy</span> | Servo6 expander: official SPIKE App library — for firmware shipped before 2026-10 | 1.0 | Servo6 | 5.3 KB | 2026-10-21 |
| [line8_pybricks_spike_v4.0.0.zip](/examples/line8/line8_pybricks_spike_v4.0.0.zip) | Line Follower 8-ch: SPIKE Pybricks library + examples (`MBC_line8_obj_Lib_v400.py` plus 2 block-style and 2 Python examples) | 4.0.0 | Pybricks 4.0.0 | 14 KB | 2026-10-21 |
| [MBC_TOF8_Pybricks_Lib.zip](/downloads/MBC_TOF8_Pybricks_Lib.zip) | Ranger 8-ch TOF8: SPIKE Pybricks library (`MBC_laser8_Lib.py`, `MBC_laser8_obj_Lib.py`) | 1.0 | Pybricks | 8.6 KB | 2026-10-21 |
| [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip) | MATRIX Mini R4: LauMo big-channel library (`MiniR4ColorSensorExt.cpp` / `.h`, replaces the two stock files) | 2.0 | MATRIX big channel | 4.1 KB | 2026-10-21 |
| [MBC_Pro_Sensors_mBlock5_v1.0.0.mext](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext) <span style="display:inline-block;background:#777;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">legacy</span> | mBlock 5 extension v1.0.0 (legacy, 15 blocks) — for firmware shipped before 2026-10 | 1.0.0 | mBlock v1 (before 2026-10) | 1.9 MB | 2026-10-21 |
<!-- /gen -->

> This table is generated from the `_build/pack-downloads.mjs` list (`_build/generated/downloads.json`) and is overwritten whenever the packs are rebuilt.

---

## 🤖 mBlock 5 (Makeblock mBot etc.)

| File | Status | Notes |
| :--- | :--- | :--- |
| [MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext) | **Publishing in progress: import it yourself (advanced)** | v2.0.0, 95 blocks (Pathfinder, Ranger, Controller, IMU, Vision, MBC common), for protocol v2 firmware. This is an *import file*: sign in to [ext.mblock.cc](https://ext.mblock.cc) → My extensions → Import data → upload → Download the final `.mext`. Once listed, just search "MBC" in the mBlock Extension Center |
| [MBC_Pro_Sensors_mBlock5_v1.0.0.mext](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext) <span style="display:inline-block;background:#777;color:#fff;font-size:0.75em;padding:1px 8px;border-radius:10px;vertical-align:middle;">legacy</span> | Legacy | v1.0.0, 15 blocks, for firmware shipped before 2026-10; drop straight into mBlock |

Install steps and block reference: [MakeBlock (mBot) block extension](/en/makeblock.md).

## 🟠 micro:bit (MakeCode extension)

Nothing to download: MakeCode → Extensions → paste `https://github.com/contactmbcrobot/pxt-mbc-sensors` (v2.0.1, 83 blocks, MIT). Wiring, hardware checklist and block list: [micro:bit overview](/en/microbit.md).

## 🟢 MATRIX Mini R4

| File | Notes |
| :--- | :--- |
| [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip) | LauMo big-channel library v2 (`begin()` wait raised to 5 ms for ESP32 products). Unzip and overwrite `arduino\libraries\MatrixMiniR4\src\Modules\Sensors\MiniR4ColorSensorExt.cpp` and `.h` in the MATRIXblock install folder; put the stock files back to return to the 0x22 blocks |

Method A (stock 0x22 blocks) needs no download. Both methods and every product's byte table: [MATRIX overview](/en/matrix.md).

## ∿ Arduino examples (.zip, 5~6 sketches each)

| Product | Chinese comments | English comments |
| :--- | :--- | :--- |
| Pathfinder 8-Way | [line8_arduino_examples.zip](/examples/line8/arduino/line8_arduino_examples.zip) | [line8_arduino_examples_en.zip](/examples/line8/arduino-en/line8_arduino_examples_en.zip) |
| Pathfinder 16-Way | [line16_arduino_examples.zip](/examples/line16/arduino/line16_arduino_examples.zip) | [line16_arduino_examples_en.zip](/examples/line16/arduino-en/line16_arduino_examples_en.zip) |
| Ranger 2-Way | [tof2_arduino_examples.zip](/examples/tof2/arduino/tof2_arduino_examples.zip) | [tof2_arduino_examples_en.zip](/examples/tof2/arduino-en/tof2_arduino_examples_en.zip) |
| Ranger 8-Way | [tof8_arduino_examples.zip](/examples/tof8/arduino/tof8_arduino_examples.zip) | [tof8_arduino_examples_en.zip](/examples/tof8/arduino-en/tof8_arduino_examples_en.zip) |
| Controller PS2 | [ps2_arduino_examples.zip](/examples/ps2/arduino/ps2_arduino_examples.zip) | [ps2_arduino_examples_en.zip](/examples/ps2/arduino-en/ps2_arduino_examples_en.zip) |
| Controller PS4 / PS5 | [ps4_arduino_examples.zip](/examples/ps4/arduino/ps4_arduino_examples.zip) | [ps4_arduino_examples_en.zip](/examples/ps4/arduino-en/ps4_arduino_examples_en.zip) |
| IMU gyro | [imu_arduino_examples.zip](/examples/imu/arduino/imu_arduino_examples.zip) | [imu_arduino_examples_en.zip](/examples/imu/arduino-en/imu_arduino_examples_en.zip) |
| Sharpshooter ESP32CAM (I2C, bench test pending) | [esp32cam_arduino_examples.zip](/examples/esp32cam/arduino/esp32cam_arduino_examples.zip) | [esp32cam_arduino_examples_en.zip](/examples/esp32cam/arduino-en/esp32cam_arduino_examples_en.zip) |
| SPIKE 6-Way Expander EXP6 (UART) | [exp6_arduino_examples.zip](/examples/exp6/arduino/exp6_arduino_examples.zip) | [exp6_arduino_examples_en.zip](/examples/exp6/arduino-en/exp6_arduino_examples_en.zip) |
| Universal I2C tools (scan, read version) | [i2c_arduino_examples.zip](/examples/i2c/arduino/i2c_arduino_examples.zip) | — |

Line-by-line explanations are on each product's "Arduino Examples" page; start at [First time with Arduino](/en/sensors/arduino-start.md).

## 🧱 SPIKE / EV3 Pybricks examples (.zip)

| Product | SPIKE Pybricks 4.0.0 | SPIKE Pybricks 3.6.1 | EV3 Pybricks |
| :--- | :--- | :--- | :--- |
| Pathfinder 8-Way | [line8_pybricks_spike_v4.0.0.zip](/examples/line8/line8_pybricks_spike_v4.0.0.zip) | [line8_pybricks_spike_v3.6.1.zip](/examples/line8/line8_pybricks_spike_v3.6.1.zip) | [line8_pybricks_ev3.zip](/examples/line8/line8_pybricks_ev3.zip) |
| Pathfinder 16-Way | [line16_pybricks_spike_v4.0.0.zip](/examples/line16/line16_pybricks_spike_v4.0.0.zip) | [line16_pybricks_spike_v3.6.1.zip](/examples/line16/line16_pybricks_spike_v3.6.1.zip) | [line16_pybricks_ev3.zip](/examples/line16/line16_pybricks_ev3.zip) |
| Ranger 2-Way | [tof2_pybricks_spike_v4.0.0.zip](/examples/tof2/tof2_pybricks_spike_v4.0.0.zip) | — | — |
| Ranger 8-Way | [tof8_pybricks_spike_v4.0.0.zip](/examples/tof8/tof8_pybricks_spike_v4.0.0.zip) | [tof8_pybricks_spike_v3.6.1.zip](/examples/tof8/tof8_pybricks_spike_v3.6.1.zip) | — |
| Controller PS2 | — | [ps2_pybricks_spike_v3.6.1.zip](/examples/ps2/ps2_pybricks_spike_v3.6.1.zip) | — |
| Controller PS4 / PS5 | — | [ps4_pybricks_spike_v3.6.1.zip](/examples/ps4/ps4_pybricks_spike_v3.6.1.zip) | — |

The beginner examples for the official SPIKE App (.llsp3), the MBC blocks site (.abp) and the EV3 software (.ev3 / .lmsp / clev3r .bp) are **not packed**: each one is downloaded from its page, with a screenshot (SPIKE example pages are in Chinese; EV3 pages are in English):

| Product | SPIKE beginner examples | EV3 beginner examples |
| :--- | :--- | :--- |
| Pathfinder 8-Way | [page (zh)](/sensors/line8/spike-examples.md) | [page](/en/sensors/line8/ev3-examples.md) |
| Pathfinder 16-Way | [page (zh)](/sensors/line16/spike-examples.md) | [page](/en/sensors/line16/ev3-examples.md) |
| Ranger 2-Way | [page (zh)](/sensors/tof2/spike-examples.md) | [page](/en/sensors/tof2/ev3-examples.md) |
| Ranger 8-Way | [page (zh)](/sensors/tof8/spike-examples.md) | [page](/en/sensors/tof8/ev3-examples.md) |
| Controller PS2 | [page (zh)](/sensors/ps2/spike-examples.md) | [page](/en/sensors/ps2/ev3-examples.md) |
| Controller PS4 / PS5 | [page (zh)](/sensors/ps4/spike-examples.md) | [page](/en/sensors/ps4/ev3-examples.md) |
| IMU gyro | — | [page](/en/sensors/imu/ev3-examples.md) |
| Sharpshooter ESP32CAM | [school (zh)](/sensors/esp32cam/spike-examples.md) · [WRO (zh)](/sensors/esp32cam/spike-examples-wro.md) | [school](/en/sensors/esp32cam/ev3-examples.md) · [WRO](/en/sensors/esp32cam/ev3-examples-wro.md) |
| SPIKE 6-Way Expander EXP6 | [page (zh)](/sensors/exp6/spike-examples.md) | — |

## 📄 Catalog and manuals

| File | Notes |
| :--- | :--- |
| [MBC-Pro-Sensors-Catalog-2026-EN.pdf](/downloads/MBC-Pro-Sensors-Catalog-2026-EN.pdf) | 2026 product catalog (English, 2.1 MB) |
| [PS2_Controller_Command_Guide_EN_Taiwan_only.pdf](/examples/ps2/PS2_Controller_Command_Guide_EN_Taiwan_only.pdf) | Controller PS2 command guide (English; the PS2 is sold in Taiwan only) |
| [PS2_Controller_Command_Guide_ZH.pdf](/examples/ps2/PS2_Controller_Command_Guide_ZH.pdf) | Controller PS2 command guide (Chinese) |

> Missing a file, or need a firmware update? [Contact us](/en/contact.md).
