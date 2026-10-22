<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 神攝手 ESP32CAM × Arduino 範例

[🔙 回到神攝手首頁](/sensors/esp32cam/index.md)　｜　[📡 MBC 通用 I2C 協議 v2（進階）](/i2c-protocol.md)

> [!WARNING]
> **硬體已確認可以接 Arduino，I2C 版韌體還在上機測試中。** 這一頁的接線與範例是照 [MBC I2C 協議 v2](/i2c-protocol.md) 的定案寫好、三種板子都編譯過的；實機測完會把這段提示拿掉。想現在就用神攝手，請先用 [SPIKE](/sensors/esp32cam/spike-examples.md)、[EV3](/sensors/esp32cam/ev3-examples.md) 版。

這一頁有 **6 個範例**：學校版（取色、色塊追蹤）5 支，WRO 版（12 個色點）1 支。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/esp32cam/arduino/esp32cam_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 神攝手（產品端） | Arduino UNO / Nano | Arduino UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: |
| 5V | 5V | 5V | 5V（VIN） |
| GND | GND | GND | GND |
| IO3（= SDA） | A4 | SDA | GPIO 21 |
| IO1（= SCL） | A5 | SCL | GPIO 22 |

<div style="text-align: center;">
<img src="/images/figures/esp32cam/wiring-uno.svg" alt="神攝手 ESP32CAM 接 Arduino UNO 的接線圖" style="max-width: 520px; width: 100%; margin: 1rem auto; display: block;" />
</div>

**產品端 IO3／IO1 是什麼？** 神攝手的主機接頭用的是 ESP32 的 **IO3 和 IO1** 兩支腳（跟 EV3／SPIKE 串口版用同一組線）。燒成 **I2C 版韌體**時，IO3 當 SDA、IO1 當 SCL，I2C 位址 `0x4C`。所以：

> [!IMPORTANT]
> - 神攝手要燒 **I2C 版韌體**。EV3 版、SPIKE 版用同一組腳走串口，**只能二選一**，不能同時支援。
> - 硬體接線已確認；I2C 版韌體**上機待測**（見頁首提示）。
> - 神攝手要先在**背面螢幕**設定取樣點 S1 和要追的顏色（設定存在機身），Arduino 只負責讀結果。
> - 神攝手是 ESP32 做的從機，小幫手裡的 `delay(2)` **不能刪**。
> - 開機要幾秒（鏡頭初始化＋螢幕），範例最多等 8 秒。接線前先拔掉 USB 線。

---

<h2 id="cmds">📖 指令總覽（這台神攝手能讀到什麼）</h2>

神攝手就像一排有編號的「格子」，每一格放一個數字（int16）。寫 `[編號]` 再讀 2 個 byte 就拿到一個值；連續的編號可以一次讀一串（最多 16 個）。範例裡的 `mbcRead(ADDR, 編號)`、`mbcReadMany(ADDR, 起始編號, 幾個, 陣列)` 就是在做這件事。**學校版**用 `01`~`0A`，**WRO 版（HUE12）**用 `10`~`1B`、`20`~`2B`；`F0` 的高 byte 告訴你是哪一版。

<!-- gen:cmd-table -->
| 編號 | 名稱 | 範圍／單位 | 說明 | 出現在 |
| :---: | :--- | :--- | :--- | :--- |
| `01` | 色相（學校版） | −1~100 | 取樣點 S1 的色相：紅 0/100、黃約 17、綠約 33、青約 50、藍約 67、紫約 83；**−1 = 黑白灰**（飽和度 < 10）；S1 沒設定 = 0 | 範例 2、3 |
| `02` | 飽和度 | 0~100 | S1 的飽和度，越高越鮮豔 | 範例 2、3 |
| `03` | 明度 | 0~100 | S1 的明度，越高越亮 | 範例 2、3 |
| `04` | 動態 | 0~100 | 全畫面動態量，畫面變化越多越大 | 範例 2 |
| `05` | 色塊 X | 0~100 | 色塊中心 X，左 = 0；沒找到 = 0 | 範例 2、4、5 |
| `06` | 色塊 Y | 0~75 | 色塊中心 Y，**下 = 0、上 = 75**；沒找到 = 0 | 範例 2、4 |
| `07` | 色塊寬 | 0~100 | 0 = 沒找到 | 範例 2、4 |
| `08` | 色塊高 | 0~75 | 沒找到 = 0 | 範例 2、4 |
| `09` | 色塊面積 | 0~7500 | 寬 × 高 | 範例 2、4 |
| `0A` | 有找到色塊 | 0～1 | 1 = 畫面裡有目標色的色塊（寬 > 0） | 範例 2、4、5 |
| `10`~`1B` | 色點 S1~S12 顏色碼（WRO 版） | 0~255 | 0~239 彩色（色相環縮成 0~239）、240~245 白、246~250 黑、255 = 色點沒開 | 範例 6 |
| `20`~`2B` | 色點 S1~S12 鮮豔度（WRO 版） | 0~255 | 黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 | 範例 6 |
| `F0` | 裝置 ID | — | 低 byte = 出廠位址 0x4C、高 byte = **1 學校版／2 WRO 版** | 範例 1、6 |
| `F1` | 協議版本 | 2 | 這份範例照版本 2 寫 | 範例 1 |
| `F2` | 韌體版本 | 主×100＋次 | 例如 102 = 1.2 | 範例 1 |
| `F3` | 狀態旗標 | bit | b0 就緒、b1 有找到色塊（學校版；WRO 版恆 0） | 範例 1、所有範例的 mbcWaitReady |
| `F5` | 開機秒數 | 0~32767 | 數字變小 = 重開機過 | 範例 1 |
<!-- /gen -->

> [!NOTE]
> 神攝手**沒有寫入命令**（目標顏色、補光燈都在機身螢幕設定）。沒有的編號（學校版讀 `10`~`2B`、WRO 版讀 `01`~`0A`）會讀到 `-32768`（範例裡叫 `MBC_NONE`），讀不到神攝手也是 `-32768`。完整規格見 [MBC 通用 I2C 協議 v2](/i2c-protocol.md)。

---

## 📋 範例總覽

| # | 範例 | 版本 | 學會什麼 | 另外要準備 |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [跟神攝手打招呼](#ex1) | 兩版 | 確認接線正確、讀身分證、分辨學校版／WRO 版 | 無 |
| 2 | [把所有的值一次印出來](#ex2) | 學校版 | 看懂 01~0A 每個編號放什麼 | 幾張彩色紙 |
| 3 | [這是什麼顏色？](#ex3) | 學校版 | 色相／飽和／明度 → 顏色名字 | 幾張彩色紙 |
| 4 | [色塊在哪裡？](#ex4) | 學校版 | X／Y／寬／高／面積／有找到 | 目標顏色的物品 |
| 5 | [伺服馬達跟著色塊轉（進階）](#ex5) | 學校版 | 用 X 控制角度；UNO 與 ESP32 兩種伺服寫法 | SG90 伺服馬達 |
| 6 | [WRO 版：12 個色點一次看](#ex6) | WRO 版 | 一次讀 12 個顏色碼＋12 個鮮豔度 | WRO 場地 |

> [!TIP]
> **建議順序：1 → 2 → 3 → 4 → 5。** 燒 WRO 版韌體的神攝手跑 1 → 6。

---

<h2 id="ex1">1️⃣ 跟神攝手打招呼</h2>

**做什麼：** 確認神攝手接好了，印出它的「身分證」（出廠位址、學校版還是 WRO 版、協議版本、韌體版本）和狀態旗標。每次接新的神攝手，都先跑這一個。

**步驟：**
1. 照上面的表格接好 4 條線，USB 插上電腦（神攝手螢幕會亮）。
2. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/esp32cam/arduino/esp32cam_01_hello/esp32cam_01_hello.ino" download data-ignore="true">📄 下載 esp32cam_01_hello.ino</a>

<!-- ino: examples/esp32cam/arduino/esp32cam_01_hello/esp32cam_01_hello.ino -->
```cpp
/*
 * 神攝手 ESP32CAM × Arduino　範例 1：跟神攝手打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認神攝手有沒有接好。接好了，會印出它的身分證（裝置 ID、是學校版還是 WRO 版、
 *   協議版本、韌體版本）和狀態旗標 F3（就緒？有沒有找到色塊？）。
 *   每次接新的神攝手，都建議先跑這個範例。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   神攝手開機要幾秒（鏡頭初始化＋螢幕），範例最多等 8 秒。
 *   這支範例學校版、WRO 版都能跑；F0 的高 byte 會告訴你是哪一版。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟神攝手講話

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);

  Serial.println(F("=== 神攝手 ESP32CAM：跟神攝手打招呼 ==="));

  // 等神攝手說「我準備好了」（狀態旗標 F3 的 bit0），最多等 8 秒
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手！請檢查："));
    Serial.println(F("  1. IO3（SDA）接 A4、IO1（SCL）接 A5（ESP32：21 / 22），有沒有接反？"));
    Serial.println(F("  2. 5V、GND 有沒有插緊？神攝手的螢幕有沒有亮？"));
    Serial.println(F("  3. 神攝手燒的是「I2C 版」韌體嗎？（EV3 版、SPIKE 版用同一組腳，不能接 Arduino）"));
    while (true) {}                      // 找不到就停在這裡
  }

  // ---- F0 裝置 ID：低 byte = 出廠位址（0x4C）、高 byte = 版本（1 = 學校版、2 = WRO 版） ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t edition = (uint8_t)((id >> 8) & 0xFF);
  Serial.print(F("找到了！出廠位址 0x"));
  Serial.println((uint8_t)(id & 0xFF), HEX);   // 應該是 4C
  Serial.print(F("版本："));
  if (edition == 1)      Serial.println(F("學校版（取色＋色塊追蹤＋動態偵測，編號 01~0A）"));
  else if (edition == 2) Serial.println(F("WRO 版 HUE12（12 個色點，編號 10~1B、20~2B）"));
  else                   Serial.println(F("不認識的版本"));

  // ---- F1 協議版本（這份範例是照版本 2 寫的） ----
  Serial.print(F("協議版本："));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 韌體版本：主版×100 + 次版（例如 102 = 1.2） ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("韌體版本："));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 狀態旗標 ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("目前狀態："));
  Serial.print(F("  bit0 就緒        ")); Serial.println((st & 0x01) ? F("是") : F("否"));
  Serial.print(F("  bit1 有找到色塊  ")); Serial.println((st & 0x02) ? F("是") : F("否（學校版才有；WRO 版恆 0）"));

  // ---- F5 開機秒數 ----
  Serial.print(F("開機秒數："));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("接線正確，可以開始玩其他範例了！"));
}

void loop() {
  // 這個範例只要在開機時檢查一次，所以 loop 裡什麼都不做
}

// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
//   沒有這個編號（空號）時感應器回 -32768（MBC_NONE）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
// UNO/Nano：SDA=A4、SCL=A5；UNO R4：SDA/SCL 腳；ESP32：預設 GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從位址 addr 的感應器，由編號 id 起連續讀 k 個值（1~16）放進 out。全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte，Wire 緩衝區上限）
  Wire.beginTransmission(addr);
  Wire.write(id);                        // 告訴感應器：我要從第 id 號開始讀
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等 2 ms 再讀（ESP32 型的產品一定要等）
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // 先來的是低位元組
    uint16_t hi = Wire.read();           // 再來高位元組（小端）
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值。讀不到（沒接好）回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]。p 指向參數、n = 參數幾個 byte（沒參數就不填）。成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（狀態旗標 F3 的 bit0 = 1），最多等 ms 毫秒。準備好了回傳 true
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**你會看到：**（學校版，鏡頭前有目標色）

```text
=== 神攝手 ESP32CAM：跟神攝手打招呼 ===
找到了！出廠位址 0x4C
版本：學校版（取色＋色塊追蹤＋動態偵測，編號 01~0A）
協議版本：2
韌體版本：1.2
目前狀態：
  bit0 就緒        是
  bit1 有找到色塊  是
開機秒數：6
接線正確，可以開始玩其他範例了！
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟神攝手「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(ADDR, 編號)`：讀一個值（讀不到回 `-32768`）
> - `mbcReadMany(ADDR, 起始編號, 幾個, 陣列)`：從某個編號起一次讀一串（最多 16 個，同一瞬間的值）
> - `mbcWaitReady(ADDR, 毫秒)`：等神攝手開機好
>
> 小幫手裡的 `delay(2)`（寫完編號等 2 毫秒再讀）**不能刪**：神攝手是 ESP32 做的從機，需要這 2 毫秒準備資料。

---

<h2 id="ex2">2️⃣ 把所有的值一次印出來（學校版）</h2>

**做什麼：** 每 0.5 秒把學校版值表 `01`~`0A` 的 10 個值一次讀出來印出來。拿不同顏色的東西到鏡頭前，看哪一行在變。

<a href="/examples/esp32cam/arduino/esp32cam_02_all_values/esp32cam_02_all_values.ino" download data-ignore="true">📄 下載 esp32cam_02_all_values.ino</a>

<!-- ino: examples/esp32cam/arduino/esp32cam_02_all_values/esp32cam_02_all_values.ino -->
```cpp
/*
 * 神攝手 ESP32CAM × Arduino　範例 2：把所有的值一次印出來（學校版）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.5 秒把學校版「值表」01~0A 的 10 個值一次讀出來印出來：
 *     01 色相、02 飽和度、03 明度、04 動態（取樣點 S1 的顏色＋全畫面動態量）
 *     05 色塊 X、06 色塊 Y、07 寬、08 高、09 面積、0A 有找到
 *   連續的編號可以「一次讀一串」（mbcReadMany），讀到的是同一瞬間的值。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 準備：
 *   先在神攝手背面的螢幕設定好取樣點 S1 和要追的顏色（設定存在機身）。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。拿不同顏色的東西到鏡頭前，看哪些數字在變。
 *
 * 小提醒：
 *   這支是「學校版」韌體用的（編號 01~0A）。WRO 版（HUE12）請看範例 6。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;             // 上一次印的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！每 0.5 秒印一次所有的值。"));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 可以改：每 0.5 秒印一次
    lastPrint = millis();

    // 編號 01~0A，一次讀 10 個值
    int16_t a[10];
    if (!mbcReadMany(ADDR, 0x01, 10, a)) { Serial.println(F("讀不到神攝手")); return; }

    Serial.println(F("------------------------------"));
    Serial.print(F("01 色相 -1~100（-1 = 黑白灰）：")); Serial.println(a[0]);
    Serial.print(F("02 飽和度 0~100              ：")); Serial.println(a[1]);
    Serial.print(F("03 明度 0~100                ：")); Serial.println(a[2]);
    Serial.print(F("04 動態 0~100                ：")); Serial.println(a[3]);
    Serial.print(F("05 色塊 X 0~100（左 = 0）    ：")); Serial.println(a[4]);
    Serial.print(F("06 色塊 Y 0~75（下 = 0）     ：")); Serial.println(a[5]);
    Serial.print(F("07 色塊寬 0~100              ：")); Serial.println(a[6]);
    Serial.print(F("08 色塊高 0~75               ：")); Serial.println(a[7]);
    Serial.print(F("09 色塊面積（寬×高）         ：")); Serial.println(a[8]);
    Serial.print(F("0A 有找到色塊（1 = 有）      ：")); Serial.println(a[9]);
  }
  delay(50);
}

// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
//   沒有這個編號（空號）時感應器回 -32768（MBC_NONE）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
// UNO/Nano：SDA=A4、SCL=A5；UNO R4：SDA/SCL 腳；ESP32：預設 GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從位址 addr 的感應器，由編號 id 起連續讀 k 個值（1~16）放進 out。全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte，Wire 緩衝區上限）
  Wire.beginTransmission(addr);
  Wire.write(id);                        // 告訴感應器：我要從第 id 號開始讀
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等 2 ms 再讀（ESP32 型的產品一定要等）
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // 先來的是低位元組
    uint16_t hi = Wire.read();           // 再來高位元組（小端）
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值。讀不到（沒接好）回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]。p 指向參數、n = 參數幾個 byte（沒參數就不填）。成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（狀態旗標 F3 的 bit0 = 1），最多等 ms 毫秒。準備好了回傳 true
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**你會看到：**（取樣點對著綠色紙，畫面右上有一塊目標色）

```text
準備好了！每 0.5 秒印一次所有的值。
------------------------------
01 色相 -1~100（-1 = 黑白灰）：34
02 飽和度 0~100              ：72
03 明度 0~100                ：65
04 動態 0~100                ：3
05 色塊 X 0~100（左 = 0）    ：78
06 色塊 Y 0~75（下 = 0）     ：60
07 色塊寬 0~100              ：22
08 色塊高 0~75               ：18
09 色塊面積（寬×高）         ：396
0A 有找到色塊（1 = 有）      ：1
```

---

<h2 id="ex3">3️⃣ 這是什麼顏色？</h2>

**做什麼：** 讀取樣點 S1 的 `01` 色相、`02` 飽和度、`03` 明度（一次讀 3 個），用中文說顏色名字：色相 −1 時用明度分黑／灰／白。顏色有變才印。

<a href="/examples/esp32cam/arduino/esp32cam_03_color_name/esp32cam_03_color_name.ino" download data-ignore="true">📄 下載 esp32cam_03_color_name.ino</a>

<!-- ino: examples/esp32cam/arduino/esp32cam_03_color_name/esp32cam_03_color_name.ino -->
```cpp
/*
 * 神攝手 ESP32CAM × Arduino　範例 3：這是什麼顏色？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀取樣點 S1 的顏色（編號 01~03，一次讀 3 個），用中文說出顏色名字：
 *     01 色相 0~100：紅 0/100、黃約 17、綠約 33、青約 50、藍約 67、紫約 83；
 *        -1 = 黑白灰（飽和度太低，沒有顏色）
 *     02 飽和度 0~100：越高顏色越鮮豔
 *     03 明度 0~100：越高越亮（黑白灰時用它分黑／灰／白）
 *   顏色有變才印，畫面不會洗版。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 準備：
 *   先在神攝手背面的螢幕設定好取樣點 S1 和要追的顏色（設定存在機身）。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。拿不同顏色的紙到取樣點前面。
 *
 * 小提醒：
 *   這支是「學校版」韌體用的（編號 01~0A）。WRO 版（HUE12）請看範例 6。
 *   分界的數字（例如黃 12~23）可以依你的場地光線調整。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

int16_t lastHue = -99, lastSat = -1, lastVal = -1;   // 上一次的值（故意用不可能的數字，第一次一定會印）

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿東西到取樣點前面。"));
}

void loop() {
  // 一次讀編號 01~03 共 3 個值：[0] 色相 [1] 飽和度 [2] 明度
  int16_t c[3];
  if (!mbcReadMany(ADDR, 0x01, 3, c)) { delay(100); return; }

  // 變化夠大才印（色相差 3 以上、或飽和／明度差 10 以上）
  if (abs(c[0] - lastHue) >= 3 || abs(c[1] - lastSat) >= 10 || abs(c[2] - lastVal) >= 10 || (c[0] < 0) != (lastHue < 0)) {
    lastHue = c[0]; lastSat = c[1]; lastVal = c[2];
    Serial.print(F("顏色："));
    if (c[0] < 0) {                      // 色相 -1：沒有顏色，用明度分黑／灰／白
      if (c[2] < 30)      Serial.print(F("黑"));
      else if (c[2] < 70) Serial.print(F("灰"));
      else                Serial.print(F("白"));
    } else {
      printHueName(c[0]);
    }
    Serial.print(F("   （色相 ")); Serial.print(c[0]);
    Serial.print(F("  飽和 ")); Serial.print(c[1]);
    Serial.print(F("  明度 ")); Serial.print(c[2]); Serial.println(F("）"));
  }
  delay(50);
}

// 把色相 0~100 變成顏色名字（紅 0/100、黃約 17、綠約 33、青約 50、藍約 67、紫約 83）
void printHueName(int16_t hue) {
  if (hue < 0)        Serial.print(F("黑白灰（沒有顏色）"));
  else if (hue < 6)   Serial.print(F("紅"));
  else if (hue < 12)  Serial.print(F("橘"));
  else if (hue < 23)  Serial.print(F("黃"));
  else if (hue < 42)  Serial.print(F("綠"));
  else if (hue < 58)  Serial.print(F("青"));
  else if (hue < 75)  Serial.print(F("藍"));
  else if (hue < 92)  Serial.print(F("紫"));
  else                Serial.print(F("紅"));
}

// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
//   沒有這個編號（空號）時感應器回 -32768（MBC_NONE）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
// UNO/Nano：SDA=A4、SCL=A5；UNO R4：SDA/SCL 腳；ESP32：預設 GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從位址 addr 的感應器，由編號 id 起連續讀 k 個值（1~16）放進 out。全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte，Wire 緩衝區上限）
  Wire.beginTransmission(addr);
  Wire.write(id);                        // 告訴感應器：我要從第 id 號開始讀
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等 2 ms 再讀（ESP32 型的產品一定要等）
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // 先來的是低位元組
    uint16_t hi = Wire.read();           // 再來高位元組（小端）
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值。讀不到（沒接好）回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]。p 指向參數、n = 參數幾個 byte（沒參數就不填）。成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（狀態旗標 F3 的 bit0 = 1），最多等 ms 毫秒。準備好了回傳 true
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**你會看到：**（依序拿紅、綠、白紙到取樣點前）

```text
準備好了！拿東西到取樣點前面。
顏色：紅   （色相 2  飽和 80  明度 70）
顏色：綠   （色相 34  飽和 72  明度 65）
顏色：白   （色相 -1  飽和 4  明度 95）
```

> [!TIP]
> 分界的數字（例如黃 12~23）在程式最下面的 `printHueName`，可以依你的場地光線調整。

---

<h2 id="ex4">4️⃣ 色塊在哪裡？</h2>

**做什麼：** 每 0.2 秒讀 `05`~`0A`（一次讀 6 個）：X、Y、寬、高、面積、有找到。用中文說色塊在「左／中／右」「上／中／下」，還有大小。沒找到時 X、Y、寬、高、面積都是 0。

<a href="/examples/esp32cam/arduino/esp32cam_04_blob_position/esp32cam_04_blob_position.ino" download data-ignore="true">📄 下載 esp32cam_04_blob_position.ino</a>

<!-- ino: examples/esp32cam/arduino/esp32cam_04_blob_position/esp32cam_04_blob_position.ino -->
```cpp
/*
 * 神攝手 ESP32CAM × Arduino　範例 4：色塊在哪裡？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀色塊的位置與大小（編號 05~0A，一次讀 6 個）：
 *     05 X 0~100（左 = 0、右 = 100）、06 Y 0~75（下 = 0、上 = 75）
 *     07 寬、08 高、09 面積（寬×高）、0A 有找到（1 = 畫面裡有目標色）
 *   每 0.2 秒用中文說色塊在「左／中／右」「上／中／下」，還有大小。
 *   沒找到時 X、Y、寬、高、面積都是 0。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 準備：
 *   先在神攝手背面的螢幕設定好取樣點 S1 和要追的顏色（設定存在機身）。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。拿目標顏色的東西在鏡頭前左右、上下、遠近移動。
 *
 * 小提醒：
 *   這支是「學校版」韌體用的（編號 01~0A）。WRO 版（HUE12）請看範例 6。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿目標顏色的東西到鏡頭前。"));
}

void loop() {
  if (millis() - lastPrint >= 200) {     // 👉 可以改：每 0.2 秒印一次
    lastPrint = millis();

    // 一次讀編號 05~0A 共 6 個值：[0] X [1] Y [2] 寬 [3] 高 [4] 面積 [5] 有找到
    int16_t b[6];
    if (!mbcReadMany(ADDR, 0x05, 6, b)) { Serial.println(F("讀不到神攝手")); return; }

    if (!b[5]) {                         // 0A = 0：畫面裡沒有目標色
      Serial.println(F("沒有找到色塊"));
      return;
    }

    // X：0~100 分成左／中／右；Y：0~75 分成下／中／上
    Serial.print(F("色塊在 "));
    if (b[0] < 35)      Serial.print(F("左"));
    else if (b[0] > 65) Serial.print(F("右"));
    else                Serial.print(F("中"));
    if (b[1] < 25)      Serial.print(F("下"));
    else if (b[1] > 50) Serial.print(F("上"));
    else                Serial.print(F("中"));

    Serial.print(F("  X=")); Serial.print(b[0]);
    Serial.print(F(" Y=")); Serial.print(b[1]);
    Serial.print(F("  寬 ")); Serial.print(b[2]);
    Serial.print(F(" 高 ")); Serial.print(b[3]);
    Serial.print(F(" 面積 ")); Serial.print(b[4]);
    if (b[4] > 2000)     Serial.print(F("（很近／很大）"));
    else if (b[4] < 100) Serial.print(F("（很遠／很小）"));
    Serial.println();
  }
  delay(50);
}

// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
//   沒有這個編號（空號）時感應器回 -32768（MBC_NONE）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
// UNO/Nano：SDA=A4、SCL=A5；UNO R4：SDA/SCL 腳；ESP32：預設 GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從位址 addr 的感應器，由編號 id 起連續讀 k 個值（1~16）放進 out。全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte，Wire 緩衝區上限）
  Wire.beginTransmission(addr);
  Wire.write(id);                        // 告訴感應器：我要從第 id 號開始讀
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等 2 ms 再讀（ESP32 型的產品一定要等）
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // 先來的是低位元組
    uint16_t hi = Wire.read();           // 再來高位元組（小端）
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值。讀不到（沒接好）回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]。p 指向參數、n = 參數幾個 byte（沒參數就不填）。成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（狀態旗標 F3 的 bit0 = 1），最多等 ms 毫秒。準備好了回傳 true
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**你會看到：**（目標物從右上移到正中間、靠近，然後拿走）

```text
準備好了！拿目標顏色的東西到鏡頭前。
色塊在 右上  X=78 Y=60  寬 22 高 18 面積 396
色塊在 中中  X=50 Y=38  寬 60 高 45 面積 2700（很近／很大）
沒有找到色塊
```

> [!NOTE]
> Y 是「下 = 0、上 = 75」，跟一般螢幕座標相反。

---

<h2 id="ex5">5️⃣ 伺服馬達跟著色塊轉（進階）</h2>

**做什麼：** 把神攝手固定在 SG90 伺服馬達上，色塊往哪邊跑，馬達就往哪邊轉：`05` 色塊 X 0~100 → 20~160 度；沒找到就回 90 度。**不用安裝任何函式庫**：UNO／Nano／R4 自己每 20 ms 送一個脈衝，ESP32 用內建的 ledc 送 50 Hz 訊號（程式用 `#if defined(ESP32)` 分開）。

**另外要準備：** SG90 伺服馬達 1 顆。

**伺服馬達接線：**

| SG90 的線 | Arduino UNO / Nano / R4 | ESP32 |
| :---: | :---: | :---: |
| 棕色 | GND | GND |
| 紅色 | 5V | 5V（VIN） |
| 橘色（訊號） | 9 | GPIO 13 |

<a href="/examples/esp32cam/arduino/esp32cam_05_track_servo/esp32cam_05_track_servo.ino" download data-ignore="true">📄 下載 esp32cam_05_track_servo.ino</a>

<!-- ino: examples/esp32cam/arduino/esp32cam_05_track_servo/esp32cam_05_track_servo.ino -->
```cpp
/*
 * 神攝手 ESP32CAM × Arduino　範例 5：伺服馬達跟著色塊轉（進階）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把神攝手裝在 SG90 伺服馬達上，色塊往哪邊跑，馬達就往哪邊轉：
 *     讀編號 05 色塊 X（0~100）→ 換算成角度，送給伺服馬達。
 *     沒找到色塊（0A = 0）就回到 90 度中間。
 *   不用安裝任何函式庫：UNO／Nano／UNO R4 自己每 20 ms 送一個脈衝；ESP32 用內建 ledc 送 50 Hz 訊號。
 *
 * 你需要：神攝手 ESP32CAM（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。另外要：SG90 伺服馬達 1 顆。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *   SG90 棕線 → GND    紅線 → 5V    橘線（訊號）→ 9
 *   （ESP32 的訊號線請接 GPIO 13）
 *
 * 準備：
 *   先在神攝手背面的螢幕設定好取樣點 S1 和要追的顏色（設定存在機身）。
 *   把神攝手固定在伺服馬達的擺臂上，鏡頭朝前。
 *
 * 怎麼看結果：
 *   拿目標顏色的東西左右移動，伺服馬達會跟著轉。序列埠監控窗（115200）會印角度。
 *
 * 小提醒：
 *   這支是「學校版」韌體用的（編號 01~0A）。WRO 版（HUE12）請看範例 6。
 *   轉的方向跟色塊相反的話，把 REVERSE 改成 true。
 *   伺服馬達一直抖或 Arduino 一直重開：USB 電不夠，紅線、棕線改接外接電池（4~6V），負極也要接 GND。
 */

#include <Wire.h>

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

#if defined(ESP32)
const int SERVO_PIN = 13;                // ESP32：訊號線接 GPIO 13
#else
const int SERVO_PIN = 9;                 // UNO / Nano / R4：訊號線接第 9 腳
#endif

// ---- 👉 可以改的參數 ----
const bool REVERSE   = false;            // 轉的方向反了就改 true
const int  MIN_ANGLE = 20;               // 色塊在最左時的角度
const int  MAX_ANGLE = 160;              // 色塊在最右時的角度

int angle = 90;
unsigned long lastRead = 0;
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  servoBegin();                          // 準備伺服馬達（UNO 自己送脈衝、ESP32 用 ledc）
  servoWrite(90);                        // 先轉到中間
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿目標顏色的東西左右移動。"));
}

void loop() {
  // ---- 1. 每 0.1 秒讀一次神攝手：編號 05~0A 共 6 個值：[0] X … [5] 有找到 ----
  if (millis() - lastRead >= 100) {
    lastRead = millis();
    int16_t b[6];
    if (!mbcReadMany(ADDR, 0x05, 6, b) || !b[5]) {
      angle = 90;                        // 讀不到、或沒找到色塊 → 回中間
    } else {
      int x = constrain(b[0], 0, 100);
      angle = REVERSE ? map(x, 0, 100, MAX_ANGLE, MIN_ANGLE) : map(x, 0, 100, MIN_ANGLE, MAX_ANGLE);
    }
  }

  // ---- 2. 每一圈都送訊號給伺服馬達（UNO 要每 20 ms 送一次脈衝，它才會停在那個角度） ----
  servoWrite(angle);

  if (millis() - lastPrint >= 300) {     // 每 0.3 秒印一次角度
    lastPrint = millis();
    Serial.print(F("角度：")); Serial.print(angle); Serial.println(F(" 度"));
  }
  delay(20);                             // 這支不是 50 ms：伺服馬達的脈衝要每 20 ms 一次
}

// ---- 伺服馬達：兩種板子兩種做法，呼叫方式一樣 ----
//   0 度 ≈ 0.544 ms、90 度 ≈ 1.47 ms、180 度 ≈ 2.4 ms 高電位，每 20 ms 一次
#if defined(ESP32)
// ESP32 用內建的 ledc 送 50 Hz（20 ms 一次）PWM，設定一次就會自己一直送
void servoBegin() {
  ledcAttach(SERVO_PIN, 50, 16);         // 50 Hz、16 位解析度（0~65535）
}
void servoWrite(int deg) {
  deg = constrain(deg, 0, 180);
  long us = map(deg, 0, 180, 544, 2400); // 角度 → 微秒
  ledcWrite(SERVO_PIN, (uint32_t)(us * 65535L / 20000L));   // 20000 微秒 = 一個週期
}
#else
// UNO / Nano / R4 不用函式庫：自己把腳拉高「幾微秒」再拉低，就是一個脈衝
void servoBegin() { pinMode(SERVO_PIN, OUTPUT); }
void servoWrite(int deg) {
  deg = constrain(deg, 0, 180);
  int us = map(deg, 0, 180, 544, 2400);  // 角度 → 微秒
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(us);
  digitalWrite(SERVO_PIN, LOW);
}
#endif

// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
//   沒有這個編號（空號）時感應器回 -32768（MBC_NONE）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
// UNO/Nano：SDA=A4、SCL=A5；UNO R4：SDA/SCL 腳；ESP32：預設 GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從位址 addr 的感應器，由編號 id 起連續讀 k 個值（1~16）放進 out。全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte，Wire 緩衝區上限）
  Wire.beginTransmission(addr);
  Wire.write(id);                        // 告訴感應器：我要從第 id 號開始讀
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等 2 ms 再讀（ESP32 型的產品一定要等）
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // 先來的是低位元組
    uint16_t hi = Wire.read();           // 再來高位元組（小端）
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值。讀不到（沒接好）回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]。p 指向參數、n = 參數幾個 byte（沒參數就不填）。成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（狀態旗標 F3 的 bit0 = 1），最多等 ms 毫秒。準備好了回傳 true
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**你會看到：**（目標物從左邊移到右邊，然後拿走）

```text
準備好了！拿目標顏色的東西左右移動。
角度：34 度
角度：90 度
角度：146 度
角度：90 度
```

> [!TIP]
> - 轉的方向跟色塊相反：把 `REVERSE` 改成 `true`。
> - 伺服馬達一直抖、或 Arduino 一直重開機：USB 的電不夠，紅線、棕線改接外接電池（4~6 V），負極也要接 Arduino GND。
> - 這支範例 loop 末是 `delay(20)` 不是 50：UNO 的伺服脈衝要每 20 ms 送一次。

---

<h2 id="ex6">6️⃣ WRO 版：12 個色點一次看</h2>

**做什麼：** 給燒 **WRO 版（HUE12）韌體**的神攝手用：每 0.5 秒把 12 個色點 S1~S12 印成 12 格。`10`~`1B` 顏色碼一次讀 12 個、`20`~`2B` 鮮豔度一次讀 12 個，每格印顏色名字＋顏色碼＋鮮豔度。

<a href="/examples/esp32cam/arduino/esp32cam_06_wro_12_spots/esp32cam_06_wro_12_spots.ino" download data-ignore="true">📄 下載 esp32cam_06_wro_12_spots.ino</a>

<!-- ino: examples/esp32cam/arduino/esp32cam_06_wro_12_spots/esp32cam_06_wro_12_spots.ino -->
```cpp
/*
 * 神攝手 ESP32CAM × Arduino　範例 6：WRO 版：12 個色點一次看
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   這支是給「WRO 版（HUE12）」韌體用的：畫面上 12 個色點 S1~S12，每 0.5 秒印成 12 格。
 *     10~1B：S1~S12 顏色碼（一次讀 12 個）
 *        0~239 彩色（色相環縮成 0~239）、240~245 白、246~250 黑、255 = 這個色點沒開
 *     20~2B：S1~S12 鮮豔度 0~255（一次讀 12 個；黑 = 1、沒開 = 0）
 *   每一格印出顏色名字＋顏色碼＋鮮豔度。
 *
 * 你需要：神攝手 ESP32CAM（WRO 版 HUE12 的 I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   神攝手 5V  → 5V       神攝手 GND → GND
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 硬體已確認可接，I2C 版韌體上機待測。IO1/IO3 跟 EV3／SPIKE 串口版共用，韌體二選一。
 *
 * 準備：
 *   先在神攝手背面的螢幕開啟要用的色點 S1~S12，並對準場地。
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   學校版韌體讀 10~2B 會全部是 -32768（空號），請改跑範例 2~5。
 *   顏色碼 255 只代表「色點沒開」；色點有開但沒對到東西時，仍會回該點目前的顏色。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x4C;               // 神攝手的「門牌號碼」（I2C 位址），學校版與 WRO 版都是 0x4C
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {
    Serial.println(F("找不到神攝手，請先跑範例 1 檢查接線。"));
    while (true) {}
  }

  // 確認是 WRO 版（F0 高 byte = 2）；學校版也能跑，只是會讀到空號
  int16_t id = mbcRead(ADDR, 0xF0);
  if (((id >> 8) & 0xFF) != 2) Serial.println(F("注意：這台不是 WRO 版韌體，10~2B 會是空號（-32768）。"));
  Serial.println(F("準備好了！每 0.5 秒印一次 12 個色點。"));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 可以改：每 0.5 秒印一次
    lastPrint = millis();

    int16_t code[12], vivid[12];
    if (!mbcReadMany(ADDR, 0x10, 12, code))  { Serial.println(F("讀不到（顏色碼）")); return; }   // 10~1B
    if (!mbcReadMany(ADDR, 0x20, 12, vivid)) { Serial.println(F("讀不到（鮮豔度）")); return; }   // 20~2B

    Serial.println(F("---- S1 ~ S12 ----"));
    for (uint8_t i = 0; i < 12; i++) {
      Serial.print(F("S")); Serial.print(i + 1); Serial.print(i < 9 ? F("  ") : F(" "));
      printCodeName(code[i]);
      Serial.print(F("  碼 ")); Serial.print(code[i]);
      Serial.print(F("  鮮豔 ")); Serial.println(vivid[i]);
    }
  }
  delay(50);
}

// 顏色碼 → 名字：0~239 彩色（把 0~239 當成色相環 0~360 度）、240~245 白、246~250 黑、255 沒開
void printCodeName(int16_t c) {
  if (c == MBC_NONE)   { Serial.print(F("（空號）")); return; }
  if (c == 255)        { Serial.print(F("沒開    ")); return; }
  if (c >= 246)        { Serial.print(F("黑      ")); return; }
  if (c >= 240)        { Serial.print(F("白      ")); return; }
  int deg = (int)((long)c * 360 / 240); // 0~239 → 0~359 度
  if (deg < 15)        Serial.print(F("紅      "));
  else if (deg < 45)   Serial.print(F("橘      "));
  else if (deg < 75)   Serial.print(F("黃      "));
  else if (deg < 165)  Serial.print(F("綠      "));
  else if (deg < 200)  Serial.print(F("青      "));
  else if (deg < 265)  Serial.print(F("藍      "));
  else if (deg < 330)  Serial.print(F("紫      "));
  else                 Serial.print(F("紅      "));
}

// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）：負責跟感應器講話。
// 所有 MBC 範例都一樣，不用改，也不用看懂。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
//   沒有這個編號（空號）時感應器回 -32768（MBC_NONE）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）
// UNO/Nano：SDA=A4、SCL=A5；UNO R4：SDA/SCL 腳；ESP32：預設 GPIO21(SDA)/22(SCL)
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時，不會讓 Arduino 當機卡住
#endif
}

// 從位址 addr 的感應器，由編號 id 起連續讀 k 個值（1~16）放進 out。全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte，Wire 緩衝區上限）
  Wire.beginTransmission(addr);
  Wire.write(id);                        // 告訴感應器：我要從第 id 號開始讀
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 等 2 ms 再讀（ESP32 型的產品一定要等）
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();           // 先來的是低位元組
    uint16_t hi = Wire.read();           // 再來高位元組（小端）
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值。讀不到（沒接好）回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]。p 指向參數、n = 參數幾個 byte（沒參數就不填）。成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（狀態旗標 F3 的 bit0 = 1），最多等 ms 毫秒。準備好了回傳 true
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}
```

**你會看到：**（開了 S1~S3，其他沒開）

```text
準備好了！每 0.5 秒印一次 12 個色點。
---- S1 ~ S12 ----
S1  紅        碼 3  鮮豔 210
S2  綠        碼 80  鮮豔 180
S3  白        碼 243  鮮豔 12
S4  沒開      碼 255  鮮豔 0
S5  沒開      碼 255  鮮豔 0
S6  沒開      碼 255  鮮豔 0
S7  沒開      碼 255  鮮豔 0
S8  沒開      碼 255  鮮豔 0
S9  沒開      碼 255  鮮豔 0
S10 沒開      碼 255  鮮豔 0
S11 沒開      碼 255  鮮豔 0
S12 沒開      碼 255  鮮豔 0
```

> [!NOTE]
> - 顏色碼 `255` 只代表「色點沒開」；色點有開但沒對到東西時，仍會回那一點目前的顏色。
> - 學校版韌體跑這支會讀到 `-32768`（空號），程式開頭會提醒。

---

## ❓ 常見問題

**Q：一直出現「找不到神攝手」？**
1. 檢查 IO3（SDA）、IO1（SCL）有沒有接反（UNO：IO3 → A4、IO1 → A5；R4：SDA／SCL；ESP32：21／22）。
2. 檢查 5V、GND 有沒有插緊。神攝手的螢幕有沒有亮？
3. 神攝手燒的是 **I2C 版韌體**嗎？EV3 版、SPIKE 版走串口，接 Arduino 讀不到。
4. 神攝手開機要幾秒，範例會等 8 秒；還是不行就按 Arduino 的 RESET 鍵重來。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：色相一直是 0？**
取樣點 S1 還沒設定（在神攝手背面螢幕設定，存在機身）；或者取樣點對著紅色（紅 = 0 也 = 100）。

**Q：色塊 X、Y 都是 0？**
畫面裡沒有目標顏色（`0A` = 0）。先在機身螢幕設定要追的顏色與 HSV 容許範圍，看螢幕上有沒有框住色塊。

**Q：讀到 `-32768` 是什麼意思？**
這是「沒有這個值」的代號（範例裡叫 `MBC_NONE`）：學校版讀 `10`~`2B`、WRO 版讀 `01`~`0A` 都會是這個；讀不到神攝手（線鬆了）也是。寫程式時先檢查 `v != MBC_NONE` 再用。

**Q：小幫手裡的 `delay(2)` 可以拿掉嗎？**
不行。神攝手是 ESP32 做的從機，它不能把 I2C 時脈拉住等資料，主機寫完編號後要等 2 毫秒它才準備好。拿掉會讀到上一次的資料。

**Q：ESP32 當主機時要注意什麼？**
一樣要 `delay(2)`；接線用 GPIO 21（SDA）／22（SCL）。範例 5 的伺服馬達在 ESP32 用 `ledcAttach`／`ledcWrite`（ESP32 核心 3.x），訊號線接 GPIO 13。

**Q：想知道每一個格子放什麼資料？**
請看上面的 [指令總覽](#cmds) 和 [MBC 通用 I2C 協議 v2](/i2c-protocol.md)（給會寫程式的人看的進階資料）。
