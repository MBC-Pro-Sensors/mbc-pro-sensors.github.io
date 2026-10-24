### What you need

- **Sensor**: {{product}} ({{edition}}, RJ12 plug). A SPIKE edition is not recognised on an EV3.
- **Brick**: LEGO MINDSTORMS EV3.
- **Cable**: the standard cable in the box, or an original LEGO EV3 cable.
- **Software**: LEGO MINDSTORMS **EV3 Classroom** (Scratch-style blocks, the same as SPIKE). The easiest start for first-time EV3 users.

### Install the software

1. Install EV3 Classroom from the [LEGO Education download page](https://education.lego.com/en-us/downloads/mindstorms-ev3/software/).
2. To open an example: download the `.lmsp` file → "**Open**" on the EV3 Classroom home screen → connect the EV3 → "**Download and Run**" at the bottom right.

### Make the software see the sensor

No extension and no driver needed. On the EV3 the sensor **emulates an original LEGO sensor** (most products show up as an "Infrared Sensor"); use the built-in sensor blocks.

- Classroom blocks look like Scratch, but the modes are exactly the same as in the official EV3 software: **Proximity / Beacon / Remote**, with the same ranges. See the readings below.
- Some products come with MBC My Blocks. They are already inside the example projects; open an example and they are there.

> [!IMPORTANT]
> **Do not switch modes in the middle of a program.** Every time the EV3 switches a sensor mode it stalls for about **0.5 s**. Pick one mode at the start and use it for the whole run.

### Wiring

1. Plug the sensor into EV3 **port {{port}}** with the cable (any of 1~4 works; choose the same port in the blocks).
2. **Turn the EV3 on first, then plug in the sensor.**
3. Latch up, push the plug in gently.

### First test

1. Download the `.lmsp` of example 1 on this page, open it, connect the EV3 and press "Download and Run".
2. If the number on the EV3 screen changes with what is in front of the sensor, everything is connected.

### FAQ

- **The number never changes?** Unplug the sensor and plug it back in (the EV3 must be on first).
- **Some examples have no Classroom version?** Examples marked "Pybricks / EV3 MicroPython / clev3r only" use a mode the Classroom blocks cannot read.
- **The program hiccups now and then?** It switches sensor modes; each switch costs about 0.5 s. Use one mode only.
