<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Official SPIKE App Section (EXP6)

<div style="background: rgba(242,194,0,0.1); border: 1px solid rgba(242,194,0,0.3); border-radius: 8px; padding: 15px 20px; margin-bottom: 20px;">
<div style="display: flex; align-items: center; gap: 15px;">
<img src="/images/hubs/spike-education-app.webp" alt="SPIKE App" style="height: 50px; filter: drop-shadow(0 0 10px rgba(242,194,0,0.3));" />
<div style="color: #ccc;">
<strong>Suitable for:</strong> Beginners, educators, and users accustomed to standard Python logic.<br>
<strong>Characteristics:</strong> Easy to read, object-oriented design, synchronously executed commands.
</div>
</div>
</div>

> If you are using the Pybricks Pro environment, please click [here to switch to the Pybricks tutorial](/en/sensors/exp6/spike-pybricks.md).

---

## 📥 Download Dedicated Library and Examples

> [!TIP]
> **How to install the library?** Before running any expansion board programs, please make sure to copy and paste the `Install_MBC_exp6_Lib.py` code from the downloaded package into your SPIKE App and run it once. This installation script will automatically write the core library into the SPIKE hub's internal firmware. After successful installation, you can directly use `from MBC_exp6_SPIKE_App_Lib import MBC_EXP6` in any new project without having to copy and paste a long string of source code!

<div style="display: flex; flex-wrap: wrap; gap: 20px; margin: 25px 0;">

<div style="flex: 1; min-width: 300px; background: rgba(0,210,255,0.05); border: 1px solid rgba(0,210,255,0.3); border-radius: 12px; padding: 25px; box-shadow: 0 8px 20px rgba(0,0,0,0.2);">
<h3 style="margin-top: 0; color: #00d2ff; display: flex; align-items: center; gap: 10px;">
<svg viewBox="0 0 24 24" style="width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:2;"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4M7 10l5 5 5-5M12 15V3"/></svg>
Download Official App Library
</h3>
<p style="color: #aaa; margin-bottom: 20px;">Contains the installation script, uninstallation script, and basic example files for the official SPIKE App.</p>
<a href="/downloads/MBC_EXP6_Official_App_Lib.zip" target="_blank" style="display: inline-block; background: linear-gradient(135deg, #00d2ff 0%, #0077ff 100%); color: white; text-decoration: none; padding: 12px 25px; border-radius: 30px; font-weight: bold; box-shadow: 0 4px 15px rgba(0,210,255,0.4); transition: transform 0.2s, box-shadow 0.2s;">
Download .zip Package
</a>
<p style="font-size: 0.85em; color: #888; margin-top: 15px;">Includes: <code>Install_MBC_exp6_Lib.py</code>, <code>Uninstall_MBC_exp6_Lib.py</code>, <code>example.py</code></p>
</div>

<div style="flex: 1; min-width: 300px; background: rgba(255,69,0,0.05); border: 1px solid rgba(255,69,0,0.3); border-radius: 12px; padding: 25px; box-shadow: 0 8px 20px rgba(0,0,0,0.2);">
<h3 style="margin-top: 0; color: #ff4500; display: flex; align-items: center; gap: 10px;">
<svg viewBox="0 0 24 24" style="width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:2;"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg>
Uninstall Library
</h3>
<p style="color: #aaa; margin-bottom: 20px;">If you no longer need the library or need to free up the hub's internal storage space, please run the uninstallation script.</p>
<div style="background: rgba(0,0,0,0.3); padding: 10px 15px; border-radius: 8px; border-left: 3px solid #ff4500;">
<p style="margin: 0; font-size: 0.9em; color: #ccc;">Run <code>Uninstall_MBC_exp6_Lib.py</code> from the downloaded package in the SPIKE App to cleanly remove the library from the firmware.</p>
</div>
</div>

</div>

<div style="margin: 30px 0; border: 1px solid rgba(255,255,255,0.1); border-radius: 12px; overflow: hidden; box-shadow: 0 10px 30px rgba(0,0,0,0.5);">
<div style="background: rgba(255,255,255,0.05); padding: 10px 20px; border-bottom: 1px solid rgba(255,255,255,0.1); font-family: monospace; color: #aaa; font-size: 0.9em;">Installation Demonstration</div>
<img src="/images/sensors/exp6/exp6_official_app_install.webp" alt="Installation Demonstration" style="width: 100%; display: block;" />
</div>

---

## 📚 1. Quick Start (Initialization)

To use the EXP6 expander, you must first import the library and tell it which port on the SPIKE hub the expander is connected to.

```python
import hub
# Import the installed core object
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

# Connect to the expander (assuming the expander is plugged into port A)
# The second parameter True indicates multitask mode (heartbeat is sent automatically in the background)
exp6 = MBC_EXP6(hub.port.A, True)

```

---

## ⚙️ 2. Motor Control

Control SPIKE motors connected to the EXP6 expander. Supports setting RPM, PID closed-loop control, braking, PID parameter tuning, and dual motor synchronized drive.

### ⚡ Start Motor (Power)
Uses pure electrical power (open-loop PWM) to drive the motor. No speed compensation, but provides the most direct and fastest response.
```python
# Make the motor connected to port 5 of the expander rotate at 50% power
exp6.motor_power(5, 50)
```

### 🔄 Closed-loop Rotation (Run)
Uses PID closed-loop control. The expander automatically adjusts power based on load to ensure the RPM stays at the set value.
```python
# Make the motor on port 1 of the expander maintain a steady speed of 30
exp6.motor_run(1, 30)
```

### 🎯 Precise Angle Control
Set the motor to rotate a specific relative angle, or move to a specific absolute angle. The built-in PID control and angle accumulator ensure precise movement.

* `degrees`: Relative angle to move (e.g., 360 is one full rotation forward).
* `angle`: Absolute angle to move to (0~359, automatically finds the shortest path).
* `stop`: Number indicating the behavior after reaching the target: `1` Coast, `2` Brake, `3` Hold, `4` Continue.

```python
# Make motor on port 1 rotate 360 degrees (relative angle) at speed 30, then Hold (3)
exp6.motor_run_degrees(1, 30, 360, 3)

# Make motor on port 1 rotate to 90 degrees (absolute angle) at speed 30, then Hold (3)
exp6.motor_track_target(1, 30, 90, 3)
```

### 🛑 Stop and Brake
```python
exp6.motor_stop(1, 1)  # Motor power off, coast to a stop
exp6.motor_stop(1, 3)  # Hard brake, lock current position
exp6.stop_all(1)       # Emergency stop! Makes all motors on the expander coast to a stop
```

### 🚗 Dual Motor Sync Control (Drive)
Simultaneously and precisely controls two motors, perfect for building wheeled chassis robots (e.g., line-following cars).
```python
# Make motors on port 1 and port 2 move forward simultaneously at speed 50 and 40
exp6.drive(1, 2, 50, 40)

# Stop dual motors (coast to a stop)
exp6.drive_stop(1, 2)
```

---

## 📡 3. Sensor Reading

Read sensor data and motor status from the expander. The expander automatically determines the sensor type and returns the corresponding values.

### 📐 Motor Angle and Speed
```python
# Reset the angle of motor on port 1 to zero
exp6.reset_angle(1)

# Read accumulated angle, absolute angle, and speed (deg/s)
angle = exp6.get_motor_angle(1)
abs_angle = exp6.get_motor_abs_angle(1)
speed = exp6.get_motor_speed(1)
```

### 🎨 Color Sensor
```python
# Read color code (-1 is no color, others correspond to standard LEGO colors)
color_code = exp6.get_color_color(3)

# Read light reflection value (0~100, suitable for line following)
reflection = exp6.get_color_reflection(3)

# Read RGB, returns (R, G, B), range 0~100
rgb = exp6.get_color_rgb(3)
```

### 📏 Ultrasonic and Force Sensors
```python
# Read ultrasonic distance (unit: millimeters mm)
dist = exp6.get_ultrasonic_distance(3)

# Read touch sensor press force (0~100)
force = exp6.get_touch_force(4)
```

### 🔋 System State & Raw
```python
# Return device ID: 0=None, 1=Force, 3=Color, 4=Ultrasonic, 5=Motor
dev_id = exp6.get_device_id(1)

# Return current battery voltage (e.g., 8.25 V)
volts = exp6.get_voltage()

# Get the raw unparsed data of port 1 (Raw Data, uint16)
raw_data = exp6.get_port_raw(1)
```

---

## 💓 4. System Heartbeat (Keep Alive)

The EXP6 expander requires a "heartbeat signal" to maintain communication with the main hub. If you set `multitask=True` during initialization, the system will automatically send heartbeats in the background. However, if you use normal mode (`multitask=False`), you must use the following commands to maintain communication.

```python
# In normal mode, use this command to replace the system's wait or sleep
# It will automatically maintain communication with the expander during the wait time
exp6.keep_alive_wait(1000)

# If you have a time-consuming while or for loop, call this inside the loop
while True:
    # Perform heavy calculations or sensing tasks...
    
    # Ensure communication is not interrupted
    exp6.keep_alive()
```
