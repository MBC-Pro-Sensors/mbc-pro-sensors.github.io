/*
 * 神攝手 ESP32CAM × Arduino　範例 1：跟神攝手打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認神攝手有沒有接好。接好了，會印出它的身分證（裝置 ID、是學校版還是 WRO 版、
 *   協議版本、韌體版本）和狀態旗標 F3（就緒？有沒有找到色塊？）。
 *   每次接新的神攝手，都建議先跑這個範例。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   神攝手開機要幾秒（鏡頭初始化＋螢幕），範例最多等 8 秒。
 *   這支範例學校版、WRO 版都能跑；F0 的高 byte 會告訴你是哪一版。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟神攝手講話

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);

  Serial.println(F("=== 神攝手 ESP32CAM：跟神攝手打招呼 ==="));

  // 等神攝手說「我準備好了」（狀態旗標 F3 的 bit0），最多等 8 秒
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手！請檢查："));
    Serial.println(F("  1. IO3（SDA）接 A4、IO1（SCL）接 A5（ESP32：21 / 22），有沒有接反？"));
    Serial.println(F("  2. 5V、GND 有沒有插緊？神攝手的螢幕有沒有亮？"));
    Serial.println(F("  3. 神攝手燒的是「I2C 版」韌體嗎？（EV3 版、SPIKE 版用同一組腳，不能接 Arduino）"));
    while (true) {}                      // 找不到就停在這裡
  }

  // ---- F0 裝置 ID：低 byte = 出廠位址（0x4C）、高 byte = 版本（1 = 學校版、2 = WRO 版） ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t edition = (uint8_t)((id >> 8) & 0xFF);
  Serial.print(F("找到了！出廠位址 0x"));
  Serial.println((uint8_t)(id & 0xFF), HEX);   // 應該是 4C
  Serial.print(F("版本："));
  if (edition == 1)      Serial.println(F("學校版（取色＋色塊追蹤＋動態偵測，編號 01~0A）"));
  else if (edition == 2) Serial.println(F("WRO 版 HUE12（12 個色點，編號 10~1B、20~2B）"));
  else                   Serial.println(F("不認識的版本"));

  // ---- F1 協議版本（這份範例是照版本 2 寫的） ----
  Serial.print(F("協議版本："));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 韌體版本：主版×100 + 次版（例如 102 = 1.2） ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("韌體版本："));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 狀態旗標 ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("目前狀態："));
  Serial.print(F("  bit0 就緒        ")); Serial.println((st & 0x01) ? F("是") : F("否"));
  Serial.print(F("  bit1 有找到色塊  ")); Serial.println((st & 0x02) ? F("是") : F("否（學校版才有；WRO 版恆 0）"));

  // ---- F5 開機秒數 ----
  Serial.print(F("開機秒數："));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("接線正確，可以開始玩其他範例了！"));
}

void loop() {
  // 這個範例只要在開機時檢查一次，所以 loop 裡什麼都不做
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
