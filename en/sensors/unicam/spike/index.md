<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Reading it on SPIKE

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

The Sharpshooter Vision Universal v1 works with SPIKE Prime (yellow hub) and Robot Inventor (teal hub). The SPIKE edition firmware detects by itself whether the hub runs the official app or Pybricks, so there is nothing to reflash. This page helps you pick one of the four ways to read it and shows the smallest working code; every slot is listed in the [value reference](/en/sensors/unicam/reference.md), full programs are in the [SPIKE examples](/en/sensors/unicam/spike-examples.md).

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
> The Universal edition's mode 9 layout is **different from the School / Kito edition**; the Kito `camLibForKitoMode9` cannot be reused.

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

## Slot tables and examples

- [Value reference: Pybricks library](/en/sensors/unicam/reference.md#host-pyb) · [SPIKE App](/en/sensors/unicam/reference.md#host-edu) · [Robot Inventor App](/en/sensors/unicam/reference.md#host-toy) · [Pybricks built-in blocks](/en/sensors/unicam/reference.md#host-pyblk)
- [SPIKE example programs (library download)](/en/sensors/unicam/spike-examples.md)
- Camera modes, hue codes, grid conversion: [product page](/en/sensors/unicam/index.md)

## FAQ

- **Top-left corner stays red?** The hub has not connected yet: unplug and replug, wait 2 s, or try another port.
- **Values don't match the tables?** Check that the "mode-sub-option" in the top-right corner of the camera (e.g. 1-3) is the one your program expects.
- **Can I reuse my School edition program?** No, the Universal layout is different; for the School edition see the [School edition guide](/en/sensors/esp32cam/index.md).
