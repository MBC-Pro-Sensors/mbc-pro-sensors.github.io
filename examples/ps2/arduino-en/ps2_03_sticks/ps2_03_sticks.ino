/*
 * Controller PS2 x Arduino  Example 3: How far is the stick pushed?
 * ------------------------------------------------------------
 * What does this example do?
 *   Turns the position of the two sticks into numbers and prints them on the computer:
 *     let go (in the middle)  = 0
 *     push right or up        = positive, all the way = +127
 *     push left or down       = negative, all the way = -127
 *   It also prints the two triggers, L2 and R2: not pressed = 0, pressed = 255
 *   (if the gamepad supports "pressure sensing", it gives 0 ~ 255 depending on how hard you press).
 *
 * You need:
 *   Controller PS2 receiver (Universal I2C edition) + a PS2 2.4G wireless gamepad.
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud. Push the sticks slowly and watch the numbers.
 *
 * Good to know:
 *   1. Sticks always 0? Press the ANALOG button in the middle of the gamepad so its light turns on (analog mode).
 *   2. When the stick moves a little near the middle, the number stays at 0. The receiver does this on purpose
 *      (it is called a "dead zone"), so a car does not slowly drive off by itself when you let go of the stick.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

bool    connected = false;               // Is the gamepad connected? (updated every 0.4 seconds)
uint8_t type      = 0;                   // Gamepad mode: 1 = digital mode, 2 = analog mode
unsigned long lastStatus = 0;            // The last time we checked "is the gamepad connected?"

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Push the sticks."));
}

void loop() {
  // ---- 1. Every 0.4 seconds, check if the gamepad is connected: box 0x31 (status data) ----
  //   Don't read it every loop: switching between two boxes all the time makes it easier for the receiver to mix up the data
  if (millis() - lastStatus >= 400) {
    lastStatus = millis();
    uint8_t st[9];
    if (mbcRead(0x31, st, 9)) {
      connected = st[0] & 0x02;          // Bit 1 of number 0: 1 = gamepad connected
      type      = st[2];                 // 1 = digital mode, 2 = analog mode
    } else {
      connected = false;                 // Cannot read the receiver: count it as not connected
    }
  }

  if (!connected) {
    Serial.println(F("Gamepad not connected (please switch the gamepad on)"));
    delay(1000);
    return;
  }
  if (type == 1) {
    Serial.println(F("The gamepad is in digital mode, the sticks do nothing! Press the ANALOG button in the middle of the gamepad (its light turns on)"));
    delay(1000);
    return;
  }

  // ---- 2. Read the "gamepad data": box 0x11, 9 numbers ----
  //   readPad is at the bottom of the file and also checks for wrong readings
  uint8_t d[9];
  if (!readPad(d)) {
    delay(200);
    return;
  }

  // Stick numbers can be positive or negative, so we use int8_t (a box that holds -128 ~ 127)
  int lx = (int8_t)d[2];                 // Left stick left/right: right = positive
  int ly = (int8_t)d[3];                 // Left stick up/down: up = positive
  int rx = (int8_t)d[4];                 // Right stick left/right: right = positive
  int ry = (int8_t)d[5];                 // Right stick up/down: up = positive
  int l2 = d[6];                         // L2 trigger: 0 ~ 255
  int r2 = d[7];                         // R2 trigger: 0 ~ 255

  // Print in neat columns. %+4d = "with a + or - sign, 4 spaces wide", %3d = "3 spaces wide"
  char line[80];
  snprintf(line, sizeof(line), "Left stick X%+4d Y%+4d | Right stick X%+4d Y%+4d | L2 %3d R2 %3d",
           lx, ly, rx, ry, l2, r2);
  Serial.println(line);

  delay(200);                            // 👉 You can change this: print every 0.2 seconds, make it smaller to see faster
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
  delay(2);                              // Wait 2 ms for the receiver to get the data ready (the PS4 receiver needs this; on the PS2 receiver it does no harm)
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
