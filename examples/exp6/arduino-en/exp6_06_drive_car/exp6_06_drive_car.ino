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
