<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Ranger 8-Way Laser Ranging (TOF8)

<div style="text-align: center;">
  <span style="display:inline-block; background:#0ABAB5; color:#fff; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">TOF-08-LSR · ADVANCED</span>
  <br>
  <img src="/images/sensors/tof8/tof8-product.webp" alt="Ranger 8-Way Laser Ranging" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(10,186,181,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>8-Channel Full-Angle Laser Ranging Array</strong><br>
    Say goodbye to ultrasonic ghosts · 180-degree zero blind spots · The LiDAR for sumo battles and high-speed obstacle avoidance
  </p>
</div>

---

> **Say goodbye to ultrasonic ghosts: 180-degree zero blind spots. The LiDAR for sumo battles and high-speed obstacle avoidance.**

## 🚀 Product Overview

Ultrasonic sensors have two fatal flaws in robotics competitions: **slow reaction speed** (limited by the speed of sound) and **susceptibility to echo ghosts** (interfering data). In sumo battles or high-speed obstacle avoidance, these flaws can directly lead to a match loss.

The Ranger 8-Way is equipped with 8 laser ranging units, covering a **complete 90 degrees left and right, totaling a 180-degree full field of view**. This gives the robot near-omniscient spatial awareness of its surroundings, making it the most comprehensive LEGO-compatible laser ranging solution on the market.

## 🎯 Core Advantages

- 🌐 **180-Degree Zero Blind Spot Coverage**: 4 units on the left, 4 on the right, covering the complete forward semicircle. Detect all threats with zero blind spots.
- ⚡ **Laser-Speed Reaction**: Measures distance at the speed of light with millisecond reaction times, allowing the robot to make decisions at maximum speed.
- 🥊 **Ultimate Sumo Weapon**: The full field of view gives opponents nowhere to hide. Lock on and pursue targets instantly.
- 🔼 **Seamless Upgrade Design**: Features dual 0-degree (straight forward) laser points, seamlessly integrating with the TOF2 programming logic for a zero-cost upgrade.

## 📐 Angle Distribution

The 8 laser ranging units are distributed at fixed angles, covering a complete 180-degree field of view:

| Channel | Angle | Direction |
| :---: | :---: | :--- |
| 1 | -90° | Direct Left |
| 2 | -60° | Left Forward 60° |
| 3 | -30° | Left Forward 30° |
| 4 | 0° | Direct Forward |
| 5 | 0° | Direct Forward (For TOF2 transition) |
| 6 | +30° | Right Forward 30° |
| 7 | +60° | Right Forward 60° |
| 8 | +90° | Direct Right |

> **💡 Design Ingenuity**: Retaining two direct forward (0-degree) sensors allows users upgrading from TOF2 to seamlessly carry over their original programming logic. You only need to add processing for the extra channels.

## 📋 Specifications

| Item | Spec |
| :--- | :--- |
| Sensing elements | 8 laser time-of-flight (ToF) ranging units |
| Range | 0 ~ 1200 mm (a reading of 1200 mm / 120 cm = no target in that direction) |
| Field of view | 180° in front (90° to each side) |
| Bearing resolution | 17 bearings (0 ~ 16, 8 = straight ahead) |
| Startup | About 2 ~ 4 s to initialize all 8 sensors (the link to the hub stays alive meanwhile) |
| Fault detection | Detects init failures, bus errors and timeouts per sensor; readable by the host |

## 🧩 Supported Platforms

| Platform | How |
| :--- | :--- |
| LEGO SPIKE Prime / Robot Inventor | Official app, Pybricks |
| LEGO MINDSTORMS EV3 | EV3 official software, Pybricks |
| Arduino / ESP32 / Raspberry Pi | Universal I2C (address `0x29`) |
| MATRIX Mini R4 | I2C |
| MakeBlock | Dedicated edition |

> [!IMPORTANT]
> Each platform is a **different factory edition** (different connector and firmware). Please order the edition that matches your controller, or [contact us](/en/contact.md) if unsure.

## 📤 Output Data

**Bearing:** `0` ~ `16`, `8` = straight ahead; smaller = further left, larger = further right. The EV3 and I2C editions report it as `-8` (far left) ~ `+8` (far right), `0` = straight ahead.

### SPIKE Modes (Pybricks: `PUPDevice(Port.X).read(mode)`)

| Mode | Content |
| :---: | :--- |
| 1 | Bearing of the nearest target (0 ~ 16) |
| 2 | Distance to the nearest target (cm, 0 ~ 120) |
| 3 | Bearing, nearest distance, leftmost distance, rightmost distance (4 values — the usual competition choice) |
| 9 | Everything: 8 distances (cm), nearest bearing & distance, farthest bearing & distance, init-OK bitmap, fault bitmap |

### EV3 Modes

| Mode | Content |
| :---: | :--- |
| 0 | Distance straight ahead (average of the two 0° sensors, 0 ~ 100) |
| 1 | All 8 distances (0 ~ 100 each) |
| 2 | Nearest bearing (-8 ~ +8), nearest distance, farthest bearing, farthest distance |

## ∿ Universal I2C Registers (address `0x29`)

See the [MBC Universal I2C Protocol](/en/i2c-protocol.md) for shared rules and Arduino helpers. Distances are uint16 in mm: **`1200` = no target in range, `0xFFFF` = sensor fault**.

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x11` | R | 6 | Bearing bundle (= `0x12` ~ `0x15`) |
| `0x12` | R | 1 | Nearest target bearing int8 (-8 left ~ +8 right, 0 = ahead) |
| `0x13` | R | 2 | Nearest distance mm |
| `0x14` | R | 1 | Farthest target bearing int8 |
| `0x15` | R | 2 | Farthest distance mm |
| `0x21` | R | 16 | All 8 distances (= `0x22` ~ `0x29`) |
| `0x22` ~ `0x29` | R | 2 | Sensor 0 ~ 7 distance mm (sensor 0 = leftmost, 7 = rightmost) |
| `0x32` | R | 1 | bit0 = ready (all 8 initialized), bit1 = any sensor faulty |
| `0x34` / `0x35` / `0x36` | R | 1 | Init-OK / bus-error / timeout bitmaps (bit i = sensor i) |

Variant `0x06` = `[8, 120]` (sensor count, max range ×10 mm); model `0x07` = `LASER8`.

```cpp
// Read the nearest target's bearing and distance
uint8_t d[6];
if (iicRead(0x29, 0x11, d, 6)) {
  int8_t   dir  = (int8_t)d[0];            // -8 left ~ +8 right
  uint16_t dist = (d[1] << 8) | d[2];      // mm, 1200 = no target
}
```

## 🔌 Hardware Wiring

1. Plug into SPIKE (Port A~F) or EV3 (Port 1~4) with a standard cable
2. Initialization takes about 2 ~ 4 s after power-on; once the indicator LED is on, it is ready

## ❓ FAQ

- **❓ Reading 120 cm (1200 mm)?** → Nothing is within range in that direction — this is normal.
- **❓ The two 0° sensors read differently?** → Tiny angle differences between them cause small deviations; average them (EV3 mode 0 already does).
- **❓ One sensor never reports?** → Check the fault bitmaps (SPIKE mode 9 or I2C `0x34`~`0x36`) to see which one, then [contact us](/en/contact.md) for service.
