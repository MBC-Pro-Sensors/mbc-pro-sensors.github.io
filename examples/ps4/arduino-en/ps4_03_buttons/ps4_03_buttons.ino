/*
 * Controller PS4/PS5 x Arduino   Example 3: Which button did you press?
 * ------------------------------------------------------------
 * What does this example do?
 *   Press any button on the gamepad and the computer tells you which one. It also
 *   tells you when you release it. You can press several buttons at the same time.
 *   The state of all 16 buttons is packed into one number (number 01, "16 buttons").
 *   Each button takes one "bit": pressed = 1, not pressed = 0, like 16 light switches in a row.
 *   Numbers 02~05 are the same buttons in "groups": D-pad, shape buttons, shoulder
 *   buttons, function buttons, 4 buttons each. Each group is 0~15; pressing several
 *   adds them up (for example up + right = 1 + 2 = 3).
 *
 * You need: Controller PS4/PS5 receiver (Universal I2C edition) + a PS4 or PS5 gamepad (already paired), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Press the PS button on the gamepad to connect it, then press buttons.
 *
 * Tip:
 *   The first time, you must pair: within 30 s after the receiver powers on (or after you press Reset),
 *   hold SHARE + PS on a PS4 gamepad, or Create + PS on a PS5 gamepad, until the light bar blinks fast.
 *   The PS button and the capture button are not in the 16 buttons; they are in number 0A system buttons (b0 PS, b1 capture).
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

uint16_t lastButtons = 0;                // The 16-button state we saw last time
int16_t  lastGroup[4] = {0, 0, 0, 0};    // The four groups 02~05 we saw last time

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Press the buttons on the gamepad."));
}

void loop() {
  // Read numbers 01~05 (5 values) in one go: [0] 16 buttons [1] D-pad [2] shape [3] shoulder [4] function
  int16_t v[5];
  if (!mbcReadMany(ADDR, 0x01, 5, v)) {
    delay(100);                          // Could not read this time, try again shortly
    return;
  }
  uint16_t buttons = (uint16_t)v[0];     // 16 buttons: PSX order (bit0 SELECT ... bit15 ■)

  // ---- 1. Check button by button: which were "just pressed" and which were "just released" ----
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t mask = 1u << i;             // The "bit" of button i
    bool now    = buttons & mask;        // Is it pressed now?
    bool before = lastButtons & mask;    // Was it pressed last time?
    if (now && !before) { Serial.print(F("Pressed: ")); printButtonName(i); Serial.println(); }
    if (!now && before) { Serial.print(F("Released: ")); printButtonName(i); Serial.println(); }
  }
  lastButtons = buttons;                 // Remember this state so we can compare next time

  // ---- 2. If any of the four group values changed, print one line (D-pad / shape / shoulder / function, each 0~15) ----
  bool changed = false;
  for (uint8_t i = 0; i < 4; i++) if (v[i + 1] != lastGroup[i]) changed = true;
  if (changed) {
    Serial.print(F("    Groups -> D-pad ")); Serial.print(v[1]);
    Serial.print(F("  Shape ")); Serial.print(v[2]);
    Serial.print(F("  Shoulder ")); Serial.print(v[3]);
    Serial.print(F("  Function ")); Serial.println(v[4]);
    for (uint8_t i = 0; i < 4; i++) lastGroup[i] = v[i + 1];
  }

  delay(50);                             // Check every 0.05 s, so even fast presses are caught
}

// Print the name of bit i in the 16-button value (PSX order)
void printButtonName(uint8_t i) {
  switch (i) {
    case 0:  Serial.print(F("SELECT (PS4: SHARE / PS5: Create)")); break;
    case 1:  Serial.print(F("L3 (press the left stick down)")); break;
    case 2:  Serial.print(F("R3 (press the right stick down)")); break;
    case 3:  Serial.print(F("START (PS4/PS5: OPTIONS)")); break;
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
