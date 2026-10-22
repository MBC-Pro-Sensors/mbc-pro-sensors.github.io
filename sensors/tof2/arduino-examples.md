<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 測距者 2 路 × Arduino 範例

[🔙 回到 2 路雷射測距首頁](/sensors/tof2/index.md)　｜　[📡 MBC 通用 I2C 協議（進階）](/i2c-protocol.md)

這一頁有 **6 個產品範例＋3 個共用範例**，從「確認接好了沒」一路做到「用 mm 精準停車」。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/tof2/arduino/tof2_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
<a href="/examples/i2c/arduino/i2c_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #5b6cff, #2a2f8f); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載共用命令範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 感應器 | Arduino UNO / Nano | ESP32 | Arduino UNO R4 |
| :---: | :---: | :---: | :---: |
| VCC | 5V | 5V（VIN） | 5V |
| GND | GND | GND | GND |
| SDA | A4 | GPIO 21 | SDA |
| SCL | A5 | GPIO 22 | SCL |

圖：`/images/figures/tof2/wiring-uno.svg`

> [!IMPORTANT]
> - 請用 **通用 I2C 版** 的測距者 2 路。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 兩顆雷射開機初始化要 1～3 秒，每個範例一開始都會先等 `F3` 就緒。
> - 「左、右」是**感應器裝在車頭（雷射朝前）、你站在車子後面往前看**的左右。
> - 接線前先拔掉 USB 線。

---

<h2 id="cmd-table">📖 指令總覽表（協議 v2）</h2>

感應器像一排有編號的「格子」，每一格放一個 16 位元的數字（int16，小端）。讀法：寫 `[編號]` → 等 2 ms → 讀 2×k 個 byte，就能從那個編號起一次拿到連續 k 格（k ≤ 16）。沒有的編號會回 `-32768`。門牌號碼（I2C 位址）是 `0x29`。

<!-- gen:cmd-table -->
| 編號 | 名稱 | 範圍 / 單位 | 說明 | 出現在 |
| :---: | :--- | :--- | :--- | :--- |
| `01` | 左距離 | 0～120 cm | 左邊那顆的距離；120 = 前面沒東西／那顆失效 | [範例 2](#ex2)、[3](#ex3) |
| `02` | 右距離 | 0～120 cm | 右邊那顆的距離；120 = 沒東西／失效 | [範例 2](#ex2)、[3](#ex3) |
| `03` | 較近距離 | 0～120 cm | 左右兩顆取較近的 | [範例 3](#ex3)、[5](#ex5) |
| `04` | 左距離 | 0～1200 mm | 左邊那顆，精細到 1 mm；1200 = 沒東西／失效 | [範例 2](#ex2)、[4](#ex4) |
| `05` | 右距離 | 0～1200 mm | 右邊那顆，精細到 1 mm | [範例 2](#ex2)、[4](#ex4) |
| `06` | 初始化位元圖 | 0～3（bit） | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功；3 = 兩顆都好 | [範例 2](#ex2)、[6](#ex6) |
| `07` | 故障位元圖 | 0～3（bit） | bit0 = 左、bit1 = 右，1 = 那顆目前失效（讀失敗或 200 ms 沒更新） | [範例 2](#ex2)、[6](#ex6) |
| `08` | 左右平均 | 0～1200 mm | (左 mm + 右 mm) / 2；跟 EV3 模式 3、SPIKE 模式 6 一致 | [範例 2](#ex2)、[4](#ex4) |
| `F0` | 裝置 ID | — | 低 byte = 出廠位址 0x29、高 byte = 2（雷射顆數） | [範例 1](#ex1)、[共用 2](#exi2)、[共用 3](#exi3) |
| `F1` | 協議版本 | 2 | 這份範例照版本 2 寫 | [範例 1](#ex1) |
| `F2` | 韌體版本 | 主×100＋次 | 例 102 = 1.2 | [範例 1](#ex1) |
| `F3` | 狀態旗標 | bit | b0 就緒（兩顆都初始化完）、b1 任一顆失效 | [範例 1](#ex1)、[6](#ex6) |
| `F4` | 更新序號 | 0～255 | 每 10 ms +1，一直跳 = 感應器活著 | [共用 2](#exi2) |
| `F5` | 開機秒數 | 0～32767 秒 | 變小 = 重開機過 | [範例 1](#ex1)、[共用 2](#exi2) |
| `F8` | 換 I2C 位址（命令） | `[新位址][取反]` | `[F8][00][FF]` = 回出廠；重開機後生效 | [共用 1](#exi1) |
| `EF` | 重新開機（命令） | `[A5]` | 送 `[EF][A5]` | [共用 1](#exi1)、[共用 2](#exi2) |
<!-- /gen -->

測距者 2 路沒有產品專用的寫入命令（E0～EE）。

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟感應器打招呼](#ex1) | 確認接線正確，讀 F0～F3 身分證與狀態 | 無 |
| 2 | [一次看完所有讀值](#ex2) | 整張值表 01～08 每一格是什麼 | 你的手 |
| 3 | [左邊近還是右邊近？](#ex3) | 01／02／03 三格，畫長條 | 一個盒子 |
| 4 | [用 mm 精準停車（進階）](#ex4) | 04／05／08 mm，到 200 mm 剛好停 | 兩輪小車、L298N 馬達驅動板 |
| 5 | [倒車雷達警報](#ex5) | 用 03 較近距離控制燈與蜂鳴器 | 蜂鳴器（可不接） |
| 6 | [雷射健康檢查](#ex6) | 06／07 初始化與故障位元圖、F3 | 無 |
| 共 1 | [換 I2C 位址](#exi1) | F8 換位址／回出廠、EF 重開機 | 無 |
| 共 2 | [身分證＋重新開機](#exi2) | F0～F5 六格共用暫存器 | 無 |
| 共 3 | [掃描線上裝置](#exi3) | 掃 0x08～0x77、用 F0 認產品 | 無 |

> [!TIP]
> **建議順序：1 → 2 → 3 → 6 → 5 → 4。** 先確認接好（1），看懂每一格（2），再做應用。

---

<h2 id="ex1">1️⃣ 跟感應器打招呼</h2>

**做什麼：** 確認感應器接好了，並讀出它的身分證：`F0` 裝置 ID（出廠位址＋雷射顆數）、`F1` 協議版本、`F2` 韌體版本、`F3` 狀態旗標逐 bit 用中文印出來。每次接新的感應器，都先跑這一個。

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
 *   確認感應器有沒有接好。接好了，會印出它的身分證（裝置 ID、協議版本、
 *   韌體版本）和目前狀態（兩顆雷射是不是都正常）。
 *   每次接新感應器，都建議先跑這個範例。
 *
 * 你需要：測距者 2 路（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟感應器講話

const uint8_t ADDR = 0x29;               // 測距者的「門牌號碼」（I2C 位址），2 路與 8 路都是 0x29
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等感應器開機 0.5 秒

  Serial.println(F("=== 測距者 2 路：跟感應器打招呼 ==="));
  Serial.println(F("等兩顆雷射開機（最多 4 秒）..."));

  // 等感應器說「我準備好了」（狀態旗標 F3 的 bit0 = 兩顆都初始化完），最多等 4 秒
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器！請檢查："));
    Serial.println(F("  1. SDA 接 A4、SCL 接 A5（ESP32：21 / 22），有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？"));
    Serial.println(F("  3. 你的感應器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    while (true) {}                      // 找不到就停在這裡
  }

  // ---- F0 裝置 ID：低 byte = 出廠位址、高 byte = 雷射顆數 ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t factoryAddr = (uint8_t)(id & 0xFF);        // 出廠位址（換過位址也不變）
  uint8_t sensors     = (uint8_t)((id >> 8) & 0xFF); // hw[0] = 幾顆雷射
  Serial.print(F("找到了！出廠位址 0x"));
  Serial.print(factoryAddr, HEX);
  Serial.print(F("，雷射數量 "));
  Serial.print(sensors);                 // 測距者 2 路 = 2
  Serial.println(F(" 顆"));

  // ---- F1 協議版本（這份範例是照版本 2 寫的） ----
  Serial.print(F("協議版本："));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 韌體版本：主版×100 + 次版（例如 102 = 1.2） ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("韌體版本："));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 狀態旗標：一個 bit 代表一件事 ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("目前狀態："));
  Serial.print(F("  bit0 就緒        ")); Serial.println((st & 0x01) ? F("是（兩顆都初始化完）") : F("否"));
  Serial.print(F("  bit1 任一顆失效  ")); Serial.println((st & 0x02) ? F("是！（看範例 6 查哪一顆）") : F("否（兩顆都正常）"));

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

**你會看到：**

```text
=== 測距者 2 路：跟感應器打招呼 ===
等兩顆雷射開機（最多 4 秒）...
找到了！出廠位址 0x29，雷射數量 2 顆
協議版本：2
韌體版本：2.0
目前狀態：
  bit0 就緒        是（兩顆都初始化完）
  bit1 任一顆失效  否（兩顆都正常）
開機秒數：5
接線正確，可以開始玩其他範例了！
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟感應器「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(位址, 編號)`：讀一格，讀不到回 `MBC_NONE`（−32768）
> - `mbcReadMany(位址, 起始編號, 幾格, 陣列)`：從起始編號一次讀連續幾格（最多 16），全部來自同一瞬間
> - `mbcWrite(位址, 命令, 參數, 參數幾個)`：送命令，沒參數時後兩個可以不寫
> - `mbcWaitReady(位址, 最多等幾毫秒)`：等 `F3` bit0 就緒

---

<h2 id="ex2">2️⃣ 一次看完所有讀值</h2>

**做什麼：** 每 0.5 秒用一次 `mbcReadMany` 把 `01`～`08` 整張值表印出來。先在這裡認識每一格，之後的範例只是各挑幾格來用。

<a href="/examples/tof2/arduino/tof2_02_all_values/tof2_02_all_values.ino" download data-ignore="true">📄 下載 tof2_02_all_values.ino</a>

<!-- ino: examples/tof2/arduino/tof2_02_all_values/tof2_02_all_values.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 2：一次看完所有讀值
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器「整張值表」每 0.5 秒印一次：
 *     01 左 cm、02 右 cm、03 較近 cm、04 左 mm、05 右 mm、
 *     06 初始化位元圖、07 故障位元圖、08 左右平均 mm。
 *   之後的範例只是各挑其中幾格來用，先在這裡把每一格認識一遍。
 *
 * 你需要：測距者 2 路（通用 I2C 版）＋ 你的手或一個盒子。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。拿手在左邊、右邊的雷射前面移動。
 *
 * 小提醒：
 *   01~08 編號連續，一次讀 8 個值（mbcReadMany），保證是同一瞬間的值。
 *   120 cm／1200 mm = 前面沒東西（或那顆失效）。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿手在左右兩顆雷射前面移動看看。"));
}

void loop() {
  int16_t a[8];                          // 放 01~08 這 8 格
  if (!mbcReadMany(ADDR, 0x01, 8, a)) { Serial.println(F("讀不到")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 左距離     ")); Serial.print(a[0]); Serial.println(F(" cm   （0~120，120 = 沒東西）"));
  Serial.print(F("02 右距離     ")); Serial.print(a[1]); Serial.println(F(" cm"));
  Serial.print(F("03 較近距離   ")); Serial.print(a[2]); Serial.println(F(" cm   （左右取比較近的那個）"));
  Serial.print(F("04 左距離     ")); Serial.print(a[3]); Serial.println(F(" mm   （0~1200，比 cm 精細 10 倍）"));
  Serial.print(F("05 右距離     ")); Serial.print(a[4]); Serial.println(F(" mm"));
  Serial.print(F("06 初始化位元圖 ")); Serial.print(a[5]); Serial.println(F("     （bit0 左、bit1 右；3 = 兩顆都開機成功）"));
  Serial.print(F("07 故障位元圖   ")); Serial.print(a[6]); Serial.println(F("     （bit0 左、bit1 右；0 = 都正常）"));
  Serial.print(F("08 左右平均   ")); Serial.print(a[7]); Serial.println(F(" mm   （(左 mm + 右 mm) / 2）"));

  delay(50);                             // 讀完值歇一下
  delay(450);                            // 每 0.5 秒更新一次，比較好看
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

**你會看到：**（手放在左邊 30 公分，右邊沒東西）

```text
---------------------------------------
01 左距離     30 cm   （0~120，120 = 沒東西）
02 右距離     120 cm
03 較近距離   30 cm   （左右取比較近的那個）
04 左距離     304 mm   （0~1200，比 cm 精細 10 倍）
05 右距離     1200 mm
06 初始化位元圖 3     （bit0 左、bit1 右；3 = 兩顆都開機成功）
07 故障位元圖   0     （bit0 左、bit1 右；0 = 都正常）
08 左右平均   752 mm   （(左 mm + 右 mm) / 2）
```

> [!TIP]
> **試試看：** 用一張紙斜著放在兩顆雷射前面，看 `04` 和 `05` 差多少——這就是測「牆壁斜不斜」的方法。

---

<h2 id="ex3">3️⃣ 左邊近還是右邊近？</h2>

**做什麼：** 讀 `01` 左 cm、`02` 右 cm、`03` 較近 cm 三格，把兩顆的距離畫成兩條長條，並說出東西在哪一邊比較近。

**準備：** 一個盒子或你的手。

<a href="/examples/tof2/arduino/tof2_03_left_right/tof2_03_left_right.ino" download data-ignore="true">📄 下載 tof2_03_left_right.ino</a>

<!-- ino: examples/tof2/arduino/tof2_03_left_right/tof2_03_left_right.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 3：左邊近還是右邊近？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀「01 左 cm」「02 右 cm」「03 較近 cm」三格，把兩顆雷射的距離畫成兩條長條，
 *   並告訴你東西在哪一邊比較近。這是避障、找對手最常用的三格。
 *
 * 你需要：測距者 2 路（通用 I2C 版）＋ 一個盒子或你的手。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。拿手從左邊移到右邊，看長條怎麼變。
 *
 * 左、右怎麼算？
 *   感應器裝在車頭（雷射朝前），你站在車子後面往前看的左、右。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿手從左邊移到右邊看看。（長條越長 = 越遠）"));
}

void loop() {
  int16_t v[3];                          // v[0] = 01 左 cm、v[1] = 02 右 cm、v[2] = 03 較近 cm
  if (!mbcReadMany(ADDR, 0x01, 3, v)) { Serial.println(F("讀不到感應器")); delay(500); return; }
  int16_t left = v[0], right = v[1], nearest = v[2];

  Serial.print(F("左 "));
  printBar(left);
  Serial.print(F("  右 "));
  printBar(right);

  if (nearest >= 120)        Serial.println(F("  → 120 公分內沒有東西"));
  else if (left < right)     { Serial.print(F("  → 左邊比較近：")); Serial.print(nearest); Serial.println(F(" 公分")); }
  else if (right < left)     { Serial.print(F("  → 右邊比較近：")); Serial.print(nearest); Serial.println(F(" 公分")); }
  else                       { Serial.print(F("  → 正前方："));     Serial.print(nearest); Serial.println(F(" 公分")); }

  delay(50);
  delay(150);                            // 每 0.2 秒印一行   👉 可以改
}

// 把 0~120 公分畫成 0~12 格的長條
void printBar(int16_t cm) {
  int8_t n = cm / 10;
  if (n > 12) n = 12;
  Serial.print('[');
  for (int8_t i = 0; i < 12; i++) Serial.print(i < n ? '#' : '.');
  Serial.print(']');
  if (cm < 10)  Serial.print(' ');
  if (cm < 100) Serial.print(' ');
  Serial.print(cm);
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

**你會看到：**（手從左邊移到右邊）

```text
左 [###.........] 35  右 [############]120  → 左邊比較近：35 公分
左 [###.........] 32  右 [###.........] 32  → 正前方：32 公分
左 [############]120  右 [####........] 41  → 右邊比較近：41 公分
左 [############]120  右 [############]120  → 120 公分內沒有東西
```

---

<h2 id="ex4">4️⃣ 用 mm 精準停車（進階）</h2>

**做什麼：** 用 `04` 左 mm、`05` 右 mm、`08` 平均 mm（精細到 1 mm），讓小車往牆開，到 `STOP_MM`（預設 200 mm）剛好停下；接近時自動減速。沒有小車也可以跑，只看序列埠數字。

**另外要準備：** 兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池、一面牆。

**馬達接線：**

| L298N | Arduino UNO / Nano / R4 | ESP32 | 控制 |
| :---: | :---: | :---: | :--- |
| ENA | 5 | GPIO 25 | 左輪速度 |
| IN1 / IN2 | 7 / 8 | GPIO 26 / 27 | 左輪方向 |
| ENB | 6 | GPIO 14 | 右輪速度 |
| IN3 / IN4 | 9 / 10 | GPIO 32 / 33 | 右輪方向 |
| GND | GND | GND | **一定要跟 Arduino 共地** |

<a href="/examples/tof2/arduino/tof2_04_mm_precise/tof2_04_mm_precise.ino" download data-ignore="true">📄 下載 tof2_04_mm_precise.ino</a>

<!-- ino: examples/tof2/arduino/tof2_04_mm_precise/tof2_04_mm_precise.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 4（進階）：用 mm 精準停車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用「04 左 mm」「05 右 mm」「08 左右平均 mm」三格（精細到 1 mm），
 *   讓小車往前開，到距離牆壁 STOP_MM（預設 200 mm = 20 公分）時剛好停下。
 *   接近時會自動減速，所以不會衝過頭。沒有小車也可以跑：只看序列埠的數字。
 *
 * 你需要：兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等）、電池、一面牆或盒子。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   （ESP32 改接：ENA → 25、IN1 → 26、IN2 → 27、ENB → 14、IN3 → 32、IN4 → 33）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！感應器裝在車頭，雷射朝前。
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。車子停下時印出的距離應該很接近 STOP_MM。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

// ---- 馬達接腳 ----
#if defined(ESP32)
const int ENA = 25, IN1 = 26, IN2 = 27;  // 左輪（ESP32 的 6~11 腳不能用）
const int ENB = 14, IN3 = 32, IN4 = 33;  // 右輪
#else
const int ENA = 5, IN1 = 7, IN2 = 8;     // 左輪
const int ENB = 6, IN3 = 9, IN4 = 10;    // 右輪
#endif

// ---- 👉 可以改的參數 ----
const int16_t STOP_MM    = 200;          // 停在離牆幾 mm（200 = 20 公分）
const int16_t SLOW_MM    = 500;          // 離牆幾 mm 以內開始減速
const int     FAST_SPEED = 150;          // 全速 0~255
const int     SLOW_SPEED = 70;           // 最慢的爬行速度（太慢馬達會不動）
const bool    LEFT_REVERSE  = false;     // 左輪轉反了改成 true
const bool    RIGHT_REVERSE = false;     // 右輪轉反了改成 true

bool stopped = false;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.print(F("3 秒後出發，目標：停在 ")); Serial.print(STOP_MM); Serial.println(F(" mm"));
  delay(3000);
}

void loop() {
  int16_t v[5];                          // 04 左 mm、05 右 mm、06、07、08 平均 mm
  if (!mbcReadMany(ADDR, 0x04, 5, v)) {  // 從 04 號起一次讀 5 格（04~08）
    drive(0, 0);                         // 讀不到 → 安全停車
    delay(50);
    return;
  }
  int16_t leftMm = v[0], rightMm = v[1], avgMm = v[4];
  int16_t dist = (leftMm < rightMm) ? leftMm : rightMm;   // 用比較近的那顆，避免斜著撞到

  Serial.print(F("左 ")); Serial.print(leftMm);
  Serial.print(F(" mm  右 ")); Serial.print(rightMm);
  Serial.print(F(" mm  平均 ")); Serial.print(avgMm);
  Serial.print(F(" mm  "));

  if (stopped || dist <= STOP_MM) {
    drive(0, 0);
    if (!stopped) { stopped = true; Serial.println(F("→ 到了，停車！")); }
    else Serial.println(F("（已停）"));
  } else if (dist <= SLOW_MM) {
    // 越近越慢：距離從 SLOW_MM 到 STOP_MM，速度從 FAST 降到 SLOW
    int speed = map(dist, STOP_MM, SLOW_MM, SLOW_SPEED, FAST_SPEED);
    drive(speed, speed);
    Serial.print(F("→ 減速 ")); Serial.println(speed);
  } else {
    drive(FAST_SPEED, FAST_SPEED);
    Serial.println(F("→ 全速前進"));
  }
  delay(50);
}

void drive(int left, int right) {
  if (LEFT_REVERSE)  left  = -left;
  if (RIGHT_REVERSE) right = -right;
  setMotor(ENA, IN1, IN2, left);
  setMotor(ENB, IN3, IN4, right);
}

void setMotor(int en, int inA, int inB, int speed) {
  speed = constrain(speed, -255, 255);
  digitalWrite(inA, speed >= 0 ? HIGH : LOW);
  digitalWrite(inB, speed >= 0 ? LOW : HIGH);
  analogWrite(en, abs(speed));
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

**你會看到：**

```text
3 秒後出發，目標：停在 200 mm
左 812 mm  右 805 mm  平均 808 mm  → 全速前進
左 486 mm  右 480 mm  平均 483 mm  → 減速 139
左 231 mm  右 226 mm  平均 228 mm  → 減速 77
左 199 mm  右 196 mm  平均 197 mm  → 到了，停車！
```

> [!TIP]
> 停得太遠就把 `SLOW_SPEED` 調大、太近（撞到）就把 `SLOW_MM` 調大。程式用「比較近的那顆」判斷，車子斜著靠近牆也不會撞到。

---

<h2 id="ex5">5️⃣ 倒車雷達警報</h2>

**做什麼：** 讀 `03` 較近距離，用板子內建的小燈（**L**）和蜂鳴器當倒車雷達：沒有東西安靜，有東西就閃／嗶，**越近越急**，貼到 10 公分內就一直亮。

**準備：** 有源蜂鳴器（可不接）：`+` → 8（ESP32 → 27）、`−` → GND。

<a href="/examples/tof2/arduino/tof2_05_distance_alarm/tof2_05_distance_alarm.ino" download data-ignore="true">📄 下載 tof2_05_distance_alarm.ino</a>

<!-- ino: examples/tof2/arduino/tof2_05_distance_alarm/tof2_05_distance_alarm.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 5：倒車雷達警報
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用板子內建的小燈（L）和一顆蜂鳴器（可不接）當倒車雷達：
 *   讀「03 較近距離 cm」，沒有東西就安靜，有東西就閃／嗶，越近越急，
 *   貼到 STOP_CM 以內就一直亮、一直響。
 *
 * 你需要：測距者 2 路（通用 I2C 版）；蜂鳴器可有可無（有的話接「有源蜂鳴器」）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   蜂鳴器 + → 8（ESP32 → 27）   蜂鳴器 − → GND
 *
 * 怎麼看結果：
 *   手慢慢靠近感應器，L 燈（和蜂鳴器）越閃越快；序列埠 115200 也會印距離。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // ESP32 開發板的內建燈通常接在 GPIO 2
#endif

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

#if defined(ESP32)
const int BUZZER = 27;                   // 蜂鳴器腳位（ESP32）
#else
const int BUZZER = 8;                    // 蜂鳴器腳位（UNO）
#endif

// ---- 👉 可以改的參數 ----
const int16_t ALARM_START_CM = 80;       // 幾公分以內開始警報
const int16_t STOP_CM        = 10;       // 幾公分以內一直亮、一直響

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 4000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！手慢慢靠近感應器，看 L 燈越閃越快。"));
}

void loop() {
  int16_t cm = mbcRead(ADDR, 0x03);      // 03 = 左右兩顆取較近的距離（cm）
  if (cm == MBC_NONE) { delay(100); return; }

  if (millis() - lastPrint >= 500) {     // 每 0.5 秒印一次距離
    lastPrint = millis();
    if (cm >= 120) Serial.println(F("沒有東西"));
    else { Serial.print(F("最近的東西：")); Serial.print(cm); Serial.println(F(" 公分")); }
  }

  if (cm >= ALARM_START_CM) {            // 太遠：安靜
    alarm(false);
    delay(50);
  } else if (cm <= STOP_CM) {            // 太近：一直亮
    alarm(true);
    delay(50);
  } else {                               // 中間：越近閃越快（間隔 50 ~ 400 ms）
    int gap = map(cm, STOP_CM, ALARM_START_CM, 50, 400);
    alarm(true);  delay(gap / 2);
    alarm(false); delay(gap / 2);
  }
}

void alarm(bool on) {
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
  digitalWrite(BUZZER, on ? HIGH : LOW);
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

**你會看到：**（手慢慢靠近）

```text
準備好了！手慢慢靠近感應器，看 L 燈越閃越快。
沒有東西
最近的東西：65 公分
最近的東西：30 公分
最近的東西：8 公分
```

---

<h2 id="ex6">6️⃣ 雷射健康檢查</h2>

**做什麼：** 讀 `06` 初始化位元圖、`07` 故障位元圖和 `F3`，每秒報告兩顆雷射是不是都健康。失效那顆的距離會固定回 120／1200，看起來像「沒東西」，所以要靠這兩格分辨。比賽前跑一下。

<a href="/examples/tof2/arduino/tof2_06_fault_check/tof2_06_fault_check.ino" download data-ignore="true">📄 下載 tof2_06_fault_check.ino</a>

<!-- ino: examples/tof2/arduino/tof2_06_fault_check/tof2_06_fault_check.ino -->
```cpp
/*
 * 測距者 2 路 × Arduino　範例 6：雷射健康檢查
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀「06 初始化位元圖」「07 故障位元圖」和「F3 狀態旗標」，
 *   每秒報告兩顆雷射是不是都健康。比賽前跑一下，就知道感應器有沒有壞。
 *     06：bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不會重試，要重新上電）
 *     07：bit0 = 左、bit1 = 右，1 = 那顆現在失效（讀不到或 200 ms 沒更新）
 *   失效那顆的距離會固定回 120 cm／1200 mm，看起來像「沒東西」，所以要靠這兩格分辨。
 *
 * 你需要：測距者 2 路（通用 I2C 版）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== 測距者 2 路：雷射健康檢查 ==="));
  // 這個範例故意不等就緒，開機失敗時也看得到 06 哪一顆沒起來
  delay(3000);                           // 兩顆雷射開機最多約 3 秒
}

void loop() {
  int16_t v[2];                          // v[0] = 06 初始化、v[1] = 07 故障
  int16_t st = mbcRead(ADDR, 0xF3);
  if (!mbcReadMany(ADDR, 0x06, 2, v) || st == MBC_NONE) {
    Serial.println(F("整個感應器都讀不到：檢查接線，或它是不是 SPIKE／EV3 版"));
    delay(1000);
    return;
  }
  int16_t initOk = v[0], fault = v[1];

  Serial.println(F("---------------------------"));
  Serial.print(F("F3 就緒 "));       Serial.print((st & 0x01) ? F("是") : F("否"));
  Serial.print(F("  任一顆失效 "));  Serial.println((st & 0x02) ? F("是") : F("否"));

  reportOne(F("左"), initOk & 0x01, fault & 0x01);
  reportOne(F("右"), initOk & 0x02, fault & 0x02);

  if (initOk == 3 && fault == 0) Serial.println(F("兩顆雷射都健康！"));
  else Serial.println(F("有問題：先重新上電；還是一樣就檢查那顆雷射有沒有被遮住或接頭鬆了"));

  delay(50);
  delay(950);
}

// 印一顆雷射的狀況
void reportOne(const __FlashStringHelper *name, int16_t ok, int16_t bad) {
  Serial.print(F("  "));
  Serial.print(name);
  Serial.print(F(" 顆：初始化 "));
  Serial.print(ok ? F("成功") : F("失敗"));
  Serial.print(F("，目前 "));
  Serial.println(bad ? F("失效！") : F("正常"));
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

**你會看到：**

```text
=== 測距者 2 路：雷射健康檢查 ===
---------------------------
F3 就緒 是  任一顆失效 否
  左 顆：初始化 成功，目前 正常
  右 顆：初始化 成功，目前 正常
兩顆雷射都健康！
```

---

## 🔧 共用命令範例（所有 MBC 通用 I2C 產品都一樣）

這三支放在 `examples/i2c/arduino/`，循行者、測距者、掌控者、IMU、神攝手都能用；程式開頭的 `ADDR`／`OLD_ADDR` 改成 `0x29` 即可。

<h3 id="exi1">共用 1：換 I2C 位址（F8）</h3>

**做什麼：** 兩個測距者 2 路要接在同一條線上（例如車頭、車尾各一個）時，用 `[F8][新位址][新位址取反]` 把其中一個換成別的位址（存 EEPROM）；`[F8][00][FF]` 回出廠。換完自動送 `[EF][A5]` 重開機，再掃一次確認。

<a href="/examples/i2c/arduino/i2c_01_change_address/i2c_01_change_address.ino" download data-ignore="true">📄 下載 i2c_01_change_address.ino</a>

<!-- ino: examples/i2c/arduino/i2c_01_change_address/i2c_01_change_address.ino -->
```cpp
/*
 * MBC 通用 I2C × Arduino　共用範例 1：換 I2C 位址（F8）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   同一條 I2C 線上要接兩個「門牌號碼」一樣的 MBC 感應器（例如兩個測距者 2 路，都是 0x29）
 *   時，先把其中一個換成別的位址。做法：
 *     寫 [F8][新位址][新位址取反]  → 存進感應器 EEPROM，重開機後生效
 *     寫 [F8][0x00][0xFF]          → 回出廠位址
 *   換完會自動送 [EF][0xA5] 重新開機，再掃一次確認新位址有出現。
 *
 * 你需要：任何一款 MBC 通用 I2C 版感應器（一次只接一個！）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼用：
 *   1. 把下面 OLD_ADDR 改成感應器現在的位址、NEW_ADDR 改成想換的位址（0x08~0x77）。
 *   2. 上傳，打開序列埠監控窗 115200，打 y 按 Enter 就換；打 r 按 Enter 回出廠。
 *   3. 之後所有範例裡的 ADDR 都要改成新位址。
 *
 * 小提醒：
 *   F0「裝置 ID」低 byte 永遠是出廠位址，換了位址也不變，所以掃描時還是認得出是哪一款。
 */

#include <Wire.h>

const uint8_t OLD_ADDR = 0x29;           // 感應器現在的位址   👉 改這裡（循行者 0x16、測距者 0x29、掌控者 0x42、IMU 0x6A、神攝手 0x4C）
const uint8_t NEW_ADDR = 0x2A;           // 想換成的位址       👉 改這裡（0x08 ~ 0x77，別跟其他裝置撞號）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC 通用 I2C：換位址 ==="));
  Serial.print(F("目前線上有哪些 MBC 裝置：")); scan();
  if (!mbcWaitReady(OLD_ADDR, 6000)) {
    Serial.print(F("在 0x")); Serial.print(OLD_ADDR, HEX); Serial.println(F(" 找不到感應器，請確認 OLD_ADDR 和接線。"));
    while (true) {}
  }
  Serial.print(F("找到了 0x")); Serial.print(OLD_ADDR, HEX);
  Serial.print(F("（出廠位址 0x")); Serial.print(mbcRead(OLD_ADDR, 0xF0) & 0xFF, HEX); Serial.println(')');
  Serial.print(F("打 y 換成 0x")); Serial.print(NEW_ADDR, HEX); Serial.println(F("；打 r 回出廠位址。然後按 Enter"));
}

void loop() {
  if (!Serial.available()) { delay(50); return; }
  char c = Serial.read();
  if (c == 'y') {
    if (NEW_ADDR < 0x08 || NEW_ADDR > 0x77) { Serial.println(F("NEW_ADDR 要在 0x08 ~ 0x77 之間")); return; }
    uint8_t p[2] = { NEW_ADDR, (uint8_t)~NEW_ADDR };   // [新位址][新位址取反]：兩個都對才會寫入，避免誤觸
    Serial.print(F("換位址 → 0x")); Serial.println(NEW_ADDR, HEX);
    if (!mbcWrite(OLD_ADDR, 0xF8, p, 2)) { Serial.println(F("寫入失敗")); return; }
    rebootAndCheck(NEW_ADDR);
  } else if (c == 'r') {
    uint8_t p[2] = { 0x00, 0xFF };       // [F8][00][FF] = 回出廠位址
    Serial.println(F("回出廠位址"));
    if (!mbcWrite(OLD_ADDR, 0xF8, p, 2)) { Serial.println(F("寫入失敗")); return; }
    rebootAndCheck(0);                   // 0 = 不知道出廠位址，掃描看結果
  }
}

// 送重開機命令，等它起來，再掃描一次
void rebootAndCheck(uint8_t expect) {
  delay(50);                             // 給 EEPROM 一點時間寫完
  uint8_t magic = 0xA5;
  mbcWrite(OLD_ADDR, 0xEF, &magic, 1);   // [EF][A5] = 重新開機（新位址重開後才生效）
  Serial.println(F("重新開機中..."));
  delay(3000);                           // 測距者 8 路開機要 2~4 秒
  Serial.print(F("掃描結果：")); scan();
  if (expect && mbcRead(expect, 0xF3) != MBC_NONE) {
    Serial.print(F("成功！感應器現在在 0x")); Serial.println(expect, HEX);
    Serial.println(F("記得把其他範例的 ADDR 改成新位址。"));
  } else if (expect) {
    Serial.println(F("新位址沒有回應。請重新上電再掃一次（有些板子的 I2C 要重新上電才會更新）。"));
  }
  Serial.println(F("（把 OLD_ADDR 改成新位址再上傳，就可以再換一次或回出廠）"));
}

// 掃 0x08~0x77，有回應的就讀 F0 看是不是 MBC 裝置
void scan() {
  uint8_t found = 0;
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() != 0) continue;
    int16_t id = mbcRead(a, 0xF0);
    Serial.print(F(" 0x")); Serial.print(a, HEX);
    if (id != MBC_NONE) { Serial.print(F("(出廠 0x")); Serial.print(id & 0xFF, HEX); Serial.print(')'); }
    found++;
  }
  if (!found) Serial.print(F("（沒有）"));
  Serial.println();
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

<h3 id="exi2">共用 2：身分證＋重新開機（F0～F5、EF）</h3>

**做什麼：** 一次讀 `F0`～`F5` 六格共用暫存器並認出產品；打 `r` 送 `[EF][A5]` 重開機，看 `F5` 開機秒數歸零。

<a href="/examples/i2c/arduino/i2c_02_reboot_and_version/i2c_02_reboot_and_version.ino" download data-ignore="true">📄 下載 i2c_02_reboot_and_version.ino</a>

<!-- ino: examples/i2c/arduino/i2c_02_reboot_and_version/i2c_02_reboot_and_version.ino -->
```cpp
/*
 * MBC 通用 I2C × Arduino　共用範例 2：讀身分證＋重新開機（F0~F5、EF）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每一款 MBC 通用 I2C 感應器都有同樣的 6 格「身分證」：
 *     F0 裝置 ID（低 byte 出廠位址、高 byte hw[0]）、F1 協議版本、F2 韌體版本、
 *     F3 狀態旗標、F4 更新序號（每 10 ms +1，0~255 循環）、F5 開機秒數
 *   這個範例每秒印一次，並認出是哪一款產品。
 *   在序列埠打 r 按 Enter → 送 [EF][0xA5] 重新開機，你會看到 F5 開機秒數歸零、F4 繼續跳。
 *
 * 你需要：任何一款 MBC 通用 I2C 版感應器。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。
 *
 * 小提醒：
 *   F4 一圈只有 2.56 秒，不能拿來判斷有沒有重開機；要看 F5 有沒有變小。
 *   F4 不動 = 感應器忙（例如校準中）。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 感應器位址   👉 改這裡（循行者 0x16、測距者 0x29、掌控者 0x42、IMU 0x6A、神攝手 0x4C）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC 通用 I2C：身分證＋重新開機 ==="));
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請確認 ADDR 和接線（可以先跑 i2c_03_scan 掃描）。"));
    while (true) {}
  }
  Serial.println(F("打 r 按 Enter = 重新開機"));
}

void loop() {
  if (Serial.available() && Serial.read() == 'r') {
    uint8_t magic = 0xA5;
    mbcWrite(ADDR, 0xEF, &magic, 1);     // [EF][A5] = 重新開機；其他數字會被忽略
    Serial.println(F(">>> 已送重新開機，看 F5 會不會歸零"));
    delay(500);
  }

  int16_t f[6];                          // F0~F5 編號連續，一次讀 6 格
  if (!mbcReadMany(ADDR, 0xF0, 6, f)) { Serial.println(F("（重開機中／讀不到）")); delay(500); return; }

  uint8_t factory = (uint8_t)(f[0] & 0xFF);
  uint8_t hw0     = (uint8_t)((f[0] >> 8) & 0xFF);
  Serial.println(F("---------------------------"));
  Serial.print(F("F0 裝置 ID    出廠位址 0x")); Serial.print(factory, HEX);
  Serial.print(F("  hw[0] = ")); Serial.print(hw0);
  Serial.print(F("  → ")); printProduct(factory, hw0); Serial.println();
  Serial.print(F("F1 協議版本   ")); Serial.println(f[1]);
  Serial.print(F("F2 韌體版本   ")); Serial.print(f[2] / 100); Serial.print('.'); Serial.println(f[2] % 100);
  Serial.print(F("F3 狀態旗標   0b"));
  for (int8_t i = 7; i >= 0; i--) Serial.print((f[3] >> i) & 1);
  Serial.println(F("  （bit0 = 就緒，其他 bit 看各產品說明）"));
  Serial.print(F("F4 更新序號   ")); Serial.print(f[4]); Serial.println(F("  （一直在跳 = 感應器活著）"));
  Serial.print(F("F5 開機秒數   ")); Serial.print(f[5]); Serial.println(F(" 秒"));

  delay(50);
  delay(950);
}

// 用出廠位址＋hw[0] 認產品
void printProduct(uint8_t factory, uint8_t hw0) {
  if      (factory == 0x16 && hw0 == 8)  Serial.print(F("循行者 8 路"));
  else if (factory == 0x16 && hw0 == 16) Serial.print(F("循行者 16 路"));
  else if (factory == 0x29 && hw0 == 2)  Serial.print(F("測距者 2 路"));
  else if (factory == 0x29 && hw0 == 8)  Serial.print(F("測距者 8 路"));
  else if (factory == 0x42 && hw0 == 1)  Serial.print(F("掌控者 PS2"));
  else if (factory == 0x42 && hw0 == 2)  Serial.print(F("掌控者 PS4"));
  else if (factory == 0x6A)              Serial.print(F("IMU 陀螺儀"));
  else if (factory == 0x4C && hw0 == 1)  Serial.print(F("神攝手 學校版"));
  else if (factory == 0x4C && hw0 == 2)  Serial.print(F("神攝手 WRO 版"));
  else                                   Serial.print(F("不認識的 MBC 裝置"));
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

<h3 id="exi3">共用 3：掃描線上裝置</h3>

**做什麼：** 掃 `0x08`～`0x77`，有回應的就讀 `F0` 認出是哪一款 MBC 產品、韌體幾版、位址有沒有換過。

<a href="/examples/i2c/arduino/i2c_03_scan/i2c_03_scan.ino" download data-ignore="true">📄 下載 i2c_03_scan.ino</a>

<!-- ino: examples/i2c/arduino/i2c_03_scan/i2c_03_scan.ino -->
```cpp
/*
 * MBC 通用 I2C × Arduino　共用範例 3：掃描線上有哪些裝置
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把 I2C 位址 0x08~0x77 全部敲一遍，有回應的就讀 F0「裝置 ID」認出是哪一款 MBC 產品
 *   （不是 MBC 的裝置會印「其他裝置」）。接線有沒有通、位址換成什麼、同一條線上接了幾個，
 *   一跑就知道。每 3 秒掃一次。
 *
 * 你需要：任何 MBC 通用 I2C 版感應器（可以同時接好幾個，位址不同就行）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。
 */

#include <Wire.h>

const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  Serial.println(F("=== MBC 通用 I2C：掃描 ==="));
}

void loop() {
  uint8_t found = 0;
  Serial.println(F("掃描 0x08 ~ 0x77 ..."));
  for (uint8_t a = 0x08; a <= 0x77; a++) {
    Wire.beginTransmission(a);           // 敲門
    if (Wire.endTransmission() != 0) continue;   // 沒人應門 → 下一個
    found++;
    Serial.print(F("  0x"));
    if (a < 0x10) Serial.print('0');
    Serial.print(a, HEX);
    Serial.print(F("  "));

    int16_t id = mbcRead(a, 0xF0);       // 讀裝置 ID
    if (id == MBC_NONE) { Serial.println(F("其他裝置（不是 MBC，或不是通用 I2C 版）")); continue; }
    uint8_t factory = (uint8_t)(id & 0xFF);
    uint8_t hw0     = (uint8_t)((id >> 8) & 0xFF);
    printProduct(factory, hw0);
    int16_t fw = mbcRead(a, 0xF2);
    Serial.print(F("  韌體 ")); Serial.print(fw / 100); Serial.print('.'); Serial.print(fw % 100);
    Serial.print(F("  協議 v")); Serial.print(mbcRead(a, 0xF1));
    if (factory != a) { Serial.print(F("  （位址已換過，出廠是 0x")); Serial.print(factory, HEX); Serial.print(')'); }
    Serial.println();
  }
  if (!found) Serial.println(F("  沒有任何裝置：檢查 SDA/SCL 有沒有接反、VCC/GND 有沒有插緊"));
  Serial.print(F("共 ")); Serial.print(found); Serial.println(F(" 個裝置"));
  delay(50);
  delay(2950);
}

// 用出廠位址＋hw[0] 認產品
void printProduct(uint8_t factory, uint8_t hw0) {
  if      (factory == 0x16 && hw0 == 8)  Serial.print(F("循行者 8 路"));
  else if (factory == 0x16 && hw0 == 16) Serial.print(F("循行者 16 路"));
  else if (factory == 0x29 && hw0 == 2)  Serial.print(F("測距者 2 路"));
  else if (factory == 0x29 && hw0 == 8)  Serial.print(F("測距者 8 路"));
  else if (factory == 0x42 && hw0 == 1)  Serial.print(F("掌控者 PS2"));
  else if (factory == 0x42 && hw0 == 2)  Serial.print(F("掌控者 PS4"));
  else if (factory == 0x6A)              Serial.print(F("IMU 陀螺儀"));
  else if (factory == 0x4C && hw0 == 1)  Serial.print(F("神攝手 學校版"));
  else if (factory == 0x4C && hw0 == 2)  Serial.print(F("神攝手 WRO 版"));
  else                                   Serial.print(F("不認識的 MBC 裝置"));
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

**你會看到：**

```text
=== MBC 通用 I2C：掃描 ===
掃描 0x08 ~ 0x77 ...
  0x29  測距者 2 路  韌體 2.0  協議 v2
共 1 個裝置
```

---

## ❓ 常見問題

**Q：一直出現「找不到感應器」？**
1. 檢查 SDA、SCL 有沒有接反（UNO：SDA → A4、SCL → A5；ESP32：21 / 22）。
2. 檢查 VCC、GND 有沒有插緊。
3. 確認是 **通用 I2C 版**：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。舊板（PA9/PA10 沒有 I2C）也沒有通用 I2C 版。
4. 跑 [共用 3 掃描](#exi3)，看線上到底有沒有裝置、位址是不是被換過。

**Q：讀到 `-32768` 代表什麼？**
`-32768`（程式裡叫 `MBC_NONE`）有兩種意思：① 小幫手讀不到感應器（接線鬆了、位址錯了）；② 那個編號是空號——測距者 2 路只有 `01`～`08`，去讀 `09` 就會拿到 `-32768`。正常的距離不可能是這個數字，所以可以直接拿來判斷「這筆資料不能用」。

**Q：ESP32 讀不到、或讀到的值怪怪的？**
MBC 有些產品（掌控者 PS4、神攝手）本身是 ESP32 做的，不能「時脈延展」，主機寫完編號要**等 2 ms** 再讀。小幫手 `mbcReadMany` 裡已經有 `delay(2)`，請不要刪掉。ESP32 當主機時 I2C 腳位預設是 GPIO 21（SDA）／22（SCL）。

**Q：距離一直是 120／1200？**
那個方向 120 公分內真的沒東西，或那顆雷射失效了。跑 [範例 6](#ex6) 看 `07` 故障位元圖就知道是哪一種。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：可以同時接測距者 2 路和 8 路嗎？**
兩款出廠都是 `0x29`，要先用 [共用 1](#exi1) 把其中一個換位址。位址不同的 MBC 產品（例如循行者 `0x16`）本來就能接在同一組線上。
