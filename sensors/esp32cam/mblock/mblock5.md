<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/mblock/mblock5 -->
# 神攝手 ESP32CAM × mBlock × mBlock 5

這一頁只講 **mBlock 5（MBC Pro Sensors 擴展）**：在 MakeBlock（mBot）＋ mBlock 5 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **MakeBlock 版**（RJ25 接頭），插 mBot／Ranger／Orion／MegaPi 的 RJ25 連接埠；各主板可用哪幾個埠見 [MakeBlock 積木擴展](/makeblock.md)。

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-mbot.svg" alt="神攝手 ESP32CAM × MakeBlock（mBot）＋ mBlock 5" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/mblock-mblock5.zh.md + _build/guides/product/esp32cam/mblock-mblock5.zh.md {"product":"神攝手 ESP32CAM","edition":"MakeBlock 版","port":"1","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：mBlock 5（MBC Pro Sensors 擴展） 說明](/sensors/esp32cam/mblock.md)。
<!-- /include-fallback -->

## 讀值（學校版）

裝好 MBC Pro Sensors 擴展後，在「神攝手」分類找積木，連接埠選感應器實際插的孔。

<section id="v01" class="ref-item">

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

<pre class="blocks">(神攝手 連接埠 [1 v] 色相 :: sensing)</pre>

<p class="ref-note">積木傳回 色相（-1~100）。主機送 <code>[01]</code> → 回 <code>[值低][值高][01 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v02" class="ref-item">

### 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點飽和度 0~100 |

<pre class="blocks">(神攝手 連接埠 [1 v] 飽和度 :: sensing)</pre>

<p class="ref-note">積木傳回 飽和度（0~100）。主機送 <code>[02]</code> → 回 <code>[值低][值高][02 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v03" class="ref-item">

### 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點明度 0~100 |

<pre class="blocks">(神攝手 連接埠 [1 v] 明度 :: sensing)</pre>

<p class="ref-note">積木傳回 明度（0~100）。主機送 <code>[03]</code> → 回 <code>[值低][值高][03 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

<pre class="blocks">(神攝手 連接埠 [1 v] 動態 :: sensing)</pre>

<p class="ref-note">積木傳回 動態（0~100）。主機送 <code>[04]</code> → 回 <code>[值低][值高][04 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊 X :: sensing)</pre>

<p class="ref-note">積木傳回 色塊 X（0~100）。主機送 <code>[05]</code> → 回 <code>[值低][值高][05 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊 Y :: sensing)</pre>

<p class="ref-note">積木傳回 色塊 Y（0~75）。主機送 <code>[06]</code> → 回 <code>[值低][值高][06 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v07" class="ref-item">

### 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊寬 W 0~100；0 = 沒找到 |

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊寬 :: sensing)</pre>

<p class="ref-note">積木傳回 色塊寬（0~100）。主機送 <code>[07]</code> → 回 <code>[值低][值高][07 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v08" class="ref-item">

### 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊高 H 0~75；沒找到 = 0 |

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊高 :: sensing)</pre>

<p class="ref-note">積木傳回 色塊高（0~75）。主機送 <code>[08]</code> → 回 <code>[值低][值高][08 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v09" class="ref-item">

### 色塊面積 <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="色塊面積" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~7500` | W×H（0~7500）；沒找到 = 0 |

<pre class="blocks">(神攝手 連接埠 [1 v] 色塊面積 :: sensing)</pre>

<p class="ref-note">積木傳回 色塊面積（0~7500）。主機送 <code>[09]</code> → 回 <code>[值低][值高][09 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v0a" class="ref-item">

### 有找到色塊 <small>found</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |

<pre class="blocks">(神攝手 連接埠 [1 v] 有找到色塊？ :: sensing)</pre>

<p class="ref-note">積木傳回 有找到色塊（0~1）。主機送 <code>[0A]</code> → 回 <code>[值低][值高][0A ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

## 命令

<section id="cx-readall" class="ref-item ref-cmd">

### 更新全部數值 <small>readAll</small>

一次讀整張值表進快取（FD 整包）；接下來 10 ms 內的讀值積木直接用快取，不再逐一詢問

<pre class="blocks">神攝手 連接埠 [1 v] 更新全部數值 :: sensing</pre>

</section>

## 讀值（WRO 版）

裝好 MBC Pro Sensors 擴展後，在「神攝手 WRO」分類找積木，連接埠選感應器實際插的孔。

<section id="w10" class="ref-item">

### 色點 1~12 顏色碼 <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="色點 1~12 顏色碼" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |

<pre class="blocks">(神攝手 WRO 連接埠 [1 v] 色點 [1 v] 顏色碼 :: sensing)</pre>

<p class="ref-note">積木傳回 色點 1~12 顏色碼（0~255）。主機送 <code>[10]</code> → 回 <code>[值低][值高][10 ^ 值低 ^ 值高]</code>，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。</p>

</section>

<section id="w20" class="ref-item">

### 色點 1~12 鮮豔度 <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="色點 1~12 鮮豔度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

<pre class="blocks">(神攝手 WRO 連接埠 [1 v] 色點 [1 v] 鮮豔度 :: sensing)</pre>

<p class="ref-note">積木傳回 色點 1~12 鮮豔度（0~255）。主機送 <code>[20]</code> → 回 <code>[值低][值高][20 ^ 值低 ^ 值高]</code>，值 = int16 小端；第 N 路的編號 = 0x20 + (N − 1)。</p>

</section>

## 命令（WRO 版）

<section id="wcx-readall" class="ref-item ref-cmd">

### 更新全部數值 <small>readAll</small>

一次讀整張值表進快取（FD 整包）；接下來 10 ms 內的讀值積木直接用快取，不再逐一詢問

<pre class="blocks">神攝手 WRO 連接埠 [1 v] 更新全部數值 :: sensing</pre>

</section>

## 範例（學校版）

<!-- examples: sensors/esp32cam/mblock-examples.md host=mblock sw=mblock5 whole -->

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
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/mblock-mblock5.zh.md`、`_build/guides/product/esp32cam/mblock-mblock5.zh.md`
- 範例：`sensors/esp32cam/mblock-examples(-wro).md`
