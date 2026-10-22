/*
 * 循行者 16 路 × Arduino　範例 4：一鍵校準
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓感應器記住「這個場地」的黑和白。在序列埠監控窗打指令：
 *     b  → 黑色校準（E2）：先把感應器 16 隻眼睛都壓在黑線上再打
 *     w  → 白色校準（E3）：先把感應器放在白底上再打
 *     a  → 5 秒動態校準（E7）：打完之後 5 秒內，把感應器在黑線和白底上來回掃
 *   校準結果存在感應器裡，關機也不會忘。換場地、燈光不一樣時都要重做。
 *
 * 你需要：循行者 16 路（通用 I2C 版）＋ 白紙＋黑色電工膠帶。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200，輸入框打 b、w 或 a 再按 Enter。
 *   程式會等「F3 bit2 校準中」變回 0，再印出「08 校準次數」確認多了 1。
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
  Serial.println(F("=== 循行者 16 路：一鍵校準 ==="));
  Serial.print(F("目前校準次數：")); Serial.println(mbcRead(ADDR, 0x08));
  Serial.println(F("打 b = 黑色校準、w = 白色校準、a = 5 秒動態校準，然後按 Enter"));
}

void loop() {
  if (!Serial.available()) { delay(50); return; }   // 沒有輸入就等
  char c = Serial.read();

  if (c == 'b') {
    Serial.println(F("黑色校準中（感應器要壓在黑線上）..."));
    mbcWrite(ADDR, 0xE2);                // E2 = 把現在看到的記成「黑」
    waitCalibDone(2000);                 // 大約 50 ms，最多等 2 秒
  } else if (c == 'w') {
    Serial.println(F("白色校準中（感應器要放在白底上）..."));
    mbcWrite(ADDR, 0xE3);                // E3 = 把現在看到的記成「白」
    waitCalibDone(2000);
  } else if (c == 'a') {
    Serial.println(F("5 秒動態校準開始！請把感應器在黑線和白底上來回掃..."));
    mbcWrite(ADDR, 0xE7);                // E7 = 5 秒內取最黑最白當參考
    waitCalibDone(8000);                 // 5 秒 + 一點餘裕
  } else {
    return;                              // Enter、空白等其他字元不理它
  }

  // 印出光值讓你確認校準後黑是 0 附近、白是 100 附近
  int16_t light[16];
  if (mbcReadMany(ADDR, 0x10, 16, light)) {
    Serial.print(F("現在 16 路光值：左 "));
    for (uint8_t i = 0; i < 16; i++) { Serial.print(light[i]); Serial.print(' '); }
    Serial.println(F("右"));
  }
  delay(50);
}

// 等「F3 bit2 校準中」變回 0，並印出校準次數
void waitCalibDone(uint16_t ms) {
  unsigned long start = millis();
  delay(20);                             // 先給感應器一點時間把旗標舉起來
  while (millis() - start < ms) {
    int16_t st = mbcRead(ADDR, 0xF3);
    if (st != MBC_NONE && !(st & 0x04)) break;   // bit2 = 0 → 做完了
    Serial.print('.');
    delay(100);
  }
  Serial.println();
  Serial.print(F("校準完成！累計次數：")); Serial.println(mbcRead(ADDR, 0x08));
  Serial.println(F("打 b / w / a 可以再做一次"));
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
