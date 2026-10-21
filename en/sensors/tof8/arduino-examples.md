<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 Ranger 8-Way × Arduino Examples

[🔙 Back to Ranger 8-Way Home](/en/sensors/tof8/index.md)　|　[📡 MBC Universal I2C Protocol (advanced)](/en/i2c-protocol.md)

This page has **6 examples**, from "is it connected?" all the way to an "obstacle-avoiding car". You can copy and upload each example as it is, and every line of the code has an English comment. **Never written code before? No problem.** Just do them one by one, in order.

> [!TIP]
> First time using Arduino? Read [🔰 First Time with Arduino: Start from Zero](/en/sensors/arduino-start.md) first. In 15 minutes you learn how to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/tof8/arduino-en/tof8_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
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
> - Use the **Universal I2C edition** of the Ranger 8-Way. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
> - When the sensor powers on, it needs **2 ~ 4 seconds** to wake up the 8 lasers one by one. During this time **every distance reads 0**, which looks like something is touching it. So every example first waits until it is "ready" (up to 6 seconds). Do the same when you write your own code.
> - Unplug the USB cable before you connect the wires.

**Where does each of the 8 lasers look?** (seen from behind the sensor, looking the same way it looks)

<!-- 需實機確認：各顆的實際安裝角度、第 0 顆是否在機器人的左邊。來源：產品頁角度表＋韌體 SENSOR_POS（第 3、4 顆同為正前方）。 -->

| Laser | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| About this angle | Left 90° | Left 60° | Left 30° | Straight ahead | Straight ahead | Right 30° | Right 60° | Right 90° |
| Bearing number | -7 | -5 | -3 | -1 | +1 | +3 | +5 | +7 |

**How to read the distance** (always in mm, 10 mm = 1 cm)

| Number you read | Meaning |
| :--- | :--- |
| `0xFFFF` (65535) | This laser is **broken** (check this first) |
| `1200` or more | **Nothing** within 120 cm in this direction |
| Anything else | How far away the thing in front is |

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the sensor](#ex1) | Check the wiring and that all 8 lasers work | Nothing |
| 2 | [How far does each of the 8 lasers see?](#ex2) | Read each laser's distance; tell "broken" from "nothing there" | Nothing |
| 3 | [Which way is the nearest thing?](#ex3) | Read the "bearing" and show the direction with an arrow | A box or your hand |
| 4 | [Parking sensor alarm light](#ex4) | Control a light with distance: closer = faster blinking | Nothing |
| 5 | [Radar bar chart](#ex5) | Draw the 8 directions as bars and see at a glance which way is open | A few books as obstacles |
| 6 | [Obstacle-avoiding car (advanced)](#ex6) | Make a car go around obstacles by itself | Two-wheel car, L298N motor driver board |

> [!TIP]
> **Suggested order: 1 → 2 → 3 → 4 → 5 → 6.** First check the wiring (1), understand each laser's distance (2), then learn directions (3) and the small projects.

---

<h2 id="ex1">1️⃣ Say hello to the sensor</h2>

**What it does:** Checks that the sensor is connected and that all 8 lasers started correctly. Run this one first every time you connect a new sensor.

**Steps:**
1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.

<a href="/examples/tof8/arduino-en/tof8_01_hello/tof8_01_hello.ino" download data-ignore="true">📄 Download tof8_01_hello.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_01_hello/tof8_01_hello.ino -->
```cpp
/*
 * Ranger 8-Way x Arduino  Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   It checks that the sensor is connected. If it is, the computer shows
 *   the sensor's name and version, and checks that all 8 lasers work.
 *   Run this example first every time you connect a new sensor.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Good to know:
 *   When the Ranger 8-Way powers on, it needs 2 ~ 4 seconds to wake up the 8 lasers one by one.
 *   During this time every distance reads 0 (it looks like something is touching it),
 *   so always wait until it is "ready" first.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the sensor

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's "address" (like a house number). It is always 0x29

void setup() {
  Serial.begin(115200);                  // Open the line to the computer, speed 115200
  mbcBegin();                            // Get I2C ready (the helpers are at the bottom of this file)
  delay(500);                            // Give the sensor 0.5 seconds to power on

  Serial.println(F("=== Ranger 8-Way: Say hello to the sensor ==="));
  Serial.println(F("Waiting for the 8 lasers to start (up to 6 seconds)..."));

  // Wait for the sensor to say "I'm ready", up to 6 seconds (the 8 lasers need 2 ~ 4 seconds)
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found! Please check:"));
    Serial.println(F("  1. SDA goes to A4 and SCL goes to A5. Are they swapped?"));
    Serial.println(F("  2. VCC goes to 5V and GND goes to GND. Are they plugged in firmly?"));
    Serial.println(F("  3. Is your sensor the Universal I2C edition? (The SPIKE and EV3 editions do not work with Arduino.)"));
    Serial.println(F("  4. Is anything else with address 0x29 on the same SDA/SCL wires (for example a Ranger 2-Way, or a VL53L0X module from a shop)?"));
    while (true) {}                      // Not found: stop here
  }

  // Read the "version info": box 0x01 of the sensor, 17 numbers
  uint8_t info[17];
  if (!mbcRead(0x01, info, 17)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    while (true) {}
  }

  // Numbers 9 ~ 16 are the sensor's name (letters)
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("Found it! Sensor name: "));
  Serial.println(name);                  // Should print LASER8

  Serial.print(F("Firmware version: "));
  Serial.print(info[2]);                 // Major version
  Serial.print('.');
  Serial.println(info[3]);               // Minor version

  Serial.print(F("This sensor has "));
  Serial.print(info[7]);                 // Ranger 8-Way = 8
  Serial.println(F(" lasers"));

  Serial.print(F("Max range: "));
  Serial.print(info[8] * 10);            // This box stores "how many 10 mm". Times 10 = millimeters (1200)
  Serial.println(F(" mm (anything farther counts as 'nothing there')"));

  // Read the "health check": box 0x31, 9 numbers
  //   Number 0: status (bit 1 = is any laser broken)
  //   Number 2: each bit is one laser. 1 = it started OK
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    while (true) {}
  }

  if (st[0] & 0x02) {                    // Bit 1 is 1 -> a laser is broken
    Serial.print(F("Warning: these lasers did not start: "));
    for (uint8_t i = 0; i < 8; i++) {
      if (!(st[2] & (1u << i))) {        // This laser's bit is 0 -> it did not start
        Serial.print(i);
        Serial.print(' ');
      }
    }
    Serial.println();
    Serial.println(F("(Run example 2 to see which one keeps showing XX, then contact us.)"));
  } else {
    Serial.println(F("All 8 lasers are OK!"));
  }

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
=== Ranger 8-Way: Say hello to the sensor ===
Waiting for the 8 lasers to start (up to 6 seconds)...
Found it! Sensor name: LASER8
Firmware version: 1.0
This sensor has 8 lasers
Max range: 1200 mm (anything farther counts as 'nothing there')
All 8 lasers are OK!
Wiring is correct. You can try the other examples now!
```

> [!NOTE]
> **What are the "Communication helpers"?** At the bottom of every example there is the same piece of code. It "talks" to the sensor. You don't need to change it or understand it. Just remember:
> - `mbcRead(box number, where to put it, how many)`: read data from the sensor
> - `mbcWaitReady(max milliseconds to wait)`: wait until the sensor has started
>
> Think of the sensor as a row of numbered "boxes" (programmers call them registers). Each box holds different data. For example, box `0x21` holds "the distances of the 8 lasers".

---

<h2 id="ex2">2️⃣ How far does each of the 8 lasers see?</h2>

**What it does:** Prints the distances of the 8 lasers in one row, with the left one on the left. `----` = nothing in this direction, `XX` = this laser is broken.

<a href="/examples/tof8/arduino-en/tof8_02_eight_distances/tof8_02_eight_distances.ino" download data-ignore="true">📄 Download tof8_02_eight_distances.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_02_eight_distances/tof8_02_eight_distances.ino -->
```cpp
/*
 * Ranger 8-Way x Arduino  Example 2: How far does each of the 8 lasers see?
 * ------------------------------------------------------------
 * What does this example do?
 *   The Ranger 8-Way has 8 lasers in a half circle. They look from the left all the way to the right.
 *   This example prints the distance each laser measures (unit: millimeters mm, 10 mm = 1 cm):
 *     number = how far away the thing in front is
 *     ----   = nothing in this direction (farther than 120 cm)
 *     XX     = this laser is broken
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Move your hand or a book left and right in front of the sensor. Which number changes?
 *
 * Good to know:
 *   Laser 0 is on the far left, laser 7 is on the far right (left and right as seen
 *   from behind the sensor, looking the same way it looks).
 *   Lasers 3 and 4 both point straight ahead.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint8_t LASERS = 8;                // The Ranger 8-Way has 8 lasers
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing in this direction
const uint16_t FAULT = 0xFFFF;           // 0xFFFF (65535) = this laser is broken

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then, so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand in front of the sensor."));
  Serial.println(F("(Unit: mm. ---- = nothing there, XX = broken)"));
  Serial.println(F("  Left                                     Right"));
  Serial.println(F("     0     1     2     3     4     5     6     7"));
}

void loop() {
  // Box 0x21: 16 numbers. Every 2 numbers make one laser's distance (laser 0 -> laser 7)
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  for (uint8_t i = 0; i < LASERS; i++) {
    // Join 2 numbers into one distance: the first one is the "big half"
    uint16_t mm = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];

    // The order of checks matters: first broken?, then nothing there?, and only then the distance
    if (mm == FAULT) {
      Serial.print(F("    XX"));          // This laser is broken
    } else if (mm >= NO_TARGET) {
      Serial.print(F("  ----"));          // Nothing in this direction
    } else {
      printRight(mm, 6);                  // Print the distance, right-aligned in 6 spaces
    }
  }
  Serial.println();

  delay(300);                             // 👉 You can change this: check every 0.3 seconds
}

// Print a number right-aligned, using width spaces in total (blank spaces in front)
void printRight(uint16_t value, uint8_t width) {
  uint8_t digits = 1;
  for (uint16_t v = value; v >= 10; v /= 10) digits++;   // Count how many digits the number has
  for (uint8_t k = digits; k < width; k++) Serial.print(' ');
  Serial.print(value);
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

**You will see:** (a hand 20 cm straight ahead, and a wall far away on the right)

```text
Ready! Move your hand in front of the sensor.
(Unit: mm. ---- = nothing there, XX = broken)
  Left                                     Right
     0     1     2     3     4     5     6     7
  ----  ----   432   215   221  ----  ----  1105
  ----  ----   430   213   224  ----  ----  1102
```

> [!TIP]
> **Try it:** Move your hand slowly from left to right and watch the number "walk" from laser 0 to laser 7. Lasers 3 and 4 both point straight ahead, so with your hand straight ahead their numbers are very close.

---

<h2 id="ex3">3️⃣ Which way is the nearest thing?</h2>

**What it does:** The sensor compares the 8 lasers by itself and tells you **which direction the nearest thing is in** and how far away it is. The computer draws a small ruler with a `*` where the thing is, and an arrow for the direction. Sumo robots use this to "find the opponent".

**You need:** A box or your hand.

<a href="/examples/tof8/arduino-en/tof8_03_nearest_direction/tof8_03_nearest_direction.ino" download data-ignore="true">📄 Download tof8_03_nearest_direction.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_03_nearest_direction/tof8_03_nearest_direction.ino -->
```cpp
/*
 * Ranger 8-Way x Arduino  Example 3: Which way is the nearest thing?
 * ------------------------------------------------------------
 * What does this example do?
 *   The sensor compares the 8 lasers by itself and finds which direction the nearest thing is in,
 *   and how far away it is. The computer draws a small ruler with a * where the thing is,
 *   and an arrow that tells you the direction:
 *     ←  left    ↖  front-left    ↑  straight ahead    ↗  front-right    →  right
 *   Sumo robots use this to "find the opponent", and cars use it to "follow your hand".
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Hold your hand about 30 cm in front of the sensor and move it slowly from left to right.
 *
 * What is the bearing number?
 *   -7 (far left) ~ 0 (straight ahead) ~ +7 (far right). Negative = left, positive = right.
 *   When the thing is right between two lasers, you get the number in between (for example -6, -4).
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing there

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then, so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Hold your hand in front of the sensor and move it slowly from left to right."));
}

void loop() {
  // Box 0x11: 6 numbers
  //   Number 0: which direction the nearest thing is in (-7 left ~ +7 right)
  //   Numbers 1, 2: how far the nearest thing is (mm)
  //   Numbers 3 ~ 5: direction and distance of the farthest (most open) way. Not used here
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  int8_t   dir = (int8_t)d[0];                         // Direction: turn it into a number with a +/- sign
  uint16_t mm  = ((uint16_t)d[1] << 8) | d[2];         // Join 2 numbers into the distance

  // First make sure something is really there, then look at the direction. With nothing there, the direction means nothing
  if (mm >= NO_TARGET) {
    Serial.println(F("Left [...............] Right   Nothing within 120 cm in front"));
    delay(300);
    return;
  }

  // ---- 1. Draw the small ruler: 15 spots for -7 ~ +7. Put a * where the thing is ----
  Serial.print(F("Left ["));
  for (int8_t pos = -7; pos <= 7; pos++) {
    if (pos == dir)      Serial.print('*');            // The thing is here
    else if (pos == 0)   Serial.print('+');            // Mark the middle
    else                 Serial.print('.');
  }
  Serial.print(F("] Right   "));

  // ---- 2. Show the direction with an arrow ----
  if (dir <= -6)      Serial.print(F("← Left       "));  // 👉 You can change this: where each zone starts and ends
  else if (dir <= -2) Serial.print(F("↖ Front-left "));
  else if (dir <= 1)  Serial.print(F("↑ Ahead      "));
  else if (dir <= 5)  Serial.print(F("↗ Front-right"));
  else                Serial.print(F("→ Right      "));

  // ---- 3. Print the distance (cm) and the bearing number ----
  Serial.print(F("  "));
  Serial.print(mm / 10);                               // mm divided by 10 = cm
  Serial.print(F(" cm (bearing "));
  Serial.print((int)dir);                              // Change to int before printing, so it prints as a number
  Serial.println(F(")"));

  delay(300);                                          // 👉 You can change this: check every 0.3 seconds
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

**You will see:** (a hand moving from front-left to straight ahead, then to the right)

```text
Left [...............] Right   Nothing within 120 cm in front
Left [..*....+.......] Right   ↖ Front-left   35 cm (bearing -5)
Left [.......*.......] Right   ↑ Ahead        28 cm (bearing 0)
Left [.......+.....*.] Right   → Right        40 cm (bearing 6)
```

**What does the bearing mean?**

| Bearing number | Meaning |
| :---: | :--- |
| 0 | The thing is straight ahead |
| Negative (-1 ~ -7) | The thing is on the **left**. Smaller = further left (-7 = directly left) |
| Positive (+1 ~ +7) | The thing is on the **right**. Bigger = further right (+7 = directly right) |

> [!WARNING]
> **Always check the distance first, then the direction.** When nothing is within 120 cm, the bearing becomes `-8` or `+8` (meaning "which side the thing was last seen on"). It does not mean something is really there. The example first checks "distance less than 1200" to make sure something is there, and only then looks at the direction.

> [!TIP]
> **Try it:** Change where the arrow zones start and end (for example, change `dir <= -6` to `dir <= -5`) and see when the arrow changes.

---

<h2 id="ex4">4️⃣ Parking sensor alarm light</h2>

**What it does:** Uses the small built-in light on the Arduino board (the one marked **L**) as an alarm, like a car's parking sensor: light off when nothing is there, blinking when something is there, **the closer, the faster**, and on all the time within 10 cm. If any of the 8 lasers sees something, it counts.

<a href="/examples/tof8/arduino-en/tof8_04_distance_alarm/tof8_04_distance_alarm.ino" download data-ignore="true">📄 Download tof8_04_distance_alarm.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_04_distance_alarm/tof8_04_distance_alarm.ino -->
```cpp
/*
 * Ranger 8-Way x Arduino  Example 4: Parking sensor alarm light
 * ------------------------------------------------------------
 * What does this example do?
 *   It uses the small built-in light on the Arduino board (the one marked L) as an alarm,
 *   just like a car's parking sensor:
 *     Nothing within 120 cm in front -> light off
 *     Something there: the closer it is, the faster it blinks
 *     Very close (within 10 cm)      -> light stays on
 *   If any one of the 8 lasers sees something, it counts. Things coming close from the left or right also make it blink.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   You don't need to add a light. The light on the board is enough.
 *
 * How to see the result:
 *   After uploading, watch the L light on the Arduino board. Move your hand slowly closer and it blinks faster.
 *   You can also open the Serial Monitor (115200) to see the nearest distance.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards don't say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint8_t LED = LED_BUILTIN;         // The light on the board (pin 13 on the UNO)
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing there

// ---- 👉 You can change these settings ----
const uint16_t ALARM_START = 1000;       // How close before it starts blinking (mm): 1000 = 1 meter
const uint16_t ALWAYS_ON   = 100;        // How close before it stays on (mm): 100 = 10 cm

uint16_t nearest = NO_TARGET;            // How far the nearest thing is (mm)
bool ledOn = false;                      // Is the light on or off right now
unsigned long lastBlink = 0;             // When the light last switched
unsigned long lastRead = 0;              // When the sensor was last read
unsigned long lastPrint = 0;             // When we last printed to the computer

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // Set the light's pin to "output" so we can turn it on and off
  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then (that would look "touching" and blink like crazy), so wait
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand slowly closer and watch the L light blink faster."));
}

void loop() {
  // ---- 1. Read the nearest distance every 0.05 seconds ----
  if (millis() - lastRead >= 50) {
    lastRead = millis();
    uint8_t d[6];
    if (mbcRead(0x11, d, 6)) {                         // Box 0x11: direction and distance of the nearest / farthest thing
      nearest = ((uint16_t)d[1] << 8) | d[2];          // Numbers 1, 2 = nearest distance (mm)
    } else {
      nearest = NO_TARGET;                             // Could not read: treat it as nothing there, light off
    }
  }

  // ---- 2. Decide how the light works from the distance ----
  if (nearest >= ALARM_START) {
    ledOn = false;                                     // Too far or nothing there: light off
  } else if (nearest <= ALWAYS_ON) {
    ledOn = true;                                      // Very close: light stays on
  } else {
    // The closer it is, the shorter the wait: 100 mm -> switch every 0.05 s; 1000 mm -> switch every 0.5 s
    unsigned long interval = map(nearest, ALWAYS_ON, ALARM_START, 50, 500);
    if (millis() - lastBlink >= interval) {
      lastBlink = millis();
      ledOn = !ledOn;                                  // On becomes off, off becomes on
    }
  }
  digitalWrite(LED, ledOn ? HIGH : LOW);

  // ---- 3. Print the distance to the computer every 0.5 seconds ----
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    if (nearest >= NO_TARGET) {
      Serial.println(F("Nothing there"));
    } else {
      Serial.print(F("Nearest thing: "));
      Serial.print(nearest / 10);
      Serial.println(F(" cm"));
    }
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

**You will see:** (a hand coming slowly closer; the L light blinks faster and faster)

```text
Ready! Move your hand slowly closer and watch the L light blink faster.
Nothing there
Nearest thing: 85 cm
Nearest thing: 42 cm
Nearest thing: 9 cm
```

> [!TIP]
> **Try it:** Change `ALARM_START` to `500` so it only starts blinking within 50 cm. If you have a buzzer, change the light's pin to the buzzer's pin and you get a parking sensor that goes "beep beep beep".

---

<h2 id="ex5">5️⃣ Radar bar chart</h2>

**What it does:** Draws the distances of the 8 directions as 8 bars: **longer bar = that direction is more open**, short bar = something is in the way. One look tells you which way is free, like a small radar.

**You need:** A few books as obstacles.

<a href="/examples/tof8/arduino-en/tof8_05_radar_bars/tof8_05_radar_bars.ino" download data-ignore="true">📄 Download tof8_05_radar_bars.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_05_radar_bars/tof8_05_radar_bars.ino -->
```cpp
/*
 * Ranger 8-Way x Arduino  Example 5: Radar bar chart
 * ------------------------------------------------------------
 * What does this example do?
 *   It draws the distances of the 8 lasers as 8 bars, one bar per laser:
 *     Long bar  = that direction is open (things are far away)
 *     Short bar = something is blocking that direction
 *   One look tells you "where the obstacles are and which way is free", like a small radar.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put the sensor on a table. Block the front, the left and the right with a book.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint8_t LASERS = 8;                // 8 lasers
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing there
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

// About which angle each laser looks at (negative = left, positive = right, 0 = straight ahead)
const int ANGLE[LASERS] = {-90, -60, -30, 0, 0, 30, 60, 90};

const uint16_t MM_PER_BLOCK = 50;        // 👉 You can change this: one # = 50 mm (5 cm). 1200 mm = exactly 24 #

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then, so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Block the front, the left and the right of the sensor with a book."));
}

void loop() {
  // Box 0x21: the distances of the 8 lasers, 2 numbers each, 16 in total
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  Serial.println();
  Serial.println(F("====== Radar (longer bar = more open) ======"));

  for (uint8_t i = 0; i < LASERS; i++) {
    uint16_t mm = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];   // Join 2 numbers into the distance

    // ---- 1. Print "which laser, which angle" ----
    Serial.print(i);
    if (ANGLE[i] > 0)       Serial.print(F("  +"));            // Put a + in front of right-side angles
    else if (ANGLE[i] == 0) Serial.print(F("   "));
    else                    Serial.print(F("  "));
    if (abs(ANGLE[i]) < 10) Serial.print(' ');                 // To line things up
    Serial.print(ANGLE[i]);
    Serial.print(F(" deg |"));

    // ---- 2. Draw the bar ----
    if (mm == FAULT) {                                         // First check if it is broken
      Serial.println(F("XXXXXXXX  This laser is broken"));
      continue;                                                // Done with this laser, go to the next one
    }
    uint16_t blocks = mm / MM_PER_BLOCK;                       // How many # to draw
    for (uint16_t k = 0; k < blocks; k++) Serial.print('#');

    // ---- 3. Write the distance after the bar ----
    if (mm >= NO_TARGET) {
      Serial.println(F("> nothing there"));
    } else {
      Serial.print(F(" "));
      Serial.print(mm / 10);                                   // mm divided by 10 = cm
      Serial.println(F(" cm"));
    }
  }

  delay(500);                                                  // 👉 You can change this: draw every 0.5 seconds
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

**You will see:** (a book 30 cm straight ahead)

```text
====== Radar (longer bar = more open) ======
0  -90 deg |########################> nothing there
1  -60 deg |########################> nothing there
2  -30 deg |############### 75 cm
3    0 deg |###### 32 cm
4    0 deg |###### 34 cm
5  +30 deg |################ 80 cm
6  +60 deg |###################### 110 cm
7  +90 deg |########################> nothing there
```

> [!TIP]
> **Try it:** Change `MM_PER_BLOCK` to `100`. The bars become half as long, which fits narrow screens better.

---

<h2 id="ex6">6️⃣ Obstacle-avoiding car (advanced)</h2>

**What it does:** Lets a two-wheel car drive around by itself without hitting walls. With nothing in front it drives straight, and if one side gets too close to a wall it steers a little to the other side. When something blocks the front, it turns on the spot toward **the more open side** and drives again when the front is clear.

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
1. Mount the sensor in the middle of the car's front, facing forward. Run [Example 5](#ex5) first and check that when your hand is on the car's **left** side, the top bars (lasers 0 ~ 2) get shorter.
2. Lift the car up (wheels in the air) and upload. Check that both wheels turn **forward**. If a wheel turns the wrong way, change `LEFT_REVERSE` or `RIGHT_REVERSE` to `true`.
3. Put the car on the floor. It starts after 3 seconds.

<a href="/examples/tof8/arduino-en/tof8_06_avoid_car/tof8_06_avoid_car.ino" download data-ignore="true">📄 Download tof8_06_avoid_car.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_06_avoid_car/tof8_06_avoid_car.ino -->
```cpp
/*
 * Ranger 8-Way x Arduino  Example 6 (advanced): Obstacle-avoiding car
 * ------------------------------------------------------------
 * What does this example do?
 *   It lets a two-wheel car drive around by itself without hitting walls or obstacles:
 *     Nothing in front       -> drive straight; if one side is too close to a wall, steer a little to the other side
 *     Something blocks front -> stop, turn on the spot toward the "more open side", and drive again when the front is clear
 *   The 8 lasers watch the left, the front and the right, so the car knows which way to turn.
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
 *   Mount the sensor in the middle of the car's front, facing forward: laser 0 on the car's left, laser 7 on the car's right.
 *
 * Before the first run:
 *   1. Run "Example 5: Radar bar chart" first. Put your hand on the car's left side and check
 *      that the top bars (lasers 0 ~ 2) get shorter.
 *   2. Lift the car up (wheels in the air) and upload. Check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE below to true.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing there
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

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
const int      FORWARD_SPEED = 130;      // Speed going straight: 0 ~ 255. Start slow, it is safer
const int      TURN_SPEED    = 110;      // Speed when turning on the spot
const int      STEER         = 40;       // How hard to steer when a side is too close
const uint16_t FRONT_STOP    = 250;      // How close in front before turning (mm): 250 = 25 cm
const uint16_t SIDE_CLOSE    = 150;      // How close on a side counts as "too close" (mm): 150 = 15 cm
const bool     LEFT_REVERSE  = false;    // Change to true if the left wheel turns the wrong way
const bool     RIGHT_REVERSE = false;    // Change to true if the right wheel turns the wrong way

uint16_t dist[8];                        // Distances of the 8 lasers (mm)

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then. Without waiting, the car would think there are walls all around
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}                      // Sensor not found: don't drive (wheels stay stopped)
  }

  Serial.println(F("Starting in 3 seconds!"));
  delay(3000);
}

void loop() {
  // Box 0x21: read the distances of all 8 lasers at once
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {           // Can't read the sensor -> stop the car to be safe
    drive(0, 0);
    delay(50);
    return;
  }

  bool allZero = true;                   // Are all 8 distances 0?
  for (uint8_t i = 0; i < 8; i++) {
    dist[i] = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];
    if (dist[i] != 0) allZero = false;
    if (dist[i] == FAULT) dist[i] = 0;   // A broken laser: treat it as "something is blocking", which is safer
  }
  if (allZero) {                         // All 0: the sensor may have just restarted (for example, low battery) and is not ready yet
    drive(0, 0);                         // -> stop the car and wait until it is ready
    delay(50);
    return;
  }

  // Split the 8 lasers into left, front and right groups. Take the "nearest one" of each group
  uint16_t left  = smallest(0, 2);       // Lasers 0 ~ 2: left and front-left
  uint16_t front = smallest(3, 4);       // Lasers 3, 4: straight ahead
  uint16_t right = smallest(5, 7);       // Lasers 5 ~ 7: front-right and right

  if (front > FRONT_STOP) {
    // ---- Nothing in front: drive straight, steer a little away if a side is too close ----
    int turn = 0;                        // Positive = steer left, negative = steer right
    if (left < SIDE_CLOSE)  turn = turn - STEER;   // Left too close -> steer right
    if (right < SIDE_CLOSE) turn = turn + STEER;   // Right too close -> steer left
    drive(FORWARD_SPEED - turn, FORWARD_SPEED + turn);
  } else {
    // ---- Something in front: turn on the spot toward the more open side ----
    if (left > right) drive(-TURN_SPEED, TURN_SPEED);   // Left is more open -> turn left on the spot
    else              drive(TURN_SPEED, -TURN_SPEED);   // Right is more open -> turn right on the spot
  }

  delay(50);                             // The sensor updates about every 0.1 seconds, so checking every 0.05 seconds is enough
}

// From laser number from to laser number to, find the nearest distance
uint16_t smallest(uint8_t from, uint8_t to) {
  uint16_t m = NO_TARGET;
  for (uint8_t i = from; i <= to; i++) {
    if (dist[i] < m) m = dist[i];
  }
  return m;
}

// Control the two wheels: speed -255 (full speed back) ~ 255 (full speed forward)
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
| Turns too late and crashes | Make `FRONT_STOP` bigger (for example 250 → 350), or make `FORWARD_SPEED` smaller |
| Starts turning while the wall is still far away | Make `FRONT_STOP` smaller (for example 250 → 180) |
| Keeps rubbing the wall when driving along it | Make `SIDE_CLOSE` or `STEER` bigger |
| Keeps spinning on the spot and never drives off | A wheel turns the wrong way: change `LEFT_REVERSE` or `RIGHT_REVERSE`. Or run Example 2 to see if a laser shows `XX` |

> [!NOTE]
> For safety, the car stops if it can't read the sensor, or if all 8 distances are 0 (the sensor just restarted, for example because the battery is low). A broken laser is treated as "something is in the way", so the car will not rush in a direction it cannot see.

---

## 📂 More Examples (for people who can already code)

The two examples below are shorter versions without line-by-line comments for beginners (their comments are in Chinese). They are for people who already know Arduino and want code they can change right away:

| Example | What it does |
| :--- | :--- |
| <a href="/examples/tof8/arduino/TOF8_Arduino_Wire_Example/TOF8_Arduino_Wire_Example.ino" download data-ignore="true">TOF8_Arduino_Wire_Example.ino</a> | Uses only `Wire.h`. Prints the 8 distances and the nearest / farthest bearing at once |
| <a href="/examples/tof8/arduino/TOF8_Arduino_Library_Example/TOF8_Arduino_Library_Example.ino" download data-ignore="true">TOF8_Arduino_Library_Example.ino</a> + <a href="/examples/tof8/arduino/TOF8_Arduino_Library_Example/MBC_TOF8.h" download data-ignore="true">MBC_TOF8.h</a> | Library style (put both files in the same folder) |

> [!WARNING]
> `TOF8_Arduino_Wire_Example` starts reading right after power-on, so for the first 2 ~ 4 seconds it prints distances that are all 0. The sensor is still starting. It is not broken.

---

## ❓ FAQ

**Q: It keeps saying "Sensor not found"?**
1. Check that SDA and SCL are not swapped (UNO: SDA → A4, SCL → A5).
2. Check that VCC and GND are plugged in firmly. Is the light on the sensor on?
3. Make sure it is the **Universal I2C edition**. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.

**Q: The Serial Monitor shows strange characters?**
The speed is not set to **115200**. Change it at the bottom right of the Serial Monitor and it will be fine.

**Q: All distances are 0 right after power-on?**
The sensor is still waking up the 8 lasers (about 2 ~ 4 seconds), and all data is 0 during this time. The examples already wait with `mbcWaitReady(6000)`. In your own code, also wait until bit 0 of box `0x32` becomes 1 before you start reading.

**Q: One laser keeps showing `XX` (broken)?**
Turn the power off, plug everything in again and try once more. If it is still the same, run [Example 1](#ex1) to see which laser did not start, then [contact us](/en/contact.md) for repair.

**Q: Something is clearly in front, but it says "nothing there"?**
The Ranger measures up to 120 cm. Also, things like **black velvet, clear glass and shiny mirrors** reflect very little laser light, or reflect it in odd directions, so they are hard to measure. Try a white or light-colored object.

**Q: How often should I read it?**
Each laser updates about every 0.1 seconds, and the sensor already filters out jumpy numbers for you. Reading every 0.05 ~ 0.1 seconds in `loop` is enough. Reading faster just gives you the same numbers.

**Q: Can I connect it together with a Ranger 2-Way, or a VL53L0X laser module from a shop?**
No. They all use address `0x29`, and only one of them can be on the same SDA/SCL wires. You can share the wires with MBC products that use a different address (for example the Pathfinder line sensor at `0x16`).

**Q: I want to know what data each box holds?**
See the [I2C register table](/en/sensors/tof8/index.md) on the product page (advanced information for people who can code).
