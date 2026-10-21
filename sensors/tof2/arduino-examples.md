<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 測距者 2 路 × Arduino 範例

[🔙 回到 2 路雷射測距首頁](/sensors/tof2/index.md)　｜　[📡 MBC 通用 I2C 協議（進階）](/i2c-protocol.md)

> [!NOTE]
> 通用 I2C 版即將推出，以下範例適用新版（2026 年 10 月之後）硬體。

這一頁有 **5 個範例**，從「確認接好了沒」一路做到測距者 2 路最厲害的功能：**判斷東西從哪一邊溜走**。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/tof2/arduino/tof2_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 感應器 | 新版板子焊錫點 | Arduino UNO / Nano | ESP32 |
| :---: | :---: | :---: | :---: |
| SCL | 1 | A5 | GPIO 22 |
| SDA | 2 | A4 | GPIO 21 |
| VCC | 3 | 5V | 5V（VIN）或 3.3V |
| GND | 4 | GND | GND |

> [!IMPORTANT]
> - 請用 **通用 I2C 版**（新版板子）的測距者 2 路。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino；舊版板子不支援 I2C。
> - 新版板子的焊錫點是 **1 = SCL、2 = SDA**，跟舊版板子印的 SDA／SCL 剛好相反。接不上時，先把 SDA、SCL 兩條線對調試試看（對調不會弄壞東西）。
> - 感應器一通電要先把 2 顆雷射叫醒，這段時間讀到的距離**全部是 0**，看起來就像貼著障礙物。所以每個範例一開始都會先等它「準備好」，你自己寫程式時也要這樣做。
> - 接線前先拔掉 USB 線。

**距離怎麼看？**（單位都是 mm，10 mm = 1 公分）

| 讀到的數字 | 意思 |
| :--- | :--- |
| `0xFFFF`（65535） | 這一顆**故障**（要最先檢查） |
| `1200` 以上 | 這一邊 120 公分內**沒有東西** |
| 其他 | 前面的東西有多遠 |

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟感應器打招呼](#ex1) | 確認接線正確、兩顆雷射都正常 | 無 |
| 2 | [左右兩邊各多遠？](#ex2) | 讀左、右兩顆的距離 | 無 |
| 3 | [東西在左邊、右邊，還是正前方？](#ex3) | 用兩顆雷射判斷東西在哪一邊 | 一個盒子或你的手 |
| 4 | [倒車雷達警報燈](#ex4) | 用距離控制燈，越近閃越快 | 無 |
| 5 | [東西從哪一邊溜走了？](#ex5) | 判斷東西往左還是往右離開 | 一個盒子 |

> [!TIP]
> **建議順序：1 → 2 → 3 → 4 → 5。** 範例 5 會用到範例 3 的「看到」判斷，先玩過範例 3 會比較好懂。

---

<h2 id="ex1">1️⃣ 跟感應器打招呼</h2>

**做什麼：** 確認感應器接好了，並檢查左、右兩顆雷射是不是都正常開機。每次接新的感應器，都先跑這一個。

**步驟：**
1. 照上面的表格接好 4 條線，USB 插上電腦。
2. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/tof2/arduino/tof2_01_hello/tof2_01_hello.ino" download data-ignore="true">📄 下載 tof2_01_hello.ino</a>

<!-- ino: examples/tof2/arduino/tof2_01_hello/tof2_01_hello.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 1：跟感應器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認感應器有沒有接好。接好了，會在電腦上印出感應器的名字、版本，
 *   並檢查左、右兩顆雷射是不是都正常。
 *   每次接新感應器，都建議先跑這個範例。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   （新版板子的焊錫點：1 = SCL、2 = SDA、3 = VCC、4 = GND）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   感應器一開機要先把 2 顆雷射叫醒，這段時間讀到的距離都是 0，
 *   所以一定要先等它「準備好」。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟感應器講話

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的「門牌號碼」，固定是 0x29

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等感應器開機 0.5 秒

  Serial.println(F("=== 測距者 2 路：跟感應器打招呼 ==="));

  // 等感應器說「我準備好了」，最多等 6 秒
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器！請檢查："));
    Serial.println(F("  1. SDA 接 A4、SCL 接 A5，有沒有接反？（新版焊錫點 1 = SCL、2 = SDA）"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？"));
    Serial.println(F("  3. 你的感應器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    Serial.println(F("  4. 同一組 SDA/SCL 上有沒有接其他門牌號碼也是 0x29 的東西（例如測距者 8 路、市售 VL53L0X 模組）？"));
    while (true) {}                      // 找不到就停在這裡
  }

  // 讀「版本資料」：感應器的第 0x01 號格子，共 17 個數字
  uint8_t info[17];
  if (!mbcRead(0x01, info, 17)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    while (true) {}
  }

  // 第 9 ~ 16 個數字是感應器的名字（英文字母）
  char name[9] = {0};
  memcpy(name, &info[9], 8);

  Serial.print(F("找到了！感應器名字："));
  Serial.println(name);                  // 應該會印出 LASER2

  Serial.print(F("韌體版本："));
  Serial.print(info[2]);                 // 主版本
  Serial.print('.');
  Serial.println(info[3]);               // 次版本

  Serial.print(F("雷射數量："));
  Serial.print(info[7]);                 // 測距者 2 路 = 2
  Serial.println(F(" 顆"));

  Serial.print(F("最遠量得到："));
  Serial.print(info[8] * 10);            // 這格存的是「幾個 10 毫米」，乘 10 變成毫米（1200）
  Serial.println(F(" mm（超過就當作「沒有東西」）"));

  // 讀「健康檢查」：第 0x31 號格子，共 9 個數字
  //   第 0 個：狀態（第 1 個位元 = 有沒有雷射故障）
  //   第 2 個：第 0 個位元 = 左邊雷射開機成功、第 1 個位元 = 右邊雷射開機成功
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    while (true) {}
  }

  if (st[0] & 0x02) {                    // 第 1 個位元是 1 → 有雷射故障
    if (!(st[2] & (1u << 0))) Serial.println(F("注意：左邊的雷射沒開機成功"));
    if (!(st[2] & (1u << 1))) Serial.println(F("注意：右邊的雷射沒開機成功"));
    Serial.println(F("（可以跑範例 2 看看是哪一邊一直顯示「故障」，再聯絡我們）"));
  } else {
    Serial.println(F("左、右兩顆雷射都正常！"));
  }

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
=== 測距者 2 路：跟感應器打招呼 ===
找到了！感應器名字：LASER2
韌體版本：1.0
雷射數量：2 顆
最遠量得到：1200 mm（超過就當作「沒有東西」）
左、右兩顆雷射都正常！
接線正確，可以開始玩其他範例了！
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟感應器「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(格子編號, 放哪裡, 幾個)`：從感應器讀資料
> - `mbcWaitReady(最多等幾毫秒)`：等感應器開機完成
>
> 感應器就像一排有編號的「格子」，每一格放著不同的資料，例如第 `0x11` 格放著「左、右兩邊的距離」。

---

<h2 id="ex2">2️⃣ 左右兩邊各多遠？</h2>

**做什麼：** 把左、右兩顆雷射量到的距離印出來，並畫成小長條（越長 = 越遠）。

<a href="/examples/tof2/arduino/tof2_02_left_right/tof2_02_left_right.ino" download data-ignore="true">📄 下載 tof2_02_left_right.ino</a>

<!-- ino: examples/tof2/arduino/tof2_02_left_right/tof2_02_left_right.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 2：左右兩邊各多遠？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   測距者 2 路有左、右兩顆雷射，都朝正前方看。
 *   這個範例把兩顆量到的距離印出來（單位：公分），並用長條畫出來：
 *     數字      = 前面的東西有多遠
 *     沒有東西  = 這一邊 120 公分內什麼都沒有
 *     故障      = 這一顆雷射壞了
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   用手只擋住左邊的雷射，再只擋右邊，看看哪一邊的數字在變。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint16_t NO_TARGET = 1200;         // 量到 1200 以上 = 這一邊沒有東西
const uint16_t FAULT = 0xFFFF;           // 收到 0xFFFF（65535）= 這一顆故障

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 雷射開機這段時間距離都是 0，一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！用手擋住左邊或右邊的雷射看看。"));
}

void loop() {
  // 第 0x11 號格子：4 個數字
  //   第 0、1 個合起來 = 左邊的距離（mm）
  //   第 2、3 個合起來 = 右邊的距離（mm）
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];   // 2 個數字合成一個距離
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  Serial.print(F("左 "));
  printSide(left);
  Serial.print(F("   右 "));
  printSide(right);
  Serial.println();

  delay(300);                            // 👉 可以改：每 0.3 秒看一次
}

// 印出一邊的距離：短長條 + 公分。判斷順序：先看故障，再看有沒有東西，最後才是距離
void printSide(uint16_t mm) {
  if (mm == FAULT) {
    Serial.print(F("[    故障    ] 這顆雷射壞了"));
    return;
  }
  if (mm >= NO_TARGET) {
    Serial.print(F("[############] 沒有東西"));
    return;
  }
  // 畫長條：每 100 mm（10 公分）一格，最多 12 格。越長 = 越遠
  Serial.print('[');
  uint8_t blocks = mm / 100;
  for (uint8_t k = 0; k < 12; k++) Serial.print(k < blocks ? '#' : ' ');
  Serial.print(F("] "));
  Serial.print(mm / 10);                 // mm 除以 10 = 公分
  Serial.print(F(" 公分"));
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

**你會看到：**（先只擋左邊，再兩邊都不擋，前面遠遠的有一面牆）

```text
準備好了！用手擋住左邊或右邊的雷射看看。
左 [###         ] 32 公分   右 [############] 沒有東西
左 [########    ] 85 公分   右 [########    ] 87 公分
```

> [!TIP]
> **試試看：** 用手只擋住一顆雷射，確認「左」真的是你以為的左邊。之後的範例都靠這個判斷方向。

---

<h2 id="ex3">3️⃣ 東西在左邊、右邊，還是正前方？</h2>

**做什麼：** 兩顆雷射並排朝前看，就像兩隻眼睛：**兩顆都看到 = 正前方**、只有左邊看到 = 偏左、只有右邊看到 = 偏右。機器人「追著東西走」的第一步就是這個。

**準備：** 一個盒子或你的手，放在感應器前面 20 公分左右。

<a href="/examples/tof2/arduino/tof2_03_where_is_it/tof2_03_where_is_it.ino" download data-ignore="true">📄 下載 tof2_03_where_is_it.ino</a>

<!-- ino: examples/tof2/arduino/tof2_03_where_is_it/tof2_03_where_is_it.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 3：東西在左邊、右邊，還是正前方？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   左、右兩顆雷射並排朝前看，就像兩隻眼睛：
 *     兩顆都看到東西   → 東西在正前方
 *     只有左邊看到     → 東西偏左
 *     只有右邊看到     → 東西偏右
 *     兩顆都沒看到     → 前面沒有東西
 *   機器人「追著東西走」或「找到目標」，第一步就是知道東西在哪一邊。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把一個盒子（或手）放在感應器前面 20 公分左右，慢慢左右移動。
 *
 * 小提醒：
 *   只算 50 公分內的東西（SEE_DIST），免得後面的牆壁也被當成目標。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

const uint16_t SEE_DIST = 500;           // 👉 可以改：多近才算「看到」（mm）。500 = 50 公分，最多 1199

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 雷射開機這段時間距離都是 0（會被當成「兩邊都貼著東西」），一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把東西放在感應器前面，慢慢左右移動。"));
}

void loop() {
  // 第 0x11 號格子：左邊距離（2 個數字）+ 右邊距離（2 個數字）
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  // 「看到」= 不是故障，而且夠近。故障的那一邊當作沒看到
  bool seeL = (left  != FAULT) && (left  < SEE_DIST);
  bool seeR = (right != FAULT) && (right < SEE_DIST);

  if (seeL && seeR) {
    Serial.print(F("↑ 正前方   距離 "));
    Serial.print(min(left, right) / 10);           // 兩邊比較近的那個，換成公分
    Serial.println(F(" 公分"));
  } else if (seeL) {
    Serial.print(F("↖ 偏左     距離 "));
    Serial.print(left / 10);
    Serial.println(F(" 公分   → 機器人要往左轉"));
  } else if (seeR) {
    Serial.print(F("↗ 偏右     距離 "));
    Serial.print(right / 10);
    Serial.println(F(" 公分   → 機器人要往右轉"));
  } else {
    Serial.println(F("   前面沒有東西"));
  }

  if (left == FAULT || right == FAULT) {
    Serial.println(F("（注意：有一顆雷射故障，請跑範例 1 檢查）"));
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

**你會看到：**（盒子從正前方慢慢往左移開）

```text
↑ 正前方   距離 18 公分
↖ 偏左     距離 22 公分   → 機器人要往左轉
   前面沒有東西
```

> [!TIP]
> **試試看：** 範例只算 50 公分內的東西（`SEE_DIST = 500`），免得把後面的牆當成目標。桌子很大、後面很空的話，可以改成 `1000` 看看。

---

<h2 id="ex4">4️⃣ 倒車雷達警報燈</h2>

**做什麼：** 用 Arduino 板子上內建的小燈（標著 **L** 那顆）當警報燈：沒有東西燈熄，有東西就閃，**越近閃越快**，貼到 10 公分內就一直亮。左、右哪一邊比較近就看哪一邊。

<a href="/examples/tof2/arduino/tof2_04_distance_alarm/tof2_04_distance_alarm.ino" download data-ignore="true">📄 下載 tof2_04_distance_alarm.ino</a>

<!-- ino: examples/tof2/arduino/tof2_04_distance_alarm/tof2_04_distance_alarm.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 4：倒車雷達警報燈
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用 Arduino 板子上內建的小燈（標著 L 的那顆）當警報燈，就像汽車的倒車雷達：
 *     前面沒有東西          → 燈熄
 *     有東西，越近閃得越快
 *     貼得很近（10 公分內） → 燈一直亮
 *   左、右兩顆雷射，哪一顆比較近就看哪一顆。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好。
 *
 * 怎麼看結果：
 *   上傳後，看 Arduino 板子上的 L 燈。手慢慢靠近感應器，燈會越閃越快。
 *   也可以打開「序列埠監控窗」（115200）看最近的距離。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）
const uint16_t NO_TARGET = 1200;         // 距離 1200 以上 = 沒有東西
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

// ---- 👉 可以改的參數 ----
const uint16_t ALARM_START = 1000;       // 多近開始閃（mm）：1000 = 1 公尺
const uint16_t ALWAYS_ON   = 100;        // 多近變成一直亮（mm）：100 = 10 公分

uint16_t nearest = NO_TARGET;            // 最近的東西有多遠（mm）
bool ledOn = false;                      // 燈現在是亮還是熄
unsigned long lastBlink = 0;             // 上一次切換燈的時間
unsigned long lastRead = 0;              // 上一次讀感應器的時間
unsigned long lastPrint = 0;             // 上一次印到電腦的時間

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);                  // 把燈的腳設成「輸出」，才能控制它亮或熄
  mbcBegin();
  delay(500);

  // 雷射開機這段時間距離都是 0（會被當成貼著東西狂閃），一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！手慢慢靠近感應器，看 L 燈越閃越快。"));
}

void loop() {
  // ---- 1. 每 0.05 秒讀一次左右距離，取比較近的那個 ----
  if (millis() - lastRead >= 50) {
    lastRead = millis();
    uint8_t d[4];
    if (mbcRead(0x11, d, 4)) {                         // 第 0x11 號格子：左距離 + 右距離
      uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
      uint16_t right = ((uint16_t)d[2] << 8) | d[3];
      if (left == FAULT)  left  = NO_TARGET;           // 故障的那一邊不算
      if (right == FAULT) right = NO_TARGET;
      nearest = min(left, right);                      // 比較近的那一邊
    } else {
      nearest = NO_TARGET;                             // 讀不到：先當作沒有東西，燈熄
    }
  }

  // ---- 2. 依距離決定燈怎麼亮 ----
  if (nearest >= ALARM_START) {
    ledOn = false;                                     // 太遠或沒有東西：燈熄
  } else if (nearest <= ALWAYS_ON) {
    ledOn = true;                                      // 非常近：燈一直亮
  } else {
    // 越近，閃的間隔越短：距離 100 mm → 每 0.05 秒切換；1000 mm → 每 0.5 秒切換
    unsigned long interval = map(nearest, ALWAYS_ON, ALARM_START, 50, 500);
    if (millis() - lastBlink >= interval) {
      lastBlink = millis();
      ledOn = !ledOn;                                  // 亮變熄、熄變亮
    }
  }
  digitalWrite(LED, ledOn ? HIGH : LOW);

  // ---- 3. 每 0.5 秒把距離印到電腦上 ----
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    if (nearest >= NO_TARGET) {
      Serial.println(F("沒有東西"));
    } else {
      Serial.print(F("最近的東西："));
      Serial.print(nearest / 10);
      Serial.println(F(" 公分"));
    }
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

**你會看到：**（手慢慢靠近，L 燈越閃越快）

```text
準備好了！手慢慢靠近感應器，看 L 燈越閃越快。
沒有東西
最近的東西：85 公分
最近的東西：42 公分
最近的東西：9 公分
```

> [!TIP]
> **試試看：** 把 `ALARM_START` 改成 `500`，變成 50 公分內才開始閃。

---

<h2 id="ex5">5️⃣ 東西從哪一邊溜走了？</h2>

**做什麼：** 這是測距者 2 路最厲害的功能。東西往右邊移走時，**左邊的雷射會先看不到它**，這時只剩右邊還看得到；等右邊也看不到，就知道它是**往右邊**溜走的。往左邊移走也是一樣的道理。相撲機器人的對手跑掉時，就知道要往哪邊轉去追。

| 東西消失前，最後是誰看得到？ | 東西往哪邊離開 |
| :--- | :--- |
| 只剩**右邊**的雷射（左邊先看不到） | 往**右邊** → |
| 只剩**左邊**的雷射（右邊先看不到） | 往**左邊** ← |
| 兩邊同時看不到 | 往後退開，或移得太快 |

**準備：** 一個盒子。先放在感應器正前方 20 公分（兩顆雷射都要看到），再**往左或往右**移開。

<a href="/examples/tof2/arduino/tof2_05_leave_direction/tof2_05_leave_direction.ino" download data-ignore="true">📄 下載 tof2_05_leave_direction.ino</a>

<!-- ino: examples/tof2/arduino/tof2_05_leave_direction/tof2_05_leave_direction.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 5：東西從哪一邊溜走了？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   這是測距者 2 路最厲害的功能：判斷東西是「從左邊」還是「從右邊」離開的。
 *   原理很簡單：東西往右邊移走時，左邊的雷射會「先」看不到它，
 *   這時只剩右邊的雷射還看得到；等右邊也看不到，就知道它是往右邊溜走的。
 *     最後只剩右邊看到 → 東西往右邊離開 →
 *     最後只剩左邊看到 → 東西往左邊離開 ←
 *   相撲機器人的對手跑掉時，就知道要往哪邊轉去追。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *   不用另外接燈，用板子上內建的燈就好（看到東西時亮）。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把一個盒子放在感應器正前方 20 公分（兩顆雷射都看到），
 *   然後把盒子往左邊或往右邊移開，看電腦說它往哪邊走了。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint8_t LED = LED_BUILTIN;         // 板子內建的燈（UNO 是第 13 腳）
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

// ---- 👉 可以改的參數 ----
const uint16_t SEE_DIST = 500;           // 多近才算「看到」（mm）：500 = 50 公分，免得把後面的牆當成目標
const unsigned long GONE_MS = 150;       // 兩邊都看不到超過多久（毫秒），才算真的離開

// 目前誰看得到：0 = 都看不到、1 = 只有左邊、2 = 只有右邊、3 = 兩邊都看到
uint8_t state = 0;
uint8_t lastOneSide = 0;                 // 東西消失前「只剩哪一邊」看到：0 = 沒有（兩邊同時消失）、1 = 左、2 = 右
bool present = false;                    // 現在是不是有東西在前面
unsigned long goneSince = 0;             // 從什麼時候開始兩邊都看不到

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);

  // 雷射開機這段時間距離都是 0（會被當成「兩邊都看到」），一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把盒子放在正前方，再往左或往右移開。"));
}

void loop() {
  // 第 0x11 號格子：左邊距離（2 個數字）+ 右邊距離（2 個數字）
  uint8_t d[4];
  if (!mbcRead(0x11, d, 4)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }
  uint16_t left  = ((uint16_t)d[0] << 8) | d[1];
  uint16_t right = ((uint16_t)d[2] << 8) | d[3];

  // 「看到」= 不是故障，而且夠近
  bool seeL = (left  != FAULT) && (left  < SEE_DIST);
  bool seeR = (right != FAULT) && (right < SEE_DIST);

  // 把兩邊看到的情形合成一個數字：左邊看到 +1、右邊看到 +2
  uint8_t now = (seeL ? 1 : 0) + (seeR ? 2 : 0);

  // ---- 1. 情形改變了，印出來 ----
  if (now != state) {
    state = now;
    if (state == 3)      Serial.println(F("  左 ● ● 右   兩邊都看到（在正前方）"));
    else if (state == 1) Serial.println(F("  左 ● ○ 右   只剩左邊看到"));
    else if (state == 2) Serial.println(F("  左 ○ ● 右   只剩右邊看到"));
    else                 Serial.println(F("  左 ○ ○ 右   兩邊都看不到"));
  }

  // ---- 2. 有看到東西：記住「最後只剩哪一邊」 ----
  if (state != 0) {
    if (!present) {
      present = true;
      Serial.println(F("發現東西！"));
    }
    if (state == 3) lastOneSide = 0;          // 兩邊都看到 → 還沒開始離開
    else            lastOneSide = state;      // 只剩一邊看到 → 記下是哪一邊
    goneSince = millis();                     // 還看得到，重新計時
  }

  // ---- 3. 兩邊都看不到一段時間了：宣布它從哪邊離開 ----
  if (state == 0 && present && millis() - goneSince >= GONE_MS) {
    present = false;
    if (lastOneSide == 1)      Serial.println(F("←←← 東西往「左邊」離開了！（右邊的雷射先看不到）"));
    else if (lastOneSide == 2) Serial.println(F("→→→ 東西往「右邊」離開了！（左邊的雷射先看不到）"));
    else                       Serial.println(F("東西兩邊同時消失了（可能是往後退開，或移得太快）"));
    Serial.println();
  }

  digitalWrite(LED, present ? HIGH : LOW);    // 有東西時燈亮

  delay(30);                                  // 讀快一點，才抓得到「哪一邊先」
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

**你會看到：**（盒子放在正前方，再往右移開）

```text
準備好了！把盒子放在正前方，再往左或往右移開。
  左 ● ● 右   兩邊都看到（在正前方）
發現東西！
  左 ○ ● 右   只剩右邊看到
  左 ○ ○ 右   兩邊都看不到
→→→ 東西往「右邊」離開了！（左邊的雷射先看不到）
```

> [!TIP]
> **試試看：** 如果一直出現「兩邊同時消失」，把盒子移慢一點，或用寬一點的盒子。學會以後，可以在「往右邊離開了」那一行後面加上讓機器人右轉的程式，做出會追對手的相撲機器人。

---

## ❓ 常見問題

**Q：一直出現「找不到感應器」？**
1. 檢查 SDA、SCL 有沒有接反（UNO：SDA → A4、SCL → A5）。新版板子焊錫點 **1 = SCL、2 = SDA**，跟舊版印的字相反，先對調兩條線試試看。
2. 檢查 VCC、GND 有沒有插緊。感應器上的燈有沒有亮？
3. 確認是 **通用 I2C 版**（新版板子）：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：一開機距離是 0？**
感應器還在把雷射叫醒，這段時間的資料都是 0。範例都已經用 `mbcWaitReady(6000)` 等好了；你自己寫程式時，也要先等第 `0x32` 格的第 0 個位元變成 1 再開始讀。

**Q：有一邊一直顯示「故障」？**
先關掉電源、重新插好再試一次。還是一樣的話，跑 [範例 1](#ex1) 看是哪一邊沒開機成功，再 [聯絡我們](/contact.md) 協助檢修。

**Q：東西明明在前面，卻顯示「沒有東西」？**
測距者最遠量到 120 公分。另外，**黑色絨布、透明玻璃、亮面鏡子**這類東西反射的雷射光很少或很亂，比較難量到，換成白色或淺色的東西試試看。

**Q：可以跟測距者 8 路，或市售的 VL53L0X 雷射模組接在一起嗎？**
不行。它們的門牌號碼都是 `0x29`，同一組 SDA/SCL 上只能接一個。可以跟其他門牌號碼不同的 MBC 產品（例如循行者 `0x16`）接在同一組線上。

**Q：想知道每一個格子放什麼資料？**

| 格子 | 讀幾個 | 內容 |
| :---: | :---: | :--- |
| `0x01` | 17 | 版本資料（第 9 ~ 16 個是名字 `LASER2`） |
| `0x11` | 4 | 左距離、右距離（各 2 個數字，大的那一半在前，單位 mm） |
| `0x31` | 9 | 狀態（第 0 個：第 0 位元 = 準備好、第 1 位元 = 有雷射故障；第 2 個：開機成功的雷射） |

完整的共通規則請看 [MBC 通用 I2C 協議](/i2c-protocol.md)。
