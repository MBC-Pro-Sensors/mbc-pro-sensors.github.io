<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 scratchpad/products/exp6 產生；Pybricks 程式碼來自 examples/exp6/spike_beginner/pybricks/*.py，官方 App 程式碼必須跟 official/*.official.py 一樣（build.py 會檢查），改程式請改產生器再重建 -->
# 🧱 SPIKE 新手範例：6 路擴充器 (EXP6)

[🔙 回到 EXP6 產品頁](/sensors/exp6/index.md)

這一頁是寫給**完全沒寫過程式**的人。EXP6 讓一台 SPIKE 主機多出 **6 個擴充孔**，可以接更多馬達和感應器。5 個範例從「一顆馬達轉一圈」開始，一步一步做到「兩輪小車自己走正方形」。每個範例都有 3 種寫法，挑你手上有的那一種就好：

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟨 **樂高官方 SPIKE App**（Python） | 學校、手上只有官方 App 的人 | 先照下面「開始之前」**安裝函式庫一次** → 下載 `.llsp3` → 打開 SPIKE App → 左上「檔案」→「開啟」→ 選檔案 → 按 ▶ |
| 🟦 **MBC 積木網站**（中文積木） | 想用積木、不想打字的人 | 打開 [MBC SPIKE 積木網站](https://mbc-spike-blocks.mbcrobot.workers.dev) → 上方選單「檔案 → 開啟」（或按 Ctrl+O）→ 選下載的 `.abp` |
| 🐍 **Pybricks**（Python 文字程式） | 想學打字寫程式的人 | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊檔案區的「Import a file（匯入檔案）」→ 範例檔和 `MBC_exp6_obj_Lib.py` **兩個都要匯入** → 點開範例 → 按 ▶ 執行 |

> [!IMPORTANT]
> - **官方 App 沒有 EXP6 的積木**，只能用 Python。範例已經寫好，下載打開就能跑，不用自己打字。
> - **MBC 積木網站與 Pybricks 要先把主機換成 Pybricks 韌體**（MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」就能燒，約 3 分鐘；之後也能用 SPIKE App 換回樂高原廠韌體）。
> - 官方 App 這裡用的是**教育版（SPIKE 3；函式庫標示適用 3.4.3 版）**。EXP6 只支援 SPIKE Prime 主機。
> - **換另一種寫法之前，先讓 EXP6 斷電一次**（關主機或拔掉 Host 線）。EXP6 一旦跑過 Pybricks 程式，就會一直用 Pybricks 的方式溝通，直到重新開機，這時候官方 App 會連不上。

## 開始之前（3 件事）

### 1. 一定要接電池

- EXP6 上的馬達**全部靠 EXP6 自己的電池**供電，只接 Host 線、不接電池，**什麼都不會動**。
- 電池：建議 **2S 鋰電池**（7.4 V，充飽 8.4 V），接頭是 **XT30**。3S 電池也能用，但不建議。
- EXP6 的電源由主機控制：**主機開機，EXP6 才會通電；主機關機，EXP6 也會跟著關**，不用另外拔電池（依電路圖推測，請以實機為準）。

### 2. 接線

- EXP6 的 **Host 線插主機 C 孔**（這一頁全部範例都用 C 孔）。
- 馬達、感應器插 **EXP6 上的擴充孔 1～6**，程式裡的號碼就是板子上印的號碼。
- 能接什麼：
  - **樂高馬達**：SPIKE 中型、大型、小型馬達，Technic 角度馬達。
  - **樂高感應器**：只有 SPIKE **顏色感應器、超音波感應器、力量感應器**。
  - MBC 自家的感應器（循行者、雷射測距…）**目前不能插在 EXP6 上**，請直接插主機。

> [!NOTE]
> 擴充孔號碼「1～6」跟板子上印的號碼是不是每一孔都對得上，我們還在逐孔實機確認。如果你發現馬達插在 1 號孔、卻是程式裡的別的號碼在動，請跟我們說。

### 3. 官方 App：先安裝函式庫（只要做一次）

官方 App 要先把 EXP6 的函式庫（程式工具包）裝進主機，之後每個範例才能用。

1. 下載 [install_exp6_lib.llsp3](/examples/exp6/spike_beginner/official/install_exp6_lib.llsp3)（安裝程式）。
2. 主機用 USB 或藍牙連上 SPIKE App。
3. 「檔案」→「開啟」→ 選 `install_exp6_lib.llsp3`。
4. 按 ▶ **執行一次**。下方的主控台（Console）出現「**安裝成功！(Installation Successful!)**」就完成了。
5. 之後打開任何 EXP6 範例都能直接執行，**不用再裝**。

> [!TIP]
> - 主機**重新安裝／更新韌體**之後，函式庫會被清掉，要再執行一次安裝程式。
> - 想移除函式庫：下載 [官方版程式庫（.zip）](/downloads/MBC_EXP6_Official_App_Lib.zip)，執行裡面的 `Uninstall_MBC_exp6_Lib.py`。
> - MBC 積木網站和 Pybricks **不用**做這一步（MBC 積木網站會自動把函式庫一起送進主機；Pybricks 是你自己匯入 `MBC_exp6_obj_Lib.py`）。

## 這頁的範例都遵守的 5 個規則

| 規則 | 為什麼 | 怎麼做 |
|---|---|---|
| **① 開始先等馬達準備好** | 主機一開機 EXP6 才通電，馬達大約要 1 秒才準備好；在這之前送的「轉幾度」指令會被直接忽略 | MBC 積木網站、Pybricks：等「裝置代碼 = 5（馬達）」才開始。官方 App：等 2 秒（官方 App 的裝置代碼永遠是 5，不能拿來判斷） |
| **② 轉幾度之後要等它轉完** | 「轉幾度」指令送出就馬上做下一行，**不會等轉完**；還沒轉完又送一個轉幾度，第二個會被丟掉 | 中間放「等待」，或一直讀角度，到了才往下 |
| **③ 最後一定要「全部停止」** | 官方 App 沒有自動保護：程式結束時，馬達會維持最後一個指令 | 每個範例最後都有「全部停止」。官方 App 中途按主機的停止鍵，馬達**可能會繼續轉**（待實機確認）→ 關主機就會停。MBC 積木網站、Pybricks 按停止鍵，EXP6 會在 0.5 秒內自己讓馬達停下來 |
| **④ Pybricks 等待用 `keep_alive_wait`** | Pybricks 寫法有保護機制：超過 0.5 秒沒收到主機的訊號，EXP6 會讓馬達自己停下來 | 等待一律寫 `exp6.keep_alive_wait(毫秒)`，不要用 `wait()`。MBC 積木網站會自動處理，直接用「等待」積木就好 |
| **⑤ 速度都是 %** | EXP6 的速度是馬達最快速度的百分比 | 三種寫法都是 **-100 ～ 100**（負的＝反轉），數字一樣，不用換算 |

停止方式（「到位後」、「停止」要選的那個數字）：**1 = 滑行**（放掉，慢慢停）、**2 = 煞車**（馬上停）、**3 = 鎖住**（停住而且用力撐住不讓你轉）。

---

## 範例 1：一顆馬達轉一圈

**這個範例會做什麼**：擴充孔 1 的馬達轉一圈（360 度），螢幕顯示角度；再反方向轉一圈回到原位，螢幕顯示 0 左右。

**要準備什麼**
- SPIKE 主機 1 台、EXP6 1 片（接好電池）
- 樂高馬達 1 顆

**怎麼接**：EXP6 Host 線 → 主機 **C 孔**；馬達 → EXP6 **擴充孔 1**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex01_one_turn.llsp3](/examples/exp6/spike_beginner/official/ex01_one_turn.llsp3) | [ex01_one_turn.abp](/examples/exp6/spike_beginner/mbcblocks/ex01_one_turn.abp) | [ex01_one_turn.py](/examples/exp6/spike_beginner/pybricks/ex01_one_turn.py)＋[函式庫](/examples/exp6/spike_beginner/pybricks/MBC_exp6_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的程式（Python）</summary>

```python
# EXP6 範例 1：一顆馬達轉一圈再轉回來（官方 SPIKE App・Python）
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1；EXP6 接上電池
# 第一次用：先執行一次 install_exp6_lib.llsp3，把函式庫裝進主機
from hub import port, light_matrix
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)      # EXP6 接在主機 C 孔
time.sleep(2)                # 等 2 秒，讓擴充孔上的馬達準備好（太早下指令會被忽略）

exp6.motor_run_degrees(1, 50, 360, 2)    # 擴充孔 1：速度 50%，轉 360 度（一圈），到了就煞車（2）
time.sleep(1.5)                          # 等它轉完（還沒轉完就送下一個轉角度指令，會被丟掉）
light_matrix.write(str(exp6.get_motor_angle(1)))   # 螢幕顯示現在的角度（大約 360）
time.sleep(2)

exp6.motor_run_degrees(1, 50, -360, 2)   # 角度寫負的 = 反方向轉一圈，回到原來的位置
time.sleep(1.5)
light_matrix.write(str(exp6.get_motor_angle(1)))   # 大約 0
time.sleep(2)

exp6.stop_all(1)             # 最後一定要全部停止（1 = 滑行）
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 1 MBC 積木網站積木](/images/sensors/exp6/spike-beginner/ex01-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# EXP6 範例 1：一顆馬達轉一圈再轉回來（Pybricks）
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1；EXP6 接上電池
from pybricks.hubs import PrimeHub
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)    # 3 = C 孔（1=A、2=B、3=C、4=D、5=E、6=F）；False = 一般寫法

while exp6.get_device_id(1) != 5:        # 等擴充孔 1 的馬達準備好（5 = 馬達，開機大約 1 秒）
    exp6.keep_alive_wait(100)

exp6.motor_run_degrees(1, 50, 360, 2)    # 擴充孔 1：速度 50%，轉 360 度（一圈），到了就煞車（2）
exp6.keep_alive_wait(1500)               # 等它轉完。等待一律用 keep_alive_wait，不要用 wait
hub.display.number(exp6.get_motor_angle(1))   # 螢幕顯示現在的角度（大約 360）
exp6.keep_alive_wait(2000)

exp6.motor_run_degrees(1, 50, -360, 2)   # 角度寫負的 = 反方向轉一圈，回到原來的位置
exp6.keep_alive_wait(1500)
hub.display.number(exp6.get_motor_angle(1))   # 大約 0
exp6.keep_alive_wait(2000)

exp6.stop_all(1)             # 最後全部停止（1 = 滑行）
```

</details>

**一步一步看懂程式**
1. **連上 EXP6**：告訴主機 EXP6 接在 **C 孔**。官方 App 寫 `port.C`；Pybricks 寫 `3`（1=A、2=B、3=C…）；MBC 積木網站是「C 接 MBC EXP6六路馬達擴充」積木。
2. **等馬達準備好**（規則 ①）。
3. **轉 360 度**：「擴充孔 1，速度 50，轉 360 度，到位後煞車」。
4. **等 1.5 秒**讓它轉完（規則 ②），再把角度顯示在螢幕上。
5. **轉 -360 度**：角度寫負的＝反方向，回到原來的位置。
6. **全部停止**（規則 ③）。

**跑起來應該看到什麼**：馬達轉一圈停住，螢幕顯示 **360 左右**；過 2 秒反轉一圈回來，螢幕顯示 **0 左右**（差幾度是正常的）。

**常見問題**
- **馬達完全不動？** → ① 電池有沒有接、有沒有電；② 馬達是不是插在 **1 號孔**；③ Host 線是不是插主機 **C 孔**。
- **只轉了第一圈，第二圈沒轉？** → 中間等待太短，第一圈還沒轉完。把「等 1.5 秒」改長一點。
- **想反方向轉？** → 把**角度**改成負的。官方 App 的函式庫只看角度的正負，**速度寫負的沒有用**；為了三種寫法結果一樣，這頁的速度一律寫正的。

---

## 範例 2：三顆馬達一起轉

**這個範例會做什麼**：3 顆馬達同時用不同速度轉 3 秒（慢、中、快），再一起反轉 3 秒，最後全部煞車、嗶一聲。

**要準備什麼**：範例 1 的東西，馬達改成 **3 顆**。

**怎麼接**：EXP6 Host 線 → 主機 **C 孔**；馬達 → EXP6 **擴充孔 1、2、3**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex02_three_motors.llsp3](/examples/exp6/spike_beginner/official/ex02_three_motors.llsp3) | [ex02_three_motors.abp](/examples/exp6/spike_beginner/mbcblocks/ex02_three_motors.abp) | [ex02_three_motors.py](/examples/exp6/spike_beginner/pybricks/ex02_three_motors.py)＋[函式庫](/examples/exp6/spike_beginner/pybricks/MBC_exp6_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的程式（Python）</summary>

```python
# EXP6 範例 2：三顆馬達一起轉（官方 SPIKE App・Python）
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1、2、3；EXP6 接上電池
from hub import port, sound
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)
time.sleep(2)                # 等馬達準備好

exp6.motor_run(1, 30)        # 擴充孔 1：速度 30%（慢）
exp6.motor_run(2, 50)        # 擴充孔 2：速度 50%
exp6.motor_run(3, 70)        # 擴充孔 3：速度 70%（快）
time.sleep(3)                # 一起轉 3 秒

exp6.motor_run(1, -30)       # 速度寫負的 = 反轉
exp6.motor_run(2, -50)
exp6.motor_run(3, -70)
time.sleep(3)

exp6.stop_all(2)             # 全部停止（2 = 煞車，馬上停）
sound.beep(880, 200)
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 2 MBC 積木網站積木](/images/sensors/exp6/spike-beginner/ex02-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# EXP6 範例 2：三顆馬達一起轉（Pybricks）
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1、2、3；EXP6 接上電池
from pybricks.hubs import PrimeHub
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)    # 3 = C 孔

for m in (1, 2, 3):          # 三顆馬達都要準備好（5 = 馬達）
    while exp6.get_device_id(m) != 5:
        exp6.keep_alive_wait(100)

exp6.motor_run(1, 30)        # 擴充孔 1：速度 30%（慢）
exp6.motor_run(2, 50)        # 擴充孔 2：速度 50%
exp6.motor_run(3, 70)        # 擴充孔 3：速度 70%（快）
exp6.keep_alive_wait(3000)   # 一起轉 3 秒

exp6.motor_run(1, -30)       # 速度寫負的 = 反轉
exp6.motor_run(2, -50)
exp6.motor_run(3, -70)
exp6.keep_alive_wait(3000)

exp6.stop_all(2)             # 全部停止（2 = 煞車，馬上停）
hub.speaker.beep(880, 200)
```

</details>

**一步一步看懂程式**
1. **等 3 顆馬達都準備好**：MBC 積木網站和 Pybricks 一顆一顆檢查「裝置代碼 = 5」。
2. **持續轉**：「擴充孔 1 速度 30」「擴充孔 2 速度 50」「擴充孔 3 速度 70」。持續轉的指令送出後馬達會一直轉，程式直接往下做，所以 3 顆幾乎同時開始。
3. **等 3 秒**。
4. **速度改成負的** → 3 顆一起反轉，再等 3 秒。
5. **全部停止，2 = 煞車**，最後嗶一聲。

**跑起來應該看到什麼**：3 顆馬達一慢、一中、一快一起轉，3 秒後一起反過來轉，再 3 秒全部停住、主機嗶一聲。

**常見問題**
- **有一顆不會轉？** → 那一顆的號碼插錯孔，或沒插緊。把它換到別的孔試試，就知道是馬達還是孔的問題。
- **速度 100 很快、很用力？** → EXP6 直接用電池的電推馬達，速度太快、裝在機構上容易撞壞，先用 50 以下。

---

## 範例 3：光控開關（顏色感應器控制馬達）

**這個範例會做什麼**：顏色感應器前面**放一張白紙（亮）→ 馬達轉、主機燈變綠**；拿開（暗）→ 馬達停、燈變紅。**左右鍵一起按住**就結束。

**要準備什麼**
- 範例 1 的東西，再加上
- SPIKE 顏色感應器 1 個、白紙 1 張

**怎麼接**：EXP6 Host 線 → 主機 **C 孔**；馬達 → **擴充孔 1**；顏色感應器 → **擴充孔 4**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex03_light_switch.llsp3](/examples/exp6/spike_beginner/official/ex03_light_switch.llsp3) | [ex03_light_switch.abp](/examples/exp6/spike_beginner/mbcblocks/ex03_light_switch.abp) | [ex03_light_switch.py](/examples/exp6/spike_beginner/pybricks/ex03_light_switch.py)＋[函式庫](/examples/exp6/spike_beginner/pybricks/MBC_exp6_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的程式（Python）</summary>

```python
# EXP6 範例 3：光控開關（官方 SPIKE App・Python）
# 感應器前面亮（放一張白紙）→ 馬達轉、主機燈變綠；暗 → 馬達停、燈變紅
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → 擴充孔 1；顏色感應器 → 擴充孔 4；EXP6 接上電池
from hub import port, button, light
import color
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)
time.sleep(2)

while not (button.pressed(button.LEFT) and button.pressed(button.RIGHT)):   # 左右鍵一起按住 → 結束
    bright = exp6.get_color_reflection(4)    # 擴充孔 4 的顏色感應器：反射光 0~100（越亮越大）
    print(bright)                            # 在電腦畫面印出來，方便看數字
    if bright > 50:                          # 亮 → 馬達轉
        exp6.motor_run(1, 50)
        light.color(light.POWER, color.GREEN)
    else:                                    # 暗 → 馬達停
        exp6.motor_stop(1, 2)
        light.color(light.POWER, color.RED)
    time.sleep(0.1)

exp6.stop_all(1)             # 結束前一定要全部停止
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 3 MBC 積木網站積木](/images/sensors/exp6/spike-beginner/ex03-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# EXP6 範例 3：光控開關（Pybricks）
# 感應器前面亮（放一張白紙）→ 馬達轉、主機燈變綠；暗 → 馬達停、燈變紅
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → 擴充孔 1；顏色感應器 → 擴充孔 4；EXP6 接上電池
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button, Color
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)    # 3 = C 孔

while exp6.get_device_id(1) != 5:        # 等擴充孔 1 的馬達準備好
    exp6.keep_alive_wait(100)

while not (Button.LEFT in hub.buttons.pressed() and Button.RIGHT in hub.buttons.pressed()):   # 左右鍵一起按住 → 結束
    bright = exp6.get_color_reflection(4)    # 擴充孔 4 的顏色感應器：反射光 0~100（越亮越大）
    print(bright)
    if bright > 50:                          # 亮 → 馬達轉
        exp6.motor_run(1, 50)
        hub.light.on(Color.GREEN)
    else:                                    # 暗 → 馬達停
        exp6.motor_stop(1, 2)
        hub.light.on(Color.RED)
    exp6.keep_alive_wait(100)

exp6.stop_all(1)
```

</details>

**一步一步看懂程式**
1. **重複，直到左右鍵一起按住**：裡面的事情會一直做；兩個鍵一起按下去就跳出來。
2. **讀反射光**：擴充孔 4 的顏色感應器有多亮（0～100，越亮越大），存進「亮度」這個變數（變數＝有名字的盒子，可以放一個數字），也印在電腦畫面上。
3. **如果 亮度 > 50** → 馬達轉、燈變綠。
4. **否則** → 馬達停（煞車）、燈變紅。
5. 等 0.1 秒再檢查一次。
6. 跳出來之後 → **全部停止**。

**跑起來應該看到什麼**：感應器前面什麼都沒有：紅燈、馬達不動。把白紙靠近感應器（1～2 公分）：綠燈、馬達轉。拿開又停。左右鍵一起按住：程式結束、馬達停。

**常見問題**
- **一直是紅燈 / 一直是綠燈？** → 看電腦畫面印出來的數字：放白紙和拿開時各是多少，把「50」改成兩個數字中間的值。
- **官方 App 的數字跟另外兩種不一樣？** → 正常。官方 App 的函式庫是用感應器的紅、綠、藍三個顏色**估算**亮度，Pybricks 和 MBC 積木網站讀的是感應器**真正的**反射光，所以同一張紙讀到的數字會差一點。
- **讀到的一直是 0？** → 顏色感應器有沒有插在 **4 號孔**；EXP6 只認得樂高 SPIKE 顏色感應器。

---

## 範例 4：用主機按鈕控制馬達

**這個範例會做什麼**：**按住左鍵 → 馬達正轉；按住右鍵 → 反轉；放開 → 停**。左右鍵一起按住就結束。

**要準備什麼**：同範例 1。

**怎麼接**：EXP6 Host 線 → 主機 **C 孔**；馬達 → **擴充孔 1**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex04_button_motor.llsp3](/examples/exp6/spike_beginner/official/ex04_button_motor.llsp3) | [ex04_button_motor.abp](/examples/exp6/spike_beginner/mbcblocks/ex04_button_motor.abp) | [ex04_button_motor.py](/examples/exp6/spike_beginner/pybricks/ex04_button_motor.py)＋[函式庫](/examples/exp6/spike_beginner/pybricks/MBC_exp6_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的程式（Python）</summary>

```python
# EXP6 範例 4：用主機按鈕控制馬達（官方 SPIKE App・Python）
# 按住左鍵 → 正轉；按住右鍵 → 反轉；放開 → 停；左右鍵一起按 → 結束
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1；EXP6 接上電池
from hub import port, button
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)
time.sleep(2)

while not (button.pressed(button.LEFT) and button.pressed(button.RIGHT)):   # 左右鍵一起按住 → 結束
    if button.pressed(button.LEFT):          # 按住左鍵
        exp6.motor_run(1, 50)                # 正轉，速度 50%
    elif button.pressed(button.RIGHT):       # 按住右鍵
        exp6.motor_run(1, -50)               # 反轉
    else:                                    # 都沒按
        exp6.motor_stop(1, 2)                # 停止（2 = 煞車）
    time.sleep(0.05)

exp6.stop_all(1)             # 結束前一定要全部停止
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 4 MBC 積木網站積木](/images/sensors/exp6/spike-beginner/ex04-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# EXP6 範例 4：用主機按鈕控制馬達（Pybricks）
# 按住左鍵 → 正轉；按住右鍵 → 反轉；放開 → 停；左右鍵一起按 → 結束
# 接線：EXP6 的 Host 線 → 主機 C 孔；馬達 → EXP6 擴充孔 1；EXP6 接上電池
from pybricks.hubs import PrimeHub
from pybricks.parameters import Button
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)    # 3 = C 孔

while exp6.get_device_id(1) != 5:        # 等擴充孔 1 的馬達準備好
    exp6.keep_alive_wait(100)

while not (Button.LEFT in hub.buttons.pressed() and Button.RIGHT in hub.buttons.pressed()):   # 左右鍵一起按住 → 結束
    if Button.LEFT in hub.buttons.pressed():         # 按住左鍵
        exp6.motor_run(1, 50)                        # 正轉，速度 50%
    elif Button.RIGHT in hub.buttons.pressed():      # 按住右鍵
        exp6.motor_run(1, -50)                       # 反轉
    else:                                            # 都沒按
        exp6.motor_stop(1, 2)                        # 停止（2 = 煞車）
    exp6.keep_alive_wait(50)

exp6.stop_all(1)
```

</details>

**一步一步看懂程式**
1. **重複，直到左右鍵一起按住**（跟範例 3 一樣）。
2. **如果 左鍵被按住** → 擴充孔 1 速度 50（正轉）。
3. **否則，如果 右鍵被按住** → 速度 -50（反轉）。
4. **都沒按** → 停止（煞車）。
5. 等 0.05 秒再檢查一次按鈕。
6. 跳出來之後 → **全部停止**。

**跑起來應該看到什麼**：按住左鍵馬達轉、放開就停；按住右鍵往另一邊轉。兩個一起按住，程式結束。

**常見問題**
- **為什麼不用中間的按鈕？** → 中間的按鈕是主機的開關，也是停止程式的按鈕，這頁的範例不拿它來控制馬達。
- **按了要等一下才轉？** → 程式每 0.05 秒檢查一次按鈕，官方 App 送指令也要一點時間，慢一點點是正常的。
- **官方 App 按停止鍵，馬達還在轉？** → 規則 ③：官方 App 沒有自動保護。用「左右鍵一起按住」結束程式；真的停不下來就關主機。

---

## 範例 5：兩輪小車走正方形

**這個範例會做什麼**：兩輪小車直走約 35 公分 → 原地右轉 90 度，重複 4 次，走出一個正方形回到起點。

**要準備什麼**
- 範例 1 的東西，再加上
- 兩輪小車（兩顆馬達當輪子，輪子直徑 56 mm、兩輪距離 112 mm，就是樂高標準小車）
- 一塊平坦的地面（大約 50 × 50 公分）

**怎麼接**：EXP6 Host 線 → 主機 **C 孔**；**左輪馬達 → 擴充孔 1、右輪馬達 → 擴充孔 2**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex05_square.llsp3](/examples/exp6/spike_beginner/official/ex05_square.llsp3) | [ex05_square.abp](/examples/exp6/spike_beginner/mbcblocks/ex05_square.abp) | [ex05_square.py](/examples/exp6/spike_beginner/pybricks/ex05_square.py)＋[函式庫](/examples/exp6/spike_beginner/pybricks/MBC_exp6_obj_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的程式（Python）</summary>

```python
# EXP6 範例 5：兩輪小車走正方形（官方 SPIKE App・Python）
# 接線：EXP6 的 Host 線 → 主機 C 孔；左輪馬達 → 擴充孔 1；右輪馬達 → 擴充孔 2；EXP6 接上電池
# 小車：輪子直徑 56 mm、兩輪距離 112 mm（樂高標準小車）
from hub import port
import time
from MBC_exp6_SPIKE_App_Lib import MBC_EXP6

exp6 = MBC_EXP6(port.C)
exp6.set_motor_inverted(1, True)   # 左輪是反過來裝的：設成「反裝」，速度正的才會往前
time.sleep(2)                      # 等馬達準備好

for side in range(4):              # 正方形有 4 個邊
    exp6.reset_angle(2)                        # 右輪角度歸零，用來量走了多遠
    exp6.drive(1, 2, 40, 40)                   # 左輪孔 1、右輪孔 2，兩輪都 40% → 直走
    while exp6.get_motor_angle(2) < 720:       # 右輪轉滿 720 度（2 圈，大約 35 公分）
        time.sleep(0.01)
    exp6.stop_all(2)                           # 煞車
    time.sleep(0.3)

    exp6.reset_angle(2)
    exp6.drive(1, 2, 30, -30)                  # 左輪往前、右輪往後 → 原地右轉
    while exp6.get_motor_angle(2) > -180:      # 右輪倒轉 180 度 ≈ 車子轉 90 度
        time.sleep(0.01)
    exp6.stop_all(2)
    time.sleep(0.3)

exp6.stop_all(1)                   # 結束前一定要全部停止
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 5 MBC 積木網站積木](/images/sensors/exp6/spike-beginner/ex05-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# EXP6 範例 5：兩輪小車走正方形（Pybricks）
# 接線：EXP6 的 Host 線 → 主機 C 孔；左輪馬達 → 擴充孔 1；右輪馬達 → 擴充孔 2；EXP6 接上電池
# 小車：輪子直徑 56 mm、兩輪距離 112 mm（樂高標準小車）
from pybricks.hubs import PrimeHub
from MBC_exp6_obj_Lib import MBC_EXP6

hub = PrimeHub()
exp6 = MBC_EXP6(3, False)          # 3 = C 孔
exp6.set_motor_inverted(1, True)   # 左輪是反過來裝的：設成「反裝」，速度正的才會往前

for m in (1, 2):                   # 兩顆馬達都要準備好
    while exp6.get_device_id(m) != 5:
        exp6.keep_alive_wait(100)
exp6.keep_alive_wait(300)

for side in range(4):              # 正方形有 4 個邊
    exp6.reset_angle(2)                        # 右輪角度歸零，用來量走了多遠
    exp6.drive(1, 2, 40, 40)                   # 左輪孔 1、右輪孔 2，兩輪都 40% → 直走
    while exp6.get_motor_angle(2) < 720:       # 右輪轉滿 720 度（2 圈，大約 35 公分）
        exp6.keep_alive_wait(10)
    exp6.stop_all(2)                           # 煞車
    exp6.keep_alive_wait(300)

    exp6.reset_angle(2)
    exp6.drive(1, 2, 30, -30)                  # 左輪往前、右輪往後 → 原地右轉
    while exp6.get_motor_angle(2) > -180:      # 右輪倒轉 180 度 ≈ 車子轉 90 度
        exp6.keep_alive_wait(10)
    exp6.stop_all(2)
    exp6.keep_alive_wait(300)

exp6.stop_all(1)
```

</details>

**一步一步看懂程式**
1. **左輪設成「反裝」**：小車的兩顆馬達是左右對稱裝的，左輪轉的方向跟右輪相反。設成反裝之後，兩輪都是「速度正的 = 往前」。
2. **等兩顆馬達準備好**。
3. **重複 4 次**（正方形 4 個邊）：
   1. **右輪角度歸零**：等一下用它來量走了多遠。
   2. **雙輪一起走**：「左輪孔 1、右輪孔 2，左 40、右 40」→ 直走。EXP6 會讓兩輪同步，走得比較直。
   3. **等到右輪轉滿 720 度**（2 圈 ≈ 35 公分）→ 全部停止（煞車），停 0.3 秒。
   4. **右輪角度再歸零**，「左 30、右 -30」→ 左輪往前、右輪往後 → **原地右轉**。
   5. **等到右輪倒轉 180 度** → 車子剛好轉 90 度（輪子轉 180 度 × 輪徑 56 ÷ 輪距 112 = 90 度）→ 停止。
4. **全部停止**（滑行）。

**跑起來應該看到什麼**：小車走一段、右轉、走一段、右轉…走完 4 個邊，大約回到出發的位置。

**常見問題**
- **小車原地打轉，不會往前？** → 左輪沒有設成反裝，或兩顆馬達插反了（左輪要在 **1 號孔**）。
- **小車往後退？** → 馬達的裝法跟樂高標準小車相反：改成「擴充孔 **2**」設反裝（官方 App／Pybricks 把 `set_motor_inverted(1, True)` 的 1 改成 2；MBC 積木網站把「安裝方向」積木的擴充孔改成 2），接線不用動。
- **轉不到 90 度 / 轉超過？** → 地面打滑或輪子大小不一樣。把 **180** 改大一點（轉不夠）或改小一點（轉太多），一次改 5～10。
- **邊長不一樣長？** → 把 **720** 改大或改小。720 度 ≈ 35 公分，每 100 度大約 5 公分。

---

## 還想知道更多

- **為什麼官方 App 是寫程式，不是積木？** → 官方 App 的積木只能控制樂高自己的零件；EXP6 要透過函式庫用 Python 控制。
- **官方 App 一定要等 2 秒嗎？** → 主機已經開機一陣子、馬達早就準備好的話，不等也可以；但剛開機時一定要等，不然第一個「轉幾度」會沒反應。
- **兩種寫法的「絕對角度」不一樣？** → 官方 App 的函式庫讀「絕對角度」是 -180 ～ 179，Pybricks 是 0 ～ 359。這頁的範例沒有用到絕對角度。
- 全部指令的完整說明：[官方 App 專區](/sensors/exp6/spike-official.md)、[Pybricks 專區](/sensors/exp6/spike-pybricks.md)
