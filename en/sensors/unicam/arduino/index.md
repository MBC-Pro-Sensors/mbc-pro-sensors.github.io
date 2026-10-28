<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Reading it on Arduino (I2C)

> [!WARNING]
> **v1 not yet hardware-verified**: the value layout is final and passes the computer-side tests, but real-hardware numbers (fps, line-following thresholds, I2C pins) may change after bench testing.

The Sharpshooter Vision Universal v1 (Universal I2C edition) uses the MBC universal I2C protocol v2, so Arduino UNO, Nano, UNO R4 and ESP32 can all read it at address **`0x4C`**. Over I2C every level gives 16 colors / 16 points, values are pixels sent as they are, and there are no grid cells to split. Every slot is in the [value reference: I2C](/en/sensors/unicam/reference.md#host-i2c), full programs in the [Arduino examples](/en/sensors/unicam/arduino-examples.md).

## Wiring

| Camera | Arduino UNO / Nano | UNO R4, ESP32 |
| :--- | :--- | :--- |
| 5V | 5V | 5V |
| GND | GND | GND |
| SDA | A4 | the pin marked SDA |
| SCL | A5 | the pin marked SCL |

> [!CAUTION]
> **I2C pins are provisional, pending hardware tests**: inside the camera I2C uses IO1 / IO3 of the ESP32-CAM (the same pair as the LEGO serial port), currently **SDA = IO3, SCL = IO1**; this is confirmed only after testing with an Arduino. The I2C edition and the SPIKE / EV3 / MATRIX editions are different firmware and cannot be used at the same time.

## How to read it

1. Write **1 byte: the value number** (e.g. `0x10`).
2. **Wait 2 ms**.
3. Read **2×k bytes**: k consecutive values from that number on (k ≤ 16), each an **int16, low byte first**. Split reads of more than 16 values.
4. Undefined numbers return **−32768**.

| Number | Content |
| :--- | :--- |
| `0x01` | Camera mode number (camera mode × 10 + sub-option, e.g. 13 = Color tracking ③ Standard) |
| `0x02` | Sub-option |
| `0x03` | Number of taught colors |
| `0x04` | How many this level gives (colors / points / blocks) |
| `0x10` on | "Value 1" in the I2C table of the value reference = `0x10`, "value 2" = `0x11` … (Color tracking ④ uses `0x10`~`0x70`) |
| `0xF0` | Device ID = `0x4C` \| 3<<8 (high byte 3 = Universal; School 1, WRO 2) |
| `0xF1`~`0xF5` | Protocol version 2, firmware version 100 (1.0), status flags, update counter, uptime seconds |

| Command | Content |
| :--- | :--- |
| `E0 [camera mode, sub-option]` | Switch camera mode (saved in the camera, still set after power-off) |
| `E1 [point 1~20, X 0~159, Y 0~119]` | Move a sample point for color at points (saved in the camera) |
| `F8 [new address, inverted new address]` | Change the I2C address (shared command, see the [universal I2C protocol](/en/i2c-protocol.md)) |
| `EF [A5]` | Reboot |

Target colors **can only be taught on the camera**; v1 has no command to write colors.

## Smallest working program

Switch to **Color tracking ③ Standard** and read color 1's X, Y, area and count (values 1~4 = `0x10`~`0x13`):

```cpp
#include <Wire.h>
const uint8_t CAM = 0x4C;                    // factory address

// Read k consecutive values from number id (k <= 16). false = no answer
bool camRead(uint8_t id, uint8_t k, int16_t *out) {
  Wire.beginTransmission(CAM);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);
  if (Wire.requestFrom(CAM, (uint8_t)(2 * k)) != 2 * k) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read(), hi = Wire.read();
    out[i] = (int16_t)(lo | (hi << 8));     // low byte first
  }
  return true;
}

void camMode(uint8_t mode, uint8_t sub) {    // command E0: switch camera mode
  Wire.beginTransmission(CAM);
  Wire.write(0xE0);
  Wire.write(mode);
  Wire.write(sub);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  camMode(1, 3);                             // Color tracking ③ Standard
  delay(100);
}

void loop() {
  int16_t v[4];
  if (camRead(0x10, 4, v)) {                 // color 1: X, Y, area %, count
    if (v[0]) {
      Serial.print("color 1  X="); Serial.print(v[0]);
      Serial.print("  Y="); Serial.print(v[1]);
      Serial.print("  area="); Serial.print(v[2]);
      Serial.print("%  count="); Serial.println(v[3]);
    } else {
      Serial.println("color 1 not seen");
    }
  } else {
    Serial.println("camera not found, check wiring and address");
  }
  delay(100);
}
```

Color 2 of the same level starts at `0x14` (4 values per color); numbers for the other camera modes are in the [value reference: I2C](/en/sensors/unicam/reference.md#host-i2c).

## Slot tables and examples

- [Value reference: I2C](/en/sensors/unicam/reference.md#host-i2c) (value 1 = `0x10`)
- [Arduino example programs](/en/sensors/unicam/arduino-examples.md)
- [MBC universal I2C protocol](/en/i2c-protocol.md): shared registers, changing the address
- On micro:bit: [Reading it on micro:bit](/en/sensors/unicam/microbit/index.md)

## FAQ

- **"camera not found" all the time?** Check SDA / SCL are not swapped and GND is shared; the v1 pins are not final yet, so [contact us](/en/contact.md) if it still fails.
- **Reading −32768?** That number is not defined in the current camera mode; read `0x01` to check the mode first.
