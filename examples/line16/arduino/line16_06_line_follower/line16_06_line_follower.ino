/*
 * 循行者 16 路 × Arduino　範例 6（進階）：循線小車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車沿著黑線自己跑。用「02 細位置」（-100 ~ +100）當誤差，
 *   線偏左就往左修、偏右就往右修，偏越多修越多（比例控制）。
 *   線跑掉時（04 線段數 = 0），看「06 最後離線方向」往那一邊原地找線。
 *
 * 你需要：
 *   兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等）、電池。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   （ESP32 改接：ENA → 25、IN1 → 26、IN2 → 27、ENB → 14、IN3 → 32、IN4 → 33）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   感應器裝在車頭正中間，離地面約 0.5～1 公分。
 *
 * 第一次跑之前：
 *   1. 先跑「範例 4：一鍵校準」校準場地。
 *   2. 把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

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
const int   BASE_SPEED   = 120;          // 直走的速度：0 ~ 255，先慢一點比較好調
const float KP           = 1.0;          // 修正力道：車子修不回來就加大，左右搖擺就減小
const int   SEARCH_SPEED = 100;          // 找線時原地轉的速度
const bool  LEFT_REVERSE  = false;       // 左輪轉反了改成 true
const bool  RIGHT_REVERSE = false;       // 右輪轉反了改成 true

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  mbcWrite(ADDR, 0xE0);                  // E0 = 循黑線（白線請改成 0xE1）

  Serial.println(F("3 秒後出發！"));
  delay(3000);
}

void loop() {
  int16_t v[6];                          // 01 線位置 02 細位置 03 線寬 04 線段數 05 位元圖 06 最後離線方向
  if (!mbcReadMany(ADDR, 0x01, 6, v)) {  // 讀不到感應器 → 為了安全，先停車
    drive(0, 0);
    delay(50);
    return;
  }

  int16_t error   = v[1];                // 02 細位置：-100（很左）~ +100（很右），0 = 正中間
  int16_t groups  = v[3];                // 04 線段數：0 = 看不到線
  int16_t lastDir = v[5];                // 06 最後離線方向：負 = 線最後在左邊

  if (groups == 0) {
    // 線跑掉了：往線最後消失的那一邊原地轉，把線找回來
    if (lastDir < 0) drive(-SEARCH_SPEED, SEARCH_SPEED);   // 線在左邊 → 原地左轉
    else             drive(SEARCH_SPEED, -SEARCH_SPEED);   // 線在右邊 → 原地右轉
    delay(50);
    return;
  }

  // 比例控制：偏多少，就修多少
  int turn = KP * error;                 // 線在右邊（正數）→ turn 是正的 → 左輪快、右輪慢 → 往右轉
  drive(BASE_SPEED + turn, BASE_SPEED - turn);
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
