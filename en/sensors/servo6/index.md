<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# SPIKE 6-Port Servo Expander (Servo6) (Coming Soon)

<!-- product-hero -->

---

> [!NOTE]
> Circuit and firmware are done; final tests with the hub are in progress, so specifications may change at launch. [Contact us](/en/contact.md) for early access or a quote.

> [!WARNING]
> **⚠️ Check before you buy**
>
> | Item | Note |
> | :--- | :--- |
> | **Compatible hub** | SPIKE™ Prime only (not Robot Inventor or EV3) |
> | **Official SPIKE App** | Python text mode only, no block coding |
> | **Pybricks** | Requires Pybricks firmware **v4.0 or later**; on the first run press **Y** in the console to allow power |
> | **Servo power** | Supplied by the hub battery through the expander, **about 0.75 A total**; check the running current of high-torque servos first |

---

## 🚀 Overview: servos on SPIKE

LEGO SPIKE Prime has no servo motors, yet grippers, turntables and arms that "turn to an angle and hold" are smallest and cheapest with hobby servos.

The **SPIKE 6-Port Servo Expander (Servo6)** plugs into any hub port and drives **six standard 3-wire servos** (SG90, MG996R and similar, 500-2500 µs signal). Angle, speed, current readback and stall protection are computed on the expander; the hub only sends commands.

## 🧠 Hardware highlights

- ⚡ **No external battery**: servo power comes from the hub battery (M1/M2 supply line) through a rectifier and an electronic fuse (eFuse), so there is no battery box to carry.
- 🛡️ **Per-channel current sensing**: each of the six channels has its own shunt; read every servo's current at any time. Current above a threshold for a set time counts as a stall and can release that channel automatically to protect servo and gears.
- 🔌 **Isolated from the hub**: digital isolator on the signal line and an isolated DC-DC for the controller; a short or spike on the servo side cannot reach the hub.
- 🎯 **Both servo types**: positional servos use "move to angle" (speed 1-99 = 6-594 °/s, 100 = immediate); 360° continuous servos use "run" (-100 to 100).
- 🔧 **Calibratable**: min/max pulse width and angle range per channel (default 500-2500 µs, 180°), or send a raw pulse width.
- 🧷 **No heartbeat needed**: every command sets a state; when the hub program ends the servos hold their last position, nothing runs away.

## 🔌 Wiring and start-up

1. **Hub cable**: plug the expander's SPIKE cable into any hub port (A-F).
2. **Servos**: plug the 3-pin servo connectors into CN1-CN6 as marked (signal / VCC / GND).
3. **No battery**: once the hub finishes the handshake it sends battery voltage to the expander; the green LED changes from fast blinking to steady.
4. **Program it**: see [Official SPIKE App](/en/sensors/servo6/spike-official.md) or [Pybricks](/en/sensors/servo6/spike-pybricks.md).

> [!TIP]
> At power-on the expander outputs no pulses, so servos do not twitch; the first angle command moves directly to the target (no speed ramp), later commands use the speed you set.

## 📐 Commands (same names in both environments)

| Function | What it does |
| :--- | :--- |
| `servo_angle(port, angle, speed=100)` | Move to 0-180° (within calibration); `speed` 1-99 = `speed×6` °/s, 100 = immediate |
| `servo_wait(port, timeout_ms=10000)` | Wait until the channel reaches the target |
| `servo_run(port, speed)` | 360° continuous servo: -100 to 100, 0 stops |
| `servo_pulse(port, pulse_us)` | Raw pulse width 500-2500 µs (calibration or special servos) |
| `servo_release(port)` / `stop_all()` | Release one channel / all channels (stop pulses) |
| `servo_calibrate(port, min_us, max_us, range_deg)` | Pulse and angle range for the channel |
| `servo_stall_config(port, current_ma, time_ms, auto_release)` | Stall threshold (default 400 mA for 300 ms, auto release; final values to be confirmed on hardware) |
| `get_current(port)` / `get_total_current()` | Channel current / total current in mA |
| `is_moving(port)` / `is_stalled(port)` / `is_active(port)` | Moving / stall flag / pulses active |
| `is_power_fault()` | Whether the eFuse has tripped |

## 🧩 Supported platforms

| Platform | How |
| :--- | :--- |
| LEGO SPIKE Prime (official app) | Python text mode + [library](/en/sensors/servo6/spike-official.md) |
| LEGO SPIKE Prime (Pybricks v4.0+) | [Library](/en/sensors/servo6/spike-pybricks.md), sync and async (multitask) styles |
| Robot Inventor, EV3 | Not supported |

## 📥 Downloads

- [📥 Official SPIKE App library + example (.zip)](/downloads/MBC_Servo6_Official_App_Lib.zip)
- [📥 Pybricks library + example (.zip)](/downloads/MBC_Servo6_Pybricks_Lib.zip)
