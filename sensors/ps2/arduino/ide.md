<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/arduino/ide -->
# 掌控者 PS2 (PS2) × Arduino × Arduino IDE

這一頁只講 **Arduino IDE（C++）**：在 Arduino／ESP32／UNO R4（I2C） 上用它讀掌控者 PS2 (PS2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**（4 pin：5V、GND、SDA、SCL），出廠 I2C 位址 `0x42`。UNO／Nano 接 A4（SDA）、A5（SCL）；UNO R4、ESP32 接板子上標 SDA／SCL 的腳。

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-uno.svg" alt="掌控者 PS2 (PS2) × Arduino／ESP32／UNO R4（I2C）" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/arduino-ide.zh.md + _build/guides/product/ps2/arduino-ide.zh.md {"product":"掌控者 PS2 (PS2)","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS2 (PS2)：Arduino IDE（C++） 說明](/sensors/ps2/arduino-i2c.md)。
<!-- /include-fallback -->

## 讀值

下面的寫法都用到 `mbcRead()` 小幫手：展開「小幫手程式」，第 ① 段（2 行）放在程式最上面、第 ② 段貼在程式最下面，位址 `0x42`。

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

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 16 個按鍵合成一個數（PSX 排列，1 = 按下）：bit0 SELECT、1 L3、2 R3、3 START、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；未連線 = 0 |

```cpp
int16_t buttons16 = mbcRead(0x42, 0x01);   // 十六鍵：0~65535 bit
```

<p class="ref-note">這個值本身就可能是 −32768（最高位元 = 1，例如 <code>0x8000</code>），跟讀不到時回的 <code>MBC_NONE</code> 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 <code>mbcReadMany(0x42, 0x01, 1, &buttons16)</code>，回傳 <code>false</code> 才是讀不到。</p>

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |

```cpp
int16_t dpad = mbcRead(0x42, 0x02);   // 方向鍵：0~15 bit
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |

```cpp
int16_t shape = mbcRead(0x42, 0x03);   // 形狀鍵：0~15 bit
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |

```cpp
int16_t shoulder = mbcRead(0x42, 0x04);   // 肩鍵：0~15 bit
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |

```cpp
int16_t func = mbcRead(0x42, 0x05);   // 功能鍵：0~15 bit
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |

```cpp
int16_t connected = mbcRead(0x42, 0x06);   // 已連線：0~1
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v07" class="ref-item">

### 電量 <small>battery</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0` % | 0~100；PS2 手把沒有電量 → 恆 0（PS4 才有） |

```cpp
int16_t battery = mbcRead(0x42, 0x07);   // 電量：0 %
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v08" class="ref-item">

### L2 類比 <small>l2Analog</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | L2 壓力 0~255（DualShock 2 類比模式才有壓力值；手把沒有壓力值時按下 = 255、放開 = 0）；未連線 = 0 |

```cpp
int16_t l2Analog = mbcRead(0x42, 0x08);   // L2 類比：0~255
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v09" class="ref-item">

### R2 類比 <small>r2Analog</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | R2 壓力 0~255（同 08）；未連線 = 0 |

```cpp
int16_t r2Analog = mbcRead(0x42, 0x09);   // R2 類比：0~255
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v0a" class="ref-item">

### 系統鍵 <small>sysKeys</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0` bit | b0 PS 鍵、b1 擷取鍵；PS2 手把沒有 → 恆 0（PS4 才有） |

```cpp
int16_t sysKeys = mbcRead(0x42, 0x0A);   // 系統鍵：0 bit
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v0b" class="ref-item">

### 手把類型 <small>padType</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~2` | 0 沒有手把、1 PS2 數位模式、2 PS2 類比模式（3 PS4、4 PS5 只有 PS4 接收器會出現） |

```cpp
int16_t padType = mbcRead(0x42, 0x0B);   // 手把類型：0~2
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

```cpp
int16_t stick[4];
mbcReadMany(0x42, 0x10, 4, stick);   // 搖桿 LX／LY／RX／RY：-100~+100
for (uint8_t i = 0; i < 4; i++) { Serial.print(stick[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x10~0x13 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

## 命令

<section id="ce5" class="ref-item ref-cmd">

### 震動 <small>vibrate</small>

[強度 0~255]，0 = 停；PS2 接收器收到後回 ACK 但忽略（250614 已關手把震動），PS4 才有作用

```cpp
mbcWrite(0x42, 0xE5, 128);   // 震動（強度 0~255）
```

</section>

<section id="ce6" class="ref-item ref-cmd">

### 燈條顏色 <small>setLed</small>

[R][G][B] 各 0~255；PS2 手把沒有燈條，接收器收到後回 ACK 但忽略，PS4 才有作用

```cpp
mbcWrite(0x42, 0xE6, 128, 128, 128);   // 燈條顏色（紅 0~255、綠 0~255、藍 0~255）
```

</section>

## 範例

<!-- examples: sensors/ps2/arduino-examples.md host=arduino sw=ide whole -->

## 其他問題

- **一直讀到 -32768？** 代表模組沒回應：檢查 SDA／SCL 有沒有接反、位址對不對、5V／GND 有沒有接好。
- **可以接很多顆嗎？** 可以，每顆位址要不同；同型號用命令 F8 換位址（見通用 I2C 協議）。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [全部範例 ps2_arduino_examples.zip](/examples/ps2/arduino/ps2_arduino_examples.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/arduino-ide.zh.md`、`_build/guides/product/ps2/arduino-ide.zh.md`
- 範例：`sensors/ps2/arduino-examples.md`
