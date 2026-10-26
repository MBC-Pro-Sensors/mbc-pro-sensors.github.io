<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/arduino/ide -->
# 神攝手 ESP32CAM × Arduino × Arduino IDE

這一頁只講 **Arduino IDE（C++）**：在 Arduino／ESP32／UNO R4（I2C） 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**（4 pin：5V、GND、SDA、SCL），出廠 I2C 位址 `0x4C`。UNO／Nano 接 A4（SDA）、A5（SCL）；UNO R4、ESP32 接板子上標 SDA／SCL 的腳。

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-uno.svg" alt="神攝手 ESP32CAM × Arduino／ESP32／UNO R4（I2C）" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/arduino-ide.zh.md + _build/guides/product/esp32cam/arduino-ide.zh.md {"product":"神攝手 ESP32CAM","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：Arduino IDE（C++） 說明](/sensors/esp32cam/arduino-i2c.md)。
<!-- /include-fallback -->

## 讀值（學校版）

下面的寫法都用到 `mbcRead()` 小幫手：展開「小幫手程式」，第 ① 段（2 行）放在程式最上面、第 ② 段貼在程式最下面，位址 `0x4C`。

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

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

```cpp
int16_t hue = mbcRead(0x4C, 0x01);   // 色相：-1~100
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v02" class="ref-item">

### 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點飽和度 0~100 |

```cpp
int16_t saturation = mbcRead(0x4C, 0x02);   // 飽和度：0~100
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v03" class="ref-item">

### 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點明度 0~100 |

```cpp
int16_t value = mbcRead(0x4C, 0x03);   // 明度：0~100
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

```cpp
int16_t motion = mbcRead(0x4C, 0x04);   // 動態：0~100
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

```cpp
int16_t blobX = mbcRead(0x4C, 0x05);   // 色塊 X：0~100
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

```cpp
int16_t blobY = mbcRead(0x4C, 0x06);   // 色塊 Y：0~75
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v07" class="ref-item">

### 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊寬 W 0~100；0 = 沒找到 |

```cpp
int16_t blobW = mbcRead(0x4C, 0x07);   // 色塊寬：0~100
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v08" class="ref-item">

### 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊高 H 0~75；沒找到 = 0 |

```cpp
int16_t blobH = mbcRead(0x4C, 0x08);   // 色塊高：0~75
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v09" class="ref-item">

### 色塊面積 <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="色塊面積" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~7500` | W×H（0~7500）；沒找到 = 0 |

```cpp
int16_t blobArea = mbcRead(0x4C, 0x09);   // 色塊面積：0~7500
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v0a" class="ref-item">

### 有找到色塊 <small>found</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |

```cpp
int16_t found = mbcRead(0x4C, 0x0A);   // 有找到色塊：0~1
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

## 讀值（WRO 版）

下面的寫法都用到 `mbcRead()` 小幫手：展開「小幫手程式」，第 ① 段（2 行）放在程式最上面、第 ② 段貼在程式最下面，位址 `0x4C`。

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

<section id="w10" class="ref-item">

### 色點 1~12 顏色碼 <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="色點 1~12 顏色碼" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |

```cpp
int16_t spotColor[12];
mbcReadMany(0x4C, 0x10, 12, spotColor);   // 色點 1~12 顏色碼：0~255
for (uint8_t i = 0; i < 12; i++) { Serial.print(spotColor[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x10~0x1B 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

<section id="w20" class="ref-item">

### 色點 1~12 鮮豔度 <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="色點 1~12 鮮豔度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

```cpp
int16_t spotVivid[12];
mbcReadMany(0x4C, 0x20, 12, spotVivid);   // 色點 1~12 鮮豔度：0~255
for (uint8_t i = 0; i < 12; i++) { Serial.print(spotVivid[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x20~0x2B 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

## 範例（學校版）

<!-- examples: sensors/esp32cam/arduino-examples.md host=arduino sw=ide whole -->

## 其他問題

- **一直讀到 -32768？** 代表模組沒回應：檢查 SDA／SCL 有沒有接反、位址對不對、5V／GND 有沒有接好。
- **可以接很多顆嗎？** 可以，每顆位址要不同；同型號用命令 F8 換位址（見通用 I2C 協議）。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [全部範例 esp32cam_arduino_examples.zip](/examples/esp32cam/arduino/esp32cam_arduino_examples.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/arduino-ide.zh.md`、`_build/guides/product/esp32cam/arduino-ide.zh.md`
- 範例：`sensors/esp32cam/arduino-examples(-wro).md`
