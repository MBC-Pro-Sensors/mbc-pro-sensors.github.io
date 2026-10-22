/*
 * IMU Gyro x Arduino   Example 7: Car turns 90 degrees on the spot (advanced)
 * ------------------------------------------------------------
 * What does this example do?
 *   At power-on it sends command E0 to set the heading to zero. Then every 3 s it makes a
 *   two-wheel car turn right 90 degrees on the spot:
 *   number 02 (fused heading) tells "how far have I turned". Far from the target = turn fast,
 *   almost there = turn slow, arrived = stop. After 4 turns the car faces the original direction again.
 *   Seen from above, clockwise (turning right) makes the heading bigger, so for a right turn target = current + 90.
 *
 * You need: IMU Gyro (Universal I2C edition), Arduino UNO / Nano / UNO R4 or ESP32. Also: a two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), batteries.
 *
 * Wiring:
 *   Gyro VCC -> 5V      Gyro GND -> GND
 *   Gyro SDA -> A4      Gyro SCL -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   The L298N GND must be connected to the Arduino GND!
 *   (ESP32 uses different pins, see "Motor pins" below)
 *
 * Before you start:
 *   1. Mount the gyro flat and firmly on the car. Do not move the car at power-on (the gyro first learns which side is up).
 *   2. Test with the car lifted up: on a right turn the left wheel should go forward and the right wheel backward.
 *      If a wheel turns the wrong way, change LEFT_REVERSE / RIGHT_REVERSE.
 *   3. If it overshoots, make TURN_SPEED smaller or SLOW_ZONE bigger.
 *
 * How to see the result:
 *   The Serial Monitor (115200) prints the target angle and the current heading.
 *
 * Tip:
 *   To turn left: change TURN_ANGLE to -90.
 */

#include <Wire.h>

const uint8_t ADDR = 0x6A;               // The IMU Gyro's "house number" (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / could not read"

// Helper declaration (the code is at the bottom of the file); mbcWrite's parameters can be left out
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- Motor pins ----
#if defined(ESP32)
// ESP32 pins 6 ~ 11 are connected to the on-board memory chip and cannot be used, so we use these instead
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 You can change these ----
const int  TURN_ANGLE    = 90;           // Degrees per turn: positive = right, negative = left
const int  TURN_SPEED    = 150;          // Turning speed 0~255
const int  SLOW_SPEED    = 80;           // Slow speed when almost there
const int  SLOW_ZONE     = 25;           // Switch to slow speed when fewer than this many degrees remain
const int  DONE_ZONE     = 2;            // Within this many degrees counts as arrived
const unsigned long WAIT_MS = 3000;      // How many ms to wait after each turn
const bool LEFT_REVERSE  = false;        // Change to true if the left wheel turns the wrong way
const bool RIGHT_REVERSE = false;        // Change to true if the right wheel turns the wrong way

long target = 0;                         // The angle to turn to this time
unsigned long doneAt = 0;                // When the last turn finished
bool turning = false;                    // Are we turning right now?

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(1000);                           // Wait a little longer so the gyro can learn which side is up
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("Gyro not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }

  mbcWrite(ADDR, 0xE0);                  // Command E0: heading to zero (current direction = 0 degrees)
  delay(100);
  Serial.println(F("Heading set to zero. Turning 90 degrees on the spot every 3 s."));
  doneAt = millis();
}

void loop() {
  int16_t heading = mbcRead(ADDR, 0x02); // 02 fused heading (degrees), bigger when turning right
  if (heading == MBC_NONE) {             // Cannot read the gyro -> stop, to be safe
    drive(0, 0);
    Serial.println(F("Cannot read the gyro -> stop"));
    delay(50);
    return;
  }

  if (!turning) {
    // ---- Resting: when the time is up, set the next target ----
    if (millis() - doneAt >= WAIT_MS) {
      target += TURN_ANGLE;
      turning = true;
      Serial.print(F("Start turning, target ")); Serial.print(target); Serial.println(F(" deg"));
    }
  } else {
    // ---- Turning: how many degrees are left? ----
    long error = target - heading;       // Positive = still need to turn right, negative = turn left (or overshot)
    if (abs(error) <= DONE_ZONE) {       // Arrived
      drive(0, 0);
      turning = false;
      doneAt = millis();
      Serial.print(F("Arrived! Heading now ")); Serial.println(heading);
    } else {
      int spd = (abs(error) < SLOW_ZONE) ? SLOW_SPEED : TURN_SPEED;   // Slow down when almost there
      if (error > 0) drive(spd, -spd);   // Right turn: left wheel forward, right wheel backward
      else           drive(-spd, spd);   // Left turn
    }
  }
  delay(50);
}

// Control the two wheels: speed -255 (full speed backward) ~ 255 (full speed forward)
void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);   // Keep the speed inside the allowed range
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
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
