# ∿ Universal I2C Version Development Guide

[🔙 Back to 8-Way Line Follower Home](/en/sensors/line8/index.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 25px 45px; display: flex; align-items: center; gap: 25px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/sensors/line8/line8-product-arduino.webp" alt="Pathfinder 8-Way Sensor (Universal I2C Edition)" style="max-height: 144px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(10,186,181,0.75)) drop-shadow(0 0 45px rgba(10,186,181,0.45));" />
    <span style="font-size: 2.5rem; color: #777; font-weight: 300; line-height: 1;">+</span>
    <div style="display: flex; gap: 15px; align-items: center;">
      <svg viewBox="0 0 100 40" style="height: 60px; fill: none; stroke: #00d2ff; stroke-width: 4; stroke-linecap: round; stroke-linejoin: round; filter: drop-shadow(0 0 10px rgba(0, 210, 255, 0.4));">
        <path d="M 5,25 H 25 V 5 H 45 V 35 H 65 V 5 H 85 V 35 H 95" />
      </svg>
    </div>
  </div>
</div>

Welcome to the professional hardware development zone! In addition to perfectly supporting the LEGO ecosystem, the MBC 8-Way Line Follower also provides an open, standard I2C communication interface for developers using **Arduino, ESP32, Raspberry Pi, and various custom master boards**. 
The read and write registers of this sensor are completely separated, utilizing a conflict-free design, making it highly suitable for developing high-speed line-following robots or PC visualization dashboards.

---

## 1. Universal MCU Platforms (Arduino / ESP32 / Raspberry Pi)

*   **Communication Protocol**: Standard I2C
*   **Default Slave Address**: `0x08`

### 📥 Write Registers (I2C Write - Control Commands)
Write format: `[Slave Address 0x08] + [Register Address] + [Value]`

<div style="display: grid; grid-template-columns: 1fr; gap: 15px; margin-bottom: 30px;">
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(255,107,53,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<code style="color: #ff6b35; font-size: 1.1rem; font-weight: bold;">0x10</code>
<span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">Toggle Target Line Mode</span>
</div>
<ul style="margin: 0; padding-left: 20px; font-size: 0.85rem; color: #aaa; line-height: 1.6;">
<li>Write <strong><code>0</code></strong>: Follow Black Line mode.</li>
<li>Write <strong><code>1</code></strong>: Follow White Line mode.</li>
</ul>
</div>
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(255,107,53,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<code style="color: #ff6b35; font-size: 1.1rem; font-weight: bold;">0x20</code>
<span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">Trigger Calibration Command</span>
</div>
<div style="font-size: 0.85rem; color: #aaa; line-height: 1.6;">
<div style="color: #0abab5; font-weight: bold; margin-bottom: 4px;">⚡ Instant Calibration Method (No robot movement required):</div>
<ul style="margin: 0 0 8px 0; padding-left: 20px;">
<li>Write <strong><code>1</code></strong>: Start 5-second dynamic calibration (requires moving the robot over the track).</li>
<li>Write <strong><code>2</code></strong>: Trigger "Static Single-Step White Calibration" 👉 Records the current view as the white extreme value (robot must be parked on the white ground).</li>
<li>Write <strong><code>3</code></strong>: Trigger "Static Single-Step Black Calibration" 👉 Records the current view as the black extreme value (robot must be positioned over the black line).</li>
</ul>
<div style="font-size: 0.75rem; color: #888;">*(Note: Writing 2 and 3 triggers underlying asynchronous EEPROM writes and gain recalculations)*</div>
</div>
</div>
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(255,107,53,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<code style="color: #ff6b35; font-size: 1.1rem; font-weight: bold;">0x30</code>
<span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">Dynamic Threshold (LSA Threshold)</span>
</div>
<p style="margin: 0; font-size: 0.85rem; color: #aaa; line-height: 1.6;">Write <strong><code>0 ~ 100</code></strong> (Default is <code>50</code>). A higher value makes it easier to judge as a black line. This can be adjusted in real-time on the web control panel.</p>
</div>
</div>

### 📤 Read Registers (I2C Read - Getting Data)
Read format: First I2C Write to the desired `[Register Address]` (do not send Stop), then execute `I2C RequestFrom` for the corresponding byte length.

<div style="display: grid; grid-template-columns: 1fr; gap: 15px; margin-bottom: 30px;">
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<div style="display: flex; align-items: center; gap: 10px;">
<code style="color: #00d2ff; font-size: 1.1rem; font-weight: bold;">0x01</code>
<span style="background: rgba(0,210,255,0.15); color: #00d2ff; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">Core Comprehensive Feature Pack</span>
</div>
<span style="background: rgba(255,255,255,0.1); color: #ccc; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.75rem;">4 Bytes</span>
</div>
<ul style="margin: 0; padding-left: 20px; font-size: 0.85rem; color: #aaa; line-height: 1.6;">
<li><strong>Byte 0</strong>: Standard position shift value (<code>linePos + 8</code>), range <code>0 ~ 16</code>. (8 is dead center)</li>
<li><strong>Byte 1</strong>: Feature line width (<code>lineWidth</code>), range <code>0 ~ 8</code>.</li>
<li><strong>Byte 2</strong>: High-resolution smooth position (<code>linePosHighResolution</code>), range <code>0 ~ 200</code> (100 is center).</li>
<li><strong>Byte 3</strong>: Binarized raw image status (<code>binRaw</code> full 8 bits).</li>
</ul>
</div>
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<div style="display: flex; align-items: center; gap: 10px;">
<code style="color: #00d2ff; font-size: 1.1rem; font-weight: bold;">0x02</code>
<span style="background: rgba(0,210,255,0.15); color: #00d2ff; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">Panoramic Absolute Physical Light Values</span>
</div>
<span style="background: rgba(255,255,255,0.1); color: #ccc; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.75rem;">8 Bytes</span>
</div>
<div style="font-size: 0.85rem; color: #aaa; line-height: 1.6;">
Read back the precisely calibrated physical light values of all 8 sensor channels at once (<code>dataIrCalib[0~7]</code>).<br>
Each channel ranges from <code>0 ~ 100</code> (100=pure white, 0=pure black).<br>
<span style="color: #0abab5; font-weight: bold;">Highly recommended for developing real-time bar chart dashboards!</span>
</div>
</div>
<div class="reg-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
<div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
<div style="display: flex; align-items: center; gap: 10px;">
<code style="color: #00d2ff; font-size: 1.1rem; font-weight: bold;">0x40 ~ 0x47</code>
<span style="background: rgba(0,210,255,0.15); color: #00d2ff; padding: 3px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem;">Single Channel Quick Read</span>
</div>
<span style="background: rgba(255,255,255,0.1); color: #ccc; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.75rem;">1 Byte</span>
</div>
<p style="margin: 0; font-size: 0.85rem; color: #aaa; line-height: 1.6;">Read the light value of a specified single channel (<code>0 ~ 100</code>). <code>0x40</code> corresponds to channel 0 (far right), <code>0x47</code> corresponds to channel 7 (far left).</p>
</div>
</div>

<style>
.reg-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3);
  background: rgba(255,255,255,0.04) !important;
}
</style>

### 💻 Arduino C++ Code Example

> [!NOTE]
> **⏳ Complete Code Example is in Preparation**
> Arduino C++ read/write example programs and related wiring diagrams utilizing the `ARDUINO_IIC` exclusive bidirectional control register system will be added later. Stay tuned!
