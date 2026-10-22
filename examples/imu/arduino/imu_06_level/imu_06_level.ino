/*
 * IMU 陀螺儀 × Arduino　範例 6：哪一面朝上？磁力可不可信？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.3 秒讀編號 08~0A（一次讀 3 個）：
 *     0A 朝上的面：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z（板子絲印的軸）
 *     08 磁力信任度 0~255：融合演算法多相信磁力計（越高越好）
 *     09 磁場干擾度 0~255：附近有沒有鐵器、馬達干擾（越低越好）
 *   朝上的面有變化就印一行；平放（+Z 朝上）時板子內建的 LED 會亮。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。把陀螺儀翻來翻去，看哪一面朝上；拿磁鐵靠近看干擾度。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int LED = LED_BUILTIN;             // 板子內建的燈（UNO 是第 13 腳）

unsigned long lastPrint = 0;
int16_t lastFace = -1;                   // 上一次朝上的面（-1 = 還沒看過）

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！翻翻看陀螺儀。"));
}

void loop() {
  if (millis() - lastPrint >= 300) {     // 👉 可以改：每 0.3 秒看一次
    lastPrint = millis();

    // 一次讀編號 08~0A 共 3 個值：[0] 磁力信任度 [1] 磁場干擾度 [2] 朝上的面
    int16_t v[3];
    if (!mbcReadMany(ADDR, 0x08, 3, v)) { Serial.println(F("讀不到陀螺儀")); return; }

    digitalWrite(LED, v[2] == 2 ? HIGH : LOW);    // +Z 朝上（平放）→ 燈亮

    if (v[2] != lastFace) {              // 朝上的面有變才印
      lastFace = v[2];
      Serial.print(F("朝上的面："));
      printFace(v[2]);
      Serial.print(F(" | 磁力信任度 ")); Serial.print(v[0]);
      Serial.print(F(" | 磁場干擾度 ")); Serial.print(v[1]);
      if (v[1] > 100) Serial.print(F("（干擾大，附近有鐵器或馬達？）"));
      Serial.println();
    }
  }
  delay(50);
}

// 0A 朝上的面：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z
void printFace(int16_t f) {
  switch (f) {
    case 0: Serial.print(F("+X（X 軸朝上）")); break;
    case 1: Serial.print(F("+Y（Y 軸朝上）")); break;
    case 2: Serial.print(F("+Z（平放，正面朝上）")); break;
    case 3: Serial.print(F("-X")); break;
    case 4: Serial.print(F("-Y")); break;
    case 5: Serial.print(F("-Z（翻面，背面朝上）")); break;
    default: Serial.print(F("？")); break;
  }
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
