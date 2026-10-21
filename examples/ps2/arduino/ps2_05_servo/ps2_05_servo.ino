/*
 * 掌控者 PS2 × Arduino　範例 5（進階）：伺服馬達跟著搖桿轉
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   左搖桿往左右推，SG90 伺服馬達就跟著轉：
 *     推到最左 = 0 度，放開（中間）= 90 度，推到最右 = 180 度。
 *   可以拿來做機械手臂的夾子、會轉頭的機器人。
 *
 * 你需要：
 *   掌控者 PS2 接收器（通用 I2C 版）＋ PS2 2.4G 無線手把、SG90 伺服馬達 1 顆。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *   SG90 棕線 → GND    紅線 → 5V    橘線（訊號）→ 9
 *   （ESP32 的訊號線請接 GPIO 13）
 *
 * 怎麼看結果：
 *   推左搖桿，看伺服馬達轉。序列埠監控窗（115200）會印出角度。
 *
 * 小提醒：
 *   1. 搖桿沒反應？按一下手把中間的 ANALOG 鍵，讓燈亮起來（類比模式）。
 *   2. 這個範例自己產生伺服馬達的訊號，不用 Servo 函式庫，
 *      所以 UNO、Nano、UNO R4、ESP32 都能直接用。
 *   3. 伺服馬達一直抖、或 Arduino 一直重開機：USB 的電不夠，
 *      請把伺服馬達的紅線、棕線改接外接電池（4~6V），電池的負極也要接 Arduino GND。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

#if defined(ESP32)
const int SERVO_PIN = 13;                // ESP32：訊號線接 GPIO 13
#else
const int SERVO_PIN = 9;                 // UNO / Nano / R4：訊號線接第 9 腳
#endif

// ---- 👉 可以改的參數 ----
const int MIN_ANGLE = 0;                 // 搖桿推到最左時的角度
const int MAX_ANGLE = 180;               // 搖桿推到最右時的角度（夾子只要轉一點點，可以改小）

int angle = 90;                          // 伺服馬達現在的角度，開機先在中間
unsigned long lastPulse = 0;             // 上一次送訊號的時間
unsigned long lastPrint = 0;             // 上一次印角度的時間

void setup() {
  Serial.begin(115200);
  pinMode(SERVO_PIN, OUTPUT);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！左右推左搖桿，伺服馬達會跟著轉。"));
}

void loop() {
  // 伺服馬達要「每 20 毫秒」收到一次訊號，才會乖乖停在那個角度
  if (millis() - lastPulse < 20) return;
  lastPulse = millis();

  // 讀手把資料（第 0x11 號格子，9 個數字）。readPad 在檔案下面，會順便檢查有沒有讀錯
  uint8_t d[9];
  if (readPad(d)) {
    int lx = (int8_t)d[2];               // 左搖桿左右：-127（最左）~ 0（中間）~ +127（最右）
    angle = map(lx, -127, 127, MIN_ANGLE, MAX_ANGLE);   // 換算成角度
  } else {
    angle = 90;                          // 讀不到接收器 → 回到中間比較安全
  }
  // 手把沒連線時，搖桿讀到的是 0，伺服馬達也會回到中間

  servoPulse(angle);                     // 送一次訊號給伺服馬達

  if (millis() - lastPrint >= 300) {     // 每 0.3 秒印一次角度
    lastPrint = millis();
    Serial.print(F("角度："));
    Serial.print(angle);
    Serial.println(F(" 度"));
  }
}

// 送一個脈衝給伺服馬達：高電位維持多久，就代表要轉到幾度
//   0 度 ≈ 0.544 毫秒、90 度 ≈ 1.47 毫秒、180 度 ≈ 2.4 毫秒（跟 Arduino 的 Servo 函式庫一樣）
void servoPulse(int deg) {
  deg = constrain(deg, 0, 180);
  int us = map(deg, 0, 180, 544, 2400);  // 角度 → 微秒（1 毫秒 = 1000 微秒）
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(us);
  digitalWrite(SERVO_PIN, LOW);
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
