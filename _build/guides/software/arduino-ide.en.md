### What you need

- **Sensor**: {{product}} ({{edition}}) with its 4-wire cable. SPIKE and EV3 editions run different firmware and cannot connect to an Arduino directly.
- **Board**: Arduino UNO (or Nano) is the most common; Mega, UNO R4 and ESP32 also work.
- **USB cable**: the UNO uses a square (USB-B) cable; a Nano uses Mini USB or Type-C depending on the board. **It must be a data cable**; some cheap cables can only charge.
- **Computer**: Windows or Mac.
- **Software**: Arduino IDE (free).

### Install the software

> [!TIP]
> First time with Arduino? Start with [First time with Arduino: from zero](/en/sensors/arduino-start.md): in 15 minutes you install the software, upload a program and see the result.

1. Download it from the official site: [arduino.cc/en/software](https://www.arduino.cc/en/software). Choose your computer system (on Windows, choose **Windows Win 10 and newer**).
2. The download page asks if you want to donate. Click **JUST DOWNLOAD** to download right away.
3. During setup, click **Next** each time. If it asks to install drivers, click **Install** every time.
4. To change the language: **File → Preferences → Language**, then click OK and restart.
5. For an ESP32, install the ESP32 board package first.

### Make the software see the sensor

**No library to install.** Every example is a single `.ino` file; the small helper functions that read and write the sensor (such as `mbcReadMany`) are already inside it. Copy, paste, done.

1. Connect the Arduino to the computer with the USB cable and choose your board (for example **Arduino Uno**) in the drop-down menu at the top of the IDE. The port is shown next to it, such as COM3 (on a Mac: `/dev/cu.usb…`).
2. On the examples page, click **Copy** at the top right of the code box, or download the `.ino` file.
3. **File → New Sketch**, delete everything in the new window and paste the code.
4. Click **→ (Upload)** at the top left. When "**Done uploading**" appears at the bottom, it worked.

The sensor talks I2C; its factory address is `{{addr}}`. Protocol details: [MBC Universal I2C protocol](/en/i2c-protocol.md).

### Wiring

MBC Universal I2C sensors use only **4 wires**: two for power (VCC, GND) and two for signals (SDA, SCL).

| Sensor | UNO / Nano | Mega | UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: | :---: |
| **VCC** | 5V | 5V | 5V | 5V (VIN) |
| **GND** | GND | GND | GND | GND |
| **SDA** | A4 | 20 | A4 | GPIO 21 |
| **SCL** | A5 | 21 | A5 | GPIO 22 |

> [!IMPORTANT]
> - **Unplug the USB cable before wiring.** Connect the wires, check them once, then plug the USB cable back in.
> - If the sensor cable is labelled TX / RX: **TX goes to SCL, RX to SDA**.
> - The connector order differs a little between products. **Always follow the labels printed on the sensor board.**
> - On the UNO, the top two unnumbered pins near the USB connector are also labelled SDA, SCL; they are the same as A4 and A5.

### First test

1. Upload example 1 on this page.
2. Click the **magnifying glass** (Serial Monitor) at the top right of the IDE and choose **115200 baud** (every MBC example uses this speed).
3. If the example's text and numbers keep updating, everything is connected.

### FAQ

- **Keeps saying the sensor is not found?** Check that SDA and SCL are not swapped and that VCC and GND are firmly in; make sure it is the **Universal I2C edition**.
- **Reading `-32768`?** The sensor could not be read (loose wire, wrong address), or that value id does not exist. Most values never take this number; the exception is a value that uses all 16 bits (Controller 16 buttons with only ■ pressed, Pathfinder 16-ch line bitmap with only channel 16 on the line, IMU raw values), which really can be -32768. For those, check the true / false returned by `mbcReadMany` instead.
- **Strange symbols in the Serial Monitor?** The speed is not **115200**.
- **Board not in the drop-down menu?** The USB cable can only charge → try another; the board is a CH340 compatible → install the CH340 driver first, then unplug and replug the USB.
- **ESP32 reads nothing or odd values?** Some MBC products are themselves built on an ESP32 and cannot clock-stretch: after writing the value id, **wait 2 ms** before reading. Do not delete the `delay(2)` in the example helper.
