<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × spike × mbcblocks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/spike/mbcblocks -->
# 掌控者 PS2 (PS2) × SPIKE × MBC 積木網站

這一頁只講 **MBC SPIKE 積木網站**：在 SPIKE Prime／Robot Inventor 上用它讀掌控者 PS2 (PS2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-mbcblocks.zh.md + _build/guides/product/ps2/spike-mbcblocks.zh.md {"product":"掌控者 PS2 (PS2)","edition":"SPIKE 版","port":"C","lib":"MBC_gamepad_Lib.py","libUrl":"/examples/ps2/spike_beginner/pybricks/MBC_gamepad_Lib.py","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS2 (PS2)：MBC SPIKE 積木網站 說明](/sensors/ps2/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

主機要先換成 Pybricks 韌體。MBC 積木網站的感應器積木讀的是感應器的模式 9（一次讀完全部數值）。

<section id="v01" class="ref-item">

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 16 個按鍵合成一個數（PSX 排列，1 = 按下）：bit0 SELECT、1 L3、2 R3、3 START、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；未連線 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「十六鍵」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

buttons16 = dev.read(9)[6]   # 十六鍵
```

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「方向鍵」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

dpad = dev.read(9)[0]   # 方向鍵
```

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |

在 MBC 積木網站加入這款感應器的積木，用「形狀鍵」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

shape = dev.read(9)[0]   # 形狀鍵
```

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |

在 MBC 積木網站加入這款感應器的積木，用「肩鍵」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

shoulder = dev.read(9)[1]   # 肩鍵
```

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |

在 MBC 積木網站加入這款感應器的積木，用「功能鍵」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

func = dev.read(9)[1]   # 功能鍵
```

</section>

<section id="v10" class="ref-item">

### 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

在 MBC 積木網站加入這款感應器的積木，用「搖桿 LX／LY／RX／RY」積木讀；積木在主機上執行的是下面這段 Pybricks 程式（模式 9）：

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

words = dev.read(9)[2:6]
stick = []
for w in words:
    stick.append(w & 0xFF)          # 奇數路
    stick.append((w >> 8) & 0xFF)   # 偶數路
```

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：已連線、電量、L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps2/spike/pybricks.md">Pybricks</a>。</p>

## 命令

SPIKE 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/ps2/spike-examples.md host=spike sw=mbcblocks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [MBC SPIKE 積木網站（線上，不用安裝）](https://mbc-spike-blocks.mbcrobot.workers.dev)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-mbcblocks.zh.md`、`_build/guides/product/ps2/spike-mbcblocks.zh.md`
- 範例：`sensors/ps2/spike-examples.md`
