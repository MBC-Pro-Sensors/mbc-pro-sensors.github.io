<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 scratchpad/products/esp32cam 產生；程式碼區塊來自 examples/esp32cam/spike_beginner/pybricks/*.py，改程式請改產生器再重建 -->
# 🧱 SPIKE 新手範例：神攝手 ESP32CAM（學校老師創意賽版）

[🔙 回到神攝手產品頁](/sensors/esp32cam/index.md)　｜　手上是 **WRO 版（HUE12）**？請看 [WRO 版新手範例](/sensors/esp32cam/spike-examples-wro.md)

這一頁是寫給**完全沒寫過程式**的人。5 個範例從「看到顏色的數字」開始，一步一步做到「機器人自己轉頭對準色塊」、「有人經過就警報」。每個範例都有 3 種寫法，挑你手上有的那一種就好：

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟨 **樂高官方 SPIKE App**（積木） | 學校、第一次用 SPIKE 的人 | 下載 `.llsp3` → 打開 SPIKE App → 左上「檔案」→「開啟」→ 選檔案 |
| 🟦 **MBC 積木網站**（中文積木） | 想用中文積木、要讀更多感應器資料的人 | 打開 [MBC SPIKE 積木網站](https://mbc-spike-blocks.mbcrobot.workers.dev) → 上方選單「檔案 → 開啟」（或按 Ctrl+O）→ 選下載的 `.abp` |
| 🐍 **Pybricks**（Python 文字程式） | 想學打字寫程式的人 | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊檔案區的「Import a file（匯入檔案）」→ 範例檔和 `MBC_cam_school_Lib.py` **兩個都要匯入** → 點開範例 → 按 ▶ 執行 |

> [!IMPORTANT]
> - **MBC 積木網站與 Pybricks 要先把主機換成 Pybricks 韌體**（MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」就能燒，約 3 分鐘；之後也能用 SPIKE App 換回樂高原廠韌體）。樂高官方 App 不用換。
> - 樂高官方 App 這裡用的是**教育版（SPIKE 3）**。家用版（Robot Inventor）的「顏色」只有 **0～10**（是教育版的十分之一），範例 2 的數字要自己改。
> - 本頁的數字是**目前出貨的韌體**（色相 0～100）。怎麼確認：開機後看感應器螢幕上方 `X` 的數字，**最大只到 98 左右**就是新版；如果會跑到 100 以上（最多 159），是舊版韌體，請先聯絡我們更新。

## 開始之前：在感應器上設定要看的顏色

神攝手要先「告訴它」要看哪個顏色，**只能用感應器機身上的按鍵設定**（程式不能改）。設定會記在感應器裡，重新開機也還在。

**先做這 3 件事**
1. **先把感應器插上主機、主機開機**。感應器螢幕停在「LEGO Sync… / connect…」是在等主機，主機開機後才會出現畫面（不接主機不能設定）。
2. **把要用的顏色積木先擺在鏡頭前**——進入設定時畫面會**定格**，要先擺好再按。
3. 按鍵：機身有 **左、確認（中）、右** 三顆，加上一顆**側鍵**。側鍵短按＝進選單／回上一層。

**設定 1：COLOR（看顏色，給「色相」用，範例 1、2）**
1. 畫面上方顯示 `RUN` 時，**短按側鍵** → 上方出現 `SET 1:COLOR`。
2. 按 **確認** → `SET X`：畫面定格，用 **左／右** 把紅色直線移到積木上。
3. 按 **確認** → `SET Y`：用 **左／右** 把綠色橫線移到積木上。
4. 按 **確認** → `TOL_HUE`（顏色容許範圍，先用預設 15）→ 按 **確認** → `TOL_STA`（先用預設）→ 再按 **確認** 就存好、回到 `RUN`。

**設定 2：TRACK（追色塊，給「色塊 X／Y／面積」用，範例 3、4）**
1. `RUN` 畫面**短按側鍵** → 按 **右** 切到 `SET 2:TRACK` → 按 **確認**。
2. `SET X`、`SET Y`：十字中心要**對準色塊的正中間**（它只取十字中心那一點的顏色）。
3. `TOL_HUE`、`TOL_STA`：畫面上跟目標同色的地方會被標出來，調到**只有你的積木被標記**。
4. `MIN AREA`（最小色塊）：背景有小雜點被框起來就調大一點 → 按 **確認** 存好，畫面上會出現追蹤框。

> [!WARNING]
> **改過鏡頭設定後，畫面上下顛倒怎麼辦？** 目前韌體有一個已知問題：在 `SET MODE CAM`（鏡頭選單）改任何一項（包括開補光燈 `Flash LED`），畫面就會**上下顛倒**，色塊的 Y 也會跟著反過來。解法：在同一個鏡頭選單把 **`V-Flip` 切成 ON**，畫面就正了，而且會記住。

**補光燈要不要開？** 先**不要開**。ESP32-CAM 的補光燈很亮，樂高積木表面會反光變白、顏色反而認不出來；場地很暗才開（開完記得看上面的 V-Flip）。

**怎麼知道設定好了？** 回到 `RUN` 畫面，螢幕上方會一直顯示 `H…（色相） A…（面積） X… Y… M…（動態）`。範例程式讀到的就是這幾個數字，先看螢幕上的數字，再寫進程式裡最準。

## 這頁會用到的數值

| 名稱 | 範圍 | 意思 |
|---|---|---|
| **色相**（顏色） | -1、0 ～ 100 | 紅 0（100 也是紅）、黃約 17、綠約 33、青約 50、藍約 67、洋紅約 83。**-1＝黑、白、灰（沒有顏色）** |
| **色塊 X** | 0 ～ 98 | 色塊在畫面的左右位置：**左 0、右 98，正中間約 50** |
| **色塊 Y** | 1 ～ 75 | 上下位置：**下面小、上面大**（下 1、上 75），中間約 37 |
| **色塊面積** | 0 ～ 100 | 色塊多大。**0＝沒找到色塊** |
| **色塊寬** | 0 ～ 98 | 色塊多寬。**0＝沒找到色塊**（MBC 積木網站與 Pybricks 用這個判斷） |
| **動態值** | 0 ～ 100 | 整個畫面跟上一張比變了多少；有東西在動，數字就變大 |

> [!TIP]
> - **沒做「設定 1：COLOR」時，色相一直是 0——跟紅色一樣！** 程式以為看到紅色，其實是還沒設定。
> - 沒找到色塊時 X、Y 都是 0，看起來像「在左下角」。所以一定**先判斷有沒有找到**（面積或寬是不是 0），再看 X。

在樂高官方 App 裡，神攝手會被當成「顏色感應器」，所以要用顏色感應器的積木來讀：

| 官方積木 | 讀到的是 |
|---|---|
| **color（顏色）** | 色相（-1、0～100），直接用數字比較 |
| **reflected light（反射光）** | 動態值（0～100） |
| **raw red（原始紅色，在「更多感應器」）** | 色塊 X |
| **raw green（原始綠色）** | 色塊 Y |
| **raw blue（原始藍色）** | 色塊面積（0＝沒找到） |

> [!CAUTION]
> 官方 App 裡**不要用「顏色是 [紅色]？」這種選顏色的積木**，它比對的是樂高自己的色號，不是神攝手的色相。一律把「顏色」放進變數，用數字比大小。

MBC 積木網站和 Pybricks 讀的資料（積木網站的「神攝手學校版」積木、Pybricks 的 `MBC_cam_school_Lib.py`）比官方 App 多了**飽和度、明度、色塊寬、色塊高**，但**沒有面積**，所以改用「色塊寬 > 0」當作「有找到」。

---

## 範例 1：看到什麼顏色

**這個範例會做什麼**：主機螢幕一直顯示神攝手看到的「色相」數字。把不同顏色的積木放到「設定 1」的那個位置，數字就會變。

**要準備什麼**
- SPIKE 主機 1 台
- 神攝手 1 個（已做「設定 1：COLOR」）
- 紅、綠、藍、白色的積木各一塊

**怎麼接**：神攝手插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex01_show_hue.llsp3](/examples/esp32cam/spike_beginner/official/ex01_show_hue.llsp3) | [ex01_show_hue.abp](/examples/esp32cam/spike_beginner/mbcblocks/ex01_show_hue.abp) | [ex01_show_hue.py](/examples/esp32cam/spike_beginner/pybricks/ex01_show_hue.py)＋[函式庫](/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 1 官方 App 積木](/images/sensors/esp32cam/spike-beginner/ex01-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 1 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex01-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手（學校版）範例 1：看到什麼顏色（主機螢幕顯示色相）
# 接線：神攝手 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)      # 神攝手插在 C 孔

while True:
    hue = cam.hue()               # 色相：0~100（紅 0／100、綠約 33、藍約 67）；-1 = 黑白灰
    print(hue)                    # 電腦畫面印出來（比較快看到）
    hub.display.text(str(hue))    # 主機螢幕一個字一個字顯示
    wait(500)                     # 等 0.5 秒再讀下一次
```

</details>

**一步一步看懂程式**
1. **一直重複**：下面的積木會一直做，直到你按停止。
2. **讀色相**：官方 App 用「color（顏色）」積木，把它放進變數「色相」（變數＝有名字的盒子，可以放一個數字）；MBC 積木網站用「神攝手學校版 C 色相」；Pybricks 用 `cam.hue()`。
3. **顯示在螢幕上**：數字會一個字一個字跑出來。MBC 積木網站和 Pybricks 也會印在電腦畫面上，看得比較快。
4. **等 0.5 秒**再讀下一次。

**跑起來應該看到什麼**
- 紅色積木：**0～5** 或 **95～100** 左右。
- 綠色積木：**30** 左右；藍色積木：**65** 左右（每個場地的光線不同，數字會差一點）。
- 白紙、黑色積木：**-1**（沒有顏色）。

**常見問題**
- **不管放什麼都是 0？** → 還沒做「設定 1：COLOR」。
- **數字一直跳來跳去？** → 教室的燈會閃爍，顏色淡的東西特別容易跳；換顏色鮮豔的積木，或靠近一點。
- **換了教室，數字變了？** → 光線不同，重新做一次「設定 1」。

---

## 範例 2：看到紅色就嗶

**這個範例會做什麼**：神攝手看到紅色，主機中間的燈亮**紅色**並嗶一聲；看到其他顏色，燈就關掉。

**要準備什麼**：同範例 1。

**怎麼接**：神攝手插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex02_red_beep.llsp3](/examples/esp32cam/spike_beginner/official/ex02_red_beep.llsp3) | [ex02_red_beep.abp](/examples/esp32cam/spike_beginner/mbcblocks/ex02_red_beep.abp) | [ex02_red_beep.py](/examples/esp32cam/spike_beginner/pybricks/ex02_red_beep.py)＋[函式庫](/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 2 官方 App 積木](/images/sensors/esp32cam/spike-beginner/ex02-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 2 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex02-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手（學校版）範例 2：看到紅色就亮紅燈、嗶一聲
# 接線：神攝手 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)

while True:
    hue = cam.hue()
    if 0 <= hue <= 5 or hue >= 95:    # 紅色在色相的兩端：0 附近和 100 附近都是紅
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)    # 頻率 880 Hz，響 0.1 秒
    else:                             # 其他顏色，或 -1（黑白灰）
        hub.light.off()
    wait(100)
```

</details>

**一步一步看懂程式**
1. **讀色相**，放進變數。
2. **如果 色相在 0～5 之間，或 色相 ≥ 95**（官方 App 寫成「> 94」，意思一樣）→ 亮紅燈、嗶一聲。紅色剛好在色相的頭和尾，所以兩邊都要檢查。
3. **否則**（其他顏色，或 -1 沒有顏色）→ 關燈。
4. **等 0.1 秒**再檢查一次。

**跑起來應該看到什麼**：紅色積木放到設定的位置 → 紅燈＋一直嗶；換成綠色或白紙 → 燈熄掉、不叫。

**常見問題**
- **什麼都沒放也一直嗶？** → 色相是 0，代表還沒做「設定 1：COLOR」（沒設定時色相是 0，跟紅色一樣）。
- **橘色也會嗶？** → 橘色的色相跟紅色很近（約 4～10），把「5」改小一點，例如 3。
- **我的紅色積木是 7，沒有嗶？** → 先用範例 1 看你的紅色是多少，再把「5」改成那個數字加一點。
- **官方 App 的燈號顏色怎麼選？** → 點「set Power Button light to」積木上的顏色圓點就能換。

---

## 範例 3：色塊在哪一邊

**這個範例會做什麼**：主機螢幕用**箭頭**告訴你色塊在畫面的哪裡：在左 → ⬅，在右 → ➡，在中間 → ⬆，找不到 → ✕。

**要準備什麼**：同範例 1，感應器要先做「**設定 2：TRACK**」。

**怎麼接**：神攝手插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex03_blob_arrow.llsp3](/examples/esp32cam/spike_beginner/official/ex03_blob_arrow.llsp3) | [ex03_blob_arrow.abp](/examples/esp32cam/spike_beginner/mbcblocks/ex03_blob_arrow.abp) | [ex03_blob_arrow.py](/examples/esp32cam/spike_beginner/pybricks/ex03_blob_arrow.py)＋[函式庫](/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 3 官方 App 積木](/images/sensors/esp32cam/spike-beginner/ex03-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 3 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex03-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手（學校版）範例 3：色塊在哪一邊（主機螢幕顯示箭頭）
# 接線：神攝手 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Icon, Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)

while True:
    x = cam.blob_x()                      # 色塊 X：左 0 ~ 右 98，中間約 50
    if not cam.found():                   # 沒找到色塊（這時 X 是 0，不能拿來判斷）
        hub.display.icon(Icon.FALSE)      # 顯示 ✕
    elif x < 35:
        hub.display.icon(Icon.ARROW_LEFT)
    elif x > 65:
        hub.display.icon(Icon.ARROW_RIGHT)
    else:                                 # 35 ~ 65 都算中間
        hub.display.icon(Icon.ARROW_UP)
    wait(100)
```

</details>

**一步一步看懂程式**
1. 讀**色塊 X**，再讀「有沒有找到」：官方 App 看「raw blue（面積）是不是 0」；MBC 積木網站看「色塊寬 = 0」；Pybricks 用 `cam.found()`。
2. **如果 沒找到** → 顯示 ✕（這時 X 是 0，不能拿來判斷，所以要最先檢查）。
3. **否則，如果 X < 35** → 左箭頭。
4. **否則，如果 X > 65** → 右箭頭。
5. **都不是**（35～65）→ 上箭頭，代表在中間。

**跑起來應該看到什麼**：拿著色塊在鏡頭前從左慢慢移到右，箭頭依序變成 ⬅ → ⬆ → ➡；把色塊拿走，變成 ✕。

**常見問題**
- **箭頭左右相反？** → 鏡頭裝的方向跟畫面相反。到鏡頭選單把 `H-Mirror` 切換一次（改完記得檢查 `V-Flip`，見上面的警告）。
- **色塊不在也顯示箭頭？** → 背景有同色的東西被追到了：回「設定 2」把 `TOL_HUE` 調小、`MIN AREA` 調大。
- **為什麼中間是 35～65，不是只有 50？** → 留一段「算中間」的範圍，箭頭才不會一直跳。

---

## 範例 4：轉頭追色塊

**這個範例會做什麼**：機器人原地轉頭，讓色塊保持在鏡頭正中間。色塊在左就往左轉、在右就往右轉、對準了（或看不到）就停。

**要準備什麼**
- 範例 3 的東西，再加上
- 兩輪小車（兩顆馬達當輪子），神攝手裝在車頭、鏡頭朝前
- 一塊做過「設定 2：TRACK」的色塊

**怎麼接**
- 神攝手插 **C 孔**
- **左輪接 E、右輪接 F**

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex04_turn_to_blob.llsp3](/examples/esp32cam/spike_beginner/official/ex04_turn_to_blob.llsp3) | [ex04_turn_to_blob.abp](/examples/esp32cam/spike_beginner/mbcblocks/ex04_turn_to_blob.abp) | [ex04_turn_to_blob.py](/examples/esp32cam/spike_beginner/pybricks/ex04_turn_to_blob.py)＋[函式庫](/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 4 官方 App 積木](/images/sensors/esp32cam/spike-beginner/ex04-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 4 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex04-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手（學校版）範例 4：轉頭追色塊（色塊在左就往左轉、在右就往右轉、在中間就停）
# 接線：神攝手 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

cam = MBC_CAM_SCHOOL(Port.C)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                                  # 右輪
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)   # 輪子直徑 56 mm、兩輪距離 112 mm

while True:
    x = cam.blob_x()
    if not cam.found():           # 沒看到色塊 → 停下來
        robot.stop()
    elif x < 40:                  # 色塊在左邊 → 原地左轉（速度 0，每秒轉 60 度）
        robot.drive(0, -60)
    elif x > 60:                  # 色塊在右邊 → 原地右轉
        robot.drive(0, 60)
    else:                         # 色塊在中間 → 已經對準，停
        robot.stop()
    wait(50)
```

</details>

**一步一步看懂程式**
1. **告訴主機輪子接在哪裡**：官方 App 是「set movement motors to E+F」；MBC 積木網站和 Pybricks 是「移動底盤 左輪 E、右輪 F」。
2. **設定速度**：官方 App 速度 20%；另外兩種是「每秒轉 60 度」。轉太快鏡頭會糊掉，所以慢慢轉。
3. **如果 沒找到色塊** → 停。
4. **否則如果 X < 40**（色塊在左）→ **原地左轉**（官方 App：steer -100）。
5. **否則如果 X > 60**（色塊在右）→ **原地右轉**（官方 App：steer 100）。
6. **否則**（40～60，已經對準）→ 停。

**跑起來應該看到什麼**：拿著色塊在車子前面慢慢往左走，車子會跟著轉頭；停下來，車子也停在正對色塊的方向。

**常見問題**
- **色塊在左，車子卻往右轉、越轉越遠？** → 左右轉反了：把兩個轉彎的數字正負對調（官方 App：-100 和 100；另外兩種：-60 和 60），或照範例 3 的方法切換 `H-Mirror`。
- **一直左右來回抖？** → 轉太快。官方 App 把速度調低（例如 15%）；另外兩種把 60 改成 40。也可以把「中間」的範圍放寬（40／60 改成 35／65）。
- **小車原地轉的方向怪怪的？** → 馬達裝法跟樂高標準小車相反。官方 App：請照樂高「標準驅動底盤」的方向裝馬達；MBC 積木網站：「移動底盤」積木裡左右輪的「正轉」方向都換成另一個；Pybricks：左輪改成 `Direction.CLOCKWISE`、右輪改成 `Motor(Port.F, Direction.COUNTERCLOCKWISE)`。

---

## 範例 5：動態偵測警報

**這個範例會做什麼**：把主機當成「警衛」放著不動。鏡頭前有東西在動（例如有人走過、揮手），就亮**紅燈**、嗶一聲，並在螢幕上顯示**這是第幾次**；沒動靜時亮**綠燈**。

**要準備什麼**：SPIKE 主機、神攝手（這個範例**不用**做顏色設定）。

**怎麼接**：神攝手插 **C 孔**，鏡頭對著門口或桌面，**整台放穩不要動**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex05_motion_alarm.llsp3](/examples/esp32cam/spike_beginner/official/ex05_motion_alarm.llsp3) | [ex05_motion_alarm.abp](/examples/esp32cam/spike_beginner/mbcblocks/ex05_motion_alarm.abp) | [ex05_motion_alarm.py](/examples/esp32cam/spike_beginner/pybricks/ex05_motion_alarm.py)＋[函式庫](/examples/esp32cam/spike_beginner/pybricks/MBC_cam_school_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 5 官方 App 積木](/images/sensors/esp32cam/spike-beginner/ex05-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 5 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex05-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手（學校版）範例 5：動態偵測警報（有東西在鏡頭前動，就亮紅燈、嗶一聲、記一次）
# 接線：神攝手 → C 埠。機器人要放著不動！
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_cam_school_Lib import MBC_CAM_SCHOOL

hub = PrimeHub()
cam = MBC_CAM_SCHOOL(Port.C)
count = 0                             # 警報響了幾次

while True:
    if cam.motion() > 20:             # 動態值：畫面變化越大數字越大（0~100）
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 200)
        count = count + 1
        hub.display.number(count)     # 螢幕顯示第幾次
        print(count)
        wait(1000)                    # 等 1 秒，同一個動作不要重複算
    else:
        hub.light.on(Color.GREEN)     # 沒動靜：綠燈
    wait(100)
```

</details>

**一步一步看懂程式**
1. **一開始把「次數」設成 0**（用變數記住警報響了幾次）。
2. **讀動態值**：官方 App 是「reflected light（反射光）」；MBC 積木網站是「神攝手學校版 C 動態值」；Pybricks 是 `cam.motion()`。
3. **如果 動態值 > 20** → 紅燈、嗶一聲、次數加 1、螢幕顯示次數，然後**等 1 秒**（同一個動作才不會被算好幾次）。
4. **否則** → 綠燈。
5. **等 0.1 秒**再檢查一次。

**跑起來應該看到什麼**：手在鏡頭前揮一下 → 紅燈＋嗶＋螢幕顯示 1；再揮一次 → 2……手不動就是綠燈。

**常見問題**
- **沒人動也一直響？** → 「20」太小了。先用 Pybricks 印出 `cam.motion()` 或看感應器螢幕上的 `M` 數字，看平常靜止時是多少，再把門檻設比它大一些。教室燈光閃爍、窗外有樹在動都會讓數字變大。
- **揮手都沒反應？** → 門檻太大，改小一點（例如 10），或手離鏡頭近一點。
- **可以裝在會跑的車上嗎？** → 不行。車子自己在動，整個畫面一直變，動態值會一直很大。動態值**只有機器人停住時**才準。

---

## 要注意的事（請先看）

- **進選單時畫面會定格、數值也不會更新**：程式執行中不要按感應器的側鍵；設定完回到 `RUN` 再跑程式。
- **主機斷線超過 3 秒，感應器畫面會停住**等重新連線，這是正常的，重新執行程式就好。
- **黑、白、灰都是 -1**：目前韌體分不出黑和白。
- **以下還在實機確認中**：
  - 目前韌體（2026-10 版）剛更新過開發工具，上面的數字範圍是照程式原始碼整理的，**還沒在新韌體上逐項實機量過**；如果你量到的數字不一樣，請跟我們說。
  - MBC 積木網站和 Pybricks 用「色塊寬 > 0」判斷有沒有找到色塊，是因為這兩種讀法沒有面積；我們推測「有找到但寬是 0」幾乎不會發生，尚未實機確認。
  - 範例 5 的門檻 20、範例 2 紅色的範圍 0～5／95～100，是依原始碼估的建議值，每個場地都要自己用範例 1 量過再調。

## 下一步

- 想更精準地追色塊：把範例 4 改成「偏多少就轉多少」——轉向＝（X − 50）× 一個數字（像循線的 P 控制）。
- 神攝手的完整說明：[產品頁](/sensors/esp32cam/index.md)
