@@@ cal
### Calibrate first (only once)

The Pathfinder has to "learn" the black and white of your mat before its numbers are right. Calibration is stored in the sensor and survives power-off (until you change mats).

1. Plug in the sensor and turn the hub on (the sensor lights come on).
2. Hold the **calibration button for about 0.5 s**, then release it (do not keep holding it; holding it keeps restarting calibration).
3. When the **green calibration LED** (next to the button) lights up, sweep the sensor back and forth between the black line and the white floor **within 5 seconds**. All %%N%% sensing points must pass over **the darkest black line** and **the brightest white area**.
4. Keep the sensor **0.5~1.2 cm** above the surface while sweeping; higher reads poorly.
5. The green LED goes off = calibration done, saved in the sensor automatically.

**How do I know it worked?** On the white floor the "line width" is 0; on a normal black line it is %%CHK%%.

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="Pathfinder calibration" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

@@@ cal-arduino
> [!TIP]
> You can also calibrate from code: send command `E7` (5-second sweep calibration; F3 bit2 = 1 meanwhile), or `E2` / `E3` to store the current reading as black / white (about 50 ms, saved to EEPROM). Example 4 on this page is the calibration example.

@@@ cal-mblock
> [!TIP]
> You can also calibrate with a block: "Pathfinder port [port] 5-second sweep calibration" does the same as the button.

@@@ cal-microbit
> [!TIP]
> You can also calibrate with a block: "line follower calibrate …" with "as black" (E2), "as white" (E3) or "5-second sweep" (E7); the result is saved in the sensor.

@@@ cal-matrixbig
> [!TIP]
> With the big-channel library you can also calibrate from blocks: **Cyan = black calibration**, **Magenta = white calibration** (on the Pathfinder 8-Way, bytes 25 / 26 count completed actions).

@@@ mblock-legacy
### Old firmware and old blocks

The legacy extension v1.0.0 (15 blocks) goes with firmware shipped before 2026-10. To use the old blocks with new firmware, the Pathfinder 8 / 16 can be flashed with the `MBC_MBLOCK_LEGACY=1` build (no legacy build exists for the other products).

@@@ line8-spike-official
### Data channel mapping

In the official App the Pathfinder 8-Way appears as a **SPIKE Color Sensor**, with the 8-channel data packed into the colour sensor channels. **The Education and home editions map differently**; use the table for yours.

**Education (LEGO Education SPIKE App, SPIKE 3)**

| Official block | Data | Range and decoding |
| :--- | :--- | :--- |
| Color | Line width | `0 ~ 8`: number of sensors on the line. A normal black line is usually `2 ~ 3`; a crossing or a large colour patch makes it much larger, useful for detecting junctions |
| Reflected Light | Standard position | `0 ~ 16`: `8` centre, `0` far left, `16` far right. Subtract **8** for the line position (negative = line on the left, positive = right); good for PID |
| Red | High-resolution smooth position | `0 ~ 200`: `100` centre. Subtract **100** for the high-precision position (negative = left); needs a proper calibration |
| Green | First 4 sensor values | `0 ~ 65535`: 4 bits per sensor, each `0 ~ 15` |
| Blue | Last 4 sensor values | `0 ~ 65535`: 4 bits per sensor, each `0 ~ 15` |

**Home edition (LEGO MINDSTORMS Robot Inventor App)**

In the home-edition App some single-channel blocks are limited to `0 ~ 126`, so the position and reflected-light data are moved to the RGB and reflected-light channels to get around that limit:

| Official block | Data | Range and decoding |
| :--- | :--- | :--- |
| Color | Line width | `0 ~ 8` (the home edition caps colour at 10); often used to detect crossings |
| Reflected Light | Standard position | `0 ~ 16`: subtract **8** for the line position, not affected by the 126 limit |
| Red | High-resolution smooth position | `0 ~ 200`: subtract **100** for the high-precision position; nothing lost on the home edition |
| Green | First 4 sensor values | `0 ~ 255`: 2 bits per sensor, each `0 ~ 3` |
| Blue | Last 4 sensor values | `0 ~ 255`: 2 bits per sensor, each `0 ~ 3` |

### Example programs

![Pathfinder 8-Way PID line following in the SPIKE App](/images/sensors/line8/spike-education-example.webp)

- [📥 Education example (.llsp3)](/examples/line8/spike/line8-demo.llsp3): open with the LEGO Education SPIKE App.
- [📥 Home-edition example (.lms)](/examples/line8/spike/line8-ri-demo.lms): open with the LEGO MINDSTORMS Robot Inventor App.

![Pathfinder 8-Way PID line following in the Robot Inventor App](/images/sensors/line8/spike-ri-example.webp)

@@@ line16-spike-official
### Data channel mapping

In the official App the Pathfinder 16-Way appears as a **SPIKE Color Sensor**, with the 16-channel data packed into the colour sensor channels. **The Education and home editions map differently**; use the table for yours.

**Education (LEGO Education SPIKE App, SPIKE 3)**

| Official block | Data | Range and decoding |
| :--- | :--- | :--- |
| Color | Line width | `0 ~ 16`: number of sensors on the line. A normal black line is usually `3 ~ 5`; a crossing or a large colour patch makes it much larger, useful for detecting junctions |
| Reflected Light | Standard position | `0 ~ 32`: `16` centre, `0` far left, `32` far right. Subtract **16** for the line position (negative = line on the left, positive = right); good for PID |
| Red | High-resolution smooth position | `0 ~ 200`: `100` centre. Subtract **100** for the high-precision position (negative = left); needs a proper calibration |
| Green | First 8 sensor values | `0 ~ 65535`: 2 bits per sensor, each `0 ~ 3` |
| Blue | Last 8 sensor values | `0 ~ 65535`: 2 bits per sensor, each `0 ~ 3` |

**Home edition (LEGO MINDSTORMS Robot Inventor App)**

In the home-edition App some single-channel blocks are limited to `0 ~ 126`, so the position and reflected-light data are moved to the RGB and reflected-light channels to get around that limit:

| Official block | Data | Range and decoding |
| :--- | :--- | :--- |
| Color | Line width | `0 ~ 10`: the home edition caps colour at 10, so the width is limited to `0 ~ 10`; often used to detect crossings |
| Reflected Light | Standard position | `0 ~ 32`: subtract **16** for the line position, not affected by the 126 limit |
| Red | High-resolution smooth position | `0 ~ 200`: subtract **100** for the high-precision position; nothing lost on the home edition |
| Green | First 8 sensor values | `0 ~ 255`: 1 bit per sensor, each `0 ~ 1` |
| Blue | Last 8 sensor values | `0 ~ 255`: 1 bit per sensor, each `0 ~ 1` |

### Example programs

![Pathfinder 16-Way PID line following in the SPIKE App](/images/sensors/line16/spike-education-example.webp)

- [📥 Education example (.llsp3)](/examples/line16/spike/line16-demo.llsp3): open with the LEGO Education SPIKE App.
- [📥 Home-edition example (.lms)](/examples/line16/spike/line16-demo.lms): open with the LEGO MINDSTORMS Robot Inventor App.

![Pathfinder 16-Way PID line following in the Robot Inventor App](/images/sensors/line16/spike-ri-example.webp)

@@@ coach
### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).

@@@ line-spike-pybricks
### Library and example downloads

Pick the pack that matches the hub's Pybricks firmware (each zip has the library + 4 examples):

- [📥 Download pack (firmware 4.0.0)](/examples/%%P%%/%%P%%_pybricks_spike_v4.0.0.zip)
- [📥 Download pack (firmware 3.6.1)](/examples/%%P%%/%%P%%_pybricks_spike_v3.6.1.zip)

Library only: [4.0.0 MBC_%%P%%_obj_Lib_v400.py](/examples/%%P%%/pybricks/For%20firmware%204.0.0/MBC_%%P%%_obj_Lib_v400.py) · [3.6.1 MBC_%%P%%_Lib_v361.py](/examples/%%P%%/pybricks/For%20firmware%203.6.1/MBC_%%P%%_Lib_v361.py)

> [!IMPORTANT]
> **Rename the file**: the examples say `import MBC_%%P%%_obj_Lib` (3.6.1: `MBC_%%P%%_Lib`), so rename the download to `MBC_%%P%%_obj_Lib.py` (drop `_v400`) or `MBC_%%P%%_Lib.py` (drop `_v361`), or you will get an `ImportError`. The library must be in the **same project list** as the main program.

![Importing the library into a Pybricks project](/images/sensors/%%P%%/pybricks-upload-lib.webp)

### Method 1: built-in sensor commands (no library)

Read with Pybricks' built-in `ColorSensor` commands, no extra file. These values are computed from the built-in commands:

| Set | Function | Range | Meaning |
| :--- | :--- | :--- | :--- |
| Basic (less sensitive to calibration, but calibrate once) | `getLinePos%%N%%()` | `-%%N%% ~ +%%N%%` | Line position (integer), 0 centre, negative left, positive right |
| | `getLineWidth%%N%%()` | `0 ~ %%N%%` | Line width (sensors triggered), for crossings and T-junctions |
| High precision (calibrate properly first) | `getLinePos100()` | `-100 ~ +100` | High-resolution interpolated position for advanced PID |
| | `getLineWidth100()` | `0 ~ %%N%%` | Line width in high-precision mode, in sync with `getLinePos100()` |

> [!TIP]
> Use position and width from **the same set**: `getLinePos%%N%%()` with `getLineWidth%%N%%()`, `getLinePos100()` with `getLineWidth100()`, so both come from the same moment.

Examples: blocks [4.0.0](/examples/%%P%%/pybricks/For%20firmware%204.0.0/%%P%%_block_native_v400.py) · [3.6.1](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_block_native_v361.py); Python [4.0.0](/examples/%%P%%/pybricks/For%20firmware%204.0.0/%%P%%_python_native_v400.py) · [3.6.1](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_python_native_v361.py)

### Method 2: import the library

| Function | Range | Meaning |
| :--- | :--- | :--- |
| `line_init_port_multitask(port, multitask)` | — | Initialise. `port` = 1~6 for A~F; `multitask=True` enables async mode (use with `await`) |
| `line_pos%%N%%()` | `-%%N%% ~ +%%N%%` | Integer line position, less sensitive to calibration (without calibration every channel reads as black) |
| `line_pos100()` | `-100 ~ +100` | High-resolution interpolated position, best for PID; calibrate properly |
| `line_width()` | `0 ~ %%N%%` | Line width, works with both position functions |
| `line_ir_calibration()` | `list[%%N%%]` | %%N%% calibrated values (each `0 ~ %%IRMAX%%`). Seen from behind the robot: index 0 = far left (ch1) |
| `line_ir_ch(ch)` | `0 ~ %%IRMAX%%` | One calibrated channel, `ch` = 1~%%N%%; fast to call repeatedly (shared cache) |
| `line_junction()` | `0, 1, 2+` | Number of line groups: 0 none, 1 single line, 2+ junction |
| `line_junction_name()` | `str` | `"none"`, `"line"`, `"junction"` |
| `line_bin_raw()` | `0 ~ %%BINMAX%%` | %%N%%-bit trigger mask, bit0 = ch1, 1 = line seen |
| `line_on_sensors()` | `list[int]` | Channels seeing the line, e.g. `%%EXON%%` |
| `line_read_all()` | `dict` | Everything at once: `ir_calibration`, `line_pos%%N%%`, `line_pos100`, `line_width`, `bin_raw`, `on_sensors`, `junction`, `junction_name` |

Examples: blocks [4.0.0](/examples/%%P%%/pybricks/For%20firmware%204.0.0/%%P%%_block_with_lib_v400.py) · [3.6.1](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_block_with_lib_v361.py); Python [4.0.0](/examples/%%P%%/pybricks/For%20firmware%204.0.0/%%P%%_python_with_lib_v400.py) · [3.6.1](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_python_with_lib_v361.py)

@@@ ev3-prox
### Proximity mode: line position

| Value | Range | Meaning |
| :--- | :--- | :--- |
| Line position | `-%%N%% ~ +%%N%%` | 0 = black line in the centre, negative left, positive right; ±%%N%% = line at the very edge |

@@@ ev3-beacon-line8
### Beacon mode: 8 reflected-light values

Read the "value list" (8 bytes). Each value is the calibrated reflection: 0 = darkest (black line), 100 = brightest (white). Channel 1 is on the far left (standing behind the robot, facing forward).

| Byte | Content | Range | EV3-G / Classroom | clev3r |
| :---: | :--- | :--- | :--- | :--- |
| 0 | Channel 1 | 0 ~ 100 | beacon channel 1 "heading" | `raw[0]` |
| 1 | Channel 2 | 0 ~ 100 | beacon channel 1 "proximity" | `raw[1]` |
| 2 | Channel 3 | 0 ~ 100 | beacon channel 2 "heading" | `raw[2]` |
| 3 | Channel 4 | 0 ~ 100 | beacon channel 2 "proximity" | `raw[3]` |
| 4 | Channel 5 | 0 ~ 100 | beacon channel 3 "heading" | `raw[4]` |
| 5 | Channel 6 | 0 ~ 100 | beacon channel 3 "proximity" | `raw[5]` |
| 6 | Channel 7 | 0 ~ 100 | beacon channel 4 "heading" | `raw[6]` |
| 7 | Channel 8 | 0 ~ 100 | beacon channel 4 "proximity" | `raw[7]` |

@@@ ev3-beacon-line16
### Beacon mode: 16 reflected-light values

Read the "value list" (8 bytes). Each channel is quantised to 1~9 (1 = darkest, 9 = brightest) and two channels are combined into one two-digit number (11~99), getting around the EV3 limit of 100.

| Byte | Content | Range | Decoding |
| :---: | :--- | :--- | :--- |
| 0 | Channels 0+1 | 11 ~ 99 | tens = channel 0, units = channel 1 |
| 1 | Channels 2+3 | 11 ~ 99 | tens = channel 2, units = channel 3 |
| 2 | Channels 4+5 | 11 ~ 99 | tens = channel 4, units = channel 5 |
| 3 | Channels 6+7 | 11 ~ 99 | tens = channel 6, units = channel 7 |
| 4 | Channels 8+9 | 11 ~ 99 | tens = channel 8, units = channel 9 |
| 5 | Channels 10+11 | 11 ~ 99 | tens = channel 10, units = channel 11 |
| 6 | Channels 12+13 | 11 ~ 99 | tens = channel 12, units = channel 13 |
| 7 | Channels 14+15 | 11 ~ 99 | tens = channel 14, units = channel 15 |

Example: a byte reads `83`; the tens digit 8 is the even channel (e.g. channel 0), the units digit 3 is the odd channel (e.g. channel 1).

@@@ ev3-remote
### Remote mode: position + width + edges (recommended for competition)

Read the "value list" (4 bytes):

| Byte | Content | Range | Meaning |
| :---: | :--- | :--- | :--- |
| 0 | High-resolution position | `-100 ~ +100` | 0 centre, negative left, positive right; the best PID error input |
| 1 | Line width | `0 ~ %%N%%` | Sensors triggered; detects crossings and T-junctions |
| 2 | Left edge light | `0 ~ 100` | Reflection of channel 1 (far left), 0 pure black, 100 pure white |
| 3 | Right edge light | `0 ~ 100` | Reflection of channel %%N%% (far right), 0 pure black, 100 pure white |

> [!TIP]
> When bytes 2 and 3 are both below 20, the front of the robot is fully on the cross line of a junction; a sudden change on one side means a fork is coming. Useful to avoid losing the line.

@@@ ev3-myblocks-ev3g
### MBC My Blocks

The example project contains three My Blocks; no extension pack to install. Each one decodes the data in the background and stores it in its own variables. **Put them inside a Loop** to always have the latest data:

1. `LinePos%%N%%_dataUpdate`: basic line following; updates `0_LinePos%%N%%` (`-%%N%% ~ +%%N%%`, left negative, right positive). Good for beginners.
2. `LinePos100_dataUpdate`: high precision (needs a proper calibration); updates `0_LinePos100` (`-100 ~ +100`), `0_LineWidth%%N%%` (`0 ~ %%N%%`) and the far-left / far-right edge light values.
3. `LineCh%%N%%_dataUpdate`: per-channel decoding; give a channel number `1 ~ %%N%%`, get that channel's quantised light `1 ~ 9` (1 darkest, 9 brightest).

@@@ ev3-myblocks-classroom
### MBC My Blocks

The example project contains three My Blocks; no extension pack to install. Each one decodes the data in the background and stores it in its own variables. **Put them inside a "forever" loop** to always have the latest data:

1. `LinePos%%N%%_dataUpdate`: basic line following; updates `0_LinePos%%N%%` (`-%%N%% ~ +%%N%%`, left negative, right positive). Good for beginners.
2. `LinePos100_dataUpdate`: high precision (needs a proper calibration); updates `0_LinePos100` (`-100 ~ +100`), `0_LineWidth%%N%%` (`0 ~ %%N%%`) and the far-left / far-right edge light values.
3. `LineCh%%N%%_dataUpdate`: full array; updates `s1` to `s%%N%%`, %%N%% variables matching each sensor from left to right.

@@@ ev3-myblocks-clev3r
### MBC subroutines

The example file contains three subroutines; no extension pack to install. **Put them inside an endless loop** to always have the latest data:

1. `LinePos%%N%%_dataUpdate`: give the `port`, returns `pos%%N%%` (`-%%N%% ~ +%%N%%`, left negative, right positive).
2. `LinePos100_dataUpdate`: give the `port`, returns `pos100` (`-100 ~ +100`, needs a proper calibration), `width` (`0 ~ %%N%%`), and `ch1` / `ch%%N%%` (far-left / far-right light `0 ~ 100`).
3. `LineCh%%N%%_dataUpdate`: give the `port`, returns the array `light%%N%%`; `light%%N%%[0]` ~ `light%%N%%[%%NM1%%]` are each sensor's quantised light `1 ~ 9` (1 darkest, 9 brightest).

@@@ ev3-ex-ev3g
### Example program

- [📥 EV3-G example (.ev3)](/examples/%%P%%/ev3/%%P%%_ev3_official.ev3): open with the official EV3 software.

![%%NAME%% PID line following in EV3-G](/images/sensors/%%P%%/ev3-official-example.webp)

@@@ ev3-ex-classroom
### Example program

- [📥 Classroom example (.lmsp)](/examples/%%P%%/ev3/%%P%%_ev3_classroom.lmsp): open with EV3 Classroom.

![%%NAME%% PID line following in EV3 Classroom](/images/sensors/%%P%%/ev3-classroom-example.webp)

@@@ ev3-ex-clev3r
### Example program

- [📥 clev3r example (.bp)](/examples/%%P%%/ev3/%%P%%_ev3_clev3r.bp): open with clev3r.

![%%NAME%% PID line following in clev3r](/images/sensors/%%P%%/ev3-clev3r-example.webp)

@@@ line-ev3-pybricks
### Library and example downloads

- [📥 Download pack (examples + library)](/examples/%%P%%/%%P%%_pybricks_ev3.zip)
- Library: [MBC_%%P%%_EV3_Lib.py](/examples/%%P%%/ev3/MBC_%%P%%_EV3_Lib.py). Import it straight into your Pybricks project; **the file name must match exactly** for the `import` to load it.

![Importing the library into an EV3 Pybricks project](/images/sensors/%%P%%/ev3-pybricks-upload-lib.webp)

### Method 1: native commands

Use Pybricks' low-level `device.mode(mode)` to get the data array directly:

| Command | Returns | Content |
| :--- | :--- | :--- |
| `device.mode(0)` | Tuple (1 byte) | Beginner mode: position `-%%N%% ~ %%N%%`, 0 = centre |
| `device.mode(1)` | Tuple (8 bytes) | Overview / debug mode: %%MODE1%% |
| `device.mode(2)` | Tuple (4 bytes) | Competition mode: high-precision position `-100 ~ 100`, width `0 ~ %%N%%`, left edge light, right edge light |

> [!NOTE]
> Pybricks has not yet updated the matching graphical and low-level commands, so native examples will follow later; for now Method 2 is recommended.

### Method 2: import the library

Initialise with `line_init_port_multitask(port, multitask)`: `port` is an EV3 port object (for example `hub.port.S1`). **Not multitask**: `multitask = False`, and choose `call` in front of each call; **multitask**: `True`, and choose `await`.

![Non-multitask setup](/images/sensors/%%P%%/ev3-pybricks-non-multitask.webp)

![Multitask setup (await)](/images/sensors/%%P%%/ev3-pybricks-multitask.webp)

| Mode | Function | Range | Meaning |
| :--- | :--- | :--- | :--- |
| Mode 0 (basic) | `line_pos%%N%%()` | `-%%N%% ~ +%%N%%` | Integer line position, 0 centre |
| Mode 1 (decode) | `line_ir_ch(ch)` | `1 ~ 9` | One channel's quantised light, `ch` = 1~%%N%%, 1 darkest, 9 brightest |
| Mode 2 (competition) | `line_pos100()` | `-100 ~ +100` | High-resolution interpolated position for PID / PD |
| Mode 2 (competition) | `line_width()` | `0 ~ %%N%%` | Line width, for junctions and wide lines |
| Mode 2 (competition) | `line_edge_l()` | `0 ~ 100` | Left edge light (0 black, 100 white), to avoid losing the line |
| Mode 2 (competition) | `line_edge_r()` | `0 ~ 100` | Right edge light (0 black, 100 white), to avoid losing the line |

> [!IMPORTANT]
> Mixing functions from different modes costs a mode-switch delay. For normal use and competition, **stick to the mode 2 functions**; use mode 1 on its own only when you need every raw sensor value for your own algorithm.

Examples: [blocks (.py)](/examples/%%P%%/ev3/%%P%%_ev3_block_with_lib.py) · [Python (.py)](/examples/%%P%%/ev3/%%P%%_ev3_python_with_lib.py)

@@@ pair-ps4
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

@@@ pair-ps2
### Connect the gamepad to the receiver

The PS2 2.4G gamepad and its receiver are **paired at the factory**; no pairing button needed.

1. Connect the receiver and turn the controller on.
2. **Switch the gamepad on**; it connects by itself.
3. Watch the **green LED** on the receiver: **on = no gamepad yet**; **off = gamepad connected**.
4. The **ANALOG (or MODE) light** on the gamepad must be on for the sticks to give values. The receiver switches to this mode on connect; if the light is off, press the ANALOG button in the middle of the gamepad.

> [!CAUTION]
> 2.4 GHz is a shared band (Wi-Fi, microwave ovens). With **12 or more sets** in use at one venue they may interfere; in that case use the Bluetooth version (PS4 / PS5).

@@@ ps4-spike-official
### Example programs

- [📥 Education example (.llsp3, SPIKE App 3.4.3)](/examples/ps4/spike/ps4_demo_spike_prime_v3.4.3.llsp3)
- [📥 Home-edition example (.lms, Robot Inventor App 10.5.0)](/examples/ps4/spike/ps4_demo_robot_inventor_v10.5.0.lms)

> [!NOTE]
> The beginner examples on this page use the Education edition (SPIKE 3). The home edition (Robot Inventor) reads the "Color" block differently, so the beginner examples do not apply; use the home-edition example above.

@@@ ps2-spike-official
> [!NOTE]
> The beginner examples on this page use the Education edition (SPIKE 3). The home edition (Robot Inventor) reads the "Color" block differently, so the beginner examples do not apply; see the [command guide (PDF)](/examples/ps2/PS2_Controller_Command_Guide_EN_Taiwan_only.pdf).

@@@ pad-spike-pybricks
### Example downloads (Pybricks 3.6.1)

- [📥 Download all (.zip)](/examples/%%P%%/%%P%%_pybricks_spike_v3.6.1.zip)
- [Full-button version (mode 9)](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_python_mode9_v361.py) · [blocks](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_block_native_v361.py) · [Python](/examples/%%P%%/pybricks/For%20firmware%203.6.1/%%P%%_python_native_v361.py)

@@@ ps2-pdf
> [!TIP]
> How to use the Controller PS2 with EV3, the SPIKE App and Pybricks blocks is also summarised in the [command guide (PDF)](/examples/ps2/PS2_Controller_Command_Guide_EN_Taiwan_only.pdf).

@@@ ps4-matrixbig
> [!NOTE]
> The PS4 receiver is an ESP32 and needs ≥ 2 ms to answer as an I2C slave; the firmware preloads the latest packet, and the 5 ms wait in `begin()` of big-channel library v2 is there for it. **Bench test pending.**

@@@ power5v-microbit
> [!IMPORTANT]
> **The %%NAME%% must use external 5 V power** (%%WHY%%); it cannot run from the micro:bit 3V pin. Share GND between the external supply and the micro:bit.

@@@ tof8-warm
### Wait 2~4 seconds after power-on

The Ranger 8-Way needs no calibration; plug it in and go. **After power-on, wait 2~4 seconds** for the 8 lasers to get ready; meanwhile everything reads 120 (cm). Start your program with a short wait before reading distances.

| Channel | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Facing | left | left-front 60° | left-front 30° | front | front | right-front 30° | right-front 60° | right |

The table assumes the sensor faces forward with the components up. Put your hand on the robot's left: example 1 should show a **negative** direction. If it shows a positive one, the sensor is mounted the other way, and "turn left" / "turn right" in the examples must be swapped too.

@@@ tof8-spike-official
> [!WARNING]
> The official App readings are worked out from the sensor firmware and **have not yet been checked one by one on a real hub**. The home edition (Robot Inventor) uses a different format, so the official App examples on this page do not apply to it. If official-App example 1 always shows -7 or odd numbers wherever your hand is, switch to the MBC Blocks website or Pybricks and let us know.

@@@ imu-ev3
> [!NOTE]
> On the EV3 the IMU shows up as a "**Gyro Sensor**" (not an infrared sensor); read it with the original gyro sensor blocks.

@@@ imu-ev3-pybricks
> [!NOTE]
> The EV3 treats the IMU as an original gyro sensor; the examples read it with `GyroSensor`.

@@@ cam-setup
### Set the colour on the sensor first

All Sharpshooter settings are made on the **colour screen and buttons on the back of the sensor**; no computer needed:

1. The sensor only starts when plugged into a hub that is **already on** (the screen first shows "LEGO Sync..."); once the camera image appears it is ready.
2. **Short-press the side button** to open the settings menu; left / right to choose, OK to enter. In the menu, hold the side button for 1 s to clear that colour set.
3. Colour settings and camera parameters (brightness, contrast, saturation, exposure, white balance, flip, mirror) are saved automatically and used at the next power-on.

@@@ cam-setup-i2c
### Set the colour on the sensor first

All Sharpshooter settings are made on the **colour screen and buttons on the back of the sensor**, no computer needed: **short-press the side button** to open the settings menu, left / right to choose, OK to enter. Colour settings and camera parameters are saved automatically and used at the next power-on.

@@@ cam-spike-official
> [!NOTE]
> - Official Education App blocks: "Color" = hue, "Reflected light" = motion value, "Raw red / green / blue" = blob X / Y / area.
> - In the home edition (Robot Inventor) "Color" only goes **0~10** (a tenth of the Education edition); change the numbers in example 2 yourself.
> - The numbers on this page are for **current shipping firmware** (hue 0~100). To check: after power-on look at the `X` number at the top of the sensor screen. If it **only goes up to about 98**, it is the new firmware; if it goes above 100 (up to 159), it is old firmware; contact us for an update first.

### WRO edition (HUE12)

The official LEGO App **has no blocks for HUE12** (HUE12 is not a LEGO sensor), so the official-App examples are **Python programs for the Education SPIKE 3 App**. For blocks, use the MBC Blocks website.

@@@ cam-spike-pyb
### WRO edition (HUE12)

The hub needs Pybricks firmware **v4.0.0b5 or later**. The MBC Blocks website's "SPIKE Hub → Install/Update Pybricks firmware" flashes v4.1.0, which works.%%EXTRA%%

@@@ cam-ev3-two
### Two EV3 firmwares

| Firmware | Shows up on the EV3 as | Software that works |
| :--- | :--- | :--- |
| School teacher creative-contest edition (colour pick + blob tracking + motion) | Infrared sensor | EV3-G, EV3 Classroom, Pybricks, clev3r |
| WRO 2026 edition (HUE12, 12 colour spots) | Third-party sensor (type 95) | EV3-G (MBC My Blocks), clev3r, Pybricks; **EV3 Classroom cannot read it** |

@@@ cam-ev3g-wro
### WRO edition (HUE12): import the MBC My Block first

In EV3-G, download the MBC My Block [MBC_Hue12Sensor.ev3b](/examples/esp32cam/ev3_beginner_wro/MBC_Hue12Sensor.ev3b) and import it once with "**Tools → Block Import**" (once per computer); only then do the WRO examples open.

@@@ cam-classroom-wro
> [!WARNING]
> **EV3 Classroom cannot read the WRO edition (HUE12)**: on the EV3 it is a third-party sensor (type 95). Use EV3-G (MBC My Blocks), clev3r or Pybricks instead.

@@@ cam-mvision-wro
### WRO edition (HUE12)

The WRO edition's mVision data **keeps the original HUE12 packing** (12 colour-spot codes + vividness); decode it as in the `HUE12_AS_mVisionSensor_v1.0.8.mbr4` example. The HUE12 examples used by WRO 2026 teams are not affected. The school edition's data 0~9 use the same ranges as the EV3 / SPIKE editions, so program logic carries over directly.

@@@ cam-microbit
> [!NOTE]
> The Sharpshooter's I2C uses **SDA = IO3, SCL = IO1** (fixed 2026-10-21); once I2C is connected the USB serial monitor is gone. Hardware confirmed, **bench test pending**.

@@@ cam-arduino
> [!NOTE]
> The Sharpshooter's I2C (address `0x4C`) uses **SDA = IO3, SCL = IO1**; once I2C is connected the USB serial monitor is gone. Hardware confirmed, **bench test pending**. The Sharpshooter is itself an ESP32 and cannot clock-stretch, so always wait 2 ms before reading (the example helper already has `delay(2)`).

@@@ cam-mblock
> [!NOTE]
> The Sharpshooter MakeBlock edition is new in October 2026; before ordering, [ask us on LINE](https://line.me/R/ti/p/@692vcvuk) to confirm the shipping version. On the mBot it uses the IO1 / IO3 serial pins, **bench test in progress**.

@@@ imu-mblock
> [!NOTE]
> The IMU MakeBlock edition is new in October 2026; before ordering, [ask us on LINE](https://line.me/R/ti/p/@692vcvuk) to confirm the shipping version.

@@@ exp6-power
### Wiring and power

1. **Host cable**: plug the expander's **Host cable** into any SPIKE hub port (A~F).
2. **Separate power**: the expander must have its own power supply (battery box or adapter) for the motors; motor current is not drawn from the hub, and the hub signal lines go through an isolator.
3. **Devices**: plug LEGO sensors (colour, ultrasonic, force) and motors into any of the expander's ports **1~6**.

> [!WARNING]
> The EXP6 **supports SPIKE Prime only** (not Robot Inventor or EV3). **Before switching to another programming method, power the EXP6 off once** (turn off the hub or unplug the Host cable): once it has run a Pybricks program it keeps talking the Pybricks way until it restarts, and the official App cannot connect.

@@@ exp6-official
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

@@@ exp6-pybricks
### Import the EXP6 library

1. Download [📥 Pybricks library v2 + examples (.zip)](/downloads/MBC_EXP6_Pybricks_Lib_v2.zip) (v1 firmware shipped before 2026-10: [legacy zip](/downloads/MBC_EXP6_Pybricks_Lib.zip)). The manual is for **Pybricks 4.0.0 and later**.
2. Put the library in your Pybricks project. The examples say `from MBC_exp6_obj_Lib import MBC_EXP6`, so the file must be named `MBC_exp6_obj_Lib.py` (rename `MBC_exp6_obj_Lib_v2.py` from the v2 pack).

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

@@@ exp6-arduino
### The EXP6 uses UART, not I2C

The EXP6 talks to the Arduino over a **UART serial port** (two wires, TX / RX), **not the I2C wiring above**. A shipped EXP6 works as is, no reflashing: it starts in "LEGO mode" and switches to "UART mode" automatically on the first command packet from the Arduino (the same command set as Pybricks).

| Board | Usable? | Talks to the EXP6 on | Notes |
| :--- | :---: | :---: | :--- |
| **Arduino UNO R4** (Minima / WiFi) | ⭐ recommended | D0, D1 (`Serial1`) | D0 / D1 are separate from USB, so uploading and the Serial Monitor are unaffected |
| Arduino Mega 2560 | ✅ | 18, 19 (`Serial1`) | 4 serial ports |
| ESP32 dev board | ✅ | GPIO16, 17 (`Serial2`) | 3.3 V signals, pin 4 goes to 3V3; install the ESP32 board package first |
| Arduino UNO R3, Nano | ❌ | — | the only hardware serial port is used by USB; software serial is unreliable at 115200 |

Connect the EXP6 **Host port** to the Arduino with a "LEGO 6-pin to Dupont" cable (or a LEGO connector breakout):

| EXP6 Host port | UNO R4 | Mega 2560 | ESP32 |
| :---: | :---: | :---: | :---: |
| pin 3 GND | GND | GND | GND |
| pin 4 VCC | **5V** | **5V** | **3V3** |
| pin 5 (EXP6 receives) | D1 (TX) | 18 (TX1) | GPIO17 (TX2) |
| pin 6 (EXP6 sends) | D0 (RX) | 19 (RX1) | GPIO16 (RX2) |
| pins 1, 2 | not connected | not connected | not connected |

> [!IMPORTANT]
> - The EXP6 XT30 power socket must have a **2~3S lithium battery**; motors and sensors run from it. The EXP6 **does not power the Arduino**.
> - **TX to EXP6 receive, RX to EXP6 send**: the two wires cross.
> - **Pin 4 must match the board's signal voltage** (UNO R4, Mega: 5V; ESP32: 3V3). This pin is also the EXP6 "ignition key": without it the EXP6 does not start.
> - Unplug USB and the battery before wiring.

> [!WARNING]
> **Watchdog**: if the EXP6 hears nothing from the Arduino for **0.5 s**, it stops the motors. Always wait with `exp6Wait(ms)`, never `delay()`.

@@@ exp6-mbcblocks
### Wiring and power

1. Plug the EXP6 **Host cable** into any SPIKE Prime port (A~F).
2. The EXP6 must have its own power supply (battery box or adapter) for the motors.
3. Plug LEGO sensors and motors into the expander's ports 1~6.

> [!WARNING]
> The EXP6 **supports SPIKE Prime only**. **Before switching to another programming method, power the EXP6 off once** (turn off the hub or unplug the Host cable): once it has run a Pybricks program it keeps talking the Pybricks way until it restarts.

@@@ servo6-common
> [!NOTE]
> Servo6 is **coming soon**: circuit and firmware are done and final hub testing is under way; specifications follow the release version. **SPIKE Prime only** (not Robot Inventor or EV3).

### Wiring and power

1. **Host cable**: plug the expander's SPIKE cable into any hub port (A~F).
2. **Servos**: plug the 3-wire servo connectors into **CN1~CN6** following the board markings (signal / VCC / GND). Servos with 500~2500 µs signals such as SG90 and MG996R work.
3. **No battery needed**: after the handshake the hub sends battery voltage to the expander and the green LED changes from fast blinking to steady. Total servo current is limited to **about 0.75 A**; check the working current of high-torque servos first.

> [!TIP]
> At power-on the expander outputs no pulses, so servos do not twitch. The first angle command moves straight to position (no speed ramp); later commands use the speed you set.

@@@ servo6-official
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

@@@ servo6-pybricks
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
