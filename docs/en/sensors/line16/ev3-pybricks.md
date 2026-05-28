# pyBricks for EV3

[🔙 Back to EV3 Hubs](/en/sensors/line16/ev3-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(0,210,255,0.03); border: 1px solid rgba(0,210,255,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/ev3-hub.webp" alt="EV3 Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/ev3-pybricks-logo.webp" alt="EV3 pyBricks" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.25));" />
  </div>
</div>

> [!WARNING] 
> **Please Note: You are currently viewing the tutorial for the 【EV3 pyBricks】 environment.**
> If you are using SPIKE or other EV3 software, please click the button above to return to the hub menu and reselect.

Applicable to users who have installed the pyBricks third-party firmware on the **LEGO® MINDSTORMS® EV3** hub. In the high-level Pybricks MicroPython environment, players can completely bypass official communication and mode limits, and communicate with the 16-Way Line Follower at high speed in the most concise and efficient way!

> [!WARNING]
> **🧪 Beta Version Notice**
> The version of the pyBricks software we tested is **v4.0.0b10 (Pybricks Beta v3.0.0-beta.7)**. Please be especially careful and note: currently, only the **Beta version** provides firmware for EV3 installation!

> [!IMPORTANT]
> **⚠️ Mode Switching Delay Warning**
> Due to the underlying communication mechanism limits of the EV3 hardware, when the program switches between different underlying reading modes, **there will still be a communication waiting delay**.
> **Strong Recommendation**: To ensure program stability and high-speed response, please choose one mode to use based on your task needs, **and do not frequently cross-call functions of different modes in the main loop** (the functions below have been marked with their mode group for you).

---

## 🚀 Reading Methods and Core Modes

In the EV3 Pybricks environment, this line follower provides two flexible data reading channels to adapt to different project needs:

1. **Native Command Reading (`device.mode`)**: Directly switch the sensor's underlying UART mode to get array data, which is the most direct operation.
2. **Import Exclusive Library Functions**: By importing our encapsulated Python functions, you can obtain independent variable returns as intuitive as blocks, greatly simplifying code logic.

---

## [📥 Example Programs and Functions Download (Packaged Download)](../examples/line16/line16_pybricks_ev3.zip)

We have provided core example programs and exclusive library functions. Please choose according to your development habits:

<!-- ========== Method 1: Native Command Reading ========== -->
<div class="category-section" style="margin: 30px 0 40px 0;">
  <div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(0,210,255,0.08), rgba(0,210,255,0.02)); border: 1px solid rgba(0,210,255,0.25); border-radius: 10px;">
    <span style="font-size: 1.6rem;">⚡</span>
    <div>
      <h3 style="margin: 0; color: #00d2ff; font-size: 1.2rem; font-weight: bold;">Method 1: Native Command Reading</h3>
      <p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">Use Pybricks' underlying <code>hub.port.X.device.mode(mode)</code> to directly get array data.</p>
    </div>
  </div>
  <div style="margin-bottom: 25px;">
    <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(240px, 1fr)); gap: 12px;">
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">device.mode(0)</code>
          <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">Tuple (1 Byte)</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;"><strong>Beginner Mode</strong>: Returns standard position offset <code>[-16, 16]</code>, 0 is center.</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">device.mode(1)</code>
          <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">Tuple (8 Bytes)</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;"><strong>Full View Debug Mode</strong>: Returns 16-channel compressed light values. Two adjacent channels are compressed into a two-digit number (<code>11~99</code>).</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #00d2ff; font-size: 0.95rem; font-weight: bold;">device.mode(2)</code>
          <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">Tuple (4 Bytes)</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;"><strong>Competitive Tactical Weapon</strong>: Returns high-precision position <code>[-100, 100]</code>, line width <code>[0, 16]</code>, left boundary physical light value, and right boundary physical light value.</p>
      </div>
    </div>
  </div>
  <div class="pybricks-download-grid" style="display: grid; grid-template-columns: 1fr; gap: 25px;">
    <div style="padding: 40px 20px; text-align: center; background: rgba(255,255,255,0.02); border: 1px dashed rgba(0,210,255,0.3); border-radius: 12px;">
      <span style="font-size: 2.5rem; display: block; margin-bottom: 15px; opacity: 0.8;">⏳</span>
      <h4 style="color: #00d2ff; margin: 0 0 10px 0; font-size: 1.15rem; font-weight: bold;">Waiting for Pybricks Official Command Update</h4>
      <p style="color: #aaa; font-size: 0.9rem; margin: 0; line-height: 1.6;">Because official graphics and underlying commands updates are still pending, native example programs in this section will be supplemented later.<br/>Currently, we strongly recommend you directly use <strong>"Method 2: Import Exclusive Library Functions"</strong> below for high-level development!</p>
    </div>
  </div>
</div>

<!-- ========== Method 2: Import Exclusive Library Functions ========== -->
<div class="category-section" style="margin: 40px 0 30px 0;">
  <div class="category-header" style="display: flex; align-items: center; gap: 12px; margin-bottom: 20px; padding: 14px 22px; background: linear-gradient(135deg, rgba(10,186,181,0.08), rgba(10,186,181,0.02)); border: 1px solid rgba(10,186,181,0.25); border-radius: 10px;">
    <span style="font-size: 1.6rem;">🔬</span>
    <div>
      <h3 style="margin: 0; color: #0abab5; font-size: 1.2rem; font-weight: bold;">Method 2: Import Exclusive Library Functions</h3>
      <p style="margin: 4px 0 0 0; font-size: 0.82rem; color: #999; line-height: 1.4;">By importing encapsulated Python subroutines, you get independent variable returns, greatly reducing code complexity.</p>
    </div>
  </div>
  <div style="margin-bottom: 25px;">
    <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 15px;">
      <h4 style="margin: 0; color: #0abab5; font-size: 1rem;">📋 Available Functions Overview</h4>
      <span style="font-size: 0.75rem; color: #888;">— Provided by <code style="background: rgba(10,186,181,0.1); padding: 1px 5px; border-radius: 3px; font-size: 0.75rem;">MBC_line16_EV3_Lib.py</code></span>
    </div>
    <!-- ⚙️ Initialization -->
    <div style="margin-bottom: 12px;">
      <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">⚙️ Initialization</div>
      <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
        <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
          <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px; flex-wrap: wrap; gap: 6px;">
            <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;">line_init_port_multitask(port, multitask)</code>
          </div>
          <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5; margin-bottom: 15px;">Initialize the sensor port. <code style="background: rgba(10,186,181,0.1); padding: 1px 4px; border-radius: 3px; font-size: 0.78rem;">port</code> passes the EV3 port object (e.g., <code>hub.port.S1</code>).<br>
          👉 <strong>Non-multitask mode</strong>: The second parameter <code>multitask</code> should be <code>False</code>, select <strong><code>call</code></strong> when calling functions.<br>
          👉 <strong>Multitask mode</strong>: The second parameter <code>multitask</code> should be <code>True</code>, select <strong><code>await</code></strong> when calling functions.</p>
          
          <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 15px;">
            <!-- Non-multitask State -->
            <div style="background: rgba(0,0,0,0.2); border: 1px solid rgba(255,255,255,0.1); border-radius: 8px; padding: 12px; text-align: center;">
              <div style="color: #ccc; font-size: 0.85rem; font-weight: bold; margin-bottom: 10px; display: flex; align-items: center; justify-content: center; gap: 6px;">
                <span style="display: inline-block; width: 8px; height: 8px; background: #ff6b35; border-radius: 50%;"></span>
                Non-multitask State Settings (Normal use)
              </div>
              <img src="../images/sensors/line16/ev3-pybricks-non-multitask.webp" alt="Non-multitask state settings" style="width: 100%; border-radius: 6px; border: 1px solid rgba(255,107,53,0.2);" />
            </div>
            
            <!-- Multitask State -->
            <div style="background: rgba(0,0,0,0.2); border: 1px solid rgba(255,255,255,0.1); border-radius: 8px; padding: 12px; text-align: center;">
              <div style="color: #ccc; font-size: 0.85rem; font-weight: bold; margin-bottom: 10px; display: flex; align-items: center; justify-content: center; gap: 6px;">
                <span style="display: inline-block; width: 8px; height: 8px; background: #0abab5; border-radius: 50%;"></span>
                Multitask State Settings (Async / await)
              </div>
              <img src="../images/sensors/line16/ev3-pybricks-multitask.webp" alt="Multitask state settings" style="width: 100%; border-radius: 6px; border: 1px solid rgba(10,186,181,0.2);" />
            </div>
          </div>
        </div>
      </div>
    </div>
  <!-- 🟢 Mode 0 -->
  <div style="margin-bottom: 12px;">
    <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🟢 Mode 0 (Beginner/Basic Line Following)</div>
    <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[Mode 0: Basic]</span>line_pos16()</code>
          <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-16 ~ +16</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get integer-resolution line position. <code>0</code> is dead center, negative is left, positive is right.</p>
      </div>
    </div>
  </div>
  <!-- 🟡 Mode 1 -->
  <div style="margin-bottom: 12px;">
    <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🟡 Mode 1 (Full View/Debug/Custom Algo)</div>
    <div style="display: grid; grid-template-columns: 1fr; gap: 12px;">
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[Mode 1: Decode]</span>line_ir_ch(ch)</code>
          <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">1 ~ 9</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get real-time quantized light value of a single sensor channel. <code>ch</code>=1~16. 1=darkest, 9=brightest.</p>
      </div>
    </div>
  </div>
  <!-- 🔵 Mode 2 -->
  <div style="margin-bottom: 12px;">
    <div style="font-size: 0.82rem; color: #0abab5; font-weight: bold; margin-bottom: 8px; padding-left: 2px;">🔵 Mode 2 (Competitive/PID Dedicated)</div>
    <div style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 12px;">
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[Mode 2: Comp]</span>line_pos100()</code>
          <span style="background: rgba(255,107,53,0.15); color: #ff6b35; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">-100 ~ +100</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">High-resolution sub-pixel interpolated position, specifically designed for high-end PID / PD control.</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[Mode 2: Comp]</span>line_width()</code>
          <span style="background: rgba(10,186,181,0.15); color: #0abab5; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 16</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get feature line width (number of triggered sensors). Can be used to detect intersections and wide lines.</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[Mode 2: Comp]</span>line_edge_l()</code>
          <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 100</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get the physical light value of the left boundary sensor (0=black, 100=white), used for derailment prevention.</p>
      </div>
      <div class="func-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(10,186,181,0.15); border-radius: 10px; padding: 16px; transition: all 0.3s ease;">
        <div style="display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px;">
          <code style="color: #0abab5; font-size: 0.95rem; font-weight: bold;"><span style="background: rgba(255,255,255,0.15); color: #fff; padding: 2px 6px; border-radius: 4px; font-size: 0.75rem; margin-right: 6px;">[Mode 2: Comp]</span>line_edge_r()</code>
          <span style="background: rgba(217,163,0,0.15); color: #d9a300; padding: 3px 10px; border-radius: 20px; font-family: monospace; font-size: 0.75rem; white-space: nowrap;">0 ~ 100</span>
        </div>
        <p style="margin: 0; font-size: 0.82rem; color: #aaa; line-height: 1.5;">Get the physical light value of the right boundary sensor (0=black, 100=white), used for derailment prevention.</p>
      </div>
    </div>
  </div>

> [!IMPORTANT]
> **⚠️ Mode Switching Delay Warning**
> Due to the underlying communication mechanism limits of the EV3 hardware, when the program switches between different underlying reading modes, **there will still be a communication waiting delay**.
> **Strong Recommendation**: To ensure program stability and high-speed response, please choose one mode to use based on your task needs, **and do not frequently cross-call functions of different modes in the main loop** (the functions above have been marked with their mode group for you).
> 
> 👉 **Practical Advice**: It is strongly recommended that general users and competitors use the **[Mode 2: Comp]** function group throughout;<br>
> Only when you need to read the raw values of all sensors for "custom special algorithm" development is it recommended to use **[Mode 1: Decode]** alone.

  </div>
  <div class="pybricks-download-grid" style="display: grid; grid-template-columns: repeat(auto-fit, minmax(280px, 1fr)); gap: 25px;">
    <!-- Card 3: Block with Lib -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line16/ev3-pybricks-block-with-lib.webp" alt="Block Mode (Import Exclusive Library)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #0abab5; font-size: 1.15rem; font-weight: bold;">🧩 Block Mode (Import Library)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">Graphical blocks combined with the exclusive library functions we developed for you, allowing easy access to high-resolution and high-precision values within block development.</p>
      </div>
      <a href="../examples/line16/ev3/line16_ev3_block_with_lib.py" target="_blank" download="line16_ev3_block_with_lib.py" class="btn-download" style="display: block; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 10px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.9rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 Download Example (.py)</a>
    </div>
    <!-- Card 4: Python with Lib -->
    <div class="download-card" style="background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.15); border-radius: 12px; padding: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); transition: all 0.3s ease; display: flex; flex-direction: column; justify-content: space-between;">
      <div>
        <div style="width: 100%; border-radius: 8px; overflow: hidden; border: 1px solid rgba(255,255,255,0.1); background: #000; margin-bottom: 15px;">
          <img src="../images/sensors/line16/ev3-pybricks-python-with-lib.webp" alt="Python Mode (Import Exclusive Library)" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.85; transition: opacity 0.3s ease;" />
        </div>
        <h3 style="margin: 0 0 10px 0; color: #0abab5; font-size: 1.15rem; font-weight: bold;">🐍 Python Mode (Import Library)</h3>
        <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #aaa; line-height: 1.5;">The best solution for advanced competitors! Import the exclusive library for the most underlying complete data calls and high-frequency PID line-following algorithm development.</p>
      </div>
      <a href="../examples/line16/ev3/line16_ev3_python_with_lib.py" target="_blank" download="line16_ev3_python_with_lib.py" class="btn-download" style="display: block; text-align: center; background: linear-gradient(135deg, #0abab5, #007a75); color: #fff; padding: 10px; border-radius: 6px; text-decoration: none; font-weight: bold; font-size: 0.9rem; box-shadow: 0 4px 15px rgba(10,186,181,0.25); transition: transform 0.2s ease;">📥 Download Example (.py)</a>
    </div>
  </div>

  <!-- Library Card (Full Width Highlighted Card) -->
  <div class="library-download-box" style="margin-top: 25px; background: rgba(10,186,181,0.03); border: 2px solid rgba(10,186,181,0.3); border-radius: 12px; padding: 25px; box-shadow: 0 15px 35px rgba(0,0,0,0.4); transition: all 0.3s ease;">
    <h3 style="margin: 0 0 15px 0; color: #0abab5; font-size: 1.3rem; font-weight: bold; text-align: center;">📦 EV3 pyBricks Exclusive Library Function (Library File)</h3>
    <div style="display: flex; flex-wrap: wrap; gap: 25px; align-items: stretch;">
      <!-- Left: Library image and download button -->
      <div style="flex: 1; min-width: 260px; display: flex; flex-direction: column; justify-content: space-between;">
        <div>
          <div style="border-radius: 8px; overflow: hidden; border: 1px solid rgba(10,186,181,0.2); background: #000; margin-bottom: 15px;">
            <img src="../images/sensors/line16/ev3-pybricks-lib-icon.webp" alt="16-Way Line Follower Exclusive Pybricks Library Function" style="width: 100%; height: 160px; object-fit: cover; opacity: 0.9;" />
          </div>
          <p style="font-size: 0.85rem; color: #ccc; line-height: 1.6; margin-bottom: 15px;">
            This is the indispensable driver core for the "Import Exclusive Library" examples. Download this library file to perfectly enable high-level interpolated position and 16-channel photoelectric fine reading.
          </p>
        </div>
        <a href="../examples/line16/ev3/MBC_line16_EV3_Lib.py" target="_blank" download="MBC_line16_EV3_Lib.py" class="btn-download-lib" style="display: block; text-align: center; padding: 12px 20px; background: linear-gradient(135deg, #0abab5, #00d2ff); color: #fff; text-decoration: none; border-radius: 8px; font-weight: bold; font-size: 1rem; box-shadow: 0 6px 20px rgba(10,186,181,0.3); transition: transform 0.2s ease, box-shadow 0.2s ease;">📥 Download Exclusive Library (MBC_line16_EV3_Lib.py)</a>
      </div>
      <!-- Right: Important tips and screenshots -->
      <div style="flex: 1.2; min-width: 280px; display: flex;">
        <div style="background: rgba(255,107,53,0.08); border-left: 4px solid #ff6b35; border-radius: 0 8px 8px 0; padding: 15px 20px; flex-grow: 1;">
          <h4 style="margin: 0 0 8px 0; color: #ff6b35; font-size: 1.05rem; display: flex; align-items: center; gap: 6px;">⚠️ Important Tip: File Placement</h4>
          <p style="margin: 0 0 15px 0; font-size: 0.85rem; color: #ddd; line-height: 1.6;">
            Please ensure you import the <code>MBC_line16_EV3_Lib.py</code> file directly into your Pybricks project, <strong>and the file name must be exactly the same</strong>, so the <code>import</code> command in the main program can load the functions correctly!
          </p>
          <div style="border-radius: 6px; overflow: hidden; border: 1px solid rgba(255,107,53,0.3); background: #000; box-shadow: 0 4px 15px rgba(0,0,0,0.2); width: 50%; margin: 0 auto;">
            <img src="../images/sensors/line16/ev3-pybricks-upload-lib.webp" alt="Pybricks File List Screenshot Explanation" style="width: 100%; display: block; opacity: 0.9;" />
          </div>
        </div>
      </div>
    </div>
  </div>
</div>

<style>
  .table-hover-row:hover {
    background: rgba(0,0,0,0.04) !important;
  }
  .func-card:hover {
    transform: translateY(-2px);
    box-shadow: 0 5px 15px rgba(0, 210, 255, 0.1);
    border-color: rgba(0, 210, 255, 0.4) !important;
  }
  .btn-download:hover {
    transform: translateY(-2px);
    box-shadow: 0 6px 20px rgba(0,210,255,0.4);
  }
</style>
