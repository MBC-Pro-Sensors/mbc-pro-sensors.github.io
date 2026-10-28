<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Reading it on EV3

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

On the EV3 the Sharpshooter Vision (EV3 edition) shows up as a **stock LEGO Infrared Sensor**, so EV3-G, EV3 Classroom and EV3 MicroPython (Pybricks) read it with the infrared sensor blocks or modes; no extension needed. Every slot is in the [value reference: EV3](/en/sensors/unicam/reference.md#host-ev3), full programs in the [EV3 examples](/en/sensors/unicam/ev3-examples.md).

## Wiring

1. Plug the camera into **port 1~4** with an EV3 cable; it is powered by the brick.
2. The top-left corner of the camera screen shows the host name: **green = connected**, red = disconnected.
3. Pick the camera mode and sub-option in the camera's **MODE** menu (see "On the camera" on the [product page](/en/sensors/unicam/index.md)).

## The three infrared modes

| Infrared mode | Values | Content |
| :--- | :---: | :--- |
| Mode 0 Proximity | 1 | **Camera mode number** (camera mode × 10 + sub-option, e.g. 13 = Color tracking ③ Standard): read it first to check the camera is set right |
| Mode 1 Beacon | 8 | "Heading" and "distance" of channels 1~4, 8 values with the main data |
| Mode 2 Remote | 4 | One value per channel 1~4; only used by sub-option ①, Color order, Line following and Object detection ① |

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

## Camera mode 5 object detection

The camera sorts the objects in the picture from largest to smallest (object 1 = largest) and gives each an **object code = color × 10 + shape** (13 = a triangle of target 1, 0 = none). Shapes: 0 other, 1 circle, 2 ellipse, 3 triangle, 4 square, 5 rectangle, 6 pentagon, 7 hexagon, 8 star, 9 trapezoid. X, Y is the **centroid** of the object; only target colors 1~8 count. Mode number **51 = ① Object list**, **52 = ② Object detail**. Split the code: color = code ÷ 10 (whole number), shape = remainder of code ÷ 10.

| Infrared mode / channel | 51 Object list (5) | 52 Object detail (2) |
| :--- | :--- | :--- |
| mode 1 channel 1 heading / distance | object 1 code / position | object 1 code / position |
| mode 1 channel 2 | object 2 code / position | object 1 area % / angle ÷ 2 |
| mode 1 channel 3 | object 3 code / position | object 2 code / position |
| mode 1 channel 4 | object 4 code / position | object 2 area % / angle ÷ 2 |
| mode 2 remote | channels 1, 2 = object 5 code, position; channel 4 = object count | unused |

- Positions are the same 8×6 cell X×10+Y (**Y bottom = small**).
- Angle: the camera sends "(angle + 1) ÷ 2"; the value × 2 is 0~179 degrees (within 1 degree; long side pointing right = 0, turning toward the bottom of the picture is positive).
- Mode 52 is all in mode 1, so the loop never switches modes; in 51 object 5 and the object count are in mode 2 (+0.5 s).

```python
#!/usr/bin/env pybricks-micropython
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

cam = LUMPDevice(Port.S1)        # camera set to Object detection ② Object detail (mode number 52)

while True:
    v = cam.read(1)              # mode 1 beacon: object 1 code, position, area, angle ÷ 2; object 2 the same 4
    code, pos, area, half = v[0], v[1], v[2], v[3]
    if code:                     # code 0 = no object
        print("object 1 code", code, "color", code // 10, "shape", code % 10,
              "X cell", pos // 10, "Y cell", pos % 10, "area", area, "% angle about", min(179, half * 2))
    else:
        print("no object")
    wait(100)
```

The MBC library `MBC_UniCam_EV3.py` is easier: `object(i)`, `object_xy(i)`, `object_area(i)`, `object_angle(i)` (already × 2), `object_count()`; see [EV3 example 4](/en/sensors/unicam/ev3-examples.md). In EV3-G / EV3 Classroom read "heading" and "proximity" as in the table and split them with divide and modulo.

## Slot tables and examples

- [Value reference: EV3](/en/sensors/unicam/reference.md#host-ev3) (one table per camera mode · sub-option)
- [EV3 example programs](/en/sensors/unicam/ev3-examples.md)
- Hue codes, grid conversion: [product page](/en/sensors/unicam/index.md)

## FAQ

- **The EV3 shows an Infrared Sensor?** That's expected: the camera emulates a stock LEGO sensor, so the stock blocks read it.
- **Mode 0 shows a different mode than I want?** Change it in the camera's MODE menu; the EV3 can only read it, not switch it.
- **Values freeze for half a second now and then?** Switching infrared modes takes about 0.5 s; read the same mode as much as you can.
