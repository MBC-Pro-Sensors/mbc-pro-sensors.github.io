# LEGO® MINDSTORMS® EV3 Official Software (EV3-G)

[🔙 Back to EV3 Hubs](/en/sensors/line8/ev3-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/ev3-hub.webp" alt="EV3 Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/ev3-official-app.webp" alt="EV3 Official App" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  </div>
</div>

> [!WARNING] 
> **Please Note: You are currently viewing the tutorial for the 【EV3-G Official Software】 environment.**
> If you are using SPIKE or other EV3 software, please click the button above to return to the hub menu and reselect.

Applicable to the **LEGO® MINDSTORMS® EV3 Official Software** (LabVIEW style interface, often referred to as EV3-G). In this environment, the Pathfinder 8-Way Sensor will be perfectly identified by the LEGO system as an official **Infrared Sensor**.

> [!IMPORTANT]
> **⚠️ Crucial Hardware Communication Delay Warning for EV3**
> Due to the hardware communication mechanism limitations of the official LEGO EV3 hub, when a program switches between different block modes (e.g., from Proximity to Remote) for reading, **the hub will generate a physical communication waiting delay of approximately 0.5 seconds**, which can cause the robot to lose control in high-speed competitions.
> **Strong Recommendation**: Depending on your task requirements (e.g., only using Remote for PID line following), **select ONE block mode at the very beginning of the program and DO NOT switch reading modes midway through the task!**

> **📌 Simulation Description**
> In the official EV3 software, the sensor will be recognized as an Infrared Sensor. Please follow the instructions below to select Proximity / Beacon / Remote modes to read different data.

---

## Read Line Position — Proximity Mode

<div style="text-align: center; margin: 15px 0;">
  <img src="../images/sensors/ev3-blocks/official/mode-proximity.webp" alt="EV3 Proximity Mode Block" style="max-height: 80px; border-radius: 8px; filter: drop-shadow(0 5px 15px rgba(10,186,181,0.2));" />
</div>

Use the sensor block, select **Proximity mode**, and read the `value` to get the line position.

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">Returned Value</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">Range</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 50%;">Description</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Line Position</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(255,107,53,0.15); color: #d32f2f; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(255,107,53,0.2);">-8 ~ +8</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 <strong><code>0</code> = black line is dead center, negative is left, positive is right</strong>; <code>±8</code> means the line is at the very edge of the sensor.
        </td>
      </tr>
    </tbody>
  </table>
</div>

---

## Read 8-Channel Photoelectric Reflection Values — Beacon Mode

<div style="text-align: center; margin: 15px 0;">
  <img src="../images/sensors/ev3-blocks/official/mode-beacon.webp" alt="EV3 Beacon Mode Block" style="max-height: 80px; border-radius: 8px; filter: drop-shadow(0 5px 15px rgba(10,186,181,0.2));" />
</div>

Use the sensor block, select **Beacon mode**, and read the `value list` (4 bytes).

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 15%;">Byte</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">Content</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 20%;">Range</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 40%;">Decoding Method</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">0</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Reflectance Ch0 + 1</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">Tens digit = Ch0, Units digit = Ch1</td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">1</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Reflectance Ch2 + 3</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">Tens digit = Ch2, Units digit = Ch3</td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">2</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Reflectance Ch4 + 5</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">Tens digit = Ch4, Units digit = Ch5</td>
      </tr>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">3</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Reflectance Ch6 + 7</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;"><span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">11 ~ 99</span></td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">Tens digit = Ch6, Units digit = Ch7</td>
      </tr>
    </tbody>
  </table>
</div>

> Each channel's photoelectric reflectance value has been quantized to 1 to 9 (1=darkest, 9=brightest), and two channels are combined into a two-digit number (11 to 99).
> 👉 **Decoding Example**: If you read a Byte value of **`83`**, its **tens digit (8)** represents the quantized light value of the even channel (e.g., Channel 0), and the **units digit (3)** represents the quantized light value of the odd channel (e.g., Channel 1). This perfectly circumvents the LEGO EV3's 100 limit!

---

## Read Position + Width + Boundary Defense — Remote Mode (Recommended for Tactical Competition!)

<div style="text-align: center; margin: 15px 0;">
  <img src="../images/sensors/ev3-blocks/official/mode-remote.webp" alt="EV3 Remote Mode Block" style="max-height: 80px; border-radius: 8px; filter: drop-shadow(0 5px 15px rgba(10,186,181,0.2));" />
</div>

Use the sensor block, select **Remote mode**, and read the `value list` (4 bytes).

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 15%;">Byte</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">Content Data</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 20%;">Value Range</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 40%;">Tactical Interpretation</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Byte 0</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">High-Res Position Shift</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(255,107,53,0.15); color: #d32f2f; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(255,107,53,0.2);">-100 ~ +100</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 <strong><code>0</code> represents dead center, negative is left, positive is right</strong>. The top choice for ultra-smooth PID error input!
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Byte 1</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Feature Line Width</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(10,186,181,0.2);">0 ~ 8</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 Number of triggered sensors. Can be used to detect intersections or T-junctions.
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Byte 2</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Left Boundary Physical Light Value</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(217,163,0,0.15); color: #b88600; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(217,163,0,0.2);">0 ~ 100</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 <code>dataIrCalib[7]</code> The reflected light of the leftmost sensor. <code>0</code>=pure black, <code>100</code>=pure white. Can be used to detect track boundaries or forks.
        </td>
      </tr>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">Byte 3</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Right Boundary Physical Light Value</td>
        <td style="padding: 18px 20px; color: #444; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(217,163,0,0.15); color: #b88600; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; border: 1px solid rgba(217,163,0,0.2);">0 ~ 100</span>
        </td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          👉 <code>dataIrCalib[0]</code> The reflected light of the rightmost sensor. <code>0</code>=pure black, <code>100</code>=pure white. Can be used to detect track boundaries or forks.
        </td>
      </tr>
    </tbody>
  </table>
</div>

> 💡 **Tactical Fool-Proofing Tip**: Byte 2 and Byte 3 in Remote mode allow you to directly obtain the extreme edge light values on both sides of the sensor within EV3-G blocks. If both values are simultaneously less than 20, it means the front of the car has completely entered the horizontal line of an intersection; if one side's value suddenly changes, it means a fork is approaching. This is the ultimate weapon to prevent losing the line!





<style>
  .table-hover-row:hover {
    background: rgba(0,0,0,0.04) !important;
  }
</style>

---

## <a href="../examples/line8/ev3/line8_ev3_official.ev3" target="_blank" data-ignore="true" download>📥 Download Example Programs</a>

<div style="text-align: center; margin: 25px 0;">
  <img src="../images/sensors/line8/ev3-official-example.webp" alt="LEGO® MINDSTORMS® EV3 Official Block Example" style="max-width: 100%; border-radius: 8px; border: 1px solid rgba(10,186,181,0.25); box-shadow: 0 10px 30px rgba(0,0,0,0.5); filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  <p style="font-size: 0.85rem; color: #888; margin-top: 10px;">💡 The image above demonstrates the classic PID line-following program structure in EV3-G for the 8-Way Line Follower.</p>
</div>

* <a href="../examples/line8/ev3/line8_ev3_official.ev3" target="_blank" download="line8_ev3_official.ev3" style="color: #0abab5; font-weight: bold; text-decoration: none;">📥 Official EV3-G Example (.ev3) </a>

---

## 🧩 EV3 Exclusive My Blocks Explanation

To make programming in EV3-G more concise and intuitive, the MBC 8-Way Line Follower provides three powerful "My Blocks". During development, you don't need to install cumbersome extension packs; just import the My Blocks, and the system will automatically parse the hardware packets in the background and output the latest sensor data for the motor control logic to use directly.

Here is an introduction to the functions and usage timings of the three core My Blocks:

### 1. Basic Line Following Block: `LinePos8`
* **Purpose**: Quickly read the most basic 8-segment line position, suitable for beginner tutorials and basic left/right steering judgments.
* **Output Node**: `0_LinePos8`.
* **Value Parsing**: The range is between `-8` and `+8`.
* **Physical Intuition**: Negative value when the black line is on the left, 0 when it is dead center, positive value when it is on the right.

### 2. Competitive High-Precision Block: `LinePos100`
* **Purpose**: Obtain high-resolution sub-pixel interpolated positions, the best choice for PID line following. Designed specifically for high-end PID/PD line-following control algorithms (applicable to various racing events), providing extremely smooth correction power. **The prerequisite is that the sensor calibration must be fully completed; the more accurate the calibration, the more precise the values.**
* **Output Nodes**: 
  * `0_LinePos100`: High-precision position, range `-100` to `+100` (also maintains the standard of left negative, right positive).
  * `0_LineWidth8`: Real-time line width detection, the value represents the number of currently triggered sensors (range `0` to `8`). Can be used to accurately judge intersections, T-junctions, or the finish stop line.
  * **Left/Right Boundaries**: Synchronously read the physical light values of the leftmost and rightmost sensors.

### 3. Full Array Expansion Block: `LineCh8`
* **Purpose**: Thoroughly decode the 8 sensors into independent states. Applicable for writing custom special intersection judgment algorithms, or drawing the status of the 8 sensors directly on the screen of the EV3 hub.
* **Output Nodes**: Outputs 8 independent values from `s1` to `s8` in one go, perfectly corresponding to the real-time readings of each sensor from left to right.

<br>

!!! success "🚀 Want to learn more advanced line-following control methods?"
    You can take classes from these awesome coaches! They have rich experience in competitions and teaching, guaranteeing you'll learn a lot! 💯

<div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; margin-top: 15px; margin-bottom: 30px;">
  <div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
    <h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@LegoLauXiao" target="_blank" style="color: inherit; text-decoration: none;">Coach LegoLauXiao</a></h4>
    <div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
      <iframe src="https://www.youtube.com/embed/WgacdWLatbk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
    </div>
  </div>
  <div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
    <h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@legolaumo" target="_blank" style="color: inherit; text-decoration: none;">Coach legolaumo</a></h4>
    <div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
      <iframe src="https://www.youtube.com/embed/T9bcndBNQvQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
    </div>
  </div>
</div>

