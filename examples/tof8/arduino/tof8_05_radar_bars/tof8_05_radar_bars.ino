/*
 * 測距者 8 路 × Arduino　範例 5：雷達長條圖
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把 8 顆雷射量到的距離畫成 8 條長條，一顆一條：
 *     長條越長 = 那個方向越空曠（東西越遠）
 *     長條很短 = 那個方向有東西擋著
 *   看一眼就知道「哪邊有障礙物、哪邊可以走」，就像一台小雷達。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把感應器放在桌上，用書本在前面、左邊、右邊擋擋看。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint8_t LASERS = 8;                // 8 顆雷射
const uint16_t NO_TARGET = 1200;         // 距離 1200 以上 = 沒有東西
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

// 每一顆雷射大約朝哪個角度看（負數 = 左邊、正數 = 右邊、0 = 正前方）
const int ANGLE[LASERS] = {-90, -60, -30, 0, 0, 30, 60, 90};

const uint16_t MM_PER_BLOCK = 50;        // 👉 可以改：一格 # 代表 50 mm（5 公分），1200 mm 剛好 24 格

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0，一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！用書本在感應器前面、左邊、右邊擋擋看。"));
}

void loop() {
  // 第 0x21 號格子：8 顆雷射的距離，每顆 2 個數字，共 16 個
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  Serial.println();
  Serial.println(F("====== 雷達畫面（長條越長 = 越空曠）======"));

  for (uint8_t i = 0; i < LASERS; i++) {
    uint16_t mm = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];   // 2 個數字合成距離

    // ---- 1. 印「第幾顆、朝哪個角度」----
    Serial.print(i);
    if (ANGLE[i] > 0)       Serial.print(F("  +"));            // 右邊的角度前面加 +
    else if (ANGLE[i] == 0) Serial.print(F("   "));
    else                    Serial.print(F("  "));
    if (abs(ANGLE[i]) < 10) Serial.print(' ');                 // 對齊用
    Serial.print(ANGLE[i]);
    Serial.print(F("度 |"));

    // ---- 2. 畫長條 ----
    if (mm == FAULT) {                                         // 先看是不是故障
      Serial.println(F("XXXXXXXX  這顆故障"));
      continue;                                                // 這一顆畫完了，換下一顆
    }
    uint16_t blocks = mm / MM_PER_BLOCK;                       // 要畫幾格
    for (uint16_t k = 0; k < blocks; k++) Serial.print('#');

    // ---- 3. 長條後面寫距離 ----
    if (mm >= NO_TARGET) {
      Serial.println(F("> 沒有東西"));
    } else {
      Serial.print(F(" "));
      Serial.print(mm / 10);                                   // mm 除以 10 = 公分
      Serial.println(F(" 公分"));
    }
  }

  delay(500);                                                  // 👉 可以改：每 0.5 秒畫一次
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
