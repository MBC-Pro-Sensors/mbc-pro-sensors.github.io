/*
 * IMU Gyro x Arduino   Example 11: Send commands and check whether they are done (command status 0B)
 * ------------------------------------------------------------
 * What does this example do?
 *   Step by step it shows the gyro's new commands. After each command it reads the "command status"
 *   (number 0B) to check the result:
 *     1. Read the current heading
 *     2. E5 [90]: set the heading to 90 degrees -> 01 / 02 / 03 all read about 90
 *     3. E0: heading to zero -> all about 0
 *     4. E7: re-estimate the gyro bias (keep it still for about 1.5 s)
 *     5. E6: north as zero -> 02 fused heading = the magnetic bearing it faces now (0 north, 90 east, 180 south, 270 west)
 *     6. Print the "command status code table"
 *   0B = (last command code x 256) + state, for example 0xE500 = E5 done, 0xE701 = E7 still busy.
 *   State: 0 done, 1 busy (still working), 2 failed, 3 rejected (cannot do it right now).
 *
 * You need: IMU Gyro (Universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32. No car needed.
 *
 * Wiring:
 *   Gyro VCC -> 5V      Gyro GND -> GND
 *   Gyro SDA -> A4      Gyro SCL -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor, choose 115200 baud. Lay the gyro flat on the desk, do not touch it,
 *   and watch it print each step.
 *   When it is finished, type any character and press Enter to run it again.
 *
 * Tip:
 *   E6 needs a working magnetometer: keep away from motors, speakers and iron desks. "3 rejected" usually means the
 *   magnetic fusion is not ready yet: wait a few seconds and try again, or calibrate the magnetometer with Example 8 first.
 *   E5 and E6 only change RAM. After a power cycle the heading starts at 0 again.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x6A;               // The IMU Gyro's "house number" (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("Gyro not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  delay(2000);                           // Wait 2 more seconds after power-on so the magnetic fusion is ready (E6 needs it)
  runDemo();
}

void loop() {
  // Type any character + Enter -> run again
  if (Serial.available()) {
    delay(50);
    while (Serial.available()) Serial.read();   // Throw away the rest of the line (for example the newline)
    runDemo();
  }
}

// ---- The whole demo ----
void runDemo() {
  Serial.println();
  Serial.println(F("=== IMU Gyro: new commands and command status 0B ==="));

  // ---- 1. Current heading ----
  Serial.println(F("[1] Current heading"));
  printHeadings();

  // ---- 2. E5 [90]: set the heading to 90 degrees (parameter int16 little-endian: low byte first, then high byte) ----
  Serial.println(F("[2] Send E5 [90]: set the heading to 90 degrees"));
  int16_t angle = 90;
  uint8_t p[2] = {(uint8_t)(angle & 0xFF), (uint8_t)((angle >> 8) & 0xFF)};
  int st = mbcCmd(ADDR, 0xE5, p, 2, 1000);
  printStatus(st);
  if (st == 0) {
    waitFused(90);                       // 02 fused heading waits for the fusion to restart (up to about 3 s)
    printHeadings();
  }

  // ---- 3. E0: heading to zero ----
  Serial.println(F("[3] Send E0: heading to zero"));
  st = mbcCmd(ADDR, 0xE0, NULL, 0, 1000);
  printStatus(st);
  if (st == 0) {
    waitFused(0);
    printHeadings();
  }

  // ---- 4. E7: re-estimate the gyro bias (needs about 1.5 s of stillness; fails if not done within 4 s) ----
  Serial.println(F("[4] Send E7: re-estimate the gyro bias. Do not move the gyro!"));
  for (int i = 3; i >= 1; i--) {         // Count down 3 s so you can take your hands away
    Serial.print(F("    ")); Serial.print(i); Serial.println(F("..."));
    delay(1000);
  }
  unsigned long t0 = millis();
  st = mbcCmd(ADDR, 0xE7, NULL, 0, 5000);
  printStatus(st);
  Serial.print(F("    took ")); Serial.print(millis() - t0); Serial.println(F(" ms"));
  if (st == 2) Serial.println(F("    Failed = it was moved during this time. Lay it flat, do not touch it, and run again."));

  // ---- 5. E6: north as zero (when done, 02 fused heading = the current magnetic bearing 0~359) ----
  Serial.println(F("[5] Send E6: north as zero"));
  st = mbcCmd(ADDR, 0xE6, NULL, 0, 4000);
  printStatus(st);
  if (st == 0) {
    delay(50);
    int16_t fused = mbcRead(ADDR, 0x02);
    int16_t mag = mbcRead(ADDR, 0x04);
    Serial.print(F("    02 fused heading = ")); Serial.print(fused);
    Serial.print(F(" deg (= the magnetic bearing it faces now: 0 north, 90 east, 180 south, 270 west)  04 magnetic heading = "));
    Serial.println(mag);
  } else if (st == 3) {
    int16_t f3 = mbcRead(ADDR, 0xF3);
    Serial.print(F("    Rejected: the magnetometer is not ready. F3 bit6 magnetic fusion initialised = "));
    Serial.println((f3 != MBC_NONE && (f3 & 0x40)) ? F("yes") : F("no"));
    Serial.println(F("    (or a calibration is running). Keep away from iron, wait a few seconds and try again; if it never works, calibrate the magnetometer with Example 8."));
  } else if (st == 2) {
    Serial.println(F("    Failed: the fusion was not ready within 3 s (too much magnetic disturbance?). Try another place."));
  }

  // ---- 6. Table ----
  printStatusTable();
  Serial.println(F("Finished. Type any character and press Enter to run again."));
}

// Read the three headings 01~03 at once and print them
void printHeadings() {
  int16_t h[3];
  if (!mbcReadMany(ADDR, 0x01, 3, h)) { Serial.println(F("    Cannot read the gyro")); return; }
  Serial.print(F("    01 gyro 3D = ")); Serial.print(h[0]);
  Serial.print(F("   02 fused = "));    Serial.print(h[1]);
  Serial.print(F("   03 gyro 1D = "));  Serial.println(h[2]);
}

// Wait until 02 fused heading catches up with the target (within 10 degrees), up to 4 s
void waitFused(int target) {
  unsigned long start = millis();
  while (millis() - start < 4000) {
    int16_t f = mbcRead(ADDR, 0x02);
    if (f != MBC_NONE && abs(f - target) <= 10) return;
    delay(100);
  }
}

// Print what the status code means, plus the raw 0B value
void printStatus(int st) {
  Serial.print(F("    -> state ")); Serial.print(st); Serial.print(' ');
  switch (st) {
    case 0:  Serial.print(F("done")); break;
    case 1:  Serial.print(F("busy (still not finished when the wait timed out)")); break;
    case 2:  Serial.print(F("failed")); break;
    case 3:  Serial.print(F("rejected (cannot do it right now)")); break;
    default: Serial.print(F("cannot read the gyro")); break;
  }
  int16_t b = mbcRead(ADDR, 0x0B);
  if (b != MBC_NONE) {
    Serial.print(F("   0B = 0x")); Serial.print((uint16_t)b, HEX);
  }
  Serial.println();
}

void printStatusTable() {
  Serial.println(F("=== Command status code table (low byte of 0B; high byte = command code) ==="));
  Serial.println(F("  0  done: the command has finished"));
  Serial.println(F("  1  busy: still working (E6 waits for the magnetic fusion, E7 for stillness, E8 for the mount axis)"));
  Serial.println(F("  2  failed: wrong parameter (missing, out of range, wrong magic byte), or it did not succeed (for example E7 was moved)"));
  Serial.println(F("  3  rejected: cannot do it right now (E5~E8 and EB during calibration; E2 / E4 / E9 when not calibrating; E6 when the magnetometer is not ready)"));
  Serial.println(F(" -1  no reply: no answer on I2C, check the wiring (this code comes from the example, not from the gyro)"));
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

// ============================================================
// Command helper (only in Examples 11 and 12): send a command and wait for the "command status" 0B to report the result.
//   0B = (command code << 8) | state; state 0 done, 1 busy, 2 failed, 3 rejected.
//   Note: when the same command is sent twice in a row, right after sending 0B may still show the result of the
//   previous one. So we first note F4 (snapshot counter: +1 every 10 ms or after a command, wraps 0~255) and only
//   trust 0B once F4 is at least 2 higher than before the command: by then 0B is surely from after this command.
// Returns: 0 done, 1 busy (not finished within timeoutMs), 2 failed, 3 rejected, -1 no reply / could not write
// ============================================================
int mbcCmd(uint8_t addr, uint8_t code, const uint8_t *p, uint8_t n, uint16_t timeoutMs) {
  int16_t s0 = mbcRead(addr, 0xF4);      // Snapshot counter before the command
  if (s0 == MBC_NONE) return -1;
  if (!mbcWrite(addr, code, p, n)) return -1;
  bool fresh = false;                    // Has F4 gone up by 2 yet? (only trust 0B after that)
  bool gotAny = false;                   // Did we ever read anything?
  unsigned long start = millis();
  while (true) {
    int16_t s = mbcRead(addr, 0xF4);     // Read F4 first, then 0B (0B is never older than F4)
    int16_t b = mbcRead(addr, 0x0B);
    if (s != MBC_NONE && b != MBC_NONE) {
      gotAny = true;
      if ((uint8_t)((uint8_t)s - (uint8_t)s0) >= 2) fresh = true;
      uint8_t who = (uint16_t)b >> 8;    // High byte: last command code
      uint8_t state = b & 0xFF;          // Low byte: state
      if (fresh && who == code && state != 1) return state;
    }
    if (millis() - start >= timeoutMs) return gotAny ? 1 : -1;
    delay(10);
  }
}
