# LEGO® MINDSTORMS® Robot Inventor

[🔙 Back to SPIKE Hubs](/en/sensors/line8/spike-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/inventor-hub.webp" alt="MINDSTORMS Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/spike-mindstorms-app.webp" alt="LEGO MINDSTORMS Inventor App" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  </div>
</div>

> [!WARNING] 
> **Please Note: You are currently viewing the tutorial for the 【LEGO® MINDSTORMS® Robot Inventor】 environment.**
> If you are using EV3 or other system software, please click the button above to return to the hub menu and reselect.

This page applies to the **LEGO® MINDSTORMS® Robot Inventor**, utilizing the **LEGO MINDSTORMS Inventor App** block programming mode.

In this environment, the Pathfinder 8-Way Sensor will be perfectly identified by the LEGO system as an official **"SPIKE Color Sensor"**.

> [!CAUTION] 
> **⚠️ Official Software Limitations of the Smart Hub (51515 MINDSTORMS® Hub)**
> Due to the limitations of the official LEGO Robot Inventor App software, the values of some single-channel blocks are **strictly limited to `0 ~ 126` by the official system**. In order to allow you to still unleash the extreme performance of the Pathfinder 8-Way on this hub, we have developed an exclusive "Data Shunting and Squeezing" transmission protocol, successfully bypassing the 126 limit for position and reflected light massive data, directly mapping them onto the RGB and reflected light channels!

---

## 📊 Physical Data Channel Mapping Table

You can directly pull the official **"Reflected Light"** or **"RGB Value"** blocks of the color sensor in your program to obtain the following data:

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">Official Block Channel</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">Mapped Physical Data</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 50%;">Value Range & Decoding Guide</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🎨 Color</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Feature Line Width</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(10,186,181,0.2);">0 ~ 8</span><br>
          👉 Due to the home version software limiting color to a maximum of 10, the feature line width here is normally between <code>0 ~ 8</code>, commonly used for detecting intersections.
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">💡 Reflected Light</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Standard Position Shift Value</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(217,163,0,0.15); color: #b88600; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(217,163,0,0.2);">0 ~ 16</span><br>
          👉 <code>8</code> represents dead center, <code>0</code> represents far left, <code>16</code> represents far right. Reading the value and subtracting 8 gives the line-following position (<strong>negative value means black line is on the left, positive means black line is on the right</strong>). The value is stable and completely unaffected by the 126 limit.
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🔴 Red Channel</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">High-Resolution Smooth Position</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(255,107,53,0.15); color: #d32f2f; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(255,107,53,0.2);">0 ~ 200</span><br>
          👉 <code>100</code> represents dead center, <code>0</code> represents extreme left, <code>200</code> represents extreme right. Reading the value and subtracting 100 gives the high-precision line-following position (<strong>negative value means black line is on the left, positive means black line is on the right</strong>). This channel is <strong>perfectly lossless</strong> on the toy version, with no degradation whatsoever, and can be directly plugged into high-end PID line-following controls!
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🟢 Green Channel</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">First 4 Photoelectric Values</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(0,180,216,0.15); color: #0077b6; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(0,180,216,0.2);">0 ~ 255</span><br>
          👉 <strong>First four photoelectric values</strong>: Every 2-bits represent a photoelectric value, value range <code>0 ~ 3</code>.
        </td>
      </tr>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🔵 Blue Channel</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Last 4 Photoelectric Values</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(0,180,216,0.15); color: #0077b6; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(0,180,216,0.2);">0 ~ 255</span><br>
          👉 <strong>Last four photoelectric values</strong>: Every 2-bits represent a photoelectric value, value range <code>0 ~ 3</code>.
        </td>
      </tr>
    </tbody>
  </table>
</div>

<style>
  .table-hover-row:hover {
    background: rgba(0,0,0,0.04) !important;
  }
</style>

---

## <a href="../examples/line8/spike/line8-ri-demo.lms" target="_blank" data-ignore="true" download>📥 Download Example Programs</a>

<div style="text-align: center; margin: 25px 0;">
  <img src="../images/sensors/line8/spike-ri-example.webp" alt="LEGO® MINDSTORMS® Robot Inventor App Block Example" style="max-width: 100%; border-radius: 8px; border: 1px solid rgba(10,186,181,0.25); box-shadow: 0 10px 30px rgba(0,0,0,0.5); filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  <p style="font-size: 0.85rem; color: #888; margin-top: 10px;">💡 The image above demonstrates the classic PID line-following program structure in the LEGO® MINDSTORMS® Robot Inventor App for the 8-Way Line Follower.</p>
</div>

* <a href="../examples/line8/spike/line8-ri-demo.lms" target="_blank" download="line8-ri-demo.lms" style="color: #0abab5; font-weight: bold; text-decoration: none;">📥 Robot Inventor Example (.lms)</a>

> After downloading, open it directly using the LEGO® MINDSTORMS® Robot Inventor App.
