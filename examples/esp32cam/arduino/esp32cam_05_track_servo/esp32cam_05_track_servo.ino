/*
 * 神攝手 ESP32CAM × Arduino　範例 5：伺服馬達跟著色塊轉（進階）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把神攝手裝在 SG90 伺服馬達上，色塊往哪邊跑，馬達就往哪邊轉：
 *     讀編號 05 色塊 X（0~100）→ 換算成角度，送給伺服馬達。
 *     沒找到色塊（0A = 0）就回到 90 度中間。
 *   不用安裝任何函式庫：UNO／Nano／UNO R4 自己每 20 ms 送一個脈衝；ESP32 用內建 ledc 送 50 Hz 訊號。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。另外要：SG90 伺服馬達 1 顆。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *   SG90 棕線 → GND    紅線 → 5V    橘線（訊號）→ 9
 *   （ESP32 的訊號線請接 GPIO 13）
 *
 * 準備：
 *   先在神攝手背面的螢幕設定好取樣點 S1 和要追的顏色（設定存在機身）。
 *   把神攝手固定在伺服馬達的擺臂上，鏡頭朝前。
 *
 * 怎麼看結果：
 *   拿目標顏色的東西左右移動，伺服馬達會跟著轉。序列埠監控窗（115200）會印角度。
 *
 * 小提醒：
 *   這支是「學校版」韌體用的（編號 01~0A）。WRO 版（HUE12）請看範例 6。
 *   轉的方向跟色塊相反的話，把 REVERSE 改成 true。
 *   伺服馬達一直抖或 Arduino 一直重開：USB 電不夠，紅線、棕線改接外接電池（4~6V），負極也要接 GND。
 */

#include <Wire.h>

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

#if defined(ESP32)
const int SERVO_PIN = 13;                // ESP32：訊號線接 GPIO 13
#else
const int SERVO_PIN = 9;                 // UNO / Nano / R4：訊號線接第 9 腳
#endif

// ---- 👉 可以改的參數 ----
const bool REVERSE   = false;            // 轉的方向反了就改 true
const int  MIN_ANGLE = 20;               // 色塊在最左時的角度
const int  MAX_ANGLE = 160;              // 色塊在最右時的角度

int angle = 90;
unsigned long lastRead = 0;
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  servoBegin();                          // 準備伺服馬達（UNO 自己送脈衝、ESP32 用 ledc）
  servoWrite(90);                        // 先轉到中間
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿目標顏色的東西左右移動。"));
}

void loop() {
  // ---- 1. 每 0.1 秒讀一次神攝手：編號 05~0A 共 6 個值：[0] X … [5] 有找到 ----
  if (millis() - lastRead >= 100) {
    lastRead = millis();
    int16_t b[6];
    if (!mbcReadMany(ADDR, 0x05, 6, b) || !b[5]) {
      angle = 90;                        // 讀不到、或沒找到色塊 → 回中間
    } else {
      int x = constrain(b[0], 0, 100);
      angle = REVERSE ? map(x, 0, 100, MAX_ANGLE, MIN_ANGLE) : map(x, 0, 100, MIN_ANGLE, MAX_ANGLE);
    }
  }

  // ---- 2. 每一圈都送訊號給伺服馬達（UNO 要每 20 ms 送一次脈衝，它才會停在那個角度） ----
  servoWrite(angle);

  if (millis() - lastPrint >= 300) {     // 每 0.3 秒印一次角度
    lastPrint = millis();
    Serial.print(F("角度：")); Serial.print(angle); Serial.println(F(" 度"));
  }
  delay(20);                             // 這支不是 50 ms：伺服馬達的脈衝要每 20 ms 一次
}

// ---- 伺服馬達：兩種板子兩種做法，呼叫方式一樣 ----
//   0 度 ≈ 0.544 ms、90 度 ≈ 1.47 ms、180 度 ≈ 2.4 ms 高電位，每 20 ms 一次
#if defined(ESP32)
// ESP32 用內建的 ledc 送 50 Hz（20 ms 一次）PWM，設定一次就會自己一直送
void servoBegin() {
  ledcAttach(SERVO_PIN, 50, 16);         // 50 Hz、16 位解析度（0~65535）
}
void servoWrite(int deg) {
  deg = constrain(deg, 0, 180);
  long us = map(deg, 0, 180, 544, 2400); // 角度 → 微秒
  ledcWrite(SERVO_PIN, (uint32_t)(us * 65535L / 20000L));   // 20000 微秒 = 一個週期
}
#else
// UNO / Nano / R4 不用函式庫：自己把腳拉高「幾微秒」再拉低，就是一個脈衝
void servoBegin() { pinMode(SERVO_PIN, OUTPUT); }
void servoWrite(int deg) {
  deg = constrain(deg, 0, 180);
  int us = map(deg, 0, 180, 544, 2400);  // 角度 → 微秒
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(us);
  digitalWrite(SERVO_PIN, LOW);
}
#endif

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
