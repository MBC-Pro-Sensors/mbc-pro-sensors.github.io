<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ∿ MBC 通用 I2C 協議 v2

MBC-Pro 全系列的「通用 I2C 版」感應器都用同一套規則：**暫存器編號就是值的編號**，寫一個編號、讀回一個或連續好幾個 16 位元整數。學會一款，其他款照著用就好；每款產品的值表請看它的 [指令參考](/reference.md)。

適用主機：**Arduino UNO / Nano / Mega / R4、ESP32、micro:bit、Raspberry Pi**，以及任何能當 I2C 主機的控制板。

> [!TIP]
> **不會寫程式也能用：** 每款產品都有可以直接複製上傳的 Arduino 範例，請從 [🔰 第一次用 Arduino](/sensors/arduino-start.md) 開始；micro:bit 請用 [MakeCode 擴充](/microbit.md)；MBC 圖控編程與 jBot 網頁版有現成積木。

> [!IMPORTANT]
> 這是 **2026-10 起的 v2 協議**（協議版本 2，`F1` 讀回 2）。2026-10 以前的韌體是 v1（0x01 版本包、0x11 合併包、高位元組在前），舊程式不相容；舊版檔案保留在 [下載中心](/downloads.md) 的舊版區。

---

## 📋 產品位址總覽

<!-- gen:i2c-addr-table -->
| 產品 | I2C 位址 | hw[0] | 型號 | 值表 | 指令參考 | 新手範例 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 神攝手 學校版 | `0x4C` | `1` | `CAMSCH` | [I2C](/sensors/esp32cam/arduino-i2c.md) | [參考](/sensors/esp32cam/reference.md) | [Arduino](/sensors/esp32cam/arduino-examples.md) |
| 神攝手 WRO 版（HUE12） | `0x4C` | `2` | `HUE12` | [I2C](/sensors/esp32cam/arduino-i2c-wro.md) | [參考](/sensors/esp32cam/reference-wro.md) | [Arduino](/sensors/esp32cam/arduino-examples.md) |
| IMU 陀螺儀 | `0x6A` | `1` | `IMU` | [I2C](/sensors/imu/arduino-i2c.md) | [參考](/sensors/imu/reference.md) | [Arduino](/sensors/imu/arduino-examples.md) |
| 循行者 16 路 | `0x16` | `16` | `LINE16` | [I2C](/sensors/line16/arduino-i2c.md) | [參考](/sensors/line16/reference.md) | [Arduino](/sensors/line16/arduino-examples.md) |
| 循行者 8 路 | `0x16` | `8` | `LINE8` | [I2C](/sensors/line8/arduino-i2c.md) | [參考](/sensors/line8/reference.md) | [Arduino](/sensors/line8/arduino-examples.md) |
| 掌控者 PS2 | `0x42` | `1` | `PS2` | [I2C](/sensors/ps2/arduino-i2c.md) | [參考](/sensors/ps2/reference.md) | [Arduino](/sensors/ps2/arduino-examples.md) |
| 掌控者 PS4／PS5 接收器 | `0x42` | `2` | `PS4` | [I2C](/sensors/ps4/arduino-i2c.md) | [參考](/sensors/ps4/reference.md) | [Arduino](/sensors/ps4/arduino-examples.md) |
| 測距者 2 路 | `0x29` | `2` | `LASER2` | [I2C](/sensors/tof2/arduino-i2c.md) | [參考](/sensors/tof2/reference.md) | [Arduino](/sensors/tof2/arduino-examples.md) |
| 測距者 8 路 | `0x29` | `8` | `LASER8` | [I2C](/sensors/tof8/arduino-i2c.md) | [參考](/sensors/tof8/reference.md) | [Arduino](/sensors/tof8/arduino-examples.md) |
<!-- /gen -->

> [!NOTE]
> - **SPIKE 6 路擴充器（EXP6）不走 I2C**：主機接頭有隔離保護，Arduino 要用序列埠（UART）控制，請看 [EXP6 Arduino 範例](/sensors/exp6/arduino-examples.md)。
> - 同一條 SDA／SCL 上，**位址相同的產品不能同時接**（循行者 8 路和 16 路、測距者 2 路和 8 路、PS2 和 PS4、神攝手兩版）。要同時接就先用 `F8` 把其中一台換位址。
> - 同一類產品共用一個位址，用 `F0` 的高位元組（hw0）分辨款式：循行者 8／16、測距者 2／8、PS2＝1／PS4＝2、神攝手學校版＝1／WRO 版＝2。

---

## 🔌 接線與電壓

- 依感應器排針標示接 **SCL、SDA、VCC、GND**（SPIKE／EV3 接頭的 4P 線：TX 腳＝SCL、RX 腳＝SDA）。
- 支援 **100 kHz**（建議，Arduino 預設）與 400 kHz。
- STM32 產品（循行者、測距者、PS2、陀螺儀）可直接接 5 V 主機；**ESP32 產品（PS4／PS5 接收器、神攝手）腳位不耐 5 V**，接 UNO 等 5 V 主機時上拉電阻要接 3.3 V。
- STM32 產品會「時脈延展」（clock stretching）；Arduino、ESP32 都支援。**Raspberry Pi 的硬體 I2C 處理時脈延展有已知問題**，讀值不穩時把速度降到 10~50 kHz 或改用 `i2c-gpio`。micro:bit V2 若讀值不穩請回報，韌體有「不延展」選項。

---

## 📥 讀取：寫編號、等 2 ms、讀 2×k 個 byte

1. 主機寫 **1 byte：值的編號**（例如 `0x01`）。
2. **等 2 ms**（ESP32 產品需要；STM32 產品不等也可以，函式庫一律等，代價很小）。
3. `requestFrom(位址, 2×k)`：從該編號起**連續 k 個值**（k ≤ 16，也就是最多 32 byte）。
4. 每個值是 **int16，低位元組在前（little-endian）**，與樂高、MATRIX、mBlock 一致。

- 沒有定義的編號回 **−32768**（`0x00 0x80`）；跨過空號連續讀也沒關係，空號就是 −32768。
- 模組每 **10 ms** 更新一次快照，一次連續讀到的值保證是同一瞬間的。
- 數值直接是使用者要的數字：線位置 −8~+8（負＝線在左）、搖桿 −100~+100、距離 cm／mm、色相 −1＝黑白灰。不用再減 100、減 50。

## 📤 寫入命令（E0～EE）

主機寫 `[命令編號][參數…]`；參數長度依命令（沒有參數就只寫 1 byte）。命令是**非同步**執行的（校準、寫 EEPROM），用 `F3` 狀態旗標確認完成。各產品的命令在它的指令參考頁，例如循行者 `E0` 切黑線、`E2` 黑色校準、`E4 [門檻]`；PS4 `E5 [強度]` 震動、`E6 [R][G][B]` 燈條；IMU `E0` 航向歸零。

---

## 🧾 共用暫存器（所有產品都有）

<!-- gen:i2c-common -->
| 編號 | 讀寫 | 名稱 | 說明 |
| :--- | :---: | :--- | :--- |
| <span id="rf0">`F0`</span> | R | 裝置 ID | 低 byte = 出廠 I2C 位址（換位址後不變），高 byte = hw[0]（通道數／顆數／類別） |
| <span id="rf1">`F1`</span> | R | 協議版本 | 本版 = 2（SPIKE 模式 8 v[1]、EV3 ID 封包同一個數字）（2） |
| <span id="rf2">`F2`</span> | R | 韌體版本 | 主版×100 + 次版（例 102 = 1.2） |
| <span id="rf3">`F3`</span> | R | 狀態旗標 | b0 就緒，其餘各產品定義（見各產品 flags）（bit） |
| <span id="rf4">`F4`</span> | R | 更新序號 | 每 10 ms 快照 +1，0~255 循環（2.56 秒一圈，不能拿來偵測重開機；不動 = 模組忙）（0~255） |
| <span id="rf5">`F5`</span> | R | 開機秒數 | 開機後秒數，到 32767 停住（約 9 小時）；數字變小 = 重開機過（0~32767） |
| <span id="rf8">`F8`</span> | W | 換 I2C 位址 | [F8][新位址 0x08~0x77][新位址取反]，存 EEPROM，重開機後生效；[F8][0x00][0xFF] = 回出廠位址（共識 B-5）；參數：新位址 0x08~0x77、新位址取反 |
| <span id="ref">`EF`</span> | W | 重新開機 | [EF][0xA5]；mBlock 先回 ACK 再重開；參數：確認碼 0xA5 |
<!-- /gen -->

**建議主機流程：** 開機讀 `F0`（認產品與款式）→ 讀 `F3` 等 bit0 = 1（就緒）→ 開始連續讀值。

**換位址：** 寫 `[F8][新位址][新位址取反]`（例如改成 0x17：`F8 17 E8`），重新開機後生效；寫 `[F8][00][FF]` 回出廠位址。`F0` 的低位元組永遠回出廠位址，所以換過位址也認得出是哪一款。

---

## 💻 Arduino 共用函式

每支範例的底部都有這一段，單檔就能用：

```cpp
#include <Wire.h>
const int16_t MBC_NONE = -32768;          // 空號／沒回應

// 從編號 id 起連續讀 k 個值（k ≤ 16）。回傳 false = 裝置沒回應
bool mbcReadMany(uint8_t addr, uint8_t id, uint8_t k, int16_t *out) {
  if (k < 1) k = 1; if (k > 16) k = 16;
  Wire.beginTransmission(addr);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                                  // ESP32 產品需要；其他產品不等也可以
  if (Wire.requestFrom(addr, (uint8_t)(2 * k)) != 2 * k) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read(), hi = Wire.read();
    out[i] = (int16_t)(lo | (hi << 8));     // 低位元組在前
  }
  return true;
}

int16_t mbcRead(uint8_t addr, uint8_t id) {  // 讀 1 個值
  int16_t v; return mbcReadMany(addr, id, 1, &v) ? v : MBC_NONE;
}

bool mbcWrite(uint8_t addr, uint8_t id, const uint8_t *p = NULL, uint8_t n = 0) {  // 命令／換位址／重開機
  Wire.beginTransmission(addr);
  Wire.write(id);
  for (uint8_t i = 0; i < n; i++) Wire.write(p[i]);
  return Wire.endTransmission() == 0;
}

bool mbcWaitReady(uint8_t addr, uint16_t ms) {   // 等 F3 bit0
  uint32_t t0 = millis();
  while (millis() - t0 < ms) { int16_t s = mbcRead(addr, 0xF3); if (s != MBC_NONE && (s & 1)) return true; delay(20); }
  return false;
}
```

### 範例：辨識接上的產品

```cpp
void setup() {
  Serial.begin(115200);
  Wire.begin();
  int16_t id = mbcRead(0x16, 0xF0);           // 0x16 = 循線系列
  if (id == MBC_NONE) { Serial.println("找不到裝置，請檢查接線與位址"); return; }
  Serial.print("出廠位址 0x"); Serial.print(id & 0xFF, HEX);
  Serial.print("，款式 hw0 = "); Serial.println(id >> 8);   // 8 = 8 路、16 = 16 路
  int16_t fw = mbcRead(0x16, 0xF2);
  Serial.print("韌體 "); Serial.print(fw / 100); Serial.print('.'); Serial.println(fw % 100);
}
void loop() {}
```

> [!NOTE]
> 通用 I2C 版與 SPIKE 版、EV3 版是**不同的硬體版本**，接頭不同、不能互換。購買前請確認版本，詳情請 [聯絡我們](/contact.md)。
