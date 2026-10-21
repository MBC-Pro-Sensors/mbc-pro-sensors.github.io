/*
 * 掌控者 PS2 × Arduino　範例 1：跟接收器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認接收器有沒有接好。接好了，會在電腦上印出接收器的名字和版本。
 *   接著會一直看著手把：手把連上、斷線時都會告訴你。
 *   每次接新的接收器，都建議先跑這個範例。
 *
 * 你需要：
 *   掌控者 PS2 接收器（通用 I2C 版）＋ PS2 2.4G 無線手把（裝好電池）。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   再打開手把電源，看看電腦有沒有說「手把連上了」。
 *
 * 小提醒：
 *   手把中間有一顆 ANALOG 鍵。燈亮 = 類比模式（搖桿會動），
 *   燈滅 = 數位模式（只有按鍵會動、搖桿沒反應）。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟接收器講話

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的「門牌號碼」，固定是 0x42

bool    wasConnected = false;            // 上一次看的時候，手把有沒有連上
uint8_t lastType     = 0;                // 上一次看到的手把模式

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等接收器開機 0.5 秒

  Serial.println(F("=== 掌控者 PS2：跟接收器打招呼 ==="));

  // 等接收器說「我準備好了」，最多等 5 秒
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器！請檢查："));
    Serial.println(F("  1. RX（SDA）接 A4、TX（SCL）接 A5，有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？接收器上的燈有沒有亮？"));
    Serial.println(F("  3. 你的接收器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    while (true) {}                      // 找不到就停在這裡
  }

  // 讀「版本資料」：接收器的第 0x01 號格子，共 17 個數字
  uint8_t info[17];
  mbcRead(0x01, info, 17);

  // 第 9 ~ 16 個數字是接收器的名字（英文字母）
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("找到了！接收器型號："));
  Serial.println(name);                  // 應該會印出 PS2

  Serial.print(F("韌體版本："));
  Serial.print(info[2]);                 // 主版本
  Serial.print('.');
  Serial.println(info[3]);               // 次版本

  Serial.print(F("款式："));
  if (info[7] == 1)      Serial.println(F("PS2 2.4G 接收器"));
  else if (info[7] == 2) Serial.println(F("PS4/PS5 藍牙接收器"));
  else                   Serial.println(F("不認識的款式"));

  Serial.println(F("接線正確！現在打開手把電源試試看。"));
}

void loop() {
  // 讀「狀態資料」：第 0x31 號格子，共 9 個數字
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {
    Serial.println(F("讀不到接收器，線鬆掉了嗎？"));
    delay(1000);
    return;
  }

  bool    connected = st[0] & 0x02;      // 第 0 個數字的「第 1 位」：1 = 手把已連線
  uint8_t type      = st[2];             // 手把模式：0 = 沒連線、1 = 數位模式、2 = 類比模式

  // 跟上一次不一樣才印，畫面才不會一直洗版
  if (connected != wasConnected || type != lastType) {
    if (!connected) {
      Serial.println(F("手把沒有連線（請打開手把電源）"));
    } else if (type == 2) {
      Serial.println(F("手把連上了！（類比模式：按鍵、搖桿都能用）"));
    } else {
      Serial.println(F("手把連上了！（數位模式：搖桿不會動，請按手把中間的 ANALOG 鍵）"));
    }
    wasConnected = connected;
    lastType = type;
  }

  delay(200);                            // 每 0.2 秒看一次
}

// ============================================================
// 以下是「通訊小幫手」：負責跟接收器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從接收器的第 reg 號格子，讀 n 個數字放進 buf。讀到了回傳 true
bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等接收器準備資料 2 毫秒（PS4 版一定要；PS2 版留著也沒關係）
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// 把數字 value 寫進接收器的第 reg 號格子。成功回傳 true
bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

// 等接收器準備好，最多等 timeoutMs 毫秒。準備好了回傳 true
bool mbcWaitReady(uint16_t timeoutMs) {
  unsigned long start = millis();
  uint8_t flags;
  while (millis() - start < timeoutMs) {
    if (mbcRead(0x32, &flags, 1) && (flags & 0x01)) return true;
    delay(50);
  }
  return false;
}
