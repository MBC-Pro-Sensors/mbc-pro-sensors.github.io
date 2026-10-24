### Calibrate first (only once)

The Pathfinder has to "learn" the black and white of your mat before its numbers are right. Calibration is stored in the sensor and survives power-off (until you change mats).

1. Plug in the sensor and turn the hub on (the sensor lights come on).
2. Hold the **calibration button for about 0.5 s**, then release it (do not keep holding it; holding it keeps restarting calibration).
3. When the **green calibration LED** (next to the button) lights up, sweep the sensor back and forth between the black line and the white floor **within 5 seconds**. All 16 sensing points must pass over **the darkest black line** and **the brightest white area**.
4. Keep the sensor **0.5~1.2 cm** above the surface while sweeping; higher reads poorly.
5. The green LED goes off = calibration done, saved in the sensor automatically.

**How do I know it worked?** On the white floor the "line width" is 0; on a normal black line it is 3~5.

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="Pathfinder calibration" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

### Proximity mode: line position

| Value | Range | Meaning |
| :--- | :--- | :--- |
| Line position | `-16 ~ +16` | 0 = black line in the centre, negative left, positive right; ±16 = line at the very edge |

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

### Remote mode: position + width + edges (recommended for competition)

Read the "value list" (4 bytes):

| Byte | Content | Range | Meaning |
| :---: | :--- | :--- | :--- |
| 0 | High-resolution position | `-100 ~ +100` | 0 centre, negative left, positive right; the best PID error input |
| 1 | Line width | `0 ~ 16` | Sensors triggered; detects crossings and T-junctions |
| 2 | Left edge light | `0 ~ 100` | Reflection of channel 1 (far left), 0 pure black, 100 pure white |
| 3 | Right edge light | `0 ~ 100` | Reflection of channel 16 (far right), 0 pure black, 100 pure white |

> [!TIP]
> When bytes 2 and 3 are both below 20, the front of the robot is fully on the cross line of a junction; a sudden change on one side means a fork is coming. Useful to avoid losing the line.

### MBC My Blocks

The example project contains three My Blocks; no extension pack to install. Each one decodes the data in the background and stores it in its own variables. **Put them inside a Loop** to always have the latest data:

1. `LinePos16_dataUpdate`: basic line following; updates `0_LinePos16` (`-16 ~ +16`, left negative, right positive). Good for beginners.
2. `LinePos100_dataUpdate`: high precision (needs a proper calibration); updates `0_LinePos100` (`-100 ~ +100`), `0_LineWidth16` (`0 ~ 16`) and the far-left / far-right edge light values.
3. `LineCh16_dataUpdate`: per-channel decoding; give a channel number `1 ~ 16`, get that channel's quantised light `1 ~ 9` (1 darkest, 9 brightest).

### Example program

- [📥 EV3-G example (.ev3)](/examples/line16/ev3/line16_ev3_official.ev3): open with the official EV3 software.

![Pathfinder 16-Way PID line following in EV3-G](/images/sensors/line16/ev3-official-example.webp)

### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).
