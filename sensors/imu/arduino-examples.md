<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 陀螺儀 IMU × Arduino 範例

[🔙 回到陀螺儀 IMU 首頁](/sensors/imu/index.md)　｜　[📡 MBC 通用 I2C 協議（進階）](/i2c-protocol.md)

這一頁有 **7 個範例**，從「確認接好了沒」一路做到「小車原地轉 90 度」。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/imu/arduino/imu_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 感應器 | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: |
| VCC | 5V | 3V3 或 5V（VIN） |
| GND | GND | GND |
| SDA | A4 | GPIO 21 |
| SCL | A5 | GPIO 22 |

> [!IMPORTANT]
> - 請用 **通用 I2C 版** 的陀螺儀。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 陀螺儀**開機時要先靜靜不動約 0.5 秒**才會準備好。每個範例一開始都會印「請不要動它」，看到「準備好了」再開始玩。
> - 接線前先拔掉 USB 線。

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟感應器打招呼](#ex1) | 確認接線正確、看哪一面朝上 | 無 |
| 2 | [我轉了幾度？](#ex2) | 讀「航向角」、歸零 | 無 |
| 3 | [水平儀](#ex3) | 用加速度判斷有沒有放平 | 一本書 |
| 4 | [轉 90 度提醒](#ex4) | 用航向角數「轉了幾個 90 度」 | 無 |
| 5 | [搖一搖偵測](#ex5) | 讀「轉動速度」（角速度） | 無 |
| 6 | [電子骰子](#ex6) | 讀「哪一面朝上」和「停穩了沒」 | 無 |
| 7 | [小車原地轉 90 度（進階）](#ex7) | 用陀螺儀讓車子轉得準 | 兩輪小車、L298N 馬達驅動板 |

> [!TIP]
> **建議順序：1 → 2 → 4 → 3 → 6 → 5 → 7。** 先確認接好（1），再弄懂「航向角」（2、4），想做車子的話，一定要先做過範例 2。

---

<h2 id="ex1">1️⃣ 跟感應器打招呼</h2>

**做什麼：** 確認感應器接好了，順便看看「現在哪一面朝上」。每次接新的感應器，都先跑這一個。

**步驟：**

1. 照上面的表格接好 4 條線，USB 插上電腦。
2. 把感應器**平放在桌上**，不要拿在手上。
3. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
4. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/imu/arduino/imu_01_hello/imu_01_hello.ino" download data-ignore="true">📄 下載 imu_01_hello.ino</a>

<!-- ino: examples/imu/arduino/imu_01_hello/imu_01_hello.ino -->
```cpp
/*
 * 陀螺儀 IMU × Arduino　範例 1：跟感應器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認感應器有沒有接好。接好了，會在電腦上印出感應器的名字、版本，
 *   還有「現在哪一面朝上」。每次接新感應器，都建議先跑這個範例。
 *
 * 你需要：
 *   Arduino UNO / Nano（或 ESP32）、陀螺儀 IMU（通用 I2C 版）、4 條杜邦線。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   陀螺儀開機時要先「靜靜不動」大約 0.5 秒，才會說「我準備好了」。
 *   所以請把感應器平放在桌上，等看到結果之前都不要碰它。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟感應器講話

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的「門牌號碼」，固定是 0x6A

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等感應器開機 0.5 秒

  Serial.println(F("=== 陀螺儀 IMU：跟感應器打招呼 ==="));
  Serial.println(F("請把感應器放在桌上，不要動它……"));

  // 等感應器說「我準備好了」，最多等 5 秒（它要先靜止約 0.5 秒）
  if (!mbcWaitReady(5000)) {
    uint8_t id;
    if (mbcRead(0x02, &id, 1) && id == 0x6A) {
      // 找得到感應器，只是它一直覺得自己在動
      Serial.println(F("找到感應器了，但它一直覺得自己在晃動，沒辦法準備好。請檢查："));
      Serial.println(F("  1. 感應器有沒有放在桌上？用手拿著會一直有一點點晃動。"));
      Serial.println(F("  2. 桌子有沒有在震動？（旁邊有馬達、風扇在轉也會）"));
      Serial.println(F("  3. 放好以後，按一下 Arduino 的 RESET 鍵再試一次。"));
    } else {
      Serial.println(F("找不到感應器！請檢查："));
      Serial.println(F("  1. SDA 接 A4、SCL 接 A5，有沒有接反？"));
      Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？感應器上的紅燈有沒有在閃？"));
      Serial.println(F("  3. 你的感應器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
      Serial.println(F("  4. 還是不行：拔掉感應器的電源再插回去，等 3 秒後按 Arduino 的 RESET 鍵。"));
    }
    while (true) {}                      // 停在這裡，不往下做了
  }

  // ---- 1. 讀「版本資料」：感應器的第 0x01 號格子，共 17 個數字 ----
  uint8_t info[17];
  mbcRead(0x01, info, 17);

  // 第 9 ~ 16 個數字是感應器的名字（英文字母）
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("找到了！感應器名字："));
  Serial.println(name);                  // 應該會印出 IMU

  Serial.print(F("韌體版本："));
  Serial.print(info[2]);                 // 主版本
  Serial.print('.');
  Serial.println(info[3]);               // 次版本

  Serial.print(F("韌體日期：20"));
  Serial.print(info[4]);                 // 年（西元末兩位）
  Serial.print('/');
  Serial.print(info[5]);                 // 月
  Serial.print('/');
  Serial.println(info[6]);               // 日

  Serial.print(F("磁力計："));
  if (info[8] == 1) Serial.println(F("正常"));
  else              Serial.println(F("沒有偵測到"));

  // ---- 2. 讀「狀態」：第 0x31 號格子，共 9 個數字 ----
  uint8_t st[9];
  mbcRead(0x31, st, 9);
  uint8_t flags = st[0];                 // 一排「開關」，每一個位元代表一件事
  uint8_t upFace = st[4];                // 現在哪一面朝上：0 ~ 5

  Serial.print(F("現在朝上的面："));
  printFace(upFace);
  Serial.println();

  if (flags & (1u << 3)) {               // 第 3 個開關 = 安裝過歪
    Serial.println(F("注意：感應器裝得太歪了（超過約 45 度），航向可能不準。請裝正一點。"));
  }

  Serial.println(F("接線正確，可以開始玩其他範例了！"));
}

void loop() {
  // 這個範例只要在開機時檢查一次，所以 loop 裡什麼都不做
}

// 把「朝上的面」編號印成看得懂的字
void printFace(uint8_t f) {
  switch (f) {
    case 0:  Serial.print(F("+X 面")); break;
    case 1:  Serial.print(F("+Y 面")); break;
    case 2:  Serial.print(F("+Z 面")); break;
    case 3:  Serial.print(F("-X 面")); break;
    case 4:  Serial.print(F("-Y 面")); break;
    case 5:  Serial.print(F("-Z 面")); break;
    default: Serial.print(F("不知道")); break;
  }
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從感應器的第 reg 號格子，讀 n 個數字放進 buf。讀到了回傳 true
bool mbcRead(uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(SENSOR_ADDR, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// 把數字 value 寫進感應器的第 reg 號格子。成功回傳 true
bool mbcWrite(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好，最多等 timeoutMs 毫秒。準備好了回傳 true
// （陀螺儀要先靜止約 0.5 秒才會準備好，所以範例都等 5 秒）
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
=== 陀螺儀 IMU：跟感應器打招呼 ===
請把感應器放在桌上，不要動它……
找到了！感應器名字：IMU
韌體版本：1.0
韌體日期：2026/10/12
磁力計：正常
現在朝上的面：+Z 面
接線正確，可以開始玩其他範例了！
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟感應器「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(格子編號, 放哪裡, 幾個)`：從感應器讀資料
> - `mbcWrite(格子編號, 數字)`：把命令寫進感應器
>
> 感應器就像一排有編號的「格子」，每一格放著不同的資料，例如第 `0x11` 格放著「轉了幾度」。

---

<h2 id="ex2">2️⃣ 我轉了幾度？</h2>

**做什麼：** 把感應器放在桌上慢慢轉，電腦會一直告訴你「現在轉到幾度」。在監控窗上方的輸入框打 **`z`** 按 Enter，就會把現在的方向當成 0 度。

<a href="/examples/imu/arduino/imu_02_heading/imu_02_heading.ino" download data-ignore="true">📄 下載 imu_02_heading.ino</a>

<!-- ino: examples/imu/arduino/imu_02_heading/imu_02_heading.ino -->
```cpp
/*
 * 陀螺儀 IMU × Arduino　範例 2：我轉了幾度？（航向角）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器放在桌上慢慢轉，電腦會一直告訴你「現在轉到幾度」。
 *     從上往下看「順時針」（向右轉）→ 數字變大
 *     從上往下看「逆時針」（向左轉）→ 數字變小（可以變成負數）
 *   數字會一直累加：右轉兩圈就是 720 度，不會跳回 0。
 *   另外也印出「0 ~ 359 度」的寫法，像時鐘一樣轉一圈又回到 0。
 *   在序列埠監控窗輸入 z 按 Enter，就會把角度歸零（現在的方向 = 0 度）。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   等看到「準備好了」再開始轉。
 *
 * 小提醒：
 *   陀螺儀開機時會把「當時的方向」當成 0 度。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  Serial.println(F("請把感應器放在桌上，不要動它……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}
  }
  Serial.println(F("準備好了！慢慢轉動感應器看看。輸入 z 按 Enter 可以歸零。"));
}

void loop() {
  // ---- 1. 收到 z 就歸零 ----
  if (Serial.available()) {
    char c = Serial.read();              // 讀一個你打的字
    if (c == 'z' || c == 'Z') {
      mbcWrite(0x10, 0x01);              // 把 0x01 寫進第 0x10 號格子 = 「航向歸零」
      delay(50);                         // 歸零後要等一下（至少 30 毫秒）再讀，才會讀到新的 0
      Serial.println(F("--- 已歸零！現在的方向就是 0 度 ---"));
    }
  }

  // ---- 2. 讀航向：第 0x11 號格子，共 6 個數字（3 種航向，每種 2 個數字）----
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {            // 讀不到就等一下再試
    delay(100);
    return;
  }

  // 第 2、3 個數字是「立體補償航向（Gyro3D）」，最推薦用這個
  // 兩個數字拼成一個「可以有負號」的數：前面的是高位、後面的是低位
  int16_t heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);

  // 換成 0 ~ 359 度（像時鐘一樣，轉一圈回到 0）
  int angle360 = ((heading % 360) + 360) % 360;

  Serial.print(F("累加角度："));
  Serial.print(heading);
  Serial.print(F(" 度　｜　0~359 寫法："));
  Serial.print(angle360);
  Serial.println(F(" 度"));

  delay(200);                            // 👉 可以改：每 0.2 秒印一次
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
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
```

**你會看到：**

```text
準備好了！慢慢轉動感應器看看。輸入 z 按 Enter 可以歸零。
累加角度：0 度　｜　0~359 寫法：0 度
累加角度：45 度　｜　0~359 寫法：45 度
累加角度：370 度　｜　0~359 寫法：10 度
累加角度：-30 度　｜　0~359 寫法：330 度
--- 已歸零！現在的方向就是 0 度 ---
累加角度：0 度　｜　0~359 寫法：0 度
```

**角度的數字是什麼意思？**

| 你怎麼轉（從上往下看） | 累加角度 |
| :--- | :--- |
| 順時針（向右轉） | 變大，例如 0 → 90 |
| 逆時針（向左轉） | 變小，可以變成負數，例如 0 → -90 |
| 往右轉兩圈 | 720（**一直累加，不會跳回 0**） |

> [!TIP]
> **兩種寫法哪個好？** 機器人要「轉 90 度」時，用**累加角度**最方便（目標 = 現在 + 90）。想知道「車頭朝哪個方向」時，用 **0~359 寫法**，像時鐘一樣。程式裡換算的公式是 `((角度 % 360) + 360) % 360`。

---

<h2 id="ex3">3️⃣ 水平儀</h2>

**做什麼：** 把感應器變成水平儀：放平了，Arduino 板子上的內建小燈（標著 **L** 那顆）就亮；歪了，電腦告訴你往哪一邊歪、歪了幾度。原理是地心引力一直往下拉，感應器裡的「加速度計」量得到這股力量往哪個方向。

**準備：** 一本書。把感應器放在書上，慢慢把書的一邊抬高。

<a href="/examples/imu/arduino/imu_03_level/imu_03_level.ino" download data-ignore="true">📄 下載 imu_03_level.ino</a>

<!-- ino: examples/imu/arduino/imu_03_level/imu_03_level.ino -->
```cpp
/*
 * 陀螺儀 IMU × Arduino　範例 3：水平儀
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器變成「水平儀」：
 *     感應器放平了 → Arduino 板子上的內建小燈（標著 L 的那顆）亮起來
 *     感應器歪了   → 燈熄，電腦告訴你往哪一邊歪、歪了幾度
 *   原理：地心引力一直往下拉，感應器裡的「加速度計」量得到這股力量
 *   往哪個方向，就知道自己有沒有放平。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把感應器放在書本上，慢慢把書的一邊抬高，看數字怎麼變。
 *
 * 小提醒：
 *   感應器平放、直立、側放都可以用，程式會自己找出哪一面朝上。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

const float LEVEL_DEG = 2.0;             // 👉 可以改：歪不到 2 度就算「水平」

const char AXIS_NAME[3] = {'X', 'Y', 'Z'};   // 三個方向的名字

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // 把燈的腳設成「輸出」，才能控制它亮或熄
  mbcBegin();
  delay(500);

  Serial.println(F("請把感應器放在桌上，不要動它……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把感應器放在書上，抬高書的一邊看看。"));
}

void loop() {
  // ---- 1. 讀加速度：第 0x23 號格子，共 6 個數字（X、Y、Z 各 2 個）----
  uint8_t d[6];
  if (!mbcRead(0x23, d, 6)) {            // 讀不到就等一下再試
    delay(100);
    return;
  }

  // 拼成 3 個數字，單位是 mg（1000 mg = 1 倍地心引力）
  // 放平靜止時，「朝上」的那個方向會讀到大約 +1000
  int16_t a[3];
  for (uint8_t i = 0; i < 3; i++) {
    a[i] = (int16_t)(((uint16_t)d[2 * i] << 8) | d[2 * i + 1]);
  }

  // ---- 2. 找出哪個方向朝上（數字的「大小」最大的那個）----
  uint8_t up = 0;
  for (uint8_t i = 1; i < 3; i++) {
    if (abs(a[i]) > abs(a[up])) up = i;
  }

  // ---- 3. 看另外兩個方向歪了幾度 ----
  bool isLevel = true;                   // 先假設是平的
  for (uint8_t i = 0; i < 3; i++) {
    if (i == up) continue;               // 朝上的方向不用看

    // 用「三角函數」把力量換成角度（atan2 是 Arduino 內建的）
    float deg = atan2((float)a[i], (float)abs(a[up])) * 180.0 / PI;

    Serial.print(AXIS_NAME[i]);
    Serial.print(F(" 方向："));
    if (fabs(deg) <= LEVEL_DEG) {
      Serial.print(F("水平　　　　"));
    } else {
      isLevel = false;
      // 數字是負的 → 「+」那一邊比較低；數字是正的 → 「-」那一邊比較低
      Serial.print(F("往 "));
      Serial.print(deg < 0 ? '+' : '-');
      Serial.print(AXIS_NAME[i]);
      Serial.print(F(" 那邊低 "));
      Serial.print(fabs(deg), 1);        // 印到小數點後 1 位
      Serial.print(F(" 度　"));
    }
  }

  // ---- 4. 平的就亮燈 ----
  if (isLevel) {
    digitalWrite(LED, HIGH);
    Serial.println(F("→ 水平！"));
  } else {
    digitalWrite(LED, LOW);
    Serial.println(F("→ 還沒平"));
  }

  delay(200);                            // 👉 可以改：每 0.2 秒看一次
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
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
```

**你會看到：**

```text
X 方向：水平　　　　Y 方向：水平　　　　→ 水平！
X 方向：往 +X 那邊低 5.3 度　Y 方向：水平　　　　→ 還沒平
X 方向：往 +X 那邊低 12.8 度　Y 方向：往 -Y 那邊低 3.1 度　→ 還沒平
```

> [!TIP]
> **試試看：找出感應器的 X 和 Y。** 把感應器的某一邊往下壓，看監控窗寫「往 +X 那邊低」還是「往 -Y 那邊低」，用筆在感應器旁邊標起來。之後把感應器裝上車子時，你就知道哪個方向是車頭了。

---

<h2 id="ex4">4️⃣ 轉 90 度提醒</h2>

**做什麼：** 把感應器放在桌上轉，每**多轉滿 90 度**（1/4 圈），內建小燈就閃一下，電腦告訴你現在轉了幾個 1/4 圈。機器人比賽常常要「轉 90 度」，先用手感受 90 度有多大。

<a href="/examples/imu/arduino/imu_04_quarter_turn/imu_04_quarter_turn.ino" download data-ignore="true">📄 下載 imu_04_quarter_turn.ino</a>

<!-- ino: examples/imu/arduino/imu_04_quarter_turn/imu_04_quarter_turn.ino -->
```cpp
/*
 * 陀螺儀 IMU × Arduino　範例 4：轉 90 度提醒
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器放在桌上轉，每多轉滿 90 度（1/4 圈），
 *   Arduino 板子上的內建小燈（標著 L 的那顆）就閃一下，
 *   電腦也會告訴你「往右轉滿 90 度！現在是 1/4 圈」。
 *   機器人比賽常常要「轉 90 度」，這個範例讓你先用手感受 90 度有多大。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把感應器平放在桌上，像轉方向盤一樣慢慢轉。
 *   輸入 z 按 Enter 可以重新從 0 開始。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

const int STEP_DEG = 90;                 // 👉 可以改：每轉幾度提醒一次（例如 45、180）

long quarter = 0;                        // 目前轉到第幾個 90 度（往右是正、往左是負）

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  Serial.println(F("請把感應器放在桌上，不要動它……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}
  }
  zeroHeading();                         // 從 0 度開始算
  Serial.println(F("準備好了！慢慢轉動感應器，每轉滿 90 度燈會閃一下。"));
}

void loop() {
  // ---- 1. 收到 z 就重新從 0 開始 ----
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'z' || c == 'Z') {
      zeroHeading();
      Serial.println(F("--- 已歸零，重新從 0 開始算 ---"));
    }
  }

  // ---- 2. 讀航向（第 0x11 號格子，6 個數字），用第 2、3 個數字（Gyro3D）----
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {
    delay(100);
    return;
  }
  long heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);   // 右轉變大、左轉變小

  // ---- 3. 有沒有「又多轉滿」一個 90 度？ ----
  // 要從上一個 90 度的位置「再轉滿 90 度」才算，在線的附近晃來晃去不會一直閃
  if (heading >= (quarter + 1) * STEP_DEG) {
    quarter = quarter + 1;
    Serial.print(F("往右轉滿 "));
    Serial.print(STEP_DEG);
    Serial.print(F(" 度！"));
    report();
  } else if (heading <= (quarter - 1) * STEP_DEG) {
    quarter = quarter - 1;
    Serial.print(F("往左轉滿 "));
    Serial.print(STEP_DEG);
    Serial.print(F(" 度！"));
    report();
  }

  delay(20);
}

// 印出現在是第幾個 1/4 圈，並閃一下燈
void report() {
  Serial.print(F("現在是 "));
  Serial.print(quarter);
  Serial.print(F("/4 圈（"));
  Serial.print(quarter * STEP_DEG);
  Serial.print(F(" 度）"));
  if (quarter != 0 && quarter % 4 == 0) {
    Serial.print(F("　剛好轉了一整圈！"));
  }
  Serial.println();

  digitalWrite(LED, HIGH);               // 閃一下
  delay(150);
  digitalWrite(LED, LOW);
}

// 把航向歸零
void zeroHeading() {
  mbcWrite(0x10, 0x01);                  // 把 0x01 寫進第 0x10 號格子 = 「航向歸零」
  delay(50);                             // 歸零後要等一下（至少 30 毫秒）再讀
  quarter = 0;
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
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
```

**你會看到：**

```text
準備好了！慢慢轉動感應器，每轉滿 90 度燈會閃一下。
往右轉滿 90 度！現在是 1/4 圈（90 度）
往右轉滿 90 度！現在是 2/4 圈（180 度）
往右轉滿 90 度！現在是 3/4 圈（270 度）
往右轉滿 90 度！現在是 4/4 圈（360 度）　剛好轉了一整圈！
往左轉滿 90 度！現在是 3/4 圈（270 度）
```

> [!TIP]
> **試試看：** 把 `STEP_DEG` 改成 `45`，就變成「每轉 45 度提醒一次」。

---

<h2 id="ex5">5️⃣ 搖一搖偵測</h2>

**做什麼：** 拿起感應器用力搖一搖（或快速扭一下），內建小燈就會亮，電腦會數「你搖了幾下」，還會告訴你剛剛轉得多快。陀螺儀量得到**轉動的速度**，轉得越快數字越大，超過門檻就算搖了一下。

**準備：** 杜邦線要插緊，搖的時候才不會鬆脫。

<a href="/examples/imu/arduino/imu_05_shake/imu_05_shake.ino" download data-ignore="true">📄 下載 imu_05_shake.ino</a>

<!-- ino: examples/imu/arduino/imu_05_shake/imu_05_shake.ino -->
```cpp
/*
 * 陀螺儀 IMU × Arduino　範例 5：搖一搖偵測
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   拿起感應器用力搖一搖（或快速扭一下），
 *   Arduino 板子上的內建小燈（標著 L 的那顆）就會亮，
 *   電腦會數「你搖了幾下」，還會告訴你剛剛轉得多快。
 *   原理：陀螺儀量得到「轉動的速度」，轉得越快數字越大，
 *   超過一個門檻，就當作「搖了一下」。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *   杜邦線要插緊，搖的時候才不會鬆脫。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   等看到「準備好了」，再拿起感應器搖一搖。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

const float SHAKE_SPEED = 300.0;         // 👉 可以改：轉動速度超過「每秒 300 度」就算搖一下
                                         //    （改小比較容易觸發，改大要搖更用力）

bool shaking = false;                    // 現在是不是正在「搖的那一下」
int  shakeCount = 0;                     // 總共搖了幾下
float peak = 0;                          // 這一下最快轉到多快

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  Serial.println(F("請把感應器放在桌上，不要動它……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿起感應器搖一搖。"));
}

void loop() {
  // ---- 1. 讀角速度：第 0x22 號格子，共 6 個數字（X、Y、Z 各 2 個）----
  uint8_t d[6];
  if (!mbcRead(0x22, d, 6)) {            // 讀不到就等一下再試
    delay(100);
    return;
  }

  // 拼成 3 個數字，單位是「0.1 度/秒」，所以要除以 10 才是「度/秒」
  float g[3];
  for (uint8_t i = 0; i < 3; i++) {
    int16_t raw = (int16_t)(((uint16_t)d[2 * i] << 8) | d[2 * i + 1]);
    g[i] = raw / 10.0;
  }

  // 三個方向合起來的「轉動速度」（不管往哪個方向轉，都是正的）
  float speed = sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

  // ---- 2. 判斷是不是「搖了一下」----
  if (!shaking && speed > SHAKE_SPEED) {
    shaking = true;                      // 開始搖了
    peak = speed;
    digitalWrite(LED, HIGH);             // 亮燈
  }
  if (shaking) {
    if (speed > peak) peak = speed;      // 記住這一下最快有多快
    if (speed < SHAKE_SPEED / 2) {       // 慢下來了（低於門檻的一半）→ 這一下結束
      shaking = false;
      shakeCount = shakeCount + 1;
      digitalWrite(LED, LOW);
      Serial.print(F("搖一搖！第 "));
      Serial.print(shakeCount);
      Serial.print(F(" 下（最快每秒轉 "));
      Serial.print((int)peak);
      Serial.println(F(" 度）"));
    }
  }

  delay(10);                             // 看得很頻繁，才不會漏掉很快的一下
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
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
```

**你會看到：**

```text
準備好了！拿起感應器搖一搖。
搖一搖！第 1 下（最快每秒轉 512 度）
搖一搖！第 2 下（最快每秒轉 634 度）
搖一搖！第 3 下（最快每秒轉 401 度）
```

> [!TIP]
> **試試看：** 把 `SHAKE_SPEED` 改成 `150`，輕輕扭一下就會觸發；改成 `600`，要很用力才行。

---

<h2 id="ex6">6️⃣ 電子骰子</h2>

**做什麼：** 把感應器當成一顆骰子：6 個面（+X、+Y、+Z、-X、-Y、-Z）各代表 1 ~ 6 點。翻個面放下來，等它停穩，電腦就會說「擲出 3 點！」，內建小燈也會閃 3 下。

<a href="/examples/imu/arduino/imu_06_dice/imu_06_dice.ino" download data-ignore="true">📄 下載 imu_06_dice.ino</a>

<!-- ino: examples/imu/arduino/imu_06_dice/imu_06_dice.ino -->
```cpp
/*
 * 陀螺儀 IMU × Arduino　範例 6：電子骰子（哪一面朝上？）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器當成一顆骰子：感應器有 6 個面（+X、+Y、+Z、-X、-Y、-Z），
 *   每一面代表 1 ~ 6 點。把感應器翻一翻、放下來，
 *   等它停穩，電腦就會說「擲出 3 點！」，
 *   Arduino 板子上的內建小燈（標著 L 的那顆）也會閃 3 下。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   拿起感應器翻個面，放回桌上不要碰，大約 0.5 秒後就會看到點數。
 *   （杜邦線不要扯太緊，翻面時才不會鬆脫）
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

// 每一面代表幾點（跟真的骰子一樣，相對的兩面加起來 = 7）
//                         +X  +Y  +Z  -X  -Y  -Z
const uint8_t DOTS[6] = {  2,  3,  1,  5,  4,  6 };   // 👉 可以改：換成你喜歡的點數

bool rolling = true;                     // 骰子是不是還在動

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  Serial.println(F("請把感應器放在桌上，不要動它……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把感應器翻個面放下來，等它停穩。"));
}

void loop() {
  // ---- 1. 讀狀態：第 0x31 號格子，共 9 個數字 ----
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {           // 讀不到就等一下再試
    delay(100);
    return;
  }
  uint8_t flags  = st[0];                // 一排「開關」
  uint8_t upFace = st[4];                // 哪一面朝上：0 ~ 5
  bool still = flags & (1u << 1);        // 第 1 個開關 = 靜止（停穩了）

  // ---- 2. 動起來了 → 骰子在滾 ----
  if (!still) {
    if (!rolling) {                      // 剛開始動，只說一次就好
      rolling = true;
      Serial.println(F("骰子在滾……"));
    }
  }

  // ---- 3. 停穩了 → 公布點數 ----
  if (still && rolling && upFace < 6) {
    rolling = false;
    uint8_t n = DOTS[upFace];
    Serial.print(F("停下來了！擲出 "));
    Serial.print(n);
    Serial.print(F(" 點（朝上的面："));
    printFace(upFace);
    Serial.println(F("）"));

    for (uint8_t i = 0; i < n; i++) {    // 幾點就閃幾下
      digitalWrite(LED, HIGH); delay(200);
      digitalWrite(LED, LOW);  delay(200);
    }
  }

  delay(50);
}

// 把「朝上的面」編號印成看得懂的字
void printFace(uint8_t f) {
  switch (f) {
    case 0:  Serial.print(F("+X")); break;
    case 1:  Serial.print(F("+Y")); break;
    case 2:  Serial.print(F("+Z")); break;
    case 3:  Serial.print(F("-X")); break;
    case 4:  Serial.print(F("-Y")); break;
    case 5:  Serial.print(F("-Z")); break;
    default: Serial.print(F("?"));  break;
  }
}

// ============================================================
// 以下是「通訊小幫手」：負責跟感應器講話。
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
```

**你會看到：**

```text
準備好了！把感應器翻個面放下來，等它停穩。
停下來了！擲出 1 點（朝上的面：+Z）
骰子在滾……
停下來了！擲出 4 點（朝上的面：-Y）
```

| 朝上的面 | +X | +Y | +Z | -X | -Y | -Z |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 點數 | 2 | 3 | 1 | 5 | 4 | 6 |

> [!TIP]
> 感應器要**停穩約 0.5 秒**才會公布點數，跟真的骰子一樣，還在滾的時候不算。

---

<h2 id="ex7">7️⃣ 小車原地轉 90 度（進階）</h2>

**做什麼：** 讓兩輪小車原地右轉 90 度、停 1 秒，再轉 90 度……轉滿 4 次（一整圈）就停。車子一邊轉一邊看陀螺儀：離目標很遠就轉快一點，快到了就慢下來，到了就停車。只靠「馬達轉幾秒」來轉彎，電池沒電、地板太滑就會不準；用陀螺儀就準多了。

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

1. 先跑 [範例 2](#ex2)，確認右轉時角度會變大。
2. 陀螺儀用螺絲或積木**固定**在車上（平放、直立、側放都可以，但不能晃）。
3. 把車子架高（輪子懸空）再上傳，確認兩個輪子都是**往前**轉。哪個輪子轉反了，就把 `LEFT_REVERSE` 或 `RIGHT_REVERSE` 改成 `true`。
4. 放到地上**不要碰**，等「準備好了」，3 秒後開始轉。

<a href="/examples/imu/arduino/imu_07_turn90_car/imu_07_turn90_car.ino" download data-ignore="true">📄 下載 imu_07_turn90_car.ino</a>

<!-- ino: examples/imu/arduino/imu_07_turn90_car/imu_07_turn90_car.ino -->
```cpp
/*
 * 陀螺儀 IMU × Arduino　範例 7（進階）：小車原地轉 90 度
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車「原地右轉 90 度」，停 1 秒，再轉 90 度……轉滿 4 次（一整圈）就停。
 *   車子一邊轉，一邊看陀螺儀的角度：
 *     離目標還很遠 → 轉快一點
 *     快到了       → 慢慢轉
 *     到了         → 停車
 *   這種「一邊看感應器、一邊修正」的方法叫做「回授控制」。
 *   只靠「馬達轉幾秒」來轉彎，電池沒電、地板太滑就會不準；用陀螺儀就準多了。
 *
 * 你需要：
 *   兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等驅動板）、電池。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   （ESP32 改接：ENA → 25、IN1 → 26、IN2 → 27、ENB → 14、IN3 → 32、IN4 → 33）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   陀螺儀用螺絲或積木固定在車上（平放、直立、側放都可以，但不能晃）。
 *
 * 第一次跑之前：
 *   1. 先跑「範例 2」，確認右轉時角度數字會變大。
 *   2. 把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把下面的 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 *   3. 開機時車子要放著不動，等「準備好了」才會出發。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x6A;        // 陀螺儀的門牌號碼

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
const int   TURN_ANGLE = 90;             // 每次轉幾度：正數 = 右轉，負數 = 左轉
const int   TURN_TIMES = 4;              // 總共轉幾次（4 次 × 90 度 = 一整圈）
const int   MAX_SPEED  = 150;            // 離目標很遠時的速度：0 ~ 255
const int   MIN_SPEED  = 70;             // 快到時的最慢速度（太小輪子會轉不動）
const float KP         = 3.0;            // 差 1 度要加多少速度：停不準就調這個
const int   TOLERANCE  = 2;              // 差 2 度以內就算「到了」
const bool  LEFT_REVERSE  = false;       // 左輪轉反了改成 true
const bool  RIGHT_REVERSE = false;       // 右輪轉反了改成 true

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);

  Serial.println(F("請不要動車子，陀螺儀準備中……"));
  if (!mbcWaitReady(5000)) {
    Serial.println(F("找不到感應器，或感應器一直在晃動。請先跑範例 1 檢查。"));
    while (true) {}                      // 馬達已經停好，就停在這裡
  }
  mbcWrite(0x10, 0x01);                  // 航向歸零：現在車頭的方向 = 0 度
  delay(50);                             // 歸零後等一下再讀

  Serial.println(F("準備好了！3 秒後開始轉。"));
  delay(3000);

  long target = 0;                       // 目標角度
  for (int i = 1; i <= TURN_TIMES; i++) {
    target = target + TURN_ANGLE;        // 每次多轉 90 度：90、180、270、360
    Serial.print(F("第 "));
    Serial.print(i);
    Serial.print(F(" 次：轉到 "));
    Serial.print(target);
    Serial.print(F(" 度…… "));

    if (!turnTo(target)) {               // 轉失敗了（為了安全已經停車）
      while (true) {}
    }
    delay(1000);                         // 停 1 秒再轉下一次
  }
  Serial.println(F("完成！按 Arduino 的 RESET 鍵可以再跑一次。"));
}

void loop() {
  // 全部都在 setup 做完了，這裡什麼都不做（車子保持停止）
}

// 原地轉到 target 度。成功回傳 true；轉不到回傳 false（已停車）
bool turnTo(long target) {
  unsigned long start = millis();
  while (true) {
    // ---- 1. 讀航向（第 0x11 號格子，用第 2、3 個數字 = Gyro3D）----
    uint8_t d[6];
    if (!mbcRead(0x11, d, 6)) {          // 讀不到感應器 → 為了安全，先停車
      drive(0, 0);
      if (millis() - start > 5000) {
        Serial.println(F("讀不到陀螺儀，停車！請檢查接線。"));
        return false;
      }
      delay(20);
      continue;
    }
    long heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);   // 右轉變大

    // ---- 2. 還差幾度？ ----
    long error = target - heading;       // 正數 = 還要往右轉，負數 = 轉過頭了

    if (abs(error) <= TOLERANCE) {       // 到了！
      drive(0, 0);
      delay(300);                        // 等車子完全停下來
      mbcRead(0x11, d, 6);               // 再看一次停在幾度
      heading = (int16_t)(((uint16_t)d[2] << 8) | d[3]);
      Serial.print(F("停在 "));
      Serial.print(heading);
      Serial.println(F(" 度"));
      return true;
    }

    // ---- 3. 安全檢查 ----
    if (abs(error) > abs(TURN_ANGLE) + 45) {   // 越轉越遠 → 方向反了
      drive(0, 0);
      Serial.println(F("越轉越遠，停車！可能是輪子轉反了：改 LEFT_REVERSE / RIGHT_REVERSE。"));
      return false;
    }
    if (millis() - start > 5000) {       // 轉了 5 秒還沒到
      drive(0, 0);
      Serial.println(F("5 秒內轉不到，停車！檢查電池，或把 MIN_SPEED 調大。"));
      return false;
    }

    // ---- 4. 差越多轉越快，快到了就慢下來 ----
    int speed = constrain((int)(KP * abs(error)), MIN_SPEED, MAX_SPEED);
    if (error > 0) drive(speed, -speed);    // 左輪前進、右輪後退 → 原地右轉（角度變大）
    else           drive(-speed, speed);    // 轉過頭了 → 原地左轉回來
    delay(10);
  }
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
// 以下是「通訊小幫手」：負責跟感應器講話。
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
```

**你會看到：**

```text
請不要動車子，陀螺儀準備中……
準備好了！3 秒後開始轉。
第 1 次：轉到 90 度…… 停在 91 度
第 2 次：轉到 180 度…… 停在 180 度
第 3 次：轉到 270 度…… 停在 272 度
第 4 次：轉到 360 度…… 停在 361 度
完成！按 Arduino 的 RESET 鍵可以再跑一次。
```

**怎麼調整，讓車子轉得更準：**

| 車子的樣子 | 怎麼改 |
| :--- | :--- |
| 每次都轉過頭好幾度 | `MAX_SPEED` 調小，或 `KP` 減小（例如 3.0 → 2.0） |
| 快到的時候停住不動，最後說「5 秒內轉不到」 | `MIN_SPEED` 調大（例如 70 → 90） |
| 一開始就說「越轉越遠，停車！」 | 有一個輪子轉反了，改 `LEFT_REVERSE` 或 `RIGHT_REVERSE` |
| 想改成左轉 | `TURN_ANGLE` 改成 `-90` |

---

## ❓ 常見問題

**Q：一直出現「找不到感應器」？**

1. 檢查 SDA、SCL 有沒有接反（UNO：SDA → A4、SCL → A5）。
2. 檢查 VCC、GND 有沒有插緊。感應器上的紅燈應該每秒閃一下，表示它有在工作。
3. 確認是 **通用 I2C 版**：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。

**Q：出現「感應器一直在晃動」，一直等不到「準備好了」？**

陀螺儀開機時要先**完全不動約 0.5 秒**，量一下自己靜止時的讀數，之後才算得準。請把感應器放在桌上（不要拿在手上），桌上不要有馬達、風扇在震動，再按 Arduino 的 RESET 鍵。感應器上的綠燈：**慢慢閃 = 還沒準備好、熄滅 = 準備好了、很快地閃 = 裝得太歪**（超過約 45 度）。

**Q：角度為什麼不是 -180 ~ 180，也不是 0 ~ 359？**

這個陀螺儀的角度會**一直累加**（右轉兩圈 = 720），跟樂高 EV3 陀螺儀一樣，這樣轉彎時比較好算。想要 0 ~ 359，用 `((角度 % 360) + 360) % 360` 換算（範例 2 有）。累加超過 ±32767（大約 91 圈）時數字會繞回去，記得在每次比賽開始前歸零。

**Q：我按了 Arduino 的 RESET，角度怎麼沒有變回 0？**

按 RESET（或打開序列埠監控窗）只會讓 Arduino 重新開機，陀螺儀沒有重開，所以它還記得原來的角度。要歸零，就寫 `mbcWrite(0x10, 0x01)`（範例 2 輸入 `z`），寫完要**等 30 毫秒以上**再讀角度。陀螺儀自己開機、抓到「安裝方向」的那一刻，也會自動歸零一次。

**Q：第 0x11 格裡的 3 種角度要用哪一個？**

用第 2、3 個數字的 **Gyro3D（立體補償航向）**，範例都是用這個。它不管感應器怎麼裝（平放、直立、側放）都算得對。第 4、5 個數字是「加上磁力計修正」的角度，長時間跑比較不會飄，但它**不是指北針**：開機或歸零時一樣是 0，而且在馬達、鐵製場地旁邊會不準。

**Q：需要校準嗎？**

一般**不需要**。陀螺儀每次靜止時都會自己修正；加速度計用的是標準值，做水平儀、骰子已經夠準。感應器上的兩顆校準按鈕請不要按；不小心按到（旁邊的燈一直亮），再按一次就會結束，沒有做完整的校準不會被存起來。

**Q：可以跟其他 MBC 感應器接在同一組 SDA/SCL 上嗎？**

可以，只要門牌號碼不一樣就好（例如循行者 8 路是 `0x16`，陀螺儀是 `0x6A`）。但是 **Arduino Nano 33 IoT、Nano RP2040 Connect** 板子上本來就有一顆門牌號碼也是 `0x6A` 的晶片，會「撞號」，這兩塊板子不能直接接這個陀螺儀。

**Q：想知道每一個格子放什麼資料？**

請看 [陀螺儀 IMU 首頁](/sensors/imu/index.md) 的「通用 I2C 暫存器」表格，以及 [MBC 通用 I2C 協議](/i2c-protocol.md)（給會寫程式的人看的進階資料）。
