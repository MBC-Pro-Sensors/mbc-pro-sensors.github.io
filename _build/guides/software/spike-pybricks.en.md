### What you need

- **Sensor**: {{product}} ({{edition}}).
- **Hub**: SPIKE Prime or Robot Inventor.
- **Computer**: Chrome. Open [code.pybricks.com](https://code.pybricks.com) and start writing; nothing to install.
- **Hub firmware**: must be switched to **Pybricks firmware** first. The examples on this site come in **3.6.1** and **4.0.0** versions; pick the one that matches your hub firmware.
- **Library**: `{{lib}}` (only needed for the "import the library" method).

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
> **The file name must match the name in the program's `import` exactly**, or the library will not load. If the downloaded file name has a version suffix (such as `_v400` or `_v361`), rename it back to `{{lib}}`, or you will get an `ImportError`.

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
- **The example does not run, or a command is unknown?** The example version does not match the hub firmware: use the 3.6.1 example on 3.6.1 firmware and the 4.0.0 example on 4.0.0 firmware.
- **An example marked "Pybricks only" shows strange numbers?** It uses a new mode in sensor firmware from 2026-10 on; older firmware returns different numbers.
- **The official App cannot connect any more?** The hub now runs Pybricks firmware: switch back to the original LEGO firmware with the SPIKE App.
