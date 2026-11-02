/*
 * IMU Gyro x Arduino   Example 12: Heading correction (when one turn is not exactly 360 degrees)
 * ------------------------------------------------------------
 * What does this example do?
 *   Some gyros do not read exactly 360 degrees for one full turn (for example 357 or 363).
 *   This example helps you measure "how many degrees one real turn reads", works out the correction
 *   factor and saves it in the gyro with command EA:
 *     1. Lay the gyro flat with its front lined up with the desk edge -> press Enter
 *        (the program first sends EA 0 to go back to "360.0 per turn", so what it measures is the raw, uncorrected angle)
 *     2. Seen from above, turn it clockwise exactly TURNS full turns (5 by default) until the front lines up
 *        with the same desk edge again -> press Enter
 *     3. It reads the angle accumulated by number 03 (gyro 1D heading) and works out EA = degrees read per turn x 10
 *        Example: 5 turns read 1785 degrees -> 357.0 degrees per turn -> EA = 3570
 *     4. EA is written only if it is within 3500~3700 (otherwise it asks you to try again)
 *   After writing, the gyro scales its readings to 360 degrees per turn (reading x 360 / 357.0).
 *   At any time type r and press Enter: write EA back to 0 (back to 360.0 per turn, no correction).
 *
 * You need: IMU Gyro (Universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32. No car needed.
 *           A desk with a straight edge (or a book) to line it up against.
 *
 * Wiring:
 *   Gyro VCC -> 5V      Gyro GND -> GND
 *   Gyro SDA -> A4      Gyro SCL -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor, choose 115200 baud and follow the instructions on screen.
 *   In the lower right corner of the Serial Monitor choose "New Line" (or "Both NL & CR"), then just press Enter;
 *   with "No line ending", type any character (for example g) and then press Enter.
 *
 * Tip:
 *   After writing, the gyro waits 3 s before saving to EEPROM: wait more than 3 s before unplugging. Once saved,
 *   it is kept after power off.
 *   Changing the factor sets the heading to zero once.
 *   Each measurement first clears the old factor to 0; if nothing is written (out of range, try again),
 *   the factor stays at 360.0.
 *   Turn slowly, wind up the cable first (or turn the Arduino together with it), and count the turns carefully.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x6A;               // The IMU Gyro's "house number" (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- 👉 You can change these ----
const int TURNS = 5;                     // How many turns (more turns = more accurate, at most 80)
const uint16_t EA_MIN = 3500;            // Range the gyro accepts: 350.0 ~ 370.0 degrees per turn
const uint16_t EA_MAX = 3700;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("Gyro not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("=== IMU Gyro: heading correction EA ==="));
  printMenu();
}

void loop() {
  char ch = waitKey(false);              // Wait for the user to press Enter (or type something)
  if (ch == 'r' || ch == 'R') resetCorrection();
  else measure();
  printMenu();
}

void printMenu() {
  Serial.println();
  Serial.println(F("Press Enter to start measuring; type r and press Enter = write EA back to 0 (360.0 per turn)"));
}

// ---- Measure once, work out EA, write it ----
void measure() {
  // Step 1: line it up
  Serial.println(F("Step 1: lay the gyro flat on the desk, line up its front (the end the printed X arrow points to) with the desk edge, keep it still, then press Enter"));
  char ch = waitKey(false);
  if (ch == 'r' || ch == 'R') { resetCorrection(); return; }

  // First go back to 360.0 per turn (no correction) so we measure the raw angle; EA also sets the heading to zero
  int st = sendEA(0);
  if (st != 0) { Serial.print(F("EA 0 did not succeed, state ")); Serial.println(st); return; }
  delay(300);
  Serial.print(F("Heading set to zero, 03 gyro 1D heading = ")); Serial.println(mbcRead(ADDR, 0x03));

  // Step 2: turn TURNS times
  Serial.print(F("Step 2: seen from above, slowly turn it clockwise exactly "));
  Serial.print(TURNS);
  Serial.println(F(" full turns, line the front up with the same desk edge again, keep it still, then press Enter"));
  waitKey(true);                         // While waiting, print the current angle every second
  delay(300);
  int16_t d = mbcRead(ADDR, 0x03);       // 03 gyro 1D heading: keeps adding up, 5 turns is about 1800
  if (d == MBC_NONE) { Serial.println(F("Cannot read the gyro")); return; }

  if (d < 0) {
    Serial.print(F("Read ")); Serial.print(d);
    Serial.println(F(" degrees (negative): it looks like you turned counterclockwise. Turn clockwise as seen from above and try again."));
    return;
  }

  // Step 3: EA = degrees read per turn x 10 (rounded)
  long ea = ((long)d * 10 + TURNS / 2) / TURNS;
  Serial.print(TURNS); Serial.print(F(" turns read ")); Serial.print(d);
  Serial.print(F(" degrees -> ")); Serial.print(ea / 10); Serial.print('.'); Serial.print(ea % 10);
  Serial.print(F(" degrees per turn -> EA = ")); Serial.println(ea);

  if (ea < EA_MIN || ea > EA_MAX) {
    Serial.println(F("Outside 3500~3700 (350~370 degrees per turn): the turns may be miscounted, not lined up, or turned too fast. Not written, please try again."));
    return;
  }

  // Step 4: write it into the gyro
  st = sendEA((uint16_t)ea);
  if (st == 0) {
    Serial.print(F("Written EA = ")); Serial.print(ea);
    Serial.println(F(". The gyro waits 3 s before saving to EEPROM, please wait before unplugging; heading set to zero."));
    delay(3500);
    Serial.print(F("Saved. To check it, open Example 3, type z to zero, then turn clockwise "));
    Serial.print(TURNS); Serial.print(F(" turns: 03 should read about ")); Serial.println(TURNS * 360L);
  } else {
    Serial.print(F("Writing failed, state ")); Serial.println(st);
  }
}

// ---- Write EA back to 0 ----
void resetCorrection() {
  int st = sendEA(0);
  if (st == 0) Serial.println(F("EA written back to 0 (360.0 per turn, no correction), heading set to zero. Saved to EEPROM after 3 s."));
  else { Serial.print(F("EA 0 did not succeed, state ")); Serial.println(st); }
}

// Send EA [factor uint16 little-endian]: low byte first, then high byte. Returns the mbcCmd state
int sendEA(uint16_t v) {
  uint8_t p[2] = {(uint8_t)(v & 0xFF), (uint8_t)(v >> 8)};
  return mbcCmd(ADDR, 0xEA, p, 2, 1500);
}

// Wait for the user to press Enter (or type a character and press Enter). Returns the first character typed (just Enter = '\n')
// showAngle = true: while waiting, print 03 gyro 1D heading every second
char waitKey(bool showAngle) {
  while (Serial.available()) Serial.read();      // First throw away anything left over
  unsigned long last = millis();
  while (!Serial.available()) {
    if (showAngle && millis() - last >= 1000) {
      last = millis();
      Serial.print(F("  now 03 = ")); Serial.println(mbcRead(ADDR, 0x03));
    }
    delay(10);
  }
  char ch = Serial.read();
  delay(50);
  while (Serial.available()) Serial.read();      // Throw away the rest of the line (for example the newline)
  return ch;
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
