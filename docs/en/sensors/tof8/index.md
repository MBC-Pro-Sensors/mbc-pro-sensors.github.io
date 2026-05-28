# Ranger 8-Way Laser Ranging (TOF8) (To be updated)

<div style="text-align: center;">
  <span style="display:inline-block; background:#0ABAB5; color:#fff; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">TOF-08-LSR · ADVANCED</span>
  <br>
  <img src="../images/sensors/tof8/tof8-product.webp" alt="Ranger 8-Way Laser Ranging" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(10,186,181,0.2));" />
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

## 🔌 Hardware Wiring

1. Plug in using a standard connection cable to SPIKE (Ports A~F) or EV3 (Ports 1~4).
2. After powering on, confirm the sensor indicator light is on.

## ❓ FAQ

- **❓ Reading strangely large numbers?** → This indicates no object was detected in that direction, which is perfectly normal. You can set a maximum threshold in your program to filter these out.
- **❓ Two sensors at the same angle show different values?** → Minor angular differences between the two 0-degree sensors might cause slightly different readings. It is recommended to use their average.
