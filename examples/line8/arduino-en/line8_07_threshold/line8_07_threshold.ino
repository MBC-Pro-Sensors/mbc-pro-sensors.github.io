/*
 * Pathfinder 8-Way × Arduino   Example 7: Adjust the threshold
 * ------------------------------------------------------------
 * What does this example do?
 *   The "threshold" is the cut-off score the sensor uses to decide "is this channel on the line":
 *   a light value (0 black ~ 100 white) below the threshold counts as on the line. Default 50.
 *   Adjust it when the line is faint or the field is shiny.
 *   Type in the Serial Monitor:
 *     a number 1~99 then Enter → set the threshold with E4, then read 07 to confirm
 *     a                        → 5 s dynamic calibration with E7 (a bad threshold is often just a missing calibration)
 *   The program also prints the 8 light values and the line bitmap once per second, so you can see which channels count as "on the line" after the change.
 *
 * You need: Pathfinder 8-Way (universal I2C edition) + white paper + black electrical tape.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * Tip:
 *   The threshold set with E4 is kept in RAM. After the sensor reboots it goes back to the default 50.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("=== Pathfinder 8-Way: Adjust the threshold ==="));
  Serial.print(F("Current threshold (07): ")); Serial.println(mbcRead(ADDR, 0x07));
  Serial.println(F("Type 1~99 to set the threshold, or a for 5 s dynamic calibration, then press Enter"));
}

void loop() {
  // ---- Handle input ----
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s == "a") {
      Serial.println(F("5 s dynamic calibration started! Sweep the sensor back and forth over the black line and white background..."));
      mbcWrite(ADDR, 0xE7);              // E7 = 5 s dynamic calibration
      unsigned long start = millis();
      delay(20);
      while (millis() - start < 8000) {  // Wait until F3 bit2 "calibrating" goes back to 0
        int16_t st = mbcRead(ADDR, 0xF3);
        if (st != MBC_NONE && !(st & 0x04)) break;
        delay(100);
      }
      Serial.println(F("Calibration done"));
    } else if (s.length() > 0) {
      int n = s.toInt();
      if (n >= 1 && n <= 99) {
        uint8_t th = (uint8_t)n;
        mbcWrite(ADDR, 0xE4, &th, 1);    // E4 + 1 parameter = set the threshold
        delay(20);
        Serial.print(F("Threshold set to ")); Serial.print(n);
        Serial.print(F(", read back 07 = ")); Serial.println(mbcRead(ADDR, 0x07));
      } else {
        Serial.println(F("Please type a number 1~99, or a"));
      }
    }
  }

  // ---- Print the light values and bitmap once per second ----
  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    int16_t light[8];
    int16_t bitmap = mbcRead(ADDR, 0x05);
    if (mbcReadMany(ADDR, 0x10, 8, light) && bitmap != MBC_NONE) {
      Serial.print(F("Light L "));
      for (uint8_t i = 0; i < 8; i++) { Serial.print(light[i]); Serial.print(' '); }
      Serial.print(F("R  on line (L→R) "));
      for (uint8_t i = 0; i < 8; i++) Serial.print((bitmap >> i) & 1 ? '#' : '.');   // Channel 1 (bit0) is printed on the far left
      Serial.println();
    }
  }
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
