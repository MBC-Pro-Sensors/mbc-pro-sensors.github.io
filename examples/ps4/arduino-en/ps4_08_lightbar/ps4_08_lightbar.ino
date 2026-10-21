/*
 * Controller PS4/PS5 x Arduino  Example 8: Change the color of the gamepad's light bar
 * ------------------------------------------------------------
 * What does this example do?
 *   The Arduino changes the color of the gamepad's light bar:
 *     press ✕ -> blue    press ● -> red    press ■ -> pink    press ▲ -> green
 *     press the PS button -> a random color
 *     slowly press the L2 trigger -> the light gets darker; all the way = off
 *   At a competition, the red team's and blue team's gamepads can shine different colors, easy to tell apart.
 *
 * You need:
 *   Controller PS4/PS5 receiver (Universal I2C edition) + a PS4 or PS5 gamepad (already paired).
 *
 * Wiring (Arduino UNO / Nano):
 *   Receiver VCC -> 5V              Receiver GND -> GND
 *   Receiver RX (= SDA) -> A4       Receiver TX (= SCL) -> A5
 *
 * How to see the result:
 *   Watch the light bar on the gamepad. The Serial Monitor (115200) prints the color.
 *
 * Good to know:
 *   A color is made from 3 numbers, "red, green, blue" (each 0 ~ 255), like mixing paint.
 *   The 3 numbers must be written into box 0x23 of the receiver all at once.
 *   It's OK to write them before the gamepad connects: the receiver remembers the color and uses it after the gamepad connects.
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // The Controller receiver's address

// Where the shape buttons are in the 16-bit button number (which bit)
const uint8_t BTN_TRIANGLE = 12, BTN_CIRCLE = 13, BTN_CROSS = 14, BTN_SQUARE = 15;

uint8_t baseColor[3] = {0, 0, 255};      // The chosen color (red, green, blue); blue at the start
uint8_t lastBright = 255;                // The brightness we sent last time
uint16_t lastButtons = 0;                // The button state last time
bool lastPs = false;                     // Was the PS button pressed last time?
unsigned long lastSend = 0;              // The last time we sent a color

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("Receiver not found. Run example 1 first to check the wiring."));
    while (true) {}
  }

  // Box 0x23 can be read and written: first read the color the receiver remembers
  uint8_t c[3];
  if (mbcRead(0x23, c, 3)) {
    Serial.print(F("Light bar color the receiver remembers now: "));
    printColor(c);
  }
  randomSeed(micros());                  // So the "random color" is different every time it powers on
  Serial.println(F("Ready! Press ✕ ● ■ ▲ or the PS button to change the color, press L2 to make it darker."));
  sendColor(255);                        // Send blue once first
}

void loop() {
  // Read the gamepad data (box 0x11, 9 numbers). readPad is at the bottom of the file and also checks for wrong readings
  uint8_t d[9];
  if (!readPad(d)) {
    delay(100);
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];
  uint16_t pressed = buttons & ~lastButtons;          // Buttons that were "just pressed"
  lastButtons = buttons;
  bool changed = false;                  // Did we change the color this time?

  if (pressed & (1u << BTN_CROSS))    { setBase(0, 0, 255);   changed = true; }   // ✕ blue
  if (pressed & (1u << BTN_CIRCLE))   { setBase(255, 0, 0);   changed = true; }   // ● red
  if (pressed & (1u << BTN_SQUARE))   { setBase(255, 0, 128); changed = true; }   // ■ pink
  if (pressed & (1u << BTN_TRIANGLE)) { setBase(0, 255, 0);   changed = true; }   // ▲ green

  bool ps = d[8] & 0x01;                 // PS button: bit 0 of number 8 (system buttons)
  if (ps && !lastPs) {                   // PS button just pressed -> random color
    setBase(random(256), random(256), random(256));
    changed = true;
  }
  lastPs = ps;

  // The deeper you press L2, the darker the light: 0 (not pressed) -> brightness 255, 255 (all the way) -> brightness 0
  uint8_t bright = 255 - d[6];

  if (changed) {
    sendColor(bright);
    Serial.print(F("New color: "));
    printColor(baseColor);
  } else if (abs((int)bright - (int)lastBright) >= 8 && millis() - lastSend >= 50) {
    sendColor(bright);                   // Only send when the brightness changed, and at most every 0.05 seconds, so we don't flood the line
  }

  delay(20);
}

// Remember the chosen color
void setBase(uint8_t r, uint8_t g, uint8_t b) {
  baseColor[0] = r;
  baseColor[1] = g;
  baseColor[2] = b;
}

// Make it darker by the brightness, then write the 3 numbers into box 0x23 all at once
void sendColor(uint8_t bright) {
  uint8_t rgb[3];
  for (uint8_t i = 0; i < 3; i++) {
    rgb[i] = (uint16_t)baseColor[i] * bright / 255;   // Change to uint16_t first, so the multiplication does not overflow
  }
  mbcWriteBytes(0x23, rgb, 3);
  lastBright = bright;
  lastSend = millis();
}

// Print "Red x  Green y  Blue z"
void printColor(const uint8_t *c) {
  Serial.print(F("Red "));
  Serial.print(c[0]);
  Serial.print(F("  Green "));
  Serial.print(c[1]);
  Serial.print(F("  Blue "));
  Serial.println(c[2]);
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
