<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × ev3 × classroom），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/ev3/classroom -->
# 掌控者 PS2 (PS2) × EV3 × EV3 Classroom

這一頁只講 **EV3 Classroom（Scratch 積木）**：在 MINDSTORMS EV3 上用它讀掌控者 PS2 (PS2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-classroom.zh.md + _build/guides/product/ps2/ev3-classroom.zh.md {"product":"掌控者 PS2 (PS2)","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS2 (PS2)：EV3 Classroom（Scratch 積木） 說明](/sensors/ps2/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值

感應器在 EV3 會顯示成樂高原廠感應器，用 Classroom 的原廠感應器積木讀（Classroom 沒有繁中介面，積木名稱用英文）。

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 方向鍵 | Infrared Sensor → Proximity | 讀到的數 | 模式 0 |

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 形狀鍵 | Infrared Sensor → Beacon heading, channel 2 | 讀到的數 | 模式 1 |

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 肩鍵 | Infrared Sensor → Beacon heading, channel 3 | 讀到的數 | 模式 1 |

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 功能鍵 | Infrared Sensor → Beacon heading, channel 4 | 讀到的數 | 模式 1 |

</section>

<section id="v10" class="ref-item">

### 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100`（EV3 模式 1、模式 2、模式 4 讀到的是換算值：0~99（50 = 中間）） | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 第 1 個 | Infrared Sensor → Beacon proximity, channel 1 | 讀到的數 | 模式 1 |
| 第 2 個 | Infrared Sensor → Beacon heading, channel 2 | 讀到的數 | 模式 1 |
| 第 3 個 | Infrared Sensor → Beacon proximity, channel 2 | 讀到的數 | 模式 1 |
| 第 4 個 | Infrared Sensor → Beacon heading, channel 3 | 讀到的數 | 模式 1 |

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：十六鍵、已連線、電量、L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps2/ev3/pybricks.md">Pybricks</a>／<a href="/sensors/ps2/ev3/clev3r.md">clev3r</a>。</p>

## 命令

EV3 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/ps2/ev3-examples.md host=ev3 sw=classroom -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-classroom.zh.md`、`_build/guides/product/ps2/ev3-classroom.zh.md`
- 範例：`sensors/ps2/ev3-examples.md`
