/*
 * Pathfinder 16-Way × Arduino   Example 2: See all the values at once
 * ------------------------------------------------------------
 * What does this example do?
 *   Prints the sensor's whole "value table" every 0.5 s:
 *     01 line position, 02 fine position, 03 line width, 04 segment count, 05 line bitmap,
 *     06 last lost direction, 07 threshold, 08 calibration count, 10~1F light values of channels 1~16.
 *   The later examples only pick a few of these boxes. Get to know every box here first.
 *
 * You need: Pathfinder 16-Way (universal I2C edition) + a sheet of white paper + black electrical tape (as the line).
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. Move the black line left and right under the sensor and watch each box change.
 *
 * Tip:
 *   Boxes with consecutive numbers can be read "in one go" (mbcReadMany). The values come from the same moment.
 *   At most 16 at a time, so 01~08 is one read and 10~1F is another.
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
  Serial.println(F("Ready! Move the black line under the sensor."));
}

void loop() {
  int16_t a[8];                          // Holds boxes 01~08
  int16_t light[16];                      // Holds boxes 10~1F (light values of channels 1~16)

  // Part 1: read 8 values in a row starting from 01 (01~08)
  if (!mbcReadMany(ADDR, 0x01, 8, a)) { Serial.println(F("Cannot read (part 1)")); delay(500); return; }
  // Part 2: read 16 values in a row starting from 10 (10~1F)
  if (!mbcReadMany(ADDR, 0x10, 16, light)) { Serial.println(F("Cannot read (part 2)")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 line position    ")); Serial.print(a[0]); Serial.println(F("   (-16 left ~ +16 right, 0 = center)"));
  Serial.print(F("02 fine position    ")); Serial.print(a[1]); Serial.println(F("   (-100 ~ +100, negative = line on the left)"));
  Serial.print(F("03 line width       ")); Serial.print(a[2]); Serial.println(F(" channels (0 = no line found)"));
  Serial.print(F("04 segment count    ")); Serial.print(a[3]); Serial.println(F("   (0 line lost, 1 one line, 2 or more = branch / crossroad)"));
  Serial.print(F("05 line bitmap      "));
  for (uint8_t i = 0; i < 16; i++) Serial.print(((uint16_t)a[4] >> i) & 1);   // Print bit0 (channel 1) on the left ... bit15 (channel 16) on the right
  Serial.println(F("   (left→right = channels 1~16, 1 = that channel is on the line)"));
  Serial.print(F("06 last lost dir    ")); Serial.print(a[5]); Serial.println(F("   (which side the line was on before it was lost, negative = left)"));
  Serial.print(F("07 threshold        ")); Serial.print(a[6]); Serial.println(F("   (1~99, default 50)"));
  Serial.print(F("08 calibration count ")); Serial.print(a[7]); Serial.println(F("   (how many calibrations since boot)"));

  Serial.print(F("10~1F light  L "));
  for (uint8_t i = 0; i < 16; i++) {      // Channel 1 (far left) is printed first
    if (light[i] < 10)  Serial.print(' ');
    if (light[i] < 100) Serial.print(' ');
    Serial.print(light[i]);
    Serial.print(' ');
  }
  Serial.println(F("R   (0 = black, 100 = white)"));

  delay(50);                             // A short rest after reading
  delay(450);                            // A bit more, so the screen updates every 0.5 s and is easy to read
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
