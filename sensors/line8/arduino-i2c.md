<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
# ∿ 通用 I2C 版本開發指南

[🔙 回到 8路循線器首頁](/sensors/line8/index.md)

<div style="display: flex; align-items: center; justify-content: center; gap: 30px; margin: 30px 0; flex-wrap: wrap;">
  <div style="background: rgba(10,186,181,0.03); border: 1px solid rgba(10,186,181,0.25); border-radius: 12px; padding: 25px 45px; display: flex; align-items: center; gap: 25px; box-shadow: 0 10px 30px rgba(0,0,0,0.15);">
    <img src="/images/sensors/line8/line8-product-arduino.webp" alt="循行者8路感應器 (通用 I2C 版)" style="max-height: 144px; object-fit: contain; filter: drop-shadow(0 0 20px rgba(10,186,181,0.75)) drop-shadow(0 0 45px rgba(10,186,181,0.45));" />
    <span style="font-size: 2.5rem; color: #777; font-weight: 300; line-height: 1;">+</span>
    <div style="display: flex; gap: 15px; align-items: center;">
      <svg viewBox="0 0 100 40" style="height: 60px; fill: none; stroke: #00d2ff; stroke-width: 4; stroke-linecap: round; stroke-linejoin: round; filter: drop-shadow(0 0 10px rgba(0, 210, 255, 0.4));">
        <path d="M 5,25 H 25 V 5 H 45 V 35 H 65 V 5 H 85 V 35 H 95" />
      </svg>
    </div>
  </div>
</div>

歡迎來到專業硬體開發專區！MBC 8 路循線感應器除了完美支援樂高生態系，也為 **Arduino、ESP32、樹莓派以及各式自主開發主控板** 開發者提供了開放式的標準 I2C 通訊介面。
本感應器讀寫暫存器完全分離，採無衝突設計，非常適合用於開發高速循線機器人或上位機視覺化儀表板。

---

> [!IMPORTANT]
> **本頁為新版 v1 協議（位址 `0x16`）。** 所有 MBC 產品共用的讀寫規則、狀態暫存器與 Arduino 共用函式，請先看 [MBC 通用 I2C 協議](/i2c-protocol.md)。

## 📡 基本資訊

| 項目 | 內容 |
| :--- | :--- |
| I2C 位址 | `0x16`（循線系列共用） |
| 型號字串 `0x07` | `LINE8` |
| 款式 `0x06` | `[8, PCB 版次]`（第 1 byte = 通道數） |
| 通道順序 | **CH0 = 最右邊**，CH7 = 最左邊 |
| 線位置方向 | **負數 = 線在右邊、正數 = 線在左邊**，0 = 正中央 |

## 1️⃣ 循線結果（最常用）

| 暫存器 | 方向 | 長度 | 內容 |
| :---: | :---: | :---: | :--- |
| `0x10` | W | 1 | 循線目標：`0` = 黑線、`1` = 白線（重開機後回到按鈕設定） |
| `0x11` | R | 7 | **循線合併包**（= `0x12` ~ `0x17`，一次讀完最方便） |
| `0x12` | R | 1 | 線位置 int8：`-8` ~ `+8` |
| `0x13` | R | 1 | **高解析位置** int8：`-100` ~ `+100`（最適合 PID 控制） |
| `0x14` | R | 1 | 線寬：同時偵測到線的通道數 |
| `0x15` | R | 1 | 線群組數：`0` = 丟線、`1` = 單線、`2` 以上 = 分岔／十字路口 |
| `0x16` | R | 2 | 二值化圖 uint16：bit i = CH i 在線上 |
| `0x17` | R | 1 | 最後離線方向 int8（丟線時用來判斷往哪邊找） |

## 2️⃣ 校準

| 暫存器 | 方向 | 長度 | 內容 |
| :---: | :---: | :---: | :--- |
| `0x20` | W | 1 | `1` = 5 秒動態校準（期間左右移動車體掃過黑線）、`2` = 目前讀值記為白、`3` = 目前讀值記為黑。結果存入 EEPROM |
| `0x21` | R | 2 | 校準合併包（= `0x22` ~ `0x23`） |
| `0x22` | RW | 1 | 二值化閾值 `1` ~ `99`（預設 50；重開機回到預設） |
| `0x23` | R | 1 | 校準完成次數（每完成一次 +1，用來確認校準已執行完） |

## 3️⃣ 狀態旗標 `0x32`

| 位元 | 內容 |
| :---: | :--- |
| bit0 | 就緒 |
| bit1 | 白線模式（0 = 黑線） |
| bit2 | 校準中（5 秒校準期間更新序號 `0x33` 會停住，屬正常） |
| bit3 | 丟線 |
| bit4 | 分岔（群組數 ≥ 2） |

## 🔬 各通道數值

| 暫存器 | 方向 | 長度 | 內容 |
| :---: | :---: | :---: | :--- |
| `0x40` ~ `0x47` | R | 1 | 單一通道校準後數值 `0` ~ `100`（`0x40` + 通道號） |
| `0x50` | R | 8 | 全部通道校準後數值 `0` ~ `100` |
| `0x51` | R | 8 | 白色參考值（EEPROM） |
| `0x52` | R | 8 | 黑色參考值（EEPROM） |
| `0x53` | R | 8 | 原始 ADC 值（8-bit） |

---

## 💻 Arduino 範例：讀取循線結果

```cpp
#include <Wire.h>

const uint8_t LINE_ADDR = 0x16;

bool iicRead(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t n) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) buf[i] = Wire.read();
  return true;
}

void iicWrite(uint8_t addr, uint8_t reg, uint8_t v) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(v);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  iicWrite(LINE_ADDR, 0x10, 0);     // 0 = 循黑線、1 = 循白線
}

void loop() {
  uint8_t d[7];
  if (iicRead(LINE_ADDR, 0x11, d, 7)) {
    int8_t   pos    = (int8_t)d[0];        // -8 ~ +8
    int8_t   posHi  = (int8_t)d[1];        // -100 ~ +100，PID 用這個
    uint8_t  width  = d[2];                // 線寬
    uint8_t  groups = d[3];                // 0 丟線 / 1 單線 / 2+ 分岔
    uint16_t bin    = (d[4] << 8) | d[5];  // 每個 bit 一個通道

    Serial.print("位置 ");     Serial.print(pos);
    Serial.print("  高解析 "); Serial.print(posHi);
    Serial.print("  線寬 ");   Serial.print(width);
    Serial.print("  群組 ");   Serial.print(groups);
    Serial.print("  二值 ");   Serial.println(bin, BIN);
  }
  delay(10);
}
```

### PID 循線的起點

```cpp
// posHi：負 = 線在右、正 = 線在左。線在右邊時要右轉 → 左輪加速、右輪減速
float Kp = 0.6;
int base = 120;                        // 基本速度（依你的馬達驅動調整）
int turn = Kp * posHi;
int leftSpeed  = base - turn;
int rightSpeed = base + turn;
```

### 用程式觸發校準

```cpp
uint8_t before[2], after[2];
iicRead(LINE_ADDR, 0x21, before, 2);   // [閾值, 校準完成次數]
iicWrite(LINE_ADDR, 0x20, 1);          // 開始 5 秒動態校準：這段時間左右移動車體掃過黑線
do {
  delay(200);
  iicRead(LINE_ADDR, 0x21, after, 2);
} while (after[1] == before[1]);       // 次數 +1 = 校準完成
```

<br>

!!! success "🚀 想要學更厲害的循線控制方法嗎？"
可以找這幾位厲害的教練上課唷！他們有非常豐富的比賽與教學經驗，保證讓你收穫滿滿～ 💯

<div style="display: flex; flex-wrap: wrap; gap: 20px; justify-content: center; margin-top: 15px; margin-bottom: 30px;">
<div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
<h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@LegoLauXiao" target="_blank" style="color: inherit; text-decoration: none;">LegoLauXiao 教練</a></h4>
<div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
<iframe src="https://www.youtube.com/embed/WgacdWLatbk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
</div>
</div>
<div style="flex: 1; min-width: 250px; max-width: 320px; display: flex; flex-direction: column; align-items: center;">
<h4 style="margin: 0 0 10px 0; text-align: center;">🏆 <a href="https://www.youtube.com/@legolaumo" target="_blank" style="color: inherit; text-decoration: none;">legolaumo 教練</a></h4>
<div style="width: 100%; aspect-ratio: 9/16; background: #000; border-radius: 12px; overflow: hidden; border: 1px solid rgba(0,210,255,0.2); box-shadow: 0 10px 25px rgba(0,0,0,0.3);">
<iframe src="https://www.youtube.com/embed/T9bcndBNQvQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen loading="lazy" style="width: 100%; height: 100%; border: none;"></iframe>
</div>
</div>
</div>

