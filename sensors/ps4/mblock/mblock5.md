<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × mblock × mblock5），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/mblock/mblock5 -->
# 掌控者 PS4/PS5 × mBlock × mBlock 5

這一頁只講 **mBlock 5（MBC Pro Sensors 擴展）**：在 MakeBlock（mBot）＋ mBlock 5 上用它讀掌控者 PS4/PS5的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **MakeBlock 版**（RJ25 接頭），插 mBot／Ranger／Orion／MegaPi 的 RJ25 連接埠；各主板可用哪幾個埠見 [MakeBlock 積木擴展](/makeblock.md)。

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-mbot.svg" alt="掌控者 PS4/PS5 × MakeBlock（mBot）＋ mBlock 5" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/mblock-mblock5.zh.md + _build/guides/product/ps4/mblock-mblock5.zh.md {"product":"掌控者 PS4/PS5","edition":"MakeBlock 版","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS4/PS5：mBlock 5（MBC Pro Sensors 擴展） 說明](/sensors/ps4/mblock.md)。
<!-- /include-fallback -->

## 讀值

裝好 MBC Pro Sensors 擴展後，在「掌控者」分類找積木，連接埠選感應器實際插的孔。

<section id="v01" class="ref-item">

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 全部按鍵一個 16 bit（PSX 排列）：bit0 SELECT(SHARE)、1 L3、2 R3、3 START(OPTIONS)、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；1 = 按下；未連線 = 0 |

<pre class="blocks">(掌控者 連接埠 [1 v] 十六鍵 :: sensing)</pre>

<p class="ref-note">積木傳回 十六鍵（0~65535 bit）。主機送 <code>[01]</code> → 回 <code>[值低][值高][01 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8，同時按相加（0~15） |

<pre class="blocks">(掌控者 連接埠 [1 v] 方向鍵 :: sensing)</pre>

<p class="ref-note">積木傳回 方向鍵（0~15 bit）。主機送 <code>[02]</code> → 回 <code>[值低][值高][02 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8，同時按相加（0~15） |

<pre class="blocks">(掌控者 連接埠 [1 v] 形狀鍵 :: sensing)</pre>

<p class="ref-note">積木傳回 形狀鍵（0~15 bit）。主機送 <code>[03]</code> → 回 <code>[值低][值高][03 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8，同時按相加（0~15） |

<pre class="blocks">(掌控者 連接埠 [1 v] 肩鍵 :: sensing)</pre>

<p class="ref-note">積木傳回 肩鍵（0~15 bit）。主機送 <code>[04]</code> → 回 <code>[值低][值高][04 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1、L3 2、R3 4、START(OPTIONS) 8，同時按相加（0~15） |

<pre class="blocks">(掌控者 連接埠 [1 v] 功能鍵 :: sensing)</pre>

<p class="ref-note">積木傳回 功能鍵（0~15 bit）。主機送 <code>[05]</code> → 回 <code>[值低][值高][05 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線且 300 ms 內有資料，0 = 未連線（斷線時按鍵 0、搖桿中間） |

<pre class="blocks">(掌控者 連接埠 [1 v] 手把已連線？ :: sensing)</pre>

<p class="ref-note">積木傳回 已連線（0~1）。主機送 <code>[06]</code> → 回 <code>[值低][值高][06 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v07" class="ref-item">

### 電量 <small>battery</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-battery.svg" alt="電量" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` % | 手把電量 0~100（PS2 接收器恆 0） |

<pre class="blocks">(掌控者 連接埠 [1 v] 電量 :: sensing)</pre>

<p class="ref-note">積木傳回 電量（0~100 %）。主機送 <code>[07]</code> → 回 <code>[值低][值高][07 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v08" class="ref-item">

### L2 類比 <small>l2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-l2Analog.svg" alt="L2 類比" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | L2 扳機壓多深 0~255（未連線 0） |

<pre class="blocks">(掌控者 連接埠 [1 v] L2 類比 :: sensing)</pre>

<p class="ref-note">積木傳回 L2 類比（0~255）。主機送 <code>[08]</code> → 回 <code>[值低][值高][08 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v09" class="ref-item">

### R2 類比 <small>r2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-r2Analog.svg" alt="R2 類比" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | R2 扳機壓多深 0~255（未連線 0） |

<pre class="blocks">(掌控者 連接埠 [1 v] R2 類比 :: sensing)</pre>

<p class="ref-note">積木傳回 R2 類比（0~255）。主機送 <code>[09]</code> → 回 <code>[值低][值高][09 ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v0a" class="ref-item">

### 系統鍵 <small>sysKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-sysKeys.svg" alt="系統鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | b0 PS 鍵、b1 擷取（PS5 靜音）鍵；未連線 0（PS2 恆 0） |

<pre class="blocks">(掌控者 連接埠 [1 v] 系統鍵 :: sensing)</pre>

<p class="ref-note">積木傳回 系統鍵（0~3 bit）。主機送 <code>[0A]</code> → 回 <code>[值低][值高][0A ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v0b" class="ref-item">

### 手把類型 <small>padType</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~4` | 0 無、1 PS2 數位、2 PS2 類比、3 PS4、4 PS5（或其他） |

<pre class="blocks">(掌控者 連接埠 [1 v] 手把類型 :: sensing)</pre>

<p class="ref-note">積木傳回 手把類型（0~4）。主機送 <code>[0B]</code> → 回 <code>[值低][值高][0B ^ 值低 ^ 值高]</code>，值 = int16 小端。</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 1~4 軸 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="搖桿 1~4 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正；未連線 0 |

<pre class="blocks">(掌控者 連接埠 [1 v] 搖桿 [軸] :: sensing)</pre>

<p class="ref-note">積木傳回 搖桿 1~4 軸（-100~+100）。主機送 <code>[10]</code> → 回 <code>[值低][值高][10 ^ 值低 ^ 值高]</code>，值 = int16 小端；第 N 路的編號 = 0x10 + (N − 1)。</p>

</section>

<section id="vx-pressed" class="ref-item">

### ［按鈕］ 被按下？ <small>pressed</small>

| 範圍 | 意思 |
| :--- | :--- |
| 是／否 | 從十六鍵 01 拆出的單一按鈕（PSX 排列），選單：✕ ● ▲ ■ 上 下 左 右 L1 R1 L2 R2 L3 R3 SELECT START |

<pre class="blocks"><掌控者 連接埠 [1 v] [■ v] 被按下？ :: sensing></pre>

</section>

## 命令

<section id="ce5" class="ref-item ref-cmd">

### 震動 <small>rumble</small>

[強度 0~255]，0 = 停；每次 200 ms（PS2 收到忽略）

<pre class="blocks">掌控者 連接埠 [1 v] 震動 (128) :: sensing</pre>

<p class="ref-note">主機送 <code>[E5][參數…]</code> → 感應器回 <code>[00][00][E5]</code>（收到了）再執行。</p>

</section>

<section id="ce6" class="ref-item ref-cmd">

### 燈條顏色 <small>lightbar</small>

[R][G][B] 各 0~255，要剛好 3 byte；未連線先記住，連上再套用；預設藍 (0,0,255)（PS2 收到忽略）

<pre class="blocks">掌控者 連接埠 [1 v] 燈條顏色 (128) (128) (128) :: sensing</pre>

<p class="ref-note">主機送 <code>[E6][參數…]</code> → 感應器回 <code>[00][00][E6]</code>（收到了）再執行。</p>

</section>

<section id="cx-readall" class="ref-item ref-cmd">

### 更新全部數值 <small>readAll</small>

一次讀整張值表進快取（FD 整包）；接下來 10 ms 內的讀值積木直接用快取，不再逐一詢問

<pre class="blocks">掌控者 連接埠 [1 v] 更新全部數值 :: sensing</pre>

</section>

## 範例

<!-- examples: sensors/ps4/mblock-examples.md host=mblock sw=mblock5 whole -->

## 其他問題

- **積木一直回 0？** 確認積木選的連接埠跟實際插的一樣，並且已經用「上傳」模式把程式燒進主板。
- **舊的擴展還能用嗎？** 2026-10 以前出貨的感應器用舊版擴展 v1.0.0；新舊積木不相容，依韌體版本選。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.1.0_import.mext)
- [MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext](/downloads/MBC_Pro_Sensors_mBlock5_v2.0.0_import.mext)
- [MBC_Pro_Sensors_mBlock5_v1.0.0.mext（舊版，2026-10 以前出貨的感應器）](/downloads/MBC_Pro_Sensors_mBlock5_v1.0.0.mext)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps4.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/mblock-mblock5.zh.md`、`_build/guides/product/ps4/mblock-mblock5.zh.md`
- 範例：`sensors/ps4/mblock-examples.md`
