/*
 * 陀螺儀 IMU × Arduino　範例 5：搖一搖偵測
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   拿起感應器用力搖一搖（或快速扭一下），
 *   Arduino 板子上的內建小燈（標著 L 的那顆）就會亮，
 *   電腦會數「你搖了幾下」，還會告訴你剛剛轉得多快。
 *   原理：陀螺儀量得到「轉動的速度」，轉得越快數字越大，
 *   超過一個門檻，就當作「搖了一下」。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *   杜邦線要插緊，搖的時候才不會鬆脫。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   等看到「準備好了」，再拿起感應器搖一搖。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

const float SHAKE_SPEED = 300.0;         // 👉 可以改：轉動速度超過「每秒 300 度」就算搖一下
                                         //    （改小比較容易觸發，改大要搖更用力）

bool shaking = false;                    // 現在是不是正在「搖的那一下」
int  shakeCount = 0;                     // 總共搖了幾下
float peak = 0;                          // 這一下最快轉到多快

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
  Serial.println(F("準備好了！拿起感應器搖一搖。"));
}

void loop() {
  // ---- 1. 讀角速度：第 0x22 號格子，共 6 個數字（X、Y、Z 各 2 個）----
  uint8_t d[6];
  if (!mbcRead(0x22, d, 6)) {            // 讀不到就等一下再試
    delay(100);
    return;
  }

  // 拼成 3 個數字，單位是「0.1 度/秒」，所以要除以 10 才是「度/秒」
  float g[3];
  for (uint8_t i = 0; i < 3; i++) {
    int16_t raw = (int16_t)(((uint16_t)d[2 * i] << 8) | d[2 * i + 1]);
    g[i] = raw / 10.0;
  }

  // 三個方向合起來的「轉動速度」（不管往哪個方向轉，都是正的）
  float speed = sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

  // ---- 2. 判斷是不是「搖了一下」----
  if (!shaking && speed > SHAKE_SPEED) {
    shaking = true;                      // 開始搖了
    peak = speed;
    digitalWrite(LED, HIGH);             // 亮燈
  }
  if (shaking) {
    if (speed > peak) peak = speed;      // 記住這一下最快有多快
    if (speed < SHAKE_SPEED / 2) {       // 慢下來了（低於門檻的一半）→ 這一下結束
      shaking = false;
      shakeCount = shakeCount + 1;
      digitalWrite(LED, LOW);
      Serial.print(F("搖一搖！第 "));
      Serial.print(shakeCount);
      Serial.print(F(" 下（最快每秒轉 "));
      Serial.print((int)peak);
      Serial.println(F(" 度）"));
    }
  }

  delay(10);                             // 看得很頻繁，才不會漏掉很快的一下
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
