/*
 * Controller PS4/PS5 x Arduino  Example 7: Make the gamepad rumble
 * ------------------------------------------------------------
 * What does this example do?
 *   The Arduino makes the gamepad vibrate (rumble):
 *     slowly press the R2 trigger  -> the gamepad rumbles; the deeper you press, the stronger it rumbles
 *     hold ✕                       -> rumble at full power
 *     let go                       -> stops at once
 *   Later you can use it for "rumble once when the robot hits a wall" or "rumble when time is up".
 *
 * You need:
 *   Controller PS4/PS5 receiver (Universal I2C edition) + a PS4 or PS5 gamepad (already paired).
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *
 * How to see the result:
 *   Press R2 or hold ✕ and feel the gamepad rumble. The Serial Monitor (115200) prints the rumble power.
 *
 * Good to know:
 *   Write a power (0 ~ 255) into box 0x10 of the receiver and the gamepad rumbles for 0.2 seconds. Write 0 = stop.
 *   To keep it rumbling, write again every 0.1 seconds, like pressing a doorbell again and again.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

const uint8_t BTN_CROSS = 14;            // Where ✕ Cross is in the 16-bit button number

uint8_t lastPower = 0;                   // The rumble power last time
unsigned long lastSend  = 0;             // The last time we sent a rumble command
unsigned long lastPrint = 0;             // The last time we printed the power

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }
  Serial.println(F("Ready! Slowly press the R2 trigger, or hold the ✕ button."));
}

void loop() {
  // Read the gamepad data (box 0x11, 9 numbers). readPad is at the bottom of the file and also checks for wrong readings
  uint8_t d[9];
  if (!readPad(d)) {
    delay(100);
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // The 16-bit button state
  uint8_t  r2      = d[7];               // R2 trigger: 0 (not pressed) ~ 255 (all the way)

  uint8_t power = r2;                    // The deeper you press, the stronger the rumble
  if (buttons & (1u << BTN_CROSS)) power = 255;      // Hold ✕ -> full power

  if (power > 0) {
    // Rumble: write every 0.1 seconds (each write rumbles for 0.2 seconds, so they join up without a break)
    if (millis() - lastSend >= 100) {
      lastSend = millis();
      mbcWrite(0x10, power);             // Box 0x10 = rumble power
    }
  } else if (lastPower > 0) {
    mbcWrite(0x10, 0);                   // Just let go -> write 0, stop at once
    Serial.println(F("Rumble stopped"));
  }

  if (power > 0 && power != lastPower && millis() - lastPrint >= 300) {
    lastPrint = millis();
    Serial.print(F("Rumble power: "));
    Serial.println(power);
  }

  lastPower = power;
  delay(20);
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
