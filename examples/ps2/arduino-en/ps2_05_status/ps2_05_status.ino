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
