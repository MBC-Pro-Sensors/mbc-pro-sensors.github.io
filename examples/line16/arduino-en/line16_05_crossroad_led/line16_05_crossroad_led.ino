/*
 * Pathfinder 16-Way x Arduino  Example 5: Crossroad detector light
 * ------------------------------------------------------------
 * What does this example do?
 *   It uses the small built-in light on the Arduino board (the one marked L) as a signal light:
 *     Sees the line             -> light on
 *     Line is lost              -> light off, and the computer tells you which side the line was lost on
 *     Reaches a crossroad/fork  -> light blinks fast 3 times, and counts "which crossroad this is"
 *   Competitions often say "go to the 3rd crossroad and turn right". This example teaches you how to count crossroads.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   No extra light needed. Just use the light on the board.
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *   Take a sheet of paper with a cross drawn on it, and move the sensor slowly along the line and over the crossroad.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards don't say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address
const uint8_t LED = LED_BUILTIN;         // The light on the board (pin 13 on the UNO)

// 👉 You can change this: if this many of the 16 eyes (or more) see the line at once, it counts as a crossroad (the cross line covers many eyes)
const uint8_t CROSS_WIDTH = 10;

int crossCount = 0;                      // How many crossroads we have passed
bool onCross = false;                    // Are we on a crossroad right now?
bool lost = false;                       // Is the line lost right now?

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // Set the light's pin to "output" so we can turn it on and off
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  mbcWrite(0x10, 0);                     // Follow a black line (for a white line, change this to 1)
  Serial.println(F("Ready! Move the sensor along the line and over the crossroads."));
}

void loop() {
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {            // Read the line results (7 numbers)
    digitalWrite(LED, LOW);              // Can't read: turn the light off first
    delay(100);
    return;
  }

  uint8_t width    = d[2];               // How many eyes see the line at the same time (0 ~ 16)
  uint8_t groups   = d[3];               // How many line pieces it sees: 0 = no line, 1 = one line, 2 or more = a fork
  int8_t  lastSide = (int8_t)d[6];       // Which side's eyes the line touched last: negative = left, positive = right

  // ---- 1. No line ----
  if (groups == 0) {
    digitalWrite(LED, LOW);              // Light off
    if (!lost) {                         // The line was just lost: say it only once
      lost = true;
      if (lastSide < 0)      Serial.println(F("Line lost! It was last on the left → search left"));
      else if (lastSide > 0) Serial.println(F("Line lost! It was last on the right → search right"));
      else                   Serial.println(F("No line"));
    }
    onCross = false;
    delay(20);
    return;
  }
  lost = false;

  // ---- 2. Is it a crossroad? ----
  // 2 or more line pieces (a fork), or many eyes seeing the line at once (the cross line of a crossroad) = a crossroad
  bool isCross = (groups >= 2) || (width >= CROSS_WIDTH);

  if (isCross && !onCross) {             // Count only at the moment we enter the crossroad, so it is not counted twice
    onCross = true;
    crossCount = crossCount + 1;
    Serial.print(F("Crossroad "));
    Serial.print(crossCount);
    Serial.println(F("!"));
    for (int i = 0; i < 3; i++) {        // Blink fast 3 times
      digitalWrite(LED, LOW);  delay(80);
      digitalWrite(LED, HIGH); delay(80);
    }
  }
  if (!isCross) onCross = false;         // We left the crossroad, so the next one can be counted

  // ---- 3. Normal case: light on when we see the line ----
  digitalWrite(LED, HIGH);
  delay(20);
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
