<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × ev3 × clev3r），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/ev3/clev3r -->
# 掌控者 PS2 (PS2) × EV3 × clev3r

這一頁只講 **clev3r（EV3 BASIC）**：在 MINDSTORMS EV3 上用它讀掌控者 PS2 (PS2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-clev3r.zh.md + _build/guides/product/ps2/ev3-clev3r.zh.md {"product":"掌控者 PS2 (PS2)","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS2 (PS2)：clev3r（EV3 BASIC） 說明](/sensors/ps2/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 1 號孔：`Sensor.SetMode` 選模式、`Sensor.ReadRawValue` 讀第幾個值。

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 1)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 3)
dpad = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
dpad = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 0 · int8 · IR-PROX = 方向鍵（.ino:130-132）；模式 1 · int8 · IR-SEEK [方向, LX, 形狀, LY, 肩, RX, 功能, RY]（按鍵與搖桿交錯）；模式 3 · int16 · #67 改：IR-REM-A 1×int16 = 方向 | 形狀<<4 | 已連線<<8（bit0-3 = 方向）；模式 4 · int8 · IR-S-ALT [方向, LX, 形狀, LY]（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shape = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 3)
shape = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
shape = Sensor.ReadRawValue(1, 2)
```

<p class="ref-note">模式 1 · int8；模式 3 · int16 · #67 改：bit4-7 = 形狀（同一個 int16）；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shoulder = Sensor.ReadRawValue(1, 4)
```

<p class="ref-note">模式 1 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
func = Sensor.ReadRawValue(1, 6)
```

<p class="ref-note">模式 1 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
connected = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 3 · int16 · #67 改：bit8 = 已連線（+256）</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
stick1 = Sensor.ReadRawValue(1, 1)   ' 第 1 筆；第 N 筆改成 1 + N - 1
Sensor.SetMode(1, 2)
stick1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
Sensor.SetMode(1, 4)
stick1 = Sensor.ReadRawValue(1, 1)   ' 第 1 筆；第 N 筆改成 1 + N - 1
```

<p class="ref-note">模式 1 · int8 · 交錯：index 1 = LX、3 = LY、5 = RX、7 = RY（偶數 index 是按鍵）；模式 2 · int8 · IR-REMOTE [LX, LY, RX, RY]（描述表宣告 0~10，實送 0~99）；模式 4 · int8 · index 1 = LX、3 = LY（只到左搖桿）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：十六鍵、電量、L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps2/arduino/ide.md">Arduino</a>／<a href="/sensors/ps2/mblock/mblock5.md">mBlock</a>／<a href="/sensors/ps2/microbit/makecode.md">micro:bit</a>。</p>

## 命令

EV3 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/ps2/ev3-examples.md host=ev3 sw=clev3r -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-clev3r.zh.md`、`_build/guides/product/ps2/ev3-clev3r.zh.md`
- 範例：`sensors/ps2/ev3-examples.md`
