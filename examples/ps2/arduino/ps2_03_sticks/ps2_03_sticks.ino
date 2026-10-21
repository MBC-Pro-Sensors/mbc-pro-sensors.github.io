/*
 * 掌控者 PS2 × Arduino　範例 3：搖桿推多少？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把兩支搖桿的位置變成數字，印在電腦上：
 *     放開（在中間）= 0
 *     往右推、往上推 = 正數，推到底 = +127
 *     往左推、往下推 = 負數，推到底 = -127
 *   另外也印出 L2、R2 兩個扳機：沒按 = 0，按下 = 255
 *   （手把支援「壓力感應」的話，會依按的力道給 0 ~ 255）。
 *
 * 你需要：
 *   掌控者 PS2 接收器（通用 I2C 版）＋ PS2 2.4G 無線手把。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。慢慢推搖桿，看數字怎麼變。
 *
 * 小提醒：
 *   1. 搖桿一直是 0？按一下手把中間的 ANALOG 鍵，讓燈亮起來（類比模式）。
 *   2. 搖桿在中間附近輕輕晃，數字會停在 0，這是接收器故意的（叫做「死區」），
 *      這樣放開搖桿時車子才不會自己慢慢跑。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

bool    connected = false;               // 手把有沒有連上（每 0.4 秒更新一次）
uint8_t type      = 0;                   // 手把模式：1 = 數位模式、2 = 類比模式
unsigned long lastStatus = 0;            // 上一次看「手把有沒有連上」的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！推推看搖桿。"));
}

void loop() {
  // ---- 1. 每 0.4 秒看一次手把有沒有連上：第 0x31 號格子（狀態資料）----
  //   不要每一圈都讀：一直在兩個格子之間換來換去，接收器比較容易交錯資料
  if (millis() - lastStatus >= 400) {
    lastStatus = millis();
    uint8_t st[9];
    if (mbcRead(0x31, st, 9)) {
      connected = st[0] & 0x02;          // 第 0 個數字的「第 1 位」：1 = 手把已連線
      type      = st[2];                 // 1 = 數位模式、2 = 類比模式
    } else {
      connected = false;                 // 讀不到接收器，當作沒連線
    }
  }

  if (!connected) {
    Serial.println(F("手把沒有連線（請打開手把電源）"));
    delay(1000);
    return;
  }
  if (type == 1) {
    Serial.println(F("手把在數位模式，搖桿不會動！請按手把中間的 ANALOG 鍵（燈會亮）"));
    delay(1000);
    return;
  }

  // ---- 2. 讀「手把資料」：第 0x11 號格子，共 9 個數字 ----
  //   readPad 在檔案下面，會順便檢查有沒有讀錯
  uint8_t d[9];
  if (!readPad(d)) {
    delay(200);
    return;
  }

  // 搖桿數字有正有負，要用 int8_t（可以裝 -128 ~ 127 的盒子）來看
  int lx = (int8_t)d[2];                 // 左搖桿 左右：右 = 正
  int ly = (int8_t)d[3];                 // 左搖桿 上下：上 = 正
  int rx = (int8_t)d[4];                 // 右搖桿 左右：右 = 正
  int ry = (int8_t)d[5];                 // 右搖桿 上下：上 = 正
  int l2 = d[6];                         // L2 扳機：0 ~ 255
  int r2 = d[7];                         // R2 扳機：0 ~ 255

  // 排整齊印出來。%+4d = 「帶正負號、佔 4 格」，%3d = 「佔 3 格」
  char line[80];
  snprintf(line, sizeof(line), "左搖桿 X%+4d Y%+4d | 右搖桿 X%+4d Y%+4d | L2 %3d R2 %3d",
           lx, ly, rx, ry, l2, r2);
  Serial.println(line);

  delay(200);                            // 👉 可以改：每 0.2 秒印一次，想看快一點就改小
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
