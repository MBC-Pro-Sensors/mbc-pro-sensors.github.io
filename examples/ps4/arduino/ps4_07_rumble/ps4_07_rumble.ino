/*
 * 掌控者 PS4/PS5 × Arduino　範例 7：讓手把震動
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用 Arduino 叫手把震動：
 *     慢慢壓 R2 扳機  → 手把跟著震，壓越深震越大力
 *     按住 ✕ 鍵      → 用最大力氣震
 *     放開           → 馬上停
 *   以後可以拿來做「撞到牆就震一下」「時間到了震動提醒」。
 *
 * 你需要：
 *   掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *
 * 怎麼看結果：
 *   壓 R2、按 ✕，感覺手把在震。序列埠監控窗（115200）會印出震動力氣。
 *
 * 小提醒：
 *   把力氣（0 ~ 255）寫進接收器的第 0x10 號格子，手把就會震 0.2 秒；寫 0 = 停。
 *   想一直震，就要每 0.1 秒再寫一次，像一直按門鈴一樣。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

const uint8_t BTN_CROSS = 14;            // ✕ 叉叉在 16 位按鍵數字裡的位置

uint8_t lastPower = 0;                   // 上一次的震動力氣
unsigned long lastSend  = 0;             // 上一次送出震動命令的時間
unsigned long lastPrint = 0;             // 上一次印出力氣的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！慢慢壓 R2 扳機，或按住 ✕ 鍵。"));
}

void loop() {
  // 讀手把資料（第 0x11 號格子，9 個數字）。readPad 在檔案下面，會順便檢查有沒有讀錯
  uint8_t d[9];
  if (!readPad(d)) {
    delay(100);
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // 16 位的按鍵狀態
  uint8_t  r2      = d[7];               // R2 扳機：0（沒按）~ 255（壓到底）

  uint8_t power = r2;                    // 壓多深，就震多大力
  if (buttons & (1u << BTN_CROSS)) power = 255;      // 按住 ✕ → 最大力氣

  if (power > 0) {
    // 要震：每 0.1 秒寫一次（每寫一次會震 0.2 秒，這樣才會接起來不中斷）
    if (millis() - lastSend >= 100) {
      lastSend = millis();
      mbcWrite(0x10, power);             // 第 0x10 號格子 = 震動力氣
    }
  } else if (lastPower > 0) {
    mbcWrite(0x10, 0);                   // 剛放開 → 寫 0，馬上停
    Serial.println(F("停止震動"));
  }

  if (power > 0 && power != lastPower && millis() - lastPrint >= 300) {
    lastPrint = millis();
    Serial.print(F("震動力氣："));
    Serial.println(power);
  }

  lastPower = power;
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
