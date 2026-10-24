<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Ranger 2-Way Laser Ranging (TOF2)

<!-- product-hero -->

<!-- platform-grid -->

---

> **Lightweight and agile: say goodbye to bulky ultrasonics. A micro-laser that accurately captures which way an object leaves.**

## 🚀 Product Overview

Traditional ultrasonic sensors are not only bulky, they also react slowly and are easily fooled by echoes (ghost readings). In high-speed robot applications they simply cannot keep up.

The Ranger 2-Way uses **laser time-of-flight (ToF)**: it measures at the speed of light, so it reacts far faster than ultrasonics. The module is tiny and fits almost anywhere on a robot. Best of all, its two laser beams let the robot tell whether an object **left to the left or to the right** — something a single ultrasonic sensor can never do.

## 🎯 Core Advantages

- ⚡ **Laser-speed reaction**: not limited by the speed of sound, detection is far faster than ultrasonics
- 🎯 **Direction judgment**: the dual-beam layout tells which side the object finally left the sensing range from
- 📦 **Extremely compact**: much smaller than an ultrasonic sensor, saving precious mounting space
- 🔌 **Plug and play**: plug straight into SPIKE / EV3; no extra power supply or setup
- 🔼 **Seamless upgrade path**: shares the dual 0° layout with the TOF8, so you can upgrade to the TOF8 later without rewriting your core logic

## 📊 Which Model Do I Need?

| Scenario | Recommended model |
| :--- | :--- |
| Simple collision avoidance, distance detection, left / right judgment | ✅ **TOF2 (this model)** |
| Sumo battles, full 180° obstacle avoidance | ➡️ TOF8 (8-way full view) |

## 📋 Specifications

| Item | Spec |
| :--- | :--- |
| Sensing elements | 2 laser time-of-flight (ToF) ranging units (one left, one right) |
| Range | 0 ~ 1200 mm (a reading of 120 cm = no target on that side) |
| Fault detection | Detects init failures, bus errors and timeouts per sensor |

## 🧩 Supported Platforms

| Platform | How |
| :--- | :--- |
| LEGO SPIKE Prime / Robot Inventor | Official app, Pybricks (👉 [SPIKE beginner examples: official app / MBC blocks site / Pybricks, 5 each — in Chinese](/sensors/tof2/spike-examples.md)) |
| LEGO MINDSTORMS EV3 | EV3 official software, Pybricks ([EV3 beginner examples](/en/sensors/tof2/ev3-examples.md)) |
| MakeBlock (mBot etc.) | Dedicated edition, [blocks & wiring](/en/makeblock.md) |
| Arduino / ESP32 (Universal I2C edition, coming soon) | Address `0x29`, [Arduino Examples](/en/sensors/tof2/arduino-examples.md) |

> [!IMPORTANT]
> Each platform is a **different factory edition**; please order the one that matches your controller.


## 📥 Example Downloads

| Hub | Files |
| :--- | :--- |
| SPIKE (Pybricks 4.0.0) | [Download all (.zip)](/examples/tof2/tof2_pybricks_spike_v4.0.0.zip) · [No library (blocks)](/examples/tof2/pybricks/For%20firmware%204.0.0/tof2_block_native_v400.py) · [Library example (blocks)](/examples/tof2/pybricks/For%20firmware%204.0.0/tof2_block_with_lib_v400.py) · [Library MBC_laser2_obj_Lib.py](/examples/tof2/pybricks/For%20firmware%204.0.0/MBC_laser2_obj_Lib.py) |
| SPIKE (Pybricks 3.6.1) | [No library (Python)](/examples/tof2/pybricks/For%20firmware%203.6.1/tof2_python_native_v361.py) |
| EV3 | [EV3 beginner examples](/en/sensors/tof2/ev3-examples.md) |
| Arduino (Universal I2C edition) | [Arduino Examples](/en/sensors/tof2/arduino-examples.md) |
| mBot etc. (MakeBlock) | [Block extension and examples](/en/makeblock.md) |

> Import the library file into the same Pybricks project without renaming it.

## 📤 Output Data

| Platform | What you read |
| :--- | :--- |
| SPIKE (Pybricks `PUPDevice(Port.X).read(3)`) | 1st value = left distance, 2nd value = right distance (cm, 0 ~ 120) |
| EV3 mode 0 | Average of left and right (0 ~ 100: 1 unit = 1.2 cm, 100 = no target) |
| EV3 mode 1 | Left distance, right distance (0 ~ 100 each, same scale; EV3 blocks: Beacon channel 1 Heading = left, Proximity = right) |

## 🔌 Hardware Wiring

1. Plug into SPIKE (Port A~F) or EV3 (Port 1~4) with a standard connection cable.
2. After powering on, check that the sensor indicator LED is lit — you are ready to go.

## 💡 How Direction Judgment Works

The two laser points sit side by side. When an object leaves the sensing range:
- **Left laser loses the signal first** → the object left to the **right**
- **Right laser loses the signal first** → the object left to the **left**

This lets your robot decide "which way to turn after the target disappears".

## ❓ FAQ

- **❓ Reading 120 cm?** → Nothing is within range on that side — this is normal (EV3 reads 100).
- **❓ Left and right read slightly differently against a flat wall?** → Tiny angle differences between the two beams cause small deviations; use the average (EV3 mode 0 already does) when you only need "how far".
- **❓ One side never reports?** → Check the init / fault bitmaps (I2C edition) or swap the cable and port, then [contact us](/en/contact.md) for service.
