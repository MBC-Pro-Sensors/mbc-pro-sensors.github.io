/*
 * Sharpshooter ESP32CAM x Arduino   Example 4: Where is the color blob?
 * ------------------------------------------------------------
 * What does this example do?
 *   Reads the position and size of the color blob (numbers 05~0A, 6 values in one read):
 *     05 X 0~100 (left = 0, right = 100), 06 Y 0~75 (bottom = 0, top = 75)
 *     07 width, 08 height, 09 area (width x height), 0A found (1 = the target color is in the image)
 *   Every 0.2 s it says whether the blob is "left / center / right", "top / middle / bottom", and how big it is.
 *   When nothing is found, X, Y, width, height and area are all 0.
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
 *   After uploading, open the Serial Monitor and choose 115200 baud. Move something of the target color left/right, up/down, near/far in front of the camera.
 *
 * Tip:
 *   This example is for the "School edition" firmware (numbers 01~0A). For the WRO edition (HUE12), see Example 6.
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
  Serial.println(F("Ready! Hold something of the target color in front of the camera."));
}

void loop() {
  if (millis() - lastPrint >= 200) {     // 👉 You can change this: print every 0.2 s
    lastPrint = millis();

    // Read numbers 05~0A (6 values) in one go: [0] X [1] Y [2] width [3] height [4] area [5] found
    int16_t b[6];
    if (!mbcReadMany(ADDR, 0x05, 6, b)) { Serial.println(F("Cannot read the Sharpshooter")); return; }

    if (!b[5]) {                         // 0A = 0: the target color is not in the image
      Serial.println(F("No blob found"));
      return;
    }

    // X: 0~100 split into left / center / right; Y: 0~75 split into bottom / middle / top
    Serial.print(F("Blob is at "));
    if (b[0] < 35)      Serial.print(F("left-"));
    else if (b[0] > 65) Serial.print(F("right-"));
    else                Serial.print(F("center-"));
    if (b[1] < 25)      Serial.print(F("bottom"));
    else if (b[1] > 50) Serial.print(F("top"));
    else                Serial.print(F("middle"));

    Serial.print(F("  X=")); Serial.print(b[0]);
    Serial.print(F(" Y=")); Serial.print(b[1]);
    Serial.print(F("  W ")); Serial.print(b[2]);
    Serial.print(F(" H ")); Serial.print(b[3]);
    Serial.print(F(" area ")); Serial.print(b[4]);
    if (b[4] > 2000)     Serial.print(F(" (very near / very big)"));
    else if (b[4] < 100) Serial.print(F(" (very far / very small)"));
    Serial.println();
  }
  delay(50);
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
