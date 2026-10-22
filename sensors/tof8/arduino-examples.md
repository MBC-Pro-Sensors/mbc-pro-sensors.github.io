<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 測距者 8 路 × Arduino 範例

[🔙 回到 8 路雷射測距首頁](/sensors/tof8/index.md)　｜　[📡 MBC 通用 I2C 協議（進階）](/i2c-protocol.md)

這一頁有 **6 個產品範例＋3 個共用範例**，從「確認接好了沒」一路做到「避障小車」。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/tof8/arduino/tof8_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
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

圖：`/images/figures/tof8/wiring-uno.svg`

> [!IMPORTANT]
> - 請用 **通用 I2C 版** 的測距者 8 路。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 感應器一通電，要花 **2～4 秒**把 8 顆雷射一顆一顆叫醒。還沒就緒前距離都是 0，看起來像貼著障礙物，所以每個範例一開始都先等 `F3` 就緒（最多 6 秒），你自己寫程式時也要這樣做。
> - 接線前先拔掉 USB 線。

**8 顆雷射朝哪裡看？**（感應器裝在車頭，站在車後往前看）

| 第幾顆 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 大約的角度 | 左 90° | 左 60° | 左 30° | 正前方 | 正前方 | 右 30° | 右 60° | 右 90° |
| 編號（cm） | `10` | `11` | `12` | `13` | `14` | `15` | `16` | `17` |
| 編號（mm） | `18` | `19` | `1A` | `1B` | `1C` | `1D` | `1E` | `1F` |

---

<h2 id="cmd-table">📖 指令總覽表（協議 v2）</h2>

感應器像一排有編號的「格子」，每一格放一個 16 位元的數字（int16，小端）。讀法：寫 `[編號]` → 等 2 ms → 讀 2×k 個 byte，就能從那個編號起一次拿到連續 k 格（k ≤ 16）。沒有的編號會回 `-32768`。門牌號碼（I2C 位址）是 `0x29`。

<!-- gen:cmd-table -->
| 編號 | 名稱 | 範圍 / 單位 | 說明 | 出現在 |
| :---: | :--- | :--- | :--- | :--- |
| `01` | 最近距離 | 0～120 cm | 8 顆裡最小的；120 = 四周沒東西 | [範例 2](#ex2)、[3](#ex3)、[4](#ex4)、[6](#ex6) |
| `02` | 最近方向 | −7～+7（±8 = 沒東西） | 最近的東西在哪：0 正前方、負 = 左、−7 正左、+7 正右；沒東西時 ±8 | [範例 3](#ex3)、[4](#ex4)、[6](#ex6) |
| `03` | 最空曠距離 | 0～120 cm | 8 顆裡最大的（壞掉那顆永遠 120，會被當最空曠） | [範例 4](#ex4) |
| `04` | 最空曠方向 | −7～+7 | 最遠在哪個方向；8 顆全 120 時 = 0 | [範例 4](#ex4) |
| `05` | 正前方距離 | 0～120 cm | 第 4、5 顆平均 | [範例 2](#ex2)、[6](#ex6) |
| `06` | 最近距離 | 0～1200 mm | 8 顆 mm 的最小值 | [範例 3](#ex3) |
| `07` | 初始化位元圖 | 0～255（bit） | bit0 = 第 1 顆（最左）… bit7 = 第 8 顆，1 = 開機成功；255 = 全部好 | [範例 1](#ex1)、[2](#ex2) |
| `08` | 故障位元圖 | 0～255（bit） | bit0 = 第 1 顆 … bit7 = 第 8 顆，1 = 那顆目前失效；失效那顆距離回 120／1200 | [範例 2](#ex2)、[5](#ex5) |
| `09` | 正前方距離 | 0～1200 mm | 第 4、5 顆平均的 mm 版 | [範例 2](#ex2)、[6](#ex6) |
| `10`～`17` | 第 1～8 顆距離 | 0～120 cm | 各顆距離；`10` = 第 1 顆（正左）、`13`／`14` = 正前方、`17` = 第 8 顆（正右） | [範例 2](#ex2)、[5](#ex5) |
| `18`～`1F` | 第 1～8 顆距離 | 0～1200 mm | 各顆距離 mm 版 | [範例 2](#ex2) |
| `F0` | 裝置 ID | — | 低 byte = 出廠位址 0x29、高 byte = 8（雷射顆數） | [範例 1](#ex1)、[共用 2](#exi2)、[共用 3](#exi3) |
| `F1` | 協議版本 | 2 | 這份範例照版本 2 寫 | [範例 1](#ex1) |
| `F2` | 韌體版本 | 主×100＋次 | 例 102 = 1.2 | [範例 1](#ex1) |
| `F3` | 狀態旗標 | bit | b0 就緒（8 顆都初始化完，開機約 2～4 秒）、b1 任一顆失效 | [範例 1](#ex1) |
| `F4` | 更新序號 | 0～255 | 每 10 ms +1，一直跳 = 感應器活著 | [共用 2](#exi2) |
| `F5` | 開機秒數 | 0～32767 秒 | 變小 = 重開機過 | [範例 1](#ex1)、[共用 2](#exi2) |
| `F8` | 換 I2C 位址（命令） | `[新位址][取反]` | `[F8][00][FF]` = 回出廠；重開機後生效 | [共用 1](#exi1) |
| `EF` | 重新開機（命令） | `[A5]` | 送 `[EF][A5]` | [共用 1](#exi1)、[共用 2](#exi2) |
<!-- /gen -->

測距者 8 路沒有產品專用的寫入命令（E0～EE）。

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟感應器打招呼](#ex1) | 確認接線正確、8 顆雷射都開機成功 | 無 |
| 2 | [一次看完所有讀值](#ex2) | 整張值表 01～09、10～17 cm、18～1F mm | 你的手 |
| 3 | [最近的東西在哪個方向？](#ex3) | 01／02／06 最近距離與方向 | 一個盒子 |
| 4 | [哪邊最空曠？](#ex4) | 03／04 最空曠距離與方向 | 幾本書 |
| 5 | [雷達長條圖](#ex5) | 10～17 八顆 cm 畫長條、08 標故障 | 幾本書 |
| 6 | [避障小車（進階）](#ex6) | 05／09 正前方＋02 最近方向避障 | 兩輪小車、L298N 馬達驅動板 |
| 共 1 | [換 I2C 位址](#exi1) | F8 換位址／回出廠、EF 重開機 | 無 |
| 共 2 | [身分證＋重新開機](#exi2) | F0～F5 六格共用暫存器 | 無 |
| 共 3 | [掃描線上裝置](#exi3) | 掃 0x08～0x77、用 F0 認產品 | 無 |

> [!TIP]
> **建議順序：1 → 2 → 3 → 4 → 5 → 6。** 先確認接好（1），看懂每一格（2），再學方向（3、4）和應用。

---

<h2 id="ex1">1️⃣ 跟感應器打招呼</h2>

**做什麼：** 確認感應器接好了，並讀出它的身分證：`F0` 裝置 ID（出廠位址＋雷射顆數）、`F1` 協議版本、`F2` 韌體版本、`F3` 狀態旗標逐 bit 用中文印出來，再用 `07` 初始化位元圖列出 8 顆有沒有都開機成功。每次接新的感應器，都先跑這一個。

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
 *   確認感應器有沒有接好。接好了，會印出它的身分證（裝置 ID、協議版本、
 *   韌體版本）和目前狀態（8 顆雷射是不是都正常）。
 *   每次接新感應器，都建議先跑這個範例。
 *
 * 你需要：測距者 8 路（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   8 顆雷射一顆一顆叫醒要 2～4 秒，還沒就緒前距離都是 0，所以一定要先等 F3 bit0。
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

  Serial.println(F("=== 測距者 8 路：跟感應器打招呼 ==="));
  Serial.println(F("等 8 顆雷射開機（最多 6 秒）..."));

  // 等感應器說「我準備好了」（狀態旗標 F3 的 bit0 = 8 顆都初始化完），最多等 6 秒
  if (!mbcWaitReady(ADDR, 6000)) {
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
  Serial.print(sensors);                 // 測距者 8 路 = 8
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
  Serial.print(F("  bit0 就緒        ")); Serial.println((st & 0x01) ? F("是（8 顆都初始化完）") : F("否"));
  Serial.print(F("  bit1 任一顆失效  ")); Serial.println((st & 0x02) ? F("是！（跑範例 2 看是哪一顆）") : F("否（8 顆都正常）"));

  // ---- 07 初始化位元圖：bit0 = 第 1 顆（最左）… bit7 = 第 8 顆 ----
  int16_t initOk = mbcRead(ADDR, 0x07);
  Serial.print(F("初始化成功的雷射（左→右）："));
  for (uint8_t i = 0; i < 8; i++) Serial.print((initOk >> i) & 1 ? 'O' : 'X');
  Serial.println(initOk == 255 ? F("  全部成功") : F("  有 X 的那顆開機失敗，請重新上電"));

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
=== 測距者 8 路：跟感應器打招呼 ===
等 8 顆雷射開機（最多 6 秒）...
找到了！出廠位址 0x29，雷射數量 8 顆
協議版本：2
韌體版本：2.0
目前狀態：
  bit0 就緒        是（8 顆都初始化完）
  bit1 任一顆失效  否（8 顆都正常）
初始化成功的雷射（左→右）：OOOOOOOO  全部成功
開機秒數：6
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

**做什麼：** 每 0.5 秒把整張值表印一次，分三段 `mbcReadMany`：`01`～`09`、`10`～`17`（8 顆 cm）、`18`～`1F`（8 顆 mm）。先在這裡認識每一格，之後的範例只是各挑幾格來用。

<a href="/examples/tof8/arduino/tof8_02_all_values/tof8_02_all_values.ino" download data-ignore="true">📄 下載 tof8_02_all_values.ino</a>

<!-- ino: examples/tof8/arduino/tof8_02_all_values/tof8_02_all_values.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 2：一次看完所有讀值
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器「整張值表」每 0.5 秒印一次：
 *     01 最近 cm、02 最近方向、03 最空曠 cm、04 最空曠方向、05 正前方 cm、
 *     06 最近 mm、07 初始化位元圖、08 故障位元圖、09 正前方 mm、
 *     10~17 第 1~8 顆 cm、18~1F 第 1~8 顆 mm。
 *   之後的範例只是各挑其中幾格來用，先在這裡把每一格認識一遍。
 *
 * 你需要：測距者 8 路（通用 I2C 版）＋ 你的手或幾本書。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。手從左邊慢慢移到右邊，看數字怎麼從第 1 顆走到第 8 顆。
 *
 * 小提醒：
 *   一次最多讀 16 個，所以分三段：01~09、10~17、18~1F。
 *   120 cm／1200 mm = 那個方向沒東西（或那顆失效）。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿手在感應器前面移動看看。"));
}

void loop() {
  int16_t a[9];                          // 01~09
  int16_t cm[8];                         // 10~17 第 1~8 顆 cm
  int16_t mm[8];                         // 18~1F 第 1~8 顆 mm
  if (!mbcReadMany(ADDR, 0x01, 9, a))  { Serial.println(F("讀不到（01~09）")); delay(500); return; }
  if (!mbcReadMany(ADDR, 0x10, 8, cm)) { Serial.println(F("讀不到（10~17）")); delay(500); return; }
  if (!mbcReadMany(ADDR, 0x18, 8, mm)) { Serial.println(F("讀不到（18~1F）")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 最近距離     ")); Serial.print(a[0]); Serial.println(F(" cm   （8 顆裡最小的；120 = 四周沒東西）"));
  Serial.print(F("02 最近方向     ")); Serial.print(a[1]); Serial.println(F("      （-7 正左 ~ +7 正右，0 = 正前方；±8 = 沒東西）"));
  Serial.print(F("03 最空曠距離   ")); Serial.print(a[2]); Serial.println(F(" cm   （8 顆裡最大的）"));
  Serial.print(F("04 最空曠方向   ")); Serial.print(a[3]); Serial.println(F("      （-7 ~ +7）"));
  Serial.print(F("05 正前方距離   ")); Serial.print(a[4]); Serial.println(F(" cm   （第 4、5 顆平均）"));
  Serial.print(F("06 最近距離     ")); Serial.print(a[5]); Serial.println(F(" mm   （比 cm 精細 10 倍）"));
  Serial.print(F("07 初始化位元圖 "));
  for (uint8_t i = 0; i < 8; i++) Serial.print((a[6] >> i) & 1);     // 左邊印 bit0（第 1 顆）
  Serial.println(F("  （左→右，1 = 開機成功）"));
  Serial.print(F("08 故障位元圖   "));
  for (uint8_t i = 0; i < 8; i++) Serial.print((a[7] >> i) & 1);
  Serial.println(F("  （左→右，1 = 那顆現在失效）"));
  Serial.print(F("09 正前方距離   ")); Serial.print(a[8]); Serial.println(F(" mm"));

  Serial.print(F("10~17 各顆 cm 左 "));
  for (uint8_t i = 0; i < 8; i++) { printPad(cm[i], 4); Serial.print(' '); }
  Serial.println(F(" 右"));
  Serial.print(F("18~1F 各顆 mm 左 "));
  for (uint8_t i = 0; i < 8; i++) { printPad(mm[i], 4); Serial.print(' '); }
  Serial.println(F(" 右   （第 1 顆正左 90°、第 4/5 顆正前、第 8 顆正右 90°）"));

  delay(50);
  delay(450);                            // 每 0.5 秒更新一次
}

// 靠右對齊印數字（寬度 w）
void printPad(int16_t v, uint8_t w) {
  uint8_t len = (v < 0) ? 2 : 1;
  for (int16_t t = (v < 0 ? -v : v); t >= 10; t /= 10) len++;
  for (uint8_t i = len; i < w; i++) Serial.print(' ');
  Serial.print(v);
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

**你會看到：**（手放在左前方 30 公分）

```text
---------------------------------------
01 最近距離     30 cm   （8 顆裡最小的；120 = 四周沒東西）
02 最近方向     -3      （-7 正左 ~ +7 正右，0 = 正前方；±8 = 沒東西）
03 最空曠距離   120 cm   （8 顆裡最大的）
04 最空曠方向   5      （-7 ~ +7）
05 正前方距離   120 cm   （第 4、5 顆平均）
06 最近距離     304 mm   （比 cm 精細 10 倍）
07 初始化位元圖 11111111  （左→右，1 = 開機成功）
08 故障位元圖   00000000  （左→右，1 = 那顆現在失效）
09 正前方距離   1200 mm
10~17 各顆 cm 左  120  120   30   85  120  120  120  120  右
18~1F 各顆 mm 左 1200 1200  304  851 1200 1200 1200 1200  右   （第 1 顆正左 90°、第 4/5 顆正前、第 8 顆正右 90°）
```

> [!TIP]
> **試試看：** 把手從左邊慢慢移到右邊，看數字是怎麼從第 1 顆「走」到第 8 顆的。第 4、5 顆都朝正前方，所以手在正前方時兩顆的數字會很接近。

---

<h2 id="ex3">3️⃣ 最近的東西在哪個方向？</h2>

**做什麼：** 感應器會自己比較 8 顆雷射：`01` 最近距離 cm、`02` 最近方向、`06` 最近距離 mm。電腦畫一把小尺用 `*` 標出方向，再用箭頭說出來。相撲機器人「找對手」用的就是這個。

**準備：** 一個盒子或你的手。

<a href="/examples/tof8/arduino/tof8_03_nearest_direction/tof8_03_nearest_direction.ino" download data-ignore="true">📄 下載 tof8_03_nearest_direction.ino</a>

<!-- ino: examples/tof8/arduino/tof8_03_nearest_direction/tof8_03_nearest_direction.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 3：最近的東西在哪個方向？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   感應器會自己比較 8 顆雷射，告訴你最近的東西在哪個方向、有多遠：
 *     01 最近距離 cm、02 最近方向（-7 正左 ~ +7 正右，0 = 正前方）、06 最近距離 mm。
 *   電腦畫一把小尺用 * 標出方向，再用箭頭說出來。相撲機器人「找對手」用的就是這個。
 *
 * 你需要：測距者 8 路（通用 I2C 版）＋ 一個盒子或你的手。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。手從左前方移到正前方，再移到右邊。
 *
 * 小提醒：
 *   120 公分內什麼都沒有時，方向會變成 ±8（代表「上次偏哪一邊」），不是真的有東西。
 *   所以一定要先看距離再看方向。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！拿東西在感應器周圍移動看看。"));
}

void loop() {
  int16_t v[6];                          // 01 最近 cm、02 最近方向、03、04、05、06 最近 mm
  if (!mbcReadMany(ADDR, 0x01, 6, v)) { Serial.println(F("讀不到感應器")); delay(500); return; }
  int16_t nearCm = v[0], dir = v[1], nearMm = v[5];

  // 畫一把 15 格的小尺：方向 -7 ~ +7 各一格，* = 最近的東西，+ = 正前方
  Serial.print(F("左 ["));
  for (int8_t i = -7; i <= 7; i++) {
    if (nearCm < 120 && i == dir) Serial.print('*');
    else if (i == 0)              Serial.print('+');
    else                          Serial.print('.');
  }
  Serial.print(F("] 右   "));

  if (nearCm >= 120) {                   // 先看距離！沒東西時方向是 ±8，不能信
    Serial.println(F("前面 120 公分內沒有東西"));
  } else {
    if      (dir <= -6) Serial.print(F("← 左邊  "));
    else if (dir <= -2) Serial.print(F("↖ 左前方"));
    else if (dir <=  1) Serial.print(F("↑ 正前方"));
    else if (dir <=  5) Serial.print(F("↗ 右前方"));
    else                Serial.print(F("→ 右邊  "));
    Serial.print(' '); Serial.print(nearCm); Serial.print(F(" 公分（"));
    Serial.print(nearMm); Serial.print(F(" mm，方向 ")); Serial.print(dir); Serial.println(')');
  }
  delay(50);
  delay(150);                            // 每 0.2 秒印一行   👉 可以改
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

**你會看到：**（手從左前方移到正前方，再移到右邊）

```text
左 [.......+.......] 右   前面 120 公分內沒有東西
左 [..*....+.......] 右   ↖ 左前方 35 公分（352 mm，方向 -5）
左 [.......*.......] 右   ↑ 正前方 28 公分（283 mm，方向 0）
左 [.......+.....*.] 右   → 右邊   40 公分（401 mm，方向 6）
```

**方向是什麼意思？**

| 方向數字 | 意思 |
| :---: | :--- |
| 0 | 東西在正前方 |
| 負數（−1～−7） | 東西在**左**邊，數字越小越偏左（−7 = 正左邊） |
| 正數（+1～+7） | 東西在**右**邊，數字越大越偏右（+7 = 正右邊） |
| ±8 | 120 公分內沒東西（只是「上次偏哪一邊」） |

> [!WARNING]
> **一定要先看距離，再看方向。** 120 公分內什麼都沒有時，方向是 `±8`，不是真的有東西在那裡。範例裡先用「距離 < 120」確定有東西，才去看方向。

---

<h2 id="ex4">4️⃣ 哪邊最空曠？</h2>

**做什麼：** 讀 `03` 最空曠距離、`04` 最空曠方向——感應器幫你找出 8 個方向裡最遠（最沒東西）的那個。迷宮、避障時「往最空的地方走」就用這兩格。同時印 `01`／`02` 最近的，讓你比較「該躲哪裡、該往哪裡走」。

**準備：** 幾本書當牆壁。

<a href="/examples/tof8/arduino/tof8_04_open_direction/tof8_04_open_direction.ino" download data-ignore="true">📄 下載 tof8_04_open_direction.ino</a>

<!-- ino: examples/tof8/arduino/tof8_04_open_direction/tof8_04_open_direction.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 4：哪邊最空曠？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀「03 最空曠距離 cm」「04 最空曠方向」：感應器幫你找出 8 個方向裡
 *   最遠（最沒東西）的那個方向。迷宮、避障時「往最空的地方走」就用這兩格。
 *   同時讀 01/02 最近的，兩個一起印，讓你比較「該躲哪裡、該往哪裡走」。
 *
 * 你需要：測距者 8 路（通用 I2C 版）＋ 幾本書當牆壁。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。用書在感應器左、右、前方擺出不同的「牆」。
 *
 * 小提醒：
 *   壞掉的那顆距離永遠是 120，會被當成「最空曠」；比賽前先跑範例 1 確認 8 顆都正常。
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！用書在感應器旁邊擺幾面牆看看。"));
}

void loop() {
  int16_t v[4];                          // 01 最近 cm、02 最近方向、03 最空曠 cm、04 最空曠方向
  if (!mbcReadMany(ADDR, 0x01, 4, v)) { Serial.println(F("讀不到感應器")); delay(500); return; }
  int16_t nearCm = v[0], nearDir = v[1], farCm = v[2], farDir = v[3];

  Serial.print(F("最空曠 → "));
  printDir(farDir);
  Serial.print(' '); Serial.print(farCm); Serial.print(F(" 公分"));

  Serial.print(F("   ｜   最近 → "));
  if (nearCm >= 120) Serial.println(F("四周 120 公分內沒東西"));
  else {
    printDir(nearDir);
    Serial.print(' '); Serial.print(nearCm); Serial.println(F(" 公分"));
  }
  delay(50);
  delay(250);                            // 每 0.3 秒印一行   👉 可以改
}

// 把 -7 ~ +7 的方向印成文字（每一格 ≈ 13°，±7 ≈ ±90°）
void printDir(int16_t dir) {
  if      (dir <= -6) Serial.print(F("正左"));
  else if (dir <= -2) Serial.print(F("左前"));
  else if (dir <=  1) Serial.print(F("正前"));
  else if (dir <=  5) Serial.print(F("右前"));
  else                Serial.print(F("正右"));
  Serial.print(F("(")); Serial.print(dir); Serial.print(')');
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

**你會看到：**（左邊和前面有書，右邊空的）

```text
最空曠 → 正右(7) 120 公分   ｜   最近 → 正前(0) 18 公分
最空曠 → 右前(4) 95 公分   ｜   最近 → 左前(-4) 22 公分
```

> [!TIP]
> 壞掉的那顆距離永遠是 120，會被當成「最空曠」。比賽前先跑 [範例 1](#ex1) 確認 8 顆都正常。

---

<h2 id="ex5">5️⃣ 雷達長條圖</h2>

**做什麼：** 一次讀 `10`～`17` 八顆 cm，畫成 8 條長條：**長條越長 = 那個方向越空曠**，很短 = 有東西擋著。順便讀 `08` 故障位元圖，壞掉的那顆標 `XX`。看一眼就知道哪邊可以走。

**準備：** 幾本書當障礙物。

<a href="/examples/tof8/arduino/tof8_05_radar_bars/tof8_05_radar_bars.ino" download data-ignore="true">📄 下載 tof8_05_radar_bars.ino</a>

<!-- ino: examples/tof8/arduino/tof8_05_radar_bars/tof8_05_radar_bars.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 5：雷達長條圖
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   一次讀「10~17 第 1~8 顆距離 cm」，把 8 個方向畫成 8 條長條：
 *   長條越長 = 那個方向越空曠，很短 = 有東西擋著。看一眼就知道哪邊可以走。
 *   順便讀「08 故障位元圖」，壞掉的那顆標 XX。
 *
 * 你需要：測距者 8 路（通用 I2C 版）＋ 幾本書當障礙物。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。
 *
 * 8 顆雷射朝哪裡看？（站在車後往前看）
 *   第 1 顆 左 90°、第 2 顆 左 60°、第 3 顆 左 30°、第 4、5 顆 正前方、
 *   第 6 顆 右 30°、第 7 顆 右 60°、第 8 顆 右 90°
 */

#include <Wire.h>

const uint8_t ADDR = 0x29;               // 測距者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int8_t ANGLE[8] = {-90, -60, -30, 0, 0, 30, 60, 90};   // 每顆的角度（負 = 左）

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！放幾本書在感應器旁邊看看。"));
}

void loop() {
  int16_t cm[8];
  int16_t fault = mbcRead(ADDR, 0x08);   // 08 故障位元圖：bit0 = 第 1 顆
  if (!mbcReadMany(ADDR, 0x10, 8, cm) || fault == MBC_NONE) { Serial.println(F("讀不到感應器")); delay(500); return; }

  Serial.println(F("====== 雷達畫面（長條越長 = 越空曠）======"));
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(i + 1);                 // 第幾顆（1~8）
    Serial.print(' ');
    if (ANGLE[i] >= 0) Serial.print('+');
    if (ANGLE[i] > -10 && ANGLE[i] < 10) Serial.print(' ');
    Serial.print(ANGLE[i]); Serial.print(F("度 |"));

    if ((fault >> i) & 1) {              // 這顆失效
      Serial.println(F("XX 故障"));
      continue;
    }
    int8_t n = cm[i] / 5;                // 每 5 公分一格，最多 24 格
    if (n > 24) n = 24;
    for (int8_t k = 0; k < n; k++) Serial.print('#');
    if (cm[i] >= 120) Serial.println(F("> 沒有東西"));
    else { Serial.print(' '); Serial.print(cm[i]); Serial.println(F(" 公分")); }
  }
  delay(50);
  delay(450);                            // 每 0.5 秒重畫一次   👉 可以改
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

**你會看到：**（正前方 30 公分有一本書）

```text
====== 雷達畫面（長條越長 = 越空曠）======
1 -90度 |########################> 沒有東西
2 -60度 |########################> 沒有東西
3 -30度 |############### 75 公分
4 + 0度 |###### 32 公分
5 + 0度 |###### 34 公分
6 +30度 |################ 80 公分
7 +60度 |###################### 110 公分
8 +90度 |########################> 沒有東西
```

---

<h2 id="ex6">6️⃣ 避障小車（進階）</h2>

**做什麼：** 讓兩輪小車自己繞過障礙物：`05` 正前方 cm 還遠就直走；前面有東西就看 `02` 最近方向，東西在左往右轉、在右往左轉；`09` 正前方 mm 太近就先倒退。一次 `mbcReadMany` 從 `01` 讀到 `09`，所有判斷用同一瞬間的資料。

**另外要準備：** 兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。

**馬達接線：**

| L298N | Arduino UNO / Nano / R4 | ESP32 | 控制 |
| :---: | :---: | :---: | :--- |
| ENA | 5 | GPIO 25 | 左輪速度 |
| IN1 / IN2 | 7 / 8 | GPIO 26 / 27 | 左輪方向 |
| ENB | 6 | GPIO 14 | 右輪速度 |
| IN3 / IN4 | 9 / 10 | GPIO 32 / 33 | 右輪方向 |
| GND | GND | GND | **一定要跟 Arduino 共地** |

**第一次跑之前：** 把車子架高（輪子懸空）再上傳，確認兩個輪子都是**往前**轉。哪個輪子轉反了，就把 `LEFT_REVERSE` 或 `RIGHT_REVERSE` 改成 `true`。

<a href="/examples/tof8/arduino/tof8_06_avoid_car/tof8_06_avoid_car.ino" download data-ignore="true">📄 下載 tof8_06_avoid_car.ino</a>

<!-- ino: examples/tof8/arduino/tof8_06_avoid_car/tof8_06_avoid_car.ino -->
```cpp
/*
 * 測距者 8 路 × Arduino　範例 6（進階）：避障小車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車自己繞過障礙物：
 *     - 「05 正前方 cm」還很遠 → 直走
 *     - 正前方有東西 → 看「02 最近方向」，東西在左就往右轉、在右就往左轉
 *     - 「09 正前方 mm」太近（快撞到）→ 先倒退一下再轉
 *   用 mbcReadMany 從 01 一次讀到 09，所有判斷都用同一瞬間的資料。
 *
 * 你需要：兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等）、電池。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   （ESP32 改接：ENA → 25、IN1 → 26、IN2 → 27、ENB → 14、IN3 → 32、IN4 → 33）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！感應器裝在車頭正中間，雷射朝前。
 *
 * 第一次跑之前：
 *   把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *   哪個輪子轉反了，就把 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
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
const int     SPEED      = 130;          // 直走速度 0~255
const int     TURN_SPEED = 110;          // 轉彎速度
const int16_t TURN_CM    = 35;           // 正前方幾公分以內要轉彎
const int16_t BACK_MM    = 120;          // 正前方幾 mm 以內先倒退
const bool    LEFT_REVERSE  = false;     // 左輪轉反了改成 true
const bool    RIGHT_REVERSE = false;     // 右輪轉反了改成 true

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 6000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("3 秒後出發！"));
  delay(3000);
}

void loop() {
  int16_t v[9];                          // 01~09
  if (!mbcReadMany(ADDR, 0x01, 9, v)) {  // 讀不到 → 安全停車
    drive(0, 0);
    delay(50);
    return;
  }
  int16_t nearCm  = v[0];                // 01 最近距離 cm
  int16_t nearDir = v[1];                // 02 最近方向（負 = 左）
  int16_t frontCm = v[4];                // 05 正前方 cm
  int16_t frontMm = v[8];                // 09 正前方 mm

  if (frontMm <= BACK_MM) {              // 快撞到了：先倒退
    Serial.println(F("太近！倒退"));
    drive(-TURN_SPEED, -TURN_SPEED);
    delay(300);
  } else if (frontCm <= TURN_CM || (nearCm < 20 && nearDir > -3 && nearDir < 3)) {
    // 正前方有東西（或很近的東西在前方附近）→ 往東西的反方向轉
    if (nearDir < 0) { Serial.println(F("障礙在左 → 右轉")); drive(TURN_SPEED, -TURN_SPEED); }
    else             { Serial.println(F("障礙在右 → 左轉")); drive(-TURN_SPEED, TURN_SPEED); }
    delay(200);
  } else {
    drive(SPEED, SPEED);                 // 前面空的：直走
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

**怎麼調整：**

| 車子的樣子 | 怎麼改 |
| :--- | :--- |
| 常常撞到才轉 | `TURN_CM` 加大（例如 35 → 50） |
| 太早轉、走不進窄路 | `TURN_CM` 調小 |
| 轉完還是對著障礙物 | 轉彎 `delay(200)` 加長 |
| 一出發就原地打轉 | 有一個輪子轉反了，改 `LEFT_REVERSE` 或 `RIGHT_REVERSE` |

---

## 🔧 共用命令範例（所有 MBC 通用 I2C 產品都一樣）

這三支放在 `examples/i2c/arduino/`，循行者、測距者、掌控者、IMU、神攝手都能用；程式開頭的 `ADDR`／`OLD_ADDR` 改成 `0x29` 即可。

<h3 id="exi1">共用 1：換 I2C 位址（F8）</h3>

**做什麼：** 同一條線要接兩個位址一樣的感應器時，用 `[F8][新位址][新位址取反]` 把其中一個換成別的位址（存 EEPROM）；`[F8][00][FF]` 回出廠。換完自動送 `[EF][A5]` 重開機，再掃一次確認。

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

**做什麼：** 一次讀 `F0`～`F5` 六格共用暫存器並認出產品；打 `r` 送 `[EF][A5]` 重開機，看 `F5` 開機秒數歸零（重開後要再等 2～4 秒才就緒）。

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
  0x29  測距者 8 路  韌體 2.0  協議 v2
共 1 個裝置
```

---

## ❓ 常見問題

**Q：一直出現「找不到感應器」？**
1. 檢查 SDA、SCL 有沒有接反（UNO：SDA → A4、SCL → A5；ESP32：21 / 22）。
2. 檢查 VCC、GND 有沒有插緊。8 顆雷射比較吃電，電腦 USB 供電不夠時換接變壓器或電池。
3. 確認是 **通用 I2C 版**：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
4. 跑 [共用 3 掃描](#exi3)，看線上到底有沒有裝置、位址是不是被換過。

**Q：讀到 `-32768` 代表什麼？**
`-32768`（程式裡叫 `MBC_NONE`）有兩種意思：① 小幫手讀不到感應器（接線鬆了、位址錯了）；② 那個編號是空號——例如 `0A`～`0F` 沒有定義，去讀就會拿到 `-32768`。正常的距離不可能是這個數字，所以可以直接拿來判斷「這筆資料不能用」。

**Q：ESP32 讀不到、或讀到的值怪怪的？**
MBC 有些產品（掌控者 PS4、神攝手）本身是 ESP32 做的，不能「時脈延展」，主機寫完編號要**等 2 ms** 再讀。小幫手 `mbcReadMany` 裡已經有 `delay(2)`，請不要刪掉。ESP32 當主機時 I2C 腳位預設是 GPIO 21（SDA）／22（SCL）。

**Q：開機後距離全部是 0？**
8 顆雷射還在初始化（2～4 秒）。等 `F3` bit0 = 1 再讀，小幫手 `mbcWaitReady` 就是在做這件事。

**Q：某一顆一直是 120？**
那個方向真的沒東西，或那顆失效了。跑 [範例 5](#ex5)，故障的那顆會標 `XX`；`07`／`08` 兩格可以分辨「開機就失敗」和「跑一跑失效」。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：可以同時接測距者 8 路和 2 路嗎？**
兩款出廠都是 `0x29`，要先用 [共用 1](#exi1) 把其中一個換位址。位址不同的 MBC 產品（例如循行者 `0x16`）本來就能接在同一組線上。
