### What you need

- **Sensor**: {{product}} ({{edition}}).
- **Hub**: SPIKE Prime or Robot Inventor.
- **Computer**: Chrome. Open [code.pybricks.com](https://code.pybricks.com) and start writing; nothing to install.
- **Hub firmware**: must be switched to **Pybricks firmware v4.1** first (the stable code.pybricks.com is v4.1.1; v4.1.0 from the MBC SPIKE Blocks website works too). Every example on this site is written for v4.1.
- **Library**: `{{lib}}` (only needed for the "import the library" method). For a new program of your own, import just `MBC_sensors.py`: one file for every MBC sensor.

> [!NOTE]
> The Pybricks **3.6.1** libraries and examples are **no longer updated** (the 2026 SPIKE Prime hubs cannot run 3.6.1 either). For hubs that have not been upgraded, the old files are under "Legacy (no longer updated)" on the [Downloads page](/en/downloads.md).

### Install the software

1. Open [code.pybricks.com](https://code.pybricks.com) in Chrome.
2. Switch the hub to Pybricks firmware. The easiest way is the [MBC SPIKE Blocks website](https://mbc-spike-blocks.mbcrobot.workers.dev): top menu "**SPIKE Hub → Install/Update Pybricks firmware**", about **3 minutes**.
3. To go back to the official LEGO App later, switch back to the original LEGO firmware with the SPIKE App.

### Make the software see the sensor

Pybricks can read the sensor two ways; pick one:

1. **Built-in commands (no library)**: read the sensor with Pybricks' own sensor commands. No extra file, quickest to start.
2. **Import the library**: download [{{lib}}]({{libUrl}}) to get more data, such as the high-resolution position and every channel.

To import the library:

1. In code.pybricks.com, click "**Import a file**" in the file panel on the left.
2. Import **both** the example file and `{{lib}}`, so they are in the same project list.
3. Open the example and press **▶** to run.

> [!IMPORTANT]
> **New projects: use `MBC_sensors.py`. Old projects: just replace the one file they already use.** `MBC_sensors.py` is one file for every MBC sensor (the EXP6 and Servo6 expander libraries are separate downloads); write `from MBC_sensors import MBC_LINE8`, with the same class names as in the old files. The old file names (such as `{{lib}}` used by the examples on this page) are kept for existing programs: swap in the new file with the same name and the program needs no changes. But every old-name file is the whole shared library, so **importing 3 or more of them in one program goes over the SPIKE program slot limit of about 51 KB**; to mix several sensors, import only `MBC_sensors.py`.

> [!TIP]
> **The file name must match the name in the program's `import` exactly**, or you will get an `ImportError`. The library files on this site have no version suffix, so you do not need to rename them.

> [!TIP]
> Most libraries take a `multitask` argument when you create the object: `False` is normal mode, one line after another; `True` is multitask mode, where every call needs `await` in front so the robot can do several things at once.

### Wiring

1. Plug the sensor into hub **port {{port}}** with the cable (any of A~F works).
2. Use the same port in the program: built-in commands use `Port.A`~`Port.F`; a library's `port` argument is usually `1`~`6` for A~F.
3. Latch up, push the plug in gently.

### First test

1. Turn on the hub and connect it in code.pybricks.com.
2. Import and open example 1 on this page, then press **▶**.
3. If the numbers printed in the console change with what is in front of the sensor, everything is connected.

### FAQ

- **`ImportError`?** The library was not imported into the same project, or its file name differs from the `import` name. Rename it to `{{lib}}` and import it again.
- **The example does not run, or a command is unknown?** The hub firmware is too old: the examples on this site are written for Pybricks v4.1, so update the hub to v4.1 first. 3.6.1 is no longer updated; the old files are on the [Downloads page](/en/downloads.md).
- **What happens if the sensor cable comes out?** The program keeps running: the library returns **the last value it read** and prints one notice in the console (`MBC: Port C 讀不到感應器，先回傳上一次的值`, "cannot read the sensor, returning the last value"); if the sensor was never connected it returns `None`. To check in your program, call the object's `sensor_ok()`: `True` if the last read worked, `False` if not (add `await` in multitask mode). Once you plug it back in the library keeps reading; if the sensor was missing from the start and you use multitask mode, run the program again.
- **An example marked "Pybricks only" shows strange numbers?** It uses a new mode in sensor firmware from 2026-10 on; older firmware returns different numbers.
- **The official App cannot connect any more?** The hub now runs Pybricks firmware: switch back to the original LEGO firmware with the SPIKE App.
