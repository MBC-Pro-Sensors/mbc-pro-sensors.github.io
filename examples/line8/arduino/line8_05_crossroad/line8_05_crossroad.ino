/*
 * 循行者 8 路 × Arduino　範例 5：十字路口偵測
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用「04 線段數」和「05 壓線位元圖」判斷路口：
 *     - 線段數 ≥ 2 → 看到岔路（兩段以上的線）
 *     - 壓線位元圖 8 個 bit 幾乎全亮（線寬很大）→ 正壓在十字路口的橫線上
 *   遇到路口，板子內建的小燈（L）快閃 3 下，並數「這是第幾個路口」。
 *   比賽常常要「走到第 3 個路口右轉」，這個範例就是在學數路口。
 *
 * 你需要：循行者 8 路（通用 I2C 版）＋ 畫了十字路口的白紙。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。讓感應器沿著線經過路口，看燈和訊息。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // ESP32 開發板的內建燈通常接在 GPIO 2
#endif

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const uint8_t CROSS_MIN_WIDTH = 6;       // 壓線位元圖裡亮的路數 ≥ 這個數就算十字路口   👉 可以改

int  crossCount = 0;                     // 數到第幾個路口
bool onCross    = false;                 // 現在是不是正在路口上（避免同一個路口數很多次）

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！讓感應器沿著線經過路口看看。"));
}

void loop() {
  int16_t v[2];                          // v[0] = 04 線段數、v[1] = 05 壓線位元圖
  if (!mbcReadMany(ADDR, 0x04, 2, v)) { delay(100); return; }
  int16_t groups = v[0];
  uint8_t bitmap = (uint8_t)v[1];        // bit0 = 第 1 路（最左）… bit7 = 第 8 路

  // 數一數位元圖裡有幾個 1（= 幾路壓在線上）
  uint8_t lit = 0;
  for (uint8_t i = 0; i < 8; i++) if (bitmap & (1u << i)) lit++;

  bool isCross = (lit >= CROSS_MIN_WIDTH) || (groups >= 2);   // 很寬 或 兩段以上 → 路口

  if (isCross && !onCross) {             // 剛「進入」路口才算一次
    onCross = true;
    crossCount++;
    Serial.print(F("第 ")); Serial.print(crossCount); Serial.print(F(" 個路口！  位元圖 "));
    for (uint8_t i = 0; i < 8; i++) Serial.print((bitmap >> i) & 1);   // 左→右 = 第 1~8 路
    Serial.print(F("  線段數 ")); Serial.println(groups);
    for (uint8_t k = 0; k < 3; k++) {    // 快閃 3 下
      digitalWrite(LED_BUILTIN, HIGH); delay(80);
      digitalWrite(LED_BUILTIN, LOW);  delay(80);
    }
  } else if (!isCross && onCross) {
    onCross = false;                     // 離開路口了，下一個路口才能再數
  }

  // 平常：看到線就亮燈、看不到就熄
  digitalWrite(LED_BUILTIN, lit > 0 ? HIGH : LOW);
  delay(50);
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
