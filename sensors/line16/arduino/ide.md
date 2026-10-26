<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/arduino/ide -->
# 循行者 16 路 (Line16) × Arduino × Arduino IDE

這一頁只講 **Arduino IDE（C++）**：在 Arduino／ESP32／UNO R4（I2C） 上用它讀循行者 16 路 (Line16)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**（4 pin：5V、GND、SDA、SCL），出廠 I2C 位址 `0x16`。UNO／Nano 接 A4（SDA）、A5（SCL）；UNO R4、ESP32 接板子上標 SDA／SCL 的腳。

<figure class="ref-fig"><img src="/images/figures/line16/wiring-uno.svg" alt="循行者 16 路 (Line16) × Arduino／ESP32／UNO R4（I2C）" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/arduino-ide.zh.md + _build/guides/product/line16/arduino-ide.zh.md {"product":"循行者 16 路 (Line16)","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 16 路 (Line16)：Arduino IDE（C++） 說明](/sensors/line16/arduino-i2c.md)。
<!-- /include-fallback -->

## 讀值

下面的寫法都用到 `mbcRead()` 小幫手：展開「小幫手程式」，第 ① 段（2 行）放在程式最上面、第 ② 段貼在程式最下面，位址 `0x16`。

<details class="helper">
<summary>小幫手程式（① 放最上面、② 貼最下面）</summary>

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

</details>

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |

```cpp
int16_t linePos = mbcRead(0x16, 0x01);   // 線位置：-16~+16
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

```cpp
int16_t linePosFine = mbcRead(0x16, 0x02);   // 細位置：-100~+100
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~16` 路 | 壓到線的路數 0~16，0 = 找不到線 |

```cpp
int16_t lineWidth = mbcRead(0x16, 0x03);   // 線寬：0~16 路
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v04" class="ref-item">

### 線段數 <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-groupCount.svg" alt="線段數" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |

```cpp
int16_t groupCount = mbcRead(0x16, 0x04);   // 線段數：0~8
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | bit0 = 第 1 路（最左）… bit15 = 第 16 路，1 = 那一路在線上 |

```cpp
int16_t bitmap = mbcRead(0x16, 0x05);   // 壓線位元圖：0~65535 bit
```

<p class="ref-note">這個值本身就可能是 −32768（最高位元 = 1，例如 <code>0x8000</code>），跟讀不到時回的 <code>MBC_NONE</code> 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 <code>mbcReadMany(0x16, 0x05, 1, &bitmap)</code>，回傳 <code>false</code> 才是讀不到。</p>

</section>

<section id="v06" class="ref-item">

### 最後離線方向 <small>lastDir</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 丟線前線最後在哪一側：−16 ~ +16（負 = 左），找回線後維持最後一次 |

```cpp
int16_t lastDir = mbcRead(0x16, 0x06);   // 最後離線方向：-16~+16
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v07" class="ref-item">

### 二值化門檻 <small>threshold</small>

| 範圍 | 意思 |
| :--- | :--- |
| `1~99` | 判斷「在線上」的光值門檻 1~99，預設 50；用 E4 改 |

```cpp
int16_t threshold = mbcRead(0x16, 0x07);   // 二值化門檻：1~99
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v08" class="ref-item">

### 校準完成次數 <small>calibCount</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` 次 | 開機後完成校準的次數（按鈕校準也算） |

```cpp
int16_t calibCount = mbcRead(0x16, 0x08);   // 校準完成次數：0~255 次
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v10" class="ref-item">

### 第 1~16 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="第 1~16 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

```cpp
int16_t light[16];
mbcReadMany(0x16, 0x10, 16, light);   // 第 1~16 路光值：0~100
for (uint8_t i = 0; i < 16; i++) { Serial.print(light[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x10~0x1F 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

## 命令

<section id="ce0" class="ref-item ref-cmd">

### 切黑線 <small>blackLine</small>

追黑線（預設）

```cpp
mbcWrite(0x16, 0xE0);   // 切黑線
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### 切白線 <small>whiteLine</small>

追白線（F3 b1 = 1）

```cpp
mbcWrite(0x16, 0xE1);   // 切白線
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### 黑色校準 <small>calibBlack</small>

把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM

```cpp
mbcWrite(0x16, 0xE2);   // 黑色校準
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // 等 F3 bit2 歸零 = 做完
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### 白色校準 <small>calibWhite</small>

把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM

```cpp
mbcWrite(0x16, 0xE3);   // 白色校準
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // 等 F3 bit2 歸零 = 做完
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### 設定門檻 <small>setThreshold</small>

二值化門檻 1~99（RAM，重開機回預設 50）

```cpp
mbcWrite(0x16, 0xE4, 50);   // 設定門檻（門檻 1~99）
```

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5 秒動態校準 <small>calibAuto</small>

5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1

```cpp
mbcWrite(0x16, 0xE7);   // 5 秒動態校準
while (mbcRead(0x16, 0xF3) & (1 << 2)) delay(10);   // 等 F3 bit2 歸零 = 做完
```

</section>

## 範例

<!-- examples: sensors/line16/arduino-examples.md host=arduino sw=ide whole -->

## 其他問題

- **一直讀到 -32768？** 代表模組沒回應：檢查 SDA／SCL 有沒有接反、位址對不對、5V／GND 有沒有接好。
- **可以接很多顆嗎？** 可以，每顆位址要不同；同型號用命令 F8 換位址（見通用 I2C 協議）。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [全部範例 line16_arduino_examples.zip](/examples/line16/arduino/line16_arduino_examples.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line16.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/arduino-ide.zh.md`、`_build/guides/product/line16/arduino-ide.zh.md`
- 範例：`sensors/line16/arduino-examples.md`
