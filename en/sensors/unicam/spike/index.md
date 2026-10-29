<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Reading it on SPIKE

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

The Sharpshooter Vision works with SPIKE Prime (yellow hub) and Robot Inventor (teal hub). The SPIKE edition firmware detects by itself whether the hub runs the official app or Pybricks, so there is nothing to reflash. This page helps you pick one of the four ways to read it and shows the smallest working code; every slot is listed in the [value reference](/en/sensors/unicam/reference.md), full programs are in the [SPIKE examples](/en/sensors/unicam/spike-examples.md).

## Pick a way to read it

| Way | What you need | Color tracking ① Max colors | Good for |
| :--- | :--- | :---: | :--- |
| **Pybricks library** (modes 9 / 6) | Pybricks firmware on the hub + the MBC library | 14 colors | Competitions, the most data, every line-following value |
| **Pybricks built-in blocks** (modes 5 / 7) | Pybricks firmware, no library | 2 colors | One or two colors, written with Pybricks blocks |
| **Official SPIKE App, Education** (mode 0) | Stock SPIKE Prime firmware | 7 colors in blocks | Classes, no reflashing |
| **Robot Inventor App, home edition** (mode 0) | Stock Robot Inventor firmware | 4 colors | Home-edition hubs |

How many colors each way gets in the other camera modes: see [colors per host](/en/sensors/unicam/reference.md#counts).

## Wiring and camera setup

1. Plug the camera into **any port A~F**. It is powered by the hub, no battery needed.
2. The top-left corner of the camera screen shows the host name: **green = connected**, red = disconnected.
3. Pick the camera mode and sub-option in the camera's **MODE** menu (see "On the camera" on the [product page](/en/sensors/unicam/index.md)). The sub-option names show how many colors this hub actually gets.
4. **The hub cannot read which mode the camera is in**, so your program must match the camera setting: pass the mode number to the Pybricks library (camera mode × 10 + sub-option, e.g. `mode=13`; color order = `30`); with the official app, decode according to the mode you picked.

## Pybricks library (mode 9)

Mode 9 reads 7 int16 = 14 bytes at once, one value per byte; coordinates are 160×120 pixels (Y: small at top), 0 = no target. The MBC library decodes everything from the mode number (it splits the 14-color cell numbers into X, Y and reads counts from mode 6); the library download and full programs are in the [SPIKE examples](/en/sensors/unicam/spike-examples.md).

> [!IMPORTANT]
> This generation's mode 9 layout is **different from the legacy School / Kito edition**; the Kito `camLibForKitoMode9` cannot be reused.

You can also read it without the library: use the Pybricks built-in `PUPDevice`, read mode 9 and split it into bytes. With the camera set to **Color tracking ③ Standard** (mode number 13), this reads color 1:

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

cam = PUPDevice(Port.A)          # camera on port A

def cam_bytes():
    """Read mode 9: split 7 int16 into 14 bytes (low byte first)"""
    out = []
    for v in cam.read(9):
        out.append(v & 0xFF)
        out.append((v >> 8) & 0xFF)
    return out

while True:
    b = cam_bytes()
    x, y, area = b[0], b[1], b[2]    # Standard level: color 1 X, Y, area %
    if x:
        print("color 1: X", x, "Y", y, "area", area, "%")
    else:
        print("color 1: not seen")
    wait(100)
```

In line following (mode numbers 41 / 42 / 43) the offsets, angle and edges can be negative; they are **signed bytes**:

```python
def s8(v):                       # 0~255 -> -128~127
    return v - 256 if v > 127 else v

b = cam_bytes()
near = s8(b[0])                  # near offset -100~100 (negative = line on the left)
angle = s8(b[3])                 # line angle -90~90
junction = b[6]                  # junction: 0 none 1 cross 2 T 3 left 4 right 5 dead end
```

- **Mode 6**: read it only when you need counts in Color tracking ① Max colors or ② Multi-color (`cam.read(6)`, switching modes adds about 30 ms); layout in the [value reference](/en/sensors/unicam/reference.md#pyb-11).
- **Mode 8**: device ID (shared by every MBC product; the MBC Blocks website uses it to recognise the product).

### Plugged into the EXP6 expander

When you run out of ports, plug the Sharpshooter Vision into port 1–6 of the [EXP6 six-port expander](/en/sensors/exp6/index.md). Use the EXP6 v2 library `MBC_exp6_obj_Lib_v2.py` ([downloads](/en/downloads.md)); the function names are the same as `UniCam` above, and a reading that cannot be made returns `None`. The EXP6 needs firmware v2. The official SPIKE App library has the same calls (`MBC_exp6_SPIKE_App_Lib_v2.py`).

```python
from MBC_exp6_obj_Lib_v2 import MBC_EXP6, run_task

exp6 = MBC_EXP6(1, True)      # EXP6 on hub port A
cam = exp6.unicam(5, 13)      # camera on EXP6 port 5; mode must match the camera menu

async def main():
    while True:
        print(await cam.xy(1))

run_task(main())
```

> [!NOTE]
> The library was compared item by item with the direct `UniCam` library on a computer; **not yet verified on hardware**.

## Pybricks built-in blocks (modes 5 / 7)

No library, just the Pybricks built-in **color sensor** blocks `reflection()` (mode 5) and `ambient()` (mode 7); each camera mode gives one or two colors. Color tracking uses a 40×30 grid, the same as the Kito edition:

```python
from pybricks.pupdevices import ColorSensor
from pybricks.parameters import Port
from pybricks.tools import wait

cam = ColorSensor(Port.A)        # camera on port A

while True:
    r = int(cam.reflection())
    if r < 0:
        r += 6399                # above 3199 it shows up negative; add 6399
    x1, y1 = r // 100, r % 100   # color 1: X 1~40, Y 1~30; 0 = not seen
    a = cam.ambient()            # color 2 shown as X.Y (e.g. 12.05 = X 12, Y 5)
    x2 = int(a)
    y2 = round((a - x2) * 100)
    print(x1, y1, x2, y2)
    wait(100)
```

How color at points, color order and line following fit into these two values: [value reference: Pybricks built-in blocks](/en/sensors/unicam/reference.md#host-pyblk). Line offsets are sent **+100**; subtract 100.

## Official SPIKE App

The official app shows the camera as a **Color Sensor**. Read it with the **color, reflected light and raw red / green / blue** blocks under More Sensors, then convert as below. Coordinates are grid cells (Y: small at top), 0 = not seen.

### Education (SPIKE Prime, yellow hub)

Example: **Color tracking ③ Standard** (4 colors, 16×12 grid):

| Value | Block | Conversion |
| :--- | :--- | :--- |
| Color 1 X (1~16) | color | as read |
| Color 1 Y (1~12) | reflected light | as read |
| Color 2 X | raw red | raw red **mod** 256 |
| Color 2 Y | raw red | raw red ÷ 256, integer part |
| Colors 3 and 4 | raw green, raw blue | same as color 2 |

**Color tracking ① Max colors and ② Multi-color** use an 8×6 grid with two colors per slot: color = colors found, reflected light = color 1 as X×10+Y; raw red = color 2 ×100 + color 3, so split ÷100 into two "X×10+Y" codes, then split each ÷10 into X and Y.

Counts and areas are in bytes 8~15 of mode 0, which **the blocks cannot read**; whether the app's Python can read them is still to be tested on hardware.

### Home edition (Robot Inventor, teal hub)

When the hub sends 0x25 the camera switches to the home-edition layout: color only goes to 0~10, reflected light up to 126, raw red / green / blue 0~255, one value per slot. Color tracking uses an 8×6 grid:

| Value | Block | Conversion |
| :--- | :--- | :--- |
| Color 1 X (1~8) | color | as read |
| Color 1 Y (1~6) | reflected light | as read |
| Color 2 | raw red | X = raw red ÷ 10 (integer part), Y = remainder of ÷10 |
| Colors 3 and 4 | raw green, raw blue | same as color 2 |

Every slot of the other camera modes: [value reference: Education](/en/sensors/unicam/reference.md#host-edu), [home edition](/en/sensors/unicam/reference.md#host-toy). Line following and other values that can be negative are sent **+100**.

## Camera mode 5 object detection

The camera sorts the objects in the picture from largest to smallest (object 1 = largest) and gives each an **object code = color × 10 + shape** (13 = a triangle of target 1, 0 = none). Shapes: 0 other, 1 circle, 2 ellipse, 3 triangle, 4 square, 5 rectangle, 6 pentagon, 7 hexagon, 8 star, 9 trapezoid. X, Y is the **centroid** of the object; only target colors 1~8 count. Mode number **51 = ① Object list**, **52 = ② Object detail**. Split the code: color = code ÷ 10 (whole number), shape = remainder of code ÷ 10.

**Pybricks library** (easiest; [download MBC_UniCam.py](/examples/unicam/pybricks/MBC_UniCam.py) and import it with your program):

```python
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam, shape_name

cam = UniCam(Port.A, mode=51)            # camera set to Object detection ① Object list
while True:
    print("objects", cam.object_count())
    for i in range(1, cam.max_object() + 1):     # SPIKE gets 4 objects, object 1 is the largest
        if cam.object_found(i):
            x, y = cam.object_xy(i)
            print(" object", i, "code", cam.object(i), shape_name(cam.object_shape(i)), "X", x, "Y", y)
    wait(200)
```

With `mode=52` you get 2 objects plus `object_size(i)` (width, height) and `object_area(i)` (area %), but no object count. Method list and a full program: [SPIKE example 6](/en/sensors/unicam/spike-examples.md).

**Without the library (split mode 9 yourself)**:

| Mode | byte 0 | Then |
| :---: | :--- | :--- |
| 51 | object count 0~8 | 3 bytes per object: code, X, Y (object 1 = bytes 1~3 … object 4 = bytes 10~12) |
| 52 | object 1 color | 7 bytes per object: color, shape, X, Y, width, height, area % (object 1 = bytes 0~6, object 2 = bytes 7~13); no count, no angle |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

cam = PUPDevice(Port.A)          # camera set to Object detection ② Object detail (mode number 52)

while True:
    b = []
    for v in cam.read(9):        # 7 int16 split into 14 bytes (low byte first)
        b.append(v & 0xFF)
        b.append((v >> 8) & 0xFF)
    for i in range(2):           # 52: 7 bytes per object, object 1 is the largest
        color, shape, x, y, w, h, area = b[7 * i:7 * i + 7]
        if color:                # color 0 = no such object
            print("object", i + 1, "code", color * 10 + shape, "X", x, "Y", y, "w", w, "h", h, "area", area, "%")
    wait(100)
```

**Pybricks built-in blocks** (`reflection()`, `ambient()`, object 1 only): `reflection()` = object 1 X cell × 100 + Y cell (40×30 grid: X 1~40, Y 1~30, top = small, 0 = none; add 6399 when negative); `ambient()` shows A.BB: A = object 1 code, BB = object count (51) or object 1 area % (52, 100 shows as 99).

```python
from pybricks.pupdevices import ColorSensor
from pybricks.parameters import Port
from pybricks.tools import wait

cam = ColorSensor(Port.A)        # camera set to Object detection ① (51) or ② (52): object 1 only

while True:
    r = int(cam.reflection())
    if r < 0:
        r += 6399                # above 3199 it shows as negative: add 6399
    gx, gy = r // 100, r % 100   # object 1 position: 40×30 grid (X 1~40, Y 1~30, top = small); 0 = none
    a = cam.ambient()            # shown as A.BB
    code = int(a)                # A: object 1 code (color × 10 + shape)
    bb = round((a - code) * 100) # BB: 51 = object count, 52 = object 1 area % (100 shows as 99)
    print("code", code, "color", code // 10, "shape", code % 10, "cell", gx, gy, bb)
    wait(100)
```

**Official SPIKE App** (the 5 values the blocks can read):

| Block | Education 51 | Education 52 | Home 51 | Home 52 |
| :--- | :--- | :--- | :--- | :--- |
| color | object count | object 1 code | object count | object count |
| reflection | object 1 code | object 1 area % | object 1 code | object 1 code |
| raw red | object 1 position (X×10+Y) | object 1 X = remainder (÷256), Y = ÷256 whole number | object 1 position (X×10+Y) | object 1 X (pixels) |
| raw green | ÷100 whole number = object 2 code, remainder = object 2 position | object 1 width = remainder, height = ÷256 | object 2 code | object 1 Y (pixels) |
| raw blue | like raw green, object 3 | object 2 code = remainder, area % = ÷256 | object 2 position | object 1 area % |

A position is an 8×6 cell "X×10+Y": X = ÷10 whole number (1~8), Y = remainder (1~6, top = small), pixels ≈ cell × 20 − 10. In Education App Python the reading helper [MBC_UniCam_App_helper.py](/examples/unicam/spike_app/MBC_UniCam_App_helper.py) has `object(i)`, `object_cell(i)`, `object_xy(1)` …; Education bytes 8~15 (objects 4~7, angle, vertices) cannot be read with blocks.

## Slot tables and examples

- [Value reference: Pybricks library](/en/sensors/unicam/reference.md#host-pyb) · [SPIKE App](/en/sensors/unicam/reference.md#host-edu) · [Robot Inventor App](/en/sensors/unicam/reference.md#host-toy) · [Pybricks built-in blocks](/en/sensors/unicam/reference.md#host-pyblk)
- [SPIKE example programs (library download)](/en/sensors/unicam/spike-examples.md)
- Camera modes, hue codes, grid conversion: [product page](/en/sensors/unicam/index.md)

## FAQ

- **Top-left corner stays red?** The hub has not connected yet: unplug and replug, wait 2 s, or try another port.
- **Values don't match the tables?** Check that the "mode-sub-option" in the top-right corner of the camera (e.g. 1-3) is the one your program expects.
- **Can I reuse a legacy School edition program?** No, the layout is different; the legacy guides stay online for reference in [Sharpshooter Vision legacy editions (no longer supported)](/en/sensors/esp32cam/index.md).
