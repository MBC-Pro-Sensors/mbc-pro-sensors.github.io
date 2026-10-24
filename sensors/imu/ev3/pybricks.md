<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（imu × ev3 × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: imu/ev3/pybricks -->
# 陀螺儀 IMU 九軸 × EV3 × Pybricks

這一頁只講 **Pybricks v4／EV3 MicroPython v2**：在 MINDSTORMS EV3 上用它讀陀螺儀 IMU 九軸的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/ev3-pybricks.zh.md + _build/guides/product/imu/ev3-pybricks.zh.md {"product":"陀螺儀 IMU 九軸","edition":"EV3 版","port":"1","lib":"","libUrl":"","addr":"0x6A"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[陀螺儀 IMU 九軸：Pybricks v4／EV3 MicroPython v2 說明](/sensors/imu/ev3-examples.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 1 號孔，用 `LUMPDevice` 讀原始模式；EV3 MicroPython v2 寫法相同。

<section id="v01" class="ref-item">

### 陀螺 3D 航向 <small>headingGyro3D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 四元數算出的航向，度，開機／歸零 = 0，連續累積可超過 360（int16 到 ±32767 繞回，約 91 圈） |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

headingGyro3D = sensor.read(0)[0]   # 陀螺 3D 航向：-32768~32767 °
```

<p class="ref-note">模式 0 · int16 · GYRO-ANG 1×int16（盤點：mode0 = 陀螺 3D，INO:232-234）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v02" class="ref-item">

### 融合航向 <small>headingFused</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 陀螺 3D 加磁力計慢修正的航向，度，減去相對零點；可超過 360 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

headingFused = sensor.read(1)[0]   # 融合航向：-32768~32767 °
```

<p class="ref-note">模式 1 · int16 · GYRO-RATE 1×int16 = 融合航向（EV3 定案：不改）（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v03" class="ref-item">

### 陀螺 1D 航向 <small>headingGyro1D</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 只繞轉向軸積分的航向，度，開機／歸零 = 0；可超過 360 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

headingGyro1D = sensor.read(3)[0]   # 陀螺 1D 航向：-32768~32767 °
```

<p class="ref-note">模式 3 · int16 · GYRO-G&A 2×int16 [陀螺 1D, 磁力航向]（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v04" class="ref-item">

### 磁力航向 <small>headingMag</small>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` ° | 磁力計傾斜補償航向，度，減去相對零點 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

headingMag = sensor.read(3)[1]   # 磁力航向：-32768~32767 °
```

<p class="ref-note">模式 3 · int16（已鎖住的模式，韌體與主機端都不再改）</p>

</section>

<section id="v05" class="ref-item">

### 轉向軸角速度 <small>yawRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-yawRate.svg" alt="轉向軸角速度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 繞轉向軸（getYawAxis()）的角速度，0.1 度／秒，正負跟航向同向 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

yawRate = sensor.read(2)[0]   # 轉向軸角速度：-32768~32767 0.1 dps
```

<p class="ref-note">模式 2 · int16 · GYRO-FAS 1×int16（#67 新增，原本 0）</p>

</section>

<section id="v06" class="ref-item">

### 俯仰角 <small>pitch</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitch.svg" alt="俯仰角" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-90~+90` ° | 板子絲印 X 軸相對水平面的仰角：X 軸朝上 +90、朝下 −90、水平 0（用重力向量 atan2 算） |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

pitch = sensor.read(6)[0]   # 俯仰角：-90~+90 °
```

<p class="ref-note">模式 6 · int16 · TILT-ANG 1×int16（#67 新增，原本 0）</p>

</section>

<section id="v07" class="ref-item">

### 俯仰角速度 <small>pitchRate</small>

<figure class="ref-fig"><img src="/images/figures/imu/value-pitchRate.svg" alt="俯仰角速度" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-32768~32767` 0.1 dps | 繞板子絲印 X 軸的角速度，0.1 度／秒（右手定則，從 +X 端看逆時針為正）；注意這是 gyro X，不是 06 的微分 |

```python
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port

sensor = LUMPDevice(Port.S1)   # 感應器插 1 號孔

pitchRate = sensor.read(5)[0]   # 俯仰角速度：-32768~32767 0.1 dps
```

<p class="ref-note">模式 5 · int16 · TILT-RATE 1×int16（#67：取代 #65 統一時放的 ID，ID 移到模式 4）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：磁力信任度、磁場干擾度、朝上的面、角速度 1~3 軸、加速度 1~3 軸、磁力 1~3 軸。要用這些值請改用 <a href="/sensors/imu/arduino/ide.md">Arduino</a>／<a href="/sensors/imu/mblock/mblock5.md">mBlock</a>／<a href="/sensors/imu/microbit/makecode.md">micro:bit</a>。</p>

## 命令

EV3 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/imu/ev3-examples.md host=ev3 sw=pybricks -->

## 其他問題

- **EV3 顯示成紅外線感應器？** 正常：感應器模擬成樂高原廠感應器，用原廠積木就能讀。
- **切換模式時卡一下？** EV3 每次換模式要等約 0.5 秒。整趟任務盡量只用同一個模式的積木。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/imu.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/ev3-pybricks.zh.md`、`_build/guides/product/imu/ev3-pybricks.zh.md`
- 範例：`sensors/imu/ev3-examples.md`
