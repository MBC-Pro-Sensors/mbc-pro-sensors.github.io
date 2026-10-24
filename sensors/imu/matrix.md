<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/imu.json 產生（matrix），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/imu/matrix/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/imu/matrix/index.md">陀螺儀 IMU 九軸 › MATRIX</a></p>

# IMU 陀螺儀 × MATRIX Mini R4

[🔙 回到IMU 陀螺儀產品頁](/sensors/imu/index.md)

IMU 陀螺儀 MATRIX 版接在 Mini R4 的 I2C 孔，位址 `0x22`，有兩種用法：**A. 原廠顏色感應器積木**（不用換函式庫，R~K 七個 0~255 的數）與 **B. LauMo 大通道**（換成大通道函式庫，一包 30 byte 什麼都有）。兩種要裝不同的函式庫檔，不能同時用。

<p class="chips"><span class="chip chip-spike off">SPIKE</span><a class="chip chip-ev3" href="/sensors/imu/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/imu/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/imu/mblock.md">mBlock</a><a class="chip chip-matrix current" href="/sensors/imu/matrix.md">MATRIX</a><a class="chip chip-microbit" href="/sensors/imu/microbit.md">micro:bit</a></p>

## 接線

<figure class="ref-fig"><img src="/images/figures/imu/wiring-matrix.svg" alt="IMU 陀螺儀 × MATRIX Mini R4" loading="lazy"></figure>

用 MATRIX 原廠 4 pin 感應器線接 I2C1 或 I2C2，積木裡選對應的孔。

## A. 原廠積木（0x22）

積木「顏色感應器 → R／G／B／C／M／Y／K」各讀 1 個 0~255 的數。⚠️ 不要用原廠的 H／S／V、顏色辨識積木（那是主機拿 R、G、B 自己算的，對本產品沒有意義）；灰階、顏色編號積木已被 MATRIX 拿掉。

| 積木 | 內容 | 解法 |
| :--- | :--- | :--- |
| R | [陀螺 3D 航向](/sensors/imu/reference.md#v01) 第 1~2 路 | R = (航向+32768) 低 byte、G = 高 byte（R4 大通道版：低在前）；主機 hi×256+lo−32768 |
| G | [陀螺 3D 航向](/sensors/imu/reference.md#v01) 第 3~4 路 | R = (航向+32768) 低 byte、G = 高 byte（R4 大通道版：低在前）；主機 hi×256+lo−32768 |
| B | [融合航向](/sensors/imu/reference.md#v02) 第 1~2 路 | B = 低 byte、C = 高 byte |
| C | [融合航向](/sensors/imu/reference.md#v02) 第 3~4 路 | B = 低 byte、C = 高 byte |
| M | [陀螺 1D 航向](/sensors/imu/reference.md#v03) 第 1~2 路 | M = 低 byte、Y = 高 byte；K（10）= 0 |
| Y | [陀螺 1D 航向](/sensors/imu/reference.md#v03) 第 3~4 路 | M = 低 byte、Y = 高 byte；K（10）= 0 |

```cpp
// MATRIXblock → Arduino：原廠函式庫
int r = MiniR4.I2C1.MXColor.getColor(R);   // 陀螺 3D 航向 第 1~2 路
```

## B. 大通道（LauMo 函式庫）

函式庫 `MiniR4ColorSensorExt大通道版`（[下載](/downloads.md)）換掉原廠檔後，同一組積木變成「指標讀法」：ColorID 把指標歸零並回傳狀態、Blue 讀 1 byte、Green 讀 1 byte 有正負、Saturation 讀 2 byte、Hue 讀 2 byte 有正負、Value 讀 4 byte 小數、Red 把指標往後移「上一個讀取的大小」。

<figure class="ref-fig"><img src="/images/figures/common/packet-big30.svg" alt="LauMo 30-byte packet" loading="lazy"></figure>

| byte | 積木 | 內容 | 備註 |
| :--- | :--- | :--- | :--- |
| 0~1 | Hue | [陀螺 3D 航向](/sensors/imu/reference.md#v01) | 鎖住（R4 大通道版已流出） |
| 2~3 | Hue | [融合航向](/sensors/imu/reference.md#v02) | 鎖住 |
| 4~5 | Hue | [陀螺 1D 航向](/sensors/imu/reference.md#v03) | 鎖住 |
| 6~7 | Hue | [磁力航向](/sensors/imu/reference.md#v04) | 鎖住 |
| 8~9 | Hue | [轉向軸角速度](/sensors/imu/reference.md#v05) | #67 追加（Hue 積木） |
| 10~11 | Hue | [俯仰角](/sensors/imu/reference.md#v06) | #67 追加 |
| 12~13 | Hue | [俯仰角速度](/sensors/imu/reference.md#v07) | #67 追加 |
| 24~28 | ColorID / C / M / Y / K | 維持 0（已流出格式鎖住） |  |
| 29 | — | CRC-8 | 多項式 0x07，涵蓋 0~28；函式庫自己檢查 |

**動作**：undefined = 航向歸零

### Arduino 寫法（MATRIXblock 的 Arduino 模式）

```cpp
// 大通道：讀第 n 個 byte（uint8）／第 n 個起的 2 byte（uint16 小端）
uint8_t bigU8(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();                 // 指標回到 byte 0
  uint8_t v = MiniR4.I2C1.MXColor.getColor(B);      // 讀 byte 0
  for (uint8_t i = 0; i < n; i++) { MiniR4.I2C1.MXColor.getColor(R); v = MiniR4.I2C1.MXColor.getColor(B); }   // Red = 前進 1
  return v;
}
uint16_t bigU16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (uint16_t)MiniR4.I2C1.MXColor.getS();      // Saturation = 讀 2 byte
}
int16_t bigI16(uint8_t n) {
  MiniR4.I2C1.MXColor.getColorID();
  MiniR4.I2C1.MXColor.getColor(B);
  for (uint8_t i = 0; i < n; i++) MiniR4.I2C1.MXColor.getColor(R);
  return (int16_t)MiniR4.I2C1.MXColor.getH();       // Hue = 讀 2 byte 有正負
}
```

```cpp
int headingGyro3D = bigU8(0);   // 陀螺 3D 航向
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
- 函式庫規則（命令 1 回 0x43、CRC-8、最快 5 ms 讀一次）以 `80_上位機研究\MatrixMiniR4\MiniR4ColorSensorExt大通道版` 為準。
