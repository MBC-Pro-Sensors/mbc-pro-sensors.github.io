<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 SPIKE 6-Way Expander (EXP6) × Arduino Examples

[🔙 Back to EXP6 overview](/en/sensors/exp6/index.md)

This page has **6 examples**, from "is it connected?" all the way to "a two-wheel car". Arduino talks to EXP6 through a **UART serial port** (two wires: TX and RX), so one Arduino can control 6 SPIKE motors and sensors. You can copy and upload every example as it is. Every line of the code has an English comment.

> [!TIP]
> New to Arduino? Start with [🔰 Your First Arduino: From Zero](/en/sensors/arduino-start.md). In 15 minutes you will learn to install the software, upload a program and see the result.

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/exp6/arduino-en/exp6_arduino_examples_en.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 Download all examples (ZIP)</a>
</div>

> [!NOTE]
> EXP6 works out of the box. **No firmware update, no changes to the board.** EXP6 starts in "LEGO mode". When it gets the first command from Arduino, it switches to "UART mode" by itself (the same command set that Pybricks uses).

---

## 🧭 Choose Your Board

| Board | Can I use it? | Talks to EXP6 with | Notes |
| :--- | :---: | :---: | :--- |
| **Arduino UNO R4** (Minima / WiFi) | ⭐ **Recommended** | D0, D1 (`Serial1`) | Looks like an UNO, but D0 / D1 and USB are **two separate serial ports**, so uploading and the Serial Monitor still work |
| Arduino Mega 2560 | ✅ | 18, 19 (`Serial1`) | Has 4 serial ports; a bigger board |
| ESP32 dev board | ✅ | GPIO16, 17 (`Serial2`) | 3.3V signals, so pin 4 goes to **3V3**; install the ESP32 board package first |
| Arduino UNO R3, Nano | ❌ | — | Only one serial port, and USB already uses it (see below) |

> [!IMPORTANT]
> **Why can't I use an UNO R3?** Every 0.01 seconds EXP6 sends back a pack of 35 numbers. The speed is fixed at 115200 and cannot be changed. The UNO R3's only hardware serial port (D0 / D1) is connected to USB. A "software serial port" (SoftwareSerial) is not reliable at 115200, and it cannot send commands while it is receiving. So on an UNO R3, the examples on this page print "This board ... cannot connect to EXP6" and stop.

---

## 🔌 Wiring (the same for every example)

The LEGO 6-pin socket on EXP6 that connects to the controller is called the **Host port**. Connect it to Arduino with a "LEGO 6-pin to Dupont cable" (or a LEGO connector breakout board):

| EXP6 Host port | Arduino UNO R4 (recommended) | Mega 2560 | ESP32 |
| :---: | :---: | :---: | :---: |
| Pin 3 GND | GND | GND | GND |
| Pin 4 VCC | **5V** | **5V** | **3V3** |
| Pin 5 (EXP6 receives) | D1 (TX) | 18 (TX1) | GPIO17 (TX2) |
| Pin 6 (EXP6 sends) | D0 (RX) | 19 (RX1) | GPIO16 (RX2) |
| Pins 1, 2 | Not connected | Not connected | Not connected |

**Power:** the XT30 power socket on EXP6 must be connected to a **2~3S lithium battery**. The motors and sensors get their power from it.

> [!IMPORTANT]
> - **TX goes to "EXP6 receives", RX goes to "EXP6 sends"**: Arduino's TX (send) connects to EXP6's "receive" (pin 5). The two wires cross over.
> - **Pin 4 must match the board's signal voltage**: 5V for UNO R4 and Mega, 3V3 for ESP32. EXP6 has an isolation chip inside, and this pin sets its signal voltage. The wrong voltage pushes the chip beyond its rating.
> - **Pin 4 is also EXP6's "power-on key"**: EXP6 only turns on the battery power when this pin has power. With only the battery and no pin 4, EXP6 does not turn on.
> - **EXP6 does not power the Arduino**: power the Arduino separately with USB or a power bank.
> - Unplug the USB cable and the battery before wiring.

---

## 🐶 Must Know: the Watchdog

EXP6 has a "watchdog": **if it gets no command from Arduino for more than 0.5 seconds, it stops the motors by itself** (except a motor that is still turning to an angle; it finishes first, then stops). This way, if the Arduino freezes or a wire comes loose, the car does not keep running away.

That is why the examples always wait with **`exp6Wait(ms)`**, not `delay()`. While `exp6Wait()` waits, it sends an "I'm still here" pack to EXP6 every 0.1 seconds. `exp6Receive()` (get data) also sends one.

| Code | Result |
| :--- | :--- |
| `motorRun(1, 50); exp6Wait(3000);` | ✅ The motor runs for 3 seconds |
| `motorRun(1, 50); delay(3000);` | ❌ The watchdog stops the motor after 0.5 seconds |

---

## 📋 Examples at a Glance

| # | Example | What you learn | Also need |
| :---: | :--- | :--- | :--- |
| 1 | [Say hello to EXP6](#ex1) | Check the wiring, see what is on the 6 ports | Any few motors or sensors |
| 2 | [Make the motor run](#ex2) | Constant speed, brake, coast, watchdog | 1 SPIKE motor (port 1) |
| 3 | [Turn to an angle](#ex3) | Turn 90 degrees, one full turn, turn to the 0 degree mark | Same as above |
| 4 | [Read the motor angle](#ex4) | Use the motor as an angle sensor, reset the angle | Same as above |
| 5 | [Read the sensors](#ex5) | Color, distance, force | SPIKE color / ultrasonic / force sensor |
| 6 | [Two-wheel car (advanced)](#ex6) | Drive two motors in sync, forward and turning | 2 SPIKE motors, LEGO car body, power bank |

> [!TIP]
> **Suggested order: 1 → 2 → 4 → 3 → 5 → 6.** First check the wiring (1), then make the motor move (2), read the angle (4), and build the car last (6).

---

<h2 id="ex1">1️⃣ Say Hello to EXP6</h2>

**What it does:** checks that Arduino and EXP6 are connected. Once every second it prints the battery voltage and what is plugged into each of the 6 ports. Plug and unplug a motor, and the list changes.

**Steps:**
1. Connect the 4 wires as in the table above, connect the EXP6 battery, and plug the USB cable into the computer.
2. Copy the code below, paste it into the Arduino IDE, **select your board** (for example Arduino UNO R4 Minima), and click **→ Upload**.
3. Open the Serial Monitor (the magnifying glass in the top right corner) and choose **115200** baud.

<a href="/examples/exp6/arduino-en/exp6_01_hello/exp6_01_hello.ino" download data-ignore="true">📄 Download exp6_01_hello.ino</a>

<!-- ino: examples/exp6/arduino-en/exp6_01_hello/exp6_01_hello.ino -->
```cpp
/*
 * SPIKE 6-Way Expander EXP6 x Arduino   Example 1: Say hello to EXP6
 * ------------------------------------------------------------
 * What does this example do?
 *   It checks that Arduino and EXP6 are connected well. If they are, once every second it prints:
 *   the battery voltage, and what is plugged into each of the 6 ports (motor, color, ultrasonic, force sensor).
 *   Try unplugging and plugging in a motor, and watch the list change.
 *   Every time you change the wiring, run this example first.
 *
 * You need:
 *   Arduino UNO R4 (Minima or WiFi), EXP6, a 2~3S lithium battery,
 *   a LEGO 6-pin to Dupont cable, and a few SPIKE motors or sensors.
 *
 * Wiring (UNO R4; for Mega and ESP32, see the wiring table on the web page):
 *   EXP6 Host port pin 3 (GND)             -> GND
 *   EXP6 Host port pin 4 (VCC)             -> 5V (EXP6 only turns on when this wire has power)
 *   EXP6 Host port pin 5 (EXP6 receives)   -> D1 (TX)
 *   EXP6 Host port pin 6 (EXP6 sends)      -> D0 (RX)
 *   EXP6 XT30 power socket                 -> 2~3S lithium battery
 *
 * How to see the result:
 *   After uploading, open the "Serial Monitor" and choose 115200 baud.
 *
 * Tip:
 *   UNO R3 and Nano cannot be used: they have only one serial port, and USB already uses it.
 */

void setup() {
  Serial.begin(115200);                  // Open the channel to talk to the computer, speed 115200
  while (!Serial && millis() < 3000) {}  // UNO R4 waits for the computer to open the serial port, up to 3 seconds
  Serial.println(F("=== EXP6: Say hello to EXP6 ==="));

  // Wake up EXP6 and wait for its data, up to 3 seconds (the helpers are at the bottom of this file)
  if (!exp6Begin(3000)) {
    Serial.println(F("EXP6 not found! Please check:"));
    Serial.println(F("  1. Pin 5 goes to TX, pin 6 goes to RX (D1, D0 on UNO R4). Are they swapped?"));
    Serial.println(F("  2. Pin 4 goes to 5V (3V3 on ESP32), pin 3 goes to GND. Are they pushed in firmly?"));
    Serial.println(F("  3. Is the EXP6 battery connected? Is the battery light on?"));
    Serial.println(F("  4. Is your board an UNO R4, Mega or ESP32? (UNO R3 cannot be used)"));
    while (true) {}                      // Not found: stop here
  }
  Serial.println(F("Connected! Updates once every second. Try plugging and unplugging a motor."));
}

void loop() {
  if (!exp6Receive()) {                  // Get the newest data pack
    Serial.println(F("No data. Check if a wire is loose."));
    exp6Wait(500);
    return;
  }

  uint16_t v = exp6Volt100();            // Battery voltage x 100, for example 812 = 8.12 V
  Serial.print(F("Battery "));
  Serial.print(v / 100);                 // Whole number part
  Serial.print('.');
  if (v % 100 < 10) Serial.print('0');   // If the decimal part has only 1 digit, add a 0 in front
  Serial.print(v % 100);
  Serial.print(F(" V |"));

  for (uint8_t port = 1; port <= 6; port++) {   // Port 1 to port 6, one by one
    Serial.print(' ');
    Serial.print(port);
    Serial.print(':');
    printDeviceName(exp6Device(port));
  }
  Serial.println();

  exp6Wait(1000);                        // Wait 1 second (while waiting, the helpers keep telling EXP6 "I'm still here")
}

// Print the device code as a word
void printDeviceName(uint8_t kind) {
  switch (kind) {
    case 0:  Serial.print(F("none"));   break;
    case 1:  Serial.print(F("force"));   break;
    case 3:  Serial.print(F("color"));   break;
    case 4:  Serial.print(F("ultrasonic")); break;
    case 5:  Serial.print(F("motor"));   break;
    default: Serial.print(F("other"));   break;
  }
}

// ============================================================
// Below are the "Communication helpers": they talk to EXP6 (UART serial port).
// They are the same in every EXP6 example. No need to change or understand them.
// You only need these:
//   exp6Begin(ms)            Wake up EXP6 and wait for its data. Returns true if it works
//   exp6Send(L1, L2, L3, L4) Send one command (4 numbers, each 0 ~ 100)
//   exp6Receive()            Get the newest data pack (6 ports + battery). Returns true if received
//   exp6Port(port)           The number for that port (motor = angle, ultrasonic = distance ...)
//   exp6Device(port)         What is on that port: 0 none, 1 force, 3 color, 4 ultrasonic, 5 motor, 7 other
//   exp6Volt100()            Battery voltage x 100 (for example 812 = 8.12 V)
//   exp6Wait(ms)             Use instead of delay(): while waiting, it keeps telling EXP6 "I'm still here"
//   exp6WaitDevice(port, kind, ms)  Wait until a port shows the device you want. Returns true if found
// ============================================================

// Which serial port talks to EXP6 (picked for your board, no need to change)
#if defined(ARDUINO_ARCH_ESP32)
#define EXP6_SERIAL Serial2              // ESP32: RX2 = GPIO16, TX2 = GPIO17
#define EXP6_RX_PIN 16
#define EXP6_TX_PIN 17
#elif defined(ARDUINO_ARCH_RENESAS) || defined(HAVE_HWSERIAL1) || defined(ARDUINO_ARCH_MEGAAVR)
#define EXP6_SERIAL Serial1              // UNO R4: D0(RX) / D1(TX); Mega: 19(RX1) / 18(TX1)
#endif

uint8_t exp6Data[32];                    // Newest data pack: 8 numbers, 4 bytes each
unsigned long exp6LastSend = 0;          // Time of the last command we sent

// Wake up EXP6: keep sending "I'm still here" until EXP6 sends data back, wait up to timeoutMs
bool exp6Begin(uint16_t timeoutMs) {
#ifdef EXP6_SERIAL
#if defined(ARDUINO_ARCH_ESP32)
  EXP6_SERIAL.begin(115200, SERIAL_8N1, EXP6_RX_PIN, EXP6_TX_PIN);
#else
  EXP6_SERIAL.begin(115200);             // EXP6 always talks at 115200
#endif
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    exp6Send(1, 0, 0, 0);                // After the first correct command, EXP6 switches from "LEGO mode" to "UART mode"
    if (exp6Receive()) {
      exp6Send(0, 1, 0, 0);              // Send a "do nothing" command first, so the next one is not skipped as a repeat
      exp6Send(0, 0, 0, 0);              // Reset everything: motors stop, angles go back to 0
      exp6Wait(300);
      return exp6Receive();
    }
  }
#else
  (void)timeoutMs;                       // This board does not use this number
  Serial.println(F("This board has no second hardware serial port (for example UNO R3, Nano). It cannot connect to EXP6."));
  Serial.println(F("Please use UNO R4, Mega 2560 or ESP32."));
#endif
  return false;
}

// Send one command: [0xA5, L1, L2, L3, L4, checksum]
void exp6Send(uint8_t L1, uint8_t L2, uint8_t L3, uint8_t L4) {
  uint8_t pkt[6] = { 0xA5, L1, L2, L3, L4, 0 };
  pkt[5] = pkt[0] ^ L1 ^ L2 ^ L3 ^ L4;   // Checksum: XOR of the first 5 numbers
#ifdef EXP6_SERIAL
  EXP6_SERIAL.write(pkt, 6);
#endif
  exp6LastSend = millis();
}

// If nothing was sent for 0.1 s, send "I'm still here" (L1 = 1). It does not move any motor
void exp6KeepAlive() {
  if (millis() - exp6LastSend >= 100) exp6Send(1, 0, 0, 0);
}

// Use instead of delay(): if EXP6 gets no command for 0.5 s, it stops the motors by itself (watchdog)
void exp6Wait(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    exp6KeepAlive();
    delay(1);
  }
}

// Get the newest data pack: [0x5A, 32, 32 numbers, checksum], 35 bytes in total. EXP6 sends one every 0.01 s
bool exp6Receive() {
#ifdef EXP6_SERIAL
  exp6KeepAlive();
  while (EXP6_SERIAL.available()) EXP6_SERIAL.read();   // Throw away old data, keep only the newest
  static uint8_t buf[32];
  uint8_t n = 0, x = 0;
  unsigned long start = millis();
  while (millis() - start < 100) {
    if (!EXP6_SERIAL.available()) continue;
    uint8_t b = EXP6_SERIAL.read();
    if (n == 0) {                        // Look for the start mark 0x5A
      if (b == 0x5A) n = 1;
    } else if (n == 1) {                 // The 2nd byte must be 32 (32 numbers follow)
      if (b == 32) { x = b; n = 2; } else n = (b == 0x5A) ? 1 : 0;
    } else if (n < 34) {                 // The 32 numbers
      buf[n - 2] = b;
      x ^= b;
      n++;
    } else {                             // The last byte is the checksum. Keep the data only if it matches
      if (b == x) {
        memcpy(exp6Data, buf, 32);
        return true;
      }
      n = 0;
    }
  }
#endif
  return false;
}

// The number for port "port" (4 bytes make one number, low byte first)
int32_t exp6Port(uint8_t port) {
  if (port < 1 || port > 6) return 0;
  const uint8_t *d = &exp6Data[(port - 1) * 4];
  return (int32_t)((uint32_t)d[0] | ((uint32_t)d[1] << 8) | ((uint32_t)d[2] << 16) | ((uint32_t)d[3] << 24));
}

// What is on port "port": each port uses 3 bits
uint8_t exp6Device(uint8_t port) {
  uint16_t r7 = exp6Data[24] | ((uint16_t)exp6Data[25] << 8);   // 7th number: voltage + port 1
  uint16_t r8 = exp6Data[28] | ((uint16_t)exp6Data[29] << 8);   // 8th number: ports 2 ~ 6
  if (port == 1) return (r7 >> 12) & 0x07;
  if (port >= 2 && port <= 6) return (r8 >> ((port - 2) * 3)) & 0x07;
  return 0;
}

// Battery voltage x 100
uint16_t exp6Volt100() {
  return (exp6Data[24] | ((uint16_t)exp6Data[25] << 8)) & 0x0FFF;
}

// Wait until port "port" shows device "kind" (for example 5 = motor), wait up to timeoutMs
bool exp6WaitDevice(uint8_t port, uint8_t kind, uint16_t timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (exp6Receive() && exp6Device(port) == kind) {
      exp6Wait(200);                     // Just found it. Give EXP6 0.2 s more to get ready
      return true;
    }
  }
  return false;
}
```

**You will see:** (motor on port 1, color sensor on port 3, ultrasonic sensor on port 5)

```text
=== EXP6: Say hello to EXP6 ===
Connected! Updates once every second. Try plugging and unplugging a motor.
Battery 8.12 V | 1:motor 2:none 3:color 4:none 5:ultrasonic 6:none
Battery 8.11 V | 1:motor 2:none 3:color 4:none 5:ultrasonic 6:none
```

> [!NOTE]
> **What are the "Communication helpers"?** At the bottom of every example there is the same piece of code. It "talks" to EXP6. You do not need to change it or understand it. Just remember:
> - `exp6Send(L1, L2, L3, L4)`: send one command, 4 numbers, each 0~100
> - `exp6Receive()`: get the newest data pack; then use `exp6Port(port)` to read that port's number, and `exp6Device(port)` to see what is on that port
> - `exp6Wait(ms)`: use it instead of `delay()`
>
> Every 0.01 seconds EXP6 sends a data pack by itself. One pack holds the numbers for all 6 ports and the battery voltage, so one pack gives Arduino everything.

> [!TIP]
> **A device you just plugged in takes 1~2 seconds to be found.** The list shows "none" at first, then changes after a moment.

---

<h2 id="ex2">2️⃣ Make the Motor Run</h2>

**What it does:** the motor on port 1 does "forward 3 s → brake → backward 3 s → coast to a stop", again and again. EXP6 adjusts the power by itself to keep the speed you set.

**Get ready:** 1 SPIKE motor plugged into **port 1** of EXP6.

<a href="/examples/exp6/arduino-en/exp6_02_motor_run/exp6_02_motor_run.ino" download data-ignore="true">📄 Download exp6_02_motor_run.ino</a>

<!-- ino: examples/exp6/arduino-en/exp6_02_motor_run/exp6_02_motor_run.ino -->
```cpp
/*
 * SPIKE 6-Way Expander EXP6 x Arduino   Example 2: Make the motor run
 * ------------------------------------------------------------
 * What does this example do?
 *   The motor on port 1 does "forward 3 s -> brake -> backward 3 s -> coast to a stop", again and again.
 *   While it runs, it prints the motor angle every 0.5 seconds.
 *   EXP6 adjusts the power by itself to keep the speed you set (even with a little load).
 *
 * You need:
 *   The wiring from Example 1, plus 1 SPIKE motor plugged into port 1 of EXP6.
 *
 * Wiring (UNO R4; for Mega and ESP32, see the wiring table on the web page):
 *   EXP6 Host port pin 3 (GND)             -> GND
 *   EXP6 Host port pin 4 (VCC)             -> 5V
 *   EXP6 Host port pin 5 (EXP6 receives)   -> D1 (TX)
 *   EXP6 Host port pin 6 (EXP6 sends)      -> D0 (RX)
 *   EXP6 XT30 power socket                 -> 2~3S lithium battery
 *
 * How to see the result:
 *   After uploading, open the "Serial Monitor", choose 115200 baud, and watch the motor turn.
 *
 * Tip:
 *   EXP6 has a "watchdog": if it gets no command from Arduino for more than 0.5 seconds, it stops the motors.
 *   So when you wait, use exp6Wait(), not delay().
 */

const uint8_t MOTOR_PORT = 1;            // 👉 You can change this: which EXP6 port the motor is on (1 ~ 6)
const int8_t  SPEED = 50;                // 👉 You can change this: speed 1 ~ 100 (100 = this motor's top speed)

// Ways to stop
const uint8_t STOP_COAST = 3;            // Coast: let go of the motor, it slows down by itself (you can turn it by hand)
const uint8_t STOP_HOLD  = 4;            // Hold: grip the current angle hard (you cannot turn it by hand)
const uint8_t STOP_BRAKE = 5;            // Brake: stop right away, but you can still turn it by hand

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 waits for the computer to open the serial port
  Serial.println(F("=== EXP6: Make the motor run ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("EXP6 not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  // Wait until port MOTOR_PORT sees a motor (kind 5), up to 3 seconds
  if (!exp6WaitDevice(MOTOR_PORT, 5, 3000)) {
    Serial.print(F("No motor on port "));
    Serial.print(MOTOR_PORT);
    Serial.println(F(". Plug the motor in, then press the Arduino reset button."));
    while (true) {}
  }
  Serial.println(F("Motor found. Here we go!"));
}

void loop() {
  Serial.println(F("Forward for 3 seconds..."));
  motorRun(MOTOR_PORT, SPEED);           // Positive = clockwise
  if (!runFor(3000)) return;             // Run 3 seconds and watch the motor; if something goes wrong, start over

  Serial.println(F("Brake!"));
  motorStop(MOTOR_PORT, STOP_BRAKE);
  exp6Wait(1000);                        // Stop for 1 second

  Serial.println(F("Backward for 3 seconds..."));
  motorRun(MOTOR_PORT, -SPEED);          // Negative = counterclockwise
  if (!runFor(3000)) return;

  Serial.println(F("Coast to a stop."));
  motorStop(MOTOR_PORT, STOP_COAST);
  exp6Wait(1000);
}

// Run for ms milliseconds, print the angle every 0.5 s. If there is no data or the motor is unplugged, stop and return false
bool runFor(unsigned long ms) {
  unsigned long start = millis();
  unsigned long lastPrint = 0;
  while (millis() - start < ms) {
    if (!exp6Receive() || exp6Device(MOTOR_PORT) != 5) {
      motorStop(MOTOR_PORT, STOP_COAST); // Safety first: stop the motor
      Serial.println(F("No data from the motor. Stopping! Check the motor and the battery."));
      exp6Wait(1000);
      return false;
    }
    if (millis() - lastPrint >= 500) {
      lastPrint = millis();
      Serial.print(F("  Angle "));
      Serial.print(exp6Port(MOTOR_PORT)); // For a motor port, the number is the angle (degrees)
      Serial.println(F(" deg"));
    }
  }
  return true;
}

// ---------- Motor command cheat sheet ----------
// Constant speed: speed = -100 ~ 100. Positive clockwise, negative counterclockwise, 0 = stop
void motorRun(uint8_t port, int8_t speed) {
  uint8_t dir = (speed >= 0) ? 1 : 2;    // L2: 1 = clockwise, 2 = counterclockwise
  int16_t s = (speed >= 0) ? speed : -(int16_t)speed;
  if (s > 100) s = 100;
  exp6Send(port * 11, dir, s, 0);        // L1 = port x 11 (11, 22 ... 66 = constant speed)
}

// Stop: how = STOP_COAST, STOP_HOLD or STOP_BRAKE
void motorStop(uint8_t port, uint8_t how) {
  exp6Send(port * 10, how, 0, 0);        // L1 = port x 10, L2 = how to stop
}

// ============================================================
// Below are the "Communication helpers": they talk to EXP6 (UART serial port).
// They are the same in every EXP6 example. No need to change or understand them.
// You only need these:
//   exp6Begin(ms)            Wake up EXP6 and wait for its data. Returns true if it works
//   exp6Send(L1, L2, L3, L4) Send one command (4 numbers, each 0 ~ 100)
//   exp6Receive()            Get the newest data pack (6 ports + battery). Returns true if received
//   exp6Port(port)           The number for that port (motor = angle, ultrasonic = distance ...)
//   exp6Device(port)         What is on that port: 0 none, 1 force, 3 color, 4 ultrasonic, 5 motor, 7 other
//   exp6Volt100()            Battery voltage x 100 (for example 812 = 8.12 V)
//   exp6Wait(ms)             Use instead of delay(): while waiting, it keeps telling EXP6 "I'm still here"
//   exp6WaitDevice(port, kind, ms)  Wait until a port shows the device you want. Returns true if found
// ============================================================

// Which serial port talks to EXP6 (picked for your board, no need to change)
#if defined(ARDUINO_ARCH_ESP32)
#define EXP6_SERIAL Serial2              // ESP32: RX2 = GPIO16, TX2 = GPIO17
#define EXP6_RX_PIN 16
#define EXP6_TX_PIN 17
#elif defined(ARDUINO_ARCH_RENESAS) || defined(HAVE_HWSERIAL1) || defined(ARDUINO_ARCH_MEGAAVR)
#define EXP6_SERIAL Serial1              // UNO R4: D0(RX) / D1(TX); Mega: 19(RX1) / 18(TX1)
#endif

uint8_t exp6Data[32];                    // Newest data pack: 8 numbers, 4 bytes each
unsigned long exp6LastSend = 0;          // Time of the last command we sent

// Wake up EXP6: keep sending "I'm still here" until EXP6 sends data back, wait up to timeoutMs
bool exp6Begin(uint16_t timeoutMs) {
#ifdef EXP6_SERIAL
#if defined(ARDUINO_ARCH_ESP32)
  EXP6_SERIAL.begin(115200, SERIAL_8N1, EXP6_RX_PIN, EXP6_TX_PIN);
#else
  EXP6_SERIAL.begin(115200);             // EXP6 always talks at 115200
#endif
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    exp6Send(1, 0, 0, 0);                // After the first correct command, EXP6 switches from "LEGO mode" to "UART mode"
    if (exp6Receive()) {
      exp6Send(0, 1, 0, 0);              // Send a "do nothing" command first, so the next one is not skipped as a repeat
      exp6Send(0, 0, 0, 0);              // Reset everything: motors stop, angles go back to 0
      exp6Wait(300);
      return exp6Receive();
    }
  }
#else
  (void)timeoutMs;                       // This board does not use this number
  Serial.println(F("This board has no second hardware serial port (for example UNO R3, Nano). It cannot connect to EXP6."));
  Serial.println(F("Please use UNO R4, Mega 2560 or ESP32."));
#endif
  return false;
}

// Send one command: [0xA5, L1, L2, L3, L4, checksum]
void exp6Send(uint8_t L1, uint8_t L2, uint8_t L3, uint8_t L4) {
  uint8_t pkt[6] = { 0xA5, L1, L2, L3, L4, 0 };
  pkt[5] = pkt[0] ^ L1 ^ L2 ^ L3 ^ L4;   // Checksum: XOR of the first 5 numbers
#ifdef EXP6_SERIAL
  EXP6_SERIAL.write(pkt, 6);
#endif
  exp6LastSend = millis();
}

// If nothing was sent for 0.1 s, send "I'm still here" (L1 = 1). It does not move any motor
void exp6KeepAlive() {
  if (millis() - exp6LastSend >= 100) exp6Send(1, 0, 0, 0);
}

// Use instead of delay(): if EXP6 gets no command for 0.5 s, it stops the motors by itself (watchdog)
void exp6Wait(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    exp6KeepAlive();
    delay(1);
  }
}

// Get the newest data pack: [0x5A, 32, 32 numbers, checksum], 35 bytes in total. EXP6 sends one every 0.01 s
bool exp6Receive() {
#ifdef EXP6_SERIAL
  exp6KeepAlive();
  while (EXP6_SERIAL.available()) EXP6_SERIAL.read();   // Throw away old data, keep only the newest
  static uint8_t buf[32];
  uint8_t n = 0, x = 0;
  unsigned long start = millis();
  while (millis() - start < 100) {
    if (!EXP6_SERIAL.available()) continue;
    uint8_t b = EXP6_SERIAL.read();
    if (n == 0) {                        // Look for the start mark 0x5A
      if (b == 0x5A) n = 1;
    } else if (n == 1) {                 // The 2nd byte must be 32 (32 numbers follow)
      if (b == 32) { x = b; n = 2; } else n = (b == 0x5A) ? 1 : 0;
    } else if (n < 34) {                 // The 32 numbers
      buf[n - 2] = b;
      x ^= b;
      n++;
    } else {                             // The last byte is the checksum. Keep the data only if it matches
      if (b == x) {
        memcpy(exp6Data, buf, 32);
        return true;
      }
      n = 0;
    }
  }
#endif
  return false;
}

// The number for port "port" (4 bytes make one number, low byte first)
int32_t exp6Port(uint8_t port) {
  if (port < 1 || port > 6) return 0;
  const uint8_t *d = &exp6Data[(port - 1) * 4];
  return (int32_t)((uint32_t)d[0] | ((uint32_t)d[1] << 8) | ((uint32_t)d[2] << 16) | ((uint32_t)d[3] << 24));
}

// What is on port "port": each port uses 3 bits
uint8_t exp6Device(uint8_t port) {
  uint16_t r7 = exp6Data[24] | ((uint16_t)exp6Data[25] << 8);   // 7th number: voltage + port 1
  uint16_t r8 = exp6Data[28] | ((uint16_t)exp6Data[29] << 8);   // 8th number: ports 2 ~ 6
  if (port == 1) return (r7 >> 12) & 0x07;
  if (port >= 2 && port <= 6) return (r8 >> ((port - 2) * 3)) & 0x07;
  return 0;
}

// Battery voltage x 100
uint16_t exp6Volt100() {
  return (exp6Data[24] | ((uint16_t)exp6Data[25] << 8)) & 0x0FFF;
}

// Wait until port "port" shows device "kind" (for example 5 = motor), wait up to timeoutMs
bool exp6WaitDevice(uint8_t port, uint8_t kind, uint16_t timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (exp6Receive() && exp6Device(port) == kind) {
      exp6Wait(200);                     // Just found it. Give EXP6 0.2 s more to get ready
      return true;
    }
  }
  return false;
}
```

**You will see:**

```text
=== EXP6: Make the motor run ===
Motor found. Here we go!
Forward for 3 seconds...
  Angle 0 deg
  Angle 208 deg
  Angle 455 deg
  …
Brake!
Backward for 3 seconds...
  Angle 1290 deg
  Angle 1080 deg
  …
Coast to a stop.
```

**Three ways to stop:**

| Stop | Code | How it feels |
| :--- | :--- | :--- |
| Coast | `STOP_COAST` | Lets go of the motor, it slows down by itself; you can turn it by hand |
| Brake | `STOP_BRAKE` | Stops right away; you can still turn it by hand |
| Hold | `STOP_HOLD` | Grips the current angle hard; you cannot turn it by hand |

> [!TIP]
> **Try this:**
> 1. Change `SPEED` to `100` or `20` and see how much the speed changes.
> 2. While it turns, gently pinch the shaft with your fingers: EXP6 gives more power to keep the same speed.
> 3. Change the `runFor(3000)` line to `delay(3000);` and watch the watchdog stop the motor after 0.5 seconds. Remember to change it back!

---

<h2 id="ex3">3️⃣ Turn to an Angle</h2>

**What it does:** the motor turns 90 degrees → turns back → turns one full circle → turns to the "0 degree mark" on the motor. After each step it prints the current angle.

**Get ready:** 1 SPIKE motor on **port 1**, with a beam on the shaft so it is easier to see how far it turns.

<a href="/examples/exp6/arduino-en/exp6_03_motor_degrees/exp6_03_motor_degrees.ino" download data-ignore="true">📄 Download exp6_03_motor_degrees.ino</a>

<!-- ino: examples/exp6/arduino-en/exp6_03_motor_degrees/exp6_03_motor_degrees.ino -->
```cpp
/*
 * SPIKE 6-Way Expander EXP6 x Arduino   Example 3: Turn to an angle
 * ------------------------------------------------------------
 * What does this example do?
 *   The motor on port 1 turns 90 degrees -> turns back -> turns one full circle -> turns to the "0 degree mark" on the motor.
 *   After each step it prints the current angle, then rests 2 seconds and starts again.
 *   EXP6 does the angle control by itself. Arduino only needs to say "turn this many degrees".
 *
 * You need:
 *   The wiring from Example 1, plus 1 SPIKE motor plugged into port 1 of EXP6.
 *   Put a beam or a wheel on the motor shaft, so it is easier to see how far it turns.
 *
 * Wiring (UNO R4; for Mega and ESP32, see the wiring table on the web page):
 *   EXP6 Host port pin 3 (GND)             -> GND
 *   EXP6 Host port pin 4 (VCC)             -> 5V
 *   EXP6 Host port pin 5 (EXP6 receives)   -> D1 (TX)
 *   EXP6 Host port pin 6 (EXP6 sends)      -> D0 (RX)
 *   EXP6 XT30 power socket                 -> 2~3S lithium battery
 *
 * How to see the result:
 *   After uploading, open the "Serial Monitor" and choose 115200 baud.
 *
 * Tip:
 *   Send a "turn to angle" command only once. EXP6 turns to the spot by itself. Do not keep sending it.
 *   One command can turn at most 2499 degrees (about 7 turns).
 */

const uint8_t MOTOR_PORT = 1;            // 👉 You can change this: which EXP6 port the motor is on (1 ~ 6)
const uint8_t SPEED = 40;                // 👉 You can change this: speed 1 ~ 100

// How to stop after reaching the angle
const uint8_t AFTER_COAST = 0;           // Coast (let go)
const uint8_t AFTER_BRAKE = 1;           // Brake
const uint8_t AFTER_HOLD  = 2;           // Hold (you cannot turn it by hand)

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 waits for the computer to open the serial port
  Serial.println(F("=== EXP6: Turn to an angle ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("EXP6 not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  if (!exp6WaitDevice(MOTOR_PORT, 5, 3000)) {
    Serial.println(F("Motor not found. Plug the motor in, then press the Arduino reset button."));
    while (true) {}
  }
  Serial.println(F("Motor found!"));
}

void loop() {
  if (!turnDegrees(90, F("Turn 90 degrees (clockwise)"))) return;
  exp6Wait(1000);
  if (!turnDegrees(-90, F("Turn back -90 degrees (counterclockwise)"))) return;
  exp6Wait(1000);
  if (!turnDegrees(360, F("Turn one full circle, 360 degrees"))) return;
  exp6Wait(1000);

  Serial.println(F("Turn to the 0 degree mark on the motor (takes the shortest way)"));
  motorGoTo(MOTOR_PORT, SPEED, 0, AFTER_HOLD);
  exp6Wait(1500);                        // Wait for it to get there
  Serial.println(F("  Done! Check if the mark on the motor lines up."));

  Serial.println(F("Rest for 2 seconds..."));
  exp6Wait(2000);
}

// Turn deg degrees, wait until it gets there, and print the result. If it cannot get there (for example it is stuck), stop and return false
bool turnDegrees(int deg, const __FlashStringHelper *title) {
  Serial.println(title);
  if (!exp6Receive()) {                  // First read the current angle
    Serial.println(F("No data. Check if a wire is loose."));
    exp6Wait(1000);
    return false;
  }
  int32_t target = exp6Port(MOTOR_PORT) + deg;   // Target = current angle + angle to turn
  motorDegrees(MOTOR_PORT, SPEED, deg, AFTER_HOLD);

  unsigned long start = millis();
  while (millis() - start < 5000) {      // Wait up to 5 seconds
    if (exp6Receive()) {
      int32_t angle = exp6Port(MOTOR_PORT);
      if (angle > target - 10 && angle < target + 10) {   // Less than 10 degrees away counts as "there"
        exp6Wait(300);                   // Wait for it to settle
        exp6Receive();
        Serial.print(F("  Done! Angle now "));
        Serial.print(exp6Port(MOTOR_PORT));
        Serial.print(F(" deg (target "));
        Serial.print(target);
        Serial.println(F(" deg)"));
        return true;
      }
    }
  }
  motorStopNow(MOTOR_PORT);              // Safety first: if it cannot get there, let go of the motor
  Serial.println(F("  Not there after 5 seconds. Is it stuck? Stopping for now."));
  exp6Wait(2000);
  return false;
}

// ---------- Motor command cheat sheet ----------
// Turn a relative angle: deg positive = clockwise, negative = counterclockwise, at most +-2499 degrees; after = how to stop when there
void motorDegrees(uint8_t port, uint8_t speed, int deg, uint8_t after) {
  uint8_t way = (deg >= 0) ? 8 : 9;      // Last digit 8 = clockwise, 9 = counterclockwise
  uint16_t d = (deg >= 0) ? deg : -deg;
  if (d > 2499) d = 2499;
  // L1 = port x 10 + 8 or 9; L3 = stop mode x 25 + hundreds digit of the angle; L4 = last two digits of the angle
  exp6Send(port * 10 + way, speed, after * 25 + d / 100, d % 100);
}

// Turn to an absolute angle 0 ~ 359 on the motor (0 = the mark on the motor). EXP6 picks the shortest way by itself
void motorGoTo(uint8_t port, uint8_t speed, uint16_t angle, uint8_t after) {
  angle = angle % 360;
  // L1 = port x 10 + 7; L3 = stop mode x 4 + hundreds digit of the angle; L4 = last two digits of the angle
  exp6Send(port * 10 + 7, speed, after * 4 + angle / 100, angle % 100);
}

// Let go of the motor right away (coast)
void motorStopNow(uint8_t port) {
  exp6Send(port * 10, 3, 0, 0);          // L2 = 3: coast
}

// ============================================================
// Below are the "Communication helpers": they talk to EXP6 (UART serial port).
// They are the same in every EXP6 example. No need to change or understand them.
// You only need these:
//   exp6Begin(ms)            Wake up EXP6 and wait for its data. Returns true if it works
//   exp6Send(L1, L2, L3, L4) Send one command (4 numbers, each 0 ~ 100)
//   exp6Receive()            Get the newest data pack (6 ports + battery). Returns true if received
//   exp6Port(port)           The number for that port (motor = angle, ultrasonic = distance ...)
//   exp6Device(port)         What is on that port: 0 none, 1 force, 3 color, 4 ultrasonic, 5 motor, 7 other
//   exp6Volt100()            Battery voltage x 100 (for example 812 = 8.12 V)
//   exp6Wait(ms)             Use instead of delay(): while waiting, it keeps telling EXP6 "I'm still here"
//   exp6WaitDevice(port, kind, ms)  Wait until a port shows the device you want. Returns true if found
// ============================================================

// Which serial port talks to EXP6 (picked for your board, no need to change)
#if defined(ARDUINO_ARCH_ESP32)
#define EXP6_SERIAL Serial2              // ESP32: RX2 = GPIO16, TX2 = GPIO17
#define EXP6_RX_PIN 16
#define EXP6_TX_PIN 17
#elif defined(ARDUINO_ARCH_RENESAS) || defined(HAVE_HWSERIAL1) || defined(ARDUINO_ARCH_MEGAAVR)
#define EXP6_SERIAL Serial1              // UNO R4: D0(RX) / D1(TX); Mega: 19(RX1) / 18(TX1)
#endif

uint8_t exp6Data[32];                    // Newest data pack: 8 numbers, 4 bytes each
unsigned long exp6LastSend = 0;          // Time of the last command we sent

// Wake up EXP6: keep sending "I'm still here" until EXP6 sends data back, wait up to timeoutMs
bool exp6Begin(uint16_t timeoutMs) {
#ifdef EXP6_SERIAL
#if defined(ARDUINO_ARCH_ESP32)
  EXP6_SERIAL.begin(115200, SERIAL_8N1, EXP6_RX_PIN, EXP6_TX_PIN);
#else
  EXP6_SERIAL.begin(115200);             // EXP6 always talks at 115200
#endif
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    exp6Send(1, 0, 0, 0);                // After the first correct command, EXP6 switches from "LEGO mode" to "UART mode"
    if (exp6Receive()) {
      exp6Send(0, 1, 0, 0);              // Send a "do nothing" command first, so the next one is not skipped as a repeat
      exp6Send(0, 0, 0, 0);              // Reset everything: motors stop, angles go back to 0
      exp6Wait(300);
      return exp6Receive();
    }
  }
#else
  (void)timeoutMs;                       // This board does not use this number
  Serial.println(F("This board has no second hardware serial port (for example UNO R3, Nano). It cannot connect to EXP6."));
  Serial.println(F("Please use UNO R4, Mega 2560 or ESP32."));
#endif
  return false;
}

// Send one command: [0xA5, L1, L2, L3, L4, checksum]
void exp6Send(uint8_t L1, uint8_t L2, uint8_t L3, uint8_t L4) {
  uint8_t pkt[6] = { 0xA5, L1, L2, L3, L4, 0 };
  pkt[5] = pkt[0] ^ L1 ^ L2 ^ L3 ^ L4;   // Checksum: XOR of the first 5 numbers
#ifdef EXP6_SERIAL
  EXP6_SERIAL.write(pkt, 6);
#endif
  exp6LastSend = millis();
}

// If nothing was sent for 0.1 s, send "I'm still here" (L1 = 1). It does not move any motor
void exp6KeepAlive() {
  if (millis() - exp6LastSend >= 100) exp6Send(1, 0, 0, 0);
}

// Use instead of delay(): if EXP6 gets no command for 0.5 s, it stops the motors by itself (watchdog)
void exp6Wait(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    exp6KeepAlive();
    delay(1);
  }
}

// Get the newest data pack: [0x5A, 32, 32 numbers, checksum], 35 bytes in total. EXP6 sends one every 0.01 s
bool exp6Receive() {
#ifdef EXP6_SERIAL
  exp6KeepAlive();
  while (EXP6_SERIAL.available()) EXP6_SERIAL.read();   // Throw away old data, keep only the newest
  static uint8_t buf[32];
  uint8_t n = 0, x = 0;
  unsigned long start = millis();
  while (millis() - start < 100) {
    if (!EXP6_SERIAL.available()) continue;
    uint8_t b = EXP6_SERIAL.read();
    if (n == 0) {                        // Look for the start mark 0x5A
      if (b == 0x5A) n = 1;
    } else if (n == 1) {                 // The 2nd byte must be 32 (32 numbers follow)
      if (b == 32) { x = b; n = 2; } else n = (b == 0x5A) ? 1 : 0;
    } else if (n < 34) {                 // The 32 numbers
      buf[n - 2] = b;
      x ^= b;
      n++;
    } else {                             // The last byte is the checksum. Keep the data only if it matches
      if (b == x) {
        memcpy(exp6Data, buf, 32);
        return true;
      }
      n = 0;
    }
  }
#endif
  return false;
}

// The number for port "port" (4 bytes make one number, low byte first)
int32_t exp6Port(uint8_t port) {
  if (port < 1 || port > 6) return 0;
  const uint8_t *d = &exp6Data[(port - 1) * 4];
  return (int32_t)((uint32_t)d[0] | ((uint32_t)d[1] << 8) | ((uint32_t)d[2] << 16) | ((uint32_t)d[3] << 24));
}

// What is on port "port": each port uses 3 bits
uint8_t exp6Device(uint8_t port) {
  uint16_t r7 = exp6Data[24] | ((uint16_t)exp6Data[25] << 8);   // 7th number: voltage + port 1
  uint16_t r8 = exp6Data[28] | ((uint16_t)exp6Data[29] << 8);   // 8th number: ports 2 ~ 6
  if (port == 1) return (r7 >> 12) & 0x07;
  if (port >= 2 && port <= 6) return (r8 >> ((port - 2) * 3)) & 0x07;
  return 0;
}

// Battery voltage x 100
uint16_t exp6Volt100() {
  return (exp6Data[24] | ((uint16_t)exp6Data[25] << 8)) & 0x0FFF;
}

// Wait until port "port" shows device "kind" (for example 5 = motor), wait up to timeoutMs
bool exp6WaitDevice(uint8_t port, uint8_t kind, uint16_t timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (exp6Receive() && exp6Device(port) == kind) {
      exp6Wait(200);                     // Just found it. Give EXP6 0.2 s more to get ready
      return true;
    }
  }
  return false;
}
```

**You will see:**

```text
=== EXP6: Turn to an angle ===
Motor found!
Turn 90 degrees (clockwise)
  Done! Angle now 90 deg (target 90 deg)
Turn back -90 degrees (counterclockwise)
  Done! Angle now 0 deg (target 0 deg)
Turn one full circle, 360 degrees
  Done! Angle now 361 deg (target 360 deg)
Turn to the 0 degree mark on the motor (takes the shortest way)
  Done! Check if the mark on the motor lines up.
Rest for 2 seconds...
```

**Two kinds of "turn to an angle":**

| | Relative angle `motorDegrees` | Absolute angle `motorGoTo` |
| :--- | :--- | :--- |
| Meaning | Turn N **more** degrees from where it is now | Turn to **a direction** on the motor |
| Range | -2499 ~ 2499 degrees (about 7 turns) | 0 ~ 359 degrees |
| Direction | You choose (positive = clockwise, negative = counterclockwise) | EXP6 picks the shortest way |
| Example | "Turn one more circle" | "Point back to 12 o'clock" |

> [!TIP]
> **Try this:** change the 90 in `turnDegrees(90, …)` to `720` (two turns); or change the 0 in `motorGoTo(MOTOR_PORT, SPEED, 0, AFTER_HOLD)` to `180` and see where the pointer stops.

> [!WARNING]
> **Send a turn-to-angle command only once.** EXP6 turns to the spot by itself. Do not keep sending it in `loop()`: if it gets the same command again after it arrives, it **turns again**.

---

<h2 id="ex4">4️⃣ Read the Motor Angle</h2>

**What it does:** a SPIKE motor has a very accurate angle sensor inside. Turn the motor by hand, and the computer prints how many degrees and turns it has moved. It also draws a small dial with 12 slots, where `#` shows the direction it points now. Type `r` in the Serial Monitor and press Enter to reset the angle to 0.

**Get ready:** 1 SPIKE motor on **port 1**.

<a href="/examples/exp6/arduino-en/exp6_04_read_angle/exp6_04_read_angle.ino" download data-ignore="true">📄 Download exp6_04_read_angle.ino</a>

<!-- ino: examples/exp6/arduino-en/exp6_04_read_angle/exp6_04_read_angle.ino -->
```cpp
/*
 * SPIKE 6-Way Expander EXP6 x Arduino   Example 4: Read the motor angle
 * ------------------------------------------------------------
 * What does this example do?
 *   A motor can do more than turn. It is also a very accurate "angle sensor".
 *   Turn the motor on port 1 by hand. The computer keeps printing how many degrees and turns it has moved,
 *   and draws a small dial that shows where it points now.
 *   Type r in the Serial Monitor and press Enter to reset the angle to 0.
 *
 * You need:
 *   The wiring from Example 1, plus 1 SPIKE motor plugged into port 1 of EXP6.
 *
 * Wiring (UNO R4; for Mega and ESP32, see the wiring table on the web page):
 *   EXP6 Host port pin 3 (GND)             -> GND
 *   EXP6 Host port pin 4 (VCC)             -> 5V
 *   EXP6 Host port pin 5 (EXP6 receives)   -> D1 (TX)
 *   EXP6 Host port pin 6 (EXP6 sends)      -> D0 (RX)
 *   EXP6 XT30 power socket                 -> 2~3S lithium battery
 *
 * How to see the result:
 *   After uploading, open the "Serial Monitor", choose 115200 baud, and turn the motor slowly by hand.
 *
 * Tip:
 *   The angle resets to 0 at power-on, so it starts at 0 degrees.
 */

const uint8_t MOTOR_PORT = 1;            // 👉 You can change this: which EXP6 port the motor is on (1 ~ 6)

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 waits for the computer to open the serial port
  Serial.println(F("=== EXP6: Read the motor angle ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("EXP6 not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  if (!exp6WaitDevice(MOTOR_PORT, 5, 3000)) {
    Serial.println(F("Motor not found. Plug the motor in, then press the Arduino reset button."));
    while (true) {}
  }
  Serial.println(F("Turn the motor by hand! Type r and press Enter to reset to 0."));
}

void loop() {
  // The computer sent r: reset the angle to 0
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'r' || c == 'R') {
      exp6Send(69 + MOTOR_PORT, 3, 0, 0);  // L1 = 69 + port (70 ~ 75), L2 = 3: reset angle to 0
      Serial.println(F("Angle reset to 0!"));
      exp6Wait(100);
    }
  }

  if (!exp6Receive()) {
    Serial.println(F("No data. Check if a wire is loose."));
    exp6Wait(500);
    return;
  }
  if (exp6Device(MOTOR_PORT) != 5) {
    Serial.println(F("Was the motor unplugged? Please plug it back in."));
    exp6Wait(500);
    return;
  }

  int32_t angle = exp6Port(MOTOR_PORT);   // How many degrees the motor has turned: clockwise goes up, counterclockwise goes down, it keeps adding up

  // Draw a small dial: one turn of 360 degrees split into 12 slots, # is where it points now
  int16_t inTurn = ((angle % 360) + 360) % 360;   // Change it to 0 ~ 359 degrees inside one turn
  Serial.print('[');
  for (uint8_t i = 0; i < 12; i++) {
    if (inTurn / 30 == i) Serial.print('#');
    else Serial.print('.');
  }
  Serial.print(F("] Angle "));
  Serial.print(angle);
  Serial.print(F(" deg, about "));
  Serial.print(angle / 360.0, 1);        // How many turns, printed to 1 decimal place
  Serial.println(F(" turns"));

  exp6Wait(200);                         // 👉 You can change this: check every 0.2 seconds
}

// ============================================================
// Below are the "Communication helpers": they talk to EXP6 (UART serial port).
// They are the same in every EXP6 example. No need to change or understand them.
// You only need these:
//   exp6Begin(ms)            Wake up EXP6 and wait for its data. Returns true if it works
//   exp6Send(L1, L2, L3, L4) Send one command (4 numbers, each 0 ~ 100)
//   exp6Receive()            Get the newest data pack (6 ports + battery). Returns true if received
//   exp6Port(port)           The number for that port (motor = angle, ultrasonic = distance ...)
//   exp6Device(port)         What is on that port: 0 none, 1 force, 3 color, 4 ultrasonic, 5 motor, 7 other
//   exp6Volt100()            Battery voltage x 100 (for example 812 = 8.12 V)
//   exp6Wait(ms)             Use instead of delay(): while waiting, it keeps telling EXP6 "I'm still here"
//   exp6WaitDevice(port, kind, ms)  Wait until a port shows the device you want. Returns true if found
// ============================================================

// Which serial port talks to EXP6 (picked for your board, no need to change)
#if defined(ARDUINO_ARCH_ESP32)
#define EXP6_SERIAL Serial2              // ESP32: RX2 = GPIO16, TX2 = GPIO17
#define EXP6_RX_PIN 16
#define EXP6_TX_PIN 17
#elif defined(ARDUINO_ARCH_RENESAS) || defined(HAVE_HWSERIAL1) || defined(ARDUINO_ARCH_MEGAAVR)
#define EXP6_SERIAL Serial1              // UNO R4: D0(RX) / D1(TX); Mega: 19(RX1) / 18(TX1)
#endif

uint8_t exp6Data[32];                    // Newest data pack: 8 numbers, 4 bytes each
unsigned long exp6LastSend = 0;          // Time of the last command we sent

// Wake up EXP6: keep sending "I'm still here" until EXP6 sends data back, wait up to timeoutMs
bool exp6Begin(uint16_t timeoutMs) {
#ifdef EXP6_SERIAL
#if defined(ARDUINO_ARCH_ESP32)
  EXP6_SERIAL.begin(115200, SERIAL_8N1, EXP6_RX_PIN, EXP6_TX_PIN);
#else
  EXP6_SERIAL.begin(115200);             // EXP6 always talks at 115200
#endif
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    exp6Send(1, 0, 0, 0);                // After the first correct command, EXP6 switches from "LEGO mode" to "UART mode"
    if (exp6Receive()) {
      exp6Send(0, 1, 0, 0);              // Send a "do nothing" command first, so the next one is not skipped as a repeat
      exp6Send(0, 0, 0, 0);              // Reset everything: motors stop, angles go back to 0
      exp6Wait(300);
      return exp6Receive();
    }
  }
#else
  (void)timeoutMs;                       // This board does not use this number
  Serial.println(F("This board has no second hardware serial port (for example UNO R3, Nano). It cannot connect to EXP6."));
  Serial.println(F("Please use UNO R4, Mega 2560 or ESP32."));
#endif
  return false;
}

// Send one command: [0xA5, L1, L2, L3, L4, checksum]
void exp6Send(uint8_t L1, uint8_t L2, uint8_t L3, uint8_t L4) {
  uint8_t pkt[6] = { 0xA5, L1, L2, L3, L4, 0 };
  pkt[5] = pkt[0] ^ L1 ^ L2 ^ L3 ^ L4;   // Checksum: XOR of the first 5 numbers
#ifdef EXP6_SERIAL
  EXP6_SERIAL.write(pkt, 6);
#endif
  exp6LastSend = millis();
}

// If nothing was sent for 0.1 s, send "I'm still here" (L1 = 1). It does not move any motor
void exp6KeepAlive() {
  if (millis() - exp6LastSend >= 100) exp6Send(1, 0, 0, 0);
}

// Use instead of delay(): if EXP6 gets no command for 0.5 s, it stops the motors by itself (watchdog)
void exp6Wait(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    exp6KeepAlive();
    delay(1);
  }
}

// Get the newest data pack: [0x5A, 32, 32 numbers, checksum], 35 bytes in total. EXP6 sends one every 0.01 s
bool exp6Receive() {
#ifdef EXP6_SERIAL
  exp6KeepAlive();
  while (EXP6_SERIAL.available()) EXP6_SERIAL.read();   // Throw away old data, keep only the newest
  static uint8_t buf[32];
  uint8_t n = 0, x = 0;
  unsigned long start = millis();
  while (millis() - start < 100) {
    if (!EXP6_SERIAL.available()) continue;
    uint8_t b = EXP6_SERIAL.read();
    if (n == 0) {                        // Look for the start mark 0x5A
      if (b == 0x5A) n = 1;
    } else if (n == 1) {                 // The 2nd byte must be 32 (32 numbers follow)
      if (b == 32) { x = b; n = 2; } else n = (b == 0x5A) ? 1 : 0;
    } else if (n < 34) {                 // The 32 numbers
      buf[n - 2] = b;
      x ^= b;
      n++;
    } else {                             // The last byte is the checksum. Keep the data only if it matches
      if (b == x) {
        memcpy(exp6Data, buf, 32);
        return true;
      }
      n = 0;
    }
  }
#endif
  return false;
}

// The number for port "port" (4 bytes make one number, low byte first)
int32_t exp6Port(uint8_t port) {
  if (port < 1 || port > 6) return 0;
  const uint8_t *d = &exp6Data[(port - 1) * 4];
  return (int32_t)((uint32_t)d[0] | ((uint32_t)d[1] << 8) | ((uint32_t)d[2] << 16) | ((uint32_t)d[3] << 24));
}

// What is on port "port": each port uses 3 bits
uint8_t exp6Device(uint8_t port) {
  uint16_t r7 = exp6Data[24] | ((uint16_t)exp6Data[25] << 8);   // 7th number: voltage + port 1
  uint16_t r8 = exp6Data[28] | ((uint16_t)exp6Data[29] << 8);   // 8th number: ports 2 ~ 6
  if (port == 1) return (r7 >> 12) & 0x07;
  if (port >= 2 && port <= 6) return (r8 >> ((port - 2) * 3)) & 0x07;
  return 0;
}

// Battery voltage x 100
uint16_t exp6Volt100() {
  return (exp6Data[24] | ((uint16_t)exp6Data[25] << 8)) & 0x0FFF;
}

// Wait until port "port" shows device "kind" (for example 5 = motor), wait up to timeoutMs
bool exp6WaitDevice(uint8_t port, uint8_t kind, uint16_t timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (exp6Receive() && exp6Device(port) == kind) {
      exp6Wait(200);                     // Just found it. Give EXP6 0.2 s more to get ready
      return true;
    }
  }
  return false;
}
```

**You will see:** (turning slowly clockwise a bit more than one turn)

```text
=== EXP6: Read the motor angle ===
Turn the motor by hand! Type r and press Enter to reset to 0.
[#...........] Angle 0 deg, about 0.0 turns
[...#........] Angle 95 deg, about 0.3 turns
[.........#..] Angle 287 deg, about 0.8 turns
[#...........] Angle 372 deg, about 1.0 turns
Angle reset to 0!
[#...........] Angle 0 deg, about 0.0 turns
```

> [!TIP]
> **Try this:** turn it counterclockwise and the angle becomes negative. The angle keeps adding up: 10 turns is 3600 degrees. It does not go back to 0.

---

<h2 id="ex5">5️⃣ Read the Sensors</h2>

**What it does:** EXP6 finds out by itself which sensor is on each port. This example prints the color, distance and force. When something comes within 10 cm of the ultrasonic sensor, or the force sensor is pressed, **the light on the Arduino board** turns on.

**Get ready:** SPIKE color, ultrasonic or force sensors. Plug in as many as you have, on any port.

<a href="/examples/exp6/arduino-en/exp6_05_sensors/exp6_05_sensors.ino" download data-ignore="true">📄 Download exp6_05_sensors.ino</a>

<!-- ino: examples/exp6/arduino-en/exp6_05_sensors/exp6_05_sensors.ino -->
```cpp
/*
 * SPIKE 6-Way Expander EXP6 x Arduino   Example 5: Read the sensors
 * ------------------------------------------------------------
 * What does this example do?
 *   EXP6 finds out by itself which sensor is on each port. This example prints all their values:
 *     Color sensor: which color it sees, how bright the reflected light is, how much red, green and blue
 *     Ultrasonic sensor: how far away the thing in front is (cm)
 *     Force sensor: how hard it is pressed
 *   When something comes within 10 cm of the ultrasonic sensor, or the force sensor is pressed, the light on the Arduino board turns on.
 *
 * You need:
 *   The wiring from Example 1, plus SPIKE color, ultrasonic or force sensors (as many as you have, on any port).
 *
 * Wiring (UNO R4; for Mega and ESP32, see the wiring table on the web page):
 *   EXP6 Host port pin 3 (GND)             -> GND
 *   EXP6 Host port pin 4 (VCC)             -> 5V
 *   EXP6 Host port pin 5 (EXP6 receives)   -> D1 (TX)
 *   EXP6 Host port pin 6 (EXP6 sends)      -> D0 (RX)
 *   EXP6 XT30 power socket                 -> 2~3S lithium battery
 *
 * How to see the result:
 *   After uploading, open the "Serial Monitor" and choose 115200 baud.
 */

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards do not say where the built-in light is, so use pin 2
#endif

const int16_t NEAR_MM   = 100;           // 👉 You can change this: how close (mm) counts as "something is near", 100 = 10 cm
const int16_t PRESS_MIN = 20;            // 👉 You can change this: above this value the force sensor counts as "pressed"

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 waits for the computer to open the serial port
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println(F("=== EXP6: Read the sensors ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("EXP6 not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  exp6Wait(1500);                        // A sensor takes a little over 1 second to be found after it is plugged in
}

void loop() {
  if (!exp6Receive()) {
    Serial.println(F("No data. Check if a wire is loose."));
    digitalWrite(LED_BUILTIN, LOW);
    exp6Wait(500);
    return;
  }

  bool alert = false;                    // Is something near, or is something pressed?
  uint8_t found = 0;                     // How many sensors were found

  for (uint8_t port = 1; port <= 6; port++) {
    uint8_t kind = exp6Device(port);
    int32_t value = exp6Port(port);      // The number for this port. What it means depends on the sensor

    if (kind == 3) {                     // ---- Color sensor ----
      found++;
      // One number holds 5 things: color code, reflected light, red, green, blue
      uint32_t v = (uint32_t)value;
      uint8_t color = (v >> 28) & 0x0F;  // Color code 0 ~ 10 (15 = cannot tell)
      uint8_t refl  = (v >> 21) & 0x7F;  // Reflected light 0 ~ 100 (brighter = bigger, used for line following)
      uint8_t r     = (v >> 14) & 0x7F;  // Red 0 ~ 100
      uint8_t g     = (v >> 7)  & 0x7F;  // Green 0 ~ 100
      uint8_t b     = v & 0x7F;          // Blue 0 ~ 100
      Serial.print(F("Port "));
      Serial.print(port);
      Serial.print(F(" color: "));
      printColorName(color);
      Serial.print(F("  reflected "));
      Serial.print(refl);
      Serial.print(F("  R "));
      Serial.print(r);
      Serial.print(F(" G "));
      Serial.print(g);
      Serial.print(F(" B "));
      Serial.println(b);
    } else if (kind == 4) {              // ---- Ultrasonic sensor ----
      found++;
      Serial.print(F("Port "));
      Serial.print(port);
      Serial.print(F(" distance: "));
      if (value < 0 || value >= 2000) {  // Too far, or nothing in front
        Serial.println(F("nothing in front"));
      } else {                           // The number is in mm. Divide by 10 to get cm
        Serial.print(value / 10);
        Serial.print('.');
        Serial.print(value % 10);
        Serial.println(F(" cm"));
        if (value < NEAR_MM) alert = true;
      }
    } else if (kind == 1) {              // ---- Force sensor ----
      found++;
      Serial.print(F("Port "));
      Serial.print(port);
      Serial.print(F(" force: "));
      Serial.println(value);             // 0 = not pressed. The harder you press, the bigger the number
      if (value > PRESS_MIN) alert = true;
    }
  }

  if (found == 0) Serial.println(F("No sensor found. Try plugging in a color, ultrasonic or force sensor."));
  Serial.println(F("--------"));

  digitalWrite(LED_BUILTIN, alert ? HIGH : LOW);   // Something near or pressed -> the light on the board turns on
  exp6Wait(500);                         // 👉 You can change this: check every 0.5 seconds
}

// Print the color code as a word (LEGO standard color codes)
void printColorName(uint8_t c) {
  switch (c) {
    case 0:  Serial.print(F("black"));   break;
    case 1:  Serial.print(F("violet"));   break;
    case 3:  Serial.print(F("blue"));   break;
    case 4:  Serial.print(F("azure")); break;
    case 5:  Serial.print(F("green"));   break;
    case 7:  Serial.print(F("yellow"));   break;
    case 9:  Serial.print(F("red"));   break;
    case 10: Serial.print(F("white"));   break;
    case 15: Serial.print(F("cannot tell")); break;
    default:
      Serial.print(F("other code "));
      Serial.print(c);
      break;
  }
}

// ============================================================
// Below are the "Communication helpers": they talk to EXP6 (UART serial port).
// They are the same in every EXP6 example. No need to change or understand them.
// You only need these:
//   exp6Begin(ms)            Wake up EXP6 and wait for its data. Returns true if it works
//   exp6Send(L1, L2, L3, L4) Send one command (4 numbers, each 0 ~ 100)
//   exp6Receive()            Get the newest data pack (6 ports + battery). Returns true if received
//   exp6Port(port)           The number for that port (motor = angle, ultrasonic = distance ...)
//   exp6Device(port)         What is on that port: 0 none, 1 force, 3 color, 4 ultrasonic, 5 motor, 7 other
//   exp6Volt100()            Battery voltage x 100 (for example 812 = 8.12 V)
//   exp6Wait(ms)             Use instead of delay(): while waiting, it keeps telling EXP6 "I'm still here"
//   exp6WaitDevice(port, kind, ms)  Wait until a port shows the device you want. Returns true if found
// ============================================================

// Which serial port talks to EXP6 (picked for your board, no need to change)
#if defined(ARDUINO_ARCH_ESP32)
#define EXP6_SERIAL Serial2              // ESP32: RX2 = GPIO16, TX2 = GPIO17
#define EXP6_RX_PIN 16
#define EXP6_TX_PIN 17
#elif defined(ARDUINO_ARCH_RENESAS) || defined(HAVE_HWSERIAL1) || defined(ARDUINO_ARCH_MEGAAVR)
#define EXP6_SERIAL Serial1              // UNO R4: D0(RX) / D1(TX); Mega: 19(RX1) / 18(TX1)
#endif

uint8_t exp6Data[32];                    // Newest data pack: 8 numbers, 4 bytes each
unsigned long exp6LastSend = 0;          // Time of the last command we sent

// Wake up EXP6: keep sending "I'm still here" until EXP6 sends data back, wait up to timeoutMs
bool exp6Begin(uint16_t timeoutMs) {
#ifdef EXP6_SERIAL
#if defined(ARDUINO_ARCH_ESP32)
  EXP6_SERIAL.begin(115200, SERIAL_8N1, EXP6_RX_PIN, EXP6_TX_PIN);
#else
  EXP6_SERIAL.begin(115200);             // EXP6 always talks at 115200
#endif
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    exp6Send(1, 0, 0, 0);                // After the first correct command, EXP6 switches from "LEGO mode" to "UART mode"
    if (exp6Receive()) {
      exp6Send(0, 1, 0, 0);              // Send a "do nothing" command first, so the next one is not skipped as a repeat
      exp6Send(0, 0, 0, 0);              // Reset everything: motors stop, angles go back to 0
      exp6Wait(300);
      return exp6Receive();
    }
  }
#else
  (void)timeoutMs;                       // This board does not use this number
  Serial.println(F("This board has no second hardware serial port (for example UNO R3, Nano). It cannot connect to EXP6."));
  Serial.println(F("Please use UNO R4, Mega 2560 or ESP32."));
#endif
  return false;
}

// Send one command: [0xA5, L1, L2, L3, L4, checksum]
void exp6Send(uint8_t L1, uint8_t L2, uint8_t L3, uint8_t L4) {
  uint8_t pkt[6] = { 0xA5, L1, L2, L3, L4, 0 };
  pkt[5] = pkt[0] ^ L1 ^ L2 ^ L3 ^ L4;   // Checksum: XOR of the first 5 numbers
#ifdef EXP6_SERIAL
  EXP6_SERIAL.write(pkt, 6);
#endif
  exp6LastSend = millis();
}

// If nothing was sent for 0.1 s, send "I'm still here" (L1 = 1). It does not move any motor
void exp6KeepAlive() {
  if (millis() - exp6LastSend >= 100) exp6Send(1, 0, 0, 0);
}

// Use instead of delay(): if EXP6 gets no command for 0.5 s, it stops the motors by itself (watchdog)
void exp6Wait(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    exp6KeepAlive();
    delay(1);
  }
}

// Get the newest data pack: [0x5A, 32, 32 numbers, checksum], 35 bytes in total. EXP6 sends one every 0.01 s
bool exp6Receive() {
#ifdef EXP6_SERIAL
  exp6KeepAlive();
  while (EXP6_SERIAL.available()) EXP6_SERIAL.read();   // Throw away old data, keep only the newest
  static uint8_t buf[32];
  uint8_t n = 0, x = 0;
  unsigned long start = millis();
  while (millis() - start < 100) {
    if (!EXP6_SERIAL.available()) continue;
    uint8_t b = EXP6_SERIAL.read();
    if (n == 0) {                        // Look for the start mark 0x5A
      if (b == 0x5A) n = 1;
    } else if (n == 1) {                 // The 2nd byte must be 32 (32 numbers follow)
      if (b == 32) { x = b; n = 2; } else n = (b == 0x5A) ? 1 : 0;
    } else if (n < 34) {                 // The 32 numbers
      buf[n - 2] = b;
      x ^= b;
      n++;
    } else {                             // The last byte is the checksum. Keep the data only if it matches
      if (b == x) {
        memcpy(exp6Data, buf, 32);
        return true;
      }
      n = 0;
    }
  }
#endif
  return false;
}

// The number for port "port" (4 bytes make one number, low byte first)
int32_t exp6Port(uint8_t port) {
  if (port < 1 || port > 6) return 0;
  const uint8_t *d = &exp6Data[(port - 1) * 4];
  return (int32_t)((uint32_t)d[0] | ((uint32_t)d[1] << 8) | ((uint32_t)d[2] << 16) | ((uint32_t)d[3] << 24));
}

// What is on port "port": each port uses 3 bits
uint8_t exp6Device(uint8_t port) {
  uint16_t r7 = exp6Data[24] | ((uint16_t)exp6Data[25] << 8);   // 7th number: voltage + port 1
  uint16_t r8 = exp6Data[28] | ((uint16_t)exp6Data[29] << 8);   // 8th number: ports 2 ~ 6
  if (port == 1) return (r7 >> 12) & 0x07;
  if (port >= 2 && port <= 6) return (r8 >> ((port - 2) * 3)) & 0x07;
  return 0;
}

// Battery voltage x 100
uint16_t exp6Volt100() {
  return (exp6Data[24] | ((uint16_t)exp6Data[25] << 8)) & 0x0FFF;
}

// Wait until port "port" shows device "kind" (for example 5 = motor), wait up to timeoutMs
bool exp6WaitDevice(uint8_t port, uint8_t kind, uint16_t timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (exp6Receive() && exp6Device(port) == kind) {
      exp6Wait(200);                     // Just found it. Give EXP6 0.2 s more to get ready
      return true;
    }
  }
  return false;
}
```

**You will see:** (color on port 3, ultrasonic on port 5, force on port 6)

```text
=== EXP6: Read the sensors ===
Port 3 color: red  reflected 41  R 62 G 9 B 8
Port 5 distance: 23.5 cm
Port 6 force: 0
--------
Port 3 color: white  reflected 96  R 88 G 90 B 85
Port 5 distance: nothing in front
Port 6 force: 57
--------
```

**Color codes:**

| Code | 0 | 1 | 3 | 4 | 5 | 7 | 9 | 10 | 15 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Color | black | violet | blue | azure | green | yellow | red | white | cannot tell |

> [!TIP]
> **Try this:** move the color sensor over a black line and white paper, and watch how the "reflected" number changes. A line-following car uses this number to know which side of the line it is on.

---

<h2 id="ex6">6️⃣ Two-Wheel Car (Advanced)</h2>

**What it does:** uses 2 SPIKE motors as the left and right wheels: forward → turn right → forward → turn left → backward → stop, again and again. The two wheels are controlled together with a "sync" command. EXP6 makes the two wheels work together, so the car drives straighter.

**Also need:** 2 SPIKE motors (left wheel on **port 1**, right wheel on **port 2**), a LEGO car body, and a power bank for the Arduino.

**Before the first run:**
1. First lift the car up (wheels in the air), then upload. Check that both wheels turn **forward**. If a wheel turns the wrong way, change `LEFT_REVERSE` or `RIGHT_REVERSE`.
2. Unplug the USB cable and plug in the power bank. Put the car on the floor and press the Arduino reset button. It starts after 3 seconds.
3. If it turns too much or too little, adjust `TURN_MS`.

<a href="/examples/exp6/arduino-en/exp6_06_drive_car/exp6_06_drive_car.ino" download data-ignore="true">📄 Download exp6_06_drive_car.ino</a>

<!-- ino: examples/exp6/arduino-en/exp6_06_drive_car/exp6_06_drive_car.ino -->
```cpp
/*
 * SPIKE 6-Way Expander EXP6 x Arduino   Example 6 (advanced): Two-wheel car
 * ------------------------------------------------------------
 * What does this example do?
 *   Use 2 SPIKE motors as the left and right wheels of a car. In order:
 *   forward -> turn right -> forward -> turn left -> backward -> stop 3 s, again and again.
 *   The two motors are controlled together with a "sync" command. EXP6 makes the two wheels work together, so the car drives straighter.
 *
 * You need:
 *   The wiring from Example 1, 2 SPIKE motors: left wheel on port 1, right wheel on port 2.
 *   A two-wheel car (a LEGO car body is fine). Arduino needs its own power (for example a power bank),
 *   because the EXP6 battery does not power the Arduino.
 *
 * Wiring (UNO R4; for Mega and ESP32, see the wiring table on the web page):
 *   EXP6 Host port pin 3 (GND)             -> GND
 *   EXP6 Host port pin 4 (VCC)             -> 5V
 *   EXP6 Host port pin 5 (EXP6 receives)   -> D1 (TX)
 *   EXP6 Host port pin 6 (EXP6 sends)      -> D0 (RX)
 *   EXP6 XT30 power socket                 -> 2~3S lithium battery
 *
 * How to see the result:
 *   First lift the car up (wheels in the air) and upload. Check that both wheels turn "forward".
 *   If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE.
 *   Put the car on the floor, press the Arduino reset button, and it starts after 3 seconds.
 *
 * Tip:
 *   Do not set either wheel speed to exactly 0 (to stop the car, use carStop).
 */

const uint8_t LEFT_PORT  = 1;            // 👉 You can change this: which port the left wheel motor is on
const uint8_t RIGHT_PORT = 2;            // 👉 You can change this: which port the right wheel motor is on (not the same as the left)
const bool LEFT_REVERSE  = true;         // 👉 You can change this: if the left wheel turns the wrong way, switch true / false
const bool RIGHT_REVERSE = false;        // 👉 You can change this: if the right wheel turns the wrong way, switch true / false
const int8_t SPEED      = 40;            // 👉 You can change this: forward speed 1 ~ 100
const int8_t TURN_SPEED = 25;            // 👉 You can change this: turning speed 1 ~ 100
const uint16_t TURN_MS  = 900;           // 👉 You can change this: how long to turn (ms). Adjust until it turns exactly 90 degrees

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 waits for the computer to open the serial port
  Serial.println(F("=== EXP6: Two-wheel car ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("EXP6 not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  if (!exp6WaitDevice(LEFT_PORT, 5, 3000) || !exp6WaitDevice(RIGHT_PORT, 5, 3000)) {
    Serial.println(F("The left or right wheel motor is not plugged in. Plug it in, then press the Arduino reset button."));
    while (true) {}
  }
  Serial.println(F("Ready. Starting in 3 seconds!"));
  exp6Wait(3000);
}

void loop() {
  Serial.println(F("Forward"));
  carDrive(SPEED, SPEED);                // Both wheels the same speed = drive straight
  if (!driveFor(2000)) return;

  Serial.println(F("Turn right"));
  carDrive(TURN_SPEED, -TURN_SPEED);     // Left wheel forward, right wheel backward = turn right on the spot
  if (!driveFor(TURN_MS)) return;

  Serial.println(F("Forward"));
  carDrive(SPEED, SPEED);
  if (!driveFor(2000)) return;

  Serial.println(F("Turn left"));
  carDrive(-TURN_SPEED, TURN_SPEED);     // Left wheel backward, right wheel forward = turn left on the spot
  if (!driveFor(TURN_MS)) return;

  Serial.println(F("Backward"));
  carDrive(-SPEED, -SPEED);
  if (!driveFor(1500)) return;

  Serial.println(F("Stop and rest for 3 seconds"));
  carStop();
  exp6Wait(3000);
}

// Drive for ms milliseconds. If there is no data or a motor is unplugged, stop and return false
bool driveFor(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (!exp6Receive() || exp6Device(LEFT_PORT) != 5 || exp6Device(RIGHT_PORT) != 5) {
      carStop();                         // Safety first: stop the car
      Serial.println(F("No data from the motors. Stopping! Check the motors and the battery."));
      exp6Wait(1000);
      return false;
    }
  }
  return true;
}

// ---------- Car command cheat sheet ----------
// Turn both wheels together: left, right = -100 ~ 100 (positive forward, negative backward)
void carDrive(int8_t left, int8_t right) {
  int16_t l = LEFT_REVERSE  ? -(int16_t)left  : left;    // For a motor mounted the other way, flip + and -
  int16_t r = RIGHT_REVERSE ? -(int16_t)right : right;
  uint8_t dirs = 0;                      // L2: bit 1 = left wheel reversed, bit 0 = right wheel reversed
  if (l < 0) { dirs |= 2; l = -l; }
  if (r < 0) { dirs |= 1; r = -r; }
  if (l > 100) l = 100;
  if (r > 100) r = 100;
  // L1 = left port x 10 + right port (for example 12); L3 = left wheel speed; L4 = right wheel speed
  exp6Send(LEFT_PORT * 10 + RIGHT_PORT, dirs, l, r);
}

// Brake both wheels together
void carStop() {
  exp6Send(LEFT_PORT * 10 + RIGHT_PORT, 5, 0, 0);   // Both speeds 0, L2 = 5: brake
}

// ============================================================
// Below are the "Communication helpers": they talk to EXP6 (UART serial port).
// They are the same in every EXP6 example. No need to change or understand them.
// You only need these:
//   exp6Begin(ms)            Wake up EXP6 and wait for its data. Returns true if it works
//   exp6Send(L1, L2, L3, L4) Send one command (4 numbers, each 0 ~ 100)
//   exp6Receive()            Get the newest data pack (6 ports + battery). Returns true if received
//   exp6Port(port)           The number for that port (motor = angle, ultrasonic = distance ...)
//   exp6Device(port)         What is on that port: 0 none, 1 force, 3 color, 4 ultrasonic, 5 motor, 7 other
//   exp6Volt100()            Battery voltage x 100 (for example 812 = 8.12 V)
//   exp6Wait(ms)             Use instead of delay(): while waiting, it keeps telling EXP6 "I'm still here"
//   exp6WaitDevice(port, kind, ms)  Wait until a port shows the device you want. Returns true if found
// ============================================================

// Which serial port talks to EXP6 (picked for your board, no need to change)
#if defined(ARDUINO_ARCH_ESP32)
#define EXP6_SERIAL Serial2              // ESP32: RX2 = GPIO16, TX2 = GPIO17
#define EXP6_RX_PIN 16
#define EXP6_TX_PIN 17
#elif defined(ARDUINO_ARCH_RENESAS) || defined(HAVE_HWSERIAL1) || defined(ARDUINO_ARCH_MEGAAVR)
#define EXP6_SERIAL Serial1              // UNO R4: D0(RX) / D1(TX); Mega: 19(RX1) / 18(TX1)
#endif

uint8_t exp6Data[32];                    // Newest data pack: 8 numbers, 4 bytes each
unsigned long exp6LastSend = 0;          // Time of the last command we sent

// Wake up EXP6: keep sending "I'm still here" until EXP6 sends data back, wait up to timeoutMs
bool exp6Begin(uint16_t timeoutMs) {
#ifdef EXP6_SERIAL
#if defined(ARDUINO_ARCH_ESP32)
  EXP6_SERIAL.begin(115200, SERIAL_8N1, EXP6_RX_PIN, EXP6_TX_PIN);
#else
  EXP6_SERIAL.begin(115200);             // EXP6 always talks at 115200
#endif
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    exp6Send(1, 0, 0, 0);                // After the first correct command, EXP6 switches from "LEGO mode" to "UART mode"
    if (exp6Receive()) {
      exp6Send(0, 1, 0, 0);              // Send a "do nothing" command first, so the next one is not skipped as a repeat
      exp6Send(0, 0, 0, 0);              // Reset everything: motors stop, angles go back to 0
      exp6Wait(300);
      return exp6Receive();
    }
  }
#else
  (void)timeoutMs;                       // This board does not use this number
  Serial.println(F("This board has no second hardware serial port (for example UNO R3, Nano). It cannot connect to EXP6."));
  Serial.println(F("Please use UNO R4, Mega 2560 or ESP32."));
#endif
  return false;
}

// Send one command: [0xA5, L1, L2, L3, L4, checksum]
void exp6Send(uint8_t L1, uint8_t L2, uint8_t L3, uint8_t L4) {
  uint8_t pkt[6] = { 0xA5, L1, L2, L3, L4, 0 };
  pkt[5] = pkt[0] ^ L1 ^ L2 ^ L3 ^ L4;   // Checksum: XOR of the first 5 numbers
#ifdef EXP6_SERIAL
  EXP6_SERIAL.write(pkt, 6);
#endif
  exp6LastSend = millis();
}

// If nothing was sent for 0.1 s, send "I'm still here" (L1 = 1). It does not move any motor
void exp6KeepAlive() {
  if (millis() - exp6LastSend >= 100) exp6Send(1, 0, 0, 0);
}

// Use instead of delay(): if EXP6 gets no command for 0.5 s, it stops the motors by itself (watchdog)
void exp6Wait(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    exp6KeepAlive();
    delay(1);
  }
}

// Get the newest data pack: [0x5A, 32, 32 numbers, checksum], 35 bytes in total. EXP6 sends one every 0.01 s
bool exp6Receive() {
#ifdef EXP6_SERIAL
  exp6KeepAlive();
  while (EXP6_SERIAL.available()) EXP6_SERIAL.read();   // Throw away old data, keep only the newest
  static uint8_t buf[32];
  uint8_t n = 0, x = 0;
  unsigned long start = millis();
  while (millis() - start < 100) {
    if (!EXP6_SERIAL.available()) continue;
    uint8_t b = EXP6_SERIAL.read();
    if (n == 0) {                        // Look for the start mark 0x5A
      if (b == 0x5A) n = 1;
    } else if (n == 1) {                 // The 2nd byte must be 32 (32 numbers follow)
      if (b == 32) { x = b; n = 2; } else n = (b == 0x5A) ? 1 : 0;
    } else if (n < 34) {                 // The 32 numbers
      buf[n - 2] = b;
      x ^= b;
      n++;
    } else {                             // The last byte is the checksum. Keep the data only if it matches
      if (b == x) {
        memcpy(exp6Data, buf, 32);
        return true;
      }
      n = 0;
    }
  }
#endif
  return false;
}

// The number for port "port" (4 bytes make one number, low byte first)
int32_t exp6Port(uint8_t port) {
  if (port < 1 || port > 6) return 0;
  const uint8_t *d = &exp6Data[(port - 1) * 4];
  return (int32_t)((uint32_t)d[0] | ((uint32_t)d[1] << 8) | ((uint32_t)d[2] << 16) | ((uint32_t)d[3] << 24));
}

// What is on port "port": each port uses 3 bits
uint8_t exp6Device(uint8_t port) {
  uint16_t r7 = exp6Data[24] | ((uint16_t)exp6Data[25] << 8);   // 7th number: voltage + port 1
  uint16_t r8 = exp6Data[28] | ((uint16_t)exp6Data[29] << 8);   // 8th number: ports 2 ~ 6
  if (port == 1) return (r7 >> 12) & 0x07;
  if (port >= 2 && port <= 6) return (r8 >> ((port - 2) * 3)) & 0x07;
  return 0;
}

// Battery voltage x 100
uint16_t exp6Volt100() {
  return (exp6Data[24] | ((uint16_t)exp6Data[25] << 8)) & 0x0FFF;
}

// Wait until port "port" shows device "kind" (for example 5 = motor), wait up to timeoutMs
bool exp6WaitDevice(uint8_t port, uint8_t kind, uint16_t timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (exp6Receive() && exp6Device(port) == kind) {
      exp6Wait(200);                     // Just found it. Give EXP6 0.2 s more to get ready
      return true;
    }
  }
  return false;
}
```

**You will see:** (while connected to the computer)

```text
=== EXP6: Two-wheel car ===
Ready. Starting in 3 seconds!
Forward
Turn right
Forward
Turn left
Backward
Stop and rest for 3 seconds
```

**How to adjust:**

| What the car does | What to change |
| :--- | :--- |
| Spins in place right at the start | One wheel turns the wrong way; change `LEFT_REVERSE` or `RIGHT_REVERSE` |
| Goes backward instead of forward | Flip both `_REVERSE` settings |
| Turns more than 90 degrees | Make `TURN_MS` smaller (for example 900 → 700) |
| Turns less than 90 degrees | Make `TURN_MS` bigger |

> [!WARNING]
> Do not let **only one** wheel speed be exactly 0 (for example `carDrive(40, 0)`): then EXP6 does not use sync control and drives the power directly, so the car is less steady. To stop the car, use `carStop()`.

---

## 📖 Command Cheat Sheet (for writing your own programs)

Every command is 6 numbers: `[0xA5, L1, L2, L3, L4, checksum]`, where checksum = XOR of the first 5 numbers. Just send it with the helper `exp6Send(L1, L2, L3, L4)`. **L1~L4 must all be between 0 and 100.**

| What you want | L1 | L2 | L3 | L4 |
| :--- | :---: | :--- | :--- | :--- |
| "I'm still here" (feed the watchdog) | `1` | 0 | 0 | 0 |
| Reset everything (stop, angles to 0, format back to auto) | `0` | 0 | 0 | 0 |
| Constant speed | port × 11 | 1 clockwise / 2 counterclockwise | speed 0~100 | 0 |
| Direct power (no speed control) | port × 10 | 1 clockwise / 2 counterclockwise | power 0~100 | acceleration (0 = instant) |
| Stop | port × 10 | 3 coast / 4 hold / 5 brake | 0 | 0 |
| Turn relative angle | port × 10 + 8 (clockwise) or + 9 (counterclockwise) | speed 1~100 | stop mode × 25 + hundreds of the angle | last two digits of the angle |
| Turn to absolute angle | port × 10 + 7 | speed 1~100 | stop mode × 4 + hundreds of the angle | last two digits of the angle |
| Two-wheel sync | left port × 10 + right port | +2 left reversed, +1 right reversed | left speed | right speed |
| Reset angle to 0 | 69 + port | 3 | 0 | 0 |
| Change the data format | 69 + port | 0 speed / 1 angle / 2 absolute angle / 255 auto | 0 | 0 |
| Motor mounted normal / reversed | 76 | port | 0 normal / 1 reversed | 0 |
| PID multipliers | 80 + port | P | I | D (50 each = factory) |

- **Stop mode** (for turning to an angle): 0 coast, 1 brake, 2 hold, 3 keep running.
- **If you send the exact same command twice in a row**, the second one is skipped (it only feeds the watchdog). Turn-to-angle and reset-angle commands are not affected by this rule.
- **Data EXP6 sends back**: one pack every 0.01 seconds, `[0x5A, 32, R1~R8, checksum]`. R1~R8 are 4 bytes each (low byte first). Checksum = XOR of bytes 2~34. R1~R6 are the numbers for the 6 ports. The low 12 bits of R7 are the battery voltage (× 100), bits 12~14 of R7 are the device code of port 1, and R8 holds the device codes of ports 2~6, 3 bits each.

---

## ❓ FAQ

**Q: I keep getting "EXP6 not found"?**
1. Check that TX and RX are not swapped: Arduino TX → pin 5, Arduino RX → pin 6.
2. Is pin 4 connected to 5V (3V3 on ESP32)? Without power on this pin, EXP6 does not turn on.
3. Is the EXP6 battery connected? Is the battery light on?
4. Make sure the board is an UNO R4, Mega or ESP32. UNO R3 and Nano cannot be used.

**Q: The first green light on EXP6 keeps blinking slowly?**
EXP6 has switched to UART mode, but it got no command for more than 0.5 seconds. Usually the program uses `delay()`, or the Arduino has frozen.

**Q: The motor turns for a moment and then stops?**
The watchdog stopped it: wait with `exp6Wait()`, not `delay()`.

**Q: The motor is plugged in, but it shows "none"?**
A motor you just plugged in takes 1~2 seconds to be found. Wait a moment in Example 1. If it still shows "none", try another port or another cable.

**Q: On UNO R4, do I need to unplug the D0 and D1 wires to upload?**
No. On UNO R4, D0 / D1 are separate from USB, so you do not need to unplug them to upload. (Only UNO R3 needs that.)

**Q: Can I use an Arduino Leonardo or Micro?**
Yes. Their D0 / D1 are also a separate `Serial1`, wired the same way as UNO R4.
