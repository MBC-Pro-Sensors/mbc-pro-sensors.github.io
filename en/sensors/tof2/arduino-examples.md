<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 Ranger 2-Way × Arduino Examples

[🔙 Back to Ranger 2-Way Home](/en/sensors/tof2/index.md)　|　[📡 MBC Universal I2C Protocol (advanced)](/en/i2c-protocol.md)

> [!NOTE]
> The Universal I2C edition is coming soon. The examples below are for the new hardware (from October 2026 on).

This page has **5 examples**, from "is it connected?" all the way to the Ranger 2-Way's best trick: **telling which side a thing slipped away to**. You can copy and upload each example as it is, and every line of the code has an English comment. **Never written code before? No problem.** Just do them one by one, in order.

> [!TIP]
> First time using Arduino? Read [🔰 First Time with Arduino: Start from Zero](/en/sensors/arduino-start.md) first. In 15 minutes you learn how to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/tof2/arduino-en/tof2_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
</div>

---

## 🔌 Wiring (the same for every example)

| Sensor | Solder pad on the new board | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: | :---: |
| SCL | 1 | A5 | GPIO 22 |
| SDA | 2 | A4 | GPIO 21 |
| VCC | 3 | 5V | 5V (VIN) or 3.3V |
| GND | 4 | GND | GND |

> [!IMPORTANT]
> - Use the **Universal I2C edition** (new board) of the Ranger 2-Way. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly. The old board does not support I2C.
> - On the new board the solder pads are **1 = SCL, 2 = SDA**. This is the opposite of the SDA / SCL printed on the old board. If it won't connect, first try swapping the SDA and SCL wires (swapping them does not damage anything).
> - When the sensor powers on, it first wakes up its 2 lasers. During this time **every distance reads 0**, which looks like something is touching it. So every example first waits until it is "ready". Do the same when you write your own code.
> - Unplug the USB cable before you connect the wires.

**How to read the distance** (always in mm, 10 mm = 1 cm)

| Number you read | Meaning |
| :--- | :--- |
| `0xFFFF` (65535) | This laser is **broken** (check this first) |
| `1200` or more | **Nothing** within 120 cm on this side |
| Anything else | How far away the thing in front is |

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the sensor](#ex1) | Check the wiring and that both lasers work | Nothing |
| 2 | [How far is each side?](#ex2) | Read the distances of the left and right lasers | Nothing |
| 3 | [Is it on the left, on the right, or straight ahead?](#ex3) | Use two lasers to find which side a thing is on | A box or your hand |
| 4 | [Parking sensor alarm light](#ex4) | Control a light with distance: closer = faster blinking | Nothing |
| 5 | [Which way did it slip away?](#ex5) | Tell whether a thing left to the left or to the right | A box |

> [!TIP]
> **Suggested order: 1 → 2 → 3 → 4 → 5.** Example 5 uses the "sees it" check from Example 3, so it is easier to understand after you try Example 3.

---

<h2 id="ex1">1️⃣ Say hello to the sensor</h2>

**What it does:** Checks that the sensor is connected and that both lasers (left and right) started correctly. Run this one first every time you connect a new sensor.

**Steps:**
1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.

<a href="/examples/tof2/arduino-en/tof2_01_hello/tof2_01_hello.ino" download data-ignore="true">📄 Download tof2_01_hello.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_01_hello/tof2_01_hello.ino -->
```cpp
/*
 * Ranger 2-Way x Arduino  Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   It checks that the sensor is connected. If it is, the computer shows
 *   the sensor's name and version, and checks that both lasers (left and right) work.
 *   Run this example first every time you connect a new sensor.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   (Solder pads on the new board: 1 = SCL, 2 = SDA, 3 = VCC, 4 = GND)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Good to know:
 *   When the sensor powers on, it first has to wake up its 2 lasers. During this time
 *   every distance reads 0, so always wait until it is "ready" first.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the sensor

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's "address" (like a house number). It is always 0x29

void setup() {
  Serial.begin(115200);                  // Open the line to the computer, speed 115200
  mbcBegin();                            // Get I2C ready (the helpers are at the bottom of this file)
  delay(500);                            // Give the sensor 0.5 seconds to power on

  Serial.println(F("=== Ranger 2-Way: Say hello to the sensor ==="));

  // Wait for the sensor to say "I'm ready", up to 6 seconds
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found! Please check:"));
    Serial.println(F("  1. SDA goes to A4 and SCL goes to A5. Are they swapped? (New board pads: 1 = SCL, 2 = SDA)"));
    Serial.println(F("  2. VCC goes to 5V and GND goes to GND. Are they plugged in firmly?"));
    Serial.println(F("  3. Is your sensor the Universal I2C edition? (The SPIKE and EV3 editions do not work with Arduino.)"));
    Serial.println(F("  4. Is anything else with address 0x29 on the same SDA/SCL wires (for example a Ranger 8-Way, or a VL53L0X module from a shop)?"));
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
  Serial.println(name);                  // Should print LASER2

  Serial.print(F("Firmware version: "));
  Serial.print(info[2]);                 // Major version
  Serial.print('.');
  Serial.println(info[3]);               // Minor version

  Serial.print(F("This sensor has "));
  Serial.print(info[7]);                 // Ranger 2-Way = 2
  Serial.println(F(" lasers"));

  Serial.print(F("Max range: "));
  Serial.print(info[8] * 10);            // This box stores "how many 10 mm". Times 10 = millimeters (1200)
  Serial.println(F(" mm (anything farther counts as 'nothing there')"));

  // Read the "health check": box 0x31, 9 numbers
  //   Number 0: status (bit 1 = is any laser broken)
  //   Number 2: bit 0 = left laser started OK, bit 1 = right laser started OK
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    while (true) {}
  }

  if (st[0] & 0x02) {                    // Bit 1 is 1 -> a laser is broken
    if (!(st[2] & (1u << 0))) Serial.println(F("Warning: the left laser did not start"));
    if (!(st[2] & (1u << 1))) Serial.println(F("Warning: the right laser did not start"));
    Serial.println(F("(Run example 2 to see which side keeps showing FAULT, then contact us.)"));
  } else {
    Serial.println(F("Both lasers (left and right) are OK!"));
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
=== Ranger 2-Way: Say hello to the sensor ===
Found it! Sensor name: LASER2
Firmware version: 1.0
This sensor has 2 lasers
Max range: 1200 mm (anything farther counts as 'nothing there')
Both lasers (left and right) are OK!
Wiring is correct. You can try the other examples now!
```

> [!NOTE]
> **What are the "Communication helpers"?** At the bottom of every example there is the same piece of code. It "talks" to the sensor. You don't need to change it or understand it. Just remember:
> - `mbcRead(box number, where to put it, how many)`: read data from the sensor
> - `mbcWaitReady(max milliseconds to wait)`: wait until the sensor has started
>
> Think of the sensor as a row of numbered "boxes" (programmers call them registers). Each box holds different data. For example, box `0x11` holds "the left and right distances".

---

<h2 id="ex2">2️⃣ How far is each side?</h2>

**What it does:** Prints the distances of the left and right lasers and draws them as small bars (longer = farther).

<a href="/examples/tof2/arduino-en/tof2_02_left_right/tof2_02_left_right.ino" download data-ignore="true">📄 Download tof2_02_left_right.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_02_left_right/tof2_02_left_right.ino -->
```cpp
/*
 * Ranger 2-Way x Arduino  Example 2: How far is each side?
 * ------------------------------------------------------------
 * What does this example do?
 *   The Ranger 2-Way has two lasers, left and right. Both look straight ahead.
 *   This example prints the distance each laser measures (unit: cm) and draws it as a bar:
 *     number        = how far away the thing in front is
 *     nothing there = nothing within 120 cm on this side
 *     FAULT         = this laser is broken
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Cover only the left laser with your hand, then only the right one. Which number changes?
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing on this side
const uint16_t FAULT = 0xFFFF;           // 0xFFFF (65535) = this laser is broken

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // While the lasers start, distances are 0, so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Cover the left or the right laser with your hand."));
}

void loop() {
  // Box 0x11: 4 numbers
  //   Numbers 0, 1 together = left distance (mm)
  //   Numbers 2, 3 together = right distance (mm)
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];   // Join 2 numbers into one distance
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  Serial.print(F("Left  "));
  printSide(left);
  Serial.print(F("   Right "));
  printSide(right);
  Serial.println();

  delay(300);                            // 👉 You can change this: check every 0.3 seconds
}

// Print one side's distance: a short bar + cm. Order of checks: broken first, then nothing there, and only then the distance
void printSide(uint16_t mm) {
  if (mm == FAULT) {
    Serial.print(F("[   FAULT    ] This laser is broken"));
    return;
  }
  if (mm >= NO_TARGET) {
    Serial.print(F("[############] nothing there"));
    return;
  }
  // Draw the bar: one block per 100 mm (10 cm), 12 blocks at most. Longer = farther
  Serial.print('[');
  uint8_t blocks = mm / 100;
  for (uint8_t k = 0; k < 12; k++) Serial.print(k < blocks ? '#' : ' ');
  Serial.print(F("] "));
  Serial.print(mm / 10);                 // mm divided by 10 = cm
  Serial.print(F(" cm"));
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

**You will see:** (first only the left side is covered, then nothing is covered and there is a wall far in front)

```text
Ready! Cover the left or the right laser with your hand.
Left  [###         ] 32 cm   Right [############] nothing there
Left  [########    ] 85 cm   Right [########    ] 87 cm
```

> [!TIP]
> **Try it:** Cover only one laser with your hand and make sure "Left" really is the side you think it is. The next examples use this to find directions.

---

<h2 id="ex3">3️⃣ Is it on the left, on the right, or straight ahead?</h2>

**What it does:** The two lasers sit side by side and look forward, like two eyes: **both see it = straight ahead**, only the left sees it = to the left, only the right sees it = to the right. This is step one when a robot "follows something".

**You need:** A box or your hand, about 20 cm in front of the sensor.

<a href="/examples/tof2/arduino-en/tof2_03_where_is_it/tof2_03_where_is_it.ino" download data-ignore="true">📄 Download tof2_03_where_is_it.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_03_where_is_it/tof2_03_where_is_it.ino -->
```cpp
/*
 * Ranger 2-Way x Arduino  Example 3: Is it on the left, on the right, or straight ahead?
 * ------------------------------------------------------------
 * What does this example do?
 *   The left and right lasers sit side by side and look forward, like two eyes:
 *     Both see something   -> it is straight ahead
 *     Only the left sees   -> it is to the left
 *     Only the right sees  -> it is to the right
 *     Neither sees         -> nothing in front
 *   When a robot "follows something" or "finds a target", step one is knowing which side it is on.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put a box (or your hand) about 20 cm in front of the sensor and move it slowly left and right.
 *
 * Good to know:
 *   Only things within 50 cm count (SEE_DIST), so a wall far behind is not taken as the target.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

const uint16_t SEE_DIST = 500;           // 👉 You can change this: how close counts as "seeing" (mm). 500 = 50 cm, 1199 at most

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // While the lasers start, distances are 0 (that would look like "something touching both sides"), so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Put something in front of the sensor and move it slowly left and right."));
}

void loop() {
  // Box 0x11: left distance (2 numbers) + right distance (2 numbers)
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }

  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  // "Sees it" = not broken, and close enough. A broken side counts as not seeing
  bool seeL = (left  != FAULT) && (left  < SEE_DIST);
  bool seeR = (right != FAULT) && (right < SEE_DIST);

  if (seeL && seeR) {
    Serial.print(F("↑ Straight ahead   distance "));
    Serial.print(min(left, right) / 10);           // The closer of the two sides, in cm
    Serial.println(F(" cm"));
  } else if (seeL) {
    Serial.print(F("↖ To the left      distance "));
    Serial.print(left / 10);
    Serial.println(F(" cm   -> the robot should turn left"));
  } else if (seeR) {
    Serial.print(F("↗ To the right     distance "));
    Serial.print(right / 10);
    Serial.println(F(" cm   -> the robot should turn right"));
  } else {
    Serial.println(F("   Nothing in front"));
  }

  if (left == FAULT || right == FAULT) {
    Serial.println(F("(Warning: one laser is broken. Run example 1 to check.)"));
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

**You will see:** (the box starts straight ahead and moves slowly away to the left)

```text
↑ Straight ahead   distance 18 cm
↖ To the left      distance 22 cm   -> the robot should turn left
   Nothing in front
```

> [!TIP]
> **Try it:** The example only counts things within 50 cm (`SEE_DIST = 500`), so a wall far behind is not taken as the target. If your table is big and the space behind is empty, try changing it to `1000`.

---

<h2 id="ex4">4️⃣ Parking sensor alarm light</h2>

**What it does:** Uses the small built-in light on the Arduino board (the one marked **L**) as an alarm: light off when nothing is there, blinking when something is there, **the closer, the faster**, and on all the time within 10 cm. It uses whichever side (left or right) is closer.

<a href="/examples/tof2/arduino-en/tof2_04_distance_alarm/tof2_04_distance_alarm.ino" download data-ignore="true">📄 Download tof2_04_distance_alarm.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_04_distance_alarm/tof2_04_distance_alarm.ino -->
```cpp
/*
 * Ranger 2-Way x Arduino  Example 4: Parking sensor alarm light
 * ------------------------------------------------------------
 * What does this example do?
 *   It uses the small built-in light on the Arduino board (the one marked L) as an alarm,
 *   just like a car's parking sensor:
 *     Nothing in front           -> light off
 *     Something there: the closer it is, the faster it blinks
 *     Very close (within 10 cm)  -> light stays on
 *   Of the left and right lasers, it uses whichever one is closer.
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
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

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

  // While the lasers start, distances are 0 (that would look "touching" and blink like crazy), so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand slowly closer and watch the L light blink faster."));
}

void loop() {
  // ---- 1. Read the left and right distances every 0.05 seconds, and take the closer one ----
  if (millis() - lastRead >= 50) {
    lastRead = millis();
    uint8_t d[4];
    if (mbcRead(0x11, d, 4)) {                         // Box 0x11: left distance + right distance
      uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
      uint16_t right = ((uint16_t)d[2] << 8) | d[3];
      if (left == FAULT)  left  = NO_TARGET;           // A broken side does not count
      if (right == FAULT) right = NO_TARGET;
      nearest = min(left, right);                      // The closer side
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
> **Try it:** Change `ALARM_START` to `500` so it only starts blinking within 50 cm.

---

<h2 id="ex5">5️⃣ Which way did it slip away?</h2>

**What it does:** This is the Ranger 2-Way's best trick. When a thing moves away to the right, **the left laser loses it first**, and only the right laser still sees it. When the right one loses it too, we know it slipped away **to the right**. Moving away to the left works the same way. When a sumo robot's opponent runs away, the robot knows which way to turn and chase.

| Before it vanished, who saw it last? | Which way it left |
| :--- | :--- |
| Only the **right** laser (the left one lost it first) | To the **right** → |
| Only the **left** laser (the right one lost it first) | To the **left** ← |
| Both lost it at the same time | It moved back, or moved too fast |

**You need:** A box. First put it 20 cm straight in front of the sensor (both lasers must see it), then move it away **to the left or to the right**.

<a href="/examples/tof2/arduino-en/tof2_05_leave_direction/tof2_05_leave_direction.ino" download data-ignore="true">📄 Download tof2_05_leave_direction.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_05_leave_direction/tof2_05_leave_direction.ino -->
```cpp
/*
 * Ranger 2-Way x Arduino  Example 5: Which way did it slip away?
 * ------------------------------------------------------------
 * What does this example do?
 *   This is the Ranger 2-Way's best trick: telling whether a thing left "to the left" or "to the right".
 *   The idea is simple: when a thing moves away to the right, the left laser loses it "first",
 *   and only the right laser still sees it. When the right one loses it too, we know it went right.
 *     Only the right one sees it at the end -> it left to the right ->
 *     Only the left one sees it at the end  -> it left to the left  <-
 *   When a sumo robot's opponent runs away, the robot knows which way to turn and chase.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   You don't need to add a light. The light on the board is enough (it turns on when something is seen).
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put a box 20 cm straight in front of the sensor (both lasers must see it),
 *   then move the box away to the left or to the right. The computer tells you which way it went.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards don't say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint8_t LED = LED_BUILTIN;         // The light on the board (pin 13 on the UNO)
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

// ---- 👉 You can change these settings ----
const uint16_t SEE_DIST = 500;           // How close counts as "seeing" (mm): 500 = 50 cm, so a wall far behind is not taken as the target
const unsigned long GONE_MS = 150;       // How long (milliseconds) neither side sees it before it really counts as gone

// Who sees it now: 0 = nobody, 1 = only left, 2 = only right, 3 = both
uint8_t state = 0;
uint8_t lastOneSide = 0;                 // Before it vanished, which side "was the only one" seeing it: 0 = none (both lost it at once), 1 = left, 2 = right
bool present = false;                    // Is something in front right now
unsigned long goneSince = 0;             // Since when neither side sees it

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  // While the lasers start, distances are 0 (that would look like "both sides see it"), so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Put a box straight in front, then move it away to the left or right."));
}

void loop() {
  // Box 0x11: left distance (2 numbers) + right distance (2 numbers)
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }
  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  // "Sees it" = not broken, and close enough
  bool seeL = (left  != FAULT) && (left  < SEE_DIST);
  bool seeR = (right != FAULT) && (right < SEE_DIST);

  // Turn what both sides see into one number: left sees it +1, right sees it +2
  uint8_t now = (seeL ? 1 : 0) + (seeR ? 2 : 0);

  // ---- 1. If the situation changed, print it ----
  if (now != state) {
    state = now;
    if (state == 3)      Serial.println(F("  L ● ● R   Both sides see it (straight ahead)"));
    else if (state == 1) Serial.println(F("  L ● ○ R   Only the left sees it"));
    else if (state == 2) Serial.println(F("  L ○ ● R   Only the right sees it"));
    else                 Serial.println(F("  L ○ ○ R   Neither side sees it"));
  }

  // ---- 2. Something is seen: remember "which side was the last one left" ----
  if (state != 0) {
    if (!present) {
      present = true;
      Serial.println(F("Found something!"));
    }
    if (state == 3) lastOneSide = 0;          // Both see it -> it has not started leaving
    else            lastOneSide = state;      // Only one side sees it -> remember which side
    goneSince = millis();                     // Still seen, so restart the timer
  }

  // ---- 3. Neither side has seen it for a while: tell which way it left ----
  if (state == 0 && present && millis() - goneSince >= GONE_MS) {
    present = false;
    if (lastOneSide == 1)      Serial.println(F("←←← It left to the LEFT! (the right laser lost it first)"));
    else if (lastOneSide == 2) Serial.println(F("→→→ It left to the RIGHT! (the left laser lost it first)"));
    else                       Serial.println(F("It vanished from both sides at once (maybe it moved back, or moved too fast)"));
    Serial.println();
  }

  digitalWrite(LED, present ? HIGH : LOW);    // Light on when something is there

  delay(30);                                  // Read fast, so we can catch "which side was first"
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

**You will see:** (the box starts straight ahead, then moves away to the right)

```text
Ready! Put a box straight in front, then move it away to the left or right.
  L ● ● R   Both sides see it (straight ahead)
Found something!
  L ○ ● R   Only the right sees it
  L ○ ○ R   Neither side sees it
→→→ It left to the RIGHT! (the left laser lost it first)
```

> [!TIP]
> **Try it:** If you keep getting "vanished from both sides at once", move the box more slowly, or use a wider box. Once you get it, add code that turns the robot right after the "left to the RIGHT" line, and you have a sumo robot that chases its opponent.

---

## ❓ FAQ

**Q: It keeps saying "Sensor not found"?**
1. Check that SDA and SCL are not swapped (UNO: SDA → A4, SCL → A5). On the new board the solder pads are **1 = SCL, 2 = SDA**, the opposite of what the old board says. Try swapping the two wires first.
2. Check that VCC and GND are plugged in firmly. Is the light on the sensor on?
3. Make sure it is the **Universal I2C edition** (new board). The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.

**Q: The Serial Monitor shows strange characters?**
The speed is not set to **115200**. Change it at the bottom right of the Serial Monitor and it will be fine.

**Q: The distances are 0 right after power-on?**
The sensor is still waking up the lasers, and all data is 0 during this time. The examples already wait with `mbcWaitReady(6000)`. In your own code, also wait until bit 0 of box `0x32` becomes 1 before you start reading.

**Q: One side keeps showing "FAULT"?**
Turn the power off, plug everything in again and try once more. If it is still the same, run [Example 1](#ex1) to see which side did not start, then [contact us](/en/contact.md) for repair.

**Q: Something is clearly in front, but it says "nothing there"?**
The Ranger measures up to 120 cm. Also, things like **black velvet, clear glass and shiny mirrors** reflect very little laser light, or reflect it in odd directions, so they are hard to measure. Try a white or light-colored object.

**Q: Can I connect it together with a Ranger 8-Way, or a VL53L0X laser module from a shop?**
No. They all use address `0x29`, and only one of them can be on the same SDA/SCL wires. You can share the wires with MBC products that use a different address (for example the Pathfinder line sensor at `0x16`).

**Q: I want to know what data each box holds?**

| Box | How many to read | What is inside |
| :---: | :---: | :--- |
| `0x01` | 17 | Version info (numbers 9 ~ 16 are the name `LASER2`) |
| `0x11` | 4 | Left distance, right distance (2 numbers each, big half first, unit mm) |
| `0x31` | 9 | Status (number 0: bit 0 = ready, bit 1 = a laser is broken; number 2: which lasers started OK) |

For the shared rules, see the [MBC Universal I2C Protocol](/en/i2c-protocol.md).
