<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/cam_school.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/esp32cam/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/esp32cam/arduino/ide.md">神攝手 ESP32CAM › Arduino › Arduino IDE</a></p>

# 神攝手 學校版 × Arduino I2C（通用 I2C 版）

[🔙 回到神攝手 學校版產品頁](/sensors/esp32cam/index.md)

神攝手 學校版通用 I2C 版用 **MBC I2C 協議 v2**：寫一個編號、讀 2 byte 就是一個值（int16 小端），連號的值可以一次讀完。本頁是這款產品的值表、命令表與接線；共用規則（F0~F5 版本與狀態、F8 換位址、EF 重開機）在 [通用 I2C 協議](/i2c-protocol.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/esp32cam/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/esp32cam/index.md">EV3</a><a class="chip chip-arduino current" href="/sensors/esp32cam/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/esp32cam/mblock.md">mBlock</a><span class="chip chip-matrix off">MATRIX</span><a class="chip chip-microbit" href="/sensors/esp32cam/microbit.md">micro:bit</a></p>

> [!TIP]
> **不太會寫程式？** 直接看 [Arduino 範例](/sensors/esp32cam/arduino-examples.md)，每支都能複製上傳。每個值的六平台讀法在 [指令參考](/sensors/esp32cam/reference.md)。

## 基本資訊

| 項目 | 內容 |
| :--- | :--- |
| I2C 位址 | `0x4C`（可用 F8 換） |
| F0 裝置 ID | 低 byte `0x4C`、高 byte `1` |
| 型號 | `CAMSCH` |
| 協議版本 F1 | `2` |
| 速度 | 100 kHz（建議）／400 kHz；寫完編號等 2 ms 再讀 |
| 讀法 | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| 快照 | 每 10 ms 更新，一次讀多個值保證同一瞬間 |

## 接線

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-uno.svg" alt="神攝手 學校版 × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/sensors/esp32cam/reference.md#v01) | 色相 `hue` | -1~100 | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |
| [`02`](/sensors/esp32cam/reference.md#v02) | 飽和度 `saturation` | 0~100 | S1 取樣點飽和度 0~100 |
| [`03`](/sensors/esp32cam/reference.md#v03) | 明度 `value` | 0~100 | S1 取樣點明度 0~100 |
| [`04`](/sensors/esp32cam/reference.md#v04) | 動態 `motion` | 0~100 | 全畫面動態量 0~100（畫面變化越多越大） |
| [`05`](/sensors/esp32cam/reference.md#v05) | 色塊 X `blobX` | 0~100 | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |
| [`06`](/sensors/esp32cam/reference.md#v06) | 色塊 Y `blobY` | 0~75 | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |
| [`07`](/sensors/esp32cam/reference.md#v07) | 色塊寬 `blobW` | 0~100 | 色塊寬 W 0~100；0 = 沒找到 |
| [`08`](/sensors/esp32cam/reference.md#v08) | 色塊高 `blobH` | 0~75 | 色塊高 H 0~75；沒找到 = 0 |
| [`09`](/sensors/esp32cam/reference.md#v09) | 色塊面積 `blobArea` | 0~7500 | W×H（0~7500）；沒找到 = 0 |
| [`0A`](/sensors/esp32cam/reference.md#v0a) | 有找到色塊 `found` | 0~1 | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |

## 狀態旗標 F3

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1（相機進選單時值不更新，但仍 1） |
| b1 | 有找到色塊 (`found`) | = 值 0A |

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

不需要安裝函式庫。分兩段貼：第 ① 段（2 行）放程式最上面，第 ② 段貼程式最下面。`#include <Wire.h>` 和 `MBC_NONE` 一定要在 `setup()` 用到之前出現，整段放最下面會編不過。

**① 放在程式最上面**（`setup()` 前面）

```cpp
#include <Wire.h>

const int16_t MBC_NONE = -32768;   // 讀不到／空號
```

**② 貼在程式最下面**（`loop()` 後面）

```cpp
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
// 2 個參數、第 1 個是 0 時要寫 (uint8_t)0，例如 mbcWrite(ADDR, 0xF8, (uint8_t)0x00, 0xFF)，不然會跟下面陣列版的 mbcWrite 分不清而編不過
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
// 小幫手第 ① 段
#include <Wire.h>

const int16_t MBC_NONE = -32768;   // 讀不到／空號

const uint8_t ADDR = 0x4C;

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
  Serial.print("hue: "); Serial.println(mbcRead(ADDR, 0x01));   // 色相
  Serial.print("saturation: "); Serial.println(mbcRead(ADDR, 0x02));   // 飽和度
  Serial.print("value: "); Serial.println(mbcRead(ADDR, 0x03));   // 明度
  Serial.print("motion: "); Serial.println(mbcRead(ADDR, 0x04));   // 動態
  Serial.print("blobX: "); Serial.println(mbcRead(ADDR, 0x05));   // 色塊 X
  Serial.print("blobY: "); Serial.println(mbcRead(ADDR, 0x06));   // 色塊 Y
  Serial.print("blobW: "); Serial.println(mbcRead(ADDR, 0x07));   // 色塊寬
  Serial.print("blobH: "); Serial.println(mbcRead(ADDR, 0x08));   // 色塊高
  Serial.print("blobArea: "); Serial.println(mbcRead(ADDR, 0x09));   // 色塊面積
  Serial.print("found: "); Serial.println(mbcRead(ADDR, 0x0A));   // 有找到色塊
  Serial.println();
  delay(200);
}

// 下面貼上「單檔小幫手函式」第 ② 段
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/cam_school.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 1（共識 #6：神攝手 1 = 學校版）；model 字串 CAMSCH 照任務指定（韌體原本沒有型號欄位）。
- D4：I2C 0x4C 用 SDA = IO3、SCL = IO1（沿 PS4 Wire.setPins 慣例），與 EV3／SPIKE 串口模式二選一編譯；ESP32 從機不能時脈延展，onReceive 預載 32 B；頁面標「硬體已確認，上機待測」。mBlock 用同一組腳 Serial.begin(38400, SERIAL_8N1, 3, 1)。
- EV3 六個模式全部鎖住照盤點（模式 3／5 的 −1 已在 #65 修成符號延伸）；EV3 定案最後的 golden 備註把 cam_school 列進「模式 5 改 ID」是筆誤（定案表本身寫「—」；模式 5 仍是 [色相, 飽和]），所以 ev3IdMode = null。
- SPIKE 模式 9 鎖住，只有 byte 8~9（v[4]）從 0 改成面積 W×H；模式 6 改 [X, Y, 面積]；模式 8 = [0x4C | 1<<8, 2, 韌體版, 更新序號]；模式 1~4 維持 0。
- SPIKE 模式 0 的「面積」是 map(像素面積 0~5000 → 0~100)，跟值表 09 的 W×H 不同，鎖住不改（照盤點）。
- mVision：N = 10（idx 0~9），色相 −1 送 255；每 20 ms 一包（B-6）；不做 0x22（hosts.matrix = false）。
- 0A 有找到色塊 = W > 0（盤點：W 0 = 沒找到）；F3 b1 同這個值。
- 值 05~08 的「沒找到 = 0」照盤點 §0；色塊 Y 下 = 0、上 = 75 照盤點（map(bestY,120,0,0,75)）。
- 沒有寫入命令（韌體無法從程式改目標顏色／補光燈）。
- 接線表的腳位以各主機板常見預設為準（ESP32 可用 `Wire.begin(sda, scl)` 改）。
