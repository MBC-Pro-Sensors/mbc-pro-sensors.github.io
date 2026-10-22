/*
 * Ranger 2-Way × Arduino   Example 4 (advanced): Stop precisely with mm
 * ------------------------------------------------------------
 * What does this example do?
 *   Uses the three boxes "04 left mm", "05 right mm" and "08 average mm" (down to 1 mm)
 *   to drive a car forward and stop exactly STOP_MM (default 200 mm = 20 cm) from a wall.
 *   It slows down as it gets close, so it will not overshoot. You can also run it without
 *   a car: just watch the numbers in the Serial Monitor.
 *
 * You need: a two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), a battery, a wall or a box.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      (left wheel)
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     (right wheel)
 *   (ESP32 instead: ENA → 25, IN1 → 26, IN2 → 27, ENB → 14, IN3 → 32, IN4 → 33)
 *   The GND of the L298N must be connected to the GND of the Arduino! Mount the sensor at the front, lasers pointing forward.
 *
 * How to see the result:
 *   Serial Monitor at 115200. When the car stops, the distance printed should be very close to STOP_MM.
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- Motor pins ----
#if defined(ESP32)
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel (pins 6 ~ 11 cannot be used on ESP32)
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 Parameters you can change ----
const int16_t STOP_MM    = 200;          // Stop this many mm from the wall (200 = 20 cm)
const int16_t SLOW_MM    = 500;          // Start slowing down within this many mm of the wall
const int     FAST_SPEED = 150;          // Full speed 0~255
const int     SLOW_SPEED = 70;           // Slowest crawling speed (too slow and the motors will not move)
const bool    LEFT_REVERSE  = false;     // Change to true if the left wheel turns the wrong way
const bool    RIGHT_REVERSE = false;     // Change to true if the right wheel turns the wrong way

bool stopped = false;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.print(F("Starting in 3 s. Goal: stop at ")); Serial.print(STOP_MM); Serial.println(F(" mm"));
  delay(3000);
}

void loop() {
  int16_t v[5];                          // 04 left mm, 05 right mm, 06, 07, 08 average mm
  if (!mbcReadMany(ADDR, 0x04, 5, v)) {  // Read 5 boxes in a row starting at 04 (04~08)
    drive(0, 0);                         // Cannot read → stop the car for safety
    delay(50);
    return;
  }
  int16_t leftMm = v[0], rightMm = v[1], avgMm = v[4];
  int16_t dist = (leftMm < rightMm) ? leftMm : rightMm;   // Use the nearer laser, so it does not hit the wall at an angle

  Serial.print(F("L ")); Serial.print(leftMm);
  Serial.print(F(" mm  R ")); Serial.print(rightMm);
  Serial.print(F(" mm  avg ")); Serial.print(avgMm);
  Serial.print(F(" mm  "));

  if (stopped || dist <= STOP_MM) {
    drive(0, 0);
    if (!stopped) { stopped = true; Serial.println(F("→ Arrived, stop!")); }
    else Serial.println(F("(stopped)"));
  } else if (dist <= SLOW_MM) {
    // The nearer, the slower: from SLOW_MM to STOP_MM the speed goes from FAST down to SLOW
    int speed = map(dist, STOP_MM, SLOW_MM, SLOW_SPEED, FAST_SPEED);
    drive(speed, speed);
    Serial.print(F("→ Slowing down ")); Serial.println(speed);
  } else {
    drive(FAST_SPEED, FAST_SPEED);
    Serial.println(F("→ Full speed ahead"));
  }
  delay(50);
}

void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
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
