// ============================================================
// TOF8_Arduino_Wire_Example.ino
// 開發板：Arduino Uno / Nano
// 用途：透過 I2C 讀取測距者 8 路（TOF8，通用 I2C 版）的 8 路距離與方位
// 寫法：直接呼叫 Wire.h 函式，不包裝成類別（Wire 寫法）
//
// MBC 通用 I2C 協議 v1（位址 0x29）：
//   1. 先寫 1 byte 暫存器編號
//   2. 再用 requestFrom 讀回資料（多位元組高位元組在前）
//   0x11        → 6 bytes ：最近方位 int8、最近距離 mm、最遠方位 int8、最遠距離 mm
//   0x21        → 16 bytes：8 顆距離 mm（第 0 顆 = 最左、第 7 顆 = 最右）
//   0x22 ~ 0x29 → 2 bytes ：第 0 ~ 7 顆距離 mm
//   距離 1200 = 範圍內沒有目標、0xFFFF = 該顆故障；方位 -8 左 ~ +8 右，0 = 正前方
//
// 接線（Uno/Nano <-> TOF8）：
//   A4 (SDA) -- SDA
//   A5 (SCL) -- SCL
//   5V       -- VCC
//   GND      -- GND（共地）
// 序列埠監控視窗：115200 bps
// ============================================================
#include <Wire.h>

#define TOF8_I2C_ADDR 0x29 // 出廠位址

// 讀 n bytes（n ≤ 32）。回傳 false = 裝置沒回應
bool iicRead(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false; // 非 0 代表 NACK / 通訊失敗
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// 高位元組在前 → 數值
uint16_t beU16(const uint8_t *p) { return (uint16_t)((p[0] << 8) | p[1]); }

void setup() {
  Wire.begin(); // Uno/Nano 固定用 A4=SDA, A5=SCL
  Serial.begin(115200);
}

void loop() {
  // ── 讀取 8 顆距離（暫存器 0x21，16 bytes）──
  uint8_t d[16];
  if (iicRead(TOF8_I2C_ADDR, 0x21, d, 16)) {
    for (uint8_t i = 0; i < 8; i++) {
      Serial.print(beU16(&d[2 * i])); // mm
      Serial.print(",");
    }
  } else {
    Serial.println("讀取 0x21 失敗（檢查接線 / 位址）");
  }

  // ── 讀取最近／最遠方位與距離（暫存器 0x11，6 bytes）──
  uint8_t m[6];
  if (iicRead(TOF8_I2C_ADDR, 0x11, m, 6)) {
    Serial.print(" Min: dir=");
    Serial.print((int8_t)m[0]);
    Serial.print(" dist=");
    Serial.print(beU16(&m[1]));
    Serial.print("mm  Max: dir=");
    Serial.print((int8_t)m[3]);
    Serial.print(" dist=");
    Serial.print(beU16(&m[4]));
    Serial.print("mm, ");
  }

  // ── 範例：只讀第 3 顆（第 2 號，暫存器 0x24）──
  uint8_t c[2];
  if (iicRead(TOF8_I2C_ADDR, 0x24, c, 2)) {
    Serial.print("CH2 = ");
    Serial.print(beU16(c));
    Serial.println("mm");
  }

  delay(50);
}
