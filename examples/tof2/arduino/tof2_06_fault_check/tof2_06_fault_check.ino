/*
 * 測距者 2 路 × Arduino　範例 6：雷射健康檢查
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀「06 初始化位元圖」「07 故障位元圖」和「F3 狀態旗標」，
 *   每秒報告兩顆雷射是不是都健康。比賽前跑一下，就知道感應器有沒有壞。
 *     06：bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不會重試，要重新上電）
 *     07：bit0 = 左、bit1 = 右，1 = 那顆現在失效（讀不到或 200 ms 沒更新）
 *   失效那顆的距離會固定回 120 cm／1200 mm，看起來像「沒東西」，所以要靠這兩格分辨。
 *
 * 你需要：測距者 2 路（通用 I2C 版）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== 測距者 2 路：雷射健康檢查 ==="));
  // 這個範例故意不等就緒，開機失敗時也看得到 06 哪一顆沒起來
  delay(3000);                           // 兩顆雷射開機最多約 3 秒
}

void loop() {
  int16_t v[2];                          // v[0] = 06 初始化、v[1] = 07 故障
  int16_t st = mbcRead(ADDR, 0xF3);
  if (!mbcReadMany(ADDR, 0x06, 2, v) || st == MBC_NONE) {
    Serial.println(F("整個感應器都讀不到：檢查接線，或它是不是 SPIKE／EV3 版"));
    delay(1000);
    return;
  }
  int16_t initOk = v[0], fault = v[1];

  Serial.println(F("---------------------------"));
  Serial.print(F("F3 就緒 "));       Serial.print((st & 0x01) ? F("是") : F("否"));
  Serial.print(F("  任一顆失效 "));  Serial.println((st & 0x02) ? F("是") : F("否"));

  reportOne(F("左"), initOk & 0x01, fault & 0x01);
  reportOne(F("右"), initOk & 0x02, fault & 0x02);

  if (initOk == 3 && fault == 0) Serial.println(F("兩顆雷射都健康！"));
  else Serial.println(F("有問題：先重新上電；還是一樣就檢查那顆雷射有沒有被遮住或接頭鬆了"));

  delay(50);
  delay(950);
}

// 印一顆雷射的狀況
void reportOne(const __FlashStringHelper *name, int16_t ok, int16_t bad) {
  Serial.print(F("  "));
  Serial.print(name);
  Serial.print(F(" 顆：初始化 "));
  Serial.print(ok ? F("成功") : F("失敗"));
  Serial.print(F("，目前 "));
  Serial.println(bad ? F("失效！") : F("正常"));
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
