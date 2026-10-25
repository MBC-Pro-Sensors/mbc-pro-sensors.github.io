> [!TIP]
> **🧩 New to the Pathfinder 16-Way? Follow the step-by-step interactive tutorial:** [LEGO SPIKE App](/en/learn/line16/spike-app.html). Pick options on the left and watch the blocks build up on the right; every function has an example, and a simulated sensor lets you try the values.

### Calibrate first (only once)

The Pathfinder has to "learn" the black and white of your mat before its numbers are right. Calibration is stored in the sensor and survives power-off (until you change mats).

1. Plug in the sensor and turn the hub on (the sensor lights come on).
2. Hold the **calibration button for about 0.5 s**, then release it (do not keep holding it; holding it keeps restarting calibration).
3. When the **green calibration LED** (next to the button) lights up, sweep the sensor back and forth between the black line and the white floor **within 5 seconds**. All 16 sensing points must pass over **the darkest black line** and **the brightest white area**.
4. Keep the sensor **0.5~1.2 cm** above the surface while sweeping; higher reads poorly.
5. The green LED goes off = calibration done, saved in the sensor automatically.

**How do I know it worked?** On the white floor the "line width" is 0; on a normal black line it is 3~5.

<iframe src="https://www.youtube.com/embed/TKi7b20x8UY" title="Pathfinder calibration" frameborder="0" allowfullscreen loading="lazy" style="width:100%;aspect-ratio:16/9;border:none;border-radius:12px;"></iframe>

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

### Want to learn more advanced line following?

These coaches have plenty of competition and teaching experience: [Coach LegoLauXiao](https://www.youtube.com/@LegoLauXiao) ([demo video](https://www.youtube.com/watch?v=WgacdWLatbk)), [Coach legolaumo](https://www.youtube.com/@legolaumo) ([demo video](https://www.youtube.com/watch?v=T9bcndBNQvQ)).
