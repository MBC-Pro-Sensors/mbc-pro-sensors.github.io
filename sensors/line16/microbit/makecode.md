<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/microbit/makecode -->
# 循行者 16 路 (Line16) × micro:bit × MakeCode

這一頁只講 **MakeCode（積木／JavaScript）**：在 BBC micro:bit V1／V2 上用它讀循行者 16 路 (Line16)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x16`。感應器要 5 V 供電，micro:bit 本身只有 3.3 V，請用有 5 V 輸出的擴充板。

<figure class="ref-fig"><img src="/images/figures/line16/wiring-microbit.svg" alt="循行者 16 路 (Line16) × BBC micro:bit V1／V2" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/microbit-makecode.zh.md + _build/guides/product/line16/microbit-makecode.zh.md {"product":"循行者 16 路 (Line16)","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 16 路 (Line16)：MakeCode（積木／JavaScript） 說明](/sensors/line16/microbit.md)。
<!-- /include-fallback -->

## 讀值

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC」分類找積木；JavaScript 寫法一併列出，位址 `0x16`。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |

<pre class="blocks">(MBC 線的位置 :: #E8711E)</pre>

```typescript
let linePos = MBC.readValue(0x16, 0x01)   // 線位置：-16~+16
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

<pre class="blocks">(MBC 線的細位置 :: #E8711E)</pre>

```typescript
let linePosFine = MBC.readValue(0x16, 0x02)   // 細位置：-100~+100
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~16` 路 | 壓到線的路數 0~16，0 = 找不到線 |

<pre class="blocks">(MBC 線寬 :: #E8711E)</pre>

```typescript
let lineWidth = MBC.readValue(0x16, 0x03)   // 線寬：0~16 路
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v04" class="ref-item">

### 線段數 <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-groupCount.svg" alt="線段數" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |

<pre class="blocks">(MBC 線段數 :: #E8711E)</pre>

```typescript
let groupCount = MBC.readValue(0x16, 0x04)   // 線段數：0~8
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | bit0 = 第 1 路（最左）… bit15 = 第 16 路，1 = 那一路在線上 |

<pre class="blocks">(MBC 壓線位元圖 :: #E8711E)</pre>

```typescript
let bitmap = MBC.readValue(0x16, 0x05)   // 壓線位元圖：0~65535 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v06" class="ref-item">

### 最後離線方向 <small>lastDir</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 丟線前線最後在哪一側：−16 ~ +16（負 = 左），找回線後維持最後一次 |

<pre class="blocks">(MBC 最後離線方向 :: #E8711E)</pre>

```typescript
let lastDir = MBC.readValue(0x16, 0x06)   // 最後離線方向：-16~+16
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v07" class="ref-item">

### 二值化門檻 <small>threshold</small>

| 範圍 | 意思 |
| :--- | :--- |
| `1~99` | 判斷「在線上」的光值門檻 1~99，預設 50；用 E4 改 |

<pre class="blocks">(MBC 門檻 :: #E8711E)</pre>

```typescript
let threshold = MBC.readValue(0x16, 0x07)   // 二值化門檻：1~99
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v08" class="ref-item">

### 校準完成次數 <small>calibCount</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` 次 | 開機後完成校準的次數（按鈕校準也算）；16 路大通道尾段鎖住為 0，不進大通道 |

micro:bit 擴充沒有這顆積木（可用 `MBC.readValue(位址, 編號)` 通用積木讀）。

</section>

<section id="v10" class="ref-item">

### 第 1~16 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="第 1~16 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

<pre class="blocks">(MBC 第 [1 v] 路光值 :: #E8711E)</pre>

```typescript
let light = MBC.readValues(0x16, 0x10, 16)   // 第 1~16 路光值
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

## 命令

<section id="ce0" class="ref-item ref-cmd">

### 切黑線 <small>blackLine</small>

追黑線（預設）

<pre class="blocks">MBC 切換為黑線 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE0)   // 切黑線
```

</section>

<section id="ce1" class="ref-item ref-cmd">

### 切白線 <small>whiteLine</small>

追白線（F3 b1 = 1）

<pre class="blocks">MBC 切換為白線 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE1)   // 切白線
```

</section>

<section id="ce2" class="ref-item ref-cmd">

### 黑色校準 <small>calibBlack</small>

把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM

<pre class="blocks">MBC 黑色校準 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE2)   // 黑色校準
```

</section>

<section id="ce3" class="ref-item ref-cmd">

### 白色校準 <small>calibWhite</small>

把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM

<pre class="blocks">MBC 白色校準 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE3)   // 白色校準
```

</section>

<section id="ce4" class="ref-item ref-cmd">

### 設定門檻 <small>setThreshold</small>

二值化門檻 1~99（RAM，重開機回預設 50）

<pre class="blocks">MBC 門檻設為 (50) :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE4, [50])   // 設定門檻
```

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5 秒動態校準 <small>calibAuto</small>

5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1

<pre class="blocks">MBC 5 秒動態校準 :: #E8711E</pre>

```typescript
MBC.writeCmd(0x16, 0xE7)   // 5 秒動態校準
```

</section>

## 範例

<!-- examples: sensors/line16/microbit-examples.md host=microbit sw=makecode whole -->

## 其他問題

- **找不到 MBC 積木？** MakeCode「擴充功能」貼上 https://github.com/contactmbcrobot/pxt-mbc-sensors 加入。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line16.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/microbit-makecode.zh.md`、`_build/guides/product/line16/microbit-makecode.zh.md`
- 範例：`sensors/line16/microbit-examples.md`
