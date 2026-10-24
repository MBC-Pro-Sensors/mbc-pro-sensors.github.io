### Set the colour on the sensor first

All Sharpshooter settings are made on the **colour screen and buttons on the back of the sensor**, no computer needed: **short-press the side button** to open the settings menu, left / right to choose, OK to enter. Colour settings and camera parameters are saved automatically and used at the next power-on.

> [!IMPORTANT]
> **The Sharpshooter must use external 5 V power** (ESP32 camera + screen + flash light); it cannot run from the micro:bit 3V pin. Share GND between the external supply and the micro:bit.

> [!NOTE]
> The Sharpshooter's I2C uses **SDA = IO3, SCL = IO1** (fixed 2026-10-21); once I2C is connected the USB serial monitor is gone. Hardware confirmed, **bench test pending**.
