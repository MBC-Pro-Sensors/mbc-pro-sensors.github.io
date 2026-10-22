/*
 * Controller PS2 x Arduino   Example 6: Tank-style remote-control car (advanced)
 * ------------------------------------------------------------
 * What does this example do?
 *   Drives a two-wheel car with the gamepad, the same way a tank is driven:
 *     left stick up/down (LY) -> left wheel; right stick up/down (RY) -> right wheel
 *     both sticks up = go straight; one up and one down = turn on the spot.
 *   The stick value -100 ~ +100 is used directly as the motor speed (times the top speed).
 *   If the gamepad disconnects, or the receiver cannot be read, the car stops at once.
 *
 * You need: Controller PS2 receiver (Universal I2C edition) + PS2 gamepad, Arduino UNO / Nano / UNO R4 or ESP32. Also: a two-wheel car, an L298N motor driver board (or a TB6612 wired the same way), batteries.
 *
 * Wiring:
 *   Receiver VCC -> 5V      Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4      Receiver TX (= SCL) -> A5         (UNO / Nano)
 *   UNO R4: SDA / SCL pins; ESP32: GPIO 21 (SDA) / GPIO 22 (SCL)
 *   (On the I2C edition, the pin marked TX is SCL and RX is SDA)
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   The L298N GND must be connected to the Arduino GND!
 *   (ESP32 uses different pins, see "Motor pins" below)
 *
 * Before you start:
 *   1. Lift the car up (wheels in the air) and upload. Push both sticks up and check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE to true.
 *   2. ⚠️ While the car is still lifted, turn the gamepad off and check that the wheels stop. Only then put it on the floor.
 *
 * How to see the result:
 *   The Serial Monitor (115200) prints the left and right wheel speed every 0.3 s.
 *
 * Tip:
 *   The ANALOG button in the middle of the gamepad must have its light on (analog mode),
 *   otherwise the sticks give no numbers.
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // The receiver's "house number" (I2C address). PS2 and PS4 are both 0x42
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
const int  MAX_SPEED     = 200;          // Speed when the stick is pushed all the way: 0 ~ 255, start slow
const bool LEFT_REVERSE  = false;        // Change to true if the left wheel turns the wrong way
const bool RIGHT_REVERSE = false;        // Change to true if the right wheel turns the wrong way

bool stopped = true;                     // Are we stopped right now? (we start out stopped)
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("Receiver not found. Run Example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Drive with the two sticks."));
}

void loop() {
  // Read numbers 06 ~ 13 (14 values) in one go (data from the same instant):
  //   v[0] = 06 connected ... v[10] = 10 LX, v[11] = 11 LY, v[12] = 12 RX, v[13] = 13 RY
  //   (0C~0F in the middle are empty slots and read as -32768; just ignore them)
  int16_t v[14];
  if (!mbcReadMany(ADDR, 0x06, 14, v) || v[0] == 0) {
    safeStop();                          // Cannot read the receiver, or the gamepad is not connected -> stop at once
    delay(50);
    return;
  }
  if (stopped) {
    stopped = false;
    Serial.println(F("Gamepad connected, you can drive!"));
  }

  int ly = v[11];                        // Left stick up/down: up = positive (-100 ~ +100)
  int ry = v[13];                        // Right stick up/down: up = positive

  // The further the stick is pushed, the faster the wheel turns: -100 ~ +100 becomes -MAX_SPEED ~ +MAX_SPEED
  int left  = (long)ly * MAX_SPEED / 100;
  int right = (long)ry * MAX_SPEED / 100;
  drive(left, right);

  if (millis() - lastPrint >= 300) {     // Print the speeds every 0.3 s
    lastPrint = millis();
    Serial.print(F("Left ")); Serial.print(left);
    Serial.print(F("   Right ")); Serial.println(right);
  }
  delay(50);
}

// Stop the car, and say so only once, right when it stops
void safeStop() {
  drive(0, 0);
  if (!stopped) {
    stopped = true;
    Serial.println(F("Cannot read the gamepad -> stop!"));
  }
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
