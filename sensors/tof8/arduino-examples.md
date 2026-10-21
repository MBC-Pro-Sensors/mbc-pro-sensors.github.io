<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 測距者 8 路 × Arduino 範例

[🔙 回到 8 路雷射測距首頁](/sensors/tof8/index.md)　｜　[📡 MBC 通用 I2C 協議（進階）](/i2c-protocol.md)

這一頁有 **6 個範例**，從「確認接好了沒」一路做到「避障小車」。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/tof8/arduino/tof8_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
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
> - 請用 **通用 I2C 版** 的測距者 8 路。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 感應器一通電，要花 **2 ~ 4 秒**把 8 顆雷射一顆一顆叫醒。這段時間讀到的距離**全部是 0**，看起來就像貼著障礙物。所以每個範例一開始都會先等它「準備好」（最多等 6 秒），你自己寫程式時也要這樣做。
> - 接線前先拔掉 USB 線。

**8 顆雷射朝哪裡看？**（從感應器後面、跟它看同一個方向）

<!-- 需實機確認：各顆的實際安裝角度、第 0 顆是否在機器人的左邊。來源：產品頁角度表＋韌體 SENSOR_POS（第 3、4 顆同為正前方）。 -->

| 第幾顆 | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 大約的角度 | 左 90° | 左 60° | 左 30° | 正前方 | 正前方 | 右 30° | 右 60° | 右 90° |
| 方位數字 | -7 | -5 | -3 | -1 | +1 | +3 | +5 | +7 |

**距離怎麼看？**（單位都是 mm，10 mm = 1 公分）

| 讀到的數字 | 意思 |
| :--- | :--- |
| `0xFFFF`（65535） | 這一顆**故障**（要最先檢查） |
| `1200` 以上 | 這個方向 120 公分內**沒有東西** |
| 其他 | 前面的東西有多遠 |

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟感應器打招呼](#ex1) | 確認接線正確、8 顆雷射都正常 | 無 |
| 2 | [8 顆雷射量到多遠？](#ex2) | 讀每一顆的距離，分辨「故障」和「沒有東西」 | 無 |
| 3 | [最近的東西在哪個方向？](#ex3) | 讀「方位」，用箭頭說出方向 | 一個盒子或你的手 |
| 4 | [倒車雷達警報燈](#ex4) | 用距離控制燈，越近閃越快 | 無 |
| 5 | [雷達長條圖](#ex5) | 把 8 個方向畫成長條，一眼看出哪邊空 | 幾本書當障礙物 |
| 6 | [避障小車（進階）](#ex6) | 讓車子自己繞過障礙物 | 兩輪小車、L298N 馬達驅動板 |

> [!TIP]
> **建議順序：1 → 2 → 3 → 4 → 5 → 6。** 先確認接好（1），看懂每一顆的距離（2），再學方向（3）和小應用。

---

<h2 id="ex1">1️⃣ 跟感應器打招呼</h2>

**做什麼：** 確認感應器接好了，並檢查 8 顆雷射是不是都正常開機。每次接新的感應器，都先跑這一個。

**步驟：**
1. 照上面的表格接好 4 條線，USB 插上電腦。
2. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/tof8/arduino/tof8_01_hello/tof8_01_hello.ino" download data-ignore="true">📄 下載 tof8_01_hello.ino</a>

<!-- ino: examples/tof8/arduino/tof8_01_hello/tof8_01_hello.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 1：跟感應器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認感應器有沒有接好。接好了，會在電腦上印出感應器的名字、版本，
 *   並檢查 8 顆雷射是不是都正常。
 *   每次接新感應器，都建議先跑這個範例。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   測距者 8 路一開機要花 2 ~ 4 秒把 8 顆雷射一顆一顆叫醒，
 *   這段時間讀到的距離全部都是 0（看起來像貼著東西），所以一定要先等它「準備好」。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟感應器講話

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的「門牌號碼」，固定是 0x29

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等感應器開機 0.5 秒

  Serial.println(F("=== 測距者 8 路：跟感應器打招呼 ==="));
  Serial.println(F("等 8 顆雷射開機（最多 6 秒）..."));

  // 等感應器說「我準備好了」，最多等 6 秒（8 顆雷射開機要 2 ~ 4 秒）
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器！請檢查："));
    Serial.println(F("  1. SDA 接 A4、SCL 接 A5，有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？"));
    Serial.println(F("  3. 你的感應器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    Serial.println(F("  4. 同一組 SDA/SCL 上有沒有接其他門牌號碼也是 0x29 的東西（例如測距者 2 路、市售 VL53L0X 模組）？"));
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
  Serial.println(name);                  // 應該會印出 LASER8

  Serial.print(F("韌體版本："));
  Serial.print(info[2]);                 // 主版本
  Serial.print('.');
  Serial.println(info[3]);               // 次版本

  Serial.print(F("雷射數量："));
  Serial.print(info[7]);                 // 測距者 8 路 = 8
  Serial.println(F(" 顆"));

  Serial.print(F("最遠量得到："));
  Serial.print(info[8] * 10);            // 這格存的是「幾個 10 毫米」，乘 10 變成毫米（1200）
  Serial.println(F(" mm（超過就當作「沒有東西」）"));

  // 讀「健康檢查」：第 0x31 號格子，共 9 個數字
  //   第 0 個：狀態（第 1 個位元 = 有沒有雷射故障）
  //   第 2 個：每一個位元代表一顆雷射有沒有開機成功（1 = 成功）
  uint8_t st[9];
  if (!mbcRead(0x31, st, 9)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    while (true) {}
  }

  if (st[0] & 0x02) {                    // 第 1 個位元是 1 → 有雷射故障
    Serial.print(F("注意：有雷射沒開機成功，編號："));
    for (uint8_t i = 0; i < 8; i++) {
      if (!(st[2] & (1u << i))) {        // 這一顆的位元是 0 → 沒開機成功
        Serial.print(i);
        Serial.print(' ');
      }
    }
    Serial.println();
    Serial.println(F("（可以跑範例 2 看看是哪一顆一直顯示 XX，再聯絡我們）"));
  } else {
    Serial.println(F("8 顆雷射都正常！"));
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
=== 測距者 8 路：跟感應器打招呼 ===
等 8 顆雷射開機（最多 6 秒）...
找到了！感應器名字：LASER8
韌體版本：1.0
雷射數量：8 顆
最遠量得到：1200 mm（超過就當作「沒有東西」）
8 顆雷射都正常！
接線正確，可以開始玩其他範例了！
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟感應器「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(格子編號, 放哪裡, 幾個)`：從感應器讀資料
> - `mbcWaitReady(最多等幾毫秒)`：等感應器開機完成
>
> 感應器就像一排有編號的「格子」，每一格放著不同的資料，例如第 `0x21` 格放著「8 顆雷射的距離」。

---

<h2 id="ex2">2️⃣ 8 顆雷射量到多遠？</h2>

**做什麼：** 把 8 顆雷射量到的距離排成一排印出來，左邊的印在左邊。`----` = 這個方向沒有東西，`XX` = 這一顆故障。

<a href="/examples/tof8/arduino/tof8_02_eight_distances/tof8_02_eight_distances.ino" download data-ignore="true">📄 下載 tof8_02_eight_distances.ino</a>

<!-- ino: examples/tof8/arduino/tof8_02_eight_distances/tof8_02_eight_distances.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 2：8 顆雷射量到多遠？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   測距者 8 路有 8 顆雷射，排成半圓形，從左邊一路看到右邊。
 *   這個範例把每一顆量到的距離（單位：毫米 mm，10 mm = 1 公分）印出來：
 *     數字  = 前面的東西有多遠
 *     ----  = 這個方向沒有東西（超過 120 公分）
 *     XX    = 這一顆雷射故障了
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   拿手或書本在感應器前面左右移動，看看是哪一顆的數字在變。
 *
 * 小提醒：
 *   第 0 顆在最左邊、第 7 顆在最右邊（從感應器後面、跟它看同一個方向時的左右）。
 *   第 3、4 顆都朝正前方。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint8_t LASERS = 8;                // 測距者 8 路有 8 顆雷射
const uint16_t NO_TARGET = 1200;         // 量到 1200 以上 = 這個方向沒有東西
const uint16_t FAULT = 0xFFFF;           // 收到 0xFFFF（65535）= 這一顆故障

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0，一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿手在感應器前面移動看看。"));
  Serial.println(F("（單位 mm；---- = 沒有東西，XX = 故障）"));
  Serial.println(F("   左                                       右"));
  Serial.println(F("     0     1     2     3     4     5     6     7"));
}

void loop() {
  // 第 0x21 號格子：16 個數字，每 2 個合成一顆雷射的距離（第 0 顆 → 第 7 顆）
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  for (uint8_t i = 0; i < LASERS; i++) {
    // 2 個數字合成一個距離：前面那個是「大的那一半」
    uint16_t mm = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];

    // 判斷的順序很重要：先看是不是故障，再看有沒有東西，最後才是距離
    if (mm == FAULT) {
      Serial.print(F("    XX"));          // 這一顆故障
    } else if (mm >= NO_TARGET) {
      Serial.print(F("  ----"));          // 這個方向沒有東西
    } else {
      printRight(mm, 6);                  // 印出距離，靠右對齊佔 6 格
    }
  }
  Serial.println();

  delay(300);                             // 👉 可以改：每 0.3 秒看一次
}

// 把數字靠右對齊印出來，總共佔 width 格（前面補空白）
void printRight(uint16_t value, uint8_t width) {
  uint8_t digits = 1;
  for (uint16_t v = value; v >= 10; v /= 10) digits++;   // 算這個數字有幾位數
  for (uint8_t k = digits; k < width; k++) Serial.print(' ');
  Serial.print(value);
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

**你會看到：**（手放在正前方 20 公分，右邊遠遠的有一面牆）

```text
準備好了！拿手在感應器前面移動看看。
（單位 mm；---- = 沒有東西，XX = 故障）
   左                                       右
     0     1     2     3     4     5     6     7
  ----  ----   432   215   221  ----  ----  1105
  ----  ----   430   213   224  ----  ----  1102
```

> [!TIP]
> **試試看：** 把手從左邊慢慢移到右邊，看數字是怎麼從第 0 顆「走」到第 7 顆的。第 3、4 顆都朝正前方，所以手在正前方時兩顆的數字會很接近。

---

<h2 id="ex3">3️⃣ 最近的東西在哪個方向？</h2>

**做什麼：** 感應器會自己比較 8 顆雷射，告訴你**最近的東西**在哪個方向、有多遠。電腦會畫一把小尺，用 `*` 標出位置，再用箭頭說方向。相撲機器人「找對手」用的就是這個。

**準備：** 一個盒子或你的手。

<a href="/examples/tof8/arduino/tof8_03_nearest_direction/tof8_03_nearest_direction.ino" download data-ignore="true">📄 下載 tof8_03_nearest_direction.ino</a>

<!-- ino: examples/tof8/arduino/tof8_03_nearest_direction/tof8_03_nearest_direction.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 3：最近的東西在哪個方向？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   感應器會自己比較 8 顆雷射，找出「離你最近的東西」在哪個方向、有多遠。
 *   電腦會畫一把小尺，用 * 標出東西的位置，再用箭頭告訴你方向：
 *     ←  左邊    ↖  左前方    ↑  正前方    ↗  右前方    →  右邊
 *   相撲機器人「找對手」、小車「追著手走」，用的就是這個。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把手放在感應器前面 30 公分左右，慢慢從左邊繞到右邊。
 *
 * 方位數字是什麼？
 *   -7（最左）~ 0（正前方）~ +7（最右）。負數 = 左邊，正數 = 右邊。
 *   東西剛好在兩顆雷射中間時，會出現兩顆中間的數字（例如 -6、-4）。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint16_t NO_TARGET = 1200;         // 距離 1200 以上 = 沒有東西

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0，一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把手放在感應器前面，從左邊慢慢移到右邊。"));
}

void loop() {
  // 第 0x11 號格子：6 個數字
  //   第 0 個：最近的東西在哪個方向（-7 左 ~ +7 右）
  //   第 1、2 個：最近的東西有多遠（mm）
  //   第 3 ~ 5 個：最遠（最空曠）的方向和距離，這個範例先不用
  uint8_t d[6];
  if (!mbcRead(0x11, d, 6)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  int8_t   dir = (int8_t)d[0];                         // 方向：要轉成「有正負號」的數字
  uint16_t mm  = ((uint16_t)d[1] << 8) | d[2];         // 2 個數字合成距離

  // 先確定「真的有東西」，再看方向。沒有東西時，方向的數字沒有意義
  if (mm >= NO_TARGET) {
    Serial.println(F("左 [...............] 右   前面 120 公分內沒有東西"));
    delay(300);
    return;
  }

  // ---- 1. 畫小尺：15 格代表 -7 ~ +7，東西在哪一格就畫 * ----
  Serial.print(F("左 ["));
  for (int8_t pos = -7; pos <= 7; pos++) {
    if (pos == dir)      Serial.print('*');            // 東西在這裡
    else if (pos == 0)   Serial.print('+');            // 正中間做個記號
    else                 Serial.print('.');
  }
  Serial.print(F("] 右   "));

  // ---- 2. 用箭頭說方向 ----
  if (dir <= -6)      Serial.print(F("← 左邊  "));     // 👉 可以改：分區的界線
  else if (dir <= -2) Serial.print(F("↖ 左前方"));
  else if (dir <= 1)  Serial.print(F("↑ 正前方"));
  else if (dir <= 5)  Serial.print(F("↗ 右前方"));
  else                Serial.print(F("→ 右邊  "));

  // ---- 3. 印距離（公分）和方位數字 ----
  Serial.print(F("  "));
  Serial.print(mm / 10);                               // mm 除以 10 = 公分
  Serial.print(F(" 公分（方位 "));
  Serial.print((int)dir);                              // 轉成 int 再印，才會印出數字
  Serial.println(F("）"));

  delay(300);                                          // 👉 可以改：每 0.3 秒看一次
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

**你會看到：**（手從左前方移到正前方，再移到右邊）

```text
左 [...............] 右   前面 120 公分內沒有東西
左 [..*....+.......] 右   ↖ 左前方  35 公分（方位 -5）
左 [.......*.......] 右   ↑ 正前方  28 公分（方位 0）
左 [.......+.....*.] 右   → 右邊    40 公分（方位 6）
```

**方位是什麼意思？**

| 方位數字 | 意思 |
| :---: | :--- |
| 0 | 東西在正前方 |
| 負數（-1 ~ -7） | 東西在**左**邊，數字越小越偏左（-7 = 正左邊） |
| 正數（+1 ~ +7） | 東西在**右**邊，數字越大越偏右（+7 = 正右邊） |

> [!WARNING]
> **一定要先看距離，再看方向。** 120 公分內什麼都沒有時，方位會變成 `-8` 或 `+8`（代表「上次看到的東西在哪一邊」），不是真的有東西在那裡。範例裡先用「距離小於 1200」確定有東西，才去看方向。

> [!TIP]
> **試試看：** 把箭頭那一段的分區界線改一改（例如 `dir <= -6` 改成 `dir <= -5`），看看箭頭什麼時候會變。

---

<h2 id="ex4">4️⃣ 倒車雷達警報燈</h2>

**做什麼：** 用 Arduino 板子上內建的小燈（標著 **L** 那顆）當警報燈，就像汽車的倒車雷達：沒有東西燈熄，有東西就閃，**越近閃越快**，貼到 10 公分內就一直亮。8 顆雷射任何一顆看到東西都算。

<a href="/examples/tof8/arduino/tof8_04_distance_alarm/tof8_04_distance_alarm.ino" download data-ignore="true">📄 下載 tof8_04_distance_alarm.ino</a>

<!-- ino: examples/tof8/arduino/tof8_04_distance_alarm/tof8_04_distance_alarm.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 4：倒車雷達警報燈
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用 Arduino 板子上內建的小燈（標著 L 的那顆）當警報燈，就像汽車的倒車雷達：
 *     前面 120 公分內沒有東西 → 燈熄
 *     有東西，越近閃得越快
 *     貼得很近（10 公分內）   → 燈一直亮
 *   8 顆雷射裡只要有任何一顆看到東西就算，左右兩邊靠近也會閃。
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

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0（會被當成貼著東西狂閃），一定要等
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！手慢慢靠近感應器，看 L 燈越閃越快。"));
}

void loop() {
  // ---- 1. 每 0.05 秒讀一次最近的距離 ----
  if (millis() - lastRead >= 50) {
    lastRead = millis();
    uint8_t d[6];
    if (mbcRead(0x11, d, 6)) {                         // 第 0x11 號格子：最近／最遠的方向和距離
      nearest = ((uint16_t)d[1] << 8) | d[2];          // 第 1、2 個數字 = 最近的距離（mm）
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
> **試試看：** 把 `ALARM_START` 改成 `500`，變成 50 公分內才開始閃；有蜂鳴器的話，把燈的腳位換成蜂鳴器的腳位，就是會「嗶嗶嗶」的倒車雷達。

---

<h2 id="ex5">5️⃣ 雷達長條圖</h2>

**做什麼：** 把 8 個方向的距離畫成 8 條長條：**長條越長 = 那個方向越空曠**，很短 = 有東西擋著。看一眼就知道哪邊可以走，就像一台小雷達。

**準備：** 幾本書當障礙物。

<a href="/examples/tof8/arduino/tof8_05_radar_bars/tof8_05_radar_bars.ino" download data-ignore="true">📄 下載 tof8_05_radar_bars.ino</a>

<!-- ino: examples/tof8/arduino/tof8_05_radar_bars/tof8_05_radar_bars.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 5：雷達長條圖
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把 8 顆雷射量到的距離畫成 8 條長條，一顆一條：
 *     長條越長 = 那個方向越空曠（東西越遠）
 *     長條很短 = 那個方向有東西擋著
 *   看一眼就知道「哪邊有障礙物、哪邊可以走」，就像一台小雷達。
 *
 * 接線（Arduino UNO / Nano）：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   把感應器放在桌上，用書本在前面、左邊、右邊擋擋看。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint8_t LASERS = 8;                // 8 顆雷射
const uint16_t NO_TARGET = 1200;         // 距離 1200 以上 = 沒有東西
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

// 每一顆雷射大約朝哪個角度看（負數 = 左邊、正數 = 右邊、0 = 正前方）
const int ANGLE[LASERS] = {-90, -60, -30, 0, 0, 30, 60, 90};

const uint16_t MM_PER_BLOCK = 50;        // 👉 可以改：一格 # 代表 50 mm（5 公分），1200 mm 剛好 24 格

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0，一定要等它準備好
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！用書本在感應器前面、左邊、右邊擋擋看。"));
}

void loop() {
  // 第 0x21 號格子：8 顆雷射的距離，每顆 2 個數字，共 16 個
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {
    Serial.println(F("讀取失敗，檢查一下線有沒有鬆掉。"));
    delay(500);
    return;
  }

  Serial.println();
  Serial.println(F("====== 雷達畫面（長條越長 = 越空曠）======"));

  for (uint8_t i = 0; i < LASERS; i++) {
    uint16_t mm = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];   // 2 個數字合成距離

    // ---- 1. 印「第幾顆、朝哪個角度」----
    Serial.print(i);
    if (ANGLE[i] > 0)       Serial.print(F("  +"));            // 右邊的角度前面加 +
    else if (ANGLE[i] == 0) Serial.print(F("   "));
    else                    Serial.print(F("  "));
    if (abs(ANGLE[i]) < 10) Serial.print(' ');                 // 對齊用
    Serial.print(ANGLE[i]);
    Serial.print(F("度 |"));

    // ---- 2. 畫長條 ----
    if (mm == FAULT) {                                         // 先看是不是故障
      Serial.println(F("XXXXXXXX  這顆故障"));
      continue;                                                // 這一顆畫完了，換下一顆
    }
    uint16_t blocks = mm / MM_PER_BLOCK;                       // 要畫幾格
    for (uint16_t k = 0; k < blocks; k++) Serial.print('#');

    // ---- 3. 長條後面寫距離 ----
    if (mm >= NO_TARGET) {
      Serial.println(F("> 沒有東西"));
    } else {
      Serial.print(F(" "));
      Serial.print(mm / 10);                                   // mm 除以 10 = 公分
      Serial.println(F(" 公分"));
    }
  }

  delay(500);                                                  // 👉 可以改：每 0.5 秒畫一次
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

**你會看到：**（正前方 30 公分有一本書）

```text
====== 雷達畫面（長條越長 = 越空曠）======
0  -90度 |########################> 沒有東西
1  -60度 |########################> 沒有東西
2  -30度 |############### 75 公分
3    0度 |###### 32 公分
4    0度 |###### 34 公分
5  +30度 |################ 80 公分
6  +60度 |###################### 110 公分
7  +90度 |########################> 沒有東西
```

> [!TIP]
> **試試看：** 把 `MM_PER_BLOCK` 改成 `100`，長條會變短一半，適合比較窄的螢幕。

---

<h2 id="ex6">6️⃣ 避障小車（進階）</h2>

**做什麼：** 讓兩輪小車自己到處跑、不撞牆。前面空空的就直走，旁邊太靠近牆就往另一邊修一點；前面有東西擋住，就原地轉向**比較空的那一邊**，直到前面空了再走。

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
1. 感應器裝在車頭正中間、朝前方。先跑 [範例 5](#ex5)，確認手放在車子**左邊**時，是上面幾條（第 0 ~ 2 顆）變短。
2. 把車子架高（輪子懸空）再上傳，確認兩個輪子都是**往前**轉。哪個輪子轉反了，就把 `LEFT_REVERSE` 或 `RIGHT_REVERSE` 改成 `true`。
3. 放到地上，3 秒後出發。

<a href="/examples/tof8/arduino/tof8_06_avoid_car/tof8_06_avoid_car.ino" download data-ignore="true">📄 下載 tof8_06_avoid_car.ino</a>

<!-- ino: examples/tof8/arduino/tof8_06_avoid_car/tof8_06_avoid_car.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 6（進階）：避障小車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車自己到處跑，不會撞到牆壁和障礙物：
 *     前面空空的        → 直走；旁邊太靠近牆，就往另一邊稍微修
 *     前面有東西擋住    → 停下來，原地轉向「比較空的那一邊」，直到前面空了再走
 *   8 顆雷射把左、前、右都看在眼裡，所以轉彎時知道該往哪邊轉。
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
 *   感應器裝在車頭正中間、朝前方，第 0 顆在車子左邊、第 7 顆在車子右邊。
 *
 * 第一次跑之前：
 *   1. 先跑「範例 5：雷達長條圖」，確認手放在車子左邊時，是上面幾條（第 0 ~ 2 顆）變短。
 *   2. 把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把下面的 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 */

#include <Wire.h>

const uint8_t SENSOR_ADDR = 0x29;        // 測距者感應器的門牌號碼
const uint16_t NO_TARGET = 1200;         // 距離 1200 以上 = 沒有東西
const uint16_t FAULT = 0xFFFF;           // 0xFFFF = 這一顆故障

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
const int      FORWARD_SPEED = 130;      // 直走的速度：0 ~ 255，先慢一點比較安全
const int      TURN_SPEED    = 110;      // 原地轉彎的速度
const int      STEER         = 40;       // 旁邊太靠近時，修正的力道
const uint16_t FRONT_STOP    = 250;      // 前面多近要轉彎（mm）：250 = 25 公分
const uint16_t SIDE_CLOSE    = 150;      // 旁邊多近算「太靠近」（mm）：150 = 15 公分
const bool     LEFT_REVERSE  = false;    // 左輪轉反了改成 true
const bool     RIGHT_REVERSE = false;    // 右輪轉反了改成 true

uint16_t dist[8];                        // 8 顆雷射的距離（mm）

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);

  // 8 顆雷射開機要 2 ~ 4 秒，這段時間距離都是 0，沒等好車子會以為四周都是牆
  if (!mbcWaitReady(6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}                      // 找不到感應器：不出發（輪子保持停止）
  }

  Serial.println(F("3 秒後出發！"));
  delay(3000);
}

void loop() {
  // 第 0x21 號格子：8 顆雷射的距離，一次讀回來
  uint8_t d[16];
  if (!mbcRead(0x21, d, 16)) {           // 讀不到感應器 → 為了安全，先停車
    drive(0, 0);
    delay(50);
    return;
  }

  bool allZero = true;                   // 8 顆是不是全部都是 0
  for (uint8_t i = 0; i < 8; i++) {
    dist[i] = ((uint16_t)d[2 * i] << 8) | d[2 * i + 1];
    if (dist[i] != 0) allZero = false;
    if (dist[i] == FAULT) dist[i] = 0;   // 故障的那一顆：當作「有東西擋著」，比較安全
  }
  if (allZero) {                         // 全部是 0：感應器可能剛重新開機（例如電池沒電），還沒準備好
    drive(0, 0);                         // → 先停車，等它準備好
    delay(50);
    return;
  }

  // 把 8 顆分成左、前、右三組，每組取「最近的那個」
  uint16_t left  = smallest(0, 2);       // 第 0 ~ 2 顆：左邊、左前方
  uint16_t front = smallest(3, 4);       // 第 3、4 顆：正前方
  uint16_t right = smallest(5, 7);       // 第 5 ~ 7 顆：右前方、右邊

  if (front > FRONT_STOP) {
    // ---- 前面空空的：直走，旁邊太靠近就往另一邊修一點 ----
    int turn = 0;                        // 正數 = 往左修，負數 = 往右修
    if (left < SIDE_CLOSE)  turn = turn - STEER;   // 左邊太近 → 往右修
    if (right < SIDE_CLOSE) turn = turn + STEER;   // 右邊太近 → 往左修
    drive(FORWARD_SPEED - turn, FORWARD_SPEED + turn);
  } else {
    // ---- 前面有東西：原地轉向比較空的那一邊 ----
    if (left > right) drive(-TURN_SPEED, TURN_SPEED);   // 左邊比較空 → 原地左轉
    else              drive(TURN_SPEED, -TURN_SPEED);   // 右邊比較空 → 原地右轉
  }

  delay(50);                             // 感應器大約 0.1 秒更新一次，每 0.05 秒看一次就夠了
}

// 從第 from 顆到第 to 顆，找出最近的距離
uint16_t smallest(uint8_t from, uint8_t to) {
  uint16_t m = NO_TARGET;
  for (uint8_t i = from; i <= to; i++) {
    if (dist[i] < m) m = dist[i];
  }
  return m;
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
| 來不及轉彎，撞上去 | `FRONT_STOP` 加大（例如 250 → 350），或 `FORWARD_SPEED` 調小 |
| 離牆還很遠就開始轉 | `FRONT_STOP` 減小（例如 250 → 180） |
| 沿著牆走時一直蹭到牆 | `SIDE_CLOSE` 或 `STEER` 加大 |
| 一直原地打轉不出發 | 有一個輪子轉反了，改 `LEFT_REVERSE` 或 `RIGHT_REVERSE`；或先跑範例 2 看是不是有一顆顯示 `XX` |

> [!NOTE]
> 為了安全，讀不到感應器、或 8 顆距離全部是 0（感應器剛重新開機、例如電池快沒電）時，車子會先停下來。故障的那一顆會被當成「有東西擋著」，所以車子不會往看不到的方向衝。

---

## 📂 其他範例（給會寫程式的人）

下面兩個是比較精簡的寫法，沒有逐行中文說明，適合已經會寫 Arduino、想直接拿去改的人：

| 範例 | 說明 |
| :--- | :--- |
| <a href="/examples/tof8/arduino/TOF8_Arduino_Wire_Example/TOF8_Arduino_Wire_Example.ino" download data-ignore="true">TOF8_Arduino_Wire_Example.ino</a> | 只用 `Wire.h`，一次印出 8 顆距離和最近／最遠方位 |
| <a href="/examples/tof8/arduino/TOF8_Arduino_Library_Example/TOF8_Arduino_Library_Example.ino" download data-ignore="true">TOF8_Arduino_Library_Example.ino</a>＋<a href="/examples/tof8/arduino/TOF8_Arduino_Library_Example/MBC_TOF8.h" download data-ignore="true">MBC_TOF8.h</a> | 用函式庫寫法（兩個檔案要放在同一個資料夾） |

> [!WARNING]
> `TOF8_Arduino_Wire_Example` 一開機就開始讀，前 2 ~ 4 秒會印出全部是 0 的距離，這是感應器還在開機，不是壞掉。

---

## ❓ 常見問題

**Q：一直出現「找不到感應器」？**
1. 檢查 SDA、SCL 有沒有接反（UNO：SDA → A4、SCL → A5）。
2. 檢查 VCC、GND 有沒有插緊。感應器上的燈有沒有亮？
3. 確認是 **通用 I2C 版**：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：一開機距離全部是 0？**
感應器還在把 8 顆雷射叫醒（約 2 ~ 4 秒），這段時間的資料都是 0。範例都已經用 `mbcWaitReady(6000)` 等好了；你自己寫程式時，也要先等第 `0x32` 格的第 0 個位元變成 1 再開始讀。

**Q：某一顆一直顯示 `XX`（故障）？**
先關掉電源、重新插好再試一次。還是一樣的話，跑 [範例 1](#ex1) 看是哪一顆沒開機成功，再 [聯絡我們](/contact.md) 協助檢修。

**Q：東西明明在前面，卻顯示「沒有東西」？**
測距者最遠量到 120 公分。另外，**黑色絨布、透明玻璃、亮面鏡子**這類東西反射的雷射光很少或很亂，比較難量到，換成白色或淺色的東西試試看。

**Q：多久讀一次比較好？**
每一顆雷射大約每 0.1 秒更新一次，感應器裡面已經幫你過濾掉跳動的數字。`loop` 裡每 0.05 ~ 0.1 秒讀一次就夠了，讀更快也只會拿到一樣的數字。

**Q：可以跟測距者 2 路，或市售的 VL53L0X 雷射模組接在一起嗎？**
不行。它們的門牌號碼都是 `0x29`，同一組 SDA/SCL 上只能接一個。可以跟其他門牌號碼不同的 MBC 產品（例如循行者 `0x16`）接在同一組線上。

**Q：想知道每一個格子放什麼資料？**
請看產品頁的 [I2C 暫存器說明](/sensors/tof8/index.md)（給會寫程式的人看的進階資料）。
