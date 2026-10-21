<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Official SPIKE App (Servo6)

[🔙 Back to Servo6](/en/sensors/servo6/index.md)

> [!WARNING]
> **You are reading the official SPIKE App guide**, for the SPIKE Prime official software (SPIKE 3) in **Python text mode**; block coding is not supported. Running Pybricks instead? [Switch to the Pybricks guide](/en/sensors/servo6/spike-pybricks.md).

## 📥 Download the library and example

[📥 Official SPIKE App library + example (.zip)](/downloads/MBC_Servo6_Official_App_Lib.zip)

A Python project in the official app cannot hold extra files, so **paste the whole content of `MBC_servo6_SPIKE_App_Lib.py` at the top of your program**, then write your own code below it.

## 🧠 How it works (no hub modification)

The expander poses as a **SPIKE distance (ultrasonic) sensor**: commands go out through the sensor's four light values, and status comes back as the 8 values of `device.data()`. The official OS keeps resending the last light payload in the background; every command sets a state, so resending is harmless. Each command waits one OS poll period (about 20-50 ms), so driving six servos in a row takes about six periods.

## 📚 Example

```python
from hub import port
import time
# paste MBC_servo6_SPIKE_App_Lib.py above this line, or import it as below
from MBC_servo6_SPIKE_App_Lib import MBC_SERVO6

sv = MBC_SERVO6(port.A)        # expander on Port A

sv.servo_angle(1, 90)          # CN1 to 90°
time.sleep_ms(1000)
sv.servo_angle(1, 0, 10)       # to 0° at 60 °/s
sv.servo_wait(1)
sv.servo_angle(1, 180, 10)
sv.servo_wait(1)
print("CN1 mA:", sv.get_current(1), " total mA:", sv.get_total_current())

sv.servo_run(2, 50)            # CN2 is a 360° servo: half speed forward
time.sleep_ms(2000)
sv.servo_run(2, -50)
time.sleep_ms(2000)
sv.servo_run(2, 0)

print("CN1 stalled:", sv.is_stalled(1), " eFuse fault:", sv.is_power_fault())
sv.stop_all()
```

## 📐 Functions

| Function | What it does |
| :--- | :--- |
| `MBC_SERVO6(port.X)` | Create the object, `port.A`-`port.F` |
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
> The official app does not release servos when the program ends (they hold their last position, nothing runs away); call `stop_all()` at the end if you want them released.
