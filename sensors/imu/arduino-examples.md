<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# 🧪 陀螺儀 IMU × Arduino 範例

[🔙 回到陀螺儀 IMU 首頁](/sensors/imu/index.md)　｜　[📡 MBC 通用 I2C 協議 v2（進階）](/i2c-protocol.md)

這一頁有 **8 個範例**，從「確認接好了沒」一路做到「小車原地轉 90 度」和「校準」。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/imu/arduino/imu_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 陀螺儀 | Arduino UNO / Nano | Arduino UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: |
| VCC | 5V | 5V | 3V3 或 5V（VIN） |
| GND | GND | GND | GND |
| SDA | A4 | SDA | GPIO 21 |
| SCL | A5 | SCL | GPIO 22 |

<div style="text-align: center;">
<img src="/images/figures/imu/wiring-uno.svg" alt="陀螺儀 IMU 接 Arduino UNO 的接線圖" style="max-width: 520px; width: 100%; margin: 1rem auto; display: block;" />
</div>

> [!IMPORTANT]
> - 請用 **通用 I2C 版** 的陀螺儀。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 陀螺儀**開機時要先放著不動約 0.5 秒**，它會自己記住「哪一面朝上」並把航向歸零。
> - 遠離馬達、喇叭、鐵桌：磁力計會被干擾（範例 6 可以看干擾度）。
> - 接線前先拔掉 USB 線。

---

<h2 id="cmds">📖 指令總覽（這個陀螺儀能讀到什麼）</h2>

陀螺儀就像一排有編號的「格子」，每一格放一個數字（int16）。寫 `[編號]` 再讀 2 個 byte 就拿到一個值；連續的編號可以一次讀一串（最多 16 個）。範例裡的 `mbcRead(ADDR, 編號)`、`mbcReadMany(ADDR, 起始編號, 幾個, 陣列)` 就是在做這件事。

<!-- gen:cmd-table -->
| 編號 | 名稱 | 範圍／單位 | 說明 | 出現在 |
| :---: | :--- | :--- | :--- | :--- |
| `01` | 陀螺 3D 航向 | 度 | 用三軸算的航向，開機／歸零 = 0；從上往下看**右轉（順時針）變大**，會一直累加（轉兩圈 = 720），到 ±32767 繞回 | 範例 2、3 |
| `02` | 融合航向 | 度 | 陀螺 3D 再用磁力計慢慢修正，久了不會漂 | 範例 2、3、7 |
| `03` | 陀螺 1D 航向 | 度 | 只用轉向那一軸積分 | 範例 2、3 |
| `04` | 磁力航向 | 度 | 只看磁力計（像指南針，會被鐵器干擾） | 範例 2、3 |
| `05` | 轉向軸角速度 | 0.1 度/秒 | 繞轉向軸轉多快（123 = 12.3 度/秒），正負跟航向同向 | 範例 2、4 |
| `06` | 俯仰角 | 度 | 板子繞絲印 X 軸翹起幾度，平放 = 0 | 範例 2、4 |
| `07` | 俯仰角速度 | 0.1 度/秒 | 俯仰角變化多快 | 範例 2、4 |
| `08` | 磁力信任度 | 0~255 | 融合演算法多相信磁力計，越高越好 | 範例 2、6 |
| `09` | 磁場干擾度 | 0~255 | 附近鐵器、馬達的干擾，越低越好 | 範例 2、6 |
| `0A` | 朝上的面 | 0~5 | 0 +X、1 +Y、2 +Z（平放）、3 −X、4 −Y、5 −Z | 範例 2、6 |
| `10`~`12` | 角速度 X／Y／Z | 0.1 度/秒 | 實體軸原始值 | 範例 2、5 |
| `13`~`15` | 加速度 X／Y／Z | mg | 1000 = 1 g，平放時 Z 約 1000 | 範例 2、5 |
| `16`~`18` | 磁力 X／Y／Z | mG | 磁場強度（毫高斯） | 範例 2、5 |
| `F0` | 裝置 ID | — | 低 byte = 出廠位址 0x6A、高 byte = 1 | 範例 1 |
| `F1` | 協議版本 | 2 | 這份範例照版本 2 寫 | 範例 1 |
| `F2` | 韌體版本 | 主×100＋次 | 例如 102 = 1.2 | 範例 1 |
| `F3` | 狀態旗標 | bit | b0 就緒、b1 靜止、b2 已擷取安裝軸、b3 安裝過歪、b4 加速度計校準中、b5 磁力計校準中、b6 磁力融合已初始化、b7 磁力自動校準已收斂 | 範例 1、8 |
| `F5` | 開機秒數 | 0~32767 | 數字變小 = 重開機過 | 範例 1 |
| `E0`（寫） | 航向歸零 | 無參數 | 四個航向都歸零 | 範例 3、7 |
| `E1`／`E2`（寫） | 加速度計校準開始／結束 | 無參數 | 期間 F3 b4 = 1；結束存 EEPROM 並自動歸零航向 | 範例 8 |
| `E3`／`E4`（寫） | 磁力計校準開始／結束 | 無參數 | 期間 F3 b5 = 1；結束存 EEPROM | 範例 8 |
<!-- /gen -->

> [!NOTE]
> 沒有的編號（例如 `0B`~`0F`）會讀到 `-32768`（範例裡叫 `MBC_NONE`），讀不到陀螺儀也是 `-32768`。完整規格見 [MBC 通用 I2C 協議 v2](/i2c-protocol.md)。

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟陀螺儀打招呼](#ex1) | 確認接線正確、讀身分證、F3 的 8 個 bit | 無 |
| 2 | [把所有的值一次印出來](#ex2) | 看懂每個編號放什麼（01~0A、10~18） | 無 |
| 3 | [我轉了幾度？](#ex3) | 四種航向、E0 歸零 | 無 |
| 4 | [轉多快？翹多高？](#ex4) | 角速度、俯仰角 | 一本書（墊高一邊） |
| 5 | [九軸原始值](#ex5) | 角速度／加速度／磁力 XYZ | 一塊磁鐵（選用） |
| 6 | [哪一面朝上？](#ex6) | 朝上的面、磁力信任度／干擾度 | 無 |
| 7 | [小車原地轉 90 度（進階）](#ex7) | 用航向控制轉彎 | 兩輪小車、L298N 馬達驅動板 |
| 8 | [校準加速度計與磁力計](#ex8) | 寫入命令 E1~E4、看 F3 | 無 |

> [!TIP]
> **建議順序：1 → 2 → 3 → 4 → 6 → 5 → 7 → 8。** 7 要另外準備零件；8 只有航向漂得很快時才需要。

---

<h2 id="ex1">1️⃣ 跟陀螺儀打招呼</h2>

**做什麼：** 確認陀螺儀接好了，印出它的「身分證」（出廠位址、協議版本、韌體版本）和狀態旗標 F3 的 8 個 bit。每次接新的陀螺儀，都先跑這一個。

**步驟：**
1. 照上面的表格接好 4 條線，USB 插上電腦。
2. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/imu/arduino/imu_01_hello/imu_01_hello.ino" download data-ignore="true">📄 下載 imu_01_hello.ino</a>

<!-- ino: examples/imu/arduino/imu_01_hello/imu_01_hello.ino -->
```cpp
/*
 * IMU 陀螺儀 × Arduino　範例 1：跟陀螺儀打招呼
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   確認陀螺儀有沒有接好。接好了，會印出它的身分證（裝置 ID、協議版本、
 *   韌體版本）和狀態旗標 F3 的 8 個 bit（靜止中？校準中？安裝有沒有歪？）。
 *   每次接新的陀螺儀，都建議先跑這個範例。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟陀螺儀講話

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等陀螺儀開機 0.5 秒

  Serial.println(F("=== IMU 陀螺儀：跟陀螺儀打招呼 ==="));

  // 等陀螺儀說「我準備好了」（狀態旗標 F3 的 bit0），最多等 3 秒
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀！請檢查："));
    Serial.println(F("  1. SDA 接 A4、SCL 接 A5（ESP32：21 / 22），有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？"));
    Serial.println(F("  3. 你的陀螺儀是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    while (true) {}                      // 找不到就停在這裡
  }

  // ---- F0 裝置 ID：低 byte = 出廠位址（0x6A）、高 byte = hw[0]（IMU = 1） ----
  int16_t id = mbcRead(ADDR, 0xF0);
  Serial.print(F("找到了！出廠位址 0x"));
  Serial.print((uint8_t)(id & 0xFF), HEX);
  Serial.print(F("，款式代號 "));
  Serial.println((uint8_t)((id >> 8) & 0xFF));   // IMU = 1

  // ---- F1 協議版本（這份範例是照版本 2 寫的） ----
  Serial.print(F("協議版本："));
  Serial.println(mbcRead(ADDR, 0xF1));

  // ---- F2 韌體版本：主版×100 + 次版（例如 102 = 1.2） ----
  int16_t fw = mbcRead(ADDR, 0xF2);
  Serial.print(F("韌體版本："));
  Serial.print(fw / 100);
  Serial.print('.');
  Serial.println(fw % 100);

  // ---- F3 狀態旗標：8 個 bit，一個 bit 代表一件事 ----
  int16_t st = mbcRead(ADDR, 0xF3);
  Serial.println(F("目前狀態："));
  printFlags(st);

  // ---- F5 開機秒數 ----
  Serial.print(F("開機秒數："));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("接線正確，可以開始玩其他範例了！"));
}

void loop() {
  // 這個範例只要在開機時檢查一次，所以 loop 裡什麼都不做
}

// 把 F3 的 8 個 bit 一行一行印出來
void printFlags(int16_t st) {
  Serial.print(F("  bit0 就緒              ")); Serial.println((st & 0x01) ? F("是") : F("否"));
  Serial.print(F("  bit1 靜止中            ")); Serial.println((st & 0x02) ? F("是") : F("否（正在動）"));
  Serial.print(F("  bit2 已擷取安裝軸      ")); Serial.println((st & 0x04) ? F("是（知道哪面朝上）") : F("否（放著不動等一下）"));
  Serial.print(F("  bit3 安裝過歪          ")); Serial.println((st & 0x08) ? F("是（請放平一點）") : F("否"));
  Serial.print(F("  bit4 加速度計校準中    ")); Serial.println((st & 0x10) ? F("是") : F("否"));
  Serial.print(F("  bit5 磁力計校準中      ")); Serial.println((st & 0x20) ? F("是") : F("否"));
  Serial.print(F("  bit6 磁力融合已初始化  ")); Serial.println((st & 0x40) ? F("是（融合航向可用）") : F("否"));
  Serial.print(F("  bit7 磁力自動校準收斂  ")); Serial.println((st & 0x80) ? F("是") : F("否"));
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
=== IMU 陀螺儀：跟陀螺儀打招呼 ===
找到了！出廠位址 0x6A，款式代號 1
協議版本：2
韌體版本：1.2
目前狀態：
  bit0 就緒              是
  bit1 靜止中            是
  bit2 已擷取安裝軸      是（知道哪面朝上）
  bit3 安裝過歪          否
  bit4 加速度計校準中    否
  bit5 磁力計校準中      否
  bit6 磁力融合已初始化  是（融合航向可用）
  bit7 磁力自動校準收斂  否
開機秒數：2
接線正確，可以開始玩其他範例了！
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟陀螺儀「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(ADDR, 編號)`：讀一個值（讀不到回 `-32768`）
> - `mbcReadMany(ADDR, 起始編號, 幾個, 陣列)`：從某個編號起一次讀一串（最多 16 個，同一瞬間的值）
> - `mbcWrite(ADDR, 命令)`：送命令（例如 `0xE0` 歸零）
> - `mbcWaitReady(ADDR, 毫秒)`：等陀螺儀開機好

---

<h2 id="ex2">2️⃣ 把所有的值一次印出來</h2>

**做什麼：** 每 0.5 秒把值表裡的每一個值都印出來：`01`~`0A` 一次讀 10 個、`10`~`18` 一次讀 9 個。轉一轉、翻一翻，看哪一行在變。

<a href="/examples/imu/arduino/imu_02_all_values/imu_02_all_values.ino" download data-ignore="true">📄 下載 imu_02_all_values.ino</a>

<!-- ino: examples/imu/arduino/imu_02_all_values/imu_02_all_values.ino -->
```cpp
/*
 * IMU 陀螺儀 × Arduino　範例 2：把所有的值一次印出來
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.5 秒把陀螺儀「值表」裡的每一個值都印出來，讓你看懂每個編號放什麼：
 *     01~0A：四種航向、角速度、俯仰角、磁力信任度／干擾度、朝上的面（一次讀 10 個）
 *     10~18：角速度、加速度、磁力各 X/Y/Z（一次讀 9 個）
 *   連續的編號可以「一次讀一串」（mbcReadMany），讀到的是同一瞬間的值。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。轉一轉、翻一翻陀螺儀，看哪些數字在變。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;             // 上一次印的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！每 0.5 秒印一次所有的值。"));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 可以改：每 0.5 秒印一次
    lastPrint = millis();

    // ---- 第一段：編號 01~0A，一次讀 10 個值 ----
    int16_t a[10];
    if (!mbcReadMany(ADDR, 0x01, 10, a)) { Serial.println(F("讀不到（第一段）")); return; }
    // ---- 第二段：編號 10~18，一次讀 9 個值 ----
    int16_t r[9];
    if (!mbcReadMany(ADDR, 0x10, 9, r)) { Serial.println(F("讀不到（第二段）")); return; }

    Serial.println(F("------------------------------"));
    Serial.print(F("01 陀螺 3D 航向（度）        ：")); Serial.println(a[0]);
    Serial.print(F("02 融合航向（度）            ：")); Serial.println(a[1]);
    Serial.print(F("03 陀螺 1D 航向（度）        ：")); Serial.println(a[2]);
    Serial.print(F("04 磁力航向（度）            ：")); Serial.println(a[3]);
    Serial.print(F("05 轉向軸角速度（0.1 度/秒） ：")); Serial.println(a[4]);
    Serial.print(F("06 俯仰角（度）              ：")); Serial.println(a[5]);
    Serial.print(F("07 俯仰角速度（0.1 度/秒）   ：")); Serial.println(a[6]);
    Serial.print(F("08 磁力信任度 0~255          ：")); Serial.println(a[7]);
    Serial.print(F("09 磁場干擾度 0~255          ：")); Serial.println(a[8]);
    Serial.print(F("0A 朝上的面 0~5              ：")); Serial.println(a[9]);
    Serial.print(F("10~12 角速度 X/Y/Z（0.1 度/秒）：")); print3(r[0], r[1], r[2]);
    Serial.print(F("13~15 加速度 X/Y/Z（mg）       ：")); print3(r[3], r[4], r[5]);
    Serial.print(F("16~18 磁力 X/Y/Z（mG）         ：")); print3(r[6], r[7], r[8]);
  }
  delay(50);
}

// 三個數字印成一行
void print3(int16_t x, int16_t y, int16_t z) {
  Serial.print(x); Serial.print(F("  ")); Serial.print(y); Serial.print(F("  ")); Serial.println(z);
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

**你會看到：**（平放不動）

```text
準備好了！每 0.5 秒印一次所有的值。
------------------------------
01 陀螺 3D 航向（度）        ：0
02 融合航向（度）            ：0
03 陀螺 1D 航向（度）        ：0
04 磁力航向（度）            ：1
05 轉向軸角速度（0.1 度/秒） ：0
06 俯仰角（度）              ：0
07 俯仰角速度（0.1 度/秒）   ：0
08 磁力信任度 0~255          ：230
09 磁場干擾度 0~255          ：12
0A 朝上的面 0~5              ：2
10~12 角速度 X/Y/Z（0.1 度/秒）：0  1  0
13~15 加速度 X/Y/Z（mg）       ：3  -5  1002
16~18 磁力 X/Y/Z（mG）         ：210  -95  -380
```

---

<h2 id="ex3">3️⃣ 我轉了幾度？（四種航向）</h2>

**做什麼：** 每 0.1 秒印出四種航向（`01`~`04` 一次讀 4 個）。從上往下看**右轉（順時針）數字變大**，會一直累加，可以變負數。在序列埠監控窗輸入 `z` 再按 Enter，會送命令 `E0` 把四個航向都歸零。

<a href="/examples/imu/arduino/imu_03_heading/imu_03_heading.ino" download data-ignore="true">📄 下載 imu_03_heading.ino</a>

<!-- ino: examples/imu/arduino/imu_03_heading/imu_03_heading.ino -->
```cpp
/*
 * IMU 陀螺儀 × Arduino　範例 3：我轉了幾度？（四種航向）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.1 秒印出四種航向（編號 01~04，一次讀 4 個）：
 *     01 陀螺 3D 航向：用全部三軸算的，最常用
 *     02 融合航向：陀螺 3D 再用磁力計慢慢修正，久了不會漂
 *     03 陀螺 1D 航向：只用轉向那一軸積分
 *     04 磁力航向：只看磁力計（像指南針，會被鐵器干擾）
 *   從上往下看「順時針」（右轉）數字變大，會一直累加（轉兩圈 = 720），可以變負數。
 *   在序列埠監控窗輸入 z 再按 Enter，會送命令 E0 把四個航向都歸零。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。把陀螺儀平放，慢慢轉一圈，看數字。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 *   要 0~359 的話自己換算：((h % 360) + 360) % 360。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！轉轉看陀螺儀。輸入 z + Enter 可以歸零。"));
  Serial.println(F("  陀螺3D   融合   陀螺1D   磁力"));
}

void loop() {
  // ---- 1. 電腦有送 z 過來嗎？有就送命令 E0（航向歸零，沒有參數） ----
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'z' || ch == 'Z') {
      mbcWrite(ADDR, 0xE0);              // 命令 E0：四個航向都歸零
      Serial.println(F(">>> 已歸零"));
    }
  }

  // ---- 2. 每 0.1 秒讀編號 01~04 四個航向 ----
  if (millis() - lastPrint >= 100) {     // 👉 可以改：每 0.1 秒印一次
    lastPrint = millis();
    int16_t h[4];
    if (!mbcReadMany(ADDR, 0x01, 4, h)) { Serial.println(F("讀不到陀螺儀")); return; }
    char line[48];
    snprintf(line, sizeof(line), "%7d %7d %7d %7d", h[0], h[1], h[2], h[3]);   // 每個數字佔 7 格，排整齊
    Serial.println(line);
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

**你會看到：**（右轉約 90 度，再輸入 z）

```text
準備好了！轉轉看陀螺儀。輸入 z + Enter 可以歸零。
  陀螺3D   融合   陀螺1D   磁力
      0       0       0       1
     45      45      44      47
     90      90      89      93
>>> 已歸零
      0       0       0       0
```

> [!TIP]
> - 平常用 `02` 融合航向就好：轉得快時跟陀螺一樣準，放久了也不會漂。
> - 要 0~359 的話自己換算：`((h % 360) + 360) % 360`。
> - 三種航向都是「相對值」（開機或歸零的方向 = 0），不是指北的羅盤角度；`04` 磁力航向也是減去零點。

---

<h2 id="ex4">4️⃣ 轉多快？翹多高？（角速度與俯仰角）</h2>

**做什麼：** 每 0.2 秒印出 `05` 轉向軸角速度、`06` 俯仰角、`07` 俯仰角速度（一次讀 3 個）。角速度單位是 0.1 度/秒，程式會印成 `12.3` 這種格式；轉超過 30 度/秒會提醒「轉太快」。

<a href="/examples/imu/arduino/imu_04_rate_tilt/imu_04_rate_tilt.ino" download data-ignore="true">📄 下載 imu_04_rate_tilt.ino</a>

<!-- ino: examples/imu/arduino/imu_04_rate_tilt/imu_04_rate_tilt.ino -->
```cpp
/*
 * IMU 陀螺儀 × Arduino　範例 4：轉多快？翹多高？（角速度與俯仰角）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.2 秒印出編號 05~07（一次讀 3 個）：
 *     05 轉向軸角速度：繞「轉向軸」轉多快，單位 0.1 度/秒（123 = 12.3 度/秒），正負跟航向同向
 *     06 俯仰角：板子繞絲印 X 軸翹起幾度（平放 = 0）
 *     07 俯仰角速度：俯仰角變化多快，單位 0.1 度/秒
 *   轉向軸角速度超過 30 度/秒時會提醒「轉太快」。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。原地轉陀螺儀看 05；把一邊翹起來看 06。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int FAST_DPS = 30;                 // 👉 可以改：超過幾度/秒算「轉太快」

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！轉轉看、翹翹看。"));
}

void loop() {
  if (millis() - lastPrint >= 200) {     // 👉 可以改：每 0.2 秒印一次
    lastPrint = millis();

    // 一次讀編號 05~07 共 3 個值：[0] 轉向軸角速度 [1] 俯仰角 [2] 俯仰角速度
    int16_t v[3];
    if (!mbcReadMany(ADDR, 0x05, 3, v)) { Serial.println(F("讀不到陀螺儀")); return; }

    Serial.print(F("轉向角速度 "));
    Serial.print(v[0] / 10); Serial.print('.'); Serial.print(abs(v[0] % 10));   // 0.1 度/秒 → 印成 12.3
    Serial.print(F(" 度/秒 | 俯仰角 "));
    Serial.print(v[1]);
    Serial.print(F(" 度 | 俯仰角速度 "));
    Serial.print(v[2] / 10); Serial.print('.'); Serial.print(abs(v[2] % 10));
    Serial.print(F(" 度/秒"));
    if (abs(v[0]) > FAST_DPS * 10) Serial.print(F("   <<< 轉太快！"));
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

**你會看到：**（平放，然後快速右轉，然後把一邊墊在書上）

```text
準備好了！轉轉看、翹翹看。
轉向角速度 0.0 度/秒 | 俯仰角 0 度 | 俯仰角速度 0.0 度/秒
轉向角速度 85.3 度/秒 | 俯仰角 0 度 | 俯仰角速度 0.2 度/秒   <<< 轉太快！
轉向角速度 0.1 度/秒 | 俯仰角 14 度 | 俯仰角速度 0.0 度/秒
```

---

<h2 id="ex5">5️⃣ 九軸原始值</h2>

**做什麼：** 每 0.2 秒印出 `10`~`18` 的九個原始值（一次讀 9 個）：角速度 XYZ（0.1 度/秒）、加速度 XYZ（mg，1000 = 1 g）、磁力 XYZ（mG）。做自己的演算法時用。

<a href="/examples/imu/arduino/imu_05_raw_9axis/imu_05_raw_9axis.ino" download data-ignore="true">📄 下載 imu_05_raw_9axis.ino</a>

<!-- ino: examples/imu/arduino/imu_05_raw_9axis/imu_05_raw_9axis.ino -->
```cpp
/*
 * IMU 陀螺儀 × Arduino　範例 5：九軸原始值
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.2 秒印出編號 10~18 的九個原始值（一次讀 9 個）：
 *     10~12 角速度 X/Y/Z，單位 0.1 度/秒
 *     13~15 加速度 X/Y/Z，單位 mg（1000 = 1 g，平放時 Z 約 1000）
 *     16~18 磁力 X/Y/Z，單位 mG（毫高斯）
 *   這些是「實體軸」的值（板子絲印的 X、Y、Z），做自己的演算法時用。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。平放時加速度 Z 約 1000、X/Y 約 0；拿磁鐵靠近看磁力變化。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！"));
  Serial.println(F("角速度 X/Y/Z(0.1度/秒) | 加速度 X/Y/Z(mg) | 磁力 X/Y/Z(mG)"));
}

void loop() {
  if (millis() - lastPrint >= 200) {     // 👉 可以改：每 0.2 秒印一次
    lastPrint = millis();

    // 一次讀編號 10~18 共 9 個值：[0..2] 角速度 [3..5] 加速度 [6..8] 磁力
    int16_t r[9];
    if (!mbcReadMany(ADDR, 0x10, 9, r)) { Serial.println(F("讀不到陀螺儀")); return; }

    char line[96];
    snprintf(line, sizeof(line), "%6d %6d %6d | %6d %6d %6d | %6d %6d %6d",
             r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8]);   // 每個數字佔 6 格
    Serial.println(line);
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

**你會看到：**（平放，然後拿磁鐵靠近）

```text
準備好了！
角速度 X/Y/Z(0.1度/秒) | 加速度 X/Y/Z(mg) | 磁力 X/Y/Z(mG)
     0      1      0 |      3     -5   1002 |    210    -95   -380
     0      0      1 |      2     -4   1001 |   1850   -620   -410
```

---

<h2 id="ex6">6️⃣ 哪一面朝上？磁力可不可信？</h2>

**做什麼：** 每 0.3 秒讀 `08` 磁力信任度、`09` 磁場干擾度、`0A` 朝上的面（一次讀 3 個）。朝上的面有變化就印一行；平放（+Z 朝上）時板子內建的 LED 會亮。

<a href="/examples/imu/arduino/imu_06_level/imu_06_level.ino" download data-ignore="true">📄 下載 imu_06_level.ino</a>

<!-- ino: examples/imu/arduino/imu_06_level/imu_06_level.ino -->
```cpp
/*
 * IMU 陀螺儀 × Arduino　範例 6：哪一面朝上？磁力可不可信？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.3 秒讀編號 08~0A（一次讀 3 個）：
 *     0A 朝上的面：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z（板子絲印的軸）
 *     08 磁力信任度 0~255：融合演算法多相信磁力計（越高越好）
 *     09 磁場干擾度 0~255：附近有沒有鐵器、馬達干擾（越低越好）
 *   朝上的面有變化就印一行；平放（+Z 朝上）時板子內建的 LED 會亮。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。把陀螺儀翻來翻去，看哪一面朝上；拿磁鐵靠近看干擾度。
 *
 * 小提醒：
 *   陀螺儀開機時要先放著不動約 0.5 秒，它會自己記住「哪一面朝上」並歸零。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

#ifndef LED_BUILTIN
#define LED_BUILTIN 2                    // 有些 ESP32 板子沒寫內建燈在哪，就用第 2 腳
#endif

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const int LED = LED_BUILTIN;             // 板子內建的燈（UNO 是第 13 腳）

unsigned long lastPrint = 0;
int16_t lastFace = -1;                   // 上一次朝上的面（-1 = 還沒看過）

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！翻翻看陀螺儀。"));
}

void loop() {
  if (millis() - lastPrint >= 300) {     // 👉 可以改：每 0.3 秒看一次
    lastPrint = millis();

    // 一次讀編號 08~0A 共 3 個值：[0] 磁力信任度 [1] 磁場干擾度 [2] 朝上的面
    int16_t v[3];
    if (!mbcReadMany(ADDR, 0x08, 3, v)) { Serial.println(F("讀不到陀螺儀")); return; }

    digitalWrite(LED, v[2] == 2 ? HIGH : LOW);    // +Z 朝上（平放）→ 燈亮

    if (v[2] != lastFace) {              // 朝上的面有變才印
      lastFace = v[2];
      Serial.print(F("朝上的面："));
      printFace(v[2]);
      Serial.print(F(" | 磁力信任度 ")); Serial.print(v[0]);
      Serial.print(F(" | 磁場干擾度 ")); Serial.print(v[1]);
      if (v[1] > 100) Serial.print(F("（干擾大，附近有鐵器或馬達？）"));
      Serial.println();
    }
  }
  delay(50);
}

// 0A 朝上的面：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z
void printFace(int16_t f) {
  switch (f) {
    case 0: Serial.print(F("+X（X 軸朝上）")); break;
    case 1: Serial.print(F("+Y（Y 軸朝上）")); break;
    case 2: Serial.print(F("+Z（平放，正面朝上）")); break;
    case 3: Serial.print(F("-X")); break;
    case 4: Serial.print(F("-Y")); break;
    case 5: Serial.print(F("-Z（翻面，背面朝上）")); break;
    default: Serial.print(F("？")); break;
  }
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

**你會看到：**（平放，翻面，再立起來）

```text
準備好了！翻翻看陀螺儀。
朝上的面：+Z（平放，正面朝上） | 磁力信任度 230 | 磁場干擾度 12
朝上的面：-Z（翻面，背面朝上） | 磁力信任度 228 | 磁場干擾度 15
朝上的面：+X（X 軸朝上） | 磁力信任度 225 | 磁場干擾度 18
```

---

<h2 id="ex7">7️⃣ 小車原地轉 90 度（進階）</h2>

**做什麼：** 開機先送 `E0` 歸零，然後每 3 秒原地右轉 90 度一次：用 `02` 融合航向看現在轉到幾度，差很多就轉快、快到了就轉慢，到了就停。轉 4 次回到原來的方向。

**另外要準備：** 兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。陀螺儀平放固定在車上。

**馬達接線：**

| L298N | Arduino UNO / Nano / R4 | ESP32 |
| :---: | :---: | :---: |
| ENA、IN1、IN2（左輪） | 5、7、8 | 25、26、27 |
| ENB、IN3、IN4（右輪） | 6、9、10 | 14、32、33 |
| GND | GND | GND |

> [!WARNING]
> 第一次跑之前，把車子**架高**（輪子懸空）再上傳：右轉時左輪應該往前、右輪往後，轉反了就改 `LEFT_REVERSE`／`RIGHT_REVERSE`。開機時車子不要動（陀螺儀要先記住哪一面朝上）。

<a href="/examples/imu/arduino/imu_07_turn90_car/imu_07_turn90_car.ino" download data-ignore="true">📄 下載 imu_07_turn90_car.ino</a>

<!-- ino: examples/imu/arduino/imu_07_turn90_car/imu_07_turn90_car.ino -->
```cpp
/*
 * IMU 陀螺儀 × Arduino　範例 7：小車原地轉 90 度（進階）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   開機後先送命令 E0 把航向歸零，然後每 3 秒讓兩輪小車原地右轉 90 度一次：
 *   用編號 02 融合航向當「現在轉到幾度」，差距大就轉快、快到了就轉慢，
 *   到了就停。轉 4 次就回到原來的方向。
 *   從上往下看順時針（右轉）航向變大，所以右轉時目標 = 原本 + 90。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。另外要：兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   （ESP32 的接腳不一樣，請看下面「馬達接腳」）
 *
 * 準備：
 *   1. 陀螺儀要平放固定在車上，開機時車子不要動（它要先記住哪一面朝上）。
 *   2. 先把車架高測試：右轉時左輪應該往前、右輪往後。轉反了就改 LEFT_REVERSE／RIGHT_REVERSE。
 *   3. 轉過頭就把 TURN_SPEED 調小、或 SLOW_ZONE 調大。
 *
 * 怎麼看結果：
 *   序列埠監控窗（115200）會印目標角度與現在的航向。
 *
 * 小提醒：
 *   想左轉：把 TURN_ANGLE 改成 -90。
 */

#include <Wire.h>

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
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
const int  TURN_ANGLE    = 90;           // 每次轉幾度：正數 = 右轉，負數 = 左轉
const int  TURN_SPEED    = 150;          // 轉彎速度 0~255
const int  SLOW_SPEED    = 80;           // 快到了時的慢速
const int  SLOW_ZONE     = 25;           // 剩不到幾度就改用慢速
const int  DONE_ZONE     = 2;            // 差不到幾度算到了
const unsigned long WAIT_MS = 3000;      // 每次轉完等幾毫秒
const bool LEFT_REVERSE  = false;        // 左輪轉反了改成 true
const bool RIGHT_REVERSE = false;        // 右輪轉反了改成 true

long target = 0;                         // 這一次要轉到的角度
unsigned long doneAt = 0;                // 上一次轉完的時間
bool turning = false;                    // 現在是不是正在轉

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(1000);                           // 多等一下，讓陀螺儀記住哪一面朝上
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }

  mbcWrite(ADDR, 0xE0);                  // 命令 E0：航向歸零（現在的方向 = 0 度）
  delay(100);
  Serial.println(F("已歸零。每 3 秒原地轉 90 度一次。"));
  doneAt = millis();
}

void loop() {
  int16_t heading = mbcRead(ADDR, 0x02); // 02 融合航向（度），右轉變大
  if (heading == MBC_NONE) {             // 讀不到陀螺儀 → 為了安全，停車
    drive(0, 0);
    Serial.println(F("讀不到陀螺儀 → 停車"));
    delay(50);
    return;
  }

  if (!turning) {
    // ---- 休息中：時間到了就設定下一個目標 ----
    if (millis() - doneAt >= WAIT_MS) {
      target += TURN_ANGLE;
      turning = true;
      Serial.print(F("開始轉，目標 ")); Serial.print(target); Serial.println(F(" 度"));
    }
  } else {
    // ---- 轉彎中：看還差幾度 ----
    long error = target - heading;       // 正數 = 還要往右轉，負數 = 要往左（或轉過頭了）
    if (abs(error) <= DONE_ZONE) {       // 到了
      drive(0, 0);
      turning = false;
      doneAt = millis();
      Serial.print(F("到了！現在航向 ")); Serial.println(heading);
    } else {
      int spd = (abs(error) < SLOW_ZONE) ? SLOW_SPEED : TURN_SPEED;   // 快到了就慢一點
      if (error > 0) drive(spd, -spd);   // 右轉：左輪前進、右輪後退
      else           drive(-spd, spd);   // 左轉
    }
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
已歸零。每 3 秒原地轉 90 度一次。
開始轉，目標 90 度
到了！現在航向 89
開始轉，目標 180 度
到了！現在航向 180
```

> [!TIP]
> 轉過頭：把 `TURN_SPEED` 調小、`SLOW_ZONE` 調大。想左轉：`TURN_ANGLE` 改成 `-90`。

---

<h2 id="ex8">8️⃣ 校準加速度計與磁力計</h2>

**做什麼：** 在序列埠監控窗打字下命令：`1` E1 加速度計校準開始、`2` E2 結束（存檔，並自動歸零航向）、`3` E3 磁力計校準開始、`4` E4 結束（存檔）。校準中 F3 的 bit4／bit5 會是 1，程式每 0.5 秒印一次。

**怎麼校準：**
- **加速度計**：按 `1`，把陀螺儀六個面各朝上平放幾秒（正面、背面、四個側面），再按 `2`。
- **磁力計**：按 `3`，把陀螺儀朝各個方向慢慢轉（畫 8 字）約 20 秒，遠離鐵器，再按 `4`。

<a href="/examples/imu/arduino/imu_08_calibrate/imu_08_calibrate.ino" download data-ignore="true">📄 下載 imu_08_calibrate.ino</a>

<!-- ino: examples/imu/arduino/imu_08_calibrate/imu_08_calibrate.ino -->
```cpp
/*
 * IMU 陀螺儀 × Arduino　範例 8：校準加速度計與磁力計
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用序列埠監控窗打字下命令，幫陀螺儀做校準（結果會存進它的 EEPROM）：
 *     1 → E1 加速度計校準開始   2 → E2 加速度計校準結束（存檔，並自動歸零航向）
 *     3 → E3 磁力計校準開始     4 → E4 磁力計校準結束（存檔）
 *   校準中狀態旗標 F3 的 bit4（加速度計）或 bit5（磁力計）會是 1，程式每 0.5 秒印一次。
 *   加速度計校準：按 1 之後把陀螺儀六個面各朝上放平幾秒，再按 2。
 *   磁力計校準：按 3 之後把陀螺儀朝各個方向慢慢轉（畫 8 字）約 20 秒，再按 4。
 *
 * 你需要：IMU 陀螺儀（通用 I2C 版）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   陀螺儀 VCC → 5V      陀螺儀 GND → GND
 *   陀螺儀 SDA → A4      陀螺儀 SCL → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200，在上面的輸入框打 1~4 再按 Enter。
 *
 * 小提醒：
 *   不確定要不要校準？出廠已經校過；只有航向漂得很快、或朝上的面判斷錯時才需要。
 *   校準時遠離馬達、喇叭、鐵桌。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x6A;               // IMU 陀螺儀的「門牌號碼」（I2C 位址）
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 3000)) {
    Serial.println(F("找不到陀螺儀，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("=== IMU 校準 ==="));
  Serial.println(F("輸入 1 = 加速度計校準開始、2 = 結束；3 = 磁力計校準開始、4 = 結束"));
}

void loop() {
  // ---- 1. 看電腦有沒有打字過來 ----
  if (Serial.available()) {
    char ch = Serial.read();
    switch (ch) {
      case '1': mbcWrite(ADDR, 0xE1); Serial.println(F(">>> E1 加速度計校準開始：六個面各朝上放平幾秒")); break;
      case '2': mbcWrite(ADDR, 0xE2); Serial.println(F(">>> E2 加速度計校準結束（存檔，航向歸零）")); break;
      case '3': mbcWrite(ADDR, 0xE3); Serial.println(F(">>> E3 磁力計校準開始：慢慢畫 8 字轉約 20 秒")); break;
      case '4': mbcWrite(ADDR, 0xE4); Serial.println(F(">>> E4 磁力計校準結束（存檔）")); break;
      default: break;                    // 其他字（例如換行）不理
    }
  }

  // ---- 2. 每 0.5 秒印一次校準狀態（F3 的 bit4、bit5）----
  if (millis() - lastPrint >= 500) {
    lastPrint = millis();
    int16_t st = mbcRead(ADDR, 0xF3);
    if (st == MBC_NONE) { Serial.println(F("讀不到陀螺儀")); return; }
    Serial.print(F("加速度計校準中 ")); Serial.print((st & 0x10) ? F("是") : F("否"));   // bit4
    Serial.print(F(" | 磁力計校準中 ")); Serial.print((st & 0x20) ? F("是") : F("否"));  // bit5
    Serial.print(F(" | 磁力融合已初始化 ")); Serial.print((st & 0x40) ? F("是") : F("否"));   // bit6
    Serial.print(F(" | 磁力自動校準收斂 ")); Serial.println((st & 0x80) ? F("是") : F("否"));  // bit7
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

**你會看到：**（輸入 3，轉一轉，再輸入 4）

```text
=== IMU 校準 ===
輸入 1 = 加速度計校準開始、2 = 結束；3 = 磁力計校準開始、4 = 結束
加速度計校準中 否 | 磁力計校準中 否 | 磁力融合已初始化 是 | 磁力自動校準收斂 否
>>> E3 磁力計校準開始：慢慢畫 8 字轉約 20 秒
加速度計校準中 否 | 磁力計校準中 是 | 磁力融合已初始化 是 | 磁力自動校準收斂 否
>>> E4 磁力計校準結束（存檔）
加速度計校準中 否 | 磁力計校準中 否 | 磁力融合已初始化 是 | 磁力自動校準收斂 是
```

> [!NOTE]
> 出廠已經校準過。只有航向漂得很快、或「朝上的面」判斷錯時才需要重做。

---

## ❓ 常見問題

**Q：一直出現「找不到陀螺儀」？**
1. 檢查 SDA、SCL 有沒有接反（UNO：SDA → A4、SCL → A5；R4：SDA／SCL；ESP32：21／22）。
2. 檢查 VCC、GND 有沒有插緊。
3. 確認是 **通用 I2C 版**：SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。

**Q：序列埠監控窗一堆亂碼？**
速度沒選 **115200**，在監控窗右下角改好就正常了。

**Q：航向放著不動也一直慢慢變？**
1. 開機時有沒有先放著不動 0.5 秒？沒有就按 Arduino 的 RESET 鍵重來。
2. 用 `02` 融合航向（有磁力計修正），不要用 `01`／`03`。
3. 遠離馬達、喇叭、鐵桌；用範例 6 看磁場干擾度。
4. 還是漂：跑範例 8 重新校準。

**Q：讀到 `-32768` 是什麼意思？**
這是「沒有這個值」的代號（範例裡叫 `MBC_NONE`）：編號是空號（例如 `0B`~`0F`），或是讀不到陀螺儀（線鬆了）。寫程式時先檢查 `v != MBC_NONE` 再用。

**Q：小幫手裡的 `delay(2)` 可以拿掉嗎？**
陀螺儀本身不需要，但 PS4/PS5 接收器和神攝手（ESP32 做的）一定要，所以 MBC 所有範例一律等 2 毫秒。建議留著。

**Q：ESP32 當主機時要注意什麼？**
接線用 GPIO 21（SDA）／22（SCL）。小車範例的馬達腳位 ESP32 跟 UNO 不同，程式裡已用 `#if defined(ESP32)` 分開；ESP32 沒有 `LED_BUILTIN` 的板子，範例 6 用第 2 腳。

**Q：想知道每一個格子放什麼資料？**
請看上面的 [指令總覽](#cmds) 和 [MBC 通用 I2C 協議 v2](/i2c-protocol.md)（給會寫程式的人看的進階資料）。
