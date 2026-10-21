<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 IMU Gyro × Arduino Examples

[🔙 Back to IMU Gyro Home](/en/sensors/imu/index.md)　|　[📡 MBC Universal I2C Protocol (advanced)](/en/i2c-protocol.md)

This page has **7 examples**, from "is it connected?" all the way to "a car that turns 90 degrees in place". You can copy and upload each example as it is, and every line of the code has an English comment. **Never written code before? No problem.** Just do them one by one, in order.

> [!TIP]
> First time using Arduino? Read [🔰 First Time with Arduino: Start from Zero](/en/sensors/arduino-start.md) first. In 15 minutes you learn how to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/imu/arduino-en/imu_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
</div>

---

## 🔌 Wiring (the same for every example)

| Sensor | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: |
| VCC | 5V | 3V3 or 5V (VIN) |
| GND | GND | GND |
| SDA | A4 | GPIO 21 |
| SCL | A5 | GPIO 22 |

> [!IMPORTANT]
> - Use the **Universal I2C edition** of the gyro. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
> - When the gyro powers on, it must **stay still for about 0.5 seconds** before it is ready. Every example first prints "do not move it". Start playing after you see "Ready".
> - Unplug the USB cable before you connect the wires.

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the sensor](#ex1) | Check the wiring and see which face is up | Nothing |
| 2 | [How many degrees did I turn?](#ex2) | Read the "heading" and reset it to 0 | Nothing |
| 3 | [Spirit level](#ex3) | Use acceleration to check if it is level | A book |
| 4 | [90-degree turn alert](#ex4) | Use the heading to count "how many 90-degree turns" | Nothing |
| 5 | [Shake detector](#ex5) | Read the "turning speed" (angular rate) | Nothing |
| 6 | [Electronic dice](#ex6) | Read "which face is up" and "is it still" | Nothing |
| 7 | [Car turns 90 degrees in place (advanced)](#ex7) | Use the gyro to make a car turn accurately | Two-wheel car, L298N motor driver board |

> [!TIP]
> **Suggested order: 1 → 2 → 4 → 3 → 6 → 5 → 7.** First check the wiring (1), then understand the "heading" (2, 4). If you want to build a car, you must do Example 2 first.

---

<h2 id="ex1">1️⃣ Say hello to the sensor</h2>

**What it does:** Checks that the sensor is connected, and shows "which face is pointing up now". Run this one first every time you connect a new sensor.

**Steps:**

1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Put the sensor **flat on the table**. Do not hold it in your hand.
3. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
4. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.

<a href="/examples/imu/arduino-en/imu_01_hello/imu_01_hello.ino" download data-ignore="true">📄 Download imu_01_hello.ino</a>

<!-- ino: examples/imu/arduino-en/imu_01_hello/imu_01_hello.ino -->
```cpp
/*
 * IMU Gyro x Arduino  Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   It checks that the sensor is connected. If it is, the computer shows
 *   the sensor's name and version, and "which face is pointing up now".
 *   Run this example first every time you connect a new sensor.
 *
 * You need:
 *   Arduino UNO / Nano (or ESP32), IMU gyro (Universal I2C edition), 4 jumper wires.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Good to know:
 *   When the gyro powers on, it must stay "still" for about 0.5 seconds
 *   before it says "I am ready". So put the sensor flat on the table
 *   and do not touch it until you see the result.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the sensor

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's "address" (like a house number). It is always 0x6A

void setup() {
  Serial.begin(115200);                  // Open the line to the computer, speed 115200
  mbcBegin();                            // Get I2C ready (the helpers are at the bottom of this file)
  delay(500);                            // Give the sensor 0.5 seconds to power on

  Serial.println(F("=== IMU Gyro: Say hello to the sensor ==="));
  Serial.println(F("Put the sensor on the table and do not move it..."));

  // Wait for the sensor to say "I am ready", up to 5 seconds (it must stay still for about 0.5 s first)
  if (!mbcWaitReady(5000)) {
    uint8_t id;
    if (mbcRead(0x02, &id, 1) && id == 0x6A) {
      // The sensor is found, but it keeps thinking it is moving
      Serial.println(F("Sensor found, but it keeps feeling shaky and cannot get ready. Please check:"));
      Serial.println(F("  1. Is the sensor on the table? Holding it in your hand always shakes it a little."));
      Serial.println(F("  2. Is the table shaking? (A motor or fan running nearby can do this too)"));
      Serial.println(F("  3. After it is still, press the Arduino RESET button and try again."));
    } else {
      Serial.println(F("Sensor not found! Please check:"));
      Serial.println(F("  1. SDA to A4, SCL to A5. Are they swapped?"));
      Serial.println(F("  2. VCC to 5V, GND to GND. Are they plugged in firmly? Is the red light on the sensor blinking?"));
      Serial.println(F("  3. Is your sensor the Universal I2C edition? (The SPIKE and EV3 editions cannot connect to Arduino)"));
      Serial.println(F("  4. Still not working: unplug the sensor power and plug it back in, wait 3 seconds, then press the Arduino RESET button."));
    }
    while (true) {}                      // Stop here and do nothing more
  }

  // ---- 1. Read the "version info": box 0x01 of the sensor, 17 numbers ----
  uint8_t info[17];
  mbcRead(0x01, info, 17);

  // Numbers 9 to 16 are the sensor's name (letters)
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("Found it! Sensor name: "));
  Serial.println(name);                  // It should show IMU

  Serial.print(F("Firmware version: "));
  Serial.print(info[2]);                 // Major version
  Serial.print('.');
  Serial.println(info[3]);               // Minor version

  Serial.print(F("Firmware date: 20"));
  Serial.print(info[4]);                 // Year (last two digits)
  Serial.print('/');
  Serial.print(info[5]);                 // Month
  Serial.print('/');
  Serial.println(info[6]);               // Day

  Serial.print(F("Magnetometer: "));
  if (info[8] == 1) Serial.println(F("OK"));
  else              Serial.println(F("not detected"));

  // ---- 2. Read the "status": box 0x31, 9 numbers ----
  uint8_t st[9];
  mbcRead(0x31, st, 9);
  uint8_t flags = st[0];                 // A row of "switches"; each bit means one thing
  uint8_t upFace = st[4];                // Which face is pointing up now: 0 to 5

  Serial.print(F("Face pointing up now: "));
  printFace(upFace);
  Serial.println();

  if (flags & (1u << 3)) {               // Switch 3 = mounted too tilted
    Serial.println(F("Note: the sensor is mounted too tilted (more than about 45 degrees). The heading may be wrong. Please mount it straighter."));
  }

  Serial.println(F("Wiring is correct. You can start the other examples now!"));
}

void loop() {
  // This example only checks once at power-on, so loop does nothing
}

// Print the "face pointing up" number as words you can read
void printFace(uint8_t f) {
  switch (f) {
    case 0:  Serial.print(F("+X face")); break;
    case 1:  Serial.print(F("+Y face")); break;
    case 2:  Serial.print(F("+Z face")); break;
    case 3:  Serial.print(F("-X face")); break;
    case 4:  Serial.print(F("-Y face")); break;
    case 5:  Serial.print(F("-Z face")); break;
    default: Serial.print(F("unknown")); break;
  }
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

// Start I2C communication (call once in setup)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// Read n numbers from box reg of the sensor into buf. Returns true if it worked
bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// Write the number value into box reg of the sensor. Returns true if it worked
bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready, up to timeoutMs milliseconds. Returns true when ready
// (The gyro must stay still for about 0.5 s before it is ready, so the examples wait 5 seconds)
bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
=== IMU Gyro: Say hello to the sensor ===
Put the sensor on the table and do not move it...
Found it! Sensor name: IMU
Firmware version: 1.0
Firmware date: 2026/10/12
Magnetometer: OK
Face pointing up now: +Z face
Wiring is correct. You can start the other examples now!
```

> [!NOTE]
> **What are the "Communication helpers"?** At the bottom of every example there is the same piece of code. It "talks" to the sensor. You do not need to change it or understand it. Just remember:
> - `mbcRead(box number, where to put it, how many)`: read data from the sensor
> - `mbcWrite(box number, number)`: write a command into the sensor
>
> The sensor is like a row of numbered "boxes". Each box holds different data. For example, box `0x11` holds "how many degrees it has turned".

---

<h2 id="ex2">2️⃣ How many degrees did I turn?</h2>

**What it does:** Turn the sensor slowly on the table. The computer keeps telling you "how many degrees it has turned now". Type **`z`** in the input box at the top of the Serial Monitor and press Enter, and the direction it faces now becomes 0 degrees.

<a href="/examples/imu/arduino-en/imu_02_heading/imu_02_heading.ino" download data-ignore="true">📄 Download imu_02_heading.ino</a>

<!-- ino: examples/imu/arduino-en/imu_02_heading/imu_02_heading.ino -->
```cpp
/*
 * IMU Gyro x Arduino  Example 2: How many degrees did I turn? (heading)
 * ------------------------------------------------------------
 * What does this example do?
 *   Turn the sensor slowly on the table. The computer keeps telling you
 *   "how many degrees it has turned now".
 *     Seen from above, turn "clockwise" (to the right)        -> the number gets bigger
 *     Seen from above, turn "counterclockwise" (to the left)  -> the number gets smaller (it can go below 0)
 *   The number keeps adding up: two turns to the right is 720 degrees. It does not jump back to 0.
 *   It also shows the "0 to 359 degrees" style, which goes back to 0 after one turn, like a clock.
 *   Type z in the Serial Monitor and press Enter to reset the angle (the direction now = 0 degrees).
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Wait until you see "Ready" before you start turning.
 *
 * Good to know:
 *   When the gyro powers on, it takes "the direction at that moment" as 0 degrees.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  Serial.println(F("Put the sensor on the table and do not move it..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}
  }
  Serial.println(F("Ready! Turn the sensor slowly and watch. Type z and press Enter to reset to 0."));
}

void loop() {
  // ---- 1. Reset to 0 when we get z ----
  if (Serial.available()) {
    char c = Serial.read();              // Read one letter you typed
    if (c == 'z' || c == 'Z') {
      mbcWrite(0x10, 0x01);              // Write 0x01 into box 0x10 = "reset heading"
      delay(50);                         // Wait a little after the reset (at least 30 ms) before reading, so you get the new 0
      Serial.println(F("--- Reset! This direction is now 0 degrees ---"));
    }
  }

  // ---- 2. Read the heading: box 0x11, 6 numbers (3 kinds of heading, 2 numbers each) ----
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {            // If reading fails, wait a little and try again
    delay(100);
    return;
  }

  // Numbers 2 and 3 are the "3D-compensated heading (Gyro3D)". This is the one we recommend most
  // The two numbers make one number that "can be negative": the first is the high part, the second is the low part
  int16_t heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);

  // Change it to 0 to 359 degrees (like a clock: after one turn it goes back to 0)
  int angle360 = ((heading % 360) + 360) % 360;

  Serial.print(F("Total angle: "));
  Serial.print(heading);
  Serial.print(F(" deg  |  0-359 style: "));
  Serial.print(angle360);
  Serial.println(F(" deg"));

  delay(200);                            // 👉 You can change this: print every 0.2 seconds
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
Ready! Turn the sensor slowly and watch. Type z and press Enter to reset to 0.
Total angle: 0 deg  |  0-359 style: 0 deg
Total angle: 45 deg  |  0-359 style: 45 deg
Total angle: 370 deg  |  0-359 style: 10 deg
Total angle: -30 deg  |  0-359 style: 330 deg
--- Reset! This direction is now 0 degrees ---
Total angle: 0 deg  |  0-359 style: 0 deg
```

**What does the angle number mean?**

| How you turn it (seen from above) | Total angle |
| :--- | :--- |
| Clockwise (turn right) | Gets bigger, for example 0 → 90 |
| Counterclockwise (turn left) | Gets smaller and can go below 0, for example 0 → -90 |
| Two turns to the right | 720 (**it keeps adding up and does not jump back to 0**) |

> [!TIP]
> **Which style is better?** When a robot must "turn 90 degrees", the **total angle** is easiest (target = now + 90). When you want to know "which way the car is facing", use the **0-359 style**, like a clock. The formula in the code is `((angle % 360) + 360) % 360`.

---

<h2 id="ex3">3️⃣ Spirit level</h2>

**What it does:** Turns the sensor into a spirit level. When it is level, the built-in light on the Arduino board (the one marked **L**) turns on. When it is tilted, the computer tells you which side is lower and by how many degrees. How it works: gravity always pulls down, and the "accelerometer" inside the sensor can feel which way this pull goes.

**You need:** A book. Put the sensor on the book, then slowly lift one side of the book.

<a href="/examples/imu/arduino-en/imu_03_level/imu_03_level.ino" download data-ignore="true">📄 Download imu_03_level.ino</a>

<!-- ino: examples/imu/arduino-en/imu_03_level/imu_03_level.ino -->
```cpp
/*
 * IMU Gyro x Arduino  Example 3: Spirit level
 * ------------------------------------------------------------
 * What does this example do?
 *   It turns the sensor into a "spirit level":
 *     Sensor is level  -> the built-in light on the Arduino board (the one marked L) turns on
 *     Sensor is tilted -> the light turns off, and the computer tells you which side is lower and by how many degrees
 *   How it works: gravity always pulls down. The "accelerometer" inside the sensor
 *   can feel which way this pull goes, so it knows if it is level or not.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. We use the light built into the board.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put the sensor on a book, then slowly lift one side of the book and watch the numbers change.
 *
 * Good to know:
 *   The sensor can lie flat, stand up, or lie on its side. The program finds which face is up by itself.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address
const uint8_t LED = LED_BUILTIN;         // The light built into the board (pin 13 on UNO)

const float LEVEL_DEG = 2.0;             // 👉 You can change this: less than 2 degrees of tilt counts as "level"

const char AXIS_NAME[3] = {'X', 'Y', 'Z'};   // The names of the three directions

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // Set the light pin to "output" so we can turn it on or off
  mbcBegin();
  delay(500);

  Serial.println(F("Put the sensor on the table and do not move it..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}
  }
  Serial.println(F("Ready! Put the sensor on a book and lift one side of the book."));
}

void loop() {
  // ---- 1. Read the acceleration: box 0x23, 6 numbers (2 each for X, Y, Z) ----
  uint8_t d[6];
  if (!mbcRead(0x23, d, 6)) {            // If reading fails, wait a little and try again
    delay(100);
    return;
  }

  // Make 3 numbers. The unit is mg (1000 mg = 1 times gravity)
  // When it lies still, the direction pointing "up" reads about +1000
  int16_t a[3];
  for (uint8_t i = 0; i < 3; i++) {
    a[i] = (int16_t)(((uint16_t)d[2 * i] << 8) | d[2 * i + 1]);
  }

  // ---- 2. Find which direction points up (the one with the biggest "size") ----
  uint8_t up = 0;
  for (uint8_t i = 1; i < 3; i++) {
    if (abs(a[i]) > abs(a[up])) up = i;
  }

  // ---- 3. See how many degrees the other two directions are tilted ----
  bool isLevel = true;                   // First, guess that it is level
  for (uint8_t i = 0; i < 3; i++) {
    if (i == up) continue;               // Skip the direction that points up

    // Use "trigonometry" to change the force into an angle (atan2 is built into Arduino)
    float deg = atan2((float)a[i], (float)abs(a[up])) * 180.0 / PI;

    Serial.print(AXIS_NAME[i]);
    Serial.print(F(" axis: "));
    if (fabs(deg) <= LEVEL_DEG) {
      Serial.print(F("level                            "));
    } else {
      isLevel = false;
      // Negative number -> the "+" side is lower; positive number -> the "-" side is lower
      Serial.print(F("the "));
      Serial.print(deg < 0 ? '+' : '-');
      Serial.print(AXIS_NAME[i]);
      Serial.print(F(" side is lower by "));
      Serial.print(fabs(deg), 1);        // Show 1 digit after the decimal point
      Serial.print(F(" deg  "));
    }
  }

  // ---- 4. Turn on the light if it is level ----
  if (isLevel) {
    digitalWrite(LED, HIGH);
    Serial.println(F("-> Level!"));
  } else {
    digitalWrite(LED, LOW);
    Serial.println(F("-> Not level yet"));
  }

  delay(200);                            // 👉 You can change this: check every 0.2 seconds
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
X axis: level                            Y axis: level                            -> Level!
X axis: the +X side is lower by 5.3 deg  Y axis: level                            -> Not level yet
X axis: the +X side is lower by 12.8 deg  Y axis: the -Y side is lower by 3.1 deg  -> Not level yet
```

> [!TIP]
> **Try this: find the X and Y of the sensor.** Press one side of the sensor down and see if the Serial Monitor says "the +X side is lower" or "the -Y side is lower". Mark it with a pen next to the sensor. Later, when you put the sensor on a car, you will know which way is the front.

---

<h2 id="ex4">4️⃣ 90-degree turn alert</h2>

**What it does:** Turn the sensor on the table. Each time it **turns 90 more degrees** (1/4 of a circle), the built-in light blinks once, and the computer tells you how many 1/4 turns it has made. Robot contests often need a "90-degree turn", so first feel how big 90 degrees is, by hand.

<a href="/examples/imu/arduino-en/imu_04_quarter_turn/imu_04_quarter_turn.ino" download data-ignore="true">📄 Download imu_04_quarter_turn.ino</a>

<!-- ino: examples/imu/arduino-en/imu_04_quarter_turn/imu_04_quarter_turn.ino -->
```cpp
/*
 * IMU Gyro x Arduino  Example 4: 90-degree turn alert
 * ------------------------------------------------------------
 * What does this example do?
 *   Turn the sensor on the table. Each time it turns 90 more degrees (1/4 of a circle),
 *   the built-in light on the Arduino board (the one marked L) blinks once,
 *   and the computer tells you "Turned right 90 degrees! Now at 1/4 turn".
 *   Robot contests often need a "90-degree turn". This example lets you feel how big 90 degrees is, by hand.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. We use the light built into the board.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put the sensor flat on the table and turn it slowly, like a steering wheel.
 *   Type z and press Enter to start again from 0.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address
const uint8_t LED = LED_BUILTIN;         // The light built into the board (pin 13 on UNO)

const int STEP_DEG = 90;                 // 👉 You can change this: alert after every this many degrees (for example 45 or 180)

long quarter = 0;                        // How many 90-degree steps we have turned (right is +, left is -)

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  Serial.println(F("Put the sensor on the table and do not move it..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}
  }
  zeroHeading();                         // Start counting from 0 degrees
  Serial.println(F("Ready! Turn the sensor slowly. The light blinks each time it turns 90 degrees."));
}

void loop() {
  // ---- 1. Start again from 0 when we get z ----
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'z' || c == 'Z') {
      zeroHeading();
      Serial.println(F("--- Reset! Counting again from 0 ---"));
    }
  }

  // ---- 2. Read the heading (box 0x11, 6 numbers), use numbers 2 and 3 (Gyro3D) ----
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {
    delay(100);
    return;
  }
  long heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);   // Turn right = bigger, turn left = smaller

  // ---- 3. Did it turn "one more" full 90 degrees? ----
  // It must turn a full 90 degrees "more" from the last 90-degree spot. Wobbling near the line will not keep blinking
  if (heading >= (quarter + 1) * STEP_DEG) {
    quarter = quarter + 1;
    Serial.print(F("Turned right "));
    Serial.print(STEP_DEG);
    Serial.print(F(" degrees! "));
    report();
  } else if (heading <= (quarter - 1) * STEP_DEG) {
    quarter = quarter - 1;
    Serial.print(F("Turned left "));
    Serial.print(STEP_DEG);
    Serial.print(F(" degrees! "));
    report();
  }

  delay(20);
}

// Show which 1/4 turn we are at now, and blink the light once
void report() {
  Serial.print(F("Now at "));
  Serial.print(quarter);
  Serial.print(F("/4 turn ("));
  Serial.print(quarter * STEP_DEG);
  Serial.print(F(" degrees)"));
  if (quarter != 0 && quarter % 4 == 0) {
    Serial.print(F("  That is exactly one full circle!"));
  }
  Serial.println();

  digitalWrite(LED, HIGH);               // Blink once
  delay(150);
  digitalWrite(LED, LOW);
}

// Reset the heading to 0
void zeroHeading() {
  mbcWrite(0x10, 0x01);                  // Write 0x01 into box 0x10 = "reset heading"
  delay(50);                             // Wait a little after the reset (at least 30 ms) before reading
  quarter = 0;
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
Ready! Turn the sensor slowly. The light blinks each time it turns 90 degrees.
Turned right 90 degrees! Now at 1/4 turn (90 degrees)
Turned right 90 degrees! Now at 2/4 turn (180 degrees)
Turned right 90 degrees! Now at 3/4 turn (270 degrees)
Turned right 90 degrees! Now at 4/4 turn (360 degrees)  That is exactly one full circle!
Turned left 90 degrees! Now at 3/4 turn (270 degrees)
```

> [!TIP]
> **Try this:** change `STEP_DEG` to `45`, and it alerts you "every 45 degrees".

---

<h2 id="ex5">5️⃣ Shake detector</h2>

**What it does:** Pick up the sensor and shake it hard (or twist it quickly). The built-in light turns on, the computer counts "how many times you shook it", and tells you how fast it just turned. The gyro measures the **turning speed**. The faster it turns, the bigger the number. When the number goes over a limit, it counts as one shake.

**You need:** Push the jumper wires in firmly so they do not come loose when you shake.

<a href="/examples/imu/arduino-en/imu_05_shake/imu_05_shake.ino" download data-ignore="true">📄 Download imu_05_shake.ino</a>

<!-- ino: examples/imu/arduino-en/imu_05_shake/imu_05_shake.ino -->
```cpp
/*
 * IMU Gyro x Arduino  Example 5: Shake detector
 * ------------------------------------------------------------
 * What does this example do?
 *   Pick up the sensor and shake it hard (or twist it quickly).
 *   The built-in light on the Arduino board (the one marked L) turns on,
 *   the computer counts "how many times you shook it", and tells you how fast it just turned.
 *   How it works: the gyro measures "how fast it is turning". The faster it turns, the bigger the number.
 *   When the number goes over a limit, we count it as "one shake".
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. We use the light built into the board.
 *   Push the jumper wires in firmly so they do not come loose when you shake.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Wait until you see "Ready", then pick up the sensor and shake it.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address
const uint8_t LED = LED_BUILTIN;         // The light built into the board (pin 13 on UNO)

const float SHAKE_SPEED = 300.0;         // 👉 You can change this: turning faster than "300 degrees per second" counts as one shake
                                         //    (smaller = easier to trigger, bigger = you must shake harder)

bool shaking = false;                    // Are we in the middle of "one shake" now?
int  shakeCount = 0;                     // How many shakes in total
float peak = 0;                          // The fastest speed during this shake

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  Serial.println(F("Put the sensor on the table and do not move it..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}
  }
  Serial.println(F("Ready! Pick up the sensor and shake it."));
}

void loop() {
  // ---- 1. Read the turning speed: box 0x22, 6 numbers (2 each for X, Y, Z) ----
  uint8_t d[6];
  if (!mbcRead(0x22, d, 6)) {            // If reading fails, wait a little and try again
    delay(100);
    return;
  }

  // Make 3 numbers. The unit is "0.1 degree per second", so divide by 10 to get "degrees per second"
  float g[3];
  for (uint8_t i = 0; i < 3; i++) {
    int16_t raw = (int16_t)(((uint16_t)d[2 * i] << 8) | d[2 * i + 1]);
    g[i] = raw / 10.0;
  }

  // The "turning speed" of all three directions together (always positive, no matter which way it turns)
  float speed = sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

  // ---- 2. Was that "one shake"? ----
  if (!shaking && speed > SHAKE_SPEED) {
    shaking = true;                      // The shake has started
    peak = speed;
    digitalWrite(LED, HIGH);             // Light on
  }
  if (shaking) {
    if (speed > peak) peak = speed;      // Remember the fastest speed of this shake
    if (speed < SHAKE_SPEED / 2) {       // It slowed down (below half the limit) -> this shake is over
      shaking = false;
      shakeCount = shakeCount + 1;
      digitalWrite(LED, LOW);
      Serial.print(F("Shake! Number "));
      Serial.print(shakeCount);
      Serial.print(F(" (fastest: "));
      Serial.print((int)peak);
      Serial.println(F(" degrees per second)"));
    }
  }

  delay(10);                             // Check very often so we do not miss a very fast shake
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
Ready! Pick up the sensor and shake it.
Shake! Number 1 (fastest: 512 degrees per second)
Shake! Number 2 (fastest: 634 degrees per second)
Shake! Number 3 (fastest: 401 degrees per second)
```

> [!TIP]
> **Try this:** change `SHAKE_SPEED` to `150`, and a small twist triggers it. Change it to `600`, and you must shake very hard.

---

<h2 id="ex6">6️⃣ Electronic dice</h2>

**What it does:** Uses the sensor as a dice. Its 6 faces (+X, +Y, +Z, -X, -Y, -Z) mean 1 to 6 dots. Flip it over, put it down and wait until it is still. The computer says "You rolled a 3!", and the built-in light blinks 3 times.

<a href="/examples/imu/arduino-en/imu_06_dice/imu_06_dice.ino" download data-ignore="true">📄 Download imu_06_dice.ino</a>

<!-- ino: examples/imu/arduino-en/imu_06_dice/imu_06_dice.ino -->
```cpp
/*
 * IMU Gyro x Arduino  Example 6: Electronic dice (which face is up?)
 * ------------------------------------------------------------
 * What does this example do?
 *   It uses the sensor as a dice: the sensor has 6 faces (+X, +Y, +Z, -X, -Y, -Z),
 *   and each face means 1 to 6 dots. Flip the sensor over and put it down.
 *   When it is still, the computer says "You rolled a 3!",
 *   and the built-in light on the Arduino board (the one marked L) blinks 3 times.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. We use the light built into the board.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Pick up the sensor, flip it, put it back on the table and do not touch it. You will see the dots in about 0.5 seconds.
 *   (Do not pull the jumper wires too tight, so they do not come loose when you flip it)
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address
const uint8_t LED = LED_BUILTIN;         // The light built into the board (pin 13 on UNO)

// How many dots each face means (like a real dice, two opposite faces add up to 7)
//                         +X  +Y  +Z  -X  -Y  -Z
const uint8_t DOTS[6] = {  2,  3,  1,  5,  4,  6 };   // 👉 You can change this: use the dots you like

bool rolling = true;                     // Is the dice still moving?

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  Serial.println(F("Put the sensor on the table and do not move it..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}
  }
  Serial.println(F("Ready! Flip the sensor over, put it down, and wait until it is still."));
}

void loop() {
  // ---- 1. Read the status: box 0x31, 9 numbers ----
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {           // If reading fails, wait a little and try again
    delay(100);
    return;
  }
  uint8_t flags  = st[0];                // A row of "switches"
  uint8_t upFace = st[4];                // Which face is up: 0 to 5
  bool still = flags & (1u << 1);        // Switch 1 = not moving (it is still)

  // ---- 2. It started moving -> the dice is rolling ----
  if (!still) {
    if (!rolling) {                      // It just started moving; say it only once
      rolling = true;
      Serial.println(F("The dice is rolling..."));
    }
  }

  // ---- 3. It is still -> show the dots ----
  if (still && rolling && upFace < 6) {
    rolling = false;
    uint8_t n = DOTS[upFace];
    Serial.print(F("It stopped! You rolled a "));
    Serial.print(n);
    Serial.print(F(" (face up: "));
    printFace(upFace);
    Serial.println(F(")"));

    for (uint8_t i = 0; i < n; i++) {    // Blink once for each dot
      digitalWrite(LED, HIGH); delay(200);
      digitalWrite(LED, LOW);  delay(200);
    }
  }

  delay(50);
}

// Print the "face pointing up" number as words you can read
void printFace(uint8_t f) {
  switch (f) {
    case 0:  Serial.print(F("+X")); break;
    case 1:  Serial.print(F("+Y")); break;
    case 2:  Serial.print(F("+Z")); break;
    case 3:  Serial.print(F("-X")); break;
    case 4:  Serial.print(F("-Y")); break;
    case 5:  Serial.print(F("-Z")); break;
    default: Serial.print(F("?"));  break;
  }
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
Ready! Flip the sensor over, put it down, and wait until it is still.
It stopped! You rolled a 1 (face up: +Z)
The dice is rolling...
It stopped! You rolled a 4 (face up: -Y)
```

| Face up | +X | +Y | +Z | -X | -Y | -Z |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Dots | 2 | 3 | 1 | 5 | 4 | 6 |

> [!TIP]
> The sensor must **stay still for about 0.5 seconds** before it shows the dots. Just like a real dice, it does not count while it is still rolling.

---

<h2 id="ex7">7️⃣ Car turns 90 degrees in place (advanced)</h2>

**What it does:** Makes a two-wheel car turn right 90 degrees in place, stop for 1 second, turn 90 degrees again... After 4 turns (one full circle) it stops. While the car turns, it keeps checking the gyro: far from the target, it turns faster; almost there, it slows down; arrived, it stops. If you turn only by "running the motors for some seconds", a weak battery or a slippery floor makes the turn wrong. With the gyro, it is much more accurate.

**You also need:** A two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), batteries.

**Motor wiring:**

| L298N | Arduino UNO / Nano | ESP32 | Controls |
| :---: | :---: | :---: | :--- |
| ENA | 5 | GPIO 25 | Left wheel speed |
| IN1 / IN2 | 7 / 8 | GPIO 26 / 27 | Left wheel direction |
| ENB | 6 | GPIO 14 | Right wheel speed |
| IN3 / IN4 | 9 / 10 | GPIO 32 / 33 | Right wheel direction |
| GND | GND | GND | **Must share GND with the Arduino** |

**Before the first run:**

1. Run [Example 2](#ex2) first and check that the angle gets bigger when you turn right.
2. **Fix** the gyro onto the car with screws or bricks (flat, standing up, or on its side are all OK, but it must not wobble).
3. Lift the car up (wheels off the ground) and upload. Check that both wheels turn **forward**. If a wheel turns the wrong way, change `LEFT_REVERSE` or `RIGHT_REVERSE` to `true`.
4. Put it on the floor and **do not touch it**. Wait for "Ready". It starts turning 3 seconds later.

<a href="/examples/imu/arduino-en/imu_07_turn90_car/imu_07_turn90_car.ino" download data-ignore="true">📄 Download imu_07_turn90_car.ino</a>

<!-- ino: examples/imu/arduino-en/imu_07_turn90_car/imu_07_turn90_car.ino -->
```cpp
/*
 * IMU Gyro x Arduino  Example 7 (advanced): Car turns 90 degrees in place
 * ------------------------------------------------------------
 * What does this example do?
 *   It makes a two-wheel car "turn right 90 degrees in place", stop for 1 second, turn 90 degrees again...
 *   After 4 turns (one full circle) it stops.
 *   While the car turns, it keeps checking the gyro angle:
 *     Still far from the target -> turn faster
 *     Almost there              -> turn slowly
 *     Arrived                   -> stop the car
 *   This way of "watching the sensor and correcting at the same time" is called "feedback control".
 *   If you turn only by "running the motors for some seconds", a weak battery or a slippery floor
 *   makes the turn wrong. With the gyro, it is much more accurate.
 *
 * You need:
 *   A two-wheel car, an L298N motor driver board (or a TB6612 or other driver wired the same way), batteries.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   (For ESP32 use: ENA -> 25, IN1 -> 26, IN2 -> 27, ENB -> 14, IN3 -> 32, IN4 -> 33)
 *   The L298N GND must be connected to the Arduino GND!
 *   Fix the gyro onto the car with screws or bricks (flat, standing up, or on its side are all OK, but it must not wobble).
 *
 * Before the first run:
 *   1. Run "Example 2" first and check that the angle number gets bigger when you turn right.
 *   2. Lift the car up (wheels off the ground) and upload. Check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE below to true.
 *   3. At power-on, keep the car still. It only starts after "Ready".
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address

// ---- Motor pins ----
#if defined(ESP32)
// ESP32 pins 6 to 11 are connected to the memory chip on the board and cannot be used, so we use these pins
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 You can change these settings ----
const int   TURN_ANGLE = 90;             // Degrees for each turn: positive = turn right, negative = turn left
const int   TURN_TIMES = 4;              // How many turns in total (4 x 90 degrees = one full circle)
const int   MAX_SPEED  = 150;            // Speed when far from the target: 0 to 255
const int   MIN_SPEED  = 70;             // The slowest speed when almost there (too small and the wheels will not move)
const float KP         = 3.0;            // How much speed to add for each 1 degree left: change this if it does not stop in the right place
const int   TOLERANCE  = 2;              // Within 2 degrees counts as "arrived"
const bool  LEFT_REVERSE  = false;       // Change to true if the left wheel turns the wrong way
const bool  RIGHT_REVERSE = false;       // Change to true if the right wheel turns the wrong way

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);

  Serial.println(F("Do not move the car. Getting the gyro ready..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}                      // The motors are already stopped, so just stop here
  }
  mbcWrite(0x10, 0x01);                  // Reset the heading: the direction the car faces now = 0 degrees
  delay(50);                             // Wait a little after the reset before reading

  Serial.println(F("Ready! Turning starts in 3 seconds."));
  delay(3000);

  long target = 0;                       // Target angle
  for (int i = 1; i <= TURN_TIMES; i++) {
    target = target + TURN_ANGLE;        // Turn 90 degrees more each time: 90, 180, 270, 360
    Serial.print(F("Turn "));
    Serial.print(i);
    Serial.print(F(": going to "));
    Serial.print(target);
    Serial.print(F(" degrees... "));

    if (!turnTo(target)) {               // The turn failed (the car is already stopped for safety)
      while (true) {}
    }
    delay(1000);                         // Stop for 1 second before the next turn
  }
  Serial.println(F("Done! Press the Arduino RESET button to run it again."));
}

void loop() {
  // Everything was done in setup, so nothing happens here (the car stays stopped)
}

// Turn in place to target degrees. Returns true if it worked; false if it could not get there (the car is stopped)
bool turnTo(long target) {
  unsigned long start = millis();
  while (true) {
    // ---- 1. Read the heading (box 0x11, use numbers 2 and 3 = Gyro3D) ----
    uint8_t d[6];
    if (!mbcRead(0x11, d, 6)) {          // Cannot read the sensor -> stop the car first, to be safe
      drive(0, 0);
      if (millis() - start > 5000) {
        Serial.println(F("Cannot read the gyro. Car stopped! Please check the wiring."));
        return false;
      }
      delay(20);
      continue;
    }
    long heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);   // Turn right = bigger

    // ---- 2. How many degrees are left? ----
    long error = target - heading;       // Positive = still need to turn right, negative = turned too far

    if (abs(error) <= TOLERANCE) {       // Arrived!
      drive(0, 0);
      delay(300);                        // Wait until the car has fully stopped
      mbcRead(0x11, d, 6);               // Check once more where it stopped
      heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);
      Serial.print(F("stopped at "));
      Serial.print(heading);
      Serial.println(F(" degrees"));
      return true;
    }

    // ---- 3. Safety checks ----
    if (abs(error) > abs(TURN_ANGLE) + 45) {   // Getting farther and farther -> wrong direction
      drive(0, 0);
      Serial.println(F("Getting farther away. Car stopped! A wheel may turn the wrong way: change LEFT_REVERSE / RIGHT_REVERSE."));
      return false;
    }
    if (millis() - start > 5000) {       // Still not there after 5 seconds
      drive(0, 0);
      Serial.println(F("Could not get there in 5 seconds. Car stopped! Check the batteries, or make MIN_SPEED bigger."));
      return false;
    }

    // ---- 4. The farther away, the faster it turns; slow down when almost there ----
    int speed = constrain((int)(KP * abs(error)), MIN_SPEED, MAX_SPEED);
    if (error > 0) drive(speed, -speed);    // Left wheel forward, right wheel backward -> turn right in place (angle gets bigger)
    else           drive(-speed, speed);    // Turned too far -> turn left in place to come back
    delay(10);
  }
}

// Control the two wheels: speed -255 (full speed backward) to 255 (full speed forward)
void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);   // Keep the speed inside the allowed range
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
Do not move the car. Getting the gyro ready...
Ready! Turning starts in 3 seconds.
Turn 1: going to 90 degrees... stopped at 91 degrees
Turn 2: going to 180 degrees... stopped at 180 degrees
Turn 3: going to 270 degrees... stopped at 272 degrees
Turn 4: going to 360 degrees... stopped at 361 degrees
Done! Press the Arduino RESET button to run it again.
```

**How to tune it so the car turns more accurately:**

| What the car does | What to change |
| :--- | :--- |
| It always turns a few degrees too far | Make `MAX_SPEED` smaller, or make `KP` smaller (for example 3.0 → 2.0) |
| It stops moving when almost there, then says "Could not get there in 5 seconds" | Make `MIN_SPEED` bigger (for example 70 → 90) |
| Right at the start it says "Getting farther away. Car stopped!" | One wheel turns the wrong way. Change `LEFT_REVERSE` or `RIGHT_REVERSE` |
| You want it to turn left | Change `TURN_ANGLE` to `-90` |

---

## ❓ FAQ

**Q: It keeps saying "Sensor not found"?**

1. Check that SDA and SCL are not swapped (UNO: SDA → A4, SCL → A5).
2. Check that VCC and GND are plugged in firmly. The red light on the sensor should blink once per second, which means it is working.
3. Make sure it is the **Universal I2C edition**. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.

**Q: It says "it keeps feeling shaky" and never gets to "Ready"?**

When the gyro powers on, it must **stay completely still for about 0.5 seconds** to measure its reading at rest. After that, it can calculate correctly. Put the sensor on the table (not in your hand), make sure no motor or fan is shaking the table, then press the Arduino RESET button. The green light on the sensor: **slow blinking = not ready yet, off = ready, fast blinking = mounted too tilted** (more than about 45 degrees).

**Q: Why is the angle not -180 to 180, and not 0 to 359?**

The angle of this gyro **keeps adding up** (two turns to the right = 720), the same as the LEGO EV3 gyro. This makes turns easier to calculate. If you want 0 to 359, use `((angle % 360) + 360) % 360` (Example 2 does this). If the total goes over ±32767 (about 91 turns), the number wraps around, so remember to reset it to 0 before each contest run.

**Q: I pressed the Arduino RESET button. Why didn't the angle go back to 0?**

Pressing RESET (or opening the Serial Monitor) only restarts the Arduino. The gyro does not restart, so it still remembers the old angle. To reset it to 0, write `mbcWrite(0x10, 0x01)` (in Example 2, type `z`). After that, **wait at least 30 ms** before reading the angle. The gyro also resets to 0 once by itself, right when it powers on and captures its "mounting direction".

**Q: Box 0x11 has 3 kinds of angle. Which one should I use?**

Use **Gyro3D (3D-compensated heading)**, numbers 2 and 3. All the examples use this one. It is correct no matter how the sensor is mounted (flat, standing up, or on its side). Numbers 4 and 5 are the angle "corrected by the magnetometer". It drifts less over long runs, but it is **not a compass**: it is also 0 at power-on or after a reset, and it is wrong near motors or steel fields.

**Q: Do I need to calibrate it?**

Usually **no**. The gyro corrects itself every time it is still. The accelerometer uses standard values, which are accurate enough for the spirit level and the dice. Please do not press the two calibration buttons on the sensor. If you press one by accident (the light next to it stays on), press it again to end it. A calibration that is not finished is not saved.

**Q: Can I connect it to the same SDA/SCL with other MBC sensors?**

Yes, as long as their addresses are different (for example, Pathfinder 8-Way is `0x16` and the gyro is `0x6A`). But the **Arduino Nano 33 IoT and Nano RP2040 Connect** boards already have a chip with the same address `0x6A`. The addresses "clash", so these two boards cannot connect to this gyro directly.

**Q: I want to know what data each box holds?**

See the "Universal I2C Registers" table on the [IMU Gyro Home](/en/sensors/imu/index.md) page, and the [MBC Universal I2C Protocol](/en/i2c-protocol.md) (advanced information for people who can write code).
