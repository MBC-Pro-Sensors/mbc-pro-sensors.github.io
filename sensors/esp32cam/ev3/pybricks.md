<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/ev3/pybricks -->
# 神攝手 ESP32CAM × EV3 × Pybricks

這一頁只講 **Pybricks v4／EV3 MicroPython v2**：在 MINDSTORMS EV3 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-pybricks.zh.md + _build/guides/product/esp32cam/ev3-pybricks.zh.md {"product":"神攝手 ESP32CAM","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：Pybricks v4／EV3 MicroPython v2 說明](/sensors/esp32cam/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值（學校版）

程式假設感應器插 1 號孔，用 `LUMPDevice` 讀原始模式；EV3 MicroPython v2 寫法相同。

<section id="v01" class="ref-item">

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

hue = sensor.read(0)[0]   # 色相：-1~100
hue = sensor.read(1)[0]   # 色相：-1~100
hue = sensor.read(2)[0]   # 色相：-1~100
hue = sensor.read(3)[0]   # 色相：-1~100
hue = sensor.read(5)[0]   # 色相：-1~100
```

<p class="ref-note">模式 0 · int8 · IR-PROX 1×int8；模式 1 · int8 · IR-SEEK [色相, 飽和, 明度, 動態, X, Y, W, H]；模式 2 · int8 · IR-REMOTE [色相, 飽和, 明度, 動態]；模式 3 · int16 · IR-REM-A 1×int16（#65 已修成正確符號延伸，−1 = 0xFFFF）；模式 5 · int16 · IR-CAL [色相, 飽和] 2×int16（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v02" class="ref-item">

### 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點飽和度 0~100 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

saturation = sensor.read(1)[1]   # 飽和度：0~100
saturation = sensor.read(2)[1]   # 飽和度：0~100
saturation = sensor.read(5)[1]   # 飽和度：0~100
```

<p class="ref-note">模式 1 · int8；模式 2 · int8；模式 5 · int16（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v03" class="ref-item">

### 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點明度 0~100 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

value = sensor.read(1)[2]   # 明度：0~100
value = sensor.read(2)[2]   # 明度：0~100
```

<p class="ref-note">模式 1 · int8；模式 2 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

motion = sensor.read(1)[3]   # 動態：0~100
motion = sensor.read(2)[3]   # 動態：0~100
```

<p class="ref-note">模式 1 · int8；模式 2 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

blobX = sensor.read(1)[4]   # 色塊 X：0~100
blobX = sensor.read(4)[0]   # 色塊 X：0~100
```

<p class="ref-note">模式 1 · int8；模式 4 · int8 · IR-S-ALT [X, Y, W, H]（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

blobY = sensor.read(1)[5]   # 色塊 Y：0~75
blobY = sensor.read(4)[1]   # 色塊 Y：0~75
```

<p class="ref-note">模式 1 · int8；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v07" class="ref-item">

### 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊寬 W 0~100；0 = 沒找到 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

blobW = sensor.read(1)[6]   # 色塊寬：0~100
blobW = sensor.read(4)[2]   # 色塊寬：0~100
```

<p class="ref-note">模式 1 · int8；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v08" class="ref-item">

### 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊高 H 0~75；沒找到 = 0 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

blobH = sensor.read(1)[7]   # 色塊高：0~75
blobH = sensor.read(4)[3]   # 色塊高：0~75
```

<p class="ref-note">模式 1 · int8；模式 4 · int8（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：色塊面積、有找到色塊。要用這些值請改用 <a href="/sensors/esp32cam/arduino/ide.md">Arduino</a>／<a href="/sensors/esp32cam/mblock/mblock5.md">mBlock</a>／<a href="/sensors/esp32cam/microbit/makecode.md">micro:bit</a>。</p>

## 讀值（WRO 版）

程式假設感應器插 1 號孔，用 `LUMPDevice` 讀原始模式；EV3 MicroPython v2 寫法相同。

<section id="w10" class="ref-item">

### 色點 1~12 顏色碼 <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="色點 1~12 顏色碼" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

spotColor = sensor.read(0)[0:12]   # 色點 1~12 顏色碼
```

<p class="ref-note">模式 0 · int32×8 · HUE12 8×int32 LE（32 B）：24 byte = [色相, 鮮豔度]×12，每 3 byte 塞一個 int32（byte3 = 0）；格 n 色相 = byte 2(n−1) → int32 k = idx/3、byte = idx%3；鎖住（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="w20" class="ref-item">

### 色點 1~12 鮮豔度 <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="色點 1~12 鮮豔度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

spotVivid = sensor.read(0)[0:12]   # 色點 1~12 鮮豔度
```

<p class="ref-note">模式 0 · int32×8 · 同 10~1B：格 n 鮮豔度 = byte 2(n−1)+1；鎖住（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

## 範例（學校版）

<!-- examples: sensors/esp32cam/ev3-examples.md host=ev3 sw=pybricks -->

## 範例（WRO 版）

<!-- examples: sensors/esp32cam/ev3-examples-wro.md host=ev3 sw=pybricks -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-pybricks.zh.md`、`_build/guides/product/esp32cam/ev3-pybricks.zh.md`
- 範例：`sensors/esp32cam/ev3-examples(-wro).md`
