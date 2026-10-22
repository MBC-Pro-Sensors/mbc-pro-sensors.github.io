/*
 * Pathfinder 8-Way × Arduino   Example 5: Crossroad detector
 * ------------------------------------------------------------
 * What does this example do?
 *   Uses "04 segment count" and "05 line bitmap" to detect a crossroad:
 *     - segment count ≥ 2 → a branch (two or more line segments)
 *     - almost all 8 bits of the bitmap are on (very wide line) → right on the cross line of a crossroad
 *   At a crossroad, the board's built-in LED (L) blinks 3 times fast, and the program counts "which crossroad is this".
 *   Contests often say "turn right at the 3rd crossroad". This example teaches you to count crossroads.
 *
 * You need: Pathfinder 8-Way (universal I2C edition) + white paper with crossroads drawn on it.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move the sensor along the line through a crossroad and watch the LED and the messages.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // On most ESP32 boards the built-in LED is on GPIO 2
#endif

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const uint8_t CROSS_MIN_WIDTH = 6;       // If this many (or more) channels in the bitmap are on, it counts as a crossroad   👉 You can change this

int  crossCount = 0;                     // Which crossroad we are at
bool onCross    = false;                 // Are we on a crossroad right now (so the same crossroad is not counted many times)

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move the sensor along the line through a crossroad."));
}

void loop() {
  int16_t v[2];                          // v[0] = 04 segment count, v[1] = 05 line bitmap
  if (!mbcReadMany(ADDR, 0x04, 2, v)) { delay(100); return; }
  int16_t groups = v[0];
  uint8_t bitmap = (uint8_t)v[1];        // bit0 = channel 1 (far left) ... bit7 = channel 8

  // Count how many 1s are in the bitmap (= how many channels are on the line)
  uint8_t lit = 0;
  for (uint8_t i = 0; i < 8; i++) if (bitmap & (1u << i)) lit++;

  bool isCross = (lit >= CROSS_MIN_WIDTH) || (groups >= 2);   // Very wide, or two or more segments → crossroad

  if (isCross && !onCross) {             // Count only when we just "enter" a crossroad
    onCross = true;
    crossCount++;
    Serial.print(F("Crossroad #")); Serial.print(crossCount); Serial.print(F("!  bitmap "));
    for (uint8_t i = 0; i < 8; i++) Serial.print((bitmap >> i) & 1);   // left→right = channels 1~8
    Serial.print(F("  segments ")); Serial.println(groups);
    for (uint8_t k = 0; k < 3; k++) {    // Blink 3 times fast
      digitalWrite(LED_BUILTIN, HIGH); delay(80);
      digitalWrite(LED_BUILTIN, LOW);  delay(80);
    }
  } else if (!isCross && onCross) {
    onCross = false;                     // Left the crossroad; the next one can be counted
  }

  // Normally: LED on when the line is in sight, off when it is not
  digitalWrite(LED_BUILTIN, lit > 0 ? HIGH : LOW);
  delay(50);
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
