<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 Ranger 2-Way × Arduino Examples

[🔙 Back to Ranger 2-Way Home](/en/sensors/tof2/index.md)　|　[📡 MBC Universal I2C Protocol (advanced)](/en/i2c-protocol.md)

This page has **6 product examples + 3 shared examples**, from "is it connected?" all the way to "stop precisely with mm". You can copy and upload each example as it is, and every line of the code has an English comment. **Never written code before? No problem.** Just do them one by one, in order.

> [!TIP]
> First time using Arduino? Read [🔰 First Time with Arduino: Start from Zero](/en/sensors/arduino-start.md) first. In 15 minutes you learn how to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/tof2/arduino-en/tof2_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
<a href="/examples/i2c/arduino-en/i2c_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #5b6cff, #2a2f8f); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download shared command examples (ZIP)</a>
</div>

---

## 🔌 Wiring (the same for every example)

| Sensor | Arduino UNO / Nano | ESP32 | Arduino UNO R4 |
| :---: | :---: | :---: | :---: |
| VCC | 5V | 5V (VIN) | 5V |
| GND | GND | GND | GND |
| SDA | A4 | GPIO 21 | SDA |
| SCL | A5 | GPIO 22 | SCL |

Figure: `/images/figures/tof2/wiring-uno.svg`

> [!IMPORTANT]
> - Use the **universal I2C edition** of the Ranger 2-Way. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
> - The two lasers take 1 ~ 3 seconds to boot. Every example waits for `F3` "ready" at the start.
> - "Left" and "right" always mean: **the sensor is mounted at the front of the robot (lasers pointing forward), and you stand behind the robot looking forward**.
> - Unplug the USB cable before you connect the wires.

---

<h2 id="cmd-table">📖 Command Overview (protocol v2)</h2>

Think of the sensor as a row of numbered "boxes". Each box holds one 16-bit number (int16, little-endian). To read: write `[number]` → wait 2 ms → read 2×k bytes, and you get k boxes in a row starting from that number (k ≤ 16). A number that does not exist returns `-32768`. The house number (I2C address) is `0x29`.

<!-- gen:cmd-table -->
| No. | Name | Range / Unit | Description | Used in |
| :---: | :--- | :--- | :--- | :--- |
| `01` | Left distance | 0 ~ 120 cm | Distance from the left laser; 120 = nothing in front / that laser failed | [Example 2](#ex2), [3](#ex3) |
| `02` | Right distance | 0 ~ 120 cm | Distance from the right laser; 120 = nothing / failed | [Example 2](#ex2), [3](#ex3) |
| `03` | Nearest distance | 0 ~ 120 cm | The nearer of left and right | [Example 3](#ex3), [5](#ex5) |
| `04` | Left distance | 0 ~ 1200 mm | Left laser, down to 1 mm; 1200 = nothing / failed | [Example 2](#ex2), [4](#ex4) |
| `05` | Right distance | 0 ~ 1200 mm | Right laser, down to 1 mm | [Example 2](#ex2), [4](#ex4) |
| `06` | Init bitmap | 0 ~ 3 (bits) | bit0 = left, bit1 = right, 1 = that laser initialized OK at boot; 3 = both OK | [Example 2](#ex2), [6](#ex6) |
| `07` | Fault bitmap | 0 ~ 3 (bits) | bit0 = left, bit1 = right, 1 = that laser has failed now (cannot be read, or not updated for 200 ms) | [Example 2](#ex2), [6](#ex6) |
| `08` | Average of left and right | 0 ~ 1200 mm | (left mm + right mm) / 2; the same as EV3 mode 3 and SPIKE mode 6 | [Example 2](#ex2), [4](#ex4) |
| `F0` | Device ID | — | Low byte = factory address 0x29, high byte = 2 (number of lasers) | [Example 1](#ex1), [Shared 2](#exi2), [Shared 3](#exi3) |
| `F1` | Protocol version | 2 | These examples are written for version 2 | [Example 1](#ex1) |
| `F2` | Firmware version | major×100 + minor | e.g. 102 = 1.2 | [Example 1](#ex1) |
| `F3` | Status flags | bits | b0 ready (both lasers initialized), b1 a laser failed | [Example 1](#ex1), [6](#ex6) |
| `F4` | Update counter | 0 ~ 255 | +1 every 10 ms, keeps changing = the sensor is alive | [Shared 2](#exi2) |
| `F5` | Seconds since boot | 0 ~ 32767 s | Got smaller = it rebooted | [Example 1](#ex1), [Shared 2](#exi2) |
| `F8` | Change I2C address (command) | `[new address][inverted]` | `[F8][00][FF]` = back to factory; takes effect after reboot | [Shared 1](#exi1) |
| `EF` | Reboot (command) | `[A5]` | Send `[EF][A5]` | [Shared 1](#exi1), [Shared 2](#exi2) |
<!-- /gen -->

The Ranger 2-Way has no product-specific write commands (E0 ~ EE).

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the sensor](#ex1) | Check the wiring, read the F0 ~ F3 ID card and status | Nothing |
| 2 | [See all the values at once](#ex2) | What every box 01 ~ 08 in the value table means | Your hand |
| 3 | [Is it nearer on the left or the right?](#ex3) | The three boxes 01 / 02 / 03, drawn as bars | A box |
| 4 | [Stop precisely with mm (advanced)](#ex4) | 04 / 05 / 08 in mm, stop exactly at 200 mm | Two-wheel car, L298N motor driver board |
| 5 | [Parking sensor alarm](#ex5) | Control the LED and buzzer with 03 nearest distance | Buzzer (optional) |
| 6 | [Laser health check](#ex6) | 06 / 07 init and fault bitmaps, F3 | Nothing |
| Shared 1 | [Change the I2C address](#exi1) | F8 change address / back to factory, EF reboot | Nothing |
| Shared 2 | [ID card + reboot](#exi2) | The 6 shared boxes F0 ~ F5 | Nothing |
| Shared 3 | [Scan the devices on the bus](#exi3) | Scan 0x08 ~ 0x77, tell the product from F0 | Nothing |

> [!TIP]
> **Suggested order: 1 → 2 → 3 → 6 → 5 → 4.** First check the wiring (1) and get to know every box (2). Then build the applications.

---

<h2 id="ex1">1️⃣ Say hello to the sensor</h2>

**What it does:** Checks that the sensor is connected and reads its ID card: `F0` device ID (factory address + number of lasers), `F1` protocol version, `F2` firmware version, and `F3` status flags printed bit by bit in plain English. Run this one first every time you connect a new sensor.

**Steps:**
1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.

<a href="/examples/tof2/arduino-en/tof2_01_hello/tof2_01_hello.ino" download data-ignore="true">📄 Download tof2_01_hello.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_01_hello/tof2_01_hello.ino -->
```cpp
/*
 * Ranger 2-Way × Arduino   Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   Checks that the sensor is connected. If it is, it prints the sensor's ID card
 *   (device ID, protocol version, firmware version) and its current status
 *   (are both lasers working?).
 *   Run this example first every time you connect a new sensor.
 *
 * You need: Ranger 2-Way (universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the sensor

const uint8_t ADDR = 0x29;               // The sensor's "house number" (I2C address). Both 2-Way and 8-Way use 0x29
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // Open the channel to the computer at 115200 baud
  mbcBegin();                            // Get I2C ready (helper at the bottom of the file)
  delay(500);                            // Wait 0.5 s for the sensor to boot

  Serial.println(F("=== Ranger 2-Way: Say hello to the sensor ==="));
  Serial.println(F("Waiting for both lasers to boot (at most 4 s)..."));

  // Wait for the sensor to say "I'm ready" (bit0 of status flags F3 = both lasers initialized), at most 4 s
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found! Please check:"));
    Serial.println(F("  1. SDA to A4, SCL to A5 (ESP32: 21 / 22). Are they swapped?"));
    Serial.println(F("  2. VCC to 5V, GND to GND. Are they plugged in firmly?"));
    Serial.println(F("  3. Is your sensor the universal I2C edition? (SPIKE and EV3 editions do not work with Arduino)"));
    while (true) {}                      // Not found: stop here
  }

  // ---- F0 device ID: low byte = factory address, high byte = number of lasers ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t factoryAddr = (uint8_t)(id & 0xFF);        // Factory address (stays the same even if the address was changed)
  uint8_t sensors     = (uint8_t)((id >> 8) & 0xFF); // hw[0] = how many lasers
  Serial.print(F("Found it! Factory address 0x"));
  Serial.print(factoryAddr, HEX);
  Serial.print(F(", "));
  Serial.print(sensors);                 // Ranger 2-Way = 2
  Serial.println(F(" lasers"));

  // ---- F1 protocol version (this example is written for version 2) ----
  Serial.print(F("Protocol version: "));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 firmware version: major×100 + minor (e.g. 102 = 1.2) ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("Firmware version: "));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 status flags: each bit means one thing ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("Current status:"));
  Serial.print(F("  bit0 ready          ")); Serial.println((st & 0x01) ? F("yes (both lasers initialized)") : F("no"));
  Serial.print(F("  bit1 a laser failed ")); Serial.println((st & 0x02) ? F("yes! (run Example 6 to see which one)") : F("no (both lasers OK)"));

  // ---- F5 seconds since boot ----
  Serial.print(F("Seconds since boot: "));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("Wiring is correct. You can try the other examples now!"));
}

void loop() {
  // This example only checks once at startup, so loop does nothing
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
=== Ranger 2-Way: Say hello to the sensor ===
Waiting for both lasers to boot (at most 4 s)...
Found it! Factory address 0x29, 2 lasers
Protocol version: 2
Firmware version: 2.0
Current status:
  bit0 ready          yes (both lasers initialized)
  bit1 a laser failed no (both lasers OK)
Seconds since boot: 5
Wiring is correct. You can try the other examples now!
```

> [!NOTE]
> **What are the "Communication helpers"?** At the bottom of every example there is the same piece of code. It "talks" to the sensor. You don't need to change it or understand it. Just remember:
> - `mbcRead(address, number)`: read one box; returns `MBC_NONE` (−32768) if it cannot be read
> - `mbcReadMany(address, first number, how many, array)`: read several boxes in a row starting from the first number (at most 16), all from the same moment
> - `mbcWrite(address, command, parameters, how many parameters)`: send a command; the last two can be left out when there are no parameters
> - `mbcWaitReady(address, max wait in ms)`: wait for `F3` bit0 "ready"

---

<h2 id="ex2">2️⃣ See all the values at once</h2>

**What it does:** Prints the whole value table `01` ~ `08` every 0.5 s with one `mbcReadMany`. Get to know every box here first; the later examples only pick a few of them.

<a href="/examples/tof2/arduino-en/tof2_02_all_values/tof2_02_all_values.ino" download data-ignore="true">📄 Download tof2_02_all_values.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_02_all_values/tof2_02_all_values.ino -->
```cpp
/*
 * Ranger 2-Way × Arduino   Example 2: See all the values at once
 * ------------------------------------------------------------
 * What does this example do?
 *   Prints the sensor's whole "value table" every 0.5 s:
 *     01 left cm, 02 right cm, 03 nearest cm, 04 left mm, 05 right mm,
 *     06 init bitmap, 07 fault bitmap, 08 average of left and right in mm.
 *   The later examples only pick a few of these boxes. Get to know every box here first.
 *
 * You need: Ranger 2-Way (universal I2C edition) + your hand or a box.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move your hand in front of the left and right lasers.
 *
 * Good to know:
 *   Numbers 01~08 are in a row, so we read all 8 values at once (mbcReadMany). They are all from the same moment.
 *   120 cm / 1200 mm = nothing in front (or that laser failed).
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand in front of the left and right lasers."));
}

void loop() {
  int16_t a[8];                          // Holds the 8 boxes 01~08
  if (!mbcReadMany(ADDR, 0x01, 8, a)) { Serial.println(F("Cannot read")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 left distance    ")); Serial.print(a[0]); Serial.println(F(" cm   (0~120, 120 = nothing there)"));
  Serial.print(F("02 right distance   ")); Serial.print(a[1]); Serial.println(F(" cm"));
  Serial.print(F("03 nearest distance ")); Serial.print(a[2]); Serial.println(F(" cm   (the nearer of left and right)"));
  Serial.print(F("04 left distance    ")); Serial.print(a[3]); Serial.println(F(" mm   (0~1200, 10 times finer than cm)"));
  Serial.print(F("05 right distance   ")); Serial.print(a[4]); Serial.println(F(" mm"));
  Serial.print(F("06 init bitmap      ")); Serial.print(a[5]); Serial.println(F("     (bit0 left, bit1 right; 3 = both lasers booted OK)"));
  Serial.print(F("07 fault bitmap     ")); Serial.print(a[6]); Serial.println(F("     (bit0 left, bit1 right; 0 = both OK)"));
  Serial.print(F("08 average          ")); Serial.print(a[7]); Serial.println(F(" mm   ((left mm + right mm) / 2)"));

  delay(50);                             // Rest a bit after reading
  delay(450);                            // Update every 0.5 s so it is easy to read
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:** (hand 30 cm away on the left, nothing on the right)

```text
---------------------------------------
01 left distance    30 cm   (0~120, 120 = nothing there)
02 right distance   120 cm
03 nearest distance 30 cm   (the nearer of left and right)
04 left distance    304 mm   (0~1200, 10 times finer than cm)
05 right distance   1200 mm
06 init bitmap      3     (bit0 left, bit1 right; 3 = both lasers booted OK)
07 fault bitmap     0     (bit0 left, bit1 right; 0 = both OK)
08 average          752 mm   ((left mm + right mm) / 2)
```

> [!TIP]
> **Try it:** Hold a sheet of paper at an angle in front of the two lasers and see how much `04` and `05` differ. This is how you measure whether a wall is at an angle.

---

<h2 id="ex3">3️⃣ Is it nearer on the left or the right?</h2>

**What it does:** Reads three boxes, `01` left cm, `02` right cm and `03` nearest cm, draws the two lasers' distances as two bars, and tells you on which side the object is nearer.

**You need:** A box or your hand.

<a href="/examples/tof2/arduino-en/tof2_03_left_right/tof2_03_left_right.ino" download data-ignore="true">📄 Download tof2_03_left_right.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_03_left_right/tof2_03_left_right.ino -->
```cpp
/*
 * Ranger 2-Way × Arduino   Example 3: Is it nearer on the left or the right?
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads three boxes, "01 left cm", "02 right cm" and "03 nearest cm", draws the two
 *   lasers' distances as two bars, and tells you on which side the object is nearer.
 *   These are the three boxes used most for avoiding obstacles and finding an opponent.
 *
 * You need: Ranger 2-Way (universal I2C edition) + a box or your hand.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move your hand from left to right and watch the bars change.
 *
 * Which side is left and right?
 *   The sensor is mounted at the front of the robot (lasers pointing forward), and you stand behind the robot looking forward.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand from left to right. (Longer bar = farther away)"));
}

void loop() {
  int16_t v[3];                          // v[0] = 01 left cm, v[1] = 02 right cm, v[2] = 03 nearest cm
  if (!mbcReadMany(ADDR, 0x01, 3, v)) { Serial.println(F("Cannot read the sensor")); delay(500); return; }
  int16_t left = v[0], right = v[1], nearest = v[2];

  Serial.print(F("L "));
  printBar(left);
  Serial.print(F("  R "));
  printBar(right);

  if (nearest >= 120)        Serial.println(F("  → Nothing within 120 cm"));
  else if (left < right)     { Serial.print(F("  → Nearer on the left: ")); Serial.print(nearest); Serial.println(F(" cm")); }
  else if (right < left)     { Serial.print(F("  → Nearer on the right: ")); Serial.print(nearest); Serial.println(F(" cm")); }
  else                       { Serial.print(F("  → Straight ahead: "));     Serial.print(nearest); Serial.println(F(" cm")); }

  delay(50);
  delay(150);                            // Print one line every 0.2 s   👉 You can change this
}

// Draw 0~120 cm as a bar of 0~12 blocks
void printBar(int16_t cm) {
  int8_t n = cm / 10;
  if (n > 12) n = 12;
  Serial.print('[');
  for (int8_t i = 0; i < 12; i++) Serial.print(i < n ? '#' : '.');
  Serial.print(']');
  if (cm < 10)  Serial.print(' ');
  if (cm < 100) Serial.print(' ');
  Serial.print(cm);
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:** (moving your hand from left to right)

```text
L [###.........] 35  R [############]120  → Nearer on the left: 35 cm
L [###.........] 32  R [###.........] 32  → Straight ahead: 32 cm
L [############]120  R [####........] 41  → Nearer on the right: 41 cm
L [############]120  R [############]120  → Nothing within 120 cm
```

---

<h2 id="ex4">4️⃣ Stop precisely with mm (advanced)</h2>

**What it does:** Uses `04` left mm, `05` right mm and `08` average mm (down to 1 mm) to drive a car toward a wall and stop exactly at `STOP_MM` (default 200 mm). It slows down automatically as it gets close. You can also run it without a car and just watch the numbers in the Serial Monitor.

**You also need:** A two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), a battery, a wall.

**Motor wiring:**

| L298N | Arduino UNO / Nano / R4 | ESP32 | Controls |
| :---: | :---: | :---: | :--- |
| ENA | 5 | GPIO 25 | Left wheel speed |
| IN1 / IN2 | 7 / 8 | GPIO 26 / 27 | Left wheel direction |
| ENB | 6 | GPIO 14 | Right wheel speed |
| IN3 / IN4 | 9 / 10 | GPIO 32 / 33 | Right wheel direction |
| GND | GND | GND | **Must share GND with the Arduino** |

<a href="/examples/tof2/arduino-en/tof2_04_mm_precise/tof2_04_mm_precise.ino" download data-ignore="true">📄 Download tof2_04_mm_precise.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_04_mm_precise/tof2_04_mm_precise.ino -->
```cpp
/*
 * Ranger 2-Way × Arduino   Example 4 (advanced): Stop precisely with mm
 * ------------------------------------------------------------
 * What does this example do?
 *   Uses the three boxes "04 left mm", "05 right mm" and "08 average mm" (down to 1 mm)
 *   to drive a car forward and stop exactly STOP_MM (default 200 mm = 20 cm) from a wall.
 *   It slows down as it gets close, so it will not overshoot. You can also run it without
 *   a car: just watch the numbers in the Serial Monitor.
 *
 * You need: a two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), a battery, a wall or a box.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      (left wheel)
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     (right wheel)
 *   (ESP32 instead: ENA → 25, IN1 → 26, IN2 → 27, ENB → 14, IN3 → 32, IN4 → 33)
 *   The GND of the L298N must be connected to the GND of the Arduino! Mount the sensor at the front, lasers pointing forward.
 *
 * How to see the result:
 *   Serial Monitor at 115200. When the car stops, the distance printed should be very close to STOP_MM.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- Motor pins ----
#if defined(ESP32)
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel (pins 6 ~ 11 cannot be used on ESP32)
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 Parameters you can change ----
const int16_t STOP_MM    = 200;          // Stop this many mm from the wall (200 = 20 cm)
const int16_t SLOW_MM    = 500;          // Start slowing down within this many mm of the wall
const int     FAST_SPEED = 150;          // Full speed 0~255
const int     SLOW_SPEED = 70;           // Slowest crawling speed (too slow and the motors will not move)
const bool    LEFT_REVERSE  = false;     // Change to true if the left wheel turns the wrong way
const bool    RIGHT_REVERSE = false;     // Change to true if the right wheel turns the wrong way

bool stopped = false;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.print(F("Starting in 3 s. Goal: stop at ")); Serial.print(STOP_MM); Serial.println(F(" mm"));
  delay(3000);
}

void loop() {
  int16_t v[5];                          // 04 left mm, 05 right mm, 06, 07, 08 average mm
  if (!mbcReadMany(ADDR, 0x04, 5, v)) {  // Read 5 boxes in a row starting at 04 (04~08)
    drive(0, 0);                         // Cannot read → stop the car for safety
    delay(50);
    return;
  }
  int16_t leftMm = v[0], rightMm = v[1], avgMm = v[4];
  int16_t dist = (leftMm < rightMm) ? leftMm : rightMm;   // Use the nearer laser, so it does not hit the wall at an angle

  Serial.print(F("L ")); Serial.print(leftMm);
  Serial.print(F(" mm  R ")); Serial.print(rightMm);
  Serial.print(F(" mm  avg ")); Serial.print(avgMm);
  Serial.print(F(" mm  "));

  if (stopped || dist <= STOP_MM) {
    drive(0, 0);
    if (!stopped) { stopped = true; Serial.println(F("→ Arrived, stop!")); }
    else Serial.println(F("(stopped)"));
  } else if (dist <= SLOW_MM) {
    // The nearer, the slower: from SLOW_MM to STOP_MM the speed goes from FAST down to SLOW
    int speed = map(dist, STOP_MM, SLOW_MM, SLOW_SPEED, FAST_SPEED);
    drive(speed, speed);
    Serial.print(F("→ Slowing down ")); Serial.println(speed);
  } else {
    drive(FAST_SPEED, FAST_SPEED);
    Serial.println(F("→ Full speed ahead"));
  }
  delay(50);
}

void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
Starting in 3 s. Goal: stop at 200 mm
L 812 mm  R 805 mm  avg 808 mm  → Full speed ahead
L 486 mm  R 480 mm  avg 483 mm  → Slowing down 139
L 231 mm  R 226 mm  avg 228 mm  → Slowing down 77
L 199 mm  R 196 mm  avg 197 mm  → Arrived, stop!
```

> [!TIP]
> If it stops too far away, increase `SLOW_SPEED`; if it stops too close (hits the wall), increase `SLOW_MM`. The program uses "the nearer laser", so the car will not hit the wall even when it approaches at an angle.

---

<h2 id="ex5">5️⃣ Parking sensor alarm</h2>

**What it does:** Reads `03` nearest distance and uses the board's built-in LED (**L**) and a buzzer as a parking sensor: nothing there = quiet, something there = blink / beep, **the nearer, the faster**, and within 10 cm it stays on.

**You need:** An active buzzer (optional): `+` → 8 (ESP32 → 27), `−` → GND.

<a href="/examples/tof2/arduino-en/tof2_05_distance_alarm/tof2_05_distance_alarm.ino" download data-ignore="true">📄 Download tof2_05_distance_alarm.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_05_distance_alarm/tof2_05_distance_alarm.ino -->
```cpp
/*
 * Ranger 2-Way × Arduino   Example 5: Parking sensor alarm
 * ------------------------------------------------------------
 * What does this example do?
 *   Uses the board's built-in LED (L) and a buzzer (optional) as a parking sensor:
 *   reads "03 nearest distance cm". Nothing there = quiet. Something there = blink / beep,
 *   faster the nearer it gets. Within STOP_CM it stays on and keeps beeping.
 *
 * You need: Ranger 2-Way (universal I2C edition); the buzzer is optional (if you have one, use an "active buzzer").
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   Buzzer + → 8 (ESP32 → 27)   Buzzer − → GND
 *
 * How to see the result:
 *   Move your hand slowly toward the sensor. The L LED (and the buzzer) blink faster and faster; the Serial Monitor at 115200 also prints the distance.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // On most ESP32 boards the built-in LED is on GPIO 2
#endif

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

#if defined(ESP32)
const int BUZZER = 27;                   // Buzzer pin (ESP32)
#else
const int BUZZER = 8;                    // Buzzer pin (UNO)
#endif

// ---- 👉 Parameters you can change ----
const int16_t ALARM_START_CM = 80;       // Start the alarm within this many cm
const int16_t STOP_CM        = 10;       // Within this many cm: stay on and keep beeping

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand slowly toward the sensor and watch the L LED blink faster."));
}

void loop() {
  int16_t cm = mbcRead(ADDR, 0x03);      // 03 = the nearer distance of the two lasers (cm)
  if (cm == MBC_NONE) { delay(100); return; }

  if (millis() - lastPrint >= 500) {     // Print the distance every 0.5 s
    lastPrint = millis();
    if (cm >= 120) Serial.println(F("Nothing there"));
    else { Serial.print(F("Nearest object: ")); Serial.print(cm); Serial.println(F(" cm")); }
  }

  if (cm >= ALARM_START_CM) {            // Too far: quiet
    alarm(false);
    delay(50);
  } else if (cm <= STOP_CM) {            // Too near: stay on
    alarm(true);
    delay(50);
  } else {                               // In between: the nearer, the faster it blinks (gap 50 ~ 400 ms)
    int gap = map(cm, STOP_CM, ALARM_START_CM, 50, 400);
    alarm(true);  delay(gap / 2);
    alarm(false); delay(gap / 2);
  }
}

void alarm(bool on) {
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  digitalWrite(BUZZER, on ? HIGH : LOW);
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:** (hand moving slowly closer)

```text
Ready! Move your hand slowly toward the sensor and watch the L LED blink faster.
Nothing there
Nearest object: 65 cm
Nearest object: 30 cm
Nearest object: 8 cm
```

---

<h2 id="ex6">6️⃣ Laser health check</h2>

**What it does:** Reads `06` init bitmap, `07` fault bitmap and `F3`, and reports every second whether both lasers are healthy. A failed laser always returns 120 / 1200, which looks like "nothing there", so you need these two boxes to tell the difference. Run it before a match.

<a href="/examples/tof2/arduino-en/tof2_06_fault_check/tof2_06_fault_check.ino" download data-ignore="true">📄 Download tof2_06_fault_check.ino</a>

<!-- ino: examples/tof2/arduino-en/tof2_06_fault_check/tof2_06_fault_check.ino -->
```cpp
/*
 * Ranger 2-Way × Arduino   Example 6: Laser health check
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads "06 init bitmap", "07 fault bitmap" and "F3 status flags", and reports
 *   every second whether both lasers are healthy. Run it before a match to know if the sensor is broken.
 *     06: bit0 = left, bit1 = right, 1 = that laser initialized OK at boot (if it failed, there is no retry; power off and on again)
 *     07: bit0 = left, bit1 = right, 1 = that laser has failed now (cannot be read, or not updated for 200 ms)
 *   A failed laser always returns 120 cm / 1200 mm, which looks like "nothing there". These two boxes tell the difference.
 *
 * You need: Ranger 2-Way (universal I2C edition).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== Ranger 2-Way: Laser health check ==="));
  // This example does not wait for "ready" on purpose, so you can still see in 06 which laser did not boot
  delay(3000);                           // Both lasers take at most about 3 s to boot
}

void loop() {
  int16_t v[2];                          // v[0] = 06 init, v[1] = 07 fault
  int16_t st = mbcRead(ADDR, 0xF3);
  if (!mbcReadMany(ADDR, 0x06, 2, v) || st == MBC_NONE) {
    Serial.println(F("Cannot read the sensor at all: check the wiring, or is it the SPIKE / EV3 edition?"));
    delay(1000);
    return;
  }
  int16_t initOk = v[0], fault = v[1];

  Serial.println(F("---------------------------"));
  Serial.print(F("F3 ready "));       Serial.print((st & 0x01) ? F("yes") : F("no"));
  Serial.print(F("  a laser failed "));  Serial.println((st & 0x02) ? F("yes") : F("no"));

  reportOne(F("Left"), initOk & 0x01, fault & 0x01);
  reportOne(F("Right"), initOk & 0x02, fault & 0x02);

  if (initOk == 3 && fault == 0) Serial.println(F("Both lasers are healthy!"));
  else Serial.println(F("Problem: power off and on first; if it is still the same, check whether that laser is covered or its connector is loose"));

  delay(50);
  delay(950);
}

// Print the status of one laser
void reportOne(const __FlashStringHelper *name, int16_t ok, int16_t bad) {
  Serial.print(F("  "));
  Serial.print(name);
  Serial.print(F(" laser: init "));
  Serial.print(ok ? F("OK") : F("failed"));
  Serial.print(F(", now "));
  Serial.println(bad ? F("FAILED!") : F("OK"));
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
=== Ranger 2-Way: Laser health check ===
---------------------------
F3 ready yes  a laser failed no
  Left laser: init OK, now OK
  Right laser: init OK, now OK
Both lasers are healthy!
```

---

## 🔧 Shared command examples (the same for every MBC universal I2C product)

These three are in `examples/i2c/arduino-en/`. They work with the Pathfinder, Ranger, Controller, IMU and Sharpshooter. Just change `ADDR` / `OLD_ADDR` at the top of the program to `0x29`.

<h3 id="exi1">Shared 1: Change the I2C address (F8)</h3>

**What it does:** When two Ranger 2-Ways must share one bus (for example one at the front and one at the back of the car), use `[F8][new address][inverted new address]` to change one of them (saved to EEPROM); `[F8][00][FF]` goes back to factory. After the change it sends `[EF][A5]` to reboot and scans again to confirm.

<a href="/examples/i2c/arduino-en/i2c_01_change_address/i2c_01_change_address.ino" download data-ignore="true">📄 Download i2c_01_change_address.ino</a>

<!-- ino: examples/i2c/arduino-en/i2c_01_change_address/i2c_01_change_address.ino -->
```cpp
/*
 * MBC universal I2C × Arduino   Shared example 1: Change the I2C address (F8)
 * ------------------------------------------------------------
 * What does this example do?
 *   When two MBC sensors with the same "house number" must share one I2C bus (for example two Ranger 2-Way,
 *   both at 0x29), change one of them to another address first. How:
 *     write [F8][new address][inverted new address]  → saved in the sensor's EEPROM, takes effect after reboot
 *     write [F8][0x00][0xFF]                         → back to the factory address
 *   After the change it automatically sends [EF][0xA5] to reboot, then scans again to confirm the new address shows up.
 *
 * You need: any MBC universal I2C edition sensor (connect only one at a time!).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to use:
 *   1. Below, change OLD_ADDR to the sensor's current address and NEW_ADDR to the address you want (0x08~0x77).
 *   2. Upload, open the Serial Monitor at 115200, type y and Enter to change; type r and Enter to go back to factory.
 *   3. From now on, change ADDR in every other example to the new address.
 *
 * Tip:
 *   The low byte of F0 "device ID" is always the factory address, even after a change, so a scan can still tell which product it is.
 */

#include <Wire.h>

const uint8_t OLD_ADDR = 0x29;           // The sensor's current address   👉 Change this (Pathfinder 0x16, Ranger 0x29, Controller 0x42, IMU 0x6A, Sharpshooter 0x4C)
const uint8_t NEW_ADDR = 0x2A;           // The address you want           👉 Change this (0x08 ~ 0x77, must not clash with other devices)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC universal I2C: Change address ==="));
  Serial.print(F("MBC devices on the bus now:")); scan();
  if (!mbcWaitReady(OLD_ADDR, 6000)) {
    Serial.print(F("No sensor found at 0x")); Serial.print(OLD_ADDR, HEX); Serial.println(F(". Please check OLD_ADDR and the wiring."));
    while (true) {}
  }
  Serial.print(F("Found 0x")); Serial.print(OLD_ADDR, HEX);
  Serial.print(F(" (factory address 0x")); Serial.print(mbcRead(OLD_ADDR, 0xF0) & 0xFF, HEX); Serial.println(')');
  Serial.print(F("Type y to change to 0x")); Serial.print(NEW_ADDR, HEX); Serial.println(F("; type r to go back to the factory address. Then press Enter"));
}

void loop() {
  if (!Serial.available()) { delay(50); return; }
  char c = Serial.read();
  if (c == 'y') {
    if (NEW_ADDR < 0x08 || NEW_ADDR > 0x77) { Serial.println(F("NEW_ADDR must be between 0x08 and 0x77")); return; }
    uint8_t p[2] = { NEW_ADDR, (uint8_t)~NEW_ADDR };   // [new address][inverted new address]: both must match, so a mistake cannot change it
    Serial.print(F("Changing address → 0x")); Serial.println(NEW_ADDR, HEX);
    if (!mbcWrite(OLD_ADDR, 0xF8, p, 2)) { Serial.println(F("Write failed")); return; }
    rebootAndCheck(NEW_ADDR);
  } else if (c == 'r') {
    uint8_t p[2] = { 0x00, 0xFF };       // [F8][00][FF] = back to the factory address
    Serial.println(F("Back to the factory address"));
    if (!mbcWrite(OLD_ADDR, 0xF8, p, 2)) { Serial.println(F("Write failed")); return; }
    rebootAndCheck(0);                   // 0 = factory address unknown, just scan and see
  }
}

// Send the reboot command, wait for it to come back, then scan again
void rebootAndCheck(uint8_t expect) {
  delay(50);                             // Give the EEPROM a moment to finish writing
  uint8_t magic = 0xA5;
  mbcWrite(OLD_ADDR, 0xEF, &magic, 1);   // [EF][A5] = reboot (the new address only takes effect after reboot)
  Serial.println(F("Rebooting..."));
  delay(3000);                           // Ranger 8-Way needs 2~4 s to boot
  Serial.print(F("Scan result:")); scan();
  if (expect && mbcRead(expect, 0xF3) != MBC_NONE) {
    Serial.print(F("Success! The sensor is now at 0x")); Serial.println(expect, HEX);
    Serial.println(F("Remember to change ADDR in the other examples to the new address."));
  } else if (expect) {
    Serial.println(F("No answer at the new address. Power off and on, then scan again (some boards only update I2C after a power cycle)."));
  }
  Serial.println(F("(Change OLD_ADDR to the new address and upload again to change once more or go back to factory)"));
}

// Scan 0x08~0x77; for every device that answers, read F0 to see if it is an MBC device
void scan() {
  uint8_t found = 0;
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() != 0) continue;
    int16_t id = mbcRead(a, 0xF0);
    Serial.print(F(" 0x")); Serial.print(a, HEX);
    if (id != MBC_NONE) { Serial.print(F("(factory 0x")); Serial.print(id & 0xFF, HEX); Serial.print(')'); }
    found++;
  }
  if (!found) Serial.print(F(" (none)"));
  Serial.println();
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

<h3 id="exi2">Shared 2: ID card + reboot (F0 ~ F5, EF)</h3>

**What it does:** Reads the 6 shared boxes `F0` ~ `F5` in one go and tells you which product it is. Type `r` to send `[EF][A5]` and reboot; watch `F5` seconds since boot go back to zero.

<a href="/examples/i2c/arduino-en/i2c_02_reboot_and_version/i2c_02_reboot_and_version.ino" download data-ignore="true">📄 Download i2c_02_reboot_and_version.ino</a>

<!-- ino: examples/i2c/arduino-en/i2c_02_reboot_and_version/i2c_02_reboot_and_version.ino -->
```cpp
/*
 * MBC universal I2C × Arduino   Shared example 2: Read the ID card + reboot (F0~F5, EF)
 * ------------------------------------------------------------
 * What does this example do?
 *   Every MBC universal I2C sensor has the same 6-box "ID card":
 *     F0 device ID (low byte factory address, high byte hw[0]), F1 protocol version, F2 firmware version,
 *     F3 status flags, F4 update counter (+1 every 10 ms, 0~255 and around), F5 seconds since boot
 *   This example prints them once per second and tells you which product it is.
 *   Type r and Enter in the Serial Monitor → sends [EF][0xA5] to reboot. You will see F5 go back to zero and F4 keep counting.
 *
 * You need: any MBC universal I2C edition sensor.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200.
 *
 * Tip:
 *   One round of F4 is only 2.56 s, so it cannot tell you if the sensor rebooted; check whether F5 got smaller.
 *   F4 not moving = the sensor is busy (for example calibrating).
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // Sensor address   👉 Change this (Pathfinder 0x16, Ranger 0x29, Controller 0x42, IMU 0x6A, Sharpshooter 0x4C)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC universal I2C: ID card + reboot ==="));
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Please check ADDR and the wiring (you can run i2c_03_scan first)."));
    while (true) {}
  }
  Serial.println(F("Type r and Enter = reboot"));
}

void loop() {
  if (Serial.available() && Serial.read() == 'r') {
    uint8_t magic = 0xA5;
    mbcWrite(ADDR, 0xEF, &magic, 1);     // [EF][A5] = reboot; any other number is ignored
    Serial.println(F(">>> Reboot sent. Watch F5 go back to zero"));
    delay(500);
  }

  int16_t f[6];                          // F0~F5 are consecutive, read all 6 in one go
  if (!mbcReadMany(ADDR, 0xF0, 6, f)) { Serial.println(F("(rebooting / cannot read)")); delay(500); return; }

  uint8_t factory = (uint8_t)(f[0] & 0xFF);
  uint8_t hw0     = (uint8_t)((f[0] >> 8) & 0xFF);
  Serial.println(F("---------------------------"));
  Serial.print(F("F0 device ID     factory address 0x")); Serial.print(factory, HEX);
  Serial.print(F("  hw[0] = ")); Serial.print(hw0);
  Serial.print(F("  → ")); printProduct(factory, hw0); Serial.println();
  Serial.print(F("F1 protocol ver  ")); Serial.println(f[1]);
  Serial.print(F("F2 firmware ver  ")); Serial.print(f[2] / 100); Serial.print('.'); Serial.println(f[2] % 100);
  Serial.print(F("F3 status flags  0b"));
  for (int8_t i = 7; i >= 0; i--) Serial.print((f[3] >> i) & 1);
  Serial.println(F("  (bit0 = ready, other bits: see each product's page)"));
  Serial.print(F("F4 update count  ")); Serial.print(f[4]); Serial.println(F("  (keeps changing = the sensor is alive)"));
  Serial.print(F("F5 uptime        ")); Serial.print(f[5]); Serial.println(F(" s"));

  delay(50);
  delay(950);
}

// Tell the product from the factory address + hw[0]
void printProduct(uint8_t factory, uint8_t hw0) {
  if      (factory == 0x16 && hw0 == 8)  Serial.print(F("Pathfinder 8-Way"));
  else if (factory == 0x16 && hw0 == 16) Serial.print(F("Pathfinder 16-Way"));
  else if (factory == 0x29 && hw0 == 2)  Serial.print(F("Ranger 2-Way"));
  else if (factory == 0x29 && hw0 == 8)  Serial.print(F("Ranger 8-Way"));
  else if (factory == 0x42 && hw0 == 1)  Serial.print(F("Controller PS2"));
  else if (factory == 0x42 && hw0 == 2)  Serial.print(F("Controller PS4"));
  else if (factory == 0x6A)              Serial.print(F("IMU 9-Axis Gyro"));
  else if (factory == 0x4C && hw0 == 1)  Serial.print(F("Sharpshooter (School edition)"));
  else if (factory == 0x4C && hw0 == 2)  Serial.print(F("Sharpshooter (WRO edition)"));
  else                                   Serial.print(F("Unknown MBC device"));
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

<h3 id="exi3">Shared 3: Scan the devices on the bus</h3>

**What it does:** Scans `0x08` ~ `0x77`. For every device that answers it reads `F0` to tell which MBC product it is, its firmware version, and whether the address was changed.

<a href="/examples/i2c/arduino-en/i2c_03_scan/i2c_03_scan.ino" download data-ignore="true">📄 Download i2c_03_scan.ino</a>

<!-- ino: examples/i2c/arduino-en/i2c_03_scan/i2c_03_scan.ino -->
```cpp
/*
 * MBC universal I2C × Arduino   Shared example 3: Scan the devices on the bus
 * ------------------------------------------------------------
 * What does this example do?
 *   Knocks on every I2C address 0x08~0x77. For every device that answers, it reads F0 "device ID" to tell which
 *   MBC product it is (a non-MBC device is printed as "other device"). Is the wiring OK, what address was it changed to,
 *   how many are on the bus: one run tells you. Scans every 3 seconds.
 *
 * You need: any MBC universal I2C edition sensor (several at once is fine, as long as the addresses differ).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200.
 */

#include <Wire.h>

const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC universal I2C: Scan ==="));
}

void loop() {
  uint8_t found = 0;
  Serial.println(F("Scanning 0x08 ~ 0x77 ..."));
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    Wire.beginTransmission(a);           // Knock on the door
    if (Wire.endTransmission() != 0) continue;   // Nobody answers → next one
    found++;
    Serial.print(F("  0x"));
    if (a < 0x10) Serial.print('0');
    Serial.print(a, HEX);
    Serial.print(F("  "));

    int16_t id = mbcRead(a, 0xF0);       // Read the device ID
    if (id == MBC_NONE) { Serial.println(F("other device (not MBC, or not the universal I2C edition)")); continue; }
    uint8_t factory = (uint8_t)(id & 0xFF);
    uint8_t hw0     = (uint8_t)((id >> 8) & 0xFF);
    printProduct(factory, hw0);
    int16_t fw = mbcRead(a, 0xF2);
    Serial.print(F("  firmware ")); Serial.print(fw / 100); Serial.print('.'); Serial.print(fw % 100);
    Serial.print(F("  protocol v")); Serial.print(mbcRead(a, 0xF1));
    if (factory != a) { Serial.print(F("  (address was changed, factory is 0x")); Serial.print(factory, HEX); Serial.print(')'); }
    Serial.println();
  }
  if (!found) Serial.println(F("  No device at all: check that SDA/SCL are not swapped and VCC/GND are plugged in firmly"));
  Serial.print(F("Total: ")); Serial.print(found); Serial.println(F(" device(s)"));
  delay(50);
  delay(2950);
}

// Tell the product from the factory address + hw[0]
void printProduct(uint8_t factory, uint8_t hw0) {
  if      (factory == 0x16 && hw0 == 8)  Serial.print(F("Pathfinder 8-Way"));
  else if (factory == 0x16 && hw0 == 16) Serial.print(F("Pathfinder 16-Way"));
  else if (factory == 0x29 && hw0 == 2)  Serial.print(F("Ranger 2-Way"));
  else if (factory == 0x29 && hw0 == 8)  Serial.print(F("Ranger 8-Way"));
  else if (factory == 0x42 && hw0 == 1)  Serial.print(F("Controller PS2"));
  else if (factory == 0x42 && hw0 == 2)  Serial.print(F("Controller PS4"));
  else if (factory == 0x6A)              Serial.print(F("IMU 9-Axis Gyro"));
  else if (factory == 0x4C && hw0 == 1)  Serial.print(F("Sharpshooter (School edition)"));
  else if (factory == 0x4C && hw0 == 2)  Serial.print(F("Sharpshooter (WRO edition)"));
  else                                   Serial.print(F("Unknown MBC device"));
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // The low byte comes first
    uint16_t hi = Wire.read();           // Then the high byte (little-endian)
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// Read one value. Returns MBC_NONE if it cannot be read (not connected)
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**You will see:**

```text
=== MBC universal I2C: Scan ===
Scanning 0x08 ~ 0x77 ...
  0x29  Ranger 2-Way  firmware 2.0  protocol v2
Total: 1 device(s)
```

---

## ❓ FAQ

**Q: It keeps saying "Sensor not found"?**
1. Check that SDA and SCL are not swapped (UNO: SDA → A4, SCL → A5; ESP32: 21 / 22).
2. Check that VCC and GND are plugged in firmly.
3. Make sure it is the **universal I2C edition**. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly. Old boards (PA9/PA10 without I2C) have no universal I2C edition either.
4. Run [Shared 3 Scan](#exi3) to see whether any device is on the bus and whether the address was changed.

**Q: What does `-32768` mean?**
`-32768` (called `MBC_NONE` in the code) means one of two things: ① the helper cannot read the sensor (loose wire, wrong address); ② that number is an empty slot. The Ranger 2-Way only has `01` ~ `08`, so reading `09` gives `-32768`. A real distance can never be this number, so you can use it to tell "this data cannot be used".

**Q: ESP32 cannot read, or the values look strange?**
Some MBC products (Controller PS4, Sharpshooter) are built on ESP32 and cannot "clock stretch", so the host must **wait 2 ms** after writing the number before reading. The helper `mbcReadMany` already has `delay(2)`. Please do not remove it. When an ESP32 is the host, its default I2C pins are GPIO 21 (SDA) / 22 (SCL).

**Q: The distance is always 120 / 1200?**
Either there really is nothing within 120 cm in that direction, or that laser has failed. Run [Example 6](#ex6) and look at the `07` fault bitmap to find out which.

**Q: The Serial Monitor shows garbage?**
The baud rate is not **115200**. Change it at the bottom right of the Serial Monitor.

**Q: Can I connect a Ranger 2-Way and a Ranger 8-Way at the same time?**
Both come from the factory at `0x29`, so first use [Shared 1](#exi1) to change one of them to another address. MBC products with different addresses (for example the Pathfinder at `0x16`) can share the bus as they are.
