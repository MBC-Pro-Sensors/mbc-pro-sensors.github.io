/*
 * 測距者 2 路 × Arduino　範例 3：東西在左邊、右邊，還是正前方？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   左、右兩顆雷射並排朝前看，就像兩隻眼睛：
 *     兩顆都看到東西   → 東西在正前方
 *     只有左邊看到     → 東西偏左
 *     只有右邊看到     → 東西偏右
 *     兩顆都沒看到     → 前面沒有東西
 *   機器人「追著東西走」或「找到目標」，第一步就是知道東西在哪一邊。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把一個盒子（或手）放在感應器前面 20 公分左右，慢慢左右移動。
 *
 * 小提醒：
 *   只算 50 公分內的東西（SEE_DIST），免得後面的牆壁也被當成目標。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

const uint16_t SEE_DIST = 500;           // 👉 可以改：多近才算「看到」（mm）。500 = 50 公分，最多 1199

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 雷射開機這段時間距離都是 0（會被當成「兩邊都貼著東西」），一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把東西放在感應器前面，慢慢左右移動。"));
}

void loop() {
  // 第 0x11 號格子：左邊距離（2 個數字）+ 右邊距離（2 個數字）
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  // 「看到」= 不是故障，而且夠近。故障的那一邊當作沒看到
  bool seeL = (left  != FAULT) && (left  < SEE_DIST);
  bool seeR = (right != FAULT) && (right < SEE_DIST);

  if (seeL && seeR) {
    Serial.print(F("↑ 正前方   距離 "));
    Serial.print(min(left, right) / 10);           // 兩邊比較近的那個，換成公分
    Serial.println(F(" 公分"));
  } else if (seeL) {
    Serial.print(F("↖ 偏左     距離 "));
    Serial.print(left / 10);
    Serial.println(F(" 公分   → 機器人要往左轉"));
  } else if (seeR) {
    Serial.print(F("↗ 偏右     距離 "));
    Serial.print(right / 10);
    Serial.println(F(" 公分   → 機器人要往右轉"));
  } else {
    Serial.println(F("   前面沒有東西"));
  }

  if (left == FAULT || right == FAULT) {
    Serial.println(F("（注意：有一顆雷射故障，請跑範例 1 檢查）"));
  }

  delay(200);                            // 👉 可以改：每 0.2 秒看一次
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
