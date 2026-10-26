<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/mblock/mblock5 -->
# 陀螺儀 IMU 九軸 × mBlock × mBlock 5

這一頁只講 **mBlock 5（MBC Pro Sensors 擴展）**：在 MakeBlock（mBot）＋ mBlock 5 上用它讀陀螺儀 IMU 九軸的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **MakeBlock 版**（RJ25 接頭），插 mBot／Ranger／Orion／MegaPi 的 RJ25 連接埠；各主板可用哪幾個埠見 [MakeBlock 積木擴展](/makeblock.md)。

<figure class="ref-fig"><img src="/images/figures/imu/wiring-mbot.svg" alt="陀螺儀 IMU 九軸 × MakeBlock（mBot）＋ mBlock 5" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/mblock-mblock5.zh.md + _build/guides/product/imu/mblock-mblock5.zh.md {"product":"陀螺儀 IMU 九軸","edition":"MakeBlock 版","port":"1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[陀螺儀 IMU 九軸：mBlock 5（MBC Pro Sensors 擴展） 說明](/sensors/imu/mblock.md)。
<!-- /include-fallback -->

## 讀值

裝好 MBC Pro Sensors 擴展後，在「IMU 陀螺儀」分類找積木，連接埠選感應器實際插的孔。

<section id="v01" class="ref-item">

### 陀螺 3D 航向 <small>headingGyro3D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 陀螺 3D 航向 :: sensing)</pre>

<p class="ref-note">積木傳回 陀螺 3D 航向（-32768~32767 °）。主機送 <code>[01]</code> → 回 <code>[值低][值高][01 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v02" class="ref-item">

### 融合航向 <small>headingFused</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 融合航向 :: sensing)</pre>

<p class="ref-note">積木傳回 融合航向（-32768~32767 °）。主機送 <code>[02]</code> → 回 <code>[值低][值高][02 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v03" class="ref-item">

### 陀螺 1D 航向 <small>headingGyro1D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 陀螺 1D 航向 :: sensing)</pre>

<p class="ref-note">積木傳回 陀螺 1D 航向（-32768~32767 °）。主機送 <code>[03]</code> → 回 <code>[值低][值高][03 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v04" class="ref-item">

### 磁力航向 <small>headingMag</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 磁力計傾斜補償航向，度，減去相對零點 |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 磁力航向 :: sensing)</pre>

<p class="ref-note">積木傳回 磁力航向（-32768~32767 °）。主機送 <code>[04]</code> → 回 <code>[值低][值高][04 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v05" class="ref-item">

### 轉向軸角速度 <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="轉向軸角速度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 繞轉向軸（getYawAxis()）的角速度，0.1 度／秒，正負跟航向同向 |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 轉向角速度 :: sensing)</pre>

<p class="ref-note">積木傳回 轉向軸角速度（-32768~32767 0.1 dps）。主機送 <code>[05]</code> → 回 <code>[值低][值高][05 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v06" class="ref-item">

### 俯仰角 <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="俯仰角" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-90~+90` ° | 板子絲印 X 軸相對水平面的仰角：X 軸朝上 +90、朝下 −90、水平 0（用重力向量 atan2 算） |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 俯仰角 :: sensing)</pre>

<p class="ref-note">積木傳回 俯仰角（-90~+90 °）。主機送 <code>[06]</code> → 回 <code>[值低][值高][06 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v07" class="ref-item">

### 俯仰角速度 <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="俯仰角速度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 繞板子絲印 X 軸的角速度，0.1 度／秒（右手定則，從 +X 端看逆時針為正）；注意這是 gyro X，不是 06 的微分 |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 俯仰角速度 :: sensing)</pre>

<p class="ref-note">積木傳回 俯仰角速度（-32768~32767 0.1 dps）。主機送 <code>[07]</code> → 回 <code>[值低][值高][07 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v08" class="ref-item">

### 磁力信任度 <small>magTrust</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magTrust.svg" alt="磁力信任度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 融合演算法對磁力計的信任度 0~255（= 0~1 × 255） |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 磁力信任度 :: sensing)</pre>

<p class="ref-note">積木傳回 磁力信任度（0~255）。主機送 <code>[08]</code> → 回 <code>[值低][值高][08 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v09" class="ref-item">

### 磁場干擾度 <small>magDisturb</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-magDisturb.svg" alt="磁場干擾度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` 0.01° | 磁場干擾程度 0~255（度 × 100，上限 255） |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 磁場干擾度 :: sensing)</pre>

<p class="ref-note">積木傳回 磁場干擾度（0~255 0.01°）。主機送 <code>[09]</code> → 回 <code>[值低][值高][09 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v0a" class="ref-item">

### 朝上的面 <small>upFace</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-upFace.svg" alt="朝上的面" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~5` | 哪個軸朝上：0 +X、1 +Y、2 +Z、3 −X、4 −Y、5 −Z（實體軸） |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 朝上的面 :: sensing)</pre>

<p class="ref-note">積木傳回 朝上的面（0~5）。主機送 <code>[0A]</code> → 回 <code>[值低][值高][0A ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v10" class="ref-item">

### 角速度 1~3 軸 <small>gyro</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-gyro.svg" alt="角速度 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 10 X、11 Y、12 Z 角速度，0.1 度／秒（實體軸，已套軸向號誌） |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 角速度 [軸] :: sensing)</pre>

<p class="ref-note">積木傳回 角速度 1~3 軸（-32768~32767 0.1 dps）。主機送 <code>[10]</code> → 回 <code>[值低][值高][10 ^ 值低 ^ 值高]</code>，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。</p>

</section>

<section id="v13" class="ref-item">

### 加速度 1~3 軸 <small>accel</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-accel.svg" alt="加速度 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` mg | 13 X、14 Y、15 Z 加速度，mg（1000 = 1 g） |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 加速度 [軸] :: sensing)</pre>

<p class="ref-note">積木傳回 加速度 1~3 軸（-32768~32767 mg）。主機送 <code>[13]</code> → 回 <code>[值低][值高][13 ^ 值低 ^ 值高]</code>，值 = int16 小端；第 N 路的編號 = 0x13 + (N − 1)。</p>

</section>

<section id="v16" class="ref-item">

### 磁力 1~3 軸 <small>mag</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-mag.svg" alt="磁力 1~3 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` mG | 16 X、17 Y、18 Z 磁場強度，mG（毫高斯） |

<pre class="blocks">(IMU 陀螺儀 連接埠 [1 v] 磁力 [軸] :: sensing)</pre>

<p class="ref-note">積木傳回 磁力 1~3 軸（-32768~32767 mG）。主機送 <code>[16]</code> → 回 <code>[值低][值高][16 ^ 值低 ^ 值高]</code>，值 = int16 小端；第 N 路的編號 = 0x16 + (N − 1)。</p>

</section>

## 命令

<section id="ce0" class="ref-item ref-cmd">

### 航向歸零 <small>zeroHeading</small>

四個航向都歸零（zeroHeadings()）；EV3 另有原廠 WRITE 0x11 陀螺歸零

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 航向歸零 :: sensing</pre>

<p class="ref-note">主機送 <code>[E0]</code> → 感應器回 <code>[00][00][E0]</code>（收到了）再執行。</p>

</section>

<section id="ce1" class="ref-item ref-cmd">

### 加速度計校準開始 <small>accelCalStart</small>

開始加速度計校準（= v1 的 0x20=0x01）；期間 F3 b4 = 1

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 加速度計校準開始 :: sensing</pre>

<p class="ref-note">主機送 <code>[E1]</code> → 感應器回 <code>[00][00][E1]</code>（收到了）再執行；執行期間 F3 bit4 = 1。</p>

</section>

<section id="ce2" class="ref-item ref-cmd">

### 加速度計校準結束 <small>accelCalEnd</small>

結束加速度計校準並存 EEPROM（= v1 的 0x20=0x02）；通過後自動航向歸零一次

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 加速度計校準結束 :: sensing</pre>

<p class="ref-note">主機送 <code>[E2]</code> → 感應器回 <code>[00][00][E2]</code>（收到了）再執行。</p>

</section>

<section id="ce3" class="ref-item ref-cmd">

### 磁力計校準開始 <small>magCalStart</small>

開始磁力計校準（= v1 的 0x20=0x03）；期間 F3 b5 = 1

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 磁力計校準開始 :: sensing</pre>

<p class="ref-note">主機送 <code>[E3]</code> → 感應器回 <code>[00][00][E3]</code>（收到了）再執行；執行期間 F3 bit5 = 1。</p>

</section>

<section id="ce4" class="ref-item ref-cmd">

### 磁力計校準結束 <small>magCalEnd</small>

結束磁力計校準並存 EEPROM（= v1 的 0x20=0x04）

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 磁力計校準結束 :: sensing</pre>

<p class="ref-note">主機送 <code>[E4]</code> → 感應器回 <code>[00][00][E4]</code>（收到了）再執行。</p>

</section>

<section id="cx-readall" class="ref-item ref-cmd">

### 更新全部數值 <small>readAll</small>

一次讀整張值表進快取（FD 整包）；接下來 10 ms 內的讀值積木直接用快取，不再逐一詢問

<pre class="blocks">IMU 陀螺儀 連接埠 [1 v] 更新全部數值 :: sensing</pre>

</section>

## 範例

<!-- examples: sensors/imu/mblock-examples.md host=mblock sw=mblock5 whole -->

## 其他問題

- **積木一直回 0？** 確認積木選的連接埠跟實際插的一樣，並且已經用「上傳」模式把程式燒進主板。
- **舊的擴展還能用嗎？** 2026-10 以前出貨的感應器用舊版擴展 v1.0.0；新舊積木不相容，依韌體版本選。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext)
- [MBC_Pro_Sensors_mBlock5_v1.0.0.mext（舊版，2026-10 以前出貨的感應器）](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/imu.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/mblock-mblock5.zh.md`、`_build/guides/product/imu/mblock-mblock5.zh.md`
- 範例：`sensors/imu/mblock-examples.md`
