<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/ps2.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/ps2/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/ps2/arduino/ide.md">掌控者 PS2 › Arduino › Arduino IDE</a></p>

# 掌控者 PS2 × Arduino I2C（通用 I2C 版）

[🔙 回到掌控者 PS2產品頁](/sensors/ps2/index.md)

掌控者 PS2通用 I2C 版用 **MBC I2C 協議 v2**：寫一個編號、讀 2 byte 就是一個值（int16 小端），連號的值可以一次讀完。本頁是這款產品的值表、命令表與接線；共用規則（F0~F5 版本與狀態、F8 換位址、EF 重開機）在 [通用 I2C 協議](/i2c-protocol.md)。

<p class="chips"><a class="chip chip-spike" href="/sensors/ps2/index.md">SPIKE</a><a class="chip chip-ev3" href="/sensors/ps2/index.md">EV3</a><a class="chip chip-arduino current" href="/sensors/ps2/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/ps2/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/ps2/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/ps2/microbit.md">micro:bit</a></p>

> [!TIP]
> **不太會寫程式？** 直接看 [Arduino 範例](/sensors/ps2/arduino-examples.md)，每支都能複製上傳。每個值的六平台讀法在 [指令參考](/sensors/ps2/reference.md)。

## 基本資訊

| 項目 | 內容 |
| :--- | :--- |
| I2C 位址 | `0x42`（可用 F8 換） |
| F0 裝置 ID | 低 byte `0x42`、高 byte `1` |
| 型號 | `PS2` |
| 協議版本 F1 | `2` |
| 速度 | 100 kHz（建議）／400 kHz；寫完編號等 2 ms 再讀 |
| 讀法 | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| 快照 | 每 10 ms 更新，一次讀多個值保證同一瞬間 |

## 接線

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-uno.svg" alt="掌控者 PS2 × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/sensors/ps2/reference.md#v01) | 十六鍵 `buttons16` | 0~65535 bit | 16 個按鍵合成一個數（PSX 排列，1 = 按下）：bit0 SELECT、1 L3、2 R3、3 START、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；未連線 = 0 |
| [`02`](/sensors/ps2/reference.md#v02) | 方向鍵 `dpad` | 0~15 bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |
| [`03`](/sensors/ps2/reference.md#v03) | 形狀鍵 `shape` | 0~15 bit | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |
| [`04`](/sensors/ps2/reference.md#v04) | 肩鍵 `shoulder` | 0~15 bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |
| [`05`](/sensors/ps2/reference.md#v05) | 功能鍵 `func` | 0~15 bit | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |
| [`06`](/sensors/ps2/reference.md#v06) | 已連線 `connected` | 0~1 | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |
| [`07`](/sensors/ps2/reference.md#v07) | 電量 `battery` | 0 % | 0~100；PS2 手把沒有電量 → 恆 0（PS4 才有） |
| [`08`](/sensors/ps2/reference.md#v08) | L2 類比 `l2Analog` | 0~255 | L2 壓力 0~255（DualShock 2 類比模式才有壓力值；手把沒有壓力值時按下 = 255、放開 = 0）；未連線 = 0 |
| [`09`](/sensors/ps2/reference.md#v09) | R2 類比 `r2Analog` | 0~255 | R2 壓力 0~255（同 08）；未連線 = 0 |
| [`0A`](/sensors/ps2/reference.md#v0a) | 系統鍵 `sysKeys` | 0 bit | b0 PS 鍵、b1 擷取鍵；PS2 手把沒有 → 恆 0（PS4 才有） |
| [`0B`](/sensors/ps2/reference.md#v0b) | 手把類型 `padType` | 0~2 | 0 沒有手把、1 PS2 數位模式、2 PS2 類比模式（3 PS4、4 PS5 只有 PS4 接收器會出現） |
| [`10~13`](/sensors/ps2/reference.md#v10) | 搖桿 LX／LY／RX／RY `stick` | -100~+100 | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

## 寫入命令

| 編號 | 命令 | 參數 | 說明 |
| :--- | :--- | :--- | :--- |
| [`E5`](/sensors/ps2/reference.md#ce5) | 震動 `vibrate` | 強度 0~255 | [強度 0~255]，0 = 停；PS2 接收器收到後回 ACK 但忽略（250614 已關手把震動），PS4 才有作用 |
| [`E6`](/sensors/ps2/reference.md#ce6) | 燈條顏色 `setLed` | 紅 0~255、綠 0~255、藍 0~255 | [R][G][B] 各 0~255；PS2 手把沒有燈條，接收器收到後回 ACK 但忽略，PS4 才有作用 |

## 狀態旗標 F3

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 手把已連線 (`connected`) | = 06 已連線 |

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
const uint8_t ADDR = 0x42;

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
  Serial.print("buttons16: "); Serial.println(mbcRead(ADDR, 0x01));   // 十六鍵
  Serial.print("dpad: "); Serial.println(mbcRead(ADDR, 0x02));   // 方向鍵
  Serial.print("shape: "); Serial.println(mbcRead(ADDR, 0x03));   // 形狀鍵
  Serial.print("shoulder: "); Serial.println(mbcRead(ADDR, 0x04));   // 肩鍵
  Serial.print("func: "); Serial.println(mbcRead(ADDR, 0x05));   // 功能鍵
  Serial.print("connected: "); Serial.println(mbcRead(ADDR, 0x06));   // 已連線
  Serial.print("battery: "); Serial.println(mbcRead(ADDR, 0x07));   // 電量
  Serial.print("l2Analog: "); Serial.println(mbcRead(ADDR, 0x08));   // L2 類比
  Serial.print("r2Analog: "); Serial.println(mbcRead(ADDR, 0x09));   // R2 類比
  Serial.print("sysKeys: "); Serial.println(mbcRead(ADDR, 0x0A));   // 系統鍵
  Serial.print("padType: "); Serial.println(mbcRead(ADDR, 0x0B));   // 手把類型
  { int16_t a[4]; mbcReadMany(ADDR, 0x10, 4, a); Serial.print("stick: "); for (uint8_t i = 0; i < 4; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// 下面貼上「單檔小幫手函式」
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/ps2.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw[0] = 1（PS2）、電量 07 恆 0、系統鍵 0A 恆 0、手把類型 0B 只會出現 0／1／2（I2C 定案 §2、§4）。
- 08／09 L2／R2 類比照定案寫 0~255：DS2 類比模式有壓力值；沒有壓力值時按下 = 255、放開 = 0（沿用 v1 0x14 的做法，盤點 .ino:598-603）。
- E5 震動、E6 燈條照 I2C 定案 §3 列在 PS2／PS4 共用命令表；PS2 收到回 ACK 後忽略（跟 v1 0x10／0x23 一樣）。
- 搖桿 10~13 在 I2C／mBlock 是 ±100（由 raw (v−128)×100/128、Y 上為正）；EV3／0x22 維持 0~99（50 中間）、SPIKE 維持 1~9（5 中間）照盤點，不改鎖住模式。
- EV3：模式 0、1、2、4 鎖住（照盤點）；模式 3 改 方向|形狀<<4|已連線<<8（原本沒有 bit8）；模式 5 改成 ID 格式 B（原本 = 按鍵／搖桿合併版）。
- SPIKE：0/5/7/9 鎖住，1、2、4、6 維持不動；模式 3 改 [已連線, 電量(0), 0]（原本 = 4 組按鍵）；模式 8 改 v[0] = ID|hw[0]<<8（原本 = 搖桿）。模式 9 v[6] button16 = PSX 排列（D3）。
- SPIKE 模式 5 只容得下 11 鍵（L2、R2、L3、R3、START 讀不到），鎖住不改。
- 接線表的腳位以各主機板常見預設為準（ESP32 可用 `Wire.begin(sda, scl)` 改）。
