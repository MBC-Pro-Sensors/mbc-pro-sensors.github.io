<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/line16.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/line16/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line16/arduino/ide.md">循行者 16 路 › Arduino › Arduino IDE</a></p>

# 循行者 16 路 × Arduino I2C（通用 I2C 版）

[🔙 回到循行者 16 路產品頁](/sensors/line16/index.md)

循行者 16 路通用 I2C 版用 **MBC I2C 協議 v2**：寫一個編號、讀 2 byte 就是一個值（int16 小端），連號的值可以一次讀完。本頁是這款產品的值表、命令表與接線；共用規則（F0~F5 版本與狀態、F8 換位址、EF 重開機）在 [通用 I2C 協議](/i2c-protocol.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/line16/spike-hub.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/line16/ev3-hub.md">EV3</a><a class="chip chip-arduino current" href="/sensors/line16/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/line16/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/line16/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/line16/microbit.md">micro:bit</a></p>

> [!TIP]
> **不太會寫程式？** 直接看 [Arduino 範例](/sensors/line16/arduino-examples.md)，每支都能複製上傳。每個值的六平台讀法在 [指令參考](/sensors/line16/reference.md)。

## 基本資訊

| 項目 | 內容 |
| :--- | :--- |
| I2C 位址 | `0x16`（可用 F8 換） |
| F0 裝置 ID | 低 byte `0x16`、高 byte `16` |
| 型號 | `LINE16` |
| 協議版本 F1 | `2` |
| 速度 | 100 kHz（建議）／400 kHz；寫完編號等 2 ms 再讀 |
| 讀法 | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| 快照 | 每 10 ms 更新，一次讀多個值保證同一瞬間 |

## 接線

<figure class="ref-fig"><img src="/images/figures/line16/wiring-uno.svg" alt="循行者 16 路 × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/sensors/line16/reference.md#v01) | 線位置 `linePos` | -16~+16 | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |
| [`02`](/sensors/line16/reference.md#v02) | 細位置 `linePosFine` | -100~+100 | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |
| [`03`](/sensors/line16/reference.md#v03) | 線寬 `lineWidth` | 0~16 路 | 壓到線的路數 0~16，0 = 找不到線 |
| [`04`](/sensors/line16/reference.md#v04) | 線段數 `groupCount` | 0~8 | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |
| [`05`](/sensors/line16/reference.md#v05) | 壓線位元圖 `bitmap` | 0~65535 bit | bit0 = 第 1 路（最左）… bit15 = 第 16 路，1 = 那一路在線上 |
| [`06`](/sensors/line16/reference.md#v06) | 最後離線方向 `lastDir` | -16~+16 | 丟線前線最後在哪一側：−16 ~ +16（負 = 左），找回線後維持最後一次 |
| [`07`](/sensors/line16/reference.md#v07) | 二值化門檻 `threshold` | 1~99 | 判斷「在線上」的光值門檻 1~99，預設 50；用 E4 改 |
| [`08`](/sensors/line16/reference.md#v08) | 校準完成次數 `calibCount` | 0~255 次 | 開機後完成校準的次數（按鈕校準也算） |
| [`10~1F`](/sensors/line16/reference.md#v10) | 第 1~16 路光值 `light` | 0~100 | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

## 寫入命令

| 編號 | 命令 | 參數 | 說明 |
| :--- | :--- | :--- | :--- |
| [`E0`](/sensors/line16/reference.md#ce0) | 切黑線 `blackLine` | — | 追黑線（預設） |
| [`E1`](/sensors/line16/reference.md#ce1) | 切白線 `whiteLine` | — | 追白線（F3 b1 = 1） |
| [`E2`](/sensors/line16/reference.md#ce2) | 黑色校準 `calibBlack` | — | 把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM（執行中 F3 bit2 = 1） |
| [`E3`](/sensors/line16/reference.md#ce3) | 白色校準 `calibWhite` | — | 把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM（執行中 F3 bit2 = 1） |
| [`E4`](/sensors/line16/reference.md#ce4) | 設定門檻 `setThreshold` | 門檻 1~99 | 二值化門檻 1~99（RAM，重開機回預設 50） |
| [`E7`](/sensors/line16/reference.md#ce7) | 5 秒動態校準 `calibAuto` | — | 5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1（執行中 F3 bit2 = 1） |

## 狀態旗標 F3

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 白線模式 (`whiteMode`) | 1 = 正在追白線 |
| b2 | 校準中 (`calibrating`) | E2／E3／E7 執行中；此時拒收新的校準命令 |
| b3 | 丟線 (`lost`) | 線段數 = 0 |
| b4 | 分岔 (`fork`) | 線段數 ≥ 2 |

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
const uint8_t ADDR = 0x16;

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
  Serial.print("linePos: "); Serial.println(mbcRead(ADDR, 0x01));   // 線位置
  Serial.print("linePosFine: "); Serial.println(mbcRead(ADDR, 0x02));   // 細位置
  Serial.print("lineWidth: "); Serial.println(mbcRead(ADDR, 0x03));   // 線寬
  Serial.print("groupCount: "); Serial.println(mbcRead(ADDR, 0x04));   // 線段數
  Serial.print("bitmap: "); Serial.println(mbcRead(ADDR, 0x05));   // 壓線位元圖
  Serial.print("lastDir: "); Serial.println(mbcRead(ADDR, 0x06));   // 最後離線方向
  Serial.print("threshold: "); Serial.println(mbcRead(ADDR, 0x07));   // 二值化門檻
  Serial.print("calibCount: "); Serial.println(mbcRead(ADDR, 0x08));   // 校準完成次數
  { int16_t a[16]; mbcReadMany(ADDR, 0x10, 16, a); Serial.print("light: "); for (uint8_t i = 0; i < 16; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// 下面貼上「單檔小幫手函式」
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/line16.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- B-11：細位置 02 與線位置 01 同號（負 = 左）；韌體 F16:1290 註解寫反，程式碼同號（IR:11 為準）。
- B-12：E7 五秒動態校準補回（v1 的 0x20=0x01），定案表沒列。
- EV3：模式 0~2 鎖住；模式 3 原本不送 → 16 bit 位元圖；模式 4 原本不送 → 同模式 2；模式 5 ID 格式 B（共用函式）。
- SPIKE：0/5/7/9 鎖住，1~4、6 維持不動；教育版模式 0 的 16 路光值是 2-bit（玩具版 1-bit），跟 8 路的 4-bit 不同；byte 8~15 補 0（#65 已改，使用者保留）；模式 9 排法照盤點（四路 4-bit 一個 int16，v6 = 位元圖）；模式 8 v[0] 改 ID|hw[0]<<8。
- 線段數 04 範圍：定案沒寫，16 路最多 8 段（交錯壓線），採 0~8。
- mBlock 舊版：MBC_MBLOCK_LEGACY=1 時逐 byte 同 2026-10-21 統一後韌體（0x01 位置、0x02 線寬、n<<4|03 反向兩路壓一 byte、0x07 細位置 0~200），0x0B 回 1（B-1）。
- Flash：C031 硬體鎖 28 KB，I2C 版餘 848 B；不夠時照 B-17 順序瘦身，JSON 不受影響。
- 接線表的腳位以各主機板常見預設為準（ESP32 可用 `Wire.begin(sda, scl)` 改）。
