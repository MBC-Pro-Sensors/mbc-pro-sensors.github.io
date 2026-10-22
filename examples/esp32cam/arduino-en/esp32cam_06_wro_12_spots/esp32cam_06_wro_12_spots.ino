/*
 * Sharpshooter ESP32CAM x Arduino   Example 6: WRO edition: all 12 color spots at once
 * ------------------------------------------------------------
 * What does this example do?
 *   This one is for the "WRO edition (HUE12)" firmware: the 12 color spots S1~S12 on the image are printed as 12 rows every 0.5 s.
 *     10~1B: S1~S12 color codes (12 values in one read)
 *        0~239 color (the hue circle squeezed into 0~239), 240~245 white, 246~250 black, 255 = this spot is off
 *     20~2B: S1~S12 vividness 0~255 (12 values in one read; black = 1, off = 0)
 *   Each row prints the color name + color code + vividness.
 *
 * You need: Sharpshooter ESP32CAM (WRO edition HUE12 I2C firmware), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Sharpshooter 5V  -> 5V       Sharpshooter GND -> GND
 *   Sharpshooter IO3 (SDA) -> A4   Sharpshooter IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   * Hardware confirmed; the I2C firmware bench test is still pending. IO1/IO3 are shared with the EV3 / SPIKE serial editions, so only one firmware at a time.
 *
 * Before you start:
 *   On the screen on the back of the Sharpshooter, turn on the color spots S1~S12 you need and aim them at the field.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Tip:
 *   On School edition firmware, 10~2B all read as -32768 (empty slots); run Examples 2~5 instead.
 *   Color code 255 only means "this spot is off"; a spot that is on but not aimed at anything still returns its current color.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x4C;               // The Sharpshooter's "house number" (I2C address). School and WRO editions are both 0x4C
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("Sharpshooter not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }

  // Check that this is the WRO edition (high byte of F0 = 2); the School edition also runs, but reads empty slots
  int16_t id = mbcRead(ADDR, 0xF0);
  if (((id >> 8) & 0xFF) != 2) Serial.println(F("Note: this is not WRO edition firmware, 10~2B will be empty slots (-32768)."));
  Serial.println(F("Ready! Printing the 12 color spots every 0.5 s."));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 You can change this: print every 0.5 s
    lastPrint = millis();

    int16_t code[12], vivid[12];
    if (!mbcReadMany(ADDR, 0x10, 12, code))  { Serial.println(F("Cannot read (color codes)")); return; }   // 10~1B
    if (!mbcReadMany(ADDR, 0x20, 12, vivid)) { Serial.println(F("Cannot read (vividness)")); return; }   // 20~2B

    Serial.println(F("---- S1 ~ S12 ----"));
    for (uint8_t i = 0; i < 12; i++) {
      Serial.print(F("S")); Serial.print(i + 1); Serial.print(i < 9 ? F("  ") : F(" "));
      printCodeName(code[i]);
      Serial.print(F("  code ")); Serial.print(code[i]);
      Serial.print(F("  vivid ")); Serial.println(vivid[i]);
    }
  }
  delay(50);
}

// Color code -> name: 0~239 color (treat 0~239 as the hue circle 0~360 degrees), 240~245 white, 246~250 black, 255 off
void printCodeName(int16_t c) {
  if (c == MBC_NONE)   { Serial.print(F("(empty) ")); return; }
  if (c == 255)        { Serial.print(F("off     ")); return; }
  if (c >= 246)        { Serial.print(F("black   ")); return; }
  if (c >= 240)        { Serial.print(F("white   ")); return; }
  int deg = (int)((long)c * 360 / 240); // 0~239 -> 0~359 degrees
  if (deg < 15)        Serial.print(F("red     "));
  else if (deg < 45)   Serial.print(F("orange  "));
  else if (deg < 75)   Serial.print(F("yellow  "));
  else if (deg < 165)  Serial.print(F("green   "));
  else if (deg < 200)  Serial.print(F("cyan    "));
  else if (deg < 265)  Serial.print(F("blue    "));
  else if (deg < 330)  Serial.print(F("purple  "));
  else                 Serial.print(F("red     "));
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
