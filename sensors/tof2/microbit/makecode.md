<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/microbit/makecode -->
# 測距者 2 路 (TOF2) × micro:bit × MakeCode

這一頁只講 **MakeCode（積木／JavaScript）**：在 BBC micro:bit V1／V2 上用它讀測距者 2 路 (TOF2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x29`。這款耗電小，VCC 可以先接 micro:bit 的 **3V** 腳（V1 約 90 mA、V2 約 190 mA）；用電池供電時電壓可能掉到 3 V 以下，不穩就改外接 5 V（GND 要和 micro:bit 共地）。

<figure class="ref-fig"><img src="/images/figures/tof2/wiring-microbit.svg" alt="測距者 2 路 (TOF2) × BBC micro:bit V1／V2" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/microbit-makecode.zh.md + _build/guides/product/tof2/microbit-makecode.zh.md {"product":"測距者 2 路 (TOF2)","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 2 路 (TOF2)：MakeCode（積木／JavaScript） 說明](/sensors/tof2/microbit.md)。
<!-- /include-fallback -->

## 讀值

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC 感應器」分類找積木；JavaScript 寫法一併列出，位址 `0x29`。

<section id="v01" class="ref-item">

### 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |

<pre class="blocks">(測距者 2 路 [左 v] 距離（[公分 v]） :: #00A4E4)</pre>

```typescript
let leftCm = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Cm)   // 左距離 cm：0~120 cm
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v02" class="ref-item">

### 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

<pre class="blocks">(測距者 2 路 [右 v] 距離（[公分 v]） :: #00A4E4)</pre>

```typescript
let rightCm = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Cm)   // 右距離 cm：0~120 cm
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v03" class="ref-item">

### 較近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-nearCm.svg" alt="較近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左右兩顆取較近的 0~120 cm |

<pre class="blocks">(測距者 2 路 較近距離（公分） :: #00A4E4)</pre>

```typescript
let nearCm = mbc.tof2NearestCm()   // 較近距離 cm：0~120 cm
```

**相關積木「當測距者 2 路 距離小於 10 公分」**

```typescript
mbc.onTof2Near(10, function () {
})
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v04" class="ref-item">

### 左距離 mm <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="左距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 左邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

<pre class="blocks">(測距者 2 路 [左 v] 距離（[公釐 v]） :: #00A4E4)</pre>

```typescript
let leftMm = mbc.tof2Distance(MbcTofSide.Left, MbcDistUnit.Mm)   // 左距離 mm：0~1200 mm
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v05" class="ref-item">

### 右距離 mm <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="右距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 右邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

<pre class="blocks">(測距者 2 路 [右 v] 距離（[公釐 v]） :: #00A4E4)</pre>

```typescript
let rightMm = mbc.tof2Distance(MbcTofSide.Right, MbcDistUnit.Mm)   // 右距離 mm：0~1200 mm
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v06" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不重試）；3 = 兩顆都好 |

擴充沒有這顆積木；在 JavaScript 用 `mbc.readValue(位址, 編號)` 讀（讀不到 = −32768）。相關積木：「測距者 2 路 ［左］ 感應器正常？」。

```typescript
let initOk = mbc.readValue(0x29, 0x06)   // 初始化位元圖：0~3 bit
```

</section>

<section id="v07" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

擴充沒有這顆積木；在 JavaScript 用 `mbc.readValue(位址, 編號)` 讀（讀不到 = −32768）。相關積木：「測距者 2 路 ［左］ 感應器正常？」。

```typescript
let fault = mbc.readValue(0x29, 0x07)   // 故障位元圖：0~3 bit
```

</section>

<section id="v08" class="ref-item">

### 左右平均 mm <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="左右平均 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | (左 mm + 右 mm) / 2，0~1200；跟 EV3 模式 3、SPIKE 模式 6 第 3 個值一致（共識 #8 補上） |

<pre class="blocks">(測距者 2 路 平均距離（公釐） :: #00A4E4)</pre>

```typescript
let avgMm = mbc.tof2AverageMm()   // 左右平均 mm：0~1200 mm
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

## 範例

<!-- examples: sensors/tof2/microbit-examples.md host=microbit sw=makecode whole -->

## 其他問題

- **找不到 MBC 積木？** MakeCode「擴充功能」貼上 https://github.com/contactmbcrobot/pxt-mbc-sensors 加入。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/microbit-makecode.zh.md`、`_build/guides/product/tof2/microbit-makecode.zh.md`
- 範例：`sensors/tof2/microbit-examples.md`
