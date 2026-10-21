/*
 * 循行者 16 路 × Arduino　範例 5：路口偵測燈
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用 Arduino 板子上內建的小燈（標著 L 的那顆）當訊號燈：
 *     看到線        → 燈亮
 *     線跑掉了      → 燈熄，電腦告訴你線最後是往哪邊跑掉的
 *     遇到十字路口、岔路 → 燈快閃 3 下，並且數「這是第幾個路口」
 *   比賽常常要「走到第 3 個路口右轉」，這個範例就是在學怎麼數路口。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   拿一張畫了十字線的紙，讓感應器沿著線慢慢經過路口。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

// 👉 可以改：16 隻眼睛裡，有幾隻以上同時看到線，就算是路口（十字路口的橫線會蓋住很多隻眼睛）
const uint8_t CROSS_WIDTH = 10;

int crossCount = 0;                      // 已經經過幾個路口
bool onCross = false;                    // 現在是不是正在路口上
bool lost = false;                       // 現在是不是看不到線

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // 把燈的腳設成「輸出」，才能控制它亮或熄
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  mbcWrite(0x10, 0);                     // 循黑線（白線請改成 1）
  Serial.println(F("準備好了！讓感應器沿著線經過路口看看。"));
}

void loop() {
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {            // 讀循線結果（7 個數字）
    digitalWrite(LED, LOW);              // 讀不到就先把燈關掉
    delay(100);
    return;
  }

  uint8_t width    = d[2];               // 有幾隻眼睛同時看到線（0 ~ 16）
  uint8_t groups   = d[3];               // 看到幾段線：0 = 沒線，1 = 一條線，2 以上 = 岔路
  int8_t  lastSide = (int8_t)d[6];       // 線最後碰到哪一邊的眼睛：負數 = 左邊，正數 = 右邊

  // ---- 1. 看不到線 ----
  if (groups == 0) {
    digitalWrite(LED, LOW);              // 燈熄
    if (!lost) {                         // 剛剛才跑掉，只說一次就好
      lost = true;
      if (lastSide < 0)      Serial.println(F("線跑掉了！最後在左邊 → 往左找"));
      else if (lastSide > 0) Serial.println(F("線跑掉了！最後在右邊 → 往右找"));
      else                   Serial.println(F("看不到線"));
    }
    onCross = false;
    delay(20);
    return;
  }
  lost = false;

  // ---- 2. 是不是路口？ ----
  // 看到 2 段以上的線（岔路），或很多隻眼睛同時看到線（十字路口的橫線），就當作路口
  bool isCross = (groups >= 2) || (width >= CROSS_WIDTH);

  if (isCross && !onCross) {             // 剛走進路口的那一下才算一次，不會重複數
    onCross = true;
    crossCount = crossCount + 1;
    Serial.print(F("第 "));
    Serial.print(crossCount);
    Serial.println(F(" 個路口！"));
    for (int i = 0; i < 3; i++) {        // 快閃 3 下
      digitalWrite(LED, LOW);  delay(80);
      digitalWrite(LED, HIGH); delay(80);
    }
  }
  if (!isCross) onCross = false;         // 離開路口了，下一個路口才能再數

  // ---- 3. 一般情況：看到線就亮燈 ----
  digitalWrite(LED, HIGH);
  delay(20);
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
