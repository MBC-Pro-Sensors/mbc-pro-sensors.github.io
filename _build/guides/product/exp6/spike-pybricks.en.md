### Wiring and power

1. **Host cable**: plug the expander's **Host cable** into any SPIKE hub port (A~F).
2. **Separate power**: the expander must have its own power supply (battery box or adapter) for the motors; motor current is not drawn from the hub, and the hub signal lines go through an isolator.
3. **Devices**: plug LEGO sensors (colour, ultrasonic, force) and motors into any of the expander's ports **1~6**.

> [!WARNING]
> The EXP6 **supports SPIKE Prime only** (not Robot Inventor or EV3). **Before switching to another programming method, power the EXP6 off once** (turn off the hub or unplug the Host cable): once it has run a Pybricks program it keeps talking the Pybricks way until it restarts, and the official App cannot connect.

### Import the EXP6 library

1. Download [📥 Pybricks library v2 + examples (.zip)](/downloads/MBC_EXP6_Pybricks_Lib_v2.zip) (v1 firmware shipped before 2026-10: [legacy zip](/downloads/MBC_EXP6_Pybricks_Lib.zip)). Use **Pybricks firmware v4.1** (the command manual says "4.0.0 and later"; v4.1 works the same).
2. Put the library in your Pybricks project. The v2 pack's library is `MBC_exp6_obj_Lib_v2.py` and its examples say `from MBC_exp6_obj_Lib_v2 import MBC_EXP6`, so no renaming is needed.

![EXP6 Pybricks example](/images/sensors/exp6/exp6_pybricks_example.webp)

### Normal mode and multitask mode

Assuming the expander is on hub **port C (3)**:

```python
# Normal mode: one line after another
from MBC_exp6_obj_Lib_v2 import MBC_EXP6
exp6 = MBC_EXP6(3, False)
exp6.motor_power(6, 100)
exp6.keep_alive_wait(5000)
```

```python
# Multitask mode: await in front of every command, use the library's run_task
from pybricks.tools import multitask, run_task, wait
from MBC_exp6_obj_Lib_v2 import MBC_EXP6, run_task
exp6 = MBC_EXP6(3, True)

async def main():
    await exp6.motor_power(6, 100)
    await exp6.keep_alive_wait(5000)

run_task(main())
```

> [!IMPORTANT]
> **Pybricks has a watchdog**: if the expander hears nothing from the hub for about **0.5 s** (disconnect or crash), the motors coast to a stop. In multitask mode the library's `run_task` sends the heartbeat in the background; in normal mode, wait with `exp6.keep_alive_wait(ms)` instead of `wait`, and call `exp6.keep_alive()` inside long loops.
