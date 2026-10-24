<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × ev3 × clev3r），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/ev3/clev3r -->
# 測距者 8 路 (TOF8) × EV3 × clev3r

這一頁只講 **clev3r（EV3 BASIC）**：在 MINDSTORMS EV3 上用它讀測距者 8 路 (TOF8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-clev3r.zh.md + _build/guides/product/tof8/ev3-clev3r.zh.md {"product":"測距者 8 路 (TOF8)","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 8 路 (TOF8)：clev3r（EV3 BASIC） 說明](/sensors/tof8/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 1 號孔：`Sensor.SetMode` 選模式、`Sensor.ReadRawValue` 讀第幾個值。

<section id="v01" class="ref-item">

### 最近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="最近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
nearCm = Sensor.ReadRawValue(1, 1)
```

<p class="ref-note">模式 2 · int8 · IR-REMOTE [最近方向−8, 最近', 最空曠方向−8, 最空曠']（INO:154-159）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v02" class="ref-item">

### 最近方向 <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="最近方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
nearDir = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 5)
nearDir = Sensor.ReadRawValue(1, 1)
```

<p class="ref-note">模式 2 · int8 · int8 二補數（方位−8）；模式 5 · int16 · #67 新：IR-CAL 2×int16 = [最近 mm, 最近方向 −8~+8]（原本 = 前 4 顆兩兩合併）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v03" class="ref-item">

### 最空曠距離 cm <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="最空曠距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
farCm = Sensor.ReadRawValue(1, 3)
```

<p class="ref-note">模式 2 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v04" class="ref-item">

### 最空曠方向 <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="最空曠方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-7~+7` | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
farDir = Sensor.ReadRawValue(1, 2)
```

<p class="ref-note">模式 2 · int8 · int8 二補數（方位−8）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v05" class="ref-item">

### 正前方 cm <small>frontCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontCm.svg" alt="正前方 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 4、5 顆（兩顆平行朝前）平均 0~120 cm；EV3 模式 3、SPIKE 模式 6 給的是同一個值的 mm；0x22 的 C 是 min(第 4, 第 5 顆) 不是平均（見 10） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
frontCm = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 0 · int8 · IR-PROX = (第 4'+第 5')>>1（鎖住，INO:148-150）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v06" class="ref-item">

### 最近距離 mm <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="最近距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 8 顆 mm 的最小值 0~1200（失效那顆以 1200 參與），1200 = 四周沒東西 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 5)
nearMm = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 5 · int16 · #67 新：[最近 mm, 最近方向]</p>

</section>

<section id="v09" class="ref-item">

### 正前方距離 mm <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="正前方距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 第 4、5 顆（正前方兩顆）平均，0~1200 mm；1200 = 前面沒東西。cm 版是 05 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
frontMm = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 3 · int16 · #67 新：IR-REM-A 1×int16 = 正前方（第 4、5 顆平均）mm（原本 = 第 1 顆 0~100）</p>

</section>

<section id="v10" class="ref-item">

### 第 1~8 顆距離 cm <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="第 1~8 顆距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
distCm1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
Sensor.SetMode(1, 4)
distCm1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
```

<p class="ref-note">模式 1 · int8 · IR-SEEK 8×int8 = 第 1~8 顆（EV3 視為頻道 1~4 的〈方位,距離〉）；模式 4 · int8 · IR-S-ALT = 前 4 顆（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：初始化位元圖、故障位元圖、第 1~8 顆距離 mm。要用這些值請改用 <a href="/sensors/tof8/arduino/ide.md">Arduino</a>／<a href="/sensors/tof8/mblock/mblock5.md">mBlock</a>／<a href="/sensors/tof8/microbit/makecode.md">micro:bit</a>。</p>

## 範例

<!-- examples: sensors/tof8/ev3-examples.md host=ev3 sw=clev3r -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-clev3r.zh.md`、`_build/guides/product/tof8/ev3-clev3r.zh.md`
- 範例：`sensors/tof8/ev3-examples.md`
