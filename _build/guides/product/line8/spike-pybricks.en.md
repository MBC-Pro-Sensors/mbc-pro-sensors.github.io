### Calibrate first (only once)

The Pathfinder has to "learn" the black and white of your mat before its numbers are right. Calibration is stored in the sensor and survives power-off (until you change mats).

1. Plug in the sensor and turn the hub on (the sensor lights come on).
2. Hold the **calibration button for about 0.5 s**, then release it (do not keep holding it; holding it keeps restarting calibration).
3. When the **green calibration LED** (next to the button) lights up, sweep the sensor back and forth between the black line and the white floor **within 5 seconds**. All 8 sensing points must pass over **the darkest black line** and **the brightest white area**.
4. Keep the sensor **0.5~1.2 cm** above the surface while sweeping; higher reads poorly.
5. The green LED goes off = calibration done, saved in the sensor automatically.

**How do I know it worked?** On the white floor the "line width" is 0; on a normal black line it is 2~3.

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="Pathfinder calibration" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

### Library and example downloads

The hub needs **Pybricks firmware v4.1**. The zip has `MBC_sensors.py`, `MBC_line8_obj_Lib.py` and 4 examples:

- [📥 Download pack (Pybricks 4.1)](/examples/line8/line8_pybricks_spike_v4.x.zip)

Library only: [MBC_sensors.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_sensors.py) (use this for new projects: one file for every MBC sensor) · [MBC_line8_obj_Lib.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_line8_obj_Lib.py) (for the examples on this page and older programs)

> [!IMPORTANT]
> The examples say `from MBC_line8_obj_Lib import MBC_LINE8`, so import `MBC_line8_obj_Lib.py` (no renaming needed). For a new program of your own, write `from MBC_sensors import MBC_LINE8` and import only `MBC_sensors.py`. The library must be in the **same project list** as the main program.

> [!NOTE]
> Pybricks 3.6.1 is no longer updated; the old files are under "Legacy (no longer updated)" on the [Downloads page](/en/downloads.md).

![Importing the library into a Pybricks project](/images/sensors/line8/pybricks-upload-lib.webp)

### Method 1: built-in sensor commands (no library)

Read with Pybricks' built-in `ColorSensor` commands, no extra file. These values are computed from the built-in commands:

| Set | Function | Range | Meaning |
| :--- | :--- | :--- | :--- |
| Basic (less sensitive to calibration, but calibrate once) | `getLinePos8()` | `-8 ~ +8` | Line position (integer), 0 centre, negative left, positive right |
| | `getLineWidth8()` | `0 ~ 8` | Line width (sensors triggered), for crossings and T-junctions |
| High precision (calibrate properly first) | `getLinePos100()` | `-100 ~ +100` | High-resolution interpolated position for advanced PID |
| | `getLineWidth100()` | `0 ~ 8` | Line width in high-precision mode, in sync with `getLinePos100()` |

> [!TIP]
> Use position and width from **the same set**: `getLinePos8()` with `getLineWidth8()`, `getLinePos100()` with `getLineWidth100()`, so both come from the same moment.

Examples: blocks [line8_block_native.py](/examples/line8/pybricks/For%20firmware%204.x/line8_block_native.py); Python [line8_python_native.py](/examples/line8/pybricks/For%20firmware%204.x/line8_python_native.py)

> [!IMPORTANT]
> **Decoding on Pybricks 4.1** (the examples above already do this)
> - `reflection()` (mode 5) = coarse position + width. From Pybricks 4.1 it carries one decimal, so **convert to an integer first**: `r = int(line.reflection())`; 8-ch: position = `int(r) // 10 - 8` (−8 to +8), width = `int(r) % 10`. Blocks have no "integer" block: use "floor" for values ≥ 0 and "ceiling" for negative ones, which equals `int()`.
> - `ambient()` (mode 7) = "fine position + 100 . width", width in the first decimal: fine position = integer part − 100, width = `round(line.ambient() * 10) % 10`. **Needs sensor firmware from 2026-10-30 or later**; on older firmware the width reads 0 (position still works).
> - Positions are sent with an offset and subtracted after reading; negative numbers are never sent directly.
> - Advanced (Python only): `line.hsv(surface=False).h` = (fine position + 100) × 100 + width, giving the full width.

### Method 2: import the library

| Function | Range | Meaning |
| :--- | :--- | :--- |
| `line_init_port_multitask(port, multitask)` | — | Initialise. `port` = 1~6 for A~F; `multitask=True` enables async mode (use with `await`) |
| `line_pos8()` | `-8 ~ +8` | Integer line position, less sensitive to calibration (without calibration every channel reads as black) |
| `line_pos100()` | `-100 ~ +100` | High-resolution interpolated position, best for PID; calibrate properly |
| `line_width()` | `0 ~ 8` | Line width, works with both position functions |
| `line_ir_calibration()` | `list[8]` | 8 calibrated values (each `0 ~ 100`). Seen from behind the robot: index 0 = far left (ch1) |
| `line_ir_ch(ch)` | `0 ~ 100` | One calibrated channel, `ch` = 1~8; fast to call repeatedly (shared cache) |
| `line_junction()` | `0, 1, 2+` | Number of line groups: 0 none, 1 single line, 2+ junction |
| `line_junction_name()` | `str` | `"none"`, `"line"`, `"junction"` |
| `line_bin_raw()` | `0 ~ 255` | 8-bit trigger mask, bit0 = ch1, 1 = line seen |
| `line_on_sensors()` | `list[int]` | Channels seeing the line, e.g. `[4, 5]` |
| `line_read_all()` | `dict` | Everything at once: `ir_calibration`, `line_pos8`, `line_pos100`, `line_width`, `bin_raw`, `on_sensors`, `junction`, `junction_name` |

Examples: blocks [line8_block_with_lib.py](/examples/line8/pybricks/For%20firmware%204.x/line8_block_with_lib.py); Python [line8_python_with_lib.py](/examples/line8/pybricks/For%20firmware%204.x/line8_python_with_lib.py)

### PD line-following demos (speed 50)

One PD line-following logic in 4 reading methods, each as blocks and as Python: 8 programs. They stop at a crossing (the whole row on the line). All 8 have been run on a Pybricks hub.

- [📥 Download all (8 Pybricks programs + libraries + official app versions)](/examples/line8/line8_pd50_examples.zip)

| Method | Reads | Suggested KP / KD | Stops at crossing | Blocks | Python |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Built-in reflection (mode 5) | `ColorSensor.reflection()`: coarse position + width | 5 / 20 | width > 5 | [mode5_block](/examples/line8/pd50/pybricks/line8_pd50_mode5_block.py) | [mode5_python](/examples/line8/pd50/pybricks/line8_pd50_mode5_python.py) |
| Built-in ambient (mode 7) | `ColorSensor.ambient()`: fine position + width (firmware 2026-10-30 or later) | 0.4 / 1.5 | width > 5 | [mode7_block](/examples/line8/pd50/pybricks/line8_pd50_mode7_block.py) | [mode7_python](/examples/line8/pd50/pybricks/line8_pd50_mode7_python.py) |
| Product library | `MBC_line8_obj_Lib`: `pos100()` + `width()` (mode 9) | 0.4 / 1.5 | width > 5 | [lib_block](/examples/line8/pd50/pybricks/line8_pd50_lib_block.py) | [lib_python](/examples/line8/pd50/pybricks/line8_pd50_lib_python.py) |
| Shared library | `MBC_sensors`: same | 0.4 / 1.5 | width > 5 | [sensors_block](/examples/line8/pd50/pybricks/line8_pd50_sensors_block.py) | [sensors_python](/examples/line8/pd50/pybricks/line8_pd50_sensors_python.py) |

- **Wiring**: sensor on port C, left motor on E (set to COUNTERCLOCKWISE in the program), right motor on F.
- Speed 50 (power %). KP and KD are starting values; tune them on your robot: lower KP if it wobbles, lower KD if turns are too slow.
- For the two library methods, first upload `MBC_line8_obj_Lib.py` or `MBC_sensors.py` to Pybricks Code into the same project list as the program (included in the zip).
- The block files open as Pybricks blocks; the first line holds the block data, so import the whole file rather than copying only the Python part.

### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).
