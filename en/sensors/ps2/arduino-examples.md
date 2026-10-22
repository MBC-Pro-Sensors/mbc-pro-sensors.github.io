<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 Controller PS2 × Arduino Examples

[🔙 Back to Controller 2.4G Receiver (PS2) Home](/en/sensors/ps2/index.md)　|　[∿ MBC Universal I2C Protocol v2 (advanced)](/en/i2c-protocol.md)

This page has **6 examples**, from "is it connected?" all the way to a "tank-style remote-control car". You can copy and upload each example as it is, and every line of the code has an English comment. **Never written code before? No problem.** Just do them one by one, in order.

> [!NOTE]
> The Controller 2.4G Receiver (PS2) is sold **in Taiwan only**. Outside Taiwan, please use the [Controller PS4/PS5 Bluetooth Receiver](/en/sensors/ps4/arduino-examples.md). Its box numbers are the same, so these examples also work with it.

> [!TIP]
> First time using Arduino? Read [🔰 First Time with Arduino: Start from Zero](/en/sensors/arduino-start.md) first. In 15 minutes you learn how to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/ps2/arduino-en/ps2_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
</div>

---

## 🔌 Wiring (the same for every example)

| Receiver | Arduino UNO / Nano | Arduino UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: |
| VCC | 5V | 5V | 5V (VIN) |
| GND | GND | GND | GND |
| RX (= SDA) | A4 | SDA | GPIO 21 |
| TX (= SCL) | A5 | SCL | GPIO 22 |

<div style="text-align: center;">
<img src="/images/figures/ps2/wiring-uno.svg" alt="Wiring diagram: Controller PS2 receiver to Arduino UNO" style="max-width: 520px; width: 100%; margin: 1rem auto; display: block;" />
</div>

> [!IMPORTANT]
> - Use the **Universal I2C edition** of the receiver. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
> - The receiver is marked **TX, RX**: on the I2C edition **TX is SCL and RX is SDA**.
> - The **ANALOG button** in the middle of the gamepad must have its light on (analog mode), otherwise the sticks give no numbers. Light off = digital mode, only the buttons work (box `0B` "gamepad type" tells you: 1 digital, 2 analog).
> - The receiver has a built-in level shifter, so UNO (5 V), UNO R4 and ESP32 (3.3 V) can all connect directly.
> - Unplug the USB cable before you connect the wires.

---

<h2 id="cmds">📖 Command overview (what this receiver can give you)</h2>

Think of the receiver as a row of numbered "boxes". Each box holds one number (int16). Write `[number]`, then read 2 bytes, and you get one value. Boxes next to each other can be read in one row (up to 16). In the examples, `mbcRead(ADDR, number)` and `mbcReadMany(ADDR, first number, how many, array)` do exactly this. The PS2 and the PS4/PS5 receivers use **exactly the same** box numbers.

<!-- gen:cmd-table -->
| Number | Name | Range / unit | Meaning | Used in |
| :---: | :--- | :--- | :--- | :--- |
| `01` | 16 buttons | 0~65535 (bits) | All 16 buttons in one number (PSX order): bit0 SELECT, 1 L3, 2 R3, 3 START, 4 ↑, 5 →, 6 ↓, 7 ←, 8 L2, 9 R2, 10 L1, 11 R1, 12 ▲, 13 ●, 14 ✕, 15 ■; 1 = pressed; not connected = 0 | Examples 2, 3 |
| `02` | D-pad | 0~15 | ↑ 1, → 2, ↓ 4, ← 8; pressing several adds them up | Examples 2, 3 |
| `03` | Shape buttons | 0~15 | ▲ 1, ● 2, ✕ 4, ■ 8 | Examples 2, 3 |
| `04` | Shoulder buttons | 0~15 | L2 1, R2 2, L1 4, R1 8 | Examples 2, 3 |
| `05` | Function buttons | 0~15 | SELECT 1, L3 2, R3 4, START 8 | Examples 2, 3 |
| `06` | Connected | 0~1 | 1 = gamepad connected; when disconnected all buttons are 0 and sticks are 0 | Examples 2, 5, 6 |
| `07` | Battery | 0 | A PS2 gamepad has no battery level, **always 0** (only PS4/PS5 have it) | Examples 1, 2, 5 |
| `08` | L2 analog | 0~255 | Only a DualShock 2 in analog mode gives a pressure value; other gamepads: pressed = 255, released = 0 | Examples 2, 5 |
| `09` | R2 analog | 0~255 | Same as above | Examples 2, 5 |
| `0A` | System buttons | 0 | A PS2 gamepad has no PS button, **always 0** | Examples 2, 5 |
| `0B` | Gamepad type | 0~2 | 0 no gamepad, 1 PS2 digital mode, 2 PS2 analog mode (ANALOG light on) | Examples 2, 5 |
| `10`~`13` | Sticks LX / LY / RX / RY | −100~+100 | 0 = middle, right / up are positive (all 0 in digital mode) | Examples 2, 4, 6 |
| `F0` | Device ID | — | low byte = factory address 0x42, high byte = type (**1 PS2**, 2 PS4/PS5) | Example 1 |
| `F1` | Protocol version | 2 | These examples are written for version 2 | Example 1 |
| `F2` | Firmware version | major x 100 + minor | for example 102 = 1.2 | Example 1 |
| `F3` | Status flags | bits | b0 ready, b1 gamepad connected | Example 1, mbcWaitReady in every example |
| `F5` | Seconds since power-on | 0~32767 | If the number gets smaller, the receiver has restarted | Example 1 |
| `E5` (write) | Rumble | [strength] | The PS2 receiver replies ACK but **ignores it** (only PS4/PS5 rumble), so there is no example on this page | — |
| `E6` (write) | Light bar color | [R][G][B] | Same as above, a PS2 gamepad has no light bar | — |
<!-- /gen -->

> [!NOTE]
> A box that does not exist (for example `0C`~`0F`) reads as `-32768` (called `MBC_NONE` in the examples). You also get `-32768` when the receiver cannot be read. Full specification: [MBC Universal I2C Protocol v2](/en/i2c-protocol.md).

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the receiver](#ex1) | Check the wiring, read the ID card (F0~F3), see whether the gamepad is connected | Nothing |
| 2 | [Print all the values at once](#ex2) | See what every box holds (01~0B, 10~13) | Nothing |
| 3 | [Which button did you press?](#ex3) | Split the 16-button value into bits, the four button groups | Nothing |
| 4 | [How far is the stick pushed?](#ex4) | Sticks ±100, drawn as a cross chart | Nothing |
| 5 | [Gamepad status](#ex5) | Connection, analog triggers, gamepad type | Nothing |
| 6 | [Tank-style remote-control car (advanced)](#ex6) | Drive with two sticks, stop automatically on disconnect | Two-wheel car, L298N motor driver board |

> [!TIP]
> **Suggested order: 1 → 2 → 3 → 4 → 5 → 6.** Only example 6 needs extra parts.

---

<h2 id="ex1">1️⃣ Say hello to the receiver</h2>

**What it does:** Checks that the receiver is connected and prints its "ID card" (factory address, type, protocol version, firmware version, status flags). Then it keeps watching the gamepad and tells you when it connects or disconnects. Run this one first every time you connect a new receiver.

**Steps:**
1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.
4. Turn on the gamepad (the light on the receiver turns on) and watch for "Gamepad connected!".

<a href="/examples/ps2/arduino-en/ps2_01_hello/ps2_01_hello.ino" download data-ignore="true">📄 Download ps2_01_hello.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_01_hello/ps2_01_hello.ino -->
```cpp
/*
 * Controller PS2 x Arduino   Example 1: Say hello to the receiver
 * ------------------------------------------------------------
 * What does this example do?
 *   Checks that the receiver is connected. If it is, it prints the receiver's
 *   "ID card" (device ID, type, protocol version, firmware version) and its
 *   current status. Then it keeps watching the gamepad: it tells you when the
 *   gamepad connects and when it disconnects.
 *   Run this example first every time you connect a new receiver.
 *
 * You need: Controller PS2 receiver (Universal I2C edition) + PS2 gamepad, Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Turn on the gamepad (the light on the receiver turns on) and watch for "Gamepad connected!".
 *
 * Tip:
 *   The ANALOG button in the middle of the gamepad must have its light on (analog mode),
 *   otherwise the sticks give no numbers.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the receiver

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

bool wasConnected = false;               // Was the gamepad connected the last time we looked?

void setup() {
  Serial.begin(115200);                  // Open the channel to the computer at 115200 baud
  mbcBegin();                            // Get I2C ready (helper at the bottom of the file)
  delay(500);                            // Give the receiver 0.5 s to boot

  Serial.println(F("=== Controller PS2: Say hello to the receiver ==="));

  // Wait until the receiver says "I am ready" (bit0 of status flags F3), up to 5 seconds
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found! Please check:"));
    Serial.println(F("  1. RX (SDA) -> A4, TX (SCL) -> A5 (ESP32: 21 / 22). Are they swapped?"));
    Serial.println(F("  2. VCC -> 5V, GND -> GND. Plugged in firmly? Is the light on the receiver on?"));
    Serial.println(F("  3. Is your receiver the Universal I2C edition? (SPIKE and EV3 editions cannot connect to Arduino)"));
    while (true) {}                      // Stop here if not found
  }

  // ---- F0 device ID: low byte = factory address, high byte = type (1 = PS2, 2 = PS4/PS5) ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t factoryAddr = (uint8_t)(id & 0xFF);        // Factory address (stays the same even if you changed the address)
  uint8_t kind        = (uint8_t)((id >> 8) & 0xFF); // hw[0]: 1 = PS2 receiver, 2 = PS4/PS5 receiver
  Serial.print(F("Found it! Factory address 0x"));
  Serial.println(factoryAddr, HEX);      // Should be 42
  Serial.print(F("Type: "));
  if (kind == 1)      Serial.println(F("PS2 2.4G receiver"));
  else if (kind == 2) Serial.println(F("PS4/PS5 Bluetooth receiver"));
  else                Serial.println(F("Unknown type"));

  // ---- F1 protocol version (these examples are written for version 2) ----
  Serial.print(F("Protocol version: "));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 firmware version: major x 100 + minor (for example 102 = 1.2) ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("Firmware version: "));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 status flags: each bit means one thing ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("Current status:"));
  Serial.print(F("  bit0 ready              ")); Serial.println((st & 0x01) ? F("yes") : F("no"));
  Serial.print(F("  bit1 gamepad connected  ")); Serial.println((st & 0x02) ? F("yes") : F("no"));

  // ---- F5 seconds since power-on (just to see how long it has been running) ----
  Serial.print(F("Seconds since power-on: "));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("Wiring is correct! Now turn on the gamepad (the light on the receiver turns on)."));
}

void loop() {
  // Every pass, look at bit1 of F3 (is the gamepad connected?). Only print when it changes, so the screen stays clean
  int16_t st = mbcRead(ADDR, 0xF3);
  if (st == MBC_NONE) {                  // Could not read = a wire came loose
    Serial.println(F("Cannot read the receiver. Did a wire come loose?"));
    delay(1000);
    return;
  }
  bool connected = st & 0x02;
  if (connected != wasConnected) {
    if (connected) {
      Serial.print(F("Gamepad connected! Battery "));
      Serial.print(mbcRead(ADDR, 0x07));  // 07 battery 0~100 (a PS2 gamepad has no battery level, always 0)
      Serial.println(F("%"));
    } else {
      Serial.println(F("Gamepad not connected"));
    }
    wasConnected = connected;
  }
  delay(50);
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] -> wait 2 ms -> read 2 x k bytes = k int16 values (little-endian) starting at that number
//   If the number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// From the sensor at address addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting at number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (ESP32-based products need this)
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

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), up to ms milliseconds. Returns true when ready
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

**You will see:** (gamepad connects, then you turn it off)

```text
=== Controller PS2: Say hello to the receiver ===
Found it! Factory address 0x42
Type: PS2 2.4G receiver
Protocol version: 2
Firmware version: 1.2
Current status:
  bit0 ready              yes
  bit1 gamepad connected  no
Seconds since power-on: 3
Wiring is correct! Now turn on the gamepad (the light on the receiver turns on).
Gamepad connected! Battery 0%
Gamepad not connected
```

> [!NOTE]
> **What are the "Communication helpers"?** At the bottom of every example there is the same piece of code. It "talks" to the receiver. You don't need to change it or understand it. Just remember:
> - `mbcRead(ADDR, number)`: read one value (returns `-32768` if it cannot be read)
> - `mbcReadMany(ADDR, first number, how many, array)`: read a row of values starting at a number (up to 16, all from the same instant)
> - `mbcWrite(ADDR, command, parameters, how many bytes)`: send a command (not needed for PS2)
> - `mbcWaitReady(ADDR, milliseconds)`: wait until the receiver has booted
>
> The helpers contain one line `delay(2)` (wait 2 ms after writing the number, before reading). The PS2 receiver itself does not need it, but the PS4/PS5 receiver at the same address does, so every MBC example keeps it. The cost is tiny.
>
> A PS2 gamepad has no battery level, so "Battery 0%" is normal.

---

<h2 id="ex2">2️⃣ Print all the values at once</h2>

**What it does:** Every 0.5 s it prints every value in the value table: `01`~`0B` in one read of 11, `10`~`13` in one read of 4. Press buttons and push the sticks, watch which line changes, and you know what each box is.

<a href="/examples/ps2/arduino-en/ps2_02_all_values/ps2_02_all_values.ino" download data-ignore="true">📄 Download ps2_02_all_values.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_02_all_values/ps2_02_all_values.ino -->
```cpp
/*
 * Controller PS2 x Arduino   Example 2: Print all the values at once
 * ------------------------------------------------------------
 * What does this example do?
 *   Every 0.5 s it prints every value in the receiver's "value table", so you can
 *   see what each number holds:
 *     01~0B: buttons, connection, battery, triggers, system buttons, gamepad type (11 values in one read)
 *     10~13: the four stick axes LX, LY, RX, RY (4 values in one read)
 *   Numbers that are next to each other can be read "in one row" (mbcReadMany),
 *   and you get the values from the same instant.
 *
 * You need: Controller PS2 receiver (Universal I2C edition) + PS2 gamepad, Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Press buttons, push the sticks, and watch which number changes.
 *
 * Tip:
 *   The ANALOG button in the middle of the gamepad must have its light on (analog mode),
 *   otherwise the sticks give no numbers.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;             // When we last printed

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Printing all values every 0.5 s."));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 You can change this: print every 0.5 s
    lastPrint = millis();

    // ---- Part 1: numbers 01~0B, 11 values in one read ----
    int16_t a[11];
    if (!mbcReadMany(ADDR, 0x01, 11, a)) { Serial.println(F("Cannot read (part 1)")); return; }
    // ---- Part 2: numbers 10~13, 4 values in one read ----
    int16_t s[4];
    if (!mbcReadMany(ADDR, 0x10, 4, s)) { Serial.println(F("Cannot read (part 2)")); return; }

    Serial.println(F("------------------------------"));
    Serial.print(F("01 16 buttons (PSX order, binary): ")); printBits16((uint16_t)a[0]); Serial.println();
    Serial.print(F("02 D-pad (up1 right2 down4 left8)     : ")); Serial.println(a[1]);
    Serial.print(F("03 Shape buttons (▲1 ●2 ✕4 ■8)        : ")); Serial.println(a[2]);
    Serial.print(F("04 Shoulder (L2 1 R2 2 L1 4 R1 8)      : ")); Serial.println(a[3]);
    Serial.print(F("05 Function (SELECT1 L3 2 R3 4 START8) : ")); Serial.println(a[4]);
    Serial.print(F("06 Connected (1 = yes)                 : ")); Serial.println(a[5]);
    Serial.print(F("07 Battery 0~100 (PS2 always 0)        : ")); Serial.println(a[6]);
    Serial.print(F("08 L2 analog 0~255                     : ")); Serial.println(a[7]);
    Serial.print(F("09 R2 analog 0~255                     : ")); Serial.println(a[8]);
    Serial.print(F("0A System buttons (b0 PS, b1 capture)  : ")); Serial.println(a[9]);
    Serial.print(F("0B Gamepad type 0~4                    : ")); Serial.println(a[10]);
    Serial.print(F("10 LX  11 LY  12 RX  13 RY (±100): "));
    for (uint8_t i = 0; i < 4; i++) { Serial.print(s[i]); Serial.print(i < 3 ? F("  ") : F("")); }
    Serial.println();
  }
  delay(50);
}

// Print a 16-bit number as 16 zeros and ones (left = bit15 ■, right = bit0 SELECT)
void printBits16(uint16_t v) {
  for (int8_t i = 15; i >= 0; i--) {
    Serial.print((v >> i) & 1);
    if (i % 4 == 0 && i > 0) Serial.print(' ');   // A space every 4 bits, easier to read
  }
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] -> wait 2 ms -> read 2 x k bytes = k int16 values (little-endian) starting at that number
//   If the number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// From the sensor at address addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting at number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (ESP32-based products need this)
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

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), up to ms milliseconds. Returns true when ready
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

**You will see:** (holding ✕ and ↑, left stick pushed all the way right; gamepad in analog mode)

```text
Ready! Printing all values every 0.5 s.
------------------------------
01 16 buttons (PSX order, binary): 0100 0000 0001 0000
02 D-pad (up1 right2 down4 left8)     : 1
03 Shape buttons (▲1 ●2 ✕4 ■8)        : 4
04 Shoulder (L2 1 R2 2 L1 4 R1 8)      : 0
05 Function (SELECT1 L3 2 R3 4 START8) : 0
06 Connected (1 = yes)                 : 1
07 Battery 0~100 (PS2 always 0)        : 0
08 L2 analog 0~255                     : 0
09 R2 analog 0~255                     : 0
0A System buttons (b0 PS, b1 capture)  : 0
0B Gamepad type 0~4                    : 2
10 LX  11 LY  12 RX  13 RY (±100): 100  0  0  0
```

> [!TIP]
> In the binary line, the left end is bit15 (■) and the right end is bit0 (SELECT). Above, ✕ is bit14 and ↑ is bit4, so you see two 1s. If the sticks are all 0, press the **ANALOG** button on the gamepad (the light must be on).

---

<h2 id="ex3">3️⃣ Which button did you press?</h2>

**What it does:** Press any button on the gamepad and the computer tells you which one; it also tells you when you release it. You can press several at the same time. When any of the four "button groups" (D-pad, shape, shoulder, function) changes, it prints one more line.

<a href="/examples/ps2/arduino-en/ps2_03_buttons/ps2_03_buttons.ino" download data-ignore="true">📄 Download ps2_03_buttons.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_03_buttons/ps2_03_buttons.ino -->
```cpp
/*
 * Controller PS2 x Arduino   Example 3: Which button did you press?
 * ------------------------------------------------------------
 * What does this example do?
 *   Press any button on the gamepad and the computer tells you which one. It also
 *   tells you when you release it. You can press several buttons at the same time.
 *   The state of all 16 buttons is packed into one number (number 01, "16 buttons").
 *   Each button takes one "bit": pressed = 1, not pressed = 0, like 16 light switches in a row.
 *   Numbers 02~05 are the same buttons in "groups": D-pad, shape buttons, shoulder
 *   buttons, function buttons, 4 buttons each. Each group is 0~15; pressing several
 *   adds them up (for example up + right = 1 + 2 = 3).
 *
 * You need: Controller PS2 receiver (Universal I2C edition) + PS2 gamepad, Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Turn on the gamepad (the light on the receiver turns on) and press buttons.
 *
 * Tip:
 *   The ANALOG button in the middle of the gamepad must have its light on (analog mode),
 *   otherwise the sticks give no numbers.
 *   A PS2 gamepad has no PS button, so number 0A (system buttons) is always 0.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

uint16_t lastButtons = 0;                // The 16-button state we saw last time
int16_t  lastGroup[4] = {0, 0, 0, 0};    // The four groups 02~05 we saw last time

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Press the buttons on the gamepad."));
}

void loop() {
  // Read numbers 01~05 (5 values) in one go: [0] 16 buttons [1] D-pad [2] shape [3] shoulder [4] function
  int16_t v[5];
  if (!mbcReadMany(ADDR, 0x01, 5, v)) {
    delay(100);                          // Could not read this time, try again shortly
    return;
  }
  uint16_t buttons = (uint16_t)v[0];     // 16 buttons: PSX order (bit0 SELECT ... bit15 ■)

  // ---- 1. Check button by button: which were "just pressed" and which were "just released" ----
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t mask = 1u << i;             // The "bit" of button i
    bool now    = buttons & mask;        // Is it pressed now?
    bool before = lastButtons & mask;    // Was it pressed last time?
    if (now && !before) { Serial.print(F("Pressed: ")); printButtonName(i); Serial.println(); }
    if (!now && before) { Serial.print(F("Released: ")); printButtonName(i); Serial.println(); }
  }
  lastButtons = buttons;                 // Remember this state so we can compare next time

  // ---- 2. If any of the four group values changed, print one line (D-pad / shape / shoulder / function, each 0~15) ----
  bool changed = false;
  for (uint8_t i = 0; i < 4; i++) if (v[i + 1] != lastGroup[i]) changed = true;
  if (changed) {
    Serial.print(F("    Groups -> D-pad ")); Serial.print(v[1]);
    Serial.print(F("  Shape ")); Serial.print(v[2]);
    Serial.print(F("  Shoulder ")); Serial.print(v[3]);
    Serial.print(F("  Function ")); Serial.println(v[4]);
    for (uint8_t i = 0; i < 4; i++) lastGroup[i] = v[i + 1];
  }

  delay(50);                             // Check every 0.05 s, so even fast presses are caught
}

// Print the name of bit i in the 16-button value (PSX order)
void printButtonName(uint8_t i) {
  switch (i) {
    case 0:  Serial.print(F("SELECT (PS4: SHARE / PS5: Create)")); break;
    case 1:  Serial.print(F("L3 (press the left stick down)")); break;
    case 2:  Serial.print(F("R3 (press the right stick down)")); break;
    case 3:  Serial.print(F("START (PS4/PS5: OPTIONS)")); break;
    case 4:  Serial.print(F("↑ D-pad up")); break;
    case 5:  Serial.print(F("→ D-pad right")); break;
    case 6:  Serial.print(F("↓ D-pad down")); break;
    case 7:  Serial.print(F("← D-pad left")); break;
    case 8:  Serial.print(F("L2")); break;
    case 9:  Serial.print(F("R2")); break;
    case 10: Serial.print(F("L1")); break;
    case 11: Serial.print(F("R1")); break;
    case 12: Serial.print(F("▲ Triangle")); break;
    case 13: Serial.print(F("● Circle")); break;
    case 14: Serial.print(F("✕ Cross")); break;
    case 15: Serial.print(F("■ Square")); break;
  }
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] -> wait 2 ms -> read 2 x k bytes = k int16 values (little-endian) starting at that number
//   If the number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// From the sensor at address addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting at number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (ESP32-based products need this)
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

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), up to ms milliseconds. Returns true when ready
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

**You will see:** (press ✕, then press ↑ and R1 together)

```text
Ready! Press the buttons on the gamepad.
Pressed: ✕ Cross
    Groups -> D-pad 0  Shape 4  Shoulder 0  Function 0
Released: ✕ Cross
    Groups -> D-pad 0  Shape 0  Shoulder 0  Function 0
Pressed: ↑ D-pad up
Pressed: R1
    Groups -> D-pad 1  Shape 0  Shoulder 8  Function 0
```

**Which bit is each of the 16 buttons?**

| Bit | Button | Bit | Button |
| :---: | :--- | :---: | :--- |
| 0 | SELECT | 8 | L2 |
| 1 | L3 (press the left stick down) | 9 | R2 |
| 2 | R3 (press the right stick down) | 10 | L1 |
| 3 | START | 11 | R1 |
| 4 | ↑ up | 12 | ▲ Triangle |
| 5 | → right | 13 | ● Circle |
| 6 | ↓ down | 14 | ✕ Cross |
| 7 | ← left | 15 | ■ Square |

> [!TIP]
> **Try it:** To know "is ✕ pressed?", just write `if (buttons & (1u << 14)) { ... }`; or read box `03` (shape buttons) and write `if (v & 4)`.

---

<h2 id="ex4">4️⃣ How far is the stick pushed?</h2>

**What it does:** Turns the position of the two sticks into numbers (box `10` LX, `11` LY, `12` RX, `13` RY): released = `0`, pushed right or up is **positive**, pushed left or down is **negative**, all the way is `+100` or `-100`. Prints every 0.3 s and also draws two cross charts.

<a href="/examples/ps2/arduino-en/ps2_04_sticks/ps2_04_sticks.ino" download data-ignore="true">📄 Download ps2_04_sticks.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_04_sticks/ps2_04_sticks.ino -->
```cpp
/*
 * Controller PS2 x Arduino   Example 4: How far is the stick pushed?
 * ------------------------------------------------------------
 * What does this example do?
 *   Turns the position of the two sticks into numbers (number 10 LX, 11 LY, 12 RX, 13 RY):
 *     released (in the middle) = 0; pushed right or up = positive, all the way = +100;
 *     pushed left or down = negative, all the way = -100.
 *   Prints the numbers every 0.3 s and draws two "cross charts": O is where the stick is now.
 *
 * You need: Controller PS2 receiver (Universal I2C edition) + PS2 gamepad, Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Push the stick slowly and watch the O move.
 *
 * Tip:
 *   When the stick wobbles a little near the middle, the number stays at 0. The receiver does this on purpose
 *   (it is called a "dead zone"), so a car does not creep when you let go of the stick.
 *   The ANALOG button in the middle of the gamepad must have its light on (analog mode),
 *   otherwise the sticks give no numbers.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Push the sticks."));
}

void loop() {
  if (millis() - lastPrint >= 300) {     // 👉 You can change this: print every 0.3 s
    lastPrint = millis();

    // Read numbers 10~13 (4 values) in one go: [0] LX [1] LY [2] RX [3] RY (-100 ~ +100)
    int16_t s[4];
    if (!mbcReadMany(ADDR, 0x10, 4, s)) { Serial.println(F("Cannot read the receiver")); return; }

    Serial.print(F("Left stick X ")); Serial.print(s[0]);
    Serial.print(F("  Y ")); Serial.print(s[1]);
    Serial.print(F("   |   Right stick X ")); Serial.print(s[2]);
    Serial.print(F("  Y ")); Serial.println(s[3]);
    drawCross(s[0], s[1], s[2], s[3]);   // Draw the two cross charts
  }
  delay(50);
}

// Draw two 7x7 cross charts (left stick and right stick side by side). The center is +, the stick position is O
//   X: -100 ~ +100 -> column 0 ~ 6 (left -> right); Y: up is positive, so +100 is the top row
void drawCross(int lx, int ly, int rx, int ry) {
  int lcol = map(lx, -100, 100, 0, 6), lrow = map(ly, 100, -100, 0, 6);
  int rcol = map(rx, -100, 100, 0, 6), rrow = map(ry, 100, -100, 0, 6);
  for (int row = 0; row < 7; row++) {
    for (int col = 0; col < 7; col++) Serial.print(cell(row, col, lrow, lcol));
    Serial.print(F("     "));            // 5 spaces between the two charts
    for (int col = 0; col < 7; col++) Serial.print(cell(row, col, rrow, rcol));
    Serial.println();
  }
  Serial.println();
}

// Decide which character to print in one cell: O = stick position, + = center, | and - = cross lines, . = empty
char cell(int row, int col, int posRow, int posCol) {
  if (row == posRow && col == posCol) return 'O';
  if (row == 3 && col == 3) return '+';
  if (col == 3) return '|';
  if (row == 3) return '-';
  return '.';
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] -> wait 2 ms -> read 2 x k bytes = k int16 values (little-endian) starting at that number
//   If the number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// From the sensor at address addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting at number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (ESP32-based products need this)
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

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), up to ms milliseconds. Returns true when ready
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

**You will see:** (left stick pushed to the top right corner, right stick released)

```text
Ready! Push the sticks.
Left stick X 100  Y 100   |   Right stick X 0  Y 0
...|..O     ...|...
...|...     ...|...
...|...     ...|...
---+---     ---O---
...|...     ...|...
...|...     ...|...
...|...     ...|...

```

> [!NOTE]
> When the stick wobbles a little near the middle, the number stays at `0`. The receiver does this on purpose (it is called a "dead zone"), so a car does not creep when you let go of the stick. If the numbers are always `0`: press the **ANALOG** button on the gamepad.

---

<h2 id="ex5">5️⃣ Gamepad status</h2>

**What it does:** Every 0.5 s it prints boxes `06`~`0B` (6 values in one read): connected, battery (PS2 always 0), L2 / R2 triggers, system buttons (PS2 always 0), gamepad type (1 digital / 2 analog). When the connection state changes, it prints an extra line.

<a href="/examples/ps2/arduino-en/ps2_05_status/ps2_05_status.ino" download data-ignore="true">📄 Download ps2_05_status.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_05_status/ps2_05_status.ino -->
```cpp
/*
 * Controller PS2 x Arduino   Example 5: Gamepad status
 * ------------------------------------------------------------
 * What does this example do?
 *   Every 0.5 s it prints the status values that are not buttons or sticks (numbers 06~0B, 6 values in one read):
 *     06 connected (1 = yes), 07 battery 0~100, 08 how hard L2 is pressed 0~255,
 *     09 how hard R2 is pressed 0~255, 0A system buttons (b0 PS button, b1 capture button), 0B gamepad type.
 *   When the connection state changes, it prints an extra line to tell you.
 *
 * You need: Controller PS2 receiver (Universal I2C edition) + PS2 gamepad, Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Press L2 and R2 slowly and watch the numbers; turn the gamepad off and watch "Connected" become 0.
 *
 * Tip:
 *   A PS2 gamepad has no battery level and no PS button: 07 and 0A are always 0.
 *   L2 / R2 analog: only a DualShock 2 in analog mode gives a pressure value; other gamepads give pressed = 255, released = 0.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;
int16_t lastConnected = -1;              // The connection state last time (-1 = not seen yet)

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Printing the gamepad status every 0.5 s."));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 You can change this: print every 0.5 s
    lastPrint = millis();

    // Read numbers 06~0B (6 values) in one go: [0] connected [1] battery [2] L2 [3] R2 [4] system buttons [5] gamepad type
    int16_t v[6];
    if (!mbcReadMany(ADDR, 0x06, 6, v)) { Serial.println(F("Cannot read the receiver")); return; }

    // If the connection state changed, print one line to tell you
    if (v[0] != lastConnected) {
      Serial.println(v[0] ? F(">>> Gamepad connected!") : F(">>> Gamepad not connected"));
      lastConnected = v[0];
    }

    Serial.print(F("Connected ")); Serial.print(v[0]);
    Serial.print(F(" | Battery ")); Serial.print(v[1]); Serial.print(F("%"));
    Serial.print(F(" | L2 ")); Serial.print(v[2]);
    Serial.print(F(" R2 ")); Serial.print(v[3]);
    Serial.print(F(" | PS button ")); Serial.print((v[4] & 0x01) ? 1 : 0);     // System buttons bit0
    Serial.print(F(" Capture ")); Serial.print((v[4] & 0x02) ? 1 : 0);     // System buttons bit1
    Serial.print(F(" | Gamepad: "));
    printPadType(v[5]);
    Serial.println();
  }
  delay(50);
}

// 0B gamepad type: 0 none, 1 PS2 digital, 2 PS2 analog, 3 PS4, 4 PS5 (or other)
void printPadType(int16_t t) {
  switch (t) {
    case 0: Serial.print(F("No gamepad")); break;
    case 1: Serial.print(F("PS2 gamepad (digital mode, sticks do nothing)")); break;
    case 2: Serial.print(F("PS2 gamepad (analog mode, ANALOG light on)")); break;
    case 3: Serial.print(F("PS4 gamepad")); break;
    case 4: Serial.print(F("PS5 gamepad (or other)")); break;
    default: Serial.print(F("Unknown"));
  }
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] -> wait 2 ms -> read 2 x k bytes = k int16 values (little-endian) starting at that number
//   If the number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// From the sensor at address addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting at number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (ESP32-based products need this)
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

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), up to ms milliseconds. Returns true when ready
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

**You will see:** (gamepad connected, holding L2, then pressing ANALOG to switch to digital mode)

```text
Ready! Printing the gamepad status every 0.5 s.
>>> Gamepad connected!
Connected 1 | Battery 0% | L2 255 R2 0 | PS button 0 Capture 0 | Gamepad: PS2 gamepad (analog mode, ANALOG light on)
Connected 1 | Battery 0% | L2 0 R2 0 | PS button 0 Capture 0 | Gamepad: PS2 gamepad (digital mode, sticks do nothing)
```

---

<h2 id="ex6">6️⃣ Tank-style remote-control car (advanced)</h2>

**What it does:** Drives a two-wheel car with the two sticks: left stick up/down (LY) controls the left wheel, right stick up/down (RY) controls the right wheel. The stick value `-100`~`+100` is multiplied straight into the speed. If the gamepad disconnects or the receiver cannot be read, the car stops at once.

**You also need:** A two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), and batteries.

**Motor wiring:**

| L298N | Arduino UNO / Nano / R4 | ESP32 |
| :---: | :---: | :---: |
| ENA, IN1, IN2 (left wheel) | 5, 7, 8 | 25, 26, 27 |
| ENB, IN3, IN4 (right wheel) | 6, 9, 10 | 14, 32, 33 |
| GND | GND | GND |

> [!WARNING]
> Before the first run, **lift the car up** (wheels in the air) and upload: push both sticks up and check that both wheels turn forward (if one turns the wrong way, change `LEFT_REVERSE` / `RIGHT_REVERSE`). Then turn the gamepad off and check that the wheels stop. Only then put the car on the floor.

<a href="/examples/ps2/arduino-en/ps2_06_tank_car/ps2_06_tank_car.ino" download data-ignore="true">📄 Download ps2_06_tank_car.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_06_tank_car/ps2_06_tank_car.ino -->
```cpp
/*
 * Controller PS2 x Arduino   Example 6: Tank-style remote-control car (advanced)
 * ------------------------------------------------------------
 * What does this example do?
 *   Drives a two-wheel car with the gamepad, the same way a tank is driven:
 *     left stick up/down (LY) -> left wheel; right stick up/down (RY) -> right wheel
 *     both sticks up = go straight; one up and one down = turn on the spot.
 *   The stick value -100 ~ +100 is used directly as the motor speed (times the top speed).
 *   If the gamepad disconnects, or the receiver cannot be read, the car stops at once.
 *
 * You need: Controller PS2 receiver (Universal I2C edition) + PS2 gamepad, Arduino UNO / Nano / UNO R4 or ESP32. Also: a two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), batteries.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   The L298N GND must be connected to the Arduino GND!
 *   (ESP32 uses different pins, see "Motor pins" below)
 *
 * Before you start:
 *   1. Lift the car up (wheels in the air) and upload. Push both sticks up and check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE to true.
 *   2. ⚠️ While the car is still lifted, turn the gamepad off and check that the wheels stop. Only then put it on the floor.
 *
 * How to see the result:
 *   The Serial Monitor (115200) prints the left and right wheel speed every 0.3 s.
 *
 * Tip:
 *   The ANALOG button in the middle of the gamepad must have its light on (analog mode),
 *   otherwise the sticks give no numbers.
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- Motor pins ----
#if defined(ESP32)
// ESP32 pins 6 ~ 11 are connected to the on-board memory chip and cannot be used, so we use these instead
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 You can change these ----
const int  MAX_SPEED     = 200;          // Speed when the stick is pushed all the way: 0 ~ 255, start slow
const bool LEFT_REVERSE  = false;        // Change to true if the left wheel turns the wrong way
const bool RIGHT_REVERSE = false;        // Change to true if the right wheel turns the wrong way

bool stopped = true;                     // Are we stopped right now? (we start out stopped)
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Drive with the two sticks."));
}

void loop() {
  // Read numbers 06 ~ 13 (14 values) in one go (data from the same instant):
  //   v[0] = 06 connected ... v[10] = 10 LX, v[11] = 11 LY, v[12] = 12 RX, v[13] = 13 RY
  //   (0C~0F in the middle are empty slots and read as -32768; just ignore them)
  int16_t v[14];
  if (!mbcReadMany(ADDR, 0x06, 14, v) || v[0] == 0) {
    safeStop();                          // Cannot read the receiver, or the gamepad is not connected -> stop at once
    delay(50);
    return;
  }
  if (stopped) {
    stopped = false;
    Serial.println(F("Gamepad connected, you can drive!"));
  }

  int ly = v[11];                        // Left stick up/down: up = positive (-100 ~ +100)
  int ry = v[13];                        // Right stick up/down: up = positive

  // The further the stick is pushed, the faster the wheel turns: -100 ~ +100 becomes -MAX_SPEED ~ +MAX_SPEED
  int left  = (long)ly * MAX_SPEED / 100;
  int right = (long)ry * MAX_SPEED / 100;
  drive(left, right);

  if (millis() - lastPrint >= 300) {     // Print the speeds every 0.3 s
    lastPrint = millis();
    Serial.print(F("Left ")); Serial.print(left);
    Serial.print(F("   Right ")); Serial.println(right);
  }
  delay(50);
}

// Stop the car, and say so only once, right when it stops
void safeStop() {
  drive(0, 0);
  if (!stopped) {
    stopped = true;
    Serial.println(F("Cannot read the gamepad -> stop!"));
  }
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
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] -> wait 2 ms -> read 2 x k bytes = k int16 values (little-endian) starting at that number
//   If the number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// From the sensor at address addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting at number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (ESP32-based products need this)
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

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), up to ms milliseconds. Returns true when ready
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

**You will see:** (both sticks pushed halfway up, then the gamepad is turned off)

```text
Ready! Drive with the two sticks.
Gamepad connected, you can drive!
Left 100   Right 100
Cannot read the gamepad -> stop!
```

> [!TIP]
> This example uses one `mbcReadMany(ADDR, 0x06, 14, v)` to read from `06` to `13` (14 values): `v[0]` is "connected", `v[11]` is LY, `v[13]` is RY, all from the same instant, so there is no need to read twice. The boxes `0C`~`0F` in the middle are empty and read as `-32768`; just ignore them.

---

## ❓ FAQ

**Q: It keeps saying "Receiver not found"?**
1. Check that TX and RX are not swapped (UNO: RX → A4, TX → A5; R4: SDA / SCL; ESP32: 21 / 22).
2. Check that VCC and GND are plugged in firmly. Is the light on the receiver on?
3. Make sure it is the **Universal I2C edition**. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
4. If the receiver has its own power supply and boots later than the Arduino, press the RESET button on the Arduino and try again.

**Q: The Serial Monitor shows strange characters?**
The speed is not set to **115200**. Change it at the bottom right of the Serial Monitor and it will be fine.

**Q: The buttons work, but the sticks are always 0?**
The gamepad is in digital mode. Press the **ANALOG** button in the middle of the gamepad. When its light is on, the sticks work. Box `0B` changes from 1 to 2.

**Q: What does `-32768` mean?**
It is the code for "no such value" (called `MBC_NONE` in the examples): either the box is empty (for example `0C`~`0F`), or the receiver cannot be read (a wire is loose). In your own code, check `v != MBC_NONE` before you use the value.

**Q: Can I remove the `delay(2)` in the helpers?**
The PS2 receiver itself does not need it, but the PS4/PS5 receiver (built on an ESP32) and the Sharpshooter do. Both receivers use the same address, so a program cannot tell them apart in advance. That is why every MBC example waits 2 ms. We suggest you keep it.

**Q: What should I know when the ESP32 is the host?**
Wire it to GPIO 21 (SDA) / 22 (SCL). The motor pins in the car example are different on ESP32 and UNO; the code already separates them with `#if defined(ESP32)`.

**Q: Can I connect the PS2 and the PS4/PS5 receiver at the same time?**
No. Both have the house number `0x42`, and only one of them can be on the same SDA/SCL wires. But their **box numbers are exactly the same**, so the examples on this page also work with the PS4/PS5 receiver. For rumble and the light bar, see [PS4/PS5 examples 7 and 8](/en/sensors/ps4/arduino-examples.md).

**Q: I want to know what data each box holds?**
See the [Command overview](#cmds) above and the [MBC Universal I2C Protocol v2](/en/i2c-protocol.md) (advanced information for people who can code).
