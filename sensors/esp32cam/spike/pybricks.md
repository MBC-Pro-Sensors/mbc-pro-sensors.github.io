<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（esp32cam × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: esp32cam/spike/pybricks -->
# 神攝手 ESP32CAM × SPIKE × Pybricks

這一頁只講 **Pybricks（Python）**：在 SPIKE Prime／Robot Inventor 上用它讀神攝手 ESP32CAM的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-pybricks.zh.md + _build/guides/product/esp32cam/spike-pybricks.zh.md {"product":"神攝手 ESP32CAM","edition":"SPIKE 版","port":"C","lib":"MBC_cam_school_Lib.py","libUrl":"/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py","addr":"0x4C"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[神攝手 ESP32CAM：Pybricks（Python） 說明](/sensors/esp32cam/spike-examples.md)。
<!-- /include-fallback -->

## 讀值（學校版）

程式假設感應器插 A 孔。「MBC 函式庫」寫法要先把函式庫 .py 一起匯入 Pybricks。

<section id="v01" class="ref-item">

### 色相 <small>hue</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-hue.svg" alt="色相" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-1~100` | S1 取樣點色相 0~100（色相環 0~240 等比縮放）；−1 = 黑白灰（飽和度 < 10）；S1 沒設定 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
hue = (w & 0xFF)   # 色相（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
hue = cam.hue()
```

<p class="ref-note">模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 1 🔒；模式 5 v[0] 🔒；模式 9 v[0] lo 🔒。備註：−1 = 0xFF; 0~10（÷10）; raw = (色相×256 ± 128)/25 ≈ 色相×10.24，v[0]=v[1]=v[2]；Pybricks reflection() ÷10.24 還原</p>

</section>

<section id="v02" class="ref-item">

### 飽和度 <small>saturation</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-saturation.svg" alt="飽和度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點飽和度 0~100 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
saturation = ((w >> 8) & 0xFF)   # 飽和度（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
saturation = cam.saturation()
```

<p class="ref-note">模式 9 v[0] hi 🔒</p>

</section>

<section id="v03" class="ref-item">

### 明度 <small>value</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-value.svg" alt="明度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | S1 取樣點明度 0~100 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
value = (w & 0xFF)   # 明度（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
value = cam.brightness()
```

<p class="ref-note">模式 9 v[1] lo 🔒</p>

</section>

<section id="v04" class="ref-item">

### 動態 <small>motion</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-motion.svg" alt="動態" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 全畫面動態量 0~100（畫面變化越多越大） |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
motion = ((w >> 8) & 0xFF)   # 動態（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
motion = cam.motion()
```

<p class="ref-note">模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 0 🔒；模式 9 v[1] hi 🔒</p>

</section>

<section id="v05" class="ref-item">

### 色塊 X <small>blobX</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobX.svg" alt="色塊 X" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊中心 X 0~100，左 = 0；沒找到 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[2]
blobX = (w & 0xFF)   # 色塊 X（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobX = cam.blob_x()
```

<p class="ref-note">模式 0 (edu) byte 2 🔒；模式 0 (toy) byte 2 🔒；模式 6 v[0]；模式 7 v[2] 🔒；模式 9 v[2] lo 🔒。備註：byte 3 = 0; ×4 查表編碼（10-bit）; #67 改：模式 6 = [X, Y, 面積]（3×int16）; X×100+Y → Pybricks ambient() 顯示 X.Y</p>

</section>

<section id="v06" class="ref-item">

### 色塊 Y <small>blobY</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobY.svg" alt="色塊 Y" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊中心 Y 0~75，下 = 0、上 = 75；沒找到 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[2]
blobY = ((w >> 8) & 0xFF)   # 色塊 Y（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobY = cam.blob_y()
```

<p class="ref-note">模式 0 (edu) byte 4 🔒；模式 0 (toy) byte 4 🔒；模式 6 v[1]；模式 7 v[2] 🔒；模式 9 v[2] hi 🔒。備註：byte 5 = 0; ×4 查表編碼; X×100+Y 的個位／十位</p>

</section>

<section id="v07" class="ref-item">

### 色塊寬 <small>blobW</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobW.svg" alt="色塊寬" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~100` | 色塊寬 W 0~100；0 = 沒找到 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[3]
blobW = (w & 0xFF)   # 色塊寬（低 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobW = cam.blob_w()
```

<p class="ref-note">模式 9 v[3] lo 🔒</p>

</section>

<section id="v08" class="ref-item">

### 色塊高 <small>blobH</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobH.svg" alt="色塊高" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~75` | 色塊高 H 0~75；沒找到 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[3]
blobH = ((w >> 8) & 0xFF)   # 色塊高（高 byte）
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobH = cam.blob_h()
```

<p class="ref-note">模式 9 v[3] hi 🔒</p>

</section>

<section id="v09" class="ref-item">

### 色塊面積 <small>blobArea</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-blobArea.svg" alt="色塊面積" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~7500` | W×H（0~7500）；沒找到 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

blobArea = dev.read(9)[4]   # 色塊面積
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.A)
blobArea = cam.area()
```

<p class="ref-note">模式 0 (edu) byte 6 🔒；模式 0 (toy) byte 6 🔒；模式 6 v[2]；模式 9 v[4] 🔒。備註：鎖住：這裡是 map(像素面積, 0~5000, 0~100)，不是 W×H；byte 7 = 0; 同上 0~100 再 ×4 查表編碼; W×H; #67 新增：byte 8~9 = W×H（原本 0）；其餘 byte 不動</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：有找到色塊。要用這些值請改用 <a href="/sensors/esp32cam/arduino/ide.md">Arduino</a>／<a href="/sensors/esp32cam/mblock/mblock5.md">mBlock</a>／<a href="/sensors/esp32cam/microbit/makecode.md">micro:bit</a>。</p>

## 讀值（WRO 版）

程式假設感應器插 A 孔。「MBC 函式庫」寫法要先把函式庫 .py 一起匯入 Pybricks。

<section id="w10" class="ref-item">

### 色點 1~12 顏色碼 <small>spotColor</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotColor.svg" alt="色點 1~12 顏色碼" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 10~1B = S1~S12 顏色碼：0~239 彩色（色相 0~255 等比縮成 0~239）、240~245 白（飽和度 < 30，越低越大）、246~250 黑（明度 < 30，越暗越大）、255 = 色點沒開 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

spotColor = dev.read(0)[0]   # 色點 1~12 顏色碼
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_hue12_Lib import HUE12Sensor

cam = HUE12Sensor(Port.A)
cam.update()   # 每圈先更新一次
spotColor = cam.get_slot(1)[1]   # list，第 1 路 = [0]
```

<p class="ref-note">模式 0 (edu) byte 0 🔒。備註：同 EV3：Pybricks read(0) 得 8 個 int32，每個拆 3 byte（積木網站 MBC_HUE12 設定檔）；玩具版不送；鎖住</p>

</section>

<section id="w20" class="ref-item">

### 色點 1~12 鮮豔度 <small>spotVivid</small>

<figure class="ref-fig"><img src="/images/figures/esp32cam/value-spotVivid.svg" alt="色點 1~12 鮮豔度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~255` | 20~2B = S1~S12 鮮豔度（飽和度）0~255：黑 = 1、白／彩色 = 實際飽和度；色點沒開 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

spotVivid = dev.read(0)[1]   # 色點 1~12 鮮豔度
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_hue12_Lib import HUE12Sensor

cam = HUE12Sensor(Port.A)
cam.update()   # 每圈先更新一次
spotVivid = cam.get_slot(1)[2]   # list，第 1 路 = [0]
```

<p class="ref-note">模式 0 (edu) byte 1 🔒。備註：同 EV3；鎖住</p>

</section>

## 範例（學校版）

<!-- examples: sensors/esp32cam/spike-examples.md host=spike sw=pybricks -->

## 範例（WRO 版）

<!-- examples: sensors/esp32cam/spike-examples-wro.md host=spike sw=pybricks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [Pybricks 函式庫 MBC_cam_school_Lib.py](/examples/esp32cam/pybricks/MBC_cam_school_Lib.py)
- [Pybricks 函式庫 MBC_hue12_Lib.py](/examples/esp32cam/pybricks/MBC_hue12_Lib.py)
- [共用函式庫（新專案用，一個檔適用全部 MBC 感應器） MBC_sensors.py](/examples/esp32cam/pybricks/MBC_sensors.py)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/cam_school.json`、`_build/protocol/cam_wro.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-pybricks.zh.md`、`_build/guides/product/esp32cam/spike-pybricks.zh.md`
- 範例：`sensors/esp32cam/spike-examples(-wro).md`
