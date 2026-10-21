/*
 * SPIKE 六路馬達擴充器 EXP6 × Arduino　範例 5：讀感應器
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   EXP6 會自動認出每一埠接的是什麼感應器，這個範例把它們的數值都印出來：
 *     顏色感應器：看到什麼顏色、反射光多亮、紅綠藍各多少
 *     超音波感應器：前面的東西離多遠（公分）
 *     力量感應器：按得多用力
 *   有東西靠近超音波 10 公分內、或力量感應器被按下時，Arduino 板上的燈會亮。
 *
 * 你需要：
 *   範例 1 的接線，加上 SPIKE 顏色、超音波或力量感應器（有幾個接幾個，插哪一埠都可以）。
 *
 * 接線（UNO R4；Mega、ESP32 請看網頁的接線表）：
 *   EXP6 Host 孔第 3 腳（GND）     → GND
 *   EXP6 Host 孔第 4 腳（VCC）     → 5V
 *   EXP6 Host 孔第 5 腳（EXP6 收） → D1（TX）
 *   EXP6 Host 孔第 6 腳（EXP6 送） → D0（RX）
 *   EXP6 的 XT30 電源座            → 2～3S 鋰電池
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 */

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const int16_t NEAR_MM   = 100;           // 👉 可以改：超音波多近（毫米）算「有東西靠近」，100 = 10 公分
const int16_t PRESS_MIN = 20;            // 👉 可以改：力量感應器超過多少算「被按下」

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 要等電腦連上序列埠
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println(F("=== EXP6：讀感應器 ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("找不到 EXP6，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  exp6Wait(1500);                        // 感應器插上後要 1 秒多才會被認出來
}

void loop() {
  if (!exp6Receive()) {
    Serial.println(F("收不到資料，檢查一下線有沒有鬆掉。"));
    digitalWrite(LED_BUILTIN, LOW);
    exp6Wait(500);
    return;
  }

  bool alert = false;                    // 有沒有東西靠近、或被按下
  uint8_t found = 0;                     // 找到幾個感應器

  for (uint8_t port = 1; port <= 6; port++) {
    uint8_t kind = exp6Device(port);
    int32_t value = exp6Port(port);      // 這一埠的數字，意思看接什麼感應器

    if (kind == 3) {                     // ---- 顏色感應器 ----
      found++;
      // 一個數字裡塞了 5 樣東西：顏色代號、反射光、紅、綠、藍
      uint32_t v = (uint32_t)value;
      uint8_t color = (v >> 28) & 0x0F;  // 顏色代號 0 ~ 10（15 = 看不出來）
      uint8_t refl  = (v >> 21) & 0x7F;  // 反射光 0 ~ 100（越亮越大，循線用）
      uint8_t r     = (v >> 14) & 0x7F;  // 紅 0 ~ 100
      uint8_t g     = (v >> 7)  & 0x7F;  // 綠 0 ~ 100
      uint8_t b     = v & 0x7F;          // 藍 0 ~ 100
      Serial.print(F("埠 "));
      Serial.print(port);
      Serial.print(F(" 顏色："));
      printColorName(color);
      Serial.print(F("  反射光 "));
      Serial.print(refl);
      Serial.print(F("  紅 "));
      Serial.print(r);
      Serial.print(F(" 綠 "));
      Serial.print(g);
      Serial.print(F(" 藍 "));
      Serial.println(b);
    } else if (kind == 4) {              // ---- 超音波感應器 ----
      found++;
      Serial.print(F("埠 "));
      Serial.print(port);
      Serial.print(F(" 距離："));
      if (value < 0 || value >= 2000) {  // 太遠或前面沒東西
        Serial.println(F("前面沒東西"));
      } else {                           // 數字的單位是毫米，除以 10 變公分
        Serial.print(value / 10);
        Serial.print('.');
        Serial.print(value % 10);
        Serial.println(F(" 公分"));
        if (value < NEAR_MM) alert = true;
      }
    } else if (kind == 1) {              // ---- 力量感應器 ----
      found++;
      Serial.print(F("埠 "));
      Serial.print(port);
      Serial.print(F(" 力量："));
      Serial.println(value);             // 0 = 沒按，按越用力數字越大
      if (value > PRESS_MIN) alert = true;
    }
  }

  if (found == 0) Serial.println(F("沒有找到感應器，插一個顏色、超音波或力量感應器看看。"));
  Serial.println(F("--------"));

  digitalWrite(LED_BUILTIN, alert ? HIGH : LOW);   // 有東西靠近或被按下 → 板子上的燈亮
  exp6Wait(500);                         // 👉 可以改：每 0.5 秒看一次
}

// 把顏色代號換成中文（樂高標準色碼）
void printColorName(uint8_t c) {
  switch (c) {
    case 0:  Serial.print(F("黑"));   break;
    case 1:  Serial.print(F("紫"));   break;
    case 3:  Serial.print(F("藍"));   break;
    case 4:  Serial.print(F("天藍")); break;
    case 5:  Serial.print(F("綠"));   break;
    case 7:  Serial.print(F("黃"));   break;
    case 9:  Serial.print(F("紅"));   break;
    case 10: Serial.print(F("白"));   break;
    case 15: Serial.print(F("看不出來")); break;
    default:
      Serial.print(F("其他代號 "));
      Serial.print(c);
      break;
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
