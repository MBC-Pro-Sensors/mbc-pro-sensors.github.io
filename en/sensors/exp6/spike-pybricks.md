<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Pybricks Pro Environment (EXP6)

[🔙 Back to EXP6 Home](/sensors/exp6/index.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
<div style="background: rgba(0,210,255,0.03); border: 1px solid rgba(0,210,255,0.25); border-radius: 12px; padding: 15px 35px; display: flex; align-items: center; gap: 20px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
<img src="/images/hubs/spike-hub.webp" alt="SPIKE Prime Hub" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(242,194,0,0.35));" />
<span style="font-size: 2.2rem; color: #666; font-weight: 300; line-height: 1;">+</span>
<img src="/images/hubs/spike-pybricks-logo.webp" alt="Pybricks" style="max-height: 90px; object-fit: contain; filter: drop-shadow(0 0 15px rgba(0,210,255,0.25));" />
</div>
</div>

> [!WARNING] 
> **Note: You are currently viewing the [Pybricks Pro Environment] tutorial.**
> Applicable to SPIKE Prime flashed with Pybricks firmware. If you are using the official SPIKE App, click [here to switch to the official tutorial](/sensors/exp6/spike-official.md)。

The Pybricks environment not only provides millisecond-level blazing fast communication, but also perfectly supports **'Asynchronous Multitasking (Async/await)'**. The exclusive EXP6 library utilizes this feature to build a **'Background Watchdog & Heartbeat Mechanism'**, ensuring the robot never disconnects during complex tasks.

---

## 📥 Download Dedicated Library and Examples

<div style="display: flex; gap: 15px; margin-top: 15px; flex-wrap: wrap;">
<a href="/downloads/MBC_EXP6_Pybricks_Lib.zip" class="btn-download" style="flex: 1; min-width: 200px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem; box-shadow: 0 4px 15px rgba(0,210,255,0.25); transition: transform 0.2s ease; display: inline-block;">📥 Download Pybricks Library & Examples (.zip)</a>
</div>

*(Note: Please place the downloaded `MBC_exp6_obj_Lib.py` into your Pybricks project to use it.)*

---

<div style="margin: 30px 0; text-align: center;">
<img src="/images/sensors/exp6/exp6_pybricks_example.webp" alt="Pybricks Program Interface Comparison" style="max-width: 100%; border-radius: 12px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); border: 1px solid rgba(0,210,255,0.3);" />
</div>

<style>
  .code-example { background: rgba(255,255,255,0.02); border: 1px solid rgba(0,210,255,0.2); border-radius: 12px; padding: 1.5rem; margin-bottom: 2rem; }
  .code-example h4 { font-size: 1.1rem; margin-bottom: 1rem; display: flex; align-items: center; gap: 0.5rem; color: #00d2ff; }
  .code-block-wrapper { margin-bottom: 1.5rem; border-radius: 8px; overflow: hidden; background: #1e1e1e; border: 1px solid rgba(0,210,255,0.2); }
  .code-block-wrapper:last-child { margin-bottom: 0; }
  .code-header { background: rgba(0,210,255,0.1); padding: 0.5rem 1rem; font-size: 0.85rem; color: #00d2ff; display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid rgba(0,210,255,0.2); font-weight: 600; }
  .code-badge { padding: 0.2rem 0.6rem; border-radius: 4px; font-size: 0.75rem; letter-spacing: 0.5px; }
  .code-badge.sync { background: rgba(59, 130, 246, 0.2); color: #60a5fa; }
  .code-badge.async { background: rgba(139, 92, 246, 0.2); color: #c084fc; }
  .blocks { display: flex; justify-content: center; align-items: center; padding: 1.5rem; background: rgba(0,0,0,0.2); border-radius: 8px; margin-bottom: 1.5rem; overflow-x: auto; border: 1px dashed rgba(255,255,255,0.1); }
  .block-label { font-size: 0.85rem; font-weight: 700; color: #aaa; margin-bottom: 0.5rem; margin-left: 0.5rem; }
  .blocks svg .sb3-input-string, .blocks svg .sb3-input-dropdown, .blocks svg .sb-input-string, .blocks svg .sb-input-dropdown, .blocks svg .sb-obsolete.sb-darker { fill: #D9576D !important; }
  .blocks svg .sb3-input-string + text.sb3-label, .blocks svg .sb3-input-dropdown + text.sb3-label, .blocks svg .sb3-input-dropdown ~ text.sb3-label, .blocks svg .sb-input-string + text.sb-label, .blocks svg .sb-input-dropdown + text.sb-label, .blocks svg .sb-input-dropdown ~ text.sb-label { fill: #FFFFFF !important; }
  .blocks svg .sb3-dropdown-arrow, .blocks svg .sb-input-dropdown polygon { fill: #FFFFFF !important; opacity: 1 !important; }
</style>
<section id="intro">
<div style="background-color: rgba(76, 175, 80, 0.15); border-left: 4px solid #4CAF50; padding: 1rem; margin-bottom: 2rem; border-radius: 4px;">
<strong style="color: #4CAF50; font-size: 1.1rem;">⚠️ Note: This documentation is for users of pyBricks version 4.0.0 and above</strong>
</div>
<h1>📚 1. Quick Start (Quick Start)</h1>
<p>Welcome to the <b>EXP6 Expander</b>! This manual will teach you how to use the <code>MBC_expObj_Lib</code> library to control motors and read sensors.</p>
<p>Our library primarily supports <b>Object-Oriented</b> syntax, which is as intuitive as dragging blocks. Furthermore, to meet various challenges, each command is divided into <b>Normal Mode</b> and <b>Multitask Mode</b>:</p>
<ul>
<li><b>Normal Mode</b>: Executes line by line sequentially. Simple and intuitive, suitable for basic teaching.</li>
<li><b>Multitask Mode</b>: Must prefix each command with the magic word <code>await</code>, allowing the robot to do many things 'simultaneously'. Suitable for advanced robotics.</li>
</ul>
<h3>🤖 Initialize Expander and Run Task (Setup &amp; Run Task)</h3>
<p>Before using any function, you must instantiate the expander object in the <code>set up</code> stage, and depending on your chosen mode (Normal/Multitask), decide whether to import <code>run_task</code>. Assuming the expander is connected to the Hub's <b>Port C (Port 3)</b>:</p>
<ul>
<li><b>Normal Mode</b>: Set multitask to <code>false</code> when creating the object, and you can directly issue control commands in <code>program</code>.</li>
<li><b>Multitask Mode</b>: Set multitask to <code>true</code>, and you <b>must</b> import the expander's exclusive <code>run_task</code> from <code>MBC_exp6_obj_Lib</code>. In the graphical interface, you just put the code under the <code>program</code> block. When converting to Python, the system automatically wraps it into <code>main()</code> and calls <code>run_task(main())</code>. The exclusive <code>run_task</code> internally starts the 'background heartbeat mechanism' to maintain connection and shuts down safely when finished.</li>
</ul>
<div class="code-example">
<h4>Init Block <code>MBC_EXP6</code> and Run Task</h4>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@turnRight set up :: #eeb014
from [MBC_exp6_obj_Lib] import [MBC_EXP6] :: #FF6680
@greenFlag [exp6] is a [MBC_EXP6] (3) (false :: #59C059) :: #FF6680

@turnLeft program :: #eeb014
@greenFlag [exp6 v] [motor_power] (6) (100) ‹ › :: #FF6680
@greenFlag [exp6 v] [keep_alive_wait] (5000) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header">
<span>Basic Normal Syntax</span>
<span class="code-badge sync">Normal</span>
</div>


```python
from MBC_exp6_obj_Lib import MBC_EXP6

# Set up.
exp6 = MBC_EXP6(3, False)

exp6.motor_power(6, 100)
exp6.keep_alive_wait(5000)

```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">@turnRight set up :: #eeb014
from [MBC_exp6_obj_Lib] import [MBC_EXP6] :: #FF6680
from [MBC_exp6_obj_Lib] import [run_task] :: #FF6680
@greenFlag [exp6] is a [MBC_EXP6] (3) (true :: #59C059) :: #FF6680

@turnLeft program :: #eeb014
@greenFlag [exp6 v] [await v] [motor_power] (6) (100) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [keep_alive_wait] (5000) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header">
<span>進階Async寫法 (需 await)</span>
<span class="code-badge async">Async</span>
</div>


```python
from pybricks.tools import multitask, run_task, wait
from MBC_exp6_obj_Lib import MBC_EXP6, run_task

# Set up.
exp6 = MBC_EXP6(3, True)

async def main():
    await exp6.motor_power(6, 100)
    await exp6.keep_alive_wait(5000)

run_task(main())

```


</div>
</div>
</section>

<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [motor_run] (1) (30) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Make motor on port 1 maintain a steady speed of 30
exp6.motor_run(1, 30)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [motor_run] (1) (30) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
await exp6.motor_run(1, 30)
```


</div>
</div>
<!-- Angle Control -->
<h3>🎯 Precise Angle Control (Angle Control)</h3>
<p>Set the motor to rotate a specific relative angle, or move to a specific absolute angle. Built-in PID and angle accumulator ensure precise movement.</p>
<div class="code-example">
<h4><code>exp6.motor_run_degrees(port, speed, degrees, stop)</code> 與 <code>exp6.motor_track_target(port, speed, angle, stop)</code></h4>
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
<li><code>speed</code>: Target speed (-100 ~ 100). Positive is clockwise, negative is counter-clockwise.</li>
<li><code>degrees</code>: Relative angle to move (e.g., 360 is one full rotation forward). Range: -2499 ~ 2499.</li>
<li><code>angle</code>: Absolute angle to move to (0~359, automatically finds the shortest path).</li>
<li><code>stop</code>: Number indicating the behavior after reaching the target.
&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;<ul>
<li><code>1</code>: Coast (STOP_COAST)</li>
<li><code>2</code>: Brake (STOP_BRAKE)</li>
<li><code>3</code>: Hold (STOP_HOLD)</li>
<li><code>4</code>: Continue (STOP_CONTINUE)</li>
</ul>
</li>
</ul>
<p><i>Note: <code>motor_run_target</code> is renamed to <code>motor_track_target</code>, but the old name is still supported.</i></p>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [motor_run_degrees] (1) (30) (360) (3) ‹ › :: #FF6680
@greenFlag [exp6 v] [call v] [motor_track_target] (1) (30) (90) (3) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# 讓孔 1 馬達以速度 30 轉動 360 度 (相對角度)，到達後Hold (HOLD)
exp6.motor_run_degrees(1, 30, 360, 3)

# 讓孔 1 馬達以速度 30 轉動到 90 度的位置 (絕對角度)，到達後Hold
exp6.motor_track_target(1, 30, 90, 3)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [motor_run_degrees] (1) (30) (360) (3) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [motor_track_target] (1) (30) (90) (3) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
await exp6.motor_run_degrees(1, 30, 360, 3)
await exp6.motor_track_target(1, 30, 90, 3)
```


</div>
</div>
<!-- Motor PID -->
<h3>🎛️ Set PID Parameters (Set PID)</h3>
<p>進階功能，用於手動調整馬達閉迴路控制的 P (比例)、I (積分)、D (微分) 參數，優化轉動的穩定性。<b>請注意：50 是系統預設的Motor Control參數基準值。</b>以 50 為基準，往上調整就是等比例放大，往下調整就是等比例縮小。</p>
<div class="code-example">
<h4><code>exp6.motor_set_pid(port, p, i, d)</code></h4>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [motor_set_pid] (1) (50) (50) (50) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Set PID parameters for motor on port 1
exp6.motor_set_pid(1, 50, 50, 50)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [motor_set_pid] (1) (50) (50) (50) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
await exp6.motor_set_pid(1, 50, 50, 50)
```


</div>
</div>
<!-- Motor Stop & Brake -->
<h3>🛑 停止與Brake (Stop &amp; Brake)</h3>
<p>你可以選擇讓馬達自然Coast停止，或是用力咬死Brake (鎖定維持在當前角度)。</p>
<div class="code-example">
<h4><code>exp6.motor_stop(port, stop)</code> 與 <code>exp6.stop_all(stop)</code></h4>
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
<li><code>stop</code>: Number indicating the stop mode.
&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;&#32;<ul>
<li><code>1</code>: Coast (STOP_COAST)</li>
<li><code>2</code>: Brake (STOP_BRAKE)</li>
<li><code>3</code>: Hold (STOP_HOLD)</li>
</ul>
</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [motor_stop] (1) (1) ‹ › :: #FF6680
@greenFlag [exp6 v] [call v] [motor_stop] (1) (3) ‹ › :: #FF6680
@greenFlag [exp6 v] [call v] [stop_all] (1) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
exp6.motor_stop(1, 1)  # 馬達斷電，自然Coast停止
exp6.motor_stop(1, 3)   # 咬死Brake，鎖定目前位置
exp6.stop_all(1)       # 緊急停止！讓擴充板上所有馬達Coast停止
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [motor_stop] (1) (1) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [motor_stop] (1) (3) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [stop_all] (1) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
await exp6.motor_stop(1, 1)
await exp6.motor_stop(1, 3)
await exp6.stop_all(1)
```


</div>
</div>
<!-- Motor Inverted -->
<h3>🔄 Set Motor Inverted (Set Inverted)</h3>
<p>Set the motor's default rotation direction. Reversing it at initialization automatically matches subsequent logic (including chassis control) without manual sign adjustments.</p>
<div class="code-example">
<h4><code>exp6.set_motor_inverted(port, inverted)</code></h4>
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
<li><code>inverted</code>: Boolean (<code>True</code>/<code>False</code>). <code>True</code> means inverted, defaults to <code>True</code> if omitted.</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [set_motor_inverted] (1) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Invert the motor direction on port 1
exp6.set_motor_inverted(1, True)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [set_motor_inverted] (1) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
await exp6.set_motor_inverted(1, True)
```


</div>
</div>
<!-- Double Motor Drive -->
<h3>🚗 Dual Motor Sync Control (Drive)</h3>
<p>Simultaneously and precisely controls two motors, perfect for wheeled chassis robots (e.g., line followers).</p>
<div class="code-example">
<h4><code>exp6.drive(left_port, right_port, left_speed, right_speed)</code></h4>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [drive] (1) (2) (50) (40) ‹ › :: #FF6680
@greenFlag [exp6 v] [call v] [drive_stop] (1) (2) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Make port 1 and 2 motors move forward at speed 50 and 40 simultaneously
exp6.drive(1, 2, 50, 40)

# 停止雙馬達 (Coast停止)
exp6.drive_stop(1, 2)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [drive] (1) (2) (50) (40) ‹ › :: #FF6680
@greenFlag [exp6 v] [await v] [drive_stop] (1) (2) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
await exp6.drive(1, 2, 50, 40)
await exp6.drive_stop(1, 2)
```


</div>
</div>
</section>
<!-- SECTION 3: Sensor -->
<section id="sensor">
<h1>📡 3. Sensor Reading (Sensors)</h1>
<p>Read sensor data and motor status. The expander automatically determines the sensor type and returns corresponding values.</p>
<!-- Motor State -->
<h3>📐 Motor Angle & Speed</h3>
<p>Get the current accumulated angle, absolute angle, or speed. Built-in 32-bit accumulator prevents overflow issues!</p>
<ul>
<li><b>Reset Angle</b>：<code>reset_angle(port)</code></li>
<li><b>Read Accumulated Angle</b>：<code>get_motor_angle(port)</code></li>
<li><b>Read Absolute Angle</b>：<code>get_motor_abs_angle(port)</code></li>
<li><b>Read Speed</b>：<code>get_motor_speed(port)</code></li>
</ul>
<div class="code-example">
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [reset_angle] (1) ‹ › :: #FF6680
(@greenFlag [exp6 v] [call v] [get_motor_angle] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_motor_abs_angle] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_motor_speed] (1) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# 將孔 1 馬達的Reset Angle
exp6.reset_angle(1)

# Read Accumulated Angle、絕對角度與速度deg/s
angle = exp6.get_motor_angle(1)
abs_angle = exp6.get_motor_abs_angle(1)
speed = exp6.get_motor_speed(1)
print("累積角度:", angle, "絕對角度:", abs_angle, "速度:", speed)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [await v] [reset_angle] (1) ‹ › :: #FF6680
(@greenFlag [exp6 v] [await v] [get_motor_angle] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_motor_abs_angle] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_motor_speed] (1) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
await exp6.reset_angle(1)

angle = await exp6.get_motor_angle(1)
abs_angle = await exp6.get_motor_abs_angle(1)
speed = await exp6.get_motor_speed(1)
print(angle, abs_angle, speed)
```


</div>
</div>
<!-- Color Sensor -->
<h3>🎨 Color Sensor (Color Sensor)</h3>
<p>取得 SPIKE Color Sensor的辨識結果與反射值。</p>
<div class="code-example">
<h4>Color Code <code>get_color_color(port)</code> and Reflection <code>get_color_reflection(port)</code></h4>
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_color_color] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_color_reflection] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# 讀取Color Code（-1 為無顏色，其他對應樂高標準色碼）
color_code = exp6.get_color_color(3)

# Read light reflection (0~100, suitable for line following)
reflection = exp6.get_color_reflection(3)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_color_color] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_color_reflection] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
color_code = await exp6.get_color_color(3)
reflection = await exp6.get_color_reflection(3)
```


</div>
</div>
<!-- Advanced Color (RGB/HSV) -->
<h3>🌈 Advanced Color Channels (RGB / HSV)</h3>
<p>除了基本的Color Code，你也可以讀取完整的 RGB 或 HSV 數值，甚至單獨擷取其中一個通道（例如只抓取紅色或色相）。這在進階影像識別或色彩過濾時非常實用。</p>
<div class="code-example">
<h4>Read full RGB and HSV tuples <code>get_color_rgb(port)</code> 與 <code>get_color_hsv(port)</code></h4>
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_color_rgb] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_color_hsv] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Read RGB, returns (R, G, B), range 0~100
rgb = exp6.get_color_rgb(3)

# Read HSV, returns (H, S, V), H range 0~359, S/V range 0~100
hsv = exp6.get_color_hsv(3)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_color_rgb] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_color_hsv] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
rgb = await exp6.get_color_rgb(3)
hsv = await exp6.get_color_hsv(3)
```


</div>
</div>
<div class="code-example">
<h4>Single Channel Extraction <code>get_color_red(port)</code> 等</h4>
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_color_red] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_color_hue] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Read RGB channel individually (range 0~100)
r = exp6.get_color_red(3)
g = exp6.get_color_green(3)
b = exp6.get_color_blue(3)

# Read HSV channel individually (H: 0~359, S/V: 0~100)
h = exp6.get_color_hue(3)
s = exp6.get_color_sat(3)
v = exp6.get_color_val(3)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_color_red] (3) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_color_hue] (3) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
r = await exp6.get_color_red(3)
g = await exp6.get_color_green(3)
b = await exp6.get_color_blue(3)

h = await exp6.get_color_hue(3)
s = await exp6.get_color_sat(3)
v = await exp6.get_color_val(3)
```


</div>
</div>
<!-- Ultrasonic & Force -->
<h3>📏 Ultrasonic & Force Sensors</h3>
<div class="code-example">
<h4>Distance <code>get_ultrasonic_distance(port)</code> and Force Depth <code>get_touch_force(port)</code></h4>
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_ultrasonic_distance] (4) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_touch_force] (5) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# 讀取超音波Distance (單位：毫米 mm)
dist = exp6.get_ultrasonic_distance(4)

# Read touch sensor press force (0~100)
force = exp6.get_touch_force(5)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_ultrasonic_distance] (4) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_touch_force] (5) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
dist = await exp6.get_ultrasonic_distance(4)
force = await exp6.get_touch_force(5)
```


</div>
</div>
<!-- System State & Raw -->
<h3>🔋 System & Raw State (System &amp; Raw)</h3>
<p>Check what devices are plugged in, battery voltage, or grab raw bottom-layer data for debugging.</p>
<div class="code-example">
<h4>Get ID <code>get_device_id(port)</code>、Raw Data <code>get_port_raw(port)</code> and Voltage <code>get_voltage()</code></h4>
<ul>
<li><code>port</code>: Port on the expander (1~6)</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [call v] [get_device_id] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_voltage] ‹ › :: #FF6680)
(@greenFlag [exp6 v] [call v] [get_port_raw] (1) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Returns device ID: 0=None, 1=Force, 3=Color, 4=Ultrasonic, 5=Motor
dev_id = exp6.get_device_id(1)

# Returns current battery voltage (e.g., 8.25 V)
volts = exp6.get_voltage()

# Get unparsed raw data from port 1 (uint16)
raw_data = exp6.get_port_raw(1)
```


</div>
<div class="block-label" style="margin-top: 1rem;">Async Block</div>
<pre v-pre class="blocks">(@greenFlag [exp6 v] [await v] [get_device_id] (1) ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_voltage] ‹ › :: #FF6680)
(@greenFlag [exp6 v] [await v] [get_port_raw] (1) ‹ › :: #FF6680)</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Async模式</span><span class="code-badge async">Async</span></div>


```python
dev_id = await exp6.get_device_id(1)
volts = await exp6.get_voltage()
raw_data = await exp6.get_port_raw(1)
```


</div>
</div>
</section>

<section id="heartbeat">
<h1>💓 4. System Heartbeat (Keep Alive) (Heartbeat)</h1>
<p>EXP6 擴充板需要與主控端維持通訊的「心跳訊號」。如果您在初始化時設定了 <code>multitask=True</code>，系統會在背景自動發送心跳，您不需要使用以下指令。但如果您使用Normal模式 (<code>multitask=False</code>)，請務必使用以下指令來維持通訊，否則擴充板0.5秒內沒有收到心跳訊號會自動關閉馬達電力輸出。</p>
<!-- keep_alive_wait -->
<h3>⏳ Heartbeat Wait (Wait)</h3>
<p>在Normal模式下，請使用這個指令來取代原本系統的 <code>wait</code> 或 <code>sleep</code>，它會在等待期間自動幫你維持與擴充板的通訊。</p>
<div class="code-example">
<h4><code>exp6.keep_alive_wait(ms)</code></h4>
<ul>
<li><code>ms</code>: Wait time (milliseconds)</li>
</ul>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [keep_alive_wait] (1000) ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Wait 1 second (1000 ms), sending heartbeats automatically
exp6.keep_alive_wait(1000)
```


</div>
</div>
<!-- keep_alive -->
<h3>💓 Manual Keep Alive (Keep Alive)</h3>
<p>If you have a time-consuming <code>while</code> or <code>for</code> loop, call this inside the loop to ensure the connection isn't dropped.</p>
<div class="code-example">
<h4><code>exp6.keep_alive()</code></h4>
<div class="block-label">Normal Block</div>
<pre v-pre class="blocks">@greenFlag [exp6 v] [call v] [keep_alive] ‹ › :: #FF6680</pre>
<div class="code-block-wrapper">
<div class="code-header"><span>Normal模式</span><span class="code-badge sync">Normal</span></div>


```python
# Manually send heartbeat in a long loop
while True:
    # Perform heavy calculations or sensing tasks...
    
    # Ensure communication is not interrupted
    exp6.keep_alive()
```


</div>
</div>
</section>

<section id="advanced">
<h1>🧠 5. Advanced Exploration (Advanced)</h1>
<p>This section is for advanced developers wanting to understand the library's underlying principles.</p>
<h3>Functional API (Functional API)</h3>
<p>In fact, the <code>MBC_EXP6</code> object calls a series of global functions underneath. If you prefer not to instantiate an object, you can import these functions directly. They are typically named with an <code>exp_</code> prefix:</p>
<div class="code-example">
<h4>Importing and Using the Functional API</h4>
<div class="code-block-wrapper">
<div class="code-header"><span>Synchronous Syntax</span></div>


```python
from MBC_uart_Lib import exp_init, exp_motor_power, exp_get_motor_angle

# Direct initialization (port=3, multitask=False)
exp_init(3, False)

# Power the motor on port 5
exp_motor_power(5, 50)

# Read angle of port 5
angle = exp_get_motor_angle(5)

```


</div>
</div>
</section>
</div>
</section>
