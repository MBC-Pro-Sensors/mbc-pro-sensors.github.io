<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 掌控者 PS4/PS5 × Arduino 範例

[🔙 回到 PS4/PS5 遙控接收器首頁](/sensors/ps4/index.md)　｜　[∿ MBC 通用 I2C 協議（進階）](/i2c-protocol.md)

這一頁有 **8 個範例**，從「確認接好了沒」一路做到「坦克遙控車」，還有 PS4/PS5 手把才有的**震動**和**燈條**。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/ps4/arduino/ps4_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 接收器 | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: |
| VCC | 5V | 5V（VIN） |
| GND | GND | GND |
| RX（= SDA） | A4 | GPIO 21 |
| TX（= SCL） | A5 | GPIO 22 |

> [!IMPORTANT]
> - 請用 **通用 I2C 版** 的接收器。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 接收器上印的是 **TX、RX**：I2C 版的 **TX 就是 SCL、RX 就是 SDA**。
> - 接收器裡面已經有電壓轉換，UNO（5 V）和 ESP32（3.3 V）都可以直接接。
> - 接線前先拔掉 USB 線。

---

<h2 id="pair">🔗 第一次用：讓手把跟接收器配對</h2>

手把要先「配對」，接收器才認得它。只要做一次，之後就會自動連上。

1. 照上面接好線，Arduino 插上 USB（接收器就會開機）。
2. **第一次用**的接收器，開機後會自動等手把 **30 秒**；**已經配對過、想換手把**，就按接收器上的 **Reset** 鍵，一樣等 30 秒。接收器的綠燈快速閃爍，就是在等手把。
3. 在這 30 秒內讓手把進入配對模式：
   - **PS4 手把**：同時長按 **SHARE + PS** 鍵，直到燈條快速閃爍。
   - **PS5 手把**：同時長按 **建立（Create）+ PS** 鍵，直到燈條快速閃爍。
4. 連上之後接收器會記住這支手把。以後只要按手把的 **PS** 鍵，就會自動連上。

> [!TIP]
> 教室裡很多組一起配對時，請**一組一組輪流**做，避免 30 秒內別組的手把也在配對模式，連到別人的接收器。綁錯了就再按一次 Reset 重新配對。

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟接收器打招呼](#ex1) | 確認接線正確、手把有沒有連上、剩多少電 | 無 |
| 2 | [按了哪一顆鍵？](#ex2) | 讀 16 顆按鍵和 PS 鍵 | 無 |
| 3 | [搖桿推多少？](#ex3) | 讀搖桿和類比扳機的數字 | 無 |
| 4 | [遙控小燈](#ex4) | 用按鍵、搖桿控制板子上的燈 | 無 |
| 5 | [伺服馬達跟著搖桿轉（進階）](#ex5) | 搖桿控制角度 | SG90 伺服馬達 |
| 6 | [坦克遙控車（進階）](#ex6) | 用兩支搖桿開車、斷線自動停車 | 兩輪小車、L298N 馬達驅動板 |
| 7 | [讓手把震動](#ex7) | 用 Arduino 叫手把震動 | 無 |
| 8 | [換燈條顏色](#ex8) | 改手把燈條的顏色和亮度 | 無 |

> [!TIP]
> **建議順序：1 → 2 → 3 → 4 → 7 → 8 → 5 → 6。** 7、8 不用接任何零件，可以先玩；5、6 要另外準備零件。

---

<h2 id="ex1">1️⃣ 跟接收器打招呼</h2>

**做什麼：** 確認接收器接好了，並且看手把有沒有連上、剩多少電。每次接新的接收器，都先跑這一個。

**步驟：**
1. 照上面的表格接好 4 條線，USB 插上電腦。
2. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。
4. 按手把的 **PS** 鍵，看電腦有沒有說「手把連上了」。（還沒配對過的手把，請先照上面 [配對](#pair) 的步驟做。）

<a href="/examples/ps4/arduino/ps4_01_hello/ps4_01_hello.ino" download data-ignore="true">📄 下載 ps4_01_hello.ino</a>

<!-- ino: examples/ps4/arduino/ps4_01_hello/ps4_01_hello.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 1：跟接收器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認接收器有沒有接好。接好了，會在電腦上印出接收器的名字和版本。
 *   接著會一直看著手把：手把連上、斷線時都會告訴你，連上時還會說剩多少電。
 *   每次接新的接收器，都建議先跑這個範例。
 *
 * 你需要：
 *   掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   再按手把中間的 PS 鍵開機，看看電腦有沒有說「手把連上了」。
 *
 * 第一次用，手把要先「配對」：
 *   1. 接收器第一次開機（還沒配對過）會自動等手把 30 秒；
 *      已經配對過、想換手把，就按接收器上的 Reset 鍵，一樣等 30 秒。
 *   2. 在這 30 秒內，PS4 手把同時長按 SHARE + PS 鍵（PS5 手把長按 建立 + PS 鍵），
 *      直到手把的燈條快速閃爍。
 *   3. 連上之後接收器會記住這支手把，下次開機只要按 PS 鍵就會自動連上。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟接收器講話

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的「門牌號碼」，固定是 0x42

bool    wasConnected = false;            // 上一次看的時候，手把有沒有連上
uint8_t lastType     = 0;                // 上一次看到的手把種類

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等接收器開機 0.5 秒

  Serial.println(F("=== 掌控者 PS4/PS5：跟接收器打招呼 ==="));

  // 等接收器說「我準備好了」，最多等 5 秒（PS4 接收器開機要 1 秒多）
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
  Serial.println(name);                  // 應該會印出 PS4

  Serial.print(F("韌體版本："));
  Serial.print(info[2]);                 // 主版本
  Serial.print('.');
  Serial.println(info[3]);               // 次版本

  Serial.print(F("款式："));
  if (info[7] == 1)      Serial.println(F("PS2 2.4G 接收器"));
  else if (info[7] == 2) Serial.println(F("PS4/PS5 藍牙接收器"));
  else                   Serial.println(F("不認識的款式"));

  Serial.println(F("接線正確！現在按手把的 PS 鍵開機試試看。"));
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
  uint8_t type      = st[2];             // 手把種類：0 = 沒連線、3 = PS4 手把、4 = PS5 或其他藍牙手把
  uint8_t battery   = st[3];             // 手把電量：0 = 不知道，255 = 滿電

  // 跟上一次不一樣才印，畫面才不會一直洗版
  if (connected != wasConnected || type != lastType) {
    if (!connected) {
      Serial.println(F("手把沒有連線（請按手把的 PS 鍵；第一次用要先配對）"));
    } else {
      if (type == 3) Serial.print(F("PS4 手把連上了！"));
      else           Serial.print(F("PS5（或其他藍牙）手把連上了！"));
      if (battery > 0) {
        Serial.print(F("電量約 "));
        Serial.print((int)battery * 100 / 255);   // 0~255 換算成 0~100%
        Serial.print('%');
      }
      Serial.println();
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
  delay(2);                              // 等接收器準備資料 2 毫秒（PS4 接收器一定要，不能刪）
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

// 一次寫好幾個數字（例如燈條的紅、綠、藍 3 個數字）。成功回傳 true
bool mbcWriteBytes(uint8_t reg, const uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(buf, n);
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
```

**你會看到：**（手把連上，之後長按 PS 鍵 10 秒讓手把關機）

```text
=== 掌控者 PS4/PS5：跟接收器打招呼 ===
找到了！接收器型號：PS4
韌體版本：1.0
款式：PS4/PS5 藍牙接收器
接線正確！現在按手把的 PS 鍵開機試試看。
PS4 手把連上了！電量約 80%
手把沒有連線（請按手把的 PS 鍵；第一次用要先配對）
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟接收器「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(格子編號, 放哪裡, 幾個)`：從接收器讀資料
> - `mbcWrite(格子編號, 數字)`：把一個數字寫進接收器（例如震動力氣）
> - `mbcWriteBytes(格子編號, 放哪裡, 幾個)`：一次寫好幾個數字（例如燈條的紅、綠、藍）
>
> 接收器就像一排有編號的「格子」，每一格放著不同的資料，例如第 `0x11` 格放著「手把的按鍵和搖桿」，第 `0x31` 格放著「手把有沒有連上」。
>
> PS4/PS5 接收器的小幫手裡有一行 `delay(2)`（等 2 毫秒再讀），**不能刪**，刪了會讀到錯的資料。
>
> 範例 2 以後，小幫手上面還多一段 `readPad`：讀手把資料（第 `0x11` 格）時順便「防呆」，讀錯的資料會丟掉。它也是每個範例都一樣，不用改。為什麼要防呆，請看 [常見問題](#faq-updown)。

---

<h2 id="ex2">2️⃣ 按了哪一顆鍵？</h2>

**做什麼：** 按下手把上任何一顆鍵，電腦會用中文告訴你按了哪一顆；放開時也會說。可以同時按好幾顆，手把中間的 **PS 鍵**也看得到。

<a href="/examples/ps4/arduino/ps4_02_buttons/ps4_02_buttons.ino" download data-ignore="true">📄 下載 ps4_02_buttons.ino</a>

<!-- ino: examples/ps4/arduino/ps4_02_buttons/ps4_02_buttons.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 2：按了哪一顆鍵？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   按下手把上的任何一顆鍵，電腦會用中文告訴你「按下：✕ 叉叉」，
 *   放開時說「放開：✕ 叉叉」。可以同時按好幾顆。手把中間的 PS 鍵也看得到。
 *
 * 你需要：
 *   掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。按 PS 鍵讓手把連上，按按看。
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
bool     lastPs      = false;            // 上一次 PS 鍵有沒有按

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

  // PS 鍵不在那 16 位裡，放在第 8 個數字（系統鍵）的「第 0 位」
  bool ps = d[8] & 0x01;
  if (ps && !lastPs) Serial.println(F("按下：PS 鍵"));
  if (!ps && lastPs) Serial.println(F("放開：PS 鍵"));
  lastPs = ps;

  lastButtons = buttons;                 // 記住這一次，下一次拿來比較
  delay(20);                             // 每 0.02 秒看一次，按很快也抓得到
}

// 印出第 i 顆鍵的中文名字（i = 第幾位）
void printButtonName(uint8_t i) {
  switch (i) {
    case 0:  Serial.print(F("SHARE 鍵（PS5 叫「建立」鍵）")); break;
    case 1:  Serial.print(F("L3（左搖桿往下壓）")); break;
    case 2:  Serial.print(F("R3（右搖桿往下壓）")); break;
    case 3:  Serial.print(F("OPTIONS 鍵")); break;
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
```

**你會看到：**（按 ✕、再同時按 ↑ 和 R1，最後按 PS 鍵）

```text
準備好了！按手把上的按鍵看看。
按下：✕ 叉叉
放開：✕ 叉叉
按下：↑ 方向鍵上
按下：R1
放開：↑ 方向鍵上
放開：R1
按下：PS 鍵
放開：PS 鍵
```

（方向鍵「上下同時按」或「左右同時按」不會印出來，因為程式把它當作讀錯了，原因請看 [常見問題](#faq-updown)。）

**16 顆按鍵各在第幾位？**

| 位 | 按鍵 | 位 | 按鍵 |
| :---: | :--- | :---: | :--- |
| 0 | SHARE（PS5：建立） | 8 | L2 |
| 1 | L3（左搖桿往下壓） | 9 | R2 |
| 2 | R3（右搖桿往下壓） | 10 | L1 |
| 3 | OPTIONS | 11 | R1 |
| 4 | ↑ 上 | 12 | ▲ 三角形 |
| 5 | → 右 | 13 | ● 圓圈 |
| 6 | ↓ 下 | 14 | ✕ 叉叉 |
| 7 | ← 左 | 15 | ■ 正方形 |

PS 鍵不在這 16 位裡，而是放在第 8 個數字（系統鍵）的第 0 位：`d[8] & 0x01`。

> [!TIP]
> **試試看：** 想知道「✕ 有沒有按」，寫 `if (buttons & (1u << 14)) { ... }` 就好。`1u << 14` 的意思是「第 14 位」。

---

<h2 id="ex3">3️⃣ 搖桿推多少？</h2>

**做什麼：** 把兩支搖桿的位置變成數字：放開 = `0`，往右、往上推是**正數**，往左、往下推是**負數**，推到底是 `+127` 或 `-127`。也會印出 L2、R2 **類比扳機**：沒壓 `0`、壓到底 `255`，壓一半大約 `128`。

<a href="/examples/ps4/arduino/ps4_03_sticks/ps4_03_sticks.ino" download data-ignore="true">📄 下載 ps4_03_sticks.ino</a>

<!-- ino: examples/ps4/arduino/ps4_03_sticks/ps4_03_sticks.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 3：搖桿推多少？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把兩支搖桿的位置變成數字，印在電腦上：
 *     放開（在中間）= 0
 *     往右推、往上推 = 正數，推到底 = +127
 *     往左推、往下推 = 負數，推到底 = -127
 *   另外也印出 L2、R2 兩個扳機：沒按 = 0，壓到底 = 255，壓一半大約 128。
 *
 * 你需要：
 *   掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。慢慢推搖桿，看數字怎麼變。
 *
 * 小提醒：
 *   搖桿在中間附近輕輕晃，數字會停在 0，這是接收器故意的（叫做「死區」），
 *   這樣放開搖桿時車子才不會自己慢慢跑。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

bool connected = false;                  // 手把有沒有連上（每 0.4 秒更新一次）
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
    connected = mbcRead(0x31, st, 9) && (st[0] & 0x02);   // 第 0 個數字的「第 1 位」：1 = 手把已連線
  }

  if (!connected) {
    Serial.println(F("手把沒有連線（請按手把的 PS 鍵）"));
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
  int l2 = d[6];                         // L2 扳機：0（沒按）~ 255（壓到底）
  int r2 = d[7];                         // R2 扳機：0（沒按）~ 255（壓到底）

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
```

**你會看到：**（先放開，再把左搖桿推到最上面，最後左搖桿往左推一半、右搖桿推到最右、L2 壓一半）

```text
準備好了！推推看搖桿。
左搖桿 X  +0 Y  +0 | 右搖桿 X  +0 Y  +0 | L2   0 R2   0
左搖桿 X  +0 Y+127 | 右搖桿 X  +0 Y  +0 | L2   0 R2   0
左搖桿 X -64 Y  +0 | 右搖桿 X+127 Y  +0 | L2 128 R2   0
```

> [!NOTE]
> 搖桿在中間附近輕輕晃時數字會停在 `0`，這是接收器故意的（叫做「死區」），放開搖桿時車子才不會自己慢慢跑。所以搖桿數字不會出現 `1` ~ `7` 這麼小的值。

---

<h2 id="ex4">4️⃣ 遙控小燈</h2>

**做什麼：** 用 Arduino 板子上內建的小燈（標著 **L** 那顆）練習遙控：按住 **✕** 燈一直亮、按住 **●** 燈熄滅；什麼都不按時燈一閃一閃，**左搖桿往上推越多，閃得越快**。

<a href="/examples/ps4/arduino/ps4_04_led_blink/ps4_04_led_blink.ino" download data-ignore="true">📄 下載 ps4_04_led_blink.ino</a>

<!-- ino: examples/ps4/arduino/ps4_04_led_blink/ps4_04_led_blink.ino -->
```cpp
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
```

**你會看到：**（左搖桿慢慢往上推到底）

```text
準備好了！按 ✕ 燈亮、按 ● 燈熄，推左搖桿改變閃爍速度。
每 1000 毫秒換一次
每 626 毫秒換一次
每 50 毫秒換一次
```

> [!TIP]
> **試試看：** 把 `int ly = (int8_t)d[3];` 改成 `int ly = d[7] / 2;`（R2 扳機 0 ~ 255 除以 2，變成 0 ~ 127），就變成「扳機壓越深，閃越快」。

---

<h2 id="ex5">5️⃣ 伺服馬達跟著搖桿轉（進階）</h2>

**做什麼：** 左搖桿往左右推，SG90 伺服馬達就跟著轉：最左 0 度、放開 90 度、最右 180 度。可以拿來做機械手臂的夾子、會轉頭的機器人。

**另外要準備：** SG90 伺服馬達 1 顆。

**伺服馬達接線：**

| SG90 的線 | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: |
| 棕色 | GND | GND |
| 紅色 | 5V | 5V（VIN） |
| 橘色（訊號） | 9 | GPIO 13 |

<a href="/examples/ps4/arduino/ps4_05_servo/ps4_05_servo.ino" download data-ignore="true">📄 下載 ps4_05_servo.ino</a>

<!-- ino: examples/ps4/arduino/ps4_05_servo/ps4_05_servo.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 5（進階）：伺服馬達跟著搖桿轉
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   左搖桿往左右推，SG90 伺服馬達就跟著轉：
 *     推到最左 = 0 度，放開（中間）= 90 度，推到最右 = 180 度。
 *   可以拿來做機械手臂的夾子、會轉頭的機器人。
 *
 * 你需要：
 *   掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、SG90 伺服馬達 1 顆。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *   SG90 棕線 → GND    紅線 → 5V    橘線（訊號）→ 9
 *   （ESP32 的訊號線請接 GPIO 13）
 *
 * 怎麼看結果：
 *   推左搖桿，看伺服馬達轉。序列埠監控窗（115200）會印出角度。
 *
 * 小提醒：
 *   1. 這個範例自己產生伺服馬達的訊號，不用 Servo 函式庫，
 *      所以 UNO、Nano、UNO R4、ESP32 都能直接用。
 *   2. 伺服馬達一直抖、或 Arduino 一直重開機：USB 的電不夠，
 *      請把伺服馬達的紅線、棕線改接外接電池（4~6V），電池的負極也要接 Arduino GND。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

#if defined(ESP32)
const int SERVO_PIN = 13;                // ESP32：訊號線接 GPIO 13
#else
const int SERVO_PIN = 9;                 // UNO / Nano / R4：訊號線接第 9 腳
#endif

// ---- 👉 可以改的參數 ----
const int MIN_ANGLE = 0;                 // 搖桿推到最左時的角度
const int MAX_ANGLE = 180;               // 搖桿推到最右時的角度（夾子只要轉一點點，可以改小）

int angle = 90;                          // 伺服馬達現在的角度，開機先在中間
unsigned long lastPulse = 0;             // 上一次送訊號的時間
unsigned long lastPrint = 0;             // 上一次印角度的時間

void setup() {
  Serial.begin(115200);
  pinMode(SERVO_PIN, OUTPUT);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！左右推左搖桿，伺服馬達會跟著轉。"));
}

void loop() {
  // 伺服馬達要「每 20 毫秒」收到一次訊號，才會乖乖停在那個角度
  if (millis() - lastPulse < 20) return;
  lastPulse = millis();

  // 讀手把資料（第 0x11 號格子，9 個數字）。readPad 在檔案下面，會順便檢查有沒有讀錯
  uint8_t d[9];
  if (readPad(d)) {
    int lx = (int8_t)d[2];               // 左搖桿左右：-127（最左）~ 0（中間）~ +127（最右）
    angle = map(lx, -127, 127, MIN_ANGLE, MAX_ANGLE);   // 換算成角度
  } else {
    angle = 90;                          // 讀不到接收器 → 回到中間比較安全
  }
  // 手把沒連線時，搖桿讀到的是 0，伺服馬達也會回到中間

  servoPulse(angle);                     // 送一次訊號給伺服馬達

  if (millis() - lastPrint >= 300) {     // 每 0.3 秒印一次角度
    lastPrint = millis();
    Serial.print(F("角度："));
    Serial.print(angle);
    Serial.println(F(" 度"));
  }
}

// 送一個脈衝給伺服馬達：高電位維持多久，就代表要轉到幾度
//   0 度 ≈ 0.544 毫秒、90 度 ≈ 1.47 毫秒、180 度 ≈ 2.4 毫秒（跟 Arduino 的 Servo 函式庫一樣）
void servoPulse(int deg) {
  deg = constrain(deg, 0, 180);
  int us = map(deg, 0, 180, 544, 2400);  // 角度 → 微秒（1 毫秒 = 1000 微秒）
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(us);
  digitalWrite(SERVO_PIN, LOW);
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
```

**你會看到：**（左搖桿從中間推到最右，再推到最左）

```text
準備好了！左右推左搖桿，伺服馬達會跟著轉。
角度：90 度
角度：134 度
角度：180 度
角度：0 度
```

> [!NOTE]
> 這個範例**自己產生伺服馬達的訊號**，不用 Servo 函式庫，所以 UNO、Nano、UNO R4、ESP32 都能直接用。伺服馬達一直抖、或 Arduino 一直重開機，代表 USB 的電不夠：把伺服馬達的紅線、棕線改接外接電池（4～6 V），電池負極也要接 Arduino 的 GND。

> [!TIP]
> **試試看：** 做夾子時只需要轉一點點，把 `MIN_ANGLE` 改成 `60`、`MAX_ANGLE` 改成 `120`。

---

<h2 id="ex6">6️⃣ 坦克遙控車（進階）</h2>

**做什麼：** 用手把開兩輪小車，開法跟坦克一樣：**左搖桿控制左輪、右搖桿控制右輪**。兩支一起往上推是直走，一前一後是原地轉彎。搖桿放開時也可以用**方向鍵**開。讀不到接收器、或手把斷線，車子會立刻停下來。

**另外要準備：** 兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。

**馬達接線：**

| L298N | Arduino UNO / Nano | ESP32 | 控制 |
| :---: | :---: | :---: | :--- |
| ENA | 5 | GPIO 25 | 左輪速度 |
| IN1 / IN2 | 7 / 8 | GPIO 26 / 27 | 左輪方向 |
| ENB | 6 | GPIO 14 | 右輪速度 |
| IN3 / IN4 | 9 / 10 | GPIO 32 / 33 | 右輪方向 |
| GND | GND | GND | **一定要跟 Arduino 共地** |

**第一次跑之前：**
1. 把車子架高（輪子懸空）再上傳，兩支搖桿一起往上推，確認兩個輪子都是**往前**轉。哪個輪子轉反了，就把 `LEFT_REVERSE` 或 `RIGHT_REVERSE` 改成 `true`。
2. **安全測試：** 車子還架高時，長按手把的 PS 鍵 10 秒讓手把關機，確認輪子會停下來，再放到地上開。

<a href="/examples/ps4/arduino/ps4_06_tank_car/ps4_06_tank_car.ino" download data-ignore="true">📄 下載 ps4_06_tank_car.ino</a>

<!-- ino: examples/ps4/arduino/ps4_06_tank_car/ps4_06_tank_car.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 6（進階）：坦克遙控車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用手把開兩輪小車，開法跟坦克一樣：
 *     左搖桿上下 → 控制左輪（往上推前進、往下推後退）
 *     右搖桿上下 → 控制右輪
 *     兩支一起往上推 = 直走；一前一後 = 原地轉彎。
 *   搖桿放開時，也可以用方向鍵開（↑ 前進、↓ 後退、← → 原地轉）。
 *   讀不到接收器、或手把斷線，車子會立刻停下來。
 *
 * 你需要：
 *   掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、
 *   兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等驅動板）、電池。
 *
 * 接線（Arduino UNO / Nano）：
 *   接收器 VCC → 5V              接收器 GND → GND
 *   接收器 RX（= SDA）→ A4       接收器 TX（= SCL）→ A5
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   （ESP32 的接腳不一樣，請看下面「馬達接腳」）
 *
 * 第一次跑之前：
 *   1. 把車子架高（輪子懸空）再上傳，兩支搖桿一起往上推，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把下面的 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 *   2. ⚠️ 安全測試：車子還架高時，長按手把的 PS 鍵 10 秒讓手把關機，
 *      確認輪子會停下來，再放到地上開。
 *   3. 接收器只要 0.3 秒沒收到手把的資料，就會把按鍵、搖桿全部歸零，車子會停。
 *   4. 為了不讀錯，程式每 0.4 秒才問一次「手把還連著嗎？」，其他時間只讀手把資料；
 *      讀到「上下同時按」「左右同時按」這種不可能的資料，會丟掉、沿用上一筆。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x42;        // 掌控者接收器的門牌號碼

// ---- 馬達接腳 ----
#if defined(ESP32)
// ESP32 的第 6 ~ 11 腳接著板子上的記憶體晶片，不能拿來用，所以改用這幾腳
const int ENA = 25, IN1 = 26, IN2 = 27;  // 左輪
const int ENB = 14, IN3 = 32, IN4 = 33;  // 右輪
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // 左輪
const int ENB = 6, IN3 = 9, IN4 = 10;    // 右輪
#endif

// ---- 👉 可以改的參數 ----
const int  MAX_SPEED  = 200;             // 搖桿推到底的速度：0 ~ 255，新手先用慢一點
const int  DPAD_SPEED = 150;             // 用方向鍵開的速度
const bool LEFT_REVERSE  = false;        // 左輪轉反了改成 true
const bool RIGHT_REVERSE = false;        // 右輪轉反了改成 true

// 方向鍵在 16 位按鍵數字裡的位置（第幾位）
const uint8_t BTN_UP = 4, BTN_RIGHT = 5, BTN_DOWN = 6, BTN_LEFT = 7;

bool stopped = false;                    // 現在是不是因為斷線而停車
bool connected = false;                  // 手把有沒有連上（每 0.4 秒更新一次）
unsigned long lastStatus = 0;            // 上一次看「手把有沒有連上」的時間
unsigned long lastPrint = 0;             // 上一次印速度的時間

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}                      // 車子保持停止
  }
  Serial.println(F("準備好了！用兩支搖桿開車。"));
}

void loop() {
  // ---- 1. 每 0.4 秒看一次手把有沒有連線（第 0x31 號格子：狀態資料）----
  //   不要每一圈都讀：一直在兩個格子之間換來換去，接收器比較容易交錯資料，
  //   車子可能會自己動。開車只要一直讀第 0x11 號格子就好。
  if (millis() - lastStatus >= 400) {
    lastStatus = millis();
    uint8_t st[9];
    connected = mbcRead(0x31, st, 9) && (st[0] & 0x02);   // 第 0 個數字的「第 1 位」：1 = 手把已連線
  }
  if (!connected) {
    safeStop();                          // 讀不到接收器，或手把沒連線 → 立刻停車
    return;
  }

  // ---- 2. 讀手把（第 0x11 號格子）。readPad 在檔案下面，會順便檢查有沒有讀錯 ----
  uint8_t d[9];
  if (!readPad(d)) {
    safeStop();                          // 讀不到 → 停車
    return;
  }
  if (stopped) {
    stopped = false;
    Serial.println(F("手把連上了，可以開車！"));
  }

  uint16_t buttons = ((uint16_t)d[0] << 8) | d[1];   // 16 位的按鍵狀態
  int ly = (int8_t)d[3];                 // 左搖桿上下：上 = 正（-127 ~ +127）
  int ry = (int8_t)d[5];                 // 右搖桿上下：上 = 正

  // 搖桿推多少，輪子就轉多快：-127 ~ +127 換算成 -MAX_SPEED ~ +MAX_SPEED
  int left  = (long)ly * MAX_SPEED / 127;
  int right = (long)ry * MAX_SPEED / 127;

  // 兩支搖桿都放開時，改看方向鍵
  if (ly == 0 && ry == 0) {
    if      (buttons & (1u << BTN_UP))    { left =  DPAD_SPEED; right =  DPAD_SPEED; }  // 前進
    else if (buttons & (1u << BTN_DOWN))  { left = -DPAD_SPEED; right = -DPAD_SPEED; }  // 後退
    else if (buttons & (1u << BTN_LEFT))  { left = -DPAD_SPEED; right =  DPAD_SPEED; }  // 原地左轉
    else if (buttons & (1u << BTN_RIGHT)) { left =  DPAD_SPEED; right = -DPAD_SPEED; }  // 原地右轉
  }

  drive(left, right);

  if (millis() - lastPrint >= 300) {     // 每 0.3 秒印一次速度
    lastPrint = millis();
    Serial.print(F("左輪 "));
    Serial.print(left);
    Serial.print(F("　右輪 "));
    Serial.println(right);
  }

  delay(20);
}

// 停車，並且只在剛停的那一下說一次
void safeStop() {
  drive(0, 0);
  if (!stopped) {
    stopped = true;
    Serial.println(F("讀不到手把 → 停車！"));
  }
  delay(50);
}

// 控制兩個輪子：速度 -255（全速後退）~ 255（全速前進）
void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);   // 不讓速度超過範圍
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
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
```

**你會看到：**

```text
準備好了！用兩支搖桿開車。
左輪 0　右輪 0
左輪 200　右輪 200
左輪 -100　右輪 100
讀不到手把 → 停車！
手把連上了，可以開車！
```

> [!NOTE]
> 接收器只要 **0.3 秒**沒收到手把的資料，就會把按鍵、搖桿全部歸零，並且回報「沒連線」，車子會停下來。手把沒電、走太遠、被干擾時都一樣。
>
> 程式每 **0.4 秒**才問一次接收器「手把還連著嗎？」（第 `0x31` 格），其他時間只讀手把資料（第 `0x11` 格）；讀到「上下同時按」「左右同時按」這種不可能的資料，會丟掉、沿用上一筆。這樣車子比較不會因為讀錯而自己動，原因請看 [常見問題](#faq-updown)。
>
> 因為每 0.4 秒才看一次連線，「讀不到手把 → 停車！」最慢大約 0.7 秒後才印出來；但搖桿在 0.3 秒內就會歸零，車子會先停下來。

**怎麼調整，讓車子更好開：**

| 車子的樣子 | 怎麼改 |
| :--- | :--- |
| 太快、很難控制 | `MAX_SPEED` 調小（例如 200 → 150） |
| 搖桿推一點點車子不會動 | 馬達需要的力氣比較大，把 `MAX_SPEED` 調大，或換新電池 |
| 往前推卻原地打轉 | 有一個輪子轉反了，改 `LEFT_REVERSE` 或 `RIGHT_REVERSE` |
| 用方向鍵開太快 | `DPAD_SPEED` 調小 |

---

<h2 id="ex7">7️⃣ 讓手把震動</h2>

**做什麼：** 用 Arduino 叫手把震動：**慢慢壓 R2 扳機**，壓越深震越大力；**按住 ✕** 用最大力氣震；放開馬上停。以後可以拿來做「撞到牆就震一下」「時間到了震動提醒」。

<a href="/examples/ps4/arduino/ps4_07_rumble/ps4_07_rumble.ino" download data-ignore="true">📄 下載 ps4_07_rumble.ino</a>

<!-- ino: examples/ps4/arduino/ps4_07_rumble/ps4_07_rumble.ino -->
```cpp
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
```

**你會看到：**（慢慢壓 R2 到底，再放開）

```text
準備好了！慢慢壓 R2 扳機，或按住 ✕ 鍵。
震動力氣：36
震動力氣：140
震動力氣：255
停止震動
```

> [!NOTE]
> 把力氣（`0` ~ `255`）寫進第 `0x10` 號格子，手把會震 **0.2 秒**；寫 `0` 馬上停。想一直震，就要每 0.1 秒再寫一次。

> [!TIP]
> **試試看：** 在範例 6 的坦克車裡，加一行「`if (left > 180) mbcWrite(0x10, 100);`」，開太快時手把就會震一下提醒你。

---

<h2 id="ex8">8️⃣ 換燈條顏色</h2>

**做什麼：** 改變手把燈條的顏色：按 **✕ 藍色、● 紅色、■ 粉紅色、▲ 綠色**，按 **PS 鍵**隨機換一個顏色；**慢慢壓 L2** 燈會變暗，壓到底就熄掉。比賽時可以讓紅隊、藍隊的手把亮不同顏色，一眼就認得出來。

<a href="/examples/ps4/arduino/ps4_08_lightbar/ps4_08_lightbar.ino" download data-ignore="true">📄 下載 ps4_08_lightbar.ino</a>

<!-- ino: examples/ps4/arduino/ps4_08_lightbar/ps4_08_lightbar.ino -->
```cpp
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
```

**你會看到：**（按 ●、再按 ▲，最後按 PS 鍵）

```text
接收器現在記住的燈條顏色：紅 0　綠 0　藍 255
準備好了！按 ✕ ● ■ ▲ 或 PS 鍵換顏色，壓 L2 變暗。
換顏色：紅 255　綠 0　藍 0
換顏色：紅 0　綠 255　藍 0
換顏色：紅 37　綠 201　藍 148
```

> [!NOTE]
> 顏色用「紅、綠、藍」3 個數字（各 `0` ~ `255`）調出來，要用 `mbcWriteBytes` **一次寫進**第 `0x23` 號格子（分開寫不會生效）。手把還沒連上時寫進去也沒關係，接收器會先記住，連上後就套用。

---

## ❓ 常見問題

**Q：一直出現「找不到接收器」？**
1. 檢查 TX、RX 有沒有接反（UNO：RX → A4、TX → A5）。
2. 檢查 VCC、GND 有沒有插緊。接收器上的燈有沒有亮？
3. 確認是 **通用 I2C 版**：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
4. 接收器開機要 1 秒多，範例會等 5 秒。如果接收器是另外供電、比 Arduino 晚開機，按一下 Arduino 的 RESET 鍵重來。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：接收器找得到，手把卻一直「沒有連線」？**
1. 按手把的 **PS** 鍵開機。
2. 第一次用的手把要先 [配對](#pair)。
3. 手把沒電了：用 USB 線幫手把充電。
4. 接收器只認配對過的那一支手把。換手把要按接收器的 Reset 鍵重新配對。

**Q：讀到的數字亂跳，或按鍵跟搖桿對不上？**
檢查小幫手 `mbcRead` 裡的 `delay(2)` 有沒有被刪掉。PS4/PS5 接收器需要這 2 毫秒準備資料。

<a id="faq-updown"></a>

**Q：為什麼程式會忽略「上下同時按」？**
範例 2 以後的程式都有一段 `readPad`：讀到方向鍵「上和下同時按」或「左和右同時按」，就把這一筆資料丟掉，沿用上一筆。原因是：
1. 手把的十字方向鍵是一整塊，**不可能同時按到上和下**（左和右也一樣）。
2. PS4/PS5 接收器很忙的時候（例如藍牙正在跟手把講話），偶爾會來不及準備，把「上一次讀的別的格子」（例如第 `0x31` 格的狀態資料）交出來。這一筆的按鍵、搖桿數字都是亂的，遙控車可能會自己動。
3. 這種讀錯的資料很容易變成「上下同時按」，所以看到它就知道讀錯了。

另外，範例 3、6 每 **0.4 秒**才讀一次第 `0x31` 格（手把有沒有連上），其他時間只讀第 `0x11` 格，也是為了減少讀錯。自己寫程式時，也建議照這樣做：開車、控制東西只讀第 `0x11` 格，連線狀態隔久一點再看。

**Q：可以同時接 PS4/PS5 和 PS2 接收器嗎？**
不行。兩款的門牌號碼都是 `0x42`，同一組 SDA/SCL 上只能接一個。但是兩款的**格子編號完全一樣**，[PS2 的 Arduino 範例](/sensors/ps2/arduino-examples.md) 接這個接收器也能用（震動、燈條只有 PS4/PS5 有）。

**Q：想知道每一個格子放什麼資料？**
請看 [PS4/PS5 首頁的 I2C 暫存器表](/sensors/ps4/index.md) 和 [MBC 通用 I2C 協議](/i2c-protocol.md)（給會寫程式的人看的進階資料）。
