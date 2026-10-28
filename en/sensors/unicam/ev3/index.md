<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Reading it on EV3

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

On the EV3 the Sharpshooter Vision Universal v1 (EV3 edition) shows up as a **stock LEGO Infrared Sensor**, so EV3-G, EV3 Classroom and EV3 MicroPython (Pybricks) read it with the infrared sensor blocks or modes; no extension needed. Every slot is in the [value reference: EV3](/en/sensors/unicam/reference.md#host-ev3), full programs in the [EV3 examples](/en/sensors/unicam/ev3-examples.md).

## Wiring

1. Plug the camera into **port 1~4** with an EV3 cable; it is powered by the brick.
2. The top-left corner of the camera screen shows the host name: **green = connected**, red = disconnected.
3. Pick the camera mode and sub-option in the camera's **MODE** menu (see "On the camera" on the [product page](/en/sensors/unicam/index.md)).

## The three infrared modes

| Infrared mode | Values | Content |
| :--- | :---: | :--- |
| Mode 0 Proximity | 1 | **Camera mode number** (camera mode × 10 + sub-option, e.g. 13 = Color tracking ③ Standard): read it first to check the camera is set right |
| Mode 1 Beacon | 8 | "Heading" and "distance" of channels 1~4, 8 values with the main data |
| Mode 2 Remote | 4 | One value per channel 1~4; only used by sub-option ①, Color order and Line following |

- Every value is an integer −127~100. **Switching modes takes about 0.5 s**, so don't keep switching between modes 1 and 2 in the same loop; decide which one you need.
- Mode 5 is the device ID (you can ignore it).

## Decoding

- **Positions use an 8×6 grid**: reading = X×10 + Y, so X = reading ÷ 10 (integer part, 1~8), Y = remainder of ÷10 (1~6). **On EV3, Y is small at the bottom** (the bottom row of the frame is 1). 0 = not seen.
- **Area**: 0~100 %, 0 = not seen.
- **Hue** (color at points, color order): positive 0~100 = hue code × 100 ÷ 240 (red 0, yellow ≈17, green ≈33, cyan 50, blue ≈67, purple ≈83); **negative = black / white / gray**: −1~−8 white → gray, −9~−15 black.
- **Line following**: offsets, angle and edges are sent as they are and can be negative (negative = line on the left).

Example: **Color tracking ③ Standard** (4 colors XY + area), all in mode 1:

| Value | Block (mode 1 beacon) |
| :--- | :--- |
| Color 1 position (X×10+Y) | channel 1 heading |
| Color 1 area % | channel 1 distance |
| Color 2 position, area | channel 2 heading, distance |
| Colors 3, 4 | channels 3, 4 |

## EV3-G / EV3 Classroom

- **EV3-G**: Infrared Sensor block → Measure → Proximity reads mode 0; Beacon with channel 1~4 gives the "heading" and "proximity" outputs, which are the heading and distance above; Remote with a channel reads mode 2.
- **EV3 Classroom**: use the Infrared Sensor proximity, beacon heading / beacon distance (pick the channel) and remote blocks, same mapping.
- Convert the readings as in "Decoding" above; in EV3-G use the Math block's divide and Modulo.

## EV3 MicroPython (Pybricks)

Read the raw modes with `LUMPDevice` (EV3 MicroPython v2 works the same way). With the camera set to **Color tracking ③ Standard**:

```python
#!/usr/bin/env pybricks-micropython
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

cam = LUMPDevice(Port.S1)        # camera on port 1

print("camera mode", cam.read(0)[0])   # mode 0: e.g. 13 = Color tracking ③ Standard

while True:
    v = cam.read(1)              # mode 1: 8 beacon values
    pos, area = v[0], v[1]       # color 1: X×10+Y, area %
    if pos:
        print("color 1: X", pos // 10, "Y", pos % 10, "area", area, "%")
    else:
        print("color 1: not seen")
    wait(100)
```

In sub-option ①, colors 9 and up are in mode 2: `cam.read(2)` (4 values).

## Slot tables and examples

- [Value reference: EV3](/en/sensors/unicam/reference.md#host-ev3) (one table per camera mode · sub-option)
- [EV3 example programs](/en/sensors/unicam/ev3-examples.md)
- Hue codes, grid conversion: [product page](/en/sensors/unicam/index.md)

## FAQ

- **The EV3 shows an Infrared Sensor?** That's expected: the camera emulates a stock LEGO sensor, so the stock blocks read it.
- **Mode 0 shows a different mode than I want?** Change it in the camera's MODE menu; the EV3 can only read it, not switch it.
- **Values freeze for half a second now and then?** Switching infrared modes takes about 0.5 s; read the same mode as much as you can.
