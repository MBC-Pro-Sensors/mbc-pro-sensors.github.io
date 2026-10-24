<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-reference.mjs 從 _build/protocol/imu.json 產生（microbit），請勿手改；改內容請改 JSON 後重跑 node _build/gen-reference.mjs -->
<!-- moved: /sensors/imu/microbit/makecode.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/imu/microbit/makecode.md">陀螺儀 IMU 九軸 › micro:bit › MakeCode</a></p>

# IMU 陀螺儀 × micro:bit（MakeCode 擴充）

[🔙 回到IMU 陀螺儀產品頁](/sensors/imu/index.md)

IMU 陀螺儀通用 I2C 版可以接 BBC micro:bit V1／V2，用 MakeCode 擴充 **pxt-mbc-sensors v2** 的積木讀值（內部走 [MBC I2C 協議 v2](/i2c-protocol.md)）。擴充安裝、硬體注意事項（3.3 V、供電）請看 [micro:bit 總覽](/microbit.md)。

<p class="chips"><span class="chip chip-spike off">SPIKE</span><a class="chip chip-ev3" href="/sensors/imu/index.md">EV3</a><a class="chip chip-arduino" href="/sensors/imu/arduino-i2c.md">Arduino I2C</a><a class="chip chip-mblock" href="/sensors/imu/mblock.md">mBlock</a><a class="chip chip-matrix" href="/sensors/imu/matrix.md">MATRIX</a><a class="chip chip-microbit current" href="/sensors/imu/microbit.md">micro:bit</a></p>

> [!IMPORTANT]
> 擴充 v2 尚未上機驗證；積木名稱以發布版為準： <https://github.com/contactmbcrobot/pxt-mbc-sensors>

## 接線

<figure class="ref-fig"><img src="/images/figures/imu/wiring-microbit.svg" alt="IMU 陀螺儀 × micro:bit" loading="lazy"></figure>

| 感應器 | micro:bit |
| :--- | :--- |
| SDA | P20 |
| SCL | P19 |
| 5V | 外接 5 V（或 V2 的 3V 腳，需確認電流） |
| GND | GND |

## 讀值積木

| 積木 | 傳回 | 編號 | 說明 |
| :--- | :--- | :--- | :--- |
| MBC 陀螺 3D 航向 | -32768~32767 ° | [`01`](/sensors/imu/reference.md#v01) | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |
| MBC 融合航向 | -32768~32767 ° | [`02`](/sensors/imu/reference.md#v02) | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |
| MBC 陀螺 1D 航向 | -32768~32767 ° | [`03`](/sensors/imu/reference.md#v03) | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |
| MBC 磁力航向 | -32768~32767 ° | [`04`](/sensors/imu/reference.md#v04) | 磁力計傾斜補償航向，度，減去相對零點 |
| MBC 轉向角速度 | -32768~32767 0.1 dps | [`05`](/sensors/imu/reference.md#v05) | 繞轉向軸（getYawAxis()）的角速度，0.1 度／秒，正負跟航向同向 |
| MBC 俯仰角 | -90~+90 ° | [`06`](/sensors/imu/reference.md#v06) | 板子絲印 X 軸相對水平面的仰角：X 軸朝上 +90、朝下 −90、水平 0（用重力向量 atan2 算） |
| MBC 俯仰角速度 | -32768~32767 0.1 dps | [`07`](/sensors/imu/reference.md#v07) | 繞板子絲印 X 軸的角速度，0.1 度／秒（右手定則，從 +X 端看逆時針為正）；注意這是 gyro X，不是 06 的微分 |
| MBC 磁力信任度 | 0~255 | [`08`](/sensors/imu/reference.md#v08) | 融合演算法對磁力計的信任度 0~255（= 0~1 × 255） |
| MBC 磁場干擾度 | 0~255 0.01° | [`09`](/sensors/imu/reference.md#v09) | 磁場干擾程度 0~255（度 × 100，上限 255） |
| MBC 朝上的面 | 0~5 | [`0A`](/sensors/imu/reference.md#v0a) | 哪個軸朝上：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z（實體軸） |
| MBC 角速度 [軸] | -32768~32767 0.1 dps | [`10~12`](/sensors/imu/reference.md#v10) | 10 X、11 Y、12 Z 角速度，0.1 度／秒（實體軸，已套軸向號誌） |
| MBC 加速度 [軸] | -32768~32767 mg | [`13~15`](/sensors/imu/reference.md#v13) | 13 X、14 Y、15 Z 加速度，mg（1000 = 1 g） |
| MBC 磁力 [軸] | -32768~32767 mG | [`16~18`](/sensors/imu/reference.md#v16) | 16 X、17 Y、18 Z 磁場強度，mG（毫高斯） |

## 命令積木

| 積木 | 編號 | 說明 |
| :--- | :--- | :--- |
| MBC 航向歸零 | [`E0`](/sensors/imu/reference.md#ce0) | 四個航向都歸零（zeroHeadings()）；EV3 另有原廠 WRITE 0x11 陀螺歸零 |
| MBC 加速度計校準開始 | [`E1`](/sensors/imu/reference.md#ce1) | 開始加速度計校準（= v1 的 0x20=0x01）；期間 F3 b4 = 1 |
| MBC 加速度計校準結束 | [`E2`](/sensors/imu/reference.md#ce2) | 結束加速度計校準並存 EEPROM（= v1 的 0x20=0x02）；通過後自動航向歸零一次 |
| MBC 磁力計校準開始 | [`E3`](/sensors/imu/reference.md#ce3) | 開始磁力計校準（= v1 的 0x20=0x03）；期間 F3 b5 = 1 |
| MBC 磁力計校準結束 | [`E4`](/sensors/imu/reference.md#ce4) | 結束磁力計校準並存 EEPROM（= v1 的 0x20=0x04） |

## 狀態

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

## JavaScript 寫法

```typescript
basic.forever(function () {
    let headingGyro3D = MBC.readValue(0x6A, 0x01)   // 陀螺 3D 航向
    basic.showNumber(headingGyro3D)
    basic.pause(100)
})
```

## 本頁依據與假設

本頁由 `_build/gen-reference.mjs` 從 `_build/protocol/imu.json`（單一真相來源，轉錄自 2026-10-21 協定定案）產生。

- hw0 = 1（共識 #6）；EV3 模式 4 = [0x6A | 1<<8, 韌體主×100+次, 0, 0]（格式 B 再補 2 個 0），模式 5 改送俯仰角速度（取代 #65 放的 ID）。
- EV3 鎖住模式 0／1／3 照盤點程式碼：0 = 陀螺 3D、1 = 融合航向、3 = [陀螺 1D, 磁力航向]（INO:213 註解寫反，以程式為準）。
- 0x22 照 R4 版（出貨版）鎖住：命令 4~9 = 三個航向 +32768 的「低 byte 在前、高 byte 在後」；盤點檔分析的是主韌體（高在前），主韌體 #67 改成跟 R4 版逐 byte 相同。命令 10~12 = 0；0x01、0x0A、0x0F、F1~F4 都觸發航向歸零（刻意，不動）。
- SPIKE 不做（hosts.spike = false，使用者先前決定），所有值 spike = []。
- B-13：E1~E4 加速度計／磁力計校準開始／結束補回（v1 的 0x20 = 01/02/03/04），I2C 定案 §3 只列 E0。busyFlag：E1 → b4、E3 → b5。
- 俯仰角範圍定案沒寫（韌體尚無現成值，#67 用 atan2 新算），range 先填「°」不給 min/max，韌體做好後再補。
- 磁力信任度 08 = round(trust×255)，磁場干擾度 09 = round(disturbance×100) 上限 255，朝上的面 0A = 軸號 + 3×負向：公式照盤點 I2C v1 0x34~0x36。
- 角速度 10~12 = round(dps×SIGN×10)、加速度 13~15 = mm/s²×1000/9806.65、磁力 16~18 = raw×SIGN×4/15：公式照盤點 I2C v1 0x22~0x24；mBlock 一併有（I2C 定案 §4 共用值表）。
- 主對話 2026-10-22：06 俯仰角範圍 −90~+90、07 = gyro X（照字面），取自 F5 韌體實作。
- 擴充內部用 `Int16LE` 讀、寫完編號 `basic.pause(2)`（D5）。
