<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof8 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof8/spike/pybricks -->
# 測距者 8 路 (TOF8) × SPIKE × Pybricks

這一頁只講 **Pybricks（Python）**：在 SPIKE Prime／Robot Inventor 上用它讀測距者 8 路 (TOF8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-pybricks.zh.md + _build/guides/product/tof8/spike-pybricks.zh.md {"product":"測距者 8 路 (TOF8)","edition":"SPIKE 版","port":"C","lib":"MBC_laser8_obj_Lib.py","libUrl":"/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 8 路 (TOF8)：Pybricks（Python） 說明](/sensors/tof8/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 A 孔。「MBC 函式庫」寫法要先把函式庫 .py 一起匯入 Pybricks。

<section id="v01" class="ref-item">

### 最近距離 cm <small>nearCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearCm.svg" alt="最近距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最小的距離 0~120 cm（失效那顆以 120 參與），120 = 四周沒東西 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
nearCm = ((w >> 8) & 0xFF)   # 最近距離 cm（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearCm = laser.min_distance()
```

<p class="ref-note">模式 2 byte 0；模式 3 byte 1；模式 4 v[0] hi；模式 7 v[2] 🔒；模式 9 v[4] hi 🔒。備註：[最近方向, 最近距離, 第 1 顆]（第 4 byte 第 8 顆被描述表砍掉）; Pybricks ambient() = int16[2]/100 = 最近 cm（INO:348-361）</p>

</section>

<section id="v02" class="ref-item">

### 最近方向 <small>nearDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearDir.svg" alt="最近方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 最近目標在哪個方向：−7（正左）~ +7（正右），0 = 正前方，負 = 左；±8 = 四周沒東西（沿用上次偏向側） |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
nearDir = (w & 0xFF) - 8   # 最近方向（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearDir = laser.min_direction()
```

<p class="ref-note">模式 1 byte 0 −8；模式 3 byte 0 −8；模式 4 v[0] lo −8；模式 5 v[0] 🔒；模式 6 v[2]；模式 9 v[4] lo −8 🔒。備註：原始方位 0~16（8 = 正前）; Pybricks reflection() 編碼：x = 方位−8，負數 +6399，a = x/25、b = 10×(x mod 25)+9，3 個 int16 = a×256+b → reflection() 向零截斷 = −8~+8（INO:319-336）; #67 新：HSV 3×int16 = [最近 mm, 正前方 mm, 最近方向 −8~+8]</p>

</section>

<section id="v03" class="ref-item">

### 最空曠距離 cm <small>farCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farCm.svg" alt="最空曠距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 8 顆裡最大的距離 0~120 cm（壞掉那顆永遠 120，會被當成最空曠） |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
farCm = ((w >> 8) & 0xFF)   # 最空曠距離 cm（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
farCm = laser.max_distance()
```

<p class="ref-note">模式 9 v[5] hi 🔒</p>

</section>

<section id="v04" class="ref-item">

### 最空曠方向 <small>farDir</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-farDir.svg" alt="最空曠方向" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-7~+7` | 最遠（最空曠）在哪個方向：−7 ~ +7，0 = 正前方，負 = 左；8 顆全 120 時 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
farDir = (w & 0xFF) - 8   # 最空曠方向（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
farDir = laser.max_direction()
```

<p class="ref-note">模式 9 v[5] lo −8 🔒</p>

</section>

<section id="v06" class="ref-item">

### 最近距離 mm <small>nearMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-nearMm.svg" alt="最近距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 8 顆 mm 的最小值 0~1200（失效那顆以 1200 參與），1200 = 四周沒東西 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

nearMm = dev.read(6)[0]   # 最近距離 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
nearMm = laser.nearest_mm()
```

<p class="ref-note">模式 6 v[0]。備註：#67 新：[最近 mm, 正前方 mm, 方向]</p>

</section>

<section id="v07" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 顆（最左）… bit7 = 第 8 顆，1 = 那顆開機初始化成功（8 顆約 2~4 秒，失敗不重試）；255 = 全部好 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[6]
initOk = (w & 0xFF)   # 初始化位元圖（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
initOk = laser.is_ok(1)
```

<p class="ref-note">模式 9 v[6] lo 🔒</p>

</section>

<section id="v08" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 顆 … bit7 = 第 8 顆，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[6]
fault = ((w >> 8) & 0xFF)   # 故障位元圖（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
fault = laser.is_ok(1)
```

<p class="ref-note">模式 9 v[6] hi 🔒</p>

</section>

<section id="v09" class="ref-item">

### 正前方距離 mm <small>frontMm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-frontMm.svg" alt="正前方距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 第 4、5 顆（正前方兩顆）平均，0~1200 mm；1200 = 前面沒東西。cm 版是 05 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

frontMm = dev.read(6)[1]   # 正前方距離 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
frontMm = laser.front_mm()
```

<p class="ref-note">模式 6 v[1]。備註：#67 新：正前方 mm</p>

</section>

<section id="v10" class="ref-item">

### 第 1~8 顆距離 cm <small>distCm</small>

<figure class="ref-fig"><img src="/images/figures/tof8/value-distCm.svg" alt="第 1~8 顆距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 第 1~8 顆各自的距離 0~120 cm（第 1 顆正左、第 4、5 顆正前、第 8 顆正右，每顆隔 30°）；120 = 那個方向沒東西／那顆失效；編號 10~17 |

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

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser8_obj_Lib import MBC_LASER8

laser = MBC_LASER8(Port.A)
distCm = laser.distances()   # list，第 1 路 = [0]
```

<p class="ref-note">模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 0 🔒；模式 3 byte 2；模式 4 v[1]；模式 8 v[0]；模式 9 v[0] 🔒。備註：16 B = [第 1~8 顆, 0×8]：App 顏色 = 第 1 顆、反射光 = 第 2 顆、原始紅 = 第 3+256×第 4 …; byte0/1 = 第 1、2 顆 ×10/120（0~10）；byte2~7 = 第 3~8 顆兩顆一組 4-bit 再查表編碼（spikeEncodeU8，INO:259-284）; byte2 = 第 1 顆（byte3 = 第 8 顆被描述表砍掉）; v1 = 第 1 顆 + 256×第 8 顆; 只送 4 B：v0 = 第 1+256×第 2、v1 = 第 3+256×第 4; 每 int16 兩顆：lo = 第 2k+1 顆、hi = 第 2k+2 顆（v0~v3 = 第 1~8 顆）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：正前方 cm、第 1~8 顆距離 mm。要用這些值請改用 <a href="/sensors/tof8/arduino/ide.md">Arduino</a>／<a href="/sensors/tof8/mblock/mblock5.md">mBlock</a>／<a href="/sensors/tof8/microbit/makecode.md">micro:bit</a>。</p>

## 範例

<!-- examples: sensors/tof8/spike-examples.md host=spike sw=pybricks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [Pybricks 函式庫 MBC_laser8_obj_Lib.py](/examples/tof8/pybricks/For%20firmware%204.0.0/MBC_laser8_obj_Lib.py)
- [tof8_pybricks_spike_v3.6.1.zip](/examples/tof8/tof8_pybricks_spike_v3.6.1.zip)
- [tof8_pybricks_spike_v4.0.0.zip](/examples/tof8/tof8_pybricks_spike_v4.0.0.zip)
- [MBC_TOF8_Pybricks_Lib.zip](/downloads/MBC_TOF8_Pybricks_Lib.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-pybricks.zh.md`、`_build/guides/product/tof8/spike-pybricks.zh.md`
- 範例：`sensors/tof8/spike-examples.md`
