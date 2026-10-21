/*
 * Pathfinder 8-Way x Arduino  Example 6 (advanced): Line-following car
 * ------------------------------------------------------------
 * What does this example do?
 *   It lets a two-wheel car follow a black line by itself. If the line is to the left, it steers left.
 *   If the line is to the right, it steers right. The further off, the harder it steers.
 *   This is called "proportional control". It is the most basic trick in line-following competitions.
 *   When the line is lost, the car turns on the spot toward the side where the line disappeared, to find it again.
 *
 * You need:
 *   A two-wheel car, an L298N motor driver board (or a TB6612 etc. wired the same way), and batteries.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   (For ESP32 use: ENA -> 25, IN1 -> 26, IN2 -> 27, ENB -> 14, IN3 -> 32, IN4 -> 33)
 *   The L298N GND must be connected to the Arduino GND!
 *   Mount the sensor in the middle of the car's front, about 0.5 ~ 1 cm above the floor.
 *
 * Before the first run:
 *   1. Run "Example 4: One-key calibration" first to calibrate the field.
 *   2. Lift the car up (wheels in the air) and upload. Check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE below to true.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // The Pathfinder sensor's address

// ---- Motor pins ----
#if defined(ESP32)
// ESP32 pins 6 ~ 11 are connected to the memory chip on the board and cannot be used, so we use these pins
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 You can change these settings ----
const int   BASE_SPEED = 120;            // Speed going straight: 0 ~ 255. Start slow, it is easier to tune
const float KP         = 1.0;            // Steering strength: make it bigger if the car can't get back to the line, smaller if it wobbles left and right
const int   SEARCH_SPEED = 100;          // Speed when turning on the spot to find the line
const bool  LEFT_REVERSE  = false;       // Change to true if the left wheel turns the wrong way
const bool  RIGHT_REVERSE = false;       // Change to true if the right wheel turns the wrong way

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(2000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  mbcWrite(0x10, 0);                     // Follow a black line (for a white line, change this to 1)

  Serial.println(F("Starting in 3 seconds!"));
  delay(3000);
}

void loop() {
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {            // Can't read the sensor -> stop the car, to be safe
    drive(0, 0);
    return;
  }

  int8_t  error  = (int8_t)d[1];         // How far off the line is: -100 (far left) ~ +100 (far right), 0 = right in the middle
  uint8_t groups = d[3];                 // 0 = no line

  if (groups == 0) {
    // Line lost: turn on the spot toward the side where the line disappeared, to find it again
    if (error < 0) drive(-SEARCH_SPEED, SEARCH_SPEED);   // Line on the left -> turn left on the spot
    else           drive(SEARCH_SPEED, -SEARCH_SPEED);   // Line on the right -> turn right on the spot
    return;
  }

  // Proportional control: the further off, the harder it steers
  int turn = KP * error;                 // Line on the right (positive) -> turn is positive -> left wheel faster, right wheel slower -> turns right
  drive(BASE_SPEED + turn, BASE_SPEED - turn);
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
