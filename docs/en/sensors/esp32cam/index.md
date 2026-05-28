# Sharpshooter Vision Sensor (ESP32CAM) (To be updated)

<div style="text-align: center;">
  <span style="display:inline-block; background:#00d2ff; color:#0a0a0a; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">CAM-VIS-PRO · VISION</span>
  <br>
  <img src="../images/sensors/esp32cam/esp32cam-product.webp" alt="Sharpshooter Vision Sensor" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(0,210,255,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>High-End Robot Vision Camera Module</strong><br>
    Grant your robot true vision · Precise color tracking · Coordinate positioning
  </p>
</div>

---

> **Grant your robot true vision: High-end camera specializing in color tracking and precise coordinate positioning.**

## 🚀 Product Overview

Official LEGO color sensors can only detect a single point of color directly underneath them, and are completely incapable of "tracking a moving color block" or "determining a target's position in the frame".

The Sharpshooter Vision Sensor gives your LEGO robot real "eyes". It instantly detects the position and size of specified color blocks within its field of view, and transmits coordinate data directly to the hub. This allows your robot to actively track targets and execute precise alignment tasks, opening up entirely new possibilities for robotics applications.

## 🎯 Core Advantages

- 👁️ **True Vision Tracking**: Detects the X/Y coordinates and area of specific color blocks in the frame.
- 🎯 **Precise Coordinate Positioning**: Identify exact positions by querying colors at specific coordinate points.
- 🔌 **Plug & Play**: Plug directly into SPIKE / EV3; official blocks read vision data instantly.
- 🚀 **Zero Programming Barrier**: No need to learn image processing. Complex vision algorithms run entirely inside the sensor.

## 📋 Main Features

### Color Block Tracking
After setting a target color, the sensor continuously outputs:
- **X Coordinate**: Horizontal position of the color block's center.
- **Y Coordinate**: Vertical position of the color block's center.
- **Area**: Size of the color block (can be used to estimate distance).

### Point Color Query
Query the color at a specific coordinate point in the frame, ideal for:
- Identifying landmark colors.
- Verifying the placement of objects.

## 🔌 Hardware Wiring

1. Plug the sensor into SPIKE (Ports A~F) or EV3 (Ports 1~4).
2. The setup is complete once the sensor indicator lights up after powering on; no additional configuration is required.
