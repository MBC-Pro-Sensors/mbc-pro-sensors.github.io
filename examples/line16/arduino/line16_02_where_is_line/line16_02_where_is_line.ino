/*
 * 循行者 16 路 × Arduino　範例 2：線在哪裡？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器放在白紙上的黑線上方，左右移動感應器，
 *   電腦會告訴你「線在左邊、中間、還是右邊」，還會畫出一條小尺標出線的位置。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   第一次使用、或換了比賽場地，請先跑「範例 4：一鍵校準」，讀數會更準。
 *   這裡說的「左、右」，是指感應器裝在車上（感應面朝下），你站在車子後面往前看的左右。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼

// 👉 可以改：0 = 循黑線（白底黑線），1 = 循白線（黑底白線）
const uint8_t LINE_COLOR = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}                      // 停在這裡
  }

  mbcWrite(0x10, LINE_COLOR);            // 告訴感應器要找黑線還是白線
  Serial.println(F("準備好了！把感應器放在線上，左右移動看看。"));
}

void loop() {
  // 一次讀 7 個數字：感應器把「循線結果」都放在第 0x11 號格子
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  int8_t  position = (int8_t)d[0];       // 線的位置：負數 = 線在左邊，正數 = 線在右邊，0 = 正中間
                                         // 看得到線時在 -15 ~ +15 之間；線從最邊邊跑出去後會變成 ±16
  uint8_t groups   = d[3];               // 看到幾條線：0 = 沒看到線

  if (groups == 0) {
    Serial.println(F("|                 |  看不到線（線跑掉了）"));
  } else {
    // 畫一把小尺：最左邊是 -16，最右邊是 +16，用 # 標出線的位置
    Serial.print('|');
    for (int8_t p = -16; p <= 16; p += 2) {
      // 每一格代表 2 個單位，看線落在哪一格（一共 17 格）
      if (position >= p && position <= p + 1) Serial.print('#');
      else Serial.print(' ');
    }
    Serial.print(F("|  位置 "));
    Serial.print(position);

    if (position > 2)       Serial.println(F("  → 線在右邊，要往右轉"));     // 👉 可以改：2 以內都算「中間」
    else if (position < -2) Serial.println(F("  → 線在左邊，要往左轉"));
    else                    Serial.println(F("  → 線在正中間，直走！"));
  }

  delay(200);                            // 👉 可以改：每 0.2 秒看一次，數字越小更新越快
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
