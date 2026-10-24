<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps4 × ev3 × clev3r），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps4/ev3/clev3r -->
# 掌控者 PS4/PS5 × EV3 × clev3r

這一頁只講 **clev3r（EV3 BASIC）**：在 MINDSTORMS EV3 上用它讀掌控者 PS4/PS5的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-clev3r.zh.md + _build/guides/product/ps4/ev3-clev3r.zh.md {"product":"掌控者 PS4/PS5","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS4/PS5：clev3r（EV3 BASIC） 說明](/sensors/ps4/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 1 號孔：`Sensor.SetMode` 選模式、`Sensor.ReadRawValue` 讀第幾個值。

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8，同時按相加（0~15） |

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

<p class="ref-note">模式 0 · int8 · IR-PROX 1×int8；模式 1 · int8 · IR-SEEK [方向鍵, LX, 形狀鍵, LY, 肩鍵, RX, 功能鍵, RY]；模式 3 · int16 · 低 nibble；高 nibble = 形狀鍵；bit8 = 已連線（+256，#67 新增）；模式 4 · int8 · IR-S-ALT [方向鍵, LX, 形狀鍵, LY]（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shapeKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shapeKeys.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8，同時按相加（0~15） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shapeKeys = Sensor.ReadRawValue(1, 2)
Sensor.SetMode(1, 3)
shapeKeys = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
shapeKeys = Sensor.ReadRawValue(1, 2)
```

<p class="ref-note">模式 1 · int8；模式 3 · int16 · 高 nibble（÷16 取整數再 mod 16）；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulderKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-shoulderKeys.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8，同時按相加（0~15） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
shoulderKeys = Sensor.ReadRawValue(1, 4)
```

<p class="ref-note">模式 1 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>funcKeys</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-funcKeys.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT(SHARE) 1、L3 2、R3 4、START(OPTIONS) 8，同時按相加（0~15） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
funcKeys = Sensor.ReadRawValue(1, 6)
```

<p class="ref-note">模式 1 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線且 300 ms 內有資料，0 = 未連線（斷線時按鍵 0、搖桿中間） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
connected = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 3 · int16 · bit8（值 ≥ 256 = 已連線，#67 新增）</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 1~4 軸 <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps4/value-stick.svg" alt="搖桿 1~4 軸" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正；未連線 0 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
stick = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 2)
stick1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
Sensor.SetMode(1, 4)
stick = Sensor.ReadRawValue(1, 1)
```

<p class="ref-note">模式 1 · int8 · LX；LY 在 index 3、RX 5、RY 7（與按鍵交錯）；0~99，50 = 中間（未連線 50）；模式 2 · int8 · IR-REMOTE [LX, LY, RX, RY] 0~99，50 = 中間；模式 4 · int8 · LX；LY 在 index 3（RX、RY 不在模式 4）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：十六鍵、電量、L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps4/arduino/ide.md">Arduino</a>／<a href="/sensors/ps4/mblock/mblock5.md">mBlock</a>／<a href="/sensors/ps4/microbit/makecode.md">micro:bit</a>。</p>

## 命令

EV3 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/ps4/ev3-examples.md host=ev3 sw=clev3r -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps4.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-clev3r.zh.md`、`_build/guides/product/ps4/ev3-clev3r.zh.md`
- 範例：`sensors/ps4/ev3-examples.md`
