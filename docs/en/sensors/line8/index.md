# Pathfinder 8-Way Sensor (LineSensor8)

<div style="text-align: center; margin-bottom: 2rem;">
  <span style="display:inline-block; background:#ff4500; color:#fff; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">LNS-08-PRO · COMPETITION GRADE</span>
  
  <div style="display: flex; flex-wrap: wrap; align-items: center; justify-content: center; gap: 30px; margin: 20px 0;">
    <div style="flex: 1; min-width: 250px; text-align: center;">
      <img src="../images/sensors/line8/line8-product-spike.webp" alt="Pathfinder 8-Way Sensor" style="max-width: 280px; width: 100%; display: block; margin: 0 auto; filter: drop-shadow(0 0 20px rgba(255,69,0,0.65)) drop-shadow(0 0 40px rgba(255,69,0,0.35));" />
    </div>
    <div style="flex: 1; min-width: 300px; max-width: 480px; aspect-ratio: 16/9; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(255,69,0,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.5);">
      <iframe src="https://www.youtube.com/embed/MuG9kp2-8FY" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
    </div>
  </div>

  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>8-Channel Anti-Interference Light Reflection Array</strong><br>
    One-Click Calibration · Ignore Track Noise · Designed for Complex WRO Maps
  </p>
</div>

> [!IMPORTANT]
> **⚠️ Purchasing and Compatibility Notice (Not universally compatible across systems!)**
> This sensor uses a **"dedicated model"** hardware design. It is manufactured with specific physical sockets and electrical protocols:
> 1. **🧱 SPIKE Edition**: Uses LPF2 (Lego 6-pin) flat socket. Compatible ONLY with SPIKE Prime, SPIKE Essential, and Robot Inventor hubs.
> 2. **🧱 EV3 Edition**: Uses RJ12 (Lego specific 6-pin) socket. Compatible ONLY with EV3 hubs.
> 3. **∿ Universal I2C Edition**: Uses standard pin headers/XH2.54 ports. Designed for Arduino, ESP32, Raspberry Pi, etc.
> *Please select the hardware version corresponding to your hub system. The physical connectors differ completely.*

---

## 🚀 Product Overview & Hardware Advantages

International competition maps like WRO often feature colorful patterns near the black line. Traditional line followers easily misjudge these, causing the robot to "follow the pattern" and lose control.
The MBC 8-Way Line Sensor is specifically optimized for this pain point. Equipped with highly advanced anti-interference algorithms, it accurately locks onto the black line and ignores all surrounding visual distractions on the most complex tracks!

* **🚀 Competition-Grade Core**: Onboard smart processing chip designed for high-frequency control, completing 8-channel noise filtering and position solving in milliseconds.
* **⚡ Zero-Latency Sampling**: Hardware-level parallel data channels. Signals from 8 paths are sampled independently and at high speed in the background without occupying hub bandwidth.
* **🛡️ WRO First Choice Layout**: Precisely arranged 8 sets of anti-interference infrared arrays (Channels 0 to 7 from right to left), ignoring flashy patterns beside the track.

## 🔌 Hardware Wiring

Just like connecting a LEGO sensor! Use a standard connection cable and plug it directly into the SPIKE or EV3 hub.

1. **Find the Port**: The port on the sensor is on the side, just like official LEGO sensors.
2. **Use the Cable**: Use the included standard cable (or original LEGO cable).
3. **Plug into Hub**: SPIKE can use any **Port A to F**. EV3 uses **Port 1 to 4**.
4. **Check Indicator**: After powering on, the green heartbeat indicator (near the socket) will start flashing.

---

## 🎯 Calibration Guide

Every surface reflects light differently. Calibration teaches the sensor "this brightness is white, and that brightness is black".

### 📺 Official One-Click Calibration Video
Watch the demonstration video below for the most intuitive guide:

<div style="max-width: 480px; aspect-ratio: 16/9; margin: 20px auto; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(255,69,0,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.5);">
  <iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
</div>

### Calibration Steps (Super Simple!)

1. **Press Calibration Button**: Hold the calibration button on the sensor for **0.5 seconds** until the **🟢 green calibration indicator** remains steadily lit.
2. **Start Scanning**: Move the sensor back and forth across the black line and white areas of your track.
3. **Maintain Height**: Keep the sensor between **0.5 ~ 1.2 cm** above the ground.
4. **Complete**: After 5 seconds, the green calibration indicator turns off. Calibration is complete and saved automatically!

---

## 🎮 Select Your Hub

Please select your controller system first, then select your software environment:

<div class="env-hub-grid" style="grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));">
  <a href="#/en/sensors/line8/spike-hub" class="env-card spike" style="padding: 40px 20px;">
    <div style="display: flex; gap: 20px; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <img src="../images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.3));" />
      <img src="../images/hubs/inventor-hub.webp" alt="Robot Inventor Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.3));" />
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">🧱 SPIKE Hubs</h3>
    <p style="font-size: 1.1rem;">Education (SPIKE 3) / Home (Robot Inventor) / pyBricks</p>
  </a>
  
  <a href="#/en/sensors/line8/ev3-hub" class="env-card ev3" style="padding: 40px 20px;">
    <div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <img src="../images/hubs/ev3-hub.webp" alt="EV3 Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.3));" />
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">🧱 EV3 Hubs</h3>
    <p style="font-size: 1.1rem;">Official Software / Classroom / clev3r / pyBricks</p>
  </a>

  <a href="#/en/sensors/line8/arduino-i2c" class="env-card mcu" style="padding: 40px 20px;">
    <div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <svg viewBox="0 0 100 40" style="width: 120px; height: 50px; fill: none; stroke: #00d2ff; stroke-width: 4; stroke-linecap: round; stroke-linejoin: round; filter: drop-shadow(0 0 15px rgba(0, 210, 255, 0.6));">
        <path d="M 5,25 H 25 V 5 H 45 V 35 H 65 V 5 H 85 V 35 H 95" />
      </svg>
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">∿ Universal I2C</h3>
    <p style="font-size: 1.1rem;">Arduino / ESP32 / Raspberry Pi / Custom Boards</p>
  </a>
</div>
