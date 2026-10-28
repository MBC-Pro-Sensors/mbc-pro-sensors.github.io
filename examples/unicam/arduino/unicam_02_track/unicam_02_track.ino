/*
 * 神攝手通用版 v1 × Arduino　範例 2：色塊在哪裡？（顏色追蹤）
 * ------------------------------------------------------------
 * 需要把相機選單切到：相機模式 1 顏色追蹤 ・子選項 ③ 標準（程式開頭也會用 E0 自動切）
 *
 * 這個範例會做什麼？
 *   每 0.2 秒讀色 1~4 的位置與面積，用中文說色塊在「左／中／右」「上／中／下」。
 *   X 0~159（左 → 右）、Y 0~119（上 → 下），X = 0 代表沒找到；面積 = 佔畫面幾 %。
 *
 * 準備：先在相機上「教色」——即時畫面停在「目標 1」按 OK，對準要追的顏色取色；
 *   想追更多色就繼續教目標 2、3、4。
 *
 * 接線（I2C 腳位是暫定，等上機確認）：
 *   神攝手 5V  → 外接 5V（2 A 以上的行動電源或變壓器；不要從 Arduino 的 5V 腳拉大電流）
 *   神攝手 GND → Arduino GND（外接電源的 GND 也要接在一起）
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 要燒「I2C 版」韌體。IO1/IO3 跟樂高串口版共用，韌體二選一。
 *
 * 怎麼看結果：打開「序列埠監控窗」（115200），拿教過的顏色在鏡頭前移動。
 */

#include <Wire.h>

// ==== MBC_UNICAM_TOP_BEGIN ====
const uint8_t ADDR = 0x4C;               // 神攝手的 I2C 位址（門牌號碼）
const int16_t MBC_NONE = -32768;         // 「讀不到／沒有這個值」的代號
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);   // 小幫手的宣告（實作在最下面）
// 一個色塊：x、y 像素（0 = 沒找到）、w、h 像素、count 塊數、area 面積 %；這個模式沒給的值是 -1
struct UcBlob { int16_t x, y, w, h, count, area; };
// 一個取樣點：h 色相碼 0~255、s 飽和度、v 明度 0~100；沒給的是 -1
struct UcSpot { int16_t h, s, v; };
// 循線 14 個值（近／中／遠偏移 -100~100：線在左 = 負）
struct UcLine { int16_t nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_TOP_END ====

const uint8_t MODE = 13;                 // 模式編號 13 = 顏色追蹤 ③ 標準（每色 X、Y、面積、數量）
const uint8_t COLORS = 4;                // 👉 可以改：要看幾色（1~16）
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {        // 最多等 8 秒讓相機開機
    Serial.println(F("找不到神攝手通用版！請檢查："));
    Serial.println(F(" 1. 相機有接外接 5V、GND 有跟 Arduino 接在一起"));
    Serial.println(F(" 2. IO3 接 SDA、IO1 接 SCL（接反了就對調）"));
    Serial.println(F(" 3. 相機燒的是「I2C 版」韌體"));
    while (true) {}                      // 停在這裡
  }
  ucSetMode(1, 3);                       // E0：切到相機模式 1、子選項 3
  Serial.println(F("準備好了！拿教過的顏色到鏡頭前。"));
}

void loop() {
  if (millis() - lastPrint < 200) return; // 👉 可以改：每 0.2 秒印一次
  lastPrint = millis();

  for (uint8_t c = 1; c <= COLORS; c++) {
    UcBlob b;
    if (!ucReadBlob(MODE, c, b)) { Serial.println(F("讀不到神攝手")); return; }
    Serial.print(F("色")); Serial.print(c); Serial.print(F(": "));
    if (b.x == 0) {                      // X = 0：沒找到這個顏色
      Serial.print(F("沒看到        "));
      continue;
    }
    // X：0~159 分成左／中／右；Y：0~119 分成上／中／下（上 = 小）
    if (b.x < 53)       Serial.print(F("左"));
    else if (b.x > 106) Serial.print(F("右"));
    else                Serial.print(F("中"));
    if (b.y < 40)       Serial.print(F("上"));
    else if (b.y > 80)  Serial.print(F("下"));
    else                Serial.print(F("中"));
    Serial.print(F(" X=")); Serial.print(b.x);
    Serial.print(F(" Y=")); Serial.print(b.y);
    Serial.print(F(" 面積")); Serial.print(b.area); Serial.print(F("%"));
    Serial.print(F(" ×")); Serial.print(b.count); Serial.print(F("塊  "));
  }
  Serial.println();
}

// ==== MBC_UNICAM_HELPERS_BEGIN ====
// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）＋「神攝手通用版小幫手」。所有範例都一樣，不用改。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）。UNO/Nano：SDA=A4、SCL=A5；ESP32：GPIO21／22
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時不會卡住
#endif
}

// 由編號 id 起連續讀 k 個值（1~16）放進 out；全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte）
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 相機（ESP32）一定要等 2 ms 再讀
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();
    uint16_t hi = Wire.read();
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值；讀不到回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]；成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（F3 的 bit0 = 1），最多等 ms 毫秒
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}

// ---------- 神攝手通用版 ----------

// 切相機模式（E0）：camMode 1~4、sub 子選項（照順序給色號填 0）。會存進相機
bool ucSetMode(uint8_t camMode, uint8_t sub) {
  uint8_t p[2] = { camMode, sub };
  bool ok = mbcWrite(ADDR, 0xE0, p, 2);
  delay(50);                             // 給相機一點時間換模式
  return ok;
}

// 改取樣點位置（E1）：n 第幾點 1~20、x 0~159、y 0~119。會存進相機
bool ucSetPoint(uint8_t n, uint8_t x, uint8_t y) {
  uint8_t p[3] = { n, x, y };
  return mbcWrite(ADDR, 0xE1, p, 3);
}

// 相機目前的模式編號（例 13 = 顏色追蹤 ③）；讀不到回傳 MBC_NONE
int16_t ucModeNumber() {
  return mbcRead(ADDR, 0x01);
}

// 顏色追蹤（模式 11~14）每色佔幾個值 / values per colour
uint8_t ucStride(uint8_t mode) {
  if (mode == 11) return 2;              // X Y
  if (mode == 12) return 3;              // X Y 數量
  if (mode == 13) return 4;              // X Y 面積 數量
  if (mode == 14) return 6;              // X Y 寬 高 數量 面積
  return 0;
}

// 讀色 color（1~16）的色塊；mode = 你切的模式編號 11~14。讀到回傳 true
bool ucReadBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  uint8_t k = ucStride(mode);
  if (k == 0 || color < 1 || color > 16) return false;
  int16_t d[6];
  if (!mbcReadMany(ADDR, 0x10 + (color - 1) * k, k, d)) return false;
  b.x = d[0]; b.y = d[1];
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 12) b.count = d[2];
  if (mode == 13) { b.area = d[2]; b.count = d[3]; }
  if (mode == 14) { b.w = d[2]; b.h = d[3]; b.count = d[4]; b.area = d[5]; }
  return true;
}

// 16 個目標色裡找到幾色（模式 11~14）
int16_t ucColorsFound(uint8_t mode) {
  uint8_t k = ucStride(mode);
  if (k == 0) return MBC_NONE;
  return mbcRead(ADDR, 0x10 + 16 * k);
}

// 讀取樣點 point（1~16）；mode = 21、22、23
bool ucReadSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (mode < 21 || mode > 23 || point < 1 || point > 16) return false;
  uint8_t k = mode - 20;                 // 21 → 1 個值、22 → 2、23 → 3
  int16_t d[3];
  if (!mbcReadMany(ADDR, 0x10 + (point - 1) * k, k, d)) return false;
  p.h = d[0];
  p.s = (k >= 2) ? d[1] : -1;
  p.v = (k >= 3) ? d[2] : -1;
  return true;
}

// 照順序給色號（模式 30）：hues 放由左到右每塊的色相碼（最多 maxN 個、≤16），回傳放了幾個；total = 總塊數
uint8_t ucReadOrder(int16_t *hues, uint8_t maxN, int16_t &total) {
  int16_t d[16];
  total = mbcRead(ADDR, 0x20);
  if (total == MBC_NONE || !mbcReadMany(ADDR, 0x10, 16, d)) { total = MBC_NONE; return 0; }
  uint8_t n = (total < 16) ? (uint8_t)total : 16;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = d[i];
  return n;
}

// 循線（模式 41~43）：一次讀 14 個值
bool ucReadLine(UcLine &l) {
  int16_t d[14];
  if (!mbcReadMany(ADDR, 0x10, 14, d)) return false;
  l.nearOff = d[0]; l.midOff = d[1]; l.farOff = d[2]; l.angle = d[3];
  l.width = d[4]; l.flags = d[5]; l.junction = d[6]; l.junctionDist = d[7];
  l.leftEdge = d[8]; l.rightEdge = d[9]; l.farWidth = d[10];
  l.marker = d[11]; l.markerCount = d[12]; l.lost = d[13];
  return true;
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
uint8_t ucColorCode(int16_t h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_HELPERS_END ====
