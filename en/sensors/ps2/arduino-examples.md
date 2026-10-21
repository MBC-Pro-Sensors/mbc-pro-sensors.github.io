<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 Controller PS2 × Arduino Examples

[🔙 Back to Controller 2.4G Receiver (PS2) Home](/en/sensors/ps2/index.md)　|　[∿ MBC Universal I2C Protocol (advanced)](/en/i2c-protocol.md)

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

| Receiver | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: |
| VCC | 5V | 5V (VIN) |
| GND | GND | GND |
| RX (= SDA) | A4 | GPIO 21 |
| TX (= SCL) | A5 | GPIO 22 |

> [!IMPORTANT]
> - Use the **Universal I2C edition** of the receiver. The SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
> - The receiver's pins are printed **TX and RX**: on the I2C edition, **TX is SCL and RX is SDA**.
> - Keep the light of the **ANALOG button** in the middle of the gamepad on (analog mode), or the sticks give no numbers. Light off = digital mode, only the buttons work.
> - Unplug the USB cable before you connect the wires.

---

## 📋 All Examples

| # | Example | What you learn | You also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to the receiver](#ex1) | Check the wiring and whether the gamepad is connected | Nothing |
| 2 | [Which button did I press?](#ex2) | Read the 16 buttons | Nothing |
| 3 | [How far is the stick pushed?](#ex3) | Read the numbers of the sticks and triggers | Nothing |
| 4 | [Remote-control light](#ex4) | Control the light on the board with buttons and a stick | Nothing |
| 5 | [A servo that follows the stick (advanced)](#ex5) | Control an angle with a stick | SG90 servo |
| 6 | [Tank-style remote-control car (advanced)](#ex6) | Drive with two sticks, stop by itself when the connection is lost | Two-wheel car, L298N motor driver board |

> [!TIP]
> **Suggested order: 1 → 2 → 3 → 4 → 5 → 6.** Each example uses what you learned in the one before.

---

<h2 id="ex1">1️⃣ Say hello to the receiver</h2>

**What it does:** Checks that the receiver is connected, and shows whether the gamepad is connected. Run this one first every time you connect a new receiver.

**Steps:**
1. Connect the 4 wires as in the table above, then plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass at the top right) and choose **115200** baud.
4. Switch the gamepad on and see if the computer says "gamepad connected".

<a href="/examples/ps2/arduino-en/ps2_01_hello/ps2_01_hello.ino" download data-ignore="true">📄 Download ps2_01_hello.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_01_hello/ps2_01_hello.ino -->
```cpp
/*
 * Controller PS2 x Arduino  Example 1: Say hello to the receiver
 * ------------------------------------------------------------
 * What does this example do?
 *   Checks that the receiver is connected. If it is, the computer shows the receiver's name and version.
 *   Then it keeps watching the gamepad: it tells you when the gamepad connects or disconnects.
 *   Run this example first every time you connect a new receiver.
 *
 * You need:
 *   Controller PS2 receiver (Universal I2C edition) + a PS2 2.4G wireless gamepad (with batteries in).
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *   (On the I2C edition, the pin printed TX is SCL and the pin printed RX is SDA)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Then switch the gamepad on and see if the computer says "Gamepad connected".
 *
 * Good to know:
 *   There is an ANALOG button in the middle of the gamepad. Light on = analog mode (the sticks work),
 *   light off = digital mode (only the buttons work, the sticks do nothing).
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the receiver

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's "address", always 0x42

bool    wasConnected = false;            // Was the gamepad connected the last time we looked?
uint8_t lastType     = 0;                // The gamepad mode we saw last time

void setup() {
  Serial.begin(115200);                  // Open the channel to the computer, speed 115200
  mbcBegin();                            // Get I2C ready (the helpers are at the bottom of the file)
  delay(500);                            // Give the receiver 0.5 seconds to power on

  Serial.println(F("=== Controller PS2: Say hello to the receiver ==="));

  // Wait until the receiver says "I'm ready", up to 5 seconds
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found! Please check:"));
    Serial.println(F("  1. RX (SDA) goes to A4 and TX (SCL) goes to A5. Are they swapped?"));
    Serial.println(F("  2. VCC goes to 5V and GND goes to GND. Are they plugged in firmly? Is the light on the receiver on?"));
    Serial.println(F("  3. Is your receiver the Universal I2C edition? (The SPIKE and EV3 editions cannot connect to Arduino)"));
    while (true) {}                      // Not found: stop here
  }

  // Read the "version data": box 0x01 of the receiver, 17 numbers
  uint8_t info[17];
  mbcRead(0x01, info, 17);

  // Numbers 9 ~ 16 are the receiver's name (letters)
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("Found it! Receiver model: "));
  Serial.println(name);                  // Should print PS2

  Serial.print(F("Firmware version: "));
  Serial.print(info[2]);                 // Major version
  Serial.print('.');
  Serial.println(info[3]);               // Minor version

  Serial.print(F("Type: "));
  if (info[7] == 1)      Serial.println(F("PS2 2.4G receiver"));
  else if (info[7] == 2) Serial.println(F("PS4/PS5 Bluetooth receiver"));
  else                   Serial.println(F("Unknown type"));

  Serial.println(F("Wiring is correct! Now switch the gamepad on."));
}

void loop() {
  // Read the "status data": box 0x31, 9 numbers
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {
    Serial.println(F("Cannot read the receiver. Is a wire loose?"));
    delay(1000);
    return;
  }

  bool    connected = st[0] & 0x02;      // Bit 1 of number 0: 1 = gamepad connected
  uint8_t type      = st[2];             // Gamepad mode: 0 = not connected, 1 = digital mode, 2 = analog mode

  // Only print when something changed, so the screen doesn't fill up
  if (connected != wasConnected || type != lastType) {
    if (!connected) {
      Serial.println(F("Gamepad not connected (please switch the gamepad on)"));
    } else if (type == 2) {
      Serial.println(F("Gamepad connected! (Analog mode: buttons and sticks both work)"));
    } else {
      Serial.println(F("Gamepad connected! (Digital mode: the sticks do nothing, press the ANALOG button in the middle of the gamepad)"));
    }
    wasConnected = connected;
    lastType = type;
  }

  delay(200);                            // Look again every 0.2 seconds
}

// ============================================================
// Below are the "Communication helpers": they talk to the receiver.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

// Start I2C communication (call once in setup)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// Read n numbers from box reg of the receiver into buf. Returns true if it worked
bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this; on the PS2 receiver it does no harm)
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// Write the number value into box reg of the receiver. Returns true if it worked
bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

// Wait until the receiver is ready, up to timeoutMs milliseconds. Returns true when ready
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
=== Controller PS2: Say hello to the receiver ===
Found it! Receiver model: PS2
Firmware version: 1.0
Type: PS2 2.4G receiver
Wiring is correct! Now switch the gamepad on.
Gamepad connected! (Analog mode: buttons and sticks both work)
Gamepad connected! (Digital mode: the sticks do nothing, press the ANALOG button in the middle of the gamepad)
```

(The second "Digital mode" line appears when you press the ANALOG button on the gamepad and its light goes off. Press it again to go back to analog mode.)

> [!NOTE]
> **What are the "Communication helpers"?** At the bottom of every example there is the same piece of code. It "talks" to the receiver. You don't need to change it or understand it. Just remember:
> - `mbcRead(box number, where to put it, how many)`: read data from the receiver
> - `mbcWrite(box number, number)`: write a setting into the receiver
>
> Think of the receiver as a row of numbered "boxes" (programmers call them registers). Each box holds different data. For example, box `0x11` holds "the gamepad's buttons and sticks", and box `0x31` holds "is the gamepad connected?".
>
> From example 2 on, there is one more piece of code above the helpers: `readPad`. It reads the gamepad data (box `0x11`) and does a "sanity check": wrong readings are thrown away. It is also the same in every example, and you don't need to change it. Why do we need it? See the [FAQ](#faq-updown).

---

<h2 id="ex2">2️⃣ Which button did I press?</h2>

**What it does:** Press any button on the gamepad and the computer tells you which one you pressed. It also tells you when you let go. You can press several at once.

<a href="/examples/ps2/arduino-en/ps2_02_buttons/ps2_02_buttons.ino" download data-ignore="true">📄 Download ps2_02_buttons.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_02_buttons/ps2_02_buttons.ino -->
```cpp
/*
 * Controller PS2 x Arduino  Example 2: Which button did I press?
 * ------------------------------------------------------------
 * What does this example do?
 *   Press any button on the gamepad and the computer tells you "Pressed: ✕ Cross".
 *   When you let go it says "Released: ✕ Cross". You can press several at once.
 *
 * You need:
 *   Controller PS2 receiver (Universal I2C edition) + a PS2 2.4G wireless gamepad.
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Switch the gamepad on and press some buttons.
 *
 * Good to know:
 *   The state of the 16 buttons is kept in one 16-bit number. Each button uses one "bit":
 *   pressed = 1, not pressed = 0. Like 16 light switches in a row.
 *   Pressing "up and down together" or "left and right together" on the D-pad is not printed:
 *   a gamepad cannot really do that, so the program treats it as a wrong reading and throws it away
 *   (see readPad at the bottom of the file).
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

uint16_t lastButtons = 0;                // The button state we saw last time

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Press the buttons on the gamepad."));
}

void loop() {
  // Read the "gamepad data": box 0x11, 9 numbers (readPad is at the bottom of the file and also checks for wrong readings)
  //   [0][1] buttons (16 bits)  [2]~[5] sticks  [6][7] L2, R2 triggers  [8] system buttons
  uint8_t d[9];
  if (!readPad(d)) {
    delay(100);                          // Could not read this time, try again in a moment
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // Join the two numbers into the 16-bit button state

  // Check the buttons one by one: which were "just pressed" and which were "just released"
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t mask = 1u << i;             // The "bit" of button i
    bool now    = buttons & mask;        // Is it pressed now?
    bool before = lastButtons & mask;    // Was it pressed last time?

    if (now && !before) {                // Just pressed
      Serial.print(F("Pressed: "));
      printButtonName(i);
      Serial.println();
    }
    if (!now && before) {                // Just released
      Serial.print(F("Released: "));
      printButtonName(i);
      Serial.println();
    }
  }

  lastButtons = buttons;                 // Remember this time, to compare next time
  delay(20);                             // Look every 0.02 seconds, so even quick presses are caught
}

// Print the name of button i (i = which bit)
void printButtonName(uint8_t i) {
  switch (i) {
    case 0:  Serial.print(F("SELECT button")); break;
    case 1:  Serial.print(F("L3 (press the left stick down)")); break;
    case 2:  Serial.print(F("R3 (press the right stick down)")); break;
    case 3:  Serial.print(F("START button")); break;
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
// Read the gamepad data and do a "sanity check" (the same in every example, no need to change it)
// ============================================================
// Why a sanity check?
//   When the receiver is very busy (for example, talking wirelessly to the gamepad), once in a while
//   it is not ready in time and gives the Arduino "the box it read last time" instead
//   (for example the status data in box 0x31).
//   Then the button and stick numbers are wrong, and a remote-control car could start moving by itself.
// How can we tell?
//   On the D-pad you cannot press "up" and "down" at the same time,
//   and you cannot press "left" and "right" at the same time.
//   If we read that, this reading is wrong: throw it away and keep using the last good one.

uint8_t lastGood[9] = {0};               // The last good gamepad data (all 0 at the start = nothing pressed)

// Read box 0x11 (gamepad data, 9 numbers) into d. Returns false if the receiver cannot be read
bool readPad(uint8_t *d) {
  if (!mbcRead(0x11, d, 9)) return false;
  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];                     // The 16-bit button state
  bool upDown    = (buttons & (1u << 4)) && (buttons & (1u << 6));   // Up (bit 4) and down (bit 6) pressed together?
  bool leftRight = (buttons & (1u << 7)) && (buttons & (1u << 5));   // Left (bit 7) and right (bit 5) pressed together?
  if (upDown || leftRight) {
    memcpy(d, lastGood, 9);              // Wrong reading -> throw it away, use the last good one
  } else {
    memcpy(lastGood, d, 9);              // Good reading -> remember it, in case the next one is wrong
  }
  return true;
}

// ============================================================
// Below are the "Communication helpers": they talk to the receiver.
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
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this; on the PS2 receiver it does no harm)
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

**You will see:** (press ✕, then press ↑ and R1 together)

```text
Ready! Press the buttons on the gamepad.
Pressed: ✕ Cross
Released: ✕ Cross
Pressed: ↑ D-pad up
Pressed: R1
Released: ↑ D-pad up
Released: R1
```

(Pressing "up and down together" or "left and right together" on the D-pad is not printed, because the program treats it as a wrong reading. See the [FAQ](#faq-updown) for why.)

**Which bit is each of the 16 buttons?**

| Bit | Button | Bit | Button |
| :---: | :--- | :---: | :--- |
| 0 | SELECT | 8 | L2 |
| 1 | L3 (press the left stick down) | 9 | R2 |
| 2 | R3 (press the right stick down) | 10 | L1 |
| 3 | START | 11 | R1 |
| 4 | ↑ Up | 12 | ▲ Triangle |
| 5 | → Right | 13 | ● Circle |
| 6 | ↓ Down | 14 | ✕ Cross |
| 7 | ← Left | 15 | ■ Square |

> [!TIP]
> **Try it:** To know "is ✕ pressed?", just write `if (buttons & (1u << 14)) { ... }`. `1u << 14` means "bit 14".

---

<h2 id="ex3">3️⃣ How far is the stick pushed?</h2>

**What it does:** Turns the position of the two sticks into numbers: let go = `0`, pushing right or up gives a **positive** number, pushing left or down gives a **negative** number, and all the way is `+127` or `-127`. It also prints the L2 and R2 triggers (not pressed `0`, pressed `255`).

<a href="/examples/ps2/arduino-en/ps2_03_sticks/ps2_03_sticks.ino" download data-ignore="true">📄 Download ps2_03_sticks.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_03_sticks/ps2_03_sticks.ino -->
```cpp
/*
 * Controller PS2 x Arduino  Example 3: How far is the stick pushed?
 * ------------------------------------------------------------
 * What does this example do?
 *   Turns the position of the two sticks into numbers and prints them on the computer:
 *     let go (in the middle)  = 0
 *     push right or up        = positive, all the way = +127
 *     push left or down       = negative, all the way = -127
 *   It also prints the two triggers, L2 and R2: not pressed = 0, pressed = 255
 *   (if the gamepad supports "pressure sensing", it gives 0 ~ 255 depending on how hard you press).
 *
 * You need:
 *   Controller PS2 receiver (Universal I2C edition) + a PS2 2.4G wireless gamepad.
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Push the sticks slowly and watch the numbers.
 *
 * Good to know:
 *   1. Sticks always 0? Press the ANALOG button in the middle of the gamepad so its light turns on (analog mode).
 *   2. When the stick moves a little near the middle, the number stays at 0. The receiver does this on purpose
 *      (it is called a "dead zone"), so a car does not slowly drive off by itself when you let go of the stick.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

bool    connected = false;               // Is the gamepad connected? (updated every 0.4 seconds)
uint8_t type      = 0;                   // Gamepad mode: 1 = digital mode, 2 = analog mode
unsigned long lastStatus = 0;            // The last time we checked "is the gamepad connected?"

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Push the sticks."));
}

void loop() {
  // ---- 1. Every 0.4 seconds, check if the gamepad is connected: box 0x31 (status data) ----
  //   Don't read it every loop: switching between two boxes all the time makes it easier for the receiver to mix up the data
  if (millis() - lastStatus >= 400) {
    lastStatus = millis();
    uint8_t st[9];
    if (mbcRead(0x31, st, 9)) {
      connected = st[0] & 0x02;          // Bit 1 of number 0: 1 = gamepad connected
      type      = st[2];                 // 1 = digital mode, 2 = analog mode
    } else {
      connected = false;                 // Cannot read the receiver: count it as not connected
    }
  }

  if (!connected) {
    Serial.println(F("Gamepad not connected (please switch the gamepad on)"));
    delay(1000);
    return;
  }
  if (type == 1) {
    Serial.println(F("The gamepad is in digital mode, the sticks do nothing! Press the ANALOG button in the middle of the gamepad (its light turns on)"));
    delay(1000);
    return;
  }

  // ---- 2. Read the "gamepad data": box 0x11, 9 numbers ----
  //   readPad is at the bottom of the file and also checks for wrong readings
  uint8_t d[9];
  if (!readPad(d)) {
    delay(200);
    return;
  }

  // Stick numbers can be positive or negative, so we use int8_t (a box that holds -128 ~ 127)
  int lx = (int8_t)d[2];                 // Left stick left/right: right = positive
  int ly = (int8_t)d[3];                 // Left stick up/down: up = positive
  int rx = (int8_t)d[4];                 // Right stick left/right: right = positive
  int ry = (int8_t)d[5];                 // Right stick up/down: up = positive
  int l2 = d[6];                         // L2 trigger: 0 ~ 255
  int r2 = d[7];                         // R2 trigger: 0 ~ 255

  // Print in neat columns. %+4d = "with a + or - sign, 4 spaces wide", %3d = "3 spaces wide"
  char line[80];
  snprintf(line, sizeof(line), "Left stick X%+4d Y%+4d | Right stick X%+4d Y%+4d | L2 %3d R2 %3d",
           lx, ly, rx, ry, l2, r2);
  Serial.println(line);

  delay(200);                            // 👉 You can change this: print every 0.2 seconds, make it smaller to see faster
}

// ============================================================
// Read the gamepad data and do a "sanity check" (the same in every example, no need to change it)
// ============================================================
// Why a sanity check?
//   When the receiver is very busy (for example, talking wirelessly to the gamepad), once in a while
//   it is not ready in time and gives the Arduino "the box it read last time" instead
//   (for example the status data in box 0x31).
//   Then the button and stick numbers are wrong, and a remote-control car could start moving by itself.
// How can we tell?
//   On the D-pad you cannot press "up" and "down" at the same time,
//   and you cannot press "left" and "right" at the same time.
//   If we read that, this reading is wrong: throw it away and keep using the last good one.

uint8_t lastGood[9] = {0};               // The last good gamepad data (all 0 at the start = nothing pressed)

// Read box 0x11 (gamepad data, 9 numbers) into d. Returns false if the receiver cannot be read
bool readPad(uint8_t *d) {
  if (!mbcRead(0x11, d, 9)) return false;
  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];                     // The 16-bit button state
  bool upDown    = (buttons & (1u << 4)) && (buttons & (1u << 6));   // Up (bit 4) and down (bit 6) pressed together?
  bool leftRight = (buttons & (1u << 7)) && (buttons & (1u << 5));   // Left (bit 7) and right (bit 5) pressed together?
  if (upDown || leftRight) {
    memcpy(d, lastGood, 9);              // Wrong reading -> throw it away, use the last good one
  } else {
    memcpy(lastGood, d, 9);              // Good reading -> remember it, in case the next one is wrong
  }
  return true;
}

// ============================================================
// Below are the "Communication helpers": they talk to the receiver.
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
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this; on the PS2 receiver it does no harm)
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

**You will see:** (first let go, then push the left stick all the way up, and last push the left stick half way left, the right stick all the way right, and hold L2)

```text
Ready! Push the sticks.
Left stick X  +0 Y  +0 | Right stick X  +0 Y  +0 | L2   0 R2   0
Left stick X  +0 Y+127 | Right stick X  +0 Y  +0 | L2   0 R2   0
Left stick X -64 Y  +0 | Right stick X+127 Y  +0 | L2 255 R2   0
```

> [!NOTE]
> When the stick moves a little near the middle, the number stays at `0`. The receiver does this on purpose (it is called a "dead zone"), so a car does not slowly drive off by itself when you let go of the stick. That is why you never see small stick numbers like `1` ~ `7`.

---

<h2 id="ex4">4️⃣ Remote-control light</h2>

**What it does:** Practice remote control with the small built-in light on the Arduino board (the one marked **L**): hold **✕** and the light stays on, hold **●** and the light goes off. When you press nothing the light blinks, and **the more you push the left stick up, the faster it blinks**.

<a href="/examples/ps2/arduino-en/ps2_04_led_blink/ps2_04_led_blink.ino" download data-ignore="true">📄 Download ps2_04_led_blink.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_04_led_blink/ps2_04_led_blink.ino -->
```cpp
/*
 * Controller PS2 x Arduino  Example 4: Remote-control light
 * ------------------------------------------------------------
 * What does this example do?
 *   Uses the gamepad to control the small built-in light on the Arduino board (the one marked L):
 *     hold ✕            -> the light stays on
 *     hold ●            -> the light goes off
 *     press nothing     -> the light blinks; the more you push the left stick up, the faster it blinks
 *
 * You need:
 *   Controller PS2 receiver (Universal I2C edition) + a PS2 2.4G wireless gamepad.
 *   No extra light needed. We use the light that is already on the board.
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *
 * How to see the result:
 *   Watch the L light on the Arduino board. The Serial Monitor (115200) also prints the blink speed.
 *
 * Good to know:
 *   Stick does nothing? Press the ANALOG button in the middle of the gamepad so its light turns on (analog mode).
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards don't say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address
const uint8_t LED = LED_BUILTIN;         // The light on the board (pin 13 on the UNO)

// Where the buttons are in the 16-bit number (which bit)
const uint8_t BTN_CROSS  = 14;           // ✕ Cross
const uint8_t BTN_CIRCLE = 13;           // ● Circle

// ---- 👉 You can change these settings ----
const int SLOW_MS = 1000;                // Stick let go: switch every 1 second (on -> off -> on)
const int FAST_MS = 50;                  // Stick pushed all the way: switch every 0.05 seconds

bool ledOn = false;                      // Is the light on or off now?
unsigned long lastToggle = 0;            // The last time we switched the light
int lastShown = -1;                      // The speed we printed last time (don't print the same one again)

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // Set the light's pin to "output" so we can turn it on and off
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Press ✕ for light on, ● for light off, push the left stick to change the blink speed."));
}

void loop() {
  // Read the gamepad data (box 0x11, 9 numbers). readPad is at the bottom of the file and also checks for wrong readings
  uint8_t d[9];
  if (!readPad(d)) {                     // Cannot read -> to be safe, turn the light off
    digitalWrite(LED, LOW);
    delay(100);
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // The 16-bit button state
  int ly = (int8_t)d[3];                 // Left stick up/down: up = positive (0 ~ +127)

  // When the gamepad is not connected, the buttons and sticks are all 0, so the light blinks slowly like "press nothing"

  // ---- 1. Hold ✕: the light stays on ----
  if (buttons & (1u << BTN_CROSS)) {
    digitalWrite(LED, HIGH);
    delay(20);
    return;
  }

  // ---- 2. Hold ●: the light goes off ----
  if (buttons & (1u << BTN_CIRCLE)) {
    digitalWrite(LED, LOW);
    delay(20);
    return;
  }

  // ---- 3. Any other time: blink; the more you push the left stick up, the faster it blinks ----
  if (ly < 0) ly = 0;                    // Pushing down counts as not pushing
  int interval = map(ly, 0, 127, SLOW_MS, FAST_MS);   // Change 0~127 into 1000~50 milliseconds

  if (millis() - lastToggle >= (unsigned long)interval) {   // Time is up: switch once
    lastToggle = millis();
    ledOn = !ledOn;                      // On becomes off, off becomes on
    digitalWrite(LED, ledOn ? HIGH : LOW);
  }

  // Only print when the speed changed
  if (abs(interval - lastShown) >= 50) {
    lastShown = interval;
    Serial.print(F("Switch every "));
    Serial.print(interval);
    Serial.println(F(" milliseconds"));
  }

  delay(20);
}

// ============================================================
// Read the gamepad data and do a "sanity check" (the same in every example, no need to change it)
// ============================================================
// Why a sanity check?
//   When the receiver is very busy (for example, talking wirelessly to the gamepad), once in a while
//   it is not ready in time and gives the Arduino "the box it read last time" instead
//   (for example the status data in box 0x31).
//   Then the button and stick numbers are wrong, and a remote-control car could start moving by itself.
// How can we tell?
//   On the D-pad you cannot press "up" and "down" at the same time,
//   and you cannot press "left" and "right" at the same time.
//   If we read that, this reading is wrong: throw it away and keep using the last good one.

uint8_t lastGood[9] = {0};               // The last good gamepad data (all 0 at the start = nothing pressed)

// Read box 0x11 (gamepad data, 9 numbers) into d. Returns false if the receiver cannot be read
bool readPad(uint8_t *d) {
  if (!mbcRead(0x11, d, 9)) return false;
  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];                     // The 16-bit button state
  bool upDown    = (buttons & (1u << 4)) && (buttons & (1u << 6));   // Up (bit 4) and down (bit 6) pressed together?
  bool leftRight = (buttons & (1u << 7)) && (buttons & (1u << 5));   // Left (bit 7) and right (bit 5) pressed together?
  if (upDown || leftRight) {
    memcpy(d, lastGood, 9);              // Wrong reading -> throw it away, use the last good one
  } else {
    memcpy(lastGood, d, 9);              // Good reading -> remember it, in case the next one is wrong
  }
  return true;
}

// ============================================================
// Below are the "Communication helpers": they talk to the receiver.
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
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this; on the PS2 receiver it does no harm)
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

**You will see:** (push the left stick slowly all the way up)

```text
Ready! Press ✕ for light on, ● for light off, push the left stick to change the blink speed.
Switch every 1000 milliseconds
Switch every 626 milliseconds
Switch every 50 milliseconds
```

> [!TIP]
> **Try it:** Change `FAST_MS` to `20`. With the stick all the way up, the light blinks so fast that it looks like it is always on.

---

<h2 id="ex5">5️⃣ A servo that follows the stick (advanced)</h2>

**What it does:** Push the left stick left and right, and the SG90 servo turns with it: all the way left 0 degrees, let go 90 degrees, all the way right 180 degrees. You can use it for the gripper of a robot arm, or a robot that turns its head.

**You also need:** 1 SG90 servo.

**Servo wiring:**

| SG90 wire | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: |
| Brown | GND | GND |
| Red | 5V | 5V (VIN) |
| Orange (signal) | 9 | GPIO 13 |

<a href="/examples/ps2/arduino-en/ps2_05_servo/ps2_05_servo.ino" download data-ignore="true">📄 Download ps2_05_servo.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_05_servo/ps2_05_servo.ino -->
```cpp
/*
 * Controller PS2 x Arduino  Example 5 (advanced): A servo that follows the stick
 * ------------------------------------------------------------
 * What does this example do?
 *   Push the left stick left and right, and the SG90 servo turns with it:
 *     all the way left = 0 degrees, let go (middle) = 90 degrees, all the way right = 180 degrees.
 *   You can use it for the gripper of a robot arm, or a robot that turns its head.
 *
 * You need:
 *   Controller PS2 receiver (Universal I2C edition) + a PS2 2.4G wireless gamepad, 1 SG90 servo.
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *   SG90 brown wire -> GND    red wire -> 5V    orange wire (signal) -> 9
 *   (On ESP32, connect the signal wire to GPIO 13)
 *
 * How to see the result:
 *   Push the left stick and watch the servo turn. The Serial Monitor (115200) prints the angle.
 *
 * Good to know:
 *   1. Stick does nothing? Press the ANALOG button in the middle of the gamepad so its light turns on (analog mode).
 *   2. This example makes the servo signal by itself, without the Servo library,
 *      so it works on UNO, Nano, UNO R4 and ESP32 as it is.
 *   3. The servo keeps shaking, or the Arduino keeps restarting: USB does not give enough power.
 *      Connect the servo's red and brown wires to a battery (4~6V) instead. The battery's minus (-) must also go to Arduino GND.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

#if defined(ESP32)
const int SERVO_PIN = 13;                // ESP32: signal wire to GPIO 13
#else
const int SERVO_PIN = 9;                 // UNO / Nano / R4: signal wire to pin 9
#endif

// ---- 👉 You can change these settings ----
const int MIN_ANGLE = 0;                 // The angle when the stick is all the way left
const int MAX_ANGLE = 180;               // The angle when the stick is all the way right (a gripper only needs a little, so you can make it smaller)

int angle = 90;                          // The servo's angle now; start in the middle
unsigned long lastPulse = 0;             // The last time we sent a signal
unsigned long lastPrint = 0;             // The last time we printed the angle

void setup() {
  Serial.begin(115200);
  pinMode(SERVO_PIN, OUTPUT);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Push the left stick left and right, and the servo will follow."));
}

void loop() {
  // The servo needs a signal "every 20 milliseconds" to stay at its angle
  if (millis() - lastPulse < 20) return;
  lastPulse = millis();

  // Read the gamepad data (box 0x11, 9 numbers). readPad is at the bottom of the file and also checks for wrong readings
  uint8_t d[9];
  if (readPad(d)) {
    int lx = (int8_t)d[2];               // Left stick left/right: -127 (all the way left) ~ 0 (middle) ~ +127 (all the way right)
    angle = map(lx, -127, 127, MIN_ANGLE, MAX_ANGLE);   // Change it into an angle
  } else {
    angle = 90;                          // Cannot read the receiver -> go back to the middle, it is safer
  }
  // When the gamepad is not connected, the stick reads 0, so the servo also goes back to the middle

  servoPulse(angle);                     // Send one signal to the servo

  if (millis() - lastPrint >= 300) {     // Print the angle every 0.3 seconds
    lastPrint = millis();
    Serial.print(F("Angle: "));
    Serial.print(angle);
    Serial.println(F(" degrees"));
  }
}

// Send one pulse to the servo: how long the pin stays HIGH tells the servo which angle to go to
//   0 degrees ≈ 0.544 ms, 90 degrees ≈ 1.47 ms, 180 degrees ≈ 2.4 ms (the same as Arduino's Servo library)
void servoPulse(int deg) {
  deg = constrain(deg, 0, 180);
  int us = map(deg, 0, 180, 544, 2400);  // Angle -> microseconds (1 millisecond = 1000 microseconds)
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(us);
  digitalWrite(SERVO_PIN, LOW);
}

// ============================================================
// Read the gamepad data and do a "sanity check" (the same in every example, no need to change it)
// ============================================================
// Why a sanity check?
//   When the receiver is very busy (for example, talking wirelessly to the gamepad), once in a while
//   it is not ready in time and gives the Arduino "the box it read last time" instead
//   (for example the status data in box 0x31).
//   Then the button and stick numbers are wrong, and a remote-control car could start moving by itself.
// How can we tell?
//   On the D-pad you cannot press "up" and "down" at the same time,
//   and you cannot press "left" and "right" at the same time.
//   If we read that, this reading is wrong: throw it away and keep using the last good one.

uint8_t lastGood[9] = {0};               // The last good gamepad data (all 0 at the start = nothing pressed)

// Read box 0x11 (gamepad data, 9 numbers) into d. Returns false if the receiver cannot be read
bool readPad(uint8_t *d) {
  if (!mbcRead(0x11, d, 9)) return false;
  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];                     // The 16-bit button state
  bool upDown    = (buttons & (1u << 4)) && (buttons & (1u << 6));   // Up (bit 4) and down (bit 6) pressed together?
  bool leftRight = (buttons & (1u << 7)) && (buttons & (1u << 5));   // Left (bit 7) and right (bit 5) pressed together?
  if (upDown || leftRight) {
    memcpy(d, lastGood, 9);              // Wrong reading -> throw it away, use the last good one
  } else {
    memcpy(lastGood, d, 9);              // Good reading -> remember it, in case the next one is wrong
  }
  return true;
}

// ============================================================
// Below are the "Communication helpers": they talk to the receiver.
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
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this; on the PS2 receiver it does no harm)
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

**You will see:** (push the left stick from the middle all the way right, then all the way left)

```text
Ready! Push the left stick left and right, and the servo will follow.
Angle: 90 degrees
Angle: 134 degrees
Angle: 180 degrees
Angle: 0 degrees
```

> [!NOTE]
> This example **makes the servo signal by itself**, without the Servo library, so it works on UNO, Nano, UNO R4 and ESP32 as it is. If the servo keeps shaking, or the Arduino keeps restarting, USB does not give enough power: connect the servo's red and brown wires to a battery (4~6 V) instead. The battery's minus (-) must also go to the Arduino GND.

> [!TIP]
> **Try it:** A gripper only needs to turn a little. Change `MIN_ANGLE` to `60` and `MAX_ANGLE` to `120`.

---

<h2 id="ex6">6️⃣ Tank-style remote-control car (advanced)</h2>

**What it does:** Drive a two-wheel car with the gamepad, the same way you drive a tank: **the left stick controls the left wheel, the right stick controls the right wheel**. Push both up to go straight; push one up and one down to turn on the spot. When both sticks are let go, you can also drive with the **D-pad**. If the receiver cannot be read, or the gamepad disconnects, the car stops at once.

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
1. Lift the car up (wheels off the ground) before uploading. Push both sticks up and check that both wheels turn **forward**. If a wheel turns the wrong way, change `LEFT_REVERSE` or `RIGHT_REVERSE` to `true`.
2. Check that the gamepad's ANALOG light is on (analog mode). In digital mode the sticks do nothing, but you can still drive with the D-pad.
3. **Safety test:** With the car still lifted, switch the gamepad off or take out its batteries. Check that the wheels stop. Then put the car on the floor and drive.

<a href="/examples/ps2/arduino-en/ps2_06_tank_car/ps2_06_tank_car.ino" download data-ignore="true">📄 Download ps2_06_tank_car.ino</a>

<!-- ino: examples/ps2/arduino-en/ps2_06_tank_car/ps2_06_tank_car.ino -->
```cpp
/*
 * Controller PS2 x Arduino  Example 6 (advanced): Tank-style remote-control car
 * ------------------------------------------------------------
 * What does this example do?
 *   Drive a two-wheel car with the gamepad, the same way you drive a tank:
 *     left stick up/down  -> controls the left wheel (push up = forward, push down = backward)
 *     right stick up/down -> controls the right wheel
 *     push both up = go straight; one up and one down = turn on the spot.
 *   When both sticks are let go, you can also drive with the D-pad (↑ forward, ↓ backward, ← → turn on the spot).
 *   If the receiver cannot be read, or the gamepad disconnects, the car stops at once.
 *
 * You need:
 *   Controller PS2 receiver (Universal I2C edition) + a PS2 2.4G wireless gamepad,
 *   a two-wheel car, an L298N motor driver board (or a TB6612 or similar board wired the same way), batteries.
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   The L298N GND must be connected to the Arduino GND!
 *   (ESP32 uses different pins, see "Motor pins" below)
 *
 * Before the first run:
 *   1. Lift the car up (wheels off the ground) before uploading. Push both sticks up and check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE below to true.
 *   2. The gamepad must be in "analog mode" (the light of the ANALOG button in the middle is on) for the sticks to work.
 *      In digital mode the sticks do nothing, but you can still drive with the D-pad.
 *   3. ⚠️ Safety test: with the car still lifted, switch the gamepad off or take out its batteries.
 *      Check that the wheels stop. Then put the car on the floor and drive.
 *   4. To avoid wrong readings, the program asks "is the gamepad still connected?" only every 0.4 seconds,
 *      and reads only the gamepad data the rest of the time.
 *      Impossible data like "up and down together" or "left and right together" is thrown away, and the last good data is used.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

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
const int  MAX_SPEED  = 200;             // Speed with the stick pushed all the way: 0 ~ 255. Beginners: start slower
const int  DPAD_SPEED = 150;             // Speed when driving with the D-pad
const bool LEFT_REVERSE  = false;        // Left wheel turns the wrong way? Change to true
const bool RIGHT_REVERSE = false;        // Right wheel turns the wrong way? Change to true

// Where the D-pad buttons are in the 16-bit button number (which bit)
const uint8_t BTN_UP = 4, BTN_RIGHT = 5, BTN_DOWN = 6, BTN_LEFT = 7;

bool stopped = false;                    // Is the car stopped now because of a lost connection?
bool connected = false;                  // Is the gamepad connected? (updated every 0.4 seconds)
unsigned long lastStatus = 0;            // The last time we checked "is the gamepad connected?"
unsigned long lastPrint = 0;             // The last time we printed the speed

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}                      // The car stays stopped
  }
  Serial.println(F("Ready! Drive with the two sticks."));
}

void loop() {
  // ---- 1. Every 0.4 seconds, check if the gamepad is connected (box 0x31: status data) ----
  //   Don't read it every loop: switching between two boxes all the time makes it easier for the receiver
  //   to mix up the data, and the car could move by itself. To drive, just keep reading box 0x11.
  if (millis() - lastStatus >= 400) {
    lastStatus = millis();
    uint8_t st[9];
    connected = mbcRead(0x31, st, 9) && (st[0] & 0x02);   // Bit 1 of number 0: 1 = gamepad connected
  }
  if (!connected) {
    safeStop();                          // Cannot read the receiver, or the gamepad is not connected -> stop at once
    return;
  }

  // ---- 2. Read the gamepad (box 0x11). readPad is at the bottom of the file and also checks for wrong readings ----
  uint8_t d[9];
  if (!readPad(d)) {
    safeStop();                          // Cannot read -> stop
    return;
  }
  if (stopped) {
    stopped = false;
    Serial.println(F("Gamepad connected, you can drive!"));
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // The 16-bit button state
  int ly = (int8_t)d[3];                 // Left stick up/down: up = positive (-127 ~ +127)
  int ry = (int8_t)d[5];                 // Right stick up/down: up = positive

  // The more you push the stick, the faster the wheel turns: -127 ~ +127 becomes -MAX_SPEED ~ +MAX_SPEED
  int left  = (long)ly * MAX_SPEED / 127;
  int right = (long)ry * MAX_SPEED / 127;

  // When both sticks are let go, look at the D-pad instead
  if (ly == 0 && ry == 0) {
    if      (buttons & (1u << BTN_UP))    { left =  DPAD_SPEED; right =  DPAD_SPEED; }  // Forward
    else if (buttons & (1u << BTN_DOWN))  { left = -DPAD_SPEED; right = -DPAD_SPEED; }  // Backward
    else if (buttons & (1u << BTN_LEFT))  { left = -DPAD_SPEED; right =  DPAD_SPEED; }  // Turn left on the spot
    else if (buttons & (1u << BTN_RIGHT)) { left =  DPAD_SPEED; right = -DPAD_SPEED; }  // Turn right on the spot
  }

  drive(left, right);

  if (millis() - lastPrint >= 300) {     // Print the speed every 0.3 seconds
    lastPrint = millis();
    Serial.print(F("Left wheel "));
    Serial.print(left);
    Serial.print(F("  Right wheel "));
    Serial.println(right);
  }

  delay(20);
}

// Stop the car, and say so only once, at the moment it stops
void safeStop() {
  drive(0, 0);
  if (!stopped) {
    stopped = true;
    Serial.println(F("Cannot read the gamepad -> STOP!"));
  }
  delay(50);
}

// Control the two wheels: speed -255 (full speed backward) ~ 255 (full speed forward)
void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);   // Keep the speed inside the range
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
}

// ============================================================
// Read the gamepad data and do a "sanity check" (the same in every example, no need to change it)
// ============================================================
// Why a sanity check?
//   When the receiver is very busy (for example, talking wirelessly to the gamepad), once in a while
//   it is not ready in time and gives the Arduino "the box it read last time" instead
//   (for example the status data in box 0x31).
//   Then the button and stick numbers are wrong, and a remote-control car could start moving by itself.
// How can we tell?
//   On the D-pad you cannot press "up" and "down" at the same time,
//   and you cannot press "left" and "right" at the same time.
//   If we read that, this reading is wrong: throw it away and keep using the last good one.

uint8_t lastGood[9] = {0};               // The last good gamepad data (all 0 at the start = nothing pressed)

// Read box 0x11 (gamepad data, 9 numbers) into d. Returns false if the receiver cannot be read
bool readPad(uint8_t *d) {
  if (!mbcRead(0x11, d, 9)) return false;
  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];                     // The 16-bit button state
  bool upDown    = (buttons & (1u << 4)) && (buttons & (1u << 6));   // Up (bit 4) and down (bit 6) pressed together?
  bool leftRight = (buttons & (1u << 7)) && (buttons & (1u << 5));   // Left (bit 7) and right (bit 5) pressed together?
  if (upDown || leftRight) {
    memcpy(d, lastGood, 9);              // Wrong reading -> throw it away, use the last good one
  } else {
    memcpy(lastGood, d, 9);              // Good reading -> remember it, in case the next one is wrong
  }
  return true;
}

// ============================================================
// Below are the "Communication helpers": they talk to the receiver.
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
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this; on the PS2 receiver it does no harm)
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
Ready! Drive with the two sticks.
Left wheel 0  Right wheel 0
Left wheel 200  Right wheel 200
Left wheel -100  Right wheel 100
Cannot read the gamepad -> STOP!
Gamepad connected, you can drive!
```

> [!WARNING]
> **When the gamepad's battery is empty or it is switched off, first check that the car stops.** After the gamepad is switched off, the PS2 2.4G receiver module sometimes keeps answering, so the receiver may not notice that the connection is "lost". Then the buttons and sticks usually become 0, and the car still stops. But every gamepad and receiver module can be different, so **always do the safety test above before driving on the floor**.

> [!NOTE]
> The program asks the receiver "is the gamepad still connected?" (box `0x31`) only every **0.4 seconds**, and reads only the gamepad data (box `0x11`) the rest of the time. Impossible data like "up and down together" or "left and right together" is thrown away, and the last good data is used. This way the car is less likely to move by itself because of a wrong reading. See the [FAQ](#faq-updown) for why.

**How to adjust the car so it is easier to drive:**

| What the car does | What to change |
| :--- | :--- |
| Too fast, hard to control | Make `MAX_SPEED` smaller (for example 200 → 150) |
| The car doesn't move when you push the stick a little | The motors need more power: make `MAX_SPEED` bigger, or use new batteries |
| You push forward but it spins on the spot | One wheel turns the wrong way: change `LEFT_REVERSE` or `RIGHT_REVERSE` |
| Too fast with the D-pad | Make `DPAD_SPEED` smaller |

---

## ❓ FAQ

**Q: It keeps saying "Receiver not found"?**
1. Check whether TX and RX are swapped (UNO: RX → A4, TX → A5).
2. Check that VCC and GND are plugged in firmly. Is the light on the receiver on?
3. Make sure it is the **Universal I2C edition**: the SPIKE and EV3 editions have different firmware and cannot connect to Arduino directly.
4. Still not found: add **one 4.7 kΩ resistor from SDA to 5V and one from SCL to 5V** (pull-up resistors). This helps especially with long wires.

**Q: The Serial Monitor shows strange characters?**
The speed is not set to **115200**. Change it at the bottom right of the Serial Monitor and it will be fine.

**Q: The buttons work, but the sticks are always 0?**
The gamepad is in digital mode. Press the **ANALOG** button in the middle of the gamepad so its light turns on.

**Q: The gamepad is switched off, but example 1 still says "connected"?**
After the gamepad is switched off, the PS2 2.4G receiver module sometimes keeps answering, so the receiver may not know the gamepad is off. Then the buttons and sticks are 0, and the car stops. Please check your gamepad with the safety test in [example 6](#ex6).

<a id="faq-updown"></a>

**Q: Why does the program ignore "up and down pressed together"?**
From example 2 on, every program has a piece of code called `readPad`. When it reads "up and down pressed together" or "left and right pressed together" on the D-pad, it throws this reading away and keeps using the last one. Here is why:
1. The D-pad of a gamepad is one piece, so **you cannot press up and down at the same time** (the same for left and right).
2. When the receiver is very busy, once in a while it is not ready in time and gives "the box it read last time" instead (for example the status data in box `0x31`). Then the button and stick numbers are wrong, and a remote-control car could move by itself. This mainly happens with the PS4/PS5 Bluetooth receiver. The PS2 examples are written the same way, so the programs of the two receivers can be swapped.
3. This kind of wrong reading often looks like "up and down pressed together", so when we see it, we know the reading is wrong.

Also, examples 3 and 6 read box `0x31` (is the gamepad connected?) only every **0.4 seconds**, and read only box `0x11` the rest of the time. This also makes wrong readings less likely. We suggest you do the same in your own programs: to drive or control things, read only box `0x11`, and check the connection status less often.

**Q: Can I connect a PS2 receiver and a PS4/PS5 receiver at the same time?**
No. Both have the address `0x42`, and only one can be on the same SDA/SCL wires. But the **box numbers of the two are exactly the same**, so the examples on this page also work with the PS4/PS5 receiver (the PS4/PS5-only examples are in the [PS4/PS5 Arduino examples](/en/sensors/ps4/arduino-examples.md)).

**Q: I want to know what data is in each box.**
See the [I2C register table on the PS2 home page](/en/sensors/ps2/index.md) and the [MBC Universal I2C Protocol](/en/i2c-protocol.md) (advanced information for people who write code).
