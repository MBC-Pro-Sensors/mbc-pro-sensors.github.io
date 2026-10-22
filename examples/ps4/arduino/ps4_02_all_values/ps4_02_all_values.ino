/*
 * 掌控者 PS4/PS5 × Arduino　範例 2：把所有的值一次印出來
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.5 秒把接收器「值表」裡的每一個值都印出來，讓你看懂每個編號放什麼：
 *     01~0B：按鍵、連線、電量、扳機、系統鍵、手把類型（一次讀 11 個）
 *     10~13：四根搖桿軸 LX、LY、RX、RY（一次讀 4 個）
 *   連續的編號可以「一次讀一串」（mbcReadMany），讀到的是同一瞬間的值。
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
 *   上傳後，打開「序列埠監控窗」，速度選 115200。按按鍵、推搖桿，看哪個數字在變。
 *
 * 小提醒：
 *   第一次用要先配對：接收器開機（或按 Reset）後 30 秒內，PS4 手把長按 SHARE + PS、PS5 手把長按 建立 + PS，直到燈條快閃。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;             // 上一次印的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！每 0.5 秒印一次所有的值。"));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 可以改：每 0.5 秒印一次
    lastPrint = millis();

    // ---- 第一段：編號 01~0B，一次讀 11 個值 ----
    int16_t a[11];
    if (!mbcReadMany(ADDR, 0x01, 11, a)) { Serial.println(F("讀不到（第一段）")); return; }
    // ---- 第二段：編號 10~13，一次讀 4 個值 ----
    int16_t s[4];
    if (!mbcReadMany(ADDR, 0x10, 4, s)) { Serial.println(F("讀不到（第二段）")); return; }

    Serial.println(F("------------------------------"));
    Serial.print(F("01 十六鍵（PSX 排列，二進位）：")); printBits16((uint16_t)a[0]); Serial.println();
    Serial.print(F("02 方向鍵（上1 右2 下4 左8）  ：")); Serial.println(a[1]);
    Serial.print(F("03 形狀鍵（▲1 ●2 ✕4 ■8）     ：")); Serial.println(a[2]);
    Serial.print(F("04 肩鍵  （L2 1 R2 2 L1 4 R1 8）：")); Serial.println(a[3]);
    Serial.print(F("05 功能鍵（SELECT1 L3 2 R3 4 START8）：")); Serial.println(a[4]);
    Serial.print(F("06 已連線（1 = 有）           ：")); Serial.println(a[5]);
    Serial.print(F("07 電量 0~100     ：")); Serial.println(a[6]);
    Serial.print(F("08 L2 類比 0~255              ：")); Serial.println(a[7]);
    Serial.print(F("09 R2 類比 0~255              ：")); Serial.println(a[8]);
    Serial.print(F("0A 系統鍵（b0 PS、b1 擷取）   ：")); Serial.println(a[9]);
    Serial.print(F("0B 手把類型 0~4               ：")); Serial.println(a[10]);
    Serial.print(F("10 LX  11 LY  12 RX  13 RY（±100）："));
    for (uint8_t i = 0; i < 4; i++) { Serial.print(s[i]); Serial.print(i < 3 ? F("  ") : F("")); }
    Serial.println();
  }
  delay(50);
}

// 把 16 位數字印成 16 個 0／1（左邊是 bit15 ■、右邊是 bit0 SELECT）
void printBits16(uint16_t v) {
  for (int8_t i = 15; i >= 0; i--) {
    Serial.print((v >> i) & 1);
    if (i % 4 == 0 && i > 0) Serial.print(' ');   // 每 4 位空一格比較好看
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
