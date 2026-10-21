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
