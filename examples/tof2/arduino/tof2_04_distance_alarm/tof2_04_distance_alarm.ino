/*
 * 測距者 2 路 × Arduino　範例 4：倒車雷達警報燈
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用 Arduino 板子上內建的小燈（標著 L 的那顆）當警報燈，就像汽車的倒車雷達：
 *     前面沒有東西          → 燈熄
 *     有東西，越近閃得越快
 *     貼得很近（10 公分內） → 燈一直亮
 *   左、右兩顆雷射，哪一顆比較近就看哪一顆。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，看 Arduino 板子上的 L 燈。手慢慢靠近感應器，燈會越閃越快。
 *   也可以打開「序列埠監控窗」（115200）看最近的距離。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）
const uint16_t NO_TARGET = 1200;         // 距離 1200 以上 = 沒有東西
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

// ---- 👉 可以改的參數 ----
const uint16_t ALARM_START = 1000;       // 多近開始閃（mm）：1000 = 1 公尺
const uint16_t ALWAYS_ON   = 100;        // 多近變成一直亮（mm）：100 = 10 公分

uint16_t nearest = NO_TARGET;            // 最近的東西有多遠（mm）
bool ledOn = false;                      // 燈現在是亮還是熄
unsigned long lastBlink = 0;             // 上一次切換燈的時間
unsigned long lastRead = 0;              // 上一次讀感應器的時間
unsigned long lastPrint = 0;             // 上一次印到電腦的時間

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // 把燈的腳設成「輸出」，才能控制它亮或熄
  mbcBegin();
  delay(500);

  // 雷射開機這段時間距離都是 0（會被當成貼著東西狂閃），一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！手慢慢靠近感應器，看 L 燈越閃越快。"));
}

void loop() {
  // ---- 1. 每 0.05 秒讀一次左右距離，取比較近的那個 ----
  if (millis() - lastRead >= 50) {
    lastRead = millis();
    uint8_t d[4];
    if (mbcRead(0x11, d, 4)) {                         // 第 0x11 號格子：左距離 + 右距離
      uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
      uint16_t right = ((uint16_t)d[2] << 8) | d[3];
      if (left == FAULT)  left  = NO_TARGET;           // 故障的那一邊不算
      if (right == FAULT) right = NO_TARGET;
      nearest = min(left, right);                      // 比較近的那一邊
    } else {
      nearest = NO_TARGET;                             // 讀不到：先當作沒有東西，燈熄
    }
  }

  // ---- 2. 依距離決定燈怎麼亮 ----
  if (nearest >= ALARM_START) {
    ledOn = false;                                     // 太遠或沒有東西：燈熄
  } else if (nearest <= ALWAYS_ON) {
    ledOn = true;                                      // 非常近：燈一直亮
  } else {
    // 越近，閃的間隔越短：距離 100 mm → 每 0.05 秒切換；1000 mm → 每 0.5 秒切換
    unsigned long interval = map(nearest, ALWAYS_ON, ALARM_START, 50, 500);
    if (millis() - lastBlink >= interval) {
      lastBlink = millis();
      ledOn = !ledOn;                                  // 亮變熄、熄變亮
    }
  }
  digitalWrite(LED, ledOn ? HIGH : LOW);

  // ---- 3. 每 0.5 秒把距離印到電腦上 ----
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    if (nearest >= NO_TARGET) {
      Serial.println(F("沒有東西"));
    } else {
      Serial.print(F("最近的東西："));
      Serial.print(nearest / 10);
      Serial.println(F(" 公分"));
    }
  }
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
