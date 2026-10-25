> [!TIP]
> **🧩 New to the Servo6? Follow the step-by-step interactive tutorial:** [LEGO SPIKE App](/en/learn/servo6/spike-app.html). Pick options on the left and watch the blocks build up on the right; every function has an example, and a simulated sensor lets you try the values.

> [!NOTE]
> Servo6 is **coming soon**: circuit and firmware are done and final hub testing is under way; specifications follow the release version. **SPIKE Prime only** (not Robot Inventor or EV3).

### Wiring and power

1. **Host cable**: plug the expander's SPIKE cable into any hub port (A~F).
2. **Servos**: plug the 3-wire servo connectors into **CN1~CN6** following the board markings (signal / VCC / GND). Servos with 500~2500 µs signals such as SG90 and MG996R work.
3. **No battery needed**: after the handshake the hub sends battery voltage to the expander and the green LED changes from fast blinking to steady. Total servo current is limited to **about 0.75 A**; check the working current of high-torque servos first.

> [!TIP]
> At power-on the expander outputs no pulses, so servos do not twitch. The first angle command moves straight to position (no speed ramp); later commands use the speed you set.

### Add the library

The official App supports **Python text mode only**, not blocks.

1. Download [📥 official SPIKE App library + examples (.zip)](/downloads/MBC_Servo6_Official_App_Lib.zip).
2. Official App Python projects cannot hold extra files, so paste **the whole content of `MBC_servo6_SPIKE_App_Lib.py` at the top of your program**, then write your own code below it.

```python
from hub import port
import time
# ↑ paste the content of MBC_servo6_SPIKE_App_Lib.py above this line
sv = MBC_SERVO6(port.A)        # expander on port A
sv.servo_angle(1, 90)          # CN1 to 90°
time.sleep_ms(1000)
sv.stop_all()
```

The expander pretends to be a **SPIKE ultrasonic sensor**: commands go out through the ultrasonic sensor's "4 light values", and status comes back through `device.data()`. Each command waits one system polling cycle (about 20~50 ms), so controlling 6 servos in a row takes about 6 cycles.

> [!TIP]
> When an official App program ends, the servos are not released (they hold their last position and do not run away); call `stop_all()` at the end of the program to release them.
