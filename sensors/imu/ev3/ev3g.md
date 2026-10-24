<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × ev3 × ev3g），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/ev3/ev3g -->
# 陀螺儀 IMU 九軸 × EV3 × EV3-G 官方軟體

這一頁只講 **EV3-G 官方軟體（EV3 Lab）**：在 MINDSTORMS EV3 上用它讀陀螺儀 IMU 九軸的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-ev3g.zh.md + _build/guides/product/imu/ev3-ev3g.zh.md {"product":"陀螺儀 IMU 九軸","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[陀螺儀 IMU 九軸：EV3-G 官方軟體（EV3 Lab） 說明](/sensors/imu/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值

感應器在 EV3 會顯示成樂高原廠感應器，用原廠感應器積木讀就好。

<section id="v01" class="ref-item">

### 陀螺 3D 航向 <small>headingGyro3D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 陀螺 3D 航向 | 陀螺儀 →「角度」 | 讀到的數 | 模式 0 |

</section>

<section id="v02" class="ref-item">

### 融合航向 <small>headingFused</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 融合航向 | 陀螺儀 →「速率」 | 讀到的數 | 模式 1 |

</section>

<section id="v03" class="ref-item">

### 陀螺 1D 航向 <small>headingGyro1D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 陀螺 1D 航向 | 陀螺儀 →「角度與速率」的角度 | 讀到的數 | 模式 3 |

</section>

<section id="v04" class="ref-item">

### 磁力航向 <small>headingMag</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 磁力計傾斜補償航向，度，減去相對零點 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 磁力航向 | 陀螺儀 →「角度與速率」的速率 | 讀到的數 | 模式 3 |

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：轉向軸角速度、俯仰角、俯仰角速度、磁力信任度、磁場干擾度、朝上的面、角速度 1~3 軸、加速度 1~3 軸、磁力 1~3 軸。要用這些值請改用 <a href="/sensors/imu/ev3/pybricks.md">Pybricks</a>／<a href="/sensors/imu/ev3/clev3r.md">clev3r</a>。</p>

## 命令

EV3 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/imu/ev3-examples.md host=ev3 sw=ev3g -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/imu.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-ev3g.zh.md`、`_build/guides/product/imu/ev3-ev3g.zh.md`
- 範例：`sensors/imu/ev3-examples.md`
