/*
 * 陀螺儀 IMU × Arduino　範例 7（進階）：小車原地轉 90 度
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車「原地右轉 90 度」，停 1 秒，再轉 90 度……轉滿 4 次（一整圈）就停。
 *   車子一邊轉，一邊看陀螺儀的角度：
 *     離目標還很遠 → 轉快一點
 *     快到了       → 慢慢轉
 *     到了         → 停車
 *   這種「一邊看感應器、一邊修正」的方法叫做「回授控制」。
 *   只靠「馬達轉幾秒」來轉彎，電池沒電、地板太滑就會不準；用陀螺儀就準多了。
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
 *   陀螺儀用螺絲或積木固定在車上（平放、直立、側放都可以，但不能晃）。
 *
 * 第一次跑之前：
 *   1. 先跑「範例 2」，確認右轉時角度數字會變大。
 *   2. 把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把下面的 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 *   3. 開機時車子要放著不動，等「準備好了」才會出發。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼

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
const int   TURN_ANGLE = 90;             // 每次轉幾度：正數 = 右轉，負數 = 左轉
const int   TURN_TIMES = 4;              // 總共轉幾次（4 次 × 90 度 = 一整圈）
const int   MAX_SPEED  = 150;            // 離目標很遠時的速度：0 ~ 255
const int   MIN_SPEED  = 70;             // 快到時的最慢速度（太小輪子會轉不動）
const float KP         = 3.0;            // 差 1 度要加多少速度：停不準就調這個
const int   TOLERANCE  = 2;              // 差 2 度以內就算「到了」
const bool  LEFT_REVERSE  = false;       // 左輪轉反了改成 true
const bool  RIGHT_REVERSE = false;       // 右輪轉反了改成 true

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);

  Serial.println(F("請不要動車子，陀螺儀準備中……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}                      // 馬達已經停好，就停在這裡
  }
  mbcWrite(0x10, 0x01);                  // 航向歸零：現在車頭的方向 = 0 度
  delay(50);                             // 歸零後等一下再讀

  Serial.println(F("準備好了！3 秒後開始轉。"));
  delay(3000);

  long target = 0;                       // 目標角度
  for (int i = 1; i <= TURN_TIMES; i++) {
    target = target + TURN_ANGLE;        // 每次多轉 90 度：90、180、270、360
    Serial.print(F("第 "));
    Serial.print(i);
    Serial.print(F(" 次：轉到 "));
    Serial.print(target);
    Serial.print(F(" 度…… "));

    if (!turnTo(target)) {               // 轉失敗了（為了安全已經停車）
      while (true) {}
    }
    delay(1000);                         // 停 1 秒再轉下一次
  }
  Serial.println(F("完成！按 Arduino 的 RESET 鍵可以再跑一次。"));
}

void loop() {
  // 全部都在 setup 做完了，這裡什麼都不做（車子保持停止）
}

// 原地轉到 target 度。成功回傳 true；轉不到回傳 false（已停車）
bool turnTo(long target) {
  unsigned long start = millis();
  while (true) {
    // ---- 1. 讀航向（第 0x11 號格子，用第 2、3 個數字 = Gyro3D）----
    uint8_t d[6];
    if (!mbcRead(0x11, d, 6)) {          // 讀不到感應器 → 為了安全，先停車
      drive(0, 0);
      if (millis() - start > 5000) {
        Serial.println(F("讀不到陀螺儀，停車！請檢查接線。"));
        return false;
      }
      delay(20);
      continue;
    }
    long heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);   // 右轉變大

    // ---- 2. 還差幾度？ ----
    long error = target - heading;       // 正數 = 還要往右轉，負數 = 轉過頭了

    if (abs(error) <= TOLERANCE) {       // 到了！
      drive(0, 0);
      delay(300);                        // 等車子完全停下來
      mbcRead(0x11, d, 6);               // 再看一次停在幾度
      heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);
      Serial.print(F("停在 "));
      Serial.print(heading);
      Serial.println(F(" 度"));
      return true;
    }

    // ---- 3. 安全檢查 ----
    if (abs(error) > abs(TURN_ANGLE) + 45) {   // 越轉越遠 → 方向反了
      drive(0, 0);
      Serial.println(F("越轉越遠，停車！可能是輪子轉反了：改 LEFT_REVERSE / RIGHT_REVERSE。"));
      return false;
    }
    if (millis() - start > 5000) {       // 轉了 5 秒還沒到
      drive(0, 0);
      Serial.println(F("5 秒內轉不到，停車！檢查電池，或把 MIN_SPEED 調大。"));
      return false;
    }

    // ---- 4. 差越多轉越快，快到了就慢下來 ----
    int speed = constrain((int)(KP * abs(error)), MIN_SPEED, MAX_SPEED);
    if (error > 0) drive(speed, -speed);    // 左輪前進、右輪後退 → 原地右轉（角度變大）
    else           drive(-speed, speed);    // 轉過頭了 → 原地左轉回來
    delay(10);
  }
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
