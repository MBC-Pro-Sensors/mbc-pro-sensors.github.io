// ============================================================
// TOF8_Arduino_Library_Example.ino
// 開發板：Arduino Uno / Nano
// 用途：透過 I2C 讀取測距者 8 路（TOF8，通用 I2C 版）的 8 路距離與方位
// 寫法：使用同資料夾內的 MBC_TOF8.h 函式庫（庫函數範例）
//
// 接線（Uno/Nano <-> TOF8）：
//   A4 (SDA) -- SDA
//   A5 (SCL) -- SCL
//   5V       -- VCC
//   GND      -- GND（共地）
// 序列埠監控視窗：115200 bps
// ============================================================
#include <Wire.h>
#include "MBC_TOF8.h"

MBC_TOF8 tof8(0x29); // 出廠位址 0x29

void setup() {
  Serial.begin(115200);
  tof8.begin();

  // 開機後約 2~4 秒完成 8 顆初始化，等就緒再開始讀
  unsigned long t0 = millis();
  while (!tof8.isReady() && millis() - t0 < 5000) delay(100);

  char model[9];
  if (tof8.readModel(model)) {
    Serial.print("Model: ");
    Serial.println(model); // LASER8
  } else {
    Serial.println("找不到感應器（檢查接線 / 位址）");
  }
}

void loop() {
  // ── 讀取 8 顆距離（mm，1200 = 沒有目標，65535 = 該顆故障）──
  uint16_t mm[8];
  if (tof8.readAll(mm)) {
    for (uint8_t i = 0; i < 8; i++) {
      Serial.print(mm[i]);
      Serial.print(",");
    }
  } else {
    Serial.println("讀取 0x21 失敗（檢查接線 / 位址）");
  }

  // ── 讀取最近／最遠方位（-8 左 ~ +8 右，0 = 正前方）與距離 ──
  int8_t minDir, maxDir;
  uint16_t minMm, maxMm;
  if (tof8.readMinMax(minDir, minMm, maxDir, maxMm)) {
    Serial.print(" Min: dir=");
    Serial.print(minDir);
    Serial.print(" dist=");
    Serial.print(minMm);
    Serial.print("mm  Max: dir=");
    Serial.print(maxDir);
    Serial.print(" dist=");
    Serial.print(maxMm);
    Serial.println("mm");
  }

  delay(50);
}
