<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 02_相機感應器\06_通用版\通用版v1\上位機函式庫\tools\gen_pages.py 產生（範本 tools\pages\spike-examples.zh.md）；程式碼區塊＝上位機函式庫 正本的原文，改程式請改正本後重跑 -->

# 🧱 SPIKE 範例：神攝手

[🔙 回到神攝手](/sensors/unicam/index.md)　｜　[SPIKE 讀法說明](/sensors/unicam/spike/index.md)　｜　[📋 讀值參考](/sensors/unicam/reference.md)

> [!WARNING]
> **v1 尚未上機驗證**：數值排法已定案並通過電腦端測試，實機數值（fps、循線門檻、I2C 腳位）上機後可能調整。

這一頁有 **Pybricks 6 個範例**（用 MBC 函式庫 `MBC_UniCam.py`）和 **樂高官方 SPIKE App（教育版）Python 2 個範例**，最後是官方 App 積木的拆值表（教育版、家用版）。題目都是學生最常用的：跟著色塊轉、多色找位置、座標取色判斷顏色、照順序給色號（WRO 顏色順序）、PD 循線＋路口停車、物體偵測（找最大的紅色三角形）。

| 寫法 | 適合誰 | 怎麼用 |
|---|---|---|
| 🐍 **Pybricks**（Python） | 想要全部的值（14 色、數量、循線 14 個值） | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊「Import a file（匯入檔案）」→ 範例檔和 [`MBC_UniCam.py`](/examples/unicam/pybricks/MBC_UniCam.py) **兩個都要匯入** → 點開範例 → ▶ |
| 🟨 **樂高官方 SPIKE App 3（教育版）Python** | 學校的 SPIKE App、不換韌體 | App 新增「Python」專案 → 整支範例貼上 → ▶（讀值小工具已經在範例最上面） |
| 🟨 官方 App 積木（教育版／家用版） | 只用積木 | 照最下面的[拆值表](#app-table)用「顏色、反射光、原始紅綠藍」積木 |

> [!IMPORTANT]
> - **Pybricks 讀不到相機目前的模式**：程式裡的 `mode=13` 要跟相機選單切的一樣（模式編號＝相機模式 ×10＋子選項，照順序給色號＝30）。切錯了數字會亂跳，但不會報錯。
> - Pybricks 要先把主機換成 Pybricks 韌體（code.pybricks.com 的設定裡可以燒，之後可用 SPIKE App 換回）。官方 App 不用換。
> - 相機一定要用 **SPIKE 版韌體**。同一個韌體會自己認出主機是官方 App 還是 Pybricks。

📦 全部平台的函式庫一包下載：[MBC_UniCam_v1_Host_Libraries.zip](/downloads/MBC_UniCam_v1_Host_Libraries.zip)（Pybricks、EV3、SPIKE App、Arduino、micro:bit、MATRIX）

## 開始之前：在相機上教色、切模式

1. **教色**：即時畫面用左右鍵停在「目標 1」→ 按 **OK** → 用左右鍵把十字移到色塊上（AxisX → AxisY）→ **OK** 取色（按住 OK 0.8 秒直接完成）。目標 2、3…照做。停在目標上按**側鍵**＝清除這個目標。
2. **切模式**：左右鍵停在 **MODE** → **OK** → 選相機模式 1～5 → **OK** → 選子選項 → **OK**。子選項會顯示這台主機實際給幾個（例如 Pybricks「7 XY」）。螢幕右上的「1-3」就是現在的模式（＝模式編號 13）。
3. **物體偵測（相機模式 5）**：教完目標 1～8 會跳出 `Target N shape`，可以指定形狀（Any＝不指定）或按側鍵跳過；只有目標色 1～8 會當成物體。
4. 設定斷電會記住（目標色、模式、子選項、指定形狀）。

| 模式編號 | 相機選單 | Pybricks 給什麼 |
|---|---|---|
| 11 | 1 顏色追蹤 ① 最多色 | 14 色位置（16×12 格，函式庫換成像素 ±5）；數量讀模式 6 |
| 12 | 1 顏色追蹤 ② 多色 | 7 色 X、Y（像素）；數量讀模式 6 |
| 13 | 1 顏色追蹤 ③ 標準 | 4 色 X、Y、面積 %、數量 |
| 14 | 1 顏色追蹤 ④ 精細 | 2 色 X、Y、寬、高、數量、面積 %；找到幾色 |
| 21／22／23 | 2 座標取色 ①②③ | 14 點色相／7 點色相＋飽和／4 點色相＋飽和＋明度 |
| 30 | 3 照順序給色號 | 左起 13 塊的色相碼＋總塊數 |
| 41／42／43 | 4 循線 黑線／白線／色線 | 14 個值（近中遠偏移、角度、線寬、旗標、路口、左右緣、標記線、離線方向） |
| 51 | 5 物體偵測 ① 物體清單 | 4 個物體的代碼、X、Y（像素）＋物體數 |
| 52 | 5 物體偵測 ② 物體詳細 | 2 個物體的色號、形狀、X、Y、寬、高、面積 %（沒有物體數、角度） |

**數字怎麼看**：座標是像素，X 0～159（左→右）、Y 0～119（上→下），**(0, 0)＝沒找到**。色相碼 0～255：紅 0／240、黃 40、綠 80、青 120、藍 160、紫 200；241～248 白／灰；249～255 黑。循線偏移 −100～100，**站在車後往前看，線在左＝負**。

**物體（模式 51／52）**：物體代碼＝**色號×10＋形狀**（例 13＝目標 1 的三角形），0＝沒有。形狀 0 其他、1 圓、2 橢圓、3 三角、4 正方、5 長方、6 五邊、7 六邊、8 星、9 梯形。物體 1、2…＝**面積由大到小**（物體 1 最大）；X、Y 是質心。只看目標色 1～8。

## 函式庫 MBC_UniCam.py 的方法

`cam = UniCam(Port.C, mode=13)`（孔位也可以寫 1～6；`UniCam(Port.C, 13, True)`＝多工，每個方法都要 `await`）

| 方法 | 回傳 | 哪些模式 |
|---|---|---|
| `xy(c)`、`x(c)`、`y(c)`、`found(c)` | 色 c 的 (X, Y) 像素、有沒有找到 | 11～14 |
| `area(c)` | 面積（佔畫面 %） | 13、14 |
| `size(c)` | (寬, 高) 像素 | 14 |
| `count(c)` | 色 c 有幾塊（11 是 0～7，讀模式 6） | 11～14 |
| `colors_found()` | 16 色裡找到幾色（11、12 讀模式 6） | 11、12、14 |
| `cell(c)` | 16×12 格 (1～16, 1～12) | 11 |
| `max_color()`／`max_point()` | 這個模式給幾色／幾點 | 1x／2x |
| `hue(i)`、`sat(i)`、`val(i)`、`hsv(i)`、`color(i)` | 取樣點 i 的色相碼、飽和、明度、顏色名稱 | 21～23 |
| `order()`、`total()` | 左起每塊的色相碼（≤13）、總塊數 | 30 |
| `line_all()` | 14 個值一次讀 | 41～43 |
| `near()` `mid()` `far()` `angle()` `width()` `flags()` `junction()` `junction_dist()` `edges()` `far_width()` `marker()` `lost()` | 循線各值 | 41～43 |
| `max_object()` | 這個模式給幾個物體：51→4、52→2 | 51、52 |
| `object_count()` | 畫面裡有幾個物體 0～8 | 51 |
| `object(i)`、`object_found(i)` | 物體 i 的代碼（色號×10＋形狀，0＝沒有）、有沒有 | 51、52 |
| `object_color(i)`、`object_shape(i)` | 色號 1～8、形狀 0～9 | 51、52 |
| `object_xy(i)` | 質心 (X, Y) 像素，(0, 0)＝沒有 | 51、52 |
| `object_size(i)`、`object_area(i)` | 外框 (寬, 高) 像素、面積 % | 52 |
| `device_id()`、`is_unicam()`、`firmware_version()` | 模式 8：(0x4C, 3, 100)、True、(1, 0) | 全部 |
| `sensor_ok()`、`mode_number()` | 最近有沒有讀到、建構時的模式編號 | 全部 |
| `color_name(h)`（模組函式） | 色相碼 → `'red'`…`'black'` | — |
| `shape_name(s, zh=False)`（模組函式） | 形狀 0～9 → `'other'` `'circle'` `'ellipse'` `'triangle'` `'square'` `'rectangle'` `'pentagon'` `'hexagon'` `'star'` `'trapezoid'`；`zh=True` 回中文 | — |

- 模式不對（例如模式 21 叫 `xy()`）或色號超出範圍會丟 `ValueError`，訊息中英並列，不會默默回錯的值。
- 拔線不會讓程式停：回傳上一次的值；從來沒讀到過回傳 `None`。
- 數量（模式 11、12）在模式 6、裝置 ID 在模式 8，換模式要多等約 30 ms，不要每圈都讀。

---

## 範例 1：跟著色塊轉

**這個範例會做什麼**：車子轉向色 1：色塊在左就左轉、在右就右轉，邊轉邊往前開；面積夠大（很近）就只轉不前進；看不到就原地慢慢轉著找。

**相機要切到**：相機模式 1 顏色追蹤 ・子選項 ③ 標準（XY＋面積）＝**模式編號 13**；先教「目標 1」。

**要準備什麼**：SPIKE 主機、驅動底座（左輪 **E**、右輪 **F**）、相機插 **C 孔**、一顆目標色的球或積木。

**下載**：[ex01_track_turn.py](/examples/unicam/pybricks/ex01_track_turn.py)＋[MBC_UniCam.py](/examples/unicam/pybricks/MBC_UniCam.py)

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 × Pybricks（SPIKE）範例 1：跟著色塊轉
# Sharpshooter Vision × Pybricks (SPIKE) example 1: turn to follow a colour blob
# 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ③ 標準（XY＋面積）＝模式編號 13
# Camera menu: camera mode 1 Colour tracking, option 3 Standard (XY + area) = mode number 13
# 準備：先在相機上教「目標 1」的顏色 / Teach "target 1" on the camera first
# 接線 Wiring：相機 → C 孔 / camera → port C；左輪 E、右輪 F / left wheel E, right wheel F
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor
from pybricks.parameters import Port, Direction, Color
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=13)                    # 模式編號要跟相機選單一樣 / must match the camera menu
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, 56, 112)          # 輪徑 56 mm、輪距 112 mm / wheel 56 mm, axle 112 mm

KP = 3            # 👉 可以改：偏 1 像素就多轉幾度／秒 / you can change: turn rate per pixel of error
STOP_AREA = 20    # 👉 可以改：面積 ≥ 20 % 就算很近 / area (%) that counts as "close"

while True:
    p = cam.xy(1)                                # 色 1 的 (X, Y) 像素；(0, 0) = 沒看到 / (0, 0) = not seen
    if p is None:                                # 從來沒讀到相機 / camera never answered
        robot.stop()
        hub.light.on(Color.RED)
    elif p[0] == 0:                              # 沒看到色 1：原地慢慢轉著找 / not seen: spin slowly to search
        robot.drive(0, 60)
        hub.light.on(Color.ORANGE)
    else:
        error = p[0] - 80                        # 畫面中間 X = 80；色塊在右 → 正 / centre is X = 80; right = positive
        if cam.area(1) >= STOP_AREA:             # 很近了：只轉不前進 / close: turn only
            robot.drive(0, error * KP)
        else:                                    # 還遠：邊前進邊轉 / far: drive and turn
            robot.drive(80, error * KP)
        hub.light.on(Color.GREEN)
    wait(20)
```

</details>

**一步一步看懂程式**
1. `UniCam(Port.C, mode=13)`：告訴函式庫相機切在模式 13（SPIKE 讀不到相機的模式，要自己填）。
2. `cam.xy(1)`：色 1 的 (X, Y) 像素。X＝0 表示沒看到。
3. `error = X − 80`：畫面中間是 80，色塊在右邊就是正的 → `robot.drive(速度, error × KP)` 轉向是正＝往右轉。
4. `cam.area(1)`：面積 %，跟 X、Y 在同一包資料（模式 9），不用多等。

**跑起來應該看到什麼**：球在左邊車頭往左轉、在右邊往右轉；球拿近（面積 ≥ 20 %）車子原地對準；球拿走變橘燈、原地轉圈找。

**常見問題**
- **轉的方向反了？** → 左右輪接反了，把 E、F 對調，或把 `Direction.COUNTERCLOCKWISE` 改到右輪。
- **一直轉圈找不到？** → 相機螢幕右上要顯示 `1-3`；目標 1 要教過；`mode=` 要跟選單一樣。
- **左右晃個不停？** → `KP` 改小（例如 2）。

---

## 範例 2：多色找位置

**這個範例會做什麼**：每 0.5 秒在電腦畫面印出色 1～7 在畫面的哪裡（左／中／右、上／中／下），主機螢幕顯示看到幾色。按主機**左鍵**多印「16 色裡找到幾色」和「色 1 有幾塊」。

**相機要切到**：相機模式 1 顏色追蹤 ・子選項 ② 多色＝**模式編號 12**；教目標 1～7（教幾色都可以）。

**要準備什麼**：SPIKE 主機、相機插 **C 孔**、幾種顏色的積木。

**下載**：[ex02_multi_color.py](/examples/unicam/pybricks/ex02_multi_color.py)＋[MBC_UniCam.py](/examples/unicam/pybricks/MBC_UniCam.py)

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 × Pybricks（SPIKE）範例 2：多色找位置
# Sharpshooter Vision × Pybricks (SPIKE) example 2: where are my colours?
# 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ② 多色 ＝模式編號 12（SPIKE 給 7 色的 X、Y）
# Camera menu: camera mode 1 Colour tracking, option 2 Multi = mode number 12 (7 colours, X and Y)
# 準備：在相機上教目標 1～7 的顏色（教幾色都可以）/ teach targets 1~7 (any number)
# 接線 Wiring：相機 → C 孔 / camera → port C
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port, Button
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=12)


def where(x, y):
    # 像素 → 文字：左／中／右 + 上／中／下 / pixels -> words
    h = '左 left' if x < 54 else ('右 right' if x > 106 else '中 middle')
    v = '上 top' if y < 40 else ('下 bottom' if y > 80 else '中 middle')
    return h + ', ' + v


while True:
    seen = 0
    for c in range(1, cam.max_color() + 1):      # 色 1～7 / colours 1~7
        p = cam.xy(c)
        if p and p[0]:                           # X = 0 表示沒找到 / X = 0 means not found
            seen += 1
            print('色 colour', c, ':', p[0], p[1], where(p[0], p[1]))
    hub.display.number(seen)                     # 主機螢幕顯示看到幾色 / show how many colours are seen
    if Button.LEFT in hub.buttons.pressed():
        # 數量放在模式 6，要多等約 30 ms，所以按鍵才讀 / counts are in mode 6 (30 ms slower): read on demand
        print('16 色裡找到', cam.colors_found(), '色 / colours found;  色 1 有', cam.count(1), '塊 / blobs of colour 1')
    print('---')
    wait(500)
```

</details>

**一步一步看懂程式**
1. `cam.max_color()`：模式 12 在 SPIKE 給 **7 色**，迴圈跑色 1～7。
2. `cam.xy(c)` 回傳 (X, Y)；X＝0 就是沒找到，不印。
3. `where()` 把像素換成文字：X < 54 是左、> 106 是右；Y < 40 是上、> 80 是下。
4. `cam.colors_found()`、`cam.count(1)` 放在**模式 6**，換模式多等約 30 ms，所以按鍵才讀。

**跑起來應該看到什麼**：電腦畫面一直印 `色 colour 1 : 35 20 左 left, 上 top` 這樣的行，每批之間一行 `---`；主機螢幕數字＝看到幾色。

**常見問題**
- **有的色一直沒出現？** → 那一色沒教，或教的時候光線跟現在差很多，重教一次。
- **想要面積？** → 改用模式 13（4 色＋面積）或 14（2 色＋寬高面積）。

---

## 範例 3：座標取色判斷顏色

**這個範例會做什麼**：讀取樣點 S1 的色相碼和飽和度，判斷是什麼顏色，主機燈號亮那個顏色，電腦畫面印出中文顏色名稱。

**相機要切到**：相機模式 2 座標取色 ・子選項 ② 色相＋飽和＝**模式編號 22**（SPIKE 給 7 點）。S1 的位置是相機螢幕上標 1 的十字。

**要準備什麼**：SPIKE 主機、相機插 **C 孔**、紅黃綠藍白黑的積木。

**下載**：[ex03_spot_color.py](/examples/unicam/pybricks/ex03_spot_color.py)＋[MBC_UniCam.py](/examples/unicam/pybricks/MBC_UniCam.py)

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 × Pybricks（SPIKE）範例 3：座標取色判斷顏色
# Sharpshooter Vision × Pybricks (SPIKE) example 3: what colour is at the spot?
# 需要把相機選單切到 相機模式 2 座標取色 ・子選項 ② 色相＋飽和 ＝模式編號 22（SPIKE 給 7 點）
# Camera menu: camera mode 2 Spot colour, option 2 Hue + saturation = mode number 22 (7 spots)
# 準備：取樣點 S1 的位置看相機螢幕上標 1 的十字 / spot S1 is the cross marked 1 on the camera screen
# 接線 Wiring：相機 → C 孔 / camera → port C
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port, Color
from pybricks.tools import wait
from MBC_UniCam import UniCam, color_name

hub = PrimeHub()
cam = UniCam(Port.C, mode=22)

# 顏色名稱 → 主機燈號 / colour name -> hub light
LIGHT = {'red': Color.RED, 'orange': Color.ORANGE, 'yellow': Color.YELLOW, 'green': Color.GREEN,
         'cyan': Color.CYAN, 'blue': Color.BLUE, 'purple': Color.VIOLET, 'white': Color.WHITE, 'black': Color.NONE}
NAME = {'red': '紅', 'orange': '橙', 'yellow': '黃', 'green': '綠', 'cyan': '青',
        'blue': '藍', 'purple': '紫', 'white': '白／灰', 'black': '黑'}
LOW_SAT = 15      # 👉 可以改：飽和度比這低就當成白／灰 / below this saturation = white / grey

while True:
    p = cam.hsv(1)                               # 取樣點 S1 的 (色相碼, 飽和度, None) / (hue, sat, None)
    if p is None:                                # 讀不到相機 / no camera
        hub.light.on(Color.NONE)
        wait(200)
        continue
    h, s = p[0], p[1]
    name = color_name(h)
    if s < LOW_SAT and name != 'black':
        name = 'white'                           # 顏色很淡：當成白／灰 / very pale: white / grey
    hub.light.on(LIGHT[name])
    print('S1 色相碼 hue', h, ' 飽和度 sat', s, ' →', NAME[name], name)
    wait(200)
```

</details>

**一步一步看懂程式**
1. `cam.hsv(1)`：模式 22 回傳 (色相碼, 飽和度, None)，明度要模式 23 才有。
2. `color_name(h)`：色相碼換成 `'red'`、`'green'`…（紅 0～11 或 220～240、橙 12～27、黃 28～55、綠 56～99、青 100～139、藍 140～179、紫 180～219、241～248 白／灰、249～255 黑）。
3. 飽和度 < 15 的「彩色」其實很淡，當成白／灰。

**跑起來應該看到什麼**：紅色積木 → 紅燈、印 `S1 色相碼 hue 3  飽和度 sat 72  → 紅 red`；白紙 → 白燈；黑色 → 燈關。

**常見問題**
- **顏色判斷錯？** → 先看印出的色相碼，把 `color_name()` 的分界照你的積木改（可以複製函式到自己的程式改）；或在相機的 SET 選單做白色校正。
- **想同時看很多點？** → 模式 21 給 14 點色相：`for i in range(1, 15): print(cam.hue(i))`。

---

## 範例 4：照順序給色號（WRO 顏色順序）

**這個範例會做什麼**：WRO 任務常要「記下場地上積木的顏色順序」。相機把畫面裡的色塊由左到右排好，程式連續 5 次讀到一樣的 4 塊才算數，嗶一聲，主機螢幕一個字母一個字母顯示（R 紅、G 綠、B 藍、Y 黃…）。

**相機要切到**：相機模式 3 照順序給色號（沒有子選項）＝**模式編號 30**；先把場地上會出現的每種顏色都教成目標。

**要準備什麼**：SPIKE 主機、相機插 **C 孔**、4 塊不同顏色的積木排成一排。

**下載**：[ex04_color_order.py](/examples/unicam/pybricks/ex04_color_order.py)＋[MBC_UniCam.py](/examples/unicam/pybricks/MBC_UniCam.py)

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 × Pybricks（SPIKE）範例 4：照順序給色號（WRO 任務：記下積木的顏色順序）
# Sharpshooter Vision × Pybricks (SPIKE) example 4: colour order (WRO: remember the order)
# 需要把相機選單切到 相機模式 3 照順序給色號（沒有子選項）＝模式編號 30
# Camera menu: camera mode 3 Colour order (no option) = mode number 30
# 準備：在相機上教會場地上會出現的每一種顏色 / teach every colour that can appear on the field
# 接線 Wiring：相機 → C 孔 / camera → port C
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_UniCam import UniCam, color_name

hub = PrimeHub()
cam = UniCam(Port.C, mode=30)

WANT = 4          # 👉 可以改：場地上有幾塊 / how many blocks to expect
LETTER = {'red': 'R', 'orange': 'O', 'yellow': 'Y', 'green': 'G', 'cyan': 'C',
          'blue': 'B', 'purple': 'P', 'white': 'W', 'black': 'K'}

# 連續 5 次讀到一樣的順序才算數（避免剛好有手經過）/ same answer 5 times in a row = stable
last = None
same = 0
names = []
while same < 5:
    hues = cam.order()                           # 由左到右每塊的色相碼 / hue codes from left to right
    names = [color_name(h) for h in hues] if hues else []
    if len(names) == WANT and names == last:
        same += 1
    else:
        same = 0
    last = names
    wait(50)

print('順序 order:', names)
hub.speaker.beep()
for n in names:                                  # 一個字母一個字母顯示 / show one letter at a time
    hub.display.char(LETTER[n])
    wait(700)
hub.display.off()
```

</details>

**一步一步看懂程式**
1. `cam.order()`：左起每塊的色相碼（SPIKE 最多 13 塊）；色相碼是**教色時那個顏色**的，同一種顏色每次都一樣。
2. `cam.total()`：畫面裡總共幾塊（可能比 13 多）。
3. `WANT = 4`：場地上有幾塊就填幾；塊數不對、或順序還在變，就重新數。

**跑起來應該看到什麼**：積木擺好後約半秒嗶一聲，螢幕依序出現 `R`、`G`、`B`、`Y`，電腦畫面印 `順序 order: ['red', 'green', 'blue', 'yellow']`。

**常見問題**
- **一直不嗶？** → 塊數跟 `WANT` 不一樣：看相機螢幕上每塊標的順序數字，背景有同色的東西要移開或把目標的 Size 調大。
- **兩塊顏色相近被認成同一色？** → 教色時把 Hue 範圍調小。

---

## 範例 5：PD 循線＋路口停車

**這個範例會做什麼**：用「近處偏移」為主、「遠處偏移」提早轉彎的 PD 循線；看到十字或 T 字路口就停車嗶一聲；線跑出畫面就往最後離線的那一邊原地轉回來。

**相機要切到**：相機模式 4 循線 ・子選項 ① 黑線＝**模式編號 41**（白線 42、色線 43 只要改 `mode=`；色線＝目標色 1）。

**要準備什麼**：SPIKE 主機、驅動底座（左輪 **E**、右輪 **F**）、相機插 **C 孔**裝在車頭斜斜往前下方看、白底黑線場地（含一個十字路口）。

**下載**：[ex05_line_follow.py](/examples/unicam/pybricks/ex05_line_follow.py)＋[MBC_UniCam.py](/examples/unicam/pybricks/MBC_UniCam.py)

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 × Pybricks（SPIKE）範例 5：PD 循線＋路口停車
# Sharpshooter Vision × Pybricks (SPIKE) example 5: PD line following + stop at a junction
# 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線 ＝模式編號 41（白線 42、色線 43 寫法一樣，改 mode）
# Camera menu: camera mode 4 Line, option 1 Black line = mode number 41 (42 white / 43 colour line: change mode)
# 準備：相機裝在車頭、斜斜往前下方看線 / mount the camera at the front, looking down and forward
# 接線 Wiring：相機 → C 孔；左輪 E、右輪 F / camera → port C; left wheel E, right wheel F
# 方向：站在車後往前看，線在左＝負 / standing behind the robot: line on the left = negative
from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor
from pybricks.parameters import Port, Direction
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam import UniCam

hub = PrimeHub()
cam = UniCam(Port.C, mode=41)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, 56, 112)

SPEED = 120       # 👉 可以改：前進速度 mm/s / forward speed
KP = 1.5          # 👉 可以改：偏多少就轉多少 / proportional gain
KD = 4.0          # 👉 可以改：偏移變快時多修一點 / derivative gain
last_error = 0

while True:
    v = cam.line_all()                           # 14 個值一次讀 / all 14 line values at once
    if v is None:                                # 讀不到相機：停車 / no camera: stop
        robot.stop()
        wait(50)
        continue
    near, far, flags, junction, lost = v[0], v[2], v[5], v[6], v[13]
    if junction in (1, 2):                       # 1 十字、2 T 字路口：停車嗶一聲 / cross or T junction: stop
        robot.stop()
        hub.speaker.beep()
        break
    if flags == 0:                               # 近中遠都沒看到線：往最後離線那一邊找 / line lost: search
        robot.drive(0, -120 if lost == 1 else 120)
        wait(10)
        continue
    error = 0.7 * near + 0.3 * far               # 近處為主、遠處提早轉彎 / mostly near, a bit of far
    turn = KP * error + KD * (error - last_error)
    last_error = error
    robot.drive(SPEED, turn)                     # 線在左（負）→ 往左轉 / line on the left → turn left
    wait(10)
```

</details>

**一步一步看懂程式**
1. `cam.line_all()`：一次拿 14 個值：`[0]` 近處偏移、`[2]` 遠處偏移、`[5]` 有線旗標（近 1＋中 2＋遠 4）、`[6]` 路口類型（0 無 1 十字 2 T 3 左 4 右 5 盡頭）、`[13]` 最後離線方向（1 左 2 右）。
2. `error = 0.7 × 近 + 0.3 × 遠`：線在左是負的 → 轉向是負＝往左轉。
3. `KD × (error − 上一次 error)`：偏移變化很快（快到彎）時多修一點，車子比較不會晃。

**跑起來應該看到什麼**：車子沿著黑線走，彎道前會提早轉；到十字路口停下來嗶一聲，程式結束。

**常見問題**
- **左右晃得厲害？** → `KP` 改小、`KD` 改大；或 `SPEED` 改慢。
- **彎道衝出去？** → 遠處的比重改大（例如 0.5／0.5）。
- **還沒到路口就停？** → 路口判斷的門檻在相機裡，v1 還沒上機調過；先把 `if junction in (1, 2)` 改成也要 `cam.junction_dist() < 40`（路口夠近才停）。

---

## 範例 6：找最大的紅色三角形

**這個範例會做什麼**：相機把畫面裡的物體由大到小排好，程式從最大的往下找「目標 1（紅色）的三角形」，轉過去開到它前面（它跑到畫面最下面）就停車嗶一聲；看不到就原地慢慢轉著找。主機螢幕顯示畫面裡有幾個物體。

**相機要切到**：相機模式 5 物體偵測 ・子選項 ① 物體清單＝**模式編號 51**；先把「目標 1」教成紅色（形狀可以不指定，相機會自己判斷）。

**要準備什麼**：SPIKE 主機、驅動底座（左輪 **E**、右輪 **F**）、相機插 **C 孔**、紅色三角形的紙板或積木（再放幾個別的形狀更好玩）。

**下載**：[ex06_object_pick.py](/examples/unicam/pybricks/ex06_object_pick.py)＋[MBC_UniCam.py](/examples/unicam/pybricks/MBC_UniCam.py)

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 × Pybricks（SPIKE）範例 6：找最大的紅色三角形、轉過去
# Sharpshooter Vision × Pybricks (SPIKE) example 6: find the biggest red triangle and drive to it
# 需要把相機選單切到 相機模式 5 物體偵測 ・子選項 ① 物體清單＝模式編號 51
# Camera menu: camera mode 5 Object detection, option 1 Object list = mode number 51
# 準備：在相機上把「目標 1」教成紅色（形狀可以不指定）/ teach "target 1" as red on the camera
# 接線 Wiring：相機 → C 孔 / camera → port C；左輪 E、右輪 F / left wheel E, right wheel F
# 同一個資料夾要有 MBC_UniCam.py / MBC_UniCam.py must be imported too
#
# 物體代碼＝色號×10＋形狀：目標 1 的三角形＝1×10＋3＝13。物體 1、2、3、4 是面積由大到小，
# 所以從物體 1 往下找，第一個代碼是 13 的就是「最大的紅色三角形」。
# Object code = colour × 10 + shape: target 1 triangle = 13. Objects 1~4 are sorted largest first,
# so the first object with code 13 is the biggest red triangle.
from pybricks.hubs import PrimeHub
from pybricks.pupdevices import Motor
from pybricks.parameters import Port, Direction, Color
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_UniCam import UniCam, shape_name

hub = PrimeHub()
cam = UniCam(Port.C, mode=51)                    # 模式編號要跟相機選單一樣 / must match the camera menu
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, 56, 112)          # 輪徑 56 mm、輪距 112 mm / wheel 56 mm, axle 112 mm

WANT = 1 * 10 + 3  # 👉 可以改：目標 1 × 10 ＋ 形狀 3（三角）；正方形改成 14、圓改成 11 / square 14, circle 11
KP = 3             # 👉 可以改：偏 1 像素就多轉幾度／秒 / turn rate per pixel of error
NEAR_Y = 100       # 👉 可以改：物體的 Y ≥ 100（畫面最下面）就算到了 / Y at the bottom of the picture = arrived


def find(want):
    # 從最大的物體往下找代碼＝want 的，回傳它的 (X, Y)；找不到 (0, 0) / largest first; (0, 0) = not found
    for i in range(1, cam.max_object() + 1):     # SPIKE 模式 51 給 4 個物體 / 4 objects on SPIKE
        if cam.object(i) == want:
            return cam.object_xy(i)
    return (0, 0)


print('要找 looking for: 目標 target', WANT // 10, shape_name(WANT % 10, True), shape_name(WANT % 10))
while True:
    n = cam.object_count()                       # 畫面裡有幾個物體 0~8 / objects in view
    if n is None:                                # 從來沒讀到相機 / camera never answered
        robot.stop()
        hub.light.on(Color.RED)
        wait(100)
        continue
    hub.display.number(n)
    x, y = find(WANT)
    if x == 0:                                   # 沒看到紅色三角形：原地慢慢轉著找 / not seen: spin to search
        robot.drive(0, 60)
        hub.light.on(Color.ORANGE)
    elif y >= NEAR_Y:                            # 到了：停車、嗶一聲 / arrived: stop and beep
        robot.stop()
        hub.light.on(Color.GREEN)
        hub.speaker.beep()
        break
    else:
        error = x - 80                           # 畫面中間 X = 80；在右 → 正 → 右轉 / right of centre = positive
        robot.drive(80, error * KP)
        hub.light.on(Color.BLUE)
    wait(20)
```

</details>

**一步一步看懂程式**
1. `WANT = 1 × 10 + 3`：物體代碼＝色號×10＋形狀，目標 1 的三角形＝13。要找正方形改 14、圓改 11。
2. `cam.object(i)`：物體 i 的代碼；物體 1 最大，所以從 1 往下找到的第一個就是最大的。SPIKE 模式 51 給 4 個物體（`cam.max_object()`）。
3. `cam.object_xy(i)`：質心 (X, Y) 像素；`X − 80` 在右是正的 → 往右轉。
4. `cam.object_count()`：畫面裡總共幾個物體（0～8，可能比 4 多）。

**跑起來應該看到什麼**：紅色三角形在左邊車子往左轉、在右邊往右轉，邊轉邊前進；開到它前面停車嗶一聲，程式結束。旁邊的紅色圓形、藍色三角形都不理。

**常見問題**
- **一直轉圈找不到？** → 相機螢幕右上要顯示 `5-1`；看相機畫面每個物體旁邊標的「色號 形狀」，三角形被判成別的形狀就到目標 1 的 `Target 1 shape` 指定成 Tri，或在 SET 選單把 **ShapeTol** 調大。
- **畫面裡東西太多？** → SPIKE 只給最大的 4 個；紅色三角形比較小時可能排不進來，把背景雜物移開。
- **想要大小、面積？** → 改用模式 52：`cam.object_size(i)`、`cam.object_area(i)`（只給 2 個物體）。

---

## 樂高官方 SPIKE App（教育版）Python 範例

官方 App 把相機當成「顏色感應器」，Python 用 `color_sensor.color()`、`reflection()`、`rgbi()` 讀到 **byte 0～7**（顏色、反射光、原始紅綠藍）。範例最上面的「讀值小工具」`UniCamApp` 會照模式把它拆好（小工具單獨下載：[MBC_UniCam_App_helper.py](/examples/unicam/spike_app/MBC_UniCam_App_helper.py)）。byte 8～15 只有 `device.data()` 可能讀得到，**還沒上機確認（任務 #77）**，小工具先不用。

| 小工具方法 | 回傳 | 模式 |
|---|---|---|
| `update()` | 讀一次（每圈先叫） | 全部 |
| `cell(c)` | 色 c 的格子 (X, Y)：13／14 是 16×12 格（色 1～4／1～2），11／12 是 8×6 格（色 1～7）；(0, 0)＝沒找到 | 11～14 |
| `size_cell(c)`、`colors_found()` | 寬高格（14）、找到幾色（11、12） | 14／11、12 |
| `hue(i)`、`sat(i)`、`val(i)` | 21：點 1～6 色相；22、23：點 1～3 色相＋飽和；23 點 1 明度 | 21～23 |
| `order()` | (左起前 6 塊色相碼, 總塊數) | 30 |
| `line()` | (近處偏移, 遠處偏移, 角度, 路口類型, 近處線寬) | 41～43 |
| `max_object()`、`object_count()` | 給幾個物體的代碼（51→3、52→2）、畫面裡幾個物體（51） | 51、52 |
| `object(i)`、`object_color(i)`、`object_shape(i)` | 物體代碼、色號、形狀（51：物體 1～3；52：物體 1～2） | 51、52 |
| `object_cell(i)` | 物體 1～3 的 8×6 格 (X, Y)（Y 上＝小） | 51 |
| `object_xy(1)`、`object_size(1)`、`object_area(i)` | 物體 1 的質心、外框像素；物體 1、2 的面積 % | 52 |

### App 範例 1：跟著色塊轉

**相機要切到**：**模式編號 13**（顏色追蹤 ③）。接線：相機 **C**、左輪 **E**、右輪 **F**。色 1 在 16×12 格裡，中間是 8.5 格。

**下載**：[ex01_track_turn.py](/examples/unicam/spike_app/ex01_track_turn.py)

<details>
<summary>🟨 官方 App Python 程式碼</summary>

```python
# 神攝手 × 樂高官方 SPIKE App（教育版 Python）範例 1：跟著色塊轉
# Sharpshooter Vision × LEGO SPIKE App (Education, Python) example 1: turn to follow a colour blob
# 需要把相機選單切到 相機模式 1 顏色追蹤 ・子選項 ③ 標準（XY＋面積）＝模式編號 13
# Camera menu: camera mode 1 Colour tracking, option 3 Standard (XY + area) = mode number 13
# 準備：先在相機上教「目標 1」的顏色 / teach "target 1" on the camera first
# 接線 Wiring：相機 → C 孔；左輪 E、右輪 F（驅動底座）/ camera → port C; left wheel E, right wheel F
# 用法：SPIKE App 新增「Python」專案，整支貼上 / new Python project in the SPIKE App, paste all of this
# ===== 神攝手 讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== UniCam reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
# 相機在 SPIKE App 裡是「顏色感應器」：顏色、反射光、原始紅／綠／藍 這 5 個數字（byte 0~7）積木與 Python 都讀得到。
# byte 8~15 只有 App Python 的 device.data() 可能讀得到，還沒上機確認（任務 #77），這個小工具不用它。
# The camera shows up as a colour sensor: colour, reflection, raw red/green/blue (bytes 0~7).
# Bytes 8~15 may only be readable with device.data() (not yet verified, #77); this helper does not use them.
# mode = 你在相機選單切的模式編號（相機模式×10＋子選項，例 13）/ mode = camera menu mode number
import color_sensor


class UniCamApp:
    def __init__(self, port_number, mode):
        self.port = port_number
        self.mode = mode
        self.b = [0] * 8                      # byte 0~7

    def update(self):
        """讀一次最新資料 / read once"""
        c = color_sensor.color(self.port)              # byte 0
        r = color_sensor.reflection(self.port)         # byte 1
        rgb = color_sensor.rgbi(self.port)             # 原始紅、綠、藍（各 2 byte）
        b = [int(c) & 255, int(r) & 255]
        for v in rgb[:3]:
            v = int(v) & 0xFFFF                        # 負數也換回 0~65535
            b += [v & 255, v >> 8]
        self.b = b
        return True

    def _w(self, i):
        # 原始紅（i=0）、綠（1）、藍（2）的 0~65535 / raw red, green, blue word
        return self.b[2 + 2 * i] | (self.b[3 + 2 * i] << 8)

    def cell(self, color):
        """顏色追蹤：色 color 的格子 (X 格, Y 格)，(0, 0)＝沒找到（Y 上＝小）
        模式 13、14：16×12 格（像素 ≈ 格×10−5）；模式 11、12：8×6 格（像素 ≈ 格×20−10），色 1~7"""
        m = self.mode
        if m in (13, 14):
            if color == 1:
                return (self.b[0], self.b[1])
            return (self.b[2 * color - 2], self.b[2 * color - 1])      # 13：色 2~4；14：色 2
        if color == 1:
            g = self.b[1]
        else:
            w = self._w((color - 2) // 2)
            g = w // 100 if color % 2 == 0 else w % 100
        return (g // 10, g % 10)

    def size_cell(self, color):
        """模式 14：色 1、2 的 (寬格, 高格)（16×12 格：寬 ≈ 格×10）"""
        w = self._w(color)                    # 色 1 在原始綠、色 2 在原始藍
        return (w & 255, w >> 8)

    def colors_found(self):
        """模式 11、12：找到幾色（顏色）"""
        return self.b[0]

    def hue(self, point):
        """座標取色：取樣點的色相碼 0~255（21：點 1~6；22、23：點 1~3）"""
        if self.mode == 21:
            return self.b[1 + point]
        return self.b[2 * point]

    def sat(self, point):
        """飽和度（22、23：點 1~3；22 的點 4 在反射光）"""
        if self.mode == 22 and point == 4:
            return self.b[1]
        return self.b[2 * point + 1]

    def val(self, point):
        """明度（23：只有點 1，在反射光；其他點在 byte 8~15）"""
        return self.b[1] if point == 1 else None

    def order(self):
        """照順序給色號：(左起前 6 塊的色相碼, 總塊數)"""
        n = min(6, self.b[0])
        return (self.b[2:2 + n], self.b[0])

    def line(self):
        """循線：(近處偏移, 遠處偏移, 角度, 路口類型, 近處線寬)；偏移、角度 −100~100（線在左＝負）"""
        return (self._w(0) - 100, self._w(1) - 100, self._w(2) - 100, self.b[0], self.b[1])

    # ── 物體偵測（51 物體清單、52 物體詳細）：物體代碼＝色號×10＋形狀，0＝沒有；物體 1＝面積最大 ──
    # ── object detection (51 list, 52 detail): code = colour×10 + shape, 0 = none; object 1 = largest ──
    def _obj(self, i, n, modes=(51, 52)):
        if self.mode not in modes:
            raise ValueError('模式 %d 沒有這個值，要用模式 %s / not available in mode %d, use %s' % (self.mode, modes, self.mode, modes))
        if not 1 <= i <= n:
            raise ValueError('第幾個物體要填 1~%d / object must be 1~%d' % (n, n))

    def max_object(self):
        """byte 0~7 給幾個物體的代碼：51→3、52→2（52 的位置、大小只有物體 1）"""
        self._obj(1, 1)
        return 3 if self.mode == 51 else 2

    def object_count(self):
        """51：畫面裡有幾個物體 0~8（顏色）/ objects in view (colour)"""
        self._obj(1, 1, (51,))
        return self.b[0]

    def object(self, i):
        """物體 i 的代碼（51：物體 1~3；52：物體 1~2）/ object code"""
        if self.mode == 51:
            self._obj(i, 3)
            return self.b[1] if i == 1 else self._w(i - 1) // 100
        self._obj(i, 2)
        return self.b[0] if i == 1 else self._w(2) & 255

    def object_color(self, i):
        """物體 i 的色號 1~8，0＝沒有"""
        return self.object(i) // 10

    def object_shape(self, i):
        """物體 i 的形狀 0~9（沒有物體也是 0）"""
        return self.object(i) % 10

    def object_cell(self, i):
        """51：物體 i（1~3）的 8×6 格 (X 格, Y 格)，(0, 0)＝沒有（Y 上＝小；像素 ≈ 格×20−10）"""
        self._obj(i, 3, (51,))
        g = self._w(0) if i == 1 else self._w(i - 1) % 100
        return (g // 10, g % 10)

    def object_xy(self, i):
        """52：物體 1 的質心 (X, Y) 像素，(0, 0)＝沒有"""
        self._obj(i, 1, (52,))
        return (self._w(0) & 255, self._w(0) >> 8)

    def object_size(self, i):
        """52：物體 1 的外框 (寬, 高) 像素"""
        self._obj(i, 1, (52,))
        return (self._w(1) & 255, self._w(1) >> 8)

    def object_area(self, i):
        """52：物體 1、2 的面積（佔畫面 %）"""
        self._obj(i, 2, (52,))
        return self.b[1] if i == 1 else self._w(2) >> 8


def shape_name(s, zh=False):
    """形狀 0~9 → 'other' 'circle' 'ellipse' 'triangle' 'square' 'rectangle' 'pentagon' 'hexagon' 'star' 'trapezoid'（zh=True 中文）"""
    if s is None or not 0 <= s <= 9:
        return None
    return ('其他', '圓', '橢圓', '三角', '正方', '長方', '五邊', '六邊', '星', '梯形')[s] if zh else         ('other', 'circle', 'ellipse', 'triangle', 'square', 'rectangle', 'pentagon', 'hexagon', 'star', 'trapezoid')[s]


def color_name(h):
    """色相碼 → 'red' 'orange' 'yellow' 'green' 'cyan' 'blue' 'purple' 'white' 'black'"""
    if h > 248:
        return 'black'
    if h > 240:
        return 'white'
    if h < 12 or h >= 220:
        return 'red'
    if h < 28:
        return 'orange'
    if h < 56:
        return 'yellow'
    if h < 100:
        return 'green'
    if h < 140:
        return 'cyan'
    if h < 180:
        return 'blue'
    return 'purple'
# ===== 小工具到這裡結束 / end of helper =====
from hub import port, light_matrix
import motor_pair
import runloop

cam = UniCamApp(port.C, 13)                       # 模式編號要跟相機選單一樣 / must match the camera menu
BASE = 200        # 👉 可以改：前進速度（度／秒）/ forward speed (deg/s)
K = 40            # 👉 可以改：偏 1 格多轉多少 / turn per grid cell of error


async def main():
    motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)
    while True:
        cam.update()
        cx, cy = cam.cell(1)                      # 色 1 的格子：X 1~16、Y 1~12；(0, 0) = 沒看到 / grid cell
        if cx == 0:                               # 沒看到：原地轉著找 / not seen: spin to search
            motor_pair.move_tank(motor_pair.PAIR_1, 120, -120)
            light_matrix.write('?')
        else:
            error = cx - 8.5                      # 畫面中間 = 8.5 格；在右 → 正 / centre = 8.5; right = positive
            motor_pair.move_tank(motor_pair.PAIR_1, int(BASE + K * error), int(BASE - K * error))
            light_matrix.write(str(cx))
        await runloop.sleep_ms(30)

runloop.run(main())
```

</details>

**應該看到什麼**：色塊在左車子往左轉、在右往右轉，主機螢幕顯示色 1 的 X 格（1～16）；看不到顯示 `?` 並原地轉。

### App 範例 2：PD 循線＋路口停車

**相機要切到**：**模式編號 41**（循線 ① 黑線）。官方 App 讀得到近處、遠處偏移、角度、路口類型、近處線寬（偏移、角度都＋100 送，小工具已經減回來）。

**下載**：[ex02_line_follow.py](/examples/unicam/spike_app/ex02_line_follow.py)

<details>
<summary>🟨 官方 App Python 程式碼</summary>

```python
# 神攝手 × 樂高官方 SPIKE App（教育版 Python）範例 2：PD 循線＋路口停車
# Sharpshooter Vision × LEGO SPIKE App (Education, Python) example 2: PD line following + stop at a junction
# 需要把相機選單切到 相機模式 4 循線 ・子選項 ① 黑線 ＝模式編號 41（白線 42、色線 43 也一樣）
# Camera menu: camera mode 4 Line, option 1 Black line = mode number 41 (42 / 43 work the same)
# 準備：相機裝在車頭、斜斜往前下方看線 / camera at the front, looking down and forward
# 接線 Wiring：相機 → C 孔；左輪 E、右輪 F / camera → port C; left wheel E, right wheel F
# 方向：站在車後往前看，線在左＝負 / standing behind the robot: line on the left = negative
# ===== 神攝手 讀值小工具（樂高官方 SPIKE App 3 教育版 Python，照抄就好、不用改）=====
# ===== UniCam reading helper (LEGO SPIKE App 3, Education, Python: copy as is) =====
# 相機在 SPIKE App 裡是「顏色感應器」：顏色、反射光、原始紅／綠／藍 這 5 個數字（byte 0~7）積木與 Python 都讀得到。
# byte 8~15 只有 App Python 的 device.data() 可能讀得到，還沒上機確認（任務 #77），這個小工具不用它。
# The camera shows up as a colour sensor: colour, reflection, raw red/green/blue (bytes 0~7).
# Bytes 8~15 may only be readable with device.data() (not yet verified, #77); this helper does not use them.
# mode = 你在相機選單切的模式編號（相機模式×10＋子選項，例 13）/ mode = camera menu mode number
import color_sensor


class UniCamApp:
    def __init__(self, port_number, mode):
        self.port = port_number
        self.mode = mode
        self.b = [0] * 8                      # byte 0~7

    def update(self):
        """讀一次最新資料 / read once"""
        c = color_sensor.color(self.port)              # byte 0
        r = color_sensor.reflection(self.port)         # byte 1
        rgb = color_sensor.rgbi(self.port)             # 原始紅、綠、藍（各 2 byte）
        b = [int(c) & 255, int(r) & 255]
        for v in rgb[:3]:
            v = int(v) & 0xFFFF                        # 負數也換回 0~65535
            b += [v & 255, v >> 8]
        self.b = b
        return True

    def _w(self, i):
        # 原始紅（i=0）、綠（1）、藍（2）的 0~65535 / raw red, green, blue word
        return self.b[2 + 2 * i] | (self.b[3 + 2 * i] << 8)

    def cell(self, color):
        """顏色追蹤：色 color 的格子 (X 格, Y 格)，(0, 0)＝沒找到（Y 上＝小）
        模式 13、14：16×12 格（像素 ≈ 格×10−5）；模式 11、12：8×6 格（像素 ≈ 格×20−10），色 1~7"""
        m = self.mode
        if m in (13, 14):
            if color == 1:
                return (self.b[0], self.b[1])
            return (self.b[2 * color - 2], self.b[2 * color - 1])      # 13：色 2~4；14：色 2
        if color == 1:
            g = self.b[1]
        else:
            w = self._w((color - 2) // 2)
            g = w // 100 if color % 2 == 0 else w % 100
        return (g // 10, g % 10)

    def size_cell(self, color):
        """模式 14：色 1、2 的 (寬格, 高格)（16×12 格：寬 ≈ 格×10）"""
        w = self._w(color)                    # 色 1 在原始綠、色 2 在原始藍
        return (w & 255, w >> 8)

    def colors_found(self):
        """模式 11、12：找到幾色（顏色）"""
        return self.b[0]

    def hue(self, point):
        """座標取色：取樣點的色相碼 0~255（21：點 1~6；22、23：點 1~3）"""
        if self.mode == 21:
            return self.b[1 + point]
        return self.b[2 * point]

    def sat(self, point):
        """飽和度（22、23：點 1~3；22 的點 4 在反射光）"""
        if self.mode == 22 and point == 4:
            return self.b[1]
        return self.b[2 * point + 1]

    def val(self, point):
        """明度（23：只有點 1，在反射光；其他點在 byte 8~15）"""
        return self.b[1] if point == 1 else None

    def order(self):
        """照順序給色號：(左起前 6 塊的色相碼, 總塊數)"""
        n = min(6, self.b[0])
        return (self.b[2:2 + n], self.b[0])

    def line(self):
        """循線：(近處偏移, 遠處偏移, 角度, 路口類型, 近處線寬)；偏移、角度 −100~100（線在左＝負）"""
        return (self._w(0) - 100, self._w(1) - 100, self._w(2) - 100, self.b[0], self.b[1])

    # ── 物體偵測（51 物體清單、52 物體詳細）：物體代碼＝色號×10＋形狀，0＝沒有；物體 1＝面積最大 ──
    # ── object detection (51 list, 52 detail): code = colour×10 + shape, 0 = none; object 1 = largest ──
    def _obj(self, i, n, modes=(51, 52)):
        if self.mode not in modes:
            raise ValueError('模式 %d 沒有這個值，要用模式 %s / not available in mode %d, use %s' % (self.mode, modes, self.mode, modes))
        if not 1 <= i <= n:
            raise ValueError('第幾個物體要填 1~%d / object must be 1~%d' % (n, n))

    def max_object(self):
        """byte 0~7 給幾個物體的代碼：51→3、52→2（52 的位置、大小只有物體 1）"""
        self._obj(1, 1)
        return 3 if self.mode == 51 else 2

    def object_count(self):
        """51：畫面裡有幾個物體 0~8（顏色）/ objects in view (colour)"""
        self._obj(1, 1, (51,))
        return self.b[0]

    def object(self, i):
        """物體 i 的代碼（51：物體 1~3；52：物體 1~2）/ object code"""
        if self.mode == 51:
            self._obj(i, 3)
            return self.b[1] if i == 1 else self._w(i - 1) // 100
        self._obj(i, 2)
        return self.b[0] if i == 1 else self._w(2) & 255

    def object_color(self, i):
        """物體 i 的色號 1~8，0＝沒有"""
        return self.object(i) // 10

    def object_shape(self, i):
        """物體 i 的形狀 0~9（沒有物體也是 0）"""
        return self.object(i) % 10

    def object_cell(self, i):
        """51：物體 i（1~3）的 8×6 格 (X 格, Y 格)，(0, 0)＝沒有（Y 上＝小；像素 ≈ 格×20−10）"""
        self._obj(i, 3, (51,))
        g = self._w(0) if i == 1 else self._w(i - 1) % 100
        return (g // 10, g % 10)

    def object_xy(self, i):
        """52：物體 1 的質心 (X, Y) 像素，(0, 0)＝沒有"""
        self._obj(i, 1, (52,))
        return (self._w(0) & 255, self._w(0) >> 8)

    def object_size(self, i):
        """52：物體 1 的外框 (寬, 高) 像素"""
        self._obj(i, 1, (52,))
        return (self._w(1) & 255, self._w(1) >> 8)

    def object_area(self, i):
        """52：物體 1、2 的面積（佔畫面 %）"""
        self._obj(i, 2, (52,))
        return self.b[1] if i == 1 else self._w(2) >> 8


def shape_name(s, zh=False):
    """形狀 0~9 → 'other' 'circle' 'ellipse' 'triangle' 'square' 'rectangle' 'pentagon' 'hexagon' 'star' 'trapezoid'（zh=True 中文）"""
    if s is None or not 0 <= s <= 9:
        return None
    return ('其他', '圓', '橢圓', '三角', '正方', '長方', '五邊', '六邊', '星', '梯形')[s] if zh else         ('other', 'circle', 'ellipse', 'triangle', 'square', 'rectangle', 'pentagon', 'hexagon', 'star', 'trapezoid')[s]


def color_name(h):
    """色相碼 → 'red' 'orange' 'yellow' 'green' 'cyan' 'blue' 'purple' 'white' 'black'"""
    if h > 248:
        return 'black'
    if h > 240:
        return 'white'
    if h < 12 or h >= 220:
        return 'red'
    if h < 28:
        return 'orange'
    if h < 56:
        return 'yellow'
    if h < 100:
        return 'green'
    if h < 140:
        return 'cyan'
    if h < 180:
        return 'blue'
    return 'purple'
# ===== 小工具到這裡結束 / end of helper =====
from hub import port, light_matrix, sound
import motor_pair
import runloop

cam = UniCamApp(port.C, 41)
BASE = 250        # 👉 可以改：前進速度（度／秒）/ forward speed (deg/s)
KP = 3            # 👉 可以改：比例 / proportional gain
KD = 8            # 👉 可以改：微分 / derivative gain


async def main():
    motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)
    last_error = 0
    while True:
        cam.update()
        near, far, angle, junction, width = cam.line()   # 偏移 −100~100、路口類型、線寬 %
        if junction in (1, 2):                    # 1 十字、2 T 字路口：停車嗶一聲 / cross or T: stop
            motor_pair.stop(motor_pair.PAIR_1)
            await sound.beep(880, 300, 100)
            break
        if width == 0:                            # 近處看不到線：往上一次偏的那邊找 / line lost: search
            turn = -150 if last_error < 0 else 150
            motor_pair.move_tank(motor_pair.PAIR_1, turn, -turn)
        else:
            error = 0.7 * near + 0.3 * far        # 近處為主、遠處提早轉彎 / mostly near, a bit of far
            turn = KP * error + KD * (error - last_error)
            last_error = error
            motor_pair.move_tank(motor_pair.PAIR_1, int(BASE + turn), int(BASE - turn))
        light_matrix.write(str(junction))
        await runloop.sleep_ms(20)

runloop.run(main())
```

</details>

**應該看到什麼**：車子沿線走，主機螢幕顯示路口類型（0＝沒有）；到十字或 T 字路口停車嗶一聲。近處線寬＝0（看不到線）時往上一次偏的那邊原地轉。

<a id="app-table"></a>

## 官方 App 積木拆值表

### 教育版（SPIKE App 3，黃色主機）

顏色、反射光各 1 byte；原始紅／綠／藍各 2 byte：「A ＋ B ×256」＝ A 是 `原始紅 % 256`（取餘數）、B 是 `原始紅 ÷ 256`（取整數）；「A ×100 ＋ B」＝ `÷100` 拆。格子 Y 上＝小。座標取色 ①：每格兩點色相。

| 模式 | 顏色 | 反射光 | 原始紅 | 原始綠 | 原始藍 |
|---|---|---|---|---|---|
| 1 ① 最多色 | 找到幾色 | 色 1 X×10＋Y（8×6 格） | 色 2 X×10＋Y（8×6 格） ×100 ＋ 色 3 X×10＋Y（8×6 格） | 色 4 X×10＋Y（8×6 格） ×100 ＋ 色 5 X×10＋Y（8×6 格） | 色 6 X×10＋Y（8×6 格） ×100 ＋ 色 7 X×10＋Y（8×6 格） |
| 1 ② 多色 | 找到幾色 | 色 1 X×10＋Y（8×6 格） | 色 2 X×10＋Y（8×6 格） ×100 ＋ 色 3 X×10＋Y（8×6 格） | 色 4 X×10＋Y（8×6 格） ×100 ＋ 色 5 X×10＋Y（8×6 格） | 色 6 X×10＋Y（8×6 格） ×100 ＋ 色 7 X×10＋Y（8×6 格） |
| 1 ③ 標準 | 色 1 X（16 格） | 色 1 Y（12 格） | 色 2 X（16 格） ＋ 色 2 Y（12 格） ×256 | 色 3 X（16 格） ＋ 色 3 Y（12 格） ×256 | 色 4 X（16 格） ＋ 色 4 Y（12 格） ×256 |
| 1 ④ 精細 | 色 1 X（16 格） | 色 1 Y（12 格） | 色 2 X（16 格） ＋ 色 2 Y（12 格） ×256 | 色 1 寬（16 格） ＋ 色 1 高（12 格） ×256 | 色 2 寬（16 格） ＋ 色 2 高（12 格） ×256 |
| 2 ① 色相 | — | — | S1 色相 ＋ S2 色相 ×256 | S3 色相 ＋ S4 色相 ×256 | S5 色相 ＋ S6 色相 ×256 |
| 2 ② 色相＋飽和 | — | S4 飽和 | S1 色相 ＋ S1 飽和 ×256 | S2 色相 ＋ S2 飽和 ×256 | S3 色相 ＋ S3 飽和 ×256 |
| 2 ③ 色相＋飽和＋明度 | — | S1 明度 | S1 色相 ＋ S1 飽和 ×256 | S2 色相 ＋ S2 飽和 ×256 | S3 色相 ＋ S3 飽和 ×256 |
| 3 照順序給色號 | 總塊數 | — | 左起第 1 塊色相 ＋ 左起第 2 塊色相 ×256 | 左起第 3 塊色相 ＋ 左起第 4 塊色相 ×256 | 左起第 5 塊色相 ＋ 左起第 6 塊色相 ×256 |
| 4 循線（①②③ 相同） | 路口類型 | 近處線寬 % | 近處偏移＋100 | 遠處偏移＋100 | 線角度＋100 |
| 5 ① 物體清單 | 物體數 | 物體 1 代碼 | 物體 1 X×10＋Y（8×6 格） | 物體 2 代碼 ×100 ＋ 物體 2 X×10＋Y（8×6 格） | 物體 3 代碼 ×100 ＋ 物體 3 X×10＋Y（8×6 格） |
| 5 ② 物體詳細 | 物體 1 代碼 | 物體 1 面積 % | 物體 1 X ＋ 物體 1 Y ×256 | 物體 1 寬 ＋ 物體 1 高 ×256 | 物體 2 代碼 ＋ 物體 2 面積 % ×256 |

### 家用版（Robot Inventor App，藍綠色主機）

家用版顏色只到 0～10、反射光最多 126、原始紅綠藍 0～255，所以每格只放 1 個值（或「A ×10 ＋ B」用 `÷10` 拆）。格子 8×6，Y 上＝小。

| 模式 | 顏色 | 反射光 | 原始紅 | 原始綠 | 原始藍 |
|---|---|---|---|---|---|
| 1 ① 最多色 | 色 1 X（8 格） | 色 1 Y（6 格） | 色 2 X（8 格） ×10 ＋ 色 2 Y（6 格） | 色 3 X（8 格） ×10 ＋ 色 3 Y（6 格） | 色 4 X（8 格） ×10 ＋ 色 4 Y（6 格） |
| 1 ② 多色 | 色 1 X（8 格） | 色 1 Y（6 格） | 色 2 X（8 格） ×10 ＋ 色 2 Y（6 格） | 色 3 X（8 格） ×10 ＋ 色 3 Y（6 格） | 色 4 X（8 格） ×10 ＋ 色 4 Y（6 格） |
| 1 ③ 標準 | 色 1 X（8 格） | 色 1 Y（6 格） | 色 2 X（8 格） ×10 ＋ 色 2 Y（6 格） | 色 3 X（8 格） ×10 ＋ 色 3 Y（6 格） | 色 4 X（8 格） ×10 ＋ 色 4 Y（6 格） |
| 1 ④ 精細 | 色 1 X（8 格） | 色 1 Y（6 格） | 色 2 X（8 格） ×10 ＋ 色 2 Y（6 格） | 色 1 寬（8 格） ×10 ＋ 色 1 高（6 格） | 色 2 寬（8 格） ×10 ＋ 色 2 高（6 格） |
| 2 ① 色相 | — | S4 色相÷2 | S1 色相 | S2 色相 | S3 色相 |
| 2 ② 色相＋飽和 | — | S2 飽和 | S1 色相 | S1 飽和 | S2 色相 |
| 2 ③ 色相＋飽和＋明度 | — | S2 飽和÷10×10＋明度÷10 | S1 色相 | S1 飽和÷10×10＋明度÷10 | S2 色相 |
| 3 照順序給色號 | — | 總塊數 | 左起第 1 塊色相 | 左起第 2 塊色相 | 左起第 3 塊色相 |
| 4 循線（①②③ 相同） | 路口類型 | 近處線寬 % | 近處偏移＋100 | 遠處偏移＋100 | 線角度＋100 |
| 5 ① 物體清單 | 物體數 | 物體 1 代碼 | 物體 1 X×10＋Y（8×6 格） | 物體 2 代碼 | 物體 2 X×10＋Y（8×6 格） |
| 5 ② 物體詳細 | 物體數 | 物體 1 代碼 | 物體 1 X | 物體 1 Y | 物體 1 面積 % |

> [!CAUTION]
> 官方 App 裡**不要用「顏色是 [紅色]？」這種選顏色的積木**，它比對的是樂高自己的色號。一律把數字放進變數再比大小。家用版 App 的 Python 讀法還沒驗證，這頁只列積木讀法。

## 要注意的事

- **以下都還沒上機**：數值排法（逐 byte）已經用規格的獨立參考實作在電腦上比對過；車子的 KP／KD、面積門檻、路口停車時機是建議值，請先印出數字再調。
- 換相機模式後，`mode=` 也要跟著改（SPIKE 讀不到相機的模式）。
- 主機斷線時相機畫面左上的主機名稱會變紅，重新執行程式就好。

## 下一步

- 每一格的完整對照：[讀值參考](/sensors/unicam/reference.md)
- EV3 版範例：[EV3 範例](/sensors/unicam/ev3-examples.md)
