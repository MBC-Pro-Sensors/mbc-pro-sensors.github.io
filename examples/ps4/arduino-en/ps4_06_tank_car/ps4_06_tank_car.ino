/*
 * Controller PS4/PS5 x Arduino  Example 6 (advanced): Tank-style remote-control car
 * ------------------------------------------------------------
 * What does this example do?
 *   Drive a two-wheel car with the gamepad, the same way you drive a tank:
 *     left stick up/down  -> controls the left wheel (push up = forward, push down = backward)
 *     right stick up/down -> controls the right wheel
 *     push both up = go straight; one up and one down = turn on the spot.
 *   When both sticks are let go, you can also drive with the D-pad (↑ forward, ↓ backward, ← → turn on the spot).
 *   If the receiver cannot be read, or the gamepad disconnects, the car stops at once.
 *
 * You need:
 *   Controller PS4/PS5 receiver (Universal I2C edition) + a PS4 or PS5 gamepad (already paired),
 *   a two-wheel car, an L298N motor driver board (or a TB6612 or similar board wired the same way), batteries.
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *   L298N ENA -> 5     IN1 -> 7     IN2 -> 8      (left wheel)
 *   L298N ENB -> 6     IN3 -> 9     IN4 -> 10     (right wheel)
 *   The L298N GND must be connected to the Arduino GND!
 *   (ESP32 uses different pins, see "Motor pins" below)
 *
 * Before the first run:
 *   1. Lift the car up (wheels off the ground) before uploading. Push both sticks up and check that both wheels turn "forward".
 *      If a wheel turns the wrong way, change LEFT_REVERSE or RIGHT_REVERSE below to true.
 *   2. ⚠️ Safety test: with the car still lifted, hold the PS button on the gamepad for 10 seconds to switch it off.
 *      Check that the wheels stop. Then put the car on the floor and drive.
 *   3. If the receiver gets no gamepad data for 0.3 seconds, it sets all buttons and sticks to zero, so the car stops.
 *   4. To avoid wrong readings, the program asks "is the gamepad still connected?" only every 0.4 seconds,
 *      and reads only the gamepad data the rest of the time.
 *      Impossible data like "up and down together" or "left and right together" is thrown away, and the last good data is used.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

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
const int  MAX_SPEED  = 200;             // Speed with the stick pushed all the way: 0 ~ 255. Beginners: start slower
const int  DPAD_SPEED = 150;             // Speed when driving with the D-pad
const bool LEFT_REVERSE  = false;        // Left wheel turns the wrong way? Change to true
const bool RIGHT_REVERSE = false;        // Right wheel turns the wrong way? Change to true

// Where the D-pad buttons are in the 16-bit button number (which bit)
const uint8_t BTN_UP = 4, BTN_RIGHT = 5, BTN_DOWN = 6, BTN_LEFT = 7;

bool stopped = false;                    // Is the car stopped now because of a lost connection?
bool connected = false;                  // Is the gamepad connected? (updated every 0.4 seconds)
unsigned long lastStatus = 0;            // The last time we checked "is the gamepad connected?"
unsigned long lastPrint = 0;             // The last time we printed the speed

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // Stop first

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}                      // The car stays stopped
  }
  Serial.println(F("Ready! Drive with the two sticks."));
}

void loop() {
  // ---- 1. Every 0.4 seconds, check if the gamepad is connected (box 0x31: status data) ----
  //   Don't read it every loop: switching between two boxes all the time makes it easier for the receiver
  //   to mix up the data, and the car could move by itself. To drive, just keep reading box 0x11.
  if (millis() - lastStatus >= 400) {
    lastStatus = millis();
    uint8_t st[9];
    connected = mbcRead(0x31, st, 9) && (st[0] & 0x02);   // Bit 1 of number 0: 1 = gamepad connected
  }
  if (!connected) {
    safeStop();                          // Cannot read the receiver, or the gamepad is not connected -> stop at once
    return;
  }

  // ---- 2. Read the gamepad (box 0x11). readPad is at the bottom of the file and also checks for wrong readings ----
  uint8_t d[9];
  if (!readPad(d)) {
    safeStop();                          // Cannot read -> stop
    return;
  }
  if (stopped) {
    stopped = false;
    Serial.println(F("Gamepad connected, you can drive!"));
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // The 16-bit button state
  int ly = (int8_t)d[3];                 // Left stick up/down: up = positive (-127 ~ +127)
  int ry = (int8_t)d[5];                 // Right stick up/down: up = positive

  // The more you push the stick, the faster the wheel turns: -127 ~ +127 becomes -MAX_SPEED ~ +MAX_SPEED
  int left  = (long)ly * MAX_SPEED / 127;
  int right = (long)ry * MAX_SPEED / 127;

  // When both sticks are let go, look at the D-pad instead
  if (ly == 0 && ry == 0) {
    if      (buttons & (1u << BTN_UP))    { left =  DPAD_SPEED; right =  DPAD_SPEED; }  // Forward
    else if (buttons & (1u << BTN_DOWN))  { left = -DPAD_SPEED; right = -DPAD_SPEED; }  // Backward
    else if (buttons & (1u << BTN_LEFT))  { left = -DPAD_SPEED; right =  DPAD_SPEED; }  // Turn left on the spot
    else if (buttons & (1u << BTN_RIGHT)) { left =  DPAD_SPEED; right = -DPAD_SPEED; }  // Turn right on the spot
  }

  drive(left, right);

  if (millis() - lastPrint >= 300) {     // Print the speed every 0.3 seconds
    lastPrint = millis();
    Serial.print(F("Left wheel "));
    Serial.print(left);
    Serial.print(F("  Right wheel "));
    Serial.println(right);
  }

  delay(20);
}

// Stop the car, and say so only once, at the moment it stops
void safeStop() {
  drive(0, 0);
  if (!stopped) {
    stopped = true;
    Serial.println(F("Cannot read the gamepad -> STOP!"));
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
  speed = constrain(speed, -255, 255);   // Keep the speed inside the range
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
}

// ============================================================
// Read the gamepad data and do a "sanity check" (the same in every example, no need to change it)
// ============================================================
// Why a sanity check?
//   When the receiver is very busy (for example, talking wirelessly to the gamepad), once in a while
//   it is not ready in time and gives the Arduino "the box it read last time" instead
//   (for example the status data in box 0x31).
//   Then the button and stick numbers are wrong, and a remote-control car could start moving by itself.
// How can we tell?
//   On the D-pad you cannot press "up" and "down" at the same time,
//   and you cannot press "left" and "right" at the same time.
//   If we read that, this reading is wrong: throw it away and keep using the last good one.

uint8_t lastGood[9] = {0};               // The last good gamepad data (all 0 at the start = nothing pressed)

// Read box 0x11 (gamepad data, 9 numbers) into d. Returns false if the receiver cannot be read
bool readPad(uint8_t *d) {
  if (!mbcRead(0x11, d, 9)) return false;
  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];                     // The 16-bit button state
  bool upDown    = (buttons & (1u << 4)) && (buttons & (1u << 6));   // Up (bit 4) and down (bit 6) pressed together?
  bool leftRight = (buttons & (1u << 7)) && (buttons & (1u << 5));   // Left (bit 7) and right (bit 5) pressed together?
  if (upDown || leftRight) {
    memcpy(d, lastGood, 9);              // Wrong reading -> throw it away, use the last good one
  } else {
    memcpy(lastGood, d, 9);              // Good reading -> remember it, in case the next one is wrong
  }
  return true;
}

// ============================================================
// Below are the "Communication helpers": they talk to the receiver.
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
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this, do not delete it)
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

bool mbcWriteBytes(uint8_t reg, const uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(buf, n);
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
