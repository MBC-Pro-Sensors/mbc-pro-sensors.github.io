# LEGO® Education SPIKE™ Prime Set

[🔙 Back to SPIKE Hubs](/en/sensors/line16/spike-hub.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(242,194,0,0.03); border: 1px solid rgba(242,194,0,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.35));" />
    <span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
    <img src="../images/hubs/spike-education-app.webp" alt="LEGO Education SPIKE App" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.15));" />
  </div>
</div>

> [!WARNING] 
> **Please Note: You are currently viewing the tutorial for the 【LEGO® Education SPIKE™ Prime Set】 environment.**
> If you are using EV3 or other system software, please click the button above to return to the hub menu and reselect.

This page applies to the **LEGO® Education SPIKE™ Prime Set**, utilizing the **LEGO Education SPIKE 3 App** block programming mode.

In this environment, the Pathfinder 16-Way Sensor will be perfectly identified by the LEGO system as an official **"SPIKE Color Sensor"**. To break through the limitations of the official hardware sockets, we utilize special encoding technology to map and package the 16-channel data into the various data channels of the official color sensor, allowing you to read it using the simplest official programming blocks!

---

## 📊 Block Command Data Channel Mapping Table

You can directly pull the official **block commands** of the color sensor in your program to obtain the following mapped data:

<div class="custom-table-wrapper" style="overflow-x: auto; margin: 25px 0; border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); background: #f8f9fa;">
  <table style="width: 100%; border-collapse: collapse; text-align: left; font-size: 0.95rem;">
    <thead>
      <tr style="background: rgba(10,186,181,0.12); border-bottom: 2px solid rgba(10,186,181,0.25);">
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">Official Block Command</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 25%;">Mapped Physical Data</th>
        <th style="padding: 15px 20px; color: #007a75; font-weight: bold; width: 50%;">Value Range & Decoding Guide</th>
      </tr>
    </thead>
    <tbody>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🎨 Color</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Feature Line Width</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(10,186,181,0.15); color: #007a75; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(10,186,181,0.2);">0 ~ 16</span><br>
          👉 Represents the number of sensors stepping on the line. A normal black line is generally <code>2 ~ 3</code>. If it is an intersection or a large color block, the value will increase significantly, which can be used for intersection protection.
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">💡 Reflected Light</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Standard Position Shift Value</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(217,163,0,0.15); color: #b88600; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(217,163,0,0.2);">0 ~ 32</span><br>
          👉 <code>16</code> represents dead center, <code>0</code> represents far left, <code>32</code> represents far right. Reading the value and subtracting 16 gives the line-following position (<strong>negative value means black line is on the left, positive means black line is on the right</strong>). This value is clear and suitable for PID line following.
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🔴 Red</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">High-Resolution Smooth Position</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(255,107,53,0.15); color: #d32f2f; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(255,107,53,0.2);">0 ~ 200</span><br>
          👉 <code>100</code> represents dead center, <code>0</code> represents extreme left, <code>200</code> represents extreme right. Reading the value and subtracting 100 gives the high-precision line-following position (<strong>negative value means black line is on the left, positive means black line is on the right</strong>). It uses smart algorithms to provide a higher-precision error margin. Using this mode requires ensuring the sensor is properly calibrated.
        </td>
      </tr>
      <tr class="table-hover-row" style="border-bottom: 1px solid rgba(0,0,0,0.06); background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🟢 Green</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">First 8 Photoelectric Values</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(0,180,216,0.15); color: #0077b6; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(0,180,216,0.2);">0 ~ 65535</span><br>
          👉 <strong>First 8 photoelectric values</strong>: Encoded mapping.
        </td>
      </tr>
      <tr class="table-hover-row" style="border: none; background: rgba(0,0,0,0.01); transition: background 0.2s ease;">
        <td style="padding: 18px 20px; font-weight: bold; color: #111; vertical-align: middle;">🔵 Blue</td>
        <td style="padding: 18px 20px; font-weight: bold; color: #333; vertical-align: middle;">Last 8 Photoelectric Values</td>
        <td style="padding: 18px 20px; color: #444; line-height: 1.6; vertical-align: middle;">
          <span style="display: inline-block; background: rgba(0,180,216,0.15); color: #0077b6; padding: 2px 8px; border-radius: 4px; font-family: monospace; font-size: 0.85rem; margin-bottom: 6px; border: 1px solid rgba(0,180,216,0.2);">0 ~ 65535</span><br>
          👉 <strong>Last 8 photoelectric values</strong>: Encoded mapping.
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

## [📥 Download Example Programs](../examples/line16/spike/line16-demo.llsp3)

<div style="text-align: center; margin: 25px 0;">
  <img src="../images/sensors/line16/spike-education-example.webp" alt="LEGO® Education SPIKE™ App Block Example" style="max-width: 100%; border-radius: 8px; border: 1px solid rgba(10,186,181,0.25); box-shadow: 0 10px 30px rgba(0,0,0,0.5); filter: drop-shadow(0 0 15px rgba(10,186,181,0.15));" />
  <p style="font-size: 0.85rem; color: #888; margin-top: 10px;">💡 The image above demonstrates the classic PID line-following program structure in the LEGO® Education SPIKE™ App for the 16-Way Line Follower.</p>
</div>

* <a href="../examples/line16/spike/line16-demo.llsp3" target="_blank" download="line16-demo.llsp3" style="color: #0abab5; font-weight: bold; text-decoration: none;">📥 SPIKE Example (.llsp3)</a>

> After downloading, open it directly using the official LEGO® Education SPIKE™ App.
