/*
 * 測距者 2 路 × Arduino　範例 3：左邊近還是右邊近？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀「01 左 cm」「02 右 cm」「03 較近 cm」三格，把兩顆雷射的距離畫成兩條長條，
 *   並告訴你東西在哪一邊比較近。這是避障、找對手最常用的三格。
 *
 * 你需要：測距者 2 路（通用 I2C 版）＋ 一個盒子或你的手。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。拿手從左邊移到右邊，看長條怎麼變。
 *
 * 左、右怎麼算？
 *   感應器裝在車頭（雷射朝前），你站在車子後面往前看的左、右。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿手從左邊移到右邊看看。（長條越長 = 越遠）"));
}

void loop() {
  int16_t v[3];                          // v[0] = 01 左 cm、v[1] = 02 右 cm、v[2] = 03 較近 cm
  if (!mbcReadMany(ADDR, 0x01, 3, v)) { Serial.println(F("讀不到感應器")); delay(500); return; }
  int16_t left = v[0], right = v[1], nearest = v[2];

  Serial.print(F("左 "));
  printBar(left);
  Serial.print(F("  右 "));
  printBar(right);

  if (nearest >= 120)        Serial.println(F("  → 120 公分內沒有東西"));
  else if (left < right)     { Serial.print(F("  → 左邊比較近：")); Serial.print(nearest); Serial.println(F(" 公分")); }
  else if (right < left)     { Serial.print(F("  → 右邊比較近：")); Serial.print(nearest); Serial.println(F(" 公分")); }
  else                       { Serial.print(F("  → 正前方："));     Serial.print(nearest); Serial.println(F(" 公分")); }

  delay(50);
  delay(150);                            // 每 0.2 秒印一行   👉 可以改
}

// 把 0~120 公分畫成 0~12 格的長條
void printBar(int16_t cm) {
  int8_t n = cm / 10;
  if (n > 12) n = 12;
  Serial.print('[');
  for (int8_t i = 0; i < 12; i++) Serial.print(i < n ? '#' : '.');
  Serial.print(']');
  if (cm < 10)  Serial.print(' ');
  if (cm < 100) Serial.print(' ');
  Serial.print(cm);
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
