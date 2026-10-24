<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- moved: /sensors/exp6/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/exp6/arduino/ide.md">SPIKE 6 路擴充器 › Arduino › Arduino IDE</a></p>

# 🧪 SPIKE 六路馬達擴充器 EXP6 × Arduino 範例

[🔙 回到 EXP6 產品介紹](/sensors/exp6/index.md)

這一頁有 **6 個範例**，從「確認接好了沒」一路做到「兩輪小車」。Arduino 透過 **UART 序列埠**（TX／RX 兩條線）跟 EXP6 講話，一塊 Arduino 就能控制 6 顆 SPIKE 馬達與感應器。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/exp6/arduino/exp6_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

> [!NOTE]
> 出貨的 EXP6 就能直接用，**不用重燒韌體、不用改板子**。EXP6 一開機是「樂高模式」，收到 Arduino 送來的第一包指令，就會自動切到「UART 模式」（跟 Pybricks 用的是同一套指令）。

---

## 🧭 先選板子

| 板子 | 可以用嗎 | 跟 EXP6 講話用 | 說明 |
| :--- | :---: | :---: | :--- |
| **Arduino UNO R4**（Minima／WiFi） | ⭐ **推薦** | D0、D1（`Serial1`） | 長得跟 UNO 一樣，但 D0／D1 跟 USB 是**分開的兩組序列埠**，上傳程式、看序列埠監控窗都不受影響 |
| Arduino Mega 2560 | ✅ | 18、19（`Serial1`） | 有 4 組序列埠，板子比較大 |
| ESP32 開發板 | ✅ | GPIO16、17（`Serial2`） | 訊號是 3.3V，第 4 腳要接 **3V3**；要先安裝 ESP32 板子套件 |
| Arduino UNO R3、Nano | ❌ | — | 只有一組序列埠，已經被 USB 用掉了（見下方說明） |

> [!IMPORTANT]
> **為什麼 UNO R3 不能用？** EXP6 每 0.01 秒會回傳一包 35 個數字的資料，速度固定是 115200，不能改。UNO R3 唯一的一組硬體序列埠（D0／D1）接著 USB；改用「軟體序列埠」（SoftwareSerial）在 115200 收不穩，而且收資料的時候不能同時送指令。所以這頁的範例在 UNO R3 上會印出「這塊板子不能接 EXP6」然後停下來。

---

## 🔌 接線（所有範例都一樣）

EXP6 上接主機的那個樂高 6 針孔叫 **Host 孔**。用一條「樂高 6 針轉杜邦線」（或樂高接頭轉接板）把它接到 Arduino：

| EXP6 Host 孔 | Arduino UNO R4（推薦） | Mega 2560 | ESP32 |
| :---: | :---: | :---: | :---: |
| 第 3 腳 GND | GND | GND | GND |
| 第 4 腳 VCC | **5V** | **5V** | **3V3** |
| 第 5 腳（EXP6 收） | D1（TX） | 18（TX1） | GPIO17（TX2） |
| 第 6 腳（EXP6 送） | D0（RX） | 19（RX1） | GPIO16（RX2） |
| 第 1、2 腳 | 不接 | 不接 | 不接 |

**電源：** EXP6 的 XT30 電源座一定要接 **2～3S 鋰電池**，馬達和感應器都靠它供電。

> [!IMPORTANT]
> - **TX 接 EXP6 收、RX 接 EXP6 送**：Arduino 的 TX（送）要接 EXP6 的「收」（第 5 腳），兩條線是交叉的。
> - **第 4 腳的電壓要跟板子的訊號一樣**：UNO R4、Mega 接 5V，ESP32 接 3V3。EXP6 裡面有一顆隔離晶片，它的訊號電壓由這一腳決定；接錯會讓晶片超過額定。
> - **第 4 腳也是 EXP6 的「開機鑰匙」**：這一腳有電，EXP6 才會打開電池供電。只接電池、不接第 4 腳，EXP6 不會開機。
> - **EXP6 不會供電給 Arduino**：Arduino 要用 USB 或行動電源另外供電。
> - 接線前先拔掉 USB 線和電池。

---

## 🐶 一定要知道：看門狗

EXP6 有一隻「看門狗」：**超過 0.5 秒沒收到 Arduino 的任何指令，就會自動把馬達停下來**（正在轉指定角度、還沒轉到的馬達例外，會轉完才停）。這是為了 Arduino 當機或線鬆掉時，車子不會一直衝出去。

所以範例裡等待時一律用 **`exp6Wait(毫秒)`**，不用 `delay()`。`exp6Wait()` 等待的同時，每 0.1 秒會送一包「我還在」給 EXP6。`exp6Receive()`（收資料）也會順便送。

| 寫法 | 結果 |
| :--- | :--- |
| `motorRun(1, 50); exp6Wait(3000);` | ✅ 馬達轉 3 秒 |
| `motorRun(1, 50); delay(3000);` | ❌ 馬達轉 0.5 秒就被看門狗停下來 |

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟 EXP6 打招呼](#ex1) | 確認接好、看 6 個埠接了什麼 | 隨便幾顆馬達或感應器 |
| 2 | [馬達轉起來](#ex2) | 定速轉、煞車、滑行、看門狗 | 1 顆 SPIKE 馬達（第 1 埠） |
| 3 | [轉指定角度](#ex3) | 轉 90 度、轉一圈、轉到 0 度記號 | 同上 |
| 4 | [讀馬達角度](#ex4) | 把馬達當角度感應器、角度歸零 | 同上 |
| 5 | [讀感應器](#ex5) | 顏色、距離、力量 | SPIKE 顏色／超音波／力量感應器 |
| 6 | [兩輪小車（進階）](#ex6) | 兩顆馬達同步前進、轉彎 | 2 顆 SPIKE 馬達、樂高車身、行動電源 |

> [!TIP]
> **建議順序：1 → 2 → 4 → 3 → 5 → 6。** 先確認接好（1），再讓馬達動（2）、讀角度（4），最後才組車（6）。

---

<h2 id="ex1">1️⃣ 跟 EXP6 打招呼</h2>

**做什麼：** 確認 Arduino 跟 EXP6 接好了。每秒印一次電池電壓，還有 6 個埠各接了什麼。拔插馬達，表格會跟著變。

**步驟：**
1. 照上面的表格接好 4 條線，接上 EXP6 的電池，USB 插上電腦。
2. 複製下面的程式，貼到 Arduino IDE，**開發板選你的板子**（例如 Arduino UNO R4 Minima），按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/exp6/arduino/exp6_01_hello/exp6_01_hello.ino" download data-ignore="true">📄 下載 exp6_01_hello.ino</a>

<!-- ino: examples/exp6/arduino/exp6_01_hello/exp6_01_hello.ino -->
```cpp
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
```

**你會看到：**（第 1 埠接馬達、第 3 埠接顏色感應器、第 5 埠接超音波感應器）

```text
=== EXP6：跟 EXP6 打招呼 ===
連上了！每秒更新一次，試著拔插馬達看看。
電池 8.12 V | 1:馬達 2:沒接 3:顏色 4:沒接 5:超音波 6:沒接
電池 8.11 V | 1:馬達 2:沒接 3:顏色 4:沒接 5:超音波 6:沒接
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟 EXP6「講話」。你不用改它，也不用看懂。只要記得：
> - `exp6Send(L1, L2, L3, L4)`：送一筆指令，4 個數字，每個 0～100
> - `exp6Receive()`：收一包最新資料；之後用 `exp6Port(埠)` 讀那一埠的數字、`exp6Device(埠)` 看那一埠接了什麼
> - `exp6Wait(毫秒)`：取代 `delay()`
>
> EXP6 每 0.01 秒會主動送一包資料，裡面一次裝著 6 個埠的數字和電池電壓，Arduino 收一包就全部都有了。

> [!TIP]
> **剛插上的裝置要 1～2 秒才會被認出來**，表格上會先顯示「沒接」，等一下就會變。

---

<h2 id="ex2">2️⃣ 馬達轉起來</h2>

**做什麼：** 第 1 埠的馬達「正轉 3 秒 → 煞車 → 反轉 3 秒 → 滑行停下」，一直重複。EXP6 會自己調整出力，讓馬達維持你指定的速度。

**準備：** 1 顆 SPIKE 馬達插在 EXP6 的**第 1 埠**。

<a href="/examples/exp6/arduino/exp6_02_motor_run/exp6_02_motor_run.ino" download data-ignore="true">📄 下載 exp6_02_motor_run.ino</a>

<!-- ino: examples/exp6/arduino/exp6_02_motor_run/exp6_02_motor_run.ino -->
```cpp
/*
 * SPIKE 六路馬達擴充器 EXP6 × Arduino　範例 2：馬達轉起來
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓第 1 埠的馬達「正轉 3 秒 → 煞車 → 反轉 3 秒 → 滑行停下」，一直重複。
 *   轉的時候每 0.5 秒印一次馬達的角度。
 *   EXP6 會自己出力，讓馬達維持你指定的速度（就算有點負載也一樣）。
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
 *   上傳後，打開「序列埠監控窗」，速度選 115200，看馬達轉。
 *
 * 小提醒：
 *   EXP6 有「看門狗」：超過 0.5 秒沒收到 Arduino 的任何指令，就會自動停下馬達。
 *   所以等待時要用 exp6Wait()，不要用 delay()。
 */

const uint8_t MOTOR_PORT = 1;            // 👉 可以改：馬達插在 EXP6 的第幾埠（1 ~ 6）
const int8_t  SPEED = 50;                // 👉 可以改：速度 1 ~ 100（100 = 這顆馬達的最快速度）

// 停止的方式
const uint8_t STOP_COAST = 3;            // 滑行：放開馬達，讓它自己慢慢停（用手轉得動）
const uint8_t STOP_HOLD  = 4;            // 鎖住：用力咬住現在的角度（用手轉不動）
const uint8_t STOP_BRAKE = 5;            // 煞車：馬上停下，但用手還轉得動

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 要等電腦連上序列埠
  Serial.println(F("=== EXP6：馬達轉起來 ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("找不到 EXP6，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  // 等第 MOTOR_PORT 埠認出馬達（種類 5），最多等 3 秒
  if (!exp6WaitDevice(MOTOR_PORT, 5, 3000)) {
    Serial.print(F("第 "));
    Serial.print(MOTOR_PORT);
    Serial.println(F(" 埠沒有馬達，請把馬達插好，再按 Arduino 的重置鍵。"));
    while (true) {}
  }
  Serial.println(F("找到馬達了，開始轉！"));
}

void loop() {
  Serial.println(F("正轉 3 秒……"));
  motorRun(MOTOR_PORT, SPEED);           // 正數 = 順時針
  if (!runFor(3000)) return;             // 轉 3 秒，同時看著馬達；出問題就重來

  Serial.println(F("煞車！"));
  motorStop(MOTOR_PORT, STOP_BRAKE);
  exp6Wait(1000);                        // 停 1 秒

  Serial.println(F("反轉 3 秒……"));
  motorRun(MOTOR_PORT, -SPEED);          // 負數 = 逆時針
  if (!runFor(3000)) return;

  Serial.println(F("滑行停下。"));
  motorStop(MOTOR_PORT, STOP_COAST);
  exp6Wait(1000);
}

// 轉 ms 毫秒，每 0.5 秒印一次角度。收不到資料或馬達被拔掉，就停車並回傳 false
bool runFor(unsigned long ms) {
  unsigned long start = millis();
  unsigned long lastPrint = 0;
  while (millis() - start < ms) {
    if (!exp6Receive() || exp6Device(MOTOR_PORT) != 5) {
      motorStop(MOTOR_PORT, STOP_COAST); // 安全第一：先停車
      Serial.println(F("收不到馬達的資料，先停車！檢查馬達和電池。"));
      exp6Wait(1000);
      return false;
    }
    if (millis() - lastPrint >= 500) {
      lastPrint = millis();
      Serial.print(F("  角度 "));
      Serial.print(exp6Port(MOTOR_PORT)); // 馬達的埠，數字就是角度（度）
      Serial.println(F(" 度"));
    }
  }
  return true;
}

// ---------- 馬達指令小抄 ----------
// 定速轉：speed = -100 ~ 100。正數順時針、負數逆時針、0 = 停
void motorRun(uint8_t port, int8_t speed) {
  uint8_t dir = (speed >= 0) ? 1 : 2;    // L2：1 = 順時針、2 = 逆時針
  int16_t s = (speed >= 0) ? speed : -(int16_t)speed;
  if (s > 100) s = 100;
  exp6Send(port * 11, dir, s, 0);        // L1 = 埠號 × 11（11、22 … 66 = 定速轉）
}

// 停止：how = STOP_COAST、STOP_HOLD 或 STOP_BRAKE
void motorStop(uint8_t port, uint8_t how) {
  exp6Send(port * 10, how, 0, 0);        // L1 = 埠號 × 10，L2 = 停止方式
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
```

**你會看到：**

```text
=== EXP6：馬達轉起來 ===
找到馬達了，開始轉！
正轉 3 秒……
  角度 0 度
  角度 208 度
  角度 455 度
  …
煞車！
反轉 3 秒……
  角度 1290 度
  角度 1080 度
  …
滑行停下。
```

**三種停法：**

| 停法 | 程式 | 感覺 |
| :--- | :--- | :--- |
| 滑行 | `STOP_COAST` | 放開馬達，自己慢慢停，用手轉得動 |
| 煞車 | `STOP_BRAKE` | 馬上停，用手還轉得動 |
| 鎖住 | `STOP_HOLD` | 用力咬住現在的角度，用手轉不動 |

> [!TIP]
> **試試看：**
> 1. 把 `SPEED` 改成 `100`、`20`，看速度差多少。
> 2. 轉的時候用手輕輕捏住轉軸：EXP6 會出更大的力，想維持原來的速度。
> 3. 把 `runFor(3000)` 那一行改成 `delay(3000);`，看看看門狗怎麼在 0.5 秒後把馬達停下來。看完記得改回來！

---

<h2 id="ex3">3️⃣ 轉指定角度</h2>

**做什麼：** 馬達轉 90 度 → 轉回來 → 轉一整圈 → 轉到馬達上的「0 度記號」，每一步都印出現在的角度。

**準備：** 1 顆 SPIKE 馬達插在**第 1 埠**，轉軸插一根樑，比較看得出轉了多少。

<a href="/examples/exp6/arduino/exp6_03_motor_degrees/exp6_03_motor_degrees.ino" download data-ignore="true">📄 下載 exp6_03_motor_degrees.ino</a>

<!-- ino: examples/exp6/arduino/exp6_03_motor_degrees/exp6_03_motor_degrees.ino -->
```cpp
/*
 * SPIKE 六路馬達擴充器 EXP6 × Arduino　範例 3：轉指定角度
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓第 1 埠的馬達：轉 90 度 → 轉回來 → 轉一整圈 → 轉到馬達上的「0 度記號」，
 *   每做完一步就印出現在的角度，然後休息 2 秒再重來。
 *   角度控制是 EXP6 自己算的，Arduino 只要說「轉幾度」就好。
 *
 * 你需要：
 *   範例 1 的接線，加上 1 顆 SPIKE 馬達插在 EXP6 的第 1 埠。
 *   在馬達的轉軸插一根樑或輪子，比較看得出轉了多少。
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
 *
 * 小提醒：
 *   「轉角度」的指令只要送一次，EXP6 就會自己轉到位，不要一直重送。
 *   一次最多轉 2499 度（大約 7 圈）。
 */

const uint8_t MOTOR_PORT = 1;            // 👉 可以改：馬達插在 EXP6 的第幾埠（1 ~ 6）
const uint8_t SPEED = 40;                // 👉 可以改：速度 1 ~ 100

// 轉到位以後要怎麼停
const uint8_t AFTER_COAST = 0;           // 滑行（放開）
const uint8_t AFTER_BRAKE = 1;           // 煞車
const uint8_t AFTER_HOLD  = 2;           // 鎖住（用手轉不動）

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 要等電腦連上序列埠
  Serial.println(F("=== EXP6：轉指定角度 ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("找不到 EXP6，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  if (!exp6WaitDevice(MOTOR_PORT, 5, 3000)) {
    Serial.println(F("找不到馬達，請把馬達插好，再按 Arduino 的重置鍵。"));
    while (true) {}
  }
  Serial.println(F("找到馬達了！"));
}

void loop() {
  if (!turnDegrees(90, F("轉 90 度（順時針）"))) return;
  exp6Wait(1000);
  if (!turnDegrees(-90, F("轉回來 -90 度（逆時針）"))) return;
  exp6Wait(1000);
  if (!turnDegrees(360, F("轉一整圈 360 度"))) return;
  exp6Wait(1000);

  Serial.println(F("轉到馬達上的 0 度記號（自動走最近的路）"));
  motorGoTo(MOTOR_PORT, SPEED, 0, AFTER_HOLD);
  exp6Wait(1500);                        // 等它轉到
  Serial.println(F("  到了！看看馬達上的記號是不是對齊了。"));

  Serial.println(F("休息 2 秒……"));
  exp6Wait(2000);
}

// 轉 deg 度並等它轉到，印出結果。轉不到（例如被卡住）就停車並回傳 false
bool turnDegrees(int deg, const __FlashStringHelper *title) {
  Serial.println(title);
  if (!exp6Receive()) {                  // 先讀現在的角度
    Serial.println(F("收不到資料，檢查一下線有沒有鬆掉。"));
    exp6Wait(1000);
    return false;
  }
  int32_t target = exp6Port(MOTOR_PORT) + deg;   // 目標 = 現在的角度 + 要轉的角度
  motorDegrees(MOTOR_PORT, SPEED, deg, AFTER_HOLD);

  unsigned long start = millis();
  while (millis() - start < 5000) {      // 最多等 5 秒
    if (exp6Receive()) {
      int32_t angle = exp6Port(MOTOR_PORT);
      if (angle > target - 10 && angle < target + 10) {   // 差不到 10 度就算到了
        exp6Wait(300);                   // 等它停穩
        exp6Receive();
        Serial.print(F("  到了！現在角度 "));
        Serial.print(exp6Port(MOTOR_PORT));
        Serial.print(F(" 度（目標 "));
        Serial.print(target);
        Serial.println(F(" 度）"));
        return true;
      }
    }
  }
  motorStopNow(MOTOR_PORT);              // 安全第一：轉不到就放開馬達
  Serial.println(F("  5 秒還沒轉到，是不是被卡住了？先停下來。"));
  exp6Wait(2000);
  return false;
}

// ---------- 馬達指令小抄 ----------
// 轉相對角度：deg 正數順時針、負數逆時針，最多 ±2499 度；after = 到位後怎麼停
void motorDegrees(uint8_t port, uint8_t speed, int deg, uint8_t after) {
  uint8_t way = (deg >= 0) ? 8 : 9;      // 個位數 8 = 順時針、9 = 逆時針
  uint16_t d = (deg >= 0) ? deg : -deg;
  if (d > 2499) d = 2499;
  // L1 = 埠號 × 10 + 8 或 9；L3 = 停法 × 25 + 角度的百位數；L4 = 角度的後兩位
  exp6Send(port * 10 + way, speed, after * 25 + d / 100, d % 100);
}

// 轉到馬達上的絕對角度 0 ~ 359（0 = 馬達上的記號），EXP6 會自己挑最近的方向
void motorGoTo(uint8_t port, uint8_t speed, uint16_t angle, uint8_t after) {
  angle = angle % 360;
  // L1 = 埠號 × 10 + 7；L3 = 停法 × 4 + 角度的百位數；L4 = 角度的後兩位
  exp6Send(port * 10 + 7, speed, after * 4 + angle / 100, angle % 100);
}

// 馬上放開馬達（滑行）
void motorStopNow(uint8_t port) {
  exp6Send(port * 10, 3, 0, 0);          // L2 = 3：滑行
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
```

**你會看到：**

```text
=== EXP6：轉指定角度 ===
找到馬達了！
轉 90 度（順時針）
  到了！現在角度 90 度（目標 90 度）
轉回來 -90 度（逆時針）
  到了！現在角度 0 度（目標 0 度）
轉一整圈 360 度
  到了！現在角度 361 度（目標 360 度）
轉到馬達上的 0 度記號（自動走最近的路）
  到了！看看馬達上的記號是不是對齊了。
休息 2 秒……
```

**兩種「轉角度」：**

| | 轉相對角度 `motorDegrees` | 轉到絕對角度 `motorGoTo` |
| :--- | :--- | :--- |
| 意思 | 從現在的位置**再轉** N 度 | 轉到馬達上的**某個方向** |
| 範圍 | -2499 ～ 2499 度（約 7 圈） | 0 ～ 359 度 |
| 方向 | 你決定（正數順時針、負數逆時針） | EXP6 自動挑最近的路 |
| 例子 | 「再轉一圈」 | 「指針轉回 12 點鐘方向」 |

> [!TIP]
> **試試看：** 把 `turnDegrees(90, …)` 的 90 改成 `720`（轉兩圈）；或把 `motorGoTo(MOTOR_PORT, SPEED, 0, AFTER_HOLD)` 的 0 改成 `180`，看指針停在哪裡。

> [!WARNING]
> **轉角度的指令只送一次**，EXP6 會自己轉到位。不要放在 `loop()` 裡一直重送：轉到之後再收到同一筆指令，它會**再轉一次**。

---

<h2 id="ex4">4️⃣ 讀馬達角度</h2>

**做什麼：** SPIKE 馬達裡面有很準的角度感應器。用手轉馬達，電腦會印出轉了幾度、幾圈，還會畫一個 12 格的小錶盤，`#` 是現在指的方向。在序列埠監控窗輸入 `r` 按 Enter，角度歸零。

**準備：** 1 顆 SPIKE 馬達插在**第 1 埠**。

<a href="/examples/exp6/arduino/exp6_04_read_angle/exp6_04_read_angle.ino" download data-ignore="true">📄 下載 exp6_04_read_angle.ino</a>

<!-- ino: examples/exp6/arduino/exp6_04_read_angle/exp6_04_read_angle.ino -->
```cpp
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
```

**你會看到：**（順時針慢慢轉一圈多）

```text
=== EXP6：讀馬達角度 ===
用手轉轉看馬達！輸入 r 按 Enter 可以歸零。
[#...........] 角度 0 度，約 0.0 圈
[...#........] 角度 95 度，約 0.3 圈
[.........#..] 角度 287 度，約 0.8 圈
[#...........] 角度 372 度，約 1.0 圈
角度歸零！
[#...........] 角度 0 度，約 0.0 圈
```

> [!TIP]
> **試試看：** 逆時針轉，角度會變成負數。角度會一直累加，轉 10 圈就是 3600 度，不會繞回 0。

---

<h2 id="ex5">5️⃣ 讀感應器</h2>

**做什麼：** EXP6 會自動認出每一埠接的是什麼感應器。這個範例把顏色、距離、力量都印出來。有東西靠近超音波 10 公分內、或力量感應器被按下時，**Arduino 板上的燈**會亮。

**準備：** SPIKE 顏色、超音波或力量感應器，有幾個接幾個，插哪一埠都可以。

<a href="/examples/exp6/arduino/exp6_05_sensors/exp6_05_sensors.ino" download data-ignore="true">📄 下載 exp6_05_sensors.ino</a>

<!-- ino: examples/exp6/arduino/exp6_05_sensors/exp6_05_sensors.ino -->
```cpp
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
```

**你會看到：**（第 3 埠顏色、第 5 埠超音波、第 6 埠力量）

```text
=== EXP6：讀感應器 ===
埠 3 顏色：紅  反射光 41  紅 62 綠 9 藍 8
埠 5 距離：23.5 公分
埠 6 力量：0
--------
埠 3 顏色：白  反射光 96  紅 88 綠 90 藍 85
埠 5 距離：前面沒東西
埠 6 力量：57
--------
```

**顏色代號：**

| 代號 | 0 | 1 | 3 | 4 | 5 | 7 | 9 | 10 | 15 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 顏色 | 黑 | 紫 | 藍 | 天藍 | 綠 | 黃 | 紅 | 白 | 看不出來 |

> [!TIP]
> **試試看：** 把顏色感應器對著黑線和白紙移動，看「反射光」的數字怎麼變。循線車就是靠這個數字判斷自己在線的哪一邊。

---

<h2 id="ex6">6️⃣ 兩輪小車（進階）</h2>

**做什麼：** 用 2 顆 SPIKE 馬達當左右輪：前進 → 右轉 → 前進 → 左轉 → 後退 → 停，一直重複。兩輪用「同步」指令一起控制，EXP6 會讓兩輪互相配合，車子走得比較直。

**另外要準備：** 2 顆 SPIKE 馬達（左輪插**第 1 埠**、右輪插**第 2 埠**）、樂高車身、給 Arduino 用的行動電源。

**第一次跑之前：**
1. 先把車子架高（輪子懸空）再上傳，確認兩個輪子都是**往前**轉。哪個輪子轉反了，就把 `LEFT_REVERSE` 或 `RIGHT_REVERSE` 改過來。
2. 拔掉 USB，改插行動電源，放到地上，按 Arduino 的重置鍵，3 秒後出發。
3. 轉彎轉太多或太少，就調整 `TURN_MS`。

<a href="/examples/exp6/arduino/exp6_06_drive_car/exp6_06_drive_car.ino" download data-ignore="true">📄 下載 exp6_06_drive_car.ino</a>

<!-- ino: examples/exp6/arduino/exp6_06_drive_car/exp6_06_drive_car.ino -->
```cpp
/*
 * SPIKE 六路馬達擴充器 EXP6 × Arduino　範例 6（進階）：兩輪小車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用 2 顆 SPIKE 馬達當小車的左右輪，照順序：
 *   前進 → 右轉 → 前進 → 左轉 → 後退 → 停 3 秒，一直重複。
 *   兩顆馬達用「同步」指令一起控制，EXP6 會自動讓兩輪配合，車子走得比較直。
 *
 * 你需要：
 *   範例 1 的接線、2 顆 SPIKE 馬達：左輪插第 1 埠、右輪插第 2 埠。
 *   一台兩輪小車（樂高車身就可以）。Arduino 要另外供電（例如行動電源），
 *   因為 EXP6 的電池不會供電給 Arduino。
 *
 * 接線（UNO R4；Mega、ESP32 請看網頁的接線表）：
 *   EXP6 Host 孔第 3 腳（GND）     → GND
 *   EXP6 Host 孔第 4 腳（VCC）     → 5V
 *   EXP6 Host 孔第 5 腳（EXP6 收） → D1（TX）
 *   EXP6 Host 孔第 6 腳（EXP6 送） → D0（RX）
 *   EXP6 的 XT30 電源座            → 2～3S 鋰電池
 *
 * 怎麼看結果：
 *   先把車子架高（輪子懸空）上傳，確認兩個輪子都往「前」轉，
 *   哪個輪子轉反了，就把 LEFT_REVERSE 或 RIGHT_REVERSE 改過來。
 *   放到地上，按 Arduino 的重置鍵，3 秒後出發。
 *
 * 小提醒：
 *   兩個輪子的速度都不要剛好是 0（要停車請用 carStop）。
 */

const uint8_t LEFT_PORT  = 1;            // 👉 可以改：左輪馬達插第幾埠
const uint8_t RIGHT_PORT = 2;            // 👉 可以改：右輪馬達插第幾埠（不能跟左輪一樣）
const bool LEFT_REVERSE  = true;         // 👉 可以改：左輪轉反了就改 true / false
const bool RIGHT_REVERSE = false;        // 👉 可以改：右輪轉反了就改 true / false
const int8_t SPEED      = 40;            // 👉 可以改：前進速度 1 ~ 100
const int8_t TURN_SPEED = 25;            // 👉 可以改：轉彎速度 1 ~ 100
const uint16_t TURN_MS  = 900;           // 👉 可以改：轉彎要轉多久（毫秒），調到剛好轉 90 度

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // UNO R4 要等電腦連上序列埠
  Serial.println(F("=== EXP6：兩輪小車 ==="));

  if (!exp6Begin(3000)) {
    Serial.println(F("找不到 EXP6，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  if (!exp6WaitDevice(LEFT_PORT, 5, 3000) || !exp6WaitDevice(RIGHT_PORT, 5, 3000)) {
    Serial.println(F("左輪或右輪的馬達沒插好，插好後再按 Arduino 的重置鍵。"));
    while (true) {}
  }
  Serial.println(F("準備好了，3 秒後出發！"));
  exp6Wait(3000);
}

void loop() {
  Serial.println(F("前進"));
  carDrive(SPEED, SPEED);                // 左右輪一樣快 = 直走
  if (!driveFor(2000)) return;

  Serial.println(F("右轉"));
  carDrive(TURN_SPEED, -TURN_SPEED);     // 左輪往前、右輪往後 = 原地右轉
  if (!driveFor(TURN_MS)) return;

  Serial.println(F("前進"));
  carDrive(SPEED, SPEED);
  if (!driveFor(2000)) return;

  Serial.println(F("左轉"));
  carDrive(-TURN_SPEED, TURN_SPEED);     // 左輪往後、右輪往前 = 原地左轉
  if (!driveFor(TURN_MS)) return;

  Serial.println(F("後退"));
  carDrive(-SPEED, -SPEED);
  if (!driveFor(1500)) return;

  Serial.println(F("停車，休息 3 秒"));
  carStop();
  exp6Wait(3000);
}

// 開 ms 毫秒。收不到資料或馬達被拔掉，就停車並回傳 false
bool driveFor(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (!exp6Receive() || exp6Device(LEFT_PORT) != 5 || exp6Device(RIGHT_PORT) != 5) {
      carStop();                         // 安全第一：先停車
      Serial.println(F("收不到馬達的資料，先停車！檢查馬達和電池。"));
      exp6Wait(1000);
      return false;
    }
  }
  return true;
}

// ---------- 小車指令小抄 ----------
// 兩輪一起轉：left、right = -100 ~ 100（正數往前、負數往後）
void carDrive(int8_t left, int8_t right) {
  int16_t l = LEFT_REVERSE  ? -(int16_t)left  : left;    // 馬達裝反的那一邊，正負顛倒
  int16_t r = RIGHT_REVERSE ? -(int16_t)right : right;
  uint8_t dirs = 0;                      // L2：第 1 位元 = 左輪逆轉、第 0 位元 = 右輪逆轉
  if (l < 0) { dirs |= 2; l = -l; }
  if (r < 0) { dirs |= 1; r = -r; }
  if (l > 100) l = 100;
  if (r > 100) r = 100;
  // L1 = 左輪埠號 × 10 + 右輪埠號（例如 12）；L3 = 左輪速度；L4 = 右輪速度
  exp6Send(LEFT_PORT * 10 + RIGHT_PORT, dirs, l, r);
}

// 兩輪一起煞車
void carStop() {
  exp6Send(LEFT_PORT * 10 + RIGHT_PORT, 5, 0, 0);   // 兩輪速度都 0，L2 = 5：煞車
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
```

**你會看到：**（接著電腦時）

```text
=== EXP6：兩輪小車 ===
準備好了，3 秒後出發！
前進
右轉
前進
左轉
後退
停車，休息 3 秒
```

**怎麼調整：**

| 車子的樣子 | 怎麼改 |
| :--- | :--- |
| 一出發就原地打轉 | 有一個輪子轉反了，改 `LEFT_REVERSE` 或 `RIGHT_REVERSE` |
| 前進變後退 | 兩個 `_REVERSE` 都改成相反 |
| 轉彎超過 90 度 | `TURN_MS` 調小（例如 900 → 700） |
| 轉彎不到 90 度 | `TURN_MS` 調大 |

> [!WARNING]
> 兩個輪子的速度不要**只有一個剛好是 0**（例如 `carDrive(40, 0)`）：這時 EXP6 不走同步控制，改成直接出力，車子會比較不穩。要停車請用 `carStop()`。

---

## 📖 指令小抄（給想自己寫程式的人）

每一筆指令都是 6 個數字：`[0xA5, L1, L2, L3, L4, 檢查碼]`，檢查碼 = 前 5 個數字 XOR。用小幫手的 `exp6Send(L1, L2, L3, L4)` 送就好。**L1～L4 都要在 0～100 之間。**

| 想做的事 | L1 | L2 | L3 | L4 |
| :--- | :---: | :--- | :--- | :--- |
| 「我還在」（餵看門狗） | `1` | 0 | 0 | 0 |
| 全部重來（停車、角度歸零、格式回自動） | `0` | 0 | 0 | 0 |
| 定速轉 | 埠 × 11 | 1 順時針／2 逆時針 | 速度 0～100 | 0 |
| 直接出力（不穩速） | 埠 × 10 | 1 順時針／2 逆時針 | 力道 0～100 | 加速度（0 = 立刻） |
| 停止 | 埠 × 10 | 3 滑行／4 鎖住／5 煞車 | 0 | 0 |
| 轉相對角度 | 埠 × 10 + 8（順）或 + 9（逆） | 速度 1～100 | 停法 × 25 + 角度百位 | 角度後兩位 |
| 轉到絕對角度 | 埠 × 10 + 7 | 速度 1～100 | 停法 × 4 + 角度百位 | 角度後兩位 |
| 兩輪同步 | 左埠 × 10 + 右埠 | +2 左逆轉、+1 右逆轉 | 左速 | 右速 |
| 角度歸零 | 69 + 埠 | 3 | 0 | 0 |
| 改回傳格式 | 69 + 埠 | 0 速度／1 角度／2 絕對角度／255 自動 | 0 | 0 |
| 馬達正裝／反裝 | 76 | 埠 | 0 正裝／1 反裝 | 0 |
| PID 倍率 | 80 + 埠 | P | I | D（各 50 = 原廠值） |

- **停法**（轉角度用）：0 滑行、1 煞車、2 鎖住、3 繼續轉。
- **連續送兩筆一模一樣的指令**，第二筆會被略過（只餵看門狗）。轉角度、角度歸零不受這條限制。
- **EXP6 回傳的資料**：每 0.01 秒一包 `[0x5A, 32, R1～R8, 檢查碼]`。R1～R8 各 4 bytes（小的在前），檢查碼 = 第 2～34 個數字 XOR。R1～R6 是 6 個埠的數字；R7 的低 12 位元是電池電壓（× 100），R7 的第 12～14 位元是第 1 埠的裝置代號，R8 每 3 個位元一組是第 2～6 埠的裝置代號。

---

## ❓ 常見問題

**Q：一直出現「找不到 EXP6」？**
1. 檢查 TX、RX 有沒有接反：Arduino TX → 第 5 腳、Arduino RX → 第 6 腳。
2. 第 4 腳有沒有接到 5V（ESP32 接 3V3）？這一腳沒電，EXP6 不會開機。
3. EXP6 的電池有沒有接上、電量燈有沒有亮？
4. 確認板子是 UNO R4、Mega 或 ESP32。UNO R3、Nano 不能用。

**Q：EXP6 的第一顆綠燈一直慢慢閃？**
EXP6 已經切到 UART 模式，但超過 0.5 秒沒收到指令。通常是程式裡用了 `delay()`，或 Arduino 當機了。

**Q：馬達轉一下就停？**
看門狗把它停下來了：等待時要用 `exp6Wait()`，不要用 `delay()`。

**Q：馬達插好了，卻顯示「沒接」？**
剛插上的馬達要 1～2 秒才會被認出來。範例 1 等一下再看；如果一直是「沒接」，換一埠或換一條線試試。

**Q：UNO R4 上傳程式時，要先拔掉 D0、D1 的線嗎？**
不用。UNO R4 的 D0／D1 跟 USB 是分開的，上傳時不用拔線。（UNO R3 才需要拔。）

**Q：可以用 Arduino Leonardo 或 Micro 嗎？**
可以，它們的 D0／D1 也是獨立的 `Serial1`，接法跟 UNO R4 一樣。
