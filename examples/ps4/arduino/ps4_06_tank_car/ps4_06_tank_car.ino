/*
 * 掌控者 PS4/PS5 × Arduino　範例 6：坦克遙控車（進階）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用手把開兩輪小車，開法跟坦克一樣：
 *     左搖桿上下（LY）→ 左輪；右搖桿上下（RY）→ 右輪
 *     兩支一起往上推 = 直走；一前一後 = 原地轉彎。
 *   搖桿的 -100 ~ +100 直接當馬達速度用（再乘上最高速度）。
 *   手把斷線、或讀不到接收器，車子會立刻停下來。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。另外要：兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   （ESP32 的接腳不一樣，請看下面「馬達接腳」）
 *
 * 準備：
 *   1. 把車子架高（輪子懸空）再上傳，兩支搖桿一起往上推，確認兩個輪子都「往前」轉。
 *      哪個輪子轉反了，就把 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 *   2. ⚠️ 車子還架高時，把手把關機，確認輪子會停下來，再放到地上開。
 *
 * 怎麼看結果：
 *   序列埠監控窗（115200）每 0.3 秒印一次左右輪速度。
 *
 * 小提醒：
 *   第一次用要先配對：接收器開機（或按 Reset）後 30 秒內，PS4 手把長按 SHARE + PS、PS5 手把長按 建立 + PS，直到燈條快閃。
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
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
const int  MAX_SPEED     = 200;          // 搖桿推到底的速度：0 ~ 255，新手先用慢一點
const bool LEFT_REVERSE  = false;        // 左輪轉反了改成 true
const bool RIGHT_REVERSE = false;        // 右輪轉反了改成 true

bool stopped = true;                     // 現在是不是停車狀態（一開始算停車）
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！用兩支搖桿開車。"));
}

void loop() {
  // 一次讀編號 06 ~ 13 共 14 個值（同一瞬間的資料）：
  //   v[0] = 06 已連線 … v[10] = 10 LX、v[11] = 11 LY、v[12] = 12 RX、v[13] = 13 RY
  //   （中間的 0C~0F 是空號，會讀到 -32768，不用理它）
  int16_t v[14];
  if (!mbcReadMany(ADDR, 0x06, 14, v) || v[0] == 0) {
    safeStop();                          // 讀不到接收器，或手把沒連線 → 立刻停車
    delay(50);
    return;
  }
  if (stopped) {
    stopped = false;
    Serial.println(F("手把連上了，可以開車！"));
  }

  int ly = v[11];                        // 左搖桿上下：上 = 正（-100 ~ +100）
  int ry = v[13];                        // 右搖桿上下：上 = 正

  // 搖桿推多少，輪子就轉多快：-100 ~ +100 換算成 -MAX_SPEED ~ +MAX_SPEED
  int left  = (long)ly * MAX_SPEED / 100;
  int right = (long)ry * MAX_SPEED / 100;
  drive(left, right);

  if (millis() - lastPrint >= 300) {     // 每 0.3 秒印一次速度
    lastPrint = millis();
    Serial.print(F("左輪 ")); Serial.print(left);
    Serial.print(F("　右輪 ")); Serial.println(right);
  }
  delay(50);
}

// 停車，並且只在剛停的那一下說一次
void safeStop() {
  drive(0, 0);
  if (!stopped) {
    stopped = true;
    Serial.println(F("讀不到手把 → 停車！"));
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
