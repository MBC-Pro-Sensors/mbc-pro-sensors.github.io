### What you need

- **Sensor**: {{product}} ({{edition}}). Each platform is a different factory edition; an EV3 edition is not recognised on a SPIKE hub.
- **Hub**: SPIKE Prime (Education, yellow hub) or Robot Inventor (home edition, teal hub).
- **Cable**: the standard cable in the box, or an original LEGO cable.
- **Software**: Education uses the LEGO Education SPIKE App (the examples on this site use **SPIKE 3**); the home edition uses the LEGO MINDSTORMS Robot Inventor App.
- **Hub firmware**: the original LEGO firmware. **No need to switch to Pybricks.**

### Install the software

1. Education: install the **SPIKE App** from the [LEGO Education download page](https://education.lego.com/en-us/downloads/spike-app/software/).
2. Home edition: install the **LEGO MINDSTORMS Robot Inventor App**.
3. If the hub has been flashed with Pybricks, switch it back to the original LEGO firmware with the SPIKE App first, or the official App cannot connect.

> [!NOTE]
> The Education and home editions read values differently: in the home-edition App some blocks are limited to `0 ~ 126`, and "Color" only goes up to 10. Use the readings for the edition you have.

### Make the software see the sensor

No extension and no driver needed. Plug the sensor into the hub and the official App treats it as an original LEGO sensor (for most products a **SPIKE Color Sensor**); just use that sensor's blocks.

- An official block returns one number at a time, so the product data is encoded into the "Color", "Reflected light" and "Raw red / green / blue" slots. See the readings below for what each slot means.
- To open an example: download the `.llsp3` (home edition: `.lms`) → open the App → top-left "File" → "Open" → pick the file.

### Wiring

1. Plug the sensor into hub **port {{port}}** with the cable (any of A~F works; choose the same port in your program).
2. The plug only goes in one way: latch up, push it in gently, do not force it.
3. When the hub is on, the light on the sensor comes on.

### First test

1. Connect the hub to the App over USB or Bluetooth.
2. Open example 1 on this page and press Run.
3. Move your hand or the mat in front of the sensor: if **the number changes**, everything is connected.

### FAQ

- **Nothing happens when I plug it in?** Check in order: both ends of the cable firmly in → another hub port → another cable → another hub.
- **Different Apps give different numbers?** The Education (yellow hub) and home (teal hub) editions use different formats. Check which edition you have and use its readings.
- **Do I need an adapter or a driver?** No. The connector and protocol are the same as a LEGO hub port; it is recognised automatically.
- **Example 6 won't open in the official App?** Examples marked "Pybricks only" use a new mode in firmware from 2026-10 on, which the official App blocks cannot read.
