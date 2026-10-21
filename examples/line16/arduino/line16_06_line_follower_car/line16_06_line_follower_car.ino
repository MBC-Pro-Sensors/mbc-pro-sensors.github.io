/*
 * 循行者 16 路 × Arduino　範例 6（進階）：循線小車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車沿著黑線自己跑。線偏左就往左修、線偏右就往右修，
 *   偏越多、修越多。這種方法叫做「比例控制」，是循線比賽最基本的招式。
 *   線跑掉時，會往線最後消失的那一邊原地找線。
 *
 * 你需要：
 *   兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等驅動板）、電池。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   （ESP32 改接：ENA → 25、IN1 → 26、IN2 → 27、ENB → 14、IN3 → 32、IN4 → 33）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   感應器裝在車頭正中間，離地面約 0.5～1 公分。
 *
 * 第一次跑之前：
 *   1. 先跑「範例 4：一鍵校準」校準場地。
 *   2. 把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把下面的 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼

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
const int   BASE_SPEED = 120;            // 直走的速度：0 ~ 255，先慢一點比較好調
const float KP         = 1.0;            // 修正力道：車子修不回來就加大，左右搖擺就減小
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
  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  mbcWrite(0x10, 0);                     // 循黑線（白線請改成 1）

  Serial.println(F("3 秒後出發！"));
  delay(3000);
}

void loop() {
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {            // 讀不到感應器 → 為了安全，先停車
    drive(0, 0);
    return;
  }

  // 線偏了多少：-100（很左）~ +100（很右），0 = 正中間
  // 保險起見，先把它「夾」在 -100 ~ +100 之間再用（萬一讀到超出範圍的數字也不會失控）
  int     error  = constrain((int8_t)d[1], -100, 100);
  uint8_t groups = d[3];                 // 0 = 看不到線

  if (groups == 0) {
    // 線跑掉了：感應器會記住線最後的位置，往那一邊原地轉，把線找回來
    if (error < 0) drive(-SEARCH_SPEED, SEARCH_SPEED);   // 線在左邊 → 原地左轉
    else           drive(SEARCH_SPEED, -SEARCH_SPEED);   // 線在右邊 → 原地右轉
    return;
  }

  // 比例控制：偏多少，就修多少
  int turn = KP * error;                 // 線在右邊（正數）→ turn 是正的 → 左輪快、右輪慢 → 往右轉
  drive(BASE_SPEED + turn, BASE_SPEED - turn);
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
// 以下是「通訊小幫手」：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
// ============================================================

void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
