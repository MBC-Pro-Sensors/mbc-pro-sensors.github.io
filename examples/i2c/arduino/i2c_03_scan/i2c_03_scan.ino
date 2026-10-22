/*
 * MBC 通用 I2C × Arduino　共用範例 3：掃描線上有哪些裝置
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把 I2C 位址 0x08~0x77 全部敲一遍，有回應的就讀 F0「裝置 ID」認出是哪一款 MBC 產品
 *   （不是 MBC 的裝置會印「其他裝置」）。接線有沒有通、位址換成什麼、同一條線上接了幾個，
 *   一跑就知道。每 3 秒掃一次。
 *
 * 你需要：任何 MBC 通用 I2C 版感應器（可以同時接好幾個，位址不同就行）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。
 */

#include <Wire.h>

const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC 通用 I2C：掃描 ==="));
}

void loop() {
  uint8_t found = 0;
  Serial.println(F("掃描 0x08 ~ 0x77 ..."));
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    Wire.beginTransmission(a);           // 敲門
    if (Wire.endTransmission() != 0) continue;   // 沒人應門 → 下一個
    found++;
    Serial.print(F("  0x"));
    if (a < 0x10) Serial.print('0');
    Serial.print(a, HEX);
    Serial.print(F("  "));

    int16_t id = mbcRead(a, 0xF0);       // 讀裝置 ID
    if (id == MBC_NONE) { Serial.println(F("其他裝置（不是 MBC，或不是通用 I2C 版）")); continue; }
    uint8_t factory = (uint8_t)(id & 0xFF);
    uint8_t hw0     = (uint8_t)((id >> 8) & 0xFF);
    printProduct(factory, hw0);
    int16_t fw = mbcRead(a, 0xF2);
    Serial.print(F("  韌體 ")); Serial.print(fw / 100); Serial.print('.'); Serial.print(fw % 100);
    Serial.print(F("  協議 v")); Serial.print(mbcRead(a, 0xF1));
    if (factory != a) { Serial.print(F("  （位址已換過，出廠是 0x")); Serial.print(factory, HEX); Serial.print(')'); }
    Serial.println();
  }
  if (!found) Serial.println(F("  沒有任何裝置：檢查 SDA/SCL 有沒有接反、VCC/GND 有沒有插緊"));
  Serial.print(F("共 ")); Serial.print(found); Serial.println(F(" 個裝置"));
  delay(50);
  delay(2950);
}

// 用出廠位址＋hw[0] 認產品
void printProduct(uint8_t factory, uint8_t hw0) {
  if      (factory == 0x16 && hw0 == 8)  Serial.print(F("循行者 8 路"));
  else if (factory == 0x16 && hw0 == 16) Serial.print(F("循行者 16 路"));
  else if (factory == 0x29 && hw0 == 2)  Serial.print(F("測距者 2 路"));
  else if (factory == 0x29 && hw0 == 8)  Serial.print(F("測距者 8 路"));
  else if (factory == 0x42 && hw0 == 1)  Serial.print(F("掌控者 PS2"));
  else if (factory == 0x42 && hw0 == 2)  Serial.print(F("掌控者 PS4"));
  else if (factory == 0x6A)              Serial.print(F("IMU 陀螺儀"));
  else if (factory == 0x4C && hw0 == 1)  Serial.print(F("神攝手 學校版"));
  else if (factory == 0x4C && hw0 == 2)  Serial.print(F("神攝手 WRO 版"));
  else                                   Serial.print(F("不認識的 MBC 裝置"));
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
