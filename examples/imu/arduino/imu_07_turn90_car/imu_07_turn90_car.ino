/*
 * IMU 陀螺儀 × Arduino　範例 7：小車原地轉 90 度（進階）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   開機後先送命令 E0 把航向歸零，然後每 3 秒讓兩輪小車原地右轉 90 度一次：
 *   用編號 02 融合航向當「現在轉到幾度」，差距大就轉快、快到了就轉慢，
 *   到了就停。轉 4 次就回到原來的方向。
 *   從上往下看順時針（右轉）航向變大，所以右轉時目標 = 原本 + 90。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。另外要：兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   （ESP32 的接腳不一樣，請看下面「馬達接腳」）
 *
 * 準備：
 *   1. 陀螺儀要平放固定在車上，開機時車子不要動（它要先記住哪一面朝上）。
 *   2. 先把車架高測試：右轉時左輪應該往前、右輪往後。轉反了就改 LEFT_REVERSE／RIGHT_REVERSE。
 *   3. 轉過頭就把 TURN_SPEED 調小、或 SLOW_ZONE 調大。
 *
 * 怎麼看結果：
 *   序列埠監控窗（115200）會印目標角度與現在的航向。
 *
 * 小提醒：
 *   想左轉：把 TURN_ANGLE 改成 -90。
 */

#include <Wire.h>

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- 馬達接腳 ----
#if defined(ESP32)
// ESP32 的第 6 ~ 11 腳接著板子上的記憶體晶片，不能拿來用，所以改用這幾腳
const int ENA = 25, IN1 = 26, IN2 = 27;  // 左輪
const int ENB = 14, IN3 = 32, IN4 = 33;  // 右輪
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // 左輪
const int ENB = 6, IN3 = 9, IN4 = 10;    // 右輪
#endif

// ---- 👉 可以改的參數 ----
const int  TURN_ANGLE    = 90;           // 每次轉幾度：正數 = 右轉，負數 = 左轉
const int  TURN_SPEED    = 150;          // 轉彎速度 0~255
const int  SLOW_SPEED    = 80;           // 快到了時的慢速
const int  SLOW_ZONE     = 25;           // 剩不到幾度就改用慢速
const int  DONE_ZONE     = 2;            // 差不到幾度算到了
const unsigned long WAIT_MS = 3000;      // 每次轉完等幾毫秒
const bool LEFT_REVERSE  = false;        // 左輪轉反了改成 true
const bool RIGHT_REVERSE = false;        // 右輪轉反了改成 true

long target = 0;                         // 這一次要轉到的角度
unsigned long doneAt = 0;                // 上一次轉完的時間
bool turning = false;                    // 現在是不是正在轉

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(1000);                           // 多等一下，讓陀螺儀記住哪一面朝上
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }

  mbcWrite(ADDR, 0xE0);                  // 命令 E0：航向歸零（現在的方向 = 0 度）
  delay(100);
  Serial.println(F("已歸零。每 3 秒原地轉 90 度一次。"));
  doneAt = millis();
}

void loop() {
  int16_t heading = mbcRead(ADDR, 0x02); // 02 融合航向（度），右轉變大
  if (heading == MBC_NONE) {             // 讀不到陀螺儀 → 為了安全，停車
    drive(0, 0);
    Serial.println(F("讀不到陀螺儀 → 停車"));
    delay(50);
    return;
  }

  if (!turning) {
    // ---- 休息中：時間到了就設定下一個目標 ----
    if (millis() - doneAt >= WAIT_MS) {
      target += TURN_ANGLE;
      turning = true;
      Serial.print(F("開始轉，目標 ")); Serial.print(target); Serial.println(F(" 度"));
    }
  } else {
    // ---- 轉彎中：看還差幾度 ----
    long error = target - heading;       // 正數 = 還要往右轉，負數 = 要往左（或轉過頭了）
    if (abs(error) <= DONE_ZONE) {       // 到了
      drive(0, 0);
      turning = false;
      doneAt = millis();
      Serial.print(F("到了！現在航向 ")); Serial.println(heading);
    } else {
      int spd = (abs(error) < SLOW_ZONE) ? SLOW_SPEED : TURN_SPEED;   // 快到了就慢一點
      if (error > 0) drive(spd, -spd);   // 右轉：左輪前進、右輪後退
      else           drive(-spd, spd);   // 左轉
    }
  }
  delay(50);
}

// 控制兩個輪子：速度 -255（全速後退）~ 255（全速前進）
void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);   // 不讓速度超過範圍
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
