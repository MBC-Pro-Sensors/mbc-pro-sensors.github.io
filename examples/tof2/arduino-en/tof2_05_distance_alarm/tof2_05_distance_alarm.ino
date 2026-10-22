/*
 * Ranger 2-Way × Arduino   Example 5: Parking sensor alarm
 * ------------------------------------------------------------
 * What does this example do?
 *   Uses the board's built-in LED (L) and a buzzer (optional) as a parking sensor:
 *   reads "03 nearest distance cm". Nothing there = quiet. Something there = blink / beep,
 *   faster the nearer it gets. Within STOP_CM it stays on and keeps beeping.
 *
 * You need: Ranger 2-Way (universal I2C edition); the buzzer is optional (if you have one, use an "active buzzer").
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   Buzzer + → 8 (ESP32 → 27)   Buzzer − → GND
 *
 * How to see the result:
 *   Move your hand slowly toward the sensor. The L LED (and the buzzer) blink faster and faster; the Serial Monitor at 115200 also prints the distance.
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // On most ESP32 boards the built-in LED is on GPIO 2
#endif

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

#if defined(ESP32)
const int BUZZER = 27;                   // Buzzer pin (ESP32)
#else
const int BUZZER = 8;                    // Buzzer pin (UNO)
#endif

// ---- 👉 Parameters you can change ----
const int16_t ALARM_START_CM = 80;       // Start the alarm within this many cm
const int16_t STOP_CM        = 10;       // Within this many cm: stay on and keep beeping

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move your hand slowly toward the sensor and watch the L LED blink faster."));
}

void loop() {
  int16_t cm = mbcRead(ADDR, 0x03);      // 03 = the nearer distance of the two lasers (cm)
  if (cm == MBC_NONE) { delay(100); return; }

  if (millis() - lastPrint >= 500) {     // Print the distance every 0.5 s
    lastPrint = millis();
    if (cm >= 120) Serial.println(F("Nothing there"));
    else { Serial.print(F("Nearest object: ")); Serial.print(cm); Serial.println(F(" cm")); }
  }

  if (cm >= ALARM_START_CM) {            // Too far: quiet
    alarm(false);
    delay(50);
  } else if (cm <= STOP_CM) {            // Too near: stay on
    alarm(true);
    delay(50);
  } else {                               // In between: the nearer, the faster it blinks (gap 50 ~ 400 ms)
    int gap = map(cm, STOP_CM, ALARM_START_CM, 50, 400);
    alarm(true);  delay(gap / 2);
    alarm(false); delay(gap / 2);
  }
}

void alarm(bool on) {
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  digitalWrite(BUZZER, on ? HIGH : LOW);
}

// ============================================================
// Below are the "Communication helpers" (MBC I2C protocol v2): they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
//   How to read: write [number] → wait 2 ms → read 2×k bytes = k int16 values in a row (little-endian) from that number
//   If a number does not exist (empty slot), the sensor returns -32768 (MBC_NONE)
// ============================================================

// Start I2C (call once in setup)
// UNO/Nano: SDA=A4, SCL=A5; UNO R4: SDA/SCL pins; ESP32: default GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, Arduino will not freeze
#endif
}

// From the sensor at addr, read k values (1~16) in a row starting at number id, into out. Returns true if all were read
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // At most 16 values at a time (32 bytes, the Wire buffer limit)
  Wire.beginTransmission(addr);
  Wire.write(id);                        // Tell the sensor: I want to read starting from number id
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // Wait 2 ms before reading (a must for ESP32-based products)
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

// Send a command: write [id][parameters...]. p points to the parameters, n = how many bytes (leave them out if none). Returns true on success
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready (bit0 of status flags F3 = 1), at most ms milliseconds. Returns true when ready
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
