<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Controller PS4/PS5 Bluetooth Receiver

<div style="text-align: center;">
  <span style="display:inline-block; background:#F2C200; color:#0a0a0a; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">CMD-BT-PRO · PS4 / PS5</span>
  <br>
  <img src="/images/sensors/ps4/ps4-product.webp" alt="Controller PS4/PS5 Bluetooth Receiver" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(242,194,0,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>Bluetooth receiver for PS4 / PS5 gamepads</strong><br>
    Bonds to your gamepad only · Auto-reconnect · Stops on signal loss
  </p>
</div>

---

> **Drive LEGO robots with a PS4 or PS5 gamepad: it only listens to your gamepad and stops the moment the signal drops — even with dozens of teams in the room.**

## 🚀 Product Overview

Dual-hub Bluetooth setups waste resources and drop out; infrared remotes have few buttons and need line of sight; 2.4G receivers interfere with each other when many robots share a venue.

The Controller PS4/PS5 Bluetooth Receiver lets SPIKE, EV3 or Arduino read every button and stick of a **PS4 (DualShock 4) or PS5 (DualSense) gamepad**. Pair once and it remembers; after that it reconnects automatically at power-on. At competitions it never connects to another team's gamepad, and if the signal drops all outputs go to zero so the robot cannot run away.

## 🎯 Core Advantages

- 🎮 **Works with PS4 and PS5 gamepads**: DualShock 4 and DualSense
- 🔒 **Bonds to your gamepad only**: once paired, Bluetooth scanning switches off, so it never grabs another team's gamepad and ignores gamepads pairing nearby — made for venues with dozens of teams
- 🛑 **Stops on signal loss**: if no gamepad data arrives for 0.3 s, outputs reset to zero — no runaway robots
- 🧠 **Pairing memory**: pair once, reconnects automatically at every power-on
- 📶 **Stable link**: solid across a normal competition venue (tested up to 100 m in open space)
- 🕹️ **Full controls**: twin sticks, analog triggers, D-pad and every function button; the I2C edition also controls rumble and the light bar

## 📊 Controller Series Comparison

| Feature | 2.4G (PS2) | Bluetooth (PS4/PS5) |
| :--- | :---: | :---: |
| Gamepad | PS2 2.4G wireless gamepad | PS4, PS5 gamepads |
| Stability with many devices | ⚠️ May interfere | ✅ Bonds to your gamepad only |
| Pairing memory (auto-connect) | ❌ | ✅ |
| Zero outputs on signal loss | — | ✅ 0.3 s |
| Range | About 10-15 m | Stable across a venue (100 m tested in open space) |
| Analog triggers | ❌ | ✅ |

## 🧩 Supported Platforms

| Platform | How |
| :--- | :--- |
| LEGO SPIKE Prime / Robot Inventor | Official app, Pybricks |
| LEGO MINDSTORMS EV3 | EV3 official software, Pybricks |
| Arduino / ESP32 / Raspberry Pi | Universal I2C (address `0x42`) |
| MakeBlock | Dedicated edition |

> [!IMPORTANT]
> Each platform is a **different factory edition**; please order the one that matches your controller.

## 🔗 Pairing

1. Plug the receiver into SPIKE (Port A~F) or EV3 (Port 1~4) and power on.
2. **First use or switching gamepads**: press the **Reset** button on the receiver. It opens a **30-second** pairing window.
3. Within those 30 seconds put the gamepad into pairing mode:
   - **PS4 gamepad**: hold **SHARE + PS** until the light bar flashes rapidly
   - **PS5 gamepad**: hold **Create + PS** until the light bar flashes rapidly
4. The first gamepad that connects is remembered and scanning stops. From then on just switch the gamepad on and it reconnects.

> [!TIP]
> At a competition, pair at your own table so no other team's gamepad is in pairing mode during those 30 seconds. Bonded the wrong one? Press Reset and pair again.

## 📤 Output Data

**Stick values (SPIKE / EV3):** `0` ~ `99`, `50` = center, **pushing up or right increases the value**.

### EV3 Modes

| Mode | Content |
| :---: | :--- |
| 0 | D-pad state |
| 1 | D-pad, left stick X, shape buttons (■ ✕ ● ▲), left stick Y, shoulder buttons (L1 R1 L2 R2), right stick X, function buttons (START SELECT L3 R3), right stick Y |
| 2 | Left stick X, left stick Y, right stick X, right stick Y |

## ∿ Universal I2C Registers (address `0x42`)

See the [MBC Universal I2C Protocol](/en/i2c-protocol.md) for shared rules and Arduino helpers.

> [!NOTE]
> The receiver has built-in level shifting and connects directly to 5 V boards like the Arduino UNO. **Wait 2 ms** after writing the register number before reading.

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x10` | W | 1 | Rumble: strength `0` ~ `255` for 200 ms; `0` = stop |
| `0x11` | R | 9 | Gamepad bundle (= `0x12` ~ `0x15`) |
| `0x12` | R | 2 | Buttons uint16 (b15 ■, b14 ✕, b13 ●, b12 ▲, b11 R1, b10 L1, b9 R2, b8 L2, b7 left, b6 down, b5 right, b4 up, b3 START, b2 R3, b1 L3, b0 SELECT) |
| `0x13` | R | 4 | Sticks LX, LY, RX, RY: int8 `-127` ~ `+127`, 0 = center, **right / up positive**, dead zone applied |
| `0x14` | R | 2 | Analog triggers L2, R2: `0` ~ `255` |
| `0x15` | R | 1 | System buttons (PS button, etc.) |
| `0x22` | R | 4 | Simple sticks LX, LY, RX, RY: `0` ~ `99`, 50 = center (same as the SPIKE / EV3 editions) |
| `0x23` | RW | 3 | Light bar color R, G, B (write all 3 bytes; remembered while disconnected, applied on connect) |
| `0x32` | R | 1 | bit0 = ready, bit1 = gamepad connected |
| `0x34` | R | 1 | Gamepad type: 0 = none, 3 = PS4 gamepad, 4 = PS5 or other Bluetooth gamepad |
| `0x35` | R | 1 | Gamepad battery `0` ~ `255` |

Variant `0x06` byte 1 = `2` (the PS2 receiver reports `1`). While disconnected, buttons, sticks and triggers read 0.

```cpp
// Read buttons and sticks (keep the delay(2) inside iicRead)
uint8_t d[9];
if (iicRead(0x42, 0x11, d, 9)) {
  uint16_t btn = (d[0] << 8) | d[1];
  int8_t lx = (int8_t)d[2], ly = (int8_t)d[3];
  bool cross = btn & (1 << 14);              // ✕ button
}

// Set the light bar to red
Wire.beginTransmission(0x42);
Wire.write(0x23); Wire.write(255); Wire.write(0); Wire.write(0);
Wire.endTransmission();
```
