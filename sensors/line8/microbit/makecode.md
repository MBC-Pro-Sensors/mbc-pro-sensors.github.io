<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/microbit/makecode -->
# 循行者 8 路 (Line8) × micro:bit × MakeCode

這一頁只講 **MakeCode（積木／JavaScript）**：在 BBC micro:bit V1／V2 上用它讀循行者 8 路 (Line8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x16`。這款耗電小，VCC 可以先接 micro:bit 的 **3V** 腳（V1 約 90 mA、V2 約 190 mA）；用電池供電時電壓可能掉到 3 V 以下，不穩就改外接 5 V（GND 要和 micro:bit 共地）。

<figure class="ref-fig"><img src="/images/figures/line8/wiring-microbit.svg" alt="循行者 8 路 (Line8) × BBC micro:bit V1／V2" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/microbit-makecode.zh.md + _build/guides/product/line8/microbit-makecode.zh.md {"product":"循行者 8 路 (Line8)","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 8 路 (Line8)：MakeCode（積木／JavaScript） 說明](/sensors/line8/microbit.md)。
<!-- /include-fallback -->

## 讀值

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC 感應器」分類找積木；JavaScript 寫法一併列出，位址 `0x16`。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 線在哪一路：−8（最左）~+8（最右），0 = 正中間；站在車後往前看，負 = 線在左 |

<pre class="blocks">(循行者 線位置 :: #00A4E4)</pre>

```typescript
let linePos = mbc.linePosition()   // 線位置：-8~+8
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

<pre class="blocks">(循行者 細位置 :: #00A4E4)</pre>

```typescript
let linePosFine = mbc.linePositionFine()   // 細位置：-100~+100
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` 路 | 壓到線的路數 0~8，0 = 找不到線 |

<pre class="blocks">(循行者 線寬 :: #00A4E4)</pre>

```typescript
let lineWidth = mbc.lineWidth()   // 線寬：0~8 路
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v04" class="ref-item">

### 線段數 <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="線段數" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~4` | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |

<pre class="blocks">(循行者 線段數 :: #00A4E4)</pre>

```typescript
let groupCount = mbc.lineSegments()   // 線段數：0~4
```

**相關積木「循行者 看到線？」**

```typescript
let lineSeen = mbc.lineSeen()
```

**相關積木「當循行者 ［丟線］」**

```typescript
mbc.onLineEvent(MbcLineEvent.Lost, function () {
})
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 路（最左）… bit7 = 第 8 路，1 = 那一路在線上 |

<pre class="blocks">(循行者 壓線位元圖 :: #00A4E4)</pre>

```typescript
let bitmap = mbc.lineBitmap()   // 壓線位元圖：0~255 bit
```

**相關積木「循行者 第 1 路在線上？」**

```typescript
let lineChannelOnLine = mbc.lineChannelOnLine(1)
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v06" class="ref-item">

### 最後離線方向 <small>lastDir</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 丟線前線最後在哪一側：−8 ~ +8（負 = 左），找回線後維持最後一次 |

<pre class="blocks">(循行者 最後離線方向 :: #00A4E4)</pre>

```typescript
let lastDir = mbc.lineLastDirection()   // 最後離線方向：-8~+8
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v07" class="ref-item">

### 二值化門檻 <small>threshold</small>

| 範圍 | 意思 |
| :--- | :--- |
| `1~99` | 判斷「在線上」的光值門檻 1~99，預設 50；用 E4 改 |

<pre class="blocks">(循行者 門檻 :: #00A4E4)</pre>

```typescript
let threshold = mbc.lineThreshold()   // 二值化門檻：1~99
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v08" class="ref-item">

### 校準完成次數 <small>calibCount</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` 次 | 開機後完成校準的次數（按鈕校準也算） |

<pre class="blocks">(循行者 校準次數 :: #00A4E4)</pre>

```typescript
let calibCount = mbc.lineCalibrationCount()   // 校準完成次數：0~255 次
```

**相關積木「循行者 校準中？」**

```typescript
let lineCalibrating = mbc.lineCalibrating()
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v10" class="ref-item">

### 第 1~8 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="第 1~8 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~8 路校準後光值 0~100（第 1 路最左）；編號 10~17 |

<pre class="blocks">(循行者 第 (1) 路光值 :: #00A4E4)</pre>

```typescript
let light = mbc.lineChannel(1)   // 第 1~8 路光值：0~100（N = 1~8）
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

## 命令

<section id="ce0" class="ref-item ref-cmd">

### 切黑線 <small>blackLine</small>

追黑線（預設）

<pre class="blocks">循行者 循 [黑 v] 線 :: #00A4E4</pre>

```typescript
mbc.lineSetColor(MbcLineColor.Black)   // 切黑線
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### 切白線 <small>whiteLine</small>

追白線（F3 b1 = 1）

<pre class="blocks">循行者 循 [白 v] 線 :: #00A4E4</pre>

```typescript
mbc.lineSetColor(MbcLineColor.White)   // 切白線
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### 黑色校準 <small>calibBlack</small>

把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM

<pre class="blocks">循行者 校準 [目前讀值記為黑 v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.Black)   // 黑色校準
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### 白色校準 <small>calibWhite</small>

把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM

<pre class="blocks">循行者 校準 [目前讀值記為白 v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.White)   // 白色校準
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### 設定門檻 <small>setThreshold</small>

二值化門檻 1~99（RAM，重開機回預設 50）

<pre class="blocks">循行者 門檻設為 (50) :: #00A4E4</pre>

```typescript
mbc.lineSetThreshold(50)   // 設定門檻
```

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5 秒動態校準 <small>calibAuto</small>

5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1

<pre class="blocks">循行者 校準 [自動（來回掃 5 秒） v] :: #00A4E4</pre>

```typescript
mbc.lineCalibrate(MbcLineCalib.Auto)   // 5 秒動態校準
```

</section>

## 範例

<!-- examples: sensors/line8/microbit-examples.md host=microbit sw=makecode whole -->

## 其他問題

- **找不到 MBC 積木？** MakeCode「擴充功能」貼上 https://github.com/contactmbcrobot/pxt-mbc-sensors 加入。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/microbit-makecode.zh.md`、`_build/guides/product/line8/microbit-makecode.zh.md`
- 範例：`sensors/line8/microbit-examples.md`
