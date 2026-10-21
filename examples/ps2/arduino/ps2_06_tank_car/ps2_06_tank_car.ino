/*
 * 掌控者 PS2 × Arduino　範例 6（進階）：坦克遙控車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用手把開兩輪小車，開法跟坦克一樣：
 *     左搖桿上下 → 控制左輪（往上推前進、往下推後退）
 *     右搖桿上下 → 控制右輪
 *     兩支一起往上推 = 直走；一前一後 = 原地轉彎。
 *   搖桿放開時，也可以用方向鍵開（↑ 前進、↓ 後退、← → 原地轉）。
 *   讀不到接收器、或手把斷線，車子會立刻停下來。
 *
 * 你需要：
 *   掌控者 PS2 接收器（通用 I2C 版）＋ PS2 2.4G 無線手把、
 *   兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等驅動板）、電池。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   （ESP32 的接腳不一樣，請看下面「馬達接腳」）
 *
 * 第一次跑之前：
 *   1. 把車子架高（輪子懸空）再上傳，兩支搖桿一起往上推，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把下面的 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 *   2. 手把要在「類比模式」（中間 ANALOG 鍵的燈亮著），搖桿才會動。
 *      數位模式時搖桿沒反應，但方向鍵還是可以開。
 *   3. ⚠️ 安全測試：車子還架高時，把手把關機、或拔掉電池，
 *      確認輪子會停下來，再放到地上開。
 *   4. 為了不讀錯，程式每 0.4 秒才問一次「手把還連著嗎？」，其他時間只讀手把資料；
 *      讀到「上下同時按」「左右同時按」這種不可能的資料，會丟掉、沿用上一筆。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

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
const int  MAX_SPEED  = 200;             // 搖桿推到底的速度：0 ~ 255，新手先用慢一點
const int  DPAD_SPEED = 150;             // 用方向鍵開的速度
const bool LEFT_REVERSE  = false;        // 左輪轉反了改成 true
const bool RIGHT_REVERSE = false;        // 右輪轉反了改成 true

// 方向鍵在 16 位按鍵數字裡的位置（第幾位）
const uint8_t BTN_UP = 4, BTN_RIGHT = 5, BTN_DOWN = 6, BTN_LEFT = 7;

bool stopped = false;                    // 現在是不是因為斷線而停車
bool connected = false;                  // 手把有沒有連上（每 0.4 秒更新一次）
unsigned long lastStatus = 0;            // 上一次看「手把有沒有連上」的時間
unsigned long lastPrint = 0;             // 上一次印速度的時間

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}                      // 車子保持停止
  }
  Serial.println(F("準備好了！用兩支搖桿開車。"));
}

void loop() {
  // ---- 1. 每 0.4 秒看一次手把有沒有連線（第 0x31 號格子：狀態資料）----
  //   不要每一圈都讀：一直在兩個格子之間換來換去，接收器比較容易交錯資料，
  //   車子可能會自己動。開車只要一直讀第 0x11 號格子就好。
  if (millis() - lastStatus >= 400) {
    lastStatus = millis();
    uint8_t st[9];
    connected = mbcRead(0x31, st, 9) && (st[0] & 0x02);   // 第 0 個數字的「第 1 位」：1 = 手把已連線
  }
  if (!connected) {
    safeStop();                          // 讀不到接收器，或手把沒連線 → 立刻停車
    return;
  }

  // ---- 2. 讀手把（第 0x11 號格子）。readPad 在檔案下面，會順便檢查有沒有讀錯 ----
  uint8_t d[9];
  if (!readPad(d)) {
    safeStop();                          // 讀不到 → 停車
    return;
  }
  if (stopped) {
    stopped = false;
    Serial.println(F("手把連上了，可以開車！"));
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // 16 位的按鍵狀態
  int ly = (int8_t)d[3];                 // 左搖桿上下：上 = 正（-127 ~ +127）
  int ry = (int8_t)d[5];                 // 右搖桿上下：上 = 正

  // 搖桿推多少，輪子就轉多快：-127 ~ +127 換算成 -MAX_SPEED ~ +MAX_SPEED
  int left  = (long)ly * MAX_SPEED / 127;
  int right = (long)ry * MAX_SPEED / 127;

  // 兩支搖桿都放開時，改看方向鍵
  if (ly == 0 && ry == 0) {
    if      (buttons & (1u << BTN_UP))    { left =  DPAD_SPEED; right =  DPAD_SPEED; }  // 前進
    else if (buttons & (1u << BTN_DOWN))  { left = -DPAD_SPEED; right = -DPAD_SPEED; }  // 後退
    else if (buttons & (1u << BTN_LEFT))  { left = -DPAD_SPEED; right =  DPAD_SPEED; }  // 原地左轉
    else if (buttons & (1u << BTN_RIGHT)) { left =  DPAD_SPEED; right = -DPAD_SPEED; }  // 原地右轉
  }

  drive(left, right);

  if (millis() - lastPrint >= 300) {     // 每 0.3 秒印一次速度
    lastPrint = millis();
    Serial.print(F("左輪 "));
    Serial.print(left);
    Serial.print(F("　右輪 "));
    Serial.println(right);
  }

  delay(20);
}

// 停車，並且只在剛停的那一下說一次
void safeStop() {
  drive(0, 0);
  if (!stopped) {
    stopped = true;
    Serial.println(F("讀不到手把 → 停車！"));
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
// 讀手把資料，順便「防呆」（每個範例都一樣，不用改）
// ============================================================
// 為什麼要防呆？
//   接收器很忙的時候（例如正在跟手把無線通訊），偶爾會來不及換資料，
//   把「上一次讀的別的格子」（例如第 0x31 號狀態資料）交給 Arduino。
//   這一筆的按鍵、搖桿數字就會是亂的，遙控車可能會自己動起來。
// 怎麼看出來？
//   手把的十字方向鍵，不可能同時按「上」和「下」，也不可能同時按「左」和「右」。
//   讀到這種組合，就知道這一筆讀錯了：把它丟掉，沿用上一筆正確的資料。

uint8_t lastGood[9] = {0};               // 上一筆正確的手把資料（一開始全部是 0 = 什麼都沒按）

// 讀第 0x11 號格子（手把資料，9 個數字）放進 d。讀不到接收器回傳 false
bool readPad(uint8_t *d) {
  if (!mbcRead(0x11, d, 9)) return false;
  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];                     // 16 位的按鍵狀態
  bool upDown    = (buttons & (1u << 4)) && (buttons & (1u << 6));   // 上（第 4 位）和下（第 6 位）一起按？
  bool leftRight = (buttons & (1u << 7)) && (buttons & (1u << 5));   // 左（第 7 位）和右（第 5 位）一起按？
  if (upDown || leftRight) {
    memcpy(d, lastGood, 9);              // 讀錯了 → 這一筆不要，改用上一筆
  } else {
    memcpy(lastGood, d, 9);              // 讀對了 → 記起來，下次讀錯時拿來用
  }
  return true;
}

// ============================================================
// 以下是「通訊小幫手」：負責跟接收器講話。
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
  delay(2);                              // 等接收器準備資料 2 毫秒（PS4 版一定要；PS2 版留著也沒關係）
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
