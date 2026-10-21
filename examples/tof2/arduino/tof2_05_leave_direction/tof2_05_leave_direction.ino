/*
 * 測距者 2 路 × Arduino　範例 5：東西從哪一邊溜走了？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   這是測距者 2 路最厲害的功能：判斷東西是「從左邊」還是「從右邊」離開的。
 *   原理很簡單：東西往右邊移走時，左邊的雷射會「先」看不到它，
 *   這時只剩右邊的雷射還看得到；等右邊也看不到，就知道它是往右邊溜走的。
 *     最後只剩右邊看到 → 東西往右邊離開 →
 *     最後只剩左邊看到 → 東西往左邊離開 ←
 *   相撲機器人的對手跑掉時，就知道要往哪邊轉去追。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好（看到東西時亮）。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把一個盒子放在感應器正前方 20 公分（兩顆雷射都看到），
 *   然後把盒子往左邊或往右邊移開，看電腦說它往哪邊走了。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

// ---- 👉 可以改的參數 ----
const uint16_t SEE_DIST = 500;           // 多近才算「看到」（mm）：500 = 50 公分，免得把後面的牆當成目標
const unsigned long GONE_MS = 150;       // 兩邊都看不到超過多久（毫秒），才算真的離開

// 目前誰看得到：0 = 都看不到、1 = 只有左邊、2 = 只有右邊、3 = 兩邊都看到
uint8_t state = 0;
uint8_t lastOneSide = 0;                 // 東西消失前「只剩哪一邊」看到：0 = 沒有（兩邊同時消失）、1 = 左、2 = 右
bool present = false;                    // 現在是不是有東西在前面
unsigned long goneSince = 0;             // 從什麼時候開始兩邊都看不到

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  // 雷射開機這段時間距離都是 0（會被當成「兩邊都看到」），一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把盒子放在正前方，再往左或往右移開。"));
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

  // 「看到」= 不是故障，而且夠近
  bool seeL = (left  != FAULT) && (left  < SEE_DIST);
  bool seeR = (right != FAULT) && (right < SEE_DIST);

  // 把兩邊看到的情形合成一個數字：左邊看到 +1、右邊看到 +2
  uint8_t now = (seeL ? 1 : 0) + (seeR ? 2 : 0);

  // ---- 1. 情形改變了，印出來 ----
  if (now != state) {
    state = now;
    if (state == 3)      Serial.println(F("  左 ● ● 右   兩邊都看到（在正前方）"));
    else if (state == 1) Serial.println(F("  左 ● ○ 右   只剩左邊看到"));
    else if (state == 2) Serial.println(F("  左 ○ ● 右   只剩右邊看到"));
    else                 Serial.println(F("  左 ○ ○ 右   兩邊都看不到"));
  }

  // ---- 2. 有看到東西：記住「最後只剩哪一邊」 ----
  if (state != 0) {
    if (!present) {
      present = true;
      Serial.println(F("發現東西！"));
    }
    if (state == 3) lastOneSide = 0;          // 兩邊都看到 → 還沒開始離開
    else            lastOneSide = state;      // 只剩一邊看到 → 記下是哪一邊
    goneSince = millis();                     // 還看得到，重新計時
  }

  // ---- 3. 兩邊都看不到一段時間了：宣布它從哪邊離開 ----
  if (state == 0 && present && millis() - goneSince >= GONE_MS) {
    present = false;
    if (lastOneSide == 1)      Serial.println(F("←←← 東西往「左邊」離開了！（右邊的雷射先看不到）"));
    else if (lastOneSide == 2) Serial.println(F("→→→ 東西往「右邊」離開了！（左邊的雷射先看不到）"));
    else                       Serial.println(F("東西兩邊同時消失了（可能是往後退開，或移得太快）"));
    Serial.println();
  }

  digitalWrite(LED, present ? HIGH : LOW);    // 有東西時燈亮

  delay(30);                                  // 讀快一點，才抓得到「哪一邊先」
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
