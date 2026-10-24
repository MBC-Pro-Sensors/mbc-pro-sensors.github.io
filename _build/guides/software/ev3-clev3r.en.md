### What you need

- **Sensor**: {{product}} ({{edition}}, RJ12 plug).
- **Brick**: LEGO MINDSTORMS EV3.
- **Software**: **clev3r** (a BASIC programming environment for the EV3, also called EV3 Basic). The version tested for this site is **v1.7.1.0**. Pick this if you already use clev3r.

### Install the software

1. Install clev3r (v1.7.1.0 recommended).
2. To open an example: open the `.bp` file in clev3r → "**Compile and download**" → run it from the folder starting with `MBC_` in the EV3 file browser.

### Make the software see the sensor

No extension needed. On the EV3 the sensor **emulates an original LEGO sensor** (most products are an "Infrared Sensor"), with exactly the same modes as the official EV3 software: **Proximity / Beacon / Remote**, with the same ranges.

- Read them with BASIC commands such as `IR_PROXIMITY` / `IR_BEACON` / `IR_REMOTE`. See the readings below.
- clev3r can also read modes that EV3-G and Classroom cannot, so examples marked "Pybricks / EV3 MicroPython / clev3r only" run here too.
- Some products come with MBC subroutines. They are already inside the example files.

> [!IMPORTANT]
> **Do not switch modes in the middle of a program.** Every time the EV3 switches a sensor mode it stalls (about 0.5 s). Pick one mode at the start and use it for the whole run.

### Wiring

1. Plug the sensor into EV3 **port {{port}}** with the cable (any of 1~4 works; use the same port in the program).
2. **Turn the EV3 on first, then plug in the sensor.**
3. Latch up, push the plug in gently.

### First test

1. Open the `.bp` of example 1 on this page and press "Compile and download".
2. Run it from the folder starting with `MBC_` in the EV3 file browser.
3. If the number on the EV3 screen changes with what is in front of the sensor, everything is connected.

### FAQ

- **Cannot find the program on the EV3?** Look in the EV3 file browser for the folder starting with `MBC_`.
- **The number never changes?** Unplug the sensor and plug it back in (the EV3 must be on first).
- **The program hiccups now and then?** It switches sensor modes. Use one mode only.
