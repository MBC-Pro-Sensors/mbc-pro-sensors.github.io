/*
 * 神攝手 ESP32CAM × Arduino　範例 3：這是什麼顏色？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀取樣點 S1 的顏色（編號 01~03，一次讀 3 個），用中文說出顏色名字：
 *     01 色相 0~100：紅 0/100、黃約 17、綠約 33、青約 50、藍約 67、紫約 83；
 *        -1 = 黑白灰（飽和度太低，沒有顏色）
 *     02 飽和度 0~100：越高顏色越鮮豔
 *     03 明度 0~100：越高越亮（黑白灰時用它分黑／灰／白）
 *   顏色有變才印，畫面不會洗版。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 準備：
 *   先在神攝手背面的螢幕設定好取樣點 S1 和要追的顏色（設定存在機身）。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。拿不同顏色的紙到取樣點前面。
 *
 * 小提醒：
 *   這支是「學校版」韌體用的（編號 01~0A）。WRO 版（HUE12）請看範例 6。
 *   分界的數字（例如黃 12~23）可以依你的場地光線調整。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

int16_t lastHue = -99, lastSat = -1, lastVal = -1;   // 上一次的值（故意用不可能的數字，第一次一定會印）

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿東西到取樣點前面。"));
}

void loop() {
  // 一次讀編號 01~03 共 3 個值：[0] 色相 [1] 飽和度 [2] 明度
  int16_t c[3];
  if (!mbcReadMany(ADDR, 0x01, 3, c)) { delay(100); return; }

  // 變化夠大才印（色相差 3 以上、或飽和／明度差 10 以上）
  if (abs(c[0] - lastHue) >= 3 || abs(c[1] - lastSat) >= 10 || abs(c[2] - lastVal) >= 10 || (c[0] < 0) != (lastHue < 0)) {
    lastHue = c[0]; lastSat = c[1]; lastVal = c[2];
    Serial.print(F("顏色："));
    if (c[0] < 0) {                      // 色相 -1：沒有顏色，用明度分黑／灰／白
      if (c[2] < 30)      Serial.print(F("黑"));
      else if (c[2] < 70) Serial.print(F("灰"));
      else                Serial.print(F("白"));
    } else {
      printHueName(c[0]);
    }
    Serial.print(F("   （色相 ")); Serial.print(c[0]);
    Serial.print(F("  飽和 ")); Serial.print(c[1]);
    Serial.print(F("  明度 ")); Serial.print(c[2]); Serial.println(F("）"));
  }
  delay(50);
}

// 把色相 0~100 變成顏色名字（紅 0/100、黃約 17、綠約 33、青約 50、藍約 67、紫約 83）
void printHueName(int16_t hue) {
  if (hue < 0)        Serial.print(F("黑白灰（沒有顏色）"));
  else if (hue < 6)   Serial.print(F("紅"));
  else if (hue < 12)  Serial.print(F("橘"));
  else if (hue < 23)  Serial.print(F("黃"));
  else if (hue < 42)  Serial.print(F("綠"));
  else if (hue < 58)  Serial.print(F("青"));
  else if (hue < 75)  Serial.print(F("藍"));
  else if (hue < 92)  Serial.print(F("紫"));
  else                Serial.print(F("紅"));
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
