<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/spike/pybricks -->
# 循行者 8 路 (Line8) × SPIKE × Pybricks

這一頁只講 **Pybricks（Python）**：在 SPIKE Prime／Robot Inventor 上用它讀循行者 8 路 (Line8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-pybricks.zh.md + _build/guides/product/line8/spike-pybricks.zh.md {"product":"循行者 8 路 (Line8)","edition":"SPIKE 版","port":"C","lib":"MBC_line8_obj_Lib.py","libUrl":"/examples/line8/spike_beginner/pybricks/MBC_line8_obj_Lib.py","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 8 路 (Line8)：Pybricks（Python） 說明](/sensors/line8/spike-pybricks.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 A 孔。「MBC 函式庫」寫法要先把函式庫 .py 一起匯入 Pybricks。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 線在哪一路：−8（最左）~+8（最右），0 = 正中間；站在車後往前看，負 = 線在左 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
linePos = (w & 0xFF) - 8   # 線位置（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
linePos = line.pos8()
```

<p class="ref-note">模式 0 (edu) byte 1 −8 🔒；模式 0 (toy) byte 0 −8 🔒；模式 1 byte 0 −8；模式 5 v[0] 🔒；模式 9 v[4] lo −8 🔒。備註：Pybricks reflection() 編碼：X = (線位置+8)×10 + 線寬（0~168），a = X/25、b = (X−a×25+1)×10−1，3 個 int16 = a×256+b → reflection() ≈ X，主機 ÷10 取整−8 = 線位置、取餘數 = 線寬（INO:947-950）</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
linePosFine = (w & 0xFF) - 100   # 細位置（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
linePosFine = line.pos100()
```

<p class="ref-note">模式 0 (edu) byte 2 −100 🔒；模式 3 byte 0 −100；模式 4 v[0] −100；模式 7 v[0] 🔒；模式 9 v[5] lo −100 🔒。備註：4 個 int16：第 1、2、4 格 = (細位置+100)×100 + 線寬（0~20008；Pybricks hsv(surface=False).h 讀第 1 格，原封不動）；第 3 格 = (細位置+100)×100 + min(線寬,9)×10（Pybricks ambient() 讀這格：4.1.2 先 ÷10 去尾再 ÷10、3.6.1／4.0.0 ÷100，都得到 細位置+100 . 線寬）。2026-10-30 起第 3 格改這個編碼</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` 路 | 壓到線的路數 0~8，0 = 找不到線 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
lineWidth = ((w >> 8) & 0xFF)   # 線寬（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
lineWidth = line.width()
```

<p class="ref-note">模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 1 🔒；模式 2 byte 0；模式 5 v[0] 🔒；模式 7 v[0] 🔒；模式 9 v[4] hi 🔒。備註：跟線位置合在 reflection() 的個位數（見線位置）; 跟細位置合在一起（見細位置）：第 1、2、4 格是個位數（hsv(surface=False).h），第 3 格是十位數、最多 9（ambient() 的小數第 1 位）</p>

</section>

<section id="v04" class="ref-item">

### 線段數 <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-groupCount.svg" alt="線段數" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~4` | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[5]
groupCount = ((w >> 8) & 0xFF)   # 線段數（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
groupCount = line.junction()
```

<p class="ref-note">模式 9 v[5] hi 🔒</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 路（最左）… bit7 = 第 8 路，1 = 那一路在線上 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[6]
bitmap = (w & 0xFF)   # 壓線位元圖（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
bitmap = line.bin_raw()
```

<p class="ref-note">模式 4 v[1]；模式 9 v[6] lo 🔒</p>

</section>

<section id="v10" class="ref-item">

### 第 1~8 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="第 1~8 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~8 路校準後光值 0~100（第 1 路最左）；編號 10~17 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

words = dev.read(9)[0:4]
light = []
for w in words:
    light.append(w & 0xFF)          # 奇數路
    light.append((w >> 8) & 0xFF)   # 偶數路
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line8_obj_Lib import MBC_LINE8

line = MBC_LINE8(Port.A)
light = line.ir_calibration()   # list，第 1 路 = [0]
```

<p class="ref-note">模式 0 (edu) byte 4 🔒；模式 9 v[0] 🔒。備註：4-bit (v×10)>>6; 每 int16 兩路：lo = 奇數路、hi = 偶數路</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：最後離線方向、二值化門檻、校準完成次數。要用這些值請改用 <a href="/sensors/line8/arduino/ide.md">Arduino</a>／<a href="/sensors/line8/mblock/mblock5.md">mBlock</a>／<a href="/sensors/line8/microbit/makecode.md">micro:bit</a>。</p>

## 命令

下命令要用 **MBC 函式庫**（2026-10 以後的版本）和 **2026-10 以後的感應器韌體**：`cmd(命令碼, 參數…)` 送出，`wait_done()` 等它做完（回 True＝完成；False＝失敗、拒絕或逾時）。感應器回的狀態：0 完成、1 執行中、2 失敗（例如參數超出範圍）、3 拒絕（不認得的命令，或校準中又送別的命令）。

- 舊韌體呼叫 `cmd()` 會顯示「韌體太舊」，讀值照常。
- 同一個命令＋同樣參數再呼叫不會重做（放在迴圈裡也只做一次），要再做一次寫 `cmd(…, again=True)`。
- `wait_done()` 每次多 40～60 ms，只在校準這類「停下來做」的命令用，不要放在循線迴圈裡。
- 官方 SPIKE App 3、Pybricks 內建 ColorSensor 積木送不了命令。

<section id="ce0" class="ref-item ref-cmd">

### 切黑線 <small>blackLine</small>

追黑線（預設）；存 RAM，重開機回預設

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE0)   # 切黑線
ok = line.wait_done()   # True＝做完；False＝失敗／拒絕／逾時
```

<p class="ref-note">Pybricks＋MBC 函式庫才送得了（官方 App、內建 ColorSensor 積木不行）；要 2026-10 以後的感應器韌體。</p>

</section>

<section id="ce1" class="ref-item ref-cmd">

### 切白線 <small>whiteLine</small>

追白線（F3 b1 = 1）；存 RAM，重開機回預設

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE1)   # 切白線
ok = line.wait_done()   # True＝做完；False＝失敗／拒絕／逾時
```

<p class="ref-note">Pybricks＋MBC 函式庫才送得了（官方 App、內建 ColorSensor 積木不行）；要 2026-10 以後的感應器韌體。</p>

</section>

<section id="ce2" class="ref-item ref-cmd">

### 黑色校準 <small>calibBlack</small>

把目前讀值記為黑（感應器壓在黑線上），約 50 ms；寫入 EEPROM

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE2)   # 黑色校準
ok = line.wait_done()   # True＝做完；False＝失敗／拒絕／逾時
```

<p class="ref-note">Pybricks＋MBC 函式庫才送得了（官方 App、內建 ColorSensor 積木不行）；要 2026-10 以後的感應器韌體。</p>

</section>

<section id="ce3" class="ref-item ref-cmd">

### 白色校準 <small>calibWhite</small>

把目前讀值記為白（感應器壓在白底上），約 50 ms；寫入 EEPROM

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE3)   # 白色校準
ok = line.wait_done()   # True＝做完；False＝失敗／拒絕／逾時
```

<p class="ref-note">Pybricks＋MBC 函式庫才送得了（官方 App、內建 ColorSensor 積木不行）；要 2026-10 以後的感應器韌體。</p>

</section>

<section id="ce4" class="ref-item ref-cmd">

### 設定門檻 <small>setThreshold</small>

二值化門檻 1~99（RAM，重開機回預設 50）；SPIKE 參數超出範圍回失敗（狀態 2）

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE4, 50)   # 設定門檻（門檻 1~99）
ok = line.wait_done()   # True＝做完；False＝失敗／拒絕／逾時
```

<p class="ref-note">Pybricks＋MBC 函式庫才送得了（官方 App、內建 ColorSensor 積木不行）；要 2026-10 以後的感應器韌體。</p>

</section>

<section id="ce7" class="ref-item ref-cmd">

### 5 秒動態校準 <small>calibAuto</small>

5 秒內把感應器在黑線與白底上來回掃，取極值當黑白參考（共識 B-12，= v1 的 0x20=0x01）；期間 F3 b2 = 1

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE7)   # 5 秒動態校準
ok = line.wait_done(8000)   # True＝做完；False＝失敗／拒絕／逾時
```

<p class="ref-note">Pybricks＋MBC 函式庫才送得了（官方 App、內建 ColorSensor 積木不行）；要 2026-10 以後的感應器韌體。</p>

</section>

<section id="ce9" class="ref-item ref-cmd">

### 左右反向 <small>reverse</small>

[0 正常／1 左右反向]：感應器反裝時用，通道順序與線位置左右對調；校準值照實體通道存，切換不用重校；存 RAM，重開機回 0（舊 mBlock 沒有）；SPIKE 參數超出範圍回失敗（狀態 2）

```python
from pybricks.parameters import Port
from MBC_sensors import MBC_LINE8

line = MBC_LINE8(Port.A)
line.cmd(0xE9, 1)   # 左右反向（反向 0~1）
ok = line.wait_done()   # True＝做完；False＝失敗／拒絕／逾時
```

<p class="ref-note">Pybricks＋MBC 函式庫才送得了（官方 App、內建 ColorSensor 積木不行）；要 2026-10 以後的感應器韌體。</p>

</section>

## 範例

<!-- examples: sensors/line8/spike-examples.md host=spike sw=pybricks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [Pybricks 函式庫 MBC_line8_obj_Lib.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_line8_obj_Lib.py)
- [共用函式庫（新專案用，一個檔適用全部 MBC 感應器） MBC_sensors.py](/examples/line8/pybricks/For%20firmware%204.x/MBC_sensors.py)
- [line8_pybricks_spike_v4.x.zip](/examples/line8/line8_pybricks_spike_v4.x.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-pybricks.zh.md`、`_build/guides/product/line8/spike-pybricks.zh.md`
- 範例：`sensors/line8/spike-examples.md`
