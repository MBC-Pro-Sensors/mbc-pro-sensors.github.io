/*
 * Pathfinder 8-Way × Arduino   Example 6 (advanced): Line-following car
 * ------------------------------------------------------------
 * What does this example do?
 *   Makes a two-wheel car follow a black line by itself. "02 fine position" (-100 ~ +100) is the error:
 *   line to the left → steer left, line to the right → steer right, further off → steer more (proportional control).
 *   When the line is lost (04 segment count = 0), look at "06 last lost direction" and turn in place that way to find it again.
 *
 * You need:
 *   A two-wheel car, an L298N motor driver board (or a TB6612 etc. wired the same way), a battery.
 *
 * Wiring:
 *   Sensor VCC → 5V      Sensor GND → GND
 *   Sensor SDA → A4      Sensor SCL → A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      (left wheel)
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     (right wheel)
 *   (ESP32 instead: ENA → 25, IN1 → 26, IN2 → 27, ENB → 14, IN3 → 32, IN4 → 33)
 *   The L298N GND must be connected to the Arduino GND!
 *   Mount the sensor at the front center of the car, about 0.5~1 cm above the floor.
 *
 * Before the first run:
 *   1. Run "Example 4: One-key calibration" to calibrate the field.
 *   2. Lift the car (wheels in the air), upload, and check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE to true.
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // The sensor's house number (I2C address)
const int16_t MBC_NONE = -32768;         // Code for "no such value / cannot read"

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- Motor pins ----
#if defined(ESP32)
// On ESP32, pins 6 ~ 11 are connected to the flash memory chip and cannot be used, so we use these instead
const int ENA = 25, IN1 = 26, IN2 = 27;  // Left wheel
const int ENB = 14, IN3 = 32, IN4 = 33;  // Right wheel
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // Left wheel
const int ENB = 6, IN3 = 9, IN4 = 10;    // Right wheel
#endif

// ---- 👉 Settings you can change ----
const int   BASE_SPEED   = 120;          // Straight-line speed: 0 ~ 255. Start slow, it is easier to tune
const float KP           = 1.0;          // Steering strength: increase if the car cannot get back on the line, decrease if it wobbles
const int   SEARCH_SPEED = 100;          // Speed for turning in place when searching for the line
const bool  LEFT_REVERSE  = false;       // Change to true if the left wheel turns the wrong way
const bool  RIGHT_REVERSE = false;       // Change to true if the right wheel turns the wrong way

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("Sensor not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  mbcWrite(ADDR, 0xE0);                  // E0 = follow a black line (for a white line change to 0xE1)

  Serial.println(F("Starting in 3 seconds!"));
  delay(3000);
}

void loop() {
  int16_t v[6];                          // 01 line position 02 fine position 03 line width 04 segment count 05 bitmap 06 last lost direction
  if (!mbcReadMany(ADDR, 0x01, 6, v)) {  // Cannot read the sensor → stop the car for safety
    drive(0, 0);
    delay(50);
    return;
  }

  int16_t error   = v[1];                // 02 fine position: -100 (far left) ~ +100 (far right), 0 = center
  int16_t groups  = v[3];                // 04 segment count: 0 = no line in sight
  int16_t lastDir = v[5];                // 06 last lost direction: negative = the line was last on the left

  if (groups == 0) {
    // Line lost: turn in place toward the side where the line disappeared, to find it again
    if (lastDir < 0) drive(-SEARCH_SPEED, SEARCH_SPEED);   // Line was on the left → turn left in place
    else             drive(SEARCH_SPEED, -SEARCH_SPEED);   // Line was on the right → turn right in place
    delay(50);
    return;
  }

  // Proportional control: the further off, the more we steer
  int turn = KP * error;                 // Line on the right (positive) → turn is positive → left wheel faster, right wheel slower → turn right
  drive(BASE_SPEED + turn, BASE_SPEED - turn);
  delay(50);
}

// Control both wheels: speed -255 (full reverse) ~ 255 (full forward)
void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);   // Keep the speed within range
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
