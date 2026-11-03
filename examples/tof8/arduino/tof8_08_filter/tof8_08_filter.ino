/*
 * 測距者 8 路（命令範例 02）× Arduino：濾波：讓數字比較穩
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用命令 E1 改濾波，看同一個東西擺著不動時，數字跳動多少：
 *     0 關（每次量到多少就是多少，反應最快、跳最多）
 *     1 最近 3 筆取中位數（出廠）
 *     2 最近 5 筆取中位數（最穩，反應慢一點）
 *
 * 你需要：測距者 8 路 通用 I2C 版 ＋ 一個盒子或白板，放在正前方（第 4、5 顆雷射的方向）約 50 公分，擺好不要動。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200，輸入 0～2 按 Enter 換濾波。每秒印一次「1 秒內最大−最小」＝跳動幾 mm。
 *
 * 小提醒：
 *   中位數會把偶爾一次的怪值濾掉，但東西移動時數字會慢 1～2 次才跟上。比賽要快就用 0，要穩就用 2。
 *   命令表：E0 測距模式、E1 濾波、E2 更新時間、E3 回出廠、E4 距離上限（每個都帶 1 個 byte 參數）。
 *   讀回目前設定：20 測距模式、21 濾波、22 更新時間 ms、23 距離上限 cm。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);
bool mbcCommand(uint8_t cmd, uint8_t arg);
void printSettings();

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  mbcCommand(0xE3, 0xA5);                // 先回出廠設定，從乾淨的狀態開始
  Serial.println(F("輸入 0～2 換濾波（0 關、1 三筆中位數、2 五筆中位數）"));
  printSettings();
}

void loop() {
  if (Serial.available()) {
    int f = Serial.parseInt();
    while (Serial.available()) Serial.read();
    if (f >= 0 && f <= 2) {
      Serial.print(F(">>> 送 E1 ")); Serial.println(f);
      mbcCommand(0xE1, f);
      printSettings();
    }
  }
  // 1 秒內一直讀 正前方 mm，記下最大、最小
  int16_t lo = 32767, hi = -32768;
  unsigned long start = millis();
  while (millis() - start < 1000) {
    int16_t d[9];  // 01 最近 cm、02 最近方向、03 最空曠 cm、04 最空曠方向、05 正前方 cm、06 最近 mm、07 初始化、08 故障、09 正前方 mm
    if (mbcReadMany(ADDR, 0x01, 9, d)) {
      if (d[8] < lo) lo = d[8];
      if (d[8] > hi) hi = d[8];
    }
    delay(20);
  }
  Serial.print(F("正前方 mm ")); Serial.print(lo); Serial.print(F("～")); Serial.print(hi);
  Serial.print(F("，跳動 ")); Serial.print(hi - lo); Serial.println(F(" mm"));
}

// 送一個命令 [命令碼][參數]，然後等感應器做完（狀態旗標 F3 的 bit2「忙碌」變 0），最多等 3 秒
// 測距模式、更新時間、回出廠要重新設定雷射晶片，約 0.1～0.5 秒；濾波、距離上限馬上生效
bool mbcCommand(uint8_t cmd, uint8_t arg) {
  if (!mbcWrite(ADDR, cmd, &arg, 1)) return false;
  delay(20);                             // 讓感應器把命令排進去
  unsigned long start = millis();
  while (millis() - start < 3000) {
    int16_t f = mbcRead(ADDR, 0xF3);
    if (f != MBC_NONE && !(f & 4)) return true;   // bit2 = 0：做完了
    delay(10);
  }
  return false;
}

// 從感應器讀回目前的 4 個設定並印出來（20 測距模式、21 濾波、22 更新時間 ms、23 距離上限 cm）
void printSettings() {
  int16_t s[4];
  if (!mbcReadMany(ADDR, 0x20, 4, s)) { Serial.println(F("讀不到設定")); return; }
  const char *modeName[] = {"一般", "遠距離", "精準", "高速"};
  Serial.print(F("【目前設定】測距模式 ")); Serial.print(s[0]); Serial.print(F(" "));
  Serial.print((s[0] >= 0 && s[0] <= 3) ? modeName[s[0]] : "?");
  Serial.print(F("｜濾波 ")); Serial.print(s[1]);
  Serial.print(F("｜更新時間 ")); Serial.print(s[2]); Serial.print(F(" ms"));
  Serial.print(F("｜距離上限 ")); Serial.print(s[3]); Serial.println(F(" cm"));
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
