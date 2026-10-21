/*
 * 測距者 8 路 × Arduino　範例 3：最近的東西在哪個方向？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   感應器會自己比較 8 顆雷射，找出「離你最近的東西」在哪個方向、有多遠。
 *   電腦會畫一把小尺，用 * 標出東西的位置，再用箭頭告訴你方向：
 *     ←  左邊    ↖  左前方    ↑  正前方    ↗  右前方    →  右邊
 *   相撲機器人「找對手」、小車「追著手走」，用的就是這個。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把手放在感應器前面 30 公分左右，慢慢從左邊繞到右邊。
 *
 * 方位數字是什麼？
 *   -7（最左）~ 0（正前方）~ +7（最右）。負數 = 左邊，正數 = 右邊。
 *   東西剛好在兩顆雷射中間時，會出現兩顆中間的數字（例如 -6、-4）。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint16_t NO_TARGET = 1200;         // 距離 1200 以上 = 沒有東西

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0，一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把手放在感應器前面，從左邊慢慢移到右邊。"));
}

void loop() {
  // 第 0x11 號格子：6 個數字
  //   第 0 個：最近的東西在哪個方向（-7 左 ~ +7 右）
  //   第 1、2 個：最近的東西有多遠（mm）
  //   第 3 ~ 5 個：最遠（最空曠）的方向和距離，這個範例先不用
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  int8_t   dir = (int8_t)d[0];                         // 方向：要轉成「有正負號」的數字
  uint16_t mm  = ((uint16_t)d[1] << 8) | d[2];         // 2 個數字合成距離

  // 先確定「真的有東西」，再看方向。沒有東西時，方向的數字沒有意義
  if (mm >= NO_TARGET) {
    Serial.println(F("左 [...............] 右   前面 120 公分內沒有東西"));
    delay(300);
    return;
  }

  // ---- 1. 畫小尺：15 格代表 -7 ~ +7，東西在哪一格就畫 * ----
  Serial.print(F("左 ["));
  for (int8_t pos = -7; pos <= 7; pos++) {
    if (pos == dir)      Serial.print('*');            // 東西在這裡
    else if (pos == 0)   Serial.print('+');            // 正中間做個記號
    else                 Serial.print('.');
  }
  Serial.print(F("] 右   "));

  // ---- 2. 用箭頭說方向 ----
  if (dir <= -6)      Serial.print(F("← 左邊  "));     // 👉 可以改：分區的界線
  else if (dir <= -2) Serial.print(F("↖ 左前方"));
  else if (dir <= 1)  Serial.print(F("↑ 正前方"));
  else if (dir <= 5)  Serial.print(F("↗ 右前方"));
  else                Serial.print(F("→ 右邊  "));

  // ---- 3. 印距離（公分）和方位數字 ----
  Serial.print(F("  "));
  Serial.print(mm / 10);                               // mm 除以 10 = 公分
  Serial.print(F(" 公分（方位 "));
  Serial.print((int)dir);                              // 轉成 int 再印，才會印出數字
  Serial.println(F("）"));

  delay(300);                                          // 👉 可以改：每 0.3 秒看一次
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
