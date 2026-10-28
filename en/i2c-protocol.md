<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ∿ MBC Universal I2C Protocol v2

Every "Universal I2C" MBC-Pro sensor follows the same rule: **the register number is the value number**. Write one number, read back one or several 16-bit integers. Learn it once and every product works the same way; each product's value table is in its [command reference](/en/reference.md).

Hosts: **Arduino UNO / Nano / Mega / R4, ESP32, micro:bit, Raspberry Pi** and any board that can act as an I2C master.

> [!TIP]
> **No coding experience?** Every product has copy-and-upload Arduino examples; start with [🔰 First time with Arduino](/en/sensors/arduino-start.md). For micro:bit use the [MakeCode extension](/en/microbit.md).

> [!IMPORTANT]
> This is the **v2 protocol (firmware from October 2026, `F1` reads 2)**. Firmware before October 2026 used v1 (0x01 version block, 0x11 combo packets, big-endian); old programs are not compatible. Legacy files are kept in the legacy section of [downloads](/en/downloads.md).

---

## 📋 Address Overview

<!-- gen:i2c-addr-table -->
| Product | I2C address | hw[0] | Model | Value table | Reference | Examples |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Vision Sensor (School Edition) (legacy, no longer supported) | `0x4C` | `1` | `CAMSCH` | [I2C](/en/sensors/esp32cam/arduino-i2c.md) | [Reference](/en/sensors/esp32cam/reference.md) | [Arduino](/en/sensors/esp32cam/arduino-examples.md) |
| Vision Sensor (WRO Edition, HUE12) (legacy, no longer supported) | `0x4C` | `2` | `HUE12` | [I2C](/en/sensors/esp32cam/arduino-i2c-wro.md) | [Reference](/en/sensors/esp32cam/reference-wro.md) | [Arduino](/en/sensors/esp32cam/arduino-examples.md) |
| IMU Gyro Sensor | `0x6A` | `1` | `IMU` | [I2C](/en/sensors/imu/arduino-i2c.md) | [Reference](/en/sensors/imu/reference.md) | [Arduino](/en/sensors/imu/arduino-examples.md) |
| Line Follower 16-ch | `0x16` | `16` | `LINE16` | [I2C](/en/sensors/line16/arduino-i2c.md) | [Reference](/en/sensors/line16/reference.md) | [Arduino](/en/sensors/line16/arduino-examples.md) |
| Line Follower 8-ch | `0x16` | `8` | `LINE8` | [I2C](/en/sensors/line8/arduino-i2c.md) | [Reference](/en/sensors/line8/reference.md) | [Arduino](/en/sensors/line8/arduino-examples.md) |
| PS2 Gamepad Receiver | `0x42` | `1` | `PS2` | [I2C](/en/sensors/ps2/arduino-i2c.md) | [Reference](/en/sensors/ps2/reference.md) | [Arduino](/en/sensors/ps2/arduino-examples.md) |
| Gamepad Receiver PS4/PS5 | `0x42` | `2` | `PS4` | [I2C](/en/sensors/ps4/arduino-i2c.md) | [Reference](/en/sensors/ps4/reference.md) | [Arduino](/en/sensors/ps4/arduino-examples.md) |
| Laser Ranger 2-ch | `0x29` | `2` | `LASER2` | [I2C](/en/sensors/tof2/arduino-i2c.md) | [Reference](/en/sensors/tof2/reference.md) | [Arduino](/en/sensors/tof2/arduino-examples.md) |
| Laser Ranger 8-ch | `0x29` | `8` | `LASER8` | [I2C](/en/sensors/tof8/arduino-i2c.md) | [Reference](/en/sensors/tof8/reference.md) | [Arduino](/en/sensors/tof8/arduino-examples.md) |
<!-- /gen -->

> [!NOTE]
> - **The current [Sharpshooter Vision](/en/sensors/unicam/arduino/index.md)** is also `0x4C`, hw0 = `3`; for which value sits in which slot see the [Sharpshooter Vision value reference](/en/sensors/unicam/reference.md). The School / WRO editions marked "legacy" above are no longer supported.
> - **The SPIKE 6-Port Expander (EXP6) does not use I2C**: its hub connector is isolated, so Arduino talks to it over UART. See [EXP6 Arduino examples](/en/sensors/exp6/arduino-examples.md).
> - Products with the **same address cannot share one SDA/SCL bus** (Line8 + Line16, TOF2 + TOF8, PS2 + PS4, the current and the legacy Sharpshooter Vision). Re-address one of them with `F8` first.
> - Products of one family share an address; tell the variant apart with the high byte of `F0` (hw0): Line 8/16, TOF 2/8, PS2 = 1 / PS4 = 2, Sharpshooter Vision = 3 (legacy school = 1 / WRO = 2).

---

## 🔌 Wiring and Voltage

- Connect **SCL, SDA, VCC, GND** as printed on the header (on the 4-pin SPIKE/EV3 style cable: TX pin = SCL, RX pin = SDA).
- **100 kHz** (recommended, Arduino default) or 400 kHz.
- STM32 products (line followers, rangers, PS2, IMU) are 5 V tolerant; **ESP32 products (PS4/PS5 receiver, camera) are not 5 V tolerant** — pull-ups must go to 3.3 V on a 5 V host.
- STM32 products use clock stretching; Arduino and ESP32 handle it. **Raspberry Pi hardware I2C has known clock-stretching issues**: drop to 10–50 kHz or use `i2c-gpio`. micro:bit V2: report unstable readings, the firmware has a no-stretch option.

---

## 📥 Reading: write the number, wait 2 ms, read 2×k bytes

1. Write **1 byte: the value number** (e.g. `0x01`).
2. **Wait 2 ms** (needed by ESP32 products; harmless for the others, the libraries always wait).
3. `requestFrom(address, 2×k)`: **k consecutive values** starting at that number (k ≤ 16, i.e. up to 32 bytes).
4. Each value is an **int16, low byte first (little-endian)** — same as LEGO, MATRIX and mBlock.

- Undefined numbers read **−32768** (`0x00 0x80`); reading across gaps is fine, gaps are simply −32768.
- The module refreshes its snapshot every **10 ms**; one burst read is guaranteed to be from the same instant.
- Values are already what you want: line position −8…+8 (negative = left), sticks −100…+100, distance in cm/mm, hue −1 = grey/black/white. No more "subtract 100".

## 📤 Commands (E0–EE)

Write `[command][parameters…]`; the parameter length depends on the command (no parameters = just 1 byte). Commands run **asynchronously** (calibration, EEPROM writes); confirm with the `F3` status flags. Each product lists its commands in its reference page, e.g. Line follower `E0` black line, `E2` calibrate black, `E4 [threshold]`; PS4 `E5 [strength]` rumble, `E6 [R][G][B]` light bar; IMU `E0` zero heading.

---

## 🧾 Common Registers (every product)

<!-- gen:i2c-common -->
| Id | R/W | Name | Meaning |
| :--- | :---: | :--- | :--- |
| <span id="rf0">`F0`</span> | R | device ID | low byte = factory I2C address (unchanged after re-addressing), high byte = hw[0] (channels / sensors / variant) |
| <span id="rf1">`F1`</span> | R | protocol version | 2 for this release (same number reported in SPIKE mode 8 v[1])（2） |
| <span id="rf2">`F2`</span> | R | firmware version | major×100 + minor (e.g. 102 = 1.2) |
| <span id="rf3">`F3`</span> | R | status flags | b0 = ready; other bits per product (see flags)（bit） |
| <span id="rf4">`F4`</span> | R | update counter | +1 every 10 ms snapshot, wraps 0~255 (2.56 s per cycle, not usable for reboot detection; frozen = module busy)（0~255） |
| <span id="rf5">`F5`</span> | R | uptime seconds | seconds since boot, saturates at 32767 (~9 h); a drop means the module rebooted（0~32767） |
| <span id="rf8">`F8`</span> | W | change I2C address | [F8][new 0x08~0x77][~new], saved to EEPROM, takes effect after reboot; [F8][0x00][0xFF] = back to factory address；params：new address 0x08~0x77, ~new address |
| <span id="ref">`EF`</span> | W | reboot | [EF][0xA5]; mBlock sends ACK first, then reboots；params：magic 0xA5 |
<!-- /gen -->

**Suggested host flow:** read `F0` at boot (product + variant) → read `F3` until bit0 = 1 (ready) → burst-read values.

**Re-addressing:** write `[F8][new][~new]` (e.g. to 0x17: `F8 17 E8`), takes effect after reboot; `[F8][00][FF]` restores the factory address. The low byte of `F0` always returns the factory address, so a re-addressed module is still identifiable.

---

## 💻 Shared Arduino helpers

Every example carries this block at the bottom, so each sketch is a single file:

```cpp
#include <Wire.h>
const int16_t MBC_NONE = -32768;          // gap / no answer

// Read k consecutive values starting at id (k ≤ 16). false = no device
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1; if (k > 16) k = 16;
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                                  // needed by ESP32 products
  if (Wire.requestFrom(addr, (uint8_t)(2 * k)) != 2 * k) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read(), hi = Wire.read();
    out[i] = (int16_t)(lo | (hi << 8));     // little-endian
  }
  return true;
}

int16_t mbcRead(uint8_t addr, uint8_t id) {  // one value
  int16_t v; return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0) {  // command / re-address / reboot
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint8_t addr, uint16_t ms) {   // wait for F3 bit0
  uint32_t t0 = millis();
  while (millis() - t0 < ms) { int16_t s = mbcRead(addr, 0xF3); if (s != MBC_NONE && (s & 1)) return true; delay(20); }
  return false;
}
```

### Example: identify the connected product

```cpp
void setup() {
  Serial.begin(115200);
  Wire.begin();
  int16_t id = mbcRead(0x16, 0xF0);           // 0x16 = line follower family
  if (id == MBC_NONE) { Serial.println("No device - check wiring and address"); return; }
  Serial.print("factory address 0x"); Serial.print(id & 0xFF, HEX);
  Serial.print(", variant hw0 = "); Serial.println(id >> 8);   // 8 = Line8, 16 = Line16
  int16_t fw = mbcRead(0x16, 0xF2);
  Serial.print("firmware "); Serial.print(fw / 100); Serial.print('.'); Serial.println(fw % 100);
}
void loop() {}
```

> [!NOTE]
> The Universal I2C edition and the SPIKE / EV3 editions are **different hardware** with different connectors. Check the edition before ordering, or [contact us](/en/contact.md).
