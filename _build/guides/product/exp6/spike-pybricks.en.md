### Wiring and power

1. **Host cable**: plug the expander's **Host cable** into any SPIKE hub port (A~F).
2. **Separate power**: the expander must have its own power supply (battery box or adapter) for the motors; motor current is not drawn from the hub, and the hub signal lines go through an isolator.
3. **Devices**: plug LEGO sensors (colour, ultrasonic, force) and motors into any of the expander's ports **1~6**.

> [!WARNING]
> With this software the EXP6 works with the **SPIKE Prime** hub only (on a Robot Inventor (51515) hub use [Python in the Robot Inventor App](/en/learn/exp6/robot-inventor.html); not EV3). **Before switching to another programming method, power the EXP6 off once** (turn off the hub or unplug the Host cable): once it has run a Pybricks program it keeps talking the Pybricks way until it restarts, and the official App cannot connect.

### Import the EXP6 library

1. Download [📥 Pybricks library + examples (.zip)](/downloads/MBC_EXP6_Pybricks_Lib.zip). The EXP6 needs firmware dated 2026-11-03 or later; use **Pybricks firmware v4.1** on the hub. The function reference is `EXP6_Library_Manual.pdf` in the pack.
2. Put the files you need in the same Pybricks project (no renaming needed):
   - **Always**: `MBC_exp6_obj_Lib.py` (EXP6, motors, LEGO sensors). The pack's examples say `from MBC_exp6_obj_Lib import MBC_EXP6`.
   - **Only with MBC sensors**: `MBC_exp6_mbc_common.py`, a shared file that the product files below load automatically. Add the product files you use: `MBC_exp6_line.py` (Line Follower 8/16-ch), `MBC_exp6_laser.py` (Laser Ranger 2/8-ch), `MBC_exp6_pad.py` (PS2/PS4 receiver), `MBC_exp6_unicam.py` (SharpShot Vision).
   - **Leave out the rest**: a Pybricks hub program (your code plus all libraries) is limited to about 52 KB, and all the files together are too big.
   - **Pybricks blocks**: the pack's `beginner_examples/` folder has 5 block examples, also as single downloads: [ex01 one turn](/examples/exp6/spike_beginner/pybricks/ex01_one_turn_block.py), [ex02 three motors](/examples/exp6/spike_beginner/pybricks/ex02_three_motors_block.py), [ex03 light switch](/examples/exp6/spike_beginner/pybricks/ex03_light_switch_block.py), [ex04 button control](/examples/exp6/spike_beginner/pybricks/ex04_button_motor_block.py), [ex05 square](/examples/exp6/spike_beginner/pybricks/ex05_square_block.py). In Pybricks Code click "Import a file", pick the file and keep it in the same project as `MBC_exp6_obj_Lib.py`; it opens as blocks. Any wait block turns the program into multitask mode (`MBC_EXP6(3, True)`, every command is `await`ed), so the examples add one more block, "from MBC_exp6_obj_Lib import run_task": it sends the heartbeat in the background while waiting, so the EXP6 watchdog does not stop the motors.

![EXP6 Pybricks example](/images/sensors/exp6/exp6_pybricks_example.webp)

### Normal mode and multitask mode

Assuming the expander is on hub **port C (3)**:

```python
# Normal mode: one line after another
from MBC_exp6_obj_Lib import MBC_EXP6
exp6 = MBC_EXP6(3, False)
exp6.motor_power(6, 100)
exp6.keep_alive_wait(5000)
```

```python
# Multitask mode: await in front of every command, use the library's run_task
from pybricks.tools import multitask, run_task, wait
from MBC_exp6_obj_Lib import MBC_EXP6, run_task
exp6 = MBC_EXP6(3, True)

async def main():
    await exp6.motor_power(6, 100)
    await exp6.keep_alive_wait(5000)

run_task(main())
```

> [!IMPORTANT]
> **Pybricks has a watchdog**: if the expander hears nothing from the hub for about **0.5 s** (disconnect or crash), the motors coast to a stop. In multitask mode the library's `run_task` sends the heartbeat in the background; in normal mode, wait with `exp6.keep_alive_wait(ms)` instead of `wait`, and call `exp6.keep_alive()` inside long loops.

### MBC sensors on the EXP6

When you run out of hub ports, you can plug MBC sensors into EXP6 ports 1–6: **Line Follower 8/16-ch, Laser Ranger 2/8-ch, PS2/PS4 receiver and SharpShot Vision**. The function names and return values are the same as when the sensor is plugged straight into the hub.

- Just plug the sensors in, the same as into the hub; there is nothing to set up.
- The EXP6 needs firmware from **2026-11-03 or later**. For older firmware, contact us to update it.

**1. Normal mode: declare what is on each port.** When you create the EXP6 object, write `p5=LINE16` (Line Follower 16-ch on port 5). For the camera, write `(UNICAM, camera_mode)`. Then get each sensor object with `exp6.sensor(port)`.

- When the program starts, the library checks that every port has the declared sensor.
- A wrong sensor stops the program with a message naming what was found, for example "port 5 should be Line16 but reads Laser2".
- The camera switches to the declared mode right away.

```python
from MBC_exp6_obj_Lib import MBC_EXP6
from MBC_exp6_line import *       # Line Follower LINE8, LINE16
from MBC_exp6_unicam import *     # SharpShot Vision UNICAM

# EXP6 on hub port C (3); Line Follower 16-ch on EXP6 port 5, camera on port 6 (mode 51, object detection)
exp6 = MBC_EXP6(3, False, p5=LINE16, p6=(UNICAM, 51))
line = exp6.sensor(5)
cam = exp6.sensor(6)

while True:
    print("pos100 =", line.pos100(), " objects =", cam.object_count())
    exp6.keep_alive_wait(100)     # in normal mode, wait with keep_alive_wait
```

**2. Multitask mode.** Declare the sensors the same way. `exp6.sensor()` needs no `await`, but it must come before `run_task`. Multitask mode does not check the sensors when you declare them.

- Switch the camera mode yourself once, with `await cam.set_mode(mode)` inside `main()`.
- Put `await` in front of every sensor function.
- For the receiver, call `pad.update()` once per loop. The buttons and sticks in that loop all use that one reading, the same as on the hub.
- `buttons()` uses the same bit order as on the hub. The old PSX order is now called `buttons_psx()`.

```python
from pybricks.tools import multitask, run_task, wait
from MBC_exp6_obj_Lib import MBC_EXP6, run_task
from MBC_exp6_pad import *        # receiver PS2, PS4
from MBC_exp6_unicam import *     # SharpShot Vision UNICAM

exp6 = MBC_EXP6(3, True, p4=PS4, p6=(UNICAM, 13))   # EXP6 on hub port C (3)
pad = exp6.sensor(4)              # no await needed, but do it before run_task
cam = exp6.sensor(6)

async def main():
    await cam.set_mode(13)        # multitask: switch the camera mode yourself once
    while True:
        await pad.update()        # read the controller once per loop; buttons and sticks below use this reading
        if await pad.pressed("CROSS"):
            break
        print("LX =", await pad.lx(), " colour 1 at", await cam.xy(1))
        await wait(50)
    print("done")

run_task(main())
```

**3. The old style still works.** You can still write `exp6.line16(5)`, `exp6.laser8(3)`, `exp6.ps2(4)` and `exp6.unicam(5, 13)`, but they do not check which sensor is plugged in. **Old programs need one extra import line for the product file**, for example `from MBC_exp6_line import *`. Without it, the program tells you which line to add.

The pack's `examples/11_MBC_Sensors.py` shows the Line Follower 8-ch, Laser Ranger 8-ch, PS2 receiver and camera together.

**4. With blocks.** Blocks only take values in order, so create the sensor object as `EXP6_LINE8(exp6, 5)` (first slot the EXP6 object, second the EXP6 port) and add one more import block for the product file. Example: PD line following with a Line Follower 8-ch in EXP6 port 5, [block file](/examples/exp6/spike_beginner/pybricks/exp6_line8_pd_block.py) and [Python file](/examples/exp6/spike_beginner/pybricks/exp6_line8_pd_python.py) (left motor in EXP6 port 1, right motor in port 2); step-by-step in the [EXP6 Pybricks blocks tutorial](/en/learn/exp6/pybricks-blocks.html#mbc-sensors).

> [!NOTE]
> Hardware-tested on 2026-11-03: EXP6 examples P1–P5, example 11 and MBC SPIKE Blocks examples M1–M5. All 11 passed.
