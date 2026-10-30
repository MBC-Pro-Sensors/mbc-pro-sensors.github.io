### Calibrate first (only once)

The Pathfinder has to "learn" the black and white of your mat before its numbers are right. Calibration is stored in the sensor and survives power-off (until you change mats).

1. Plug in the sensor and turn the hub on (the sensor lights come on).
2. Hold the **calibration button for about 0.5 s**, then release it (do not keep holding it; holding it keeps restarting calibration).
3. When the **green calibration LED** (next to the button) lights up, sweep the sensor back and forth between the black line and the white floor **within 5 seconds**. All 8 sensing points must pass over **the darkest black line** and **the brightest white area**.
4. Keep the sensor **0.5~1.2 cm** above the surface while sweeping; higher reads poorly.
5. The green LED goes off = calibration done, saved in the sensor automatically.

**How do I know it worked?** On the white floor the "line width" is 0; on a normal black line it is 2~3.

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="Pathfinder calibration" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

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

### PD line-following demos (speed 50)

The same PD logic as the Pybricks version, as blocks and Python for both the Education and the Robot Inventor app: 4 programs, all run on a hub.

- Education (SPIKE App 3): [📥 blocks .llsp3](/examples/line8/pd50/spike_app/line8_pd50_edu_blocks.llsp3) · [📥 Python (helper built in)](/examples/line8/pd50/spike_app/line8_pd50_edu_python.py)
- Robot Inventor App: [📥 blocks .lms](/examples/line8/pd50/spike_app/line8_pd50_ri_blocks.lms) · [📥 Python (helper built in)](/examples/line8/pd50/spike_app/line8_pd50_ri_python.py)
- [📥 Download all (with the Pybricks versions)](/examples/line8/line8_pd50_examples.zip)

- **Wiring**: sensor on port C, left motor on E, right motor on F. The motors face opposite ways, so **the left motor needs negative power to drive forward**: left = −(speed + PD), right = speed − PD.
- **Reading**: fine position = "raw red" − 100 (both apps). Width: **blocks (Education and Robot Inventor alike) use the "color" block**; only **Robot Inventor Python** uses reflected light `get_reflected_light()`.
- Speed 50 (power %), KP 0.4 and KD 1.5 are starting values; stop at a crossing: width > 5.
- Works with older firmware too: the official apps read mode 0, which the mode 7 change does not touch.

### Robot Inventor App Python

> [!WARNING]
> LEGO ended support for the Robot Inventor App on 2026-10-01. This section is only for people who still have a Robot Inventor hub; for new projects use SPIKE App 3 or Pybricks.

- Reading (measured on hardware): fine position = `get_rgb_intensity()[0] − 100`, width = `get_reflected_light()`. `get_color()` only names a few positions, so it is not a position; coarse position and per-channel readings are not available in this app.
- Helper: [MBC_line8_ri_App_helper.py](/examples/line8/spike_app_ri/MBC_line8_ri_App_helper.py) (class `Line8RI`: `pos100()`, `width()`); paste the whole file at the top of your program.
- Example: [ex01_line_follow.py](/examples/line8/spike_app_ri/ex01_line_follow.py) (helper already included): line following + stop at a crossing, wiring C / E / F, run on a 51515 hub (App 10.5.4).

### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).
