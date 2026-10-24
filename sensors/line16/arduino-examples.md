<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- moved: /sensors/line16/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line16/arduino/ide.md">循行者 16 路 › Arduino › Arduino IDE</a></p>

# 🧪 循行者 16 路 × Arduino 範例

[🔙 回到 16 路循線器首頁](/sensors/line16/index.md)　｜　[📡 I2C 暫存器說明（進階）](/sensors/line16/arduino-i2c.md)

這一頁有 **7 個產品範例＋3 個共用範例**，從「確認接好了沒」一路做到「循線小車」。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/line16/arduino/line16_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
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

圖：`/images/figures/line16/wiring-uno.svg`

> [!IMPORTANT]
> - 請用 **通用 I2C 版** 的循行者。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 感應器裝在車上時，**底部離地面約 0.5～1 公分**最準。
> - 接線前先拔掉 USB 線。
> - 「左、右」一律是**感應器裝在車上、你站在車子後面往前看**的左右：線在左 = 負數，第 1 路在最左。

---

<h2 id="cmd-table">📖 指令總覽表（協議 v2）</h2>

感應器像一排有編號的「格子」，每一格放一個 16 位元的數字（int16，小端）。讀法：寫 `[編號]` → 等 2 ms → 讀 2×k 個 byte，就能從那個編號起一次拿到連續 k 格（k ≤ 16）。沒有的編號會回 `-32768`。門牌號碼（I2C 位址）是 `0x16`。

<!-- gen:cmd-table -->
| 編號 | 名稱 | 範圍 / 單位 | 說明 | 出現在 |
| :---: | :--- | :--- | :--- | :--- |
| `01` | 線位置 | −16～+16 | 線在哪一路，0 = 正中間，負 = 左 | [範例 2](#ex2)、[3](#ex3) |
| `02` | 細位置 | −100～+100 | 線位置的細分，負 = 左 | [範例 3](#ex3)、[6](#ex6) |
| `03` | 線寬 | 0～16 路 | 壓到線的路數，0 = 找不到線 | [範例 3](#ex3) |
| `04` | 線段數 | 0～8 | 0 丟線、1 一條線、2 以上 = 分岔／十字 | [範例 5](#ex5)、[6](#ex6) |
| `05` | 壓線位元圖 | 0～65535（bit） | bit0 = 第 1 路（最左）… bit15 = 第 16 路，1 = 在線上 | [範例 5](#ex5)、[7](#ex7) |
| `06` | 最後離線方向 | −16～+16 | 丟線前線在哪一側，負 = 左 | [範例 6](#ex6) |
| `07` | 二值化門檻 | 1～99 | 判斷「在線上」的光值門檻，預設 50 | [範例 7](#ex7) |
| `08` | 校準完成次數 | 0～255 次 | 開機後完成校準的次數 | [範例 4](#ex4) |
| `10`～`1F` | 第 1～16 路光值 | 0～100 | 校準後的光值，0 = 黑、100 = 白；`10` = 第 1 路（最左） | [範例 2](#ex2)、[4](#ex4)、[7](#ex7) |
| `E0` | 切黑線（命令） | 無參數 | 追黑線（預設） | [範例 3](#ex3)、[6](#ex6) |
| `E1` | 切白線（命令） | 無參數 | 追白線，F3 bit1 = 1 | [範例 3](#ex3) |
| `E2` | 黑色校準（命令） | 無參數 | 把目前讀值記為黑，約 50 ms，存 EEPROM | [範例 4](#ex4) |
| `E3` | 白色校準（命令） | 無參數 | 把目前讀值記為白，約 50 ms，存 EEPROM | [範例 4](#ex4) |
| `E4` | 設定門檻（命令） | 1 個參數 1～99 | 門檻存 RAM，重開機回 50 | [範例 7](#ex7) |
| `E7` | 5 秒動態校準（命令） | 無參數 | 5 秒內在黑白上來回掃，取極值當參考；期間 F3 bit2 = 1 | [範例 4](#ex4)、[7](#ex7) |
| `F0` | 裝置 ID | — | 低 byte = 出廠位址 0x16、高 byte = 16（通道數） | [範例 1](#ex1)、[共用 2](#exi2)、[共用 3](#exi3) |
| `F1` | 協議版本 | 2 | 這份範例照版本 2 寫 | [範例 1](#ex1) |
| `F2` | 韌體版本 | 主×100＋次 | 例 102 = 1.2 | [範例 1](#ex1) |
| `F3` | 狀態旗標 | bit | b0 就緒、b1 白線模式、b2 校準中、b3 丟線、b4 分岔 | [範例 1](#ex1)、[4](#ex4) |
| `F4` | 更新序號 | 0～255 | 每 10 ms +1，一直跳 = 感應器活著 | [共用 2](#exi2) |
| `F5` | 開機秒數 | 0～32767 秒 | 變小 = 重開機過 | [範例 1](#ex1)、[共用 2](#exi2) |
| `F8` | 換 I2C 位址（命令） | `[新位址][取反]` | `[F8][00][FF]` = 回出廠；重開機後生效 | [共用 1](#exi1) |
| `EF` | 重新開機（命令） | `[A5]` | 送 `[EF][A5]` | [共用 1](#exi1)、[共用 2](#exi2) |
<!-- /gen -->

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟感應器打招呼](#ex1) | 確認接線正確，讀 F0～F3 身分證與狀態 | 無 |
| 2 | [一次看完所有讀值](#ex2) | 整張值表每一格是什麼 | 一張白紙＋黑色電工膠帶 |
| 3 | [線在哪裡？](#ex3) | 線位置、細位置、線寬 | 同上 |
| 4 | [一鍵校準](#ex4) | E2／E3／E7 校準，等 F3「校準中」歸零 | 同上 |
| 5 | [十字路口偵測](#ex5) | 線段數＋壓線位元圖數路口 | 畫了十字路口的紙 |
| 6 | [循線小車（進階）](#ex6) | 用細位置做比例控制、丟線用最後離線方向找回 | 兩輪小車、L298N 馬達驅動板 |
| 7 | [調整門檻](#ex7) | E4 設門檻、讀 07 確認 | 白紙＋黑膠帶 |
| 共 1 | [換 I2C 位址](#exi1) | F8 換位址／回出廠、EF 重開機 | 無 |
| 共 2 | [身分證＋重新開機](#exi2) | F0～F5 六格共用暫存器 | 無 |
| 共 3 | [掃描線上裝置](#exi3) | 掃 0x08～0x77、用 F0 認產品 | 無 |

> [!TIP]
> **建議順序：1 → 4 → 2 → 3 → 5 → 7 → 6。** 先確認接好（1），校準場地（4），之後的讀數才會準。

---

<h2 id="ex1">1️⃣ 跟感應器打招呼</h2>

**做什麼：** 確認感應器接好了，並讀出它的身分證：`F0` 裝置 ID（出廠位址＋通道數）、`F1` 協議版本、`F2` 韌體版本、`F3` 狀態旗標逐 bit 用中文印出來。每次接新的感應器，都先跑這一個。

**步驟：**
1. 照上面的表格接好 4 條線，USB 插上電腦。
2. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/line16/arduino/line16_01_hello/line16_01_hello.ino" download data-ignore="true">📄 下載 line16_01_hello.ino</a>

<!-- ino: examples/line16/arduino/line16_01_hello/line16_01_hello.ino -->
```cpp
/*
 * 循行者 16 路 × Arduino　範例 1：跟感應器打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認感應器有沒有接好。接好了，會印出它的身分證（裝置 ID、協議版本、
 *   韌體版本）和目前的狀態（有沒有看到線、是不是白線模式…）。
 *   每次接新感應器，都建議先跑這個範例。
 *
 * 你需要：循行者 16 路（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
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

const uint8_t ADDR = 0x16;               // 循行者的「門牌號碼」（I2C 位址），8 路與 16 路都是 0x16
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等感應器開機 0.5 秒

  Serial.println(F("=== 循行者 16 路：跟感應器打招呼 ==="));

  // 等感應器說「我準備好了」（狀態旗標 F3 的 bit0），最多等 2 秒
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器！請檢查："));
    Serial.println(F("  1. SDA 接 A4、SCL 接 A5（ESP32：21 / 22），有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？"));
    Serial.println(F("  3. 你的感應器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    while (true) {}                      // 找不到就停在這裡
  }

  // ---- F0 裝置 ID：低 byte = 出廠位址、高 byte = 通道數 ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t factoryAddr = (uint8_t)(id & 0xFF);        // 出廠位址（換過位址也不變）
  uint8_t channels    = (uint8_t)((id >> 8) & 0xFF); // hw[0] = 幾路
  Serial.print(F("找到了！出廠位址 0x"));
  Serial.print(factoryAddr, HEX);
  Serial.print(F("，通道數 "));
  Serial.print(channels);                // 循行者 16 路 = 16
  Serial.println(F(" 路"));

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
  Serial.print(F("  bit0 就緒      ")); Serial.println((st & 0x01) ? F("是") : F("否"));
  Serial.print(F("  bit1 白線模式  ")); Serial.println((st & 0x02) ? F("是（追白線）") : F("否（追黑線）"));
  Serial.print(F("  bit2 校準中    ")); Serial.println((st & 0x04) ? F("是") : F("否"));
  Serial.print(F("  bit3 丟線      ")); Serial.println((st & 0x08) ? F("是（看不到線）") : F("否（看得到線）"));
  Serial.print(F("  bit4 分岔      ")); Serial.println((st & 0x10) ? F("是（看到兩段以上）") : F("否"));

  // ---- F5 開機秒數（順便看它開機多久了） ----
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
=== 循行者 16 路：跟感應器打招呼 ===
找到了！出廠位址 0x16，通道數 16 路
協議版本：2
韌體版本：4.0
目前狀態：
  bit0 就緒      是
  bit1 白線模式  否（追黑線）
  bit2 校準中    否
  bit3 丟線      是（看不到線）
  bit4 分岔      否
開機秒數：12
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

**做什麼：** 每 0.5 秒把整張值表印一次：`01`～`08` 一段、`10`～`1F` 一段（各用一次 `mbcReadMany`）。先在這裡認識每一格，之後的範例只是各挑幾格來用。

**準備：** 在白紙上貼一條黑色電工膠帶（寬約 2 公分）當作線。

<a href="/examples/line16/arduino/line16_02_all_values/line16_02_all_values.ino" download data-ignore="true">📄 下載 line16_02_all_values.ino</a>

<!-- ino: examples/line16/arduino/line16_02_all_values/line16_02_all_values.ino -->
```cpp
/*
 * 循行者 16 路 × Arduino　範例 2：一次看完所有讀值
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把感應器「整張值表」每 0.5 秒印一次：
 *     01 線位置、02 細位置、03 線寬、04 線段數、05 壓線位元圖、
 *     06 最後離線方向、07 門檻、08 校準次數、10~1F 第 1~16 路光值。
 *   之後的範例只是各挑其中幾格來用，先在這裡把每一格認識一遍。
 *
 * 你需要：循行者 16 路（通用 I2C 版）＋ 一張白紙＋黑色電工膠帶（當作線）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。把黑線在感應器下面左右移動，看每一格怎麼變。
 *
 * 小提醒：
 *   連續的編號可以「一次讀一串」（mbcReadMany），讀到的是同一瞬間的值。
 *   一次最多 16 個，所以 01~08 讀一次、10~1F 再讀一次。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！把黑線在感應器下面移動看看。"));
}

void loop() {
  int16_t a[8];                          // 放 01~08 這 8 格
  int16_t light[16];                      // 放 10~1F 這 16 格（第 1~16 路光值）

  // 第一段：從 01 號起連續讀 8 個值（01~08）
  if (!mbcReadMany(ADDR, 0x01, 8, a)) { Serial.println(F("讀不到（第一段）")); delay(500); return; }
  // 第二段：從 10 號起連續讀 16 個值（10~1F）
  if (!mbcReadMany(ADDR, 0x10, 16, light)) { Serial.println(F("讀不到（第二段）")); delay(500); return; }

  Serial.println(F("---------------------------------------"));
  Serial.print(F("01 線位置       ")); Serial.print(a[0]); Serial.println(F("   （-16 左 ~ +16 右，0 = 正中間）"));
  Serial.print(F("02 細位置       ")); Serial.print(a[1]); Serial.println(F("   （-100 ~ +100，負 = 線在左）"));
  Serial.print(F("03 線寬         ")); Serial.print(a[2]); Serial.println(F(" 路（0 = 找不到線）"));
  Serial.print(F("04 線段數       ")); Serial.print(a[3]); Serial.println(F("   （0 丟線、1 一條線、2 以上 = 分岔／十字）"));
  Serial.print(F("05 壓線位元圖   "));
  for (uint8_t i = 0; i < 16; i++) Serial.print(((uint16_t)a[4] >> i) & 1);   // 左邊印 bit0（第 1 路）…右邊印 bit15（第 16 路）
  Serial.println(F("   （左→右 = 第 1~16 路，1 = 那一路在線上）"));
  Serial.print(F("06 最後離線方向 ")); Serial.print(a[5]); Serial.println(F("   （丟線前線在哪一側，負 = 左）"));
  Serial.print(F("07 門檻         ")); Serial.print(a[6]); Serial.println(F("   （1~99，預設 50）"));
  Serial.print(F("08 校準次數     ")); Serial.print(a[7]); Serial.println(F("   （開機後完成校準幾次）"));

  Serial.print(F("10~1F 光值 左 "));
  for (uint8_t i = 0; i < 16; i++) {      // 第 1 路（最左）先印
    if (light[i] < 10)  Serial.print(' ');
    if (light[i] < 100) Serial.print(' ');
    Serial.print(light[i]);
    Serial.print(' ');
  }
  Serial.println(F("右   （0 = 黑、100 = 白）"));

  delay(50);                             // 讀完值歇一下
  delay(450);                            // 再加一點，讓畫面每 0.5 秒更新一次，比較好看
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

**你會看到：**（線在中間偏左）

```text
---------------------------------------
01 線位置       -3   （-16 左 ~ +16 右，0 = 正中間）
02 細位置       -31   （-100 ~ +100，負 = 線在左）
03 線寬         2 路（0 = 找不到線）
04 線段數       1   （0 丟線、1 一條線、2 以上 = 分岔／十字）
05 壓線位元圖   0000111000000000   （左→右 = 第 1~16 路，1 = 那一路在線上）
06 最後離線方向 -3   （丟線前線在哪一側，負 = 左）
07 門檻         50   （1~99，預設 50）
08 校準次數     1   （開機後完成校準幾次）
10~1F 光值 左 100  98 100  99  12   8  15  95 100 100 100  99 100 100  98 100 右   （0 = 黑、100 = 白）
```

> [!TIP]
> **試試看：** 慢慢把感應器抬高，看光值怎麼變。抬太高時黑白分數會越來越接近，這就是為什麼感應器要貼近地面。

---

<h2 id="ex3">3️⃣ 線在哪裡？</h2>

**做什麼：** 讀 `01` 線位置、`02` 細位置、`03` 線寬三格，畫一把小尺用 `#` 標出線的位置，並告訴你該往哪邊轉。開頭用 `E0` 命令設成追黑線。

<a href="/examples/line16/arduino/line16_03_where_is_line/line16_03_where_is_line.ino" download data-ignore="true">📄 下載 line16_03_where_is_line.ino</a>

<!-- ino: examples/line16/arduino/line16_03_where_is_line/line16_03_where_is_line.ino -->
```cpp
/*
 * 循行者 16 路 × Arduino　範例 3：線在哪裡？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讀「01 線位置」「02 細位置」「03 線寬」三格，畫一把小尺用 # 標出線的位置，
 *   並告訴你該往哪邊轉。這三格就是循線最常用的資料。
 *
 * 你需要：循行者 16 路（通用 I2C 版）＋ 白紙＋黑色電工膠帶（寬約 2 公分）。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。把黑線從感應器左邊慢慢移到右邊。
 *
 * 左、右怎麼算？
 *   感應器裝在車上（感應面朝下），你站在車子後面往前看：
 *   線在左邊 = 負數、線在右邊 = 正數、0 = 正中間。第 1 路在最左。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  mbcWrite(ADDR, 0xE0);                  // E0 = 追黑線（白線請改成 0xE1）  👉 可以改
  Serial.println(F("準備好了！把黑線從左邊慢慢移到右邊。"));
}

void loop() {
  int16_t v[3];                          // v[0] = 01 線位置、v[1] = 02 細位置、v[2] = 03 線寬
  if (!mbcReadMany(ADDR, 0x01, 3, v)) {  // 從 01 號起一次讀 3 格
    Serial.println(F("讀不到感應器"));
    delay(500);
    return;
  }
  int16_t pos   = v[0];                  // -16（最左）~ +16（最右）
  int16_t fine  = v[1];                  // -100 ~ +100，比線位置更細
  int16_t width = v[2];                  // 壓到線的路數，0 = 看不到線

  // 畫一把 9 格的小尺：位置 -4 ~ +4 各一格（把 -16~+16 除以 4），# 代表線
  Serial.print('|');
  for (int8_t i = -4; i <= 4; i++) Serial.print((width > 0 && i == pos / 4) ? '#' : ' ');
  Serial.print(F("|  "));

  if (width == 0) {
    Serial.println(F("看不到線（線跑掉了）"));
  } else {
    Serial.print(F("位置 ")); Serial.print(pos);
    Serial.print(F("  細位置 ")); Serial.print(fine);
    Serial.print(F("  線寬 ")); Serial.print(width);
    if      (pos < -1) Serial.println(F("  → 線在左邊，要往左轉"));
    else if (pos >  1) Serial.println(F("  → 線在右邊，要往右轉"));
    else               Serial.println(F("  → 線在正中間，直走！"));
  }
  delay(50);
  delay(150);                            // 每 0.2 秒印一行，看得清楚   👉 可以改
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

**你會看到：**（把黑線從左邊慢慢移到右邊）

```text
|  #      |  位置 -5  細位置 -62  線寬 2  → 線在左邊，要往左轉
|    #    |  位置 -1  細位置 -8  線寬 2  → 線在正中間，直走！
|      #  |  位置 3  細位置 40  線寬 2  → 線在右邊，要往右轉
|         |  看不到線（線跑掉了）
```

**位置是什麼意思？**

| 位置數字 | 意思 |
| :---: | :--- |
| 0 | 線在正中間 |
| 負數（−1～−15） | 線在**左**邊，數字越小越偏左 |
| 正數（+1～+15） | 線在**右**邊，數字越大越偏右 |
| ±16 | 線剛從最左（−16）或最右（+16）跑掉（只維持 200 ms，平常最多 ±15） |

> [!TIP]
> **試試看：** 把 `mbcWrite(ADDR, 0xE0)` 改成 `0xE1`，再拿黑紙貼白膠帶，就變成循白線了。

---

<h2 id="ex4">4️⃣ 一鍵校準</h2>

**做什麼：** 讓感應器記住**這個場地**的黑色和白色。在序列埠打 `b`（`E2` 黑色校準）、`w`（`E3` 白色校準）或 `a`（`E7` 5 秒動態校準），程式會等 `F3` bit2「校準中」變回 0，再印 `08` 校準次數確認多了 1。校準結果存在感應器裡，關機也不會忘。

**步驟：**
1. 上傳程式，打開序列埠監控窗（115200）。
2. 把感應器放在線的上方，高度跟裝在車上一樣（離地約 0.5～1 公分）。
3. 最簡單：在輸入框打 `a`，按 Enter，**5 秒內**慢慢左右移動感應器，讓每一隻眼睛都掃過黑線和白底。
4. 要更準：感應器 16 隻眼睛全壓在黑線上打 `b`，再放到白底上打 `w`。

<a href="/examples/line16/arduino/line16_04_calibrate/line16_04_calibrate.ino" download data-ignore="true">📄 下載 line16_04_calibrate.ino</a>

<!-- ino: examples/line16/arduino/line16_04_calibrate/line16_04_calibrate.ino -->
```cpp
/*
 * 循行者 16 路 × Arduino　範例 4：一鍵校準
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓感應器記住「這個場地」的黑和白。在序列埠監控窗打指令：
 *     b  → 黑色校準（E2）：先把感應器 16 隻眼睛都壓在黑線上再打
 *     w  → 白色校準（E3）：先把感應器放在白底上再打
 *     a  → 5 秒動態校準（E7）：打完之後 5 秒內，把感應器在黑線和白底上來回掃
 *   校準結果存在感應器裡，關機也不會忘。換場地、燈光不一樣時都要重做。
 *
 * 你需要：循行者 16 路（通用 I2C 版）＋ 白紙＋黑色電工膠帶。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200，輸入框打 b、w 或 a 再按 Enter。
 *   程式會等「F3 bit2 校準中」變回 0，再印出「08 校準次數」確認多了 1。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("=== 循行者 16 路：一鍵校準 ==="));
  Serial.print(F("目前校準次數：")); Serial.println(mbcRead(ADDR, 0x08));
  Serial.println(F("打 b = 黑色校準、w = 白色校準、a = 5 秒動態校準，然後按 Enter"));
}

void loop() {
  if (!Serial.available()) { delay(50); return; }   // 沒有輸入就等
  char c = Serial.read();

  if (c == 'b') {
    Serial.println(F("黑色校準中（感應器要壓在黑線上）..."));
    mbcWrite(ADDR, 0xE2);                // E2 = 把現在看到的記成「黑」
    waitCalibDone(2000);                 // 大約 50 ms，最多等 2 秒
  } else if (c == 'w') {
    Serial.println(F("白色校準中（感應器要放在白底上）..."));
    mbcWrite(ADDR, 0xE3);                // E3 = 把現在看到的記成「白」
    waitCalibDone(2000);
  } else if (c == 'a') {
    Serial.println(F("5 秒動態校準開始！請把感應器在黑線和白底上來回掃..."));
    mbcWrite(ADDR, 0xE7);                // E7 = 5 秒內取最黑最白當參考
    waitCalibDone(8000);                 // 5 秒 + 一點餘裕
  } else {
    return;                              // Enter、空白等其他字元不理它
  }

  // 印出光值讓你確認校準後黑是 0 附近、白是 100 附近
  int16_t light[16];
  if (mbcReadMany(ADDR, 0x10, 16, light)) {
    Serial.print(F("現在 16 路光值：左 "));
    for (uint8_t i = 0; i < 16; i++) { Serial.print(light[i]); Serial.print(' '); }
    Serial.println(F("右"));
  }
  delay(50);
}

// 等「F3 bit2 校準中」變回 0，並印出校準次數
void waitCalibDone(uint16_t ms) {
  unsigned long start = millis();
  delay(20);                             // 先給感應器一點時間把旗標舉起來
  while (millis() - start < ms) {
    int16_t st = mbcRead(ADDR, 0xF3);
    if (st != MBC_NONE && !(st & 0x04)) break;   // bit2 = 0 → 做完了
    Serial.print('.');
    delay(100);
  }
  Serial.println();
  Serial.print(F("校準完成！累計次數：")); Serial.println(mbcRead(ADDR, 0x08));
  Serial.println(F("打 b / w / a 可以再做一次"));
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
=== 循行者 16 路：一鍵校準 ===
目前校準次數：0
打 b = 黑色校準、w = 白色校準、a = 5 秒動態校準，然後按 Enter
5 秒動態校準開始！請把感應器在黑線和白底上來回掃...
..................................................
校準完成！累計次數：1
打 b / w / a 可以再做一次
現在 16 路光值：左 100 99 97 100 98 100 99 100 100 99 97 100 98 100 99 100 右
```

> [!WARNING]
> 校準時**每一隻眼睛都要掃過黑線**。如果最旁邊的眼睛沒掃到，它會一直以為整個場地都是白的，循線時就會判斷錯誤。

---

<h2 id="ex5">5️⃣ 十字路口偵測</h2>

**做什麼：** 用 `04` 線段數和 `05` 壓線位元圖判斷路口：位元圖亮的路數很多（壓在橫線上）或線段數 ≥ 2（岔路）就算路口。用板子內建的小燈（標著 **L** 那顆）當訊號燈：看到線就亮、線跑掉就熄，遇到路口**快閃 3 下**並數「這是第幾個路口」。

**準備：** 在白紙上用黑膠帶貼一條直線，再橫貼 2～3 條當作路口。

<a href="/examples/line16/arduino/line16_05_crossroad/line16_05_crossroad.ino" download data-ignore="true">📄 下載 line16_05_crossroad.ino</a>

<!-- ino: examples/line16/arduino/line16_05_crossroad/line16_05_crossroad.ino -->
```cpp
/*
 * 循行者 16 路 × Arduino　範例 5：十字路口偵測
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用「04 線段數」和「05 壓線位元圖」判斷路口：
 *     - 線段數 ≥ 2 → 看到岔路（兩段以上的線）
 *     - 壓線位元圖 16 個 bit 幾乎全亮（線寬很大）→ 正壓在十字路口的橫線上
 *   遇到路口，板子內建的小燈（L）快閃 3 下，並數「這是第幾個路口」。
 *   比賽常常要「走到第 3 個路口右轉」，這個範例就是在學數路口。
 *
 * 你需要：循行者 16 路（通用 I2C 版）＋ 畫了十字路口的白紙。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   序列埠監控窗 115200。讓感應器沿著線經過路口，看燈和訊息。
 */

#include <Wire.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // ESP32 開發板的內建燈通常接在 GPIO 2
#endif

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const uint8_t CROSS_MIN_WIDTH = 12;       // 壓線位元圖裡亮的路數 ≥ 這個數就算十字路口   👉 可以改

int  crossCount = 0;                     // 數到第幾個路口
bool onCross    = false;                 // 現在是不是正在路口上（避免同一個路口數很多次）

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！讓感應器沿著線經過路口看看。"));
}

void loop() {
  int16_t v[2];                          // v[0] = 04 線段數、v[1] = 05 壓線位元圖
  if (!mbcReadMany(ADDR, 0x04, 2, v)) { delay(100); return; }
  int16_t groups = v[0];
  uint16_t bitmap = (uint16_t)v[1];      // bit0 = 第 1 路（最左）… bit15 = 第 16 路

  // 數一數位元圖裡有幾個 1（= 幾路壓在線上）
  uint8_t lit = 0;
  for (uint8_t i = 0; i < 16; i++) if (bitmap & (1u << i)) lit++;

  bool isCross = (lit >= CROSS_MIN_WIDTH) || (groups >= 2);   // 很寬 或 兩段以上 → 路口

  if (isCross && !onCross) {             // 剛「進入」路口才算一次
    onCross = true;
    crossCount++;
    Serial.print(F("第 ")); Serial.print(crossCount); Serial.print(F(" 個路口！  位元圖 "));
    for (uint8_t i = 0; i < 16; i++) Serial.print((bitmap >> i) & 1);   // 左→右 = 第 1~16 路
    Serial.print(F("  線段數 ")); Serial.println(groups);
    for (uint8_t k = 0; k < 3; k++) {    // 快閃 3 下
      digitalWrite(LED_BUILTIN, HIGH); delay(80);
      digitalWrite(LED_BUILTIN, LOW);  delay(80);
    }
  } else if (!isCross && onCross) {
    onCross = false;                     // 離開路口了，下一個路口才能再數
  }

  // 平常：看到線就亮燈、看不到就熄
  digitalWrite(LED_BUILTIN, lit > 0 ? HIGH : LOW);
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

**你會看到：**

```text
準備好了！讓感應器沿著線經過路口看看。
第 1 個路口！  位元圖 1111111111111111  線段數 1
第 2 個路口！  位元圖 1111100000011111  線段數 2
```

> [!TIP]
> **試試看：** 加一行 `if (crossCount == 3) { ... }`，讓第 3 個路口時做一件特別的事，例如燈一直閃。線比較細時把 `CROSS_MIN_WIDTH` 改小。

---

<h2 id="ex6">6️⃣ 循線小車（進階）</h2>

**做什麼：** 讓兩輪小車沿著黑線自己跑。用 `02` 細位置（−100～+100）當誤差，**偏越多修越多**（比例控制）；`04` 線段數 = 0 代表線跑掉了，就看 `06` 最後離線方向往那一邊原地找回來。一次 `mbcReadMany` 從 `01` 讀到 `06`。

**另外要準備：** 兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。

**馬達接線：**

| L298N | Arduino UNO / Nano / R4 | ESP32 | 控制 |
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

<a href="/examples/line16/arduino/line16_06_line_follower/line16_06_line_follower.ino" download data-ignore="true">📄 下載 line16_06_line_follower.ino</a>

<!-- ino: examples/line16/arduino/line16_06_line_follower/line16_06_line_follower.ino -->
```cpp
/*
 * 循行者 16 路 × Arduino　範例 6（進階）：循線小車
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   讓兩輪小車沿著黑線自己跑。用「02 細位置」（-100 ~ +100）當誤差，
 *   線偏左就往左修、偏右就往右修，偏越多修越多（比例控制）。
 *   線跑掉時（04 線段數 = 0），看「06 最後離線方向」往那一邊原地找線。
 *
 * 你需要：
 *   兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612 等）、電池。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   （ESP32 改接：ENA → 25、IN1 → 26、IN2 → 27、ENB → 14、IN3 → 32、IN4 → 33）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   感應器裝在車頭正中間，離地面約 0.5～1 公分。
 *
 * 第一次跑之前：
 *   1. 先跑「範例 4：一鍵校準」校準場地。
 *   2. 把車子架高（輪子懸空）上傳，確認兩個輪子都是「往前」轉。
 *      哪個輪子轉反了，就把 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

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
const int   BASE_SPEED   = 120;          // 直走的速度：0 ~ 255，先慢一點比較好調
const float KP           = 1.0;          // 修正力道：車子修不回來就加大，左右搖擺就減小
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
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  mbcWrite(ADDR, 0xE0);                  // E0 = 循黑線（白線請改成 0xE1）

  Serial.println(F("3 秒後出發！"));
  delay(3000);
}

void loop() {
  int16_t v[6];                          // 01 線位置 02 細位置 03 線寬 04 線段數 05 位元圖 06 最後離線方向
  if (!mbcReadMany(ADDR, 0x01, 6, v)) {  // 讀不到感應器 → 為了安全，先停車
    drive(0, 0);
    delay(50);
    return;
  }

  int16_t error   = v[1];                // 02 細位置：-100（很左）~ +100（很右），0 = 正中間
  int16_t groups  = v[3];                // 04 線段數：0 = 看不到線
  int16_t lastDir = v[5];                // 06 最後離線方向：負 = 線最後在左邊

  if (groups == 0) {
    // 線跑掉了：往線最後消失的那一邊原地轉，把線找回來
    if (lastDir < 0) drive(-SEARCH_SPEED, SEARCH_SPEED);   // 線在左邊 → 原地左轉
    else             drive(SEARCH_SPEED, -SEARCH_SPEED);   // 線在右邊 → 原地右轉
    delay(50);
    return;
  }

  // 比例控制：偏多少，就修多少
  int turn = KP * error;                 // 線在右邊（正數）→ turn 是正的 → 左輪快、右輪慢 → 往右轉
  drive(BASE_SPEED + turn, BASE_SPEED - turn);
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

**怎麼調整，讓車子跑得更好：**

| 車子的樣子 | 怎麼改 |
| :--- | :--- |
| 轉彎時衝出線外，修不回來 | `KP` 加大（例如 1.0 → 1.5），或 `BASE_SPEED` 調小 |
| 在直線上一直左右搖擺 | `KP` 減小（例如 1.0 → 0.6） |
| 跑得很穩，想更快 | `BASE_SPEED` 一次加 20，再重新調 `KP` |
| 一出發就原地打轉 | 有一個輪子轉反了，改 `LEFT_REVERSE` 或 `RIGHT_REVERSE` |

---

<h2 id="ex7">7️⃣ 調整門檻</h2>

**做什麼：** 「門檻」是感應器判斷「這一路在不在線上」的分數線（光值比門檻低就算在線上，預設 50）。在序列埠打 1～99 用 `E4` 設定，程式讀回 `07` 確認；打 `a` 做 `E7` 5 秒動態校準。同時每秒印 16 路光值和 `05` 壓線位元圖，讓你看門檻改了之後哪幾路算「在線上」。

<a href="/examples/line16/arduino/line16_07_threshold/line16_07_threshold.ino" download data-ignore="true">📄 下載 line16_07_threshold.ino</a>

<!-- ino: examples/line16/arduino/line16_07_threshold/line16_07_threshold.ino -->
```cpp
/*
 * 循行者 16 路 × Arduino　範例 7：調整門檻
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   「門檻」是感應器判斷「這一路在不在線上」的分數線：光值（0 黑 ~ 100 白）
 *   比門檻低就算在線上。預設 50。線很淡、場地反光時可以調。
 *   在序列埠監控窗打：
 *     數字 1~99 再按 Enter → 用 E4 設定門檻，然後讀 07 確認
 *     a                    → 用 E7 做 5 秒動態校準（門檻不準常常是沒校準）
 *   同時每秒印一次 16 路光值和壓線位元圖，讓你看門檻改了之後哪幾路算「在線上」。
 *
 * 你需要：循行者 16 路（通用 I2C 版）＋ 白紙＋黑色電工膠帶。
 *
 * 接線：
 *   感應器 VCC → 5V      感應器 GND → GND
 *   感應器 SDA → A4      感應器 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 小提醒：
 *   E4 設定的門檻存在 RAM，感應器重開機會回到預設 50。
 */

#include <Wire.h>

const uint8_t ADDR = 0x16;               // 循行者的門牌號碼（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 2000)) {
    Serial.println(F("找不到感應器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("=== 循行者 16 路：調整門檻 ==="));
  Serial.print(F("目前門檻（07）：")); Serial.println(mbcRead(ADDR, 0x07));
  Serial.println(F("打 1~99 設定門檻、打 a 做 5 秒動態校準，然後按 Enter"));
}

void loop() {
  // ---- 處理輸入 ----
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s == "a") {
      Serial.println(F("5 秒動態校準開始！把感應器在黑線和白底上來回掃..."));
      mbcWrite(ADDR, 0xE7);              // E7 = 5 秒動態校準
      unsigned long start = millis();
      delay(20);
      while (millis() - start < 8000) {  // 等 F3 bit2「校準中」變回 0
        int16_t st = mbcRead(ADDR, 0xF3);
        if (st != MBC_NONE && !(st & 0x04)) break;
        delay(100);
      }
      Serial.println(F("校準完成"));
    } else if (s.length() > 0) {
      int n = s.toInt();
      if (n >= 1 && n <= 99) {
        uint8_t th = (uint8_t)n;
        mbcWrite(ADDR, 0xE4, &th, 1);    // E4 + 1 個參數 = 設定門檻
        delay(20);
        Serial.print(F("門檻設為 ")); Serial.print(n);
        Serial.print(F("，讀回 07 = ")); Serial.println(mbcRead(ADDR, 0x07));
      } else {
        Serial.println(F("請打 1~99 的數字，或 a"));
      }
    }
  }

  // ---- 每秒印一次光值與位元圖 ----
  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    int16_t light[16];
    int16_t bitmap = mbcRead(ADDR, 0x05);
    if (mbcReadMany(ADDR, 0x10, 16, light) && bitmap != MBC_NONE) {
      Serial.print(F("光值 左 "));
      for (uint8_t i = 0; i < 16; i++) { Serial.print(light[i]); Serial.print(' '); }
      Serial.print(F("右  在線上(左→右) "));
      for (uint8_t i = 0; i < 16; i++) Serial.print(((uint16_t)bitmap >> i) & 1 ? '#' : '.');   // 第 1 路（bit0）印最左
      Serial.println();
    }
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

**你會看到：**

```text
=== 循行者 16 路：調整門檻 ===
目前門檻（07）：50
打 1~99 設定門檻、打 a 做 5 秒動態校準，然後按 Enter
光值 左 100 98 100 99 12 8 35 100 100 100 99 100 100 98 100 99 右  在線上(左→右) ....###.........
門檻設為 30，讀回 07 = 30
光值 左 100 98 100 99 12 8 35 100 100 100 99 100 100 98 100 99 右  在線上(左→右) ....##..........
```

> [!TIP]
> 線很淡（灰色）時把門檻調高、場地反光時調低。`E4` 設的門檻存在 RAM，感應器重開機會回到 50；想長久有效，程式開頭每次都送一次 `E4`。

---

## 🔧 共用命令範例（所有 MBC 通用 I2C 產品都一樣）

這三支放在 `examples/i2c/arduino/`，循行者、測距者、掌控者、IMU、神攝手都能用；程式開頭的 `ADDR`／`OLD_ADDR` 改成你的產品位址即可。

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

**做什麼：** 掃 `0x08`～`0x77`，有回應的就讀 `F0` 認出是哪一款 MBC 產品、韌體幾版、位址有沒有換過。接線有沒有通一跑就知道。

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
  0x16  循行者 16 路  韌體 4.0  協議 v2
共 1 個裝置
```

---

## ❓ 常見問題

**Q：一直出現「找不到感應器」？**
1. 檢查 SDA、SCL 有沒有接反（UNO：SDA → A4、SCL → A5；ESP32：21 / 22）。
2. 檢查 VCC、GND 有沒有插緊。感應器上的燈有沒有亮？
3. 確認是 **通用 I2C 版**：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
4. 跑 [共用 3 掃描](#exi3)，看線上到底有沒有裝置、位址是不是被換過。

**Q：讀到 `-32768` 代表什麼？**
`-32768`（程式裡叫 `MBC_NONE`）有兩種意思：① 小幫手讀不到感應器（接線鬆了、位址錯了）；② 那個編號是空號——例如 16 路只有 `10`～`1F`，去讀 `20` 就會拿到 `-32768`。正常的值不可能是這個數字，所以可以直接拿來判斷「這筆資料不能用」。

**Q：ESP32 讀不到、或讀到的值怪怪的？**
MBC 有些產品（掌控者 PS4、神攝手）本身是 ESP32 做的，不能「時脈延展」，主機寫完編號要**等 2 ms** 再讀。小幫手 `mbcReadMany` 裡已經有 `delay(2)`，請不要刪掉；自己寫程式時也要照做。ESP32 當主機時 I2C 腳位預設是 GPIO 21（SDA）／22（SCL）。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：線明明在中間，位置卻一直跳？**
先跑 [範例 4](#ex4) 校準。再檢查感應器離地面的高度（0.5～1 公分最好）。

**Q：可以同時接兩個循行者嗎？**
可以，但要先用 [共用 1](#exi1) 把其中一個換成別的位址（例如 `0x17`）。沒換的話兩個都是 `0x16`，同一組 SDA/SCL 上會打架。位址不同的 MBC 產品（例如陀螺儀 `0x6A`）本來就能接在同一組線上。

**Q：想知道每一個格子放什麼資料？**
上面的 [指令總覽表](#cmd-table) 就是完整清單；更細的封包格式請看 [I2C 暫存器說明](/sensors/line16/arduino-i2c.md)。
