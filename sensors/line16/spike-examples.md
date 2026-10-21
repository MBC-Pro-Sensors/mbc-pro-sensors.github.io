<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 scratchpad/products/line16 產生；程式碼區塊來自 examples/line16/spike_beginner/pybricks/*.py，改程式請改產生器再重建 -->
# 🧱 SPIKE 新手範例：循行者 16 路 (Line16)

[🔙 回到循行者 16 路產品頁](/sensors/line16/index.md)

這一頁是寫給**完全沒寫過程式**的人。5 個範例從「看到數字」開始，一步一步做到「認出路口在哪一邊」和「機器人自己沿著黑線走、遇到十字路口停車」。每個範例都有 3 種寫法，挑你手上有的那一種就好：

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟨 **樂高官方 SPIKE App**（積木） | 學校、第一次用 SPIKE 的人 | 下載 `.llsp3` → 打開 SPIKE App → 左上「檔案」→「開啟」→ 選檔案 |
| 🟦 **MBC 積木網站**（中文積木） | 想用中文積木、要讀更多感應器資料的人 | 打開 [MBC SPIKE 積木網站](https://mbc-spike-blocks.mbcrobot.workers.dev) → 上方選單「檔案 → 開啟」（或按 Ctrl+O）→ 選下載的 `.abp` |
| 🐍 **Pybricks**（Python 文字程式） | 想學打字寫程式的人 | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊檔案區的「Import a file（匯入檔案）」→ 範例檔和 `MBC_line16_obj_Lib.py` **兩個都要匯入** → 點開範例 → 按 ▶ 執行 |

> [!IMPORTANT]
> - **MBC 積木網站與 Pybricks 要先把主機換成 Pybricks 韌體**（MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」就能燒，約 3 分鐘；之後也能用 SPIKE App 換回樂高原廠韌體）。樂高官方 App 不用換。
> - 樂高官方 App 這裡用的是**教育版（SPIKE 3）**。家用版（Robot Inventor）讀法不一樣（線寬最多只到 10），請看 [家用版說明](/sensors/line16/spike-ri.md)。

## 開始之前：先校準（只要做一次）

循行者要先「認識」你場地上的黑色和白色，數值才會準。校準完會記在感應器裡，重新開機也還在。

1. 感應器插好、主機開機（感應器的燈亮起來）。
2. **按一下**感應器上的校準按鈕就放開（不要一直按著，按著不放會一直重新校準）。
3. 綠色校準燈亮起後，**5 秒內**把感應器在黑線和白色地面之間來回掃幾次，16 個感應點都要掃過黑和白。
4. 綠燈熄滅＝校準完成。

**怎麼知道校準好了？** 跑下面的範例 2：放在白色地面時是紅燈（線寬 0），壓在黑線上是綠燈。

## 這頁會用到的 3 個數值

| 名稱 | 範圍 | 意思 |
|---|---|---|
| **線位置**（粗） | -16 ～ 16 | 0＝線在正中間；**負的＝線在左邊**、正的＝線在右邊 |
| **線位置**（細） | -100 ～ 100 | 跟上面一樣，只是分得更細，循線會更順 |
| **線寬** | 0 ～ 16 | 有幾個感應點踩在黑線上。**0＝找不到線**；一般黑線只有幾路；碰到路口會變很寬，**十字路口整排踩到，會到 11 以上** |

> [!TIP]
> 線不見的時候，「線位置」會**停在最後一次看到的值**，不會變回 0。所以判斷「有沒有線」一律看**線寬是不是 0**。

在樂高官方 App 裡，循行者會被當成「顏色感應器」，所以要用顏色感應器的積木來讀：

| 官方積木 | 讀到的是 | 換算 |
|---|---|---|
| **reflected light（反射光）** | 線位置（粗）＋16 | 讀到的數字 **減 16** |
| **color（顏色）** | 線寬 | 直接用 |
| **raw red（原始紅色，在「更多感應器」）** | 線位置（細）＋100 | 讀到的數字 **減 100** |

---

## 範例 1：看見黑線

**這個範例會做什麼**：主機螢幕一直顯示線的位置（-16 ～ 16）。把黑線從左邊慢慢移到右邊，數字會從負的變成正的。

**要準備什麼**
- SPIKE 主機 1 台
- 循行者 16 路 1 個（已校準）
- 一張白紙畫一條黑線（或比賽地圖）

**怎麼接**：循行者插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex01_show_position.llsp3](/examples/line16/spike_beginner/official/ex01_show_position.llsp3) | [ex01_show_position.abp](/examples/line16/spike_beginner/mbcblocks/ex01_show_position.abp) | [ex01_show_position.py](/examples/line16/spike_beginner/pybricks/ex01_show_position.py)＋[函式庫](/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 1 官方 App 積木](/images/sensors/line16/spike-beginner/ex01-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 1 MBC 積木網站積木](/images/sensors/line16/spike-beginner/ex01-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 16 路 範例 1：看見黑線（顯示線位置）
# 接線：循行者 16 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

hub = PrimeHub()
line16 = MBC_LINE16(3)        # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

while True:
    pos = line16.pos16()      # 線位置：-16（線在左）~ 0（中間）~ 16（線在右）
    hub.display.number(pos)   # 主機螢幕顯示這個數字
    print(pos)                # 電腦畫面也印出來
    wait(200)                 # 等 0.2 秒再讀下一次
```

</details>

**一步一步看懂程式**
1. **一直重複**：下面的積木會一直做，直到你按停止。
2. **讀線位置**：官方 App 是「反射光 減 16」；MBC 積木網站和 Pybricks 直接有「線位置（-16~16）」。
3. **顯示在螢幕上**：官方 App 用「write（寫字）」，MBC 積木網站用「螢幕顯示數字」，Pybricks 用 `hub.display.number()`。
4. **等 0.2 秒**：讓數字停一下，眼睛才看得清楚。

**跑起來應該看到什麼**
- 線在感應器正下方：螢幕顯示 **0** 附近（-2 ～ 2 都算中間）。
- 把黑線往左移：數字變成 **負的**，越左越接近 -16。
- 往右移：變成 **正的**，越右越接近 16。

**常見問題**
- **數字一直是 0，不會變？** → 先跑範例 2 看看：放在白紙上還是綠燈，代表每一點都以為是黑色，還沒校準，回到上面「先校準」。
- **左右相反？** → 先確認感應器沒有裝反（元件那一面要朝下、朝向地面）。裝對了還是相反，就把後面範例裡「左轉」「右轉」的數字正負對調。
- **官方 App 顯示 16 而不是 0？** → 忘了「減 16」。

---

## 範例 2：黑線警報

**這個範例會做什麼**：有看到黑線，主機中間的燈亮**綠色**；找不到線，燈變**紅色**並且嗶嗶叫。

**要準備什麼**：同範例 1。

**怎麼接**：循行者插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex02_line_alarm.llsp3](/examples/line16/spike_beginner/official/ex02_line_alarm.llsp3) | [ex02_line_alarm.abp](/examples/line16/spike_beginner/mbcblocks/ex02_line_alarm.abp) | [ex02_line_alarm.py](/examples/line16/spike_beginner/pybricks/ex02_line_alarm.py)＋[函式庫](/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 2 官方 App 積木](/images/sensors/line16/spike-beginner/ex02-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 2 MBC 積木網站積木](/images/sensors/line16/spike-beginner/ex02-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 16 路 範例 2：黑線警報（壓到線亮綠燈；找不到線亮紅燈並嗶一聲）
# 接線：循行者 16 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

hub = PrimeHub()
line16 = MBC_LINE16(3)                # 3 = C 埠

while True:
    if line16.width() > 0:            # 線寬 = 有幾路踩在黑線上；大於 0 就是有看到線
        hub.light.on(Color.GREEN)
    else:                             # 線寬 = 0：16 路都沒踩到 → 找不到線
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)    # 頻率 880 Hz，響 0.1 秒
    wait(100)
```

</details>

**一步一步看懂程式**
1. **讀線寬**：官方 App 用「color（顏色）」積木，讀到的就是線寬。
2. **如果 線寬 > 0 就…**：16 個感應點裡有任何一個踩到黑線 → 亮綠燈。
3. **否則…**：線寬是 0，代表一點都沒踩到 → 亮紅燈、嗶一聲。
4. **等 0.1 秒**再檢查一次。

**跑起來應該看到什麼**
- 感應器放在黑線上：綠燈。
- 移到白色地面：紅燈＋一直嗶嗶叫。

**常見問題**
- **一直是綠燈，放在白紙上也是？** → 還沒校準（每一點都被當成黑色）。
- **官方 App 的燈號顏色怎麼選？** → 點「set Power Button light to」積木上的顏色圓點就能換。

---

## 範例 3：路口在哪一邊（16 路專屬）

**這個範例會做什麼**：16 路的感應點排得又寬又密，碰到路口時看得出「黑色那一大塊在哪裡」。主機螢幕會告訴你：
- 一般的線 → ⬆
- **左邊有岔路**（T 字往左）→ ⬅
- **右邊有岔路**（T 字往右）→ ➡
- **十字路口** → ＋
- 找不到線 → ✕

**要準備什麼**：同範例 1，再用黑膠帶或筆畫出「往左的 T 字」「往右的 T 字」「十字」三種路口。

**怎麼接**：循行者插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex03_junction_side.llsp3](/examples/line16/spike_beginner/official/ex03_junction_side.llsp3) | [ex03_junction_side.abp](/examples/line16/spike_beginner/mbcblocks/ex03_junction_side.abp) | [ex03_junction_side.py](/examples/line16/spike_beginner/pybricks/ex03_junction_side.py)＋[函式庫](/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 3 官方 App 積木](/images/sensors/line16/spike-beginner/ex03-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 3 MBC 積木網站積木](/images/sensors/line16/spike-beginner/ex03-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 16 路 範例 3：路口在哪一邊（左邊有岔路、右邊有岔路，還是十字路口）
# 接線：循行者 16 路 → C 埠
# 想法：一般的線只有幾路踩到；碰到路口時黑色會變很寬（線寬變大），
#       再看這一大塊黑色偏左、偏右還是在中間，就知道岔路在哪一邊。
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

hub = PrimeHub()
line16 = MBC_LINE16(3)                    # 3 = C 埠

while True:
    pos = line16.pos16()                  # 黑色那一塊的中心：負 = 偏左，正 = 偏右
    width = line16.width()                # 有幾路踩到黑色（0 ~ 16）
    if width == 0:                        # 沒看到線（這時位置會停在最後一次的值，不能拿來判斷）
        hub.display.icon(Icon.FALSE)      # 顯示 ✕
    elif width < 6:                       # 窄窄的一條 → 一般的線
        hub.display.icon(Icon.ARROW_UP)
    elif pos < -3:                        # 很寬而且偏左 → 左邊有岔路
        hub.display.icon(Icon.ARROW_LEFT)
    elif pos > 3:                         # 很寬而且偏右 → 右邊有岔路
        hub.display.icon(Icon.ARROW_RIGHT)
    else:                                 # 很寬又在中間 → 十字路口
        hub.display.char('+')
    wait(100)
```

</details>

**一步一步看懂程式**
1. 先讀**線位置**和**線寬**，存進兩個「變數」（變數＝有名字的盒子，可以放一個數字）。
2. **如果 線寬 = 0** → 顯示 ✕（找不到線，這時線位置不準，所以要最先檢查）。
3. **否則，如果 線寬 < 6** → 只有窄窄的一條，是一般的線 → 顯示 ⬆。
4. **否則**（線寬 6 以上＝黑色變很寬，碰到路口了），再看黑色那一塊的中心「線位置」：
   - **線位置 < -3**：黑色偏左 → 左邊有岔路 → ⬅
   - **線位置 > 3**：黑色偏右 → 右邊有岔路 → ➡
   - **都不是**：黑色在正中間、左右一樣寬 → 十字路口 → ＋

**跑起來應該看到什麼**：把感應器慢慢推過三種路口。經過往左的 T 字時會閃一下 ⬅，往右的 T 字閃 ➡，十字閃 ＋，其他時候是 ⬆。

**常見問題**
- **一直顯示 ⬆，路口認不出來？** → 推慢一點；或岔路的線太細，把「6」改成 5。
- **一般的彎道也被當成路口？** → 線太粗，把「6」改成 7。
- **官方 App 為什麼沒有「＋」符號？** → 官方 App 用自己畫的 5×5 圖案（中間一橫一豎）；MBC 積木網站和 Pybricks 直接顯示「+」這個字。
- **官方 App 的圖案要怎麼改？** → 點「turn on」積木上的小方格圖，就能自己點亮想要的格子。

---

## 範例 4：最簡單的循線

**這個範例會做什麼**：機器人自己沿著黑線走。線在左邊就往左轉、在右邊就往右轉、在中間就直走。

**要準備什麼**
- 範例 1 的東西，再加上
- 兩輪小車（兩顆馬達當輪子）
- 一圈黑線（寬約 2 公分，轉彎不要太急）

**怎麼接**
- 循行者插 **C 孔**，裝在車頭、離地約 0.5～1 公分
- **左輪接 E、右輪接 F**

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex04_simple_follow.llsp3](/examples/line16/spike_beginner/official/ex04_simple_follow.llsp3) | [ex04_simple_follow.abp](/examples/line16/spike_beginner/mbcblocks/ex04_simple_follow.abp) | [ex04_simple_follow.py](/examples/line16/spike_beginner/pybricks/ex04_simple_follow.py)＋[函式庫](/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 4 官方 App 積木](/images/sensors/line16/spike-beginner/ex04-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 4 MBC 積木網站積木](/images/sensors/line16/spike-beginner/ex04-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 16 路 範例 4：最簡單的循線（線在左就左轉、線在右就右轉、中間就直走）
# 接線：循行者 16 路 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

line16 = MBC_LINE16(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    pos = line16.pos16()
    if pos < -2:                  # 線在左邊 → 一邊前進一邊左轉
        robot.drive(100, -60)     # 速度 100 mm/秒，每秒左轉 60 度
    elif pos > 2:                 # 線在右邊 → 右轉
        robot.drive(100, 60)
    else:                         # 線在中間（-2 ~ 2）→ 直走
        robot.drive(100, 0)
    wait(10)
```

</details>

**一步一步看懂程式**
1. **告訴主機輪子接在哪裡**：官方 App 是「set movement motors to E+F」；MBC 積木網站和 Pybricks 是「移動底盤 左輪 E、右輪 F」。
2. **設定速度**：官方 App 速度 30%；另外兩種是每秒 100 mm。先慢慢來，成功了再加快。
3. **一直重複**：讀線位置。
4. **如果 線位置 < -2**（線在左）→ 一邊走一邊**左轉**。
5. **否則如果 線位置 > 2**（線在右）→ **右轉**。
6. **否則**（-2 ～ 2）→ **直走**。

**跑起來應該看到什麼**：小車沿著線走，走起來會左右搖擺（這是正常的，範例 5 會改善）。

**常見問題**
- **一開就往外衝、離線越來越遠？** → 轉彎方向反了：把左右兩個轉彎的數字正負對調（官方 App：-40 和 40 對調；另外兩種：-60 和 60 對調）。
- **小車往後退？** → 馬達的裝法跟樂高標準小車相反。官方 App：請照樂高「標準驅動底盤」的方向裝馬達；MBC 積木網站：「移動底盤」積木裡左右輪的「正轉」方向都換成另一個；Pybricks：左輪改成 `Direction.CLOCKWISE`、右輪改成 `Motor(Port.F, Direction.COUNTERCLOCKWISE)`。
- **轉彎太急衝出去？** → 把速度調低。
- **為什麼中間是 -2 ～ 2？** → 16 路分得比較細，線稍微偏一點數字就會動，留一點範圍車子才不會一直扭來扭去。

---

## 範例 5：平順循線＋十字路口停車

**這個範例會做什麼**：用「偏多少就轉多少」的方法循線，走得比範例 4 平順；遇到**十字路口**（整排感應點幾乎都踩到黑色）就停車並嗶一聲。經過 T 字岔路不會停，會繼續往前走。

**要準備什麼**：同範例 4，黑線上要有一個十字路口。

**怎麼接**：循行者插 **C 孔**；**左輪接 E、右輪接 F**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex05_smooth_follow_stop.llsp3](/examples/line16/spike_beginner/official/ex05_smooth_follow_stop.llsp3) | [ex05_smooth_follow_stop.abp](/examples/line16/spike_beginner/mbcblocks/ex05_smooth_follow_stop.abp) | [ex05_smooth_follow_stop.py](/examples/line16/spike_beginner/pybricks/ex05_smooth_follow_stop.py)＋[函式庫](/examples/line16/spike_beginner/pybricks/MBC_line16_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 5 官方 App 積木](/images/sensors/line16/spike-beginner/ex05-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 5 MBC 積木網站積木](/images/sensors/line16/spike-beginner/ex05-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 16 路 範例 5：平順循線，遇到十字路口就停車
# 接線：循行者 16 路 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_line16_obj_Lib import MBC_LINE16

hub = PrimeHub()
line16 = MBC_LINE16(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    if line16.width() >= 11:      # 11 路以上同時踩到黑線 → 十字路口（整排 16 路幾乎全黑）
        robot.stop()
        hub.speaker.beep(880, 500)
        break                     # 跳出迴圈，程式結束
    pos = line16.pos100()         # 更精細的位置：-100 ~ 100
    robot.drive(120, pos * 1.5)   # 偏多少就轉多少：偏越多轉越快
    wait(10)
```

</details>

**一步一步看懂程式**
1. 讀**細的線位置**（-100 ～ 100）：官方 App 是「raw red（原始紅色）減 100」。
2. 讀**線寬**。
3. **如果 線寬 ≥ 11**（官方 App 寫成「線寬 > 10」，意思一樣）→ 十字路口：**停車 → 嗶一聲 → 結束程式**。
4. **否則** → 轉彎的力道＝**線位置 × 一個數字**（官方 App 乘 0.5，另外兩種乘 1.5）。偏得越多轉得越大，偏一點點就只轉一點點，所以不會左右亂晃。

**跑起來應該看到什麼**：小車順順地沿線走，經過 T 字岔路繼續走，到十字路口停下來嗶一聲。

**常見問題**
- **還是左右搖晃？** → 把「乘的數字」調小一點（例如 0.5 → 0.3）。
- **轉彎跟不上、衝出線外？** → 「乘的數字」調大一點，或把速度調低。
- **到了十字路口沒停？** → 車子太快、一下就衝過去了，把速度調低；或把「11」改成 10。
- **還沒到十字路口，在 T 字岔路就停了？** → 線太粗，把「11」改成 12 或 13（最大是 16）。
- **官方 App 版到十字路口都不停？** → 「color（顏色）」積木讀到 11 以上這件事我們還沒在每一版 SPIKE App 上實際測過。請把範例 1 的「反射光 減 16」換成「color」，把感應器放在十字路口上看螢幕的數字；如果最多只到 10，就把範例 5 的「> 10」改成「> 9」。
- **官方 App 找不到「raw red」積木？** → 開檔後會自動加上「更多感應器（More Sensors）」積木區；如果沒有，點左下角積木區的「＋」加入「More Sensors」。

---

## 下一步

- 想更精準的循線：[SPIKE 循線 PID 教學](/articles/spike-pid-line-following.md)
- 全部數值的完整說明：[官方 App 教育版](/sensors/line16/spike-education.md)、[Pybricks](/sensors/line16/spike-pybricks.md)
