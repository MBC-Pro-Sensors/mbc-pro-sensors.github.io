<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（tof2 × spike × mbcblocks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: tof2/spike/mbcblocks -->
# 測距者 2 路 (TOF2) × SPIKE × MBC 積木網站

這一頁只講 **MBC SPIKE 積木網站**：在 SPIKE Prime／Robot Inventor 上用它讀測距者 2 路 (TOF2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-mbcblocks.zh.md + _build/guides/product/tof2/spike-mbcblocks.zh.md {"product":"測距者 2 路 (TOF2)","edition":"SPIKE 版","port":"C","lib":"MBC_laser2_obj_Lib.py","libUrl":"/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py","addr":"0x29"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[測距者 2 路 (TOF2)：MBC SPIKE 積木網站 說明](/sensors/tof2/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

主機要先換成 Pybricks 韌體。MBC 積木網站的感應器積木讀的是感應器的模式 9（一次讀完全部數值）。

<section id="v01" class="ref-item">

### 左距離 cm <small>leftCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-leftCm.svg" alt="左距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 左邊那顆的距離 0~120 cm（站在車後往前看，左 = 第 0 顆），120 = 前面沒東西／那顆失效 |

在 MBC 積木網站加入這款感應器的積木，用「左距離 cm」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
leftCm = (w & 0xFF)   # 左距離 cm（低 byte）
```

</section>

<section id="v02" class="ref-item">

### 右距離 cm <small>rightCm</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-rightCm.svg" alt="右距離 cm" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~120` cm | 右邊那顆的距離 0~120 cm（右 = 第 1 顆），120 = 前面沒東西／那顆失效 |

在 MBC 積木網站加入這款感應器的積木，用「右距離 cm」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[0]
rightCm = ((w >> 8) & 0xFF)   # 右距離 cm（高 byte）
```

</section>

<section id="v06" class="ref-item">

### 初始化位元圖 <small>initOk</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-initOk.svg" alt="初始化位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆開機初始化成功（失敗不重試）；3 = 兩顆都好 |

在 MBC 積木網站加入這款感應器的積木，用「初始化位元圖」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
initOk = (w & 0xFF)   # 初始化位元圖（低 byte）
```

</section>

<section id="v07" class="ref-item">

### 故障位元圖 <small>fault</small>

<figure class="ref-fig"><img src="/images/figures/tof2/value-fault.svg" alt="故障位元圖" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~3` bit | bit0 = 左、bit1 = 右，1 = 那顆目前失效（I2C 讀失敗或 200 ms 沒更新）；失效那顆距離回 120／1200 |

在 MBC 積木網站加入這款感應器的積木，用「故障位元圖」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

w = dev.read(9)[1]
fault = ((w >> 8) & 0xFF)   # 故障位元圖（高 byte）
```

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：較近距離 cm、左距離 mm、右距離 mm、左右平均 mm。要用這些值請改用 <a href="/sensors/tof2/spike/official.md">樂高官方 App</a>／<a href="/sensors/tof2/spike/pybricks.md">Pybricks</a>。</p>

## 範例

<!-- examples: sensors/tof2/spike-examples.md host=spike sw=mbcblocks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MBC SPIKE 積木網站（線上，不用安裝）](https://mbc-spike-blocks.mbcrobot.workers.dev)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/tof2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-mbcblocks.zh.md`、`_build/guides/product/tof2/spike-mbcblocks.zh.md`
- 範例：`sensors/tof2/spike-examples.md`
