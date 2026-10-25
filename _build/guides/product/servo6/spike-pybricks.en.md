> [!TIP]
> **🧩 New to the Servo6? Follow the step-by-step interactive tutorial:** [MicroPython](/en/learn/servo6/micropython.html) · [Pybricks blocks](/en/learn/servo6/pybricks-blocks.html). Pick options on the left and watch the code build up on the right; every function has an example, and a simulated sensor lets you try the values.

> [!NOTE]
> Servo6 is **coming soon**: circuit and firmware are done and final hub testing is under way; specifications follow the release version. **SPIKE Prime only** (not Robot Inventor or EV3).

### Wiring and power

1. **Host cable**: plug the expander's SPIKE cable into any hub port (A~F).
2. **Servos**: plug the 3-wire servo connectors into **CN1~CN6** following the board markings (signal / VCC / GND). Servos with 500~2500 µs signals such as SG90 and MG996R work.
3. **No battery needed**: after the handshake the hub sends battery voltage to the expander and the green LED changes from fast blinking to steady. Total servo current is limited to **about 0.75 A**; check the working current of high-torque servos first.

> [!TIP]
> At power-on the expander outputs no pulses, so servos do not twitch. The first angle command moves straight to position (no speed ramp); later commands use the speed you set.

### Add the library

1. The hub needs Pybricks firmware **v4.0 or later**.
2. Download [📥 Pybricks library + examples (.zip)](/downloads/MBC_Servo6_Pybricks_Lib.zip) and put `MBC_servo6_Lib.py` in your Pybricks project (next to the main program) so you can `import` it.

### Press Y on the first run

The library opens `UARTDevice(port, 115200, power_pin=1)` to ask the hub to send battery voltage to the expander. **On the first run** the console shows:

```
Custom electronics may damage your LEGO hub ... Press Y to proceed.
```

Press **Y** (only once; the hub remembers). If you press N, the servos get no power.

```python
from pybricks.tools import wait
from MBC_servo6_Lib import MBC_SERVO6
sv = MBC_SERVO6(1, False)      # expander on port A (1=A … 6=F), False = normal mode
sv.servo_angle(1, 90)          # CN1 to 90°
wait(1000)
sv.stop_all()                  # release all servos before the program ends
```

> [!TIP]
> The expander has **no watchdog**: after the program ends the servos hold their last position. Call `stop_all()` at the end to release them (the library's `run_task` does it automatically in multitask mode). After a stall auto-release, sending **the same angle** again does not re-power the servo; send a different angle first, or `servo_release()` first.
