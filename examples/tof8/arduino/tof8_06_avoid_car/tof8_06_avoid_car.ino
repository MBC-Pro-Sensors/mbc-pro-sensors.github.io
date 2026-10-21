/*
 * 測距者 8 路 × Arduino　範例 6（進階）：避障小車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車自己到處跑，不會撞到牆壁和障礙物：
 *     前面空空的        → 直走；旁邊太靠近牆，就往另一邊稍微修
 *     前面有東西擋住    → 停下來，原地轉向「比較空的那一邊」，直到前面空了再走
 *   8 顆雷射把左、前、右都看在眼裡，所以轉彎時知道該往哪邊轉。
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
 *   感應器裝在車頭正中間、朝前方，第 0 顆在車子左邊、第 7 顆在車子右邊。
 *
 * 第一次跑之前：
 *   1. 先跑「範例 5：雷達長條圖」，確認手放在車子左邊時，是上面幾條（第 0 ~ 2 顆）變短。
 *   2. 把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把下面的 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint16_t NO_TARGET = 1200;         // 距離 1200 以上 = 沒有東西
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

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
const int      FORWARD_SPEED = 130;      // 直走的速度：0 ~ 255，先慢一點比較安全
const int      TURN_SPEED    = 110;      // 原地轉彎的速度
const int      STEER         = 40;       // 旁邊太靠近時，修正的力道
const uint16_t FRONT_STOP    = 250;      // 前面多近要轉彎（mm）：250 = 25 公分
const uint16_t SIDE_CLOSE    = 150;      // 旁邊多近算「太靠近」（mm）：150 = 15 公分
const bool     LEFT_REVERSE  = false;    // 左輪轉反了改成 true
const bool     RIGHT_REVERSE = false;    // 右輪轉反了改成 true

uint16_t dist[8];                        // 8 顆雷射的距離（mm）

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0，沒等好車子會以為四周都是牆
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}                      // 找不到感應器：不出發（輪子保持停止）
  }

  Serial.println(F("3 秒後出發！"));
  delay(3000);
}

void loop() {
  // 第 0x21 號格子：8 顆雷射的距離，一次讀回來
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {           // 讀不到感應器 → 為了安全，先停車
    drive(0, 0);
    delay(50);
    return;
  }

  bool allZero = true;                   // 8 顆是不是全部都是 0
  for (uint8_t i = 0; i < 8; i++) {
    dist[i] = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];
    if (dist[i] != 0) allZero = false;
    if (dist[i] == FAULT) dist[i] = 0;   // 故障的那一顆：當作「有東西擋著」，比較安全
  }
  if (allZero) {                         // 全部是 0：感應器可能剛重新開機（例如電池沒電），還沒準備好
    drive(0, 0);                         // → 先停車，等它準備好
    delay(50);
    return;
  }

  // 把 8 顆分成左、前、右三組，每組取「最近的那個」
  uint16_t left  = smallest(0, 2);       // 第 0 ~ 2 顆：左邊、左前方
  uint16_t front = smallest(3, 4);       // 第 3、4 顆：正前方
  uint16_t right = smallest(5, 7);       // 第 5 ~ 7 顆：右前方、右邊

  if (front > FRONT_STOP) {
    // ---- 前面空空的：直走，旁邊太靠近就往另一邊修一點 ----
    int turn = 0;                        // 正數 = 往左修，負數 = 往右修
    if (left < SIDE_CLOSE)  turn = turn - STEER;   // 左邊太近 → 往右修
    if (right < SIDE_CLOSE) turn = turn + STEER;   // 右邊太近 → 往左修
    drive(FORWARD_SPEED - turn, FORWARD_SPEED + turn);
  } else {
    // ---- 前面有東西：原地轉向比較空的那一邊 ----
    if (left > right) drive(-TURN_SPEED, TURN_SPEED);   // 左邊比較空 → 原地左轉
    else              drive(TURN_SPEED, -TURN_SPEED);   // 右邊比較空 → 原地右轉
  }

  delay(50);                             // 感應器大約 0.1 秒更新一次，每 0.05 秒看一次就夠了
}

// 從第 from 顆到第 to 顆，找出最近的距離
uint16_t smallest(uint8_t from, uint8_t to) {
  uint16_t m = NO_TARGET;
  for (uint8_t i = from; i <= to; i++) {
    if (dist[i] < m) m = dist[i];
  }
  return m;
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
