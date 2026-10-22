/*
 * Pathfinder 8-Way × Arduino   Example 4: One-key calibration
 * ------------------------------------------------------------
 * What does this example do?
 *   Makes the sensor remember the black and the white of "this field". Type a command in the Serial Monitor:
 *     b  → black calibration (E2): first put all 8 eyes of the sensor on the black line, then type it
 *     w  → white calibration (E3): first put the sensor on the white background, then type it
 *     a  → 5-second dynamic calibration (E7): after typing it, sweep the sensor back and forth over the black line and white background for 5 seconds
 *   The result is saved inside the sensor, so it remembers even after power off.
 *   Do it again on a new field or when the lighting is different.
 *
 * You need: Pathfinder 8-Way (universal I2C edition) + white paper + black electrical tape.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Type b, w or a in the input box and press Enter.
 *   The program waits until "F3 bit2 calibrating" goes back to 0, then prints "08 calibration count" to confirm it went up by 1.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("=== Pathfinder 8-Way: One-key calibration ==="));
  Serial.print(F("Calibration count so far: ")); Serial.println(mbcRead(ADDR, 0x08));
  Serial.println(F("Type b = black calibration, w = white calibration, a = 5 s dynamic calibration, then press Enter"));
}

void loop() {
  if (!Serial.available()) { delay(50); return; }   // No input: keep waiting
  char c = Serial.read();

  if (c == 'b') {
    Serial.println(F("Black calibration (the sensor must be on the black line)..."));
    mbcWrite(ADDR, 0xE2);                // E2 = remember what it sees now as "black"
    waitCalibDone(2000);                 // About 50 ms, wait at most 2 s
  } else if (c == 'w') {
    Serial.println(F("White calibration (the sensor must be on the white background)..."));
    mbcWrite(ADDR, 0xE3);                // E3 = remember what it sees now as "white"
    waitCalibDone(2000);
  } else if (c == 'a') {
    Serial.println(F("5 s dynamic calibration started! Sweep the sensor back and forth over the black line and white background..."));
    mbcWrite(ADDR, 0xE7);                // E7 = within 5 s, take the darkest and brightest as references
    waitCalibDone(8000);                 // 5 s + a little margin
  } else {
    return;                              // Ignore Enter, spaces and other characters
  }

  // Print the light values so you can check: after calibration black is near 0 and white is near 100
  int16_t light[8];
  if (mbcReadMany(ADDR, 0x10, 8, light)) {
    Serial.print(F("Light values of the 8 channels now: L "));
    for (uint8_t i = 0; i < 8; i++) { Serial.print(light[i]); Serial.print(' '); }
    Serial.println(F("R"));
  }
  delay(50);
}

// Wait until "F3 bit2 calibrating" goes back to 0, then print the calibration count
void waitCalibDone(uint16_t ms) {
  unsigned long start = millis();
  delay(20);                             // Give the sensor a moment to raise the flag
  while (millis() - start < ms) {
    int16_t st = mbcRead(ADDR, 0xF3);
    if (st != MBC_NONE && !(st & 0x04)) break;   // bit2 = 0 → done
    Serial.print('.');
    delay(100);
  }
  Serial.println();
  Serial.print(F("Calibration done! Total count: ")); Serial.println(mbcRead(ADDR, 0x08));
  Serial.println(F("Type b / w / a to do it again"));
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
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

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
