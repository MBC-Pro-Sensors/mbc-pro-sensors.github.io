<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ∿ MBC Universal I2C Protocol (v1)

Every "Universal I2C Edition" sensor in the MBC-Pro family uses the same register rules: learn one and the rest work the same way. This page covers the shared parts; each product's data registers are on its own page.

Works with: **Arduino UNO / Nano / Mega / R4, ESP32, Raspberry Pi**, and any board that can act as an I2C master.

---

## 📋 Address Overview

| Product | I2C address | Model string (`0x07`) | Registers |
| :--- | :---: | :---: | :--- |
| Pathfinder 8-Way (Line8) | `0x16` | `LINE8` | [Line8 I2C guide](/en/sensors/line8/arduino-i2c.md) |
| Pathfinder 16-Way (Line16) | `0x16` | `LINE16` | [Line16 I2C guide](/en/sensors/line16/arduino-i2c.md) |
| Ranger 8-Way (TOF8) | `0x29` | `LASER8` | [TOF8 page](/en/sensors/tof8/index.md) |
| Controller PS2 | `0x42` | `PS2` | [PS2 page](/en/sensors/ps2/index.md) |
| Controller PS4/PS5 | `0x42` | `PS4` | [PS4/PS5 page](/en/sensors/ps4/index.md) |
| SPIKE 6-Way Expander (EXP6) | `0x6E` | `EXP6` | [EXP6 I2C guide](/en/sensors/exp6/arduino-i2c.md) |
| IMU Gyro Sensor | `0x6A` | `IMU` | [IMU page](/en/sensors/imu/index.md) |

> [!TIP]
> Products of the same family share one address (all line sensors use `0x16`); register `0x06` tells the variants apart. Read `0x01` once at startup to know exactly which product is connected.

---

## 🔌 Wiring & Voltage

- Connect **SCL, SDA, VCC, GND** as labeled on the sensor's header.
- **100 kHz** and **400 kHz** are supported.
- **Every product** connects directly to 5 V boards like the Arduino UNO — no extra level shifter needed.

---

## 📥 Reading Data (module → host)

1. The host writes 1 byte: the **register number**.
2. The host calls `requestFrom(address, n)`, n = 1~32.
3. The module always prepares 32 bytes: valid data first, the rest zero-padded. Read as many as you need; extra bytes are just zeros and never hang the bus.

- Multi-byte values are **big-endian (MSB first)**; signed values are two's complement.
- Undefined registers read back as all zeros.
- **PS4/PS5 receiver**: wait **at least 2 ms** after writing the register number before reading. Other products need no delay.

## 📤 Writing Commands (host → module)

- Format: `[register, value0, value1, …]`, multi-byte values big-endian.
- Too short = ignored; extra bytes are discarded. Only registers marked **W** or **RW** are writable.
- Commands run **asynchronously** (calibration, EEPROM writes, …). Check the status flag or counter to confirm completion instead of assuming it took effect immediately.

## 🔄 Data Consistency

The module refreshes a data snapshot every **10 ms** and each read copies a whole snapshot, so every value inside one bundle comes from the same moment — never half old, half new.

---

## 🗂️ Register Map

| Range | Purpose |
| :--- | :--- |
| `0x01` ~ `0x0F` | Device info (common to all products) |
| `0x10` ~ `0x1F` | Group 1: primary data |
| `0x20` ~ `0x2F` | Group 2: secondary data / calibration |
| `0x30` ~ `0x3F` | Group 3: system & status (first part common) |
| `0x40` and up | Product extensions: arrays, channels, ports |

Every group follows the same pattern:

- **`0xN0`** = the group's command register (write)
- **`0xN1`** = the group's **bundle**: `0xN2` onward concatenated, read in one go
- **`0xN2` onward** = individual items

## 🧾 Common Registers (every product)

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x01` | R | 17 | Version bundle (= `0x02` ~ `0x07`) |
| `0x02` | R | 1 | Device ID (= factory address, identifies the product) |
| `0x03` | R | 1 | Protocol version (currently 1) |
| `0x04` | R | 2 | Firmware version (major, minor) |
| `0x05` | R | 3 | Firmware build date (year % 100, month, day) |
| `0x06` | R | 2 | Hardware / variant (product-defined) |
| `0x07` | R | 8 | Model string (ASCII, zero-padded, e.g. `LINE16`) |
| `0x30` | W | 1 | System command: `0x01` = reboot |
| `0x31` | R | 9 | Status bundle (= `0x32` ~ `0x37`) |
| `0x32` | R | 1 | Status flags: **bit0 = ready**; bits 1~7 product-defined |
| `0x33` | R | 1 | Update counter: +1 per snapshot; not changing = module busy |
| `0x34` ~ `0x36` | R | 1 | Product-defined |
| `0x37` | R | 4 | Uptime (ms); a smaller value means the module rebooted |

**Recommended host flow:** read `0x01` to identify the product → wait for `0x32` bit0 = 1 (ready) → start reading product data.

---

## 💻 Arduino Helper Functions

These work with every MBC Universal I2C product and are used in all product examples:

```cpp
#include <Wire.h>

// Read n bytes (n ≤ 32). Returns false if the device did not respond
bool iicRead(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                                   // needed for PS4/PS5; optional for other products
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// Write a 1-byte command / setting
void iicWrite(uint8_t addr, uint8_t reg, uint8_t v) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(v);
  Wire.endTransmission();
}

// Big-endian bytes -> numbers
int16_t  be16(const uint8_t *p)  { return (int16_t)((p[0] << 8) | p[1]); }
uint16_t beU16(const uint8_t *p) { return (uint16_t)((p[0] << 8) | p[1]); }
```

### Example: Identify the Connected Product

```cpp
void setup() {
  Serial.begin(115200);
  Wire.begin();

  uint8_t info[17];
  if (iicRead(0x16, 0x01, info, 17)) {        // 0x16 = line sensor family
    char model[9] = {0};
    memcpy(model, &info[9], 8);               // 0x07 model string = bundle bytes 9~16
    Serial.print("Model: ");    Serial.println(model);
    Serial.print("Firmware: "); Serial.print(info[2]); Serial.print('.'); Serial.println(info[3]);
  } else {
    Serial.println("Device not found - check wiring and address");
  }
}

void loop() {}
```

> [!NOTE]
> The Universal I2C Edition is a **different hardware version** from the SPIKE and EV3 editions — the connectors differ and are not interchangeable. Please confirm the edition before ordering, or [contact us](/en/contact.md).
