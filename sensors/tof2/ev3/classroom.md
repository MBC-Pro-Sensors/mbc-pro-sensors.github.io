<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × ev3 × classroom），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/ev3/classroom -->
# 測距者 2 路 (TOF2) × EV3 × EV3 Classroom

這一頁只講 **EV3 Classroom（Scratch 積木）**：在 MINDSTORMS EV3 上用它讀測距者 2 路 (TOF2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-classroom.zh.md + _build/guides/product/tof2/ev3-classroom.zh.md {"product":"測距者 2 路 (TOF2)","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 2 路 (TOF2)：EV3 Classroom（Scratch 積木） 說明](/sensors/tof2/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值

感應器在 EV3 會顯示成樂高原廠感應器，用 Classroom 的原廠感應器積木讀（Classroom 沒有繁中介面，積木名稱用英文）。

<section id="v01" class="ref-item">

### 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm（EV3 模式 1、模式 2、模式 4 讀到的是換算值：cm×100/120（0~100）） | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 左距離 cm | Infrared Sensor → Beacon heading, channel 1 | 讀到的數 | 模式 1 |

</section>

<section id="v02" class="ref-item">

### 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm（EV3 模式 1、模式 2、模式 4 讀到的是換算值：cm×100/120（0~100）） | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 右距離 cm | Infrared Sensor → Beacon proximity, channel 1 | 讀到的數 | 模式 1 |

</section>

<section id="v08" class="ref-item">

### 左右平均 mm <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="左右平均 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm（EV3 模式 0 讀到的是換算值：cm 平均 ×100/120（0~100）） | (左 mm + 右 mm) / 2，0~1200；跟 EV3 模式 3、SPIKE 模式 6 第 3 個值一致（共識 #8 補上） |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 左右平均 mm | Infrared Sensor → Proximity | 讀到的數 | 模式 0 |

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：較近距離 cm、左距離 mm、右距離 mm、初始化位元圖、故障位元圖。要用這些值請改用 <a href="/sensors/tof2/ev3/pybricks.md">Pybricks</a>／<a href="/sensors/tof2/ev3/clev3r.md">clev3r</a>。</p>

## 範例

<!-- examples: sensors/tof2/ev3-examples.md host=ev3 sw=classroom -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-classroom.zh.md`、`_build/guides/product/tof2/ev3-classroom.zh.md`
- 範例：`sensors/tof2/ev3-examples.md`
