<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line16 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line16/spike/pybricks -->
# 循行者 16 路 (Line16) × SPIKE × Pybricks

這一頁只講 **Pybricks（Python）**：在 SPIKE Prime／Robot Inventor 上用它讀循行者 16 路 (Line16)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-pybricks.zh.md + _build/guides/product/line16/spike-pybricks.zh.md {"product":"循行者 16 路 (Line16)","edition":"SPIKE 版","port":"C","lib":"MBC_line16_obj_Lib.py","libUrl":"/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 16 路 (Line16)：Pybricks（Python） 說明](/sensors/line16/spike-pybricks.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 A 孔。「MBC 函式庫」寫法要先把函式庫 .py 一起匯入 Pybricks。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-16~+16` | 線在哪一路：−16（最左）~+16（最右），0 = 正中間；站在車後往前看，負 = 線在左（平常 ±15，±16 只在最外側掉線後 200 ms 內） |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[4]
linePos = (w & 0xFF) - 16   # 線位置（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line16_obj_Lib import MBC_LINE16

line = MBC_LINE16(Port.A)
linePos = line.pos16()
```

<p class="ref-note">模式 0 (edu) byte 1 −16 🔒；模式 0 (toy) byte 0 −16 🔒；模式 1 byte 0 −16；模式 5 v[0] 🔒；模式 9 v[4] lo −16 🔒。備註：Pybricks reflection() 編碼：X = (線位置+16)×100 + 線寬，a = X/25、b = (X−a×25+1)×10−1，3 個 int16 = a×256+b；主機 ×3200/32768 ≈ X（X ≥ 3200 時讀到負數，F16:905）</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

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
from MBC_line16_obj_Lib import MBC_LINE16

line = MBC_LINE16(Port.A)
linePosFine = line.pos100()
```

<p class="ref-note">模式 0 (edu) byte 2 −100 🔒；模式 0 (toy) byte 2 −100 🔒；模式 3 byte 0 −100；模式 4 v[0] −100；模式 7 v[0] 🔒；模式 9 v[5] lo −100 🔒。備註：byte2 = 細位置+100（0~200）、byte3 = 0；App 原始紅讀成 int16; 細位置+100 經玩具版查表編碼成 byte2~3; Pybricks ambient()：4 個相同 int16 = (細位置+100)×100 + 線寬（0~20016），主機 ÷100 → 位置.線寬</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~16` 路 | 壓到線的路數 0~16，0 = 找不到線 |

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
from MBC_line16_obj_Lib import MBC_LINE16

line = MBC_LINE16(Port.A)
lineWidth = line.width()
```

<p class="ref-note">模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 1 🔒；模式 2 byte 0；模式 3 byte 1；模式 9 v[4] hi 🔒。備註：min(線寬, 10)</p>

</section>

<section id="v04" class="ref-item">

### 線段數 <small>groupCount</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-groupCount.svg" alt="線段數" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` | 看到幾段線：0 丟線、1 一條線、2 以上 = 分岔／十字 |

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
from MBC_line16_obj_Lib import MBC_LINE16

line = MBC_LINE16(Port.A)
groupCount = line.junction()
```

<p class="ref-note">模式 9 v[5] hi 🔒</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | bit0 = 第 1 路（最左）… bit15 = 第 16 路，1 = 那一路在線上 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

bitmap = dev.read(9)[6]   # 壓線位元圖
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_line16_obj_Lib import MBC_LINE16

line = MBC_LINE16(Port.A)
bitmap = line.bin_raw()
```

<p class="ref-note">模式 4 v[1]；模式 9 v[6] 🔒。備註：binRaw 16 bit（byte2 低、byte3 高）; byte12 低、byte13 高</p>

</section>

<section id="v10" class="ref-item">

### 第 1~16 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line16/value-light.svg" alt="第 1~16 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~16 路校準後光值 0~100（第 1 路最左）；編號 10~1F |

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
from MBC_line16_obj_Lib import MBC_LINE16

line = MBC_LINE16(Port.A)
light = line.ir_calibration()   # list，第 1 路 = [0]
```

<p class="ref-note">模式 0 (edu) byte 4 🔒；模式 0 (toy) byte 4 🔒；模式 3 byte 2；模式 6 v[0]；模式 8 v[0]；模式 9 v[0] 🔒。備註：每路 2 bit（(v×10)>>8：0~25→0、26~51→1、52~76→2、77~100→3）；byte4 = 第 5~8 路（第 5 路在 bit7-6）、byte5 = 第 1~4 路、byte6 = 第 13~16 路、byte7 = 第 9~12 路 → App 原始綠 = 第 1~8 路（bit15-14 = 第 1 路）、原始藍 = 第 9~16 路；byte 8~15 = 0; 每路 1 bit（光值 > 50 = 1 亮）：第 1~8 路合成 1 byte（第 1 路在 bit7）經玩具版查表編碼成 byte4~5；第 9~16 路 → byte6~7; byte2 = 第 1 路、byte3 = 第 16 路（描述表只宣告 3×int8，第 4 byte 主機可能砍掉）; byte0~5 = 第 6~11 路光值 → 3×int16，每個 lo = 前一路、hi = 後一路; byte0~3 = 第 7~10 路（只送 4 B）; 每 int16 四路 4-bit（(v×5)>>5，0~15）：bit0-3 = 第 1 路、bit4-7 = 第 2 路、bit8-11 = 第 3 路、bit12-15 = 第 4 路；v0~v3 = 第 1~16 路</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：最後離線方向、二值化門檻、校準完成次數。要用這些值請改用 <a href="/sensors/line16/arduino/ide.md">Arduino</a>／<a href="/sensors/line16/mblock/mblock5.md">mBlock</a>／<a href="/sensors/line16/microbit/makecode.md">micro:bit</a>。</p>

## 命令

SPIKE 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/line16/spike-examples.md host=spike sw=pybricks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [Pybricks 函式庫 MBC_line16_obj_Lib_v400.py](/examples/line16/pybricks/For%20firmware%204.0.0/MBC_line16_obj_Lib_v400.py)
- [line16_pybricks_spike_v3.6.1.zip](/examples/line16/line16_pybricks_spike_v3.6.1.zip)
- [line16_pybricks_spike_v4.0.0.zip](/examples/line16/line16_pybricks_spike_v4.0.0.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line16.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-pybricks.zh.md`、`_build/guides/product/line16/spike-pybricks.zh.md`
- 範例：`sensors/line16/spike-examples.md`
