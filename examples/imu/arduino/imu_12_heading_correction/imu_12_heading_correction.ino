/*
 * IMU 陀螺儀 × Arduino　範例 12：航向修正係數（轉一圈不是剛好 360 度時）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   有些陀螺儀轉整整一圈，讀到的不是剛好 360 度（例如 357 或 363）。
 *   這個範例帶你量出「每轉一圈實際讀到幾度」，算出修正係數，用命令 EA 存進陀螺儀：
 *     1. 陀螺儀平放、前端對齊桌邊 → 按 Enter
 *        （程式先送 EA 0 回到「每圈 360.0」，再量，量到的才是沒修正過的原始角度）
 *     2. 從上往下看「順時針」轉整整 TURNS 圈（預設 5 圈），前端對齊回同一個桌邊 → 按 Enter
 *     3. 讀 03 陀螺 1D 航向累積的角度，算出 EA = 每圈讀到的度數 × 10
 *        例：轉 5 圈讀到 1785 度 → 每圈 357.0 度 → EA = 3570
 *     4. EA 在 3500～3700 才寫進去（超出就請你重做）
 *   寫進去以後，陀螺儀會把讀值放大或縮小成每圈 360 度（讀值 × 360 ÷ 357.0）。
 *   隨時輸入 r 再按 Enter：把 EA 寫回 0（回到每圈 360.0，不修正）。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。不用小車。
 *         一張有直邊的桌子（或一本書）當「對齊線」。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200，照著畫面的提示做。
 *   監控窗右下角選「換行」（或「NL 和 CR 都要」），直接按 Enter 就可以；
 *   選「沒有行結尾」的話，請打任何一個字（例如 g）再按 Enter。
 *
 * 小提醒：
 *   寫入後陀螺儀要「停 3 秒」才存進 EEPROM，請等 3 秒以上再拔電；存好以後斷電也記得。
 *   改了係數，航向會自動歸零一次。
 *   每次量都會先把舊係數清成 0；量完沒寫入（超出範圍、重做）的話，係數就是 360.0。
 *   轉的時候慢慢轉、線先繞好（或把 Arduino 一起轉），圈數一定要數對。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- 👉 可以改的參數 ----
const int TURNS = 5;                     // 要轉幾圈（越多圈越準，最多 80 圈）
const uint16_t EA_MIN = 3500;            // 陀螺儀接受的範圍：每圈 350.0 ~ 370.0 度
const uint16_t EA_MAX = 3700;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("=== IMU 陀螺儀：航向修正係數 EA ==="));
  printMenu();
}

void loop() {
  char ch = waitKey(false);              // 等使用者按 Enter（或打字）
  if (ch == 'r' || ch == 'R') resetCorrection();
  else measure();
  printMenu();
}

void printMenu() {
  Serial.println();
  Serial.println(F("按 Enter 開始量；輸入 r 再按 Enter = 把 EA 寫回 0（每圈 360.0）"));
}

// ---- 量一次、算 EA、寫進去 ----
void measure() {
  // 步驟 1：對齊
  Serial.println(F("步驟 1：陀螺儀平放在桌上，前端（絲印 X 箭頭那端）對齊桌邊，放好不要動，然後按 Enter"));
  char ch = waitKey(false);
  if (ch == 'r' || ch == 'R') { resetCorrection(); return; }

  // 先回到每圈 360.0（不修正），量到的才是原始角度；EA 會順便把航向歸零
  int st = sendEA(0);
  if (st != 0) { Serial.print(F("EA 0 沒成功，狀態 ")); Serial.println(st); return; }
  delay(300);
  Serial.print(F("已歸零，03 陀螺 1D 航向 = ")); Serial.println(mbcRead(ADDR, 0x03));

  // 步驟 2：轉 TURNS 圈
  Serial.print(F("步驟 2：從上往下看「順時針」慢慢轉整整 "));
  Serial.print(TURNS);
  Serial.println(F(" 圈，前端對齊回同一個桌邊，放好不要動，然後按 Enter"));
  waitKey(true);                         // 等的時候每秒印一次目前角度
  delay(300);
  int16_t d = mbcRead(ADDR, 0x03);       // 03 陀螺 1D 航向：一直累積，轉 5 圈約 1800
  if (d == MBC_NONE) { Serial.println(F("讀不到陀螺儀")); return; }

  if (d < 0) {
    Serial.print(F("讀到 ")); Serial.print(d);
    Serial.println(F(" 度（負數）：好像轉成「逆時針」了。請從上往下看順時針轉，重做一次。"));
    return;
  }

  // 步驟 3：算 EA = 每圈讀到的度數 × 10（四捨五入）
  long ea = ((long)d * 10 + TURNS / 2) / TURNS;
  Serial.print(F("轉 ")); Serial.print(TURNS); Serial.print(F(" 圈讀到 ")); Serial.print(d);
  Serial.print(F(" 度 → 每圈 ")); Serial.print(ea / 10); Serial.print('.'); Serial.print(ea % 10);
  Serial.print(F(" 度 → EA = ")); Serial.println(ea);

  if (ea < EA_MIN || ea > EA_MAX) {
    Serial.println(F("超出 3500～3700（每圈 350～370 度）：可能圈數數錯、沒對齊、或轉太快。不寫入，請重做一次。"));
    return;
  }

  // 步驟 4：寫進陀螺儀
  st = sendEA((uint16_t)ea);
  if (st == 0) {
    Serial.print(F("已寫入 EA = ")); Serial.print(ea);
    Serial.println(F("。陀螺儀停 3 秒才存進 EEPROM，請等一下再拔電；航向已歸零。"));
    delay(3500);
    Serial.print(F("存好了。想檢查的話，開範例 3、輸入 z 歸零，再順時針轉 "));
    Serial.print(TURNS); Serial.print(F(" 圈，03 應該讀到約 ")); Serial.println(TURNS * 360L);
  } else {
    Serial.print(F("寫入失敗，狀態 ")); Serial.println(st);
  }
}

// ---- EA 寫回 0 ----
void resetCorrection() {
  int st = sendEA(0);
  if (st == 0) Serial.println(F("已把 EA 寫回 0（每圈 360.0，不修正），航向已歸零。3 秒後存進 EEPROM。"));
  else { Serial.print(F("EA 0 沒成功，狀態 ")); Serial.println(st); }
}

// 送 EA [係數 uint16 小端]：先送低 byte 再送高 byte。回傳 mbcCmd 的狀態
int sendEA(uint16_t v) {
  uint8_t p[2] = {(uint8_t)(v & 0xFF), (uint8_t)(v >> 8)};
  return mbcCmd(ADDR, 0xEA, p, 2, 1500);
}

// 等使用者按 Enter（或打一個字再按 Enter）。回傳打的第一個字（只按 Enter = '\n'）
// showAngle = true 時，等的時候每秒印一次 03 陀螺 1D 航向
char waitKey(bool showAngle) {
  while (Serial.available()) Serial.read();      // 先把之前剩下的字清掉
  unsigned long last = millis();
  while (!Serial.available()) {
    if (showAngle && millis() - last >= 1000) {
      last = millis();
      Serial.print(F("  現在 03 = ")); Serial.println(mbcRead(ADDR, 0x03));
    }
    delay(10);
  }
  char ch = Serial.read();
  delay(50);
  while (Serial.available()) Serial.read();      // 這一行剩下的字（例如換行）不要
  return ch;
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

// ============================================================
// 命令小幫手（範例 11、12 才有）：送命令，等「命令狀態」0B 回報結果。
//   0B = (命令碼 << 8) | 狀態；狀態 0 完成、1 忙碌、2 失敗、3 拒絕。
//   注意：同一個命令連送兩次時，剛送完那一下 0B 可能還是「上一次」的結果。
//   所以先記下 F4（快照序號，每 10 ms 或收到命令後 +1，0~255 繞回），
//   等 F4 比送命令前多 2 以上，那時讀到的 0B 一定是這次命令之後的。
// 回傳：0 完成、1 忙碌（等到 timeoutMs 還沒好）、2 失敗、3 拒絕、-1 讀不到／寫不進去
// ============================================================
int mbcCmd(uint8_t addr, uint8_t code, const uint8_t *p, uint8_t n, uint16_t timeoutMs) {
  int16_t s0 = mbcRead(addr, 0xF4);      // 送命令前的快照序號
  if (s0 == MBC_NONE) return -1;
  if (!mbcWrite(addr, code, p, n)) return -1;
  bool fresh = false;                    // F4 已經多 2 了嗎？（多了以後的 0B 才採信）
  bool gotAny = false;                   // 有沒有讀到過任何東西
  unsigned long start = millis();
  while (true) {
    int16_t s = mbcRead(addr, 0xF4);     // 先讀 F4、再讀 0B（0B 一定不會比 F4 舊）
    int16_t b = mbcRead(addr, 0x0B);
    if (s != MBC_NONE && b != MBC_NONE) {
      gotAny = true;
      if ((uint8_t)((uint8_t)s - (uint8_t)s0) >= 2) fresh = true;
      uint8_t who = (uint16_t)b >> 8;    // 高 byte：最後一個命令碼
      uint8_t state = b & 0xFF;          // 低 byte：狀態
      if (fresh && who == code && state != 1) return state;
    }
    if (millis() - start >= timeoutMs) return gotAny ? 1 : -1;
    delay(10);
  }
}
