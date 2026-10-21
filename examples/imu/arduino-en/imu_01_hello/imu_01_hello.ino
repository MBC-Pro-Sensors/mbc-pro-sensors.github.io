/*
 * IMU Gyro x Arduino  Example 1: Say hello to the sensor
 * ------------------------------------------------------------
 * What does this example do?
 *   It checks that the sensor is connected. If it is, the computer shows
 *   the sensor's name and version, and "which face is pointing up now".
 *   Run this example first every time you connect a new sensor.
 *
 * You need:
 *   Arduino UNO / Nano (or ESP32), IMU gyro (Universal I2C edition), 4 jumper wires.
 *
 * Wiring (Arduino UNO / Nano):
 *   Sensor VCC -> 5V      Sensor GND -> GND
 *   Sensor SDA -> A4      Sensor SCL -> A5
 *
 * How to see the result:
 *   After uploading, open the Serial Monitor and choose 115200 baud.
 *
 * Good to know:
 *   When the gyro powers on, it must stay "still" for about 0.5 seconds
 *   before it says "I am ready". So put the sensor flat on the table
 *   and do not touch it until you see the result.
 */

#include <Wire.h>                        // Arduino's built-in I2C tool, used to talk to the sensor

const uint8_t SENSOR_ADDR = 0x6A;        // The gyro's "address" (like a house number). It is always 0x6A

void setup() {
  Serial.begin(115200);                  // Open the line to the computer, speed 115200
  mbcBegin();                            // Get I2C ready (the helpers are at the bottom of this file)
  delay(500);                            // Give the sensor 0.5 seconds to power on

  Serial.println(F("=== IMU Gyro: Say hello to the sensor ==="));
  Serial.println(F("Put the sensor on the table and do not move it..."));

  // Wait for the sensor to say "I am ready", up to 5 seconds (it must stay still for about 0.5 s first)
  if (!mbcWaitReady(5000)) {
    uint8_t id;
    if (mbcRead(0x02, &id, 1) && id == 0x6A) {
      // The sensor is found, but it keeps thinking it is moving
      Serial.println(F("Sensor found, but it keeps feeling shaky and cannot get ready. Please check:"));
      Serial.println(F("  1. Is the sensor on the table? Holding it in your hand always shakes it a little."));
      Serial.println(F("  2. Is the table shaking? (A motor or fan running nearby can do this too)"));
      Serial.println(F("  3. After it is still, press the Arduino RESET button and try again."));
    } else {
      Serial.println(F("Sensor not found! Please check:"));
      Serial.println(F("  1. SDA to A4, SCL to A5. Are they swapped?"));
      Serial.println(F("  2. VCC to 5V, GND to GND. Are they plugged in firmly? Is the red light on the sensor blinking?"));
      Serial.println(F("  3. Is your sensor the Universal I2C edition? (The SPIKE and EV3 editions cannot connect to Arduino)"));
      Serial.println(F("  4. Still not working: unplug the sensor power and plug it back in, wait 3 seconds, then press the Arduino RESET button."));
    }
    while (true) {}                      // Stop here and do nothing more
  }

  // ---- 1. Read the "version info": box 0x01 of the sensor, 17 numbers ----
  uint8_t info[17];
  mbcRead(0x01, info, 17);

  // Numbers 9 to 16 are the sensor's name (letters)
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("Found it! Sensor name: "));
  Serial.println(name);                  // It should show IMU

  Serial.print(F("Firmware version: "));
  Serial.print(info[2]);                 // Major version
  Serial.print('.');
  Serial.println(info[3]);               // Minor version

  Serial.print(F("Firmware date: 20"));
  Serial.print(info[4]);                 // Year (last two digits)
  Serial.print('/');
  Serial.print(info[5]);                 // Month
  Serial.print('/');
  Serial.println(info[6]);               // Day

  Serial.print(F("Magnetometer: "));
  if (info[8] == 1) Serial.println(F("OK"));
  else              Serial.println(F("not detected"));

  // ---- 2. Read the "status": box 0x31, 9 numbers ----
  uint8_t st[9];
  mbcRead(0x31, st, 9);
  uint8_t flags = st[0];                 // A row of "switches"; each bit means one thing
  uint8_t upFace = st[4];                // Which face is pointing up now: 0 to 5

  Serial.print(F("Face pointing up now: "));
  printFace(upFace);
  Serial.println();

  if (flags & (1u << 3)) {               // Switch 3 = mounted too tilted
    Serial.println(F("Note: the sensor is mounted too tilted (more than about 45 degrees). The heading may be wrong. Please mount it straighter."));
  }

  Serial.println(F("Wiring is correct. You can start the other examples now!"));
}

void loop() {
  // This example only checks once at power-on, so loop does nothing
}

// Print the "face pointing up" number as words you can read
void printFace(uint8_t f) {
  switch (f) {
    case 0:  Serial.print(F("+X face")); break;
    case 1:  Serial.print(F("+Y face")); break;
    case 2:  Serial.print(F("+Z face")); break;
    case 3:  Serial.print(F("-X face")); break;
    case 4:  Serial.print(F("-Y face")); break;
    case 5:  Serial.print(F("-Z face")); break;
    default: Serial.print(F("unknown")); break;
  }
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
// (The gyro must stay still for about 0.5 s before it is ready, so the examples wait 5 seconds)
bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
