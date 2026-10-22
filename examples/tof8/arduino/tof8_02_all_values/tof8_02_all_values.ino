/*
 * 測距者 8 路 × Arduino　範例 2：一次看完所有讀值
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器「整張值表」每 0.5 秒印一次：
 *     01 最近 cm、02 最近方向、03 最空曠 cm、04 最空曠方向、05 正前方 cm、
 *     06 最近 mm、07 初始化位元圖、08 故障位元圖、09 正前方 mm、
 *     10~17 第 1~8 顆 cm、18~1F 第 1~8 顆 mm。
 *   之後的範例只是各挑其中幾格來用，先在這裡把每一格認識一遍。
 *
 * 你需要：測距者 8 路（通用 I2C 版）＋ 你的手或幾本書。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。手從左邊慢慢移到右邊，看數字怎麼從第 1 顆走到第 8 顆。
 *
 * 小提醒：
 *   一次最多讀 16 個，所以分三段：01~09、10~17、18~1F。
 *   120 cm／1200 mm = 那個方向沒東西（或那顆失效）。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿手在感應器前面移動看看。"));
}

void loop() {
  int16_t a[9];                          // 01~09
  int16_t cm[8];                         // 10~17 第 1~8 顆 cm
  int16_t mm[8];                         // 18~1F 第 1~8 顆 mm
  if (!mbcReadMany(ADDR, 0x01, 9, a))  { Serial.println(F("讀不到（01~09）")); delay(500); return; }
  if (!mbcReadMany(ADDR, 0x10, 8, cm)) { Serial.println(F("讀不到（10~17）")); delay(500); return; }
  if (!mbcReadMany(ADDR, 0x18, 8, mm)) { Serial.println(F("讀不到（18~1F）")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 最近距離     ")); Serial.print(a[0]); Serial.println(F(" cm   （8 顆裡最小的；120 = 四周沒東西）"));
  Serial.print(F("02 最近方向     ")); Serial.print(a[1]); Serial.println(F("      （-7 正左 ~ +7 正右，0 = 正前方；±8 = 沒東西）"));
  Serial.print(F("03 最空曠距離   ")); Serial.print(a[2]); Serial.println(F(" cm   （8 顆裡最大的）"));
  Serial.print(F("04 最空曠方向   ")); Serial.print(a[3]); Serial.println(F("      （-7 ~ +7）"));
  Serial.print(F("05 正前方距離   ")); Serial.print(a[4]); Serial.println(F(" cm   （第 4、5 顆平均）"));
  Serial.print(F("06 最近距離     ")); Serial.print(a[5]); Serial.println(F(" mm   （比 cm 精細 10 倍）"));
  Serial.print(F("07 初始化位元圖 "));
  for (uint8_t i = 0; i < 8; i++) Serial.print((a[6] >> i) & 1);     // 左邊印 bit0（第 1 顆）
  Serial.println(F("  （左→右，1 = 開機成功）"));
  Serial.print(F("08 故障位元圖   "));
  for (uint8_t i = 0; i < 8; i++) Serial.print((a[7] >> i) & 1);
  Serial.println(F("  （左→右，1 = 那顆現在失效）"));
  Serial.print(F("09 正前方距離   ")); Serial.print(a[8]); Serial.println(F(" mm"));

  Serial.print(F("10~17 各顆 cm 左 "));
  for (uint8_t i = 0; i < 8; i++) { printPad(cm[i], 4); Serial.print(' '); }
  Serial.println(F(" 右"));
  Serial.print(F("18~1F 各顆 mm 左 "));
  for (uint8_t i = 0; i < 8; i++) { printPad(mm[i], 4); Serial.print(' '); }
  Serial.println(F(" 右   （第 1 顆正左 90°、第 4/5 顆正前、第 8 顆正右 90°）"));

  delay(50);
  delay(450);                            // 每 0.5 秒更新一次
}

// 靠右對齊印數字（寬度 w）
void printPad(int16_t v, uint8_t w) {
  uint8_t len = (v < 0) ? 2 : 1;
  for (int16_t t = (v < 0 ? -v : v); t >= 10; t /= 10) len++;
  for (uint8_t i = len; i < w; i++) Serial.print(' ');
  Serial.print(v);
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
