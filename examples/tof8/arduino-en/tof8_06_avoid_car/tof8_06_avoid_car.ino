/*
 * Ranger 8-Way x Arduino  Example 6 (advanced): Obstacle-avoiding car
 * ------------------------------------------------------------
 * What does this example do?
 *   It lets a two-wheel car drive around by itself without hitting walls or obstacles:
 *     Nothing in front       -> drive straight; if one side is too close to a wall, steer a little to the other side
 *     Something blocks front -> stop, turn on the spot toward the "more open side", and drive again when the front is clear
 *   The 8 lasers watch the left, the front and the right, so the car knows which way to turn.
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
 *   Mount the sensor in the middle of the car's front, facing forward: laser 0 on the car's left, laser 7 on the car's right.
 *
 * Before the first run:
 *   1. Run "Example 5: Radar bar chart" first. Put your hand on the car's left side and check
 *      that the top bars (lasers 0 ~ 2) get shorter.
 *   2. Lift the car up (wheels in the air) and upload. Check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE below to true.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's address
const uint16_t NO_TARGET = 1200;         // 1200 or more = nothing there
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = this laser is broken

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
const int      FORWARD_SPEED = 130;      // Speed going straight: 0 ~ 255. Start slow, it is safer
const int      TURN_SPEED    = 110;      // Speed when turning on the spot
const int      STEER         = 40;       // How hard to steer when a side is too close
const uint16_t FRONT_STOP    = 250;      // How close in front before turning (mm): 250 = 25 cm
const uint16_t SIDE_CLOSE    = 150;      // How close on a side counts as "too close" (mm): 150 = 15 cm
const bool     LEFT_REVERSE  = false;    // Change to true if the left wheel turns the wrong way
const bool     RIGHT_REVERSE = false;    // Change to true if the right wheel turns the wrong way

uint16_t dist[8];                        // Distances of the 8 lasers (mm)

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);

  // The 8 lasers need 2 ~ 4 seconds to start. Distances are 0 until then. Without waiting, the car would think there are walls all around
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found. Run example 1 first to check the wiring."));
    while (true) {}                      // Sensor not found: don't drive (wheels stay stopped)
  }

  Serial.println(F("Starting in 3 seconds!"));
  delay(3000);
}

void loop() {
  // Box 0x21: read the distances of all 8 lasers at once
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {           // Can't read the sensor -> stop the car to be safe
    drive(0, 0);
    delay(50);
    return;
  }

  bool allZero = true;                   // Are all 8 distances 0?
  for (uint8_t i = 0; i < 8; i++) {
    dist[i] = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];
    if (dist[i] != 0) allZero = false;
    if (dist[i] == FAULT) dist[i] = 0;   // A broken laser: treat it as "something is blocking", which is safer
  }
  if (allZero) {                         // All 0: the sensor may have just restarted (for example, low battery) and is not ready yet
    drive(0, 0);                         // -> stop the car and wait until it is ready
    delay(50);
    return;
  }

  // Split the 8 lasers into left, front and right groups. Take the "nearest one" of each group
  uint16_t left  = smallest(0, 2);       // Lasers 0 ~ 2: left and front-left
  uint16_t front = smallest(3, 4);       // Lasers 3, 4: straight ahead
  uint16_t right = smallest(5, 7);       // Lasers 5 ~ 7: front-right and right

  if (front > FRONT_STOP) {
    // ---- Nothing in front: drive straight, steer a little away if a side is too close ----
    int turn = 0;                        // Positive = steer left, negative = steer right
    if (left < SIDE_CLOSE)  turn = turn - STEER;   // Left too close -> steer right
    if (right < SIDE_CLOSE) turn = turn + STEER;   // Right too close -> steer left
    drive(FORWARD_SPEED - turn, FORWARD_SPEED + turn);
  } else {
    // ---- Something in front: turn on the spot toward the more open side ----
    if (left > right) drive(-TURN_SPEED, TURN_SPEED);   // Left is more open -> turn left on the spot
    else              drive(TURN_SPEED, -TURN_SPEED);   // Right is more open -> turn right on the spot
  }

  delay(50);                             // The sensor updates about every 0.1 seconds, so checking every 0.05 seconds is enough
}

// From laser number from to laser number to, find the nearest distance
uint16_t smallest(uint8_t from, uint8_t to) {
  uint16_t m = NO_TARGET;
  for (uint8_t i = from; i <= to; i++) {
    if (dist[i] < m) m = dist[i];
  }
  return m;
}

// Control the two wheels: speed -255 (full speed back) ~ 255 (full speed forward)
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
