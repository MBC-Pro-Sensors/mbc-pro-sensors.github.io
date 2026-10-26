<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/microbit/makecode -->
# 陀螺儀 IMU 九軸 × micro:bit × MakeCode

這一頁只講 **MakeCode（積木／JavaScript）**：在 BBC micro:bit V1／V2 上用它讀陀螺儀 IMU 九軸的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x6A`。這款耗電小，VCC 可以先接 micro:bit 的 **3V** 腳（V1 約 90 mA、V2 約 190 mA）；用電池供電時電壓可能掉到 3 V 以下，不穩就改外接 5 V（GND 要和 micro:bit 共地）。

<figure class="ref-fig"><img src="/images/figures/imu/wiring-microbit.svg" alt="陀螺儀 IMU 九軸 × BBC micro:bit V1／V2" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/microbit-makecode.zh.md + _build/guides/product/imu/microbit-makecode.zh.md {"product":"陀螺儀 IMU 九軸","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[陀螺儀 IMU 九軸：MakeCode（積木／JavaScript） 說明](/sensors/imu/microbit.md)。
<!-- /include-fallback -->

## 讀值

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC 感應器」分類找積木；JavaScript 寫法一併列出，位址 `0x6A`。

<section id="v01" class="ref-item">

### 陀螺 3D 航向 <small>headingGyro3D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |

<pre class="blocks">(陀螺儀 [陀螺 3D v] 航向（°） :: #00A4E4)</pre>

```typescript
let headingGyro3D = mbc.imuHeading(MbcImuHeading.Gyro3D)   // 陀螺 3D 航向：-32768~32767 °
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v02" class="ref-item">

### 融合航向 <small>headingFused</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |

<pre class="blocks">(陀螺儀 [融合 v] 航向（°） :: #00A4E4)</pre>

```typescript
let headingFused = mbc.imuHeading(MbcImuHeading.Fused)   // 融合航向：-32768~32767 °
```

**相關積木「當陀螺儀 轉了 90 °」**

```typescript
mbc.onImuTurned(90, function () {
})
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v03" class="ref-item">

### 陀螺 1D 航向 <small>headingGyro1D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |

<pre class="blocks">(陀螺儀 [陀螺 1D v] 航向（°） :: #00A4E4)</pre>

```typescript
let headingGyro1D = mbc.imuHeading(MbcImuHeading.Gyro1D)   // 陀螺 1D 航向：-32768~32767 °
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v04" class="ref-item">

### 磁力航向 <small>headingMag</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 磁力計傾斜補償航向，度，減去相對零點 |

<pre class="blocks">(陀螺儀 [磁力計 v] 航向（°） :: #00A4E4)</pre>

```typescript
let headingMag = mbc.imuHeading(MbcImuHeading.Magnetic)   // 磁力航向：-32768~32767 °
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v05" class="ref-item">

### 轉向軸角速度 <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="轉向軸角速度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 繞轉向軸（getYawAxis()）的角速度，0.1 度／秒，正負跟航向同向 |

<pre class="blocks">(陀螺儀 轉向角速度（0.1 °/s） :: #00A4E4)</pre>

```typescript
let yawRate = mbc.imuYawRate()   // 轉向軸角速度：-32768~32767 0.1 dps
```

**相關積木「陀螺儀 ［靜止］？」**

```typescript
let imuFlag = mbc.imuFlag(MbcImuFlag.Still)
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v06" class="ref-item">

### 俯仰角 <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="俯仰角" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-90~+90` ° | 板子絲印 X 軸相對水平面的仰角：X 軸朝上 +90、朝下 −90、水平 0（用重力向量 atan2 算） |

<pre class="blocks">(陀螺儀 俯仰角（°） :: #00A4E4)</pre>

```typescript
let pitch = mbc.imuPitch()   // 俯仰角：-90~+90 °
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v07" class="ref-item">

### 俯仰角速度 <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="俯仰角速度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 繞板子絲印 X 軸的角速度，0.1 度／秒（右手定則，從 +X 端看逆時針為正）；注意這是 gyro X，不是 06 的微分 |

<pre class="blocks">(陀螺儀 俯仰角速度（0.1 °/s） :: #00A4E4)</pre>

```typescript
let pitchRate = mbc.imuPitchRate()   // 俯仰角速度：-32768~32767 0.1 dps
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v08" class="ref-item">

### 磁力信任度 <small>magTrust</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magTrust.svg" alt="磁力信任度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 融合演算法對磁力計的信任度 0~255（= 0~1 × 255） |

<pre class="blocks">(陀螺儀 磁力信任度 :: #00A4E4)</pre>

```typescript
let magTrust = mbc.imuMagTrust()   // 磁力信任度：0~255
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v09" class="ref-item">

### 磁場干擾度 <small>magDisturb</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magDisturb.svg" alt="磁場干擾度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` 0.01° | 磁場干擾程度 0~255（度 × 100，上限 255） |

<pre class="blocks">(陀螺儀 磁場干擾度 :: #00A4E4)</pre>

```typescript
let magDisturb = mbc.imuMagDisturbance()   // 磁場干擾度：0~255 0.01°
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v0a" class="ref-item">

### 朝上的面 <small>upFace</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-upFace.svg" alt="朝上的面" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~5` | 哪個軸朝上：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z（實體軸） |

<pre class="blocks">(陀螺儀 朝上的面 :: #00A4E4)</pre>

```typescript
let upFace = mbc.imuFaceUp()   // 朝上的面：0~5
```

**相關積木「陀螺儀 朝上的面是 ［+Z］？」**

```typescript
let imuFaceUpIs = mbc.imuFaceUpIs(MbcImuFace.PlusZ)
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v10" class="ref-item">

### 角速度 1~3 軸 <small>gyro</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-gyro.svg" alt="角速度 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 10 X、11 Y、12 Z 角速度，0.1 度／秒（實體軸，已套軸向號誌） |

<pre class="blocks">(陀螺儀 [角速度（0.1 °/s） v] [X v] 軸 :: #00A4E4)</pre>

```typescript
let gyro = [mbc.imuAxis(MbcImuSensor.Gyro, MbcAxis.X), mbc.imuAxis(MbcImuSensor.Gyro, MbcAxis.Y), mbc.imuAxis(MbcImuSensor.Gyro, MbcAxis.Z)]   // 角速度 1~3 軸：-32768~32767 0.1 dps
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v13" class="ref-item">

### 加速度 1~3 軸 <small>accel</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-accel.svg" alt="加速度 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` mg | 13 X、14 Y、15 Z 加速度，mg（1000 = 1 g） |

<pre class="blocks">(陀螺儀 [加速度（mg） v] [X v] 軸 :: #00A4E4)</pre>

```typescript
let accel = [mbc.imuAxis(MbcImuSensor.Accel, MbcAxis.X), mbc.imuAxis(MbcImuSensor.Accel, MbcAxis.Y), mbc.imuAxis(MbcImuSensor.Accel, MbcAxis.Z)]   // 加速度 1~3 軸：-32768~32767 mg
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v16" class="ref-item">

### 磁力 1~3 軸 <small>mag</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-mag.svg" alt="磁力 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` mG | 16 X、17 Y、18 Z 磁場強度，mG（毫高斯） |

<pre class="blocks">(陀螺儀 [磁力（mG） v] [X v] 軸 :: #00A4E4)</pre>

```typescript
let mag = [mbc.imuAxis(MbcImuSensor.Mag, MbcAxis.X), mbc.imuAxis(MbcImuSensor.Mag, MbcAxis.Y), mbc.imuAxis(MbcImuSensor.Mag, MbcAxis.Z)]   // 磁力 1~3 軸：-32768~32767 mG
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

## 命令

<section id="ce0" class="ref-item ref-cmd">

### 航向歸零 <small>zeroHeading</small>

四個航向都歸零（zeroHeadings()）；EV3 另有原廠 WRITE 0x11 陀螺歸零

<pre class="blocks">陀螺儀 航向歸零 :: #00A4E4</pre>

```typescript
mbc.imuZeroHeading()   // 航向歸零
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### 加速度計校準開始 <small>accelCalStart</small>

開始加速度計校準（= v1 的 0x20=0x01）；期間 F3 b4 = 1

<pre class="blocks">陀螺儀 校準 [加速度計 開始 v] :: #00A4E4</pre>

```typescript
mbc.imuCalibrate(MbcImuCalib.AccelStart)   // 加速度計校準開始
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### 加速度計校準結束 <small>accelCalEnd</small>

結束加速度計校準並存 EEPROM（= v1 的 0x20=0x02）；通過後自動航向歸零一次

<pre class="blocks">陀螺儀 校準 [加速度計 結束 v] :: #00A4E4</pre>

```typescript
mbc.imuCalibrate(MbcImuCalib.AccelEnd)   // 加速度計校準結束
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### 磁力計校準開始 <small>magCalStart</small>

開始磁力計校準（= v1 的 0x20=0x03）；期間 F3 b5 = 1

<pre class="blocks">陀螺儀 校準 [磁力計 開始 v] :: #00A4E4</pre>

```typescript
mbc.imuCalibrate(MbcImuCalib.MagStart)   // 磁力計校準開始
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### 磁力計校準結束 <small>magCalEnd</small>

結束磁力計校準並存 EEPROM（= v1 的 0x20=0x04）

<pre class="blocks">陀螺儀 校準 [磁力計 結束 v] :: #00A4E4</pre>

```typescript
mbc.imuCalibrate(MbcImuCalib.MagEnd)   // 磁力計校準結束
```

</section>

## 範例

<!-- examples: sensors/imu/microbit-examples.md host=microbit sw=makecode whole -->

## 其他問題

- **找不到 MBC 積木？** MakeCode「擴充功能」貼上 https://github.com/contactmbcrobot/pxt-mbc-sensors 加入。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/imu.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/microbit-makecode.zh.md`、`_build/guides/product/imu/microbit-makecode.zh.md`
- 範例：`sensors/imu/microbit-examples.md`
