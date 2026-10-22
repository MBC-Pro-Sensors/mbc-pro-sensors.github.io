/*
 * 測距者 2 路 × Arduino　範例 4（進階）：用 mm 精準停車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用「04 左 mm」「05 右 mm」「08 左右平均 mm」三格（精細到 1 mm），
 *   讓小車往前開，到距離牆壁 STOP_MM（預設 200 mm = 20 公分）時剛好停下。
 *   接近時會自動減速，所以不會衝過頭。沒有小車也可以跑：只看序列埠的數字。
 *
 * 你需要：兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等）、電池、一面牆或盒子。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   （ESP32 改接：ENA → 25、IN1 → 26、IN2 → 27、ENB → 14、IN3 → 32、IN4 → 33）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！感應器裝在車頭，雷射朝前。
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。車子停下時印出的距離應該很接近 STOP_MM。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- 馬達接腳 ----
#if defined(ESP32)
const int ENA = 25, IN1 = 26, IN2 = 27;  // 左輪（ESP32 的 6~11 腳不能用）
const int ENB = 14, IN3 = 32, IN4 = 33;  // 右輪
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // 左輪
const int ENB = 6, IN3 = 9, IN4 = 10;    // 右輪
#endif

// ---- 👉 可以改的參數 ----
const int16_t STOP_MM    = 200;          // 停在離牆幾 mm（200 = 20 公分）
const int16_t SLOW_MM    = 500;          // 離牆幾 mm 以內開始減速
const int     FAST_SPEED = 150;          // 全速 0~255
const int     SLOW_SPEED = 70;           // 最慢的爬行速度（太慢馬達會不動）
const bool    LEFT_REVERSE  = false;     // 左輪轉反了改成 true
const bool    RIGHT_REVERSE = false;     // 右輪轉反了改成 true

bool stopped = false;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.print(F("3 秒後出發，目標：停在 ")); Serial.print(STOP_MM); Serial.println(F(" mm"));
  delay(3000);
}

void loop() {
  int16_t v[5];                          // 04 左 mm、05 右 mm、06、07、08 平均 mm
  if (!mbcReadMany(ADDR, 0x04, 5, v)) {  // 從 04 號起一次讀 5 格（04~08）
    drive(0, 0);                         // 讀不到 → 安全停車
    delay(50);
    return;
  }
  int16_t leftMm = v[0], rightMm = v[1], avgMm = v[4];
  int16_t dist = (leftMm < rightMm) ? leftMm : rightMm;   // 用比較近的那顆，避免斜著撞到

  Serial.print(F("左 ")); Serial.print(leftMm);
  Serial.print(F(" mm  右 ")); Serial.print(rightMm);
  Serial.print(F(" mm  平均 ")); Serial.print(avgMm);
  Serial.print(F(" mm  "));

  if (stopped || dist <= STOP_MM) {
    drive(0, 0);
    if (!stopped) { stopped = true; Serial.println(F("→ 到了，停車！")); }
    else Serial.println(F("（已停）"));
  } else if (dist <= SLOW_MM) {
    // 越近越慢：距離從 SLOW_MM 到 STOP_MM，速度從 FAST 降到 SLOW
    int speed = map(dist, STOP_MM, SLOW_MM, SLOW_SPEED, FAST_SPEED);
    drive(speed, speed);
    Serial.print(F("→ 減速 ")); Serial.println(speed);
  } else {
    drive(FAST_SPEED, FAST_SPEED);
    Serial.println(F("→ 全速前進"));
  }
  delay(50);
}

void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
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
