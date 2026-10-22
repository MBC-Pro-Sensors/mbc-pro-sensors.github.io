/*
 * 神攝手 ESP32CAM × Arduino　範例 6：WRO 版：12 個色點一次看
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   這支是給「WRO 版（HUE12）」韌體用的：畫面上 12 個色點 S1~S12，每 0.5 秒印成 12 格。
 *     10~1B：S1~S12 顏色碼（一次讀 12 個）
 *        0~239 彩色（色相環縮成 0~239）、240~245 白、246~250 黑、255 = 這個色點沒開
 *     20~2B：S1~S12 鮮豔度 0~255（一次讀 12 個；黑 = 1、沒開 = 0）
 *   每一格印出顏色名字＋顏色碼＋鮮豔度。
 *
 * 你需要：神攝手 ESP32CAM（WRO 版 HUE12 的 I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 準備：
 *   先在神攝手背面的螢幕開啟要用的色點 S1~S12，並對準場地。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   學校版韌體讀 10~2B 會全部是 -32768（空號），請改跑範例 2~5。
 *   顏色碼 255 只代表「色點沒開」；色點有開但沒對到東西時，仍會回該點目前的顏色。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }

  // 確認是 WRO 版（F0 高 byte = 2）；學校版也能跑，只是會讀到空號
  int16_t id = mbcRead(ADDR, 0xF0);
  if (((id >> 8) & 0xFF) != 2) Serial.println(F("注意：這台不是 WRO 版韌體，10~2B 會是空號（-32768）。"));
  Serial.println(F("準備好了！每 0.5 秒印一次 12 個色點。"));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 可以改：每 0.5 秒印一次
    lastPrint = millis();

    int16_t code[12], vivid[12];
    if (!mbcReadMany(ADDR, 0x10, 12, code))  { Serial.println(F("讀不到（顏色碼）")); return; }   // 10~1B
    if (!mbcReadMany(ADDR, 0x20, 12, vivid)) { Serial.println(F("讀不到（鮮豔度）")); return; }   // 20~2B

    Serial.println(F("---- S1 ~ S12 ----"));
    for (uint8_t i = 0; i < 12; i++) {
      Serial.print(F("S")); Serial.print(i + 1); Serial.print(i < 9 ? F("  ") : F(" "));
      printCodeName(code[i]);
      Serial.print(F("  碼 ")); Serial.print(code[i]);
      Serial.print(F("  鮮豔 ")); Serial.println(vivid[i]);
    }
  }
  delay(50);
}

// 顏色碼 → 名字：0~239 彩色（把 0~239 當成色相環 0~360 度）、240~245 白、246~250 黑、255 沒開
void printCodeName(int16_t c) {
  if (c == MBC_NONE)   { Serial.print(F("（空號）")); return; }
  if (c == 255)        { Serial.print(F("沒開    ")); return; }
  if (c >= 246)        { Serial.print(F("黑      ")); return; }
  if (c >= 240)        { Serial.print(F("白      ")); return; }
  int deg = (int)((long)c * 360 / 240); // 0~239 → 0~359 度
  if (deg < 15)        Serial.print(F("紅      "));
  else if (deg < 45)   Serial.print(F("橘      "));
  else if (deg < 75)   Serial.print(F("黃      "));
  else if (deg < 165)  Serial.print(F("綠      "));
  else if (deg < 200)  Serial.print(F("青      "));
  else if (deg < 265)  Serial.print(F("藍      "));
  else if (deg < 330)  Serial.print(F("紫      "));
  else                 Serial.print(F("紅      "));
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
