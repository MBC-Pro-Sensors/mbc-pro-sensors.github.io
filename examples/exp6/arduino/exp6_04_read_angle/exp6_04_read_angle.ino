/*
 * SPIKE 六路馬達擴充器 EXP6 × Arduino　範例 4：讀馬達角度
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   馬達不只會轉，還是一個很準的「角度感應器」。
 *   用手轉第 1 埠的馬達，電腦會一直印出它轉了幾度、幾圈，
 *   還會畫一個小錶盤標出現在指向哪裡。
 *   在序列埠監控窗輸入 r 再按 Enter，角度會歸零。
 *
 * 你需要：
 *   範例 1 的接線，加上 1 顆 SPIKE 馬達插在 EXP6 的第 1 埠。
 *
 * 接線（UNO R4；Mega、ESP32 請看網頁的接線表）：
 *   EXP6 Host 孔第 3 腳（GND）     → GND
 *   EXP6 Host 孔第 4 腳（VCC）     → 5V
 *   EXP6 Host 孔第 5 腳（EXP6 收） → D1（TX）
 *   EXP6 Host 孔第 6 腳（EXP6 送） → D0（RX）
 *   EXP6 的 XT30 電源座            → 2～3S 鋰電池
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200，慢慢用手轉馬達。
 *
 * 小提醒：
 *   開機時角度會自動歸零，所以一開始是 0 度。
 */

const uint8_t MOTOR_PORT = 1;            // 👉 可以改：馬達插在 EXP6 的第幾埠（1 ~ 6）

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 要等電腦連上序列埠
  Serial.println(F("=== EXP6：讀馬達角度 ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("找不到 EXP6，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  if (!exp6WaitDevice(MOTOR_PORT, 5, 3000)) {
    Serial.println(F("找不到馬達，請把馬達插好，再按 Arduino 的重置鍵。"));
    while (true) {}
  }
  Serial.println(F("用手轉轉看馬達！輸入 r 按 Enter 可以歸零。"));
}

void loop() {
  // 電腦傳來 r：角度歸零
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'r' || c == 'R') {
      exp6Send(69 + MOTOR_PORT, 3, 0, 0);  // L1 = 69 + 埠號（70 ~ 75），L2 = 3：角度歸零
      Serial.println(F("角度歸零！"));
      exp6Wait(100);
    }
  }

  if (!exp6Receive()) {
    Serial.println(F("收不到資料，檢查一下線有沒有鬆掉。"));
    exp6Wait(500);
    return;
  }
  if (exp6Device(MOTOR_PORT) != 5) {
    Serial.println(F("馬達被拔掉了？請插回去。"));
    exp6Wait(500);
    return;
  }

  int32_t angle = exp6Port(MOTOR_PORT);   // 馬達轉了幾度：順時針變大、逆時針變小，會一直累加

  // 畫一個小錶盤：一圈 360 度分成 12 格，# 是現在指的方向
  int16_t inTurn = ((angle % 360) + 360) % 360;   // 換算成一圈裡面的 0 ~ 359 度
  Serial.print('[');
  for (uint8_t i = 0; i < 12; i++) {
    if (inTurn / 30 == i) Serial.print('#');
    else Serial.print('.');
  }
  Serial.print(F("] 角度 "));
  Serial.print(angle);
  Serial.print(F(" 度，約 "));
  Serial.print(angle / 360.0, 1);        // 轉了幾圈，印到小數第 1 位
  Serial.println(F(" 圈"));

  exp6Wait(200);                         // 👉 可以改：每 0.2 秒看一次
}

// ============================================================
// 以下是「通訊小幫手」：負責跟 EXP6 講話（UART 序列埠）。
// 所有 EXP6 範例都一樣，不用改，也不用看懂。會用到的只有這幾個：
//   exp6Begin(毫秒)          叫醒 EXP6，等它開始回傳資料。成功回傳 true
//   exp6Send(L1, L2, L3, L4) 送一筆指令（4 個數字，每個 0 ~ 100）
//   exp6Receive()            收一包最新資料（6 個埠＋電池）。收到回傳 true
//   exp6Port(埠)             第幾埠的數字（馬達 = 角度、超音波 = 距離 …）
//   exp6Device(埠)           第幾埠接了什麼：0 沒接、1 力量、3 顏色、4 超音波、5 馬達、7 其他
//   exp6Volt100()            電池電壓 × 100（例如 812 = 8.12 V）
//   exp6Wait(毫秒)           取代 delay()：等待的同時，一直跟 EXP6 說「我還在」
//   exp6WaitDevice(埠, 種類, 毫秒)  等某一埠出現指定的裝置。等到了回傳 true
// ============================================================

// 用哪一組序列埠跟 EXP6 講話（依板子自動選，不用改）
#if defined(ARDUINO_ARCH_ESP32)
#define EXP6_SERIAL Serial2              // ESP32：RX2 = GPIO16、TX2 = GPIO17
#define EXP6_RX_PIN 16
#define EXP6_TX_PIN 17
#elif defined(ARDUINO_ARCH_RENESAS) || defined(HAVE_HWSERIAL1) || defined(ARDUINO_ARCH_MEGAAVR)
#define EXP6_SERIAL Serial1              // UNO R4：D0(RX) / D1(TX)；Mega：19(RX1) / 18(TX1)
#endif

uint8_t exp6Data[32];                    // 最新一包資料：8 個數字，每個 4 bytes
unsigned long exp6LastSend = 0;          // 上一次送指令的時間

// 叫醒 EXP6：一直送「我還在」，直到收到 EXP6 回傳的資料，最多等 timeoutMs 毫秒
bool exp6Begin(uint16_t timeoutMs) {
#ifdef EXP6_SERIAL
#if defined(ARDUINO_ARCH_ESP32)
  EXP6_SERIAL.begin(115200, SERIAL_8N1, EXP6_RX_PIN, EXP6_TX_PIN);
#else
  EXP6_SERIAL.begin(115200);             // EXP6 固定用 115200 的速度講話
#endif
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    exp6Send(1, 0, 0, 0);                // 收到第一包正確的指令，EXP6 就從「樂高模式」切到「UART 模式」
    if (exp6Receive()) {
      exp6Send(0, 1, 0, 0);              // 先送一筆「什麼都不做」的指令，下一筆才不會被當成重複指令略過
      exp6Send(0, 0, 0, 0);              // 全部重來：馬達停下、角度歸零
      exp6Wait(300);
      return exp6Receive();
    }
  }
#else
  (void)timeoutMs;                       // 這種板子用不到這個數字
  Serial.println(F("這塊板子沒有第二組硬體序列埠（例如 UNO R3、Nano），不能接 EXP6。"));
  Serial.println(F("請改用 UNO R4、Mega 2560 或 ESP32。"));
#endif
  return false;
}

// 送一筆指令：[0xA5, L1, L2, L3, L4, 檢查碼]
void exp6Send(uint8_t L1, uint8_t L2, uint8_t L3, uint8_t L4) {
  uint8_t pkt[6] = { 0xA5, L1, L2, L3, L4, 0 };
  pkt[5] = pkt[0] ^ L1 ^ L2 ^ L3 ^ L4;   // 檢查碼：前 5 個數字 XOR 起來
#ifdef EXP6_SERIAL
  EXP6_SERIAL.write(pkt, 6);
#endif
  exp6LastSend = millis();
}

// 超過 0.1 秒沒送東西，就送一包「我還在」（L1 = 1），不會讓馬達做任何事
void exp6KeepAlive() {
  if (millis() - exp6LastSend >= 100) exp6Send(1, 0, 0, 0);
}

// 取代 delay()：EXP6 超過 0.5 秒沒收到指令，會自動把馬達停下來（看門狗）
void exp6Wait(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    exp6KeepAlive();
    delay(1);
  }
}

// 收一包最新資料：[0x5A, 32, 32 個數字, 檢查碼]，共 35 bytes。EXP6 每 0.01 秒送一包
bool exp6Receive() {
#ifdef EXP6_SERIAL
  exp6KeepAlive();
  while (EXP6_SERIAL.available()) EXP6_SERIAL.read();   // 丟掉舊資料，只要最新的
  static uint8_t buf[32];
  uint8_t n = 0, x = 0;
  unsigned long start = millis();
  while (millis() - start < 100) {
    if (!EXP6_SERIAL.available()) continue;
    uint8_t b = EXP6_SERIAL.read();
    if (n == 0) {                        // 找開頭記號 0x5A
      if (b == 0x5A) n = 1;
    } else if (n == 1) {                 // 第 2 個一定是 32（後面有 32 個數字）
      if (b == 32) { x = b; n = 2; } else n = (b == 0x5A) ? 1 : 0;
    } else if (n < 34) {                 // 32 個數字
      buf[n - 2] = b;
      x ^= b;
      n++;
    } else {                             // 最後一個是檢查碼，對了才收下
      if (b == x) {
        memcpy(exp6Data, buf, 32);
        return true;
      }
      n = 0;
    }
  }
#endif
  return false;
}

// 第 port 埠的數字（4 個 bytes 組成一個數字，小的在前面）
int32_t exp6Port(uint8_t port) {
  if (port < 1 || port > 6) return 0;
  const uint8_t *d = &exp6Data[(port - 1) * 4];
  return (int32_t)((uint32_t)d[0] | ((uint32_t)d[1] << 8) | ((uint32_t)d[2] << 16) | ((uint32_t)d[3] << 24));
}

// 第 port 埠接了什麼：每個埠用 3 個位元記錄
uint8_t exp6Device(uint8_t port) {
  uint16_t r7 = exp6Data[24] | ((uint16_t)exp6Data[25] << 8);   // 第 7 個數字：電壓＋第 1 埠
  uint16_t r8 = exp6Data[28] | ((uint16_t)exp6Data[29] << 8);   // 第 8 個數字：第 2 ~ 6 埠
  if (port == 1) return (r7 >> 12) & 0x07;
  if (port >= 2 && port <= 6) return (r8 >> ((port - 2) * 3)) & 0x07;
  return 0;
}

// 電池電壓 × 100
uint16_t exp6Volt100() {
  return (exp6Data[24] | ((uint16_t)exp6Data[25] << 8)) & 0x0FFF;
}

// 等第 port 埠出現 kind 這種裝置（例如 5 = 馬達），最多等 timeoutMs 毫秒
bool exp6WaitDevice(uint8_t port, uint8_t kind, uint16_t timeoutMs) {
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (exp6Receive() && exp6Device(port) == kind) {
      exp6Wait(200);                     // 剛認出來，再給 EXP6 0.2 秒準備
      return true;
    }
  }
  return false;
}
