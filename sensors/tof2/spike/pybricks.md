<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/spike/pybricks -->
# 測距者 2 路 (TOF2) × SPIKE × Pybricks

這一頁只講 **Pybricks（Python）**：在 SPIKE Prime／Robot Inventor 上用它讀測距者 2 路 (TOF2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-pybricks.zh.md + _build/guides/product/tof2/spike-pybricks.zh.md {"product":"測距者 2 路 (TOF2)","edition":"SPIKE 版","port":"C","lib":"MBC_laser2_obj_Lib.py","libUrl":"/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 2 路 (TOF2)：Pybricks（Python） 說明](/sensors/tof2/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 A 孔。「MBC 函式庫」寫法要先把函式庫 .py 一起匯入 Pybricks。

<section id="v01" class="ref-item">

### 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
leftCm = (w & 0xFF)   # 左距離 cm（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
leftCm = laser.left()
```

<p class="ref-note">模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 0 🔒；模式 1 byte 0；模式 2 byte 0；模式 3 byte 0 🔒；模式 4 v[0] lo；模式 5 v[0] 🔒；模式 8 v[0] lo；模式 9 v[0] lo 🔒。備註：16 B = [左, 右, 0, 左, 0, 右, 0, 平均, 0×8]：App 顏色 = 左、原始紅 = 256×左; 8 B 原樣（不壓縮）; 韌體註解寫「最近」，實際送左（INO:261-263）; 網站 read(3) 在用：[左, 右, 0]; Pybricks reflection() 編碼：L = 左 cm，a = L/25、b = 10×(L mod 25)+9，3 個 int16 = a×256+b → reflection() = L; 只送 4 B，v0 = 左 + 256×右</p>

</section>

<section id="v02" class="ref-item">

### 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
rightCm = ((w >> 8) & 0xFF)   # 右距離 cm（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
rightCm = laser.right()
```

<p class="ref-note">模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 1 🔒；模式 3 byte 1 🔒；模式 4 v[0] hi；模式 7 v[2] 🔒；模式 8 v[0] hi；模式 9 v[0] hi 🔒。備註：App 反射光 = 右、原始綠 = 256×右; 網站 read(3) 在用; Pybricks ambient() = int16[2]/100 = 右 cm（INO:300-312）</p>

</section>

<section id="v04" class="ref-item">

### 左距離 mm <small>leftMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftMm.svg" alt="左距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 左邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

leftMm = dev.read(6)[0]   # 左距離 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
leftMm = laser.left_mm()
```

<p class="ref-note">模式 6 v[0]。備註：#67 新：HSV 3×int16 = [左 mm, 右 mm, 平均 mm]（原本是基底緩衝合併值）</p>

</section>

<section id="v05" class="ref-item">

### 右距離 mm <small>rightMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightMm.svg" alt="右距離 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | 右邊那顆的距離 0~1200 mm，1200 = 前面沒東西／那顆失效 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

rightMm = dev.read(6)[1]   # 右距離 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
rightMm = laser.right_mm()
```

<p class="ref-note">模式 6 v[1]。備註：#67 新</p>

</section>

<section id="v06" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不重試）；3 = 兩顆都好 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
initOk = (w & 0xFF)   # 初始化位元圖（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
initOk = laser.is_ok(1)
```

<p class="ref-note">模式 9 v[1] lo 🔒</p>

</section>

<section id="v07" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
fault = ((w >> 8) & 0xFF)   # 故障位元圖（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
fault = laser.is_ok(1)
```

<p class="ref-note">模式 9 v[1] hi 🔒</p>

</section>

<section id="v08" class="ref-item">

### 左右平均 mm <small>avgMm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-avgMm.svg" alt="左右平均 mm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1200` mm | (左 mm + 右 mm) / 2，0~1200；跟 EV3 模式 3、SPIKE 模式 6 第 3 個值一致（共識 #8 補上） |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

avgMm = dev.read(0)[7]   # 左右平均 mm
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_laser2_obj_Lib import MBC_LASER2

laser = MBC_LASER2(Port.A)
avgMm = laser.average_mm()
```

<p class="ref-note">模式 0 (edu) byte 7 🔒；模式 6 v[2]。備註：byte7 = (左 cm + 右 cm)>>1；App 原始藍 = 256×平均; #67 新：平均 mm</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：較近距離 cm。要用這些值請改用 <a href="/sensors/tof2/arduino/ide.md">Arduino</a>／<a href="/sensors/tof2/mblock/mblock5.md">mBlock</a>／<a href="/sensors/tof2/microbit/makecode.md">micro:bit</a>。</p>

## 範例

<!-- examples: sensors/tof2/spike-examples.md host=spike sw=pybricks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [Pybricks 函式庫 MBC_laser2_obj_Lib.py](/examples/tof2/pybricks/For%20firmware%204.x/MBC_laser2_obj_Lib.py)
- [共用函式庫（新專案用，一個檔適用全部 MBC 感應器） MBC_sensors.py](/examples/tof2/pybricks/For%20firmware%204.x/MBC_sensors.py)
- [tof2_pybricks_spike_v4.x.zip](/examples/tof2/tof2_pybricks_spike_v4.x.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-pybricks.zh.md`、`_build/guides/product/tof2/spike-pybricks.zh.md`
- 範例：`sensors/tof2/spike-examples.md`
