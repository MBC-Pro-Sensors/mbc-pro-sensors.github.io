/*
 * Ranger 2-Way × Arduino   Example 2: See all the values at once
 * ------------------------------------------------------------
 * What does this example do?
 *   Prints the sensor's whole "value table" every 0.5 s:
 *     01 left cm, 02 right cm, 03 nearest cm, 04 left mm, 05 right mm,
 *     06 init bitmap, 07 fault bitmap, 08 average of left and right in mm.
 *   The later examples only pick a few of these boxes. Get to know every box here first.
 *
 * You need: Ranger 2-Way (universal I2C edition) + your hand or a box.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move your hand in front of the left and right lasers.
 *
 * Good to know:
 *   Numbers 01~08 are in a row, so we read all 8 values at once (mbcReadMany). They are all from the same moment.
 *   120 cm / 1200 mm = nothing in front (or that laser failed).
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand in front of the left and right lasers."));
}

void loop() {
  int16_t a[8];                          // Holds the 8 boxes 01~08
  if (!mbcReadMany(ADDR, 0x01, 8, a)) { Serial.println(F("Cannot read")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 left distance    ")); Serial.print(a[0]); Serial.println(F(" cm   (0~120, 120 = nothing there)"));
  Serial.print(F("02 right distance   ")); Serial.print(a[1]); Serial.println(F(" cm"));
  Serial.print(F("03 nearest distance ")); Serial.print(a[2]); Serial.println(F(" cm   (the nearer of left and right)"));
  Serial.print(F("04 left distance    ")); Serial.print(a[3]); Serial.println(F(" mm   (0~1200, 10 times finer than cm)"));
  Serial.print(F("05 right distance   ")); Serial.print(a[4]); Serial.println(F(" mm"));
  Serial.print(F("06 init bitmap      ")); Serial.print(a[5]); Serial.println(F("     (bit0 left, bit1 right; 3 = both lasers booted OK)"));
  Serial.print(F("07 fault bitmap     ")); Serial.print(a[6]); Serial.println(F("     (bit0 left, bit1 right; 0 = both OK)"));
  Serial.print(F("08 average          ")); Serial.print(a[7]); Serial.println(F(" mm   ((left mm + right mm) / 2)"));

  delay(50);                             // Rest a bit after reading
  delay(450);                            // Update every 0.5 s so it is easy to read
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
