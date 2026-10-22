/*
 * 循行者 8 路 × Arduino　範例 7：調整門檻
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   「門檻」是感應器判斷「這一路在不在線上」的分數線：光值（0 黑 ~ 100 白）
 *   比門檻低就算在線上。預設 50。線很淡、場地反光時可以調。
 *   在序列埠監控窗打：
 *     數字 1~99 再按 Enter → 用 E4 設定門檻，然後讀 07 確認
 *     a                    → 用 E7 做 5 秒動態校準（門檻不準常常是沒校準）
 *   同時每秒印一次 8 路光值和壓線位元圖，讓你看門檻改了之後哪幾路算「在線上」。
 *
 * 你需要：循行者 8 路（通用 I2C 版）＋ 白紙＋黑色電工膠帶。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 小提醒：
 *   E4 設定的門檻存在 RAM，感應器重開機會回到預設 50。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("=== 循行者 8 路：調整門檻 ==="));
  Serial.print(F("目前門檻（07）：")); Serial.println(mbcRead(ADDR, 0x07));
  Serial.println(F("打 1~99 設定門檻、打 a 做 5 秒動態校準，然後按 Enter"));
}

void loop() {
  // ---- 處理輸入 ----
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s == "a") {
      Serial.println(F("5 秒動態校準開始！把感應器在黑線和白底上來回掃..."));
      mbcWrite(ADDR, 0xE7);              // E7 = 5 秒動態校準
      unsigned long start = millis();
      delay(20);
      while (millis() - start < 8000) {  // 等 F3 bit2「校準中」變回 0
        int16_t st = mbcRead(ADDR, 0xF3);
        if (st != MBC_NONE && !(st & 0x04)) break;
        delay(100);
      }
      Serial.println(F("校準完成"));
    } else if (s.length() > 0) {
      int n = s.toInt();
      if (n >= 1 && n <= 99) {
        uint8_t th = (uint8_t)n;
        mbcWrite(ADDR, 0xE4, &th, 1);    // E4 + 1 個參數 = 設定門檻
        delay(20);
        Serial.print(F("門檻設為 ")); Serial.print(n);
        Serial.print(F("，讀回 07 = ")); Serial.println(mbcRead(ADDR, 0x07));
      } else {
        Serial.println(F("請打 1~99 的數字，或 a"));
      }
    }
  }

  // ---- 每秒印一次光值與位元圖 ----
  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    int16_t light[8];
    int16_t bitmap = mbcRead(ADDR, 0x05);
    if (mbcReadMany(ADDR, 0x10, 8, light) && bitmap != MBC_NONE) {
      Serial.print(F("光值 左 "));
      for (uint8_t i = 0; i < 8; i++) { Serial.print(light[i]); Serial.print(' '); }
      Serial.print(F("右  在線上(左→右) "));
      for (uint8_t i = 0; i < 8; i++) Serial.print((bitmap >> i) & 1 ? '#' : '.');   // 第 1 路（bit0）印最左
      Serial.println();
    }
  }
  delay(50);
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
