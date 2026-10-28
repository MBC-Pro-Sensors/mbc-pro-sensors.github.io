<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Reading it on MATRIX (mVision)

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

To a MATRIX Mini R4 the Sharpshooter Vision (MATRIX edition) is an **mVision camera**: read it with the built-in MATRIXblock mVision blocks, **no library to install**, no I2C. Every slot is in the [value reference: MATRIX](/en/sensors/unicam/reference.md#host-mx), full programs in the [MATRIX examples](/en/sensors/unicam/matrix-examples.md).

## Wiring

1. Connect the camera to the serial port the Mini R4 uses for the mVision camera (Serial1, 115200).
2. The top-left corner of the camera screen shows the host name: **green = connected**, red = disconnected.
3. Pick the camera mode and sub-option in the camera's **MODE** menu (see "On the camera" on the [product page](/en/sensors/unicam/index.md)). The MATRIX cannot read which mode the camera is in, so decode according to the mode you picked.

## The 10 data slots

- The "**mVision data 0~9**" block gives one number 0~65535 per slot. "Slot 1" in the value reference = **data 0** in the block, "slot 10" = **data 9**.
- **Don't use data 10**: the MATRIX program only declares 10 slots; data 10 reads unrelated memory.
- A slot often holds two values: **A + B×256**, A = reading **mod** 256, B = reading ÷ 256 (integer part).
- Coordinates are **160×120 pixels** (X 0~159 left → right, Y 0~119 top → bottom), 0 = not seen.
- Values that can be negative (line offsets, angle, edges) are sent **+100**; subtract 100.

Example: **Color tracking ③ Standard** (5 colors XY + area):

| Block | Content | Conversion |
| :--- | :--- | :--- |
| mVision data 0 | color 1 X + color 1 Y × 256 | X = mod 256, Y = ÷256 integer part |
| mVision data 1 | color 1 area % + color 1 count × 256 | area = mod 256, count = ÷256 integer part |
| mVision data 2, 3 | color 2 position, area and count | same |
| mVision data 4~9 | colors 3~5 | same |

In MATRIXblock use the Math divide and remainder blocks; check that X is not 0 before using the coordinates.

## Other camera modes

| Camera mode | What MATRIX gets |
| :--- | :--- |
| Color tracking ① Max / ② Multi / ④ Fine | 16 colors / 10 colors / 3 colors XYWH |
| Color at points ① / ② / ③ | 20 points hue / 10 points hue + saturation / 6 points hue + saturation + value |
| Color order | 18 blocks hue code + total blocks |
| Line following | 14 values (offsets, angle, widths, junction …) |

What each slot holds: [value reference: MATRIX](/en/sensors/unicam/reference.md#host-mx).

## Slot tables and examples

- [Value reference: MATRIX](/en/sensors/unicam/reference.md#host-mx)
- [MATRIX example programs](/en/sensors/unicam/matrix-examples.md)
- Hue codes, grid conversion: [product page](/en/sensors/unicam/index.md)

## FAQ

- **Odd numbers?** Make sure you are not using "data 10"; only data 0~9 mean anything. Also check that the "mode-sub-option" in the top-right corner of the camera matches how your program decodes.
- **The color sensor blocks read nothing?** On MATRIX this product talks serial, so only the mVision blocks work.
