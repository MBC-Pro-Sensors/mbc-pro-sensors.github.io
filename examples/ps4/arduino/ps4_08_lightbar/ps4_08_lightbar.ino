/*
 * 掌控者 PS4/PS5 × Arduino　範例 8：換手把燈條的顏色
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用 Arduino 改變手把燈條的顏色：
 *     按 ✕ → 藍色    按 ● → 紅色    按 ■ → 粉紅色    按 ▲ → 綠色
 *     按 PS 鍵 → 隨機變一個顏色
 *     慢慢壓 L2 扳機 → 燈變暗，壓到底就熄掉
 *   比賽時可以讓紅隊、藍隊的手把亮不同顏色，一眼就認得出來。
 *
 * 你需要：
 *   掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *
 * 怎麼看結果：
 *   看手把上的燈條。序列埠監控窗（115200）會印出顏色。
 *
 * 小提醒：
 *   顏色用「紅、綠、藍」3 個數字（各 0 ~ 255）調出來，跟調顏料一樣。
 *   3 個數字要一次寫進接收器的第 0x23 號格子。
 *   手把還沒連上時寫進去也沒關係，接收器會先記住，連上後就套用。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

// 形狀鍵在 16 位按鍵數字裡的位置（第幾位）
const uint8_t BTN_TRIANGLE = 12, BTN_CIRCLE = 13, BTN_CROSS = 14, BTN_SQUARE = 15;

uint8_t baseColor[3] = {0, 0, 255};      // 選好的顏色（紅, 綠, 藍），一開始是藍色
uint8_t lastBright = 255;                // 上一次送出的亮度
uint16_t lastButtons = 0;                // 上一次的按鍵狀態
bool lastPs = false;                     // 上一次 PS 鍵有沒有按
unsigned long lastSend = 0;              // 上一次送出顏色的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }

  // 第 0x23 號格子可以讀也可以寫：先讀出接收器記住的顏色
  uint8_t c[3];
  if (mbcRead(0x23, c, 3)) {
    Serial.print(F("接收器現在記住的燈條顏色："));
    printColor(c);
  }
  randomSeed(micros());                  // 讓「隨機顏色」每次開機都不一樣
  Serial.println(F("準備好了！按 ✕ ● ■ ▲ 或 PS 鍵換顏色，壓 L2 變暗。"));
  sendColor(255);                        // 先送一次藍色
}

void loop() {
  // 讀手把資料（第 0x11 號格子，9 個數字）。readPad 在檔案下面，會順便檢查有沒有讀錯
  uint8_t d[9];
  if (!readPad(d)) {
    delay(100);
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];
  uint16_t pressed = buttons & ~lastButtons;          // 「剛按下」的鍵
  lastButtons = buttons;
  bool changed = false;                  // 這一次有沒有換顏色

  if (pressed & (1u << BTN_CROSS))    { setBase(0, 0, 255);   changed = true; }   // ✕ 藍
  if (pressed & (1u << BTN_CIRCLE))   { setBase(255, 0, 0);   changed = true; }   // ● 紅
  if (pressed & (1u << BTN_SQUARE))   { setBase(255, 0, 128); changed = true; }   // ■ 粉紅
  if (pressed & (1u << BTN_TRIANGLE)) { setBase(0, 255, 0);   changed = true; }   // ▲ 綠

  bool ps = d[8] & 0x01;                 // PS 鍵：第 8 個數字（系統鍵）的第 0 位
  if (ps && !lastPs) {                   // 剛按下 PS 鍵 → 隨機顏色
    setBase(random(256), random(256), random(256));
    changed = true;
  }
  lastPs = ps;

  // L2 扳機壓越深，燈越暗：0（沒壓）→ 亮度 255，255（壓到底）→ 亮度 0
  uint8_t bright = 255 - d[6];

  if (changed) {
    sendColor(bright);
    Serial.print(F("換顏色："));
    printColor(baseColor);
  } else if (abs((int)bright - (int)lastBright) >= 8 && millis() - lastSend >= 50) {
    sendColor(bright);                   // 亮度變了才送，而且最快 0.05 秒送一次，不要塞爆通訊
  }

  delay(20);
}

// 記住選好的顏色
void setBase(uint8_t r, uint8_t g, uint8_t b) {
  baseColor[0] = r;
  baseColor[1] = g;
  baseColor[2] = b;
}

// 照亮度調暗之後，把 3 個數字一次寫進第 0x23 號格子
void sendColor(uint8_t bright) {
  uint8_t rgb[3];
  for (uint8_t i = 0; i < 3; i++) {
    rgb[i] = (uint16_t)baseColor[i] * bright / 255;   // 先轉成 uint16_t，乘起來才不會爆掉
  }
  mbcWriteBytes(0x23, rgb, 3);
  lastBright = bright;
  lastSend = millis();
}

// 印出「紅 x 綠 y 藍 z」
void printColor(const uint8_t *c) {
  Serial.print(F("紅 "));
  Serial.print(c[0]);
  Serial.print(F("　綠 "));
  Serial.print(c[1]);
  Serial.print(F("　藍 "));
  Serial.println(c[2]);
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
