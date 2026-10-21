/*
 * Controller PS4/PS5 x Arduino  Example 5 (advanced): A servo that follows the stick
 * ------------------------------------------------------------
 * What does this example do?
 *   Push the left stick left and right, and the SG90 servo turns with it:
 *     all the way left = 0 degrees, let go (middle) = 90 degrees, all the way right = 180 degrees.
 *   You can use it for the gripper of a robot arm, or a robot that turns its head.
 *
 * You need:
 *   Controller PS4/PS5 receiver (Universal I2C edition) + a PS4 or PS5 gamepad (already paired), 1 SG90 servo.
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *   SG90 brown wire -> GND    red wire -> 5V    orange wire (signal) -> 9
 *   (On ESP32, connect the signal wire to GPIO 13)
 *
 * How to see the result:
 *   Push the left stick and watch the servo turn. The Serial Monitor (115200) prints the angle.
 *
 * Good to know:
 *   1. This example makes the servo signal by itself, without the Servo library,
 *      so it works on UNO, Nano, UNO R4 and ESP32 as it is.
 *   2. The servo keeps shaking, or the Arduino keeps restarting: USB does not give enough power.
 *      Connect the servo's red and brown wires to a battery (4~6V) instead. The battery's minus (-) must also go to Arduino GND.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

#if defined(ESP32)
const int SERVO_PIN = 13;                // ESP32: signal wire to GPIO 13
#else
const int SERVO_PIN = 9;                 // UNO / Nano / R4: signal wire to pin 9
#endif

// ---- 👉 You can change these settings ----
const int MIN_ANGLE = 0;                 // The angle when the stick is all the way left
const int MAX_ANGLE = 180;               // The angle when the stick is all the way right (a gripper only needs a little, so you can make it smaller)

int angle = 90;                          // The servo's angle now; start in the middle
unsigned long lastPulse = 0;             // The last time we sent a signal
unsigned long lastPrint = 0;             // The last time we printed the angle

void setup() {
  Serial.begin(115200);
  pinMode(SERVO_PIN, OUTPUT);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Push the left stick left and right, and the servo will follow."));
}

void loop() {
  // The servo needs a signal "every 20 milliseconds" to stay at its angle
  if (millis() - lastPulse < 20) return;
  lastPulse = millis();

  // Read the gamepad data (box 0x11, 9 numbers). readPad is at the bottom of the file and also checks for wrong readings
  uint8_t d[9];
  if (readPad(d)) {
    int lx = (int8_t)d[2];               // Left stick left/right: -127 (all the way left) ~ 0 (middle) ~ +127 (all the way right)
    angle = map(lx, -127, 127, MIN_ANGLE, MAX_ANGLE);   // Change it into an angle
  } else {
    angle = 90;                          // Cannot read the receiver -> go back to the middle, it is safer
  }
  // When the gamepad is not connected, the stick reads 0, so the servo also goes back to the middle

  servoPulse(angle);                     // Send one signal to the servo

  if (millis() - lastPrint >= 300) {     // Print the angle every 0.3 seconds
    lastPrint = millis();
    Serial.print(F("Angle: "));
    Serial.print(angle);
    Serial.println(F(" degrees"));
  }
}

// Send one pulse to the servo: how long the pin stays HIGH tells the servo which angle to go to
//   0 degrees ≈ 0.544 ms, 90 degrees ≈ 1.47 ms, 180 degrees ≈ 2.4 ms (the same as Arduino's Servo library)
void servoPulse(int deg) {
  deg = constrain(deg, 0, 180);
  int us = map(deg, 0, 180, 544, 2400);  // Angle -> microseconds (1 millisecond = 1000 microseconds)
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(us);
  digitalWrite(SERVO_PIN, LOW);
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
