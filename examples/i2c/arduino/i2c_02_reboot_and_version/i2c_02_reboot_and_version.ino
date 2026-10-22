/*
 * MBC 通用 I2C × Arduino　共用範例 2：讀身分證＋重新開機（F0~F5、EF）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每一款 MBC 通用 I2C 感應器都有同樣的 6 格「身分證」：
 *     F0 裝置 ID（低 byte 出廠位址、高 byte hw[0]）、F1 協議版本、F2 韌體版本、
 *     F3 狀態旗標、F4 更新序號（每 10 ms +1，0~255 循環）、F5 開機秒數
 *   這個範例每秒印一次，並認出是哪一款產品。
 *   在序列埠打 r 按 Enter → 送 [EF][0xA5] 重新開機，你會看到 F5 開機秒數歸零、F4 繼續跳。
 *
 * 你需要：任何一款 MBC 通用 I2C 版感應器。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。
 *
 * 小提醒：
 *   F4 一圈只有 2.56 秒，不能拿來判斷有沒有重開機；要看 F5 有沒有變小。
 *   F4 不動 = 感應器忙（例如校準中）。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 感應器位址   👉 改這裡（循行者 0x16、測距者 0x29、掌控者 0x42、IMU 0x6A、神攝手 0x4C）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC 通用 I2C：身分證＋重新開機 ==="));
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請確認 ADDR 和接線（可以先跑 i2c_03_scan 掃描）。"));
    while (true) {}
  }
  Serial.println(F("打 r 按 Enter = 重新開機"));
}

void loop() {
  if (Serial.available() && Serial.read() == 'r') {
    uint8_t magic = 0xA5;
    mbcWrite(ADDR, 0xEF, &magic, 1);     // [EF][A5] = 重新開機；其他數字會被忽略
    Serial.println(F(">>> 已送重新開機，看 F5 會不會歸零"));
    delay(500);
  }

  int16_t f[6];                          // F0~F5 編號連續，一次讀 6 格
  if (!mbcReadMany(ADDR, 0xF0, 6, f)) { Serial.println(F("（重開機中／讀不到）")); delay(500); return; }

  uint8_t factory = (uint8_t)(f[0] & 0xFF);
  uint8_t hw0     = (uint8_t)((f[0] >> 8) & 0xFF);
  Serial.println(F("---------------------------"));
  Serial.print(F("F0 裝置 ID    出廠位址 0x")); Serial.print(factory, HEX);
  Serial.print(F("  hw[0] = ")); Serial.print(hw0);
  Serial.print(F("  → ")); printProduct(factory, hw0); Serial.println();
  Serial.print(F("F1 協議版本   ")); Serial.println(f[1]);
  Serial.print(F("F2 韌體版本   ")); Serial.print(f[2] / 100); Serial.print('.'); Serial.println(f[2] % 100);
  Serial.print(F("F3 狀態旗標   0b"));
  for (int8_t i = 7; i >= 0; i--) Serial.print((f[3] >> i) & 1);
  Serial.println(F("  （bit0 = 就緒，其他 bit 看各產品說明）"));
  Serial.print(F("F4 更新序號   ")); Serial.print(f[4]); Serial.println(F("  （一直在跳 = 感應器活著）"));
  Serial.print(F("F5 開機秒數   ")); Serial.print(f[5]); Serial.println(F(" 秒"));

  delay(50);
  delay(950);
}

// 用出廠位址＋hw[0] 認產品
void printProduct(uint8_t factory, uint8_t hw0) {
  if      (factory == 0x16 && hw0 == 8)  Serial.print(F("循行者 8 路"));
  else if (factory == 0x16 && hw0 == 16) Serial.print(F("循行者 16 路"));
  else if (factory == 0x29 && hw0 == 2)  Serial.print(F("測距者 2 路"));
  else if (factory == 0x29 && hw0 == 8)  Serial.print(F("測距者 8 路"));
  else if (factory == 0x42 && hw0 == 1)  Serial.print(F("掌控者 PS2"));
  else if (factory == 0x42 && hw0 == 2)  Serial.print(F("掌控者 PS4"));
  else if (factory == 0x6A)              Serial.print(F("IMU 陀螺儀"));
  else if (factory == 0x4C && hw0 == 1)  Serial.print(F("神攝手 學校版"));
  else if (factory == 0x4C && hw0 == 2)  Serial.print(F("神攝手 WRO 版"));
  else                                   Serial.print(F("不認識的 MBC 裝置"));
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
