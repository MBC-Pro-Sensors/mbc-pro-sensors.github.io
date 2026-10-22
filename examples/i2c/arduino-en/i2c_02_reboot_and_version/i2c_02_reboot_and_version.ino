/*
 * MBC universal I2C × Arduino   Shared example 2: Read the ID card + reboot (F0~F5, EF)
 * ------------------------------------------------------------
 * What does this example do?
 *   Every MBC universal I2C sensor has the same 6-box "ID card":
 *     F0 device ID (low byte factory address, high byte hw[0]), F1 protocol version, F2 firmware version,
 *     F3 status flags, F4 update counter (+1 every 10 ms, 0~255 and around), F5 seconds since boot
 *   This example prints them once per second and tells you which product it is.
 *   Type r and Enter in the Serial Monitor → sends [EF][0xA5] to reboot. You will see F5 go back to zero and F4 keep counting.
 *
 * You need: any MBC universal I2C edition sensor.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200.
 *
 * Tip:
 *   One round of F4 is only 2.56 s, so it cannot tell you if the sensor rebooted; check whether F5 got smaller.
 *   F4 not moving = the sensor is busy (for example calibrating).
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // Sensor address   👉 Change this (Pathfinder 0x16, Ranger 0x29, Controller 0x42, IMU 0x6A, Sharpshooter 0x4C)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC universal I2C: ID card + reboot ==="));
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("Sensor not found. Please check ADDR and the wiring (you can run i2c_03_scan first)."));
    while (true) {}
  }
  Serial.println(F("Type r and Enter = reboot"));
}

void loop() {
  if (Serial.available() && Serial.read() == 'r') {
    uint8_t magic = 0xA5;
    mbcWrite(ADDR, 0xEF, &magic, 1);     // [EF][A5] = reboot; any other number is ignored
    Serial.println(F(">>> Reboot sent. Watch F5 go back to zero"));
    delay(500);
  }

  int16_t f[6];                          // F0~F5 are consecutive, read all 6 in one go
  if (!mbcReadMany(ADDR, 0xF0, 6, f)) { Serial.println(F("(rebooting / cannot read)")); delay(500); return; }

  uint8_t factory = (uint8_t)(f[0] & 0xFF);
  uint8_t hw0     = (uint8_t)((f[0] >> 8) & 0xFF);
  Serial.println(F("---------------------------"));
  Serial.print(F("F0 device ID     factory address 0x")); Serial.print(factory, HEX);
  Serial.print(F("  hw[0] = ")); Serial.print(hw0);
  Serial.print(F("  → ")); printProduct(factory, hw0); Serial.println();
  Serial.print(F("F1 protocol ver  ")); Serial.println(f[1]);
  Serial.print(F("F2 firmware ver  ")); Serial.print(f[2] / 100); Serial.print('.'); Serial.println(f[2] % 100);
  Serial.print(F("F3 status flags  0b"));
  for (int8_t i = 7; i >= 0; i--) Serial.print((f[3] >> i) & 1);
  Serial.println(F("  (bit0 = ready, other bits: see each product's page)"));
  Serial.print(F("F4 update count  ")); Serial.print(f[4]); Serial.println(F("  (keeps changing = the sensor is alive)"));
  Serial.print(F("F5 uptime        ")); Serial.print(f[5]); Serial.println(F(" s"));

  delay(50);
  delay(950);
}

// Tell the product from the factory address + hw[0]
void printProduct(uint8_t factory, uint8_t hw0) {
  if      (factory == 0x16 && hw0 == 8)  Serial.print(F("Pathfinder 8-Way"));
  else if (factory == 0x16 && hw0 == 16) Serial.print(F("Pathfinder 16-Way"));
  else if (factory == 0x29 && hw0 == 2)  Serial.print(F("Ranger 2-Way"));
  else if (factory == 0x29 && hw0 == 8)  Serial.print(F("Ranger 8-Way"));
  else if (factory == 0x42 && hw0 == 1)  Serial.print(F("Controller PS2"));
  else if (factory == 0x42 && hw0 == 2)  Serial.print(F("Controller PS4"));
  else if (factory == 0x6A)              Serial.print(F("IMU 9-Axis Gyro"));
  else if (factory == 0x4C && hw0 == 1)  Serial.print(F("Sharpshooter (School edition)"));
  else if (factory == 0x4C && hw0 == 2)  Serial.print(F("Sharpshooter (WRO edition)"));
  else                                   Serial.print(F("Unknown MBC device"));
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
