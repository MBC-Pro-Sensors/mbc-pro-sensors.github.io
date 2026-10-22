<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 scratchpad/products/tof2 產生；程式碼區塊來自 examples/tof2/spike_beginner/pybricks/*.py，改程式請改產生器再重建 -->
# 🧱 SPIKE 新手範例：測距者 2 路 (TOF2)

[🔙 回到測距者 2 路產品頁](/sensors/tof2/index.md)

這一頁是寫給**完全沒寫過程式**的人。5 個範例從「看到距離的數字」開始，一步一步做到「機器人自己開到牆前、把車頭轉正」。每個範例都有 3 種寫法，挑你手上有的那一種就好：

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟨 **樂高官方 SPIKE App**（積木） | 學校、第一次用 SPIKE 的人 | 下載 `.llsp3` → 打開 SPIKE App → 左上「檔案」→「開啟」→ 選檔案 |
| 🟦 **MBC 積木網站**（中文積木） | 想用中文積木、要讀更多感應器資料的人 | 打開 [MBC SPIKE 積木網站](https://mbc-spike-blocks.mbcrobot.workers.dev) → 上方選單「檔案 → 開啟」（或按 Ctrl+O）→ 選下載的 `.abp` |
| 🐍 **Pybricks**（Python 文字程式） | 想學打字寫程式的人 | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊檔案區的「Import a file（匯入檔案）」→ 範例檔和 `MBC_laser2_obj_Lib.py` **兩個都要匯入** → 點開範例 → 按 ▶ 執行 |

> [!IMPORTANT]
> - **MBC 積木網站與 Pybricks 要先把主機換成 Pybricks 韌體**（MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」就能燒，約 3 分鐘；之後也能用 SPIKE App 換回樂高原廠韌體）。樂高官方 App 不用換。
> - 樂高官方 App 這裡用的是**教育版（SPIKE 3）**。

> [!NOTE]
> 最後的**範例 6** 是進階加碼，**只有 Pybricks 版**：它用的數值放在感應器 2026-10 以後韌體的新模式裡，樂高官方 App 和 MBC 積木網站的積木讀不到。

## 開始之前：認識測距者 2 路

測距者 2 路前面有**兩顆並排、都朝正前方**的雷射，分別量「左邊那顆」和「右邊那顆」前面的東西有多遠。不用校準，插上去就能用；開機後等 1～2 秒讓雷射準備好。

**這頁會用到的數值**

| 名稱 | 範圍 | 意思 |
|---|---|---|
| **左距離**、**右距離** | 0 ～ 120 公分 | 左邊、右邊那顆雷射量到的距離。**120＝前面沒東西**（或超過 120 公分） |
| **較近的距離** | 0 ～ 120 公分 | 左右兩個裡面比較小的那個，用來判斷「前面有沒有東西靠近」 |
| **平均距離** | 0 ～ 120 公分 | （左＋右）÷ 2 |
| **左減右** | -120 ～ 120 | 左距離 減 右距離。**負的＝左邊比較近**，正的＝右邊比較近，0＝一樣近 |

> [!TIP]
> **讀到 120 有 4 種可能**：前面真的沒東西、東西超過 120 公分、剛開機雷射還沒準備好、那顆雷射壞了。黑色、亮面、透明（玻璃、壓克力）的東西比較難量，**黑色物體大概 30～50 公分外就量不到**，所以範例的判斷距離都設在 60 公分以內。

**官方 App 怎麼讀？** 在樂高官方 App 裡，測距者會被當成「顏色感應器」。我們用「更多感應器（More Sensors）」裡的**原始紅／綠／藍**積木來讀，讀到的數字要 **÷ 256 再取整數（無條件捨去）**：

| 官方積木 | 讀到的是 | 換算 |
|---|---|---|
| **raw red（原始紅色）** | 左距離 × 256 | ÷ 256 取整數（`floor` 積木）＝ **左距離** |
| **raw green（原始綠色）** | 右距離 × 256 | ÷ 256 取整數 ＝ **右距離** |
| **raw blue（原始藍色）** | 平均距離 × 256 | ÷ 256 取整數 ＝ **平均距離** |

> [!WARNING]
> **官方 App 的讀法是依照感應器韌體推算的，還沒有用實機逐一確認。** 如果範例 1 在官方 App 顯示的數字怪怪的（例如一直是 0、或是好幾千），請先改用 MBC 積木網站或 Pybricks，並跟我們回報。
> 另外，「color（顏色）」積木讀到的也是左距離、「reflected light（反射光）」積木讀到的是右距離，但 App 可能只認得 0～10（顏色）、0～100（反射光）的數字，超過的會不會被截掉還沒確認，所以這頁的範例**不用這兩個積木**。

---

## 範例 1：看見距離

**這個範例會做什麼**：主機螢幕一直捲動顯示「左距離 右距離」兩個數字（單位公分），例如「40 80」。

**要準備什麼**
- SPIKE 主機 1 台
- 測距者 2 路 1 個
- 一本書或一個盒子當作「障礙物」

**怎麼接**：測距者 2 路插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex01_show_distance.llsp3](/examples/tof2/spike_beginner/official/ex01_show_distance.llsp3) | [ex01_show_distance.abp](/examples/tof2/spike_beginner/mbcblocks/ex01_show_distance.abp) | [ex01_show_distance.py](/examples/tof2/spike_beginner/pybricks/ex01_show_distance.py)＋[函式庫](/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 1 官方 App 積木](/images/sensors/tof2/spike-beginner/ex01-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 1 MBC 積木網站積木](/images/sensors/tof2/spike-beginner/ex01-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 2 路 範例 1：看見距離（螢幕捲動顯示「左距離 右距離」）
# 接線：測距者 2 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)              # 3 = C 埠（1=A、2=B、3=C、4=D、5=E、6=F）

while True:
    left = laser2.left()            # 左邊雷射量到的距離（公分，0~120；120 = 前面沒東西）
    right = laser2.right()          # 右邊雷射量到的距離
    hub.display.text(str(left) + ' ' + str(right))   # 主機螢幕捲動顯示，例如「40 80」
    print('左', left, '  右', right)                  # 電腦畫面也印出來
    wait(200)                       # 等 0.2 秒再讀下一次
```

</details>

**一步一步看懂程式**
1. **一直重複**：下面的積木會一直做，直到你按停止。
2. **讀左距離、右距離**：官方 App 是「原始紅 ÷ 256 取整數」和「原始綠 ÷ 256 取整數」，存進兩個「變數」（變數＝有名字的盒子，可以放一個數字）；MBC 積木網站和 Pybricks 直接有「左雷射距離」「右雷射距離」。
3. **顯示在螢幕上**：把「左距離」「空格」「右距離」接成一串文字，讓螢幕捲動顯示。MBC 積木網站和 Pybricks 也會在電腦畫面印出來。
4. **等 0.2 秒**再讀下一次。

**跑起來應該看到什麼**
- 前面什麼都沒有：顯示 **120 120**。
- 把書放在前面 30 公分：兩個數字都變成 30 左右。
- **用手只擋住左邊那顆雷射**：第一個數字變小、第二個不變。

> [!IMPORTANT]
> **請先用這個範例確認左右。** 感應器正面朝前、元件面朝上的時候，用手只擋住你看到的「左邊那顆」：應該是**第一個數字**變小。如果是第二個數字變小，代表你的裝法跟範例相反（例如感應器上下顛倒裝），後面範例的「左」「右」也要跟著對調。

**常見問題**
- **一直顯示 120 120？** → 前面真的沒東西，或東西太遠、是黑色／透明的。拿一本白色的書放在 20 公分前試試。剛開機的 1～2 秒也會是 120。
- **只有一邊一直是 120？** → 那顆雷射可能被擋住、髒了或壞了。用 MBC 積木網站的「雷射正常」積木或 Pybricks 的 `laser2.is_ok(1)` 可以檢查。
- **數字一直跳？** → 正常，雷射每秒量十幾次，會有 1～2 公分的誤差。
- **官方 App 顯示好幾千的數字？** → 忘了「÷ 256 取整數」。

---

## 範例 2：太近就嗶

**這個範例會做什麼**：左右任何一邊有東西**小於 20 公分**，主機燈變**紅色**並且嗶嗶叫；沒有就亮**綠色**。

**要準備什麼**：同範例 1。

**怎麼接**：測距者 2 路插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex02_too_close_beep.llsp3](/examples/tof2/spike_beginner/official/ex02_too_close_beep.llsp3) | [ex02_too_close_beep.abp](/examples/tof2/spike_beginner/mbcblocks/ex02_too_close_beep.abp) | [ex02_too_close_beep.py](/examples/tof2/spike_beginner/pybricks/ex02_too_close_beep.py)＋[函式庫](/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 2 官方 App 積木](/images/sensors/tof2/spike-beginner/ex02-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 2 MBC 積木網站積木](/images/sensors/tof2/spike-beginner/ex02-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 2 路 範例 2：太近就嗶（任何一邊小於 20 公分：紅燈＋嗶聲；否則綠燈）
# 接線：測距者 2 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                  # 3 = C 埠

while True:
    if laser2.min_distance() < 20:      # 左右兩邊比較近的那個距離，小於 20 公分
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)      # 頻率 880 Hz，響 0.1 秒
    else:
        hub.light.on(Color.GREEN)
    wait(100)
```

</details>

**一步一步看懂程式**
1. **讀距離**：官方 App 讀左、右兩個距離；MBC 積木網站和 Pybricks 直接讀「較近的距離」（左右比較小的那個）。
2. **如果 太近就…**：官方 App 寫成「左 < 20 **或** 右 < 20」，意思跟「較近的距離 < 20」一樣 → 亮紅燈、嗶一聲。
3. **否則…**：亮綠燈。
4. **等 0.1 秒**再檢查一次。

**跑起來應該看到什麼**
- 前面沒東西：綠燈。
- 手慢慢靠近，到 20 公分以內：紅燈＋一直嗶嗶叫。手拿開又變回綠燈。

**常見問題**
- **手貼到感應器上反而不叫？** → 貼太近（幾公分以內）雷射可能量不到，會讀成 120。這是雷射的特性，留一點距離就好。
- **想改成 30 公分就叫？** → 把「20」改成「30」（官方 App 有兩個 20 都要改）。

---

## 範例 3：哪一邊比較近

**這個範例會做什麼**：主機螢幕用**箭頭**告訴你東西在哪邊：左邊比較近 → ⬅，右邊比較近 → ➡，兩邊差不多 → ⬆，什麼都沒有 → ✕。

**要準備什麼**：同範例 1。

**怎麼接**：測距者 2 路插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex03_which_closer.llsp3](/examples/tof2/spike_beginner/official/ex03_which_closer.llsp3) | [ex03_which_closer.abp](/examples/tof2/spike_beginner/mbcblocks/ex03_which_closer.abp) | [ex03_which_closer.py](/examples/tof2/spike_beginner/pybricks/ex03_which_closer.py)＋[函式庫](/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 3 官方 App 積木](/images/sensors/tof2/spike-beginner/ex03-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 3 MBC 積木網站積木](/images/sensors/tof2/spike-beginner/ex03-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 2 路 範例 3：哪一邊比較近（主機螢幕顯示箭頭）
# 接線：測距者 2 路 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                      # 3 = C 埠

while True:
    diff = laser2.diff()                    # 左距離 減 右距離：負的 = 左邊比較近，正的 = 右邊比較近
    if laser2.min_distance() >= 120:        # 兩邊都是 120 → 前面什麼都沒有
        hub.display.icon(Icon.FALSE)        # 顯示 ✕
    elif diff < -5:                         # 左邊近了 5 公分以上
        hub.display.icon(Icon.ARROW_LEFT)
    elif diff > 5:                          # 右邊近了 5 公分以上
        hub.display.icon(Icon.ARROW_RIGHT)
    else:                                   # 兩邊差不多 → 東西在正前方
        hub.display.icon(Icon.ARROW_UP)
    wait(100)
```

</details>

**一步一步看懂程式**
1. 先算 **左減右**＝左距離 減 右距離。左邊比較近時，左距離比較小，所以結果是**負的**。
2. **如果 兩邊都是 120** → 顯示 ✕（前面沒東西，要最先檢查）。官方 App 寫成「左 > 119 而且 右 > 119」。
3. **否則，如果 左減右 < -5** → 左邊近了 5 公分以上 → 左箭頭。
4. **否則，如果 左減右 > 5** → 右邊近了 5 公分以上 → 右箭頭。
5. **都不是**（差不到 5 公分）→ 上箭頭，代表東西在正前方。

**跑起來應該看到什麼**：拿一本書從左邊慢慢移到右邊（距離 30 公分左右），箭頭依序變成 ⬅ → ⬆ → ➡；把書拿走，變成 ✕。

**常見問題**
- **為什麼要差 5 公分以上才算？** → 雷射本身有 1～2 公分的誤差，留一點空間，箭頭才不會一直跳來跳去。
- **箭頭左右相反？** → 回到範例 1 確認左右，感應器可能裝反了。
- **東西很遠時箭頭亂跳？** → 兩顆雷射的光束會越遠越寬、在遠處重疊，所以 50 公分以外很難分左右。這個範例請在 50 公分內玩。

---

## 範例 4：前面有東西就後退轉彎

**這個範例會做什麼**：機器人一直往前走。前面有東西（小於 20 公分）就**停車 → 嗶一聲 → 後退 10 公分 → 右轉 90 度**，然後繼續走。

**要準備什麼**
- 範例 1 的東西，再加上
- 兩輪小車（兩顆馬達當輪子）
- 一塊空地，放幾個盒子當障礙物

**怎麼接**
- 測距者 2 路插 **C 孔**，裝在**車頭、朝正前方**，離地至少 5 公分（太低會量到地板）
- **左輪接 E、右輪接 F**

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex04_obstacle_back.llsp3](/examples/tof2/spike_beginner/official/ex04_obstacle_back.llsp3) | [ex04_obstacle_back.abp](/examples/tof2/spike_beginner/mbcblocks/ex04_obstacle_back.abp) | [ex04_obstacle_back.py](/examples/tof2/spike_beginner/pybricks/ex04_obstacle_back.py)＋[函式庫](/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 4 官方 App 積木](/images/sensors/tof2/spike-beginner/ex04-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 4 MBC 積木網站積木](/images/sensors/tof2/spike-beginner/ex04-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 2 路 範例 4：一直往前走，前面有東西（小於 20 公分）就停車、嗶一聲、後退、右轉
# 接線：測距者 2 路 → C 埠（裝在車頭、朝前）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    if laser2.min_distance() < 20:    # 左右任何一邊小於 20 公分 → 前面有東西
        robot.stop()                  # 先停車
        hub.speaker.beep(880, 200)
        robot.straight(-100)          # 後退 100 mm（10 公分）
        robot.turn(90)                # 原地右轉 90 度
    else:
        robot.drive(150, 0)           # 每秒 150 mm 直直往前走
    wait(50)
```

</details>

**一步一步看懂程式**
1. **告訴主機輪子接在哪裡**：官方 App 是「set movement motors to E+F」；MBC 積木網站和 Pybricks 是「移動底盤 左輪 E、右輪 F」。
2. **設定速度**：官方 App 速度 30%；另外兩種是每秒 150 mm。
3. **一直重複**：讀距離。
4. **如果 較近的距離 < 20** → 停車 → 嗶 → 後退 10 公分 → 原地右轉。官方 App 的「右轉」是「以轉向 100 移動 0.5 圈」，樂高標準小車大約轉 90 度。
5. **否則** → 往前走。

**跑起來應該看到什麼**：小車往前走，快撞到盒子時停下來、嗶一聲、退一點點、右轉，再繼續往前。

**常見問題**
- **撞上去才停？** → 開太快了，或障礙物是黑色、透明的（量不到）。把速度調低，或把「20」改大一點（例如 25）。
- **轉的角度不到 90 度／轉太多？** → 官方 App：改「0.5 圈」那個數字；MBC 積木網站和 Pybricks：改「90」。
- **小車往後退、方向相反？** → 馬達的裝法跟樂高標準小車相反。官方 App：請照樂高「標準驅動底盤」的方向裝馬達；MBC 積木網站：「移動底盤」積木裡左右輪的「正轉」方向都換成另一個；Pybricks：左輪改成 `Direction.CLOCKWISE`、右輪改成 `Motor(Port.F, Direction.COUNTERCLOCKWISE)`。
- **一直原地打轉不前進？** → 感應器裝太低，量到地板了。把感應器墊高一點、稍微朝上。

---

## 範例 5：開到牆前並把車頭轉正

**這個範例會做什麼**：機器人往前開到**離牆 20 公分**停下，再**原地慢慢轉**，直到左右兩顆雷射量到的距離一樣（差 1 公分以內），代表車頭正對著牆。完成後嗶一聲、螢幕顯示笑臉。比賽時常用這招讓機器人「對正」。

**要準備什麼**：同範例 4，再加一面牆（或一個大紙箱的平面）。

**怎麼接**：測距者 2 路插 **C 孔**、裝在車頭朝前；**左輪接 E、右輪接 F**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex05_square_up.llsp3](/examples/tof2/spike_beginner/official/ex05_square_up.llsp3) | [ex05_square_up.abp](/examples/tof2/spike_beginner/mbcblocks/ex05_square_up.abp) | [ex05_square_up.py](/examples/tof2/spike_beginner/pybricks/ex05_square_up.py)＋[函式庫](/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 5 官方 App 積木](/images/sensors/tof2/spike-beginner/ex05-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 5 MBC 積木網站積木](/images/sensors/tof2/spike-beginner/ex05-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 2 路 範例 5：往前開到離牆 20 公分停下，再原地轉到兩邊距離一樣（車頭對正牆面）
# 接線：測距者 2 路 → C 埠（裝在車頭、朝前）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Icon, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

# 第 1 步：往前開，直到左右平均距離 ≤ 20 公分
robot.drive(100, 0)
while laser2.average() > 20:
    wait(20)
robot.stop()

# 第 2 步：原地轉，直到左右只差 1 公分以內
while abs(laser2.diff()) > 1:          # diff = 左 減 右
    if laser2.diff() < 0:              # 左邊比較近 → 車頭偏右了 → 往左轉
        robot.drive(0, -30)            # 速度 0（不前進），每秒左轉 30 度
    else:                              # 右邊比較近 → 往右轉
        robot.drive(0, 30)
    wait(20)
robot.stop()

# 第 3 步：完成！
hub.speaker.beep(880, 500)
hub.display.icon(Icon.HAPPY)
```

</details>

**一步一步看懂程式**（這個範例**不是**「一直重複」，而是照順序做完 3 步就結束）
1. **第 1 步：往前開**，「等待直到」平均距離 ≤ 20 公分，就停車。官方 App 的平均距離是「原始藍 ÷ 256 取整數」，寫成「< 21」意思一樣。
2. **第 2 步：轉正**。「重複直到」左右只差 1 公分以內：
   - 左減右 < 0（左邊比較近）→ 車頭偏右了 → **往左轉**；
   - 否則 → **往右轉**。
   - 官方 App 一開始先把「左減右」設成 99，這樣「重複直到」才會至少檢查一次。
3. **第 3 步：完成**：停車、嗶一聲、顯示笑臉。

**跑起來應該看到什麼**：把小車斜斜地對著牆放好（車頭偏一點點），按執行。小車往前開、停下，左右扭一下轉正，然後嗶一聲。

**常見問題**
- **一直左右轉、停不下來？** → 牆面不平，或「差 1 公分」太嚴格。把「1」改成「2」（官方 App 是把「< 2」改成「< 3」）。
- **轉正的方向反了，越轉越歪？** → 回到範例 1 確認左右；左右相反的話，把「往左轉」「往右轉」對調。
- **斜太多（超過 30 度）就轉不正？** → 斜太多時，其中一顆雷射會打到牆外面去（讀到 120）。先把車大致擺正再開始。
- **牆是黑色的？** → 黑色牆面雷射量得比較短、比較不準，換成淺色的牆或在牆上貼白紙。

---

## 範例 6（進階）：用毫米精準停車

> [!NOTE]
> **這個範例只有 Pybricks 版。** 它用的「毫米距離」放在感應器的模式 6，樂高官方 App 和 MBC 積木網站讀不到（它們只讀公分）。感應器韌體要是 **2026-10 以後**的版本，舊韌體讀到的會是別的數字。 程式碼就是範例，不需要截圖。

**這個範例會做什麼**：機器人往前開，離牆 **20 公分**時放慢，到 **15 公分（150 mm）** 就停，嗶一聲、螢幕顯示停下來時的距離（公分）。公分只能分到 1 公分，毫米可以分到 0.1 公分，所以停的位置更準。

**要準備什麼**：同範例 5（小車＋一面牆）。

**怎麼接**：測距者 2 路插 **C 孔**、裝在車頭朝前；**左輪接 E、右輪接 F**。

**下載程式**

| Pybricks |
|---|
| [ex06_park_mm.py](/examples/tof2/spike_beginner/pybricks/ex06_park_mm.py)＋[函式庫](/examples/tof2/spike_beginner/pybricks/MBC_laser2_obj_Lib.py) |

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 測距者 2 路 範例 6（進階）：用毫米精準停車（離牆 20 公分放慢、15 公分停）
# 接線：測距者 2 路 → C 埠（裝在車頭朝前）；左輪馬達 → E 埠；右輪馬達 → F 埠
# 只有 Pybricks 版：毫米距離放在感應器的「模式 6」，官方 App 和積木網站讀不到；感應器韌體要 2026-10 以後
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_laser2_obj_Lib import MBC_LASER2

hub = PrimeHub()
laser2 = MBC_LASER2(3)                                 # 3 = C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

robot.drive(100, 0)                                    # 每秒 100 mm 往前開
while laser2.average_mm() > 200:                       # 左右平均距離（毫米）還大於 200 mm = 20 公分
    wait(10)
robot.drive(40, 0)                                     # 剩 20 公分：放慢到每秒 40 mm
while laser2.average_mm() > 150:                       # 到 150 mm = 15 公分
    wait(10)
robot.stop()                                           # 停車
hub.speaker.beep()
mm = laser2.average_mm()                               # 停下來之後再量一次
hub.display.number(mm // 10)                           # 螢幕顯示公分（15 或 14）
print('停在', mm, 'mm')
```

</details>

**一步一步看懂程式**
1. `laser2.average_mm()`：左右平均距離，單位**毫米**（0～1200；1200＝前面沒東西）。感應器自己算好平均，不用自己加。
2. 先用每秒 100 mm 往前開，**等到**平均距離 ≤ 200 mm（20 公分）。
3. 換成每秒 40 mm 慢慢開，**等到** ≤ 150 mm 就停車。
4. 嗶一聲，把停下來的距離 ÷ 10 顯示在螢幕上（公分）。
5. 函式庫還有 `laser2.left_mm()`、`laser2.right_mm()`（左、右各自的毫米）和 `laser2.device_id()`、`laser2.firmware_version()`（感應器型號與韌體版本）。

**跑起來應該看到什麼**：小車往前開、接近牆時變慢、在離牆 15 公分左右停下來，嗶一聲、螢幕顯示 15 或 14。

**常見問題**
- **停的位置每次都差 1～2 公分？** → 雷射本身有 1～2 公分的誤差，而且車子有慣性。把「40」改更小（例如 25），停得更準。
- **想同時用公分和毫米？** → 可以，但不要在同一個迴圈裡混著用：每次切換模式感應器要多花幾十 ms。
- **數字一直是 1200 不動？** → 前面真的沒東西；或感應器韌體太舊（2026-10 以前）沒有毫米模式，請更新韌體。

---

## 速度單位說明

- 官方 App 的速度是**百分比（%）**：30% 大約是馬達最快速度的三成。
- MBC 積木網站和 Pybricks 的速度是**每秒幾 mm**，轉彎是**每秒幾度**。
- 所以三種寫法的數字不一樣，但動作的意思相同。

## 下一步

- 產品介紹、規格與其他平台範例：[測距者 2 路產品頁](/sensors/tof2/index.md)
- 想要看到左右 180 度、做相撲或全方位避障：[測距者 8 路](/sensors/tof8/index.md)、[測距者 8 路 SPIKE 新手範例](/sensors/tof8/spike-examples.md)
- Pybricks 函式庫的全部功能：打開 `MBC_laser2_obj_Lib.py`，最上面有中文說明（例如 `laser2.is_ok(1)` 檢查左邊雷射是不是正常）。
