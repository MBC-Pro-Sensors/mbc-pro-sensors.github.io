<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × matrix × matrixblock），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/matrix/matrixblock -->
# 陀螺儀 IMU 九軸 × MATRIX × MATRIXblock 原廠積木

這一頁只講 **MATRIXblock 原廠顏色感應器積木（0x22）**：在 MATRIX Mini R4 上用它讀陀螺儀 IMU 九軸的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/matrix-matrixblock.zh.md + _build/guides/product/imu/matrix-matrixblock.zh.md {"product":"陀螺儀 IMU 九軸","edition":"MATRIX 版","port":"I2C1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[陀螺儀 IMU 九軸：MATRIXblock 原廠顏色感應器積木（0x22） 說明](/sensors/imu/matrix.md)。
<!-- /include-fallback -->

## 讀值

用 MATRIXblock 原廠「顏色感應器」積木的 R／G／B／C／M／Y／K 讀（不要用 H／S／V 與顏色辨識積木）；Arduino 模式寫法一併列出。

<section id="v01" class="ref-item">

### 陀螺 3D 航向 <small>headingGyro3D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |

積木「顏色感應器 → R~G」：R = (航向+32768) 低 byte、G = 高 byte（R4 大通道版：低在前）；主機 hi×256+lo−32768

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(R);   // 第 1 組
```

</section>

<section id="v02" class="ref-item">

### 融合航向 <small>headingFused</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |

積木「顏色感應器 → B~C」：B = 低 byte、C = 高 byte

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(B);   // 第 1 組
```

</section>

<section id="v03" class="ref-item">

### 陀螺 1D 航向 <small>headingGyro1D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |

積木「顏色感應器 → M~Y」：M = 低 byte、Y = 高 byte；K（10）= 0

```cpp
int raw = MiniR4.I2C1.MXColor.getColor(M);   // 第 1 組
```

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：磁力航向、轉向軸角速度、俯仰角、俯仰角速度、磁力信任度、磁場干擾度、朝上的面、角速度 1~3 軸、加速度 1~3 軸、磁力 1~3 軸。要用這些值請改用 <a href="/sensors/imu/matrix/bigchannel.md">大通道函式庫</a>。</p>

## 命令

這個軟體不能送命令；需要時改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<p class="no-examples">這個軟體還沒有整支的範例程式。上面「讀值」和「命令」每一項都附了可以直接照著做的寫法，照順序組起來就能用；需要整支範例請 <a href="/contact.html">聯絡我們</a>。</p>

## 其他問題

- **原廠積木和大通道可以一起用嗎？** 不行：兩種要裝不同的函式庫檔，一個程式只能選一種。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/imu.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/matrix-matrixblock.zh.md`、`_build/guides/product/imu/matrix-matrixblock.zh.md`
