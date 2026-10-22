/*
 * MBC 通用 I2C × Arduino　共用範例 1：換 I2C 位址（F8）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   同一條 I2C 線上要接兩個「門牌號碼」一樣的 MBC 感應器（例如兩個測距者 2 路，都是 0x29）
 *   時，先把其中一個換成別的位址。做法：
 *     寫 [F8][新位址][新位址取反]  → 存進感應器 EEPROM，重開機後生效
 *     寫 [F8][0x00][0xFF]          → 回出廠位址
 *   換完會自動送 [EF][0xA5] 重新開機，再掃一次確認新位址有出現。
 *
 * 你需要：任何一款 MBC 通用 I2C 版感應器（一次只接一個！）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼用：
 *   1. 把下面 OLD_ADDR 改成感應器現在的位址、NEW_ADDR 改成想換的位址（0x08~0x77）。
 *   2. 上傳，打開序列埠監控窗 115200，打 y 按 Enter 就換；打 r 按 Enter 回出廠。
 *   3. 之後所有範例裡的 ADDR 都要改成新位址。
 *
 * 小提醒：
 *   F0「裝置 ID」低 byte 永遠是出廠位址，換了位址也不變，所以掃描時還是認得出是哪一款。
 */

#include <Wire.h>

const uint8_t OLD_ADDR = 0x29;           // 感應器現在的位址   👉 改這裡（循行者 0x16、測距者 0x29、掌控者 0x42、IMU 0x6A、神攝手 0x4C）
const uint8_t NEW_ADDR = 0x2A;           // 想換成的位址       👉 改這裡（0x08 ~ 0x77，別跟其他裝置撞號）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC 通用 I2C：換位址 ==="));
  Serial.print(F("目前線上有哪些 MBC 裝置：")); scan();
  if (!mbcWaitReady(OLD_ADDR, 6000)) {
    Serial.print(F("在 0x")); Serial.print(OLD_ADDR, HEX); Serial.println(F(" 找不到感應器，請確認 OLD_ADDR 和接線。"));
    while (true) {}
  }
  Serial.print(F("找到了 0x")); Serial.print(OLD_ADDR, HEX);
  Serial.print(F("（出廠位址 0x")); Serial.print(mbcRead(OLD_ADDR, 0xF0) & 0xFF, HEX); Serial.println(')');
  Serial.print(F("打 y 換成 0x")); Serial.print(NEW_ADDR, HEX); Serial.println(F("；打 r 回出廠位址。然後按 Enter"));
}

void loop() {
  if (!Serial.available()) { delay(50); return; }
  char c = Serial.read();
  if (c == 'y') {
    if (NEW_ADDR < 0x08 || NEW_ADDR > 0x77) { Serial.println(F("NEW_ADDR 要在 0x08 ~ 0x77 之間")); return; }
    uint8_t p[2] = { NEW_ADDR, (uint8_t)~NEW_ADDR };   // [新位址][新位址取反]：兩個都對才會寫入，避免誤觸
    Serial.print(F("換位址 → 0x")); Serial.println(NEW_ADDR, HEX);
    if (!mbcWrite(OLD_ADDR, 0xF8, p, 2)) { Serial.println(F("寫入失敗")); return; }
    rebootAndCheck(NEW_ADDR);
  } else if (c == 'r') {
    uint8_t p[2] = { 0x00, 0xFF };       // [F8][00][FF] = 回出廠位址
    Serial.println(F("回出廠位址"));
    if (!mbcWrite(OLD_ADDR, 0xF8, p, 2)) { Serial.println(F("寫入失敗")); return; }
    rebootAndCheck(0);                   // 0 = 不知道出廠位址，掃描看結果
  }
}

// 送重開機命令，等它起來，再掃描一次
void rebootAndCheck(uint8_t expect) {
  delay(50);                             // 給 EEPROM 一點時間寫完
  uint8_t magic = 0xA5;
  mbcWrite(OLD_ADDR, 0xEF, &magic, 1);   // [EF][A5] = 重新開機（新位址重開後才生效）
  Serial.println(F("重新開機中..."));
  delay(3000);                           // 測距者 8 路開機要 2~4 秒
  Serial.print(F("掃描結果：")); scan();
  if (expect && mbcRead(expect, 0xF3) != MBC_NONE) {
    Serial.print(F("成功！感應器現在在 0x")); Serial.println(expect, HEX);
    Serial.println(F("記得把其他範例的 ADDR 改成新位址。"));
  } else if (expect) {
    Serial.println(F("新位址沒有回應。請重新上電再掃一次（有些板子的 I2C 要重新上電才會更新）。"));
  }
  Serial.println(F("（把 OLD_ADDR 改成新位址再上傳，就可以再換一次或回出廠）"));
}

// 掃 0x08~0x77，有回應的就讀 F0 看是不是 MBC 裝置
void scan() {
  uint8_t found = 0;
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() != 0) continue;
    int16_t id = mbcRead(a, 0xF0);
    Serial.print(F(" 0x")); Serial.print(a, HEX);
    if (id != MBC_NONE) { Serial.print(F("(出廠 0x")); Serial.print(id & 0xFF, HEX); Serial.print(')'); }
    found++;
  }
  if (!found) Serial.print(F("（沒有）"));
  Serial.println();
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
