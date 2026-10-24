<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/arduino/ide -->
# 測距者 8 路 (TOF8) × Arduino × Arduino IDE

這一頁只講 **Arduino IDE（C++）**：在 Arduino／ESP32／UNO R4（I2C） 上用它讀測距者 8 路 (TOF8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**（4 pin：5V、GND、SDA、SCL），出廠 I2C 位址 `0x29`。UNO／Nano 接 A4（SDA）、A5（SCL）；UNO R4、ESP32 接板子上標 SDA／SCL 的腳。

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-uno.svg" alt="測距者 8 路 (TOF8) × Arduino／ESP32／UNO R4（I2C）" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/arduino-ide.zh.md + _build/guides/product/tof8/arduino-ide.zh.md {"product":"測距者 8 路 (TOF8)","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 8 路 (TOF8)：Arduino IDE（C++） 說明](/sensors/tof8/arduino-i2c.md)。
<!-- /include-fallback -->

## 讀值

下面的寫法都用到 `mbcRead()` 小幫手（展開「小幫手程式」貼在程式最下面），位址 `0x29`。

<details class="helper">
<summary>小幫手程式（貼在程式最下面）</summary>

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

</details>

<section id="v01" class="ref-item">

### 最近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="最近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |

```cpp
int16_t nearCm = mbcRead(0x29, 0x01);   // 最近距離 cm：0~120 cm
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v02" class="ref-item">

### 最近方向 <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="最近方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |

```cpp
int16_t nearDir = mbcRead(0x29, 0x02);   // 最近方向：-8~+8
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v03" class="ref-item">

### 最空曠距離 cm <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="最空曠距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |

```cpp
int16_t farCm = mbcRead(0x29, 0x03);   // 最空曠距離 cm：0~120 cm
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v04" class="ref-item">

### 最空曠方向 <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="最空曠方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-7~+7` | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |

```cpp
int16_t farDir = mbcRead(0x29, 0x04);   // 最空曠方向：-7~+7
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v05" class="ref-item">

### 正前方 cm <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="正前方 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 4、5 顆（兩顆平行朝前）平均 0~120 cm；EV3 模式 3、SPIKE 模式 6 給的是同一個值的 mm；0x22 的 C 是 min(第 4, 第 5 顆) 不是平均（見 10） |

```cpp
int16_t frontCm = mbcRead(0x29, 0x05);   // 正前方 cm：0~120 cm
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v06" class="ref-item">

### 最近距離 mm <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="最近距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 8 顆 mm 的最小值 0~1200（失效那顆以 1200 參與），1200 = 四周沒東西 |

```cpp
int16_t nearMm = mbcRead(0x29, 0x06);   // 最近距離 mm：0~1200 mm
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v07" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 顆（最左）… bit7 = 第 8 顆，1 = 那顆開機初始化成功（8 顆約 2~4 秒，失敗不重試）；255 = 全部好 |

```cpp
int16_t initOk = mbcRead(0x29, 0x07);   // 初始化位元圖：0~255 bit
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v08" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 顆 … bit7 = 第 8 顆，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

```cpp
int16_t fault = mbcRead(0x29, 0x08);   // 故障位元圖：0~255 bit
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v09" class="ref-item">

### 正前方距離 mm <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="正前方距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 第 4、5 顆（正前方兩顆）平均，0~1200 mm；1200 = 前面沒東西。cm 版是 05 |

```cpp
int16_t frontMm = mbcRead(0x29, 0x09);   // 正前方距離 mm：0~1200 mm
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v10" class="ref-item">

### 第 1~8 顆距離 cm <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="第 1~8 顆距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |

```cpp
int16_t distCm[8];
mbcReadMany(0x29, 0x10, 8, distCm);   // 第 1~8 顆距離 cm：0~120 cm
for (uint8_t i = 0; i < 8; i++) { Serial.print(distCm[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x10~0x17 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

<section id="v18" class="ref-item">

### 第 1~8 顆距離 mm <small>distMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distMm.svg" alt="第 1~8 顆距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 第 1~8 顆各自的距離 0~1200 mm，1200 = 那個方向沒東西／那顆失效；編號 18~1F |

```cpp
int16_t distMm[8];
mbcReadMany(0x29, 0x18, 8, distMm);   // 第 1~8 顆距離 mm：0~1200 mm
for (uint8_t i = 0; i < 8; i++) { Serial.print(distMm[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x18~0x1F 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

## 範例

<!-- examples: sensors/tof8/arduino-examples.md host=arduino sw=ide whole -->

## 其他問題

- **一直讀到 -32768？** 代表模組沒回應：檢查 SDA／SCL 有沒有接反、位址對不對、5V／GND 有沒有接好。
- **可以接很多顆嗎？** 可以，每顆位址要不同；同型號用命令 F8 換位址（見通用 I2C 協議）。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [全部範例 tof8_arduino_examples.zip](/examples/tof8/arduino/tof8_arduino_examples.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/arduino-ide.zh.md`、`_build/guides/product/tof8/arduino-ide.zh.md`
- 範例：`sensors/tof8/arduino-examples.md`
