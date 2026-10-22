/*
 * 掌控者 PS4/PS5 × Arduino　範例 7：讓手把震動
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   按住 ✕ 鍵，手把就震動（力道 200）；放開就停。
 *   壓 R2 扳機，壓多深震多大（R2 的 0~255 直接當震動強度）。
 *   用的是命令 E5：寫 [0xE5][強度 0~255]，0 = 停；每送一次震 200 ms，
 *   所以按住時程式每 150 ms 再送一次，震動才會連續。
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
 *   上傳後按 ✕ 或壓 R2，手把會震。序列埠監控窗（115200）會印出送出的強度。
 *
 * 小提醒：
 *   PS2 接收器收到 E5 會回 ACK 但忽略（PS2 手把震動已關閉），這個範例只對 PS4/PS5 有作用。
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const uint8_t BTN_CROSS = 14;            // ✕ 在十六鍵的第 14 位
const uint8_t RUMBLE_BY_CROSS = 200;     // 👉 可以改：按 ✕ 時的震動強度 0~255

uint8_t lastSent = 0;                    // 上一次送出的強度
unsigned long lastSend = 0;              // 上一次送的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！按 ✕ 或壓 R2，手把會震動。"));
}

void loop() {
  // 一次讀編號 01 ~ 09 共 9 個值：v[0] = 01 十六鍵、v[8] = 09 R2 類比
  int16_t v[9];
  if (!mbcReadMany(ADDR, 0x01, 9, v)) { delay(100); return; }

  uint8_t strength = 0;                                  // 這一圈要送的強度
  if ((uint16_t)v[0] & (1u << BTN_CROSS)) strength = RUMBLE_BY_CROSS;   // 按住 ✕
  if (v[8] > strength) strength = (uint8_t)v[8];         // R2 壓更深就用 R2 的值

  // 強度有變、或按住超過 150 ms，就送一次 E5（每次只震 200 ms，要一直補送）
  if (strength != lastSent || (strength > 0 && millis() - lastSend >= 150)) {
    mbcWrite(ADDR, 0xE5, &strength, 1);  // 命令 E5：[強度]
    lastSend = millis();
    if (strength != lastSent) {
      Serial.print(F("震動強度 → ")); Serial.println(strength);
    }
    lastSent = strength;
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
