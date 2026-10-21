/*
 * 掌控者 PS4/PS5 × Arduino　範例 4：遙控小燈
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用手把遙控 Arduino 板子上內建的小燈（標著 L 的那顆）：
 *     按住 ✕ 鍵      → 燈一直亮
 *     按住 ● 鍵      → 燈熄滅
 *     什麼都不按     → 燈一閃一閃，左搖桿往上推越多，閃得越快
 *
 * 你需要：
 *   掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）。
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *
 * 怎麼看結果：
 *   看 Arduino 板子上的 L 燈。序列埠監控窗（115200）也會印出閃爍速度。
 *
 * 小提醒：
 *   手把沒連上時，按鍵和搖桿都是 0，燈會慢慢閃。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

// 按鍵在 16 位數字裡的位置（第幾位）
const uint8_t BTN_CROSS  = 14;           // ✕ 叉叉
const uint8_t BTN_CIRCLE = 13;           // ● 圓圈

// ---- 👉 可以改的參數 ----
const int SLOW_MS = 1000;                // 搖桿放開時：每 1 秒換一次（亮 → 熄 → 亮）
const int FAST_MS = 50;                  // 搖桿推到底時：每 0.05 秒換一次

bool ledOn = false;                      // 燈現在是亮還是熄
unsigned long lastToggle = 0;            // 上一次換燈的時間
int lastShown = -1;                      // 上一次印出來的速度（一樣就不重複印）

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // 把燈的腳設成「輸出」，才能控制它亮或熄
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！按 ✕ 燈亮、按 ● 燈熄，推左搖桿改變閃爍速度。"));
}

void loop() {
  // 讀手把資料（第 0x11 號格子，9 個數字）。readPad 在檔案下面，會順便檢查有沒有讀錯
  uint8_t d[9];
  if (!readPad(d)) {                     // 讀不到 → 為了安全，燈熄掉
    digitalWrite(LED, LOW);
    delay(100);
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // 16 位的按鍵狀態
  int ly = (int8_t)d[3];                 // 左搖桿上下：上 = 正（0 ~ +127）

  // 手把沒連線時，按鍵和搖桿都是 0，燈會照「什麼都不按」慢慢閃

  // ---- 1. 按住 ✕：燈一直亮 ----
  if (buttons & (1u << BTN_CROSS)) {
    digitalWrite(LED, HIGH);
    delay(20);
    return;
  }

  // ---- 2. 按住 ●：燈熄滅 ----
  if (buttons & (1u << BTN_CIRCLE)) {
    digitalWrite(LED, LOW);
    delay(20);
    return;
  }

  // ---- 3. 其他時候：一閃一閃，左搖桿越往上推，閃越快 ----
  if (ly < 0) ly = 0;                    // 往下推當作沒推
  int interval = map(ly, 0, 127, SLOW_MS, FAST_MS);   // 把 0~127 換算成 1000~50 毫秒

  if (millis() - lastToggle >= (unsigned long)interval) {   // 時間到了就換一次
    lastToggle = millis();
    ledOn = !ledOn;                      // 亮變熄、熄變亮
    digitalWrite(LED, ledOn ? HIGH : LOW);
  }

  // 速度有變才印出來
  if (abs(interval - lastShown) >= 50) {
    lastShown = interval;
    Serial.print(F("每 "));
    Serial.print(interval);
    Serial.println(F(" 毫秒換一次"));
  }

  delay(20);
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
  delay(2);                              // 等接收器準備資料 2 毫秒（PS4 接收器一定要，不能刪）
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

bool mbcWriteBytes(uint8_t reg, const uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(buf, n);
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
