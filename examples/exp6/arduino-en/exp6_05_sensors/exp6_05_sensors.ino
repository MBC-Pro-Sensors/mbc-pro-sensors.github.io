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
