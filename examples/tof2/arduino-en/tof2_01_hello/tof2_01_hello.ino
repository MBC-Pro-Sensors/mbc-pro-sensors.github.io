/*
 * Ranger 2-Way x Arduino  Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   It checks that the sensor is connected. If it is, the computer shows
 *   the sensor's name and version, and checks that both lasers (left and right) work.
 *   Run this example first every time you connect a new sensor.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *   (Solder pads on the new board: 1 = SCL, 2 = SDA, 3 = VCC, 4 = GND)
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Good to know:
 *   When the sensor powers on, it first has to wake up its 2 lasers. During this time
 *   every distance reads 0, so always wait until it is "ready" first.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the sensor

const uint8_t SENSOR_ADDR = 0x29;        // The Ranger sensor's "address" (like a house number). It is always 0x29

void setup() {
  Serial.begin(115200);                  // Open the line to the computer, speed 115200
  mbcBegin();                            // Get I2C ready (the helpers are at the bottom of this file)
  delay(500);                            // Give the sensor 0.5 seconds to power on

  Serial.println(F("=== Ranger 2-Way: Say hello to the sensor ==="));

  // Wait for the sensor to say "I'm ready", up to 6 seconds
  if (!mbcWaitReady(6000)) {
    Serial.println(F("Sensor not found! Please check:"));
    Serial.println(F("  1. SDA goes to A4 and SCL goes to A5. Are they swapped? (New board pads: 1 = SCL, 2 = SDA)"));
    Serial.println(F("  2. VCC goes to 5V and GND goes to GND. Are they plugged in firmly?"));
    Serial.println(F("  3. Is your sensor the Universal I2C edition? (The SPIKE and EV3 editions do not work with Arduino.)"));
    Serial.println(F("  4. Is anything else with address 0x29 on the same SDA/SCL wires (for example a Ranger 8-Way, or a VL53L0X module from a shop)?"));
    while (true) {}                      // Not found: stop here
  }

  // Read the "version info": box 0x01 of the sensor, 17 numbers
  uint8_t info[17];
  if (!mbcRead(0x01, info, 17)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    while (true) {}
  }

  // Numbers 9 ~ 16 are the sensor's name (letters)
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("Found it! Sensor name: "));
  Serial.println(name);                  // Should print LASER2

  Serial.print(F("Firmware version: "));
  Serial.print(info[2]);                 // Major version
  Serial.print('.');
  Serial.println(info[3]);               // Minor version

  Serial.print(F("This sensor has "));
  Serial.print(info[7]);                 // Ranger 2-Way = 2
  Serial.println(F(" lasers"));

  Serial.print(F("Max range: "));
  Serial.print(info[8] * 10);            // This box stores "how many 10 mm". Times 10 = millimeters (1200)
  Serial.println(F(" mm (anything farther counts as 'nothing there')"));

  // Read the "health check": box 0x31, 9 numbers
  //   Number 0: status (bit 1 = is any laser broken)
  //   Number 2: bit 0 = left laser started OK, bit 1 = right laser started OK
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {
    Serial.println(F("Read failed. Check if a wire is loose."));
    while (true) {}
  }

  if (st[0] & 0x02) {                    // Bit 1 is 1 -> a laser is broken
    if (!(st[2] & (1u << 0))) Serial.println(F("Warning: the left laser did not start"));
    if (!(st[2] & (1u << 1))) Serial.println(F("Warning: the right laser did not start"));
    Serial.println(F("(Run example 2 to see which side keeps showing FAULT, then contact us.)"));
  } else {
    Serial.println(F("Both lasers (left and right) are OK!"));
  }

  Serial.println(F("Wiring is correct. You can try the other examples now!"));
}

void loop() {
  // This example only checks once at power-on, so loop does nothing
}

// ============================================================
// Below are the "Communication helpers": they talk to the sensor.
// They are the same in every MBC example. No need to change or understand them.
// ============================================================

// Start I2C communication (call once in setup)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // If a wire is loose, the Arduino will not freeze
#endif
}

// Read n numbers from box reg of the sensor into buf. Returns true if it worked
bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// Write the number value into box reg of the sensor. Returns true if it worked
bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

// Wait until the sensor is ready, up to timeoutMs milliseconds. Returns true when ready
bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
