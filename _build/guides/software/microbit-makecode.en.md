### What you need

- **Sensor**: {{product}} ({{edition}}). SPIKE and EV3 editions run different firmware and cannot be read by a micro:bit.
- **Board**: BBC micro:bit **V1 or V2**, plus a **breakout board or crocodile-clip adapter** to reach P19 / P20.
- **Software**: **MakeCode for micro:bit** in the browser ([makecode.microbit.org](https://makecode.microbit.org/), blocks / JavaScript); the offline app (Windows / Mac) also works, but adding an extension the first time needs internet.
- **Extension**: **pxt-mbc-sensors v2** (MIT, nothing to download, just paste the URL).

> [!IMPORTANT]
> **Bench test pending**: extension v2.0.3 compiles in MakeCode (V1 + V2) but has not yet been checked block by block on real hardware. Block names follow the [GitHub release](https://github.com/contactmbcrobot/pxt-mbc-sensors).

### Install the software

1. Open [makecode.microbit.org](https://makecode.microbit.org/) → **New Project**.
2. Traditional Chinese block names are included: switch the editor language (gearwheel → Language) if you prefer.

### Make the software see the sensor

1. Click **Advanced → Extensions** at the bottom of the toolbox (also in the gearwheel menu).
2. Paste `https://github.com/contactmbcrobot/pxt-mbc-sensors` into the search box and press Enter.
3. Click the **mbc-sensors** card. An "**MBC Sensors**" category appears with six groups: Line, Distance, Gamepad, IMU, Camera and Device.
4. To use an example: switch to **JavaScript** at the top right → paste the example → download the `.hex` and drag it onto the **MICROBIT** drive.

> [!TIP]
> No approval, no download: teachers just hand the URL to students. To pin a version, paste `https://github.com/contactmbcrobot/pxt-mbc-sensors#v2.0.3`.

### Wiring

The micro:bit I2C pins are fixed: **P19 = SCL, P20 = SDA** (on the edge connector, so you need a breakout board or clip adapter). On the MBC 4-pin sensor cable **TX goes to SCL and RX to SDA**, exactly as when wiring to an Arduino.

| Sensor | micro:bit | Note |
| :--- | :--- | :--- |
| SCL (TX) | **P19** | I2C clock |
| SDA (RX) | **P20** | I2C data |
| VCC | **3V** or external 5 V | power-hungry products must not draw from the micro:bit 3V pin |
| GND | GND | with external power, share GND with the micro:bit |

- **3V pin current**: V1 about **90 mA**, V2 about **190 mA**; on batteries the rail can drop to just over 2 V. Ranger 8-way, Controller PS4 / PS5 and the Sharpshooter **must use external 5 V**; other products can try 3V first, with external 5 V recommended on batteries.
- **Logic level**: MBC product I2C lines are 3.3 V, compatible with the micro:bit without a level shifter.
- This product's factory address is `{{addr}}`; products with different addresses can share the same P19 / P20 bus.

### First test

1. Check the wiring first with the "**Device → wait for device at {{addr}} ready**" or "**device at {{addr}} is ready**" block.
2. Download the `.hex` of example 1 on this page to the micro:bit.
3. If the number on the LEDs changes with what is in front of the sensor, everything is connected.

### FAQ

- **Values are always 0?** With no device connected the reporter blocks return **0** (the internal −32768 is filtered) and never throw. Check in order: wiring (SDA = P20, SCL = P19) → external power on → "device at … is ready" is true.
- **On a V1, the compass and accelerometer stop working too?** On V1 the external I2C bus is shared with the onboard accelerometer, so a sensor holding SDA low also kills them. V2 has a separate external bus.
- **Works on and off?** STM32 products (Pathfinder, Ranger, PS2, IMU) clock-stretch, and the nRF52 on the V2 has a known clock-stretching issue (errata 219), so V1 and V2 both need a bench test. ESP32 products (PS4, Sharpshooter) do not stretch; the extension already waits 2 ms after writing the value id.
- **Two products of the same kind?** They share an address: first change one with "Device → change device at … to address …" (applied after reboot), then tell the extension with that product's "use address" block.
