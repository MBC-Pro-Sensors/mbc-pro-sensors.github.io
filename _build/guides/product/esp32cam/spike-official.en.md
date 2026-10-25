> [!TIP]
> **🧩 New to the Sharpshooter Vision (school edition)? Follow the step-by-step interactive tutorial:** [LEGO SPIKE App](/en/learn/cam_school/spike-app.html). Pick options on the left and watch the blocks build up on the right; every function has an example, and a simulated sensor lets you try the values.

> [!TIP]
> **🧩 New to the Sharpshooter Vision WRO edition (HUE12)? Follow the step-by-step interactive tutorial:** [LEGO SPIKE App](/en/learn/hue12/spike-app.html). Pick options on the left and watch the blocks build up on the right; every function has an example, and a simulated sensor lets you try the values.

### Set the colour on the sensor first

All Sharpshooter settings are made on the **colour screen and buttons on the back of the sensor**; no computer needed:

1. The sensor only starts when plugged into a hub that is **already on** (the screen first shows "LEGO Sync..."); once the camera image appears it is ready.
2. **Short-press the side button** to open the settings menu; left / right to choose, OK to enter. In the menu, hold the side button for 1 s to clear that colour set.
3. Colour settings and camera parameters (brightness, contrast, saturation, exposure, white balance, flip, mirror) are saved automatically and used at the next power-on.

> [!NOTE]
> - Official Education App blocks: "Color" = hue, "Reflected light" = motion value, "Raw red / green / blue" = blob X / Y / area.
> - In the home edition (Robot Inventor) "Color" only goes **0~10** (a tenth of the Education edition); change the numbers in example 2 yourself.
> - The numbers on this page are for **current shipping firmware** (hue 0~100). To check: after power-on look at the `X` number at the top of the sensor screen. If it **only goes up to about 98**, it is the new firmware; if it goes above 100 (up to 159), it is old firmware; contact us for an update first.

### WRO edition (HUE12)

The official LEGO App **has no blocks for HUE12** (HUE12 is not a LEGO sensor), so the official-App examples are **Python programs for the Education SPIKE 3 App**. For blocks, use the MBC Blocks website.
