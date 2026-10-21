/*
 * 測距者 2 路 × Arduino　範例 2：左右兩邊各多遠？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   測距者 2 路有左、右兩顆雷射，都朝正前方看。
 *   這個範例把兩顆量到的距離印出來（單位：公分），並用長條畫出來：
 *     數字      = 前面的東西有多遠
 *     沒有東西  = 這一邊 120 公分內什麼都沒有
 *     故障      = 這一顆雷射壞了
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   用手只擋住左邊的雷射，再只擋右邊，看看哪一邊的數字在變。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint16_t NO_TARGET = 1200;         // 量到 1200 以上 = 這一邊沒有東西
const uint16_t FAULT = 0xFFFF;           // 收到 0xFFFF（65535）= 這一顆故障

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 雷射開機這段時間距離都是 0，一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！用手擋住左邊或右邊的雷射看看。"));
}

void loop() {
  // 第 0x11 號格子：4 個數字
  //   第 0、1 個合起來 = 左邊的距離（mm）
  //   第 2、3 個合起來 = 右邊的距離（mm）
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];   // 2 個數字合成一個距離
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  Serial.print(F("左 "));
  printSide(left);
  Serial.print(F("   右 "));
  printSide(right);
  Serial.println();

  delay(300);                            // 👉 可以改：每 0.3 秒看一次
}

// 印出一邊的距離：短長條 + 公分。判斷順序：先看故障，再看有沒有東西，最後才是距離
void printSide(uint16_t mm) {
  if (mm == FAULT) {
    Serial.print(F("[    故障    ] 這顆雷射壞了"));
    return;
  }
  if (mm >= NO_TARGET) {
    Serial.print(F("[############] 沒有東西"));
    return;
  }
  // 畫長條：每 100 mm（10 公分）一格，最多 12 格。越長 = 越遠
  Serial.print('[');
  uint8_t blocks = mm / 100;
  for (uint8_t k = 0; k < 12; k++) Serial.print(k < blocks ? '#' : ' ');
  Serial.print(F("] "));
  Serial.print(mm / 10);                 // mm 除以 10 = 公分
  Serial.print(F(" 公分"));
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
