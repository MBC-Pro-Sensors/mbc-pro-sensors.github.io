### Calibrate first (only once)

The Pathfinder has to "learn" the black and white of your mat before its numbers are right. Calibration is stored in the sensor and survives power-off (until you change mats).

1. Plug in the sensor and turn the hub on (the sensor lights come on).
2. Hold the **calibration button for about 0.5 s**, then release it (do not keep holding it; holding it keeps restarting calibration).
3. When the **green calibration LED** (next to the button) lights up, sweep the sensor back and forth between the black line and the white floor **within 5 seconds**. All 8 sensing points must pass over **the darkest black line** and **the brightest white area**.
4. Keep the sensor **0.5~1.2 cm** above the surface while sweeping; higher reads poorly.
5. The green LED goes off = calibration done, saved in the sensor automatically.

**How do I know it worked?** On the white floor the "line width" is 0; on a normal black line it is 2~3.

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="Pathfinder calibration" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

### Proximity mode: line position

| Value | Range | Meaning |
| :--- | :--- | :--- |
| Line position | `-8 ~ +8` | 0 = black line in the centre, negative left, positive right; ±8 = line at the very edge |

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

### Remote mode: position + width + edges (recommended for competition)

Read the "value list" (4 bytes):

| Byte | Content | Range | Meaning |
| :---: | :--- | :--- | :--- |
| 0 | High-resolution position | `-100 ~ +100` | 0 centre, negative left, positive right; the best PID error input |
| 1 | Line width | `0 ~ 8` | Sensors triggered; detects crossings and T-junctions |
| 2 | Left edge light | `0 ~ 100` | Reflection of channel 1 (far left), 0 pure black, 100 pure white |
| 3 | Right edge light | `0 ~ 100` | Reflection of channel 8 (far right), 0 pure black, 100 pure white |

> [!TIP]
> When bytes 2 and 3 are both below 20, the front of the robot is fully on the cross line of a junction; a sudden change on one side means a fork is coming. Useful to avoid losing the line.

### MBC My Blocks

The example project contains three My Blocks; no extension pack to install. Each one decodes the data in the background and stores it in its own variables. **Put them inside a "forever" loop** to always have the latest data:

1. `LinePos8_dataUpdate`: basic line following; updates `0_LinePos8` (`-8 ~ +8`, left negative, right positive). Good for beginners.
2. `LinePos100_dataUpdate`: high precision (needs a proper calibration); updates `0_LinePos100` (`-100 ~ +100`), `0_LineWidth8` (`0 ~ 8`) and the far-left / far-right edge light values.
3. `LineCh8_dataUpdate`: full array; updates `s1` to `s8`, 8 variables matching each sensor from left to right.

### Example program

- [📥 Classroom example (.lmsp)](/examples/line8/ev3/line8_ev3_classroom.lmsp): open with EV3 Classroom.

![Pathfinder 8-Way PID line following in EV3 Classroom](/images/sensors/line8/ev3-classroom-example.webp)

### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).
