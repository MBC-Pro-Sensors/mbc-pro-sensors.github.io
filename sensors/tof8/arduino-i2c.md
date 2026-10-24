<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/tof8.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/tof8/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/tof8/arduino/ide.md">測距者 8 路 › Arduino › Arduino IDE</a></p>

# 測距者 8 路 × Arduino I2C（通用 I2C 版）

[🔙 回到測距者 8 路產品頁](/sensors/tof8/index.md)

測距者 8 路通用 I2C 版用 **MBC I2C 協議 v2**：寫一個編號、讀 2 byte 就是一個值（int16 小端），連號的值可以一次讀完。本頁是這款產品的值表、命令表與接線；共用規則（F0~F5 版本與狀態、F8 換位址、EF 重開機）在 [通用 I2C 協議](/i2c-protocol.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/tof8/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/tof8/index.md">EV3</a><a class="chip chip-arduino current" href="/sensors/tof8/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/tof8/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/tof8/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/tof8/microbit.md">micro:bit</a></p>

> [!TIP]
> **不太會寫程式？** 直接看 [Arduino 範例](/sensors/tof8/arduino-examples.md)，每支都能複製上傳。每個值的六平台讀法在 [指令參考](/sensors/tof8/reference.md)。

## 基本資訊

| 項目 | 內容 |
| :--- | :--- |
| I2C 位址 | `0x29`（可用 F8 換） |
| F0 裝置 ID | 低 byte `0x29`、高 byte `8` |
| 型號 | `LASER8` |
| 協議版本 F1 | `2` |
| 速度 | 100 kHz（建議）／400 kHz；寫完編號等 2 ms 再讀 |
| 讀法 | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| 快照 | 每 10 ms 更新，一次讀多個值保證同一瞬間 |

## 接線

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-uno.svg" alt="測距者 8 路 × Arduino UNO" loading="lazy"></figure>

| 感應器 | Arduino UNO / Nano | Arduino R4 | ESP32 |
| :--- | :--- | :--- | :--- |
| SDA | A4 | SDA | GPIO 21 |
| SCL | A5 | SCL | GPIO 22 |
| 5V | 5V | 5V | 5V |
| GND | GND | GND | GND |

5 V 主機可直接接，不需準位轉換；同一組 SDA／SCL 上位址相同的產品不能同時接（用 F8 先換位址）。

## 值表

每個編號讀回一個 int16；點編號看六平台讀法與示意圖。

| 編號 | 值 | 範圍 | 說明 |
| :--- | :--- | :--- | :--- |
| [`01`](/sensors/tof8/reference.md#v01) | 最近距離 cm `nearCm` | 0~120 cm | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |
| [`02`](/sensors/tof8/reference.md#v02) | 最近方向 `nearDir` | -8~+8 | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |
| [`03`](/sensors/tof8/reference.md#v03) | 最空曠距離 cm `farCm` | 0~120 cm | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |
| [`04`](/sensors/tof8/reference.md#v04) | 最空曠方向 `farDir` | -7~+7 | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |
| [`05`](/sensors/tof8/reference.md#v05) | 正前方 cm `frontCm` | 0~120 cm | 第 4、5 顆（兩顆平行朝前）平均 0~120 cm；EV3 模式 3、SPIKE 模式 6 給的是同一個值的 mm；0x22 的 C 是 min(第 4, 第 5 顆) 不是平均（見 10） |
| [`06`](/sensors/tof8/reference.md#v06) | 最近距離 mm `nearMm` | 0~1200 mm | 8 顆 mm 的最小值 0~1200（失效那顆以 1200 參與），1200 = 四周沒東西 |
| [`07`](/sensors/tof8/reference.md#v07) | 初始化位元圖 `initOk` | 0~255 bit | bit0 = 第 1 顆（最左）… bit7 = 第 8 顆，1 = 那顆開機初始化成功（8 顆約 2~4 秒，失敗不重試）；255 = 全部好 |
| [`08`](/sensors/tof8/reference.md#v08) | 故障位元圖 `fault` | 0~255 bit | bit0 = 第 1 顆 … bit7 = 第 8 顆，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |
| [`09`](/sensors/tof8/reference.md#v09) | 正前方距離 mm `frontMm` | 0~1200 mm | 第 4、5 顆（正前方兩顆）平均，0~1200 mm；1200 = 前面沒東西。cm 版是 05 |
| [`10~17`](/sensors/tof8/reference.md#v10) | 第 1~8 顆距離 cm `distCm` | 0~120 cm | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |
| [`18~1F`](/sensors/tof8/reference.md#v18) | 第 1~8 顆距離 mm `distMm` | 0~1200 mm | 第 1~8 顆各自的距離 0~1200 mm，1200 = 那個方向沒東西／那顆失效；編號 18~1F |

## 狀態旗標 F3

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 8 顆都初始化完（開機約 2~4 秒）、快照開始更新後恆 1 |
| b1 | 任一顆失效 (`fault`) | 初始化失敗、I2C 讀失敗或 200 ms 沒更新，任一顆就是 1（= 08 ≠ 0 或 07 ≠ 255） |

## 共用暫存器

| 編號 | 名稱 | 說明 |
| :--- | :--- | :--- |
| [`F0`](/i2c-protocol.md#rf0) | 裝置 ID | 低 byte = 出廠 I2C 位址（換位址後不變），高 byte = hw[0]（通道數／顆數／類別） |
| [`F1`](/i2c-protocol.md#rf1) | 協議版本 | 本版 = 2（SPIKE 模式 8 v[1]、EV3 ID 封包同一個數字） |
| [`F2`](/i2c-protocol.md#rf2) | 韌體版本 | 主版×100 + 次版（例 102 = 1.2） |
| [`F3`](/i2c-protocol.md#rf3) | 狀態旗標 | b0 就緒，其餘各產品定義（見各產品 flags） |
| [`F4`](/i2c-protocol.md#rf4) | 更新序號 | 每 10 ms 快照 +1，0~255 循環（2.56 秒一圈，不能拿來偵測重開機；不動 = 模組忙） |
| [`F5`](/i2c-protocol.md#rf5) | 開機秒數 | 開機後秒數，到 32767 停住（約 9 小時）；數字變小 = 重開機過 |
| [`F8`](/i2c-protocol.md#rf8) | 換 I2C 位址 | [F8][新位址 0x08~0x77][新位址取反]，存 EEPROM，重開機後生效；[F8][0x00][0xFF] = 回出廠位址（共識 B-5） |
| [`EF`](/i2c-protocol.md#ref) | 重新開機 | [EF][0xA5]；mBlock 先回 ACK 再重開 |

## 單檔小幫手函式

每支範例底部都貼這一段，不需要安裝函式庫：

```cpp
#include <Wire.h>

const int16_t MBC_NONE = -32768;   // 讀不到／空號

// 從編號 id 起連續讀 k 個值（k ≤ 16），int16 小端
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                                        // 一律等 2 ms（ESP32 產品不能時脈延展）
  if (Wire.requestFrom(addr, (uint8_t)(k * 2)) != (uint8_t)(k * 2)) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint8_t lo = Wire.read(), hi = Wire.read();
    out[i] = (int16_t)(lo | (hi << 8));
  }
  return true;
}

// 讀 1 個值；讀不到回 MBC_NONE
int16_t mbcRead(uint8_t addr, uint8_t id) {
  int16_t v;
  return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

// 寫入命令（0~3 個參數）
void mbcWrite(uint8_t addr, uint8_t id, const uint8_t *args, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(args[i]);
  Wire.endTransmission();
}
void mbcWrite(uint8_t addr, uint8_t id) { mbcWrite(addr, id, nullptr, 0); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a) { mbcWrite(addr, id, &a, 1); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a, uint8_t b) { uint8_t p[2] = {a, b}; mbcWrite(addr, id, p, 2); }
void mbcWrite(uint8_t addr, uint8_t id, uint8_t a, uint8_t b, uint8_t c) { uint8_t p[3] = {a, b, c}; mbcWrite(addr, id, p, 3); }

// 等模組就緒（F3 bit0 = 1），最多 ms 毫秒
bool mbcWaitReady(uint8_t addr, unsigned long ms) {
  unsigned long t0 = millis();
  while (millis() - t0 < ms) {
    int16_t st = mbcRead(addr, 0xF3);
    if (st != MBC_NONE && (st & 1)) return true;
    delay(20);
  }
  return false;
}
```

## 範例：整張值表逐行印

```cpp
const uint8_t ADDR = 0x29;

void setup() {
  Serial.begin(115200);
  Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
  Wire.setWireTimeout(25000, true);   // AVR：模組沒回應時不會卡死
#endif
  if (!mbcWaitReady(ADDR, 2000)) Serial.println("找不到模組，請檢查接線與位址");
  Serial.print("ID 0x"); Serial.println(mbcRead(ADDR, 0xF0), HEX);   // 低 byte 位址、高 byte hw[0]
  Serial.print("FW ");    Serial.println(mbcRead(ADDR, 0xF2));
}

void loop() {
  Serial.print("nearCm: "); Serial.println(mbcRead(ADDR, 0x01));   // 最近距離 cm
  Serial.print("nearDir: "); Serial.println(mbcRead(ADDR, 0x02));   // 最近方向
  Serial.print("farCm: "); Serial.println(mbcRead(ADDR, 0x03));   // 最空曠距離 cm
  Serial.print("farDir: "); Serial.println(mbcRead(ADDR, 0x04));   // 最空曠方向
  Serial.print("frontCm: "); Serial.println(mbcRead(ADDR, 0x05));   // 正前方 cm
  Serial.print("nearMm: "); Serial.println(mbcRead(ADDR, 0x06));   // 最近距離 mm
  Serial.print("initOk: "); Serial.println(mbcRead(ADDR, 0x07));   // 初始化位元圖
  Serial.print("fault: "); Serial.println(mbcRead(ADDR, 0x08));   // 故障位元圖
  Serial.print("frontMm: "); Serial.println(mbcRead(ADDR, 0x09));   // 正前方距離 mm
  { int16_t a[8]; mbcReadMany(ADDR, 0x10, 8, a); Serial.print("distCm: "); for (uint8_t i = 0; i < 8; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  { int16_t a[8]; mbcReadMany(ADDR, 0x18, 8, a); Serial.print("distMm: "); for (uint8_t i = 0; i < 8; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// 下面貼上「單檔小幫手函式」
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/tof8.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- 正前方 mm（EV3 模式 3、SPIKE 模式 6 v[1]）在定案 §5 沒有獨立 I2C 編號 → 掛在 05 正前方 cm 底下，用 scale:mm 標示，不自己加編號。
- 0x22（使用者 2026-10-21 最終定案）：只用一個位址，R~K 由左到右 7 個 cm：第 1、2、3 顆、min(第 4, 第 5)、第 6、7、8 顆；11、12 不放資料；最近距離／方向由主機自己比較。此排法只用在 0x22，EV3／SPIKE不動。
- SPIKE：0/5/7/9 鎖住，1、2、3、4、8 維持不動；模式 6 改 [最近 mm, 正前方 mm, 最近方向 −8~+8]（原本 = 第 2~7 顆兩兩合併）；模式 8 v[0] 改 ID|hw[0]<<8。
- 最空曠方向 04 範圍 −7~+7（maxDirection 永遠不會是 0／16，盤點 INO:1086-1088）；最近方向 02 ±8 = 沒目標、沿用上次偏向側，開機預設 −8。
- 沒有寫入命令；F8 換位址／EF 重開機用共用的。
- 主對話 2026-10-21：補 09 正前方 mm（定案 §5 只有 05 正前方 cm，但 EV3 模式 3、SPIKE 模式 6 都送 mm，I2C／mBlock 應有同一個值）。
- 接線表的腳位以各主機板常見預設為準（ESP32 可用 `Wire.begin(sda, scl)` 改）。
