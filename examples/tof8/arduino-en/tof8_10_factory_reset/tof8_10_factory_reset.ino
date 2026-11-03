/*
 * Ranger 8-Way (command example 04) × Arduino: factory reset
 * ------------------------------------------------------------
 * What does this example do?
 *   First messes up the settings with E0, E1, E2 and E4, then uses command E3 (the parameter must be A5) to put everything back to the factory values at once:
 *     ranging mode 0 normal, filter 1, timing budget 33 ms, max distance 120 cm
 *
 * You need: Ranger 8-Way (universal I2C edition); nothing has to be placed in front of it.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   Serial Monitor at 115200. It prints the settings three times: "at the start", "after messing up" and "after the factory reset". Then type anything to do it again.
 *
 * Note:
 *   If the parameter is not A5 the sensor does nothing (so it cannot happen by accident). A factory reset does not change an I2C address you have changed.
 *   Unplugging the power and plugging it back in also brings the factory settings back (settings are kept only in memory).
 *   Commands: E0 ranging mode, E1 filter, E2 timing budget, E3 factory reset, E4 max distance (each takes 1 parameter byte).
 *   Read the current settings back: 20 ranging mode, 21 filter, 22 timing budget ms, 23 max distance cm.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);
bool mbcCommand(uint8_t cmd, uint8_t arg);
void printSettings();
void run();

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  mbcCommand(0xE3, 0xA5);                // Factory reset first, so we start from a clean state
  run();
}

void run() {
  Serial.println(F("---- 1. At the start ----"));
  printSettings();
  Serial.println(F("---- 2. Mess up: E0 1 long range, E1 0 filter off, E2 100 ms, E4 80 cm ----"));
  mbcCommand(0xE0, 1);
  mbcCommand(0xE1, 0);
  mbcCommand(0xE2, 100);
  mbcCommand(0xE4, 80);
  printSettings();
  Serial.println(F("---- 3. Send E3 A5 factory reset ----"));
  mbcCommand(0xE3, 0xA5);
  printSettings();
  Serial.println(F("(Type anything to do it again)"));
}

void loop() {
  if (Serial.available()) {
    while (Serial.available()) Serial.read();
    run();
  }
}

// Send one command [code][parameter], then wait until the sensor has finished (bit2 "busy" of status flags F3 turns 0), at most 3 s
// Ranging mode, timing budget and factory reset re-configure the laser chips, about 0.1~0.5 s; filter and max distance take effect at once
bool mbcCommand(uint8_t cmd, uint8_t arg) {
  if (!mbcWrite(ADDR, cmd, &arg, 1)) return false;
  delay(20);                             // Give the sensor time to queue the command
  unsigned long start = millis();
  while (millis() - start < 3000) {
    int16_t f = mbcRead(ADDR, 0xF3);
    if (f != MBC_NONE && !(f & 4)) return true;   // bit2 = 0: done
    delay(10);
  }
  return false;
}

// Read the 4 current settings back from the sensor and print them (20 ranging mode, 21 filter, 22 timing budget ms, 23 max distance cm)
void printSettings() {
  int16_t s[4];
  if (!mbcReadMany(ADDR, 0x20, 4, s)) { Serial.println(F("Cannot read the settings")); return; }
  const char *modeName[] = {"normal", "long range", "accurate", "fast"};
  Serial.print(F("[Current settings] ranging mode ")); Serial.print(s[0]); Serial.print(F(" "));
  Serial.print((s[0] >= 0 && s[0] <= 3) ? modeName[s[0]] : "?");
  Serial.print(F(" | filter ")); Serial.print(s[1]);
  Serial.print(F(" | timing budget ")); Serial.print(s[2]); Serial.print(F(" ms"));
  Serial.print(F(" | max distance ")); Serial.print(s[3]); Serial.println(F(" cm"));
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
