/*
 * Ranger 2-Way x Arduino  Example 5: Which way did it slip away?
 * ------------------------------------------------------------
 * What does this example do?
 *   This is the Ranger 2-Way's best trick: telling whether a thing left "to the left" or "to the right".
 *   The idea is simple: when a thing moves away to the right, the left laser loses it "first",
 *   and only the right laser still sees it. When the right one loses it too, we know it went right.
 *     Only the right one sees it at the end -> it left to the right ->
 *     Only the left one sees it at the end  -> it left to the left  <-
 *   When a sumo robot's opponent runs away, the robot knows which way to turn and chase.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   You don't need to add a light. The light on the board is enough (it turns on when something is seen).
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Put a box 20 cm straight in front of the sensor (both lasers must see it),
 *   then move the box away to the left or to the right. The computer tells you which way it went.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards don't say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint8_t LED = LED_BUILTIN;         // The light on the board (pin 13 on the UNO)
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

// ---- 👉 You can change these settings ----
const uint16_t SEE_DIST = 500;           // How close counts as "seeing" (mm): 500 = 50 cm, so a wall far behind is not taken as the target
const unsigned long GONE_MS = 150;       // How long (milliseconds) neither side sees it before it really counts as gone

// Who sees it now: 0 = nobody, 1 = only left, 2 = only right, 3 = both
uint8_t state = 0;
uint8_t lastOneSide = 0;                 // Before it vanished, which side "was the only one" seeing it: 0 = none (both lost it at once), 1 = left, 2 = right
bool present = false;                    // Is something in front right now
unsigned long goneSince = 0;             // Since when neither side sees it

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  // While the lasers start, distances are 0 (that would look like "both sides see it"), so always wait until ready
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Put a box straight in front, then move it away to the left or right."));
}

void loop() {
  // Box 0x11: left distance (2 numbers) + right distance (2 numbers)
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    delay(500);
    return;
  }
  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  // "Sees it" = not broken, and close enough
  bool seeL = (left  != FAULT) && (left  < SEE_DIST);
  bool seeR = (right != FAULT) && (right < SEE_DIST);

  // Turn what both sides see into one number: left sees it +1, right sees it +2
  uint8_t now = (seeL ? 1 : 0) + (seeR ? 2 : 0);

  // ---- 1. If the situation changed, print it ----
  if (now != state) {
    state = now;
    if (state == 3)      Serial.println(F("  L ● ● R   Both sides see it (straight ahead)"));
    else if (state == 1) Serial.println(F("  L ● ○ R   Only the left sees it"));
    else if (state == 2) Serial.println(F("  L ○ ● R   Only the right sees it"));
    else                 Serial.println(F("  L ○ ○ R   Neither side sees it"));
  }

  // ---- 2. Something is seen: remember "which side was the last one left" ----
  if (state != 0) {
    if (!present) {
      present = true;
      Serial.println(F("Found something!"));
    }
    if (state == 3) lastOneSide = 0;          // Both see it -> it has not started leaving
    else            lastOneSide = state;      // Only one side sees it -> remember which side
    goneSince = millis();                     // Still seen, so restart the timer
  }

  // ---- 3. Neither side has seen it for a while: tell which way it left ----
  if (state == 0 && present && millis() - goneSince >= GONE_MS) {
    present = false;
    if (lastOneSide == 1)      Serial.println(F("←←← It left to the LEFT! (the right laser lost it first)"));
    else if (lastOneSide == 2) Serial.println(F("→→→ It left to the RIGHT! (the left laser lost it first)"));
    else                       Serial.println(F("It vanished from both sides at once (maybe it moved back, or moved too fast)"));
    Serial.println();
  }

  digitalWrite(LED, present ? HIGH : LOW);    // Light on when something is there

  delay(30);                                  // Read fast, so we can catch "which side was first"
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
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

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
