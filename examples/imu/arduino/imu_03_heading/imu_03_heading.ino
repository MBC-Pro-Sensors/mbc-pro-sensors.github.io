/*
 * IMU 陀螺儀 × Arduino　範例 3：我轉了幾度？（四種航向）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.1 秒印出四種航向（編號 01~04，一次讀 4 個）：
 *     01 陀螺 3D 航向：用全部三軸算的，最常用
 *     02 融合航向：陀螺 3D 再用磁力計慢慢修正，久了不會漂
 *     03 陀螺 1D 航向：只用轉向那一軸積分
 *     04 磁力航向：只看磁力計（像指南針，會被鐵器干擾）
 *   從上往下看「順時針」（右轉）數字變大，會一直累加（轉兩圈 = 720），可以變負數。
 *   在序列埠監控窗輸入 z 再按 Enter，會送命令 E0 把四個航向都歸零。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。把陀螺儀平放，慢慢轉一圈，看數字。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 *   要 0~359 的話自己換算：((h % 360) + 360) % 360。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！轉轉看陀螺儀。輸入 z + Enter 可以歸零。"));
  Serial.println(F("  陀螺3D   融合   陀螺1D   磁力"));
}

void loop() {
  // ---- 1. 電腦有送 z 過來嗎？有就送命令 E0（航向歸零，沒有參數） ----
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'z' || ch == 'Z') {
      mbcWrite(ADDR, 0xE0);              // 命令 E0：四個航向都歸零
      Serial.println(F(">>> 已歸零"));
    }
  }

  // ---- 2. 每 0.1 秒讀編號 01~04 四個航向 ----
  if (millis() - lastPrint >= 100) {     // 👉 可以改：每 0.1 秒印一次
    lastPrint = millis();
    int16_t h[4];
    if (!mbcReadMany(ADDR, 0x01, 4, h)) { Serial.println(F("讀不到陀螺儀")); return; }
    char line[48];
    snprintf(line, sizeof(line), "%7d %7d %7d %7d", h[0], h[1], h[2], h[3]);   // 每個數字佔 7 格，排整齊
    Serial.println(line);
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
