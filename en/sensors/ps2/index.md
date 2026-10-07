<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Controller 2.4G Receiver (PS2)

<!-- product-hero -->

---

> **Cost-effective entry-level choice: Easily expand your control buttons. The perfect wireless tool for teaching and small projects.**

## 🚀 Product Overview

The infrared remote control provided by LEGO offers a frustratingly small number of buttons, and using two EV3 hubs for a Bluetooth connection is extremely resource-intensive and prone to disconnection.

The Controller 2.4G Receiver provides a **high cost-performance solution**: It allows your SPIKE or EV3 hub to directly read input from a wireless controller featuring dual joysticks and abundant buttons via standard interfaces, unleashing your complete control over the robot.

## 🎯 Core Advantages

- 🎮 **Rich Button Layout**: Dual joysticks + directional pad + multiple action buttons, offering vastly more control options than the original remote.
- 💰 **High Cost-Performance**: The most economical solution for entry-level remote control needs.
- 🔌 **Plug & Play**: Plug directly into SPIKE / EV3; official blocks read button states instantly.

## ⚠️ Limitations & Recommended Scenarios

> [!CAUTION]
> **2.4G Frequency Band Notice**
> 2.4GHz is a public frequency band shared with Wi-Fi, microwaves, etc. When **12 or more sets** of equipment are used simultaneously in the same venue, interference may occur due to channel congestion. We recommend upgrading to the Bluetooth version (PS4/PS5) for such scenarios.

| Scenario | Suitability |
| :--- | :--- |
| Personal practice, home use, small projects | ✅ Highly suitable |
| School clubs (under 10 sets) | ✅ Suitable |
| Large competition venues (12+ sets simultaneously) | ⚠️ Recommend the PS4/PS5 version |

## 🧩 Supported Platforms

| Platform | How |
| :--- | :--- |
| LEGO SPIKE Prime / Robot Inventor | Official app, Pybricks |
| LEGO MINDSTORMS EV3 | EV3 official software, Pybricks |
| Arduino / ESP32 / Raspberry Pi | Universal I2C (address `0x42`) |
| MakeBlock | Dedicated edition |

> [!IMPORTANT]
> Each platform is a **different factory edition**; please order the one that matches your controller.

## 📤 Output Data

**Stick values (SPIKE / EV3):** `0` ~ `99`, `50` = center, **pushing up or right increases the value**.

| EV3 mode | Content |
| :---: | :--- |
| 0 | D-pad state |
| 1 | D-pad, left stick X, shape buttons (■ ✕ ● ▲), left stick Y, shoulder buttons (L1 R1 L2 R2), right stick X, function buttons (START SELECT L3 R3), right stick Y |
| 2 | Left stick X, left stick Y, right stick X, right stick Y |

## ∿ Universal I2C Registers (address `0x42`)

Same register table as the [PS4/PS5 receiver](/en/sensors/ps4/index.md), so host code can be shared. Shared rules: [MBC Universal I2C Protocol](/en/i2c-protocol.md).

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x11` | R | 9 | Gamepad bundle (= `0x12` ~ `0x15`) |
| `0x12` | R | 2 | Buttons uint16 (b15 ■, b14 ✕, b13 ●, b12 ▲, b11 R1, b10 L1, b9 R2, b8 L2, b7 left, b6 down, b5 right, b4 up, b3 START, b2 R3, b1 L3, b0 SELECT) |
| `0x13` | R | 4 | Sticks LX, LY, RX, RY: int8 `-127` ~ `+127`, 0 = center, right / up positive |
| `0x14` | R | 2 | Triggers L2, R2: `0` ~ `255` (only 0 / 255 if the gamepad has no pressure sensing) |
| `0x22` | R | 4 | Simple sticks LX, LY, RX, RY: `0` ~ `99`, 50 = center |
| `0x32` | R | 1 | bit0 = ready, bit1 = gamepad connected |
| `0x34` | R | 1 | Gamepad type: 0 = none, 1 = digital mode, 2 = analog mode |

Variant `0x06` byte 1 = `1`. The PS2 edition has no rumble or light bar (writes to `0x10` and `0x23` are ignored).

## 🔌 Hardware Wiring

1. Plug the receiver into SPIKE (Ports A~F) or EV3 (Ports 1~4).
2. Power on the controller and wait for pairing to complete (indicator light remains steady).
3. Read sensor values in the official block environment to retrieve button states.
