<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ∿ EXP6 Universal I2C Developer Guide

[🔙 Back to EXP6 overview](/en/sensors/exp6/index.md)

The EXP6 Universal I2C edition lets **Arduino, ESP32 and Raspberry Pi** drive 6 LEGO SPIKE motors and sensors directly: constant speed, run to angle, synchronized dual motors, read angle and speed, read color / ultrasonic / force sensors — all with I2C commands.

> [!IMPORTANT]
> For the shared read/write rules, status registers and Arduino helpers, see the [MBC Universal I2C Protocol](/en/i2c-protocol.md) first.

## 📡 Basics

| Item | Value |
| :--- | :--- |
| I2C address | `0x6E` |
| Model string `0x07` | `EXP6` |
| Variant `0x06` | `[6, battery cells]` (port count, 2 or 3 cells) |
| Safety | **Watchdog**: by default, if there is no I2C traffic for 500 ms, motors not running to an angle coast to a stop |

## 1️⃣ Overview

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x10` | W | 4 | Raw command `[L1, L2, L3, L4]` (same as the Pybricks edition; `[0,0,0,0]` = stop all and reset) |
| `0x11` | R | 8 | Overview bundle (= `0x12` ~ `0x13`) |
| `0x12` | R | 6 | Device on each of the 6 ports: `0` none, `1` force, `3` color, `4` ultrasonic, `5` motor, `7` other |
| `0x13` | R | 2 | Battery voltage uint16, mV |

## 2️⃣ Dual-Motor Sync & Settings

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x20` | W | 4~5 | Dual-motor sync `[left port 1~6, right port 1~6, left speed -100~100, right speed -100~100, stop mode (optional)]`; both speeds 0 = stop |
| `0x22` | RW | 1 | Watchdog timeout in 100 ms (`0` = off, default `5` = 500 ms). Increase or disable it if your sketch uses long `delay()` calls |

## 3️⃣ Status Flags `0x32`

| Bit / register | Meaning |
| :---: | :--- |
| bit1 | A motor is running |
| bit2 | The watchdog stopped the motors (cleared by the next command) |
| `0x34` | Commands waiting in the queue |
| `0x35` | Queue overflow count (commands dropped because they arrived too fast) |

## 🔌 Per-Port Control (ports 1~6)

Each port has its own register block, **base B = `0x40` + `0x10` × (port − 1)**: port 1 = `0x40`, port 2 = `0x50`, …, port 6 = `0x90`.

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| B+`0x00` | W | 1~5 | Port command `[command, params…]` (see below) |
| B+`0x01` | R | 8 | Port bundle (= B+`0x02` ~ B+`0x05`) |
| B+`0x02` | R | 1 | Device type (same as `0x12`) |
| B+`0x03` | R | 4 | Motor angle int32, degrees |
| B+`0x04` | R | 2 | Motor speed int16, deg/s |
| B+`0x05` | R | 1 | Motor state: low 4 bits = mode; bit4 running to angle, bit5 mounted reversed, bit6 motor ready |
| B+`0x06` | R | 4 | Sensor raw value int32 (color sensor = color, reflection, R, G, B) |

### Port Commands

| Command | Params | Action |
| :---: | :--- | :--- |
| `0x01` | — | Coast |
| `0x02` | — | Brake |
| `0x03` | — | Hold current angle |
| `0x10` | speed -100 ~ 100 | Constant speed (closed-loop PID) |
| `0x11` | power -100 ~ 100, acceleration 0 ~ 100 (optional) | Open-loop PWM |
| `0x20` | speed 1 ~ 100, angle int16 -2499 ~ 2499, stop mode | Run relative angle |
| `0x21` | speed 1 ~ 100, angle 0 ~ 359, stop mode | Run to absolute angle (shortest path) |
| `0x30` | — | Reset angle to zero |
| `0x31` | `0` normal / `1` reversed | Set mounting direction (resets angle) |
| `0x32` | kp, ki, kd (0 ~ 100, 50 = factory) | PID multipliers |

**Stop mode:** `0` coast, `1` brake, `2` hold, `3` keep running. A new angle command is ignored while a motor is still running to an angle; any other command takes over immediately.

---

## 💻 Arduino Example

```cpp
#include <Wire.h>

const uint8_t EXP6 = 0x6E;

void sendCmd(const uint8_t *bytes, uint8_t n) {
  Wire.beginTransmission(EXP6);
  Wire.write(bytes, n);
  Wire.endTransmission();
}

bool iicRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(EXP6);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(EXP6, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // This demo uses delay(2000), so turn the watchdog off first
  // (in real programs keep it on and talk to EXP6 regularly instead)
  uint8_t wd[] = { 0x22, 0 };
  sendCmd(wd, 2);

  // Run the motor on port 1 at 50% constant speed
  uint8_t run[] = { 0x40, 0x10, 50 };
  sendCmd(run, 3);
  delay(2000);

  // Turn port 1 forward 360 degrees, then brake
  uint8_t deg[] = { 0x40, 0x20, 40, 0x01, 0x68, 1 };   // 360 = 0x0168
  sendCmd(deg, 6);
}

void loop() {
  uint8_t d[4];
  if (iicRead(0x43, d, 4)) {                         // port 1 angle
    int32_t angle = ((int32_t)d[0] << 24) | ((int32_t)d[1] << 16) | (d[2] << 8) | d[3];
    Serial.println(angle);
  }
  delay(100);                                        // regular traffic also keeps the watchdog happy
}
```

> [!WARNING]
> **EXP6 I2C wiring is the reverse of other MBC products**: the former TX pin = SDA, RX pin = SCL. Follow the board labels; if swapped, the host will not find the device.
