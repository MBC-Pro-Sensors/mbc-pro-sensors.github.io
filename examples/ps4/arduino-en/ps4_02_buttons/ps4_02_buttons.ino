/*
 * Controller PS4/PS5 x Arduino  Example 2: Which button did I press?
 * ------------------------------------------------------------
 * What does this example do?
 *   Press any button on the gamepad and the computer tells you "Pressed: ✕ Cross".
 *   When you let go it says "Released: ✕ Cross". You can press several at once.
 *   The PS button in the middle of the gamepad works too.
 *
 * You need:
 *   Controller PS4/PS5 receiver (Universal I2C edition) + a PS4 or PS5 gamepad (already paired).
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Press the PS button to connect the gamepad, then press some buttons.
 *
 * Good to know:
 *   The state of the 16 buttons is kept in one 16-bit number. Each button uses one "bit":
 *   pressed = 1, not pressed = 0. Like 16 light switches in a row.
 *   Pressing "up and down together" or "left and right together" on the D-pad is not printed:
 *   a gamepad cannot really do that, so the program treats it as a wrong reading and throws it away
 *   (see readPad at the bottom of the file).
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

uint16_t lastButtons = 0;                // The button state we saw last time
bool     lastPs      = false;            // Was the PS button pressed last time?

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Press the buttons on the gamepad."));
}

void loop() {
  // Read the "gamepad data": box 0x11, 9 numbers (readPad is at the bottom of the file and also checks for wrong readings)
  //   [0][1] buttons (16 bits)  [2]~[5] sticks  [6][7] L2, R2 triggers  [8] system buttons
  uint8_t d[9];
  if (!readPad(d)) {
    delay(100);                          // Could not read this time, try again in a moment
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // Join the two numbers into the 16-bit button state

  // Check the buttons one by one: which were "just pressed" and which were "just released"
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t mask = 1u << i;             // The "bit" of button i
    bool now    = buttons & mask;        // Is it pressed now?
    bool before = lastButtons & mask;    // Was it pressed last time?

    if (now && !before) {                // Just pressed
      Serial.print(F("Pressed: "));
      printButtonName(i);
      Serial.println();
    }
    if (!now && before) {                // Just released
      Serial.print(F("Released: "));
      printButtonName(i);
      Serial.println();
    }
  }

  // The PS button is not in those 16 bits. It is bit 0 of number 8 (system buttons)
  bool ps = d[8] & 0x01;
  if (ps && !lastPs) Serial.println(F("Pressed: PS button"));
  if (!ps && lastPs) Serial.println(F("Released: PS button"));
  lastPs = ps;

  lastButtons = buttons;                 // Remember this time, to compare next time
  delay(20);                             // Look every 0.02 seconds, so even quick presses are caught
}

// Print the name of button i (i = which bit)
void printButtonName(uint8_t i) {
  switch (i) {
    case 0:  Serial.print(F("SHARE button (called Create on PS5)")); break;
    case 1:  Serial.print(F("L3 (press the left stick down)")); break;
    case 2:  Serial.print(F("R3 (press the right stick down)")); break;
    case 3:  Serial.print(F("OPTIONS button")); break;
    case 4:  Serial.print(F("↑ D-pad up")); break;
    case 5:  Serial.print(F("→ D-pad right")); break;
    case 6:  Serial.print(F("↓ D-pad down")); break;
    case 7:  Serial.print(F("← D-pad left")); break;
    case 8:  Serial.print(F("L2")); break;
    case 9:  Serial.print(F("R2")); break;
    case 10: Serial.print(F("L1")); break;
    case 11: Serial.print(F("R1")); break;
    case 12: Serial.print(F("▲ Triangle")); break;
    case 13: Serial.print(F("● Circle")); break;
    case 14: Serial.print(F("✕ Cross")); break;
    case 15: Serial.print(F("■ Square")); break;
  }
}

// ============================================================
// Read the gamepad data and do a "sanity check" (the same in every example, no need to change it)
// ============================================================
// Why a sanity check?
//   When the receiver is very busy (for example, talking wirelessly to the gamepad), once in a while
//   it is not ready in time and gives the Arduino "the box it read last time" instead
//   (for example the status data in box 0x31).
//   Then the button and stick numbers are wrong, and a remote-control car could start moving by itself.
// How can we tell?
//   On the D-pad you cannot press "up" and "down" at the same time,
//   and you cannot press "left" and "right" at the same time.
//   If we read that, this reading is wrong: throw it away and keep using the last good one.

uint8_t lastGood[9] = {0};               // The last good gamepad data (all 0 at the start = nothing pressed)

// Read box 0x11 (gamepad data, 9 numbers) into d. Returns false if the receiver cannot be read
bool readPad(uint8_t *d) {
  if (!mbcRead(0x11, d, 9)) return false;
  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];                     // The 16-bit button state
  bool upDown    = (buttons & (1u << 4)) && (buttons & (1u << 6));   // Up (bit 4) and down (bit 6) pressed together?
  bool leftRight = (buttons & (1u << 7)) && (buttons & (1u << 5));   // Left (bit 7) and right (bit 5) pressed together?
  if (upDown || leftRight) {
    memcpy(d, lastGood, 9);              // Wrong reading -> throw it away, use the last good one
  } else {
    memcpy(lastGood, d, 9);              // Good reading -> remember it, in case the next one is wrong
  }
  return true;
}

// ============================================================
// Below are the "Communication helpers": they talk to the receiver.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this, do not delete it)
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWriteBytes(uint8_t reg, const uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(buf, n);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
