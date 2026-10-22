/*
 * 掌控者 PS2 × Arduino　範例 4：搖桿推多少？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把兩支搖桿的位置變成數字（編號 10 LX、11 LY、12 RX、13 RY）：
 *     放開（在中間）= 0；往右推、往上推 = 正數，推到底 = +100；
 *     往左推、往下推 = 負數，推到底 = -100。
 *   每 0.3 秒印一次數字，並畫成兩個「十字圖」：O 就是搖桿現在的位置。
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
 *   上傳後，打開「序列埠監控窗」，速度選 115200。慢慢推搖桿，看 O 怎麼跑。
 *
 * 小提醒：
 *   搖桿在中間附近輕晃，數字會停在 0，這是接收器故意的（叫做「死區」），放開時車子才不會自己慢慢跑。
 *   手把中間的 ANALOG 鍵要讓燈亮著（類比模式），搖桿才會有數字。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！推推看搖桿。"));
}

void loop() {
  if (millis() - lastPrint >= 300) {     // 👉 可以改：每 0.3 秒印一次
    lastPrint = millis();

    // 一次讀編號 10~13 共 4 個值：[0] LX [1] LY [2] RX [3] RY（-100 ~ +100）
    int16_t s[4];
    if (!mbcReadMany(ADDR, 0x10, 4, s)) { Serial.println(F("讀不到接收器")); return; }

    Serial.print(F("左搖桿 X ")); Serial.print(s[0]);
    Serial.print(F("  Y ")); Serial.print(s[1]);
    Serial.print(F("   |   右搖桿 X ")); Serial.print(s[2]);
    Serial.print(F("  Y ")); Serial.println(s[3]);
    drawCross(s[0], s[1], s[2], s[3]);   // 畫兩個十字圖
  }
  delay(50);
}

// 畫兩個 7×7 的十字圖（左搖桿、右搖桿並排）。中心是 +，搖桿位置畫 O
//   X：-100 ~ +100 → 第 0 ~ 6 格（左 → 右）；Y：上為正，所以 +100 在最上面一列
void drawCross(int lx, int ly, int rx, int ry) {
  int lcol = map(lx, -100, 100, 0, 6), lrow = map(ly, 100, -100, 0, 6);
  int rcol = map(rx, -100, 100, 0, 6), rrow = map(ry, 100, -100, 0, 6);
  for (int row = 0; row < 7; row++) {
    for (int col = 0; col < 7; col++) Serial.print(cell(row, col, lrow, lcol));
    Serial.print(F("     "));            // 兩個圖中間空 5 格
    for (int col = 0; col < 7; col++) Serial.print(cell(row, col, rrow, rcol));
    Serial.println();
  }
  Serial.println();
}

// 決定圖上某一格要印什麼字：O = 搖桿位置、+ = 中心、| 和 - = 十字線、. = 空白
char cell(int row, int col, int posRow, int posCol) {
  if (row == posRow && col == posCol) return 'O';
  if (row == 3 && col == 3) return '+';
  if (col == 3) return '|';
  if (row == 3) return '-';
  return '.';
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
