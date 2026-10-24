### What you need

- **Sensor**: {{product}} ({{edition}}, RJ12 plug).
- **Brick**: LEGO MINDSTORMS EV3.
- **Software** (pick one):
  - **Pybricks v4**: open [code.pybricks.com](https://code.pybricks.com) in Chrome; the EV3 needs the Pybricks firmware first (the stable release now includes EV3 firmware, **v4.1.1** as of 2026-10).
  - **EV3 MicroPython v2.0**: the official LEGO "EV3 MicroPython" microSD boot card + the "LEGO MINDSTORMS EV3 MicroPython" extension for VS Code. Pick this if you already use it.

### Install the software

**Pybricks v4**

1. Open [code.pybricks.com](https://code.pybricks.com) in Chrome.
2. Follow the website to switch the EV3 to Pybricks firmware. The stable code.pybricks.com now includes EV3 firmware (**v4.1.1** as of 2026-10). The examples on this site were first bench-tested on v4.0.0b10; v4.1.1 uses the same code.
3. To open an example: drag the `.py` file into the editor and press Run.

**EV3 MicroPython v2.0**

1. Boot the EV3 from the official LEGO EV3 MicroPython microSD card.
2. Install the "LEGO MINDSTORMS EV3 MicroPython" extension in VS Code.
3. To open an example: paste the program into `main.py` and press **F5** to download and run.

### Make the software see the sensor

The beginner examples **need no library**; they read the sensor data directly:

- Pybricks v4 uses `LUMPDevice(Port.S1)`; `sensor.read(mode)` returns that mode's values.
- EV3 MicroPython v2.0 uses `Ev3devSensor`.
- Both can read modes that EV3-G and Classroom cannot, so examples marked "Pybricks / EV3 MicroPython / clev3r only" run here too.

> [!IMPORTANT]
> **Do not switch modes often inside the main loop.** The EV3 hardware adds a communication wait every time a sensor mode changes. Pick one mode for the task and keep it for the whole run.

### Wiring

1. Plug the sensor into EV3 **port {{port}}** with the cable (any of 1~4 works; use the same port as `Port.S1`~`Port.S4` in the program).
2. **Turn the EV3 on first, then plug in the sensor.**
3. Latch up, push the plug in gently.

### First test

1. Open example 1 on this page and run it (F5 for the MicroPython version).
2. If the number on the EV3 screen changes with what is in front of the sensor, everything is connected.

### FAQ

- **Cannot find Pybricks firmware for the EV3?** Use the stable code.pybricks.com (EV3 v4.1.1 built in since 2026-10); the old ev3dev-based Pybricks (microSD card) is a different system and does not apply here.
- **The number never changes?** Unplug the sensor and plug it back in (the EV3 must be on first).
- **The program reacts unevenly?** The main loop reads different modes in turn and waits at every switch. Use one mode only.
- **Error when importing a library?** The library file must be imported into the same Pybricks project, and its file name must match the `import` name exactly.
