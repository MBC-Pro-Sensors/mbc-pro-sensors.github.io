/*
 * 測距者 2 路 × Arduino　範例 2：一次看完所有讀值
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器「整張值表」每 0.5 秒印一次：
 *     01 左 cm、02 右 cm、03 較近 cm、04 左 mm、05 右 mm、
 *     06 初始化位元圖、07 故障位元圖、08 左右平均 mm。
 *   之後的範例只是各挑其中幾格來用，先在這裡把每一格認識一遍。
 *
 * 你需要：測距者 2 路（通用 I2C 版）＋ 你的手或一個盒子。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。拿手在左邊、右邊的雷射前面移動。
 *
 * 小提醒：
 *   01~08 編號連續，一次讀 8 個值（mbcReadMany），保證是同一瞬間的值。
 *   120 cm／1200 mm = 前面沒東西（或那顆失效）。
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
  Serial.println(F("準備好了！拿手在左右兩顆雷射前面移動看看。"));
}

void loop() {
  int16_t a[8];                          // 放 01~08 這 8 格
  if (!mbcReadMany(ADDR, 0x01, 8, a)) { Serial.println(F("讀不到")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 左距離     ")); Serial.print(a[0]); Serial.println(F(" cm   （0~120，120 = 沒東西）"));
  Serial.print(F("02 右距離     ")); Serial.print(a[1]); Serial.println(F(" cm"));
  Serial.print(F("03 較近距離   ")); Serial.print(a[2]); Serial.println(F(" cm   （左右取比較近的那個）"));
  Serial.print(F("04 左距離     ")); Serial.print(a[3]); Serial.println(F(" mm   （0~1200，比 cm 精細 10 倍）"));
  Serial.print(F("05 右距離     ")); Serial.print(a[4]); Serial.println(F(" mm"));
  Serial.print(F("06 初始化位元圖 ")); Serial.print(a[5]); Serial.println(F("     （bit0 左、bit1 右；3 = 兩顆都開機成功）"));
  Serial.print(F("07 故障位元圖   ")); Serial.print(a[6]); Serial.println(F("     （bit0 左、bit1 右；0 = 都正常）"));
  Serial.print(F("08 左右平均   ")); Serial.print(a[7]); Serial.println(F(" mm   （(左 mm + 右 mm) / 2）"));

  delay(50);                             // 讀完值歇一下
  delay(450);                            // 每 0.5 秒更新一次，比較好看
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
