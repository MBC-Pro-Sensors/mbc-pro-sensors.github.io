# Ranger 2-Way Laser Ranging (TOF2) (To be updated)

<div style="text-align: center;">
  <span style="display:inline-block; background:#0ABAB5; color:#fff; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">TOF-02-LSR · ADVANCED</span>
  <br>
  <img src="../images/sensors/tof2/tof2-product.webp" alt="Ranger 2-Way Laser Ranging" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(10,186,181,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>Dual-Beam Laser Ranging Module</strong><br>
    Lightweight and agile · Accurately capture object lateral movements · Say goodbye to bulky ultrasonics
  </p>
</div>

---

> **Lightweight and agile: Say goodbye to bulky ultrasonics. A micro-laser that accurately captures the left and right movements of objects.**

## 🚀 Product Overview

Traditional ultrasonic sensors are not only bulky but also suffer from slow reaction speeds and susceptibility to echo interference (producing ghost data). In high-speed robotics applications, they are completely inadequate.

The Ranger 2-Way utilizes **Laser Time-of-Flight (ToF) technology**, measuring distance at the speed of light. Its reaction speed far exceeds ultrasonic sensors. Its extremely lightweight design makes it suitable for installation in various positions on a robot. Even more uniquely, its dual-laser configuration enables the robot to accurately determine whether an object **left from the left side or the right side**—a feature ultrasonic sensors simply cannot achieve.

## 🎯 Core Advantages

- ⚡ **Laser-Speed Reaction**: Unconstrained by the speed of sound, detection speed is vastly faster than ultrasonics.
- 🎯 **Directional Judgment**: Unique dual-beam design can determine whether an object finally left the sensing range from the left or right side.
- 📦 **Extremely Lightweight**: Significantly smaller than ultrasonics, saving valuable installation space on the robot.
- 🔌 **Plug & Play**: Plug directly into SPIKE / EV3; no extra power supply or complex setup required.
- 🔼 **Seamless Upgrade Path**: Retains a dual 0-degree design compatible with TOF8, allowing future upgrades to TOF8 without modifying core programming logic.

## 📊 Application Scenarios Comparison

| Scenario | Recommended Model |
| :--- | :--- |
| Simple collision avoidance, distance detection, directional judgment | ✅ **TOF2 (This model)** |
| Sumo battles, full 180-degree obstacle avoidance | ➡️ TOF8 (8-channel full view) |

## 🔌 Hardware Wiring

1. Plug in using a standard connection cable to SPIKE (Ports A~F) or EV3 (Ports 1~4).
2. After powering on, confirm the sensor indicator light is on.

## 💡 Directional Judgment Explanation

The two laser points on the sensor are arranged side-by-side. When an object leaves the sensing range:
- **Left laser loses signal first** → The object left from the **right** side.
- **Right laser loses signal first** → The object left from the **left** side.

This allows your robot to make intelligent decisions regarding "which direction to turn after the target disappears".
