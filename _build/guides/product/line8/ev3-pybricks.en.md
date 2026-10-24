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

- [📥 Download pack (examples + library)](/examples/line8/line8_pybricks_ev3.zip)
- Library: [MBC_line8_EV3_Lib.py](/examples/line8/ev3/MBC_line8_EV3_Lib.py). Import it straight into your Pybricks project; **the file name must match exactly** for the `import` to load it.

![Importing the library into an EV3 Pybricks project](/images/sensors/line8/ev3-pybricks-upload-lib.webp)

### Method 1: native commands

Use Pybricks' low-level `device.mode(mode)` to get the data array directly:

| Command | Returns | Content |
| :--- | :--- | :--- |
| `device.mode(0)` | Tuple (1 byte) | Beginner mode: position `-8 ~ 8`, 0 = centre |
| `device.mode(1)` | Tuple (8 bytes) | Overview / debug mode: 8 calibrated reflection values, each `0 ~ 100` (0 = darkest, 100 = brightest), channel 1 on the far left |
| `device.mode(2)` | Tuple (4 bytes) | Competition mode: high-precision position `-100 ~ 100`, width `0 ~ 8`, left edge light, right edge light |

> [!NOTE]
> Pybricks has not yet updated the matching graphical and low-level commands, so native examples will follow later; for now Method 2 is recommended.

### Method 2: import the library

Initialise with `line_init_port_multitask(port, multitask)`: `port` is an EV3 port object (for example `hub.port.S1`). **Not multitask**: `multitask = False`, and choose `call` in front of each call; **multitask**: `True`, and choose `await`.

![Non-multitask setup](/images/sensors/line8/ev3-pybricks-non-multitask.webp)

![Multitask setup (await)](/images/sensors/line8/ev3-pybricks-multitask.webp)

| Mode | Function | Range | Meaning |
| :--- | :--- | :--- | :--- |
| Mode 0 (basic) | `line_pos8()` | `-8 ~ +8` | Integer line position, 0 centre |
| Mode 1 (decode) | `line_ir_ch(ch)` | `1 ~ 9` | One channel's quantised light, `ch` = 1~8, 1 darkest, 9 brightest |
| Mode 2 (competition) | `line_pos100()` | `-100 ~ +100` | High-resolution interpolated position for PID / PD |
| Mode 2 (competition) | `line_width()` | `0 ~ 8` | Line width, for junctions and wide lines |
| Mode 2 (competition) | `line_edge_l()` | `0 ~ 100` | Left edge light (0 black, 100 white), to avoid losing the line |
| Mode 2 (competition) | `line_edge_r()` | `0 ~ 100` | Right edge light (0 black, 100 white), to avoid losing the line |

> [!IMPORTANT]
> Mixing functions from different modes costs a mode-switch delay. For normal use and competition, **stick to the mode 2 functions**; use mode 1 on its own only when you need every raw sensor value for your own algorithm.

Examples: [blocks (.py)](/examples/line8/ev3/line8_ev3_block_with_lib.py) · [Python (.py)](/examples/line8/ev3/line8_ev3_python_with_lib.py)

### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).
