/*
 * 陀螺儀 IMU × Arduino　範例 6：電子骰子（哪一面朝上？）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器當成一顆骰子：感應器有 6 個面（+X、+Y、+Z、-X、-Y、-Z），
 *   每一面代表 1 ~ 6 點。把感應器翻一翻、放下來，
 *   等它停穩，電腦就會說「擲出 3 點！」，
 *   Arduino 板子上的內建小燈（標著 L 的那顆）也會閃 3 下。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   拿起感應器翻個面，放回桌上不要碰，大約 0.5 秒後就會看到點數。
 *   （杜邦線不要扯太緊，翻面時才不會鬆脫）
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

// 每一面代表幾點（跟真的骰子一樣，相對的兩面加起來 = 7）
//                         +X  +Y  +Z  -X  -Y  -Z
const uint8_t DOTS[6] = {  2,  3,  1,  5,  4,  6 };   // 👉 可以改：換成你喜歡的點數

bool rolling = true;                     // 骰子是不是還在動

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
  Serial.println(F("準備好了！把感應器翻個面放下來，等它停穩。"));
}

void loop() {
  // ---- 1. 讀狀態：第 0x31 號格子，共 9 個數字 ----
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {           // 讀不到就等一下再試
    delay(100);
    return;
  }
  uint8_t flags  = st[0];                // 一排「開關」
  uint8_t upFace = st[4];                // 哪一面朝上：0 ~ 5
  bool still = flags & (1u << 1);        // 第 1 個開關 = 靜止（停穩了）

  // ---- 2. 動起來了 → 骰子在滾 ----
  if (!still) {
    if (!rolling) {                      // 剛開始動，只說一次就好
      rolling = true;
      Serial.println(F("骰子在滾……"));
    }
  }

  // ---- 3. 停穩了 → 公布點數 ----
  if (still && rolling && upFace < 6) {
    rolling = false;
    uint8_t n = DOTS[upFace];
    Serial.print(F("停下來了！擲出 "));
    Serial.print(n);
    Serial.print(F(" 點（朝上的面："));
    printFace(upFace);
    Serial.println(F("）"));

    for (uint8_t i = 0; i < n; i++) {    // 幾點就閃幾下
      digitalWrite(LED, HIGH); delay(200);
      digitalWrite(LED, LOW);  delay(200);
    }
  }

  delay(50);
}

// 把「朝上的面」編號印成看得懂的字
void printFace(uint8_t f) {
  switch (f) {
    case 0:  Serial.print(F("+X")); break;
    case 1:  Serial.print(F("+Y")); break;
    case 2:  Serial.print(F("+Z")); break;
    case 3:  Serial.print(F("-X")); break;
    case 4:  Serial.print(F("-Y")); break;
    case 5:  Serial.print(F("-Z")); break;
    default: Serial.print(F("?"));  break;
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
