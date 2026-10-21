/*
 * IMU Gyro x Arduino  Example 7 (advanced): Car turns 90 degrees in place
 * ------------------------------------------------------------
 * What does this example do?
 *   It makes a two-wheel car "turn right 90 degrees in place", stop for 1 second, turn 90 degrees again...
 *   After 4 turns (one full circle) it stops.
 *   While the car turns, it keeps checking the gyro angle:
 *     Still far from the target -> turn faster
 *     Almost there              -> turn slowly
 *     Arrived                   -> stop the car
 *   This way of "watching the sensor and correcting at the same time" is called "feedback control".
 *   If you turn only by "running the motors for some seconds", a weak battery or a slippery floor
 *   makes the turn wrong. With the gyro, it is much more accurate.
 *
 * You need:
 *   A two-wheel car, an L298N motor driver board (or a TB6612 or other driver wired the same way), batteries.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   (For ESP32 use: ENA -> 25, IN1 -> 26, IN2 -> 27, ENB -> 14, IN3 -> 32, IN4 -> 33)
 *   The L298N GND must be connected to the Arduino GND!
 *   Fix the gyro onto the car with screws or bricks (flat, standing up, or on its side are all OK, but it must not wobble).
 *
 * Before the first run:
 *   1. Run "Example 2" first and check that the angle number gets bigger when you turn right.
 *   2. Lift the car up (wheels off the ground) and upload. Check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE below to true.
 *   3. At power-on, keep the car still. It only starts after "Ready".
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's address

// ---- Motor pins ----
#if defined(ESP32)
// ESP32 pins 6 to 11 are connected to the memory chip on the board and cannot be used, so we use these pins
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 You can change these settings ----
const int   TURN_ANGLE = 90;             // Degrees for each turn: positive = turn right, negative = turn left
const int   TURN_TIMES = 4;              // How many turns in total (4 x 90 degrees = one full circle)
const int   MAX_SPEED  = 150;            // Speed when far from the target: 0 to 255
const int   MIN_SPEED  = 70;             // The slowest speed when almost there (too small and the wheels will not move)
const float KP         = 3.0;            // How much speed to add for each 1 degree left: change this if it does not stop in the right place
const int   TOLERANCE  = 2;              // Within 2 degrees counts as "arrived"
const bool  LEFT_REVERSE  = false;       // Change to true if the left wheel turns the wrong way
const bool  RIGHT_REVERSE = false;       // Change to true if the right wheel turns the wrong way

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);

  Serial.println(F("Do not move the car. Getting the gyro ready..."));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Sensor not found, or it keeps shaking. Run Example 1 first to check."));
    while (true) {}                      // The motors are already stopped, so just stop here
  }
  mbcWrite(0x10, 0x01);                  // Reset the heading: the direction the car faces now = 0 degrees
  delay(50);                             // Wait a little after the reset before reading

  Serial.println(F("Ready! Turning starts in 3 seconds."));
  delay(3000);

  long target = 0;                       // Target angle
  for (int i = 1; i <= TURN_TIMES; i++) {
    target = target + TURN_ANGLE;        // Turn 90 degrees more each time: 90, 180, 270, 360
    Serial.print(F("Turn "));
    Serial.print(i);
    Serial.print(F(": going to "));
    Serial.print(target);
    Serial.print(F(" degrees... "));

    if (!turnTo(target)) {               // The turn failed (the car is already stopped for safety)
      while (true) {}
    }
    delay(1000);                         // Stop for 1 second before the next turn
  }
  Serial.println(F("Done! Press the Arduino RESET button to run it again."));
}

void loop() {
  // Everything was done in setup, so nothing happens here (the car stays stopped)
}

// Turn in place to target degrees. Returns true if it worked; false if it could not get there (the car is stopped)
bool turnTo(long target) {
  unsigned long start = millis();
  while (true) {
    // ---- 1. Read the heading (box 0x11, use numbers 2 and 3 = Gyro3D) ----
    uint8_t d[6];
    if (!mbcRead(0x11, d, 6)) {          // Cannot read the sensor -> stop the car first, to be safe
      drive(0, 0);
      if (millis() - start > 5000) {
        Serial.println(F("Cannot read the gyro. Car stopped! Please check the wiring."));
        return false;
      }
      delay(20);
      continue;
    }
    long heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);   // Turn right = bigger

    // ---- 2. How many degrees are left? ----
    long error = target - heading;       // Positive = still need to turn right, negative = turned too far

    if (abs(error) <= TOLERANCE) {       // Arrived!
      drive(0, 0);
      delay(300);                        // Wait until the car has fully stopped
      mbcRead(0x11, d, 6);               // Check once more where it stopped
      heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);
      Serial.print(F("stopped at "));
      Serial.print(heading);
      Serial.println(F(" degrees"));
      return true;
    }

    // ---- 3. Safety checks ----
    if (abs(error) > abs(TURN_ANGLE) + 45) {   // Getting farther and farther -> wrong direction
      drive(0, 0);
      Serial.println(F("Getting farther away. Car stopped! A wheel may turn the wrong way: change LEFT_REVERSE / RIGHT_REVERSE."));
      return false;
    }
    if (millis() - start > 5000) {       // Still not there after 5 seconds
      drive(0, 0);
      Serial.println(F("Could not get there in 5 seconds. Car stopped! Check the batteries, or make MIN_SPEED bigger."));
      return false;
    }

    // ---- 4. The farther away, the faster it turns; slow down when almost there ----
    int speed = constrain((int)(KP * abs(error)), MIN_SPEED, MAX_SPEED);
    if (error > 0) drive(speed, -speed);    // Left wheel forward, right wheel backward -> turn right in place (angle gets bigger)
    else           drive(-speed, speed);    // Turned too far -> turn left in place to come back
    delay(10);
  }
}

// Control the two wheels: speed -255 (full speed backward) to 255 (full speed forward)
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
