<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🟢 MATRIX Mini R4

<!-- hide:hideMatrixBigChannel -->
MBC MATRIX-edition sensors plug into a Mini R4 **I2C port** (address `0x22`) and are read with the stock "colour sensor" blocks in MATRIXblock; the Sharpshooter uses the **serial port** and the stock "mVision" camera blocks. This page is the overview of the three methods; each product has its own page at the bottom.
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
MBC MATRIX-edition sensors plug into a Mini R4 **I2C port** (address `0x22`) and are read with the stock "colour sensor" blocks in MATRIXblock; the Sharpshooter uses the **serial port** and the stock "mVision" camera blocks. Neither needs any file to be installed or replaced. This page is the overview; each product has its own page at the bottom.
<!-- /show -->

<!-- hide:hideMatrixBigChannel -->
| Method | Products | What to install | What you get |
| :--- | :--- | :--- | :--- |
| **A. Stock blocks (0x22)** | Pathfinder 8 / 16, Ranger 2 / 8, IMU, Controller PS2 / PS4 | Nothing, built into MATRIXblock | R, G, B, C, M, Y, K: seven 0~255 numbers |
| **B. LauMo big channel** | same | Swap two library files ([download](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip)) | one 30-byte packet with every product value (mm, bitmaps, status) |
| **C. mVision (serial)** | Sharpshooter school / WRO edition | Nothing | camera blocks "mVision data 0~9" |

> [!IMPORTANT]
> A and B need **different library files**; one computer can use only one at a time. The firmware supports both, so switching methods never needs a sensor re-flash.
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
| Method | Products | What to install | What you get |
| :--- | :--- | :--- | :--- |
| **A. Stock blocks (0x22)** | Pathfinder 8 / 16, Ranger 2 / 8, IMU, Controller PS2 / PS4 | Nothing, built into MATRIXblock | R, G, B, C, M, Y, K: seven 0~255 numbers |
| **B. mVision (serial)** | Sharpshooter school / WRO edition | Nothing | camera blocks "mVision data 0~9" |
<!-- /show -->

---

## A. Stock blocks (0x22)

The "colour sensor → R / G / B / C / M / Y / K" blocks each return one 0~255 number; the firmware packs the product data into these seven slots.

> [!WARNING]
> **Do not use the H / S / V or colour-ID blocks**: the host computes them from R, G, B, which is meaningless for MBC products. The grayscale and colour-number blocks were removed by MATRIX, so only these seven slots are usable.

### Seven-slot map

| Block | Pathfinder 8 | Pathfinder 16 | Ranger 2 | Ranger 8 | IMU gyro | Controller PS2 / PS4 |
| :---: | :--- | :--- | :--- | :--- | :--- | :--- |
| **R** | line position 0~16 (minus 8) | line position 0~32 (minus 16) | left distance cm | sensor 1 cm (far left) | gyro 3D heading low byte | d-pad + shapes |
| **G** | line width 0~8 | line width 0~16 | right distance cm | sensor 2 (left 60°) | gyro 3D heading high byte | shoulders + functions |
| **B** | fine position 0~200 (minus 100) | fine position 0~200 (minus 100) | nearest cm | sensor 3 (left 30°) | fused heading low | left stick X 0~99 |
| **C** | channels 1, 2 reflect | channels 1~4 reflect | 0 | **min(sensor 4, sensor 5) (straight ahead)** | fused heading high | left stick Y 0~99 |
| **M** | channels 3, 4 | channels 5~8 | 0 | sensor 6 (right 30°) | gyro 1D heading low | right stick X 0~99 |
| **Y** | channels 5, 6 | channels 9~12 | 0 | sensor 7 (right 60°) | gyro 1D heading high | right stick Y 0~99 |
| **K** | channels 7, 8 | channels 13~16 | 0 | sensor 8 (far right) | 0 | connected (1 / 0) |

### Decoding

| Product | Block | Decode |
| :--- | :--- | :--- |
| Pathfinder 8 | R / B | line position = R − 8 (negative = line on the left, viewed from behind the robot); fine position = B − 100 |
| | C~K | two channels per slot: tens digit = first channel, units digit = second, each 1~9 (1 darkest, 9 brightest) |
| Pathfinder 16 | R / B | line position = R − 16; fine position = B − 100 |
| | C~K | four channels per slot, 2 bits each (0 darkest ~ 3 brightest): channel 1 = ÷64 rounded down, channel 2 = (÷16 rounded down) mod 4, channel 3 = (÷4 rounded down) mod 4, channel 4 = mod 4 |
| Ranger 2 | R~B | 0~120 cm, 120 = nothing on that side |
| Ranger 8 | R~K | use directly, 0~120 cm (1 cm steps), 120 = nothing in that direction; ordered left to right, C = straight ahead (nearer of sensors 4 and 5 so a narrow object is not averaged away). Find the nearest distance / direction with compare blocks |
| IMU | R~Y | angle = high byte × 256 + low byte − 32768 (keeps counting past 360, left turn negative) |
| Controller | R | up 1, right 2, down 4, left 8, ▲ 16, ● 32, ✕ 64, ■ 128 (add when pressed together) |
| | G | L2 1, R2 2, L1 4, R1 8, SELECT 16, L3 32, R3 64, START 128 |
| | B~Y | 0~99, 50 = centre |

```cpp
// MATRIXblock Arduino mode: stock library, read slot R
int r = MiniR4.I2C1.MXColor.getColor(R);   // Pathfinder 8: line position + 8
int linePos = r - 8;
```

---

<!-- hide:hideMatrixBigChannel -->
## B. LauMo big channel

The big channel was designed by coach LauMo: replace the two stock `MiniR4ColorSensorExt` files and the same colour-sensor blocks become *pointer reads* that fetch one **30-byte packet** (24 bytes of product data + status + 4 extension bytes + CRC). The Pathfinder 16 and the IMU (R4 big-channel firmware) have shipped with this format for a while; every other product supports it from 2026-10.

### Install the library

1. Download [MiniR4ColorSensorExt_LauMo_v2.zip](/downloads/MiniR4ColorSensorExt_LauMo_v2.zip) (`MiniR4ColorSensorExt.cpp`, `MiniR4ColorSensorExt.h`; v2 raises the `begin()` wait from 1 ms to **5 ms** so ESP32 products can answer in time).
2. Close MATRIXblock and overwrite the two files in the install folder (default `C:\matrixblock-r4\arduino\libraries\MatrixMiniR4\src\Modules\Sensors\`). Back up the originals first; put them back to return to the stock 0x22 method.
3. Reopen MATRIXblock. The blocks look the same; read them with the pointer method below.

### Pointer reads

| Block | Action |
| :--- | :--- |
| ColorID | reset the pointer to byte 0, return byte 24 (status flags) |
| Blue | read 1 byte (0~255) |
| Green | read 1 signed byte (−128~127) |
| Saturation | read 2 bytes (0~65535, little-endian) |
| Hue | read 2 signed bytes (−32768~32767, little-endian) |
| Value | read a 4-byte float |
| Red | advance the pointer by the size of the last read, return the new position |
| Cyan / Magenta / Yellow / Key | send command F1 / F2 / F3 / F4, return byte 25 / 26 / 27 / 28 of the previous packet |

Sequence: **ColorID → (Blue reads byte 0) → Red → (Blue reads byte 1) → Red → …** — after ColorID read once before pressing Red. To jump to byte n: "ColorID → Blue → Red × n".

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

Library rules (the firmware follows them, your program does not need to): write `0x21`, wait 300 µs, read 30 bytes; byte 29 is CRC-8 (polynomial 0x07 over bytes 0~28) and a short or bad packet is dropped, keeping the previous data; read at most every 5 ms; `begin()` sends command 1 and expects `0x43`.

### Byte tables per product (bytes 0~23)

**Pathfinder 8** (new in 2026-10)

| byte | Block | Content |
| :---: | :--- | :--- |
| 0 | Blue | line position + 8 (0~16) |
| 1 | Blue | line width 0~8 |
| 2 | Blue | fine position + 100 (0~200) |
| 3~10 | Blue | channels 1~8 reflect 0~100 |
| 11 | Blue | line bitmap (bit0 = channel 1) |
| 12 | Blue | line segments |

**Pathfinder 16** (already shipped, bytes 0~18 unchanged)

| byte | Block | Content |
| :---: | :--- | :--- |
| 0 | Blue | line position + 16 (0~32) |
| 1 | Blue | line width 0~16 |
| 2 | Blue | fine position + 100 (0~200) |
| 3~18 | Blue | channels 1~16 reflect 0~100 |
| 19~20 | Saturation | line bitmap, 16 bits (new) |
| 21 | Blue | line segments (new) |

**Ranger 2**

| byte | Block | Content |
| :---: | :--- | :--- |
| 0 | Blue | left distance cm |
| 1 | Blue | right distance cm |
| 2 | Blue | nearest distance cm |
| 3 | Blue | init bitmap (bit0 left, bit1 right) |
| 4 | Blue | fault bitmap |
| 5~6 | Saturation | left distance mm 0~1200 (new) |
| 7~8 | Saturation | right distance mm (new) |

**Ranger 8**

| byte | Block | Content |
| :---: | :--- | :--- |
| 0~7 | Blue | sensors 1~8 distance cm (sensor 1 = far left) |
| 8 | Blue | nearest direction 0~16 (8 = straight ahead) |
| 9 | Blue | nearest distance cm |
| 10 | Blue | most open direction 0~16 |
| 11 | Blue | most open distance cm |
| 12 | Blue | init bitmap (bit0 = sensor 1) |
| 13 | Blue | fault bitmap |
| 14~15 | Saturation | nearest distance mm (new) |
| 16~17 | Saturation | straight-ahead (average of sensors 4, 5) mm (new) |

**IMU gyro** (already shipped, bytes 0~7 unchanged)

| byte | Block | Content |
| :---: | :--- | :--- |
| 0~1 | Hue | gyro 3D heading (°) |
| 2~3 | Hue | fused heading |
| 4~5 | Hue | gyro 1D heading |
| 6~7 | Hue | magnetic heading |
| 8~9 | Hue | turn rate (0.1 °/s) (new) |
| 10~11 | Hue | pitch angle (°) (new) |
| 12~13 | Hue | pitch rate (0.1 °/s) (new) |

**Controller PS2 / PS4** (new in 2026-10)

| byte | Block | Content |
| :---: | :--- | :--- |
| 0 | Blue | d-pad 0~15 (up 1, right 2, down 4, left 8) |
| 1 | Blue | shape buttons 0~15 (▲1, ●2, ✕4, ■8) |
| 2 | Blue | shoulder buttons 0~15 (L2 1, R2 2, L1 4, R1 8) |
| 3 | Blue | function buttons 0~15 (SELECT 1, L3 2, R3 4, START 8) |
| 4~7 | Blue | LX, LY, RX, RY (0~99, 50 = centre) |
| 8~9 | Saturation | all 16 buttons (PSX layout) |
| 10 | Blue | connected 1 / 0 |
| 11 | Blue | battery 0~100 (PS4; PS2 = 0) |

### Tail bytes: ColorID / Cyan / Magenta / Yellow / Key

| Block | Pathfinder 16 | IMU | Pathfinder 8, Ranger, Controller |
| :--- | :--- | :--- | :--- |
| ColorID returns | 0 (kept for shipped units) | 0 (kept for shipped units) | status flags, byte 24 (= I2C v2 F3, bit0 = ready) |
| Cyan | calibrate black | zero heading | Pathfinder 8: calibrate black; others: no action |
| Magenta | calibrate white | zero heading | Pathfinder 8: calibrate white; others: no action |
| Yellow | 0 | zero heading | device id (Pathfinder 0x16, Ranger 0x29, Controller 0x42) |
| Key | 0 | zero heading | Pathfinder channels 8 / Ranger sensors 2 or 8 / Controller 1 = PS2, 2 = PS4 |

On the Pathfinder 8, bytes 25 / 26 also count completed Cyan / Magenta actions.

### Arduino (MATRIXblock Arduino mode)

```cpp
// Big channel: read byte n (uint8) / 2 bytes from n (uint16 little-endian)
uint8_t bigU8(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();                 // pointer back to byte 0
  uint8_t v = MiniR4.I2C1.MXColor.getColor(B);      // read byte 0
  for (uint8_t i = 0; i < n; i++) { MiniR4.I2C1.MXColor.getColor(R); v = MiniR4.I2C1.MXColor.getColor(B); }
  return v;
}
uint16_t bigU16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (uint16_t)MiniR4.I2C1.MXColor.getS();      // Saturation = read 2 bytes
}
int16_t bigI16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (int16_t)MiniR4.I2C1.MXColor.getH();       // Hue = read 2 signed bytes
}

int leftMm = bigU16(5);     // Ranger 2: left distance mm (bytes 5~6)
int heading = bigI16(2);    // IMU: fused heading (bytes 2~3)
```

> [!NOTE]
> The PS4 receiver is an ESP32 whose I2C slave needs ≥ 2 ms to answer; the firmware pre-loads the latest packet, and the 5 ms wait in the v2 `begin()` exists for it. **Bench test pending.**

---
<!-- /hide -->

<!-- hide:hideMatrixBigChannel -->
## C. Sharpshooter: mVision camera blocks (serial)
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
## B. Sharpshooter: mVision camera blocks (serial)
<!-- /show -->

<!-- hide:hideMatrixBigChannel -->
MATRIXblock's camera blocks are mVision (Serial1 115200) and HuskyLens only — there is no I2C camera block — so the Sharpshooter **does not use 0x22 or the big channel**. It sends data in the mVision packet format: the "mVision data 0~9" blocks each hold one number, **one packet every 20 ms**.
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
MATRIXblock's camera blocks are mVision (Serial1 115200) and HuskyLens only — there is no I2C camera block — so the Sharpshooter **does not use 0x22**. It sends data in the mVision packet format: the "mVision data 0~9" blocks each hold one number, **one packet every 20 ms**.
<!-- /show -->

> [!WARNING]
> **Do not use data 10**: the MATRIX program declares only 10 slots (data 0~9); slot 10 reads unrelated memory.

| mVision data | School edition (from 2026-10) | WRO edition (HUE12, unchanged) |
| :---: | :--- | :--- |
| 0 | hue 0~100 (**255 = black / white / grey**) | keeps the existing HUE12 packing (12 spot colour codes + vividness); decode as in the `HUE12_AS_mVisionSensor_v1.0.8.mbr4` example |
| 1 | saturation 0~100 | |
| 2 | brightness 0~100 | |
| 3 | motion 0~100 | |
| 4 | blob X 0~100 (left → right) | |
| 5 | blob Y 0~75 (bottom → top) | |
| 6 | blob width W 0~100 (0 = not found) | |
| 7 | blob height H 0~75 | |
| 8 | blob area W × H | |
| 9 | blob found 1 / 0 | |

Ranges match the EV3 / SPIKE editions, so program logic carries over. The HUE12 example used by WRO 2026 teams is unaffected.

<figure class="ref-fig"><img src="/images/figures/common/packet-mvision.svg" alt="mVision packet format" loading="lazy"></figure>

---

## 📖 Per-product MATRIX pages

<!-- hide:hideMatrixBigChannel -->
One page per product: wiring figure, the 0x22 seven-slot map, the big-channel byte table (linked to each value's command reference) and the Arduino form.
<!-- /hide -->
<!-- show:hideMatrixBigChannel -->
One page per product: wiring figure, the 0x22 seven-slot map (linked to each value's command reference) and the Arduino form.
<!-- /show -->

| Product | MATRIX page |
| :--- | :--- |
| Pathfinder 8-Way | [line8 × MATRIX](/en/sensors/line8/matrix.md) |
| Pathfinder 16-Way | [line16 × MATRIX](/en/sensors/line16/matrix.md) |
| Ranger 2-Way | [tof2 × MATRIX](/en/sensors/tof2/matrix.md) |
| Ranger 8-Way | [tof8 × MATRIX](/en/sensors/tof8/matrix.md) |
| Controller PS2 | [ps2 × MATRIX](/en/sensors/ps2/matrix.md) |
| Controller PS4 / PS5 | [ps4 × MATRIX](/en/sensors/ps4/matrix.md) |
| IMU gyro | [imu × MATRIX](/en/sensors/imu/matrix.md) |
| Sharpshooter ESP32CAM | [esp32cam × MATRIX (mVision)](/en/sensors/esp32cam/matrix.md) |

The EXP6 and Servo6 have no MATRIX edition. Library downloads and files for the other platforms: [Downloads](/en/downloads.md).
