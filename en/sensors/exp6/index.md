<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# MBC EXP6 6-Port Motor Expander (for LEGO® SPIKE™)

<!-- product-hero -->

<!-- platform-grid -->


> [!IMPORTANT]
> **⚠️ Important Notice on Purchase and Compatibility**
> This expander is designed **EXCLUSIVELY for the SPIKE series**, featuring the LPF2 (Lego 6-pin) interface.
> It works with the **SPIKE Prime** hub, and with the **Robot Inventor (51515)** hub through Python in the Robot Inventor App only ([guide](/en/sensors/exp6/spike/official.md), [interactive tutorial](/en/learn/exp6/robot-inventor.html)). **It DOES NOT support EV3 hubs!**

---

## 🚀 Product Overview: Break the Port Limit

The SPIKE Prime hub only has 6 connection ports, but advanced robotics projects often require multiple sensors and motors simultaneously, making the ports insufficient.

The **MBC EXP6 6-Port Motor Expander (for LEGO® SPIKE™)** allows you to connect up to 6 sensors or motors using just a single port. It is not just a "splitter", but a **"smart controller"** with a built-in high-performance processor. It can independently calculate PID angle control for motors, significantly reducing the computing load on the SPIKE hub; more importantly, this product features an **independent isolated power supply system**. The large current required by the motors will not be drawn from the main hub, **completely eliminating the fatal flaw of hub crashes or burnouts caused by motor power consumption!**

## 🧠 Core Hardware Features

<div class="responsive-grid-2" style="gap: 20px; margin: 25px 0;">
<div style="background: rgba(255,69,0,0.05); border: 1px solid rgba(255,69,0,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #ff4500; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">⚡ Independent Isolated Power</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">The motor driving current is completely supplied by EXP6's own 2S LiPo battery, and the signal lines to the SPIKE hub are fully electrically isolated by an isolation chip. This protects your expensive hub from abnormal large currents, ensuring crash-free competitions.</p>
</div>
<div style="background: rgba(0,255,100,0.05); border: 1px solid rgba(0,255,100,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #00ff64; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🧠 On-board Edge Computing</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">A built-in high-performance microcontroller silently runs 6-channel motor PID absolute angle calculations and motor synchronization in the background. The SPIKE hub only needs to issue a command, and the expander handles all the precise control.</p>
</div>
<div style="background: rgba(242,194,0,0.05); border: 1px solid rgba(242,194,0,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #f2c200; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🎯 Extreme 1-Wire 6-Channel Comm</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">Utilizing a custom high-speed data flow protocol, the hub can receive 32-bit data packets from all 6 sensors in a single cycle, achieving near-zero latency multi-sensor sampling.</p>
</div>
<div style="background: rgba(0,210,255,0.05); border: 1px solid rgba(0,210,255,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #00d2ff; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🛡️ Watchdog Protection (Pybricks)</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">In the Pybricks environment, if the expander receives no signal from the hub for about 0.5 seconds (disconnection or program crash), it lets the motors <b>coast to a stop</b> to prevent runaways. <b>The official SPIKE App has no watchdog</b> — always end your program with <code>stop_all(1)</code>.</p>
</div>
</div>

<style>
/* 行動裝置響應式：寬度小於 768px 時自動降為單排 1x4 */
@media (max-width: 768px) {
div[style*="grid-template-columns: repeat(2, 1fr)"] {
grid-template-columns: 1fr !important;
}
}
</style>

---

## 🔌 Hardware Wiring & Startup

1. **Hub Communication Connection**: Plug the "Host Comm Cable" of the expander into any port (Port A~F) of the SPIKE hub.
2. **Connect a 2S LiPo Battery**: The expander must have a **2S LiPo** (7.4 V, 8.4 V fully charged, XT30 connector). Both the motors and the LEGO sensors are powered by this battery, not by the hub. **Do not use 3S or higher, or a wall adapter**: above 9 V, EXP6 cuts power to LEGO Color / Ultrasonic sensors.
3. **Connect Devices**: Plug your LEGO sensors (color, ultrasonic, force) and motors into any of the ports 1~6 on the expander.
4. **Ready to Code!**

---

## 💡 On-board LEDs

**Battery LEDs** (4 LEDs, top to bottom: LED1, LED2, LED3, red): the level fills **from the bottom up**; LED3, right next to the red LED, is the first bar. The levels are scaled for a **2S LiPo**. The voltage is smoothed over about 4 seconds, and moving up a bar needs an extra 0.1 V, so the LEDs do not flicker between levels.

| Battery voltage (2S) | Red | LED3 | LED2 | LED1 (top) |
| :--- | :--- | :--- | :--- | :--- |
| Below 6.9 V | Slow blink (almost empty, please charge) | Off | Off | See link status |
| 6.9~7.2 V | On | Off | Off | See link status |
| 7.2~7.5 V | Off | On | Off | See link status |
| 7.5~7.8 V | Off | On | On | See link status |
| 7.8 V and above | Off | On | On | On |

**LED1 also shows the link status** (link status takes priority over the battery level):

- **Fast blink** (0.1 s) = waiting for the hub handshake. With Pybricks, EXP6 only connects when the program reaches `MBC_EXP6(...)`, so **fast blinking after power-on, before any program runs, is normal**.
- **Slow blink** (0.5 s) = in Pybricks mode, no data from the hub for more than 0.5 s (the program ended or the stop button was pressed); the watchdog has already let the motors coast to a stop.
- Otherwise = the third battery bar (on at 7.8 V and above).

**Port LEDs** (6 LEDs, port k lights LED k):

| Port LED | Meaning |
| :--- | :--- |
| Off | Empty port |
| Fast blink | Handshaking with the device |
| Steady on | Connected |
| Slow blink | The motor on this port is running a move |

---

## 🎮 Choose Your Software Environment (Dual Ecosystem Support)

The greatest feature of the EXP6 expander is its **"Perfect Dual Ecosystem Support"**. Please select the software platform you are currently using to view the dedicated connection guide and API documentation:

<div class="env-hub-grid" style="grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));">
<a href="/en/sensors/exp6/spike-official.md" class="env-card spike" style="padding: 40px 20px;">
<div style="display: flex; gap: 20px; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
<img src="/images/hubs/spike-education-app.webp" alt="SPIKE App" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.3));" />
</div>
<h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">Official SPIKE App</h3>
<p style="font-size: 1.1rem;">Supports the official Python environment.<br>Features synchronous (Sync) execution and a simple, intuitive object-oriented design, perfect for beginners and education!</p>
</a>

<a href="/en/sensors/exp6/spike-pybricks.md" class="env-card pybricks" style="padding: 40px 20px; border-color: rgba(0,210,255,0.4);">
<div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
<img src="/images/hubs/spike-pybricks-logo.webp" alt="Pybricks" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.3));" />
</div>
<h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">Pybricks Pro Environment</h3>
<p style="font-size: 1.1rem;">Built for competitions and extreme performance.<br>Natively supports asynchronous multitasking (Async/await) and blazing-fast RAW communication!</p>
</a>

</div>
