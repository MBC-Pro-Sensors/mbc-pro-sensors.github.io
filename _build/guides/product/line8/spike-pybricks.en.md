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

Pick the pack that matches the hub's Pybricks firmware (each zip has the library + 4 examples):

- [📥 Download pack (firmware 4.0.0)](/examples/line8/line8_pybricks_spike_v4.0.0.zip)
- [📥 Download pack (firmware 3.6.1)](/examples/line8/line8_pybricks_spike_v3.6.1.zip)

Library only: [4.0.0 MBC_line8_obj_Lib_v400.py](/examples/line8/pybricks/For%20firmware%204.0.0/MBC_line8_obj_Lib_v400.py) · [3.6.1 MBC_line8_Lib_v361.py](/examples/line8/pybricks/For%20firmware%203.6.1/MBC_line8_Lib_v361.py)

> [!IMPORTANT]
> **Rename the file**: the examples say `import MBC_line8_obj_Lib` (3.6.1: `MBC_line8_Lib`), so rename the download to `MBC_line8_obj_Lib.py` (drop `_v400`) or `MBC_line8_Lib.py` (drop `_v361`), or you will get an `ImportError`. The library must be in the **same project list** as the main program.

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

Examples: blocks [4.0.0](/examples/line8/pybricks/For%20firmware%204.0.0/line8_block_native_v400.py) · [3.6.1](/examples/line8/pybricks/For%20firmware%203.6.1/line8_block_native_v361.py); Python [4.0.0](/examples/line8/pybricks/For%20firmware%204.0.0/line8_python_native_v400.py) · [3.6.1](/examples/line8/pybricks/For%20firmware%203.6.1/line8_python_native_v361.py)

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

Examples: blocks [4.0.0](/examples/line8/pybricks/For%20firmware%204.0.0/line8_block_with_lib_v400.py) · [3.6.1](/examples/line8/pybricks/For%20firmware%203.6.1/line8_block_with_lib_v361.py); Python [4.0.0](/examples/line8/pybricks/For%20firmware%204.0.0/line8_python_with_lib_v400.py) · [3.6.1](/examples/line8/pybricks/For%20firmware%203.6.1/line8_python_with_lib_v361.py)

### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).
