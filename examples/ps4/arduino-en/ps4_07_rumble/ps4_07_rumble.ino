/*
 * Controller PS4/PS5 x Arduino   Example 7: Make the gamepad rumble
 * ------------------------------------------------------------
 * What does this example do?
 *   Hold the ✕ button and the gamepad rumbles (strength 200); release it and it stops.
 *   Press the R2 trigger: the harder you press, the stronger it rumbles (R2's 0~255 is used directly as the strength).
 *   It uses command E5: write [0xE5][strength 0~255], 0 = stop. Each command rumbles for 200 ms,
 *   so while you hold the button, the program sends it again every 150 ms to keep the rumble going.
 *
 * You need: Controller PS4/PS5 receiver (Universal I2C edition) + a PS4 or PS5 gamepad (already paired), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *
 * How to see the result:
 *   After uploading, press ✕ or R2 and the gamepad rumbles. The Serial Monitor (115200) prints the strength that was sent.
 *
 * Tip:
 *   The PS2 receiver replies ACK to E5 but ignores it (rumble is turned off for PS2 gamepads). This example only works with PS4/PS5.
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const uint8_t BTN_CROSS = 14;            // ✕ is bit 14 of the 16 buttons
const uint8_t RUMBLE_BY_CROSS = 200;     // 👉 You can change this: rumble strength when ✕ is held, 0~255

uint8_t lastSent = 0;                    // The strength we sent last time
unsigned long lastSend = 0;              // When we last sent it

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Press ✕ or R2 and the gamepad rumbles."));
}

void loop() {
  // Read numbers 01 ~ 09 (9 values) in one go: v[0] = 01 16 buttons, v[8] = 09 R2 analog
  int16_t v[9];
  if (!mbcReadMany(ADDR, 0x01, 9, v)) { delay(100); return; }

  uint8_t strength = 0;                                  // The strength to send this pass
  if ((uint16_t)v[0] & (1u << BTN_CROSS)) strength = RUMBLE_BY_CROSS;   // ✕ is held
  if (v[8] > strength) strength = (uint8_t)v[8];         // If R2 is pressed harder, use R2's value

  // If the strength changed, or the button has been held for more than 150 ms, send E5 once (each rumble lasts only 200 ms, so keep sending)
  if (strength != lastSent || (strength > 0 && millis() - lastSend >= 150)) {
    mbcWrite(ADDR, 0xE5, &strength, 1);  // Command E5: [strength]
    lastSend = millis();
    if (strength != lastSent) {
      Serial.print(F("Rumble strength -> ")); Serial.println(strength);
    }
    lastSent = strength;
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
