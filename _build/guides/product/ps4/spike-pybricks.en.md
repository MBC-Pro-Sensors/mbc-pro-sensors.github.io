### Pair the gamepad (first use or new gamepad)

1. Connect the receiver and turn the controller on. **Do not hold the receiver's button while plugging it in** (that enters flashing mode and the receiver does nothing; release the button, unplug and replug).
2. **First use**: a receiver that remembers no gamepad opens a **30-second** pairing window at power-on (green LED blinking fast). **To change gamepads, or if the 30 seconds are over**: press the receiver's button (Reset) once; it forgets the old gamepad and opens 30 seconds again.
3. Within those 30 seconds put the gamepad in pairing mode until its light bar flashes fast: **PS4** hold **SHARE + PS**; **PS5** hold **Create + PS**.
4. Once connected, the receiver **remembers this gamepad** and will not connect to anyone else's. From then on, just press the gamepad's **PS button** after power-on.

| Receiver green LED | Meaning |
| :--- | :--- |
| Blinking fast | Waiting for a gamepad to pair |
| Steady on | No gamepad connected (press the PS button to reconnect) |
| Off | Gamepad connected |

> [!TIP]
> At a competition, pair at your own table so no other team's gamepad is in pairing mode during the 30 seconds. Paired the wrong one? Press Reset and pair again.

### Library and example downloads (Pybricks 4.1)

- [📥 Download all (.zip)](/examples/ps4/ps4_pybricks_spike_v4.x.zip): `MBC_sensors.py`, `MBC_gamepad_Lib.py` and the 6 beginner examples
- Library only: [MBC_sensors.py](/examples/ps4/pybricks/MBC_sensors.py) (use this for new projects: one file for every MBC sensor, `from MBC_sensors import MBC_GAMEPAD`) · [MBC_gamepad_Lib.py](/examples/ps4/pybricks/MBC_gamepad_Lib.py) (for the examples on this page and older programs)

> [!NOTE]
> The Pybricks 3.6.1 examples are no longer updated; the old files are under "Legacy (no longer updated)" on the [Downloads page](/en/downloads.md).

### All buttons as one number: buttons() and buttons_psx()

The library has two ways to pack the 16 buttons into one number. The buttons are the same; **each button just has a different value**:

| Button | `pad.buttons()` (MBC order) | `pad.buttons_psx()` (PSX order) |
| :--- | :--- | :--- |
| Up / Right / Down / Left | 1 / 2 / 4 / 8 | 16 / 32 / 64 / 128 |
| ▲ / ● / ✕ / ■ | 16 / 32 / 64 / 128 | 4096 / 8192 / 16384 / 32768 |
| L2 / R2 / L1 / R1 | 256 / 512 / 1024 / 2048 | 256 / 512 / 1024 / 2048 |
| SHARE (PS5: Create) / L3 / R3 / OPTIONS | 4096 / 8192 / 16384 / 32768 | 1 / 2 / 4 / 8 |
| Check one button | `pad.pressed('CROSS')` | `pad.pressed_psx('CROSS')` or `pad.buttons_psx() & PSX_CROSS` |

- **Beginners: use `buttons()` / `pressed()`.** The D-pad and shape-button values match the official App's "reflected light"; every example on this page uses them.
- **`buttons_psx()` uses the PS2 gamepad's native button order.** PS2 and PS4 receivers send the same number, and Arduino (I2C) reads the 16 buttons in this order too, so use it when your program shares button values with Arduino code. SPIKE edition only (the 7th value of mode 9).
- Both are 0 while no gamepad is connected.

```python
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD, PSX_CROSS

pad = MBC_GAMEPAD(Port.C)
while True:
    pad.update()                 # update once per loop
    n = pad.buttons_psx()        # 16 buttons, PSX order
    if n & PSX_CROSS:            # ✕ pressed
        print('CROSS')
    if pad.pressed_psx('L1'):    # or check by name
        print('L1')
    wait(50)
```
