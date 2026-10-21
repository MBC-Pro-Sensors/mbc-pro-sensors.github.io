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
