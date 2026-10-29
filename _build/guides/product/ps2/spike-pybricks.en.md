### Connect the gamepad to the receiver

The PS2 2.4G gamepad and its receiver are **paired at the factory**; no pairing button needed.

1. Connect the receiver and turn the controller on.
2. **Switch the gamepad on**; it connects by itself.
3. Watch the **green LED** on the receiver: **on = no gamepad yet**; **off = gamepad connected**.
4. The **ANALOG (or MODE) light** on the gamepad must be on for the sticks to give values. The receiver switches to this mode on connect; if the light is off, press the ANALOG button in the middle of the gamepad.

> [!CAUTION]
> 2.4 GHz is a shared band (Wi-Fi, microwave ovens). With **12 or more sets** in use at one venue they may interfere; in that case use the Bluetooth version (PS4 / PS5).

### Library and example downloads (Pybricks 4.1)

- [📥 Download all (.zip)](/examples/ps2/ps2_pybricks_spike_v4.x.zip): `MBC_sensors.py`, `MBC_gamepad_Lib.py` and the 6 beginner examples
- Library only: [MBC_sensors.py](/examples/ps2/pybricks/MBC_sensors.py) (use this for new projects: one file for every MBC sensor, `from MBC_sensors import MBC_GAMEPAD`) · [MBC_gamepad_Lib.py](/examples/ps2/pybricks/MBC_gamepad_Lib.py) (for the examples on this page and older programs)

> [!NOTE]
> The Pybricks 3.6.1 examples are no longer updated; the old files are under "Legacy (no longer updated)" on the [Downloads page](/en/downloads.md).

### All buttons as one number: buttons() and buttons_psx()

The library has two ways to pack the 16 buttons into one number. The buttons are the same; **each button just has a different value**:

| Button | `pad.buttons()` (MBC order) | `pad.buttons_psx()` (PSX order) |
| :--- | :--- | :--- |
| Up / Right / Down / Left | 1 / 2 / 4 / 8 | 16 / 32 / 64 / 128 |
| ▲ / ● / ✕ / ■ | 16 / 32 / 64 / 128 | 4096 / 8192 / 16384 / 32768 |
| L2 / R2 / L1 / R1 | 256 / 512 / 1024 / 2048 | 256 / 512 / 1024 / 2048 |
| SELECT / L3 / R3 / START | 4096 / 8192 / 16384 / 32768 | 1 / 2 / 4 / 8 |
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

> [!TIP]
> How to use the Controller PS2 with EV3, the SPIKE App and Pybricks blocks is also summarised in the [command guide (PDF)](/examples/ps2/PS2_Controller_Command_Guide_EN_Taiwan_only.pdf).
