### Which side is left

**Left and right are as seen from behind the sensor, looking the way it faces (left = J5, right = J3).** Facing the sensor, they are the other way round, so when you test with your hand, stand behind the sensor and look the same way it does. "Left is closer" and "turn left" in the examples follow this direction.

### Test results (Pybricks 4.1.2)

The SPIKE edition **passed hardware testing on Pybricks 4.1.2 (2026-11-02)**: readings, the library and the beginner examples were all run on a hub.

- When you read it directly with `ColorSensor`, sensor firmware from 2026-11 on gives a whole number from `reflection()` on Pybricks 4.x (for example 31); older firmware gives one decimal place (for example 30.8). Writing `int(...)` works with both, so just keep it.

<!-- tof2-cmd-examples -->

### Change the settings with commands (E0 ~ E4)

> [!TIP]
> **Tested with SPIKE Pybricks (2026-11-02)**: every command E0 ~ E4 was checked on the hub: valid parameters return "done", out-of-range ones return "failed" and leave the setting unchanged, long-range mode reads up to 200 cm, and after a power cut the settings are back to factory.

Use `cmd()` in the MBC library `MBC_laser2_obj_Lib.py` to send a command and `wait_done()` to wait until it is done (True = done, False = failed or no reply). **The official SPIKE App and the Pybricks built-in `ColorSensor` blocks cannot send commands**; the sensor firmware must be from 2026-11 or later.

```python
from MBC_laser2_obj_Lib import MBC_LASER2

laser2 = MBC_LASER2(3)          # 3 = port C
laser2.cmd(0xE0, 1)             # switch to long-range mode (up to 200 cm)
if laser2.wait_done():          # True = done
    print('long range on')
```

| Command | Parameter (out of range = failed) | What it does | When to use it | Example |
| :---: | :--- | :--- | :--- | :---: |
| `E0` ranging mode | 0 normal (factory, up to 120 cm) / 1 long range (up to 200 cm) / 2 accurate (every 100 ms) / 3 fast (every 20 ms) | Changes the mode and resets the timing budget and max distance to that mode's defaults | Long range to measure beyond 120 cm; accurate for steady numbers; fast for quick reactions | 1 |
| `E1` filter | 0 off / 1 median of the last 3 (factory) / 2 median of the last 5 | Removes an odd reading now and then | Numbers jumping around → 2; fastest reaction in a match → 0 | 2 |
| `E2` timing budget | 20 ~ 200 ms (0 = mode default) | How long each reading takes: shorter is faster, longer is steadier | Fast obstacle avoiding → 20; far and steady → 200 | 3 |
| `E3` factory reset | must be `0xA5` | Mode 0, filter 1, 33 ms, limit 120 cm | Send it at the start of a program to begin from a clean state | 4 |
| `E4` max distance | 10 ~ mode limit in cm (normal 120, long range 200; 0 = mode limit) | Anything beyond the limit counts as nothing, and the reading is the limit | React only to near things, not to people walking around far away | 5 |

- **On SPIKE, out of range = failed, setting unchanged** (`wait_done()` returns False); it is not clamped to the nearest value as on Arduino I2C.
- **Changing the mode (E0) resets the timing budget and the max distance**: to use E2 / E4 as well, send E0 first.
- **Settings are kept only in the sensor's memory**: unplugging the sensor or turning the hub off brings back the factory settings (normal mode, limit 120 cm), so send them again every time the program starts.
- **"Nothing" reads as the limit**: 120 in normal mode, 200 in long-range mode, or the limit you set with E4.
- **The same command with the same parameters is not done again** (even inside a loop it runs once); to run it again write `cmd(..., again=True)` (example 4).
- Each `wait_done()` adds a few tens of milliseconds, so **do not put it in a fast line-following or obstacle-avoiding loop**; set things up at the start of the program. E0, E2 and E3 re-configure the laser chips, which takes about 0.1 ~ 0.5 s.
- **The EV3 and MATRIX editions have no commands** (read only).

#### Command examples (5 programs)

For each one: sensor on **port C**; import the example and `MBC_laser2_obj_Lib.py` into Pybricks Code together, then run it. Change the setting with the hub's left / right buttons; the screen shows the current setting and the computer prints the readings. [📥 Download all (5 examples + library)](/examples/tof2/pybricks_cmd_en/tof2_pybricks_cmd_examples_en.zip)

**1. Ranging mode (E0)**

Put a white wall or big whiteboard about 150 cm ahead. Normal mode reads 120 (beyond the limit = nothing); press the right button to switch to 1 long range and it reads about 150.

[📄 Download cmd01_long_range.py](/examples/tof2/pybricks_cmd_en/cmd01_long_range.py) + [library](/examples/tof2/pybricks_cmd_en/MBC_laser2_obj_Lib.py)

```python
# Ranger 2-Way command example 01 (Pybricks, SPIKE Prime / Robot Inventor hub): ranging mode: normal / long range / accurate / fast
# Switch between the 4 ranging modes (command E0) with the left / right buttons on the hub; the screen shows the mode and the console prints the distances:
#   0 normal (factory, up to 120 cm), 1 long range (up to 200 cm), 2 accurate (every 100 ms, steadiest), 3 fast (every 20 ms, fastest)
# Setup: a white wall or big whiteboard about 150 cm in front of both lasers. Normal mode reads 120 (beyond the limit = nothing); mode 1 reads about 150
# Wiring: Ranger 2-Way → port C; upload it together with MBC_laser2_obj_Lib.py (keep both files in Pybricks Code)
# Only the SPIKE edition takes commands (sensor firmware 2026-11 or later); settings stay in the sensor memory and go back to factory when unplugged
#   cmd(code, parameter): send a command; wait_done(): wait until it is done, True = done, False = failed (parameter out of range) or no reply
#   Commands: E0 ranging mode, E1 filter, E2 timing budget, E3 factory reset (parameter 0xA5), E4 max distance
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = port C (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)

mode = 0
laser2.cmd(0xE3, 0xA5)              # Factory reset first (normal mode)
laser2.wait_done()
hub.display.number(mode)
t = StopWatch()

while True:
    pressed = hub.buttons.pressed()    # Check the buttons every 10 ms, so one press is enough
    if Button.RIGHT in pressed or Button.LEFT in pressed:
        mode = (mode + (1 if Button.RIGHT in pressed else 3)) % 4   # right button +1, left button -1
        laser2.cmd(0xE0, mode)        # Change the ranging mode (re-configures the laser chips, about 0.1~0.5 s)
        ok = laser2.wait_done()       # Wait until done: True = done
        hub.display.number(mode)
        print('mode', mode, ['normal', 'long range', 'accurate', 'fast'][mode], 'done' if ok else 'failed')
        while hub.buttons.pressed():   # Wait until the button is released
            wait(10)
    if t.time() >= 300:                # Print the distances every 0.3 s
        t.reset()
        print('left', laser2.left(), 'right', laser2.right(), 'cm | left', laser2.left_mm(), 'mm')
    wait(10)
```

**2. Filter (E1)**

Put a box or whiteboard about 50 cm ahead and keep it still. Every second it prints "max - min within 1 s" = how many mm it jumps; a bigger filter is steadier.

[📄 Download cmd02_filter.py](/examples/tof2/pybricks_cmd_en/cmd02_filter.py) + [library](/examples/tof2/pybricks_cmd_en/MBC_laser2_obj_Lib.py)

```python
# Ranger 2-Way command example 02 (Pybricks, SPIKE Prime / Robot Inventor hub): filter: steadier numbers
# Switch the filter (command E1) with the left / right buttons: 0 off, 1 median of the last 3 (factory), 2 median of the last 5
# Setup: a box or whiteboard about 50 cm in front of both lasers, kept still. Every second it prints "max - min within 1 s" = how many mm the number jumps; a bigger filter is steadier
# Wiring: Ranger 2-Way → port C; upload it together with MBC_laser2_obj_Lib.py (keep both files in Pybricks Code)
# Only the SPIKE edition takes commands (sensor firmware 2026-11 or later); settings stay in the sensor memory and go back to factory when unplugged
#   cmd(code, parameter): send a command; wait_done(): wait until it is done, True = done, False = failed (parameter out of range) or no reply
#   Commands: E0 ranging mode, E1 filter, E2 timing budget, E3 factory reset (parameter 0xA5), E4 max distance
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = port C (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)

filt = 1
laser2.cmd(0xE3, 0xA5)              # Factory reset first (filter 1)
laser2.wait_done()
wait(500)                        # Wait for fresh readings before counting
hub.display.number(filt)

while True:
    lo, hi = 99999, -1
    t = StopWatch()
    while t.time() < 1000:           # Keep reading for 1 s and remember the max and min
        mm = laser2.left_mm()
        lo = min(lo, mm)
        hi = max(hi, mm)
        pressed = hub.buttons.pressed()
        if Button.RIGHT in pressed or Button.LEFT in pressed:
            filt = (filt + (1 if Button.RIGHT in pressed else 2)) % 3
            laser2.cmd(0xE1, filt)      # Change the filter (takes effect at once)
            laser2.wait_done()
            hub.display.number(filt)
            print('filter', filt)
            while hub.buttons.pressed():
                wait(10)
            lo, hi = 99999, -1
            t.reset()
        wait(20)
    print('filter', filt, '| left mm', lo, '~', hi, '| jumps', hi - lo, 'mm')
```

**3. Timing budget (E2)**

Put a box or whiteboard about 50 cm ahead and keep it still. Every second it prints how many times the value changed (≈ updates) and how many mm it jumps: 20 ms is fastest, 200 ms steadiest.

[📄 Download cmd03_budget.py](/examples/tof2/pybricks_cmd_en/cmd03_budget.py) + [library](/examples/tof2/pybricks_cmd_en/MBC_laser2_obj_Lib.py)

```python
# Ranger 2-Way command example 03 (Pybricks, SPIKE Prime / Robot Inventor hub): timing budget: faster or more accurate
# Switch the timing budget (command E2) with the left / right buttons: 20, 33 (factory), 100, 200 ms
# Setup: a box or whiteboard about 50 cm in front of both lasers, kept still. Every second it prints "how many times the value changed" (≈ updates) and "how many mm it jumps"
# Wiring: Ranger 2-Way → port C; upload it together with MBC_laser2_obj_Lib.py (keep both files in Pybricks Code)
# Only the SPIKE edition takes commands (sensor firmware 2026-11 or later); settings stay in the sensor memory and go back to factory when unplugged
#   cmd(code, parameter): send a command; wait_done(): wait until it is done, True = done, False = failed (parameter out of range) or no reply
#   Commands: E0 ranging mode, E1 filter, E2 timing budget, E3 factory reset (parameter 0xA5), E4 max distance
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = port C (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)

CHOICES = [20, 33, 100, 200]
k = 1
laser2.cmd(0xE3, 0xA5)              # Factory reset first (33 ms)
laser2.wait_done()
laser2.cmd(0xE1, 0)                 # Filter off, so the difference is easier to see
laser2.wait_done()
wait(500)                        # Wait for fresh readings before counting
hub.display.number(CHOICES[k])

while True:
    last, changes, lo, hi = -1, 0, 99999, -1
    t = StopWatch()
    while t.time() < 1000:
        mm = laser2.left_mm()
        if mm != last:
            changes += 1
            last = mm
        lo = min(lo, mm)
        hi = max(hi, mm)
        pressed = hub.buttons.pressed()
        if Button.RIGHT in pressed or Button.LEFT in pressed:
            k = (k + (1 if Button.RIGHT in pressed else 3)) % 4
            laser2.cmd(0xE2, CHOICES[k])    # Change the timing budget (re-configures the laser chips)
            ok = laser2.wait_done()
            hub.display.number(CHOICES[k])
            print('timing budget', CHOICES[k], 'ms', 'done' if ok else 'failed')
            while hub.buttons.pressed():
                wait(10)
            last, changes, lo, hi = -1, 0, 99999, -1
            t.reset()
        wait(5)
    print(CHOICES[k], 'ms | changed', changes, 'times in 1 s | jumps', hi - lo, 'mm')
```

**4. Factory reset (E3)**

Put a whiteboard about 60 cm ahead. The limit is set to 30 cm first, so the reading is "capped" at 30; press the center button to send E3, the limit goes back to 120 and the board at 60 cm shows up again. To stop the program: hold the center button.

[📄 Download cmd04_factory_reset.py](/examples/tof2/pybricks_cmd_en/cmd04_factory_reset.py) + [library](/examples/tof2/pybricks_cmd_en/MBC_laser2_obj_Lib.py)

```python
# Ranger 2-Way command example 04 (Pybricks, SPIKE Prime / Robot Inventor hub): factory reset
# Factory reset (command E3, the parameter must be 0xA5): first set the max distance to 30 cm and see the reading "capped" at 30;
# press the center button to send E3 → the limit is back to 120 cm and far objects show up again
# Setup: a whiteboard about 60 cm in front of both lasers (farther than 30, nearer than 120)
# Wiring: Ranger 2-Way → port C; upload it together with MBC_laser2_obj_Lib.py (keep both files in Pybricks Code)
# Only the SPIKE edition takes commands (sensor firmware 2026-11 or later); settings stay in the sensor memory and go back to factory when unplugged
#   cmd(code, parameter): send a command; wait_done(): wait until it is done, True = done, False = failed (parameter out of range) or no reply
#   Commands: E0 ranging mode, E1 filter, E2 timing budget, E3 factory reset (parameter 0xA5), E4 max distance
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = port C (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)

laser2.cmd(0xE4, 30)                # Set the max distance to 30 cm first (beyond 30 = nothing, reads 30)
laser2.wait_done()
hub.display.number(30)
print('Max distance 30 cm: press the center button to send E3 factory reset')
hub.system.set_stop_button(None)   # Use the center button in the program (to stop the program: hold the center button)

t = StopWatch()
while True:
    if Button.CENTER in hub.buttons.pressed():   # Check the buttons every 10 ms
        laser2.cmd(0xE3, 0xA5, again=True)   # Factory reset; again=True: run the same command again too
        ok = laser2.wait_done()
        hub.display.number(120)
        hub.speaker.beep()
        print('factory reset', 'done' if ok else 'failed', ': limit back to 120 cm')
        while hub.buttons.pressed():
            wait(10)
    if t.time() >= 300:                # Print the distances every 0.3 s
        t.reset()
        print('left', laser2.left(), 'right', laser2.right(), 'cm | left', laser2.left_mm(), 'mm')
    wait(10)
```

**5. Max distance (E4)**

The left / right buttons change the limit to 10, 20 ... 120 cm. Closer than the limit: red light + beep, otherwise green; people walking around far away do not trigger it.

[📄 Download cmd05_max_distance.py](/examples/tof2/pybricks_cmd_en/cmd05_max_distance.py) + [library](/examples/tof2/pybricks_cmd_en/MBC_laser2_obj_Lib.py)

```python
# Ranger 2-Way command example 05 (Pybricks, SPIKE Prime / Robot Inventor hub): max distance: only near things count
# Max distance (command E4, cm): anything beyond the limit counts as "nothing" and reads as the limit → it only reacts to near things
# Left / right buttons change the limit to 10, 20, 30 ... 120 cm (shown on the screen); if the nearer distance is closer than the limit: red light + beep, otherwise green
# Wiring: Ranger 2-Way → port C; upload it together with MBC_laser2_obj_Lib.py (keep both files in Pybricks Code)
# Only the SPIKE edition takes commands (sensor firmware 2026-11 or later); settings stay in the sensor memory and go back to factory when unplugged
#   cmd(code, parameter): send a command; wait_done(): wait until it is done, True = done, False = failed (parameter out of range) or no reply
#   Commands: E0 ranging mode, E1 filter, E2 timing budget, E3 factory reset (parameter 0xA5), E4 max distance
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from pybricks.tools import wait, StopWatch
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                   # 3 = port C (1=A, 2=B, 3=C, 4=D, 5=E, 6=F)

limit = 30
laser2.cmd(0xE3, 0xA5)              # Factory reset first
laser2.wait_done()
laser2.cmd(0xE4, limit)             # Max distance 30 cm
laser2.wait_done()
hub.display.number(limit)
t = StopWatch()

while True:
    pressed = hub.buttons.pressed()    # Check the buttons every 10 ms
    if Button.RIGHT in pressed or Button.LEFT in pressed:
        limit = limit + 10 if Button.RIGHT in pressed else limit - 10
        limit = max(10, min(120, limit))
        laser2.cmd(0xE4, limit)       # Change the limit (takes effect at once)
        laser2.wait_done()
        hub.display.number(limit)
        while hub.buttons.pressed():
            wait(10)
    if t.time() >= 200:                # Check the distance every 0.2 s
        t.reset()
        cm = min(laser2.left(), laser2.right())
        if cm < limit:               # Closer than the limit = something is really there
            hub.light.on(Color.RED)
            hub.speaker.beep(1000, 50)
        else:
            hub.light.on(Color.GREEN)
        print('limit', limit, 'cm |', 'left', laser2.left(), 'right', laser2.right(), 'cm | left', laser2.left_mm(), 'mm')
    wait(10)
```
