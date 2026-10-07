<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Sharpshooter ESP32CAM Vision Sensor

<div style="text-align: center;">
  <span style="display:inline-block; background:#00d2ff; color:#0a0a0a; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">CAM-VIS-PRO · VISION</span>
  <br>
  <img src="/images/sensors/esp32cam/esp32cam-product.webp" alt="Sharpshooter ESP32CAM vision sensor" style="max-width: 280px; margin: 1rem auto; display: block; filter: drop-shadow(0 0 30px rgba(0,210,255,0.2));" />
  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>LEGO robot vision camera with a built-in screen</strong><br>
    Color blob tracking · Point color pick · Motion detection · On-device setup
  </p>
</div>

---

> **Give your LEGO robot real vision: pick the color to track right on the sensor's screen — no image-processing code needed.**

## 🚀 Product Overview

The official LEGO color sensor only sees a single spot directly underneath it. It cannot track a moving color blob or tell where a target is in the scene.

The Sharpshooter ESP32CAM gives your robot real eyes: it finds the position and size of a chosen color blob in real time, reads the color at any point of the image, and detects motion. Everything is set up on the **color screen and buttons on the back of the sensor** while you watch the live image; settings are stored on the device and ready at the next power-on.

## 🎯 Core Advantages

- 🖥️ **Live preview on the device**: a 1.8" color screen on the back shows the camera image and detection results
- 🎛️ **Button menu setup**: pick point position, HSV tolerances and minimum blob area on the sensor itself — no computer needed
- 💾 **Settings survive power-off**: color setups and camera settings (brightness, contrast, saturation, exposure, white balance, flip, mirror) are stored on the device
- 🎯 **Five outputs at once**: point color, blob area, blob X / Y, motion value
- 💡 **Built-in flash light**: stable color detection in dim venues
- 🔭 **Optional lenses**: narrow (see far) or wide (see more) to suit your field

<div style="text-align: center;">
<img src="/images/sensors/esp32cam/esp32cam-product2.webp" alt="1.8-inch color screen on the back of the Sharpshooter" style="max-width: 320px; margin: 1rem auto; display: block;" />
</div>

## 📋 Specifications

| Item | Spec |
| :--- | :--- |
| Image resolution | 160 × 120 (X `0` ~ `159`, Y `0` ~ `119`) |
| Screen | 1.8" 128 × 160 color TFT |
| Buttons | Left, right, enter, side (short press: open/close menu; long press 1 s in the menu: clear that color slot) |
| Lens options (field of view) | 65° / 110° / 120° / 160° |
| Flash light | Built-in, switched in the on-device menu (setting is saved) |

## 📤 Output Data

| Output | Content |
| :--- | :--- |
| Point color | Color at the chosen point (hue code: `0` ~ `239` = hue, `240` ~ `245` = white, `246` ~ `250` = black) |
| Blob area | `0` ~ `100` (`0` = not found; X / Y are then invalid) |
| Blob X | Horizontal position of the blob center |
| Blob Y | Vertical position of the blob center |
| Motion value | `0` ~ `100`, higher = more change in the image |

## 🧩 Supported Platforms

| Platform | How |
| :--- | :--- |
| LEGO SPIKE Prime / Robot Inventor | Official app, Pybricks |
| LEGO MINDSTORMS EV3 | EV3 official software, Pybricks |
| MATRIX Mini R4 | UART |

> [!IMPORTANT]
> Each platform is a **different factory edition**; please order the one that matches your controller.

## 🔌 Hardware Wiring

1. Plug the sensor into SPIKE (Port A~F) or EV3 (Port 1~4)
2. Once the screen shows the camera image, it is ready
3. Short-press the side button to open the setup menu; use left / right to choose and enter to confirm. Color setups and camera settings are saved automatically
