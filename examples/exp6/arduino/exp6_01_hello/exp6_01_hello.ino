/*
 * SPIKE 六路馬達擴充器 EXP6 × Arduino　範例 1：跟 EXP6 打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認 Arduino 跟 EXP6 有沒有接好。接好了，每秒會印出一次：
 *   電池電壓，還有 6 個埠各接了什麼（馬達、顏色、超音波、力量感應器）。
 *   試著拔掉、插上馬達，看看印出來的表會不會跟著變。
 *   每次重新接線，都建議先跑這個範例。
 *
 * 你需要：
 *   Arduino UNO R4（Minima 或 WiFi）、EXP6、2～3S 鋰電池、
 *   樂高 6 針轉杜邦線、幾顆 SPIKE 馬達或感應器。
 *
 * 接線（UNO R4；Mega、ESP32 請看網頁的接線表）：
 *   EXP6 Host 孔第 3 腳（GND）     → GND
 *   EXP6 Host 孔第 4 腳（VCC）     → 5V（EXP6 要有這條電才會開機）
 *   EXP6 Host 孔第 5 腳（EXP6 收） → D1（TX）
 *   EXP6 Host 孔第 6 腳（EXP6 送） → D0（RX）
 *   EXP6 的 XT30 電源座            → 2～3S 鋰電池
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   UNO R3、Nano 不能用：它們只有一組序列埠，已經被 USB 用掉了。
 */

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  while (!Serial && millis() < 3000) {}  // UNO R4 要等電腦連上序列埠，最多等 3 秒
  Serial.println(F("=== EXP6：跟 EXP6 打招呼 ==="));

  // 叫醒 EXP6，等它開始回傳資料，最多等 3 秒（小幫手在檔案最下面）
  if (!exp6Begin(3000)) {
    Serial.println(F("找不到 EXP6！請檢查："));
    Serial.println(F("  1. 第 5 腳接 TX、第 6 腳接 RX（UNO R4 是 D1、D0），有沒有接反？"));
    Serial.println(F("  2. 第 4 腳接 5V（ESP32 接 3V3）、第 3 腳接 GND，有沒有插緊？"));
    Serial.println(F("  3. EXP6 的電池有沒有接上？電量燈有沒有亮？"));
    Serial.println(F("  4. 你的板子是 UNO R4、Mega 或 ESP32 嗎？（UNO R3 不能用）"));
    while (true) {}                      // 找不到就停在這裡
  }
  Serial.println(F("連上了！每秒更新一次，試著拔插馬達看看。"));
}

void loop() {
  if (!exp6Receive()) {                  // 收一包最新資料
    Serial.println(F("收不到資料，檢查一下線有沒有鬆掉。"));
    exp6Wait(500);
    return;
  }

  uint16_t v = exp6Volt100();            // 電池電壓 × 100，例如 812 = 8.12 V
  Serial.print(F("電池 "));
  Serial.print(v / 100);                 // 整數部分
  Serial.print('.');
  if (v % 100 < 10) Serial.print('0');   // 小數不到 2 位數，前面補 0
  Serial.print(v % 100);
  Serial.print(F(" V |"));

  for (uint8_t port = 1; port <= 6; port++) {   // 第 1 埠到第 6 埠，一個一個看
    Serial.print(' ');
    Serial.print(port);
    Serial.print(':');
    printDeviceName(exp6Device(port));
  }
  Serial.println();

  exp6Wait(1000);                        // 等 1 秒（等的時候，小幫手會一直跟 EXP6 說「我還在」）
}

// 把裝置代號換成中文印出來
void printDeviceName(uint8_t kind) {
  switch (kind) {
    case 0:  Serial.print(F("沒接"));   break;
    case 1:  Serial.print(F("力量"));   break;
    case 3:  Serial.print(F("顏色"));   break;
    case 4:  Serial.print(F("超音波")); break;
    case 5:  Serial.print(F("馬達"));   break;
    default: Serial.print(F("其他"));   break;
  }
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
