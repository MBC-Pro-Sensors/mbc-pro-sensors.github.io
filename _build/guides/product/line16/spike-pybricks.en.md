### Calibrate first (only once)

The Pathfinder has to "learn" the black and white of your mat before its numbers are right. Calibration is stored in the sensor and survives power-off (until you change mats).

1. Plug in the sensor and turn the hub on (the sensor lights come on).
2. Hold the **calibration button for about 0.5 s**, then release it (do not keep holding it; holding it keeps restarting calibration).
3. When the **green calibration LED** (next to the button) lights up, sweep the sensor back and forth between the black line and the white floor **within 5 seconds**. All 16 sensing points must pass over **the darkest black line** and **the brightest white area**.
4. Keep the sensor **0.5~1.2 cm** above the surface while sweeping; higher reads poorly.
5. The green LED goes off = calibration done, saved in the sensor automatically.

**How do I know it worked?** On the white floor the "line width" is 0; on a normal black line it is 3~5.

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="Pathfinder calibration" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

### Library and example downloads

The hub needs **Pybricks firmware v4.1**. The zip has `MBC_sensors.py`, `MBC_line16_obj_Lib.py` and 4 examples:

- [📥 Download pack (Pybricks 4.1)](/examples/line16/line16_pybricks_spike_v4.x.zip)

Library only: [MBC_sensors.py](/examples/line16/pybricks/For%20firmware%204.x/MBC_sensors.py) (use this for new projects: one file for every MBC sensor) · [MBC_line16_obj_Lib.py](/examples/line16/pybricks/For%20firmware%204.x/MBC_line16_obj_Lib.py) (for the examples on this page and older programs)

> [!IMPORTANT]
> The examples say `from MBC_line16_obj_Lib import MBC_LINE16`, so import `MBC_line16_obj_Lib.py` (no renaming needed). For a new program of your own, write `from MBC_sensors import MBC_LINE16` and import only `MBC_sensors.py`. The library must be in the **same project list** as the main program.

> [!NOTE]
> Pybricks 3.6.1 is no longer updated; the old files are under "Legacy (no longer updated)" on the [Downloads page](/en/downloads.md).

![Importing the library into a Pybricks project](/images/sensors/line16/pybricks-upload-lib.webp)

### Method 1: built-in sensor commands (no library)

Read with Pybricks' built-in `ColorSensor` commands, no extra file. These values are computed from the built-in commands:

| Set | Function | Range | Meaning |
| :--- | :--- | :--- | :--- |
| Basic (less sensitive to calibration, but calibrate once) | `getLinePos16()` | `-16 ~ +16` | Line position (integer), 0 centre, negative left, positive right |
| | `getLineWidth16()` | `0 ~ 16` | Line width (sensors triggered), for crossings and T-junctions |
| High precision (calibrate properly first) | `getLinePos100()` | `-100 ~ +100` | High-resolution interpolated position for advanced PID |
| | `getLineWidth100()` | `0 ~ 9` | Line width in high-precision mode, in sync with `getLinePos100()` |

> [!TIP]
> Use position and width from **the same set**: `getLinePos16()` with `getLineWidth16()`, `getLinePos100()` with `getLineWidth100()`, so both come from the same moment.

Examples: blocks [line16_block_native.py](/examples/line16/pybricks/For%20firmware%204.x/line16_block_native.py); Python [line16_python_native.py](/examples/line16/pybricks/For%20firmware%204.x/line16_python_native.py)

> [!IMPORTANT]
> **Decoding on Pybricks 4.1** (the examples above already do this)
> - `reflection()` (mode 5) = coarse position + width. From Pybricks 4.1 it carries one decimal, so **convert to an integer first**: `r = int(line.reflection())`; 16-ch: add 6399 when negative, then position = `r // 100 - 16` (−16 to +16), width = `r % 100`. Blocks have no "integer" block: use "floor" for values ≥ 0 and "ceiling" for negative ones, which equals `int()`.
> - `ambient()` (mode 7) = "fine position + 100 . width", width in the first decimal: fine position = integer part − 100, width = `round(line.ambient() * 10) % 10` (16-ch shows at most 9 = 9 or more). **Needs sensor firmware from 2026-10-30 or later**; on older firmware the width reads 0 (position still works).
> - Positions are sent with an offset and subtracted after reading; negative numbers are never sent directly.
> - Advanced (Python only): `line.hsv(surface=False).h` = (fine position + 100) × 100 + width, giving the full width.

### Method 2: import the library

| Function | Range | Meaning |
| :--- | :--- | :--- |
| `line_init_port_multitask(port, multitask)` | — | Initialise. `port` = 1~6 for A~F; `multitask=True` enables async mode (use with `await`) |
| `line_pos16()` | `-16 ~ +16` | Integer line position, less sensitive to calibration (without calibration every channel reads as black) |
| `line_pos100()` | `-100 ~ +100` | High-resolution interpolated position, best for PID; calibrate properly |
| `line_width()` | `0 ~ 16` | Line width, works with both position functions |
| `line_ir_calibration()` | `list[16]` | 16 calibrated values (each `0 ~ 15`). Seen from behind the robot: index 0 = far left (ch1) |
| `line_ir_ch(ch)` | `0 ~ 15` | One calibrated channel, `ch` = 1~16; fast to call repeatedly (shared cache) |
| `line_junction()` | `0, 1, 2+` | Number of line groups: 0 none, 1 single line, 2+ junction |
| `line_junction_name()` | `str` | `"none"`, `"line"`, `"junction"` |
| `line_bin_raw()` | `0 ~ 65535` | 16-bit trigger mask, bit0 = ch1, 1 = line seen |
| `line_on_sensors()` | `list[int]` | Channels seeing the line, e.g. `[7, 8, 9]` |
| `line_read_all()` | `dict` | Everything at once: `ir_calibration`, `line_pos16`, `line_pos100`, `line_width`, `bin_raw`, `on_sensors`, `junction`, `junction_name` |

Examples: blocks [line16_block_with_lib.py](/examples/line16/pybricks/For%20firmware%204.x/line16_block_with_lib.py); Python [line16_python_with_lib.py](/examples/line16/pybricks/For%20firmware%204.x/line16_python_with_lib.py)

### Command examples (calibration, mirror)

These send commands with the shared MBC library (see "Commands" further down this page for each command). First upload [MBC_sensors.py](/examples/line16/pybricks/For%20firmware%204.x/MBC_sensors.py) to Pybricks Code into the same project list; sensor on **port C**; follow the hints in the output pane and press the hub's **left button** to go on (the centre button still stops the program). Needs sensor firmware from 2026-10 or later. All 3 have been run on a hub.

| Example | What it does | How to tell it worked |
| :--- | :--- | :--- |
| [line16_cmd_calib_bw.py](/examples/line16/pybricks/For%20firmware%204.x/line16_cmd_calib_bw.py) | Black/white calibration (E2 black, E3 white): put the whole row on black and press left, then on white paper and press left (keep the height fixed) | Black light values all 0; white-paper light values (`ir_calibration()`, 0–15) 13 or more |
| [line16_cmd_calib_auto.py](/examples/line16/pybricks/For%20firmware%204.x/line16_cmd_calib_auto.py) | E7 5-second auto calibration: sweep the sensor across the black line and white surface for 5 s (fixed height, no tilt) | State goes from busy to done; white-paper light values 11 or more (quality depends on how you sweep) |
| [line16_cmd_reverse.py](/examples/line16/pybricks/For%20firmware%204.x/line16_cmd_reverse.py) | E9 mirror left/right, and shows that a wrong parameter returns "failed" | Line position changes sign (e.g. −15 → +15, send 0 to switch back); sending 5 returns "failed" |

### PD line-following demos (speed 50)

One PD line-following logic in 4 reading methods, each as blocks and as Python: 8 programs. They stop at a crossing (the whole row on the line). All 8 have been run on a Pybricks hub.

- [📥 Download all (8 Pybricks programs + libraries + official app versions)](/examples/line16/line16_pd50_examples.zip)

| Method | Reads | Suggested KP / KD | Stops at crossing | Blocks | Python |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Built-in reflection (mode 5) | `ColorSensor.reflection()`: coarse position + width | 2.5 / 10 | width > 10 | [mode5_block](/examples/line16/pd50/pybricks/line16_pd50_mode5_block.py) | [mode5_python](/examples/line16/pd50/pybricks/line16_pd50_mode5_python.py) |
| Built-in ambient (mode 7) | `ColorSensor.ambient()`: fine position + width (firmware 2026-10-30 or later) | 0.4 / 1.5 | width > 8 | [mode7_block](/examples/line16/pd50/pybricks/line16_pd50_mode7_block.py) | [mode7_python](/examples/line16/pd50/pybricks/line16_pd50_mode7_python.py) |
| Product library | `MBC_line16_obj_Lib`: `pos100()` + `width()` (mode 9) | 0.4 / 1.5 | width > 10 | [lib_block](/examples/line16/pd50/pybricks/line16_pd50_lib_block.py) | [lib_python](/examples/line16/pd50/pybricks/line16_pd50_lib_python.py) |
| Shared library | `MBC_sensors`: same | 0.4 / 1.5 | width > 10 | [sensors_block](/examples/line16/pd50/pybricks/line16_pd50_sensors_block.py) | [sensors_python](/examples/line16/pd50/pybricks/line16_pd50_sensors_python.py) |

- **Wiring**: sensor on port C, left motor on E (set to COUNTERCLOCKWISE in the program), right motor on F.
- Speed 50 (power %). KP and KD are starting values; tune them on your robot: lower KP if it wobbles, lower KD if turns are too slow.
- For the two library methods, first upload `MBC_line16_obj_Lib.py` or `MBC_sensors.py` to Pybricks Code into the same project list as the program (included in the zip).
- The block files open as Pybricks blocks; the first line holds the block data, so import the whole file rather than copying only the Python part.

### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).
