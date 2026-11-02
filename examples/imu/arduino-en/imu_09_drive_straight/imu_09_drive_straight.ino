/*
 * IMU Gyro x Arduino   Example 9: Car drives in a straight line (advanced)
 * ------------------------------------------------------------
 * What does this example do?
 *   At power-on it sends command E0 to set the heading to zero (the direction the car faces now = 0 degrees),
 *   then it keeps driving forward.
 *   Number 02 (fused heading) tells whether the car is drifting. If it drifts, the two wheels get different
 *   speeds to pull the nose back:
 *     correction fix = KP x heading (at most MAX_FIX)
 *     left = BASE_SPEED - fix     right = BASE_SPEED + fix
 *   Seen from above, clockwise (drifting right) makes the heading bigger -> fix is positive ->
 *   left wheel slows down, right wheel speeds up -> the nose turns back to the left.
 *   Drifting left makes the heading smaller (negative) -> fix is negative ->
 *   left wheel speeds up, right wheel slows down -> the nose turns back to the right.
 *   If the gyro cannot be read, the car stops at once.
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
 *   2. Test with the car lifted up: both wheels should turn forward.
 *      If a wheel turns the wrong way, change LEFT_REVERSE / RIGHT_REVERSE.
 *   3. If the car wiggles left and right (snaking), make KP smaller; if it cannot pull back, make KP bigger.
 *
 * How to see the result:
 *   The Serial Monitor (115200) prints every 0.2 s: heading, correction, left and right wheel speed.
 *   Turn the car's nose a little by hand: the correction gets bigger and the car turns itself back to 0 degrees.
 *
 * Tip:
 *   To use number 03 (gyro 1D heading) instead: change HEADING_ID to 0x03 (not affected by magnetic fields,
 *   more stable near motors or iron desks).
 *   To stop after a while: change RUN_MS to the number of milliseconds to drive (for example 5000 = stop after 5 s).
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
const int   BASE_SPEED   = 150;          // Basic forward speed 0~255
const float KP           = 4.0;          // How much speed to correct per degree of drift (bigger = stronger)
const int   MAX_FIX      = 80;           // Largest correction (so one wheel never stops or turns backward)
const uint8_t HEADING_ID = 0x02;         // Which heading to use: 0x02 fused heading, 0x03 gyro 1D heading
const unsigned long RUN_MS = 0;          // Stop after this many ms: 0 = drive forever
const bool  LEFT_REVERSE  = false;       // Change to true if the left wheel turns the wrong way
const bool  RIGHT_REVERSE = false;       // Change to true if the right wheel turns the wrong way

unsigned long startAt = 0;               // When the car started driving
unsigned long lastPrint = 0;             // When we last printed

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

  mbcWrite(ADDR, 0xE0);                  // Command E0: heading to zero (the direction the car faces now = 0 degrees)
  delay(100);
  Serial.println(F("Heading set to zero. Driving straight ahead."));
  Serial.println(F("Head    Fix  Left Right"));
  startAt = millis();
}

void loop() {
  // ---- 1. Stop after RUN_MS (RUN_MS = 0 drives forever) ----
  if (RUN_MS > 0 && millis() - startAt >= RUN_MS) {
    drive(0, 0);
    return;
  }

  // ---- 2. Read the heading: 0 = straight ahead, bigger when drifting right, smaller when drifting left ----
  int16_t heading = mbcRead(ADDR, HEADING_ID);
  if (heading == MBC_NONE) {             // Cannot read the gyro -> stop, to be safe
    drive(0, 0);
    Serial.println(F("Cannot read the gyro -> stop"));
    delay(50);
    return;
  }

  // ---- 3. Work out the correction: more drift = more correction, but never more than MAX_FIX ----
  int fix = (int)(KP * heading);
  fix = constrain(fix, -MAX_FIX, MAX_FIX);
  int left  = BASE_SPEED - fix;          // Drifting right (fix positive) -> left wheel a bit slower
  int right = BASE_SPEED + fix;          //                               -> right wheel a bit faster, nose turns back left
  drive(left, right);

  // ---- 4. Print every 0.2 s ----
  if (millis() - lastPrint >= 200) {
    lastPrint = millis();
    char line[40];
    snprintf(line, sizeof(line), "%4d  %6d  %4d  %4d", heading, fix, left, right);
    Serial.println(line);
  }
  delay(20);
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
