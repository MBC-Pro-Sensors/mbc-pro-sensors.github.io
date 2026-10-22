/*
 * 掌控者 PS2 × Arduino　範例 3：按了哪一顆鍵？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   按下手把上任何一顆鍵，電腦會用中文告訴你按了哪一顆；放開時也會說。
 *   可以同時按好幾顆。
 *   16 顆鍵的狀態放在編號 01（十六鍵）一個數字裡，一顆鍵佔一個「位」（bit）：
 *   按下 = 1、沒按 = 0，就像 16 個電燈開關排成一排。
 *   編號 02~05 是同一批按鍵的「分組版」：方向鍵、形狀鍵、肩鍵、功能鍵各 4 顆，
 *   一組 0~15，同時按就相加（例如 ↑ + → = 1 + 2 = 3）。
 *
 * 你需要：掌控者 PS2 接收器（通用 I2C 版）＋ PS2 手把、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。把手把開機（接收器的燈亮），按按看。
 *
 * 小提醒：
 *   手把中間的 ANALOG 鍵要讓燈亮著（類比模式），搖桿才會有數字。
 *   PS2 手把沒有 PS 鍵，編號 0A 系統鍵恆 0。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

uint16_t lastButtons = 0;                // 上一次看到的十六鍵狀態
int16_t  lastGroup[4] = {0, 0, 0, 0};    // 上一次看到的 02~05 四組

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！按手把上的按鍵看看。"));
}

void loop() {
  // 一次讀編號 01~05 共 5 個值：[0] 十六鍵 [1] 方向鍵 [2] 形狀鍵 [3] 肩鍵 [4] 功能鍵
  int16_t v[5];
  if (!mbcReadMany(ADDR, 0x01, 5, v)) {
    delay(100);                          // 這次沒讀到，等一下再試
    return;
  }
  uint16_t buttons = (uint16_t)v[0];     // 十六鍵：PSX 排列（bit0 SELECT … bit15 ■）

  // ---- 1. 一顆一顆檢查：哪些鍵「剛按下」、哪些「剛放開」 ----
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t mask = 1u << i;             // 第 i 顆鍵的「位」
    bool now    = buttons & mask;        // 現在有沒有按
    bool before = lastButtons & mask;    // 上一次有沒有按
    if (now && !before) { Serial.print(F("按下：")); printButtonName(i); Serial.println(); }
    if (!now && before) { Serial.print(F("放開：")); printButtonName(i); Serial.println(); }
  }
  lastButtons = buttons;                 // 記住這一次，下一次拿來比較

  // ---- 2. 四組分組值有變化就印一行（方向／形狀／肩／功能，各 0~15） ----
  bool changed = false;
  for (uint8_t i = 0; i < 4; i++) if (v[i + 1] != lastGroup[i]) changed = true;
  if (changed) {
    Serial.print(F("    分組 → 方向鍵 ")); Serial.print(v[1]);
    Serial.print(F("  形狀鍵 ")); Serial.print(v[2]);
    Serial.print(F("  肩鍵 ")); Serial.print(v[3]);
    Serial.print(F("  功能鍵 ")); Serial.println(v[4]);
    for (uint8_t i = 0; i < 4; i++) lastGroup[i] = v[i + 1];
  }

  delay(50);                             // 每 0.05 秒看一次，按很快也抓得到
}

// 印出十六鍵裡第 i 位的按鍵名字（PSX 排列）
void printButtonName(uint8_t i) {
  switch (i) {
    case 0:  Serial.print(F("SELECT（PS4：SHARE／PS5：建立）")); break;
    case 1:  Serial.print(F("L3（左搖桿往下壓）")); break;
    case 2:  Serial.print(F("R3（右搖桿往下壓）")); break;
    case 3:  Serial.print(F("START（PS4/PS5：OPTIONS）")); break;
    case 4:  Serial.print(F("↑ 方向鍵上")); break;
    case 5:  Serial.print(F("→ 方向鍵右")); break;
    case 6:  Serial.print(F("↓ 方向鍵下")); break;
    case 7:  Serial.print(F("← 方向鍵左")); break;
    case 8:  Serial.print(F("L2")); break;
    case 9:  Serial.print(F("R2")); break;
    case 10: Serial.print(F("L1")); break;
    case 11: Serial.print(F("R1")); break;
    case 12: Serial.print(F("▲ 三角形")); break;
    case 13: Serial.print(F("● 圓圈")); break;
    case 14: Serial.print(F("✕ 叉叉")); break;
    case 15: Serial.print(F("■ 正方形")); break;
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
