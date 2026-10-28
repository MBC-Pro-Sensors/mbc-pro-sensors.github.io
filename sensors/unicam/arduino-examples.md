<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 程式碼區塊來自 examples/unicam/arduino/*/*.ino，由 node _build/sync-arduino-examples.mjs 填入；改程式請改 .ino 後重跑 -->

# 🧪 神攝手通用版 v1 × Arduino 範例

[🔙 回到神攝手通用版首頁](/sensors/unicam/index.md)　｜　[📡 MBC 通用 I2C 協議 v2（進階）](/i2c-protocol.md)

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

這一頁有 **5 個範例**：確認接好、顏色追蹤、座標取色、照順序給色號（WRO 顏色順序）、循線小車。每個範例都可以直接複製、上傳，程式裡每一行都有中文說明。**完全沒寫過程式也沒關係**，照順序一個一個做就好。

> [!TIP]
> 第一次用 Arduino？先看 [🔰 第一次用 Arduino：從零開始](/sensors/arduino-start.md)，15 分鐘學會安裝軟體、上傳程式、看結果。

<div style="display: flex; gap: 15px; flex-wrap: wrap; margin: 25px 0;">
<a href="/examples/unicam/arduino/unicam_arduino_examples.zip" target="_blank" data-ignore="true" download class="btn-download" style="flex: 1; min-width: 220px; text-align: center; background: linear-gradient(135deg, #00d2ff, #007a75); color: #fff; padding: 12px; border-radius: 8px; text-decoration: none; font-weight: bold; font-size: 1rem;">📥 下載全部範例（ZIP）</a>
</div>

---

## 🔌 接線（所有範例都一樣）

| 神攝手通用版（產品端） | Arduino UNO / Nano | Arduino UNO R4 | ESP32 |
| :---: | :---: | :---: | :---: |
| 5V | **外接 5V**（行動電源／變壓器 2 A 以上） | 外接 5V | 外接 5V |
| GND | GND（外接電源的 GND 也接這裡） | GND | GND |
| IO3（= SDA，暫定） | A4 | SDA | GPIO 21 |
| IO1（= SCL，暫定） | A5 | SCL | GPIO 22 |

> [!IMPORTANT]
> - 神攝手要燒 **I2C 版韌體**，I2C 位址 `0x4C`。IO1／IO3 跟樂高（EV3／SPIKE）串口版用同一組腳，**韌體只能二選一**。
> - **SDA＝IO3、SCL＝IO1 是暫定**，要等上機確認；讀不到時先把兩條線對調試試看。
> - 相機（ESP32＋螢幕）峰值電流大，**5V 請外接**，GND 一定要跟 Arduino 接在一起。
> - 相機是 ESP32 做的從機，小幫手裡的 `delay(2)` **不能刪**。開機要幾秒，範例最多等 8 秒。

---

## 🎛️ 先在相機上做兩件事

1. **教色**：即時畫面用左右鍵停在「目標 1」→ 按 **OK** → 用左右鍵把十字移到要追的顏色上 → **OK** 取色（按住 OK 0.8 秒直接完成）。要追更多色就繼續教目標 2、3…（最多 16 色）。範例 2、4、5（色線）會用到。
2. **切相機模式**（可以不做）：即時畫面停在 **MODE** 按 OK → 選相機模式 → 選子選項。Arduino 範例開頭都會用 **E0 命令自動切**，所以這一步可以省略；相機右上角會顯示「模式-子選項」（例 `1-3`）。

| 模式編號 | 相機模式・子選項 | 每色／每點給什麼（I2C 都給 16 色／16 點） | 範例 |
| :---: | :--- | :--- | :---: |
| 11 | 1 顏色追蹤 ① 最多色 | X、Y | — |
| 12 | 1 顏色追蹤 ② 多色 | X、Y、數量 | — |
| 13 | 1 顏色追蹤 ③ 標準 | X、Y、面積、數量 | 1、2 |
| 14 | 1 顏色追蹤 ④ 精細 | X、Y、寬、高、數量、面積 | — |
| 21／22／23 | 2 座標取色 ①／②／③ | 色相／＋飽和／＋明度 | 3 |
| 30 | 3 照順序給色號 | 由左到右每塊的色相碼＋總塊數 | 4 |
| 41／42／43 | 4 循線 黑線／白線／色線 | 近中遠偏移、角度、路口…14 個值 | 5 |

---

<h2 id="cmds">📖 值編號總覽（讀與寫）</h2>

神攝手就像一排有編號的「格子」，每一格放一個數字（int16）。寫 `[編號]`、等 2 毫秒、再讀，就拿到從這一格起連續的值（一次最多 16 個）。

| 編號 | 名稱 | 範圍 | 說明 |
| :---: | :--- | :--- | :--- |
| `01` | 模式編號 | 11～43 | 相機模式 × 10 ＋ 子選項（照順序給色號＝30） |
| `02` | 子選項 | 0～4 | |
| `03` | 已教幾色 | 0～16 | |
| `04` | 這一層給幾個值 | | 例：模式 13 給 65 個（16 色 × 4 ＋ 找到幾色） |
| `10` 起 | 這個模式的值 | | 依模式排（見下表）；沒有的編號回 `-32768` |
| `F0` | 裝置 ID | | 低 byte＝`0x4C`、高 byte **hw0＝3（通用版）**；學校版 1、WRO 版 2 |
| `F1` | 協議版本 | 2 | |
| `F2` | 韌體版本 | 100 | 主 × 100 ＋ 次（100 ＝ 1.0） |
| `F3` | 狀態旗標 | bit | bit0＝準備好（`mbcWaitReady` 用它） |

**`10` 起的排法**（第 c 色、第 p 點都從 1 起算）：

| 模式 | 排法 | 最後一格 |
| :---: | :--- | :--- |
| 11 | 每色 2 格：`10+2(c−1)`＝X、＋1＝Y | `30`＝找到幾色 |
| 12 | 每色 3 格：X、Y、數量 | `40`＝找到幾色 |
| 13 | 每色 4 格：X、Y、面積 %、數量 | `50`＝找到幾色 |
| 14 | 每色 6 格：X、Y、寬、高、數量、面積 % | `70`＝找到幾色 |
| 21／22／23 | 每點 1／2／3 格：色相碼（、飽和度、明度） | — |
| 30 | `10`～`1F`＝左起第 1～16 塊色相碼 | `20`＝總塊數 |
| 41～43 | `10` 近處偏移、`11` 中段、`12` 遠處、`13` 角度、`14` 近處線寬 %、`15` 有線旗標（近 1＋中 2＋遠 4）、`16` 路口類型（0 無 1 十字 2 T 3 左 4 右 5 盡頭）、`17` 路口距離 %、`18` 左緣、`19` 右緣、`1A` 遠處線寬 %、`1B` 標記線色號、`1C` 標記條數、`1D` 最後離線方向（0 有線 1 左 2 右） | — |

- **座標**：X 0～159（左 → 右）、Y 0～119（上 → 下），**0＝沒找到**（找到時至少是 1）。
- **色相碼** 0～255：0～240 彩色（紅 0／240、黃 40、綠 80、青 120、藍 160、紫 200）、241～248 白／灰、249～255 黑。
- **偏移、角度、左右緣** 有正負：**線在左＝負**（站在車子後面往前看）。

**寫入命令：**

| 命令 | 參數 | 做什麼 |
| :---: | :--- | :--- |
| `E0` | [相機模式 1～4, 子選項] | 切模式（存進相機，斷電也記得） |
| `E1` | [第幾點 1～20, X 0～159, Y 0～119] | 改座標取色的取樣點位置（存進相機） |

> [!NOTE]
> 目標顏色**只能在相機上教**（v1 沒有寫入顏色的命令）。

---

## 🧰 小幫手（每個範例最下面那一段）

每個範例最上面和最下面都有一段一模一樣的程式（`MBC_UNICAM_…_BEGIN`～`END` 之間），負責跟相機「講話」。你不用改它，只要會用這幾個：

| 小幫手 | 做什麼 | 需要的模式 |
| :--- | :--- | :---: |
| `ucSetMode(相機模式, 子選項)` | E0 切模式（照順序給色號填 `ucSetMode(3, 0)`） | 全部 |
| `ucSetPoint(第幾點, X, Y)` | E1 移動取樣點 | 21～23 |
| `ucModeNumber()` | 讀目前的模式編號（例 13） | 全部 |
| `ucReadBlob(模式, 第幾色, b)` | 讀一色 → `UcBlob b`：`b.x`、`b.y`、`b.w`、`b.h`、`b.count`、`b.area`（這個模式沒給的是 -1） | 11～14 |
| `ucColorsFound(模式)` | 16 色裡找到幾色 | 11～14 |
| `ucReadSpot(模式, 第幾點, s)` | 讀一點 → `UcSpot s`：`s.h`、`s.s`、`s.v`（沒給的是 -1） | 21～23 |
| `ucReadOrder(陣列, 最多幾個, total)` | 由左到右的色相碼放進陣列，回傳放了幾個；`total`＝總塊數 | 30 |
| `ucReadLine(l)` | 一次讀 14 個 → `UcLine l`：`nearOff`、`midOff`、`farOff`、`angle`、`width`、`flags`、`junction`、`junctionDist`、`leftEdge`、`rightEdge`、`farWidth`、`marker`、`markerCount`、`lost` | 41～43 |
| `ucColorCode(色相碼)` | 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑 | 任何 |
| `mbcRead`／`mbcReadMany`／`mbcWaitReady` | 直接讀編號、等相機開機 | 任何 |

> [!TIP]
> **想自己寫程式？** 下載 [MBC_UniCam.h](/examples/unicam/arduino/MBC_UniCam.h)，放進你的草稿碼資料夾（跟 .ino 同一層），程式開頭寫 `#include "MBC_UniCam.h"`，就能直接用上面這些小幫手，不用把整段貼進去。

---

## 📋 範例總覽

| # | 範例 | 相機模式・子選項 | 學會什麼 | 另外要準備 |
| :---: | :--- | :--- | :--- | :--- |
| 1 | [接好了嗎？](#ex1) | 1 顏色追蹤 ③（13） | 讀裝置 ID 確認通用版、韌體版、用 E0 切模式 | 無 |
| 2 | [色塊在哪裡？](#ex2) | 1 顏色追蹤 ③（13） | 4 色的 X／Y／面積／數量 | 教過顏色的物品 |
| 3 | [這是什麼顏色？](#ex3) | 2 座標取色 ③（23） | E1 移動取樣點、色相碼 → 顏色名稱 | 幾塊彩色積木 |
| 4 | [顏色順序（WRO）](#ex4) | 3 照順序給色號（30） | 由左到右每塊的顏色＋總塊數 | 一排顏色方塊 |
| 5 | [看前方的循線小車（進階）](#ex5) | 4 循線 ① 黑線（41） | 近／遠偏移 PD 循線、路口停車 | 兩輪小車、L298N |

> [!TIP]
> **建議順序：1 → 2 → 3 → 4 → 5。** 每換一個範例，程式開頭會自動把相機切到需要的模式。

---

<h2 id="ex1">1️⃣ 接好了嗎？</h2>

**需要的相機模式：** 相機模式 1 顏色追蹤・子選項 ③ 標準（程式開頭會用 E0 自動切）

**做什麼：** 讀 `F0` 裝置 ID，確認低 byte 是 `0x4C`、高 byte（hw0）是 **3＝通用版**；印出協議版、韌體版；用 `ucSetMode(1, 3)` 把相機切到模式 13，再讀 `01` 確認真的換過去。之後每秒印一次目前的模式編號，你在相機選單換模式，這裡也會跟著變。每次接新的相機，都先跑這一個。

**步驟：**
1. 照上面的表格接好 4 條線，外接 5V 打開、USB 插上電腦。
2. 複製下面的程式，貼到 Arduino IDE，按 **→ 上傳**。
3. 打開序列埠監控窗（右上角放大鏡），速度選 **115200**。

<a href="/examples/unicam/arduino/unicam_01_hello/unicam_01_hello.ino" download data-ignore="true">📄 下載 unicam_01_hello.ino</a>

<!-- ino: examples/unicam/arduino/unicam_01_hello/unicam_01_hello.ino -->
```cpp
/*
 * 神攝手通用版 v1 × Arduino　範例 1：接好了嗎？
 * ------------------------------------------------------------
 * 需要把相機選單切到：相機模式 1 顏色追蹤 ・子選項 ③ 標準（程式開頭也會用 E0 自動切）
 *
 * 這個範例會做什麼？
 *   1. 讀 F0 裝置 ID：低位元組 = 0x4C（位址）、高位元組 hw0 = 3 就是「通用版」
 *      （hw0 = 1 是學校版、2 是 WRO 版，讀法不一樣，要用各自的範例）
 *   2. 讀 F2 韌體版本（100 = 1.0）
 *   3. 用 E0 命令把相機切到「模式 13」，再讀 0x01 確認相機真的換過去
 *   4. 之後每秒印一次目前的模式編號（你在相機選單換模式，這裡也會跟著變）
 *
 * 模式編號 = 相機模式 × 10 + 子選項：
 *   11~14 顏色追蹤、21~23 座標取色、30 照順序給色號、41~43 循線
 *
 * 你需要：神攝手通用版 v1（I2C 版韌體）、Arduino UNO／Nano／UNO R4 或 ESP32。
 *
 * 接線（I2C 腳位是暫定，等上機確認）：
 *   神攝手 5V  → 外接 5V（2 A 以上的行動電源或變壓器；不要從 Arduino 的 5V 腳拉大電流）
 *   神攝手 GND → Arduino GND（外接電源的 GND 也要接在一起）
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 要燒「I2C 版」韌體。IO1/IO3 跟樂高串口版共用，韌體二選一。
 *
 * 怎麼看結果：上傳後打開「序列埠監控窗」，速度選 115200。
 */

#include <Wire.h>                        // Arduino 內建的 I2C 通訊工具

// ==== MBC_UNICAM_TOP_BEGIN ====
const uint8_t ADDR = 0x4C;               // 神攝手的 I2C 位址（門牌號碼）
const int16_t MBC_NONE = -32768;         // 「讀不到／沒有這個值」的代號
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);   // 小幫手的宣告（實作在最下面）
// 一個色塊：x、y 像素（0 = 沒找到）、w、h 像素、count 塊數、area 面積 %；這個模式沒給的值是 -1
struct UcBlob { int16_t x, y, w, h, count, area; };
// 一個取樣點：h 色相碼 0~255、s 飽和度、v 明度 0~100；沒給的是 -1
struct UcSpot { int16_t h, s, v; };
// 循線 14 個值（近／中／遠偏移 -100~100：線在左 = 負）
struct UcLine { int16_t nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_TOP_END ====

unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {        // 最多等 8 秒讓相機開機
    Serial.println(F("找不到神攝手通用版！請檢查："));
    Serial.println(F(" 1. 相機有接外接 5V、GND 有跟 Arduino 接在一起"));
    Serial.println(F(" 2. IO3 接 SDA、IO1 接 SCL（接反了就對調）"));
    Serial.println(F(" 3. 相機燒的是「I2C 版」韌體"));
    while (true) {}                      // 停在這裡
  }

  // 一次讀 F0~F3 四個值：[0] 裝置 ID [1] 協議版 [2] 韌體版 [3] 狀態旗標
  int16_t id[4];
  if (!mbcReadMany(ADDR, 0xF0, 4, id)) { Serial.println(F("讀不到 F0~F3")); while (true) {} }
  uint8_t addrLow = id[0] & 0xFF;        // 低位元組：出廠位址
  uint8_t hw0 = (id[0] >> 8) & 0xFF;     // 高位元組：哪一版（3 = 通用版）
  Serial.print(F("位址 0x")); Serial.print(addrLow, HEX);
  Serial.print(F("  hw0=")); Serial.print(hw0);
  if (addrLow == 0x4C && hw0 == 3) Serial.println(F("  → 是神攝手通用版 ✔"));
  else if (hw0 == 1) Serial.println(F("  → 這是學校版，請改用學校版的範例"));
  else if (hw0 == 2) Serial.println(F("  → 這是 WRO 版（HUE12），請改用 WRO 版的範例"));
  else Serial.println(F("  → 不認得的版本"));
  Serial.print(F("協議版 ")); Serial.print(id[1]);
  Serial.print(F("  韌體版 ")); Serial.print(id[2] / 100);
  Serial.print('.'); Serial.println(id[2] % 100);

  // E0 切模式：相機模式 1（顏色追蹤）、子選項 3（標準 XY＋面積）→ 模式編號 13
  ucSetMode(1, 3);                       // 👉 可以改：例如 ucSetMode(4, 1) = 循線黑線
  delay(200);
  Serial.print(F("切換後的模式編號："));
  Serial.println(ucModeNumber());        // 應該印 13
}

void loop() {
  if (millis() - lastPrint >= 1000) {    // 每 1 秒印一次
    lastPrint = millis();
    // 0x01~0x04：[0] 模式編號 [1] 子選項 [2] 已教幾色 [3] 這一層給幾個
    int16_t m[4];
    if (!mbcReadMany(ADDR, 0x01, 4, m)) { Serial.println(F("讀不到神攝手")); return; }
    Serial.print(F("模式編號 ")); Serial.print(m[0]);
    Serial.print(F("  子選項 ")); Serial.print(m[1]);
    Serial.print(F("  已教 ")); Serial.print(m[2]);
    Serial.print(F(" 色  這一層給 ")); Serial.println(m[3]);
  }
  delay(20);
}

// ==== MBC_UNICAM_HELPERS_BEGIN ====
// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）＋「神攝手通用版小幫手」。所有範例都一樣，不用改。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）。UNO/Nano：SDA=A4、SCL=A5；ESP32：GPIO21／22
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時不會卡住
#endif
}

// 由編號 id 起連續讀 k 個值（1~16）放進 out；全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte）
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 相機（ESP32）一定要等 2 ms 再讀
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();
    uint16_t hi = Wire.read();
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值；讀不到回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]；成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（F3 的 bit0 = 1），最多等 ms 毫秒
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}

// ---------- 神攝手通用版 ----------

// 切相機模式（E0）：camMode 1~4、sub 子選項（照順序給色號填 0）。會存進相機
bool ucSetMode(uint8_t camMode, uint8_t sub) {
  uint8_t p[2] = { camMode, sub };
  bool ok = mbcWrite(ADDR, 0xE0, p, 2);
  delay(50);                             // 給相機一點時間換模式
  return ok;
}

// 改取樣點位置（E1）：n 第幾點 1~20、x 0~159、y 0~119。會存進相機
bool ucSetPoint(uint8_t n, uint8_t x, uint8_t y) {
  uint8_t p[3] = { n, x, y };
  return mbcWrite(ADDR, 0xE1, p, 3);
}

// 相機目前的模式編號（例 13 = 顏色追蹤 ③）；讀不到回傳 MBC_NONE
int16_t ucModeNumber() {
  return mbcRead(ADDR, 0x01);
}

// 顏色追蹤（模式 11~14）每色佔幾個值 / values per colour
uint8_t ucStride(uint8_t mode) {
  if (mode == 11) return 2;              // X Y
  if (mode == 12) return 3;              // X Y 數量
  if (mode == 13) return 4;              // X Y 面積 數量
  if (mode == 14) return 6;              // X Y 寬 高 數量 面積
  return 0;
}

// 讀色 color（1~16）的色塊；mode = 你切的模式編號 11~14。讀到回傳 true
bool ucReadBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  uint8_t k = ucStride(mode);
  if (k == 0 || color < 1 || color > 16) return false;
  int16_t d[6];
  if (!mbcReadMany(ADDR, 0x10 + (color - 1) * k, k, d)) return false;
  b.x = d[0]; b.y = d[1];
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 12) b.count = d[2];
  if (mode == 13) { b.area = d[2]; b.count = d[3]; }
  if (mode == 14) { b.w = d[2]; b.h = d[3]; b.count = d[4]; b.area = d[5]; }
  return true;
}

// 16 個目標色裡找到幾色（模式 11~14）
int16_t ucColorsFound(uint8_t mode) {
  uint8_t k = ucStride(mode);
  if (k == 0) return MBC_NONE;
  return mbcRead(ADDR, 0x10 + 16 * k);
}

// 讀取樣點 point（1~16）；mode = 21、22、23
bool ucReadSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (mode < 21 || mode > 23 || point < 1 || point > 16) return false;
  uint8_t k = mode - 20;                 // 21 → 1 個值、22 → 2、23 → 3
  int16_t d[3];
  if (!mbcReadMany(ADDR, 0x10 + (point - 1) * k, k, d)) return false;
  p.h = d[0];
  p.s = (k >= 2) ? d[1] : -1;
  p.v = (k >= 3) ? d[2] : -1;
  return true;
}

// 照順序給色號（模式 30）：hues 放由左到右每塊的色相碼（最多 maxN 個、≤16），回傳放了幾個；total = 總塊數
uint8_t ucReadOrder(int16_t *hues, uint8_t maxN, int16_t &total) {
  int16_t d[16];
  total = mbcRead(ADDR, 0x20);
  if (total == MBC_NONE || !mbcReadMany(ADDR, 0x10, 16, d)) { total = MBC_NONE; return 0; }
  uint8_t n = (total < 16) ? (uint8_t)total : 16;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = d[i];
  return n;
}

// 循線（模式 41~43）：一次讀 14 個值
bool ucReadLine(UcLine &l) {
  int16_t d[14];
  if (!mbcReadMany(ADDR, 0x10, 14, d)) return false;
  l.nearOff = d[0]; l.midOff = d[1]; l.farOff = d[2]; l.angle = d[3];
  l.width = d[4]; l.flags = d[5]; l.junction = d[6]; l.junctionDist = d[7];
  l.leftEdge = d[8]; l.rightEdge = d[9]; l.farWidth = d[10];
  l.marker = d[11]; l.markerCount = d[12]; l.lost = d[13];
  return true;
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
uint8_t ucColorCode(int16_t h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_HELPERS_END ====
```

**你會看到：**

```text
位址 0x4C  hw0=3  → 是神攝手通用版 ✔
協議版 2  韌體版 1.0
切換後的模式編號：13
模式編號 13  子選項 3  已教 2 色  這一層給 65
模式編號 13  子選項 3  已教 2 色  這一層給 65
```

**試試看：** 在相機選單把模式換成「4 循線 ①」，序列埠應該變成 `模式編號 41`。把程式裡的 `ucSetMode(1, 3)` 改成 `ucSetMode(2, 1)` 再上傳，開機後應該印 `21`。

---

<h2 id="ex2">2️⃣ 色塊在哪裡？</h2>

**需要的相機模式：** 相機模式 1 顏色追蹤・子選項 ③ 標準（程式開頭會用 E0 自動切）

**做什麼：** 每 0.2 秒讀色 1～4 的位置、面積和塊數，用中文說色塊在「左／中／右」「上／中／下」。`b.x = 0` 代表這個顏色沒找到。

**步驟：**
1. 先在相機上教 1～4 種顏色（見上面「先在相機上做兩件事」）。
2. 上傳程式、打開序列埠監控窗（115200）。
3. 拿教過的顏色在鏡頭前左右、上下、遠近移動。

<a href="/examples/unicam/arduino/unicam_02_track/unicam_02_track.ino" download data-ignore="true">📄 下載 unicam_02_track.ino</a>

<!-- ino: examples/unicam/arduino/unicam_02_track/unicam_02_track.ino -->
```cpp
/*
 * 神攝手通用版 v1 × Arduino　範例 2：色塊在哪裡？（顏色追蹤）
 * ------------------------------------------------------------
 * 需要把相機選單切到：相機模式 1 顏色追蹤 ・子選項 ③ 標準（程式開頭也會用 E0 自動切）
 *
 * 這個範例會做什麼？
 *   每 0.2 秒讀色 1~4 的位置與面積，用中文說色塊在「左／中／右」「上／中／下」。
 *   X 0~159（左 → 右）、Y 0~119（上 → 下），X = 0 代表沒找到；面積 = 佔畫面幾 %。
 *
 * 準備：先在相機上「教色」——即時畫面停在「目標 1」按 OK，對準要追的顏色取色；
 *   想追更多色就繼續教目標 2、3、4。
 *
 * 接線（I2C 腳位是暫定，等上機確認）：
 *   神攝手 5V  → 外接 5V（2 A 以上的行動電源或變壓器；不要從 Arduino 的 5V 腳拉大電流）
 *   神攝手 GND → Arduino GND（外接電源的 GND 也要接在一起）
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 要燒「I2C 版」韌體。IO1/IO3 跟樂高串口版共用，韌體二選一。
 *
 * 怎麼看結果：打開「序列埠監控窗」（115200），拿教過的顏色在鏡頭前移動。
 */

#include <Wire.h>

// ==== MBC_UNICAM_TOP_BEGIN ====
const uint8_t ADDR = 0x4C;               // 神攝手的 I2C 位址（門牌號碼）
const int16_t MBC_NONE = -32768;         // 「讀不到／沒有這個值」的代號
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);   // 小幫手的宣告（實作在最下面）
// 一個色塊：x、y 像素（0 = 沒找到）、w、h 像素、count 塊數、area 面積 %；這個模式沒給的值是 -1
struct UcBlob { int16_t x, y, w, h, count, area; };
// 一個取樣點：h 色相碼 0~255、s 飽和度、v 明度 0~100；沒給的是 -1
struct UcSpot { int16_t h, s, v; };
// 循線 14 個值（近／中／遠偏移 -100~100：線在左 = 負）
struct UcLine { int16_t nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_TOP_END ====

const uint8_t MODE = 13;                 // 模式編號 13 = 顏色追蹤 ③ 標準（每色 X、Y、面積、數量）
const uint8_t COLORS = 4;                // 👉 可以改：要看幾色（1~16）
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {        // 最多等 8 秒讓相機開機
    Serial.println(F("找不到神攝手通用版！請檢查："));
    Serial.println(F(" 1. 相機有接外接 5V、GND 有跟 Arduino 接在一起"));
    Serial.println(F(" 2. IO3 接 SDA、IO1 接 SCL（接反了就對調）"));
    Serial.println(F(" 3. 相機燒的是「I2C 版」韌體"));
    while (true) {}                      // 停在這裡
  }
  ucSetMode(1, 3);                       // E0：切到相機模式 1、子選項 3
  Serial.println(F("準備好了！拿教過的顏色到鏡頭前。"));
}

void loop() {
  if (millis() - lastPrint < 200) return; // 👉 可以改：每 0.2 秒印一次
  lastPrint = millis();

  for (uint8_t c = 1; c <= COLORS; c++) {
    UcBlob b;
    if (!ucReadBlob(MODE, c, b)) { Serial.println(F("讀不到神攝手")); return; }
    Serial.print(F("色")); Serial.print(c); Serial.print(F(": "));
    if (b.x == 0) {                      // X = 0：沒找到這個顏色
      Serial.print(F("沒看到        "));
      continue;
    }
    // X：0~159 分成左／中／右；Y：0~119 分成上／中／下（上 = 小）
    if (b.x < 53)       Serial.print(F("左"));
    else if (b.x > 106) Serial.print(F("右"));
    else                Serial.print(F("中"));
    if (b.y < 40)       Serial.print(F("上"));
    else if (b.y > 80)  Serial.print(F("下"));
    else                Serial.print(F("中"));
    Serial.print(F(" X=")); Serial.print(b.x);
    Serial.print(F(" Y=")); Serial.print(b.y);
    Serial.print(F(" 面積")); Serial.print(b.area); Serial.print(F("%"));
    Serial.print(F(" ×")); Serial.print(b.count); Serial.print(F("塊  "));
  }
  Serial.println();
}

// ==== MBC_UNICAM_HELPERS_BEGIN ====
// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）＋「神攝手通用版小幫手」。所有範例都一樣，不用改。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）。UNO/Nano：SDA=A4、SCL=A5；ESP32：GPIO21／22
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時不會卡住
#endif
}

// 由編號 id 起連續讀 k 個值（1~16）放進 out；全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte）
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 相機（ESP32）一定要等 2 ms 再讀
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();
    uint16_t hi = Wire.read();
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值；讀不到回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]；成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（F3 的 bit0 = 1），最多等 ms 毫秒
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}

// ---------- 神攝手通用版 ----------

// 切相機模式（E0）：camMode 1~4、sub 子選項（照順序給色號填 0）。會存進相機
bool ucSetMode(uint8_t camMode, uint8_t sub) {
  uint8_t p[2] = { camMode, sub };
  bool ok = mbcWrite(ADDR, 0xE0, p, 2);
  delay(50);                             // 給相機一點時間換模式
  return ok;
}

// 改取樣點位置（E1）：n 第幾點 1~20、x 0~159、y 0~119。會存進相機
bool ucSetPoint(uint8_t n, uint8_t x, uint8_t y) {
  uint8_t p[3] = { n, x, y };
  return mbcWrite(ADDR, 0xE1, p, 3);
}

// 相機目前的模式編號（例 13 = 顏色追蹤 ③）；讀不到回傳 MBC_NONE
int16_t ucModeNumber() {
  return mbcRead(ADDR, 0x01);
}

// 顏色追蹤（模式 11~14）每色佔幾個值 / values per colour
uint8_t ucStride(uint8_t mode) {
  if (mode == 11) return 2;              // X Y
  if (mode == 12) return 3;              // X Y 數量
  if (mode == 13) return 4;              // X Y 面積 數量
  if (mode == 14) return 6;              // X Y 寬 高 數量 面積
  return 0;
}

// 讀色 color（1~16）的色塊；mode = 你切的模式編號 11~14。讀到回傳 true
bool ucReadBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  uint8_t k = ucStride(mode);
  if (k == 0 || color < 1 || color > 16) return false;
  int16_t d[6];
  if (!mbcReadMany(ADDR, 0x10 + (color - 1) * k, k, d)) return false;
  b.x = d[0]; b.y = d[1];
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 12) b.count = d[2];
  if (mode == 13) { b.area = d[2]; b.count = d[3]; }
  if (mode == 14) { b.w = d[2]; b.h = d[3]; b.count = d[4]; b.area = d[5]; }
  return true;
}

// 16 個目標色裡找到幾色（模式 11~14）
int16_t ucColorsFound(uint8_t mode) {
  uint8_t k = ucStride(mode);
  if (k == 0) return MBC_NONE;
  return mbcRead(ADDR, 0x10 + 16 * k);
}

// 讀取樣點 point（1~16）；mode = 21、22、23
bool ucReadSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (mode < 21 || mode > 23 || point < 1 || point > 16) return false;
  uint8_t k = mode - 20;                 // 21 → 1 個值、22 → 2、23 → 3
  int16_t d[3];
  if (!mbcReadMany(ADDR, 0x10 + (point - 1) * k, k, d)) return false;
  p.h = d[0];
  p.s = (k >= 2) ? d[1] : -1;
  p.v = (k >= 3) ? d[2] : -1;
  return true;
}

// 照順序給色號（模式 30）：hues 放由左到右每塊的色相碼（最多 maxN 個、≤16），回傳放了幾個；total = 總塊數
uint8_t ucReadOrder(int16_t *hues, uint8_t maxN, int16_t &total) {
  int16_t d[16];
  total = mbcRead(ADDR, 0x20);
  if (total == MBC_NONE || !mbcReadMany(ADDR, 0x10, 16, d)) { total = MBC_NONE; return 0; }
  uint8_t n = (total < 16) ? (uint8_t)total : 16;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = d[i];
  return n;
}

// 循線（模式 41~43）：一次讀 14 個值
bool ucReadLine(UcLine &l) {
  int16_t d[14];
  if (!mbcReadMany(ADDR, 0x10, 14, d)) return false;
  l.nearOff = d[0]; l.midOff = d[1]; l.farOff = d[2]; l.angle = d[3];
  l.width = d[4]; l.flags = d[5]; l.junction = d[6]; l.junctionDist = d[7];
  l.leftEdge = d[8]; l.rightEdge = d[9]; l.farWidth = d[10];
  l.marker = d[11]; l.markerCount = d[12]; l.lost = d[13];
  return true;
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
uint8_t ucColorCode(int16_t h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_HELPERS_END ====
```

**你會看到：**（教了 2 色，只拿第 1 色到鏡頭前）

```text
色1: 左上 X=30 Y=25 面積4% ×1塊  色2: 沒看到        色3: 沒看到        色4: 沒看到        
```

**試試看：** 把 `COLORS` 改成 `8`，就能看 8 色（I2C 一次給 16 色）。想要寬、高，就改用模式 14：`ucSetMode(1, 4)`、`MODE = 14`，再印 `b.w`、`b.h`。

---

<h2 id="ex3">3️⃣ 這是什麼顏色？</h2>

**需要的相機模式：** 相機模式 2 座標取色・子選項 ③ 色相＋飽和＋明度（程式開頭會用 E0 自動切）

**做什麼：** 用 `E1` 把取樣點 1～3 排在畫面中間一列（左 X=40、中 X=80、右 X=120，Y 都是 60），每 0.3 秒讀 3 個點的色相碼、飽和度、明度，用 `ucColorCode` 換成顏色名稱。取樣點位置會存在相機裡，相機畫面上會畫十字和編號。

**步驟：**
1. 上傳程式、打開序列埠監控窗（115200）。
2. 在鏡頭前左、中、右各放一塊顏色積木，看相機畫面上的十字有沒有落在積木上。

<a href="/examples/unicam/arduino/unicam_03_spot_color/unicam_03_spot_color.ino" download data-ignore="true">📄 下載 unicam_03_spot_color.ino</a>

<!-- ino: examples/unicam/arduino/unicam_03_spot_color/unicam_03_spot_color.ino -->
```cpp
/*
 * 神攝手通用版 v1 × Arduino　範例 3：這是什麼顏色？（座標取色）
 * ------------------------------------------------------------
 * 需要把相機選單切到：相機模式 2 座標取色 ・子選項 ③ 色相＋飽和＋明度（程式開頭也會用 E0 自動切）
 *
 * 這個範例會做什麼？
 *   1. 用 E1 命令把取樣點 1~3 排在畫面中間一列：左（40, 60）、中（80, 60）、右（120, 60）
 *      （取樣點位置會存在相機裡；相機畫面上會畫十字和編號）
 *   2. 每 0.3 秒讀 3 個點的色相碼、飽和度、明度，印出顏色名稱。
 *   色相碼 0~240 是彩色（紅 0／240、黃 40、綠 80、青 120、藍 160、紫 200），241~248 白／灰，249~255 黑。
 *
 * 接線（I2C 腳位是暫定，等上機確認）：
 *   神攝手 5V  → 外接 5V（2 A 以上的行動電源或變壓器；不要從 Arduino 的 5V 腳拉大電流）
 *   神攝手 GND → Arduino GND（外接電源的 GND 也要接在一起）
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 要燒「I2C 版」韌體。IO1/IO3 跟樂高串口版共用，韌體二選一。
 *
 * 怎麼看結果：打開「序列埠監控窗」（115200），在鏡頭前左、中、右各放一塊顏色積木。
 */

#include <Wire.h>

// ==== MBC_UNICAM_TOP_BEGIN ====
const uint8_t ADDR = 0x4C;               // 神攝手的 I2C 位址（門牌號碼）
const int16_t MBC_NONE = -32768;         // 「讀不到／沒有這個值」的代號
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);   // 小幫手的宣告（實作在最下面）
// 一個色塊：x、y 像素（0 = 沒找到）、w、h 像素、count 塊數、area 面積 %；這個模式沒給的值是 -1
struct UcBlob { int16_t x, y, w, h, count, area; };
// 一個取樣點：h 色相碼 0~255、s 飽和度、v 明度 0~100；沒給的是 -1
struct UcSpot { int16_t h, s, v; };
// 循線 14 個值（近／中／遠偏移 -100~100：線在左 = 負）
struct UcLine { int16_t nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_TOP_END ====

const uint8_t MODE = 23;                 // 模式編號 23 = 座標取色 ③（色相＋飽和＋明度）
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {        // 最多等 8 秒讓相機開機
    Serial.println(F("找不到神攝手通用版！請檢查："));
    Serial.println(F(" 1. 相機有接外接 5V、GND 有跟 Arduino 接在一起"));
    Serial.println(F(" 2. IO3 接 SDA、IO1 接 SCL（接反了就對調）"));
    Serial.println(F(" 3. 相機燒的是「I2C 版」韌體"));
    while (true) {}                      // 停在這裡
  }
  ucSetMode(2, 3);                       // E0：切到相機模式 2、子選項 3
  // E1：取樣點 第幾點, X, Y（X 0~159、Y 0~119）
  ucSetPoint(1, 40, 60);                 // 👉 可以改：點 1 放左邊
  ucSetPoint(2, 80, 60);                 // 點 2 放中間
  ucSetPoint(3, 120, 60);                // 點 3 放右邊
  Serial.println(F("準備好了！在左、中、右各放一塊顏色積木。"));
}

void loop() {
  if (millis() - lastPrint < 300) return;
  lastPrint = millis();

  for (uint8_t p = 1; p <= 3; p++) {
    UcSpot s;
    if (!ucReadSpot(MODE, p, s)) { Serial.println(F("讀不到神攝手")); return; }
    Serial.print(F("點")); Serial.print(p); Serial.print(F(": "));
    printColorName(ucColorCode(s.h));    // 色相碼 → 顏色名稱
    Serial.print(F(" (H")); Serial.print(s.h);
    Serial.print(F(" S")); Serial.print(s.s);
    Serial.print(F(" V")); Serial.print(s.v);
    Serial.print(F(")   "));
  }
  Serial.println();
}

// 印出顏色名稱（代號來自 ucColorCode）
void printColorName(uint8_t code) {
  switch (code) {
    case 1: Serial.print(F("紅")); break;
    case 2: Serial.print(F("橙")); break;
    case 3: Serial.print(F("黃")); break;
    case 4: Serial.print(F("綠")); break;
    case 5: Serial.print(F("青")); break;
    case 6: Serial.print(F("藍")); break;
    case 7: Serial.print(F("紫")); break;
    case 8: Serial.print(F("白／灰")); break;
    default: Serial.print(F("黑")); break;
  }
}

// ==== MBC_UNICAM_HELPERS_BEGIN ====
// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）＋「神攝手通用版小幫手」。所有範例都一樣，不用改。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）。UNO/Nano：SDA=A4、SCL=A5；ESP32：GPIO21／22
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時不會卡住
#endif
}

// 由編號 id 起連續讀 k 個值（1~16）放進 out；全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte）
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 相機（ESP32）一定要等 2 ms 再讀
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();
    uint16_t hi = Wire.read();
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值；讀不到回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]；成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（F3 的 bit0 = 1），最多等 ms 毫秒
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}

// ---------- 神攝手通用版 ----------

// 切相機模式（E0）：camMode 1~4、sub 子選項（照順序給色號填 0）。會存進相機
bool ucSetMode(uint8_t camMode, uint8_t sub) {
  uint8_t p[2] = { camMode, sub };
  bool ok = mbcWrite(ADDR, 0xE0, p, 2);
  delay(50);                             // 給相機一點時間換模式
  return ok;
}

// 改取樣點位置（E1）：n 第幾點 1~20、x 0~159、y 0~119。會存進相機
bool ucSetPoint(uint8_t n, uint8_t x, uint8_t y) {
  uint8_t p[3] = { n, x, y };
  return mbcWrite(ADDR, 0xE1, p, 3);
}

// 相機目前的模式編號（例 13 = 顏色追蹤 ③）；讀不到回傳 MBC_NONE
int16_t ucModeNumber() {
  return mbcRead(ADDR, 0x01);
}

// 顏色追蹤（模式 11~14）每色佔幾個值 / values per colour
uint8_t ucStride(uint8_t mode) {
  if (mode == 11) return 2;              // X Y
  if (mode == 12) return 3;              // X Y 數量
  if (mode == 13) return 4;              // X Y 面積 數量
  if (mode == 14) return 6;              // X Y 寬 高 數量 面積
  return 0;
}

// 讀色 color（1~16）的色塊；mode = 你切的模式編號 11~14。讀到回傳 true
bool ucReadBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  uint8_t k = ucStride(mode);
  if (k == 0 || color < 1 || color > 16) return false;
  int16_t d[6];
  if (!mbcReadMany(ADDR, 0x10 + (color - 1) * k, k, d)) return false;
  b.x = d[0]; b.y = d[1];
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 12) b.count = d[2];
  if (mode == 13) { b.area = d[2]; b.count = d[3]; }
  if (mode == 14) { b.w = d[2]; b.h = d[3]; b.count = d[4]; b.area = d[5]; }
  return true;
}

// 16 個目標色裡找到幾色（模式 11~14）
int16_t ucColorsFound(uint8_t mode) {
  uint8_t k = ucStride(mode);
  if (k == 0) return MBC_NONE;
  return mbcRead(ADDR, 0x10 + 16 * k);
}

// 讀取樣點 point（1~16）；mode = 21、22、23
bool ucReadSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (mode < 21 || mode > 23 || point < 1 || point > 16) return false;
  uint8_t k = mode - 20;                 // 21 → 1 個值、22 → 2、23 → 3
  int16_t d[3];
  if (!mbcReadMany(ADDR, 0x10 + (point - 1) * k, k, d)) return false;
  p.h = d[0];
  p.s = (k >= 2) ? d[1] : -1;
  p.v = (k >= 3) ? d[2] : -1;
  return true;
}

// 照順序給色號（模式 30）：hues 放由左到右每塊的色相碼（最多 maxN 個、≤16），回傳放了幾個；total = 總塊數
uint8_t ucReadOrder(int16_t *hues, uint8_t maxN, int16_t &total) {
  int16_t d[16];
  total = mbcRead(ADDR, 0x20);
  if (total == MBC_NONE || !mbcReadMany(ADDR, 0x10, 16, d)) { total = MBC_NONE; return 0; }
  uint8_t n = (total < 16) ? (uint8_t)total : 16;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = d[i];
  return n;
}

// 循線（模式 41~43）：一次讀 14 個值
bool ucReadLine(UcLine &l) {
  int16_t d[14];
  if (!mbcReadMany(ADDR, 0x10, 14, d)) return false;
  l.nearOff = d[0]; l.midOff = d[1]; l.farOff = d[2]; l.angle = d[3];
  l.width = d[4]; l.flags = d[5]; l.junction = d[6]; l.junctionDist = d[7];
  l.leftEdge = d[8]; l.rightEdge = d[9]; l.farWidth = d[10];
  l.marker = d[11]; l.markerCount = d[12]; l.lost = d[13];
  return true;
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
uint8_t ucColorCode(int16_t h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_HELPERS_END ====
```

**你會看到：**（左紅、中綠、右白）

```text
點1: 紅 (H3 S78 V65)   點2: 綠 (H84 S60 V52)   點3: 白／灰 (H246 S5 V90)   
```

**試試看：** 改 `ucSetPoint(…)` 的 X、Y 把取樣點移到你要的位置（X 0～159、Y 0～119）。顏色名稱的分界在小幫手 `ucColorCode` 裡（例：色相碼 56～99 算綠），場地燈光偏色時可以照你量到的數字改判斷。

---

<h2 id="ex4">4️⃣ 顏色順序（WRO 任務賽）</h2>

**需要的相機模式：** 相機模式 3 照順序給色號（沒有子選項；程式開頭會用 E0 自動切）

**做什麼：** WRO 常見任務是「一排顏色方塊，要知道由左到右是什麼顏色」。相機把畫面裡**教過的顏色**的色塊由左到右排好，給每塊的色相碼和總塊數；這支每 0.5 秒印一次。色號用的是「教色時那個顏色」的色相碼，所以同一種顏色每次都會得到同一個數字，拿來比對很穩。

**步驟：**
1. 先把場地上會出現的顏色都教過（目標 1、2、3…）。
2. 上傳程式、打開序列埠監控窗（115200）。
3. 在鏡頭前排一排顏色方塊。

<a href="/examples/unicam/arduino/unicam_04_color_order/unicam_04_color_order.ino" download data-ignore="true">📄 下載 unicam_04_color_order.ino</a>

<!-- ino: examples/unicam/arduino/unicam_04_color_order/unicam_04_color_order.ino -->
```cpp
/*
 * 神攝手通用版 v1 × Arduino　範例 4：顏色順序（WRO 任務賽）
 * ------------------------------------------------------------
 * 需要把相機選單切到：相機模式 3 照順序給色號（沒有子選項；程式開頭也會用 E0 自動切）
 *
 * 這個範例會做什麼？
 *   WRO 常見任務：一排顏色方塊，要知道「由左到右是什麼顏色」。
 *   相機把畫面裡教過的顏色色塊由左到右排好，給每塊的色相碼和總塊數；
 *   這支每 0.5 秒印一次「共 N 塊：第 1 塊 紅、第 2 塊 綠…」。
 *
 * 準備：先在相機上把會出現的顏色都教過（目標 1、2、3…）。
 *   色號用的是「教色時那個顏色」的色相碼，所以同一種顏色每次都會得到同一個數字。
 *
 * 接線（I2C 腳位是暫定，等上機確認）：
 *   神攝手 5V  → 外接 5V（2 A 以上的行動電源或變壓器；不要從 Arduino 的 5V 腳拉大電流）
 *   神攝手 GND → Arduino GND（外接電源的 GND 也要接在一起）
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 要燒「I2C 版」韌體。IO1/IO3 跟樂高串口版共用，韌體二選一。
 *
 * 怎麼看結果：打開「序列埠監控窗」（115200），在鏡頭前排一排顏色方塊。
 */

#include <Wire.h>

// ==== MBC_UNICAM_TOP_BEGIN ====
const uint8_t ADDR = 0x4C;               // 神攝手的 I2C 位址（門牌號碼）
const int16_t MBC_NONE = -32768;         // 「讀不到／沒有這個值」的代號
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);   // 小幫手的宣告（實作在最下面）
// 一個色塊：x、y 像素（0 = 沒找到）、w、h 像素、count 塊數、area 面積 %；這個模式沒給的值是 -1
struct UcBlob { int16_t x, y, w, h, count, area; };
// 一個取樣點：h 色相碼 0~255、s 飽和度、v 明度 0~100；沒給的是 -1
struct UcSpot { int16_t h, s, v; };
// 循線 14 個值（近／中／遠偏移 -100~100：線在左 = 負）
struct UcLine { int16_t nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_TOP_END ====

const uint8_t MAX_BLOCKS = 6;            // 👉 可以改：最多印幾塊（1~16）
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {        // 最多等 8 秒讓相機開機
    Serial.println(F("找不到神攝手通用版！請檢查："));
    Serial.println(F(" 1. 相機有接外接 5V、GND 有跟 Arduino 接在一起"));
    Serial.println(F(" 2. IO3 接 SDA、IO1 接 SCL（接反了就對調）"));
    Serial.println(F(" 3. 相機燒的是「I2C 版」韌體"));
    while (true) {}                      // 停在這裡
  }
  ucSetMode(3, 0);                       // E0：切到相機模式 3（照順序給色號沒有子選項，填 0）
  Serial.println(F("準備好了！在鏡頭前排一排顏色方塊。"));
}

void loop() {
  if (millis() - lastPrint < 500) return;
  lastPrint = millis();

  int16_t hues[16];                      // 由左到右每塊的色相碼
  int16_t total;                         // 畫面裡總共幾塊
  uint8_t n = ucReadOrder(hues, MAX_BLOCKS, total);
  if (total == MBC_NONE) { Serial.println(F("讀不到神攝手")); return; }

  Serial.print(F("共 ")); Serial.print(total); Serial.print(F(" 塊："));
  if (total == 0) { Serial.println(F("（沒看到教過的顏色）")); return; }
  for (uint8_t i = 0; i < n; i++) {
    Serial.print(i + 1); Serial.print(' ');           // 第幾塊（左起）
    printColorName(ucColorCode(hues[i]));
    Serial.print(F("(")); Serial.print(hues[i]); Serial.print(F(")  "));
  }
  Serial.println();
}

// 印出顏色名稱（代號來自 ucColorCode）
void printColorName(uint8_t code) {
  switch (code) {
    case 1: Serial.print(F("紅")); break;
    case 2: Serial.print(F("橙")); break;
    case 3: Serial.print(F("黃")); break;
    case 4: Serial.print(F("綠")); break;
    case 5: Serial.print(F("青")); break;
    case 6: Serial.print(F("藍")); break;
    case 7: Serial.print(F("紫")); break;
    case 8: Serial.print(F("白／灰")); break;
    default: Serial.print(F("黑")); break;
  }
}

// ==== MBC_UNICAM_HELPERS_BEGIN ====
// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）＋「神攝手通用版小幫手」。所有範例都一樣，不用改。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）。UNO/Nano：SDA=A4、SCL=A5；ESP32：GPIO21／22
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時不會卡住
#endif
}

// 由編號 id 起連續讀 k 個值（1~16）放進 out；全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte）
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 相機（ESP32）一定要等 2 ms 再讀
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();
    uint16_t hi = Wire.read();
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值；讀不到回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]；成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（F3 的 bit0 = 1），最多等 ms 毫秒
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}

// ---------- 神攝手通用版 ----------

// 切相機模式（E0）：camMode 1~4、sub 子選項（照順序給色號填 0）。會存進相機
bool ucSetMode(uint8_t camMode, uint8_t sub) {
  uint8_t p[2] = { camMode, sub };
  bool ok = mbcWrite(ADDR, 0xE0, p, 2);
  delay(50);                             // 給相機一點時間換模式
  return ok;
}

// 改取樣點位置（E1）：n 第幾點 1~20、x 0~159、y 0~119。會存進相機
bool ucSetPoint(uint8_t n, uint8_t x, uint8_t y) {
  uint8_t p[3] = { n, x, y };
  return mbcWrite(ADDR, 0xE1, p, 3);
}

// 相機目前的模式編號（例 13 = 顏色追蹤 ③）；讀不到回傳 MBC_NONE
int16_t ucModeNumber() {
  return mbcRead(ADDR, 0x01);
}

// 顏色追蹤（模式 11~14）每色佔幾個值 / values per colour
uint8_t ucStride(uint8_t mode) {
  if (mode == 11) return 2;              // X Y
  if (mode == 12) return 3;              // X Y 數量
  if (mode == 13) return 4;              // X Y 面積 數量
  if (mode == 14) return 6;              // X Y 寬 高 數量 面積
  return 0;
}

// 讀色 color（1~16）的色塊；mode = 你切的模式編號 11~14。讀到回傳 true
bool ucReadBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  uint8_t k = ucStride(mode);
  if (k == 0 || color < 1 || color > 16) return false;
  int16_t d[6];
  if (!mbcReadMany(ADDR, 0x10 + (color - 1) * k, k, d)) return false;
  b.x = d[0]; b.y = d[1];
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 12) b.count = d[2];
  if (mode == 13) { b.area = d[2]; b.count = d[3]; }
  if (mode == 14) { b.w = d[2]; b.h = d[3]; b.count = d[4]; b.area = d[5]; }
  return true;
}

// 16 個目標色裡找到幾色（模式 11~14）
int16_t ucColorsFound(uint8_t mode) {
  uint8_t k = ucStride(mode);
  if (k == 0) return MBC_NONE;
  return mbcRead(ADDR, 0x10 + 16 * k);
}

// 讀取樣點 point（1~16）；mode = 21、22、23
bool ucReadSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (mode < 21 || mode > 23 || point < 1 || point > 16) return false;
  uint8_t k = mode - 20;                 // 21 → 1 個值、22 → 2、23 → 3
  int16_t d[3];
  if (!mbcReadMany(ADDR, 0x10 + (point - 1) * k, k, d)) return false;
  p.h = d[0];
  p.s = (k >= 2) ? d[1] : -1;
  p.v = (k >= 3) ? d[2] : -1;
  return true;
}

// 照順序給色號（模式 30）：hues 放由左到右每塊的色相碼（最多 maxN 個、≤16），回傳放了幾個；total = 總塊數
uint8_t ucReadOrder(int16_t *hues, uint8_t maxN, int16_t &total) {
  int16_t d[16];
  total = mbcRead(ADDR, 0x20);
  if (total == MBC_NONE || !mbcReadMany(ADDR, 0x10, 16, d)) { total = MBC_NONE; return 0; }
  uint8_t n = (total < 16) ? (uint8_t)total : 16;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = d[i];
  return n;
}

// 循線（模式 41~43）：一次讀 14 個值
bool ucReadLine(UcLine &l) {
  int16_t d[14];
  if (!mbcReadMany(ADDR, 0x10, 14, d)) return false;
  l.nearOff = d[0]; l.midOff = d[1]; l.farOff = d[2]; l.angle = d[3];
  l.width = d[4]; l.flags = d[5]; l.junction = d[6]; l.junctionDist = d[7];
  l.leftEdge = d[8]; l.rightEdge = d[9]; l.farWidth = d[10];
  l.marker = d[11]; l.markerCount = d[12]; l.lost = d[13];
  return true;
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
uint8_t ucColorCode(int16_t h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_HELPERS_END ====
```

**你會看到：**（左到右：紅、黃、綠、藍）

```text
共 4 塊：1 紅(2)  2 黃(41)  3 綠(83)  4 藍(158)  
```

**試試看：** WRO 要的是「顏色號碼」時，在迴圈裡用 `ucColorCode(hues[i])` 的結果換成大會規定的號碼。I2C 一次最多給 16 塊；`MAX_BLOCKS` 改大就印更多。

---

<h2 id="ex5">5️⃣ 看前方的循線小車（進階）</h2>

**需要的相機模式：** 相機模式 4 循線・子選項 ① 黑線（程式開頭會用 E0 自動切；白線改 `ucSetMode(4, 2)`）

**做什麼：** 相機往前下方看，一次給 14 個循線值。誤差＝近處偏移 × 0.7 ＋ 遠處偏移 × 0.3（看得到前面的彎，比較早轉），用 PD 控制修方向。看到十字（1）或 T 字（2）路口、而且路口距離 ≤ 30（很近了）就停 3 秒再走。三條掃描列都沒看到線（`flags = 0`）時，照「最後離線方向」原地轉回去找線；讀不到相機就停車。

**接線（加在上面的表格之外）：**

| L298N | UNO / Nano / R4 | ESP32 |
| :---: | :---: | :---: |
| ENA（左輪） | 5 | 25 |
| IN1／IN2 | 7／8 | 26／27 |
| ENB（右輪） | 6 | 14 |
| IN3／IN4 | 9／10 | 32／33 |
| GND | GND | GND |

**步驟：**
1. 相機裝在車頭、往前下方看（畫面下方是車頭前面）。
2. 先把車子架高（輪子懸空）上傳，確認兩個輪子都往前轉；轉反了就把 `LEFT_REVERSE` 或 `RIGHT_REVERSE` 改成 `true`。
3. 放到黑線上，3 秒後出發。

<a href="/examples/unicam/arduino/unicam_05_line_follow/unicam_05_line_follow.ino" download data-ignore="true">📄 下載 unicam_05_line_follow.ino</a>

<!-- ino: examples/unicam/arduino/unicam_05_line_follow/unicam_05_line_follow.ino -->
```cpp
/*
 * 神攝手通用版 v1 × Arduino　範例 5（進階）：看前方的循線小車
 * ------------------------------------------------------------
 * 需要把相機選單切到：相機模式 4 循線 ・子選項 ① 黑線（程式開頭也會用 E0 自動切）
 *
 * 這個範例會做什麼？
 *   相機往前下方看，給「近處偏移」和「遠處偏移」（-100 ~ +100，線在左 = 負）。
 *   誤差 = 近處 × 0.7 ＋ 遠處 × 0.3（看得到前面的彎，比較早轉），用 PD 控制修方向。
 *   看到十字或 T 字路口、而且路口已經很近，就停車 3 秒再走。
 *   三條掃描列都沒看到線時，照「最後離線方向」原地轉回去找線；讀不到相機就停車。
 *
 * 你需要：兩輪小車、L298N 馬達驅動板、電池。相機裝在車頭、往前下方看，畫面下方是車頭前面。
 *
 * 接線（I2C 腳位是暫定，等上機確認）：
 *   神攝手 5V  → 外接 5V（2 A 以上的行動電源或變壓器；不要從 Arduino 的 5V 腳拉大電流）
 *   神攝手 GND → Arduino GND（外接電源的 GND 也要接在一起）
 *   神攝手 IO3（SDA）→ A4   神攝手 IO1（SCL）→ A5      （UNO / Nano）
 *   UNO R4 接 SDA / SCL 腳；ESP32 接 GPIO 21 (SDA) / GPIO 22 (SCL)
 *   ※ 要燒「I2C 版」韌體。IO1/IO3 跟樂高串口版共用，韌體二選一。
 *   L298N ENA → 5     IN1 → 7     IN2 → 8      （左輪）
 *   L298N ENB → 6     IN3 → 9     IN4 → 10     （右輪）
 *   （ESP32 改接：ENA → 25、IN1 → 26、IN2 → 27、ENB → 14、IN3 → 32、IN4 → 33）
 *   L298N 的 GND 要跟 Arduino 的 GND 接在一起！
 *
 * 第一次跑之前：把車子架高（輪子懸空）上傳，確認兩個輪子都「往前」轉；
 *   轉反了就把 LEFT_REVERSE 或 RIGHT_REVERSE 改成 true。
 *   ※ v1 還沒上機：KP、KD、路口停車門檻都是建議值，要在你的場地調。
 */

#include <Wire.h>

// ==== MBC_UNICAM_TOP_BEGIN ====
const uint8_t ADDR = 0x4C;               // 神攝手的 I2C 位址（門牌號碼）
const int16_t MBC_NONE = -32768;         // 「讀不到／沒有這個值」的代號
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0);   // 小幫手的宣告（實作在最下面）
// 一個色塊：x、y 像素（0 = 沒找到）、w、h 像素、count 塊數、area 面積 %；這個模式沒給的值是 -1
struct UcBlob { int16_t x, y, w, h, count, area; };
// 一個取樣點：h 色相碼 0~255、s 飽和度、v 明度 0~100；沒給的是 -1
struct UcSpot { int16_t h, s, v; };
// 循線 14 個值（近／中／遠偏移 -100~100：線在左 = 負）
struct UcLine { int16_t nearOff, midOff, farOff, angle, width, flags, junction, junctionDist,
                leftEdge, rightEdge, farWidth, marker, markerCount, lost; };
// ==== MBC_UNICAM_TOP_END ====

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
const int   BASE_SPEED    = 110;         // 直走的速度：0 ~ 255，先慢一點比較好調
const float KP            = 1.2;         // 修正力道：修不回來就加大，左右搖擺就減小
const float KD            = 3.0;         // 煞住擺動：搖擺越來越大就加大
const int   SEARCH_SPEED  = 100;         // 找線時原地轉的速度
const int   JUNCTION_NEAR = 30;          // 路口距離 ≤ 這個數就停（0 = 就在車頭前，100 = 畫面最遠）
const bool  LEFT_REVERSE  = false;       // 左輪轉反了改成 true
const bool  RIGHT_REVERSE = false;       // 右輪轉反了改成 true

float lastError = 0;                     // 上一次的誤差（D 控制用）

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  drive(0, 0);                           // 先停好

  mbcBegin();
  delay(500);
  if (!mbcWaitReady(ADDR, 8000)) {        // 最多等 8 秒讓相機開機
    Serial.println(F("找不到神攝手通用版！請檢查："));
    Serial.println(F(" 1. 相機有接外接 5V、GND 有跟 Arduino 接在一起"));
    Serial.println(F(" 2. IO3 接 SDA、IO1 接 SCL（接反了就對調）"));
    Serial.println(F(" 3. 相機燒的是「I2C 版」韌體"));
    while (true) {}                      // 停在這裡
  }
  ucSetMode(4, 1);                       // E0：相機模式 4 循線、子選項 1 黑線（白線改 ucSetMode(4, 2)）
  Serial.println(F("3 秒後出發！"));
  delay(3000);
}

void loop() {
  UcLine l;
  if (!ucReadLine(l)) {                  // 讀不到相機 → 為了安全，先停車
    drive(0, 0);
    delay(50);
    return;
  }

  if (l.flags == 0) {
    // 近、中、遠三條掃描列都沒看到線：往最後離線的那一邊原地轉
    if (l.lost == 1) drive(-SEARCH_SPEED, SEARCH_SPEED);       // 線最後在左邊 → 原地左轉
    else             drive(SEARCH_SPEED, -SEARCH_SPEED);       // 線最後在右邊 → 原地右轉
    delay(20);
    return;
  }

  // 十字（1）或 T 字（2）路口，而且已經很近 → 停 3 秒
  if ((l.junction == 1 || l.junction == 2) && l.junctionDist <= JUNCTION_NEAR) {
    drive(0, 0);
    Serial.println(F("路口！停 3 秒"));
    delay(3000);
    drive(BASE_SPEED, BASE_SPEED);       // 直直開過路口
    delay(400);
    return;
  }

  // PD 控制：誤差 = 近處 70% ＋ 遠處 30%
  float error = 0.7 * l.nearOff + 0.3 * l.farOff;
  float turn = KP * error + KD * (error - lastError);
  lastError = error;
  // 線在右邊（正）→ turn 是正的 → 左輪快、右輪慢 → 往右轉
  drive(BASE_SPEED + (int)turn, BASE_SPEED - (int)turn);
  delay(20);
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

// ==== MBC_UNICAM_HELPERS_BEGIN ====
// ============================================================
// 以下是「通訊小幫手」（MBC I2C 協議 v2）＋「神攝手通用版小幫手」。所有範例都一樣，不用改。
//   讀法：寫 [編號] → 等 2 ms → 讀 2×k 個 byte = 從該編號起連續 k 個 int16（小端）
// ============================================================

// 開始 I2C 通訊（setup 裡呼叫一次）。UNO/Nano：SDA=A4、SCL=A5；ESP32：GPIO21／22
void mbcBegin() {
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);      // 線沒接好時不會卡住
#endif
}

// 由編號 id 起連續讀 k 個值（1~16）放進 out；全部讀到回傳 true
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1;
  if (k > 16) k = 16;                    // 一次最多 16 個值（32 byte）
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                              // 相機（ESP32）一定要等 2 ms 再讀
  uint8_t n = (uint8_t)(2 * k);
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read();
    uint16_t hi = Wire.read();
    out[i] = (int16_t)(uint16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀一個值；讀不到回傳 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 送命令：寫 [id][參數…]；成功回傳 true
bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

// 等感應器準備好（F3 的 bit0 = 1），最多等 ms 毫秒
bool mbcWaitReady(uint8_t addr, uint16_t ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    int16_t f = mbcRead(addr, 0xF3);
    if (f != MBC_NONE && (f & 1)) return true;
    delay(50);
  }
  return false;
}

// ---------- 神攝手通用版 ----------

// 切相機模式（E0）：camMode 1~4、sub 子選項（照順序給色號填 0）。會存進相機
bool ucSetMode(uint8_t camMode, uint8_t sub) {
  uint8_t p[2] = { camMode, sub };
  bool ok = mbcWrite(ADDR, 0xE0, p, 2);
  delay(50);                             // 給相機一點時間換模式
  return ok;
}

// 改取樣點位置（E1）：n 第幾點 1~20、x 0~159、y 0~119。會存進相機
bool ucSetPoint(uint8_t n, uint8_t x, uint8_t y) {
  uint8_t p[3] = { n, x, y };
  return mbcWrite(ADDR, 0xE1, p, 3);
}

// 相機目前的模式編號（例 13 = 顏色追蹤 ③）；讀不到回傳 MBC_NONE
int16_t ucModeNumber() {
  return mbcRead(ADDR, 0x01);
}

// 顏色追蹤（模式 11~14）每色佔幾個值 / values per colour
uint8_t ucStride(uint8_t mode) {
  if (mode == 11) return 2;              // X Y
  if (mode == 12) return 3;              // X Y 數量
  if (mode == 13) return 4;              // X Y 面積 數量
  if (mode == 14) return 6;              // X Y 寬 高 數量 面積
  return 0;
}

// 讀色 color（1~16）的色塊；mode = 你切的模式編號 11~14。讀到回傳 true
bool ucReadBlob(uint8_t mode, uint8_t color, UcBlob &b) {
  uint8_t k = ucStride(mode);
  if (k == 0 || color < 1 || color > 16) return false;
  int16_t d[6];
  if (!mbcReadMany(ADDR, 0x10 + (color - 1) * k, k, d)) return false;
  b.x = d[0]; b.y = d[1];
  b.w = -1; b.h = -1; b.count = -1; b.area = -1;
  if (mode == 12) b.count = d[2];
  if (mode == 13) { b.area = d[2]; b.count = d[3]; }
  if (mode == 14) { b.w = d[2]; b.h = d[3]; b.count = d[4]; b.area = d[5]; }
  return true;
}

// 16 個目標色裡找到幾色（模式 11~14）
int16_t ucColorsFound(uint8_t mode) {
  uint8_t k = ucStride(mode);
  if (k == 0) return MBC_NONE;
  return mbcRead(ADDR, 0x10 + 16 * k);
}

// 讀取樣點 point（1~16）；mode = 21、22、23
bool ucReadSpot(uint8_t mode, uint8_t point, UcSpot &p) {
  if (mode < 21 || mode > 23 || point < 1 || point > 16) return false;
  uint8_t k = mode - 20;                 // 21 → 1 個值、22 → 2、23 → 3
  int16_t d[3];
  if (!mbcReadMany(ADDR, 0x10 + (point - 1) * k, k, d)) return false;
  p.h = d[0];
  p.s = (k >= 2) ? d[1] : -1;
  p.v = (k >= 3) ? d[2] : -1;
  return true;
}

// 照順序給色號（模式 30）：hues 放由左到右每塊的色相碼（最多 maxN 個、≤16），回傳放了幾個；total = 總塊數
uint8_t ucReadOrder(int16_t *hues, uint8_t maxN, int16_t &total) {
  int16_t d[16];
  total = mbcRead(ADDR, 0x20);
  if (total == MBC_NONE || !mbcReadMany(ADDR, 0x10, 16, d)) { total = MBC_NONE; return 0; }
  uint8_t n = (total < 16) ? (uint8_t)total : 16;
  if (n > maxN) n = maxN;
  for (uint8_t i = 0; i < n; i++) hues[i] = d[i];
  return n;
}

// 循線（模式 41~43）：一次讀 14 個值
bool ucReadLine(UcLine &l) {
  int16_t d[14];
  if (!mbcReadMany(ADDR, 0x10, 14, d)) return false;
  l.nearOff = d[0]; l.midOff = d[1]; l.farOff = d[2]; l.angle = d[3];
  l.width = d[4]; l.flags = d[5]; l.junction = d[6]; l.junctionDist = d[7];
  l.leftEdge = d[8]; l.rightEdge = d[9]; l.farWidth = d[10];
  l.marker = d[11]; l.markerCount = d[12]; l.lost = d[13];
  return true;
}

// 色相碼 → 顏色代號：1 紅 2 橙 3 黃 4 綠 5 青 6 藍 7 紫 8 白／灰 9 黑
uint8_t ucColorCode(int16_t h) {
  if (h > 248) return 9;
  if (h > 240) return 8;
  if (h < 12 || h >= 220) return 1;
  if (h < 28) return 2;
  if (h < 56) return 3;
  if (h < 100) return 4;
  if (h < 140) return 5;
  if (h < 180) return 6;
  return 7;
}
// ==== MBC_UNICAM_HELPERS_END ====
```

**你會看到：** 小車沿著黑線走，轉彎前會提早修方向；到十字或 T 字路口停 3 秒，序列埠印 `路口！停 3 秒`，再直直開過路口。

**試試看：**
- 修不回來就把 `KP` 加大；左右搖擺越來越大就把 `KD` 加大、`KP` 減小。
- 還沒到路口就停 → `JUNCTION_NEAR` 改小；衝過頭才停 → 改大。
- v1 還沒上機，這些數字都是建議值，請在你的場地調。

---

## ❓ 常見問題

**Q：一直出現「找不到神攝手通用版」？**
1. SDA／SCL 是暫定接法，先把 IO3、IO1 兩條線**對調**試試看。
2. 外接 5V 有沒有開？相機螢幕有沒有亮？GND 有沒有跟 Arduino 接在一起？
3. 相機燒的是 **I2C 版韌體**嗎？EV3／SPIKE／MATRIX 版走串口，接 Arduino 讀不到。
4. 相機開機要幾秒，範例會等 8 秒；還是不行就按 Arduino 的 RESET 鍵重來。

**Q：範例 1 說是學校版或 WRO 版？**
那是另一款韌體（讀法不一樣），請改用 [神攝手學校版／WRO 版的 Arduino 範例](/sensors/esp32cam/arduino-examples.md)，或請我們幫你更新成通用版。

**Q：色塊一直「沒看到」？**
還沒在相機上教色，或教的顏色跟眼前的差太多（燈光不同）。在相機上停在那個目標重新教一次；最小面積（Size）設太大也會看不到小色塊。

**Q：讀到 `-32768` 是什麼意思？**
「沒有這個值」的代號（範例裡叫 `MBC_NONE`）：讀了這個模式沒有的編號，或讀不到相機（線鬆了）。

**Q：小幫手裡的 `delay(2)` 可以拿掉嗎？**
不行。相機是 ESP32 做的從機，不能把 I2C 時脈拉住等資料，主機寫完編號後要等 2 毫秒它才準備好。

**Q：為什麼一次只能讀 16 個值？**
Arduino UNO 的 Wire 緩衝區只有 32 byte（16 個 int16）。小幫手已經幫你分好；自己用 `mbcReadMany` 時 k 不要超過 16。

**Q：想知道每一格放什麼？**
看上面的 [值編號總覽](#cmds)，或 [神攝手通用版首頁](/sensors/unicam/index.md) 的完整規格。
