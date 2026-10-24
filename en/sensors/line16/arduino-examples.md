<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- moved: /en/sensors/line16/arduino/ide.md -->
<p class="moved-note">This page has moved into the new guide: <a href="/en/sensors/line16/arduino/ide.md">Pathfinder 16-Way › Arduino › Arduino IDE</a></p>

# 🧪 Pathfinder 16-Way × Arduino Examples

[🔙 Back to Pathfinder 16-Way Home](/en/sensors/line16/index.md)　|　[📡 I2C Register Guide (advanced)](/en/sensors/line16/arduino-i2c.md)

This page has **7 product examples + 3 shared examples**, from "is it connected?" all the way to a "line-following car". You can copy and upload each example as it is, and every line of the code has an English comment. **Never written code before? No problem.** Just do them one by one, in order.

> [!TIP]
> First time using Arduino? Read [🔰 First Time with Arduino: Start from Zero](/en/sensors/arduino-start.md) first. In 15 minutes you learn how to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/line16/arduino-en/line16_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
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

Figure: `/images/figures/line16/wiring-uno.svg`

> [!IMPORTANT]
> - Use the **universal I2C edition** of the Pathfinder. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
> - When the sensor is mounted on the robot, it works best with **its bottom about 0.5 ~ 1 cm above the floor**.
> - Unplug the USB cable before you connect the wires.
> - "Left" and "right" always mean: **the sensor is mounted on the robot, and you stand behind the robot looking forward**. Line on the left = negative, channel 1 is on the far left.

---

<h2 id="cmd-table">📖 Command Overview (protocol v2)</h2>

Think of the sensor as a row of numbered "boxes". Each box holds one 16-bit number (int16, little-endian). To read: write `[number]` → wait 2 ms → read 2×k bytes, and you get k boxes in a row starting from that number (k ≤ 16). A number that does not exist returns `-32768`. The house number (I2C address) is `0x16`.

<!-- gen:cmd-table -->
| No. | Name | Range / Unit | Description | Used in |
| :---: | :--- | :--- | :--- | :--- |
| `01` | Line position | −16 ~ +16 | Which channel the line is on, 0 = center, negative = left | [Example 2](#ex2), [3](#ex3) |
| `02` | Fine position | −100 ~ +100 | Finer version of the line position, negative = left | [Example 3](#ex3), [6](#ex6) |
| `03` | Line width | 0 ~ 16 channels | How many channels are on the line, 0 = no line found | [Example 3](#ex3) |
| `04` | Segment count | 0 ~ 8 | 0 line lost, 1 one line, 2 or more = branch / crossroad | [Example 5](#ex5), [6](#ex6) |
| `05` | Line bitmap | 0 ~ 65535 (bits) | bit0 = channel 1 (far left) ... bit15 = channel 16, 1 = on the line | [Example 5](#ex5), [7](#ex7) |
| `06` | Last lost direction | −16 ~ +16 | Which side the line was on before it was lost, negative = left | [Example 6](#ex6) |
| `07` | Threshold | 1 ~ 99 | Light-value cut-off for "on the line", default 50 | [Example 7](#ex7) |
| `08` | Calibration count | 0 ~ 255 | How many calibrations finished since boot | [Example 4](#ex4) |
| `10` ~ `1F` | Light value of channels 1 ~ 16 | 0 ~ 100 | Calibrated light value, 0 = black, 100 = white; `10` = channel 1 (far left) | [Example 2](#ex2), [4](#ex4), [7](#ex7) |
| `E0` | Black-line mode (command) | no parameter | Follow a black line (default) | [Example 3](#ex3), [6](#ex6) |
| `E1` | White-line mode (command) | no parameter | Follow a white line, F3 bit1 = 1 | [Example 3](#ex3) |
| `E2` | Black calibration (command) | no parameter | Remember the current reading as black, about 50 ms, saved to EEPROM | [Example 4](#ex4) |
| `E3` | White calibration (command) | no parameter | Remember the current reading as white, about 50 ms, saved to EEPROM | [Example 4](#ex4) |
| `E4` | Set threshold (command) | 1 parameter, 1 ~ 99 | Threshold is kept in RAM, back to 50 after reboot | [Example 7](#ex7) |
| `E7` | 5-second dynamic calibration (command) | no parameter | Sweep over black and white for 5 s, the extremes become the references; F3 bit2 = 1 meanwhile | [Example 4](#ex4), [7](#ex7) |
| `F0` | Device ID | — | Low byte = factory address 0x16, high byte = 16 (channel count) | [Example 1](#ex1), [Shared 2](#exi2), [Shared 3](#exi3) |
| `F1` | Protocol version | 2 | These examples are written for version 2 | [Example 1](#ex1) |
| `F2` | Firmware version | major×100 + minor | e.g. 102 = 1.2 | [Example 1](#ex1) |
| `F3` | Status flags | bits | b0 ready, b1 white-line mode, b2 calibrating, b3 line lost, b4 branch | [Example 1](#ex1), [4](#ex4) |
| `F4` | Update counter | 0 ~ 255 | +1 every 10 ms, keeps changing = the sensor is alive | [Shared 2](#exi2) |
| `F5` | Seconds since boot | 0 ~ 32767 s | Got smaller = it rebooted | [Example 1](#ex1), [Shared 2](#exi2) |
| `F8` | Change I2C address (command) | `[new address][inverted]` | `[F8][00][FF]` = back to factory; takes effect after reboot | [Shared 1](#exi1) |
| `EF` | Reboot (command) | `[A5]` | Send `[EF][A5]` | [Shared 1](#exi1), [Shared 2](#exi2) |
<!-- /gen -->

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the sensor](#ex1) | Check the wiring, read the F0 ~ F3 ID card and status | Nothing |
| 2 | [See all the values at once](#ex2) | What every box in the value table means | A sheet of white paper + black electrical tape |
| 3 | [Where is the line?](#ex3) | Line position, fine position, line width | Same as above |
| 4 | [One-key calibration](#ex4) | E2 / E3 / E7 calibration, wait for F3 "calibrating" to clear | Same as above |
| 5 | [Crossroad detector](#ex5) | Count crossroads with the segment count + line bitmap | Paper with crossroads on it |
| 6 | [Line-following car (advanced)](#ex6) | Proportional control with the fine position, find the line again with the last lost direction | Two-wheel car, L298N motor driver board |
| 7 | [Adjust the threshold](#ex7) | Set the threshold with E4, read 07 to confirm | White paper + black tape |
| Shared 1 | [Change the I2C address](#exi1) | F8 change address / back to factory, EF reboot | Nothing |
| Shared 2 | [ID card + reboot](#exi2) | The 6 shared boxes F0 ~ F5 | Nothing |
| Shared 3 | [Scan the devices on the bus](#exi3) | Scan 0x08 ~ 0x77, tell the product from F0 | Nothing |

> [!TIP]
> **Suggested order: 1 → 4 → 2 → 3 → 5 → 7 → 6.** First check the wiring (1) and calibrate the field (4). Then the readings in the other examples will be accurate.

---

<h2 id="ex1">1️⃣ Say hello to the sensor</h2>

**What it does:** Checks that the sensor is connected and reads its ID card: `F0` device ID (factory address + channel count), `F1` protocol version, `F2` firmware version, and `F3` status flags printed bit by bit in plain English. Run this one first every time you connect a new sensor.

**Steps:**
1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.

<a href="/examples/line16/arduino-en/line16_01_hello/line16_01_hello.ino" download data-ignore="true">📄 Download line16_01_hello.ino</a>

<!-- ino: examples/line16/arduino-en/line16_01_hello/line16_01_hello.ino -->
```cpp
/*
 * Pathfinder 16-Way × Arduino   Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   Checks that the sensor is connected. If it is, it prints the sensor's ID card
 *   (device ID, protocol version, firmware version) and its current status
 *   (does it see the line, is it in white-line mode...).
 *   Run this example first every time you connect a new sensor.
 *
 * You need: Pathfinder 16-Way (universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32.
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

const uint8_t ADDR = 0x16;               // The sensor's "house number" (I2C address). Both 8-Way and 16-Way use 0x16
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // Open the channel to the computer at 115200 baud
  mbcBegin();                            // Get I2C ready (helper at the bottom of the file)
  delay(500);                            // Wait 0.5 s for the sensor to boot

  Serial.println(F("=== Pathfinder 16-Way: Say hello to the sensor ==="));

  // Wait for the sensor to say "I'm ready" (bit0 of status flags F3), at most 2 s
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found! Please check:"));
    Serial.println(F("  1. SDA to A4, SCL to A5 (ESP32: 21 / 22). Are they swapped?"));
    Serial.println(F("  2. VCC to 5V, GND to GND. Are they plugged in firmly?"));
    Serial.println(F("  3. Is your sensor the universal I2C edition? (SPIKE and EV3 editions do not work with Arduino)"));
    while (true) {}                      // Not found: stop here
  }

  // ---- F0 device ID: low byte = factory address, high byte = number of channels ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t factoryAddr = (uint8_t)(id & 0xFF);        // Factory address (stays the same even if the address was changed)
  uint8_t channels    = (uint8_t)((id >> 8) & 0xFF); // hw[0] = how many channels
  Serial.print(F("Found it! Factory address 0x"));
  Serial.print(factoryAddr, HEX);
  Serial.print(F(", "));
  Serial.print(channels);                // Pathfinder 16-Way = 16
  Serial.println(F(" channels"));

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
  Serial.print(F("  bit0 ready          ")); Serial.println((st & 0x01) ? F("yes") : F("no"));
  Serial.print(F("  bit1 white-line mode ")); Serial.println((st & 0x02) ? F("yes (follow white line)") : F("no (follow black line)"));
  Serial.print(F("  bit2 calibrating    ")); Serial.println((st & 0x04) ? F("yes") : F("no"));
  Serial.print(F("  bit3 line lost      ")); Serial.println((st & 0x08) ? F("yes (no line in sight)") : F("no (line in sight)"));
  Serial.print(F("  bit4 branch         ")); Serial.println((st & 0x10) ? F("yes (two or more segments)") : F("no"));

  // ---- F5 seconds since boot (just to see how long it has been on) ----
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
=== Pathfinder 16-Way: Say hello to the sensor ===
Found it! Factory address 0x16, 16 channels
Protocol version: 2
Firmware version: 4.0
Current status:
  bit0 ready          yes
  bit1 white-line mode no (follow black line)
  bit2 calibrating    no
  bit3 line lost      yes (no line in sight)
  bit4 branch         no
Seconds since boot: 12
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

**What it does:** Prints the whole value table every 0.5 s: `01` ~ `08` in one part and `10` ~ `1F` in another (one `mbcReadMany` each). Get to know every box here first; the later examples only pick a few of them.

**You need:** A strip of black electrical tape (about 2 cm wide) on white paper as the line.

<a href="/examples/line16/arduino-en/line16_02_all_values/line16_02_all_values.ino" download data-ignore="true">📄 Download line16_02_all_values.ino</a>

<!-- ino: examples/line16/arduino-en/line16_02_all_values/line16_02_all_values.ino -->
```cpp
/*
 * Pathfinder 16-Way × Arduino   Example 2: See all the values at once
 * ------------------------------------------------------------
 * What does this example do?
 *   Prints the sensor's whole "value table" every 0.5 s:
 *     01 line position, 02 fine position, 03 line width, 04 segment count, 05 line bitmap,
 *     06 last lost direction, 07 threshold, 08 calibration count, 10~1F light values of channels 1~16.
 *   The later examples only pick a few of these boxes. Get to know every box here first.
 *
 * You need: Pathfinder 16-Way (universal I2C edition) + a sheet of white paper + black electrical tape (as the line).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move the black line left and right under the sensor and watch each box change.
 *
 * Tip:
 *   Boxes with consecutive numbers can be read "in one go" (mbcReadMany). The values come from the same moment.
 *   At most 16 at a time, so 01~08 is one read and 10~1F is another.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move the black line under the sensor."));
}

void loop() {
  int16_t a[8];                          // Holds boxes 01~08
  int16_t light[16];                      // Holds boxes 10~1F (light values of channels 1~16)

  // Part 1: read 8 values in a row starting from 01 (01~08)
  if (!mbcReadMany(ADDR, 0x01, 8, a)) { Serial.println(F("Cannot read (part 1)")); delay(500); return; }
  // Part 2: read 16 values in a row starting from 10 (10~1F)
  if (!mbcReadMany(ADDR, 0x10, 16, light)) { Serial.println(F("Cannot read (part 2)")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 line position    ")); Serial.print(a[0]); Serial.println(F("   (-16 left ~ +16 right, 0 = center)"));
  Serial.print(F("02 fine position    ")); Serial.print(a[1]); Serial.println(F("   (-100 ~ +100, negative = line on the left)"));
  Serial.print(F("03 line width       ")); Serial.print(a[2]); Serial.println(F(" channels (0 = no line found)"));
  Serial.print(F("04 segment count    ")); Serial.print(a[3]); Serial.println(F("   (0 line lost, 1 one line, 2 or more = branch / crossroad)"));
  Serial.print(F("05 line bitmap      "));
  for (uint8_t i = 0; i < 16; i++) Serial.print(((uint16_t)a[4] >> i) & 1);   // Print bit0 (channel 1) on the left ... bit15 (channel 16) on the right
  Serial.println(F("   (left→right = channels 1~16, 1 = that channel is on the line)"));
  Serial.print(F("06 last lost dir    ")); Serial.print(a[5]); Serial.println(F("   (which side the line was on before it was lost, negative = left)"));
  Serial.print(F("07 threshold        ")); Serial.print(a[6]); Serial.println(F("   (1~99, default 50)"));
  Serial.print(F("08 calibration count ")); Serial.print(a[7]); Serial.println(F("   (how many calibrations since boot)"));

  Serial.print(F("10~1F light  L "));
  for (uint8_t i = 0; i < 16; i++) {      // Channel 1 (far left) is printed first
    if (light[i] < 10)  Serial.print(' ');
    if (light[i] < 100) Serial.print(' ');
    Serial.print(light[i]);
    Serial.print(' ');
  }
  Serial.println(F("R   (0 = black, 100 = white)"));

  delay(50);                             // A short rest after reading
  delay(450);                            // A bit more, so the screen updates every 0.5 s and is easy to read
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

**You will see:** (the line a little left of the middle)

```text
---------------------------------------
01 line position    -3   (-16 left ~ +16 right, 0 = center)
02 fine position    -31   (-100 ~ +100, negative = line on the left)
03 line width       2 channels (0 = no line found)
04 segment count    1   (0 line lost, 1 one line, 2 or more = branch / crossroad)
05 line bitmap      0000111000000000   (left→right = channels 1~16, 1 = that channel is on the line)
06 last lost dir    -3   (which side the line was on before it was lost, negative = left)
07 threshold        50   (1~99, default 50)
08 calibration count 1   (how many calibrations since boot)
10~1F light  L 100  98 100  99  12   8  15  95 100 100 100  99 100 100  98 100 R   (0 = black, 100 = white)
```

> [!TIP]
> **Try it:** Slowly lift the sensor higher and watch the light values change. When it is too high, the black and white scores get closer and closer. That is why the sensor must stay close to the floor.

---

<h2 id="ex3">3️⃣ Where is the line?</h2>

**What it does:** Reads three boxes, `01` line position, `02` fine position and `03` line width, draws a small ruler with `#` at the line position, and tells you which way to turn. At the start it sends the `E0` command to follow a black line.

<a href="/examples/line16/arduino-en/line16_03_where_is_line/line16_03_where_is_line.ino" download data-ignore="true">📄 Download line16_03_where_is_line.ino</a>

<!-- ino: examples/line16/arduino-en/line16_03_where_is_line/line16_03_where_is_line.ino -->
```cpp
/*
 * Pathfinder 16-Way × Arduino   Example 3: Where is the line?
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads three boxes: "01 line position", "02 fine position" and "03 line width".
 *   It draws a small ruler, marks the line with #, and tells you which way to turn.
 *   These three boxes are the data used most often for line following.
 *
 * You need: Pathfinder 16-Way (universal I2C edition) + white paper + black electrical tape (about 2 cm wide).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move the black line slowly from the left of the sensor to the right.
 *
 * What do "left" and "right" mean?
 *   The sensor is mounted on the robot (sensing side down). You stand behind the robot and look forward:
 *   line on the left = negative, line on the right = positive, 0 = center. Channel 1 is on the far left.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  mbcWrite(ADDR, 0xE0);                  // E0 = follow a black line (for a white line change to 0xE1)  👉 You can change this
  Serial.println(F("Ready! Move the black line slowly from left to right."));
}

void loop() {
  int16_t v[3];                          // v[0] = 01 line position, v[1] = 02 fine position, v[2] = 03 line width
  if (!mbcReadMany(ADDR, 0x01, 3, v)) {  // Read 3 boxes in one go starting from 01
    Serial.println(F("Cannot read the sensor"));
    delay(500);
    return;
  }
  int16_t pos   = v[0];                  // -16 (far left) ~ +16 (far right)
  int16_t fine  = v[1];                  // -100 ~ +100, finer than the line position
  int16_t width = v[2];                  // How many channels are on the line, 0 = no line in sight

  // Draw a small 9-cell ruler: one cell for each position -4 ~ +4 (-16~+16 divided by 4), # marks the line
  Serial.print('|');
  for (int8_t i = -4; i <= 4; i++) Serial.print((width > 0 && i == pos / 4) ? '#' : ' ');
  Serial.print(F("|  "));

  if (width == 0) {
    Serial.println(F("No line (the line is lost)"));
  } else {
    Serial.print(F("Position ")); Serial.print(pos);
    Serial.print(F("  Fine ")); Serial.print(fine);
    Serial.print(F("  Width ")); Serial.print(width);
    if      (pos < -1) Serial.println(F("  → The line is on the left, turn left"));
    else if (pos >  1) Serial.println(F("  → The line is on the right, turn right"));
    else               Serial.println(F("  → The line is in the middle, go straight!"));
  }
  delay(50);
  delay(150);                            // Print one line every 0.2 s so it is easy to read   👉 You can change this
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

**You will see:** (moving the black line slowly from left to right)

```text
|  #      |  Position -5  Fine -62  Width 2  → The line is on the left, turn left
|    #    |  Position -1  Fine -8  Width 2  → The line is in the middle, go straight!
|      #  |  Position 3  Fine 40  Width 2  → The line is on the right, turn right
|         |  No line (the line is lost)
```

**What does the position mean?**

| Position number | Meaning |
| :---: | :--- |
| 0 | The line is right in the middle |
| Negative (−1 ~ −15) | The line is on the **left**. Smaller = further left |
| Positive (+1 ~ +15) | The line is on the **right**. Bigger = further right |
| ±16 | The line just ran off the far left (−16) or the far right (+16) (only for 200 ms; normally at most ±15) |

> [!TIP]
> **Try it:** Change `mbcWrite(ADDR, 0xE0)` to `0xE1` and use white tape on black paper. Now it follows a white line.

---

<h2 id="ex4">4️⃣ One-key calibration</h2>

**What it does:** Makes the sensor remember the black and white of **this field**. Type `b` (`E2` black calibration), `w` (`E3` white calibration) or `a` (`E7` 5-second dynamic calibration) in the Serial Monitor. The program waits until `F3` bit2 "calibrating" goes back to 0, then prints `08` calibration count to confirm it went up by 1. The result is saved in the sensor, so it remembers even after power off.

**Steps:**
1. Upload the program and open the Serial Monitor (115200).
2. Put the sensor above the line, at the same height as on the robot (about 0.5 ~ 1 cm above the floor).
3. Easiest: type `a` in the input box, press Enter, and **within 5 seconds** slowly move the sensor left and right so that every eye sweeps over the black line and the white background.
4. For better accuracy: put all 16 eyes on the black line and type `b`, then put it on the white background and type `w`.

<a href="/examples/line16/arduino-en/line16_04_calibrate/line16_04_calibrate.ino" download data-ignore="true">📄 Download line16_04_calibrate.ino</a>

<!-- ino: examples/line16/arduino-en/line16_04_calibrate/line16_04_calibrate.ino -->
```cpp
/*
 * Pathfinder 16-Way × Arduino   Example 4: One-key calibration
 * ------------------------------------------------------------
 * What does this example do?
 *   Makes the sensor remember the black and the white of "this field". Type a command in the Serial Monitor:
 *     b  → black calibration (E2): first put all 16 eyes of the sensor on the black line, then type it
 *     w  → white calibration (E3): first put the sensor on the white background, then type it
 *     a  → 5-second dynamic calibration (E7): after typing it, sweep the sensor back and forth over the black line and white background for 5 seconds
 *   The result is saved inside the sensor, so it remembers even after power off.
 *   Do it again on a new field or when the lighting is different.
 *
 * You need: Pathfinder 16-Way (universal I2C edition) + white paper + black electrical tape.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Type b, w or a in the input box and press Enter.
 *   The program waits until "F3 bit2 calibrating" goes back to 0, then prints "08 calibration count" to confirm it went up by 1.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("=== Pathfinder 16-Way: One-key calibration ==="));
  Serial.print(F("Calibration count so far: ")); Serial.println(mbcRead(ADDR, 0x08));
  Serial.println(F("Type b = black calibration, w = white calibration, a = 5 s dynamic calibration, then press Enter"));
}

void loop() {
  if (!Serial.available()) { delay(50); return; }   // No input: keep waiting
  char c = Serial.read();

  if (c == 'b') {
    Serial.println(F("Black calibration (the sensor must be on the black line)..."));
    mbcWrite(ADDR, 0xE2);                // E2 = remember what it sees now as "black"
    waitCalibDone(2000);                 // About 50 ms, wait at most 2 s
  } else if (c == 'w') {
    Serial.println(F("White calibration (the sensor must be on the white background)..."));
    mbcWrite(ADDR, 0xE3);                // E3 = remember what it sees now as "white"
    waitCalibDone(2000);
  } else if (c == 'a') {
    Serial.println(F("5 s dynamic calibration started! Sweep the sensor back and forth over the black line and white background..."));
    mbcWrite(ADDR, 0xE7);                // E7 = within 5 s, take the darkest and brightest as references
    waitCalibDone(8000);                 // 5 s + a little margin
  } else {
    return;                              // Ignore Enter, spaces and other characters
  }

  // Print the light values so you can check: after calibration black is near 0 and white is near 100
  int16_t light[16];
  if (mbcReadMany(ADDR, 0x10, 16, light)) {
    Serial.print(F("Light values of the 16 channels now: L "));
    for (uint8_t i = 0; i < 16; i++) { Serial.print(light[i]); Serial.print(' '); }
    Serial.println(F("R"));
  }
  delay(50);
}

// Wait until "F3 bit2 calibrating" goes back to 0, then print the calibration count
void waitCalibDone(uint16_t ms) {
  unsigned long start = millis();
  delay(20);                             // Give the sensor a moment to raise the flag
  while (millis() - start < ms) {
    int16_t st = mbcRead(ADDR, 0xF3);
    if (st != MBC_NONE && !(st & 0x04)) break;   // bit2 = 0 → done
    Serial.print('.');
    delay(100);
  }
  Serial.println();
  Serial.print(F("Calibration done! Total count: ")); Serial.println(mbcRead(ADDR, 0x08));
  Serial.println(F("Type b / w / a to do it again"));
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
=== Pathfinder 16-Way: One-key calibration ===
Calibration count so far: 0
Type b = black calibration, w = white calibration, a = 5 s dynamic calibration, then press Enter
5 s dynamic calibration started! Sweep the sensor back and forth over the black line and white background...
..................................................
Calibration done! Total count: 1
Type b / w / a to do it again
Light values of the 16 channels now: L 100 99 97 100 98 100 99 100 100 99 97 100 98 100 99 100 R
```

> [!WARNING]
> During calibration, **every eye must sweep over the black line**. If an eye at the edge never sees the line, it will think the whole field is white, and line following will go wrong.

---

<h2 id="ex5">5️⃣ Crossroad detector</h2>

**What it does:** Uses `04` segment count and `05` line bitmap to detect a crossroad: many bits on in the bitmap (on the cross line) or segment count ≥ 2 (a branch) counts as a crossroad. The board's built-in LED (the one marked **L**) is the signal light: on when the line is in sight, off when it is lost, and **3 fast blinks** at a crossroad while the program counts "which crossroad is this".

**You need:** On white paper, one straight line of black tape, plus 2 ~ 3 lines across it as crossroads.

<a href="/examples/line16/arduino-en/line16_05_crossroad/line16_05_crossroad.ino" download data-ignore="true">📄 Download line16_05_crossroad.ino</a>

<!-- ino: examples/line16/arduino-en/line16_05_crossroad/line16_05_crossroad.ino -->
```cpp
/*
 * Pathfinder 16-Way × Arduino   Example 5: Crossroad detector
 * ------------------------------------------------------------
 * What does this example do?
 *   Uses "04 segment count" and "05 line bitmap" to detect a crossroad:
 *     - segment count ≥ 2 → a branch (two or more line segments)
 *     - almost all 16 bits of the bitmap are on (very wide line) → right on the cross line of a crossroad
 *   At a crossroad, the board's built-in LED (L) blinks 3 times fast, and the program counts "which crossroad is this".
 *   Contests often say "turn right at the 3rd crossroad". This example teaches you to count crossroads.
 *
 * You need: Pathfinder 16-Way (universal I2C edition) + white paper with crossroads drawn on it.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move the sensor along the line through a crossroad and watch the LED and the messages.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // On most ESP32 boards the built-in LED is on GPIO 2
#endif

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const uint8_t CROSS_MIN_WIDTH = 12;       // If this many (or more) channels in the bitmap are on, it counts as a crossroad   👉 You can change this

int  crossCount = 0;                     // Which crossroad we are at
bool onCross    = false;                 // Are we on a crossroad right now (so the same crossroad is not counted many times)

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move the sensor along the line through a crossroad."));
}

void loop() {
  int16_t v[2];                          // v[0] = 04 segment count, v[1] = 05 line bitmap
  if (!mbcReadMany(ADDR, 0x04, 2, v)) { delay(100); return; }
  int16_t groups = v[0];
  uint16_t bitmap = (uint16_t)v[1];      // bit0 = channel 1 (far left) ... bit15 = channel 16

  // Count how many 1s are in the bitmap (= how many channels are on the line)
  uint8_t lit = 0;
  for (uint8_t i = 0; i < 16; i++) if (bitmap & (1u << i)) lit++;

  bool isCross = (lit >= CROSS_MIN_WIDTH) || (groups >= 2);   // Very wide, or two or more segments → crossroad

  if (isCross && !onCross) {             // Count only when we just "enter" a crossroad
    onCross = true;
    crossCount++;
    Serial.print(F("Crossroad #")); Serial.print(crossCount); Serial.print(F("!  bitmap "));
    for (uint8_t i = 0; i < 16; i++) Serial.print((bitmap >> i) & 1);   // left→right = channels 1~16
    Serial.print(F("  segments ")); Serial.println(groups);
    for (uint8_t k = 0; k < 3; k++) {    // Blink 3 times fast
      digitalWrite(LED_BUILTIN, HIGH); delay(80);
      digitalWrite(LED_BUILTIN, LOW);  delay(80);
    }
  } else if (!isCross && onCross) {
    onCross = false;                     // Left the crossroad; the next one can be counted
  }

  // Normally: LED on when the line is in sight, off when it is not
  digitalWrite(LED_BUILTIN, lit > 0 ? HIGH : LOW);
  delay(50);
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
Ready! Move the sensor along the line through a crossroad.
Crossroad #1!  bitmap 1111111111111111  segments 1
Crossroad #2!  bitmap 1111100000011111  segments 2
```

> [!TIP]
> **Try it:** Add a line `if (crossCount == 3) { ... }` to do something special at the 3rd crossroad, for example keep the LED blinking. For a thinner line, make `CROSS_MIN_WIDTH` smaller.

---

<h2 id="ex6">6️⃣ Line-following car (advanced)</h2>

**What it does:** Makes a two-wheel car follow a black line by itself. `02` fine position (−100 ~ +100) is the error, and **the further off, the more it steers** (proportional control). `04` segment count = 0 means the line is lost, so it looks at `06` last lost direction and turns in place that way to find the line again. One `mbcReadMany` reads `01` to `06`.

**You also need:** A two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), a battery.

**Motor wiring:**

| L298N | Arduino UNO / Nano / R4 | ESP32 | Controls |
| :---: | :---: | :---: | :--- |
| ENA | 5 | GPIO 25 | Left wheel speed |
| IN1 / IN2 | 7 / 8 | GPIO 26 / 27 | Left wheel direction |
| ENB | 6 | GPIO 14 | Right wheel speed |
| IN3 / IN4 | 9 / 10 | GPIO 32 / 33 | Right wheel direction |
| GND | GND | GND | **Must share GND with the Arduino** |

**Before the first run:**
1. Run [Example 4](#ex4) to calibrate the field.
2. Lift the car (wheels in the air) and upload. Check that both wheels turn **forward**. If a wheel turns the wrong way, change `LEFT_REVERSE` or `RIGHT_REVERSE` to `true`.
3. Put it on the line. It starts after 3 seconds.

<a href="/examples/line16/arduino-en/line16_06_line_follower/line16_06_line_follower.ino" download data-ignore="true">📄 Download line16_06_line_follower.ino</a>

<!-- ino: examples/line16/arduino-en/line16_06_line_follower/line16_06_line_follower.ino -->
```cpp
/*
 * Pathfinder 16-Way × Arduino   Example 6 (advanced): Line-following car
 * ------------------------------------------------------------
 * What does this example do?
 *   Makes a two-wheel car follow a black line by itself. "02 fine position" (-100 ~ +100) is the error:
 *   line to the left → steer left, line to the right → steer right, further off → steer more (proportional control).
 *   When the line is lost (04 segment count = 0), look at "06 last lost direction" and turn in place that way to find it again.
 *
 * You need:
 *   A two-wheel car, an L298N motor driver board (or a TB6612 etc. wired the same way), a battery.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      (left wheel)
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     (right wheel)
 *   (ESP32 instead: ENA → 25, IN1 → 26, IN2 → 27, ENB → 14, IN3 → 32, IN4 → 33)
 *   The L298N GND must be connected to the Arduino GND!
 *   Mount the sensor at the front center of the car, about 0.5~1 cm above the floor.
 *
 * Before the first run:
 *   1. Run "Example 4: One-key calibration" to calibrate the field.
 *   2. Lift the car (wheels in the air), upload, and check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE to true.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- Motor pins ----
#if defined(ESP32)
// On ESP32, pins 6 ~ 11 are connected to the flash memory chip and cannot be used, so we use these instead
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 Settings you can change ----
const int   BASE_SPEED   = 120;          // Straight-line speed: 0 ~ 255. Start slow, it is easier to tune
const float KP           = 1.0;          // Steering strength: increase if the car cannot get back on the line, decrease if it wobbles
const int   SEARCH_SPEED = 100;          // Speed for turning in place when searching for the line
const bool  LEFT_REVERSE  = false;       // Change to true if the left wheel turns the wrong way
const bool  RIGHT_REVERSE = false;       // Change to true if the right wheel turns the wrong way

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  mbcWrite(ADDR, 0xE0);                  // E0 = follow a black line (for a white line change to 0xE1)

  Serial.println(F("Starting in 3 seconds!"));
  delay(3000);
}

void loop() {
  int16_t v[6];                          // 01 line position 02 fine position 03 line width 04 segment count 05 bitmap 06 last lost direction
  if (!mbcReadMany(ADDR, 0x01, 6, v)) {  // Cannot read the sensor → stop the car for safety
    drive(0, 0);
    delay(50);
    return;
  }

  int16_t error   = v[1];                // 02 fine position: -100 (far left) ~ +100 (far right), 0 = center
  int16_t groups  = v[3];                // 04 segment count: 0 = no line in sight
  int16_t lastDir = v[5];                // 06 last lost direction: negative = the line was last on the left

  if (groups == 0) {
    // Line lost: turn in place toward the side where the line disappeared, to find it again
    if (lastDir < 0) drive(-SEARCH_SPEED, SEARCH_SPEED);   // Line was on the left → turn left in place
    else             drive(SEARCH_SPEED, -SEARCH_SPEED);   // Line was on the right → turn right in place
    delay(50);
    return;
  }

  // Proportional control: the further off, the more we steer
  int turn = KP * error;                 // Line on the right (positive) → turn is positive → left wheel faster, right wheel slower → turn right
  drive(BASE_SPEED + turn, BASE_SPEED - turn);
  delay(50);
}

// Control both wheels: speed -255 (full reverse) ~ 255 (full forward)
void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);   // Keep the speed within range
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

**How to tune it so the car runs better:**

| What the car does | What to change |
| :--- | :--- |
| Runs off the line in corners and cannot get back | Increase `KP` (e.g. 1.0 → 1.5), or lower `BASE_SPEED` |
| Wobbles left and right on a straight line | Decrease `KP` (e.g. 1.0 → 0.6) |
| Runs well, want it faster | Add 20 to `BASE_SPEED` at a time, then tune `KP` again |
| Spins in place right at the start | One wheel is reversed. Change `LEFT_REVERSE` or `RIGHT_REVERSE` |

---

<h2 id="ex7">7️⃣ Adjust the threshold</h2>

**What it does:** The "threshold" is the cut-off score the sensor uses to decide "is this channel on the line" (a light value below the threshold counts as on the line, default 50). Type 1 ~ 99 in the Serial Monitor to set it with `E4`; the program reads `07` back to confirm. Type `a` for an `E7` 5-second dynamic calibration. It also prints the 16 light values and the `05` line bitmap once per second, so you can see which channels count as "on the line" after the change.

<a href="/examples/line16/arduino-en/line16_07_threshold/line16_07_threshold.ino" download data-ignore="true">📄 Download line16_07_threshold.ino</a>

<!-- ino: examples/line16/arduino-en/line16_07_threshold/line16_07_threshold.ino -->
```cpp
/*
 * Pathfinder 16-Way × Arduino   Example 7: Adjust the threshold
 * ------------------------------------------------------------
 * What does this example do?
 *   The "threshold" is the cut-off score the sensor uses to decide "is this channel on the line":
 *   a light value (0 black ~ 100 white) below the threshold counts as on the line. Default 50.
 *   Adjust it when the line is faint or the field is shiny.
 *   Type in the Serial Monitor:
 *     a number 1~99 then Enter → set the threshold with E4, then read 07 to confirm
 *     a                        → 5 s dynamic calibration with E7 (a bad threshold is often just a missing calibration)
 *   The program also prints the 16 light values and the line bitmap once per second, so you can see which channels count as "on the line" after the change.
 *
 * You need: Pathfinder 16-Way (universal I2C edition) + white paper + black electrical tape.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * Tip:
 *   The threshold set with E4 is kept in RAM. After the sensor reboots it goes back to the default 50.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("=== Pathfinder 16-Way: Adjust the threshold ==="));
  Serial.print(F("Current threshold (07): ")); Serial.println(mbcRead(ADDR, 0x07));
  Serial.println(F("Type 1~99 to set the threshold, or a for 5 s dynamic calibration, then press Enter"));
}

void loop() {
  // ---- Handle input ----
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s == "a") {
      Serial.println(F("5 s dynamic calibration started! Sweep the sensor back and forth over the black line and white background..."));
      mbcWrite(ADDR, 0xE7);              // E7 = 5 s dynamic calibration
      unsigned long start = millis();
      delay(20);
      while (millis() - start < 8000) {  // Wait until F3 bit2 "calibrating" goes back to 0
        int16_t st = mbcRead(ADDR, 0xF3);
        if (st != MBC_NONE && !(st & 0x04)) break;
        delay(100);
      }
      Serial.println(F("Calibration done"));
    } else if (s.length() > 0) {
      int n = s.toInt();
      if (n >= 1 && n <= 99) {
        uint8_t th = (uint8_t)n;
        mbcWrite(ADDR, 0xE4, &th, 1);    // E4 + 1 parameter = set the threshold
        delay(20);
        Serial.print(F("Threshold set to ")); Serial.print(n);
        Serial.print(F(", read back 07 = ")); Serial.println(mbcRead(ADDR, 0x07));
      } else {
        Serial.println(F("Please type a number 1~99, or a"));
      }
    }
  }

  // ---- Print the light values and bitmap once per second ----
  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    int16_t light[16];
    int16_t bitmap = mbcRead(ADDR, 0x05);
    if (mbcReadMany(ADDR, 0x10, 16, light) && bitmap != MBC_NONE) {
      Serial.print(F("Light L "));
      for (uint8_t i = 0; i < 16; i++) { Serial.print(light[i]); Serial.print(' '); }
      Serial.print(F("R  on line (L→R) "));
      for (uint8_t i = 0; i < 16; i++) Serial.print(((uint16_t)bitmap >> i) & 1 ? '#' : '.');   // Channel 1 (bit0) is printed on the far left
      Serial.println();
    }
  }
  delay(50);
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
=== Pathfinder 16-Way: Adjust the threshold ===
Current threshold (07): 50
Type 1~99 to set the threshold, or a for 5 s dynamic calibration, then press Enter
Light L 100 98 100 99 12 8 35 100 100 100 99 100 100 98 100 99 R  on line (L→R) ....###.........
Threshold set to 30, read back 07 = 30
Light L 100 98 100 99 12 8 35 100 100 100 99 100 100 98 100 99 R  on line (L→R) ....##..........
```

> [!TIP]
> Raise the threshold when the line is faint (grey); lower it when the field is shiny. The threshold set with `E4` is kept in RAM, so the sensor goes back to 50 after a reboot. To make it stick, send `E4` once at the start of your program every time.

---

## 🔧 Shared command examples (the same for every MBC universal I2C product)

These three are in `examples/i2c/arduino-en/`. They work with the Pathfinder, Ranger, Controller, IMU and Sharpshooter. Just change `ADDR` / `OLD_ADDR` at the top of the program to your product's address.

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

**What it does:** Scans `0x08` ~ `0x77`. For every device that answers it reads `F0` to tell which MBC product it is, its firmware version, and whether the address was changed. One run tells you if the wiring works.

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
  0x16  Pathfinder 16-Way  firmware 4.0  protocol v2
Total: 1 device(s)
```

---

## ❓ FAQ

**Q: It keeps saying "Sensor not found"?**
1. Check that SDA and SCL are not swapped (UNO: SDA → A4, SCL → A5; ESP32: 21 / 22).
2. Check that VCC and GND are plugged in firmly. Is the LED on the sensor lit?
3. Make sure it is the **universal I2C edition**. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
4. Run [Shared 3 Scan](#exi3) to see whether any device is on the bus and whether the address was changed.

**Q: What does `-32768` mean?**
`-32768` (called `MBC_NONE` in the code) means one of two things: ① the helper cannot read the sensor (loose wire, wrong address); ② that number is an empty slot. For example the 16-Way only has `10` ~ `1F`, so reading `20` gives `-32768`. A real value can never be this number, so you can use it to tell "this data cannot be used".

**Q: ESP32 cannot read, or the values look strange?**
Some MBC products (Controller PS4, Sharpshooter) are built on ESP32 and cannot "clock stretch", so the host must **wait 2 ms** after writing the number before reading. The helper `mbcReadMany` already has `delay(2)`. Please do not remove it, and do the same in your own code. When an ESP32 is the host, its default I2C pins are GPIO 21 (SDA) / 22 (SCL).

**Q: The Serial Monitor shows garbage?**
The baud rate is not **115200**. Change it at the bottom right of the Serial Monitor.

**Q: The line is in the middle, but the position keeps jumping?**
Run [Example 4](#ex4) to calibrate first. Then check the height of the sensor above the floor (0.5 ~ 1 cm is best).

**Q: Can I connect two Pathfinders at the same time?**
Yes, but first use [Shared 1](#exi1) to change one of them to another address (for example `0x17`). Otherwise both are `0x16` and they clash on the same SDA/SCL bus. MBC products with different addresses (for example the IMU at `0x6A`) can share the bus as they are.

**Q: I want to know what every box holds?**
The [Command Overview](#cmd-table) above is the full list. For the detailed packet format see the [I2C Register Guide](/en/sensors/line16/arduino-i2c.md).
