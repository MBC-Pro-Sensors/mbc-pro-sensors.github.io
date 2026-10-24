<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 _build/gen-guides.mjs 產生（ps2 × spike × pybricks），請勿手改；改內容請改 _build/guides.json、_build/guides/**、_build/protocol/*.json 後重跑 node _build/gen-guides.mjs -->
<!-- guide: ps2/spike/pybricks -->
# 掌控者 PS2 (PS2) × SPIKE × Pybricks

這一頁只講 **Pybricks（Python）**：在 SPIKE Prime／Robot Inventor 上用它讀掌控者 PS2 (PS2)的安裝步驟、每個值怎麼讀、能用的命令與範例。要換產品、主機或軟體，用上方的切換列。

## 安裝與設定

<!-- setup: _build/guides/software/spike-pybricks.zh.md + _build/guides/product/ps2/spike-pybricks.zh.md {"product":"掌控者 PS2 (PS2)","edition":"SPIKE 版","port":"C","lib":"MBC_gamepad_Lib.py","libUrl":"/examples/ps2/spike_beginner/pybricks/MBC_gamepad_Lib.py","addr":"0x42"} -->
<!-- include-fallback -->
> [!NOTE]
> 這個軟體的安裝步驟正在搬到這一頁。先看原本的說明：[掌控者 PS2 (PS2)：Pybricks（Python） 說明](/sensors/ps2/spike-examples.md)。
<!-- /include-fallback -->

## 讀值

程式假設感應器插 A 孔。「MBC 函式庫」寫法要先把函式庫 .py 一起匯入 Pybricks。

<section id="v01" class="ref-item">

### 十六鍵 <small>buttons16</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-buttons16.svg" alt="十六鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~65535` bit | 16 個按鍵合成一個數（PSX 排列，1 = 按下）：bit0 SELECT、1 L3、2 R3、3 START、4 上、5 右、6 下、7 左、8 L2、9 R2、10 L1、11 R1、12 ▲、13 ●、14 ✕、15 ■；未連線 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

buttons16 = dev.read(9)[6]   # 十六鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
buttons16 = pad.buttons_psx()
```

<p class="ref-note">模式 0 (edu) byte 6 🔒；模式 5 v[0] 🔒；模式 6 v[2]；模式 9 v[6] 🔒。備註：App 原始藍 = button16（.ino:201-211）; Pybricks reflection() 編碼：11 鍵 complexXY = SELECT<<10 | R1<<9 | L1<<8 | ■<<7 | ✕<<6 | ●<<5 | ▲<<4 | 左<<3 | 下<<2 | 右<<1 | 上，a = /25、b = (餘+1)×10−1，3 個 int16 = a×256+b → reflection() = complexXY；L2、R2、L3、R3、START 讀不到（.ino:334-358）; v2 = button16（PSX）; v6 = button16（PSX）</p>

</section>

<section id="v02" class="ref-item">

### 方向鍵 <small>dpad</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-dpad.svg" alt="方向鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | 上 1、右 2、下 4、左 8（同時按相加），0~15；未連線 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

dpad = dev.read(9)[0]   # 方向鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
dpad = pad.pressed('UP')
```

<p class="ref-note">模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 0 🔒；模式 1 byte 0；模式 6 v[0] lo；模式 9 v[0] 🔒。備註：byte1 = 方向 | 形狀<<4（App 反射光）：低 4 bit; byte0 = 方向 | 形狀<<4; 低 4 bit（高 4 bit = 形狀）; v0 = 方向 | 形狀<<4（bit0-3）</p>

</section>

<section id="v03" class="ref-item">

### 形狀鍵 <small>shape</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shape.svg" alt="形狀鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | ▲ 1、● 2、✕ 4、■ 8（同時按相加），0~15；未連線 = 0；0x22 的 R 高 4 bit |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

shape = dev.read(9)[0]   # 形狀鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
shape = pad.pressed('TRIANGLE')
```

<p class="ref-note">模式 0 (edu) byte 1 🔒；模式 0 (toy) byte 0 🔒；模式 1 byte 0；模式 6 v[0] hi；模式 9 v[0] 🔒。備註：高 4 bit; 高 4 bit; 高 4 bit; v0 bit4-7</p>

</section>

<section id="v04" class="ref-item">

### 肩鍵 <small>shoulder</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-shoulder.svg" alt="肩鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | L2 1、R2 2、L1 4、R1 8（同時按相加），0~15；未連線 = 0 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

shoulder = dev.read(9)[1]   # 肩鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
shoulder = pad.pressed('L1')
```

<p class="ref-note">模式 0 (edu) byte 0 🔒；模式 0 (toy) byte 6 🔒；模式 2 byte 0；模式 6 v[1] lo；模式 9 v[1] 🔒。備註：byte0 = 肩 | 功能<<4（App 顏色，START = −128）：低 4 bit; byte6~7 = (肩 | 功能<<4) 經玩具版查表編碼；byte1 = 單鍵編號（L2=1 R2=2 L1=3 R1=4 SELECT=5 L3=6 R3=7 START=8，沒按 0）; 低 4 bit（高 4 bit = 功能）; v1 = 肩 | 功能<<4（bit0-3）</p>

</section>

<section id="v05" class="ref-item">

### 功能鍵 <small>func</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-func.svg" alt="功能鍵" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `0~15` bit | SELECT 1、L3 2、R3 4、START 8（同時按相加），0~15；未連線 = 0；0x22 的 G 高 4 bit |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

func = dev.read(9)[1]   # 功能鍵
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
func = pad.pressed('START')
```

<p class="ref-note">模式 0 (edu) byte 0 🔒；模式 2 byte 0；模式 6 v[1] hi；模式 9 v[1] 🔒。備註：高 4 bit; 高 4 bit; v1 bit4-7</p>

</section>

<section id="v06" class="ref-item">

### 已連線 <small>connected</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0~1` | 1 = 手把已連線（psx.read() 成功），0 = 沒有；未連線時所有按鍵 0、搖桿回中 |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

connected = dev.read(3)[0]   # 已連線
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
connected = pad.connected()
```

<p class="ref-note">模式 3 byte 0。備註：#67 改：LIGHT 3×int8 = [已連線, 電量, 0]（原本 = 4 組按鍵）</p>

</section>

<section id="v07" class="ref-item">

### 電量 <small>battery</small>

| 範圍 | 意思 |
| :--- | :--- |
| `0` % | 0~100；PS2 手把沒有電量 → 恆 0（PS4 才有） |

```python
from pybricks.iodevices import PUPDevice
from pybricks.parameters import Port

dev = PUPDevice(Port.A)   # 感應器插 A 孔

battery = dev.read(3)[1]   # 電量
```

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
battery = pad.battery()
```

<p class="ref-note">模式 3 byte 1。備註：#67 改：PS2 送 0</p>

</section>

<section id="v10" class="ref-item">

### 搖桿 LX／LY／RX／RY <small>stick</small>

<figure class="ref-fig"><img src="/images/figures/ps2/value-stick.svg" alt="搖桿 LX／LY／RX／RY" loading="lazy"></figure>

| 範圍 | 意思 |
| :--- | :--- |
| `-100~+100` | 10 LX、11 LY、12 RX、13 RY：−100 ~ +100，0 = 中間，右／上為正（原始 0~255 死區 ±7 吸成中心）；未連線 = 0。EV3／SPIKE／MATRIX 送的是 0~99（50 = 中間）或 1~9（5 = 中間），見各平台欄 |

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

**MBC 函式庫**

```python
from pybricks.parameters import Port
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.A)
pad.update()   # 每圈先更新一次
stick = pad.stick('LX')   # list，第 1 路 = [0]
```

<p class="ref-note">模式 0 (edu) byte 2 🔒；模式 0 (toy) byte 2 🔒；模式 4 v[0]；模式 7 v[2] 🔒；模式 8 v[0]；模式 9 v[2] 🔒。備註：byte2~3 = LX×10+LY、byte4~5 = RX×10+RY（11~99，中心 55；App 原始紅／綠）; LX×10+LY、RX×10+RY 經玩具版查表編碼成 byte2~3、byte4~5; v0 = LX | LY<<8、v1 = RX | RY<<8; Pybricks ambient() 用：v2 = LX×1000 + LY×100 + RX×10 + RY（1111~9999，中心 5555）；v0、v1 同模式 6; 只送 4 B：v0 = LX | LY<<8、v1 = RX | RY<<8; v2~v5 = LX、LY、RX、RY（0~99 ÷10，0 補 1）</p>

</section>

<p class="ref-missing"><b>這個軟體讀不到</b>：L2 類比、R2 類比、系統鍵、手把類型。要用這些值請改用 <a href="/sensors/ps2/arduino/ide.md">Arduino</a>／<a href="/sensors/ps2/mblock/mblock5.md">mBlock</a>／<a href="/sensors/ps2/microbit/makecode.md">micro:bit</a>。</p>

## 命令

SPIKE 接頭是唯讀的，這個軟體不能送命令：校準用感應器上的按鈕，切黑白線、門檻等設定改用 Arduino I2C、mBlock 或 micro:bit。

## 範例

<!-- examples: sensors/ps2/spike-examples.md host=spike sw=pybricks -->

## 其他問題

- **主機認不到感應器？** 拔掉重插一次，等 2 秒；換一個孔試試。官方 App 會把它顯示成「顏色感應器」，這是正常的。
- **可以同時用官方 App 和 Pybricks 嗎？** 一次只能裝一種韌體：Pybricks 與 MBC 積木網站用 Pybricks 韌體，官方 App 用樂高原廠韌體；隨時可以用 SPIKE App 換回原廠韌體。

還有問題？到 [聯絡我們](/contact.md) 附上你用的軟體和程式截圖。

## 下載

- [Pybricks 函式庫 MBC_gamepad_Lib.py](/examples/ps2/pybricks/MBC_gamepad_Lib.py)
- [ps2_pybricks_spike_v3.6.1.zip](/examples/ps2/ps2_pybricks_spike_v3.6.1.zip)
- [下載中心（全部產品的函式庫、韌體、型錄）](/downloads.md)

## 本頁依據

- 主機與軟體清單：`_build/guides.json`
- 值與命令：`_build/protocol/ps2.json`（單一真相來源）
- 安裝與設定：`_build/guides/software/spike-pybricks.zh.md`、`_build/guides/product/ps2/spike-pybricks.zh.md`
- 範例：`sensors/ps2/spike-examples.md`
