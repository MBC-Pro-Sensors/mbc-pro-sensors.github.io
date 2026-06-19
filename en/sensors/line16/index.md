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

---

## 🧠 Core Data Features & Algorithms

To achieve competition-level smoothness and stability, the onboard hardware processes all 16 raw reflection signals in the background with real-time calibration, binarization, and continuity filtering, producing four core data features:

<div class="responsive-grid-2" style="gap: 20px; margin: 25px 0;">
  <div style="background: rgba(10,186,181,0.05); border: 1px solid rgba(10,186,181,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
    <h4 style="color: var(--theme-color); margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">📊 Absolute Physical Light Value (0 ~ 100)</h4>
    <p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">Reflects the truest physical reflection intensity. <strong>100 = pure white (high reflectance), 0 = pure black (no reflectance)</strong>. This value remains physically accurate and does not invert when switching between black/white line modes. Ideal for building real-time bar graph visualization dashboards.</p>
  </div>
  <div style="background: rgba(242,194,0,0.05); border: 1px solid rgba(242,194,0,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
    <h4 style="color: var(--accent-color); margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🎯 Sub-Pixel High-Resolution Smooth Position (0 ~ 200)</h4>
    <p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">Uses high-precision geometric interpolation of adjacent sensor reflection intensities to output a <strong>super-smooth continuous position from 0 ~ 200 (100 = dead center)</strong>. Designed specifically for advanced PID control algorithms to completely eliminate micro-jitter.</p>
  </div>
  <div style="background: rgba(255,107,53,0.05); border: 1px solid rgba(255,107,53,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
    <h4 style="color: #ff6b35; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">📐 Feature Line Width & Binarized Matrix</h4>
    <p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">Reports the number of currently triggered sensors (line width — useful for detecting intersections, T-junctions, and right angles), as well as the 0/1 binarized trigger state matrix of all 16 sensors for fine trajectory reconstruction.</p>
  </div>
  <div style="background: rgba(0,210,255,0.05); border: 1px solid rgba(0,210,255,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
    <h4 style="color: #00d2ff; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🛡️ Boundary Defense & Anti-Derailment Memory</h4>
    <p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">The system continuously monitors the physical light intensity of the leftmost and rightmost sensors, instantly detecting track splits or the robot's nose accidentally going off-track. Combined with a directional memory algorithm, it enables high-speed cornering without losing the line!</p>
  </div>
</div>

<style>
  @media (max-width: 768px) {
    div[style*="grid-template-columns: repeat(2, 1fr)"] {
      grid-template-columns: 1fr !important;
    }
  }
</style>

---

## 🚦 Hardware Indicator LEDs & Button

To ensure excellent visual readability across all theme backgrounds, the sensor features a high-contrast, color-coded status dashboard. **Note: The heartbeat LED and the calibration LED are two physically separate, independent indicators!**

<div style="display: flex; flex-direction: column; gap: 12px; margin: 20px 0;">
  <div style="display: flex; align-items: center; gap: 15px; background: rgba(0, 255, 100, 0.06); border: 1px solid rgba(0, 255, 100, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(0,255,100,0.5)); line-height: 1;">🟢</span>
    <div style="flex: 1;">
      <strong style="color: #00ff64; font-size: 1.05rem;">Green Heartbeat LED (near the socket)</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        <strong>Heartbeat Blinking</strong>: Flashes once per second — indicates the sensor is powered and the system is operating normally.
      </div>
    </div>
  </div>

  <div style="display: flex; align-items: center; gap: 15px; background: rgba(0, 255, 100, 0.06); border: 1px solid rgba(0, 255, 100, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(0,255,100,0.5)); line-height: 1;">🟢</span>
    <div style="flex: 1;">
      <strong style="color: #00ff64; font-size: 1.05rem;">Green Calibration LED (near the calibration button)</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        <strong>Calibration Steady-On</strong>: Stays lit for 5 seconds after holding the button for 0.5 sec — indicates calibration is in progress. Turns off when calibration completes successfully.
      </div>
    </div>
  </div>

  <div style="display: flex; align-items: center; gap: 15px; background: rgba(255, 50, 50, 0.06); border: 1px solid rgba(255, 50, 50, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(255,50,50,0.5)); line-height: 1;">🔴</span>
    <div style="flex: 1;">
      <strong style="color: #ff4a4a; font-size: 1.05rem;">Red LED (Communication Debug Indicator)</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        <strong>Normal state</strong>: Off.<br>
        <strong>Red LED steady-on</strong>: Indicates communication failure with the hub. Check that the cable is firmly connected, or try a different hub port / replacement cable.
      </div>
    </div>
  </div>

  <div style="display: flex; align-items: center; gap: 15px; background: rgba(242, 194, 0, 0.06); border: 1px solid rgba(242, 194, 0, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(242,194,0,0.5)); line-height: 1;">🔘</span>
    <div style="flex: 1;">
      <strong style="color: #f2c200; font-size: 1.05rem;">One-Click Calibration Button</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        Hold for <strong>0.5 seconds</strong> then release to enter automatic sweep calibration mode (green calibration LED stays on for 5 seconds).
      </div>
    </div>
  </div>

  <div style="display: flex; align-items: center; gap: 15px; background: rgba(0, 210, 255, 0.06); border: 1px solid rgba(0, 210, 255, 0.25); border-radius: 8px; padding: 12px 15px;">
    <span style="font-size: 1.5rem; filter: drop-shadow(0 0 5px rgba(0,210,255,0.5)); line-height: 1;">💡</span>
    <div style="flex: 1;">
      <strong style="color: #00d2ff; font-size: 1.05rem;">Onboard 16-Channel Status LED Array</strong>
      <div style="color: #ddd; font-size: 0.95rem; margin-top: 4px; line-height: 1.5;">
        <strong>Real-time trigger display</strong>: LED on = that channel detects a black line (black-line tracking mode). Provides an extremely intuitive view of the black line's absolute position relative to each channel from below.
      </div>
    </div>
  </div>
</div>

---

## ⚙️ General Tips

### 🛠️ One-Click Auto Sweep Calibration

To get the best comparison gain from all 16 photoelectric sensors, this product provides a simple and efficient auto-calibration system:
*   **Trigger**: Hold the **🔘 Calibration Button for 0.5 seconds** until the **🟢 Green Calibration LED (near the calibration button)** stays steadily lit.
*   **Procedure**: Within the **5-second window** while the green LED is on, sweep the robot's front across the black line and white ground area (make sure all sensors cross both the black line and the white surface).
*   **Completion**: After 5 seconds, the **🟢 Green Calibration LED** turns off — calibration is complete, and the data is automatically saved to non-volatile memory asynchronously.

### 🎨 Switch Between Black Line and White Line Tracking
While the sensor is powered on, **hold the calibration button and then release** to toggle the tracking mode:
Default is **Black Line Mode** (tracking dark lines). After switching, it becomes **White Line Mode** (tracking light lines).

> **💡 Data Polarity Auto-Sync**: After switching tracking mode, the sensor's output reflection intensity and high-precision continuous position data **also automatically flip in polarity**. This means your program logic (e.g., PID line-following algorithm) requires absolutely zero modifications — it just works!
> This setting persists until the next power cycle. After a power cycle, the sensor resets to the default black-line tracking mode.

---

## ❓ Frequently Asked Questions (FAQ)

- **❓ Sensor plugged in but no response?**
  → First check whether the **🟢 Green Heartbeat LED (near the socket)** is blinking. If there is no blinking at all, follow these steps in order:
  1. Check that both ends of the connection cable are firmly plugged in.
  2. Try a different port on the hub.
  3. Swap to a different cable for cross-testing.
  4. **Test the sensor on a different hub** to rule out a faulty port on your original hub.

- **❓ Robot wobbles while following the line?**
  → The most common cause: **not yet calibrated**, or calibration did not succeed. Return to the calibration steps and recalibrate.
  *If already calibrated but still wobbling, check:*
  1. Is the sensor height between **0.5 and 1.2 cm** above the ground?

- **❓ Calibrated but still can't detect the line?**
  → Make sure calibration covered both "the darkest black line" and "the whitest white area". Check that lighting is even and that sensor height is 0.5 to 1.2 cm.

- **❓ Different apps return different values?**
  → The Education Edition (yellow hub) and Home Edition (teal hub) return data in different formats. Confirm which version you are using and refer to the corresponding platform documentation page.

- **❓ Red LED is staying on — what do I do?**
  → **🔴 Red LED steady-on** means communication with the hub has failed. Check the cable connection, try a different port, or swap the cable. If the red LED remains on after changing ports, cables, and hubs, please contact our technical support.

---

### 🌐 Dedicated Online Support

If you've tried all the above steps and still can't resolve the issue, please contact us anytime! To help our engineers pinpoint the problem as quickly as possible, please prepare the following:
1. **📸 Video/Photo**: Record a short clip of your **robot running or the sensor indicator LED status** (so we can visually verify the signal lights and physical operation).
2. **💻 Program**: Provide a screenshot of your **block program, project file (.lms / .ev3), or source code**.
3. **💬 Contact Us**: Send the above to our technical support email, or directly message us via the official LINE support.

Our professional R&D engineers will respond immediately to assist with online diagnosis and program tuning — fully committed to powering your success in every competition!
