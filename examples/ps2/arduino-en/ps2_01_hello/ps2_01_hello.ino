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
