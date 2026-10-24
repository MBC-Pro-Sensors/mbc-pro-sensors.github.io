### The EXP6 uses UART, not I2C

The EXP6 talks to the Arduino over a **UART serial port** (two wires, TX / RX), **not the I2C wiring above**. A shipped EXP6 works as is, no reflashing: it starts in "LEGO mode" and switches to "UART mode" automatically on the first command packet from the Arduino (the same command set as Pybricks).

| Board | Usable? | Talks to the EXP6 on | Notes |
| :--- | :---: | :---: | :--- |
| **Arduino UNO R4** (Minima / WiFi) | ⭐ recommended | D0, D1 (`Serial1`) | D0 / D1 are separate from USB, so uploading and the Serial Monitor are unaffected |
| Arduino Mega 2560 | ✅ | 18, 19 (`Serial1`) | 4 serial ports |
| ESP32 dev board | ✅ | GPIO16, 17 (`Serial2`) | 3.3 V signals, pin 4 goes to 3V3; install the ESP32 board package first |
| Arduino UNO R3, Nano | ❌ | — | the only hardware serial port is used by USB; software serial is unreliable at 115200 |

Connect the EXP6 **Host port** to the Arduino with a "LEGO 6-pin to Dupont" cable (or a LEGO connector breakout):

| EXP6 Host port | UNO R4 | Mega 2560 | ESP32 |
| :---: | :---: | :---: | :---: |
| pin 3 GND | GND | GND | GND |
| pin 4 VCC | **5V** | **5V** | **3V3** |
| pin 5 (EXP6 receives) | D1 (TX) | 18 (TX1) | GPIO17 (TX2) |
| pin 6 (EXP6 sends) | D0 (RX) | 19 (RX1) | GPIO16 (RX2) |
| pins 1, 2 | not connected | not connected | not connected |

> [!IMPORTANT]
> - The EXP6 XT30 power socket must have a **2~3S lithium battery**; motors and sensors run from it. The EXP6 **does not power the Arduino**.
> - **TX to EXP6 receive, RX to EXP6 send**: the two wires cross.
> - **Pin 4 must match the board's signal voltage** (UNO R4, Mega: 5V; ESP32: 3V3). This pin is also the EXP6 "ignition key": without it the EXP6 does not start.
> - Unplug USB and the battery before wiring.

> [!WARNING]
> **Watchdog**: if the EXP6 hears nothing from the Arduino for **0.5 s**, it stops the motors. Always wait with `exp6Wait(ms)`, never `delay()`.
