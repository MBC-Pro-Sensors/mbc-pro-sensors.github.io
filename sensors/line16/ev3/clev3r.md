<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × ev3 × clev3r），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/ev3/clev3r -->
# 循行者 16 路 (Line16) × EV3 × clev3r

這一頁只講 **clev3r（EV3 BASIC）**：在 MINDSTORMS EV3 上用它讀循行者 16 路 (Line16)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-clev3r.zh.md + _build/guides/product/line16/ev3-clev3r.zh.md {"product":"循行者 16 路 (Line16)","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 16 路 (Line16)：clev3r（EV3 BASIC） 說明](/sensors/line16/ev3-clev3r.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 1 號孔：`Sensor.SetMode` 選模式、`Sensor.ReadRawValue` 讀第幾個值。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 0)
linePos = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 0 · int8 · IR-PROX（F16:739-741）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
linePosFine = Sensor.ReadRawValue(1, 0)
Sensor.SetMode(1, 4)
linePosFine = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 2 · int8 · IR-REMOTE byte0 = highRes−100 存成 int8（F16:745-751）；模式 4 · int8 · #67 新：IR-S-ALT 內容同模式 2（原本不送）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~16` 路 | 壓到線的路數 0~16，0 = 找不到線 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 2)
lineWidth = Sensor.ReadRawValue(1, 1)
Sensor.SetMode(1, 4)
lineWidth = Sensor.ReadRawValue(1, 1)
```

<p class="ref-note">模式 2 · int8；模式 4 · int8 · #67 新：同模式 2（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | bit0 = 第 1 路（最左）… bit15 = 第 16 路，1 = 那一路在線上 |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 3)
bitmap = Sensor.ReadRawValue(1, 0)
```

<p class="ref-note">模式 3 · int16 · #67 新：IR-REM-A 1×int16 = 16 bit 位元圖（原本不送）</p>

</section>

<section id="v10" class="ref-item">

### 第 1~16 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="第 1~16 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

```basic
' clev3r (BASIC)
Sensor.SetMode(1, 1)
light1 = Sensor.ReadRawValue(1, 0)   ' 第 1 筆；第 N 筆改成 0 + N - 1
Sensor.SetMode(1, 2)
light1 = Sensor.ReadRawValue(1, 2)   ' 第 1 筆；第 N 筆改成 2 + N - 1
Sensor.SetMode(1, 4)
light1 = Sensor.ReadRawValue(1, 2)   ' 第 1 筆；第 N 筆改成 2 + N - 1
```

<p class="ref-note">模式 1 · int8 · IR-SEEK 8×int8 兩路壓一 byte：byte i = convert9(第 2i+1 路)×10 + convert9(第 2i+2 路)（11~99；convert9：0~11→1、12~22→2 … 89~100→9，F16:716-723）；模式 2 · int8 · index 2 = 第 1 路、index 3 = 第 16 路（0~100）；模式 4 · int8 · #67 新：同模式 2（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：線段數、最後離線方向、二值化門檻、校準完成次數。要用這些值請改用 <a href="/sensors/line16/arduino/ide.md">Arduino</a>／<a href="/sensors/line16/mblock/mblock5.md">mBlock</a>／<a href="/sensors/line16/microbit/makecode.md">micro:bit</a>。</p>

## 命令

EV3 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/line16/ev3-examples.md host=ev3 sw=clev3r -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line16.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-clev3r.zh.md`、`_build/guides/product/line16/ev3-clev3r.zh.md`
- 範例：`sensors/line16/ev3-examples.md`
