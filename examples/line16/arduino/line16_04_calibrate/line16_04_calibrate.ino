/*
 * 循行者 16 路 × Arduino　範例 4：一鍵校準
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   「校準」就是讓感應器記住比賽場地的黑色和白色長什麼樣子。
 *   換了場地、燈光不一樣，都建議重新校準一次。校準結果會存在感應器裡，關機也不會忘記。
 *
 * 怎麼校準：
 *   1. 把感應器放在線的上方（離地面約 0.5～1 公分，跟裝在車上一樣高）。
 *   2. 在序列埠監控窗上方的輸入框打 c，按 Enter。
 *   3. 接下來 5 秒內，慢慢把感應器左右移動，讓 16 隻眼睛都「掃過」黑線和白底。
 *   4. 看到「校準完成」就好了。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼
const uint8_t EYES = 16;                 // 循行者 16 路有 16 隻眼睛

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！在上方輸入框打 c 再按 Enter，開始校準。"));
}

void loop() {
  // 有沒有人在序列埠監控窗打字？
  if (Serial.available()) {
    char key = Serial.read();            // 讀進一個字
    if (key == 'c' || key == 'C') {
      calibrate();
    }
  }
}

// 校準的步驟都寫在這裡
void calibrate() {
  // 第 0x21 號格子有 2 個數字，第 2 個是「已經校準過幾次」
  uint8_t before[2];
  if (!mbcRead(0x21, before, 2)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    return;
  }

  // 寫 1 到第 0x20 號格子 = 開始 5 秒校準
  mbcWrite(0x20, 1);
  Serial.println(F("開始校準！5 秒內請左右移動感應器，掃過黑線和白底……"));

  // 一直檢查「校準次數」有沒有加 1。加 1 了就代表校準完成
  unsigned long start = millis();
  while (millis() - start < 7000) {      // 最多等 7 秒
    delay(250);
    Serial.print('.');                   // 印一個點，表示還在等

    uint8_t now[2];
    if (mbcRead(0x21, now, 2) && now[1] != before[1]) {
      Serial.println();
      Serial.println(F("校準完成！結果已存進感應器，關機也不會忘記。"));
      showResult();
      return;
    }
  }

  Serial.println();
  Serial.println(F("等太久了，校準沒有完成。請再打一次 c 試試看。"));
}

// 把每隻眼睛記住的「白色」和「黑色」印出來看看
void showResult() {
  uint8_t white[EYES], black[EYES];
  if (!mbcRead(0x51, white, EYES) ||     // 第 0x51 號格子：每隻眼睛記住的白色（16 個數字）
      !mbcRead(0x52, black, EYES)) {     // 第 0x52 號格子：每隻眼睛記住的黑色（16 個數字）
    Serial.println(F("讀取校準結果失敗，檢查一下線有沒有鬆掉。"));
    return;
  }

  Serial.println(F("眼睛（左→右）  白色  黑色  差距"));
  for (int8_t i = 0; i < EYES; i++) {      // 第 0 隻在最左邊，第 15 隻在最右邊（站在車子後面往前看）
    Serial.print(F("  第 "));
    if (i < 10) Serial.print(' ');       // 對齊用：個位數前面補一個空白
    Serial.print(i);
    Serial.print(F(" 隻      "));
    printNum(white[i]);
    Serial.print(F("  "));
    printNum(black[i]);
    Serial.print(F("  "));
    printNum(white[i] - black[i]);
    Serial.println();
  }
  Serial.println(F("「差距」越大越好。差距太小（小於 20）代表那隻眼睛沒掃到黑線，請重新校準。"));
}

// 印一個數字，固定佔 4 格，讓表格對齊
void printNum(int v) {
  if (v < 1000 && v > -100) Serial.print(' ');
  if (v < 100 && v > -10)   Serial.print(' ');
  if (v < 10 && v >= 0)     Serial.print(' ');
  Serial.print(v);
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
