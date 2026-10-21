<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🔰 First Time with Arduino: Start from Zero

This page is for people who have **never written code**. Follow it once, in about 15 minutes, and you will connect an MBC sensor to an Arduino and see the sensor's numbers on your computer. Every product's examples work the same way after that.

> [!TIP]
> If a word is new to you, skip it and just follow the steps. After doing it once, it will make sense.

---

## What You Need

| Item | Notes |
| :--- | :--- |
| Arduino UNO (or Nano) | The most common Arduino board. Mega, UNO R4 and ESP32 also work; see the pin table below |
| USB cable | UNO uses a "square" (USB-B) cable; Nano uses Mini USB or Type-C, depending on the board. **It must be a data cable**: some cheap cables can only charge |
| Computer | Windows or Mac |
| MBC sensor (Universal I2C edition) | Comes with a 4-wire cable |

---

## Step 1: Install the Arduino IDE

The Arduino IDE is free software for writing programs and sending them to the Arduino.

1. Download it from the official site: <https://www.arduino.cc/en/software>. Choose your computer system (on Windows, choose **Windows Win 10 and newer**).
2. The download page asks if you want to donate. Click **JUST DOWNLOAD** to download right away.
3. During setup, click **Next** each time. If it asks to install drivers, click **Install** every time.
4. Open the Arduino IDE. The menus are in English. To change the language: **File → Preferences → Language**, then click OK and restart.

## Step 2: Connect the Arduino with the USB Cable

1. Connect the Arduino to the computer with the USB cable. A light on the board turns on.
2. In the drop-down menu at the top of the Arduino IDE (**Select Board**), choose your board, for example **Arduino Uno**. The port name is shown next to it, such as COM3 (on a Mac: `/dev/cu.usb…`). You can also use **Tools → Board** and **Tools → Port**.

> [!WARNING]
> **Can't find your board in the menu?** There are two common reasons:
> 1. The USB cable can only charge and cannot send data → try a different cable.
> 2. Your board is a compatible board (the USB chip on the board says **CH340**) → install the CH340 driver first (search for "CH340 driver", download and install it). Then unplug the USB cable and plug it in again.

## Step 3: Connect the MBC Sensor

MBC Universal I2C sensors use only **4 wires**: two for power (VCC, GND) and two for signals (SDA, SCL).

| Label on the sensor | Arduino UNO / Nano | Arduino Mega | UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: | :---: |
| **VCC** (or 5V) | 5V | 5V | 5V | 5V (the pin labeled VIN or 5V) |
| **GND** | GND | GND | GND | GND |
| **SDA** | A4 | 20 | A4 | GPIO 21 |
| **SCL** | A5 | 21 | A5 | GPIO 22 |

> [!IMPORTANT]
> - **Unplug the USB cable before wiring.** Connect the wires, check them once, then plug the USB cable back in.
> - On the UNO, near the USB connector, the top two pins without numbers are also labeled **SDA, SCL**. They are the same as A4 and A5, so you can use either.
> - The connector order is a little different on each product. **Always follow the labels printed on the sensor board.**

## Step 4: Send the Example Program to the Arduino

1. On the product's examples page, click **Copy** at the top right of the code box, or download the `.ino` file.
2. In the Arduino IDE: **File → New Sketch**. Delete everything in the new window, then paste the code you copied.
3. Click the **→ (Upload)** button at the top left. The first time, it asks where to save. Give it any name and save it.
4. When you see "**Done uploading**" at the bottom, it worked.

> [!TIP]
> Upload failed with red text? The most common reason is **the wrong board or port**. Go back to Step 2 and choose them again.

## Step 5: Open the Serial Monitor to See the Numbers

The Arduino has no screen. The examples "print" what the sensor reads to your computer.

1. Click the **magnifying glass icon** (Serial Monitor) at the top right of the Arduino IDE. You can also use **Tools → Serial Monitor**.
2. In the speed menu on the right side of the bottom window, choose **115200 baud** (every MBC example uses this speed).
3. You should see the example's text and numbers updating again and again.

> [!WARNING]
> **Seeing strange symbols?** The speed is not 115200. Change it and it will look normal.
>
> **Keeps saying the sensor is not found?** Check the 4 wires from Step 3: are SDA and SCL swapped? Are VCC and GND firmly plugged in?

---

## What Do the Symbols in the Code Mean?

Every line in the examples has an English note. Knowing these few is enough to start:

| What you see | Meaning |
| :--- | :--- |
| Words after `//` | A **note** for people to read. The Arduino does not run it |
| `void setup() { … }` | Things done **only once** at power-on (for example: get the sensor ready) |
| `void loop() { … }` | Things done **again and again** (for example: keep reading the sensor) |
| `delay(500);` | Wait a moment. The unit is milliseconds: `500` = 0.5 seconds |
| `Serial.println(…);` | Print something to the Serial Monitor, then start a new line |
| `if (…) { … }` | "If" the thing in the round brackets is true, do the things in the curly brackets |
| Things like `int distance = …;` | Make a "box" (a variable) named `distance` to store a number, so you can use it later. Box names can only use English letters and numbers |

> [!TIP]
> Numbers marked **"👉 You can change this"** in the examples are safe to change. Change one, click Upload again, and see what is different.

---

## Arduino Examples for Each Product

| Product | Examples page |
| :--- | :--- |
| Pathfinder 8-Way | [Arduino Examples](/en/sensors/line8/arduino-examples.md) |
| Pathfinder 16-Way | [Arduino Examples](/en/sensors/line16/arduino-examples.md) |
| Ranger 8-Way | [Arduino Examples](/en/sensors/tof8/arduino-examples.md) |
| Ranger 2-Way (new version coming soon) | [Arduino Examples](/en/sensors/tof2/arduino-examples.md) |
| Controller 2.4G (PS2, Taiwan only) | [Arduino Examples](/en/sensors/ps2/arduino-examples.md) |
| Controller PS4/PS5 | [Arduino Examples](/en/sensors/ps4/arduino-examples.md) |
| IMU Gyro | [Arduino Examples](/en/sensors/imu/arduino-examples.md) |
| SPIKE 6-Way Expander | [Arduino Examples](/en/sensors/exp6/arduino-examples.md) (this one connects over serial, not like Step 3 above; see its examples page) |

Want the communication rules shared by every MBC product (for people who can code)? See the [MBC Universal I2C Protocol](/en/i2c-protocol.md).
