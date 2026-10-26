<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/mblock/mblock5 -->
# 循行者 8 路 (Line8) × mBlock × mBlock 5

這一頁只講 **mBlock 5（MBC Pro Sensors 擴展）**：在 MakeBlock（mBot）＋ mBlock 5 上用它讀循行者 8 路 (Line8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **MakeBlock 版**（RJ25 接頭），插 mBot／Ranger／Orion／MegaPi 的 RJ25 連接埠；各主板可用哪幾個埠見 [MakeBlock 積木擴展](/makeblock.md)。

<figure class="ref-fig"><img src="/images/figures/line8/wiring-mbot.svg" alt="循行者 8 路 (Line8) × MakeBlock（mBot）＋ mBlock 5" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/mblock-mblock5.zh.md + _build/guides/product/line8/mblock-mblock5.zh.md {"product":"循行者 8 路 (Line8)","edition":"MakeBlock 版","port":"1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 8 路 (Line8)：mBlock 5（MBC Pro Sensors 擴展） 說明](/sensors/line8/mblock.md)。
<!-- /include-fallback -->

## 讀值

裝好 MBC Pro Sensors 擴展後，在「循行者」分類找積木，連接埠選感應器實際插的孔。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 線在哪一路：−8（最左）~+8（最右），0 = 正中間；站在車後往前看，負 = 線在左 |

<pre class="blocks">(循行者 連接埠 [1 v] 線的位置 :: sensing)</pre>

<p class="ref-note">積木傳回 線位置（-8~+8）。主機送 <code>[01]</code> → 回 <code>[值低][值高][01 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

<pre class="blocks">(循行者 連接埠 [1 v] 線的細位置 :: sensing)</pre>

<p class="ref-note">積木傳回 細位置（-100~+100）。主機送 <code>[02]</code> → 回 <code>[值低][值高][02 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` 路 | 壓到線的路數 0~8，0 = 找不到線 |

<pre class="blocks">(循行者 連接埠 [1 v] 線寬 :: sensing)</pre>

<p class="ref-note">積木傳回 線寬（0~8 路）。主機送 <code>[03]</code> → 回 <code>[值低][值高][03 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v04" class="ref-item">

### 線段數 <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="線段數" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~4` | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |

<pre class="blocks">(循行者 連接埠 [1 v] 線段數 :: sensing)</pre>

<p class="ref-note">積木傳回 線段數（0~4）。主機送 <code>[04]</code> → 回 <code>[值低][值高][04 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 路（最左）… bit7 = 第 8 路，1 = 那一路在線上 |

<pre class="blocks">(循行者 連接埠 [1 v] 壓線位元圖 :: sensing)</pre>

<p class="ref-note">積木傳回 壓線位元圖（0~255 bit）。主機送 <code>[05]</code> → 回 <code>[值低][值高][05 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v06" class="ref-item">

### 最後離線方向 <small>lastDir</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 丟線前線最後在哪一側：−8 ~ +8（負 = 左），找回線後維持最後一次 |

<pre class="blocks">(循行者 連接埠 [1 v] 最後離線方向 :: sensing)</pre>

<p class="ref-note">積木傳回 最後離線方向（-8~+8）。主機送 <code>[06]</code> → 回 <code>[值低][值高][06 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v07" class="ref-item">

### 二值化門檻 <small>threshold</small>

| 範圍 | 意思 |
| :--- | :--- |
| `1~99` | 判斷「在線上」的光值門檻 1~99，預設 50；用 E4 改 |

<pre class="blocks">(循行者 連接埠 [1 v] 門檻 :: sensing)</pre>

<p class="ref-note">積木傳回 二值化門檻（1~99）。主機送 <code>[07]</code> → 回 <code>[值低][值高][07 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v08" class="ref-item">

### 校準完成次數 <small>calibCount</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` 次 | 開機後完成校準的次數（按鈕校準也算） |

<pre class="blocks">(循行者 連接埠 [1 v] 校準次數 :: sensing)</pre>

<p class="ref-note">積木傳回 校準完成次數（0~255 次）。主機送 <code>[08]</code> → 回 <code>[值低][值高][08 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v10" class="ref-item">

### 第 1~8 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="第 1~8 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~8 路校準後光值 0~100（第 1 路最左）；編號 10~17 |

<pre class="blocks">(循行者 連接埠 [1 v] 第 [1 v] 路光值 :: sensing)</pre>

<p class="ref-note">積木傳回 第 1~8 路光值（0~100）。主機送 <code>[10]</code> → 回 <code>[值低][值高][10 ^ 值低 ^ 值高]</code>，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。</p>

</section>

<section id="vx-online" class="ref-item">

### 第 N 路在線上？ <small>onLine</small>

| 範圍 | 意思 |
| :--- | :--- |
| 是／否 | 壓線位元圖 05 的第 N 個 bit（第 1 路 = bit0，最左）：那一路壓到線 = 是 |

<pre class="blocks"><循行者 連接埠 [1 v] 第 [1 v] 路在線上？ :: sensing></pre>

</section>

## 命令

<section id="ce0" class="ref-item ref-cmd">

### 切黑線 <small>blackLine</small>

追黑線（預設）

<pre class="blocks">循行者 連接埠 [1 v] 切黑線 :: sensing</pre>

<p class="ref-note">主機送 <code>[E0]</code> → 感應器回 <code>[00][00][E0]</code>（收到了）再執行。</p>

</section>

<section id="ce1" class="ref-item ref-cmd">

### 切白線 <small>whiteLine</small>

追白線（F3 b1 = 1）

<pre class="blocks">循行者 連接埠 [1 v] 切白線 :: sensing</pre>

<p class="ref-note">主機送 <code>[E1]</code> → 感應器回 <code>[00][00][E1]</code>（收到了）再執行。</p>

</section>

<section id="ce2" class="ref-item ref-cmd">

### 黑色校準 <small>calibBlack</small>

把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM

<pre class="blocks">循行者 連接埠 [1 v] 黑色校準 :: sensing</pre>

<p class="ref-note">主機送 <code>[E2]</code> → 感應器回 <code>[00][00][E2]</code>（收到了）再執行；執行期間 F3 bit2 = 1。</p>

</section>

<section id="ce3" class="ref-item ref-cmd">

### 白色校準 <small>calibWhite</small>

把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM

<pre class="blocks">循行者 連接埠 [1 v] 白色校準 :: sensing</pre>

<p class="ref-note">主機送 <code>[E3]</code> → 感應器回 <code>[00][00][E3]</code>（收到了）再執行；執行期間 F3 bit2 = 1。</p>

</section>

<section id="ce4" class="ref-item ref-cmd">

### 設定門檻 <small>setThreshold</small>

二值化門檻 1~99（RAM，重開機回預設 50）

<pre class="blocks">循行者 連接埠 [1 v] 設定門檻 (50) :: sensing</pre>

<p class="ref-note">主機送 <code>[E4][參數…]</code> → 感應器回 <code>[00][00][E4]</code>（收到了）再執行。</p>

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5 秒動態校準 <small>calibAuto</small>

5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1

<pre class="blocks">循行者 連接埠 [1 v] 5 秒動態校準 :: sensing</pre>

<p class="ref-note">主機送 <code>[E7]</code> → 感應器回 <code>[00][00][E7]</code>（收到了）再執行；執行期間 F3 bit2 = 1。</p>

</section>

<section id="cx-readall" class="ref-item ref-cmd">

### 更新全部數值 <small>readAll</small>

一次讀整張值表進快取（FD 整包）；接下來 10 ms 內的讀值積木直接用快取，不再逐一詢問

<pre class="blocks">循行者 連接埠 [1 v] 更新全部數值 :: sensing</pre>

</section>

## 範例

<!-- examples: sensors/line8/mblock-examples.md host=mblock sw=mblock5 whole -->

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
- 值與命令：`_build/protocol/line8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/mblock-mblock5.zh.md`、`_build/guides/product/line8/mblock-mblock5.zh.md`
- 範例：`sensors/line8/mblock-examples.md`
