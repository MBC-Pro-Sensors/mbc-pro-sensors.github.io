<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Pybricks Environment (Servo6)

[🔙 Back to Servo6](/en/sensors/servo6/index.md)

> [!WARNING]
> **You are reading the Pybricks guide**, for SPIKE Prime hubs running Pybricks firmware **v4.0 or later**. Using the official SPIKE App instead? [Switch to the official app guide](/en/sensors/servo6/spike-official.md).

## 📥 Download the library and example

[📥 Pybricks library + example (.zip)](/downloads/MBC_Servo6_Pybricks_Lib.zip) → put `MBC_servo6_Lib.py` in your Pybricks project next to the main program, then `import` it.

## 🔑 What happens on the first run

When Pybricks opens a `UARTDevice`, the hub no longer treats the expander as a sensor that needs power, so the library opens the port with `UARTDevice(port, 115200, power_pin=1)` to ask the hub for battery power. The **first time** the console shows:

```
Custom electronics may damage your LEGO hub ... Press Y to proceed.
```

Press **Y** once (the hub remembers it). If you press N the servos get no power.

## 📚 Sync style (one line after another)

```python
from pybricks.tools import wait
from MBC_servo6_Lib import MBC_SERVO6

sv = MBC_SERVO6(1, False)      # expander on Port A (1=A ... 6=F), False = sync mode

sv.servo_angle(1, 90)          # CN1 to 90° (first command moves immediately)
wait(1000)
sv.servo_angle(1, 0, 10)       # to 0° at 60 °/s (10×6)
sv.servo_wait(1)               # wait until it arrives
sv.servo_angle(1, 180, 10)
while sv.is_moving(1):
    print("CN1 mA:", sv.get_current(1), " total mA:", sv.get_total_current())
    wait(100)

sv.servo_run(2, 50)            # CN2 is a 360° servo: half speed forward
wait(2000)
sv.servo_run(2, -50)           # half speed reverse
wait(2000)
sv.servo_run(2, 0)             # stop

print("CN1 stalled:", sv.is_stalled(1), " eFuse fault:", sv.is_power_fault())
sv.stop_all()                  # release all servos before the program ends
```

## ⚡ Async style (multitask)

```python
from pybricks.tools import wait, run_task
from MBC_servo6_Lib import MBC_SERVO6, run_task   # the library's run_task releases the servos when the program ends

sv = MBC_SERVO6(1, True)       # True = multitask, every call needs await

async def gripper():
    await sv.servo_angle(1, 30, 20)
    await sv.servo_wait(1)
    await sv.servo_angle(1, 120, 20)

async def main():
    await gripper()

run_task(main())
```

## 📐 Functions

| Function | What it does |
| :--- | :--- |
| `MBC_SERVO6(port, multitask)` | Create the object; `port` 1-6 = Port.A-F; with `multitask` True add `await` to every call |
| `servo_angle(ch, angle, speed=100)` | Move to angle; `speed` 1-99 = `speed×6` °/s, 100 = immediate |
| `servo_wait(ch, timeout_ms=10000)` | Wait until it arrives |
| `servo_run(ch, speed)` | 360° servo: -100 to 100, 0 stops |
| `servo_speed_config(ch, stop_us=1500, span_us=1000)` | Stop pulse and full-speed offset for 360° servos |
| `servo_pulse(ch, pulse_us)` | Raw pulse width 500-2500 µs |
| `servo_release(ch)` / `stop_all()` | Release one / all |
| `servo_calibrate(ch, min_us=500, max_us=2500, range_deg=180)` | Pulse and angle range |
| `servo_stall_config(ch, current_ma=400, time_ms=300, auto_release=True)` | Stall threshold (0 = off) |
| `get_current(ch)` / `get_total_current()` | Current in mA |
| `is_moving(ch)` / `is_stalled(ch)` / `is_active(ch)` / `is_power_fault()` | Status |

> [!TIP]
> The expander has **no watchdog**: servos hold their last position after the program ends. Call `stop_all()` at the end to release them (the library's `run_task` does it for you in async style). After an automatic stall release, sending the **same angle** again does nothing (duplicate commands are ignored); send a different angle or `servo_release()` first.
