<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ∿ MBC 通用 I2C 協議 (v1)

MBC-Pro 全系列的「通用 I2C 版」感應器都使用同一套暫存器規則：學會一款，其他款照著用就好。本頁說明所有產品共用的部分；各產品的資料暫存器請看各自的說明頁。

適用主機：**Arduino UNO / Nano / Mega / R4、ESP32、Raspberry Pi**，以及任何能當 I2C 主機的控制板。

---

## 📋 產品位址總覽

| 產品 | I2C 位址 | 型號字串 (`0x07`) | 暫存器說明 |
| :--- | :---: | :---: | :--- |
| 循行者 8 路 (Line8) | `0x16` | `LINE8` | [Line8 I2C 說明](/sensors/line8/arduino-i2c.md) |
| 循行者 16 路 (Line16) | `0x16` | `LINE16` | [Line16 I2C 說明](/sensors/line16/arduino-i2c.md) |
| 測距者 8 路 (TOF8) | `0x29` | `LASER8` | [TOF8 產品頁](/sensors/tof8/index.md) |
| 掌控者 PS2 | `0x42` | `PS2` | [PS2 產品頁](/sensors/ps2/index.md) |
| 掌控者 PS4/PS5 | `0x42` | `PS4` | [PS4/PS5 產品頁](/sensors/ps4/index.md) |
| SPIKE 6 路擴充器 (EXP6) | `0x6E` | `EXP6` | [EXP6 I2C 說明](/sensors/exp6/arduino-i2c.md) |
| 陀螺儀 IMU | `0x6A` | `IMU` | [IMU 產品頁](/sensors/imu/index.md) |

> [!TIP]
> 同一類產品共用一個位址（例如循線系列都是 `0x16`），用 `0x06` 暫存器分辨款式。主機開機時讀一次 `0x01`，就能知道接上的是哪一款。

---

## 🔌 接線與電壓

- 依感應器排針標示接 **SCL、SDA、VCC、GND**。
- 支援 **100 kHz** 與 **400 kHz**。
- **全系列產品**的 I2C 都可直接接 Arduino UNO 等 5 V 主機，不需要另外加準位轉換。

---

## 📥 讀取資料（模組 → 主機）

1. 主機寫 1 byte：要讀的**暫存器編號**。
2. 主機 `requestFrom(位址, n)`，n = 1~32。
3. 模組固定準備 32 bytes：有效資料在前、其餘補 0。主機要幾個就收幾個，多讀只會拿到 0，不會卡住匯流排。

- 多位元組數值一律 **高位元組在前（MSB-first）**；有號數為二補數。
- 未定義的暫存器讀回全 0。
- **PS4/PS5 接收器**：寫完暫存器編號後要**等至少 2 ms 再讀**。其他產品不需要等待。

## 📤 寫入命令（主機 → 模組）

- 格式：`[暫存器編號, 值0, 值1, …]`，多位元組同樣高位元組在前。
- 長度不足整筆忽略；多出來的 bytes 丟棄。只有標示 **W** 或 **RW** 的暫存器可以寫。
- 命令是**非同步**執行的（例如校準、寫入 EEPROM），請用狀態旗標或計數器確認已完成，不要假設寫完就生效。

## 🔄 資料一致性

模組每 **10 ms** 更新一次資料快照，讀取時一次取出整份快照。所以同一個「合併包」裡的各個數值，一定是同一時間點的資料，不會讀到半新半舊的值。

---

## 🗂️ 暫存器分區

| 範圍 | 用途 |
| :--- | :--- |
| `0x01` ~ `0x0F` | 裝置資訊（所有產品共通） |
| `0x10` ~ `0x1F` | 第 1 組：主要資料 |
| `0x20` ~ `0x2F` | 第 2 組：次要資料／校準 |
| `0x30` ~ `0x3F` | 第 3 組：系統與狀態（前段共通） |
| `0x40` 以上 | 產品擴充區：陣列、各通道、各埠 |

每一組的規則都一樣：

- **`0xN0`** = 該組的命令暫存器（寫入用）
- **`0xN1`** = 該組的**合併包**：把 `0xN2` 起的單項依序串在一起，一次讀完
- **`0xN2` 起** = 各個單項

## 🧾 共通暫存器（所有產品都有）

| 暫存器 | 方向 | 長度 | 內容 |
| :---: | :---: | :---: | :--- |
| `0x01` | R | 17 | 版本合併包（= `0x02` ~ `0x07`） |
| `0x02` | R | 1 | 裝置 ID（= 該產品出廠位址，用來辨識產品） |
| `0x03` | R | 1 | 協議版本（目前 = 1） |
| `0x04` | R | 2 | 韌體版本（主, 次） |
| `0x05` | R | 3 | 韌體編譯日期（年末兩位, 月, 日） |
| `0x06` | R | 2 | 硬體／款式（各產品定義） |
| `0x07` | R | 8 | 型號字串（ASCII，不足補 0，例如 `LINE16`） |
| `0x30` | W | 1 | 系統命令：`0x01` = 重新開機 |
| `0x31` | R | 9 | 狀態合併包（= `0x32` ~ `0x37`） |
| `0x32` | R | 1 | 狀態旗標：**bit0 = 就緒**；bit1~7 各產品定義 |
| `0x33` | R | 1 | 更新序號：每次資料更新 +1；數字不動代表模組忙碌中 |
| `0x34` ~ `0x36` | R | 1 | 各產品定義 |
| `0x37` | R | 4 | 開機經過時間（ms）；數字變小代表模組重開機過 |

**建議主機流程：** 開機先讀 `0x01` 確認產品 → 讀 `0x32` 等到 bit0 = 1（就緒）→ 開始讀取各產品資料。

---

## 💻 Arduino 共用函式

以下函式適用所有 MBC 通用 I2C 產品，各產品範例都會用到：

```cpp
#include <Wire.h>

// 讀 n bytes（n ≤ 32）。回傳 false = 裝置沒回應
bool iicRead(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  delay(2);                                   // PS4/PS5 需要；其他產品可刪
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

// 寫 1 byte 命令／設定
void iicWrite(uint8_t addr, uint8_t reg, uint8_t v) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(v);
  Wire.endTransmission();
}

// 高位元組在前 → 數值
int16_t  be16(const uint8_t *p)  { return (int16_t)((p[0] << 8) | p[1]); }
uint16_t beU16(const uint8_t *p) { return (uint16_t)((p[0] << 8) | p[1]); }
```

### 範例：辨識接上的產品

```cpp
void setup() {
  Serial.begin(115200);
  Wire.begin();

  uint8_t info[17];
  if (iicRead(0x16, 0x01, info, 17)) {        // 0x16 = 循線系列
    char model[9] = {0};
    memcpy(model, &info[9], 8);               // 0x07 型號字串位於合併包第 9~16 byte
    Serial.print("型號: ");     Serial.println(model);
    Serial.print("韌體版本: "); Serial.print(info[2]); Serial.print('.'); Serial.println(info[3]);
  } else {
    Serial.println("找不到裝置，請檢查接線與位址");
  }
}

void loop() {}
```

> [!NOTE]
> 通用 I2C 版與 SPIKE 版、EV3 版是**不同的硬體版本**，接頭不同、不能互換。購買前請確認版本，詳情請 [聯絡我們](/contact.md)。
