/*
 * IMU 陀螺儀 × Arduino　範例 9：小車走直線（進階）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   開機後先送命令 E0 把航向歸零（現在車頭的方向 = 0 度），然後一直往前走。
 *   用編號 02 融合航向看車子有沒有走歪，歪了就讓兩輪速度不一樣，把車頭拉回來：
 *     修正量 fix = KP × 航向（最多 MAX_FIX）
 *     左輪 = BASE_SPEED − fix　　右輪 = BASE_SPEED + fix
 *   從上往下看順時針（往右歪）航向變大 → fix 是正的 → 左輪減速、右輪加速 → 車頭往左轉回來。
 *   往左歪航向變小（負數）→ fix 是負的 → 左輪加速、右輪減速 → 車頭往右轉回來。
 *   讀不到陀螺儀就馬上停車。
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
 *   2. 先把車架高測試：兩輪都應該往前轉。轉反了就改 LEFT_REVERSE／RIGHT_REVERSE。
 *   3. 車子左右扭來扭去（蛇行）→ KP 調小；歪了拉不回來 → KP 調大。
 *
 * 怎麼看結果：
 *   序列埠監控窗（115200）每 0.2 秒印一次：航向、修正量、左右輪速度。
 *   用手把車頭轉歪一點，會看到修正量變大、車子自己轉回 0 度。
 *
 * 小提醒：
 *   想改用 03 陀螺 1D 航向：把 HEADING_ID 改成 0x03（不受磁場影響，旁邊有馬達、鐵桌時比較穩）。
 *   不想一直走：把 RUN_MS 改成想走的毫秒數（例如 5000 = 走 5 秒就停）。
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
const int   BASE_SPEED   = 150;          // 往前走的基本速度 0~255
const float KP           = 4.0;          // 歪 1 度要修正多少速度（越大修得越用力）
const int   MAX_FIX      = 80;           // 修正量最多多少（不讓一邊輪子停下來或倒轉）
const uint8_t HEADING_ID = 0x02;         // 用哪一種航向：0x02 融合航向、0x03 陀螺 1D 航向
const unsigned long RUN_MS = 0;          // 走幾毫秒就停：0 = 一直走
const bool  LEFT_REVERSE  = false;       // 左輪轉反了改成 true
const bool  RIGHT_REVERSE = false;       // 右輪轉反了改成 true

unsigned long startAt = 0;               // 開始走的時間
unsigned long lastPrint = 0;             // 上一次印出來的時間

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

  mbcWrite(ADDR, 0xE0);                  // 命令 E0：航向歸零（現在車頭的方向 = 0 度）
  delay(100);
  Serial.println(F("已歸零。開始往前走直線。"));
  Serial.println(F("航向  修正量  左輪  右輪"));
  startAt = millis();
}

void loop() {
  // ---- 1. 走夠久了就停（RUN_MS = 0 就一直走） ----
  if (RUN_MS > 0 && millis() - startAt >= RUN_MS) {
    drive(0, 0);
    return;
  }

  // ---- 2. 讀航向：0 = 車頭正前方，往右歪變大、往左歪變小 ----
  int16_t heading = mbcRead(ADDR, HEADING_ID);
  if (heading == MBC_NONE) {             // 讀不到陀螺儀 → 為了安全，停車
    drive(0, 0);
    Serial.println(F("讀不到陀螺儀 → 停車"));
    delay(50);
    return;
  }

  // ---- 3. 算修正量：歪越多修越多，但不超過 MAX_FIX ----
  int fix = (int)(KP * heading);
  fix = constrain(fix, -MAX_FIX, MAX_FIX);
  int left  = BASE_SPEED - fix;          // 往右歪（fix 正）→ 左輪慢一點
  int right = BASE_SPEED + fix;          //                 → 右輪快一點，車頭轉回左邊
  drive(left, right);

  // ---- 4. 每 0.2 秒印一次 ----
  if (millis() - lastPrint >= 200) {
    lastPrint = millis();
    char line[40];
    snprintf(line, sizeof(line), "%4d  %6d  %4d  %4d", heading, fix, left, right);
    Serial.println(line);
  }
  delay(20);
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
