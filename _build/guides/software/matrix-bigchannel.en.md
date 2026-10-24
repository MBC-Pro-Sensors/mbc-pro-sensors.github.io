### What you need

- **Sensor**: {{product}} ({{edition}}).
- **Controller**: MATRIX Mini R4.
- **Cable**: the stock MATRIX 4-pin sensor cable.
- **Software**: **MATRIXblock** (blocks or Arduino mode).
- **Library**: LauMo big-channel library v2, [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip) (`MiniR4ColorSensorExt.cpp`, `MiniR4ColorSensorExt.h`).

The big channel was designed by coach LauMo: after you replace the two stock files, the same colour sensor blocks switch to a "pointer method" and fetch one **30-byte** packet at a time (24 bytes of product data + status + 4 extension bytes + CRC), so every product value (including mm, bitmaps and status) can be read.

### Install the software

1. Install MATRIXblock (default folder `C:\matrixblock-r4\`).

### Make the software see the sensor

Replace the library (only once):

1. Download [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip). v2 changes the wait in `begin()` from 1 ms to **5 ms**, so ESP32 products have time to answer.
2. **Close MATRIXblock** and overwrite the two files in the install folder: `C:\matrixblock-r4\arduino\libraries\MatrixMiniR4\src\Modules\Sensors\`. **Back up the originals first**; put them back to return to the stock 0x22 method.
3. Reopen MATRIXblock. The blocks look the same; read them with the pointer method below.

Pointer method:

| Block | What it does |
| :--- | :--- |
| ColorID | pointer back to byte 0, returns byte 24 (status flags) |
| Blue | reads 1 byte (0~255) |
| Green | reads 1 signed byte (−128~127) |
| Saturation | reads 2 bytes (0~65535, little-endian) |
| Hue | reads 2 signed bytes (−32768~32767, little-endian) |
| Value | reads a 4-byte float |
| Red | moves the pointer forward by "the size of the previous read block", returns the new position |
| Cyan / Magenta / Yellow / Key | send command F1 / F2 / F3 / F4, return byte 25 / 26 / 27 / 28 of the previous packet |

Order: **ColorID → (Blue reads byte 0) → Red → (Blue reads byte 1) → Red → …**; after ColorID, read once before pressing Red. To jump to byte n: "ColorID → Blue → Red × n". See the readings below for what each byte holds.

![Big-channel 30-byte packet](/images/figures/common/packet-big30.svg)

### Wiring

1. Connect the stock MATRIX 4-pin sensor cable to the Mini R4 **I2C1** or **I2C2** port (I2C address `0x22`).
2. Choose that port in the blocks or code (for example `MiniR4.I2C1`).
3. 5 V direct, no level shifter needed.

### First test

1. Read byte 0 with "ColorID → Blue" and show the number.
2. If the number changes with what is in front of the sensor, the library is replaced and the sensor connected.

### FAQ

- **The numbers jump around and make no sense?** The library is still the stock one: check that both files were overwritten and that MATRIXblock was restarted.
- **Want the stock 0x22 method back?** Put the two backed-up stock files back. One computer uses one method at a time; the sensor **does not need reflashing**.
- **Getting old data?** The library drops a packet that is short or fails the CRC and keeps the previous one; it reads at most every 5 ms, so reading faster returns the same packet.
