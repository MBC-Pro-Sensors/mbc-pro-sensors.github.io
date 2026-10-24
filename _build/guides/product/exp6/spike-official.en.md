### Wiring and power

1. **Host cable**: plug the expander's **Host cable** into any SPIKE hub port (A~F).
2. **Separate power**: the expander must have its own power supply (battery box or adapter) for the motors; motor current is not drawn from the hub, and the hub signal lines go through an isolator.
3. **Devices**: plug LEGO sensors (colour, ultrasonic, force) and motors into any of the expander's ports **1~6**.

> [!WARNING]
> The EXP6 **supports SPIKE Prime only** (not Robot Inventor or EV3). **Before switching to another programming method, power the EXP6 off once** (turn off the hub or unplug the Host cable): once it has run a Pybricks program it keeps talking the Pybricks way until it restarts, and the official App cannot connect.

### Install the EXP6 library (only once)

The official App has no EXP6 blocks; **Python text mode only** (SPIKE 3; the library is marked for version 3.4.3). The library is plain Python without multitasking (async); every command runs **synchronously**.

1. Download [📥 official App library v2 + examples (.zip)](/downloads/MBC_EXP6_Official_App_Lib_v2.zip) (v1 firmware shipped before 2026-10: [legacy zip](/downloads/MBC_EXP6_Official_App_Lib.zip)).
2. Paste the **install script** from the `SPIKE_Education` folder of the download (`Install_MBC_exp6_Lib…py`) into a SPIKE App Python project and run it once. The script writes the core library into the hub.
3. After that, any new project can simply `from MBC_exp6_SPIKE_App_Lib import MBC_EXP6`; no more pasting long source code.
4. When you no longer need it, run the **uninstall script** (`Uninstall_MBC_exp6_Lib…py`) from the download to remove the library from the hub cleanly.

![Running the install script in the SPIKE App](/images/sensors/exp6/exp6_official_app_install.webp)

### Create the connection object

Assuming the EXP6 is on hub **port F**:

```python
from hub import port
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.F)   # expander on hub port F
time.sleep(1)             # wait 1 s for the expander to be ready

exp6.motor_power(1, 50)
time.sleep(3)
exp6.stop_all(1)          # always stop all motors before the program ends (1 = coast)
```

> [!IMPORTANT]
> **No watchdog in the official App**: if the program crashes or the hub disconnects, the motors keep the last command. Always end with `exp6.stop_all(1)`; wait with plain `time.sleep()`, no heartbeat command needed.
