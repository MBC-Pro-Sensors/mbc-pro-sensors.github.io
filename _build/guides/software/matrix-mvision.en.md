### What you need

- **Sensor**: {{product}} ({{edition}}).
- **Controller**: MATRIX Mini R4.
- **Software**: **MATRIXblock**, with the built-in **mVision** camera blocks. **No library needed.**

<!-- hide:hideMatrixBigChannel -->
MATRIXblock only has camera blocks for mVision (Serial1 115200) and HuskyLens, with no I2C version, so this product **does not use 0x22 or the big channel**: it sends data in the mVision packet format instead.
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
MATRIXblock only has camera blocks for mVision (Serial1 115200) and HuskyLens, with no I2C version, so this product **does not use 0x22**: it sends data in the mVision packet format instead.
<!-- /show -->

### Install the software

1. Install MATRIXblock (default folder `C:\matrixblock-r4\`).
<!-- hide:hideMatrixBigChannel -->
2. No library files to change; the stock 0x22 or big-channel library does not affect this method.
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
2. No library files to change.
<!-- /show -->

### Make the software see the sensor

To the Mini R4, the sensor is an mVision camera:

- The "**mVision data 0~9**" blocks hold one number each; the sensor **sends a packet every 20 ms**.
- See the readings below for what each slot means.

> [!WARNING]
> **Do not use data 10**: the MATRIX program only declares 10 slots (data 0~9); slot 10 reads some other memory.

![mVision packet format](/images/figures/common/packet-mvision.svg)

### Wiring

1. Connect the sensor to the Mini R4 serial port used by the mVision camera (Serial1, 115200).
2. Use the mVision blocks in the program, not the colour sensor blocks.

### First test

1. Show the number from the "mVision data 0" block.
2. Hold something coloured in front of the lens. If the number changes, everything is connected.

### FAQ

- **Strange numbers?** Make sure "data 10" is not used; only data 0~9 are meaningful.
- **Nothing from the colour sensor blocks?** On MATRIX this product uses the serial port, so only the mVision blocks work.
