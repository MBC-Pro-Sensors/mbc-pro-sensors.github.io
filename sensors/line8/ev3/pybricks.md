<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（line8 × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: line8/ev3/pybricks -->
# 循行者 8 路 (Line8) × EV3 × Pybricks

這一頁只講 **Pybricks v4／EV3 MicroPython v2**：在 MINDSTORMS EV3 上用它讀循行者 8 路 (Line8)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-pybricks.zh.md + _build/guides/product/line8/ev3-pybricks.zh.md {"product":"循行者 8 路 (Line8)","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x16"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[循行者 8 路 (Line8)：Pybricks v4／EV3 MicroPython v2 說明](/sensors/line8/ev3-pybricks.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 1 號孔，用 `LUMPDevice` 讀原始模式；EV3 MicroPython v2 寫法相同。

<section id="v01" class="ref-item">

### 線位置 <small>linePos</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePos.svg" alt="線位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-8~+8` | 線在哪一路：−8（最左）~+8（最右），0 = 正中間；站在車後往前看，負 = 線在左 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

linePos = sensor.read(0)[0]   # 線位置：-8~+8
```

<p class="ref-note">模式 0 · int8 · IR-PROX（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v02" class="ref-item">

### 細位置 <small>linePosFine</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-linePosFine.svg" alt="細位置" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 線位置的細分：−100 ~ +100，0 = 正中間，負 = 線在左（與線位置同號） |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

linePosFine = sensor.read(2)[0]   # 細位置：-100~+100
```

<p class="ref-note">模式 2 · int8 · IR-REMOTE byte0 = highRes−100 存成 int8（F030 ev3Uart case 2），主機直接讀（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v03" class="ref-item">

### 線寬 <small>lineWidth</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-lineWidth.svg" alt="線寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~8` 路 | 壓到線的路數 0~8，0 = 找不到線 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

lineWidth = sensor.read(2)[1]   # 線寬：0~8 路
```

<p class="ref-note">模式 2 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v05" class="ref-item">

### 壓線位元圖 <small>bitmap</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-bitmap.svg" alt="壓線位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` bit | bit0 = 第 1 路（最左）… bit7 = 第 8 路，1 = 那一路在線上 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

bitmap = sensor.read(3)[0]   # 壓線位元圖：0~255 bit
```

<p class="ref-note">模式 3 · int16 · #67 新：IR-REM-A 1×int16</p>

</section>

<section id="v10" class="ref-item">

### 第 1~8 路光值 <small>light</small>

<figure class="ref-fig"><img src="/images/figures/line8/value-light.svg" alt="第 1~8 路光值" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 第 1~8 路校準後光值 0~100（第 1 路最左）；編號 10~17 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

light = sensor.read(1)[0:8]   # 第 1~8 路光值
```

<p class="ref-note">模式 1 · int8 · IR-SEEK 8×int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：線段數、最後離線方向、二值化門檻、校準完成次數。要用這些值請改用 <a href="/sensors/line8/arduino/ide.md">Arduino</a>／<a href="/sensors/line8/mblock/mblock5.md">mBlock</a>／<a href="/sensors/line8/microbit/makecode.md">micro:bit</a>。</p>

## 命令

EV3 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/line8/ev3-examples.md host=ev3 sw=pybricks -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [line8_pybricks_ev3.zip](/examples/line8/line8_pybricks_ev3.zip)
- [MBC_line8_EV3_Lib.py](/examples/line8/ev3/MBC_line8_EV3_Lib.py)
- [MBC_sensors.py](/examples/line8/ev3/MBC_sensors.py)
- [line8_ev3_block_with_lib.py](/examples/line8/ev3/line8_ev3_block_with_lib.py)
- [line8_ev3_python_with_lib.py](/examples/line8/ev3/line8_ev3_python_with_lib.py)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/line8.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-pybricks.zh.md`、`_build/guides/product/line8/ev3-pybricks.zh.md`
- 範例：`sensors/line8/ev3-examples.md`
