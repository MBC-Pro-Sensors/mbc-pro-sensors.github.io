/*
 * IMU 陀螺儀 × Arduino　範例 4：轉多快？翹多高？（角速度與俯仰角）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.2 秒印出編號 05~07（一次讀 3 個）：
 *     05 轉向軸角速度：繞「轉向軸」轉多快，單位 0.1 度/秒（123 = 12.3 度/秒），正負跟航向同向
 *     06 俯仰角：板子繞絲印 X 軸翹起幾度（平放 = 0）
 *     07 俯仰角速度：俯仰角變化多快，單位 0.1 度/秒
 *   轉向軸角速度超過 30 度/秒時會提醒「轉太快」。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。原地轉陀螺儀看 05；把一邊翹起來看 06。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int FAST_DPS = 30;                 // 👉 可以改：超過幾度/秒算「轉太快」

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！轉轉看、翹翹看。"));
}

void loop() {
  if (millis() - lastPrint >= 200) {     // 👉 可以改：每 0.2 秒印一次
    lastPrint = millis();

    // 一次讀編號 05~07 共 3 個值：[0] 轉向軸角速度 [1] 俯仰角 [2] 俯仰角速度
    int16_t v[3];
    if (!mbcReadMany(ADDR, 0x05, 3, v)) { Serial.println(F("讀不到陀螺儀")); return; }

    Serial.print(F("轉向角速度 "));
    Serial.print(v[0] / 10); Serial.print('.'); Serial.print(abs(v[0] % 10));   // 0.1 度/秒 → 印成 12.3
    Serial.print(F(" 度/秒 | 俯仰角 "));
    Serial.print(v[1]);
    Serial.print(F(" 度 | 俯仰角速度 "));
    Serial.print(v[2] / 10); Serial.print('.'); Serial.print(abs(v[2] % 10));
    Serial.print(F(" 度/秒"));
    if (abs(v[0]) > FAST_DPS * 10) Serial.print(F("   <<< 轉太快！"));
    Serial.println();
  }
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
