# 🧱 SPIKE Series Hubs (16-Way Line Follower)

[🔙 Back to 16-Way Line Follower Home](/en/sensors/line16/index.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 25px 45px; display: flex; align-items: center; gap: 25px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="../images/sensors/line16/line16-product-spike.webp" alt="Pathfinder 16-Way Sensor" style="max-height: 144px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(255,107,53,0.75)) drop-shadow(0 0 45px rgba(255,107,53,0.45));" />
    <span style="font-size: 2.5rem; color: #777; font-weight: 300; line-height: 1;">+</span>
    <div style="display: flex; gap: 15px; align-items: center;">
      <img src="../images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 120px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(242,194,0,0.35));" />
      <img src="../images/hubs/inventor-hub.webp" alt="Robot Inventor Hub" style="max-height: 120px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(10,186,181,0.35));" />
    </div>
  </div>
</div>

Welcome to the SPIKE zone! The Pathfinder 16-Way Sensor perfectly supports the following two flagship LEGO hub systems:

<div class="responsive-grid-2" style="gap: 20px; margin: 25px 0;">
  <!-- Yellow Hub Card -->
  <div style="background: rgba(242,194,0,0.04); border: 1px solid rgba(242,194,0,0.25); border-radius: 12px; padding: 25px; display: flex; flex-direction: column; align-items: center; text-align: center;">
    <img src="../images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 120px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(242,194,0,0.3)); margin-bottom: 15px;" />
    <h3 style="color: #f2c200; margin: 0 0 15px 0; font-size: 1.3rem;">🟡 SPIKE Prime Education Edition (Yellow Hub)</h3>
    <div style="font-size: 1rem; line-height: 1.8; color: #ddd; border-top: 1px solid rgba(242,194,0,0.15); padding-top: 15px; width: 100%;">
      LEGO® Education SPIKE™ Prime Set
    </div>
  </div>

  <!-- Teal Hub Card -->
  <div style="background: rgba(10,186,181,0.04); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 25px; display: flex; flex-direction: column; align-items: center; text-align: center;">
    <img src="../images/hubs/inventor-hub.webp" alt="Robot Inventor Hub" style="max-height: 120px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(10,186,181,0.3)); margin-bottom: 15px;" />
    <h3 style="color: #0abab5; margin: 0 0 15px 0; font-size: 1.3rem;">🟢 Robot Inventor Home Edition (Teal Hub)</h3>
    <div style="font-size: 1rem; line-height: 1.8; color: #ddd; border-top: 1px solid rgba(10,186,181,0.15); padding-top: 15px; width: 100%;">
      LEGO® MINDSTORMS® Robot Inventor
    </div>
  </div>
</div>

<style>
  @media (max-width: 768px) {
    div[style*="grid-template-columns: repeat(2, 1fr)"] {
      grid-template-columns: 1fr !important;
    }
  }
</style>

> [!NOTE]
> In the official block Apps, the sensor will by default **emulate a "SPIKE Color Sensor"**, utilizing special encoding techniques to package the 16-channel data into the color, reflected light, and RGB channels.

Please select the environment that matches your setup below to view the corresponding installation and programming block tutorials:

<div class="env-hub-grid">
  <!-- Card 1 -->
  <a href="#/en/sensors/line16/spike-education" class="env-card spike" style="display: flex; flex-direction: column; align-items: center; justify-content: center; min-height: 220px; padding: 30px 20px;">
    <img src="../images/hubs/spike-education-app.webp" alt="LEGO Education SPIKE App" style="max-height: 80px; object-fit: contain; margin-bottom: 20px; filter: drop-shadow(0 0 10px rgba(242,194,0,0.15));" />
    <h3 style="margin: 0 !important; font-size: 1.3rem;">LEGO Education SPIKE App</h3>
  </a>

  <!-- Card 2 -->
  <a href="#/en/sensors/line16/spike-ri" class="env-card spike" style="display: flex; flex-direction: column; align-items: center; justify-content: center; min-height: 220px; padding: 30px 20px;">
    <img src="../images/hubs/spike-mindstorms-app.webp" alt="LEGO MINDSTORMS Inventor App" style="max-height: 80px; object-fit: contain; margin-bottom: 20px; filter: drop-shadow(0 0 10px rgba(10,186,181,0.15));" />
    <h3 style="margin: 0 !important; font-size: 1.3rem;">LEGO MINDSTORMS Inventor</h3>
  </a>

  <!-- Card 3 -->
  <a href="#/en/sensors/line16/spike-pybricks" class="env-card spike" style="display: flex; flex-direction: column; align-items: center; justify-content: center; min-height: 220px; padding: 30px 20px;">
    <img src="../images/hubs/spike-pybricks-logo.webp" alt="pyBricks Logo" style="max-height: 80px; object-fit: contain; margin-bottom: 20px; filter: drop-shadow(0 0 10px rgba(0,210,255,0.15));" />
    <h3 style="margin: 0 !important; font-size: 1.3rem;">pyBricks</h3>
  </a>
</div>
