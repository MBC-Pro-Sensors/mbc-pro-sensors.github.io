<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ∿ Universal I2C Version Development Guide

[🔙 Back to 16-Way Line Follower Home](/en/sensors/line16/index.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 25px 45px; display: flex; align-items: center; gap: 25px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="/images/sensors/line16/line16-product-arduino.webp" alt="Pathfinder 16-Way Sensor (Universal I2C Edition)" style="max-height: 144px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(10,186,181,0.75)) drop-shadow(0 0 45px rgba(10,186,181,0.45));" />
    <span style="font-size: 2.5rem; color: #777; font-weight: 300; line-height: 1;">+</span>
    <div style="display: flex; gap: 15px; align-items: center;">
      <svg viewBox="0 0 100 40" style="height: 60px; fill: none; stroke: #00d2ff; stroke-width: 4; stroke-linecap: round; stroke-linejoin: round; filter: drop-shadow(0 0 10px rgba(0, 210, 255, 0.4));">
        <path d="M 5,25 H 25 V 5 H 45 V 35 H 65 V 5 H 85 V 35 H 95" />
      </svg>
    </div>
  </div>
</div>

Welcome to the professional hardware development zone! In addition to perfectly supporting the LEGO ecosystem, the MBC 16-Way Line Follower also provides an open, standard I2C communication interface for developers using **Arduino, ESP32, Raspberry Pi, and various custom master boards**. 
The read and write registers of this sensor are completely separated, utilizing a conflict-free design, making it highly suitable for developing high-speed line-following robots or PC visualization dashboards.

---

> [!IMPORTANT]
> **This page documents the new v1 protocol (address `0x16`).** For the read/write rules, status registers and Arduino helper functions shared by every MBC product, see the [MBC Universal I2C Protocol](/en/i2c-protocol.md) first.

## 📡 Basics

| Item | Value |
| :--- | :--- |
| I2C address | `0x16` (shared by the line sensor family) |
| Model string `0x07` | `LINE16` |
| Variant `0x06` | `[16, PCB revision]` (byte 1 = channel count) |
| Channel order | **CH0 = rightmost**, CH15 = leftmost |
| Line position sign | **Negative = line on the right, positive = line on the left**, 0 = centered |

## 1️⃣ Line Result (most used)

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x10` | W | 1 | Target: `0` = black line, `1` = white line (reverts to the button setting after reboot) |
| `0x11` | R | 7 | **Line bundle** (= `0x12` ~ `0x17`, easiest to read in one go) |
| `0x12` | R | 1 | Line position int8: `-16` ~ `+16` |
| `0x13` | R | 1 | **High-resolution position** int8: `-100` ~ `+100` (best for PID) |
| `0x14` | R | 1 | Line width: number of channels on the line |
| `0x15` | R | 1 | Line groups: `0` = line lost, `1` = single line, `2+` = fork / intersection |
| `0x16` | R | 2 | Binary map uint16: bit i = CH i on the line |
| `0x17` | R | 1 | Last exit side int8 (which way to search when the line is lost) |

## 2️⃣ Calibration

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x20` | W | 1 | `1` = 5-second dynamic calibration (sweep the robot across the line), `2` = store current reading as white, `3` = store current reading as black. Saved to EEPROM |
| `0x21` | R | 2 | Calibration bundle (= `0x22` ~ `0x23`) |
| `0x22` | RW | 1 | Threshold `1` ~ `99` (default 50; resets on reboot) |
| `0x23` | R | 1 | Calibration counter (+1 each time a calibration finishes) |

## 3️⃣ Status Flags `0x32`

| Bit | Meaning |
| :---: | :--- |
| bit0 | Ready |
| bit1 | White-line mode (0 = black line) |
| bit2 | Calibrating (the update counter `0x33` pauses during the 5-second calibration — normal) |
| bit3 | Line lost |
| bit4 | Fork (groups ≥ 2) |

## 🔬 Per-Channel Values

| Register | Dir | Len | Content |
| :---: | :---: | :---: | :--- |
| `0x40` ~ `0x4F` | R | 1 | One channel, calibrated `0` ~ `100` (`0x40` + channel) |
| `0x50` | R | 16 | All channels, calibrated `0` ~ `100` |
| `0x51` | R | 16 | White reference (EEPROM) |
| `0x52` | R | 16 | Black reference (EEPROM) |
| `0x53` | R | 16 | Raw ADC (8-bit) |

---

## 💻 Arduino Example: Read the Line Result

```cpp
#include <Wire.h>

const uint8_t LINE_ADDR = 0x16;

bool iicRead(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

void iicWrite(uint8_t addr, uint8_t reg, uint8_t v) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(v);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  iicWrite(LINE_ADDR, 0x10, 0);     // 0 = black line, 1 = white line
}

void loop() {
  uint8_t d[7];
  if (iicRead(LINE_ADDR, 0x11, d, 7)) {
    int8_t   pos    = (int8_t)d[0];        // -16 ~ +16
    int8_t   posHi  = (int8_t)d[1];        // -100 ~ +100, use this for PID
    uint8_t  width  = d[2];                // line width
    uint8_t  groups = d[3];                // 0 lost / 1 single / 2+ fork
    uint16_t bin    = (d[4] << 8) | d[5];  // one bit per channel

    Serial.print("pos ");      Serial.print(pos);
    Serial.print("  hi-res "); Serial.print(posHi);
    Serial.print("  width ");  Serial.print(width);
    Serial.print("  groups "); Serial.print(groups);
    Serial.print("  bin ");    Serial.println(bin, BIN);
  }
  delay(10);
}
```

### A Starting Point for PID Line Following

```cpp
// posHi: negative = line on the right, positive = on the left.
// Line on the right -> turn right -> speed up the left wheel
float Kp = 0.6;
int base = 120;                        // base speed (adjust for your motor driver)
int turn = Kp * posHi;
int leftSpeed  = base - turn;
int rightSpeed = base + turn;
```

### Trigger Calibration from Code

```cpp
uint8_t before[2], after[2];
iicRead(LINE_ADDR, 0x21, before, 2);   // [threshold, calibration counter]
iicWrite(LINE_ADDR, 0x20, 1);          // start 5-second calibration: sweep the robot across the line
do {
  delay(200);
  iicRead(LINE_ADDR, 0x21, after, 2);
} while (after[1] == before[1]);       // counter +1 = calibration finished
```

<br>

!!! success "🚀 Want to learn more advanced line-following control methods?"
You can take classes from these awesome coaches! They have rich experience in competitions and teaching, guaranteeing you'll learn a lot! 💯

<div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; margin-top: 15px; margin-bottom: 30px;">
<div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
<h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@LegoLauXiao" target="_blank" style="color: inherit; text-decoration: none;">Coach LegoLauXiao</a></h4>
<div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
<iframe src="https://www.youtube.com/embed/WgacdWLatbk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
</div>
</div>
<div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
<h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@legolaumo" target="_blank" style="color: inherit; text-decoration: none;">Coach legolaumo</a></h4>
<div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
<iframe src="https://www.youtube.com/embed/T9bcndBNQvQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
</div>
</div>
</div>

