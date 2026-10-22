/*
 * 測距者 8 路 × Arduino　範例 5：雷達長條圖
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   一次讀「10~17 第 1~8 顆距離 cm」，把 8 個方向畫成 8 條長條：
 *   長條越長 = 那個方向越空曠，很短 = 有東西擋著。看一眼就知道哪邊可以走。
 *   順便讀「08 故障位元圖」，壞掉的那顆標 XX。
 *
 * 你需要：測距者 8 路（通用 I2C 版）＋ 幾本書當障礙物。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。
 *
 * 8 顆雷射朝哪裡看？（站在車後往前看）
 *   第 1 顆 左 90°、第 2 顆 左 60°、第 3 顆 左 30°、第 4、5 顆 正前方、
 *   第 6 顆 右 30°、第 7 顆 右 60°、第 8 顆 右 90°
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int8_t ANGLE[8] = {-90, -60, -30, 0, 0, 30, 60, 90};   // 每顆的角度（負 = 左）

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！放幾本書在感應器旁邊看看。"));
}

void loop() {
  int16_t cm[8];
  int16_t fault = mbcRead(ADDR, 0x08);   // 08 故障位元圖：bit0 = 第 1 顆
  if (!mbcReadMany(ADDR, 0x10, 8, cm) || fault == MBC_NONE) { Serial.println(F("讀不到感應器")); delay(500); return; }

  Serial.println(F("====== 雷達畫面（長條越長 = 越空曠）======"));
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(i + 1);                 // 第幾顆（1~8）
    Serial.print(' ');
    if (ANGLE[i] >= 0) Serial.print('+');
    if (ANGLE[i] > -10 && ANGLE[i] < 10) Serial.print(' ');
    Serial.print(ANGLE[i]); Serial.print(F("度 |"));

    if ((fault >> i) & 1) {              // 這顆失效
      Serial.println(F("XX 故障"));
      continue;
    }
    int8_t n = cm[i] / 5;                // 每 5 公分一格，最多 24 格
    if (n > 24) n = 24;
    for (int8_t k = 0; k < n; k++) Serial.print('#');
    if (cm[i] >= 120) Serial.println(F("> 沒有東西"));
    else { Serial.print(' '); Serial.print(cm[i]); Serial.println(F(" 公分")); }
  }
  delay(50);
  delay(450);                            // 每 0.5 秒重畫一次   👉 可以改
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
