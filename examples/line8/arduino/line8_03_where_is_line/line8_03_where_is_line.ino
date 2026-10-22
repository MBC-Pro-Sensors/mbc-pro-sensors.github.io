/*
 * 循行者 8 路 × Arduino　範例 3：線在哪裡？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀「01 線位置」「02 細位置」「03 線寬」三格，畫一把小尺用 # 標出線的位置，
 *   並告訴你該往哪邊轉。這三格就是循線最常用的資料。
 *
 * 你需要：循行者 8 路（通用 I2C 版）＋ 白紙＋黑色電工膠帶（寬約 2 公分）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。把黑線從感應器左邊慢慢移到右邊。
 *
 * 左、右怎麼算？
 *   感應器裝在車上（感應面朝下），你站在車子後面往前看：
 *   線在左邊 = 負數、線在右邊 = 正數、0 = 正中間。第 1 路在最左。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  mbcWrite(ADDR, 0xE0);                  // E0 = 追黑線（白線請改成 0xE1）  👉 可以改
  Serial.println(F("準備好了！把黑線從左邊慢慢移到右邊。"));
}

void loop() {
  int16_t v[3];                          // v[0] = 01 線位置、v[1] = 02 細位置、v[2] = 03 線寬
  if (!mbcReadMany(ADDR, 0x01, 3, v)) {  // 從 01 號起一次讀 3 格
    Serial.println(F("讀不到感應器"));
    delay(500);
    return;
  }
  int16_t pos   = v[0];                  // -8（最左）~ +8（最右）
  int16_t fine  = v[1];                  // -100 ~ +100，比線位置更細
  int16_t width = v[2];                  // 壓到線的路數，0 = 看不到線

  // 畫一把 9 格的小尺：位置 -4 ~ +4 各一格（把 -8~+8 除以 2），# 代表線
  Serial.print('|');
  for (int8_t i = -4; i <= 4; i++) Serial.print((width > 0 && i == pos / 2) ? '#' : ' ');
  Serial.print(F("|  "));

  if (width == 0) {
    Serial.println(F("看不到線（線跑掉了）"));
  } else {
    Serial.print(F("位置 ")); Serial.print(pos);
    Serial.print(F("  細位置 ")); Serial.print(fine);
    Serial.print(F("  線寬 ")); Serial.print(width);
    if      (pos < -1) Serial.println(F("  → 線在左邊，要往左轉"));
    else if (pos >  1) Serial.println(F("  → 線在右邊，要往右轉"));
    else               Serial.println(F("  → 線在正中間，直走！"));
  }
  delay(50);
  delay(150);                            // 每 0.2 秒印一行，看得清楚   👉 可以改
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
