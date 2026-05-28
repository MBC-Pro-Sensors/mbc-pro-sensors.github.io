# pyBricks for SPIKE Prime / Robot Inventor

[🔙 Back to SPIKE Hubs](/en/sensors/line16/spike-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(0,210,255,0.03); border: 1px solid rgba(0,210,255,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/inventor-hub.webp" alt="MINDSTORMS Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/spike-pybricks-logo.webp" alt="pyBricks" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.25));" />
  </div>
</div>

> [!WARNING] 
> **Please Note: You are currently viewing the tutorial for the 【pyBricks Zone (Dual Hubs)】 MicroPython high-level development environment.**
> If you are using EV3 or other official system software, please click the button above to return to the hub menu and reselect.

This page applies to users who have installed the pyBricks third-party MicroPython firmware on the **LEGO® Education SPIKE™ Prime Set** or **LEGO® MINDSTORMS® Robot Inventor**.

In the Pybricks development environment, users can utilize Python commands for high-speed, stable communication. This sensor supports simple reading with built-in block commands, or importing our exclusive library functions to obtain high-resolution data and richer control functions.

---

## 🚀 Reading Methods and Core Modes

In the Pybricks development environment, this line follower provides two flexible data reading channels to adapt to different project needs:

1. **Built-in Sensor Commands (Read as a Color Sensor)**: Directly use Pybricks' built-in `ColorSensor` commands to read basic photoelectric or feature data, without loading extra code, for the most intuitive operation.
2. **Import Exclusive Library Functions**: By importing the Python library functions specially written by us for the 16-Way Line Follower, you can directly obtain high-resolution interpolated positions, complete 16-channel reflected light values, and feature line widths, providing richer high-level line-following control.

---

## [📥 Example Programs and Library Download (Packaged Download)](../examples/line16/line16_pybricks_spike.zip)

We have provided four core example programs and exclusive library functions. Please choose the download according to your development mode:

<!-- ========== Category 1: Built-in Sensor Commands ========== -->
<div class="category-section" style="margin: 30px 0 40px 0;">
  <div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(0,210,255,0.08), rgba(0,210,255,0.02)); border: 1px solid rgba(0,210,255,0.25); border-radius: 10px;">
    <span style="font-size: 1.6rem;">⚡</span>
    <div>
      <h3 style="margin: 0; color: #00d2ff; font-size: 1.2rem; font-weight: bold;">Method 1: Built-in Sensor Commands</h3>
      <p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">Directly use Pybricks' built-in commands to read the sensor, no extra code needed, quick start.</p>
    </div>
  </div>
  <div style="margin-bottom: 25px;">
    <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 15px;">
      <h4 style="margin: 0; color: #00d2ff; font-size: 1rem;">📋 Available Functions Overview</h4>
      <span style="font-size: 0.75rem; color: #888;">— Calculated via Pybricks' built-in <code style="background: rgba(0,210,255,0.1); padding: 1px 5px; border-radius: 3px; font-size: 0.75rem;">ColorSensor</code>, no extra library files to load</span>
    </div>
    <!-- 🎯 Basic Combination -->
    <div style="margin-bottom: 16px;">
      <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 8px; padding-left: 2px;">
        <span style="font-size: 0.85rem; color: #00d2ff; font-weight: bold;">🎯 Basic Combination</span>
        <span style="font-size: 0.72rem; color: #777; border-left: 1px solid rgba(255,255,255,0.1); padding-left: 8px;">Out of the box, no calibration needed, suitable for quick development and beginners</span>
      </div>
      <div style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">getLinePos16()</code>
            <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-16 ~ +16</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get line position (integer resolution). <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">0</code> is dead center, negative is left, positive is right. Suitable for basic PID line following.</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">getLineWidth16()</code>
            <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 16</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get currently detected feature line width (number of triggered sensors). Can be used to detect intersections, T-junctions, and other wide-line features.</p>
        </div>
      </div>
    </div>
    <!-- 🔬 High-Precision Combination -->
    <div>
      <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 8px; padding-left: 2px;">
        <span style="font-size: 0.85rem; color: #00d2ff; font-weight: bold;">🔬 High-Precision Combination</span>
        <span style="font-size: 0.72rem; color: #777; border-left: 1px solid rgba(255,255,255,0.1); padding-left: 8px;">Recommended to complete sensor calibration first; the more precise the calibration, the more accurate the values</span>
      </div>
      <div style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">getLinePos100()</code>
            <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-100 ~ +100</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get high-resolution sub-pixel interpolated position. Accuracy is much higher than <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">getLinePos16()</code>, suitable for high-end PID precision line following.</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">getLineWidth100()</code>
            <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 16</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get feature line width in high-precision mode. Use in combination with <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">getLinePos100()</code> to ensure data synchronization.</p>
        </div>
      </div>
    </div>
    <div style="margin-top: 12px; padding: 10px 14px; background: rgba(0,210,255,0.04); border-left: 3px solid rgba(0,210,255,0.3); border-radius: 0 6px 6px 0; font-size: 0.78rem; color: #999; line-height: 1.5;">
      💡 <strong style="color: #bbb;">Usage Tip:</strong> Please choose position and line width functions from the same set to use together. For example, if you use <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.75rem;">getLinePos16()</code>, pair it with <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.75rem;">getLineWidth16()</code>; if you use <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.75rem;">getLinePos100()</code>, pair it with <code style="background: rgba(0,210,255,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.75rem;">getLineWidth100()</code> to obtain the most real-time and accurate reading results.
    </div>
  </div>
  <div class="pybricks-download-grid" style="display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 25px;">
    <!-- Card 1: Block Native -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line16/pybricks-block-native.webp" alt="Block Mode (Built-in Commands)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #00d2ff; font-size: 1.15rem; font-weight: bold;">🧩 Block Mode (Built-in)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">Use Pybricks' built-in graphical block commands to read the sensor, suitable for beginners to quickly learn basic line-following logic.</p>
      </div>
      <a href="../examples/line16/pybricks/line16_block_native.py" target="_blank" download="line16_block_native.py" class="btn-download" style="display: block; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 10px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.9rem; box-shadow: 0 4px 15px rgba(0,210,255,0.25); transition: transform 0.2s ease;">📥 Download Example (.py)</a>
    </div>
    <!-- Card 2: Python Native -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line16/pybricks-python-native.webp" alt="Python Mode (Built-in Commands)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #00d2ff; font-size: 1.15rem; font-weight: bold;">🐍 Python Mode (Built-in)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">Use Pybricks' native MicroPython built-in sensor commands to read directly, balancing program flexibility and pure text development experience.</p>
      </div>
      <a href="../examples/line16/pybricks/line16_python_native.py" target="_blank" download="line16_python_native.py" class="btn-download" style="display: block; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 10px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.9rem; box-shadow: 0 4px 15px rgba(0,210,255,0.25); transition: transform 0.2s ease;">📥 Download Example (.py)</a>
    </div>
  </div>
</div>

<!-- ========== Category 2: Import Exclusive Library Functions ========== -->
<div class="category-section" style="margin: 40px 0 30px 0;">
  <div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(10,186,181,0.08), rgba(10,186,181,0.02)); border: 1px solid rgba(10,186,181,0.25); border-radius: 10px;">
    <span style="font-size: 1.6rem;">🔬</span>
    <div>
      <h3 style="margin: 0; color: #0abab5; font-size: 1.2rem; font-weight: bold;">Method 2: Import Exclusive Library Functions</h3>
      <p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">By importing our exclusive Python library functions, obtain advanced features like high-resolution interpolated position, full 16-channel data, and line width.</p>
    </div>
  </div>
  <div style="margin-bottom: 25px;">
    <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 15px;">
      <h4 style="margin: 0; color: #0abab5; font-size: 1rem;">📋 Available Functions Overview</h4>
      <span style="font-size: 0.75rem; color: #888;">— Provided by <code style="background: rgba(10,186,181,0.1); padding: 1px 5px; border-radius: 3px; font-size: 0.75rem;">MBC_line16_Lib.py</code></span>
    </div>
    <!-- ⚙️ Initialization -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">⚙️ Initialization</div>
      <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px; flex-wrap: wrap; gap: 6px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_init_port_multitask(port, multitask)</code>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Initialize the sensor port. <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">port</code>=1~6 corresponding to A~F; <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">multitask</code>=True to enable asynchronous mode (use with <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">await</code>).</p>
        </div>
      </div>
    </div>
    <!-- 📍 Position and Line Width -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">📍 Position and Line Width</div>
      <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(220px, 1fr)); gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <div>
              <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_pos16()</code>
              <span style="font-size: 0.7rem; color: #888; margin-left: 6px;">🎯 Basic No Calib</span>
            </div>
            <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-16 ~ +16</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get integer-resolution line position. <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">0</code> is dead center, negative is left, positive is right. <strong style="color: #bbb;">Out of the box, no calibration needed, suitable for quick development and beginners.</strong></p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <div>
              <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_pos100()</code>
              <span style="font-size: 0.7rem; color: #888; margin-left: 6px;">🔬 Needs Calib</span>
            </div>
            <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-100 ~ +100</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get high-resolution sub-pixel interpolated position, the best PID line-following choice. <strong style="color: #bbb;">The prerequisite is that the sensor calibration must be fully completed; the more accurate the calibration, the more precise the values.</strong></p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_width()</code>
            <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 16</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get feature line width (number of triggered sensors). Can be used to detect intersections and wide-line features, <strong style="color: #bbb;">applicable to both position functions above.</strong></p>
        </div>
      </div>
    </div>
    <!-- 🔦 16-Channel Infrared Light Values -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🔦 16-Channel Infrared Light Values</div>
      <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(240px, 1fr)); gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_ir_calibration()</code>
            <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">list[16]</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Return the array of 16 calibrated infrared reflected light values (each value 0~15). index 0 = rightmost (ch1), index 15 = leftmost (ch16).</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_ir_ch(ch)</code>
            <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 15</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get the calibrated light value of a single channel. <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">ch</code>=1~16, can be called continuously with high efficiency (shared cache).</p>
        </div>
      </div>
    </div>
    <!-- 🔀 Intersection Detection and Binary Data -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🔀 Intersection Detection & Binary Data</div>
      <div style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_junction()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0, 1, 2+</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Return the number of detected line groups. <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">0</code>=no line, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">1</code>=single line, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">2+</code>=intersection branch.</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_junction_name()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">str</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Return text description of intersection status: <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">"none"</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">"line"</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">"junction"</code>.</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_bin_raw()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 65535</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get the 16-bit raw trigger mask. Each bit corresponds to one sensor channel (bit0=ch1), 1=line detected.</p>
        </div>
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_on_sensors()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">list[int]</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Return the list of currently triggered channel numbers (1~16), e.g. <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">[4, 5]</code>.</p>
        </div>
      </div>
    </div>
    <!-- 📦 Read All at Once -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">📦 Read All at Once</div>
      <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_read_all()</code>
            <span style="background: rgba(156,39,176,0.15); color: #ce93d8; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">dict</span>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Read all data at once, return a dictionary containing all fields: <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">ir_calibration</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">line_pos16</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">line_pos100</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">line_width</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">bin_raw</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">on_sensors</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">junction</code>, <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">junction_name</code>.</p>
        </div>
      </div>
    </div>
  </div>
  <div class="pybricks-download-grid" style="display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 25px;">
    <!-- Card 3: Block with Lib -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line16/pybricks-block-with-lib.webp" alt="Block Mode (Import Exclusive Library)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #0abab5; font-size: 1.15rem; font-weight: bold;">🧩 Block Mode (Import Library)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">Graphical blocks combined with the exclusive library functions we developed for you, allowing easy access to high-resolution and high-precision values within block development.</p>
      </div>
      <a href="../examples/line16/pybricks/line16_block_with_lib.py" target="_blank" download="line16_block_with_lib.py" class="btn-download" style="display: block; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 10px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.9rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 Download Example (.py)</a>
    </div>
    <!-- Card 4: Python with Lib -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line16/pybricks-python-with-lib.webp" alt="Python Mode (Import Exclusive Library)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #0abab5; font-size: 1.15rem; font-weight: bold;">🐍 Python Mode (Import Library)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">The best solution for advanced competitors! Import the exclusive library for the most underlying complete data calls and high-frequency PID line-following algorithm development.</p>
      </div>
      <a href="../examples/line16/pybricks/line16_python_with_lib.py" target="_blank" download="line16_python_with_lib.py" class="btn-download" style="display: block; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 10px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.9rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 Download Example (.py)</a>
    </div>
  </div>

  <!-- Library Card (Full Width Highlighted Card) -->
  <div class="library-download-box" style="margin-top: 25px; background: rgba(10,186,181,0.03); border: 2px solid rgba(10,186,181,0.3); border-radius: 12px; padding: 25px; box-shadow: 0 15px 35px rgba(0,0,0,0.4); transition: all 0.3s ease;">
    <h3 style="margin: 0 0 15px 0; color: #0abab5; font-size: 1.3rem; font-weight: bold; text-align: center;">📦 Exclusive Pybricks Library Function for 16-Way Line Follower (Library File)</h3>
    <div style="display: flex; flex-wrap: wrap; gap: 25px; align-items: stretch;">
      <!-- Left: Library image and download button -->
      <div style="flex: 1; min-width: 260px; display: flex; flex-direction: column; justify-content: space-between;">
        <div>
          <div style="border-radius: 8px; overflow: hidden; border: 1px solid rgba(10,186,181,0.2); background: #000; margin-bottom: 15px;">
            <img src="../images/sensors/line16/pybricks-lib-icon.webp" alt="Exclusive Library Function" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85;" />
          </div>
          <p style="font-size: 0.85rem; color: #ccc; line-height: 1.6; margin-bottom: 15px;">
            This is the indispensable driver core for the "Import Exclusive Library" examples. Download this library file to perfectly enable high-level interpolated position and 16-channel photoelectric fine reading.
          </p>
        </div>
        <a href="../examples/line16/pybricks/MBC_line16_Lib.py" target="_blank" download="MBC_line16_Lib.py" class="btn-download-lib" style="display: block; text-align: center; padding: 12px 20px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 1rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 Download Exclusive Library (MBC_line16_Lib.py)</a>
      </div>
      <!-- Right: Important tips and screenshots -->
      <div style="flex: 1.2; min-width: 280px; display: flex;">
        <div style="background: rgba(255,107,53,0.08); border-left: 4px solid #ff6b35; border-radius: 0 8px 8px 0; padding: 15px 20px; flex-grow: 1;">
          <h4 style="margin: 0 0 8px 0; color: #ff6b35; font-size: 1.05rem; display: flex; align-items: center; gap: 6px;">⚠️ Important Tip: File Placement</h4>
          <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #ddd; line-height: 1.6;">
            You <strong>must place this library file (<code style="background: rgba(255,255,255,0.1); padding: 1px 4px; border-radius: 3px;">MBC_line16_Lib.py</code>) in the same project directory as your main program</strong>. Otherwise, the program will report a file not found error when executing!
          </p>
          <div style="border-radius: 6px; overflow: hidden; border: 1px solid rgba(255,107,53,0.3); background: #000; box-shadow: 0 4px 15px rgba(0,0,0,0.2); width: 50%; margin: 0 auto;">
            <img src="../images/sensors/line16/pybricks-upload-lib.webp" alt="File Placement" style="width: 100%; display: block; opacity: 0.9;" />
          </div>
        </div>
      </div>
    </div>
  </div>
</div>

<style>
  .download-card:hover {
    transform: translateY(-5px);
    border-color: rgba(0,210,255,0.4) !important;
    box-shadow: 0 15px 35px rgba(0,210,255,0.15) !important;
    background: rgba(255,255,255,0.04) !important;
  }
  .download-card:hover img {
    opacity: 1 !important;
  }
  .library-download-box:hover {
    border-color: rgba(10,186,181,0.5) !important;
    box-shadow: 0 20px 45px rgba(10,186,181,0.2) !important;
    background: rgba(10,186,181,0.05) !important;
  }
  .btn-download:hover, .btn-download-lib:hover {
    transform: scale(1.03);
  }
  .btn-download:active, .btn-download-lib:active {
    transform: scale(0.98);
  }
  .func-card:hover {
    transform: translateY(-3px);
    border-color: rgba(0,210,255,0.35) !important;
    box-shadow: 0 8px 25px rgba(0,210,255,0.12) !important;
    background: rgba(255,255,255,0.04) !important;
  }
</style>
