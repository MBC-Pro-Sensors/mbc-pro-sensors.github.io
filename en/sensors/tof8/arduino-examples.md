<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- moved: /en/sensors/tof8/arduino/ide.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/tof8/arduino/ide.md">Ranger 8-Way › Arduino › Arduino IDE</a></p>

# 🧪 Ranger 8-Way × Arduino Examples

[🔙 Back to Ranger 8-Way Home](/en/sensors/tof8/index.md)　|　[📡 MBC Universal I2C Protocol (advanced)](/en/i2c-protocol.md)

This page has **6 product examples + 3 shared examples**, from "is it connected?" all the way to an "obstacle-avoiding car". You can copy and upload each example as it is, and every line of the code has an English comment. **Never written code before? No problem.** Just do them one by one, in order.

> [!TIP]
> First time using Arduino? Read [🔰 First Time with Arduino: Start from Zero](/en/sensors/arduino-start.md) first. In 15 minutes you learn how to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/tof8/arduino-en/tof8_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
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

Figure: `/images/figures/tof8/wiring-uno.svg`

> [!IMPORTANT]
> - Use the **universal I2C edition** of the Ranger 8-Way. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
> - After power-on, the sensor needs **2 ~ 4 seconds** to wake up the 8 lasers one by one. Before it is ready every distance is 0, which looks like an obstacle right in front of it. That is why every example first waits for `F3` ready (at most 6 s). Do the same in your own programs.
> - Unplug the USB cable before you connect the wires.

**Where do the 8 lasers look?** (the sensor is mounted at the front of the robot, and you stand behind the robot looking forward)

| Laser | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Approx. angle | left 90° | left 60° | left 30° | ahead | ahead | right 30° | right 60° | right 90° |
| Number (cm) | `10` | `11` | `12` | `13` | `14` | `15` | `16` | `17` |
| Number (mm) | `18` | `19` | `1A` | `1B` | `1C` | `1D` | `1E` | `1F` |

---

<h2 id="cmd-table">📖 Command Overview (protocol v2)</h2>

Think of the sensor as a row of numbered "boxes". Each box holds one 16-bit number (int16, little-endian). To read: write `[number]` → wait 2 ms → read 2×k bytes, and you get k boxes in a row starting from that number (k ≤ 16). A number that does not exist returns `-32768`. The house number (I2C address) is `0x29`.

<!-- gen:cmd-table -->
| No. | Name | Range / Unit | Description | Used in |
| :---: | :--- | :--- | :--- | :--- |
| `01` | Nearest distance | 0 ~ 120 cm | Smallest of the 8; 120 = nothing around | [Example 2](#ex2), [3](#ex3), [4](#ex4), [6](#ex6) |
| `02` | Nearest direction | −7 ~ +7 (±8 = nothing) | Where the nearest object is: 0 straight ahead, negative = left, −7 far left, +7 far right; ±8 when there is nothing | [Example 3](#ex3), [4](#ex4), [6](#ex6) |
| `03` | Most open distance | 0 ~ 120 cm | Largest of the 8 (a failed laser always reads 120, so it looks like the most open one) | [Example 4](#ex4) |
| `04` | Most open direction | −7 ~ +7 | Which direction is farthest; 0 when all 8 read 120 | [Example 4](#ex4) |
| `05` | Front distance | 0 ~ 120 cm | Average of lasers 4 and 5 | [Example 2](#ex2), [6](#ex6) |
| `06` | Nearest distance | 0 ~ 1200 mm | Smallest of the 8 in mm | [Example 3](#ex3) |
| `07` | Init bitmap | 0 ~ 255 (bits) | bit0 = laser 1 (far left) ... bit7 = laser 8, 1 = booted OK; 255 = all OK | [Example 1](#ex1), [2](#ex2) |
| `08` | Fault bitmap | 0 ~ 255 (bits) | bit0 = laser 1 ... bit7 = laser 8, 1 = that laser has failed; a failed laser reads 120 / 1200 | [Example 2](#ex2), [5](#ex5) |
| `09` | Front distance | 0 ~ 1200 mm | Average of lasers 4 and 5, in mm | [Example 2](#ex2), [6](#ex6) |
| `10` ~ `17` | Distance of lasers 1 ~ 8 | 0 ~ 120 cm | Each laser's distance; `10` = laser 1 (far left), `13` / `14` = straight ahead, `17` = laser 8 (far right) | [Example 2](#ex2), [5](#ex5) |
| `18` ~ `1F` | Distance of lasers 1 ~ 8 | 0 ~ 1200 mm | Each laser's distance in mm | [Example 2](#ex2) |
| `F0` | Device ID | — | Low byte = factory address 0x29, high byte = 8 (laser count) | [Example 1](#ex1), [Shared 2](#exi2), [Shared 3](#exi3) |
| `F1` | Protocol version | 2 | These examples are written for version 2 | [Example 1](#ex1) |
| `F2` | Firmware version | major×100 + minor | e.g. 102 = 1.2 | [Example 1](#ex1) |
| `F3` | Status flags | bits | b0 ready (all 8 lasers initialized, about 2 ~ 4 s after boot), b1 any laser failed | [Example 1](#ex1) |
| `F4` | Update counter | 0 ~ 255 | +1 every 10 ms, keeps changing = the sensor is alive | [Shared 2](#exi2) |
| `F5` | Seconds since boot | 0 ~ 32767 s | Got smaller = it rebooted | [Example 1](#ex1), [Shared 2](#exi2) |
| `F8` | Change I2C address (command) | `[new address][inverted]` | `[F8][00][FF]` = back to factory; takes effect after reboot | [Shared 1](#exi1) |
| `EF` | Reboot (command) | `[A5]` | Send `[EF][A5]` | [Shared 1](#exi1), [Shared 2](#exi2) |
<!-- /gen -->

The Ranger 8-Way has no product-specific write commands (E0 ~ EE).

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the sensor](#ex1) | Check the wiring, check that all 8 lasers booted OK | Nothing |
| 2 | [See all the values at once](#ex2) | The whole value table: 01 ~ 09, 10 ~ 17 cm, 18 ~ 1F mm | Your hand |
| 3 | [Which direction is the nearest object?](#ex3) | 01 / 02 / 06 nearest distance and direction | A box |
| 4 | [Which side is most open?](#ex4) | 03 / 04 most open distance and direction | A few books |
| 5 | [Radar bar chart](#ex5) | Draw bars from 10 ~ 17 (8 lasers in cm), mark faults with 08 | A few books |
| 6 | [Obstacle-avoiding car (advanced)](#ex6) | Avoid obstacles with 05 / 09 front distance + 02 nearest direction | Two-wheel car, L298N motor driver board |
| Shared 1 | [Change the I2C address](#exi1) | F8 change address / back to factory, EF reboot | Nothing |
| Shared 2 | [ID card + reboot](#exi2) | The 6 shared boxes F0 ~ F5 | Nothing |
| Shared 3 | [Scan the devices on the bus](#exi3) | Scan 0x08 ~ 0x77, tell the product from F0 | Nothing |

> [!TIP]
> **Suggested order: 1 → 2 → 3 → 4 → 5 → 6.** First check the wiring (1) and get to know every box (2). Then learn the directions (3, 4) and the applications.

---

<h2 id="ex1">1️⃣ Say hello to the sensor</h2>

**What it does:** Checks that the sensor is connected and reads its ID card: `F0` device ID (factory address + laser count), `F1` protocol version, `F2` firmware version, and `F3` status flags printed bit by bit in plain English. Then it uses the `07` init bitmap to list whether all 8 lasers booted OK. Run this one first every time you connect a new sensor.

**Steps:**
1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.

<a href="/examples/tof8/arduino-en/tof8_01_hello/tof8_01_hello.ino" download data-ignore="true">📄 Download tof8_01_hello.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_01_hello/tof8_01_hello.ino -->
```cpp
/*
 * Ranger 8-Way × Arduino   Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   Checks that the sensor is connected. If it is, it prints the sensor's ID card
 *   (device ID, protocol version, firmware version) and its current status
 *   (are all 8 lasers working?).
 *   Run this example first every time you connect a new sensor.
 *
 * You need: Ranger 8-Way (universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Note:
 *   Waking up the 8 lasers one by one takes 2~4 s. Before the sensor is ready every distance is 0, so always wait for F3 bit0 first.
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

  Serial.println(F("=== Ranger 8-Way: Say hello to the sensor ==="));
  Serial.println(F("Waiting for the 8 lasers to boot (at most 6 s)..."));

  // Wait for the sensor to say "I'm ready" (bit0 of status flags F3 = all 8 lasers initialized), at most 6 s
  if (!mbcWaitReady(ADDR, 6000)) {
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
  Serial.print(sensors);                 // Ranger 8-Way = 8
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
  Serial.print(F("  bit0 ready            ")); Serial.println((st & 0x01) ? F("yes (all 8 lasers initialized)") : F("no"));
  Serial.print(F("  bit1 any laser failed ")); Serial.println((st & 0x02) ? F("yes! (run Example 2 to see which one)") : F("no (all 8 lasers OK)"));

  // ---- 07 init bitmap: bit0 = laser 1 (far left) ... bit7 = laser 8 ----
  int16_t initOk = mbcRead(ADDR, 0x07);
  Serial.print(F("Lasers that booted OK (left→right): "));
  for (uint8_t i = 0; i < 8; i++) Serial.print((initOk >> i) & 1 ? 'O' : 'X');
  Serial.println(initOk == 255 ? F("  all OK") : F("  a laser marked X failed to boot, please power off and on"));

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
=== Ranger 8-Way: Say hello to the sensor ===
Waiting for the 8 lasers to boot (at most 6 s)...
Found it! Factory address 0x29, 8 lasers
Protocol version: 2
Firmware version: 2.0
Current status:
  bit0 ready            yes (all 8 lasers initialized)
  bit1 any laser failed no (all 8 lasers OK)
Lasers that booted OK (left→right): OOOOOOOO  all OK
Seconds since boot: 6
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

**What it does:** Prints the whole value table every 0.5 s, in three `mbcReadMany` parts: `01` ~ `09`, `10` ~ `17` (8 lasers in cm), `18` ~ `1F` (8 lasers in mm). Get to know every box here first; the later examples only pick a few of them.

<a href="/examples/tof8/arduino-en/tof8_02_all_values/tof8_02_all_values.ino" download data-ignore="true">📄 Download tof8_02_all_values.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_02_all_values/tof8_02_all_values.ino -->
```cpp
/*
 * Ranger 8-Way × Arduino   Example 2: See all the values at once
 * ------------------------------------------------------------
 * What does this example do?
 *   Prints the sensor's "whole value table" every 0.5 s:
 *     01 nearest cm, 02 nearest direction, 03 most open cm, 04 most open direction, 05 front cm,
 *     06 nearest mm, 07 init bitmap, 08 fault bitmap, 09 front mm,
 *     10~17 lasers 1~8 in cm, 18~1F lasers 1~8 in mm.
 *   The later examples only pick a few of these boxes. Get to know every box here first.
 *
 * You need: Ranger 8-Way (universal I2C edition) + your hand or a few books.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move your hand slowly from left to right and watch the numbers move from laser 1 to laser 8.
 *
 * Note:
 *   At most 16 values can be read at a time, so we read in three parts: 01~09, 10~17, 18~1F.
 *   120 cm / 1200 mm = nothing in that direction (or that laser failed).
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand in front of the sensor."));
}

void loop() {
  int16_t a[9];                          // 01~09
  int16_t cm[8];                         // 10~17 lasers 1~8 in cm
  int16_t mm[8];                         // 18~1F lasers 1~8 in mm
  if (!mbcReadMany(ADDR, 0x01, 9, a))  { Serial.println(F("Cannot read (01~09)")); delay(500); return; }
  if (!mbcReadMany(ADDR, 0x10, 8, cm)) { Serial.println(F("Cannot read (10~17)")); delay(500); return; }
  if (!mbcReadMany(ADDR, 0x18, 8, mm)) { Serial.println(F("Cannot read (18~1F)")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 nearest distance     ")); Serial.print(a[0]); Serial.println(F(" cm   (smallest of the 8; 120 = nothing around)"));
  Serial.print(F("02 nearest direction    ")); Serial.print(a[1]); Serial.println(F("      (-7 far left ~ +7 far right, 0 = straight ahead; ±8 = nothing)"));
  Serial.print(F("03 most open distance   ")); Serial.print(a[2]); Serial.println(F(" cm   (largest of the 8)"));
  Serial.print(F("04 most open direction  ")); Serial.print(a[3]); Serial.println(F("      (-7 ~ +7)"));
  Serial.print(F("05 front distance       ")); Serial.print(a[4]); Serial.println(F(" cm   (average of lasers 4 and 5)"));
  Serial.print(F("06 nearest distance     ")); Serial.print(a[5]); Serial.println(F(" mm   (10 times finer than cm)"));
  Serial.print(F("07 init bitmap          "));
  for (uint8_t i = 0; i < 8; i++) Serial.print((a[6] >> i) & 1);     // bit0 (laser 1) is printed on the left
  Serial.println(F("  (left→right, 1 = booted OK)"));
  Serial.print(F("08 fault bitmap         "));
  for (uint8_t i = 0; i < 8; i++) Serial.print((a[7] >> i) & 1);
  Serial.println(F("  (left→right, 1 = that laser has failed)"));
  Serial.print(F("09 front distance       ")); Serial.print(a[8]); Serial.println(F(" mm"));

  Serial.print(F("10~17 each laser cm  L "));
  for (uint8_t i = 0; i < 8; i++) { printPad(cm[i], 4); Serial.print(' '); }
  Serial.println(F(" R"));
  Serial.print(F("18~1F each laser mm  L "));
  for (uint8_t i = 0; i < 8; i++) { printPad(mm[i], 4); Serial.print(' '); }
  Serial.println(F(" R   (laser 1 = left 90°, lasers 4/5 = straight ahead, laser 8 = right 90°)"));

  delay(50);
  delay(450);                            // Update every 0.5 s
}

// Print a number right-aligned (width w)
void printPad(int16_t v, uint8_t w) {
  uint8_t len = (v < 0) ? 2 : 1;
  for (int16_t t = (v < 0 ? -v : v); t >= 10; t /= 10) len++;
  for (uint8_t i = len; i < w; i++) Serial.print(' ');
  Serial.print(v);
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

**You will see:** (your hand at the front left, 30 cm away)

```text
---------------------------------------
01 nearest distance     30 cm   (smallest of the 8; 120 = nothing around)
02 nearest direction    -3      (-7 far left ~ +7 far right, 0 = straight ahead; ±8 = nothing)
03 most open distance   120 cm   (largest of the 8)
04 most open direction  5      (-7 ~ +7)
05 front distance       120 cm   (average of lasers 4 and 5)
06 nearest distance     304 mm   (10 times finer than cm)
07 init bitmap          11111111  (left→right, 1 = booted OK)
08 fault bitmap         00000000  (left→right, 1 = that laser has failed)
09 front distance       1200 mm
10~17 each laser cm  L  120  120   30  120  120  120  120  120  R
18~1F each laser mm  L 1200 1200  304 1200 1200 1200 1200 1200  R   (laser 1 = left 90°, lasers 4/5 = straight ahead, laser 8 = right 90°)
```

> [!TIP]
> **Try it:** Move your hand slowly from left to right and watch how the numbers "walk" from laser 1 to laser 8. Lasers 4 and 5 both look straight ahead, so when your hand is right in front, their two numbers will be very close.

---

<h2 id="ex3">3️⃣ Which direction is the nearest object?</h2>

**What it does:** The sensor compares its 8 lasers by itself: `01` nearest distance in cm, `02` nearest direction, `06` nearest distance in mm. The computer draws a small ruler with `*` at that direction, then says it with an arrow. A sumo robot uses this to "find the opponent".

**You need:** A box or your hand.

<a href="/examples/tof8/arduino-en/tof8_03_nearest_direction/tof8_03_nearest_direction.ino" download data-ignore="true">📄 Download tof8_03_nearest_direction.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_03_nearest_direction/tof8_03_nearest_direction.ino -->
```cpp
/*
 * Ranger 8-Way × Arduino   Example 3: Which direction is the nearest object?
 * ------------------------------------------------------------
 * What does this example do?
 *   The sensor compares its 8 lasers by itself and tells you which direction the nearest object is in, and how far:
 *     01 nearest distance cm, 02 nearest direction (-7 far left ~ +7 far right, 0 = straight ahead), 06 nearest distance mm.
 *   The computer draws a small ruler with * at that direction, then says it with an arrow. A sumo robot uses this to "find the opponent".
 *
 * You need: Ranger 8-Way (universal I2C edition) + a box or your hand.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move your hand from the front left to straight ahead, then to the right.
 *
 * Note:
 *   When there is nothing within 120 cm, the direction becomes ±8 (meaning "which side it was on last time"), not a real object.
 *   So always look at the distance first, then the direction.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move an object around the sensor."));
}

void loop() {
  int16_t v[6];                          // 01 nearest cm, 02 nearest direction, 03, 04, 05, 06 nearest mm
  if (!mbcReadMany(ADDR, 0x01, 6, v)) { Serial.println(F("Cannot read the sensor")); delay(500); return; }
  int16_t nearCm = v[0], dir = v[1], nearMm = v[5];

  // Draw a small ruler with 15 cells: one cell for each direction -7 ~ +7, * = nearest object, + = straight ahead
  Serial.print(F("L ["));
  for (int8_t i = -7; i <= 7; i++) {
    if (nearCm < 120 && i == dir) Serial.print('*');
    else if (i == 0)              Serial.print('+');
    else                          Serial.print('.');
  }
  Serial.print(F("] R   "));

  if (nearCm >= 120) {                   // Look at the distance first! When there is nothing, the direction is ±8 and cannot be trusted
    Serial.println(F("Nothing within 120 cm ahead"));
  } else {
    if      (dir <= -6) Serial.print(F("← left       "));
    else if (dir <= -2) Serial.print(F("↖ front left "));
    else if (dir <=  1) Serial.print(F("↑ ahead      "));
    else if (dir <=  5) Serial.print(F("↗ front right"));
    else                Serial.print(F("→ right      "));
    Serial.print(' '); Serial.print(nearCm); Serial.print(F(" cm ("));
    Serial.print(nearMm); Serial.print(F(" mm, direction ")); Serial.print(dir); Serial.println(')');
  }
  delay(50);
  delay(150);                            // Print one line every 0.2 s   👉 You can change this
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

**You will see:** (moving your hand from the front left to straight ahead, then to the right)

```text
L [.......+.......] R   Nothing within 120 cm ahead
L [..*....+.......] R   ↖ front left  35 cm (352 mm, direction -5)
L [.......*.......] R   ↑ ahead       28 cm (283 mm, direction 0)
L [.......+.....*.] R   → right       40 cm (401 mm, direction 6)
```

**What does the direction mean?**

| Direction number | Meaning |
| :---: | :--- |
| 0 | The object is straight ahead |
| Negative (−1 ~ −7) | The object is on the **left**. Smaller = further left (−7 = far left) |
| Positive (+1 ~ +7) | The object is on the **right**. Bigger = further right (+7 = far right) |
| ±8 | Nothing within 120 cm (it only tells "which side it was on last time") |

> [!WARNING]
> **Always look at the distance first, then the direction.** When there is nothing within 120 cm, the direction is `±8`, and there is no real object there. The example first checks "distance < 120" to make sure there is something, and only then looks at the direction.

---

<h2 id="ex4">4️⃣ Which side is most open?</h2>

**What it does:** Reads `03` most open distance and `04` most open direction. The sensor finds the direction among the 8 that is farthest (most empty) for you. In a maze or when avoiding obstacles, use these two boxes to "go where it is most open". It also prints `01` / `02` (the nearest), so you can compare "where to avoid" and "where to go".

**You need:** A few books as walls.

<a href="/examples/tof8/arduino-en/tof8_04_open_direction/tof8_04_open_direction.ino" download data-ignore="true">📄 Download tof8_04_open_direction.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_04_open_direction/tof8_04_open_direction.ino -->
```cpp
/*
 * Ranger 8-Way × Arduino   Example 4: Which side is most open?
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads "03 most open distance cm" and "04 most open direction": the sensor finds the direction
 *   among the 8 that is farthest (most empty) for you. In a maze or when avoiding obstacles, use these two boxes to "go where it is most open".
 *   It also reads 01/02 (the nearest) and prints both, so you can compare "where to avoid" and "where to go".
 *
 * You need: Ranger 8-Way (universal I2C edition) + a few books as walls.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Use books to build different "walls" on the left, right and front of the sensor.
 *
 * Note:
 *   A failed laser always reads 120, so it looks like the "most open" direction. Run Example 1 before a match to check that all 8 lasers are OK.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Build a few walls with books next to the sensor."));
}

void loop() {
  int16_t v[4];                          // 01 nearest cm, 02 nearest direction, 03 most open cm, 04 most open direction
  if (!mbcReadMany(ADDR, 0x01, 4, v)) { Serial.println(F("Cannot read the sensor")); delay(500); return; }
  int16_t nearCm = v[0], nearDir = v[1], farCm = v[2], farDir = v[3];

  Serial.print(F("Most open → "));
  printDir(farDir);
  Serial.print(' '); Serial.print(farCm); Serial.print(F(" cm"));

  Serial.print(F("   |   Nearest → "));
  if (nearCm >= 120) Serial.println(F("nothing within 120 cm around"));
  else {
    printDir(nearDir);
    Serial.print(' '); Serial.print(nearCm); Serial.println(F(" cm"));
  }
  delay(50);
  delay(250);                            // Print one line every 0.3 s   👉 You can change this
}

// Print a direction -7 ~ +7 as words (each step ≈ 13°, ±7 ≈ ±90°)
void printDir(int16_t dir) {
  if      (dir <= -6) Serial.print(F("left"));
  else if (dir <= -2) Serial.print(F("front left"));
  else if (dir <=  1) Serial.print(F("ahead"));
  else if (dir <=  5) Serial.print(F("front right"));
  else                Serial.print(F("right"));
  Serial.print(F("(")); Serial.print(dir); Serial.print(')');
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

**You will see:** (books on the left and in front, nothing on the right)

```text
Most open → right(7) 120 cm   |   Nearest → ahead(0) 18 cm
Most open → front right(4) 95 cm   |   Nearest → front left(-4) 22 cm
```

> [!TIP]
> A failed laser always reads 120, so it looks like the "most open" direction. Run [Example 1](#ex1) before a match to check that all 8 lasers are OK.

---

<h2 id="ex5">5️⃣ Radar bar chart</h2>

**What it does:** Reads `10` ~ `17` (8 lasers in cm) in one go and draws them as 8 bars: **a longer bar = that direction is more open**, a very short bar = something is blocking it. It also reads the `08` fault bitmap and marks a failed laser with `XX`. One look tells you where you can go.

**You need:** A few books as obstacles.

<a href="/examples/tof8/arduino-en/tof8_05_radar_bars/tof8_05_radar_bars.ino" download data-ignore="true">📄 Download tof8_05_radar_bars.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_05_radar_bars/tof8_05_radar_bars.ino -->
```cpp
/*
 * Ranger 8-Way × Arduino   Example 5: Radar bar chart
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads "10~17 lasers 1~8 distance cm" in one go and draws the 8 directions as 8 bars:
 *   a longer bar = that direction is more open, a very short bar = something is blocking it. One look tells you where you can go.
 *   It also reads "08 fault bitmap" and marks a failed laser with XX.
 *
 * You need: Ranger 8-Way (universal I2C edition) + a few books as obstacles.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200.
 *
 * Where do the 8 lasers look? (standing behind the robot, looking forward)
 *   Laser 1 left 90°, laser 2 left 60°, laser 3 left 30°, lasers 4 and 5 straight ahead,
 *   laser 6 right 30°, laser 7 right 60°, laser 8 right 90°
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int8_t ANGLE[8] = {-90, -60, -30, 0, 0, 30, 60, 90};   // Angle of each laser (negative = left)

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Put a few books next to the sensor."));
}

void loop() {
  int16_t cm[8];
  int16_t fault = mbcRead(ADDR, 0x08);   // 08 fault bitmap: bit0 = laser 1
  if (!mbcReadMany(ADDR, 0x10, 8, cm) || fault == MBC_NONE) { Serial.println(F("Cannot read the sensor")); delay(500); return; }

  Serial.println(F("====== Radar view (longer bar = more open) ======"));
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(i + 1);                 // Laser number (1~8)
    Serial.print(' ');
    if (ANGLE[i] >= 0) Serial.print('+');
    if (ANGLE[i] > -10 && ANGLE[i] < 10) Serial.print(' ');
    Serial.print(ANGLE[i]); Serial.print(F("° |"));

    if ((fault >> i) & 1) {              // This laser failed
      Serial.println(F("XX fault"));
      continue;
    }
    int8_t n = cm[i] / 5;                // One cell per 5 cm, at most 24 cells
    if (n > 24) n = 24;
    for (int8_t k = 0; k < n; k++) Serial.print('#');
    if (cm[i] >= 120) Serial.println(F("> nothing"));
    else { Serial.print(' '); Serial.print(cm[i]); Serial.println(F(" cm")); }
  }
  delay(50);
  delay(450);                            // Redraw every 0.5 s   👉 You can change this
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

**You will see:** (a book 30 cm straight ahead)

```text
====== Radar view (longer bar = more open) ======
1 -90° |########################> nothing
2 -60° |########################> nothing
3 -30° |############### 75 cm
4 + 0° |###### 32 cm
5 + 0° |###### 34 cm
6 +30° |################ 80 cm
7 +60° |###################### 110 cm
8 +90° |########################> nothing
```

---

<h2 id="ex6">6️⃣ Obstacle-avoiding car (advanced)</h2>

**What it does:** Makes a two-wheel car drive around obstacles by itself: while `05` front distance (cm) is still far, go straight; when something is ahead, look at `02` nearest direction and turn right if the object is on the left, left if it is on the right; when `09` front distance (mm) is too close, back up first. One `mbcReadMany` reads `01` to `09`, so every decision uses data from the same moment.

**You also need:** A two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), a battery.

**Motor wiring:**

| L298N | Arduino UNO / Nano / R4 | ESP32 | Controls |
| :---: | :---: | :---: | :--- |
| ENA | 5 | GPIO 25 | Left wheel speed |
| IN1 / IN2 | 7 / 8 | GPIO 26 / 27 | Left wheel direction |
| ENB | 6 | GPIO 14 | Right wheel speed |
| IN3 / IN4 | 9 / 10 | GPIO 32 / 33 | Right wheel direction |
| GND | GND | GND | **Must share GND with the Arduino** |

**Before the first run:** Lift the car (wheels in the air) and upload. Check that both wheels turn **forward**. If a wheel turns the wrong way, change `LEFT_REVERSE` or `RIGHT_REVERSE` to `true`.

<a href="/examples/tof8/arduino-en/tof8_06_avoid_car/tof8_06_avoid_car.ino" download data-ignore="true">📄 Download tof8_06_avoid_car.ino</a>

<!-- ino: examples/tof8/arduino-en/tof8_06_avoid_car/tof8_06_avoid_car.ino -->
```cpp
/*
 * Ranger 8-Way × Arduino   Example 6 (advanced): Obstacle-avoiding car
 * ------------------------------------------------------------
 * What does this example do?
 *   Makes a two-wheel car drive around obstacles by itself:
 *     - "05 front cm" is still far → go straight
 *     - Something ahead → look at "02 nearest direction": object on the left → turn right, on the right → turn left
 *     - "09 front mm" too close (about to crash) → back up a little first, then turn
 *   mbcReadMany reads 01 to 09 in one go, so every decision uses data from the same moment.
 *
 * You need: A two-wheel car, an L298N motor driver board (or a TB6612 etc. wired the same way), a battery.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      (left wheel)
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     (right wheel)
 *   (ESP32 instead: ENA → 25, IN1 → 26, IN2 → 27, ENB → 14, IN3 → 32, IN4 → 33)
 *   The L298N GND must be connected to the Arduino GND! Mount the sensor at the front center of the car, lasers facing forward.
 *
 * Before the first run:
 *   Lift the car (wheels in the air), upload, and check that both wheels turn "forward".
 *   If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE to true.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- Motor pins ----
#if defined(ESP32)
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel (pins 6~11 cannot be used on ESP32)
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 Settings you can change ----
const int     SPEED      = 130;          // Straight-line speed 0~255
const int     TURN_SPEED = 110;          // Turning speed
const int16_t TURN_CM    = 35;           // Turn when something is within this many cm ahead
const int16_t BACK_MM    = 120;          // Back up first when something is within this many mm ahead
const bool    LEFT_REVERSE  = false;     // Change to true if the left wheel turns the wrong way
const bool    RIGHT_REVERSE = false;     // Change to true if the right wheel turns the wrong way

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Starting in 3 seconds!"));
  delay(3000);
}

void loop() {
  int16_t v[9];                          // 01~09
  if (!mbcReadMany(ADDR, 0x01, 9, v)) {  // Cannot read → stop for safety
    drive(0, 0);
    delay(50);
    return;
  }
  int16_t nearCm  = v[0];                // 01 nearest distance cm
  int16_t nearDir = v[1];                // 02 nearest direction (negative = left)
  int16_t frontCm = v[4];                // 05 front distance cm
  int16_t frontMm = v[8];                // 09 front distance mm

  if (frontMm <= BACK_MM) {              // About to crash: back up first
    Serial.println(F("Too close! Backing up"));
    drive(-TURN_SPEED, -TURN_SPEED);
    delay(300);
  } else if (frontCm <= TURN_CM || (nearCm < 20 && nearDir > -3 && nearDir < 3)) {
    // Something straight ahead (or something very close near the front) → turn away from it
    if (nearDir < 0) { Serial.println(F("Obstacle on the left → turn right")); drive(TURN_SPEED, -TURN_SPEED); }
    else             { Serial.println(F("Obstacle on the right → turn left")); drive(-TURN_SPEED, TURN_SPEED); }
    delay(200);
  } else {
    drive(SPEED, SPEED);                 // Nothing ahead: go straight
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

**How to tune it:**

| What the car does | What to change |
| :--- | :--- |
| Often bumps into things before it turns | Increase `TURN_CM` (e.g. 35 → 50) |
| Turns too early, cannot get through narrow gaps | Decrease `TURN_CM` |
| Still faces the obstacle after turning | Make the turning `delay(200)` longer |
| Spins in place right at the start | One wheel is reversed. Change `LEFT_REVERSE` or `RIGHT_REVERSE` |

---

## 🔧 Shared command examples (the same for every MBC universal I2C product)

These three are in `examples/i2c/arduino-en/`. They work with the Pathfinder, Ranger, Controller, IMU and Sharpshooter. Just change `ADDR` / `OLD_ADDR` at the top of the program to `0x29`.

<h3 id="exi1">Shared 1: Change the I2C address (F8)</h3>

**What it does:** When two sensors with the same address must share one bus, use `[F8][new address][inverted new address]` to change one of them (saved to EEPROM); `[F8][00][FF]` goes back to factory. After the change it sends `[EF][A5]` to reboot and scans again to confirm.

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

**What it does:** Reads the 6 shared boxes `F0` ~ `F5` in one go and tells you which product it is. Type `r` to send `[EF][A5]` and reboot; watch `F5` seconds since boot go back to zero (after a reboot it needs another 2 ~ 4 s to be ready).

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
  0x29  Ranger 8-Way  firmware 2.0  protocol v2
Total: 1 device(s)
```

---

## ❓ FAQ

**Q: It keeps saying "Sensor not found"?**
1. Check that SDA and SCL are not swapped (UNO: SDA → A4, SCL → A5; ESP32: 21 / 22).
2. Check that VCC and GND are plugged in firmly. The 8 lasers use more power; if the computer's USB port cannot supply enough, use a power adapter or a battery.
3. Make sure it is the **universal I2C edition**. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
4. Run [Shared 3 Scan](#exi3) to see whether any device is on the bus and whether the address was changed.

**Q: What does `-32768` mean?**
`-32768` (called `MBC_NONE` in the code) means one of two things: ① the helper cannot read the sensor (loose wire, wrong address); ② that number is an empty slot. For example `0A` ~ `0F` are not defined, so reading them gives `-32768`. A real distance can never be this number, so you can use it to tell "this data cannot be used".

**Q: ESP32 cannot read, or the values look strange?**
Some MBC products (Controller PS4, Sharpshooter) are built on ESP32 and cannot "clock stretch", so the host must **wait 2 ms** after writing the number before reading. The helper `mbcReadMany` already has `delay(2)`. Please do not remove it. When an ESP32 is the host, its default I2C pins are GPIO 21 (SDA) / 22 (SCL).

**Q: Every distance is 0 right after power-on?**
The 8 lasers are still initializing (2 ~ 4 s). Wait until `F3` bit0 = 1 before reading. That is exactly what the helper `mbcWaitReady` does.

**Q: One laser always reads 120?**
Either there really is nothing in that direction, or that laser has failed. Run [Example 5](#ex5): a failed laser is marked `XX`. The `07` / `08` boxes tell "failed at boot" from "failed while running" apart.

**Q: The Serial Monitor shows garbage?**
The baud rate is not **115200**. Change it at the bottom right of the Serial Monitor.

**Q: Can I connect a Ranger 8-Way and a Ranger 2-Way at the same time?**
Both come from the factory at `0x29`, so first use [Shared 1](#exi1) to change one of them to another address. MBC products with different addresses (for example the Pathfinder at `0x16`) can share the bus as they are.
