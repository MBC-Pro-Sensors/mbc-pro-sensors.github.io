<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/microbit/makecode -->
# 測距者 8 路 (TOF8) × micro:bit × MakeCode

這一頁只講 **MakeCode（積木／JavaScript）**：在 BBC micro:bit V1／V2 上用它讀測距者 8 路 (TOF8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x29`。感應器要 5 V 供電，micro:bit 本身只有 3.3 V，請用有 5 V 輸出的擴充板。

<figure class="ref-fig"><img src="/images/figures/tof8/wiring-microbit.svg" alt="測距者 8 路 (TOF8) × BBC micro:bit V1／V2" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/microbit-makecode.zh.md + _build/guides/product/tof8/microbit-makecode.zh.md {"product":"測距者 8 路 (TOF8)","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 8 路 (TOF8)：MakeCode（積木／JavaScript） 說明](/sensors/tof8/microbit.md)。
<!-- /include-fallback -->

## 讀值

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC」分類找積木；JavaScript 寫法一併列出，位址 `0x29`。

<section id="v01" class="ref-item">

### 最近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="最近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |

<pre class="blocks">(MBC 最近距離 cm :: #E8711E)</pre>

```typescript
let nearCm = MBC.readValue(0x29, 0x01)   // 最近距離 cm：0~120 cm
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v02" class="ref-item">

### 最近方向 <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="最近方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |

<pre class="blocks">(MBC 最近方向 :: #E8711E)</pre>

```typescript
let nearDir = MBC.readValue(0x29, 0x02)   // 最近方向：-8~+8
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v03" class="ref-item">

### 最空曠距離 cm <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="最空曠距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |

<pre class="blocks">(MBC 最空曠距離 cm :: #E8711E)</pre>

```typescript
let farCm = MBC.readValue(0x29, 0x03)   // 最空曠距離 cm：0~120 cm
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v04" class="ref-item">

### 最空曠方向 <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="最空曠方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-7~+7` | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |

<pre class="blocks">(MBC 最空曠方向 :: #E8711E)</pre>

```typescript
let farDir = MBC.readValue(0x29, 0x04)   // 最空曠方向：-7~+7
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v05" class="ref-item">

### 正前方 cm <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="正前方 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 4、5 顆（兩顆平行朝前）平均 0~120 cm；EV3 模式 3、SPIKE 模式 6 給的是同一個值的 mm；0x22 的 C 是 min(第 4, 第 5 顆) 不是平均（見 10） |

<pre class="blocks">(MBC 正前方距離 cm :: #E8711E)</pre>

```typescript
let frontCm = MBC.readValue(0x29, 0x05)   // 正前方 cm：0~120 cm
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v06" class="ref-item">

### 最近距離 mm <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="最近距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 8 顆 mm 的最小值 0~1200（失效那顆以 1200 參與），1200 = 四周沒東西 |

<pre class="blocks">(MBC 最近距離 mm :: #E8711E)</pre>

```typescript
let nearMm = MBC.readValue(0x29, 0x06)   // 最近距離 mm：0~1200 mm
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v07" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 顆（最左）… bit7 = 第 8 顆，1 = 那顆開機初始化成功（8 顆約 2~4 秒，失敗不重試）；255 = 全部好 |

<pre class="blocks">(MBC 初始化位元圖 :: #E8711E)</pre>

```typescript
let initOk = MBC.readValue(0x29, 0x07)   // 初始化位元圖：0~255 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v08" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 顆 … bit7 = 第 8 顆，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

<pre class="blocks">(MBC 故障位元圖 :: #E8711E)</pre>

```typescript
let fault = MBC.readValue(0x29, 0x08)   // 故障位元圖：0~255 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v09" class="ref-item">

### 正前方距離 mm <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="正前方距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 第 4、5 顆（正前方兩顆）平均，0~1200 mm；1200 = 前面沒東西。cm 版是 05 |

<pre class="blocks">(MBC 正前方距離 mm :: #E8711E)</pre>

```typescript
let frontMm = MBC.readValue(0x29, 0x09)   // 正前方距離 mm：0~1200 mm
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v10" class="ref-item">

### 第 1~8 顆距離 cm <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="第 1~8 顆距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |

<pre class="blocks">(MBC 第 [1 v] 顆距離 cm :: #E8711E)</pre>

```typescript
let distCm = MBC.readValues(0x29, 0x10, 8)   // 第 1~8 顆距離 cm
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v18" class="ref-item">

### 第 1~8 顆距離 mm <small>distMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distMm.svg" alt="第 1~8 顆距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 第 1~8 顆各自的距離 0~1200 mm，1200 = 那個方向沒東西／那顆失效；編號 18~1F |

<pre class="blocks">(MBC 第 [1 v] 顆距離 mm :: #E8711E)</pre>

```typescript
let distMm = MBC.readValues(0x29, 0x18, 8)   // 第 1~8 顆距離 mm
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

## 範例

<!-- examples: sensors/tof8/microbit-examples.md host=microbit sw=makecode whole -->

## 其他問題

- **找不到 MBC 積木？** MakeCode「擴充功能」貼上 https://github.com/contactmbcrobot/pxt-mbc-sensors 加入。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/microbit-makecode.zh.md`、`_build/guides/product/tof8/microbit-makecode.zh.md`
- 範例：`sensors/tof8/microbit-examples.md`
