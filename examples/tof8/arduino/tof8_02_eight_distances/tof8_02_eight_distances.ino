/*
 * 測距者 8 路 × Arduino　範例 2：8 顆雷射量到多遠？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   測距者 8 路有 8 顆雷射，排成半圓形，從左邊一路看到右邊。
 *   這個範例把每一顆量到的距離（單位：毫米 mm，10 mm = 1 公分）印出來：
 *     數字  = 前面的東西有多遠
 *     ----  = 這個方向沒有東西（超過 120 公分）
 *     XX    = 這一顆雷射故障了
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   拿手或書本在感應器前面左右移動，看看是哪一顆的數字在變。
 *
 * 小提醒：
 *   第 0 顆在最左邊、第 7 顆在最右邊（從感應器後面、跟它看同一個方向時的左右）。
 *   第 3、4 顆都朝正前方。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint8_t LASERS = 8;                // 測距者 8 路有 8 顆雷射
const uint16_t NO_TARGET = 1200;         // 量到 1200 以上 = 這個方向沒有東西
const uint16_t FAULT = 0xFFFF;           // 收到 0xFFFF（65535）= 這一顆故障

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0，一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿手在感應器前面移動看看。"));
  Serial.println(F("（單位 mm；---- = 沒有東西，XX = 故障）"));
  Serial.println(F("   左                                       右"));
  Serial.println(F("     0     1     2     3     4     5     6     7"));
}

void loop() {
  // 第 0x21 號格子：16 個數字，每 2 個合成一顆雷射的距離（第 0 顆 → 第 7 顆）
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  for (uint8_t i = 0; i < LASERS; i++) {
    // 2 個數字合成一個距離：前面那個是「大的那一半」
    uint16_t mm = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];

    // 判斷的順序很重要：先看是不是故障，再看有沒有東西，最後才是距離
    if (mm == FAULT) {
      Serial.print(F("    XX"));          // 這一顆故障
    } else if (mm >= NO_TARGET) {
      Serial.print(F("  ----"));          // 這個方向沒有東西
    } else {
      printRight(mm, 6);                  // 印出距離，靠右對齊佔 6 格
    }
  }
  Serial.println();

  delay(300);                             // 👉 可以改：每 0.3 秒看一次
}

// 把數字靠右對齊印出來，總共佔 width 格（前面補空白）
void printRight(uint16_t value, uint8_t width) {
  uint8_t digits = 1;
  for (uint16_t v = value; v >= 10; v /= 10) digits++;   // 算這個數字有幾位數
  for (uint8_t k = digits; k < width; k++) Serial.print(' ');
  Serial.print(value);
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
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

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
