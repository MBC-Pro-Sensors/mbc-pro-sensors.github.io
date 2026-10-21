/*
 * 掌控者 PS2 × Arduino　範例 2：按了哪一顆鍵？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   按下手把上的任何一顆鍵，電腦會用中文告訴你「按下：✕ 叉叉」，
 *   放開時說「放開：✕ 叉叉」。可以同時按好幾顆。
 *
 * 你需要：
 *   掌控者 PS2 接收器（通用 I2C 版）＋ PS2 2.4G 無線手把。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。打開手把電源，按按看。
 *
 * 小提醒：
 *   16 顆按鍵的狀態，放在一個 16 位的數字裡，一顆鍵佔一個「位」：
 *   按下 = 1，沒按 = 0。就像 16 個電燈開關排成一排。
 *   方向鍵「上下同時按」或「左右同時按」不會印出來：手把不可能這樣按，
 *   程式會當作「讀錯了」丟掉（看檔案下面的 readPad）。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

uint16_t lastButtons = 0;                // 上一次看到的按鍵狀態

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！按手把上的按鍵看看。"));
}

void loop() {
  // 讀「手把資料」：第 0x11 號格子，共 9 個數字（readPad 在檔案下面，會順便檢查有沒有讀錯）
  //   [0][1] 按鍵（16 位）  [2]~[5] 搖桿  [6][7] L2、R2 扳機  [8] 系統鍵
  uint8_t d[9];
  if (!readPad(d)) {
    delay(100);                          // 這次沒讀到，等一下再試
    return;
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // 把兩個數字拼成 16 位的按鍵狀態

  // 一顆一顆檢查：哪些鍵「剛按下」、哪些「剛放開」
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t mask = 1u << i;             // 第 i 顆鍵的「位」
    bool now    = buttons & mask;        // 現在有沒有按
    bool before = lastButtons & mask;    // 上一次有沒有按

    if (now && !before) {                // 剛按下
      Serial.print(F("按下："));
      printButtonName(i);
      Serial.println();
    }
    if (!now && before) {                // 剛放開
      Serial.print(F("放開："));
      printButtonName(i);
      Serial.println();
    }
  }

  lastButtons = buttons;                 // 記住這一次，下一次拿來比較
  delay(20);                             // 每 0.02 秒看一次，按很快也抓得到
}

// 印出第 i 顆鍵的中文名字（i = 第幾位）
void printButtonName(uint8_t i) {
  switch (i) {
    case 0:  Serial.print(F("SELECT 選擇鍵")); break;
    case 1:  Serial.print(F("L3（左搖桿往下壓）")); break;
    case 2:  Serial.print(F("R3（右搖桿往下壓）")); break;
    case 3:  Serial.print(F("START 開始鍵")); break;
    case 4:  Serial.print(F("↑ 方向鍵上")); break;
    case 5:  Serial.print(F("→ 方向鍵右")); break;
    case 6:  Serial.print(F("↓ 方向鍵下")); break;
    case 7:  Serial.print(F("← 方向鍵左")); break;
    case 8:  Serial.print(F("L2")); break;
    case 9:  Serial.print(F("R2")); break;
    case 10: Serial.print(F("L1")); break;
    case 11: Serial.print(F("R1")); break;
    case 12: Serial.print(F("▲ 三角形")); break;
    case 13: Serial.print(F("● 圓圈")); break;
    case 14: Serial.print(F("✕ 叉叉")); break;
    case 15: Serial.print(F("■ 正方形")); break;
  }
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
