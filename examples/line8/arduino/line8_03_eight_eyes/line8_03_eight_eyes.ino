/*
 * 循行者 8 路 × Arduino　範例 3：8 隻眼睛看到什麼？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   循行者底下有 8 顆光感應器，就像 8 隻眼睛排成一排。
 *   這個範例把每隻眼睛看到的東西印出來：
 *     ● = 這隻眼睛看到線      ○ = 這隻眼睛看到底色
 *   下面再印出每隻眼睛的「顏色分數」：0 = 很黑，100 = 很白。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   印出來的順序就是感應器裝在車上、你站在車子後面往前看的順序：左邊的眼睛印在左邊。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼
const uint8_t EYES = 8;                  // 循行者 8 路有 8 隻眼睛

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把感應器移到線上，看看哪幾隻眼睛會變成 ●"));
}

void loop() {
  // 第 0x16 號格子：2 個數字，合起來的每一個「位元」代表一隻眼睛有沒有看到線
  uint8_t b[2];
  // 第 0x50 號格子：8 個數字，每隻眼睛的顏色分數 0 ~ 100
  uint8_t score[EYES];

  if (!mbcRead(0x16, b, 2) || !mbcRead(0x50, score, EYES)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  uint16_t onLine = ((uint16_t)b[0] << 8) | b[1];  // 把 2 個數字合成一個

  // 眼睛的編號（站在車子後面往前看）：第 0 隻在最左邊，第 7 隻在最右邊。
  // 所以從第 0 隻印到第 7 隻，畫面就跟實際的左右一樣。
  Serial.print(F("左 "));
  for (int8_t i = 0; i < EYES; i++) {
    if (onLine & (1u << i)) Serial.print(F("  ● "));
    else                    Serial.print(F("  ○ "));
  }
  Serial.println(F("右"));

  Serial.print(F("分"));
  for (int8_t i = 0; i < EYES; i++) {
    Serial.print(' ');
    if (score[i] < 100) Serial.print(' ');   // 對齊用：不到 3 位數，前面補空白
    if (score[i] < 10)  Serial.print(' ');
    Serial.print(score[i]);
  }
  Serial.println();
  Serial.println();

  delay(300);                            // 👉 可以改：每 0.3 秒看一次
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
