<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Reading it in mBlock 5

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passes desktop tests; real-world numbers (fps, line thresholds) may change after hardware testing. The mBlock edition has only been tested on a PC (replies for every value id, E0/E1, time-outs) and **has not been tried on an mBot yet**.

The **mBlock edition firmware** of the Vision Universal v1 lets mBot, mBot Ranger, Orion and MegaPi read the camera with mBlock 5 blocks: the MBC Pro Sensors extension **v2.1.0** adds a "Vision Universal" category (22 blocks). The value table is exactly the one of the [Arduino I2C edition](/en/sensors/unicam/arduino/index.md), carried over the RJ25 serial link instead; the blocks first ask the camera which mode it is in, then read the right value.

## Wiring

The camera must run the **mBlock edition firmware** (a different .bin from the SPIKE / EV3 / MATRIX / I2C editions; ask us for a camera flashed with it).

| Host | How to connect | Usable ports |
| :--- | :--- | :--- |
| mBot (mCore) | camera → **mBot socket board** → RJ25 port | 1-4 |
| mBot Ranger (Auriga) | same | 6-10 (1-4 are red motor ports, 5 is the hardware serial port) |
| Orion | same | 3, 4, 6 (1, 2 are ≈12 V motor ports; 7, 8 have analog-only TX pins) |
| Ultimate 2.0 (MegaPi) | RJ25 adapter board | 6-8 |
| MegaPi Pro | RJ25 adapter board | 7-10 |
| Plain Arduino boards (UNO, Mega…) | camera TX (IO1) → board RX, camera RX (IO3) ← board TX, 5V, GND (see "Without mBlock" below) | software serial on UNO, Serial1 on Mega |

- Serial **38400 bps, 8N1**; camera RX = IO3, TX = IO1, **crossed** to the host (the socket board already does this: S1 = host TX, S2 = host RX).
- The mBot2 (CyberPi) has no RJ25 port and is not supported.

> [!CAUTION]
> **Wiring a 5 V Arduino directly (no socket board)**: the UNO / Mega TX is 5 V and the ESP32 pins only take 3.3 V, so put a divider (e.g. 1 kΩ + 2 kΩ) between board TX and camera RX; camera TX → board RX can be wired directly.

## Install the extension

1. **New extension in development**: MBC Pro Sensors extension v2.1.0 is still being developed; **before it is officially listed you can download the import file to try it (not yet hardware-verified)**. Once listed it will be in the mBlock 5 Extension Center (search "MBC") and this page will be updated.
2. **Try it with the import file**: download [MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext) → sign in to [ext.mblock.cc](https://ext.mblock.cc) with your own Makeblock account → My extensions → Import data → upload (about 2 minutes; stay on the page until it reports success) → click "Download" to get a `.mext` you can drop into mBlock.
3. Open **mBlock 5 desktop**, add your board under "Devices" (bottom left, e.g. mBot), **drop the `.mext` into the mBlock window**; a "Vision Universal" category appears (next to the other MBC categories).
4. Switch the mode toggle (bottom right) to **Upload**: the extension only works in Upload mode (the program runs on the board).

The other categories of v2.1.0 (Pathfinder, Ranger, Controller, IMU, Vision school / WRO) are unchanged from v2.0.0; see [Makeblock extension](/en/makeblock.md).

## Set the camera mode first

The same value id means different things in different camera modes (`0x10` is colour 1 x in colour tracking but near offset in line following), so start the program with "set mode", or pick the mode in the camera menu (MODE). Every reporter first reads the **mode number (value `0x01`)** (reusing the last one for 100 ms) and returns **−32768** in the wrong mode.

| "set mode" option | Mode number (0x01) | Command sent |
| :--- | :---: | :--- |
| colour tracking 1 max colours | 11 | `E0 [1, 1]` |
| colour tracking 2 multi | 12 | `E0 [1, 2]` |
| colour tracking 3 standard | 13 | `E0 [1, 3]` |
| colour tracking 4 fine | 14 | `E0 [1, 4]` |
| colour pick 1 hue | 21 | `E0 [2, 1]` |
| colour pick 2 hue + saturation | 22 | `E0 [2, 2]` |
| colour pick 3 hue + sat + brightness | 23 | `E0 [2, 3]` |
| colour order | 30 | `E0 [3, 0]` |
| line follow 1 black line | 41 | `E0 [4, 1]` |
| line follow 2 white line | 42 | `E0 [4, 2]` |
| line follow 3 colour line | 43 | `E0 [4, 3]` |

Target colours **can only be taught on the camera** (stop on target 1-16 and press OK); programs cannot change them. Colour-pick sample points can be moved with the "set sample point" block (stored in the camera).

## Blocks

Pick the port the sensor is actually plugged into. **Colour c** = target c on the camera (from 1, the same number as on the camera screen); **k** = values per colour (per point) in that sub-option: colour tracking 1 → 2, 2 → 3, 3 → 4, 4 → 6; colour pick 1 → 1, 2 → 2, 3 → 3.

| Block | Returns | Mode with values | Value id | Meaning |
| :--- | :--- | :--- | :---: | :--- |
| Vision Universal port [1] set mode [mode ▾] | — | all | `0xE0` | switches camera mode and sub-option (E0 [mode, sub], stored in the camera); waits 50 ms afterwards |
| Vision Universal port [1] set sample point (number) x (number) y (number) | — | all | `0xE1` | moves a colour-pick sample point (E1 [point, X, Y] in pixels, stored in the camera) |
| Vision Universal port [1] mode number | 11~43 | all | `0x01` | camera mode × 10 + sub-option (e.g. 13 = colour tracking 3 standard, 30 = colour order, 41 = black line) |
| Vision Universal port [1] colours taught | 0~16 | all | `0x03` | how many target colours have been taught |
| Vision Universal port [1] colour [1~16] x | 0~159 | colour tracking (11~14) | `0x10 + (c−1)×k + slot` | blob centre X in pixels (left → right); 0 = not found |
| Vision Universal port [1] colour [1~16] y | 0~119 | colour tracking (11~14) | `0x10 + (c−1)×k + slot` | blob centre Y in pixels (top → bottom); 0 = not found |
| Vision Universal port [1] colour [1~16] width | 0~160 | colour tracking 4 (14) | `0x10 + (c−1)×k + slot` | blob width in pixels, 4 fine only |
| Vision Universal port [1] colour [1~16] height | 0~120 | colour tracking 4 (14) | `0x10 + (c−1)×k + slot` | blob height in pixels, 4 fine only |
| Vision Universal port [1] colour [1~16] count | 0~15 | colour tracking 2/3/4 (12~14) | `0x10 + (c−1)×k + slot` | how many blobs of this colour (above the minimum area); 2, 3, 4 |
| Vision Universal port [1] colour [1~16] area % | 0~100 | colour tracking 3/4 (13~14) | `0x10 + (c−1)×k + slot` | blob share of the picture in % (found but under 1% = 1); 3, 4 |
| Vision Universal port [1] colours found | 0~16 | colour tracking (11~14) | `0x10 + (c−1)×k + slot` | how many of the 16 target colours are in view |
| Vision Universal port [1] colour [1~16] found? | true/false | colour tracking (11~14) | `0x10 + (c−1)×k + slot` | X of colour n is above 0 |
| Vision Universal port [1] sample point [1~16] hue | 0~255 | colour pick (21~23) | `0x10 + (i−1)×k + slot` | hue code: 0~240 hue (red 0, yellow 40, green 80, cyan 120, blue 160, purple 200), 241~248 white→grey, 249~255 black |
| Vision Universal port [1] sample point [1~16] saturation | 0~100 | colour pick 2/3 (22~23) | `0x10 + (i−1)×k + slot` | saturation (3×3 average); 2, 3 |
| Vision Universal port [1] sample point [1~16] brightness | 0~100 | colour pick 3 (23) | `0x10 + (i−1)×k + slot` | brightness (3×3 average); 3 only |
| Vision Universal port [1] order block [1~16] hue | 0~255 | colour order (30) | `0x10~0x1F` | hue code of block n from the left (the hue taught for that colour) |
| Vision Universal port [1] order total blocks | 0~18 | colour order (30) | `0x20` | number of blocks in view |
| Vision Universal port [1] line [line value ▾] | number | line follow (41~43) | `0x10~0x1D` | one of the 14 line values (values 1~14 = ids 0x10~0x1D); near / middle / far offset −100~100 (line on the left = negative, right = positive), line angle −90~90 |
| Vision Universal port [1] line seen? | true/false | line follow (41~43) | `0x15` | line flags (near 1 + middle 2 + far 4) is not 0 |
| Vision Universal port [1] value (number) | number | all | `0x10~0x7F` | "value N" of the spec table = id 0x10 + N − 1 (N 1~112); accepts a variable |
| Vision Universal port [1] read value id (number) | number | all | `0x00~0xFF` | reads one value id directly (decimal, e.g. 16 = 0x10, 240 = 0xF0) |
| Vision Universal port [1] sensor OK? | true/false | all | `0xF0` | replies and the device id is the Vision Universal (F0 = 0x034C) |

- **−32768** means: no reply (time-out, checksum error, unplugged) or the current mode / sub-option has no such value (e.g. 1 max colours has no width / height). Booleans are false then.
- Slot order inside one colour: 1 X Y; 2 X Y count; 3 X Y area count; 4 X Y width height count area (the order of [value reference: I2C](/en/sensors/unicam/reference.md#host-i2c)).
- Out-of-range arguments are clamped (colour 0 → 1, colour 99 → 16).

**The 14 options of "line [line value ▾]"**

| Option | Value id | Range | Note |
| :--- | :---: | :--- | :--- |
| near offset | `0x10` | −100~100 | line on the left = negative, right = positive |
| middle offset | `0x11` | −100~100 |  |
| far offset | `0x12` | −100~100 |  |
| line angle | `0x13` | −90~90 |  |
| near line width % | `0x14` | 0~100 |  |
| line flags | `0x15` | 0~7 | near 1 + middle 2 + far 4 |
| junction type | `0x16` | 0~5 | 0 none 1 cross 2 T 3 left 4 right 5 dead end |
| junction distance % | `0x17` | 0~100 |  |
| near line left edge | `0x18` | −100~100 |  |
| near line right edge | `0x19` | −100~100 |  |
| far line width % | `0x1A` | 0~100 |  |
| marker colour | `0x1B` | 0~8 | colour of the marker line just crossed |
| marker count | `0x1C` | 0~99 | marker lines counted so far |
| last lost side | `0x1D` | 0~2 | 0 line seen 1 left 2 right |

## How it talks

- Read: the host sends **1 byte [id]** → the camera replies **[low][high][id ^ low ^ high]** (int16 little-endian); unknown ids reply `[00][80][id^80]` (−32768). The library waits 10 ms.
- Write: **E0 [camera mode, sub-option]**, **E1 [point, X, Y]**; the camera ACKs `[00][00][id]` only after all parameters arrived, then runs it; an incomplete command is dropped after 20 ms. EF ACKs and reboots; there is no F8 (re-addressing only matters for I2C).
- The **FD** pack-all reply is 235 bytes (116 values), more than the Arduino UNO 64-byte serial buffer, so the extension **has no "update all values" block**: each block asks only for the value it needs (about 1 ms per value).
- Shared values: F0 device id = `0x034C` (what "sensor OK?" checks), F1 protocol 2, F2 firmware 100, F3 status flags.

## Smallest program

Switch to colour tracking 3 standard; target 1 in view → green LED (brighter further right), otherwise dim red:

<pre class="blocks">when mBot starts up :: events hat
wait (2) seconds
Vision Universal port [1 v] set mode [colour tracking 3 standard v] :: #E8590C
forever
  if &lt;Vision Universal port [1 v] colour [1 v] found? :: #E8590C&gt; then
    LED [all v] colour red (0) green (Vision Universal port [1 v] colour [1 v] x :: #E8590C) blue (0) :: looks
  else
    LED [all v] colour red (40) green (0) blue (0) :: looks
  end
end</pre>

<details>
<summary>🔧 Equivalent Arduino sketch (mBot)</summary>

```cpp
// Vision Universal × mBot: smallest reading sketch (does what the blocks above do, Arduino IDE version)
// Needs the Makeblock library + MBC_Sensors v2.1.0 (the four files in src/ of the extension .mext, in Documents/Arduino/libraries/MBC_Sensors/)
#include <MeMCore.h>
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>

MBCPort cam(1);                 // camera on mBot port 1
MeRGBLed rgbled(7, 2);

void setup() {
  cam.begin();
  delay(2000);                  // let the camera boot
  ucSetMode(cam, 13);           // colour tracking 3 standard (E0 [1, 3])
  while (1) {
    if (ucFound(cam, 1)) {      // colour 1 found?
      int x = ucBlob(cam, 1, 0);          // colour 1 x (0~159)
      rgbled.setColor(0, 0, x, 0);        // brighter green the further right
    } else {
      rgbled.setColor(0, 40, 0, 0);       // not found: dim red
    }
    rgbled.show();
  }
}

void loop() {}
```

</details>

More examples (hear the colour, follow a blob, stop when close, follow a black line): [mBlock beginner examples](/en/sensors/unicam/mblock-examples.md).

## Without mBlock: a plain Arduino board

The `MBC_Sensors` library inside the extension also works in the Arduino IDE: unzip the `.mext`, put the four files of `src/` (MBC_Sensors.h/.cpp, mbc_proto.h, mbc_ids.h) into `Documents/Arduino/libraries/MBC_Sensors/`, and give `MBCPort` any serial port with `begin(serial)`. The functions match the blocks one to one (`ucSetMode`, `ucBlob`, `ucFound`, `ucSpot`, `ucOrderHue`, `ucLine`, `ucValue`, `ucOk`…). For Arduino without mBlock, the [I2C edition](/en/sensors/unicam/arduino/index.md) is faster (16 values per burst read).

```cpp
// Vision Universal (mBlock firmware) on a plain Arduino serial port (no mBot): prints line near offset and angle
// Needs MBC_Sensors v2.1.0 (as above; no Makeblock library). UNO uses software serial D10/D11; Mega uses Serial1 (D19 RX, D18 TX)
#include <SoftwareSerial.h>
#include <MBC_Sensors.h>

MBCPort cam;                    // no port number: begin(serial) below
#if defined(__AVR_ATmega2560__)
#define CAM_SERIAL Serial1
#else
SoftwareSerial camSerial(10, 11);   // RX = D10 (to camera TX = IO1), TX = D11 (to camera RX = IO3, through a divider)
#define CAM_SERIAL camSerial
#endif

void setup() {
  Serial.begin(115200);
  CAM_SERIAL.begin(38400);
  cam.begin(CAM_SERIAL);
  delay(2000);
  Serial.println(ucOk(cam) ? "Vision Universal OK" : "No reply: check wiring and that the firmware is the mBlock edition");
  ucSetMode(cam, 41);           // line follow 1 black line
}

void loop() {
  Serial.print("near offset ");
  Serial.print(ucLine(cam, 0));
  Serial.print("  angle ");
  Serial.print(ucLine(cam, 3));
  Serial.print("  junction ");
  Serial.println(ucLine(cam, 6));
  delay(100);
}
```

## FAQ

- **Values stay at −32768?** → Check "sensor OK?" first: false = wrong port, socket board loose, or the camera is not running the mBlock firmware; true = wrong mode, read "mode number" to see which mode the camera is in (colour tracking 3 = 13, black line = 41).
- **No "Vision Universal" category?** → You loaded an older extension (before v2.1.0); install v2.1.0 as above.
- **I changed the mode in the camera menu; how soon do the blocks follow?** → Within 0.1 s (the mode-number cache time).
- **Always "not found"?** → The target colour is not taught on the camera, or the lighting changed a lot; check that the camera screen boxes the blob.
- **Can I use Live mode (no upload)?** → No, the extension only supports Upload mode.

## More

- [Sharpshooter Vision Universal v1 product page](/en/sensors/unicam/index.md)
- [Value reference: I2C (the same table as mBlock)](/en/sensors/unicam/reference.md#host-i2c)
- [mBlock beginner examples](/en/sensors/unicam/mblock-examples.md)
- [MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext)
