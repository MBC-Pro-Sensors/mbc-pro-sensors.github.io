/*
 * 測距者 2 路 × Arduino　範例 5：倒車雷達警報
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用板子內建的小燈（L）和一顆蜂鳴器（可不接）當倒車雷達：
 *   讀「03 較近距離 cm」，沒有東西就安靜，有東西就閃／嗶，越近越急，
 *   貼到 STOP_CM 以內就一直亮、一直響。
 *
 * 你需要：測距者 2 路（通用 I2C 版）；蜂鳴器可有可無（有的話接「有源蜂鳴器」）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   蜂鳴器 + → 8（ESP32 → 27）   蜂鳴器 − → GND
 *
 * 怎麼看結果：
 *   手慢慢靠近感應器，L 燈（和蜂鳴器）越閃越快；序列埠 115200 也會印距離。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // ESP32 開發板的內建燈通常接在 GPIO 2
#endif

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

#if defined(ESP32)
const int BUZZER = 27;                   // 蜂鳴器腳位（ESP32）
#else
const int BUZZER = 8;                    // 蜂鳴器腳位（UNO）
#endif

// ---- 👉 可以改的參數 ----
const int16_t ALARM_START_CM = 80;       // 幾公分以內開始警報
const int16_t STOP_CM        = 10;       // 幾公分以內一直亮、一直響

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！手慢慢靠近感應器，看 L 燈越閃越快。"));
}

void loop() {
  int16_t cm = mbcRead(ADDR, 0x03);      // 03 = 左右兩顆取較近的距離（cm）
  if (cm == MBC_NONE) { delay(100); return; }

  if (millis() - lastPrint >= 500) {     // 每 0.5 秒印一次距離
    lastPrint = millis();
    if (cm >= 120) Serial.println(F("沒有東西"));
    else { Serial.print(F("最近的東西：")); Serial.print(cm); Serial.println(F(" 公分")); }
  }

  if (cm >= ALARM_START_CM) {            // 太遠：安靜
    alarm(false);
    delay(50);
  } else if (cm <= STOP_CM) {            // 太近：一直亮
    alarm(true);
    delay(50);
  } else {                               // 中間：越近閃越快（間隔 50 ~ 400 ms）
    int gap = map(cm, STOP_CM, ALARM_START_CM, 50, 400);
    alarm(true);  delay(gap / 2);
    alarm(false); delay(gap / 2);
  }
}

void alarm(bool on) {
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  digitalWrite(BUZZER, on ? HIGH : LOW);
}

// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
//   沒有這個編號（空號）時感應器回 -32768（MBC_NONE）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
// UNO/Nano：SDA=A4、SCL=A5；UNO R4：SDA/SCL 腳；ESP32：預設 GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從位址 addr 的感應器，由編號 id 起連續讀 k 個值（1~16）放進 out。全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte，Wire 緩衝區上限）
  Wire.beginTransmission(addr);
  Wire.write(id);                        // 告訴感應器：我要從第 id 號開始讀
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等 2 ms 再讀（ESP32 型的產品一定要等）
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // 先來的是低位元組
    uint16_t hi = Wire.read();           // 再來高位元組（小端）
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值。讀不到（沒接好）回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]。p 指向參數、n = 參數幾個 byte（沒參數就不填）。成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（狀態旗標 F3 的 bit0 = 1），最多等 ms 毫秒。準備好了回傳 true
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
