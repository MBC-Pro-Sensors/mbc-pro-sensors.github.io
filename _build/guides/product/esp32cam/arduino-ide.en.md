### Set the colour on the sensor first

All Sharpshooter settings are made on the **colour screen and buttons on the back of the sensor**, no computer needed: **short-press the side button** to open the settings menu, left / right to choose, OK to enter. Colour settings and camera parameters are saved automatically and used at the next power-on.

> [!NOTE]
> The Sharpshooter's I2C (address `0x4C`) uses **SDA = IO3, SCL = IO1**; once I2C is connected the USB serial monitor is gone. Hardware confirmed, **bench test pending**. The Sharpshooter is itself an ESP32 and cannot clock-stretch, so always wait 2 ms before reading (the example helper already has `delay(2)`).
