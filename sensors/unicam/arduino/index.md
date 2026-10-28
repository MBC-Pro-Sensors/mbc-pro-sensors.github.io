<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# Arduino（I2C）讀值教學

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

神攝手通用版 v1（通用 I2C 版）用 MBC 通用 I2C 協議 v2，Arduino UNO、Nano、UNO R4、ESP32 都能讀，位址 **`0x4C`**。I2C 版每一層都給 16 色／16 點，數值是像素、照原值送，不用拆格子。每一格的完整對照在 [讀值參考：I2C](/sensors/unicam/reference.md#host-i2c)，整支範例在 [Arduino 範例](/sensors/unicam/arduino-examples.md)。

## 接線

| 相機 | Arduino UNO／Nano | UNO R4、ESP32 |
| :--- | :--- | :--- |
| 5V | 5V | 5V |
| GND | GND | GND |
| SDA | A4 | 板子上標 SDA 的腳 |
| SCL | A5 | 板子上標 SCL 的腳 |

> [!CAUTION]
> **I2C 腳位暫定，待上機**：相機裡 I2C 走 ESP32-CAM 的 IO1／IO3（跟樂高串口同一對），目前是 **SDA＝IO3、SCL＝IO1**，要等接 Arduino 實測後才定案。I2C 版和 SPIKE／EV3／MATRIX 版是不同韌體，不能同時用。

## 怎麼讀

1. 寫 **1 byte：值的編號**（例 `0x10`）。
2. **等 2 ms**。
3. 讀 **2×k byte**：從那個編號起連續 k 個值（k ≤ 16），每個值是 **int16，低位元組在前**。超過 16 個值分次讀。
4. 沒有的編號回 **−32768**。

| 編號 | 內容 |
| :--- | :--- |
| `0x01` | 相機模式編號（相機模式 × 10 ＋ 子選項，例 13＝顏色追蹤 ③ 標準） |
| `0x02` | 子選項 |
| `0x03` | 已教幾色 |
| `0x04` | 這一層給幾個（色數／點數／塊數） |
| `0x10` 起 | 讀值參考 I2C 表的「值 1」＝`0x10`、「值 2」＝`0x11`…（顏色追蹤 ④ 用到 `0x10`～`0x70`） |
| `0xF0` | 裝置 ID＝`0x4C`｜3<<8（高 byte 3＝通用版；學校版 1、WRO 版 2） |
| `0xF1`～`0xF5` | 協議版 2、韌體版 100（1.0）、狀態旗標、更新序號、開機秒數 |

| 命令 | 內容 |
| :--- | :--- |
| `E0 [相機模式, 子選項]` | 切換相機模式（存進相機，下次開機也是這個模式） |
| `E1 [第幾點 1～20, X 0～159, Y 0～119]` | 改座標取色的取樣點位置（存進相機） |
| `F8 [新位址, 新位址取反]` | 換 I2C 位址（共用命令，見 [通用 I2C 協議](/i2c-protocol.md)） |
| `EF [A5]` | 重新開機 |

目標顏色**只能在相機上教**，v1 沒有寫入顏色的命令。

## 最小讀值程式

切到**顏色追蹤 ③ 標準**，讀色 1 的 X、Y、面積、數量（值 1～4＝`0x10`～`0x13`）：

```cpp
#include <Wire.h>
const uint8_t CAM = 0x4C;                    // 出廠位址

// 從編號 id 起連續讀 k 個值（k ≤ 16）。回傳 false = 相機沒回應
bool camRead(uint8_t id, uint8_t k, int16_t *out) {
  Wire.beginTransmission(CAM);
  Wire.write(id);
  if (Wire.endTransmission() != 0) return false;
  delay(2);
  if (Wire.requestFrom(CAM, (uint8_t)(2 * k)) != 2 * k) return false;
  for (uint8_t i = 0; i < k; i++) {
    uint16_t lo = Wire.read(), hi = Wire.read();
    out[i] = (int16_t)(lo | (hi << 8));     // 低位元組在前
  }
  return true;
}

void camMode(uint8_t mode, uint8_t sub) {    // 命令 E0：切相機模式
  Wire.beginTransmission(CAM);
  Wire.write(0xE0);
  Wire.write(mode);
  Wire.write(sub);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  camMode(1, 3);                             // 顏色追蹤 ③ 標準
  delay(100);
}

void loop() {
  int16_t v[4];
  if (camRead(0x10, 4, v)) {                 // 色 1：X、Y、面積 %、數量
    if (v[0]) {
      Serial.print("色 1  X="); Serial.print(v[0]);
      Serial.print("  Y="); Serial.print(v[1]);
      Serial.print("  面積="); Serial.print(v[2]);
      Serial.print("%  數量="); Serial.println(v[3]);
    } else {
      Serial.println("色 1 沒看到");
    }
  } else {
    Serial.println("找不到相機，請檢查接線與位址");
  }
  delay(100);
}
```

同一層的色 2 從 `0x14` 起（每色 4 個值）；其他相機模式每個值的編號看 [讀值參考：I2C](/sensors/unicam/reference.md#host-i2c)。

## 逐格對照與範例

- [讀值參考：I2C](/sensors/unicam/reference.md#host-i2c)（值 1＝`0x10`）
- [Arduino 範例程式](/sensors/unicam/arduino-examples.md)
- [MBC 通用 I2C 協議](/i2c-protocol.md)：共用暫存器、換位址
- 用 micro:bit：[micro:bit 讀值教學](/sensors/unicam/microbit/index.md)

## 常見問題

- **一直「找不到相機」？** 確認 SDA／SCL 沒接反、GND 有共地；v1 的腳位還沒上機定案，有問題請 [聯絡我們](/contact.md)。
- **讀到 −32768？** 那個編號在目前的相機模式沒有定義；先讀 `0x01` 確認模式。
