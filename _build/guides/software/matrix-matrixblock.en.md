### What you need

- **Sensor**: {{product}} ({{edition}}).
- **Controller**: MATRIX Mini R4.
- **Cable**: the stock MATRIX 4-pin sensor cable.
- **Software**: **MATRIXblock** (the stock MATRIX block software). This method **needs no extra library**.

### Install the software

1. Install MATRIXblock (default folder `C:\matrixblock-r4\`).
2. If you installed the "LauMo big-channel" library before, put the stock `MiniR4ColorSensorExt.cpp` / `.h` files back first, so the stock blocks read the way described below.

### Make the software see the sensor

The Mini R4 sees the sensor as a stock **colour sensor** (I2C address `0x22`); read it with the built-in blocks, no extension needed:

- The "**colour sensor → R / G / B / C / M / Y / K**" blocks each read one `0 ~ 255` number. The firmware packs the product data into these seven slots; see the readings below for what each slot means.
- MATRIXblock's Arduino mode works too: `MiniR4.I2C1.MXColor.getColor(R)`.

> [!WARNING]
> **Do not use the H / S / V or colour ID blocks**: the controller computes those from R, G, B itself, so they mean nothing for MBC products. MATRIX removed the grey-level and colour-number blocks, so only these seven slots are usable.

### Wiring

1. Connect the stock MATRIX 4-pin sensor cable to the Mini R4 **I2C1** or **I2C2** port.
2. Choose that port in the blocks (for example `I2C1`).
3. 5 V direct, no level shifter needed.

### First test

1. Show the number from the "colour sensor → R" block.
2. If the number changes with what is in front of the sensor, everything is connected.

### FAQ

- **The numbers make no sense?** Make sure you use the seven slots R / G / B / C / M / Y / K, not the H / S / V or colour ID blocks.
- **Need more data (mm, bitmaps, status)?** The seven stock slots hold only 7 numbers of 0~255; switch to the "big-channel library" to get 30 bytes at once.
- **Can I use the stock blocks and the big channel together?** No. They need **different library files**, so one computer uses one method at a time. The sensor firmware supports both, so switching methods **does not require reflashing the sensor**.
