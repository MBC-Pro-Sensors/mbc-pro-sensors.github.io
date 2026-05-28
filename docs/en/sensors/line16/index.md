# Pathfinder 16-Way Sensor (LineSensor16)

<div style="text-align: center; margin-bottom: 2rem;">
  <span style="display:inline-block; background:#ff6b35; color:#fff; padding:3px 12px; border-radius:20px; font-family:monospace; margin-bottom:1rem; font-size: 0.85rem;">LNS-16-PRO · EXTREME SPEED</span>
  
  <div style="display: flex; flex-wrap: wrap; align-items: center; justify-content: center; gap: 30px; margin: 20px 0;">
    <div style="flex: 1; min-width: 250px; text-align: center;">
      <img src="../images/sensors/line16/line16-product-spike.webp" alt="Pathfinder 16-Way Sensor" style="max-width: 280px; width: 100%; display: block; margin: 0 auto; filter: drop-shadow(0 0 20px rgba(255,107,53,0.65)) drop-shadow(0 0 40px rgba(255,107,53,0.35));" />
    </div>
    <div style="flex: 1; min-width: 300px; max-width: 480px; aspect-ratio: 16/9; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(255,107,53,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.5);">
      <iframe src="https://www.youtube.com/embed/MuG9kp2-8FY" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
    </div>
  </div>

  <p style="font-family: monospace; color: #aaa; letter-spacing: 1px;">
    <strong>16-Channel Ultra-Wide Light Reflection Array</strong><br>
    Double the Field of View · Say Goodbye to Patching · The King of High-Speed Following
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

When robots engage in extremely high-speed line following, a narrow field of view often causes them to completely lose the line during sharp turns. Competitors used to physically patch two 8-way sensors together, wasting a precious hub port and increasing program complexity.
The Pathfinder 16-Way fundamentally solves this. It provides a massive 16-channel array taking up only ONE port!

* **🚀 Double the Vision**: Features 16 anti-interference infrared units spanning the entire width of the robot. Never lose the track again on sharp turns.
* **⚡ Single Port Efficiency**: Eliminates the need to use two hub ports, freeing up space for more motors or mechanisms.
* **🛡️ Competition-Grade Reliability**: Fully inherits the advanced anti-interference core of the Line8, remaining stable on the most complex WRO maps.

## 🔌 Hardware Wiring

Just like connecting a LEGO sensor! Use a standard connection cable and plug it directly into the SPIKE or EV3 hub.

1. **Find the Port**: The port on the sensor is on the side.
2. **Use the Cable**: Use the included standard cable.
3. **Plug into Hub**: SPIKE can use any **Port A to F**. EV3 uses **Port 1 to 4**.
4. **Check Indicator**: After powering on, the green heartbeat indicator will start flashing.

---

## 🎯 Calibration Guide

Every surface reflects light differently. Calibration teaches the sensor "this brightness is white, and that brightness is black".

### 📺 Official One-Click Calibration Video
Watch the demonstration video below for the most intuitive guide:

<div style="max-width: 480px; aspect-ratio: 16/9; margin: 20px auto; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(255,107,53,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.5);">
  <iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
</div>

### Calibration Steps (Super Simple!)

1. **Press Calibration Button**: Hold the calibration button on the sensor for **0.5 seconds** until the **🟢 green calibration indicator** remains steadily lit.
2. **Start Scanning**: Move the sensor back and forth across the black line and white areas of your track. Ensure all 16 sensors pass over the black line and white ground.
3. **Maintain Height**: Keep the sensor between **0.5 ~ 1.2 cm** above the ground.
4. **Complete**: After 5 seconds, the green calibration indicator turns off. Calibration is complete and saved automatically!

---

## 🎮 Select Your Hub

Please select your controller system first, then select your software environment:

<div class="env-hub-grid" style="grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));">
  <a href="#/en/sensors/line16/spike-hub" class="env-card spike" style="padding: 40px 20px;">
    <div style="display: flex; gap: 20px; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <img src="../images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.3));" />
      <img src="../images/hubs/inventor-hub.webp" alt="Robot Inventor Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.3));" />
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">🧱 SPIKE Hubs</h3>
    <p style="font-size: 1.1rem;">Education (SPIKE 3) / Home (Robot Inventor) / pyBricks</p>
  </a>
  
  <a href="#/en/sensors/line16/ev3-hub" class="env-card ev3" style="padding: 40px 20px;">
    <div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <img src="../images/hubs/ev3-hub.webp" alt="EV3 Hub" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.3));" />
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">🧱 EV3 Hubs</h3>
    <p style="font-size: 1.1rem;">Official Software / Classroom / clev3r / pyBricks</p>
  </a>

  <a href="#/en/sensors/line16/arduino-i2c" class="env-card mcu" style="padding: 40px 20px;">
    <div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
      <svg viewBox="0 0 100 40" style="width: 120px; height: 50px; fill: none; stroke: #00d2ff; stroke-width: 4; stroke-linecap: round; stroke-linejoin: round; filter: drop-shadow(0 0 15px rgba(0, 210, 255, 0.6));">
        <path d="M 5,25 H 25 V 5 H 45 V 35 H 65 V 5 H 85 V 35 H 95" />
      </svg>
    </div>
    <h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">∿ Universal I2C</h3>
    <p style="font-size: 1.1rem;">Arduino / ESP32 / Raspberry Pi / Custom Boards</p>
  </a>
</div>
