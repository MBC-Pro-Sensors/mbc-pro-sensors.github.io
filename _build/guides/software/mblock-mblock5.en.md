### What you need

- **Sensor**: {{product}} ({{edition}}, different factory firmware from the other editions), plus an **mBot adapter** (to plug into an RJ25 port). SPIKE, EV3 and I2C editions cannot connect to a Makeblock board directly.
- **Board**: mBot (mCore), mBot Ranger (Auriga), Orion, Ultimate 2.0 (MegaPi), MegaPi Pro. **mBot2 (CyberPi) has no RJ25 port and is not supported.**
- **Software**: **mBlock 5 for PC** (Upload mode: the program is flashed to the board).
- **Extension**: MBC Pro Sensors **v2.0.0** (for protocol v2 firmware shipped from 2026-10).

### Install the software

1. Install **mBlock 5 for PC** from the [mBlock website](https://mblock.cc/pages/downloads).
2. Open mBlock 5 and add your board (for example mBot) under "**Devices**" at the bottom left.
3. Switch the mode toggle at the bottom right to **Upload**.

### Make the software see the sensor

Add one extension and the MBC blocks appear:

1. Get the v2.0.0 extension: once it is listed, search "**MBC**" in the mBlock 5 "**Extension Center**" and add it. Before it is listed (under review), advanced users can sign in to [ext.mblock.cc](https://ext.mblock.cc) with their own Makeblock account → My extensions → Import data, upload [mbc_pro_sensors_2.0.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext) (about 1 minute), then click "Download" to get a `.mext`.
2. **Drag the `.mext` file into the mBlock window.** Six new categories appear: Pathfinder, Ranger, Controller, IMU gyro, Vision and MBC common.
3. Start the program with the board's start block (for example "when mBot starts up") and click "**Upload**" to flash it to the board.
4. To open an example: "**File → Open from your computer**" and pick the `.mblock` file.

> [!IMPORTANT]
> The new extension (v2) only works with new firmware, and the legacy extension (v1.0.0) only with firmware from before 2026-10; their blocks are not compatible. Not sure which firmware you have? Drag in the v2 extension and read the "**MBC common → protocol version**" block: **2** means new firmware. The legacy extension is in the [Downloads](/en/downloads.md).

### Wiring

Sensor → **mBot adapter** → the board's RJ25 port (UART 38400). The blocks ask for a **port number**; the extension maps it to the right pins for your board.

| Board | Usable ports | Notes |
| :--- | :---: | :--- |
| mBot (mCore) | 1, 2, 3, 4 | all four ports |
| mBot Ranger (Auriga) | 6 ~ 10 | 1 ~ 4 are red motor ports, 5 is the serial port |
| Orion | 3, 4, 6 | 1, 2 are red motor ports; 7, 8 are analog only |
| Ultimate 2.0 (MegaPi) | 6, 7, 8 | needs the RJ25 shield; 5 is the serial port |
| MegaPi Pro | 7, 8, 9, 10 | needs the RJ25 shield |

The examples on this page use mBot **port {{port}}**; to use another port, change the port number on the blocks. On another board, replace "when mBot starts up" with that board's start block.

### First test

1. Start the program with "**wait 1 second**" so the sensor can boot; reading too early returns `-32768`.
2. Check the wiring with the "**MBC common → connection OK?**" block, then look at the values.
3. Upload example 1 on this page. If the number changes with the sensor, everything is connected.

### FAQ

- **"connection OK?" is false?** Wrong port, the sensor is not the MakeBlock edition, or the firmware is legacy (use the v1 extension with old firmware).
- **Reading `-32768`?** Nothing has been read since power-on: check the wiring; or this product has no such value.
- **The value never changes?** Make sure the adapter is firmly in and the sensor light is on; "protocol version" reading 2 means communication works.
- **Still stuck?** [Contact us on LINE](https://line.me/R/ti/p/@692vcvuk) with the board model, the port and the value of the "device ID" block.
