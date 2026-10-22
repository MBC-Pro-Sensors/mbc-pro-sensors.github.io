/*
 * 掌控者 PS4/PS5 × Arduino　範例 5：手把的狀態
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.5 秒印出跟「按鍵、搖桿」無關的狀態值（編號 06~0B，一次讀 6 個）：
 *     06 已連線（1 = 有）、07 電量 0~100、08 L2 扳機壓多深 0~255、
 *     09 R2 扳機壓多深 0~255、0A 系統鍵（b0 PS 鍵、b1 擷取鍵）、0B 手把類型。
 *   連線狀態有變化時會另外印一行提醒。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。慢慢壓 L2、R2 看數字變化；把手把關機看「已連線」變 0。
 *
 * 小提醒：
 *   壓 L2／R2 時 08／09 會慢慢從 0 變到 255，輕壓、重壓都看得出來。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;
int16_t lastConnected = -1;              // 上一次的連線狀態（-1 = 還沒看過）

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！每 0.5 秒印一次手把狀態。"));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 可以改：每 0.5 秒印一次
    lastPrint = millis();

    // 一次讀編號 06~0B 共 6 個值：[0] 已連線 [1] 電量 [2] L2 [3] R2 [4] 系統鍵 [5] 手把類型
    int16_t v[6];
    if (!mbcReadMany(ADDR, 0x06, 6, v)) { Serial.println(F("讀不到接收器")); return; }

    // 連線狀態有變化就提醒一行
    if (v[0] != lastConnected) {
      Serial.println(v[0] ? F(">>> 手把連上了！") : F(">>> 手把沒有連線"));
      lastConnected = v[0];
    }

    Serial.print(F("已連線 ")); Serial.print(v[0]);
    Serial.print(F(" | 電量 ")); Serial.print(v[1]); Serial.print(F("%"));
    Serial.print(F(" | L2 ")); Serial.print(v[2]);
    Serial.print(F(" R2 ")); Serial.print(v[3]);
    Serial.print(F(" | PS 鍵 ")); Serial.print((v[4] & 0x01) ? 1 : 0);     // 系統鍵 bit0
    Serial.print(F(" 擷取鍵 ")); Serial.print((v[4] & 0x02) ? 1 : 0);     // 系統鍵 bit1
    Serial.print(F(" | 手把："));
    printPadType(v[5]);
    Serial.println();
  }
  delay(50);
}

// 0B 手把類型：0 沒有、1 PS2 數位、2 PS2 類比、3 PS4、4 PS5（或其他）
void printPadType(int16_t t) {
  switch (t) {
    case 0: Serial.print(F("沒有手把")); break;
    case 1: Serial.print(F("PS2 手把（數位模式）")); break;
    case 2: Serial.print(F("PS2 手把（類比模式）")); break;
    case 3: Serial.print(F("PS4 手把")); break;
    case 4: Serial.print(F("PS5 手把（或其他藍牙手把）")); break;
    default: Serial.print(F("不認識"));
  }
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
