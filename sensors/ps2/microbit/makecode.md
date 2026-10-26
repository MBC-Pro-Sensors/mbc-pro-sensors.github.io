<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/microbit/makecode -->
# 掌控者 PS2 (PS2) × micro:bit × MakeCode

這一頁只講 **MakeCode（積木／JavaScript）**：在 BBC micro:bit V1／V2 上用它讀掌控者 PS2 (PS2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x42`。這款耗電小，VCC 可以先接 micro:bit 的 **3V** 腳（V1 約 90 mA、V2 約 190 mA）；用電池供電時電壓可能掉到 3 V 以下，不穩就改外接 5 V（GND 要和 micro:bit 共地）。

<figure class="ref-fig"><img src="/images/figures/ps2/wiring-microbit.svg" alt="掌控者 PS2 (PS2) × BBC micro:bit V1／V2" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/microbit-makecode.zh.md + _build/guides/product/ps2/microbit-makecode.zh.md {"product":"掌控者 PS2 (PS2)","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS2 (PS2)：MakeCode（積木／JavaScript） 說明](/sensors/ps2/microbit.md)。
<!-- /include-fallback -->

## 讀值

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC 感應器」分類找積木；JavaScript 寫法一併列出，位址 `0x42`。

<section id="v01" class="ref-item">

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 16 個按鍵合成一個數（PSX 排列，1 = 按下）：bit0 SELECT、1 L3、2 R3、3 START、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；未連線 = 0 |

<pre class="blocks">(掌控者 十六鍵位元圖 :: #00A4E4)</pre>

```typescript
let buttons16 = mbc.padButtons()   // 十六鍵：0~65535 bit
```

**相關積木「掌控者 按鈕 ［✕］ 被按住？」**

```typescript
let padButtonPressed = mbc.padButtonPressed(MbcPadButton.Cross)
```

**相關積木「當掌控者 按鈕 ［✕］ ［按下］」**

```typescript
mbc.onPadButton(MbcPadButton.Cross, MbcPadButtonEvent.Pressed, function () {
})
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |

<pre class="blocks">(掌控者 [方向鍵 v]（0~15） :: #00A4E4)</pre>

```typescript
let dpad = mbc.padGroup(MbcPadGroup.Dpad)   // 方向鍵：0~15 bit
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |

<pre class="blocks">(掌控者 [形狀鍵 v]（0~15） :: #00A4E4)</pre>

```typescript
let shape = mbc.padGroup(MbcPadGroup.Shapes)   // 形狀鍵：0~15 bit
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |

<pre class="blocks">(掌控者 [肩鍵 v]（0~15） :: #00A4E4)</pre>

```typescript
let shoulder = mbc.padGroup(MbcPadGroup.Shoulders)   // 肩鍵：0~15 bit
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |

<pre class="blocks">(掌控者 [功能鍵 v]（0~15） :: #00A4E4)</pre>

```typescript
let func = mbc.padGroup(MbcPadGroup.Functions)   // 功能鍵：0~15 bit
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |

<pre class="blocks"><掌控者 手把已連線？ :: #00A4E4></pre>

```typescript
let connected = mbc.padConnected()   // 已連線：0~1
```

<p class="ref-note">感應器沒接好（讀不到）時回「否」。</p>

</section>

<section id="v07" class="ref-item">

### 電量 <small>battery</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0` % | 0~100；PS2 手把沒有電量 → 恆 0（PS4 才有） |

<pre class="blocks">(掌控者 電量（百分比） :: #00A4E4)</pre>

```typescript
let battery = mbc.padBattery()   // 電量：0 %
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v08" class="ref-item">

### L2 類比 <small>l2Analog</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | L2 壓力 0~255（DualShock 2 類比模式才有壓力值；手把沒有壓力值時按下 = 255、放開 = 0）；未連線 = 0 |

<pre class="blocks">(掌控者 扳機 [L2 v] 類比值 :: #00A4E4)</pre>

```typescript
let l2Analog = mbc.padTrigger(MbcPadTrigger.L2)   // L2 類比：0~255
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v09" class="ref-item">

### R2 類比 <small>r2Analog</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | R2 壓力 0~255（同 08）；未連線 = 0 |

<pre class="blocks">(掌控者 扳機 [R2 v] 類比值 :: #00A4E4)</pre>

```typescript
let r2Analog = mbc.padTrigger(MbcPadTrigger.R2)   // R2 類比：0~255
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v0a" class="ref-item">

### 系統鍵 <small>sysKeys</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0` bit | b0 PS 鍵、b1 擷取鍵；PS2 手把沒有 → 恆 0（PS4 才有） |

擴充沒有這顆積木；在 JavaScript 用 `mbc.readValue(位址, 編號)` 讀（讀不到 = −32768）。相關積木：「掌控者 系統鍵 ［PS］ 被按住？」。

```typescript
let sysKeys = mbc.readValue(0x42, 0x0A)   // 系統鍵：0 bit
```

</section>

<section id="v0b" class="ref-item">

### 手把類型 <small>padType</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~2` | 0 沒有手把、1 PS2 數位模式、2 PS2 類比模式（3 PS4、4 PS5 只有 PS4 接收器會出現） |

<pre class="blocks">(掌控者 手把類型 :: #00A4E4)</pre>

```typescript
let padType = mbc.padType()   // 手把類型：0~2
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

<pre class="blocks">(掌控者 搖桿 [左搖桿 X v] :: #00A4E4)</pre>

```typescript
let stick = [mbc.padStick(MbcPadAxis.LeftX), mbc.padStick(MbcPadAxis.LeftY), mbc.padStick(MbcPadAxis.RightX), mbc.padStick(MbcPadAxis.RightY)]   // 搖桿 LX／LY／RX／RY：-100~+100
```

<p class="ref-note">感應器沒接好（讀不到）時回 0，先用「位址 … 的裝置就緒？」確認接線。</p>

</section>

## 命令

<section id="ce5" class="ref-item ref-cmd">

### 震動 <small>vibrate</small>

[強度 0~255]，0 = 停；PS2 接收器收到後回 ACK 但忽略（250614 已關手把震動），PS4 才有作用

<pre class="blocks">掌控者 震動 強度 (200) :: #00A4E4</pre>

```typescript
mbc.padRumble(200)   // 震動
```

</section>

<section id="ce6" class="ref-item ref-cmd">

### 燈條顏色 <small>setLed</small>

[R][G][B] 各 0~255；PS2 手把沒有燈條，接收器收到後回 ACK 但忽略，PS4 才有作用

<pre class="blocks">掌控者 燈條顏色 [#ff0000] :: #00A4E4</pre>

```typescript
mbc.padLight(0xff0000)   // 燈條顏色
```

</section>

## 範例

<!-- examples: sensors/ps2/microbit-examples.md host=microbit sw=makecode whole -->

## 其他問題

- **找不到 MBC 積木？** MakeCode「擴充功能」貼上 https://github.com/contactmbcrobot/pxt-mbc-sensors 加入。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/microbit-makecode.zh.md`、`_build/guides/product/ps2/microbit-makecode.zh.md`
- 範例：`sensors/ps2/microbit-examples.md`
