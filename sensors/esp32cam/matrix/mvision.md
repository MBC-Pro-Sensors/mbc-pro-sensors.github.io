<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × matrix × mvision），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/matrix/mvision -->
# 神攝手 ESP32CAM × MATRIX × mVision 積木

這一頁只講 **MATRIXblock mVision 串口積木**：在 MATRIX Mini R4 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **MATRIX 版**，用 MATRIX 原廠 4 pin 感應器線接 Mini R4 的 I2C1 或 I2C2（位址 `0x22`），積木裡選對應的孔。

## 安裝與設定

<!-- setup: _build/guides/software/matrix-mvision.zh.md + _build/guides/product/esp32cam/matrix-mvision.zh.md {"product":"神攝手 ESP32CAM","edition":"MATRIX 版","port":"I2C1","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：MATRIXblock mVision 串口積木 說明](/sensors/esp32cam/matrix.md)。
<!-- /include-fallback -->

## 讀值（學校版）

神攝手在 MATRIX 走串口，用 MATRIXblock 內建的 mVision 攝影機積木讀，不用 I2C。

<section id="v01" class="ref-item">

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

MATRIXblock 的 mVision 積木讀第 0 格資料：0~100；255 = 黑白灰（−1 不能放 uint16，MATRIX 定案 §7）。

</section>

<section id="v02" class="ref-item">

### 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點飽和度 0~100 |

MATRIXblock 的 mVision 積木讀第 1 格資料。

</section>

<section id="v03" class="ref-item">

### 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點明度 0~100 |

MATRIXblock 的 mVision 積木讀第 2 格資料。

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

MATRIXblock 的 mVision 積木讀第 3 格資料。

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

MATRIXblock 的 mVision 積木讀第 4 格資料。

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

MATRIXblock 的 mVision 積木讀第 5 格資料。

</section>

<section id="v07" class="ref-item">

### 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊寬 W 0~100；0 = 沒找到 |

MATRIXblock 的 mVision 積木讀第 6 格資料。

</section>

<section id="v08" class="ref-item">

### 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊高 H 0~75；沒找到 = 0 |

MATRIXblock 的 mVision 積木讀第 7 格資料。

</section>

<section id="v09" class="ref-item">

### 色塊面積 <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="色塊面積" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~7500` | W×H（0~7500）；沒找到 = 0 |

MATRIXblock 的 mVision 積木讀第 8 格資料：W×H。

</section>

<section id="v0a" class="ref-item">

### 有找到色塊 <small>found</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |

MATRIXblock 的 mVision 積木讀第 9 格資料。

</section>

## 讀值（WRO 版）

神攝手在 MATRIX 走串口，用 MATRIXblock 內建的 mVision 攝影機積木讀，不用 I2C。

<section id="w10" class="ref-item">

### 色點 1~12 顏色碼 <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="色點 1~12 顏色碼" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |

MATRIXblock 的 mVision 積木讀第 0~5 格資料：不動：byte 0~11 = S1~S12 顏色碼各 1 byte（idx 0 = S1 低 byte／S2 高 byte … idx 5 = S11／S12）；照 HUE12_AS_mVisionSensor_v1.0.8.mbr4 解碼。

</section>

<section id="w20" class="ref-item">

### 色點 1~12 鮮豔度 <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="色點 1~12 鮮豔度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

MATRIXblock 的 mVision 積木讀第 6~8 格資料：不動：byte 12~17 = 12 格各 4 bit（S>>4，1~15；黑 = 1；沒抓到 = 0），byte 12+2j = S(4j+1) | S(4j+2)<<4、byte 13+2j = S(4j+3) | S(4j+4)<<4。

</section>

## 範例

<p class="no-examples">這個軟體還沒有整支的範例程式。上面「讀值」每一項都附了可以直接照著做的寫法，照順序組起來就能用；需要整支範例請 <a href="/contact.html">聯絡我們</a>。</p>

## 其他問題



還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/matrix-mvision.zh.md`、`_build/guides/product/esp32cam/matrix-mvision.zh.md`
