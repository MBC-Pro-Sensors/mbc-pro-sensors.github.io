<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/microbit/makecode -->
# 神攝手 ESP32CAM × micro:bit × MakeCode

這一頁只講 **MakeCode（積木／JavaScript）**：在 BBC micro:bit V1／V2 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x4C`。**一定要外接 5 V 供電**（ESP32 相機＋螢幕＋補光燈），不能吃 micro:bit 的 3V 腳；外部電源的 GND 要和 micro:bit 共地。

<figure class="ref-fig"><img src="/images/figures/esp32cam/wiring-microbit.svg" alt="神攝手 ESP32CAM × BBC micro:bit V1／V2" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/microbit-makecode.zh.md + _build/guides/product/esp32cam/microbit-makecode.zh.md {"product":"神攝手 ESP32CAM","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：MakeCode（積木／JavaScript） 說明](/sensors/esp32cam/microbit.md)。
<!-- /include-fallback -->

## 讀值（學校版）

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC 感應器」分類找積木；JavaScript 寫法一併列出，位址 `0x4C`。

<section id="v01" class="ref-item">

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

<pre class="blocks">(神攝手 顏色 [色相 v] :: #00A4E4)</pre>

```typescript
let hue = mbc.camColor(MbcCamColor.Hue)   // 色相：-1~100
```

**相關積木「神攝手 顏色是黑白灰？」**

```typescript
let camIsGrey = mbc.camIsGrey()
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v02" class="ref-item">

### 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點飽和度 0~100 |

<pre class="blocks">(神攝手 顏色 [飽和度 v] :: #00A4E4)</pre>

```typescript
let saturation = mbc.camColor(MbcCamColor.Saturation)   // 飽和度：0~100
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v03" class="ref-item">

### 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點明度 0~100 |

<pre class="blocks">(神攝手 顏色 [明度 v] :: #00A4E4)</pre>

```typescript
let value = mbc.camColor(MbcCamColor.Brightness)   // 明度：0~100
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

<pre class="blocks">(神攝手 動態 :: #00A4E4)</pre>

```typescript
let motion = mbc.camMotion()   // 動態：0~100
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

<pre class="blocks">(神攝手 色塊 [X v] :: #00A4E4)</pre>

```typescript
let blobX = mbc.camBlob(MbcCamBlob.X)   // 色塊 X：0~100
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

<pre class="blocks">(神攝手 色塊 [Y v] :: #00A4E4)</pre>

```typescript
let blobY = mbc.camBlob(MbcCamBlob.Y)   // 色塊 Y：0~75
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v07" class="ref-item">

### 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊寬 W 0~100；0 = 沒找到 |

<pre class="blocks">(神攝手 色塊 [寬 v] :: #00A4E4)</pre>

```typescript
let blobW = mbc.camBlob(MbcCamBlob.Width)   // 色塊寬：0~100
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v08" class="ref-item">

### 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊高 H 0~75；沒找到 = 0 |

<pre class="blocks">(神攝手 色塊 [高 v] :: #00A4E4)</pre>

```typescript
let blobH = mbc.camBlob(MbcCamBlob.Height)   // 色塊高：0~75
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v09" class="ref-item">

### 色塊面積 <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="色塊面積" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~7500` | W×H（0~7500）；沒找到 = 0 |

<pre class="blocks">(神攝手 色塊 [面積 v] :: #00A4E4)</pre>

```typescript
let blobArea = mbc.camBlob(MbcCamBlob.Area)   // 色塊面積：0~7500
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v0a" class="ref-item">

### 有找到色塊 <small>found</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 畫面裡有目標色的色塊（W > 0），0 = 沒有 |

<pre class="blocks"><神攝手 有找到色塊？ :: #00A4E4></pre>

```typescript
let found = mbc.camFound()   // 有找到色塊：0~1
```

**相關積木「當神攝手 色塊 ［找到］」**

```typescript
mbc.onCamBlob(MbcCamEvent.Found, function () {
})
```

<p class="ref-note">感應器沒接好（讀不到）時回「否」。</p>

</section>

## 讀值（WRO 版）

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC 感應器」分類找積木；JavaScript 寫法一併列出，位址 `0x4C`。

<section id="w10" class="ref-item">

### 色點 1~12 顏色碼 <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="色點 1~12 顏色碼" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |

<pre class="blocks">(神攝手 色點 (1) 顏色碼 :: #00A4E4)</pre>

```typescript
let spotColor = mbc.camSpotColor(1)   // 色點 1~12 顏色碼：0~255（N = 1~12）
```

**相關積木「神攝手 色點 ［1~12］ 是 ［彩色］？」**

```typescript
let camSpotIs = mbc.camSpotIs(1, MbcCamSpotKind.Colour)
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="w20" class="ref-item">

### 色點 1~12 鮮豔度 <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="色點 1~12 鮮豔度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

<pre class="blocks">(神攝手 色點 (1) 鮮豔度 :: #00A4E4)</pre>

```typescript
let spotVivid = mbc.camSpotVivid(1)   // 色點 1~12 鮮豔度：0~255（N = 1~12）
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

## 範例（學校版）

<!-- examples: sensors/esp32cam/microbit-examples.md host=microbit sw=makecode whole -->

## 其他問題

- **找不到 MBC 積木？** MakeCode「擴充功能」貼上 https://github.com/contactmbcrobot/pxt-mbc-sensors 加入。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/microbit-makecode.zh.md`、`_build/guides/product/esp32cam/microbit-makecode.zh.md`
- 範例：`sensors/esp32cam/microbit-examples(-wro).md`
