<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × spike × mbcblocks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/spike/mbcblocks -->
# 測距者 8 路 (TOF8) × SPIKE × MBC 積木網站

這一頁只講 **MBC SPIKE 積木網站**：在 SPIKE Prime／Robot Inventor 上用它讀測距者 8 路 (TOF8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-mbcblocks.zh.md + _build/guides/product/tof8/spike-mbcblocks.zh.md {"product":"測距者 8 路 (TOF8)","edition":"SPIKE 版","port":"C","lib":"MBC_laser8_obj_Lib.py","libUrl":"/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 8 路 (TOF8)：MBC SPIKE 積木網站 說明](/sensors/tof8/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

主機要先換成 Pybricks 韌體。MBC 積木網站的感應器積木讀的是感應器的模式 9（一次讀完全部數值）。

<section id="v01" class="ref-item">

### 最近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="最近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |

在 MBC 積木網站加入這款感應器的積木，用「最近距離 cm」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
nearCm = ((w >> 8) & 0xFF)   # 最近距離 cm（高 byte）
```

</section>

<section id="v02" class="ref-item">

### 最近方向 <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="最近方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |

在 MBC 積木網站加入這款感應器的積木，用「最近方向」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
nearDir = (w & 0xFF) - 8   # 最近方向（低 byte）
```

</section>

<section id="v03" class="ref-item">

### 最空曠距離 cm <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="最空曠距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |

在 MBC 積木網站加入這款感應器的積木，用「最空曠距離 cm」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
farCm = ((w >> 8) & 0xFF)   # 最空曠距離 cm（高 byte）
```

</section>

<section id="v04" class="ref-item">

### 最空曠方向 <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="最空曠方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-7~+7` | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「最空曠方向」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
farDir = (w & 0xFF) - 8   # 最空曠方向（低 byte）
```

</section>

<section id="v07" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 顆（最左）… bit7 = 第 8 顆，1 = 那顆開機初始化成功（8 顆約 2~4 秒，失敗不重試）；255 = 全部好 |

在 MBC 積木網站加入這款感應器的積木，用「初始化位元圖」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[6]
initOk = (w & 0xFF)   # 初始化位元圖（低 byte）
```

</section>

<section id="v08" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 顆 … bit7 = 第 8 顆，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

在 MBC 積木網站加入這款感應器的積木，用「故障位元圖」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[6]
fault = ((w >> 8) & 0xFF)   # 故障位元圖（高 byte）
```

</section>

<section id="v10" class="ref-item">

### 第 1~8 顆距離 cm <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="第 1~8 顆距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |

在 MBC 積木網站加入這款感應器的積木，用「第 1~8 顆距離 cm」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

words = dev.read(9)[0:4]
distCm = []
for w in words:
    distCm.append(w & 0xFF)          # 奇數路
    distCm.append((w >> 8) & 0xFF)   # 偶數路
```

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：正前方 cm、最近距離 mm、正前方距離 mm、第 1~8 顆距離 mm。要用這些值請改用 <a href="/sensors/tof8/spike/pybricks.md">Pybricks</a>。</p>

## 範例

<!-- examples: sensors/tof8/spike-examples.md host=spike sw=mbcblocks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MBC SPIKE 積木網站（線上，不用安裝）](https://mbc-spike-blocks.mbcrobot.workers.dev)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-mbcblocks.zh.md`、`_build/guides/product/tof8/spike-mbcblocks.zh.md`
- 範例：`sensors/tof8/spike-examples.md`
