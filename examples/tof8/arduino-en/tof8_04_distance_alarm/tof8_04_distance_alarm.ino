/*
 * Ranger 8-Way x Arduino  Example 4: Parking sensor alarm light
 * ------------------------------------------------------------
 * What does this example do?
 *   It uses the small built-in light on the Arduino board (the one marked L) as an alarm,
 *   just like a car's parking sensor:
 *     Nothing within 120 cm in front -> light off
 *     Something there: the closer it is, the faster it blinks
 *     Very close (within 10 cm)      -> light stays on
 *   If any one of the 8 lasers sees something, it counts. Things coming close from the left or right also make it blink.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   You don't need to add a light. The light on the board is enough.
 *
 * How to see the result:
 *   After uploading, watch the L light on the Arduino board. Move your hand slowly closer and it blinks faster.
 *   You can also open the Serial Monitor (115200) to see the nearest distance.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // Some ESP32 boards don't say where the built-in light is, so use pin 2
#endif

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint8_t LED = LED_BUILTIN;         // The light on the board (pin 13 on the UNO)
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing there

// ---- 👉 You can change these settings ----
const uint16_t ALARM_START = 1000;       // How close before it starts blinking (mm): 1000 = 1 meter
const uint16_t ALWAYS_ON   = 100;        // How close before it stays on (mm): 100 = 10 cm

uint16_t nearest = NO_TARGET;            // How far the nearest thing is (mm)
bool ledOn = false;                      // Is the light on or off right now
unsigned long lastBlink = 0;             // When the light last switched
unsigned long lastRead = 0;              // When the sensor was last read
unsigned long lastPrint = 0;             // When we last printed to the computer

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // Set the light's pin to "output" so we can turn it on and off
  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then (that would look "touching" and blink like crazy), so wait
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand slowly closer and watch the L light blink faster."));
}

void loop() {
  // ---- 1. Read the nearest distance every 0.05 seconds ----
  if (millis() - lastRead >= 50) {
    lastRead = millis();
    uint8_t d[6];
    if (mbcRead(0x11, d, 6)) {                         // Box 0x11: direction and distance of the nearest / farthest thing
      nearest = ((uint16_t)d[1] << 8) | d[2];          // Numbers 1, 2 = nearest distance (mm)
    } else {
      nearest = NO_TARGET;                             // Could not read: treat it as nothing there, light off
    }
  }

  // ---- 2. Decide how the light works from the distance ----
  if (nearest >= ALARM_START) {
    ledOn = false;                                     // Too far or nothing there: light off
  } else if (nearest <= ALWAYS_ON) {
    ledOn = true;                                      // Very close: light stays on
  } else {
    // The closer it is, the shorter the wait: 100 mm -> switch every 0.05 s; 1000 mm -> switch every 0.5 s
    unsigned long interval = map(nearest, ALWAYS_ON, ALARM_START, 50, 500);
    if (millis() - lastBlink >= interval) {
      lastBlink = millis();
      ledOn = !ledOn;                                  // On becomes off, off becomes on
    }
  }
  digitalWrite(LED, ledOn ? HIGH : LOW);

  // ---- 3. Print the distance to the computer every 0.5 seconds ----
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    if (nearest >= NO_TARGET) {
      Serial.println(F("Nothing there"));
    } else {
      Serial.print(F("Nearest thing: "));
      Serial.print(nearest / 10);
      Serial.println(F(" cm"));
    }
  }
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
