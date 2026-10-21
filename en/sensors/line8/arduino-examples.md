<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 Pathfinder 8-Way × Arduino Examples

[🔙 Back to Pathfinder 8-Way Home](/en/sensors/line8/index.md)　|　[📡 I2C Register Guide (advanced)](/en/sensors/line8/arduino-i2c.md)

This page has **6 examples**, from "is it connected?" all the way to a "line-following car". You can copy and upload each example as it is, and every line of the code has an English comment. **Never written code before? No problem.** Just do them one by one, in order.

> [!TIP]
> First time using Arduino? Read [🔰 First Time with Arduino: Start from Zero](/en/sensors/arduino-start.md) first. In 15 minutes you learn how to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/line8/arduino-en/line8_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
</div>

---

## 🔌 Wiring (the same for every example)

| Sensor | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: |
| VCC | 5V | 5V (VIN) |
| GND | GND | GND |
| SDA | A4 | GPIO 21 |
| SCL | A5 | GPIO 22 |

> [!IMPORTANT]
> - Use the **Universal I2C edition** of the Pathfinder. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
> - When the sensor is mounted on the robot, it works best with **its bottom about 0.5 ~ 1 cm above the floor**.
> - Unplug the USB cable before you connect the wires.

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the sensor](#ex1) | Check that the wiring is correct | Nothing |
| 2 | [Where is the line?](#ex2) | Read "the line position" | A sheet of white paper + black electrical tape |
| 3 | [What do the 8 eyes see?](#ex3) | Read each light sensor on its own | Same as above |
| 4 | [One-key calibration](#ex4) | Make the sensor learn the black and white of your field | Same as above |
| 5 | [Crossroad detector light](#ex5) | Count crossroads, and tell which side the line was lost on | Paper with crossroads on it |
| 6 | [Line-following car (advanced)](#ex6) | Make a car follow the line by itself | Two-wheel car, L298N motor driver board |

> [!TIP]
> **Suggested order: 1 → 4 → 2 → 3 → 5 → 6.** First check the wiring (1) and calibrate the field (4). Then the readings in the other examples will be accurate.

---

<h2 id="ex1">1️⃣ Say hello to the sensor</h2>

**What it does:** Checks that the sensor is connected. Run this one first every time you connect a new sensor.

**Steps:**
1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.

<a href="/examples/line8/arduino-en/line8_01_hello/line8_01_hello.ino" download data-ignore="true">📄 Download line8_01_hello.ino</a>

<!-- ino: examples/line8/arduino-en/line8_01_hello/line8_01_hello.ino -->
```cpp
/*
 * Pathfinder 8-Way x Arduino  Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   It checks that the sensor is connected. If it is, the computer shows
 *   the sensor's name and version.
 *   Run this example first every time you connect a new sensor.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the sensor

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's "address" (like a house number). It is always 0x16

void setup() {
  Serial.begin(115200);                  // Open the line to the computer, speed 115200
  mbcBegin();                            // Get I2C ready (the helpers are at the bottom of this file)
  delay(500);                            // Give the sensor 0.5 seconds to power on

  Serial.println(F("=== Pathfinder 8-Way: Say hello to the sensor ==="));

  // Wait for the sensor to say "I'm ready", up to 2 seconds
  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found! Please check:"));
    Serial.println(F("  1. SDA goes to A4 and SCL goes to A5. Are they swapped?"));
    Serial.println(F("  2. VCC goes to 5V and GND goes to GND. Are they plugged in firmly?"));
    Serial.println(F("  3. Is your sensor the Universal I2C edition? (The SPIKE and EV3 editions do not work with Arduino.)"));
    return;                              // Sensor not found: stop here
  }

  // Read the "version data": box 0x01 of the sensor, 17 numbers in total
  uint8_t info[17];
  mbcRead(0x01, info, 17);

  // Numbers 9 ~ 16 are the sensor's name (English letters)
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("Found it! Sensor name: "));
  Serial.println(name);                  // It should show LINE8

  Serial.print(F("Firmware version: "));
  Serial.print(info[2]);                 // Major version
  Serial.print('.');
  Serial.println(info[3]);               // Minor version

  Serial.print(F("This sensor has "));
  Serial.print(info[7]);                 // Pathfinder 8-Way = 8
  Serial.println(F(" light sensors"));

  Serial.println(F("Wiring is correct. You can try the other examples now!"));
}

void loop() {
  // This example only checks once at power-on, so loop does nothing
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
=== Pathfinder 8-Way: Say hello to the sensor ===
Found it! Sensor name: LINE8
Firmware version: 1.0
This sensor has 8 light sensors
Wiring is correct. You can try the other examples now!
```

> [!NOTE]
> **What are the "Communication helpers"?** At the bottom of every example there is the same piece of code. It "talks" to the sensor. You don't need to change it or understand it. Just remember:
> - `mbcRead(box number, where to put it, how many)`: read data from the sensor
> - `mbcWrite(box number, number)`: write a setting into the sensor
>
> Think of the sensor as a row of numbered "boxes" (programmers call them registers). Each box holds different data. For example, box `0x11` holds "where the line is".

---

<h2 id="ex2">2️⃣ Where is the line?</h2>

**What it does:** Move the sensor left and right. The computer draws a small ruler, marks the line position with `#`, and tells you which way to turn.

> [!NOTE]
> **What do "left" and "right" mean?** The sensor is mounted on the robot (sensing side down), **viewed from behind the robot, looking forward**. All Pathfinder examples use this direction.

**You need:** A strip of black electrical tape (about 2 cm wide) on white paper as the line.

<a href="/examples/line8/arduino-en/line8_02_where_is_line/line8_02_where_is_line.ino" download data-ignore="true">📄 Download line8_02_where_is_line.ino</a>

<!-- ino: examples/line8/arduino-en/line8_02_where_is_line/line8_02_where_is_line.ino -->
```cpp
/*
 * Pathfinder 8-Way x Arduino  Example 2: Where is the line?
 * ------------------------------------------------------------
 * What does this example do?
 *   Hold the sensor over a black line on white paper and move it left and right.
 *   The computer tells you "the line is on the left, in the middle, or on the right",
 *   and draws a small ruler that marks where the line is.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Good to know:
 *   The first time you use it, or on a new competition field, run "Example 4: One-key calibration" first. The readings will be more accurate.
 *   "Left" and "right" here mean: the sensor is mounted on the robot (sensing side down),
 *   viewed from behind the robot, looking forward.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address

// 👉 You can change this: 0 = follow a black line (black line on white), 1 = follow a white line (white line on black)
const uint8_t LINE_COLOR = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}                      // Stop here
  }

  mbcWrite(0x10, LINE_COLOR);            // Tell the sensor to look for a black line or a white line
  Serial.println(F("Ready! Put the sensor over the line and move it left and right."));
}

void loop() {
  // Read 7 numbers at once: the sensor puts all the "line results" starting at box 0x11
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {
    Serial.println(F("Read failed. Check that no wire is loose."));
    delay(500);
    return;
  }

  int8_t  position = (int8_t)d[0];       // Line position: negative = line on the left, positive = line on the right, 0 = right in the middle
  uint8_t groups   = d[3];               // How many lines it sees: 0 = no line

  if (groups == 0) {
    Serial.println(F("|         |  No line (the line is lost)"));
  } else {
    // Draw a small ruler: -8 on the far left, +8 on the far right, # marks where the line is
    Serial.print('|');
    for (int8_t p = -8; p <= 8; p += 2) {
      // Each cell is 2 units wide. Find the cell the line is in
      if (position >= p && position <= p + 1) Serial.print('#');
      else Serial.print(' ');
    }
    Serial.print(F("|  Position "));
    Serial.print(position);

    if (position > 1)       Serial.println(F("  → The line is on the right, turn right"));
    else if (position < -1) Serial.println(F("  → The line is on the left, turn left"));
    else                    Serial.println(F("  → The line is in the middle, go straight!"));
  }

  delay(200);                            // 👉 You can change this: look every 0.2 seconds. Smaller number = faster updates
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

**You will see:** (moving the black line slowly from left to right)

```text
| #       |  Position -5  → The line is on the left, turn left
|   #     |  Position -1  → The line is in the middle, go straight!
|     #   |  Position 3  → The line is on the right, turn right
|         |  No line (the line is lost)
```

**What does the position mean?**

| Position number | Meaning |
| :---: | :--- |
| 0 | The line is right in the middle |
| Negative (-1 ~ -7) | The line is on the **left**. Smaller = further left |
| Positive (+1 ~ +7) | The line is on the **right**. Bigger = further right |
| ±8 | The line just ran off the far left (-8) or the far right (+8) |

> [!TIP]
> **Try it:** Change `LINE_COLOR` to `1` and use white tape on black paper. Now it follows a white line.

---

<h2 id="ex3">3️⃣ What do the 8 eyes see?</h2>

**What it does:** Under the Pathfinder there are 8 light sensors, like 8 eyes in a row. This example draws what each eye sees: `●` = sees the line, `○` = sees the background. The row below shows each eye's **color score** (0 = very dark, 100 = very bright).

<a href="/examples/line8/arduino-en/line8_03_eight_eyes/line8_03_eight_eyes.ino" download data-ignore="true">📄 Download line8_03_eight_eyes.ino</a>

<!-- ino: examples/line8/arduino-en/line8_03_eight_eyes/line8_03_eight_eyes.ino -->
```cpp
/*
 * Pathfinder 8-Way x Arduino  Example 3: What do the 8 eyes see?
 * ------------------------------------------------------------
 * What does this example do?
 *   Under the Pathfinder there are 8 light sensors, like 8 eyes in a row.
 *   This example shows what each eye sees:
 *     ● = this eye sees the line      ○ = this eye sees the background
 *   Below that, it shows each eye's "color score": 0 = very dark, 100 = very bright.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   The printed order is the real order with the sensor on the robot, viewed from behind the robot,
 *   looking forward: the left eye is printed on the left.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address
const uint8_t EYES = 8;                  // The Pathfinder 8-Way has 8 eyes

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move the sensor over the line and see which eyes turn into ●"));
}

void loop() {
  // Box 0x16: 2 numbers. Together, each "bit" in them tells if one eye sees the line
  uint8_t b[2];
  // Box 0x50: 8 numbers, the color score of each eye, 0 ~ 100
  uint8_t score[EYES];

  if (!mbcRead(0x16, b, 2) || !mbcRead(0x50, score, EYES)) {
    Serial.println(F("Read failed. Check that no wire is loose."));
    delay(500);
    return;
  }

  uint16_t onLine = ((uint16_t)b[0] << 8) | b[1];  // Join the 2 numbers into one

  // Eye numbers (viewed from behind the robot, looking forward): eye 0 is on the far left, eye 7 on the far right.
  // So printing from eye 0 to eye 7 gives the same left and right as the real sensor.
  Serial.print(F("Left  "));
  for (int8_t i = 0; i < EYES; i++) {
    if (onLine & (1u << i)) Serial.print(F("  ● "));
    else                    Serial.print(F("  ○ "));
  }
  Serial.println(F("Right"));

  Serial.print(F("Score"));
  for (int8_t i = 0; i < EYES; i++) {
    Serial.print(' ');
    if (score[i] < 100) Serial.print(' ');   // For lining up: add spaces in front of numbers with fewer than 3 digits
    if (score[i] < 10)  Serial.print(' ');
    Serial.print(score[i]);
  }
  Serial.println();
  Serial.println();

  delay(300);                            // 👉 You can change this: look every 0.3 seconds
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

**You will see:** (the line a little left of the middle)

```text
Left    ○   ○   ●   ●   ○   ○   ○   ○ Right
Score 100  98  12   8  95 100 100  99
```

> [!TIP]
> **Try it:** Slowly lift the sensor higher and watch the scores change. When it is too high, the black and white scores get closer and closer. That is why the sensor must stay close to the floor.

---

<h2 id="ex4">4️⃣ One-key calibration</h2>

**What it does:** Makes the sensor learn the black and white of **this field**. Calibrate again on a new field or when the lights are different. The result is saved in the sensor, so it remembers even after you turn it off.

**Steps:**
1. Upload the program and open the Serial Monitor (115200).
2. Hold the sensor over the line, at the same height as on the robot (about 0.5 ~ 1 cm above the floor).
3. In the **input box at the top of the Serial Monitor, type `c` and press Enter**.
4. In the **next 5 seconds**, slowly move the sensor left and right so every eye sweeps over the black line and the white background.
5. When you see "Calibration done", you are finished.

<a href="/examples/line8/arduino-en/line8_04_calibrate/line8_04_calibrate.ino" download data-ignore="true">📄 Download line8_04_calibrate.ino</a>

<!-- ino: examples/line8/arduino-en/line8_04_calibrate/line8_04_calibrate.ino -->
```cpp
/*
 * Pathfinder 8-Way x Arduino  Example 4: One-key calibration
 * ------------------------------------------------------------
 * What does this example do?
 *   "Calibration" means the sensor learns what black and white look like on your competition field.
 *   On a new field, or when the lights are different, calibrate again. The result is saved in the sensor,
 *   so it remembers even after you turn it off.
 *
 * How to calibrate:
 *   1. Hold the sensor over the line (about 0.5 ~ 1 cm above the floor, the same height as on the robot).
 *   2. Type c in the input box at the top of the Serial Monitor and press Enter.
 *   3. In the next 5 seconds, slowly move the sensor left and right so every eye "sweeps" over the black line and the white background.
 *   4. When you see "Calibration done", you are finished.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Type c in the input box at the top and press Enter to start calibrating."));
}

void loop() {
  // Did someone type something in the Serial Monitor?
  if (Serial.available()) {
    char key = Serial.read();            // Read one letter
    if (key == 'c' || key == 'C') {
      calibrate();
    }
  }
}

// All the calibration steps are here
void calibrate() {
  // Box 0x21 has 2 numbers. The 2nd one is "how many times it has been calibrated"
  uint8_t before[2];
  if (!mbcRead(0x21, before, 2)) {
    Serial.println(F("Read failed. Check that no wire is loose."));
    return;
  }

  // Write 1 into box 0x20 = start the 5-second calibration
  mbcWrite(0x20, 1);
  Serial.println(F("Calibration started! In the next 5 seconds, move the sensor left and right over the black line and the white background..."));

  // Keep checking if the "calibration count" went up by 1. If it did, calibration is done
  unsigned long start = millis();
  while (millis() - start < 7000) {      // Wait up to 7 seconds
    delay(250);
    Serial.print('.');                   // Print a dot to show it is still waiting

    uint8_t now[2];
    if (mbcRead(0x21, now, 2) && now[1] != before[1]) {
      Serial.println();
      Serial.println(F("Calibration done! The result is saved in the sensor and stays there even after power off."));
      showResult();
      return;
    }
  }

  Serial.println();
  Serial.println(F("Waited too long. Calibration did not finish. Type c again and try once more."));
}

// Show the "white" and "black" that each eye has learned
void showResult() {
  uint8_t white[8], black[8];
  mbcRead(0x51, white, 8);               // Box 0x51: the white each eye has learned
  mbcRead(0x52, black, 8);               // Box 0x52: the black each eye has learned

  Serial.println(F("Eye (left → right)  White  Black  Gap"));
  for (int8_t i = 0; i < 8; i++) {      // Eye 0 is on the far left (viewed from behind the robot, looking forward)
    Serial.print(F("  Eye "));
    Serial.print(i);
    Serial.print(F("             "));
    Serial.print(white[i]);
    Serial.print(F("   "));
    Serial.print(black[i]);
    Serial.print(F("   "));
    Serial.println(white[i] - black[i]);
  }
  Serial.println(F("A bigger 'Gap' is better. A very small gap (less than 20) means that eye did not sweep over the black line. Please calibrate again."));
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

> [!WARNING]
> During calibration, **every eye must sweep over the black line**. If an eye at the edge misses it, that eye will always think the whole field is white, and line following will go wrong.

---

<h2 id="ex5">5️⃣ Crossroad detector light</h2>

**What it does:** Uses the small built-in light on the Arduino board (the one marked **L**) as a signal light: on when it sees the line, off when the line is lost. At a crossroad or a fork it **blinks fast 3 times** and counts "which crossroad this is". Competitions often say "go to the 3rd crossroad and turn right", and this example teaches you how to count crossroads.

**You need:** A straight line of black tape on white paper, with 2 ~ 3 short pieces across it as crossroads.

<a href="/examples/line8/arduino-en/line8_05_crossroad_led/line8_05_crossroad_led.ino" download data-ignore="true">📄 Download line8_05_crossroad_led.ino</a>

<!-- ino: examples/line8/arduino-en/line8_05_crossroad_led/line8_05_crossroad_led.ino -->
```cpp
/*
 * Pathfinder 8-Way x Arduino  Example 5: Crossroad detector light
 * ------------------------------------------------------------
 * What does this example do?
 *   It uses the small built-in light on the Arduino board (the one marked L) as a signal light:
 *     Sees the line             -> light on
 *     Line is lost              -> light off, and the computer tells you which side the line was lost on
 *     Reaches a crossroad/fork  -> light blinks fast 3 times, and counts "which crossroad this is"
 *   Competitions often say "go to the 3rd crossroad and turn right". This example teaches you how to count crossroads.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. Just use the light on the board.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Take a sheet of paper with a cross drawn on it, and move the sensor slowly along the line and over the crossroad.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards don't say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address
const uint8_t LED = LED_BUILTIN;         // The light on the board (pin 13 on the UNO)

int crossCount = 0;                      // How many crossroads we have passed
bool onCross = false;                    // Are we on a crossroad right now?
bool lost = false;                       // Is the line lost right now?

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // Set the light's pin to "output" so we can turn it on and off
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move the sensor along the line and over the crossroads."));
}

void loop() {
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {            // Read the line results (7 numbers)
    delay(100);
    return;
  }

  uint8_t width    = d[2];               // How many eyes see the line at the same time
  uint8_t groups   = d[3];               // How many line pieces it sees: 0 = no line, 1 = one line, 2 or more = a fork
  int8_t  lastSide = (int8_t)d[6];       // Which edge the line touched last: negative = left, positive = right

  // ---- 1. No line ----
  if (groups == 0) {
    digitalWrite(LED, LOW);              // Light off
    if (!lost) {                         // The line was just lost: say it only once
      lost = true;
      if (lastSide < 0)      Serial.println(F("Line lost! It was last on the left → search left"));
      else if (lastSide > 0) Serial.println(F("Line lost! It was last on the right → search right"));
      else                   Serial.println(F("No line"));
    }
    onCross = false;
    delay(20);
    return;
  }
  lost = false;

  // ---- 2. Is it a crossroad? ----
  // 2 or more line pieces (a fork), or many eyes seeing the line at once (the cross line of a crossroad) = a crossroad
  bool isCross = (groups >= 2) || (width >= 6);   // 👉 You can change this: 6 or more eyes on the line = a crossroad

  if (isCross && !onCross) {             // Count only at the moment we enter the crossroad, so it is not counted twice
    onCross = true;
    crossCount = crossCount + 1;
    Serial.print(F("Crossroad "));
    Serial.print(crossCount);
    Serial.println(F("!"));
    for (int i = 0; i < 3; i++) {        // Blink fast 3 times
      digitalWrite(LED, LOW);  delay(80);
      digitalWrite(LED, HIGH); delay(80);
    }
  }
  if (!isCross) onCross = false;         // We left the crossroad, so the next one can be counted

  // ---- 3. Normal case: light on when we see the line ----
  digitalWrite(LED, HIGH);
  delay(20);
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
Ready! Move the sensor along the line and over the crossroads.
Crossroad 1!
Crossroad 2!
Line lost! It was last on the left → search left
```

> [!TIP]
> **Try it:** Add a line `if (crossCount == 3) { ... }` to do something special at the 3rd crossroad, for example keep the light blinking.

---

<h2 id="ex6">6️⃣ Line-following car (advanced)</h2>

**What it does:** Lets a two-wheel car follow a black line by itself. If the line is to the left, it steers left. If it is to the right, it steers right. **The further off, the harder it steers.** This is called "proportional control", and it is the most basic trick in line-following competitions. When the line is lost, the car turns on the spot toward the side where the line disappeared, to find it again.

**You also need:** A two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), and batteries.

**Motor wiring:**

| L298N | Arduino UNO / Nano | ESP32 | Controls |
| :---: | :---: | :---: | :--- |
| ENA | 5 | GPIO 25 | Left wheel speed |
| IN1 / IN2 | 7 / 8 | GPIO 26 / 27 | Left wheel direction |
| ENB | 6 | GPIO 14 | Right wheel speed |
| IN3 / IN4 | 9 / 10 | GPIO 32 / 33 | Right wheel direction |
| GND | GND | GND | **Must share GND with the Arduino** |

**Before the first run:**
1. Run [Example 4](#ex4) first to calibrate the field.
2. Lift the car up (wheels in the air) and upload. Check that both wheels turn **forward**. If a wheel turns the wrong way, change `LEFT_REVERSE` or `RIGHT_REVERSE` to `true`.
3. Put the car on the line. It starts after 3 seconds.

<a href="/examples/line8/arduino-en/line8_06_line_follower_car/line8_06_line_follower_car.ino" download data-ignore="true">📄 Download line8_06_line_follower_car.ino</a>

<!-- ino: examples/line8/arduino-en/line8_06_line_follower_car/line8_06_line_follower_car.ino -->
```cpp
/*
 * Pathfinder 8-Way x Arduino  Example 6 (advanced): Line-following car
 * ------------------------------------------------------------
 * What does this example do?
 *   It lets a two-wheel car follow a black line by itself. If the line is to the left, it steers left.
 *   If the line is to the right, it steers right. The further off, the harder it steers.
 *   This is called "proportional control". It is the most basic trick in line-following competitions.
 *   When the line is lost, the car turns on the spot toward the side where the line disappeared, to find it again.
 *
 * You need:
 *   A two-wheel car, an L298N motor driver board (or a TB6612 etc. wired the same way), and batteries.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   (For ESP32 use: ENA -> 25, IN1 -> 26, IN2 -> 27, ENB -> 14, IN3 -> 32, IN4 -> 33)
 *   The L298N GND must be connected to the Arduino GND!
 *   Mount the sensor in the middle of the car's front, about 0.5 ~ 1 cm above the floor.
 *
 * Before the first run:
 *   1. Run "Example 4: One-key calibration" first to calibrate the field.
 *   2. Lift the car up (wheels in the air) and upload. Check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE below to true.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address

// ---- Motor pins ----
#if defined(ESP32)
// ESP32 pins 6 ~ 11 are connected to the memory chip on the board and cannot be used, so we use these pins
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 You can change these settings ----
const int   BASE_SPEED = 120;            // Speed going straight: 0 ~ 255. Start slow, it is easier to tune
const float KP         = 1.0;            // Steering strength: make it bigger if the car can't get back to the line, smaller if it wobbles left and right
const int   SEARCH_SPEED = 100;          // Speed when turning on the spot to find the line
const bool  LEFT_REVERSE  = false;       // Change to true if the left wheel turns the wrong way
const bool  RIGHT_REVERSE = false;       // Change to true if the right wheel turns the wrong way

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  mbcWrite(0x10, 0);                     // Follow a black line (for a white line, change this to 1)

  Serial.println(F("Starting in 3 seconds!"));
  delay(3000);
}

void loop() {
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {            // Can't read the sensor -> stop the car, to be safe
    drive(0, 0);
    return;
  }

  int8_t  error  = (int8_t)d[1];         // How far off the line is: -100 (far left) ~ +100 (far right), 0 = right in the middle
  uint8_t groups = d[3];                 // 0 = no line

  if (groups == 0) {
    // Line lost: turn on the spot toward the side where the line disappeared, to find it again
    if (error < 0) drive(-SEARCH_SPEED, SEARCH_SPEED);   // Line on the left -> turn left on the spot
    else           drive(SEARCH_SPEED, -SEARCH_SPEED);   // Line on the right -> turn right on the spot
    return;
  }

  // Proportional control: the further off, the harder it steers
  int turn = KP * error;                 // Line on the right (positive) -> turn is positive -> left wheel faster, right wheel slower -> turns right
  drive(BASE_SPEED + turn, BASE_SPEED - turn);
}

// Control the two wheels: speed -255 (full speed backward) ~ 255 (full speed forward)
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

**How to tune it so the car drives better:**

| What the car does | What to change |
| :--- | :--- |
| Runs off the line in turns and can't get back | Make `KP` bigger (for example 1.0 → 1.5), or make `BASE_SPEED` smaller |
| Keeps wobbling left and right on a straight line | Make `KP` smaller (for example 1.0 → 0.6) |
| Drives smoothly, and you want it faster | Add 20 to `BASE_SPEED` at a time, then tune `KP` again |
| Spins on the spot right after it starts | A wheel turns the wrong way: change `LEFT_REVERSE` or `RIGHT_REVERSE` |

---

## ❓ FAQ

**Q: It keeps saying "Sensor not found"?**
1. Check that SDA and SCL are not swapped (UNO: SDA → A4, SCL → A5).
2. Check that VCC and GND are plugged in firmly. Is the light on the sensor on?
3. Make sure it is the **Universal I2C edition**. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.

**Q: The Serial Monitor shows strange characters?**
The speed is not set to **115200**. Change it at the bottom right of the Serial Monitor and it will be fine.

**Q: The line is in the middle, but the position keeps jumping?**
Run [Example 4](#ex4) first to calibrate. Then check the sensor's height above the floor (0.5 ~ 1 cm is best).

**Q: Can I connect two Pathfinders at the same time?**
No. The Pathfinder 8-Way and 16-Way both use address `0x16`, and only one of them can be on the same SDA/SCL wires. You can share the wires with MBC products that use a different address (for example the gyro at `0x6A`).

**Q: I want to know what data each box holds?**
See the [I2C Register Guide](/en/sensors/line8/arduino-i2c.md) (advanced information for people who can code).
