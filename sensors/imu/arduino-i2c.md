<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/imu.json 產生（arduino-i2c），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/imu/arduino/ide.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/imu/arduino/ide.md">陀螺儀 IMU 九軸 › Arduino › Arduino IDE</a></p>

# IMU 陀螺儀 × Arduino I2C（通用 I2C 版）

[🔙 回到IMU 陀螺儀產品頁](/sensors/imu/index.md)

IMU 陀螺儀通用 I2C 版用 **MBC I2C 協議 v2**：寫一個編號、讀 2 byte 就是一個值（int16 小端），連號的值可以一次讀完。本頁是這款產品的值表、命令表與接線；共用規則（F0~F5 版本與狀態、F8 換位址、EF 重開機）在 [通用 I2C 協議](/i2c-protocol.md)。

<p class="chips"><span class="chip chip-spike off">SPIKE</span><a class="chip chip-ev3" href="/sensors/imu/index.md">EV3</a><a class="chip chip-arduino current" href="/sensors/imu/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/imu/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/imu/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/imu/microbit.md">micro:bit</a></p>

> [!TIP]
> **不太會寫程式？** 直接看 [Arduino 範例](/sensors/imu/arduino-examples.md)，每支都能複製上傳。每個值的六平台讀法在 [指令參考](/sensors/imu/reference.md)。

## 基本資訊

| 項目 | 內容 |
| :--- | :--- |
| I2C 位址 | `0x6A`（可用 F8 換） |
| F0 裝置 ID | 低 byte `0x6A`、高 byte `1` |
| 型號 | `IMU` |
| 協議版本 F1 | `2` |
| 速度 | 100 kHz（建議）／400 kHz；寫完編號等 2 ms 再讀 |
| 讀法 | 主機寫 [編號] → 等 2 ms → 讀 2×k byte（k≤16）= 從該編號起連續 k 個 int16 小端；空號回 -32768 |
| 快照 | 每 10 ms 更新，一次讀多個值保證同一瞬間 |

## 接線

<figure class="ref-fig"><img src="/images/figures/imu/wiring-uno.svg" alt="IMU 陀螺儀 × Arduino UNO" loading="lazy"></figure>

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
| [`01`](/sensors/imu/reference.md#v01) | 陀螺 3D 航向 `headingGyro3D` | -32768~32767 ° | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |
| [`02`](/sensors/imu/reference.md#v02) | 融合航向 `headingFused` | -32768~32767 ° | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |
| [`03`](/sensors/imu/reference.md#v03) | 陀螺 1D 航向 `headingGyro1D` | -32768~32767 ° | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |
| [`04`](/sensors/imu/reference.md#v04) | 磁力航向 `headingMag` | -32768~32767 ° | 磁力計傾斜補償航向，度，減去相對零點 |
| [`05`](/sensors/imu/reference.md#v05) | 轉向軸角速度 `yawRate` | -32768~32767 0.1 dps | 繞轉向軸（getYawAxis()）的角速度，0.1 度／秒，正負跟航向同向 |
| [`06`](/sensors/imu/reference.md#v06) | 俯仰角 `pitch` | -90~+90 ° | 板子絲印 X 軸相對水平面的仰角：X 軸朝上 +90、朝下 −90、水平 0（用重力向量 atan2 算） |
| [`07`](/sensors/imu/reference.md#v07) | 俯仰角速度 `pitchRate` | -32768~32767 0.1 dps | 繞板子絲印 X 軸的角速度，0.1 度／秒（右手定則，從 +X 端看逆時針為正）；注意這是 gyro X，不是 06 的微分 |
| [`08`](/sensors/imu/reference.md#v08) | 磁力信任度 `magTrust` | 0~255 | 融合演算法對磁力計的信任度 0~255（= 0~1 × 255） |
| [`09`](/sensors/imu/reference.md#v09) | 磁場干擾度 `magDisturb` | 0~255 0.01° | 磁場干擾程度 0~255（度 × 100，上限 255） |
| [`0A`](/sensors/imu/reference.md#v0a) | 朝上的面 `upFace` | 0~5 | 哪個軸朝上：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z（實體軸） |
| [`10~12`](/sensors/imu/reference.md#v10) | 角速度 1~3 軸 `gyro` | -32768~32767 0.1 dps | 10 X、11 Y、12 Z 角速度，0.1 度／秒（實體軸，已套軸向號誌） |
| [`13~15`](/sensors/imu/reference.md#v13) | 加速度 1~3 軸 `accel` | -32768~32767 mg | 13 X、14 Y、15 Z 加速度，mg（1000 = 1 g） |
| [`16~18`](/sensors/imu/reference.md#v16) | 磁力 1~3 軸 `mag` | -32768~32767 mG | 16 X、17 Y、18 Z 磁場強度，mG（毫高斯） |

## 寫入命令

| 編號 | 命令 | 參數 | 說明 |
| :--- | :--- | :--- | :--- |
| [`E0`](/sensors/imu/reference.md#ce0) | 航向歸零 `zeroHeading` | — | 四個航向都歸零（zeroHeadings()）；EV3 另有原廠 WRITE 0x11 陀螺歸零 |
| [`E1`](/sensors/imu/reference.md#ce1) | 加速度計校準開始 `accelCalStart` | — | 開始加速度計校準（= v1 的 0x20=0x01）；期間 F3 b4 = 1（執行中 F3 bit4 = 1） |
| [`E2`](/sensors/imu/reference.md#ce2) | 加速度計校準結束 `accelCalEnd` | — | 結束加速度計校準並存 EEPROM（= v1 的 0x20=0x02）；通過後自動航向歸零一次 |
| [`E3`](/sensors/imu/reference.md#ce3) | 磁力計校準開始 `magCalStart` | — | 開始磁力計校準（= v1 的 0x20=0x03）；期間 F3 b5 = 1（執行中 F3 bit5 = 1） |
| [`E4`](/sensors/imu/reference.md#ce4) | 磁力計校準結束 `magCalEnd` | — | 結束磁力計校準並存 EEPROM（= v1 的 0x20=0x04） |

## 狀態旗標 F3

| bit | 名稱 | 說明 |
| :--- | :--- | :--- |
| b0 | 就緒 (`ready`) | 恆 1 |
| b1 | 靜止 (`still`) | 1 = 感應器靜止中 |
| b2 | 已擷取安裝軸 (`axisCaptured`) | 1 = 已判定安裝方向（首次擷取後自動歸零一次） |
| b3 | 安裝過歪 (`mountTilted`) | 1 = 安裝角度偏離太多 |
| b4 | 加速度計校準中 (`accelCalibrating`) | E1 之後、E2 之前 = 1 |
| b5 | 磁力計校準中 (`magCalibrating`) | E3 之後、E4 之前 = 1 |
| b6 | 磁力融合已初始化 (`fusionInit`) | 1 = 融合航向可用 |
| b7 | 磁力自動校準已收斂 (`magAutoCalDone`) | 1 = 磁力計自動校準偏移已收斂 |

## 共用暫存器

| 編號 | 名稱 | 說明 |
| :--- | :--- | :--- |
| [`F0`](/i2c-protocol.md#rf0) | 裝置 ID | 低 byte = 出廠 I2C 位址（換位址後不變），高 byte = hw[0]（通道數／顆數／類別） |
| [`F1`](/i2c-protocol.md#rf1) | 協議版本 | 本版 = 2（SPIKE 模式 8 v[1]、EV3 ID 封包同一個數字） |
| [`F2`](/i2c-protocol.md#rf2) | 韌體版本 | 主版×100 + 次版（例 102 = 1.2） |
| [`F3`](/i2c-protocol.md#rf3) | 狀態旗標 | b0 就緒，其餘各產品定義（見各產品 flags）；MATRIX 大通道 byte 24 同一個 byte |
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
const uint8_t ADDR = 0x6A;

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
  Serial.print("headingGyro3D: "); Serial.println(mbcRead(ADDR, 0x01));   // 陀螺 3D 航向
  Serial.print("headingFused: "); Serial.println(mbcRead(ADDR, 0x02));   // 融合航向
  Serial.print("headingGyro1D: "); Serial.println(mbcRead(ADDR, 0x03));   // 陀螺 1D 航向
  Serial.print("headingMag: "); Serial.println(mbcRead(ADDR, 0x04));   // 磁力航向
  Serial.print("yawRate: "); Serial.println(mbcRead(ADDR, 0x05));   // 轉向軸角速度
  Serial.print("pitch: "); Serial.println(mbcRead(ADDR, 0x06));   // 俯仰角
  Serial.print("pitchRate: "); Serial.println(mbcRead(ADDR, 0x07));   // 俯仰角速度
  Serial.print("magTrust: "); Serial.println(mbcRead(ADDR, 0x08));   // 磁力信任度
  Serial.print("magDisturb: "); Serial.println(mbcRead(ADDR, 0x09));   // 磁場干擾度
  Serial.print("upFace: "); Serial.println(mbcRead(ADDR, 0x0A));   // 朝上的面
  { int16_t a[3]; mbcReadMany(ADDR, 0x10, 3, a); Serial.print("gyro: "); for (uint8_t i = 0; i < 3; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  { int16_t a[3]; mbcReadMany(ADDR, 0x13, 3, a); Serial.print("accel: "); for (uint8_t i = 0; i < 3; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  { int16_t a[3]; mbcReadMany(ADDR, 0x16, 3, a); Serial.print("mag: "); for (uint8_t i = 0; i < 3; i++) { Serial.print(a[i]); Serial.print(' '); } Serial.println(); }
  Serial.println();
  delay(200);
}

// 下面貼上「單檔小幫手函式」
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/imu.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 1（共識 #6）；EV3 模式 4 = [0x6A | 1<<8, 韌體主×100+次, 0, 0]（格式 B 再補 2 個 0），模式 5 改送俯仰角速度（取代 #65 放的 ID）。
- EV3 鎖住模式 0／1／3 照盤點程式碼：0 = 陀螺 3D、1 = 融合航向、3 = [陀螺 1D, 磁力航向]（INO:213 註解寫反，以程式為準）。
- 0x22 照 R4 大通道版（出貨版）鎖住：命令 4~9 = 三個航向 +32768 的「低 byte 在前、高 byte 在後」；盤點檔分析的是主韌體（高在前），主韌體 #67 改成跟 R4 版逐 byte 相同。命令 10~12 = 0；0x01、0x0A、0x0F、F1~F4 都觸發航向歸零（刻意，不動）。
- 大通道 [0..7] 四個航向 int16 鎖住；[8..9] 轉向軸角速度、[10..11] 俯仰角、[12..13] 俯仰角速度為 #67 追加；[14..23] 與尾段 24~28 維持 0（教練程式可能讀 ColorID／Cyan 回傳值，不放 F3／ID）。
- SPIKE 不做（hosts.spike = false，使用者先前決定），所有值 spike = []。
- B-13：E1~E4 加速度計／磁力計校準開始／結束補回（v1 的 0x20 = 01/02/03/04），I2C 定案 §3 只列 E0。busyFlag：E1 → b4、E3 → b5。
- 俯仰角範圍定案沒寫（韌體尚無現成值，#67 用 atan2 新算），range 先填「°」不給 min/max，韌體做好後再補。
- 磁力信任度 08 = round(trust×255)，磁場干擾度 09 = round(disturbance×100) 上限 255，朝上的面 0A = 軸號 + 3×負向：公式照盤點 I2C v1 0x34~0x36。
- 角速度 10~12 = round(dps×SIGN×10)、加速度 13~15 = mm/s²×1000/9806.65、磁力 16~18 = raw×SIGN×4/15：公式照盤點 I2C v1 0x22~0x24；mBlock 一併有（I2C 定案 §4 共用值表）。
- 主對話 2026-10-22：06 俯仰角範圍 −90~+90、07 = gyro X（照字面），取自 F5 韌體實作。
- 接線表的腳位以各主機板常見預設為準（ESP32 可用 `Wire.begin(sda, scl)` 改）。
