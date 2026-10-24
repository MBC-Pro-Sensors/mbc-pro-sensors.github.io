<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × spike × mbcblocks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/spike/mbcblocks -->
# 神攝手 ESP32CAM × SPIKE × MBC 積木網站

這一頁只講 **MBC SPIKE 積木網站**：在 SPIKE Prime／Robot Inventor 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-mbcblocks.zh.md + _build/guides/product/esp32cam/spike-mbcblocks.zh.md {"product":"神攝手 ESP32CAM","edition":"SPIKE 版","port":"C","lib":"MBC_cam_school_Lib.py","libUrl":"/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：MBC SPIKE 積木網站 說明](/sensors/esp32cam/spike-examples.md)。
<!-- /include-fallback -->

## 讀值（學校版）

主機要先換成 Pybricks 韌體。MBC 積木網站的感應器積木讀的是感應器的模式 9（一次讀完全部數值）。

<section id="v01" class="ref-item">

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「色相」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
hue = (w & 0xFF)   # 色相（低 byte）
```

</section>

<section id="v02" class="ref-item">

### 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點飽和度 0~100 |

在 MBC 積木網站加入這款感應器的積木，用「飽和度」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
saturation = ((w >> 8) & 0xFF)   # 飽和度（高 byte）
```

</section>

<section id="v03" class="ref-item">

### 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點明度 0~100 |

在 MBC 積木網站加入這款感應器的積木，用「明度」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
value = (w & 0xFF)   # 明度（低 byte）
```

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

在 MBC 積木網站加入這款感應器的積木，用「動態」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
motion = ((w >> 8) & 0xFF)   # 動態（高 byte）
```

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「色塊 X」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[2]
blobX = (w & 0xFF)   # 色塊 X（低 byte）
```

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「色塊 Y」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[2]
blobY = ((w >> 8) & 0xFF)   # 色塊 Y（高 byte）
```

</section>

<section id="v07" class="ref-item">

### 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊寬 W 0~100；0 = 沒找到 |

在 MBC 積木網站加入這款感應器的積木，用「色塊寬」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[3]
blobW = (w & 0xFF)   # 色塊寬（低 byte）
```

</section>

<section id="v08" class="ref-item">

### 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊高 H 0~75；沒找到 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「色塊高」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[3]
blobH = ((w >> 8) & 0xFF)   # 色塊高（高 byte）
```

</section>

<section id="v09" class="ref-item">

### 色塊面積 <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="色塊面積" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~7500` | W×H（0~7500）；沒找到 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「色塊面積」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

blobArea = dev.read(9)[4]   # 色塊面積
```

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：有找到色塊。要用這些值請改用 <a href="/sensors/esp32cam/arduino/ide.md">Arduino</a>／<a href="/sensors/esp32cam/mblock/mblock5.md">mBlock</a>／<a href="/sensors/esp32cam/microbit/makecode.md">micro:bit</a>。</p>

## 讀值（WRO 版）

主機要先換成 Pybricks 韌體。MBC 積木網站的感應器積木讀的是感應器的模式 9（一次讀完全部數值）。

<p class="ref-missing"><b>這個軟體讀不到</b>：色點 1~12 顏色碼、色點 1~12 鮮豔度。要用這些值請改用 <a href="/sensors/esp32cam/spike/pybricks.md">Pybricks</a>。</p>

## 範例（學校版）

<!-- examples: sensors/esp32cam/spike-examples.md host=spike sw=mbcblocks -->

## 範例（WRO 版）

<!-- examples: sensors/esp32cam/spike-examples-wro.md host=spike sw=mbcblocks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MBC SPIKE 積木網站（線上，不用安裝）](https://mbc-spike-blocks.mbcrobot.workers.dev)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-mbcblocks.zh.md`、`_build/guides/product/esp32cam/spike-mbcblocks.zh.md`
- 範例：`sensors/esp32cam/spike-examples(-wro).md`
