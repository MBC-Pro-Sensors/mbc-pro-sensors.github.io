<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 循行者 8 路 × Arduino 範例

[🔙 回到 8 路循線器首頁](/sensors/line8/index.md)　｜　[📡 I2C 暫存器說明（進階）](/sensors/line8/arduino-i2c.md)

這一頁有 **6 個範例**，從「確認接好了沒」一路做到「循線小車」。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/line8/arduino/line8_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 感應器 | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: |
| VCC | 5V | 5V（VIN） |
| GND | GND | GND |
| SDA | A4 | GPIO 21 |
| SCL | A5 | GPIO 22 |

> [!IMPORTANT]
> - 請用 **通用 I2C 版** 的循行者。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 感應器裝在車上時，**底部離地面約 0.5～1 公分**最準。
> - 接線前先拔掉 USB 線。

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟感應器打招呼](#ex1) | 確認接線正確 | 無 |
| 2 | [線在哪裡？](#ex2) | 讀「線的位置」 | 一張白紙＋黑色電工膠帶 |
| 3 | [8 隻眼睛看到什麼？](#ex3) | 每一顆感應器各自的讀值 | 同上 |
| 4 | [一鍵校準](#ex4) | 讓感應器記住場地的黑和白 | 同上 |
| 5 | [路口偵測燈](#ex5) | 數路口、判斷線往哪邊跑掉 | 畫了十字路口的紙 |
| 6 | [循線小車（進階）](#ex6) | 讓車子自己沿線跑 | 兩輪小車、L298N 馬達驅動板 |

> [!TIP]
> **建議順序：1 → 4 → 2 → 3 → 5 → 6。** 先確認接好（1），校準場地（4），之後的讀數才會準。

---

<h2 id="ex1">1️⃣ 跟感應器打招呼</h2>

**做什麼：** 確認感應器接好了。每次接新的感應器，都先跑這一個。

**步驟：**
1. 照上面的表格接好 4 條線，USB 插上電腦。
2. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/line8/arduino/line8_01_hello/line8_01_hello.ino" download data-ignore="true">📄 下載 line8_01_hello.ino</a>

<!-- ino: examples/line8/arduino/line8_01_hello/line8_01_hello.ino -->
```cpp
/*
 * 循行者 8 路 × Arduino　範例 1：跟感應器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認感應器有沒有接好。接好了，會在電腦上印出感應器的名字和版本。
 *   每次接新感應器，都建議先跑這個範例。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟感應器講話

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的「門牌號碼」，固定是 0x16

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等感應器開機 0.5 秒

  Serial.println(F("=== 循行者 8 路：跟感應器打招呼 ==="));

  // 等感應器說「我準備好了」，最多等 2 秒
  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器！請檢查："));
    Serial.println(F("  1. SDA 接 A4、SCL 接 A5，有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？"));
    Serial.println(F("  3. 你的感應器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    return;                              // 找不到就不往下做了
  }

  // 讀「版本資料」：感應器的第 0x01 號格子，共 17 個數字
  uint8_t info[17];
  mbcRead(0x01, info, 17);

  // 第 9 ~ 16 個數字是感應器的名字（英文字母）
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("找到了！感應器名字："));
  Serial.println(name);                  // 應該會印出 LINE8

  Serial.print(F("韌體版本："));
  Serial.print(info[2]);                 // 主版本
  Serial.print('.');
  Serial.println(info[3]);               // 次版本

  Serial.print(F("感應器數量："));
  Serial.print(info[7]);                 // 循行者 8 路 = 8
  Serial.println(F(" 個"));

  Serial.println(F("接線正確，可以開始玩其他範例了！"));
}

void loop() {
  // 這個範例只要在開機時檢查一次，所以 loop 裡什麼都不做
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
=== 循行者 8 路：跟感應器打招呼 ===
找到了！感應器名字：LINE8
韌體版本：1.0
感應器數量：8 個
接線正確，可以開始玩其他範例了！
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟感應器「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(格子編號, 放哪裡, 幾個)`：從感應器讀資料
> - `mbcWrite(格子編號, 數字)`：把設定寫進感應器
>
> 感應器就像一排有編號的「格子」，每一格放著不同的資料，例如第 `0x11` 格放著「線在哪裡」。

---

<h2 id="ex2">2️⃣ 線在哪裡？</h2>

**做什麼：** 左右移動感應器，電腦會畫一把小尺，用 `#` 標出線的位置，並告訴你該往哪邊轉。

> [!NOTE]
> **「左、右」怎麼算？** 感應器裝在車上（感應面朝下），**你站在車子後面往前看**的左右。所有循行者範例都用這個方向。

**準備：** 在白紙上貼一條黑色電工膠帶（寬約 2 公分）當作線。

<a href="/examples/line8/arduino/line8_02_where_is_line/line8_02_where_is_line.ino" download data-ignore="true">📄 下載 line8_02_where_is_line.ino</a>

<!-- ino: examples/line8/arduino/line8_02_where_is_line/line8_02_where_is_line.ino -->
```cpp
/*
 * 循行者 8 路 × Arduino　範例 2：線在哪裡？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器放在白紙上的黑線上方，左右移動感應器，
 *   電腦會告訴你「線在左邊、中間、還是右邊」，還會畫出一條小尺標出線的位置。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   第一次使用、或換了比賽場地，請先跑「範例 4：一鍵校準」，讀數會更準。
 *   這裡說的「左、右」，是指感應器裝在車上（感應面朝下），你站在車子後面往前看的左右。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼

// 👉 可以改：0 = 循黑線（白底黑線），1 = 循白線（黑底白線）
const uint8_t LINE_COLOR = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}                      // 停在這裡
  }

  mbcWrite(0x10, LINE_COLOR);            // 告訴感應器要找黑線還是白線
  Serial.println(F("準備好了！把感應器放在線上，左右移動看看。"));
}

void loop() {
  // 一次讀 7 個數字：感應器把「循線結果」都放在第 0x11 號格子
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  int8_t  position = (int8_t)d[0];       // 線的位置：負數 = 線在左邊，正數 = 線在右邊，0 = 正中間
  uint8_t groups   = d[3];               // 看到幾條線：0 = 沒看到線

  if (groups == 0) {
    Serial.println(F("|         |  看不到線（線跑掉了）"));
  } else {
    // 畫一把小尺：最左邊是 -8，最右邊是 +8，用 # 標出線的位置
    Serial.print('|');
    for (int8_t p = -8; p <= 8; p += 2) {
      // 每一格代表 2 個單位，看線落在哪一格
      if (position >= p && position <= p + 1) Serial.print('#');
      else Serial.print(' ');
    }
    Serial.print(F("|  位置 "));
    Serial.print(position);

    if (position > 1)       Serial.println(F("  → 線在右邊，要往右轉"));
    else if (position < -1) Serial.println(F("  → 線在左邊，要往左轉"));
    else                    Serial.println(F("  → 線在正中間，直走！"));
  }

  delay(200);                            // 👉 可以改：每 0.2 秒看一次，數字越小更新越快
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

**你會看到：**（把黑線從左邊慢慢移到右邊）

```text
| #       |  位置 -5  → 線在左邊，要往左轉
|   #     |  位置 -1  → 線在正中間，直走！
|     #   |  位置 3  → 線在右邊，要往右轉
|         |  看不到線（線跑掉了）
```

**位置是什麼意思？**

| 位置數字 | 意思 |
| :---: | :--- |
| 0 | 線在正中間 |
| 負數（-1 ~ -7） | 線在**左**邊，數字越小越偏左 |
| 正數（+1 ~ +7） | 線在**右**邊，數字越大越偏右 |
| ±8 | 線剛從最左（-8）或最右（+8）跑掉 |

> [!TIP]
> **試試看：** 把 `LINE_COLOR` 改成 `1`，再拿黑紙貼白膠帶，就變成循白線了。

---

<h2 id="ex3">3️⃣ 8 隻眼睛看到什麼？</h2>

**做什麼：** 循行者底下有 8 顆光感應器，像 8 隻眼睛排成一排。這個範例把每一隻眼睛看到什麼都畫出來：`●` = 看到線、`○` = 看到底色；下面一排是每隻眼睛的**顏色分數**（0 = 很黑，100 = 很白）。

<a href="/examples/line8/arduino/line8_03_eight_eyes/line8_03_eight_eyes.ino" download data-ignore="true">📄 下載 line8_03_eight_eyes.ino</a>

<!-- ino: examples/line8/arduino/line8_03_eight_eyes/line8_03_eight_eyes.ino -->
```cpp
/*
 * 循行者 8 路 × Arduino　範例 3：8 隻眼睛看到什麼？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   循行者底下有 8 顆光感應器，就像 8 隻眼睛排成一排。
 *   這個範例把每隻眼睛看到的東西印出來：
 *     ● = 這隻眼睛看到線      ○ = 這隻眼睛看到底色
 *   下面再印出每隻眼睛的「顏色分數」：0 = 很黑，100 = 很白。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   印出來的順序就是感應器裝在車上、你站在車子後面往前看的順序：左邊的眼睛印在左邊。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼
const uint8_t EYES = 8;                  // 循行者 8 路有 8 隻眼睛

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把感應器移到線上，看看哪幾隻眼睛會變成 ●"));
}

void loop() {
  // 第 0x16 號格子：2 個數字，合起來的每一個「位元」代表一隻眼睛有沒有看到線
  uint8_t b[2];
  // 第 0x50 號格子：8 個數字，每隻眼睛的顏色分數 0 ~ 100
  uint8_t score[EYES];

  if (!mbcRead(0x16, b, 2) || !mbcRead(0x50, score, EYES)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  uint16_t onLine = ((uint16_t)b[0] << 8) | b[1];  // 把 2 個數字合成一個

  // 眼睛的編號（站在車子後面往前看）：第 0 隻在最左邊，第 7 隻在最右邊。
  // 所以從第 0 隻印到第 7 隻，畫面就跟實際的左右一樣。
  Serial.print(F("左 "));
  for (int8_t i = 0; i < EYES; i++) {
    if (onLine & (1u << i)) Serial.print(F("  ● "));
    else                    Serial.print(F("  ○ "));
  }
  Serial.println(F("右"));

  Serial.print(F("分"));
  for (int8_t i = 0; i < EYES; i++) {
    Serial.print(' ');
    if (score[i] < 100) Serial.print(' ');   // 對齊用：不到 3 位數，前面補空白
    if (score[i] < 10)  Serial.print(' ');
    Serial.print(score[i]);
  }
  Serial.println();
  Serial.println();

  delay(300);                            // 👉 可以改：每 0.3 秒看一次
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

**你會看到：**（線在中間偏左）

```text
左   ○   ○   ●   ●   ○   ○   ○   ○ 右
分 100  98  12   8  95 100 100  99
```

> [!TIP]
> **試試看：** 慢慢把感應器抬高，看分數怎麼變。抬太高時黑白分數會越來越接近，這就是為什麼感應器要貼近地面。

---

<h2 id="ex4">4️⃣ 一鍵校準</h2>

**做什麼：** 讓感應器記住**這個場地**的黑色和白色。換場地、燈光不一樣時都要重新校準。校準結果會存在感應器裡，關機也不會忘記。

**步驟：**
1. 上傳程式，打開序列埠監控窗（115200）。
2. 把感應器放在線的上方，高度跟裝在車上一樣（離地約 0.5～1 公分）。
3. 在監控窗上方的**輸入框打 `c`，按 Enter**。
4. 接下來 **5 秒內**，慢慢把感應器左右移動，讓每一隻眼睛都掃過黑線和白底。
5. 看到「校準完成」就好了。

<a href="/examples/line8/arduino/line8_04_calibrate/line8_04_calibrate.ino" download data-ignore="true">📄 下載 line8_04_calibrate.ino</a>

<!-- ino: examples/line8/arduino/line8_04_calibrate/line8_04_calibrate.ino -->
```cpp
/*
 * 循行者 8 路 × Arduino　範例 4：一鍵校準
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   「校準」就是讓感應器記住比賽場地的黑色和白色長什麼樣子。
 *   換了場地、燈光不一樣，都建議重新校準一次。校準結果會存在感應器裡，關機也不會忘記。
 *
 * 怎麼校準：
 *   1. 把感應器放在線的上方（離地面約 0.5～1 公分，跟裝在車上一樣高）。
 *   2. 在序列埠監控窗上方的輸入框打 c，按 Enter。
 *   3. 接下來 5 秒內，慢慢把感應器左右移動，讓每一隻眼睛都「掃過」黑線和白底。
 *   4. 看到「校準完成」就好了。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！在上方輸入框打 c 再按 Enter，開始校準。"));
}

void loop() {
  // 有沒有人在序列埠監控窗打字？
  if (Serial.available()) {
    char key = Serial.read();            // 讀進一個字
    if (key == 'c' || key == 'C') {
      calibrate();
    }
  }
}

// 校準的步驟都寫在這裡
void calibrate() {
  // 第 0x21 號格子有 2 個數字，第 2 個是「已經校準過幾次」
  uint8_t before[2];
  if (!mbcRead(0x21, before, 2)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    return;
  }

  // 寫 1 到第 0x20 號格子 = 開始 5 秒校準
  mbcWrite(0x20, 1);
  Serial.println(F("開始校準！5 秒內請左右移動感應器，掃過黑線和白底……"));

  // 一直檢查「校準次數」有沒有加 1。加 1 了就代表校準完成
  unsigned long start = millis();
  while (millis() - start < 7000) {      // 最多等 7 秒
    delay(250);
    Serial.print('.');                   // 印一個點，表示還在等

    uint8_t now[2];
    if (mbcRead(0x21, now, 2) && now[1] != before[1]) {
      Serial.println();
      Serial.println(F("校準完成！結果已存進感應器，關機也不會忘記。"));
      showResult();
      return;
    }
  }

  Serial.println();
  Serial.println(F("等太久了，校準沒有完成。請再打一次 c 試試看。"));
}

// 把每隻眼睛記住的「白色」和「黑色」印出來看看
void showResult() {
  uint8_t white[8], black[8];
  mbcRead(0x51, white, 8);               // 第 0x51 號格子：每隻眼睛記住的白色
  mbcRead(0x52, black, 8);               // 第 0x52 號格子：每隻眼睛記住的黑色

  Serial.println(F("眼睛（左→右）  白色  黑色  差距"));
  for (int8_t i = 0; i < 8; i++) {      // 第 0 隻在最左邊（站在車子後面往前看）
    Serial.print(F("  第 "));
    Serial.print(i);
    Serial.print(F(" 隻        "));
    Serial.print(white[i]);
    Serial.print(F("   "));
    Serial.print(black[i]);
    Serial.print(F("   "));
    Serial.println(white[i] - black[i]);
  }
  Serial.println(F("「差距」越大越好。差距太小（小於 20）代表那隻眼睛沒掃到黑線，請重新校準。"));
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

> [!WARNING]
> 校準時**每一隻眼睛都要掃過黑線**。如果最旁邊的眼睛沒掃到，它會一直以為整個場地都是白的，循線時就會判斷錯誤。

---

<h2 id="ex5">5️⃣ 路口偵測燈</h2>

**做什麼：** 用 Arduino 板子上內建的小燈（標著 **L** 那顆）當訊號燈：看到線就亮、線跑掉就熄，遇到十字路口或岔路會**快閃 3 下**，並且數出「這是第幾個路口」。比賽常常要「走到第 3 個路口右轉」，這個範例就是在學數路口。

**準備：** 在白紙上用黑膠帶貼一條直線，再橫貼 2～3 條當作路口。

<a href="/examples/line8/arduino/line8_05_crossroad_led/line8_05_crossroad_led.ino" download data-ignore="true">📄 下載 line8_05_crossroad_led.ino</a>

<!-- ino: examples/line8/arduino/line8_05_crossroad_led/line8_05_crossroad_led.ino -->
```cpp
/*
 * 循行者 8 路 × Arduino　範例 5：路口偵測燈
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用 Arduino 板子上內建的小燈（標著 L 的那顆）當訊號燈：
 *     看到線        → 燈亮
 *     線跑掉了      → 燈熄，電腦告訴你線最後是往哪邊跑掉的
 *     遇到十字路口、岔路 → 燈快閃 3 下，並且數「這是第幾個路口」
 *   比賽常常要「走到第 3 個路口右轉」，這個範例就是在學怎麼數路口。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   拿一張畫了十字線的紙，讓感應器沿著線慢慢經過路口。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）

int crossCount = 0;                      // 已經經過幾個路口
bool onCross = false;                    // 現在是不是正在路口上
bool lost = false;                       // 現在是不是看不到線

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // 把燈的腳設成「輸出」，才能控制它亮或熄
  mbcBegin();
  delay(500);

  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！讓感應器沿著線經過路口看看。"));
}

void loop() {
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {            // 讀循線結果（7 個數字）
    delay(100);
    return;
  }

  uint8_t width    = d[2];               // 有幾隻眼睛同時看到線
  uint8_t groups   = d[3];               // 看到幾段線：0 = 沒線，1 = 一條線，2 以上 = 岔路
  int8_t  lastSide = (int8_t)d[6];       // 線最後碰到哪一邊的邊緣：負數 = 左邊，正數 = 右邊

  // ---- 1. 看不到線 ----
  if (groups == 0) {
    digitalWrite(LED, LOW);              // 燈熄
    if (!lost) {                         // 剛剛才跑掉，只說一次就好
      lost = true;
      if (lastSide < 0)      Serial.println(F("線跑掉了！最後在左邊 → 往左找"));
      else if (lastSide > 0) Serial.println(F("線跑掉了！最後在右邊 → 往右找"));
      else                   Serial.println(F("看不到線"));
    }
    onCross = false;
    delay(20);
    return;
  }
  lost = false;

  // ---- 2. 是不是路口？ ----
  // 看到 2 段以上的線（岔路），或很多隻眼睛同時看到線（十字路口的橫線），就當作路口
  bool isCross = (groups >= 2) || (width >= 6);   // 👉 可以改：6 隻以上眼睛看到線算路口

  if (isCross && !onCross) {             // 剛走進路口的那一下才算一次，不會重複數
    onCross = true;
    crossCount = crossCount + 1;
    Serial.print(F("第 "));
    Serial.print(crossCount);
    Serial.println(F(" 個路口！"));
    for (int i = 0; i < 3; i++) {        // 快閃 3 下
      digitalWrite(LED, LOW);  delay(80);
      digitalWrite(LED, HIGH); delay(80);
    }
  }
  if (!isCross) onCross = false;         // 離開路口了，下一個路口才能再數

  // ---- 3. 一般情況：看到線就亮燈 ----
  digitalWrite(LED, HIGH);
  delay(20);
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
準備好了！讓感應器沿著線經過路口看看。
第 1 個路口！
第 2 個路口！
線跑掉了！最後在左邊 → 往左找
```

> [!TIP]
> **試試看：** 加一行 `if (crossCount == 3) { ... }`，讓第 3 個路口時做一件特別的事，例如燈一直閃。

---

<h2 id="ex6">6️⃣ 循線小車（進階）</h2>

**做什麼：** 讓兩輪小車沿著黑線自己跑。線偏左就往左修、偏右就往右修，**偏越多修越多**，這叫「比例控制」，是循線比賽最基本的招式。線跑掉時，會往線最後消失的那一邊原地找回來。

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
1. 先跑 [範例 4](#ex4) 校準場地。
2. 把車子架高（輪子懸空）再上傳，確認兩個輪子都是**往前**轉。哪個輪子轉反了，就把 `LEFT_REVERSE` 或 `RIGHT_REVERSE` 改成 `true`。
3. 放到線上，3 秒後出發。

<a href="/examples/line8/arduino/line8_06_line_follower_car/line8_06_line_follower_car.ino" download data-ignore="true">📄 下載 line8_06_line_follower_car.ino</a>

<!-- ino: examples/line8/arduino/line8_06_line_follower_car/line8_06_line_follower_car.ino -->
```cpp
/*
 * 循行者 8 路 × Arduino　範例 6（進階）：循線小車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車沿著黑線自己跑。線偏左就往左修、線偏右就往右修，
 *   偏越多、修越多。這種方法叫做「比例控制」，是循線比賽最基本的招式。
 *   線跑掉時，會往線最後消失的那一邊原地找線。
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
 *   感應器裝在車頭正中間，離地面約 0.5～1 公分。
 *
 * 第一次跑之前：
 *   1. 先跑「範例 4：一鍵校準」校準場地。
 *   2. 把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把下面的 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x16;        // 循行者感應器的門牌號碼

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
const int   BASE_SPEED = 120;            // 直走的速度：0 ~ 255，先慢一點比較好調
const float KP         = 1.0;            // 修正力道：車子修不回來就加大，左右搖擺就減小
const int   SEARCH_SPEED = 100;          // 找線時原地轉的速度
const bool  LEFT_REVERSE  = false;       // 左輪轉反了改成 true
const bool  RIGHT_REVERSE = false;       // 右輪轉反了改成 true

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  mbcWrite(0x10, 0);                     // 循黑線（白線請改成 1）

  Serial.println(F("3 秒後出發！"));
  delay(3000);
}

void loop() {
  uint8_t d[7];
  if (!mbcRead(0x11, d, 7)) {            // 讀不到感應器 → 為了安全，先停車
    drive(0, 0);
    return;
  }

  int8_t  error  = (int8_t)d[1];         // 線偏了多少：-100（很左）~ +100（很右），0 = 正中間
  uint8_t groups = d[3];                 // 0 = 看不到線

  if (groups == 0) {
    // 線跑掉了：往線最後消失的那一邊原地轉，把線找回來
    if (error < 0) drive(-SEARCH_SPEED, SEARCH_SPEED);   // 線在左邊 → 原地左轉
    else           drive(SEARCH_SPEED, -SEARCH_SPEED);   // 線在右邊 → 原地右轉
    return;
  }

  // 比例控制：偏多少，就修多少
  int turn = KP * error;                 // 線在右邊（正數）→ turn 是正的 → 左輪快、右輪慢 → 往右轉
  drive(BASE_SPEED + turn, BASE_SPEED - turn);
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

**怎麼調整，讓車子跑得更好：**

| 車子的樣子 | 怎麼改 |
| :--- | :--- |
| 轉彎時衝出線外，修不回來 | `KP` 加大（例如 1.0 → 1.5），或 `BASE_SPEED` 調小 |
| 在直線上一直左右搖擺 | `KP` 減小（例如 1.0 → 0.6） |
| 跑得很穩，想更快 | `BASE_SPEED` 一次加 20，再重新調 `KP` |
| 一出發就原地打轉 | 有一個輪子轉反了，改 `LEFT_REVERSE` 或 `RIGHT_REVERSE` |

---

## ❓ 常見問題

**Q：一直出現「找不到感應器」？**
1. 檢查 SDA、SCL 有沒有接反（UNO：SDA → A4、SCL → A5）。
2. 檢查 VCC、GND 有沒有插緊。感應器上的燈有沒有亮？
3. 確認是 **通用 I2C 版**：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：線明明在中間，位置卻一直跳？**
先跑 [範例 4](#ex4) 校準。再檢查感應器離地面的高度（0.5～1 公分最好）。

**Q：可以同時接兩個循行者嗎？**
不行。循行者 8 路和 16 路的門牌號碼都是 `0x16`，同一組 SDA/SCL 上只能接一個。可以跟其他門牌號碼不同的 MBC 產品（例如陀螺儀 `0x6A`）接在同一組線上。

**Q：想知道每一個格子放什麼資料？**
請看 [I2C 暫存器說明](/sensors/line8/arduino-i2c.md)（給會寫程式的人看的進階資料）。
