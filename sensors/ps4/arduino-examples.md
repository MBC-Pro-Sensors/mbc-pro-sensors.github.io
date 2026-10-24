<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- moved: /sensors/ps4/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/ps4/arduino/ide.md">掌控者 PS4/PS5 › Arduino › Arduino IDE</a></p>

# 🧪 掌控者 PS4/PS5 × Arduino 範例

[🔙 回到 PS4/PS5 遙控接收器首頁](/sensors/ps4/index.md)　｜　[∿ MBC 通用 I2C 協議 v2（進階）](/i2c-protocol.md)

這一頁有 **8 個範例**，從「確認接好了沒」一路做到「坦克遙控車」，還有 PS4/PS5 手把才有的**震動**和**燈條**。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/ps4/arduino/ps4_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 接收器 | Arduino UNO / Nano | Arduino UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: |
| VCC | 5V | 5V | 5V（VIN） |
| GND | GND | GND | GND |
| RX（= SDA） | A4 | SDA | GPIO 21 |
| TX（= SCL） | A5 | SCL | GPIO 22 |

<div style="text-align: center;">
<img src="/images/figures/ps4/wiring-uno.svg" alt="掌控者 PS4/PS5 接 Arduino UNO 的接線圖" style="max-width: 520px; width: 100%; margin: 1rem auto; display: block;" />
</div>

> [!IMPORTANT]
> - 請用 **通用 I2C 版** 的接收器。SPIKE 版、EV3 版的韌體不同，不能直接接 Arduino。
> - 接收器上印的是 **TX、RX**：I2C 版的 **TX 就是 SCL、RX 就是 SDA**。
> - 接收器裡面已經有電壓轉換，UNO（5 V）、UNO R4 和 ESP32（3.3 V）都可以直接接。
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

<h2 id="cmds">📖 指令總覽（這個接收器能讀到什麼）</h2>

接收器就像一排有編號的「格子」，每一格放一個數字（int16）。寫 `[編號]` 再讀 2 個 byte 就拿到一個值；連續的編號可以一次讀一串（最多 16 個）。範例裡的 `mbcRead(ADDR, 編號)`、`mbcReadMany(ADDR, 起始編號, 幾個, 陣列)` 就是在做這件事。

<!-- gen:cmd-table -->
| 編號 | 名稱 | 範圍／單位 | 說明 | 出現在 |
| :---: | :--- | :--- | :--- | :--- |
| `01` | 十六鍵 | 0~65535（bit） | 16 顆鍵合成一個數（PSX 排列）：bit0 SELECT(SHARE)、1 L3、2 R3、3 START(OPTIONS)、4 ↑、5 →、6 ↓、7 ←、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；1 = 按下 | 範例 2、3、7 |
| `02` | 方向鍵 | 0~15 | ↑ 1、→ 2、↓ 4、← 8，同時按相加 | 範例 2、3 |
| `03` | 形狀鍵 | 0~15 | ▲ 1、● 2、✕ 4、■ 8 | 範例 2、3 |
| `04` | 肩鍵 | 0~15 | L2 1、R2 2、L1 4、R1 8 | 範例 2、3 |
| `05` | 功能鍵 | 0~15 | SELECT(SHARE) 1、L3 2、R3 4、START(OPTIONS) 8 | 範例 2、3 |
| `06` | 已連線 | 0～1 | 1 = 手把已連線且 0.3 秒內有資料；斷線時按鍵全 0、搖桿 0 | 範例 2、5、6 |
| `07` | 電量 | 0~100 % | 手把電量 | 範例 1、2、5 |
| `08` | L2 類比 | 0~255 | L2 扳機壓多深 | 範例 2、5 |
| `09` | R2 類比 | 0~255 | R2 扳機壓多深 | 範例 2、5、7 |
| `0A` | 系統鍵 | bit | b0 PS 鍵、b1 擷取鍵（PS5：靜音鍵） | 範例 2、5 |
| `0B` | 手把類型 | 0~4 | 0 無、1 PS2 數位、2 PS2 類比、3 PS4、4 PS5（或其他） | 範例 2、5 |
| `10`~`13` | 搖桿 LX／LY／RX／RY | −100~+100 | 0 = 中間，右／上為正 | 範例 2、4、6、8 |
| `F0` | 裝置 ID | — | 低 byte = 出廠位址 0x42、高 byte = 款式（1 PS2、**2 PS4/PS5**） | 範例 1 |
| `F1` | 協議版本 | 2 | 這份範例照版本 2 寫 | 範例 1 |
| `F2` | 韌體版本 | 主×100＋次 | 例如 102 = 1.2 | 範例 1 |
| `F3` | 狀態旗標 | bit | b0 就緒、b1 手把已連線 | 範例 1、所有範例的 mbcWaitReady |
| `F5` | 開機秒數 | 0~32767 | 數字變小 = 接收器重開機過 | 範例 1 |
| `E5`（寫） | 震動 | [強度 0~255] | 0 = 停；每送一次震 200 ms | 範例 7 |
| `E6`（寫） | 燈條顏色 | [R][G][B] | 各 0~255，要剛好 3 byte；沒連線時先記住 | 範例 8 |
<!-- /gen -->

> [!NOTE]
> 沒有的編號（例如 `0C`~`0F`）會讀到 `-32768`（範例裡叫 `MBC_NONE`），讀不到接收器也是 `-32768`。完整規格見 [MBC 通用 I2C 協議 v2](/i2c-protocol.md)。

---

## 📋 範例總覽

| # | 範例 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- |
| 1 | [跟接收器打招呼](#ex1) | 確認接線正確、讀身分證（F0~F3）、看手把連上沒 | 無 |
| 2 | [把所有的值一次印出來](#ex2) | 看懂每個編號放什麼（01~0B、10~13） | 無 |
| 3 | [按了哪一顆鍵？](#ex3) | 十六鍵拆 bit、四組分組鍵 | 無 |
| 4 | [搖桿推多少？](#ex4) | 搖桿 ±100，畫成十字圖 | 無 |
| 5 | [手把的狀態](#ex5) | 連線、電量、扳機類比、PS 鍵、手把類型 | 無 |
| 6 | [坦克遙控車（進階）](#ex6) | 兩支搖桿開車、斷線自動停車 | 兩輪小車、L298N 馬達驅動板 |
| 7 | [讓手把震動](#ex7) | 寫入命令 E5 | 無 |
| 8 | [搖桿調燈條顏色](#ex8) | 寫入命令 E6（3 個參數） | 無 |

> [!TIP]
> **建議順序：1 → 2 → 3 → 4 → 5 → 7 → 8 → 6。** 只有 6 要另外準備零件。

---

<h2 id="ex1">1️⃣ 跟接收器打招呼</h2>

**做什麼：** 確認接收器接好了，印出它的「身分證」（出廠位址、款式、協議版本、韌體版本、狀態旗標），然後一直看著手把：連上、斷線都會說。每次接新的接收器，都先跑這一個。

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
 *   確認接收器有沒有接好。接好了，會印出它的身分證（裝置 ID、款式、協議版本、
 *   韌體版本）和目前狀態。之後一直看著手把：連上、斷線都會告訴你。
 *   每次接新的接收器，都建議先跑這個範例。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。
 *   按手把的 PS 鍵讓它連上，看電腦有沒有說「手把連上了」。
 *
 * 小提醒：
 *   第一次用要先配對：接收器開機（或按 Reset）後 30 秒內，PS4 手把長按 SHARE + PS、PS5 手把長按 建立 + PS，直到燈條快閃。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具，用來跟接收器講話

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

bool wasConnected = false;               // 上一次看的時候，手把有沒有連上

void setup() {
  Serial.begin(115200);                  // 打開跟電腦講話的通道，速度 115200
  mbcBegin();                            // 準備好 I2C 通訊（小幫手在檔案最下面）
  delay(500);                            // 等接收器開機 0.5 秒

  Serial.println(F("=== 掌控者 PS4/PS5：跟接收器打招呼 ==="));

  // 等接收器說「我準備好了」（狀態旗標 F3 的 bit0），最多等 5 秒
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器！請檢查："));
    Serial.println(F("  1. RX（SDA）接 A4、TX（SCL）接 A5（ESP32：21 / 22），有沒有接反？"));
    Serial.println(F("  2. VCC 接 5V、GND 接 GND，有沒有插緊？接收器的燈有沒有亮？"));
    Serial.println(F("  3. 你的接收器是「通用 I2C 版」嗎？（SPIKE 版、EV3 版不能接 Arduino）"));
    while (true) {}                      // 找不到就停在這裡
  }

  // ---- F0 裝置 ID：低 byte = 出廠位址、高 byte = 款式（1 = PS2、2 = PS4/PS5） ----
  int16_t id = mbcRead(ADDR, 0xF0);
  uint8_t factoryAddr = (uint8_t)(id & 0xFF);        // 出廠位址（換過位址也不變）
  uint8_t kind        = (uint8_t)((id >> 8) & 0xFF); // hw[0]：1 = PS2 接收器、2 = PS4/PS5 接收器
  Serial.print(F("找到了！出廠位址 0x"));
  Serial.println(factoryAddr, HEX);      // 應該是 42
  Serial.print(F("款式："));
  if (kind == 1)      Serial.println(F("PS2 2.4G 接收器"));
  else if (kind == 2) Serial.println(F("PS4/PS5 藍牙接收器"));
  else                Serial.println(F("不認識的款式"));

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
  Serial.print(F("  bit0 就緒        ")); Serial.println((st & 0x01) ? F("是") : F("否"));
  Serial.print(F("  bit1 手把已連線  ")); Serial.println((st & 0x02) ? F("是") : F("否"));

  // ---- F5 開機秒數（順便看它開機多久了） ----
  Serial.print(F("開機秒數："));
  Serial.println(mbcRead(ADDR, 0xF5));

  Serial.println(F("接線正確！現在按手把的 PS 鍵讓它連上試試看。"));
}

void loop() {
  // 每一圈都看一下 F3 的 bit1（手把有沒有連上），有變化才印，畫面才不會洗版
  int16_t st = mbcRead(ADDR, 0xF3);
  if (st == MBC_NONE) {                  // 讀不到 = 線鬆掉了
    Serial.println(F("讀不到接收器，線鬆掉了嗎？"));
    delay(1000);
    return;
  }
  bool connected = st & 0x02;
  if (connected != wasConnected) {
    if (connected) {
      Serial.print(F("手把連上了！電量 "));
      Serial.print(mbcRead(ADDR, 0x07));  // 07 電量 0~100（PS2 手把沒有電量，恆 0）
      Serial.println(F("%"));
    } else {
      Serial.println(F("手把沒有連線"));
    }
    wasConnected = connected;
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

**你會看到：**（手把連上，之後長按 PS 鍵 10 秒讓手把關機）

```text
=== 掌控者 PS4/PS5：跟接收器打招呼 ===
找到了！出廠位址 0x42
款式：PS4/PS5 藍牙接收器
協議版本：2
韌體版本：1.2
目前狀態：
  bit0 就緒        是
  bit1 手把已連線  否
開機秒數：3
接線正確！現在按手把的 PS 鍵讓它連上試試看。
手把連上了！電量 80%
手把沒有連線
```

> [!NOTE]
> **「通訊小幫手」是什麼？** 每個範例最下面都有一段一模一樣的程式，負責跟接收器「講話」。你不用改它，也不用看懂。只要記得：
> - `mbcRead(ADDR, 編號)`：讀一個值（讀不到回 `-32768`）
> - `mbcReadMany(ADDR, 起始編號, 幾個, 陣列)`：從某個編號起一次讀一串（最多 16 個，同一瞬間的值）
> - `mbcWrite(ADDR, 命令, 參數, 幾個 byte)`：送命令（例如震動、燈條）
> - `mbcWaitReady(ADDR, 毫秒)`：等接收器開機好
>
> 小幫手裡有一行 `delay(2)`（寫完編號等 2 毫秒再讀），**不能刪**：PS4/PS5 接收器是 ESP32 做的，需要這 2 毫秒準備資料。

---

<h2 id="ex2">2️⃣ 把所有的值一次印出來</h2>

**做什麼：** 每 0.5 秒把值表裡的每一個值都印出來：`01`~`0B` 一次讀 11 個、`10`~`13` 一次讀 4 個。按按鍵、推搖桿，看哪一行在變，就知道每個編號是什麼。

<a href="/examples/ps4/arduino/ps4_02_all_values/ps4_02_all_values.ino" download data-ignore="true">📄 下載 ps4_02_all_values.ino</a>

<!-- ino: examples/ps4/arduino/ps4_02_all_values/ps4_02_all_values.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 2：把所有的值一次印出來
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.5 秒把接收器「值表」裡的每一個值都印出來，讓你看懂每個編號放什麼：
 *     01~0B：按鍵、連線、電量、扳機、系統鍵、手把類型（一次讀 11 個）
 *     10~13：四根搖桿軸 LX、LY、RX、RY（一次讀 4 個）
 *   連續的編號可以「一次讀一串」（mbcReadMany），讀到的是同一瞬間的值。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。按按鍵、推搖桿，看哪個數字在變。
 *
 * 小提醒：
 *   第一次用要先配對：接收器開機（或按 Reset）後 30 秒內，PS4 手把長按 SHARE + PS、PS5 手把長按 建立 + PS，直到燈條快閃。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;             // 上一次印的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！每 0.5 秒印一次所有的值。"));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 可以改：每 0.5 秒印一次
    lastPrint = millis();

    // ---- 第一段：編號 01~0B，一次讀 11 個值 ----
    int16_t a[11];
    if (!mbcReadMany(ADDR, 0x01, 11, a)) { Serial.println(F("讀不到（第一段）")); return; }
    // ---- 第二段：編號 10~13，一次讀 4 個值 ----
    int16_t s[4];
    if (!mbcReadMany(ADDR, 0x10, 4, s)) { Serial.println(F("讀不到（第二段）")); return; }

    Serial.println(F("------------------------------"));
    Serial.print(F("01 十六鍵（PSX 排列，二進位）：")); printBits16((uint16_t)a[0]); Serial.println();
    Serial.print(F("02 方向鍵（上1 右2 下4 左8）  ：")); Serial.println(a[1]);
    Serial.print(F("03 形狀鍵（▲1 ●2 ✕4 ■8）     ：")); Serial.println(a[2]);
    Serial.print(F("04 肩鍵  （L2 1 R2 2 L1 4 R1 8）：")); Serial.println(a[3]);
    Serial.print(F("05 功能鍵（SELECT1 L3 2 R3 4 START8）：")); Serial.println(a[4]);
    Serial.print(F("06 已連線（1 = 有）           ：")); Serial.println(a[5]);
    Serial.print(F("07 電量 0~100     ：")); Serial.println(a[6]);
    Serial.print(F("08 L2 類比 0~255              ：")); Serial.println(a[7]);
    Serial.print(F("09 R2 類比 0~255              ：")); Serial.println(a[8]);
    Serial.print(F("0A 系統鍵（b0 PS、b1 擷取）   ：")); Serial.println(a[9]);
    Serial.print(F("0B 手把類型 0~4               ：")); Serial.println(a[10]);
    Serial.print(F("10 LX  11 LY  12 RX  13 RY（±100）："));
    for (uint8_t i = 0; i < 4; i++) { Serial.print(s[i]); Serial.print(i < 3 ? F("  ") : F("")); }
    Serial.println();
  }
  delay(50);
}

// 把 16 位數字印成 16 個 0／1（左邊是 bit15 ■、右邊是 bit0 SELECT）
void printBits16(uint16_t v) {
  for (int8_t i = 15; i >= 0; i--) {
    Serial.print((v >> i) & 1);
    if (i % 4 == 0 && i > 0) Serial.print(' ');   // 每 4 位空一格比較好看
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

**你會看到：**（按住 ✕ 和 ↑，左搖桿推到最右）

```text
準備好了！每 0.5 秒印一次所有的值。
------------------------------
01 十六鍵（PSX 排列，二進位）：0100 0000 0001 0000
02 方向鍵（上1 右2 下4 左8）  ：1
03 形狀鍵（▲1 ●2 ✕4 ■8）     ：4
04 肩鍵  （L2 1 R2 2 L1 4 R1 8）：0
05 功能鍵（SELECT1 L3 2 R3 4 START8）：0
06 已連線（1 = 有）           ：1
07 電量 0~100（PS2 恆 0）     ：80
08 L2 類比 0~255              ：0
09 R2 類比 0~255              ：0
0A 系統鍵（b0 PS、b1 擷取）   ：0
0B 手把類型 0~4               ：3
10 LX  11 LY  12 RX  13 RY（±100）：100  0  0  0
```

> [!TIP]
> 二進位那一行左邊是 bit15（■）、右邊是 bit0（SELECT）。上面 ✕ 是 bit14、↑ 是 bit4，所以看到兩個 1。

---

<h2 id="ex3">3️⃣ 按了哪一顆鍵？</h2>

**做什麼：** 按下手把上任何一顆鍵，電腦會用中文告訴你按了哪一顆；放開時也會說。可以同時按好幾顆。四組「分組鍵」（方向、形狀、肩、功能）有變化時也會印一行。

<a href="/examples/ps4/arduino/ps4_03_buttons/ps4_03_buttons.ino" download data-ignore="true">📄 下載 ps4_03_buttons.ino</a>

<!-- ino: examples/ps4/arduino/ps4_03_buttons/ps4_03_buttons.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 3：按了哪一顆鍵？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   按下手把上任何一顆鍵，電腦會用中文告訴你按了哪一顆；放開時也會說。
 *   可以同時按好幾顆。
 *   16 顆鍵的狀態放在編號 01（十六鍵）一個數字裡，一顆鍵佔一個「位」（bit）：
 *   按下 = 1、沒按 = 0，就像 16 個電燈開關排成一排。
 *   編號 02~05 是同一批按鍵的「分組版」：方向鍵、形狀鍵、肩鍵、功能鍵各 4 顆，
 *   一組 0~15，同時按就相加（例如 ↑ + → = 1 + 2 = 3）。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。按手把的 PS 鍵讓它連上，按按看。
 *
 * 小提醒：
 *   第一次用要先配對：接收器開機（或按 Reset）後 30 秒內，PS4 手把長按 SHARE + PS、PS5 手把長按 建立 + PS，直到燈條快閃。
 *   PS 鍵、擷取鍵不在十六鍵裡，它們在編號 0A 系統鍵（b0 PS、b1 擷取）。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

uint16_t lastButtons = 0;                // 上一次看到的十六鍵狀態
int16_t  lastGroup[4] = {0, 0, 0, 0};    // 上一次看到的 02~05 四組

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！按手把上的按鍵看看。"));
}

void loop() {
  // 一次讀編號 01~05 共 5 個值：[0] 十六鍵 [1] 方向鍵 [2] 形狀鍵 [3] 肩鍵 [4] 功能鍵
  int16_t v[5];
  if (!mbcReadMany(ADDR, 0x01, 5, v)) {
    delay(100);                          // 這次沒讀到，等一下再試
    return;
  }
  uint16_t buttons = (uint16_t)v[0];     // 十六鍵：PSX 排列（bit0 SELECT … bit15 ■）

  // ---- 1. 一顆一顆檢查：哪些鍵「剛按下」、哪些「剛放開」 ----
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t mask = 1u << i;             // 第 i 顆鍵的「位」
    bool now    = buttons & mask;        // 現在有沒有按
    bool before = lastButtons & mask;    // 上一次有沒有按
    if (now && !before) { Serial.print(F("按下：")); printButtonName(i); Serial.println(); }
    if (!now && before) { Serial.print(F("放開：")); printButtonName(i); Serial.println(); }
  }
  lastButtons = buttons;                 // 記住這一次，下一次拿來比較

  // ---- 2. 四組分組值有變化就印一行（方向／形狀／肩／功能，各 0~15） ----
  bool changed = false;
  for (uint8_t i = 0; i < 4; i++) if (v[i + 1] != lastGroup[i]) changed = true;
  if (changed) {
    Serial.print(F("    分組 → 方向鍵 ")); Serial.print(v[1]);
    Serial.print(F("  形狀鍵 ")); Serial.print(v[2]);
    Serial.print(F("  肩鍵 ")); Serial.print(v[3]);
    Serial.print(F("  功能鍵 ")); Serial.println(v[4]);
    for (uint8_t i = 0; i < 4; i++) lastGroup[i] = v[i + 1];
  }

  delay(50);                             // 每 0.05 秒看一次，按很快也抓得到
}

// 印出十六鍵裡第 i 位的按鍵名字（PSX 排列）
void printButtonName(uint8_t i) {
  switch (i) {
    case 0:  Serial.print(F("SELECT（PS4：SHARE／PS5：建立）")); break;
    case 1:  Serial.print(F("L3（左搖桿往下壓）")); break;
    case 2:  Serial.print(F("R3（右搖桿往下壓）")); break;
    case 3:  Serial.print(F("START（PS4/PS5：OPTIONS）")); break;
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

**你會看到：**（按 ✕、再同時按 ↑ 和 R1）

```text
準備好了！按手把上的按鍵看看。
按下：✕ 叉叉
    分組 → 方向鍵 0  形狀鍵 4  肩鍵 0  功能鍵 0
放開：✕ 叉叉
    分組 → 方向鍵 0  形狀鍵 0  肩鍵 0  功能鍵 0
按下：↑ 方向鍵上
按下：R1
    分組 → 方向鍵 1  形狀鍵 0  肩鍵 8  功能鍵 0
```

**16 顆按鍵各在第幾位？**

| 位 | 按鍵 | 位 | 按鍵 |
| :---: | :--- | :---: | :--- |
| 0 | SELECT（PS4：SHARE／PS5：建立） | 8 | L2 |
| 1 | L3（左搖桿往下壓） | 9 | R2 |
| 2 | R3（右搖桿往下壓） | 10 | L1 |
| 3 | START（PS4/PS5：OPTIONS） | 11 | R1 |
| 4 | ↑ 上 | 12 | ▲ 三角形 |
| 5 | → 右 | 13 | ● 圓圈 |
| 6 | ↓ 下 | 14 | ✕ 叉叉 |
| 7 | ← 左 | 15 | ■ 正方形 |

PS 鍵、擷取鍵不在這 16 位裡，在編號 `0A` 系統鍵（b0 PS、b1 擷取），範例 5 會讀。

> [!TIP]
> **試試看：** 想知道「✕ 有沒有按」，寫 `if (buttons & (1u << 14)) { ... }` 就好；或者讀編號 `03` 形狀鍵，`if (v & 4)`。

---

<h2 id="ex4">4️⃣ 搖桿推多少？</h2>

**做什麼：** 把兩支搖桿的位置變成數字（編號 `10` LX、`11` LY、`12` RX、`13` RY）：放開 = `0`，往右、往上推是**正數**，往左、往下推是**負數**，推到底是 `+100` 或 `-100`。每 0.3 秒印一次，還畫成兩個十字圖。

<a href="/examples/ps4/arduino/ps4_04_sticks/ps4_04_sticks.ino" download data-ignore="true">📄 下載 ps4_04_sticks.ino</a>

<!-- ino: examples/ps4/arduino/ps4_04_sticks/ps4_04_sticks.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 4：搖桿推多少？
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   把兩支搖桿的位置變成數字（編號 10 LX、11 LY、12 RX、13 RY）：
 *     放開（在中間）= 0；往右推、往上推 = 正數，推到底 = +100；
 *     往左推、往下推 = 負數，推到底 = -100。
 *   每 0.3 秒印一次數字，並畫成兩個「十字圖」：O 就是搖桿現在的位置。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。慢慢推搖桿，看 O 怎麼跑。
 *
 * 小提醒：
 *   搖桿在中間附近輕晃，數字會停在 0，這是接收器故意的（叫做「死區」），放開時車子才不會自己慢慢跑。
 *   第一次用要先配對：接收器開機（或按 Reset）後 30 秒內，PS4 手把長按 SHARE + PS、PS5 手把長按 建立 + PS，直到燈條快閃。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！推推看搖桿。"));
}

void loop() {
  if (millis() - lastPrint >= 300) {     // 👉 可以改：每 0.3 秒印一次
    lastPrint = millis();

    // 一次讀編號 10~13 共 4 個值：[0] LX [1] LY [2] RX [3] RY（-100 ~ +100）
    int16_t s[4];
    if (!mbcReadMany(ADDR, 0x10, 4, s)) { Serial.println(F("讀不到接收器")); return; }

    Serial.print(F("左搖桿 X ")); Serial.print(s[0]);
    Serial.print(F("  Y ")); Serial.print(s[1]);
    Serial.print(F("   |   右搖桿 X ")); Serial.print(s[2]);
    Serial.print(F("  Y ")); Serial.println(s[3]);
    drawCross(s[0], s[1], s[2], s[3]);   // 畫兩個十字圖
  }
  delay(50);
}

// 畫兩個 7×7 的十字圖（左搖桿、右搖桿並排）。中心是 +，搖桿位置畫 O
//   X：-100 ~ +100 → 第 0 ~ 6 格（左 → 右）；Y：上為正，所以 +100 在最上面一列
void drawCross(int lx, int ly, int rx, int ry) {
  int lcol = map(lx, -100, 100, 0, 6), lrow = map(ly, 100, -100, 0, 6);
  int rcol = map(rx, -100, 100, 0, 6), rrow = map(ry, 100, -100, 0, 6);
  for (int row = 0; row < 7; row++) {
    for (int col = 0; col < 7; col++) Serial.print(cell(row, col, lrow, lcol));
    Serial.print(F("     "));            // 兩個圖中間空 5 格
    for (int col = 0; col < 7; col++) Serial.print(cell(row, col, rrow, rcol));
    Serial.println();
  }
  Serial.println();
}

// 決定圖上某一格要印什麼字：O = 搖桿位置、+ = 中心、| 和 - = 十字線、. = 空白
char cell(int row, int col, int posRow, int posCol) {
  if (row == posRow && col == posCol) return 'O';
  if (row == 3 && col == 3) return '+';
  if (col == 3) return '|';
  if (row == 3) return '-';
  return '.';
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

**你會看到：**（左搖桿推到右上角，右搖桿放開）

```text
準備好了！推推看搖桿。
左搖桿 X 100  Y 100   |   右搖桿 X 0  Y 0
...|..O     ...|...
...|...     ...|...
...|...     ...|...
---+---     ---O---
...|...     ...|...
...|...     ...|...
...|...     ...|...

```

> [!NOTE]
> 搖桿在中間附近輕輕晃時數字會停在 `0`，這是接收器故意的（叫做「死區」），放開搖桿時車子才不會自己慢慢跑。

---

<h2 id="ex5">5️⃣ 手把的狀態</h2>

**做什麼：** 每 0.5 秒印出編號 `06`~`0B`（一次讀 6 個）：已連線、電量、L2／R2 扳機壓多深、PS 鍵／擷取鍵、手把類型。連線狀態有變化時另外提醒一行。

<a href="/examples/ps4/arduino/ps4_05_status/ps4_05_status.ino" download data-ignore="true">📄 下載 ps4_05_status.ino</a>

<!-- ino: examples/ps4/arduino/ps4_05_status/ps4_05_status.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 5：手把的狀態
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   每 0.5 秒印出跟「按鍵、搖桿」無關的狀態值（編號 06~0B，一次讀 6 個）：
 *     06 已連線（1 = 有）、07 電量 0~100、08 L2 扳機壓多深 0~255、
 *     09 R2 扳機壓多深 0~255、0A 系統鍵（b0 PS 鍵、b1 擷取鍵）、0B 手把類型。
 *   連線狀態有變化時會另外印一行提醒。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後，打開「序列埠監控窗」，速度選 115200。慢慢壓 L2、R2 看數字變化；把手把關機看「已連線」變 0。
 *
 * 小提醒：
 *   壓 L2／R2 時 08／09 會慢慢從 0 變到 255，輕壓、重壓都看得出來。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

unsigned long lastPrint = 0;
int16_t lastConnected = -1;              // 上一次的連線狀態（-1 = 還沒看過）

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！每 0.5 秒印一次手把狀態。"));
}

void loop() {
  if (millis() - lastPrint >= 500) {     // 👉 可以改：每 0.5 秒印一次
    lastPrint = millis();

    // 一次讀編號 06~0B 共 6 個值：[0] 已連線 [1] 電量 [2] L2 [3] R2 [4] 系統鍵 [5] 手把類型
    int16_t v[6];
    if (!mbcReadMany(ADDR, 0x06, 6, v)) { Serial.println(F("讀不到接收器")); return; }

    // 連線狀態有變化就提醒一行
    if (v[0] != lastConnected) {
      Serial.println(v[0] ? F(">>> 手把連上了！") : F(">>> 手把沒有連線"));
      lastConnected = v[0];
    }

    Serial.print(F("已連線 ")); Serial.print(v[0]);
    Serial.print(F(" | 電量 ")); Serial.print(v[1]); Serial.print(F("%"));
    Serial.print(F(" | L2 ")); Serial.print(v[2]);
    Serial.print(F(" R2 ")); Serial.print(v[3]);
    Serial.print(F(" | PS 鍵 ")); Serial.print((v[4] & 0x01) ? 1 : 0);     // 系統鍵 bit0
    Serial.print(F(" 擷取鍵 ")); Serial.print((v[4] & 0x02) ? 1 : 0);     // 系統鍵 bit1
    Serial.print(F(" | 手把："));
    printPadType(v[5]);
    Serial.println();
  }
  delay(50);
}

// 0B 手把類型：0 沒有、1 PS2 數位、2 PS2 類比、3 PS4、4 PS5（或其他）
void printPadType(int16_t t) {
  switch (t) {
    case 0: Serial.print(F("沒有手把")); break;
    case 1: Serial.print(F("PS2 手把（數位模式）")); break;
    case 2: Serial.print(F("PS2 手把（類比模式）")); break;
    case 3: Serial.print(F("PS4 手把")); break;
    case 4: Serial.print(F("PS5 手把（或其他藍牙手把）")); break;
    default: Serial.print(F("不認識"));
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

**你會看到：**（手把連上，L2 壓一半、按住 PS 鍵）

```text
準備好了！每 0.5 秒印一次手把狀態。
>>> 手把連上了！
已連線 1 | 電量 80% | L2 0 R2 0 | PS 鍵 0 擷取鍵 0 | 手把：PS4 手把
已連線 1 | 電量 80% | L2 128 R2 0 | PS 鍵 1 擷取鍵 0 | 手把：PS4 手把
```

---

<h2 id="ex6">6️⃣ 坦克遙控車（進階）</h2>

**做什麼：** 用兩支搖桿開兩輪小車：左搖桿上下（LY）控制左輪、右搖桿上下（RY）控制右輪，搖桿的 `-100`~`+100` 直接乘上速度。手把斷線或讀不到接收器就立刻停車。

**另外要準備：** 兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。

**馬達接線：**

| L298N | Arduino UNO / Nano / R4 | ESP32 |
| :---: | :---: | :---: |
| ENA、IN1、IN2（左輪） | 5、7、8 | 25、26、27 |
| ENB、IN3、IN4（右輪） | 6、9、10 | 14、32、33 |
| GND | GND | GND |

> [!WARNING]
> 第一次跑之前，把車子**架高**（輪子懸空）再上傳：兩支搖桿一起往上推，確認兩個輪子都往前轉（轉反了就改 `LEFT_REVERSE`／`RIGHT_REVERSE`）；然後把手把關機，確認輪子會停，再放到地上開。

<a href="/examples/ps4/arduino/ps4_06_tank_car/ps4_06_tank_car.ino" download data-ignore="true">📄 下載 ps4_06_tank_car.ino</a>

<!-- ino: examples/ps4/arduino/ps4_06_tank_car/ps4_06_tank_car.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 6：坦克遙控車（進階）
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   用手把開兩輪小車，開法跟坦克一樣：
 *     左搖桿上下（LY）→ 左輪；右搖桿上下（RY）→ 右輪
 *     兩支一起往上推 = 直走；一前一後 = 原地轉彎。
 *   搖桿的 -100 ~ +100 直接當馬達速度用（再乘上最高速度）。
 *   手把斷線、或讀不到接收器，車子會立刻停下來。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。另外要：兩輪小車、L298N 馬達驅動板（或同樣接法的 TB6612）、電池。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *   （ESP32 的接腳不一樣，請看下面「馬達接腳」）
 *
 * 準備：
 *   1. 把車子架高（輪子懸空）再上傳，兩支搖桿一起往上推，確認兩個輪子都「往前」轉。
 *      哪個輪子轉反了，就把 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 *   2. ⚠️ 車子還架高時，把手把關機，確認輪子會停下來，再放到地上開。
 *
 * 怎麼看結果：
 *   序列埠監控窗（115200）每 0.3 秒印一次左右輪速度。
 *
 * 小提醒：
 *   第一次用要先配對：接收器開機（或按 Reset）後 30 秒內，PS4 手把長按 SHARE + PS、PS5 手把長按 建立 + PS，直到燈條快閃。
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
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
const int  MAX_SPEED     = 200;          // 搖桿推到底的速度：0 ~ 255，新手先用慢一點
const bool LEFT_REVERSE  = false;        // 左輪轉反了改成 true
const bool RIGHT_REVERSE = false;        // 右輪轉反了改成 true

bool stopped = true;                     // 現在是不是停車狀態（一開始算停車）
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！用兩支搖桿開車。"));
}

void loop() {
  // 一次讀編號 06 ~ 13 共 14 個值（同一瞬間的資料）：
  //   v[0] = 06 已連線 … v[10] = 10 LX、v[11] = 11 LY、v[12] = 12 RX、v[13] = 13 RY
  //   （中間的 0C~0F 是空號，會讀到 -32768，不用理它）
  int16_t v[14];
  if (!mbcReadMany(ADDR, 0x06, 14, v) || v[0] == 0) {
    safeStop();                          // 讀不到接收器，或手把沒連線 → 立刻停車
    delay(50);
    return;
  }
  if (stopped) {
    stopped = false;
    Serial.println(F("手把連上了，可以開車！"));
  }

  int ly = v[11];                        // 左搖桿上下：上 = 正（-100 ~ +100）
  int ry = v[13];                        // 右搖桿上下：上 = 正

  // 搖桿推多少，輪子就轉多快：-100 ~ +100 換算成 -MAX_SPEED ~ +MAX_SPEED
  int left  = (long)ly * MAX_SPEED / 100;
  int right = (long)ry * MAX_SPEED / 100;
  drive(left, right);

  if (millis() - lastPrint >= 300) {     // 每 0.3 秒印一次速度
    lastPrint = millis();
    Serial.print(F("左輪 ")); Serial.print(left);
    Serial.print(F("　右輪 ")); Serial.println(right);
  }
  delay(50);
}

// 停車，並且只在剛停的那一下說一次
void safeStop() {
  drive(0, 0);
  if (!stopped) {
    stopped = true;
    Serial.println(F("讀不到手把 → 停車！"));
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

**你會看到：**（兩支搖桿往上推一半，然後把手把關機）

```text
準備好了！用兩支搖桿開車。
手把連上了，可以開車！
左輪 100　右輪 100
讀不到手把 → 停車！
```

> [!TIP]
> 這支範例用一次 `mbcReadMany(ADDR, 0x06, 14, v)` 從 `06` 讀到 `13`（共 14 個），`v[0]` 是「已連線」、`v[11]` 是 LY、`v[13]` 是 RY，同一瞬間的資料，不用分兩次讀。中間的 `0C`~`0F` 是空號，會讀到 `-32768`，不用理它。

---

<h2 id="ex7">7️⃣ 讓手把震動</h2>

**做什麼：** 按住 **✕**，手把震動（力道 200）；壓 **R2** 扳機，壓多深震多大。用的是寫入命令 `E5`：`mbcWrite(ADDR, 0xE5, &強度, 1)`，每送一次震 200 ms，所以按住時每 150 ms 補送一次。

<a href="/examples/ps4/arduino/ps4_07_rumble/ps4_07_rumble.ino" download data-ignore="true">📄 下載 ps4_07_rumble.ino</a>

<!-- ino: examples/ps4/arduino/ps4_07_rumble/ps4_07_rumble.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 7：讓手把震動
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   按住 ✕ 鍵，手把就震動（力道 200）；放開就停。
 *   壓 R2 扳機，壓多深震多大（R2 的 0~255 直接當震動強度）。
 *   用的是命令 E5：寫 [0xE5][強度 0~255]，0 = 停；每送一次震 200 ms，
 *   所以按住時程式每 150 ms 再送一次，震動才會連續。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後按 ✕ 或壓 R2，手把會震。序列埠監控窗（115200）會印出送出的強度。
 *
 * 小提醒：
 *   PS2 接收器收到 E5 會回 ACK 但忽略（PS2 手把震動已關閉），這個範例只對 PS4/PS5 有作用。
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

const uint8_t BTN_CROSS = 14;            // ✕ 在十六鍵的第 14 位
const uint8_t RUMBLE_BY_CROSS = 200;     // 👉 可以改：按 ✕ 時的震動強度 0~255

uint8_t lastSent = 0;                    // 上一次送出的強度
unsigned long lastSend = 0;              // 上一次送的時間

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！按 ✕ 或壓 R2，手把會震動。"));
}

void loop() {
  // 一次讀編號 01 ~ 09 共 9 個值：v[0] = 01 十六鍵、v[8] = 09 R2 類比
  int16_t v[9];
  if (!mbcReadMany(ADDR, 0x01, 9, v)) { delay(100); return; }

  uint8_t strength = 0;                                  // 這一圈要送的強度
  if ((uint16_t)v[0] & (1u << BTN_CROSS)) strength = RUMBLE_BY_CROSS;   // 按住 ✕
  if (v[8] > strength) strength = (uint8_t)v[8];         // R2 壓更深就用 R2 的值

  // 強度有變、或按住超過 150 ms，就送一次 E5（每次只震 200 ms，要一直補送）
  if (strength != lastSent || (strength > 0 && millis() - lastSend >= 150)) {
    mbcWrite(ADDR, 0xE5, &strength, 1);  // 命令 E5：[強度]
    lastSend = millis();
    if (strength != lastSent) {
      Serial.print(F("震動強度 → ")); Serial.println(strength);
    }
    lastSent = strength;
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

**你會看到：**（按 ✕ 再放開，然後 R2 慢慢壓到底）

```text
準備好了！按 ✕ 或壓 R2，手把會震動。
震動強度 → 200
震動強度 → 0
震動強度 → 63
震動強度 → 180
震動強度 → 255
震動強度 → 0
```

---

<h2 id="ex8">8️⃣ 搖桿調燈條顏色</h2>

**做什麼：** 左搖桿左右 → 紅、左搖桿上下 → 綠、右搖桿左右 → 藍，`-100`~`+100` 換算成 `0`~`255`。用的是寫入命令 `E6`：`mbcWrite(ADDR, 0xE6, rgb, 3)`，**要剛好 3 個 byte**。顏色有變才送，最快每 0.1 秒一次。

<a href="/examples/ps4/arduino/ps4_08_lightbar/ps4_08_lightbar.ino" download data-ignore="true">📄 下載 ps4_08_lightbar.ino</a>

<!-- ino: examples/ps4/arduino/ps4_08_lightbar/ps4_08_lightbar.ino -->
```cpp
/*
 * 掌控者 PS4/PS5 × Arduino　範例 8：搖桿調燈條顏色
 * ------------------------------------------------------------
 * 這個範例會做什麼？
 *   推搖桿就改手把燈條的顏色：
 *     左搖桿左右（LX）→ 紅、左搖桿上下（LY）→ 綠、右搖桿左右（RX）→ 藍
 *     -100 ~ +100 換算成 0 ~ 255；都放開 = 中間亮度的白。
 *   用的是命令 E6：寫 [0xE6][R][G][B]，剛好 3 個 byte。
 *   顏色有變才送（最快每 100 ms 一次），不會一直塞 I2C。
 *
 * 你需要：掌控者 PS4/PS5 接收器（通用 I2C 版）＋ PS4 或 PS5 手把（已配對好）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線：
 *   接收器 VCC → 5V      接收器 GND → GND
 *   接收器 RX（= SDA） → A4      接收器 TX（= SCL） → A5         （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   （I2C 版接收器上印的 TX 就是 SCL、RX 就是 SDA）
 *
 * 怎麼看結果：
 *   上傳後推搖桿，看手把燈條變色。序列埠監控窗（115200）會印出 R、G、B。
 *
 * 小提醒：
 *   手把還沒連上時送進去也沒關係，接收器會先記住，連上後套用；預設顏色是藍 (0,0,255)。
 *   PS2 手把沒有燈條，PS2 接收器收到 E6 會回 ACK 但忽略。
 */

#include <Wire.h>

const uint8_t ADDR = 0x42;               // 掌控者接收器的「門牌號碼」（I2C 位址），PS2 與 PS4 都是 0x42
const int16_t MBC_NONE = -32768;         // 「沒有這個值／讀不到」的代號

// 小幫手的宣告（實作在檔案最下面）；mbcWrite 的參數可以省略
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);

uint8_t lastRGB[3] = {255, 255, 255};    // 上一次送出的顏色（故意跟一開始不一樣，第一圈就會送）
unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 5000)) {
    Serial.println(F("找不到接收器，請先跑範例 1 檢查接線。"));
    while (true) {}
  }
  Serial.println(F("準備好了！推搖桿改燈條顏色。"));
}

void loop() {
  if (millis() - lastSend >= 100) {      // 最快每 0.1 秒送一次
    // 一次讀編號 10~12 共 3 個值：[0] LX [1] LY [2] RX（-100 ~ +100）
    int16_t s[3];
    if (!mbcReadMany(ADDR, 0x10, 3, s)) { delay(100); return; }

    uint8_t rgb[3];
    rgb[0] = map(s[0], -100, 100, 0, 255);   // 紅：左搖桿左右
    rgb[1] = map(s[1], -100, 100, 0, 255);   // 綠：左搖桿上下
    rgb[2] = map(s[2], -100, 100, 0, 255);   // 藍：右搖桿左右

    // 顏色有變才送（差 1 以內當作沒變，搖桿輕微抖動不會一直送）
    if (abs(rgb[0] - lastRGB[0]) > 1 || abs(rgb[1] - lastRGB[1]) > 1 || abs(rgb[2] - lastRGB[2]) > 1) {
      mbcWrite(ADDR, 0xE6, rgb, 3);      // 命令 E6：[R][G][B]，要剛好 3 個 byte
      lastSend = millis();
      memcpy(lastRGB, rgb, 3);
      Serial.print(F("燈條 → 紅 ")); Serial.print(rgb[0]);
      Serial.print(F("　綠 ")); Serial.print(rgb[1]);
      Serial.print(F("　藍 ")); Serial.println(rgb[2]);
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

**你會看到：**（搖桿都放開，然後左搖桿推到最右）

```text
準備好了！推搖桿改燈條顏色。
燈條 → 紅 127　綠 127　藍 127
燈條 → 紅 255　綠 127　藍 127
```

> [!NOTE]
> 手把還沒連上時送進去也沒關係，接收器會先記住，連上後就套用；接收器預設是藍色 `(0, 0, 255)`。

---

## ❓ 常見問題

**Q：一直出現「找不到接收器」？**
1. 檢查 TX、RX 有沒有接反（UNO：RX → A4、TX → A5；R4：SDA／SCL；ESP32：21／22）。
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

**Q：讀到 `-32768` 是什麼意思？**
這是「沒有這個值」的代號（範例裡叫 `MBC_NONE`）：編號是空號（例如 `0C`~`0F`），或是讀不到接收器（線鬆了）。寫程式時先檢查 `v != MBC_NONE` 再用。

**Q：小幫手裡的 `delay(2)` 可以拿掉嗎？**
不行。PS4/PS5 接收器是 ESP32 做的從機，它不能把 I2C 時脈拉住等資料，所以主機寫完編號後要等 2 毫秒它才準備好。拿掉會讀到上一次的資料。神攝手也一樣。

**Q：ESP32 當主機時要注意什麼？**
一樣要 `delay(2)`；接線用 GPIO 21（SDA）／22（SCL）。小車範例的馬達腳位 ESP32 跟 UNO 不同，程式裡已用 `#if defined(ESP32)` 分開。

**Q：可以同時接 PS4/PS5 和 PS2 接收器嗎？**
不行。兩款的門牌號碼都是 `0x42`，同一組 SDA/SCL 上只能接一個。但是兩款的**編號完全一樣**，[PS2 的 Arduino 範例](/sensors/ps2/arduino-examples.md) 接這個接收器也能用（震動、燈條只有 PS4/PS5 有作用，PS2 接收器收到會忽略）。用 `F0` 的高 byte（1 = PS2、2 = PS4/PS5）可以分辨接的是哪一款。

**Q：想知道每一個格子放什麼資料？**
請看上面的 [指令總覽](#cmds) 和 [MBC 通用 I2C 協議 v2](/i2c-protocol.md)（給會寫程式的人看的進階資料）。
