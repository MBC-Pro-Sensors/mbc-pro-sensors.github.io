<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 scratchpad/products/line8 產生；程式碼區塊來自 examples/line8/spike_beginner/pybricks/*.py，改程式請改產生器再重建 -->
<!-- moved: /sensors/line8/spike/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line8/spike/index.md">循行者 8 路 › SPIKE</a></p>

# 🧱 SPIKE 新手範例：循行者 8 路 (Line8)

[🔙 回到循行者 8 路產品頁](/sensors/line8/index.md)

這一頁是寫給**完全沒寫過程式**的人。5 個範例從「看到數字」開始，一步一步做到「機器人自己沿著黑線走、遇到路口停車」。每個範例都有 3 種寫法，挑你手上有的那一種就好：

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟨 **樂高官方 SPIKE App**（積木） | 學校、第一次用 SPIKE 的人 | 下載 `.llsp3` → 打開 SPIKE App → 左上「檔案」→「開啟」→ 選檔案 |
| 🟦 **MBC 積木網站**（中文積木） | 想用中文積木、要讀更多感應器資料的人 | 打開 [MBC SPIKE 積木網站](https://mbc-spike-blocks.mbcrobot.workers.dev) → 上方選單「檔案 → 開啟」（或按 Ctrl+O）→ 選下載的 `.abp` |
| 🐍 **Pybricks**（Python 文字程式） | 想學打字寫程式的人 | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊檔案區的「Import a file（匯入檔案）」→ 範例檔和 `MBC_line8_obj_Lib.py` **兩個都要匯入** → 點開範例 → 按 ▶ 執行 |

> [!IMPORTANT]
> - **MBC 積木網站與 Pybricks 要先把主機換成 Pybricks 韌體**（MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」就能燒，約 3 分鐘；之後也能用 SPIKE App 換回樂高原廠韌體）。樂高官方 App 不用換。
> - 樂高官方 App 這裡用的是**教育版（SPIKE 3）**。家用版（Robot Inventor）讀法不一樣，請看 [家用版說明](/sensors/line8/spike-ri.md)。

## 開始之前：先校準（只要做一次）

循行者要先「認識」你場地上的黑色和白色，數值才會準。校準完會記在感應器裡，重新開機也還在。

1. 感應器插好、主機開機（感應器的燈亮起來）。
2. **按一下**感應器上的校準按鈕就放開（不要一直按著，按著不放會一直重新校準）。
3. 綠色校準燈亮起後，**5 秒內**把感應器在黑線和白色地面之間來回掃幾次，8 個感應點都要掃過黑和白。
4. 綠燈熄滅＝校準完成。

**怎麼知道校準好了？** 跑下面的範例 1：放在白色地面時「線寬」是 0，壓在一般黑線上是 2～3。

## 這頁會用到的 3 個數值

| 名稱 | 範圍 | 意思 |
|---|---|---|
| **線位置**（粗） | -8 ～ 8 | 0＝線在正中間；**負的＝線在左邊**、正的＝線在右邊 |
| **線位置**（細） | -100 ～ 100 | 跟上面一樣，只是分得更細，循線會更順 |
| **線寬** | 0 ～ 8 | 有幾個感應點踩在黑線上。**0＝找不到線**；一般黑線 2～3；**6 以上＝十字路口**（整排都踩到） |

> [!TIP]
> 線不見的時候，「線位置」會**停在最後一次看到的值**，不會變回 0。所以判斷「有沒有線」一律看**線寬是不是 0**。

在樂高官方 App 裡，循行者會被當成「顏色感應器」，所以要用顏色感應器的積木來讀：

| 官方積木 | 讀到的是 | 換算 |
|---|---|---|
| **reflected light（反射光）** | 線位置（粗）＋8 | 讀到的數字 **減 8** |
| **color（顏色）** | 線寬 | 直接用 |
| **raw red（原始紅色，在「更多感應器」）** | 線位置（細）＋100 | 讀到的數字 **減 100** |

---

## 範例 1：看見黑線

**這個範例會做什麼**：主機螢幕一直顯示線的位置（-8 ～ 8）。把黑線從左邊慢慢移到右邊，數字會從負的變成正的。

**要準備什麼**
- SPIKE 主機 1 台
- 循行者 8 路 1 個（已校準）
- 一張白紙畫一條黑線（或比賽地圖）

**怎麼接**：循行者插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex01_show_position.llsp3](/examples/line8/spike_beginner/official/ex01_show_position.llsp3) | [ex01_show_position.abp](/examples/line8/spike_beginner/mbcblocks/ex01_show_position.abp) | [ex01_show_position.py](/examples/line8/spike_beginner/pybricks/ex01_show_position.py)＋[函式庫](/examples/line8/spike_beginner/pybricks/MBC_line8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 1 官方 App 積木](/images/sensors/line8/spike-beginner/ex01-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 1 MBC 積木網站積木](/images/sensors/line8/spike-beginner/ex01-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 8 路 範例 1：看見黑線（顯示線位置）
# 接線：循行者 8 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.tools import wait
from MBC_line8_obj_Lib import MBC_LINE8

hub = PrimeHub()
line8 = MBC_LINE8(3)          # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

while True:
    pos = line8.pos8()        # 線位置：-8（線在左）~ 0（中間）~ 8（線在右）
    hub.display.number(pos)   # 主機螢幕顯示這個數字
    print(pos)                # 電腦畫面也印出來
    wait(200)                 # 等 0.2 秒再讀下一次
```

</details>

**一步一步看懂程式**
1. **一直重複**：下面的積木會一直做，直到你按停止。
2. **讀線位置**：官方 App 是「反射光 減 8」；MBC 積木網站和 Pybricks 直接有「線位置（-8~8）」。
3. **顯示在螢幕上**：官方 App 用「write（寫字）」，MBC 積木網站用「螢幕顯示數字」，Pybricks 用 `hub.display.number()`。
4. **等 0.2 秒**：讓數字停一下，眼睛才看得清楚。

**跑起來應該看到什麼**
- 線在感應器正下方：螢幕顯示 **0**（-1、1 也算中間）。
- 把黑線往左移：數字變成 **負的**，越左越接近 -8。
- 往右移：變成 **正的**，越右越接近 8。

**常見問題**
- **數字一直是 0，不會變？** → 先看「線寬」是不是 8（每一點都以為是黑色）：代表還沒校準，回到上面「先校準」。
- **左右相反？** → 感應器裝反了（元件那一面要朝下、朝向地面）。
- **官方 App 顯示 8 而不是 0？** → 忘了「減 8」。

---

## 範例 2：黑線警報

**這個範例會做什麼**：有看到黑線，主機中間的燈亮**綠色**；找不到線，燈變**紅色**並且嗶嗶叫。

**要準備什麼**：同範例 1。

**怎麼接**：循行者插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex02_line_alarm.llsp3](/examples/line8/spike_beginner/official/ex02_line_alarm.llsp3) | [ex02_line_alarm.abp](/examples/line8/spike_beginner/mbcblocks/ex02_line_alarm.abp) | [ex02_line_alarm.py](/examples/line8/spike_beginner/pybricks/ex02_line_alarm.py)＋[函式庫](/examples/line8/spike_beginner/pybricks/MBC_line8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 2 官方 App 積木](/images/sensors/line8/spike-beginner/ex02-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 2 MBC 積木網站積木](/images/sensors/line8/spike-beginner/ex02-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 8 路 範例 2：黑線警報（壓到線亮綠燈；找不到線亮紅燈並嗶一聲）
# 接線：循行者 8 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color
from pybricks.tools import wait
from MBC_line8_obj_Lib import MBC_LINE8

hub = PrimeHub()
line8 = MBC_LINE8(3)                  # 3 = C 埠

while True:
    if line8.width() > 0:             # 線寬 = 有幾路踩在黑線上；大於 0 就是有看到線
        hub.light.on(Color.GREEN)
    else:                             # 線寬 = 0：一路都沒踩到 → 找不到線
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)    # 頻率 880 Hz，響 0.1 秒
    wait(100)
```

</details>

**一步一步看懂程式**
1. **讀線寬**：官方 App 用「color（顏色）」積木，讀到的就是線寬。
2. **如果 線寬 > 0 就…**：有任何一個感應點踩到黑線 → 亮綠燈。
3. **否則…**：線寬是 0，代表一點都沒踩到 → 亮紅燈、嗶一聲。
4. **等 0.1 秒**再檢查一次。

**跑起來應該看到什麼**
- 感應器放在黑線上：綠燈。
- 移到白色地面：紅燈＋一直嗶嗶叫。

**常見問題**
- **一直是綠燈，放在白紙上也是？** → 還沒校準（每一點都被當成黑色）。
- **官方 App 的燈號顏色怎麼選？** → 點「set Power Button light to」積木上的顏色圓點就能換。

---

## 範例 3：線在哪一邊

**這個範例會做什麼**：主機螢幕用**箭頭**告訴你線在哪裡：線在左 → ⬅，線在右 → ➡，在中間 → ⬆，找不到線 → ✕。

**要準備什麼**：同範例 1。

**怎麼接**：循行者插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex03_which_side.llsp3](/examples/line8/spike_beginner/official/ex03_which_side.llsp3) | [ex03_which_side.abp](/examples/line8/spike_beginner/mbcblocks/ex03_which_side.abp) | [ex03_which_side.py](/examples/line8/spike_beginner/pybricks/ex03_which_side.py)＋[函式庫](/examples/line8/spike_beginner/pybricks/MBC_line8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 3 官方 App 積木](/images/sensors/line8/spike-beginner/ex03-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 3 MBC 積木網站積木](/images/sensors/line8/spike-beginner/ex03-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 8 路 範例 3：線在哪一邊（主機螢幕顯示箭頭）
# 接線：循行者 8 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon
from pybricks.tools import wait
from MBC_line8_obj_Lib import MBC_LINE8

hub = PrimeHub()
line8 = MBC_LINE8(3)                      # 3 = C 埠

while True:
    pos = line8.pos8()                    # 負 = 線在左，正 = 線在右
    if line8.width() == 0:                # 沒看到線（這時位置會停在最後一次的值，不能拿來判斷）
        hub.display.icon(Icon.FALSE)      # 顯示 ✕
    elif pos < -1:
        hub.display.icon(Icon.ARROW_LEFT)
    elif pos > 1:
        hub.display.icon(Icon.ARROW_RIGHT)
    else:                                 # -1、0、1 都算在中間
        hub.display.icon(Icon.ARROW_UP)
    wait(100)
```

</details>

**一步一步看懂程式**
1. 先讀**線位置**和**線寬**，存進兩個「變數」（變數＝有名字的盒子，可以放一個數字）。
2. **如果 線寬 = 0** → 顯示 ✕（找不到線，這時線位置不準，所以要最先檢查）。
3. **否則，如果 線位置 < -1** → 顯示左箭頭。
4. **否則，如果 線位置 > 1** → 顯示右箭頭。
5. **都不是**（-1、0、1）→ 顯示上箭頭，代表在中間。

**跑起來應該看到什麼**：慢慢把線從左移到右，箭頭依序變成 ⬅ → ⬆ → ➡；把感應器拿離開線，變成 ✕。

**常見問題**
- **為什麼中間是 -1 ～ 1，不是只有 0？** → 線有寬度，稍微偏一點點也算在中間，箭頭才不會一直跳來跳去。
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
| [ex04_simple_follow.llsp3](/examples/line8/spike_beginner/official/ex04_simple_follow.llsp3) | [ex04_simple_follow.abp](/examples/line8/spike_beginner/mbcblocks/ex04_simple_follow.abp) | [ex04_simple_follow.py](/examples/line8/spike_beginner/pybricks/ex04_simple_follow.py)＋[函式庫](/examples/line8/spike_beginner/pybricks/MBC_line8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 4 官方 App 積木](/images/sensors/line8/spike-beginner/ex04-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 4 MBC 積木網站積木](/images/sensors/line8/spike-beginner/ex04-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 8 路 範例 4：最簡單的循線（線在左就左轉、線在右就右轉、中間就直走）
# 接線：循行者 8 路 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_line8_obj_Lib import MBC_LINE8

line8 = MBC_LINE8(3)                                   # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    pos = line8.pos8()
    if pos < -1:                  # 線在左邊 → 一邊前進一邊左轉
        robot.drive(100, -60)     # 速度 100 mm/秒，每秒左轉 60 度
    elif pos > 1:                 # 線在右邊 → 右轉
        robot.drive(100, 60)
    else:                         # 線在中間 → 直走
        robot.drive(100, 0)
    wait(10)
```

</details>

**一步一步看懂程式**
1. **告訴主機輪子接在哪裡**：官方 App 是「set movement motors to E+F」；MBC 積木網站和 Pybricks 是「移動底盤 左輪 E、右輪 F」。
2. **設定速度**：官方 App 速度 30%；另外兩種是每秒 100 mm。先慢慢來，成功了再加快。
3. **一直重複**：讀線位置。
4. **如果 線位置 < -1**（線在左）→ 一邊走一邊**左轉**。
5. **否則如果 線位置 > 1**（線在右）→ **右轉**。
6. **否則** → **直走**。

**跑起來應該看到什麼**：小車沿著線走，走起來會左右搖擺（這是正常的，範例 5 會改善）。

**常見問題**
- **一開就往外衝、離線越來越遠？** → 轉彎方向反了：把左右兩個轉彎的數字正負對調（官方 App：-40 和 40 對調；另外兩種：-60 和 60 對調）。
- **小車往後退？** → 馬達的裝法跟樂高標準小車相反。官方 App：請照樂高「標準驅動底盤」的方向裝馬達；MBC 積木網站：「移動底盤」積木裡左右輪的「正轉」方向都換成另一個；Pybricks：左輪改成 `Direction.CLOCKWISE`、右輪改成 `Motor(Port.F, Direction.COUNTERCLOCKWISE)`。
- **轉彎太急衝出去？** → 把速度調低。

---

## 範例 5：平順循線＋路口停車

**這個範例會做什麼**：用「偏多少就轉多少」的方法循線，走得比範例 4 平順；遇到**十字路口**（整排感應點都踩到黑色）就停車並嗶一聲。

**要準備什麼**：同範例 4，黑線上要有一個十字路口。

**怎麼接**：循行者插 **C 孔**；**左輪接 E、右輪接 F**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex05_smooth_follow_stop.llsp3](/examples/line8/spike_beginner/official/ex05_smooth_follow_stop.llsp3) | [ex05_smooth_follow_stop.abp](/examples/line8/spike_beginner/mbcblocks/ex05_smooth_follow_stop.abp) | [ex05_smooth_follow_stop.py](/examples/line8/spike_beginner/pybricks/ex05_smooth_follow_stop.py)＋[函式庫](/examples/line8/spike_beginner/pybricks/MBC_line8_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 5 官方 App 積木](/images/sensors/line8/spike-beginner/ex05-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 5 MBC 積木網站積木](/images/sensors/line8/spike-beginner/ex05-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 循行者 8 路 範例 5：平順循線，遇到十字路口就停車
# 接線：循行者 8 路 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_line8_obj_Lib import MBC_LINE8

hub = PrimeHub()
line8 = MBC_LINE8(3)                                   # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    if line8.width() >= 6:        # 6 路以上同時踩到黑線 → 十字路口
        robot.stop()
        hub.speaker.beep(880, 500)
        break                     # 跳出迴圈，程式結束
    pos = line8.pos100()          # 更精細的位置：-100 ~ 100
    robot.drive(120, pos * 1.5)   # 偏多少就轉多少：偏越多轉越快
    wait(10)
```

</details>

**一步一步看懂程式**
1. 讀**細的線位置**（-100 ～ 100）：官方 App 是「raw red（原始紅色）減 100」。
2. 讀**線寬**。
3. **如果 線寬 ≥ 6**（官方 App 寫成「線寬 > 5」，意思一樣）→ 十字路口：**停車 → 嗶一聲 → 結束程式**。
4. **否則** → 轉彎的力道＝**線位置 × 一個數字**（官方 App 乘 0.5，另外兩種乘 1.5）。偏得越多轉得越大，偏一點點就只轉一點點，所以不會左右亂晃。

**跑起來應該看到什麼**：小車順順地沿線走，到十字路口停下來嗶一聲。

**常見問題**
- **還是左右搖晃？** → 把「乘的數字」調小一點（例如 0.5 → 0.3）。
- **轉彎跟不上、衝出線外？** → 「乘的數字」調大一點，或把速度調低。
- **還沒到路口就停了？** → 線太粗或感應器離地太近，把「6」改成 7。
- **官方 App 找不到「raw red」積木？** → 開檔後會自動加上「更多感應器（More Sensors）」積木區；如果沒有，點左下角積木區的「＋」加入「More Sensors」。

---

## 下一步

- 想更精準的循線：[SPIKE 循線 PID 教學](/articles/spike-pid-line-following.md)
- 全部數值的完整說明：[官方 App 教育版](/sensors/line8/spike-education.md)、[Pybricks](/sensors/line8/spike-pybricks.md)
