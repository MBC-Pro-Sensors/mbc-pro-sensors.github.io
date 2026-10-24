<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/ev3/pybricks -->
# 測距者 2 路 (TOF2) × EV3 × Pybricks

這一頁只講 **Pybricks v4／EV3 MicroPython v2**：在 MINDSTORMS EV3 上用它讀測距者 2 路 (TOF2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-pybricks.zh.md + _build/guides/product/tof2/ev3-pybricks.zh.md {"product":"測距者 2 路 (TOF2)","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 2 路 (TOF2)：Pybricks v4／EV3 MicroPython v2 說明](/sensors/tof2/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 1 號孔，用 `LUMPDevice` 讀原始模式；EV3 MicroPython v2 寫法相同。

<section id="v01" class="ref-item">

### 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

leftCm = sensor.read(1)[0]   # 左距離 cm：0~120 cm
leftCm = sensor.read(2)[0]   # 左距離 cm：0~120 cm
leftCm = sensor.read(4)[0]   # 左距離 cm：0~120 cm
```

<p class="ref-note">模式 1 · int8 · #67 新：IR-SEEK 8×int8 = [左, 右, 左, 右, 左, 右, 左, 右]（index 0/2/4/6 = 左；原本 index 2~7 = 0）；模式 2 · int8 · IR-REMOTE [左, 右, 0, 0]（INO:137-139）；模式 4 · int8 · IR-S-ALT 同模式 2（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v02" class="ref-item">

### 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

rightCm = sensor.read(1)[1]   # 右距離 cm：0~120 cm
rightCm = sensor.read(2)[1]   # 右距離 cm：0~120 cm
rightCm = sensor.read(4)[1]   # 右距離 cm：0~120 cm
```

<p class="ref-note">模式 1 · int8 · #67 新：index 1/3/5/7 = 右；模式 2 · int8；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v04" class="ref-item">

### 左距離 mm <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="左距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 左邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

leftMm = sensor.read(5)[0]   # 左距離 mm：0~1200 mm
```

<p class="ref-note">模式 5 · int16 · #67 新：IR-CAL 2×int16 = [左 mm, 右 mm]（原本 word0 = 左'+256×右'）</p>

</section>

<section id="v05" class="ref-item">

### 右距離 mm <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="右距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 右邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

rightMm = sensor.read(5)[1]   # 右距離 mm：0~1200 mm
```

<p class="ref-note">模式 5 · int16 · #67 新</p>

</section>

<section id="v08" class="ref-item">

### 左右平均 mm <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="左右平均 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | (左 mm + 右 mm) / 2，0~1200；跟 EV3 模式 3、SPIKE 模式 6 第 3 個值一致（共識 #8 補上） |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

avgMm = sensor.read(0)[0]   # 左右平均 mm：0~1200 mm
avgMm = sensor.read(3)[0]   # 左右平均 mm：0~1200 mm
```

<p class="ref-note">模式 0 · int8 · IR-PROX = (左'+右')>>1，cm 縮放後平均（鎖住，INO:131-133）；模式 3 · int16 · #67 新：IR-REM-A 1×int16 = 左右平均 mm（原本 = 左 0~100）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：較近距離 cm、初始化位元圖、故障位元圖。要用這些值請改用 <a href="/sensors/tof2/arduino/ide.md">Arduino</a>／<a href="/sensors/tof2/mblock/mblock5.md">mBlock</a>／<a href="/sensors/tof2/microbit/makecode.md">micro:bit</a>。</p>

## 範例

<!-- examples: sensors/tof2/ev3-examples.md host=ev3 sw=pybricks -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-pybricks.zh.md`、`_build/guides/product/tof2/ev3-pybricks.zh.md`
- 範例：`sensors/tof2/ev3-examples.md`
