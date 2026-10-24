<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × ev3 × classroom），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/ev3/classroom -->
# 神攝手 ESP32CAM × EV3 × EV3 Classroom

這一頁只講 **EV3 Classroom（Scratch 積木）**：在 MINDSTORMS EV3 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-classroom.zh.md + _build/guides/product/esp32cam/ev3-classroom.zh.md {"product":"神攝手 ESP32CAM","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：EV3 Classroom（Scratch 積木） 說明](/sensors/esp32cam/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值（學校版）

感應器在 EV3 會顯示成樂高原廠感應器，用 Classroom 的原廠感應器積木讀（Classroom 沒有繁中介面，積木名稱用英文）。

<section id="v01" class="ref-item">

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 色相 | Infrared Sensor → Proximity | 讀到的數 | 模式 0 |

</section>

<section id="v02" class="ref-item">

### 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點飽和度 0~100 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 飽和度 | Infrared Sensor → Beacon proximity, channel 1 | 讀到的數 | 模式 1 |

</section>

<section id="v03" class="ref-item">

### 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點明度 0~100 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 明度 | Infrared Sensor → Beacon heading, channel 2 | 讀到的數 | 模式 1 |

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 動態 | Infrared Sensor → Beacon proximity, channel 2 | 讀到的數 | 模式 1 |

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 色塊 X | Infrared Sensor → Beacon heading, channel 3 | 讀到的數 | 模式 1 |

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 色塊 Y | Infrared Sensor → Beacon proximity, channel 3 | 讀到的數 | 模式 1 |

</section>

<section id="v07" class="ref-item">

### 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊寬 W 0~100；0 = 沒找到 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 色塊寬 | Infrared Sensor → Beacon heading, channel 4 | 讀到的數 | 模式 1 |

</section>

<section id="v08" class="ref-item">

### 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊高 H 0~75；沒找到 = 0 |

| 值 | 積木 | 換算 | 模式 |
| :--- | :--- | :--- | :--- |
| 色塊高 | Infrared Sensor → Beacon proximity, channel 4 | 讀到的數 | 模式 1 |

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：色塊面積、有找到色塊。要用這些值請改用 <a href="/sensors/esp32cam/arduino/ide.md">Arduino</a>／<a href="/sensors/esp32cam/mblock/mblock5.md">mBlock</a>／<a href="/sensors/esp32cam/microbit/makecode.md">micro:bit</a>。</p>

## 讀值（WRO 版）

感應器在 EV3 會顯示成樂高原廠感應器，用 Classroom 的原廠感應器積木讀（Classroom 沒有繁中介面，積木名稱用英文）。

<section id="w10" class="ref-item">

### 色點 1~12 顏色碼 <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="色點 1~12 顏色碼" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |

這款不是模擬樂高原廠感應器，原廠積木讀不到；請用範例裡附的 MBC 自訂積木（見上方安裝與設定）。

</section>

<section id="w20" class="ref-item">

### 色點 1~12 鮮豔度 <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="色點 1~12 鮮豔度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

這款不是模擬樂高原廠感應器，原廠積木讀不到；請用範例裡附的 MBC 自訂積木（見上方安裝與設定）。

</section>

## 範例（學校版）

<!-- examples: sensors/esp32cam/ev3-examples.md host=ev3 sw=classroom -->

## 範例（WRO 版）

<!-- examples: sensors/esp32cam/ev3-examples-wro.md host=ev3 sw=classroom -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-classroom.zh.md`、`_build/guides/product/esp32cam/ev3-classroom.zh.md`
- 範例：`sensors/esp32cam/ev3-examples(-wro).md`
