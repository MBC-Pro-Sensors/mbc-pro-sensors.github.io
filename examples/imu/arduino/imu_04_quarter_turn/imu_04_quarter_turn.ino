/*
 * 陀螺儀 IMU × Arduino　範例 4：轉 90 度提醒
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器放在桌上轉，每多轉滿 90 度（1/4 圈），
 *   Arduino 板子上的內建小燈（標著 L 的那顆）就閃一下，
 *   電腦也會告訴你「往右轉滿 90 度！現在是 1/4 圈」。
 *   機器人比賽常常要「轉 90 度」，這個範例讓你先用手感受 90 度有多大。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把感應器平放在桌上，像轉方向盤一樣慢慢轉。
 *   輸入 z 按 Enter 可以重新從 0 開始。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

const int STEP_DEG = 90;                 // 👉 可以改：每轉幾度提醒一次（例如 45、180）

long quarter = 0;                        // 目前轉到第幾個 90 度（往右是正、往左是負）

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  Serial.println(F("請把感應器放在桌上，不要動它……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}
  }
  zeroHeading();                         // 從 0 度開始算
  Serial.println(F("準備好了！慢慢轉動感應器，每轉滿 90 度燈會閃一下。"));
}

void loop() {
  // ---- 1. 收到 z 就重新從 0 開始 ----
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'z' || c == 'Z') {
      zeroHeading();
      Serial.println(F("--- 已歸零，重新從 0 開始算 ---"));
    }
  }

  // ---- 2. 讀航向（第 0x11 號格子，6 個數字），用第 2、3 個數字（Gyro3D）----
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {
    delay(100);
    return;
  }
  long heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);   // 右轉變大、左轉變小

  // ---- 3. 有沒有「又多轉滿」一個 90 度？ ----
  // 要從上一個 90 度的位置「再轉滿 90 度」才算，在線的附近晃來晃去不會一直閃
  if (heading >= (quarter + 1) * STEP_DEG) {
    quarter = quarter + 1;
    Serial.print(F("往右轉滿 "));
    Serial.print(STEP_DEG);
    Serial.print(F(" 度！"));
    report();
  } else if (heading <= (quarter - 1) * STEP_DEG) {
    quarter = quarter - 1;
    Serial.print(F("往左轉滿 "));
    Serial.print(STEP_DEG);
    Serial.print(F(" 度！"));
    report();
  }

  delay(20);
}

// 印出現在是第幾個 1/4 圈，並閃一下燈
void report() {
  Serial.print(F("現在是 "));
  Serial.print(quarter);
  Serial.print(F("/4 圈（"));
  Serial.print(quarter * STEP_DEG);
  Serial.print(F(" 度）"));
  if (quarter != 0 && quarter % 4 == 0) {
    Serial.print(F("　剛好轉了一整圈！"));
  }
  Serial.println();

  digitalWrite(LED, HIGH);               // 閃一下
  delay(150);
  digitalWrite(LED, LOW);
}

// 把航向歸零
void zeroHeading() {
  mbcWrite(0x10, 0x01);                  // 把 0x01 寫進第 0x10 號格子 = 「航向歸零」
  delay(50);                             // 歸零後要等一下（至少 30 毫秒）再讀
  quarter = 0;
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
