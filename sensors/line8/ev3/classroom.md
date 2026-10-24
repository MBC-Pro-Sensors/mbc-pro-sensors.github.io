<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × ev3 × classroom），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/ev3/classroom -->
# 循行者 8 路 (Line8) × EV3 × EV3 Classroom

這一頁只講 **EV3 Classroom（Scratch 積木）**：在 MINDSTORMS EV3 上用它讀循行者 8 路 (Line8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-classroom.zh.md + _build/guides/product/line8/ev3-classroom.zh.md {"product":"循行者 8 路 (Line8)","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 8 路 (Line8)：EV3 Classroom（Scratch 積木） 說明](/sensors/line8/ev3-classroom.md)。
<!-- /include-fallback -->

## 讀值

感應器在 EV3 會顯示成樂高原廠感應器，用 Classroom 的原廠感應器積木讀（Classroom 沒有繁中介面，積木名稱用英文）。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 線在哪一路：−8（最左）~+8（最右），0 = 正中間；站在車後往前看，負 = 線在左 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 線位置 | Infrared Sensor → Proximity | 讀到的數 | 模式 0 |

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 細位置 | Infrared Sensor → Remote, channel 1 | 讀到的數 | 模式 2 |

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` 路 | 壓到線的路數 0~8，0 = 找不到線 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 線寬 | Infrared Sensor → Remote, channel 2 | 讀到的數 | 模式 2 |

</section>

<section id="v10" class="ref-item">

### 第 1~8 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="第 1~8 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~8 路校準後光值 0~100（第 1 路最左）；編號 10~17 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 第 1 個 | Infrared Sensor → Beacon heading, channel 1 | 讀到的數 | 模式 1 |
| 第 2 個 | Infrared Sensor → Beacon proximity, channel 1 | 讀到的數 | 模式 1 |
| 第 3 個 | Infrared Sensor → Beacon heading, channel 2 | 讀到的數 | 模式 1 |
| 第 4 個 | Infrared Sensor → Beacon proximity, channel 2 | 讀到的數 | 模式 1 |
| 第 5 個 | Infrared Sensor → Beacon heading, channel 3 | 讀到的數 | 模式 1 |
| 第 6 個 | Infrared Sensor → Beacon proximity, channel 3 | 讀到的數 | 模式 1 |
| 第 7 個 | Infrared Sensor → Beacon heading, channel 4 | 讀到的數 | 模式 1 |
| 第 8 個 | Infrared Sensor → Beacon proximity, channel 4 | 讀到的數 | 模式 1 |

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：線段數、壓線位元圖、最後離線方向、二值化門檻、校準完成次數。要用這些值請改用 <a href="/sensors/line8/ev3/pybricks.md">Pybricks</a>／<a href="/sensors/line8/ev3/clev3r.md">clev3r</a>。</p>

## 命令

EV3 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/line8/ev3-examples.md host=ev3 sw=classroom -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-classroom.zh.md`、`_build/guides/product/line8/ev3-classroom.zh.md`
- 範例：`sensors/line8/ev3-examples.md`
