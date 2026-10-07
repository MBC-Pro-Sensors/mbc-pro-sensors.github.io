<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# SPIKE 6-Way Expander (EXP6)

<!-- product-hero -->

> [!IMPORTANT]
> **⚠️ Important Notice on Purchase and Compatibility**
> This expander is designed **EXCLUSIVELY for the SPIKE series**, featuring the LPF2 (Lego 6-pin) interface.
> It is ONLY compatible with the SPIKE Prime hub. **It DOES NOT support Robot Inventor or EV3 hubs!**

---

## 🚀 Product Overview: Break the Port Limit

The SPIKE Prime hub only has 6 connection ports, but advanced robotics projects often require multiple sensors and motors simultaneously, making the ports insufficient.

The **SPIKE 6-Way Expander (EXP6)** allows you to connect up to 6 sensors or motors using just a single port. It is not just a "splitter", but a **"smart controller"** with a built-in high-performance processor. It can independently calculate PID angle control for motors, significantly reducing the computing load on the SPIKE hub; more importantly, this product features an **independent isolated power supply system**. The large current required by the motors will not be drawn from the main hub, **completely eliminating the fatal flaw of hub crashes or burnouts caused by motor power consumption!**

## 🧠 Core Hardware Features

<div class="responsive-grid-2" style="gap: 20px; margin: 25px 0;">
<div style="background: rgba(255,69,0,0.05); border: 1px solid rgba(255,69,0,0.3); border-radius: 8px; padding: 20px; display: flex; flex-direction: column; justify-content: space-between;">
<h4 style="color: #ff4500; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">⚡ Independent Isolated Power</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">The motor driving current is completely supplied by an independent battery box/adapter, fully physically opto-isolated from the SPIKE hub. This protects your expensive hub from abnormal large currents, ensuring crash-free competitions.</p>
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
<h4 style="color: #00d2ff; margin-top: 0; font-size: 1.15rem; margin-bottom: 10px;">🛡️ Smart Watchdog Protection</h4>
<p style="font-size: 0.95em; margin: 0; line-height: 1.6; color: #ccc;">Hardware-level Watchdog protection mechanism. Upon disconnection from the hub or program crash, the expander will automatically brake and lock the motors to prevent runaway incidents, providing the most solid safety backing for high-end mecha competitions.</p>
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
2. **Connect Independent Power**: The expander must be connected to an additional independent power supply (battery box or adapter), which will exclusively power the motors.
3. **Connect Devices**: Plug your LEGO sensors (color, ultrasonic, force) and motors into any of the ports 1~6 on the expander.
4. **Ready to Code!**

---

## 🎮 Choose Your Software Environment (Dual Ecosystem Support)

The greatest feature of the EXP6 expander is its **"Perfect Dual Ecosystem Support"**. Please select the software platform you are currently using to view the dedicated connection guide and API documentation:

<div class="env-hub-grid" style="grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));">
<a href="#/en/sensors/exp6/spike-official" class="env-card spike" style="padding: 40px 20px;">
<div style="display: flex; gap: 20px; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
<img src="/images/hubs/spike-education-app.webp" alt="SPIKE App" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.3));" />
</div>
<h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">Official SPIKE App</h3>
<p style="font-size: 1.1rem;">Supports the official Python environment.<br>Features synchronous (Sync) execution and a simple, intuitive object-oriented design, perfect for beginners and education!</p>
</a>

<a href="#/en/sensors/exp6/spike-pybricks" class="env-card pybricks" style="padding: 40px 20px; border-color: rgba(0,210,255,0.4);">
<div style="display: flex; justify-content: center; margin-bottom: 20px; min-height: 110px; align-items: flex-end;">
<img src="/images/hubs/spike-pybricks-logo.webp" alt="Pybricks" style="max-height: 100px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.3));" />
</div>
<h3 style="font-size: 1.8rem; margin-bottom: 15px !important;">Pybricks Pro Environment</h3>
<p style="font-size: 1.1rem;">Built for competitions and extreme performance.<br>Natively supports asynchronous multitasking (Async/await) and blazing-fast RAW communication!</p>
</a>
</div>
