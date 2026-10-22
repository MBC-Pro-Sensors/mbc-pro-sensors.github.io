/*
 * Sharpshooter ESP32CAM x Arduino   Example 5: Servo follows the color blob (advanced)
 * ------------------------------------------------------------
 * What does this example do?
 *   Mount the Sharpshooter on an SG90 servo. Wherever the color blob goes, the servo turns that way:
 *     read number 05 blob X (0~100) -> turn it into an angle and send it to the servo.
 *     If no blob is found (0A = 0), go back to 90 degrees (the middle).
 *   No library to install: UNO / Nano / UNO R4 send one pulse every 20 ms by themselves; ESP32 uses the built-in ledc to send a 50 Hz signal.
 *
 * You need: Sharpshooter ESP32CAM (I2C firmware), Arduino UNO / Nano / UNO R4 or ESP32. Also: 1 SG90 servo.
 *
 * Wiring:
 *   Sharpshooter 5V  -> 5V       Sharpshooter GND -> GND
 *   Sharpshooter IO3 (SDA) -> A4   Sharpshooter IO1 (SCL) -> A5      (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   * Hardware confirmed; the I2C firmware bench test is still pending. IO1/IO3 are shared with the EV3 / SPIKE serial editions, so only one firmware at a time.
 *   SG90 brown -> GND    red -> 5V    orange (signal) -> 9
 *   (On ESP32, connect the signal wire to GPIO 13)
 *
 * Before you start:
 *   On the screen on the back of the Sharpshooter, set up sample point S1 and the color to track (the settings are stored on the device).
 *   Fix the Sharpshooter on the servo arm, camera facing forward.
 *
 * How to see the result:
 *   Move something of the target color left and right; the servo follows it. The Serial Monitor (115200) prints the angle.
 *
 * Tip:
 *   This example is for the "School edition" firmware (numbers 01~0A). For the WRO edition (HUE12), see Example 6.
 *   If the servo turns the opposite way from the blob, change REVERSE to true.
 *   If the servo keeps jittering or the Arduino keeps restarting: USB power is not enough. Connect the red and brown wires to an external battery (4~6V), and its minus side to GND too.
 */

#include <Wire.h>

const uint8_t ADDR = 0x4C;               // The Sharpshooter's "house number" (I2C address). School and WRO editions are both 0x4C
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

#if defined(ESP32)
const int SERVO_PIN = 13;                // ESP32: signal wire on GPIO 13
#else
const int SERVO_PIN = 9;                 // UNO / Nano / R4: signal wire on pin 9
#endif

// ---- 👉 You can change these ----
const bool REVERSE   = false;            // Change to true if it turns the wrong way
const int  MIN_ANGLE = 20;               // Angle when the blob is at the far left
const int  MAX_ANGLE = 160;              // Angle when the blob is at the far right

int angle = 90;
unsigned long lastRead = 0;
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  servoBegin();                          // Get the servo ready (UNO sends pulses itself, ESP32 uses ledc)
  servoWrite(90);                        // Turn to the middle first
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("Sharpshooter not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Move something of the target color left and right."));
}

void loop() {
  // ---- 1. Every 0.1 s read the Sharpshooter: numbers 05~0A, 6 values: [0] X ... [5] found ----
  if (millis() - lastRead >= 100) {
    lastRead = millis();
    int16_t b[6];
    if (!mbcReadMany(ADDR, 0x05, 6, b) || !b[5]) {
      angle = 90;                        // Cannot read, or no blob found -> back to the middle
    } else {
      int x = constrain(b[0], 0, 100);
      angle = REVERSE ? map(x, 0, 100, MAX_ANGLE, MIN_ANGLE) : map(x, 0, 100, MIN_ANGLE, MAX_ANGLE);
    }
  }

  // ---- 2. Send the signal to the servo every pass (UNO must send a pulse every 20 ms, or the servo will not hold the angle) ----
  servoWrite(angle);

  if (millis() - lastPrint >= 300) {     // Print the angle every 0.3 s
    lastPrint = millis();
    Serial.print(F("Angle: ")); Serial.print(angle); Serial.println(F(" deg"));
  }
  delay(20);                             // Not 50 ms in this example: the servo pulse must come every 20 ms
}

// ---- Servo: two boards, two methods, same way to call ----
//   0 deg ~ 0.544 ms, 90 deg ~ 1.47 ms, 180 deg ~ 2.4 ms HIGH, once every 20 ms
#if defined(ESP32)
// ESP32 uses the built-in ledc to send 50 Hz (once every 20 ms) PWM; set it once and it keeps sending by itself
void servoBegin() {
  ledcAttach(SERVO_PIN, 50, 16);         // 50 Hz, 16-bit resolution (0~65535)
}
void servoWrite(int deg) {
  deg = constrain(deg, 0, 180);
  long us = map(deg, 0, 180, 544, 2400); // Angle -> microseconds
  ledcWrite(SERVO_PIN, (uint32_t)(us * 65535L / 20000L));   // 20000 microseconds = one period
}
#else
// UNO / Nano / R4 without a library: pull the pin HIGH for "some microseconds" then LOW, that is one pulse
void servoBegin() { pinMode(SERVO_PIN, OUTPUT); }
void servoWrite(int deg) {
  deg = constrain(deg, 0, 180);
  int us = map(deg, 0, 180, 544, 2400);  // Angle -> microseconds
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(us);
  digitalWrite(SERVO_PIN, LOW);
}
#endif

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
