<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/mblock/mblock5 -->
# 測距者 2 路 (TOF2) × mBlock × mBlock 5

這一頁只講 **mBlock 5（MBC Pro Sensors 擴展）**：在 MakeBlock（mBot）＋ mBlock 5 上用它讀測距者 2 路 (TOF2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **MakeBlock 版**（RJ25 接頭），插 mBot／Ranger／Orion／MegaPi 的 RJ25 連接埠；各主板可用哪幾個埠見 [MakeBlock 積木擴展](/makeblock.md)。

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-mbot.svg" alt="測距者 2 路 (TOF2) × MakeBlock（mBot）＋ mBlock 5" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/mblock-mblock5.zh.md + _build/guides/product/tof2/mblock-mblock5.zh.md {"product":"測距者 2 路 (TOF2)","edition":"MakeBlock 版","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 2 路 (TOF2)：mBlock 5（MBC Pro Sensors 擴展） 說明](/sensors/tof2/mblock.md)。
<!-- /include-fallback -->

## 讀值

裝好 MBC Pro Sensors 擴展後，在「測距者 2 路」分類找積木，連接埠選感應器實際插的孔。

<section id="v01" class="ref-item">

### 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 左距離 cm :: sensing)</pre>

<p class="ref-note">積木傳回 左距離 cm（0~120 cm）。主機送 <code>[01]</code> → 回 <code>[值低][值高][01 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v02" class="ref-item">

### 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 右距離 cm :: sensing)</pre>

<p class="ref-note">積木傳回 右距離 cm（0~120 cm）。主機送 <code>[02]</code> → 回 <code>[值低][值高][02 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v03" class="ref-item">

### 較近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="較近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左右兩顆取較近的 0~120 cm |

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 較近距離 cm :: sensing)</pre>

<p class="ref-note">積木傳回 較近距離 cm（0~120 cm）。主機送 <code>[03]</code> → 回 <code>[值低][值高][03 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v04" class="ref-item">

### 左距離 mm <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="左距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 左邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 左距離 mm :: sensing)</pre>

<p class="ref-note">積木傳回 左距離 mm（0~1200 mm）。主機送 <code>[04]</code> → 回 <code>[值低][值高][04 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v05" class="ref-item">

### 右距離 mm <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="右距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 右邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 右距離 mm :: sensing)</pre>

<p class="ref-note">積木傳回 右距離 mm（0~1200 mm）。主機送 <code>[05]</code> → 回 <code>[值低][值高][05 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v06" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不重試）；3 = 兩顆都好 |

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 初始化位元圖 :: sensing)</pre>

<p class="ref-note">積木傳回 初始化位元圖（0~3 bit）。主機送 <code>[06]</code> → 回 <code>[值低][值高][06 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v07" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 故障位元圖 :: sensing)</pre>

<p class="ref-note">積木傳回 故障位元圖（0~3 bit）。主機送 <code>[07]</code> → 回 <code>[值低][值高][07 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v08" class="ref-item">

### 左右平均 mm <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="左右平均 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | (左 mm + 右 mm) / 2，0~1200；跟 EV3 模式 3、SPIKE 模式 6 第 3 個值一致（共識 #8 補上） |

<pre class="blocks">(測距者 2 路 連接埠 [1 v] 左右平均 mm :: sensing)</pre>

<p class="ref-note">積木傳回 左右平均 mm（0~1200 mm）。主機送 <code>[08]</code> → 回 <code>[值低][值高][08 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

## 命令

<section id="cx-readall" class="ref-item ref-cmd">

### 更新全部數值 <small>readAll</small>

一次讀整張值表進快取（FD 整包）；接下來 10 ms 內的讀值積木直接用快取，不再逐一詢問

<pre class="blocks">測距者 2 路 連接埠 [1 v] 更新全部數值 :: sensing</pre>

</section>

## 範例

<!-- examples: sensors/tof2/mblock-examples.md host=mblock sw=mblock5 whole -->

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
- 值與命令：`_build/protocol/tof2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/mblock-mblock5.zh.md`、`_build/guides/product/tof2/mblock-mblock5.zh.md`
- 範例：`sensors/tof2/mblock-examples.md`
