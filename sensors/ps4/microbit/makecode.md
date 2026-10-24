<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × microbit × makecode），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/microbit/makecode -->
# 掌控者 PS4/PS5 × micro:bit × MakeCode

這一頁只講 **MakeCode（積木／JavaScript）**：在 BBC micro:bit V1／V2 上用它讀掌控者 PS4/PS5的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 要買哪個版本、怎麼接

買 **通用 I2C 版**，接 micro:bit 擴充板的 I2C 孔（SDA = P20、SCL = P19），出廠位址 `0x42`。感應器要 5 V 供電，micro:bit 本身只有 3.3 V，請用有 5 V 輸出的擴充板。

<figure class="ref-fig"><img src="/images/figures/ps4/wiring-microbit.svg" alt="掌控者 PS4/PS5 × BBC micro:bit V1／V2" loading="lazy"></figure>

## 安裝與設定

<!-- setup: _build/guides/software/microbit-makecode.zh.md + _build/guides/product/ps4/microbit-makecode.zh.md {"product":"掌控者 PS4/PS5","edition":"通用 I2C 版","port":"I2C","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS4/PS5：MakeCode（積木／JavaScript） 說明](/sensors/ps4/microbit.md)。
<!-- /include-fallback -->

## 讀值

MakeCode 加入 pxt-mbc-sensors 擴充後，在「MBC」分類找積木；JavaScript 寫法一併列出，位址 `0x42`。

<section id="v01" class="ref-item">

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 全部按鍵一個 16 bit（PSX 排列）：bit0 SELECT(SHARE)、1 L3、2 R3、3 START(OPTIONS)、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；1 = 按下；未連線 = 0 |

<pre class="blocks">(MBC 十六鍵 :: #E8711E)</pre>

```typescript
let buttons16 = MBC.readValue(0x42, 0x01)   // 十六鍵：0~65535 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8，同時按相加（0~15） |

<pre class="blocks">(MBC 方向鍵 :: #E8711E)</pre>

```typescript
let dpad = MBC.readValue(0x42, 0x02)   // 方向鍵：0~15 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8，同時按相加（0~15） |

<pre class="blocks">(MBC 形狀鍵 :: #E8711E)</pre>

```typescript
let shapeKeys = MBC.readValue(0x42, 0x03)   // 形狀鍵：0~15 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8，同時按相加（0~15） |

<pre class="blocks">(MBC 肩鍵 :: #E8711E)</pre>

```typescript
let shoulderKeys = MBC.readValue(0x42, 0x04)   // 肩鍵：0~15 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1、L3 2、R3 4、START(OPTIONS) 8，同時按相加（0~15） |

<pre class="blocks">(MBC 功能鍵 :: #E8711E)</pre>

```typescript
let funcKeys = MBC.readValue(0x42, 0x05)   // 功能鍵：0~15 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線且 300 ms 內有資料，0 = 未連線（斷線時按鍵 0、搖桿中間） |

<pre class="blocks">(MBC 手把已連線？ :: #E8711E)</pre>

```typescript
let connected = MBC.readValue(0x42, 0x06)   // 已連線：0~1
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v07" class="ref-item">

### 電量 <small>battery</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-battery.svg" alt="電量" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` % | 手把電量 0~100（PS2 接收器恆 0） |

<pre class="blocks">(MBC 電量 :: #E8711E)</pre>

```typescript
let battery = MBC.readValue(0x42, 0x07)   // 電量：0~100 %
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v08" class="ref-item">

### L2 類比 <small>l2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-l2Analog.svg" alt="L2 類比" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | L2 扳機壓多深 0~255（未連線 0） |

<pre class="blocks">(MBC L2 類比 :: #E8711E)</pre>

```typescript
let l2Analog = MBC.readValue(0x42, 0x08)   // L2 類比：0~255
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v09" class="ref-item">

### R2 類比 <small>r2Analog</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-r2Analog.svg" alt="R2 類比" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | R2 扳機壓多深 0~255（未連線 0） |

<pre class="blocks">(MBC R2 類比 :: #E8711E)</pre>

```typescript
let r2Analog = MBC.readValue(0x42, 0x09)   // R2 類比：0~255
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v0a" class="ref-item">

### 系統鍵 <small>sysKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-sysKeys.svg" alt="系統鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | b0 PS 鍵、b1 擷取（PS5 靜音）鍵；未連線 0（PS2 恆 0） |

<pre class="blocks">(MBC 系統鍵 :: #E8711E)</pre>

```typescript
let sysKeys = MBC.readValue(0x42, 0x0A)   // 系統鍵：0~3 bit
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v0b" class="ref-item">

### 手把類型 <small>padType</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~4` | 0 無、1 PS2 數位、2 PS2 類比、3 PS4、4 PS5（或其他） |

<pre class="blocks">(MBC 手把類型 :: #E8711E)</pre>

```typescript
let padType = MBC.readValue(0x42, 0x0B)   // 手把類型：0~4
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 1~4 軸 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="搖桿 1~4 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正；未連線 0 |

<pre class="blocks">(MBC 搖桿 [軸] :: #E8711E)</pre>

```typescript
let stick = MBC.readValues(0x42, 0x10, 4)   // 搖桿 1~4 軸
```

<p class="ref-note">擴充內部用 <code>Int16LE</code> 讀 2 byte；寫完編號 <code>basic.pause(2)</code> 再讀。</p>

</section>

## 命令

<section id="ce5" class="ref-item ref-cmd">

### 震動 <small>rumble</small>

[強度 0~255]，0 = 停；每次 200 ms（PS2 收到忽略）

<pre class="blocks">MBC 震動 [強度] :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE5, [50])   // 震動
```

</section>

<section id="ce6" class="ref-item ref-cmd">

### 燈條顏色 <small>lightbar</small>

[R][G][B] 各 0~255，要剛好 3 byte；未連線先記住，連上再套用；預設藍 (0,0,255)（PS2 收到忽略）

<pre class="blocks">MBC 燈條 R[r] G[g] B[b] :: #E8711E</pre>

```typescript
MBC.writeCmd(0x42, 0xE6, [50, 50, 50])   // 燈條顏色
```

</section>

## 範例

<!-- examples: sensors/ps4/microbit-examples.md host=microbit sw=makecode whole -->

## 其他問題

- **找不到 MBC 積木？** MakeCode「擴充功能」貼上 https://github.com/contactmbcrobot/pxt-mbc-sensors 加入。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MakeCode 擴充 pxt-mbc-sensors（在 MakeCode「擴充功能」貼上這個網址）](https://github.com/contactmbcrobot/pxt-mbc-sensors)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps4.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/microbit-makecode.zh.md`、`_build/guides/product/ps4/microbit-makecode.zh.md`
- 範例：`sensors/ps4/microbit-examples.md`
