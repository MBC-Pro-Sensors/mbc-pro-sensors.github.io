<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × arduino × ide），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/arduino/ide -->
# 陀螺儀 IMU 九軸 × Arduino × Arduino IDE

這一頁只講 **Arduino IDE（C++）**：在 Arduino／ESP32／UNO R4（I2C） 上用它讀陀螺儀 IMU 九軸的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**（4 pin：5V、GND、SDA、SCL），出廠 I2C 位址 `0x6A`。UNO／Nano 接 A4（SDA）、A5（SCL）；UNO R4、ESP32 接板子上標 SDA／SCL 的腳。

<figure class="ref-fig"><img src="/images/figures/imu/wiring-uno.svg" alt="陀螺儀 IMU 九軸 × Arduino／ESP32／UNO R4（I2C）" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/arduino-ide.zh.md + _build/guides/product/imu/arduino-ide.zh.md {"product":"陀螺儀 IMU 九軸","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[陀螺儀 IMU 九軸：Arduino IDE（C++） 說明](/sensors/imu/arduino-i2c.md)。
<!-- /include-fallback -->

## 讀值

下面的寫法都用到 `mbcRead()` 小幫手：展開「小幫手程式」，第 ① 段（2 行）放在程式最上面、第 ② 段貼在程式最下面，位址 `0x6A`。

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

### 陀螺 3D 航向 <small>headingGyro3D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |

```cpp
int16_t headingGyro3D = mbcRead(0x6A, 0x01);   // 陀螺 3D 航向：-32768~32767 °
```

<p class="ref-note">這個值本身就可能是 −32768（最高位元 = 1，例如 <code>0x8000</code>），跟讀不到時回的 <code>MBC_NONE</code> 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 <code>mbcReadMany(0x6A, 0x01, 1, &headingGyro3D)</code>，回傳 <code>false</code> 才是讀不到。</p>

</section>

<section id="v02" class="ref-item">

### 融合航向 <small>headingFused</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |

```cpp
int16_t headingFused = mbcRead(0x6A, 0x02);   // 融合航向：-32768~32767 °
```

<p class="ref-note">這個值本身就可能是 −32768（最高位元 = 1，例如 <code>0x8000</code>），跟讀不到時回的 <code>MBC_NONE</code> 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 <code>mbcReadMany(0x6A, 0x02, 1, &headingFused)</code>，回傳 <code>false</code> 才是讀不到。</p>

</section>

<section id="v03" class="ref-item">

### 陀螺 1D 航向 <small>headingGyro1D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |

```cpp
int16_t headingGyro1D = mbcRead(0x6A, 0x03);   // 陀螺 1D 航向：-32768~32767 °
```

<p class="ref-note">這個值本身就可能是 −32768（最高位元 = 1，例如 <code>0x8000</code>），跟讀不到時回的 <code>MBC_NONE</code> 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 <code>mbcReadMany(0x6A, 0x03, 1, &headingGyro1D)</code>，回傳 <code>false</code> 才是讀不到。</p>

</section>

<section id="v04" class="ref-item">

### 磁力航向 <small>headingMag</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 磁力計傾斜補償航向，度，減去相對零點 |

```cpp
int16_t headingMag = mbcRead(0x6A, 0x04);   // 磁力航向：-32768~32767 °
```

<p class="ref-note">這個值本身就可能是 −32768（最高位元 = 1，例如 <code>0x8000</code>），跟讀不到時回的 <code>MBC_NONE</code> 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 <code>mbcReadMany(0x6A, 0x04, 1, &headingMag)</code>，回傳 <code>false</code> 才是讀不到。</p>

</section>

<section id="v05" class="ref-item">

### 轉向軸角速度 <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="轉向軸角速度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 繞轉向軸（getYawAxis()）的角速度，0.1 度／秒，正負跟航向同向 |

```cpp
int16_t yawRate = mbcRead(0x6A, 0x05);   // 轉向軸角速度：-32768~32767 0.1 dps
```

<p class="ref-note">這個值本身就可能是 −32768（最高位元 = 1，例如 <code>0x8000</code>），跟讀不到時回的 <code>MBC_NONE</code> 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 <code>mbcReadMany(0x6A, 0x05, 1, &yawRate)</code>，回傳 <code>false</code> 才是讀不到。</p>

</section>

<section id="v06" class="ref-item">

### 俯仰角 <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="俯仰角" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-90~+90` ° | 板子絲印 X 軸相對水平面的仰角：X 軸朝上 +90、朝下 −90、水平 0（用重力向量 atan2 算） |

```cpp
int16_t pitch = mbcRead(0x6A, 0x06);   // 俯仰角：-90~+90 °
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v07" class="ref-item">

### 俯仰角速度 <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="俯仰角速度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 繞板子絲印 X 軸的角速度，0.1 度／秒（右手定則，從 +X 端看逆時針為正）；注意這是 gyro X，不是 06 的微分 |

```cpp
int16_t pitchRate = mbcRead(0x6A, 0x07);   // 俯仰角速度：-32768~32767 0.1 dps
```

<p class="ref-note">這個值本身就可能是 −32768（最高位元 = 1，例如 <code>0x8000</code>），跟讀不到時回的 <code>MBC_NONE</code> 一樣，不要拿 −32768 判斷有沒有讀到：要分辨就改用 <code>mbcReadMany(0x6A, 0x07, 1, &pitchRate)</code>，回傳 <code>false</code> 才是讀不到。</p>

</section>

<section id="v08" class="ref-item">

### 磁力信任度 <small>magTrust</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magTrust.svg" alt="磁力信任度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 融合演算法對磁力計的信任度 0~255（= 0~1 × 255） |

```cpp
int16_t magTrust = mbcRead(0x6A, 0x08);   // 磁力信任度：0~255
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v09" class="ref-item">

### 磁場干擾度 <small>magDisturb</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magDisturb.svg" alt="磁場干擾度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` 0.01° | 磁場干擾程度 0~255（度 × 100，上限 255） |

```cpp
int16_t magDisturb = mbcRead(0x6A, 0x09);   // 磁場干擾度：0~255 0.01°
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v0a" class="ref-item">

### 朝上的面 <small>upFace</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-upFace.svg" alt="朝上的面" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~5` | 哪個軸朝上：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z（實體軸） |

```cpp
int16_t upFace = mbcRead(0x6A, 0x0A);   // 朝上的面：0~5
```

<p class="ref-note">讀不到（沒接、位址錯）時回 <code>MBC_NONE</code>（−32768）。</p>

</section>

<section id="v10" class="ref-item">

### 角速度 1~3 軸 <small>gyro</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-gyro.svg" alt="角速度 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 10 X、11 Y、12 Z 角速度，0.1 度／秒（實體軸，已套軸向號誌） |

```cpp
int16_t gyro[3];
mbcReadMany(0x6A, 0x10, 3, gyro);   // 角速度 1~3 軸：-32768~32767 0.1 dps
for (uint8_t i = 0; i < 3; i++) { Serial.print(gyro[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x10~0x12 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

<section id="v13" class="ref-item">

### 加速度 1~3 軸 <small>accel</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-accel.svg" alt="加速度 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` mg | 13 X、14 Y、15 Z 加速度，mg（1000 = 1 g） |

```cpp
int16_t accel[3];
mbcReadMany(0x6A, 0x13, 3, accel);   // 加速度 1~3 軸：-32768~32767 mg
for (uint8_t i = 0; i < 3; i++) { Serial.print(accel[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x13~0x15 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

<section id="v16" class="ref-item">

### 磁力 1~3 軸 <small>mag</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-mag.svg" alt="磁力 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` mG | 16 X、17 Y、18 Z 磁場強度，mG（毫高斯） |

```cpp
int16_t mag[3];
mbcReadMany(0x6A, 0x16, 3, mag);   // 磁力 1~3 軸：-32768~32767 mG
for (uint8_t i = 0; i < 3; i++) { Serial.print(mag[i]); Serial.print(' '); }
Serial.println();
```

<p class="ref-note">編號 0x16~0x18 連號，一次 <code>mbcReadMany</code> 讀完保證同一瞬間。</p>

</section>

## 命令

<section id="ce0" class="ref-item ref-cmd">

### 航向歸零 <small>zeroHeading</small>

四個航向都歸零（zeroHeadings()）；EV3 另有原廠 WRITE 0x11 陀螺歸零

```cpp
mbcWrite(0x6A, 0xE0);   // 航向歸零
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### 加速度計校準開始 <small>accelCalStart</small>

開始加速度計校準（= v1 的 0x20=0x01）；期間 F3 b4 = 1

```cpp
mbcWrite(0x6A, 0xE1);   // 加速度計校準開始
// 校準中 F3 bit4 = 1。照產品說明做完動作後送「加速度計校準結束」：mbcWrite(0x6A, 0xE2); bit4 才會變 0
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### 加速度計校準結束 <small>accelCalEnd</small>

結束加速度計校準並存 EEPROM（= v1 的 0x20=0x02）；通過後自動航向歸零一次

```cpp
mbcWrite(0x6A, 0xE2);   // 加速度計校準結束
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### 磁力計校準開始 <small>magCalStart</small>

開始磁力計校準（= v1 的 0x20=0x03）；期間 F3 b5 = 1

```cpp
mbcWrite(0x6A, 0xE3);   // 磁力計校準開始
// 校準中 F3 bit5 = 1。照產品說明做完動作後送「磁力計校準結束」：mbcWrite(0x6A, 0xE4); bit5 才會變 0
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### 磁力計校準結束 <small>magCalEnd</small>

結束磁力計校準並存 EEPROM（= v1 的 0x20=0x04）

```cpp
mbcWrite(0x6A, 0xE4);   // 磁力計校準結束
```

</section>

## 範例

<!-- examples: sensors/imu/arduino-examples.md host=arduino sw=ide whole -->

## 其他問題

- **一直讀到 -32768？** 代表模組沒回應：檢查 SDA／SCL 有沒有接反、位址對不對、5V／GND 有沒有接好。
- **可以接很多顆嗎？** 可以，每顆位址要不同；同型號用命令 F8 換位址（見通用 I2C 協議）。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [全部範例 imu_arduino_examples.zip](/examples/imu/arduino/imu_arduino_examples.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/imu.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/arduino-ide.zh.md`、`_build/guides/product/imu/arduino-ide.zh.md`
- 範例：`sensors/imu/arduino-examples.md`
