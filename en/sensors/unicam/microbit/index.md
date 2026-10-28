<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Reading it on micro:bit

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

The Sharpshooter Vision (Universal I2C edition) is read over I2C exactly like on Arduino; a micro:bit V1 / V2 reads it with the built-in MakeCode I2C functions at address **`0x4C`**. Value numbers and commands are the same as in [Reading it on Arduino](/en/sensors/unicam/arduino/index.md); every slot is in the [value reference: I2C](/en/sensors/unicam/reference.md#host-i2c), full programs in the [micro:bit examples](/en/sensors/unicam/microbit-examples.md).

## Wiring

1. Camera SDA to **P20** of the breakout, SCL to **P19**, GND to GND.
2. It **must use an external 5 V supply** (ESP32 camera + screen draw a lot), not the micro:bit 3V pin; share the supply GND with the micro:bit.

> [!CAUTION]
> **I2C pins are provisional, pending hardware tests**: on the camera side they are currently SDA = IO3, SCL = IO1.

## Smallest working program (MakeCode JavaScript)

Switch to **Color tracking ③ Standard** and plot color 1's position on the LEDs (values 1~4 = `0x10`~`0x13`: X, Y, area %, count):

```typescript
const CAM = 0x4C

// Read k consecutive values from number id (k <= 16), each int16 low byte first
function camRead(id: number, k: number): number[] {
    pins.i2cWriteNumber(CAM, id, NumberFormat.UInt8LE, false)
    basic.pause(2)
    const buf = pins.i2cReadBuffer(CAM, 2 * k, false)
    const out: number[] = []
    for (let i = 0; i < k; i++) out.push(buf.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// Command E0: switch to Color tracking ③ Standard
const cmd = pins.createBuffer(3)
cmd[0] = 0xE0
cmd[1] = 1
cmd[2] = 3
pins.i2cWriteBuffer(CAM, cmd, false)

basic.forever(function () {
    const v = camRead(0x10, 4)          // color 1: X, Y, area, count
    basic.clearScreen()
    if (v[0] > 0) {
        led.plot(Math.idiv(v[0] * 5, 160), Math.idiv(v[1] * 5, 120))
    }
    basic.pause(100)
})
```

Paste it into the MakeCode JavaScript tab; switch back to Blocks to see the matching blocks. The MBC MakeCode extension (pxt-mbc-sensors) has no Sharpshooter Vision blocks yet.

## Camera mode 5 object detection

The camera sorts the objects in the picture from largest to smallest (object 1 = largest) and gives each an **object code = color × 10 + shape** (13 = a triangle of target 1, 0 = none). Shapes: 0 other, 1 circle, 2 ellipse, 3 triangle, 4 square, 5 rectangle, 6 pentagon, 7 hexagon, 8 star, 9 trapezoid. X, Y is the **centroid** of the object; only target colors 1~8 count. Mode number **51 = ① Object list**, **52 = ② Object detail**. Split the code: color = code ÷ 10 (whole number), shape = remainder of code ÷ 10.

The value table is the same as for Arduino: 51 has 4 values per object (color, shape, X, Y) and the object count at `0x30`; 52 has 10 values per object (plus width, height, area %, angle, vertices, track id) and the count at `0x60` (full table in [How Arduino reads it](/en/sensors/unicam/arduino/index.md)). This program switches to the object list (51), plots the largest object on the LEDs and shows its code on button A:

```typescript
const CAM = 0x4C

// Read k consecutive values from number id (k <= 16), each int16 low byte first
function camRead(id: number, k: number): number[] {
    pins.i2cWriteNumber(CAM, id, NumberFormat.UInt8LE, false)
    basic.pause(2)
    const buf = pins.i2cReadBuffer(CAM, 2 * k, false)
    const out: number[] = []
    for (let i = 0; i < k; i++) out.push(buf.getNumber(NumberFormat.Int16LE, 2 * i))
    return out
}

// Command E0 [5, 1]: switch to Object detection ① Object list
const cmd = pins.createBuffer(3)
cmd[0] = 0xE0
cmd[1] = 5
cmd[2] = 1
pins.i2cWriteBuffer(CAM, cmd, false)

basic.forever(function () {
    const n = camRead(0x30, 1)[0]       // 0x30: object count (mode 51)
    const v = camRead(0x10, 4)          // object 1 (largest): color, shape, X, Y
    basic.clearScreen()
    if (n > 0) {
        led.plot(Math.idiv(v[2] * 5, 160), Math.idiv(v[3] * 5, 120))
    }
    basic.pause(100)
})

input.onButtonPressed(Button.A, function () {
    const v = camRead(0x10, 2)
    basic.showNumber(v[0] * 10 + v[1])  // object 1 code = color × 10 + shape (0 = none)
})
```

The helpers in the examples, `ucReadObject(mode, i)`, `ucObjectCount(mode)` and `ucShapeName(s)`, wrap all of this; see [micro:bit example 4](/en/sensors/unicam/microbit-examples.md).

## Slot tables and examples

- [Value reference: I2C](/en/sensors/unicam/reference.md#host-i2c) (value 1 = `0x10`)
- [micro:bit example programs](/en/sensors/unicam/microbit-examples.md)
- Value numbers and commands: [Reading it on Arduino](/en/sensors/unicam/arduino/index.md)

## FAQ

- **Only zeros or odd numbers?** Check the external 5 V is on and GND is shared, then read `0x01` to check the camera mode.
- **Reading −32768?** That number is not defined in the current camera mode.
