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

## Slot tables and examples

- [Value reference: I2C](/en/sensors/unicam/reference.md#host-i2c) (value 1 = `0x10`)
- [micro:bit example programs](/en/sensors/unicam/microbit-examples.md)
- Value numbers and commands: [Reading it on Arduino](/en/sensors/unicam/arduino/index.md)

## FAQ

- **Only zeros or odd numbers?** Check the external 5 V is on and GND is shared, then read `0x01` to check the camera mode.
- **Reading −32768?** That number is not defined in the current camera mode.
