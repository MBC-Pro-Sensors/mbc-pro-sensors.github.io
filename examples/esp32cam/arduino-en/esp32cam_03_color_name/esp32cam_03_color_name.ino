/*
 * Sharpshooter ESP32CAM x Arduino   Example 3: What color is this?
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads the color at sample point S1 (numbers 01~03, 3 values in one read) and says the color name:
 *     01 hue 0~100: red 0/100, yellow about 17, green about 33, cyan about 50, blue about 67, purple about 83;
 *        -1 = black/white/gray (saturation too low, no color)
 *     02 saturation 0~100: higher = more vivid
 *     03 brightness 0~100: higher = brighter (for black/white/gray it tells black / gray / white apart)
 *   It only prints when the color changes, so the screen stays clean.
 *
 * You need: Sharpshooter ESP32CAM (I2C firmware), Arduino UNO / Nano / UNO R4 or ESP32.
 *
 * Wiring:
 *   Sharpshooter 5V  -> 5V       Sharpshooter GND -> GND
 *   Sharpshooter IO3 (SDA) -> A4   Sharpshooter IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   * Hardware confirmed; the I2C firmware bench test is still pending. IO1/IO3 are shared with the EV3 / SPIKE serial editions, so only one firmware at a time.
 *
 * Before you start:
 *   On the screen on the back of the Sharpshooter, set up sample point S1 and the color to track (the settings are stored on the device).
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Hold paper of different colors in front of the sample point.
 *
 * Tip:
 *   This example is for the "School edition" firmware (numbers 01~0A). For the WRO edition (HUE12), see Example 6.
 *   You can adjust the limits (for example yellow 12~23) to the lighting of your field.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t ADDR = 0x4C;               // The Sharpshooter's "house number" (I2C address). School and WRO editions are both 0x4C
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

int16_t lastHue = -99, lastSat = -1, lastVal = -1;   // Last values (impossible numbers on purpose, so the first time always prints)

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("Sharpshooter not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Hold something in front of the sample point."));
}

void loop() {
  // Read numbers 01~03 (3 values) in one go: [0] hue [1] saturation [2] brightness
  int16_t c[3];
  if (!mbcReadMany(ADDR, 0x01, 3, c)) { delay(100); return; }

  // Only print when the change is big enough (hue differs by 3 or more, or saturation / brightness by 10 or more)
  if (abs(c[0] - lastHue) >= 3 || abs(c[1] - lastSat) >= 10 || abs(c[2] - lastVal) >= 10 || (c[0] < 0) != (lastHue < 0)) {
    lastHue = c[0]; lastSat = c[1]; lastVal = c[2];
    Serial.print(F("Color: "));
    if (c[0] < 0) {                      // Hue -1: no color, use brightness to tell black / gray / white apart
      if (c[2] < 30)      Serial.print(F("black"));
      else if (c[2] < 70) Serial.print(F("gray"));
      else                Serial.print(F("white"));
    } else {
      printHueName(c[0]);
    }
    Serial.print(F("   (hue ")); Serial.print(c[0]);
    Serial.print(F("  sat ")); Serial.print(c[1]);
    Serial.print(F("  bright ")); Serial.print(c[2]); Serial.println(F(")"));
  }
  delay(50);
}

// Turn hue 0~100 into a color name (red 0/100, yellow about 17, green about 33, cyan about 50, blue about 67, purple about 83)
void printHueName(int16_t hue) {
  if (hue < 0)        Serial.print(F("black/white/gray (no color)"));
  else if (hue < 6)   Serial.print(F("red"));
  else if (hue < 12)  Serial.print(F("orange"));
  else if (hue < 23)  Serial.print(F("yellow"));
  else if (hue < 42)  Serial.print(F("green"));
  else if (hue < 58)  Serial.print(F("cyan"));
  else if (hue < 75)  Serial.print(F("blue"));
  else if (hue < 92)  Serial.print(F("purple"));
  else                Serial.print(F("red"));
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
