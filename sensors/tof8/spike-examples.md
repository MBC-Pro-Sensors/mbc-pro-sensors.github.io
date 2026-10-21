<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 scratchpad/products/tof8 產生；程式碼區塊來自 examples/tof8/spike_beginner/pybricks/*.py，改程式請改產生器再重建 -->
# 🧱 SPIKE 新手範例：測距者 8 路 (TOF8)

[🔙 回到測距者 8 路產品頁](/sensors/tof8/index.md)

這一頁是寫給**完全沒寫過程式**的人。5 個範例從「看到最近的東西在哪個方向」開始，一步一步做到「機器人自己避開障礙物」「跟著你的手走」。每個範例都有 3 種寫法，挑你手上有的那一種就好：

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟨 **樂高官方 SPIKE App**（積木） | 學校、第一次用 SPIKE 的人 | 下載 `.llsp3` → 打開 SPIKE App → 左上「檔案」→「開啟」→ 選檔案 |
| 🟦 **MBC 積木網站**（中文積木） | 想用中文積木、要讀更多感應器資料的人 | 打開 [MBC SPIKE 積木網站](https://mbc-spike-blocks.mbcrobot.workers.dev) → 上方選單「檔案 → 開啟」（或按 Ctrl+O）→ 選下載的 `.abp` |
| 🐍 **Pybricks**（Python 文字程式） | 想學打字寫程式的人 | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊檔案區的「Import a file（匯入檔案）」→ 範例檔和 `MBC_laser8_obj_Lib.py` **兩個都要匯入** → 點開範例 → 按 ▶ 執行 |

> [!IMPORTANT]
> - **MBC 積木網站與 Pybricks 要先把主機換成 Pybricks 韌體**（MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」就能燒，約 3 分鐘；之後也能用 SPIKE App 換回樂高原廠韌體）。樂高官方 App 不用換。
> - 樂高官方 App 這裡用的是**教育版（SPIKE 3）**。家用版（Robot Inventor）讀到的格式不一樣，這頁的官方 App 範例不適用。
> - **MBC 積木網站和 Pybricks 寫起來比較短**：感應器自己會算好「最近的方向、距離」「最空曠的方向」。官方 App 沒有這些積木，要自己用積木一路一路比，所以官方 App 的範例會比較長。

## 開始之前：認識測距者 8 路

測距者 8 路有 **8 顆雷射**，排成半圓，從車子的正左邊、正前方一直到正右邊。不用校準，插上去就能用；**開機後要等 2～4 秒**讓 8 顆雷射準備好（這段時間全部讀到 120）。

| 第幾路 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| 朝哪裡 | 正左邊 | 左前 60° | 左前 30° | 正前方 | 正前方 | 右前 30° | 右前 60° | 正右邊 |

> [!IMPORTANT]
> **請先用範例 1 確認你的感應器哪一路對應哪個方向。** 上面的表是感應器正面朝前、元件面朝上的裝法。手放在你的機器人左邊，範例 1 應該顯示**負的**數字；如果顯示正的，代表裝法跟範例相反，後面範例的「左轉」「右轉」也要對調。

**這頁會用到的數值**

| 名稱 | 範圍 | 意思 |
|---|---|---|
| **第 1～8 路距離** | 0 ～ 120 公分 | 每一顆雷射量到的距離。**120＝那個方向沒東西**（或超過 120 公分） |
| **最近目標距離** | 0 ～ 120 公分 | 8 路裡面最近的那個距離 |
| **最近目標方向** | -7 ～ 7 | 最近的東西在哪：**負的＝左邊、0＝正前方、正的＝右邊**（-7 正左邊、-5 左前 60°、-3 左前 30°、-1～1 正前方，右邊同理）。四周都沒東西時是 -8 或 8，所以這頁一律先用「最近目標距離是不是 120」判斷有沒有東西 |
| **最空曠方向** | -7 ～ 7 | 哪個方向最空、最遠（範例 4 避障用） |

> [!TIP]
> **讀到 120 有 4 種可能**：那個方向真的沒東西、東西超過 120 公分、剛開機還沒準備好、那顆雷射壞了。黑色、亮面、透明（玻璃、壓克力）的東西比較難量，**黑色物體大概 30～50 公分外就量不到**，所以範例的判斷距離都設在 60 公分以內。

**官方 App 怎麼讀？** 在樂高官方 App 裡，測距者會被當成「顏色感應器」，8 路的距離分散在 5 個積木裡：

| 官方積木 | 讀到的是 | 怎麼拆 |
|---|---|---|
| **color（顏色）** | 第 1 路 | 直接用 |
| **reflected light（反射光）** | 第 2 路 | 直接用 |
| **raw red（原始紅色，在「更多感應器」）** | 第 3 路 ＋ 第 4 路 × 256 | 除以 256 的**餘數**＝第 3 路；÷ 256 **取整數**＝第 4 路 |
| **raw green（原始綠色）** | 第 5 路 ＋ 第 6 路 × 256 | 餘數＝第 5 路；取整數＝第 6 路 |
| **raw blue（原始藍色）** | 第 7 路 ＋ 第 8 路 × 256 | 餘數＝第 7 路；取整數＝第 8 路 |

範例 1 會教你把這 5 個積木拆成「第1路」～「第8路」8 個變數，後面的範例都用同一段積木開頭。

> [!WARNING]
> **官方 App 的讀法是依照感應器韌體推算的，還沒有用實機逐一確認。** 特別是第 1 路（顏色積木）和第 2 路（反射光積木）：App 可能只認得 0～10（顏色）、0～100（反射光）的數字，超過的會不會被截掉還沒確認。如果官方 App 版的範例 1 **不管手放哪裡都顯示 -7**（第 1 路）或怪怪的數字，請先改用 MBC 積木網站或 Pybricks，並跟我們回報。

---

## 範例 1：最近的東西在哪個方向

**這個範例會做什麼**：主機螢幕顯示**最近的東西在哪個方向**（-7 ～ 7）。手放左邊顯示負的、放正前方顯示 0、放右邊顯示正的；四周都沒東西顯示 ✕。

**要準備什麼**
- SPIKE 主機 1 台
- 測距者 8 路 1 個
- 你的手（或一本書）

**怎麼接**：測距者 8 路插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex01_nearest_direction.llsp3](/examples/tof8/spike_beginner/official/ex01_nearest_direction.llsp3) | [ex01_nearest_direction.abp](/examples/tof8/spike_beginner/mbcblocks/ex01_nearest_direction.abp) | [ex01_nearest_direction.py](/examples/tof8/spike_beginner/pybricks/ex01_nearest_direction.py)＋[函式庫](/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 1 官方 App 積木](/images/sensors/tof8/spike-beginner/ex01-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 1 MBC 積木網站積木](/images/sensors/tof8/spike-beginner/ex01-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 8 路 範例 1：最近的東西在哪個方向（螢幕顯示 -7 ~ 7，沒東西顯示 ✕）
# 接線：測距者 8 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

hub = PrimeHub()
laser8 = MBC_LASER8(3)                      # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

while True:
    dist = laser8.min_distance()            # 最近的東西有多遠（公分，0~120；120 = 四周都沒東西）
    direction = laser8.min_direction()      # 最近的東西在哪個方向：負的 = 左邊、0 = 正前方、正的 = 右邊
    if dist >= 120:
        hub.display.icon(Icon.FALSE)        # 沒看到東西 → 顯示 ✕
    else:
        hub.display.number(direction)
        print('方向', direction, '  距離', dist)
    wait(200)                               # 等 0.2 秒再讀下一次
```

</details>

**一步一步看懂程式**
1. **一直重複**：下面的積木會一直做，直到你按停止。
2. **讀最近的距離和方向**：MBC 積木網站和 Pybricks 直接有「最近目標距離」「最近目標方向」。
   官方 App 要自己做，分 3 段：
   - **讀 8 路**：照上面的表，把 5 個積木拆成「第1路」～「第8路」8 個變數。
   - **一路一路比**：先假設「最近距離」是 120、「最近第幾路」是 0；然後從第 1 路比到第 8 路，**如果這一路比「最近距離」還近，就把它記下來**。比完以後，「最近第幾路」就是最近的那一路（都沒東西的話還是 0）。
   - **換成方向**：方向＝第幾路 × 2 － 9（第 1 路 → -7、第 4 路 → -1、第 5 路 → 1、第 8 路 → 7）。
3. **如果 沒東西**（MBC 積木網站和 Pybricks：最近距離 ≥ 120；官方 App：最近第幾路 = 0）→ 顯示 ✕。
4. **否則** → 螢幕顯示方向。MBC 積木網站和 Pybricks 也會在電腦畫面印出方向和距離。

**跑起來應該看到什麼**
- 四周沒東西：✕。
- 手放在正左邊 30 公分：**-7**；左前方：**-5** 或 **-3**。
- 手放在正前方：**0**（官方 App 版會是 **-1** 或 **1**，因為它只挑一路，第 4、5 路都朝正前方）。
- 手放在右邊：正的數字。

**常見問題**
- **左右相反？** → 你的感應器裝法跟表上相反（例如上下顛倒），後面範例的左右轉也要對調。
- **一直顯示 ✕？** → 剛開機要等 2～4 秒；或手太遠（超過 120 公分）、東西是黑色／透明的。
- **MBC 積木網站顯示偶數（例如 -6、-2）？** → 正常。手剛好在兩路中間、兩路都看到時，方向會取平均。
- **手放哪裡都顯示同一個數字？** → 可能有一路壞了或被擋住，一直讀到很小的數字。用 MBC 積木網站的「第 N 路正常」積木或 Pybricks 的 `laser8.is_ok(1)`～`is_ok(8)` 檢查。

---

## 範例 2：有東西靠近就變燈號

**這個範例會做什麼**：8 個方向任何一個有東西靠近，主機燈號會變：**50 公分內亮黃燈，20 公分內亮紅燈並嗶嗶叫**，都沒有就亮綠燈。像倒車雷達一樣。

**要準備什麼**：同範例 1。

**怎麼接**：測距者 8 路插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex02_near_light.llsp3](/examples/tof8/spike_beginner/official/ex02_near_light.llsp3) | [ex02_near_light.abp](/examples/tof8/spike_beginner/mbcblocks/ex02_near_light.abp) | [ex02_near_light.py](/examples/tof8/spike_beginner/pybricks/ex02_near_light.py)＋[函式庫](/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 2 官方 App 積木](/images/sensors/tof8/spike-beginner/ex02-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 2 MBC 積木網站積木](/images/sensors/tof8/spike-beginner/ex02-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 8 路 範例 2：有東西靠近就變燈號（< 20 公分紅燈＋嗶、< 50 公分黃燈、其他綠燈）
# 接線：測距者 8 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

hub = PrimeHub()
laser8 = MBC_LASER8(3)                  # 3 = C 埠

while True:
    dist = laser8.min_distance()        # 8 路裡最近的那個距離（公分）
    if dist < 20:                       # 很近了！
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)
    elif dist < 50:                     # 有東西在附近
        hub.light.on(Color.YELLOW)
    else:                               # 50 公分內都沒東西
        hub.light.on(Color.GREEN)
    wait(100)
```

</details>

**一步一步看懂程式**
1. **讀最近的距離**：官方 App 用範例 1 那段「讀 8 路＋一路一路比」積木；另外兩種直接讀「最近目標距離」。
2. **如果 最近距離 < 20** → 紅燈＋嗶一聲。
3. **否則，如果 最近距離 < 50** → 黃燈。
4. **都不是** → 綠燈。
5. **等 0.1 秒**再檢查一次。

**跑起來應該看到什麼**：手從遠處慢慢靠近，燈號從綠 → 黃 → 紅（嗶嗶叫）。手從左邊、右邊靠近也一樣會變。

**常見問題**
- **為什麼要先檢查 < 20，再檢查 < 50？** → 15 公分同時也「小於 50」。如果先檢查 < 50，就永遠不會亮紅燈了。條件要從**最嚴格的**開始寫。
- **手貼到感應器上反而變綠燈？** → 貼太近（幾公分以內）雷射可能量不到，會讀成 120。留一點距離就好。
- **放在桌上一直是黃燈？** → 側邊那幾路量到了桌上的其他東西，或感應器裝太低量到桌面。

---

## 範例 3：轉頭朝向最近的東西

**這個範例會做什麼**：機器人**原地轉**，讓車頭對準最近的東西（60 公分內）。手在左邊就往左轉、在右邊就往右轉，對準了就停。

**要準備什麼**
- 範例 1 的東西，再加上
- 兩輪小車（兩顆馬達當輪子）

**怎麼接**
- 測距者 8 路插 **C 孔**，裝在**車頭**，第 4、5 路朝正前方，離地至少 5 公分
- **左輪接 E、右輪接 F**

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex03_turn_to_nearest.llsp3](/examples/tof8/spike_beginner/official/ex03_turn_to_nearest.llsp3) | [ex03_turn_to_nearest.abp](/examples/tof8/spike_beginner/mbcblocks/ex03_turn_to_nearest.abp) | [ex03_turn_to_nearest.py](/examples/tof8/spike_beginner/pybricks/ex03_turn_to_nearest.py)＋[函式庫](/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 3 官方 App 積木](/images/sensors/tof8/spike-beginner/ex03-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 3 MBC 積木網站積木](/images/sensors/tof8/spike-beginner/ex03-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 8 路 範例 3：原地轉頭，讓車頭朝向最近的東西（60 公分外的不理它）
# 接線：測距者 8 路 → C 埠（裝在車頭）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

laser8 = MBC_LASER8(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    dist = laser8.min_distance()
    direction = laser8.min_direction()      # -7（最左）~ 0（正前方）~ 7（最右）
    if dist > 60:                           # 太遠或沒東西 → 停著等
        robot.stop()
    elif direction < -1:                    # 東西在左邊 → 原地左轉
        robot.drive(0, -90)                 # 不前進，每秒左轉 90 度
    elif direction > 1:                     # 東西在右邊 → 原地右轉
        robot.drive(0, 90)
    else:                                   # -1、0、1：已經在正前方 → 停
        robot.stop()
    wait(50)
```

</details>

**一步一步看懂程式**
1. **告訴主機輪子接在哪裡**：官方 App 是「set movement motors to E+F」，速度 20%；MBC 積木網站和 Pybricks 是「移動底盤 左輪 E、右輪 F」。
2. **讀最近的距離和方向**（官方 App 是「最近第幾路」）。
3. **如果 最近距離 > 60** → 太遠或沒東西 → 停著等。
4. **否則，如果 東西在左邊**（方向 < -1；官方 App：第 1～3 路）→ **原地左轉**。
5. **否則，如果 東西在右邊**（方向 > 1；官方 App：第 6～8 路）→ **原地右轉**。
6. **都不是**（正前方）→ 停。

**跑起來應該看到什麼**：手放在機器人左邊，它會轉過來面對你的手；手繞著它慢慢走，它會一直轉頭跟著你。

**常見問題**
- **轉過頭、在手的兩邊晃來晃去？** → 轉太快了。MBC 積木網站和 Pybricks：把「90」改小（例如 60）；官方 App：把速度 20% 改小（例如 15%）。
- **轉的方向相反、越轉越遠？** → 回到範例 1 確認左右；或是馬達裝法相反（看範例 4 的常見問題）。
- **一直轉不停？** → 旁邊的牆、桌腳比你的手還近，它就會去找牆。在空曠的地方玩。

---

## 範例 4：避障，往最空曠的地方走

**這個範例會做什麼**：機器人一直往前走。**正前方 30 公分內有東西**就停下來，看看**哪一邊比較空曠**，往那邊轉 45 度，再繼續走。

**要準備什麼**：同範例 3，再找一塊空地、放幾個盒子當障礙物。

**怎麼接**：測距者 8 路插 **C 孔**、裝在車頭；**左輪接 E、右輪接 F**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex04_avoid_open_side.llsp3](/examples/tof8/spike_beginner/official/ex04_avoid_open_side.llsp3) | [ex04_avoid_open_side.abp](/examples/tof8/spike_beginner/mbcblocks/ex04_avoid_open_side.abp) | [ex04_avoid_open_side.py](/examples/tof8/spike_beginner/pybricks/ex04_avoid_open_side.py)＋[函式庫](/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 4 官方 App 積木](/images/sensors/tof8/spike-beginner/ex04-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 4 MBC 積木網站積木](/images/sensors/tof8/spike-beginner/ex04-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 8 路 範例 4：避障。一直往前走；正前方（第 4、5 路）小於 30 公分就停下，往最空曠的那一邊轉 45 度
# 接線：測距者 8 路 → C 埠（裝在車頭）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

laser8 = MBC_LASER8(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    if laser8.ch(4) < 30 or laser8.ch(5) < 30:   # 第 4、5 路都朝正前方，任何一個小於 30 公分 → 前面有東西
        robot.stop()
        if laser8.max_direction() < 0:           # 最空曠的方向在左邊（負的）
            robot.turn(-45)                      # 原地左轉 45 度
        else:
            robot.turn(45)                       # 原地右轉 45 度
    else:
        robot.drive(150, 0)                      # 每秒 150 mm 往前走
    wait(50)
```

</details>

**一步一步看懂程式**
1. **設定輪子和速度**：官方 App 速度 30%；另外兩種是每秒 150 mm。
2. **一直重複**：看正前方的**第 4 路或第 5 路**有沒有小於 30 公分。
3. **如果 有** → 停車，然後找最空曠的方向：
   - MBC 積木網站和 Pybricks 直接讀「最空曠方向」：**負的（左邊比較空）→ 左轉 45 度**，否則右轉 45 度。
   - 官方 App 用跟範例 1 一樣的「一路一路比」，只是改成找**最遠**的那一路：第 1～4 路 → 左轉，第 5～8 路 → 右轉。官方 App 的轉 45 度是「以轉向 -100（或 100）移動 0.25 圈」，樂高標準小車大約轉 45 度。
4. **否則** → 往前走。

**跑起來應該看到什麼**：小車往前走，快撞到盒子時停下、往比較空的那邊轉，再繼續走。走到牆角也會一直轉，直到找到出路。

**常見問題**
- **撞上去才停？** → 太快了，或障礙物是黑色、透明的。把速度調低，或把「30」改大。
- **擦到旁邊的東西？** → 這個範例只看正前方兩路。想更安全，可以把第 3、6 路也加進「或」裡面一起檢查。
- **轉的方向跟空曠的那邊相反？** → 回到範例 1 確認左右；左右相反的話，把左轉、右轉對調。
- **小車往後退、方向相反？** → 馬達的裝法跟樂高標準小車相反。官方 App：請照樂高「標準驅動底盤」的方向裝馬達；MBC 積木網站：「移動底盤」積木裡左右輪的「正轉」方向都換成另一個；Pybricks：左輪改成 `Direction.CLOCKWISE`、右輪改成 `Motor(Port.F, Direction.COUNTERCLOCKWISE)`。
- **某一路壞了會怎樣？** → 壞掉的那一路一直讀 120，會被當成「最空曠」，機器人可能往障礙物轉。用範例 1 的常見問題檢查每一路。

---

## 範例 5：跟著手走

**這個範例會做什麼**：機器人跟著你的手走，並且**保持 15～25 公分的距離**：手在遠一點（25～60 公分）就一邊往前走、一邊轉向手；剛剛好就停；太近（小於 15 公分）就後退；手拿走（超過 60 公分）就停。

**要準備什麼**：同範例 3。

**怎麼接**：測距者 8 路插 **C 孔**、裝在車頭；**左輪接 E、右輪接 F**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex05_follow_hand.llsp3](/examples/tof8/spike_beginner/official/ex05_follow_hand.llsp3) | [ex05_follow_hand.abp](/examples/tof8/spike_beginner/mbcblocks/ex05_follow_hand.abp) | [ex05_follow_hand.py](/examples/tof8/spike_beginner/pybricks/ex05_follow_hand.py)＋[函式庫](/examples/tof8/spike_beginner/pybricks/MBC_laser8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 5 官方 App 積木](/images/sensors/tof8/spike-beginner/ex05-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 5 MBC 積木網站積木](/images/sensors/tof8/spike-beginner/ex05-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 8 路 範例 5：跟著手走。手在 25~60 公分：一邊往前走一邊轉向手；15~25 公分：停；太近：後退
# 接線：測距者 8 路 → C 埠（裝在車頭）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser8_obj_Lib import MBC_LASER8

laser8 = MBC_LASER8(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    dist = laser8.min_distance()            # 手（最近的東西）有多遠
    direction = laser8.min_direction()      # 手在哪個方向：-7 ~ 7
    if dist > 60:                           # 看不到手 → 停
        robot.stop()
    elif dist < 15:                         # 太近了 → 每秒 80 mm 往後退
        robot.drive(-80, 0)
    elif dist > 25:                         # 有點遠 → 往前走，手偏多少就轉多少
        robot.drive(100, direction * 15)    # 方向 × 15 = 每秒轉幾度（左負右正）
    else:                                   # 15 ~ 25 公分：剛剛好 → 停
        robot.stop()
    wait(50)
```

</details>

**一步一步看懂程式**
1. **讀手（最近的東西）的距離和方向**（官方 App 用範例 1 那段積木）。
2. 依照距離分 4 種情況，**從上往下檢查**：
   - **> 60 公分** → 看不到手 → 停。
   - **< 15 公分** → 太近 → 後退（MBC 積木網站和 Pybricks：每秒 80 mm）。
   - **> 25 公分** → 有點遠 → 往前走，並且**手偏多少就轉多少**：MBC 積木網站和 Pybricks 每秒轉「方向 × 15」度；官方 App 的轉向是「方向 × 10」（方向一樣是第幾路 × 2 － 9）。
   - **剩下的（15～25 公分）** → 剛剛好 → 停。

**跑起來應該看到什麼**：手放在機器人前面 40 公分，它會往前走；手往左移，它會邊走邊左轉；手停住，它也在 20 公分左右停下來；手往前推，它會往後退。

**常見問題**
- **一直前後抖動？** → 「剛剛好」的範圍太窄。把 15 改成 12、25 改成 30。
- **轉彎太猛／太慢？** → 改「× 15」（官方 App 是「× 10」）那個數字：大一點轉得快、小一點轉得慢。
- **跟著牆走，不跟手？** → 旁邊的東西比你的手還近。在空曠的地方玩，手要是最近的東西。
- **官方 App 版不會後退？** → 請確認「start moving（開始移動）」積木的方向是「往後」的箭頭。

---

## 速度單位說明

- 官方 App 的速度是**百分比（%）**：30% 大約是馬達最快速度的三成。
- MBC 積木網站和 Pybricks 的速度是**每秒幾 mm**，轉彎是**每秒幾度**。
- 所以三種寫法的數字不一樣，但動作的意思相同。

## 下一步

- 產品介紹、規格、角度分佈與其他平台範例：[測距者 8 路產品頁](/sensors/tof8/index.md)
- 只需要前方左右兩點：[測距者 2 路 SPIKE 新手範例](/sensors/tof2/spike-examples.md)
- Pybricks 函式庫的全部功能：打開 `MBC_laser8_obj_Lib.py`，最上面有中文說明（例如 `laser8.distances()` 一次拿到 8 路距離、`laser8.is_ok(3)` 檢查第 3 路是不是正常）。
