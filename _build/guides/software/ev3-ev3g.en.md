### What you need

- **Sensor**: {{product}} ({{edition}}, RJ12 plug). A SPIKE edition is not recognised on an EV3.
- **Brick**: LEGO MINDSTORMS EV3.
- **Cable**: the standard cable in the box, or an original LEGO EV3 cable.
- **Software**: the official LEGO MINDSTORMS EV3 software (LabVIEW-style interface, usually called **EV3-G**). The most common choice in school computer labs and WRO teams.

### Install the software

1. Install the official EV3 software (EV3 Lab) from the [LEGO Education download page](https://education.lego.com/en-us/downloads/retiredproducts/mindstorms-ev3-lab/software/).
2. To open an example: download the `.ev3` file → "**File → Open Project**" → connect the EV3 → "**Download and Run**" at the bottom right.

### Make the software see the sensor

No extension and no driver needed. On the EV3 the sensor **emulates an original LEGO sensor** (most products show up as an "Infrared Sensor"); use the built-in sensor blocks and pick the right mode.

- The infrared sensor has three modes, **Proximity**, **Beacon** and **Remote**; the product puts different data in different modes. See the readings below.
- Some products come with MBC My Blocks. They are already inside the example projects; open an example and they are there.

> [!IMPORTANT]
> **Do not switch modes in the middle of a program.** Every time the EV3 switches a sensor mode (for example from Proximity to Remote) it stalls for about **0.5 s**, which can make a fast robot lose control. Pick one mode at the start and use it for the whole run.

### Wiring

1. Plug the sensor into EV3 **port {{port}}** with the cable (any of 1~4 works; choose the same port in the blocks).
2. **Turn the EV3 on first, then plug in the sensor.**
3. Latch up, push the plug in gently; do not force it.

### First test

1. Download the `.ev3` of example 1 on this page, open it and press "Download and Run".
2. If the number on the EV3 screen changes with what is in front of the sensor, everything is connected.

### FAQ

- **The number never changes?** Unplug the sensor and plug it back in (the EV3 must be on first).
- **The EV3 shows an "Infrared Sensor"?** That is normal: read the values with the infrared sensor blocks.
- **Some examples have no EV3-G version?** Examples marked "Pybricks / EV3 MicroPython / clev3r only" use a mode the EV3-G blocks cannot read.
- **The program hiccups now and then?** It switches sensor modes; each switch costs about 0.5 s. Use one mode only.
