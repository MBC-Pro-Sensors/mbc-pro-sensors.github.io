<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 02_相機感應器\06_通用版\通用版v1\上位機函式庫\tools\gen_pages.py 產生（範本 tools\pages\ev3-examples.zh.md）；程式碼區塊＝上位機函式庫 正本的原文，改程式請改正本後重跑 -->

# 🤖 EV3 範例：神攝手通用版 v1

[🔙 回到神攝手通用版 v1](/sensors/unicam/index.md)　｜　[EV3 讀法說明](/sensors/unicam/ev3/index.md)　｜　[📋 讀值參考：EV3](/sensors/unicam/reference.md#host-ev3)

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

這一頁是 **EV3 的 Python 範例 3 個**：跟著色塊轉、座標取色判斷顏色、PD 循線＋路口停車。同一支程式在 **Pybricks v4** 和 **樂高官方 EV3 MicroPython v2** 都能跑，函式庫 `MBC_UniCam_EV3.py` 會自己判斷。EV3-G、EV3 Classroom 用紅外線感應器積木讀，做法在 [EV3 讀法說明](/sensors/unicam/ev3/index.md)。

| 軟體 | 怎麼用 |
|---|---|
| 🐍 **Pybricks v4**（[code.pybricks.com](https://code.pybricks.com)，EV3 要先燒 Pybricks 韌體） | 左邊「Import a file」→ 範例檔和 [`MBC_UniCam_EV3.py`](/examples/unicam/ev3/MBC_UniCam_EV3.py) **兩個都要匯入** → ▶ |
| 🐍 **樂高官方 EV3 MicroPython v2**（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充） | 範例檔和 `MBC_UniCam_EV3.py` 放在同一個專案資料夾 → 下載並執行 |

> [!IMPORTANT]
> - 相機要用 **EV3 版韌體**。相機在 EV3 上顯示成**樂高紅外線感應器**：模式 0 近距＝模式編號、模式 1 信標＝8 個值、模式 2 遙控＝4 個值、模式 5＝裝置 ID。
> - **EV3 讀得到相機目前的模式**：`UniCam(Port.S1)` 不填 mode，第一次用時自動讀模式 0（多約 0.5 秒）；範例會先檢查模式對不對，切錯了會嗶一聲、印提醒。
> - **EV3 每換一次模式約 0.5 秒**：同一個迴圈盡量只用同一個 EV3 模式的值（下面的表有寫每個值在哪個模式）。

📦 全部平台的函式庫一包下載：[MBC_UniCam_v1_Host_Libraries.zip](/downloads/MBC_UniCam_v1_Host_Libraries.zip)

## 開始之前：在相機上教色、切模式

1. **教色**：即時畫面用左右鍵停在「目標 1」→ **OK** → 把十字移到色塊上（AxisX → AxisY）→ **OK** 取色。
2. **切模式**：左右鍵停在 **MODE** → **OK** → 選相機模式 → **OK** → 選子選項 → **OK**。螢幕右上的「1-3」＝模式編號 13。子選項會顯示 EV3 實際給幾個（例如「8 XY」）。

| 模式編號 | 相機選單 | EV3 給什麼 |
|---|---|---|
| 11 | 1 顏色追蹤 ① 最多色 | 12 色位置（8×6 格）；色 1～8 在模式 1、色 9～12 在模式 2 |
| 12 | 1 顏色追蹤 ② 多色 | 8 色位置（模式 1） |
| 13 | 1 顏色追蹤 ③ 標準 | 4 色位置＋面積 %（模式 1） |
| 14 | 1 顏色追蹤 ④ 精細 | 2 色位置、寬高（8×6 格）、數量、面積 %（模式 1） |
| 21 | 2 座標取色 ① 色相 | 12 點色相（0～100 送，換回色相碼 ±2）；點 1～8 模式 1、9～12 模式 2 |
| 22 | 2 座標取色 ② | 4 點色相（±2）＋飽和、明度（取到十位）（模式 1） |
| 23 | 2 座標取色 ③ | 2 點原始色相碼、飽和、明度（模式 1） |
| 30 | 3 照順序給色號 | 左起 11 塊色相（±2）＋總塊數（模式 1＋2） |
| 41／42／43 | 4 循線 | 近中遠偏移、角度、線寬、路口、旗標、路口距離、標記色號（模式 1）；左右緣、遠線寬、標記條數（模式 2）；EV3 沒有離線方向 |

**數字怎麼看**：函式庫把 EV3 的 8×6 格**換成像素**（X 0～159 左→右、Y 0～119 上→下，跟 SPIKE 一樣），是格子中心，誤差 ±10 像素；**(0, 0)＝沒找到**。要 EV3 原始的格子（Y 下＝小）用 `grid(c)`。循線偏移 −100～100，**站在車後往前看，線在左＝負**。

## 函式庫 MBC_UniCam_EV3.py 的方法

`cam = UniCam(Port.S1)` 或 `UniCam(Port.S1, mode=13)`（孔位也可以寫 1～4）。方法名稱跟 SPIKE 的 `MBC_UniCam.py` 一樣。

| 方法 | 回傳 | 哪些模式 |
|---|---|---|
| `mode_number()` | 相機目前的模式編號（mode 沒填時讀模式 0） | 全部 |
| `xy(c)`、`x(c)`、`y(c)`、`found(c)`、`grid(c)` | 位置（像素 ±10）、有沒有找到、原始 8×6 格 | 11～14 |
| `area(c)` | 面積 % | 13、14 |
| `size(c)`、`count(c)` | (寬, 高) 像素 ±10、數量 | 14 |
| `max_color()`／`max_point()` | 這個模式給幾色／幾點 | 1x／2x |
| `hue(i)`、`sat(i)`、`val(i)`、`hsv(i)`、`color(i)` | 色相碼、飽和、明度、顏色名稱 | 21～23 |
| `order()`、`total()` | 左起每塊色相碼（≤11）、總塊數 | 30 |
| `near()` `mid()` `far()` `angle()` `width()` `junction()` `flags()` `junction_dist()` | 循線（模式 1） | 41～43 |
| `edges()`、`far_width()`、`marker()`、`line_all()` | 循線（要讀模式 2，會多 0.5 秒） | 41～43 |
| `device_id()`、`is_unicam()`、`sensor_ok()` | 模式 5：(0x4C, 3, 100)、True、最近有沒有讀到 | 全部 |

- EV3 讀不到的值（模式 11～13 的數量、找到幾色、離線方向）呼叫會丟 `ValueError`；模式不對也會丟，訊息中英並列。
- 讀不到相機時回傳 `None`，範例都會先判斷 `None` 再用。

---

## 範例 1：跟著色塊轉

**這個範例會做什麼**：車子轉向色 1，邊轉邊往前開；面積夠大就只轉不前進；看不到就原地慢慢轉著找。開頭會檢查相機是不是在模式 13。

**相機要切到**：相機模式 1 顏色追蹤 ・子選項 ③ 標準（XY＋面積）＝**模式編號 13**；先教「目標 1」。

**要準備什麼**：EV3 主機、相機插 **1 號孔**、左輪馬達 **B**、右輪 **C**、目標色的球或積木。

**下載**：[ex01_track_turn.py](/examples/unicam/ev3/ex01_track_turn.py)＋[MBC_UniCam_EV3.py](/examples/unicam/ev3/MBC_UniCam_EV3.py)

<details>
<summary>🐍 Python 程式碼（Pybricks v4／EV3 MicroPython v2）</summary>

```python
#!/usr/bin/env pybricks-micropython
# 神攝手通用版 v1 × EV3 範例 1：跟著色塊轉
# Sharpshooter Vision Universal v1 × EV3 example 1: turn to follow a colour blob
# 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ③ 標準（XY＋面積）＝模式編號 13
# Camera menu: camera mode 1 Colour tracking, option 3 Standard (XY + area) = mode number 13
# 軟體 Software：Pybricks v4（code.pybricks.com）或 樂高官方 EV3 MicroPython v2（同一支程式都能跑）
# 準備：先在相機上教「目標 1」的顏色 / teach "target 1" on the camera first
# 接線 Wiring：相機 → 1 號孔 / camera → port 1；左輪 B、右輪 C / left wheel B, right wheel C
# 同一個資料夾要有 MBC_UniCam_EV3.py / put MBC_UniCam_EV3.py in the same folder
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.parameters import Port
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam_EV3 import UniCam

ev3 = EV3Brick()
cam = UniCam(Port.S1)                            # 不填 mode：自動讀相機目前的模式 / mode read from the camera
left = Motor(Port.B)
right = Motor(Port.C)
robot = DriveBase(left, right, 56, 114)          # 輪徑 56 mm、輪距 114 mm / wheel 56 mm, axle 114 mm

if cam.mode_number() != 13:                      # EV3 讀得到相機的模式，切錯了就提醒 / EV3 can check the mode
    print('請把相機切到 模式 13（顏色追蹤 ③）/ please set the camera to mode 13')
    ev3.speaker.beep(200, 500)
    wait(3000)
    raise SystemExit

KP = 3            # 👉 可以改：偏 1 像素就多轉幾度／秒 / turn rate per pixel of error
STOP_AREA = 20    # 👉 可以改：面積 ≥ 20 % 就算很近 / area (%) that counts as "close"

while True:
    p = cam.xy(1)                                # 色 1 的 (X, Y) 像素（EV3 是 8×6 格，±10）/ pixels (±10 on EV3)
    if p is None:                                # 讀不到相機 / no camera
        robot.stop()
    elif p[0] == 0:                              # 沒看到：原地慢慢轉著找 / not seen: spin to search
        robot.drive(0, 60)
    else:
        error = p[0] - 80                        # 畫面中間 X = 80；色塊在右 → 正 / right of centre = positive
        speed = 0 if cam.area(1) >= STOP_AREA else 80
        robot.drive(speed, error * KP)           # 面積在同一個模式（模式 1），不用多等 / same EV3 mode, no wait
    wait(20)
```

</details>

**一步一步看懂程式**
1. `UniCam(Port.S1)`：不填 mode → `cam.mode_number()` 讀模式 0 拿到相機的模式編號，不是 13 就提醒並結束。
2. `cam.xy(1)`、`cam.area(1)`：都在 EV3 模式 1（信標），讀起來不用換模式。
3. `error = X − 80`：色塊在右是正的 → 轉向正＝往右轉。

**跑起來應該看到什麼**：球在左車頭往左轉、在右往右轉；很近就原地對準；球拿走原地轉圈找。

**常見問題**
- **一開始就嗶一聲結束？** → 相機不在模式 13：在相機選單 MODE 切到「1 顏色追蹤 → ③」，螢幕右上要顯示 `1-3`。
- **左右晃？** → EV3 位置是 8×6 格（每格 20 像素），`KP` 改小一點（例如 2）。

---

## 範例 2：座標取色判斷顏色

**這個範例會做什麼**：讀取樣點 S1、S2 的原始色相碼、飽和、明度，判斷顏色並印出來；S1 是紅、綠、橙、黃時 EV3 的燈亮那個顏色；兩點同色時多印一行。

**相機要切到**：相機模式 2 座標取色 ・子選項 ③ 色相＋飽和＋明度＝**模式編號 23**（EV3 給 2 點，數字是原始值）。

**要準備什麼**：EV3 主機、相機插 **1 號孔**、各色積木。

**下載**：[ex02_spot_color.py](/examples/unicam/ev3/ex02_spot_color.py)＋[MBC_UniCam_EV3.py](/examples/unicam/ev3/MBC_UniCam_EV3.py)

<details>
<summary>🐍 Python 程式碼（Pybricks v4／EV3 MicroPython v2）</summary>

```python
#!/usr/bin/env pybricks-micropython
# 神攝手通用版 v1 × EV3 範例 2：座標取色判斷顏色
# Sharpshooter Vision Universal v1 × EV3 example 2: what colour is at the spot?
# 需要把相機選單切到 相機模式 2 座標取色 ・子選項 ③ 色相＋飽和＋明度 ＝模式編號 23（EV3 給 2 點，原始值）
# Camera menu: camera mode 2 Spot colour, option 3 Hue + sat + value = mode number 23 (2 spots, exact values)
# 軟體 Software：Pybricks v4 或 樂高官方 EV3 MicroPython v2
# 準備：取樣點 S1、S2 的位置看相機螢幕上標 1、2 的十字 / spots S1, S2 are the crosses on the camera screen
# 接線 Wiring：相機 → 1 號孔 / camera → port 1
from pybricks.hubs import EV3Brick
from pybricks.parameters import Port, Color
from pybricks.tools import wait
from MBC_UniCam_EV3 import UniCam, color_name

ev3 = EV3Brick()
cam = UniCam(Port.S1, mode=23)                   # 填了 mode 就不用先讀模式 0 / giving mode skips one mode switch

NAME = {'red': '紅', 'orange': '橙', 'yellow': '黃', 'green': '綠', 'cyan': '青',
        'blue': '藍', 'purple': '紫', 'white': '白／灰', 'black': '黑'}
LIGHT = {'red': Color.RED, 'orange': Color.ORANGE, 'yellow': Color.YELLOW, 'green': Color.GREEN}
LOW_SAT = 15      # 👉 可以改：飽和度比這低就當成白／灰 / below this saturation = white / grey

while True:
    names = []
    for point in (1, 2):
        p = cam.hsv(point)                       # (色相碼, 飽和度, 明度) / (hue, sat, val)
        if p is None:
            names.append(None)
            continue
        n = color_name(p[0])
        if p[1] < LOW_SAT and n != 'black':
            n = 'white'
        names.append(n)
        print('S%d 色相碼 hue %d 飽和 sat %d 明度 val %d → %s %s' % (point, p[0], p[1], p[2], NAME[n], n))
    if names[0] in LIGHT:                        # EV3 的燈只有紅、綠、橙、黃 / EV3 light: red, green, orange, yellow
        ev3.light.on(LIGHT[names[0]])
    else:
        ev3.light.off()
    if names[0] is not None and names[0] == names[1]:
        print('兩點一樣顏色 / both spots are the same colour')
    wait(300)
```

</details>

**一步一步看懂程式**
1. `UniCam(Port.S1, mode=23)`：填了 mode 就不用先讀模式 0。
2. `cam.hsv(point)`：EV3 模式 23 用兩格放一個色相碼（百位、餘數），函式庫已經合回 0～255。
3. `color_name()`：色相碼換成顏色名稱，分界跟 SPIKE 版一樣。

**跑起來應該看到什麼**：每 0.3 秒印兩行，例如 `S1 色相碼 hue 5 飽和 sat 80 明度 val 70 → 紅 red`。

**常見問題**
- **想看更多點？** → 模式 21 給 12 點色相（±2），但點 9～12 在模式 2，要多 0.5 秒。
- **飽和、明度只有 10 的倍數？** → 那是模式 22（EV3 把兩個數字壓在一格）；要原始值用模式 23。

---

## 範例 3：PD 循線＋路口停車

**這個範例會做什麼**：近處偏移為主、遠處偏移提早轉彎的 PD 循線；十字或 T 字路口停車嗶一聲；三條掃描線都沒看到線時往上一次偏的那邊轉。只用 EV3 模式 1 的值，迴圈裡不換模式。

**相機要切到**：相機模式 4 循線 ・子選項 ① 黑線＝**模式編號 41**（白線 42、色線 43 也可以，程式會檢查是不是循線模式）。

**要準備什麼**：EV3 主機、相機插 **1 號孔**裝在車頭斜斜往前下方看、左輪 **B**、右輪 **C**、白底黑線場地。

**下載**：[ex03_line_follow.py](/examples/unicam/ev3/ex03_line_follow.py)＋[MBC_UniCam_EV3.py](/examples/unicam/ev3/MBC_UniCam_EV3.py)

<details>
<summary>🐍 Python 程式碼（Pybricks v4／EV3 MicroPython v2）</summary>

```python
#!/usr/bin/env pybricks-micropython
# 神攝手通用版 v1 × EV3 範例 3：PD 循線＋路口停車
# Sharpshooter Vision Universal v1 × EV3 example 3: PD line following + stop at a junction
# 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線 ＝模式編號 41（白線 42、色線 43 也可以）
# Camera menu: camera mode 4 Line, option 1 Black line = mode number 41 (42 / 43 also work)
# 軟體 Software：Pybricks v4 或 樂高官方 EV3 MicroPython v2
# 準備：相機裝在車頭、斜斜往前下方看線 / camera at the front, looking down and forward
# 接線 Wiring：相機 → 1 號孔；左輪 B、右輪 C / camera → port 1; left wheel B, right wheel C
# 方向：站在車後往前看，線在左＝負 / standing behind the robot: line on the left = negative
# EV3 每換一次模式約 0.5 秒：這支只用模式 1 的值（近、遠、旗標、路口）/ only mode-1 values (no mode switch)
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.parameters import Port
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam_EV3 import UniCam

ev3 = EV3Brick()
cam = UniCam(Port.S1)
left = Motor(Port.B)
right = Motor(Port.C)
robot = DriveBase(left, right, 56, 114)

if cam.mode_number() not in (41, 42, 43):        # 相機沒切到循線就提醒 / camera not in line mode
    print('請把相機切到 循線（模式 41／42／43）/ please set the camera to line mode')
    ev3.speaker.beep(200, 500)
    wait(3000)
    raise SystemExit

SPEED = 120       # 👉 可以改：前進速度 mm/s / forward speed
KP = 1.5          # 👉 可以改：比例 / proportional gain
KD = 4.0          # 👉 可以改：微分 / derivative gain
last_error = 0

while True:
    junction = cam.junction()                    # 0 無 1 十字 2 T 3 左 4 右 5 盡頭 / junction type
    if junction is None:                         # 讀不到相機：停車 / no camera: stop
        robot.stop()
        wait(50)
        continue
    if junction in (1, 2):                       # 十字或 T 字路口：停車嗶一聲 / cross or T: stop
        robot.stop()
        ev3.speaker.beep()
        break
    if cam.flags() == 0:                         # 近中遠都沒看到線：往上一次偏的那邊找 / line lost
        robot.drive(0, -120 if last_error < 0 else 120)
        wait(10)
        continue
    error = 0.7 * cam.near() + 0.3 * cam.far()   # 近處為主、遠處提早轉彎 / mostly near, a bit of far
    turn = KP * error + KD * (error - last_error)
    last_error = error
    robot.drive(SPEED, turn)                     # 線在左（負）→ 往左轉 / line on the left → turn left
    wait(10)
```

</details>

**一步一步看懂程式**
1. `cam.junction()`：路口類型（0 無 1 十字 2 T 3 左 4 右 5 盡頭）；EV3 把它跟有線旗標放在同一格（路口×10＋旗標），函式庫已經拆開。
2. `cam.flags()`：近 1＋中 2＋遠 4；0＝都沒看到線。
3. `error = 0.7 × 近 + 0.3 × 遠`，線在左是負的 → 往左轉；`KD` 讓轉彎比較穩。

**跑起來應該看到什麼**：車子沿線走，彎道前提早轉；到十字路口停車嗶一聲，程式結束。

**常見問題**
- **很慢、一頓一頓？** → 迴圈裡用到 `edges()`、`far_width()`、`marker()`、`line_all()` 會換到模式 2（每次 0.5 秒），循線迴圈不要用。
- **晃得厲害？** → `KP` 改小、`KD` 改大，或 `SPEED` 改慢。

## 要注意的事

- **以下都還沒上機**：數值排法已經在電腦上跟規格的獨立參考實作逐格比對過（Pybricks v4 與 EV3 MicroPython v2 兩種讀法都測）；EV3 MicroPython v2 的 `Ev3devSensor` 模式名稱（IR-PROX、IR-SEEK、IR-REMOTE、IR-CAL）照樂高紅外線感應器，要上機確認。
- KP／KD、面積門檻是建議值，先印出數字再調。

## 下一步

- 每一格的完整對照：[讀值參考：EV3](/sensors/unicam/reference.md#host-ev3)
- SPIKE 版範例：[SPIKE 範例](/sensors/unicam/spike-examples.md)
