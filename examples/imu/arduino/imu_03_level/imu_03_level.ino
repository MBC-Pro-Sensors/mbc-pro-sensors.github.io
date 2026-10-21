/*
 * 陀螺儀 IMU × Arduino　範例 3：水平儀
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器變成「水平儀」：
 *     感應器放平了 → Arduino 板子上的內建小燈（標著 L 的那顆）亮起來
 *     感應器歪了   → 燈熄，電腦告訴你往哪一邊歪、歪了幾度
 *   原理：地心引力一直往下拉，感應器裡的「加速度計」量得到這股力量
 *   往哪個方向，就知道自己有沒有放平。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把感應器放在書本上，慢慢把書的一邊抬高，看數字怎麼變。
 *
 * 小提醒：
 *   感應器平放、直立、側放都可以用，程式會自己找出哪一面朝上。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

const float LEVEL_DEG = 2.0;             // 👉 可以改：歪不到 2 度就算「水平」

const char AXIS_NAME[3] = {'X', 'Y', 'Z'};   // 三個方向的名字

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // 把燈的腳設成「輸出」，才能控制它亮或熄
  mbcBegin();
  delay(500);

  Serial.println(F("請把感應器放在桌上，不要動它……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把感應器放在書上，抬高書的一邊看看。"));
}

void loop() {
  // ---- 1. 讀加速度：第 0x23 號格子，共 6 個數字（X、Y、Z 各 2 個）----
  uint8_t d[6];
  if (!mbcRead(0x23, d, 6)) {            // 讀不到就等一下再試
    delay(100);
    return;
  }

  // 拼成 3 個數字，單位是 mg（1000 mg = 1 倍地心引力）
  // 放平靜止時，「朝上」的那個方向會讀到大約 +1000
  int16_t a[3];
  for (uint8_t i = 0; i < 3; i++) {
    a[i] = (int16_t)(((uint16_t)d[2 * i] << 8) | d[2 * i + 1]);
  }

  // ---- 2. 找出哪個方向朝上（數字的「大小」最大的那個）----
  uint8_t up = 0;
  for (uint8_t i = 1; i < 3; i++) {
    if (abs(a[i]) > abs(a[up])) up = i;
  }

  // ---- 3. 看另外兩個方向歪了幾度 ----
  bool isLevel = true;                   // 先假設是平的
  for (uint8_t i = 0; i < 3; i++) {
    if (i == up) continue;               // 朝上的方向不用看

    // 用「三角函數」把力量換成角度（atan2 是 Arduino 內建的）
    float deg = atan2((float)a[i], (float)abs(a[up])) * 180.0 / PI;

    Serial.print(AXIS_NAME[i]);
    Serial.print(F(" 方向："));
    if (fabs(deg) <= LEVEL_DEG) {
      Serial.print(F("水平　　　　"));
    } else {
      isLevel = false;
      // 數字是負的 → 「+」那一邊比較低；數字是正的 → 「-」那一邊比較低
      Serial.print(F("往 "));
      Serial.print(deg < 0 ? '+' : '-');
      Serial.print(AXIS_NAME[i]);
      Serial.print(F(" 那邊低 "));
      Serial.print(fabs(deg), 1);        // 印到小數點後 1 位
      Serial.print(F(" 度　"));
    }
  }

  // ---- 4. 平的就亮燈 ----
  if (isLevel) {
    digitalWrite(LED, HIGH);
    Serial.println(F("→ 水平！"));
  } else {
    digitalWrite(LED, LOW);
    Serial.println(F("→ 還沒平"));
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
