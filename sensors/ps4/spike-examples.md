<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 scratchpad/products/ps4 產生（跟 ps2 共用 build.py）；程式碼區塊來自 examples/ps4/spike_beginner/pybricks/*.py，改程式請改產生器再重建 -->
# 🧱 SPIKE 新手範例：掌控者 PS4/PS5 藍牙遙控接收器

[🔙 回到掌控者 PS4/PS5 產品頁](/sensors/ps4/index.md)

這一頁是寫給**完全沒寫過程式**的人。5 個範例從「看到按鍵的數字」開始，一步一步做到「用兩支搖桿開坦克車」。每個範例都有 3 種寫法，挑你手上有的那一種就好：

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟨 **樂高官方 SPIKE App**（積木） | 學校、第一次用 SPIKE 的人 | 下載 `.llsp3` → 打開 SPIKE App → 左上「檔案」→「開啟」→ 選檔案 |
| 🟦 **MBC 積木網站**（中文積木） | 想用中文積木、直接選「哪一個鍵」的人 | 打開 [MBC SPIKE 積木網站](https://mbc-spike-blocks.mbcrobot.workers.dev) → 上方選單「檔案 → 開啟」（或按 Ctrl+O）→ 選下載的 `.abp` |
| 🐍 **Pybricks**（Python 文字程式） | 想學打字寫程式的人 | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊檔案區的「Import a file（匯入檔案）」→ 範例檔和 `MBC_gamepad_Lib.py` **兩個都要匯入** → 點開範例 → 按 ▶ 執行 |

> [!IMPORTANT]
> - 接收器要是 **SPIKE 版韌體**（每個平台是不同的出廠版本；EV3 版插在 SPIKE 上不會被認出來）。
> - **MBC 積木網站與 Pybricks 要先把主機換成 Pybricks 韌體**（MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」就能燒，約 3 分鐘；之後也能用 SPIKE App 換回樂高原廠韌體）。樂高官方 App 不用換。
> - 樂高官方 App 這裡用的是**教育版（SPIKE 3）**。家用版（Robot Inventor）的「顏色」積木讀法不一樣，這頁的官方 App 範例不適用。

> [!NOTE]
> 最後的**範例 6** 是進階加碼，**只有 Pybricks 版**：它用的數值放在感應器 2026-10 以後韌體的新模式裡，樂高官方 App 和 MBC 積木網站的積木讀不到。

## 開始之前：配對手把（只要做一次）

1. 接收器插在主機的 **C 孔**，主機開機。**插上去的時候不要按住接收器上的按鍵**（會進入燒錄模式，接收器就不會動作；放開按鍵、拔掉重插就好）。
2. **第一次使用**：接收器還沒記住任何手把時，一開機就會自動開放 **30 秒**配對（綠燈快速閃）。
   **要換手把、或 30 秒已經過了**：按一下接收器上的按鍵（Reset），會清掉舊的手把、重新開放 30 秒。
3. 在這 30 秒內讓手把進入配對模式，直到手把的燈條快速閃爍：
   - **PS4 手把**：同時長按 **SHARE＋PS**
   - **PS5 手把**：同時長按 **建立（Create）＋PS**
4. 連上之後接收器會**記住這支手把**，也不會再連別人的手把。之後開機，只要按手把的 **PS 鍵**就會自動連上。

| 接收器的綠燈 | 意思 |
|---|---|
| 快速閃 | 正在等手把配對 |
| 一直亮 | 沒有連上手把（按手把 PS 鍵重新連線） |
| 熄掉 | 已經連上手把 |

**怎麼知道連好了？** 跑下面的範例 2：放開搖桿時螢幕是 **5**，左搖桿往上推到底變 **9**。

## 這頁會用到的數值

**搖桿**：每一支搖桿有「左右（X）」和「上下（Y）」兩個數字，都是 **1 ～ 9**。

| 搖桿位置 | 數字 |
|---|---|
| 放開（在中間） | **5** |
| 往上推到底／往右推到底 | **9** |
| 往下拉到底／往左推到底 | **1** |

> [!TIP]
> 只推一點點時，往左、往下很快就變成 4，往右、往上要推到一半左右才變成 6（兩邊不一樣靈敏）。所以這頁的範例都把 **4 ～ 6 當成「在中間」**，**7 以上才算往上推、3 以下才算往下拉**，左右兩邊才會一樣好控制。

**按鍵**：MBC 積木網站和 Pybricks 可以直接選「哪一個鍵」。Pybricks 的按鍵名稱：

| 按鍵 | Pybricks 名稱 | 按鍵 | Pybricks 名稱 |
|---|---|---|---|
| 上、右、下、左 | `'UP'`、`'RIGHT'`、`'DOWN'`、`'LEFT'` | L1、R1、L2、R2 | `'L1'`、`'R1'`、`'L2'`、`'R2'` |
| ▲、●、✕、■ | `'TRIANGLE'`、`'CIRCLE'`、`'CROSS'`、`'SQUARE'` | SHARE（PS5：建立）、OPTIONS | `'SHARE'`、`'OPTIONS'`（也可以寫 `'SELECT'`、`'START'`） |
| 左搖桿按下去、右搖桿按下去 | `'L3'`、`'R3'` | | |

PS 鍵和觸控板讀不到。MBC 積木網站的下拉選單寫的是 SELECT（＝SHARE／建立）、START（＝OPTIONS）。

搖桿名稱：`'LX'` 左搖桿左右、`'LY'` 左搖桿上下、`'RX'` 右搖桿左右、`'RY'` 右搖桿上下。

在樂高官方 App 裡，接收器會被當成「顏色感應器」，所以要用顏色感應器的積木來讀：

| 官方積木 | 讀到的是 | 每個鍵的數字（同時按就相加） |
|---|---|---|
| **reflected light（反射光）** | 方向鍵＋圖形鍵 | 上 1、右 2、下 4、左 8、▲ 16、● 32、✕ 64、■ 128 |
| **color（顏色）** | 肩鍵＋功能鍵 | L2 1、R2 2、L1 4、R1 8、SHARE（PS5：建立）16、L3 32、R3 64、OPTIONS **-128** |
| **raw red（原始紅色，在「更多感應器」）** | 左搖桿 | **X×10＋Y**，放開時是 55。上下（Y）＝**除以 10 的餘數**；左右（X）＝除以 10 取整數 |
| **raw green（原始綠色）** | 右搖桿 | 同上 |

> [!TIP]
> **官方 App 怎麼知道某一個鍵有沒有按？** 用「(讀到的數字 ÷ 那個鍵的數字) 取整數，再除以 2 的餘數是不是 1」。例如 ✕ 是 64：只按 ✕ 時反射光是 64，64 ÷ 64 = 1，1 除以 2 餘 1 → 有按；同時按上（1）和 ✕ 也是 65 ÷ 64 → 取整數 1 → 餘 1 → 有按。
> - 不要用官方 App「顏色是不是紅色」這種比對積木，一定要用「顏色」的**數字**。
> - 「原始藍色」這頁不用（PS2 和 PS4 的排列不一樣）。

---

## 範例 1：按鍵測試

**這個範例會做什麼**：主機螢幕一直顯示「按鍵的數字」。按不同的鍵，數字就不一樣，可以用來確認每一個鍵都正常。

**要準備什麼**
- SPIKE 主機 1 台
- 掌控者 PS4/PS5 接收器＋PS4 或 PS5 手把（已配對，見上面）

**怎麼接**：接收器插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex01_button_number.llsp3](/examples/ps4/spike_beginner/official/ex01_button_number.llsp3) | [ex01_button_number.abp](/examples/ps4/spike_beginner/mbcblocks/ex01_button_number.abp) | [ex01_button_number.py](/examples/ps4/spike_beginner/pybricks/ex01_button_number.py)＋[函式庫](/examples/ps4/spike_beginner/pybricks/MBC_gamepad_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 1 官方 App 積木](/images/sensors/ps4/spike-beginner/ex01-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 1 MBC 積木網站積木](/images/sensors/ps4/spike-beginner/ex01-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 掌控者PS4 範例 1：按鍵測試（螢幕顯示按鍵數字）
# 接線：掌控者PS4 接收器 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)       # 接收器插在 C 埠

while True:
    pad.update()                # 先讀一次手把
    n = pad.buttons()           # 全部按鍵合成一個數字：沒按 = 0，上 = 1、右 = 2、下 = 4、左 = 8…
    hub.display.text(str(n))    # 主機螢幕捲動顯示這個數字
    print(n)                    # 電腦畫面也印出來
    wait(100)                   # 等 0.1 秒再讀下一次
```

</details>

**一步一步看懂程式**
1. **一直重複**：下面的積木會一直做，直到你按停止。
2. **讀按鍵**：官方 App 用「反射光」，讀到方向鍵和圖形鍵；MBC 積木網站和 Pybricks 用「全部按鍵」，16 個鍵都讀得到。
3. **顯示在螢幕上**：數字會在螢幕上捲過去（按住按鍵，等它捲完）。
4. **等 0.1 秒**再讀下一次。

**跑起來應該看到什麼**

| 按住 | 三種寫法都顯示 | MBC 積木網站、Pybricks 才有 |
|---|---|---|
| 沒按 | 0 | |
| 上 | 1 | |
| 右／下／左 | 2／4／8 | |
| ▲／●／✕／■ | 16／32／64／128 | |
| 上＋▲ 一起按 | 17（1＋16，同時按就相加） | |
| L2／R2／L1／R1 | 官方 App 還是 0（肩鍵要用「顏色」積木，範例 3 會用到） | 256／512／1024／2048 |
| SHARE／L3／R3／OPTIONS | 官方 App 還是 0 | 4096／8192／16384／32768 |

**常見問題**
- **一直是 0，按什麼都沒反應？** → 手把還沒連上：看接收器的綠燈。一直亮＝按手把 PS 鍵重新連線；快速閃＝還在等配對，照上面「配對手把」做。
- **手把一直連上又斷掉？** → 接收器記住的是別支手把：按一下接收器的按鍵（Reset），重新配對。
- **主機說找不到感應器、或數字怪怪的？** → 接收器可能是 EV3 版韌體，要用 SPIKE 版。

---

## 範例 2：搖桿數值

**這個範例會做什麼**：主機螢幕顯示**左搖桿上下的位置**（1 ～ 9）。放開是 5，往上推變大、往下拉變小。

**要準備什麼**：同範例 1。

**怎麼接**：接收器插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex02_stick_value.llsp3](/examples/ps4/spike_beginner/official/ex02_stick_value.llsp3) | [ex02_stick_value.abp](/examples/ps4/spike_beginner/mbcblocks/ex02_stick_value.abp) | [ex02_stick_value.py](/examples/ps4/spike_beginner/pybricks/ex02_stick_value.py)＋[函式庫](/examples/ps4/spike_beginner/pybricks/MBC_gamepad_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 2 官方 App 積木](/images/sensors/ps4/spike-beginner/ex02-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 2 MBC 積木網站積木](/images/sensors/ps4/spike-beginner/ex02-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 掌控者PS4 範例 2：搖桿數值（螢幕顯示左搖桿上下的位置）
# 接線：掌控者PS4 接收器 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)       # 接收器插在 C 埠

while True:
    pad.update()                # 先讀一次手把
    y = pad.stick('LY')         # 左搖桿上下：1（拉到最下）~ 5（中間）~ 9（推到最上）
    hub.display.number(y)       # 主機螢幕顯示這個數字
    print('LX', pad.stick('LX'), 'LY', y, 'RX', pad.stick('RX'), 'RY', pad.stick('RY'))   # 電腦畫面印出 4 個方向
    wait(100)
```

</details>

**一步一步看懂程式**
1. **讀左搖桿上下**：
   - 官方 App：「原始紅色」讀到的是 **X×10＋Y**（例如 59＝左右在中間 5、往上推到底 9），所以用「**除以 10 的餘數**」把 Y 拿出來（59 → 9）。
   - MBC 積木網站：直接選「左搖桿 Y（1~9）」。
   - Pybricks：`pad.stick('LY')`。
2. **顯示在螢幕上**。MBC 積木網站和 Pybricks 也會在電腦畫面印出數字（Pybricks 4 個方向都印）。
3. **等 0.1 秒**再讀下一次。

**跑起來應該看到什麼**
- 放開搖桿：**5**。
- 慢慢往上推：6、7、8、**9**。
- 慢慢往下拉：4、3、2、**1**。

**常見問題**
- **一直是 5，推了也不會變？** → 手把沒連上時搖桿也是 5：看接收器的綠燈有沒有熄掉（熄掉才是連上）。
- **想看右搖桿或左右方向？** → MBC 積木網站：把下拉選單換成「右搖桿 Y」或「左搖桿 X」；官方 App：右搖桿用「原始綠色」，左右方向用「除以 10 取整數」；Pybricks：把 `'LY'` 換成 `'RY'`、`'LX'`、`'RX'`。

---

## 範例 3：按鍵控制燈和聲音

**這個範例會做什麼**：按 ▲ 主機燈變**綠色**、按 ● 變**紅色**、按 ✕ 變**藍色**、按 ■ 變**粉紅色**（跟 PS4 手把上符號的顏色一樣），都沒按就關燈。按 **L1** 嗶一聲低音、按 **R1** 嗶一聲高音。

**要準備什麼**：同範例 1。

**怎麼接**：接收器插 **C 孔**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex03_light_sound.llsp3](/examples/ps4/spike_beginner/official/ex03_light_sound.llsp3) | [ex03_light_sound.abp](/examples/ps4/spike_beginner/mbcblocks/ex03_light_sound.abp) | [ex03_light_sound.py](/examples/ps4/spike_beginner/pybricks/ex03_light_sound.py)＋[函式庫](/examples/ps4/spike_beginner/pybricks/MBC_gamepad_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 3 官方 App 積木](/images/sensors/ps4/spike-beginner/ex03-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 3 MBC 積木網站積木](/images/sensors/ps4/spike-beginner/ex03-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 掌控者PS4 範例 3：按鍵控制燈和聲音
# 接線：掌控者PS4 接收器 → C 埠
# ▲ 綠燈、● 紅燈、✕ 藍燈、■ 粉紅燈（跟手把上的顏色一樣），都沒按就關燈；L1 低音嗶、R1 高音嗶
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)               # 接收器插在 C 埠

while True:
    pad.update()                        # 先讀一次手把
    if pad.pressed('TRIANGLE'):         # ▲
        hub.light.on(Color.GREEN)
    elif pad.pressed('CIRCLE'):         # ●
        hub.light.on(Color.RED)
    elif pad.pressed('CROSS'):          # ✕
        hub.light.on(Color.BLUE)
    elif pad.pressed('SQUARE'):         # ■
        hub.light.on(Color.MAGENTA)
    else:                               # 4 個都沒按
        hub.light.off()

    if pad.pressed('L1'):
        hub.speaker.beep(262, 100)      # 低音 Do（262 Hz），響 0.1 秒
    if pad.pressed('R1'):
        hub.speaker.beep(523, 100)      # 高音 Do（523 Hz）
    wait(50)
```

</details>

**一步一步看懂程式**
1. 先把按鍵讀進「變數」（變數＝有名字的盒子，可以放一個數字）：官方 App 用兩個盒子，「按鍵」放反射光（方向鍵＋圖形鍵）、「肩鍵」放顏色（L1、R1…）。MBC 積木網站和 Pybricks 不用自己存，直接問「某個鍵有沒有被按下」。
2. **如果 ▲ 有按** → 綠燈。
3. **否則，如果 ● 有按** → 紅燈；**否則如果 ✕** → 藍燈；**否則如果 ■** → 粉紅燈。
4. **都不是** → 關燈。
5. 另外兩個「如果」：**L1 有按** → 嗶低音（官方 App 音符 60、另外兩種 262 Hz，都是「Do」）；**R1 有按** → 嗶高音（音符 72／523 Hz，高八度的 Do）。
6. 官方 App 判斷「▲ 有按」的寫法：(按鍵 ÷ 16) 取整數，除以 2 的餘數 = 1。● 換成 32、✕ 換成 64、■ 換成 128；L1 是 (肩鍵 ÷ 4)、R1 是 (肩鍵 ÷ 8)。

**跑起來應該看到什麼**
- 按住 ▲：綠燈；放開：燈熄。
- 同時按 ▲ 和 ●：綠燈（上面的「如果」先成立就不看下面的）。
- 按住 L1：一直嗶嗶嗶低音。

**常見問題**
- **官方 App 按 L1、R1 沒聲音？** → 肩鍵是用「顏色」積木讀的（本頁依韌體和官方範例整理，尚未逐一上機確認）。先跑範例 1 確認方向鍵、圖形鍵正常；再把範例 1 的「反射光」換成「顏色」，按 L1 應該顯示 4、R1 顯示 8。
- **燈的顏色怎麼換？** → 官方 App：點「set Power Button light to」積木上的顏色圓點；MBC 積木網站：點「主機燈號設為」的下拉選單；Pybricks：把 `Color.GREEN` 換成 `Color.YELLOW` 之類。

---

## 範例 4：方向鍵開車

**這個範例會做什麼**：按住方向鍵，車子就往那個方向走；放開就停。上＝前進、下＝後退、左＝原地左轉、右＝原地右轉。

**要準備什麼**
- 範例 1 的東西，再加上
- 兩輪小車（兩顆馬達當輪子）

**怎麼接**
- 接收器插 **C 孔**
- **左輪接 E、右輪接 F**

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex04_dpad_drive.llsp3](/examples/ps4/spike_beginner/official/ex04_dpad_drive.llsp3) | [ex04_dpad_drive.abp](/examples/ps4/spike_beginner/mbcblocks/ex04_dpad_drive.abp) | [ex04_dpad_drive.py](/examples/ps4/spike_beginner/pybricks/ex04_dpad_drive.py)＋[函式庫](/examples/ps4/spike_beginner/pybricks/MBC_gamepad_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 4 官方 App 積木](/images/sensors/ps4/spike-beginner/ex04-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 4 MBC 積木網站積木](/images/sensors/ps4/spike-beginner/ex04-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 掌控者PS4 範例 4：方向鍵開車（按住才走，放開就停）
# 接線：掌控者PS4 接收器 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

pad = MBC_GAMEPAD(Port.C)                          # 接收器插在 C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)   # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                              # 右輪

while True:
    pad.update()                    # 先讀一次手把
    if pad.pressed('UP'):           # 上 → 前進（功率 50%）
        left.dc(50)
        right.dc(50)
    elif pad.pressed('DOWN'):       # 下 → 後退
        left.dc(-50)
        right.dc(-50)
    elif pad.pressed('LEFT'):       # 左 → 原地左轉：左輪往後、右輪往前
        left.dc(-30)
        right.dc(30)
    elif pad.pressed('RIGHT'):      # 右 → 原地右轉
        left.dc(30)
        right.dc(-30)
    else:                           # 都沒按 → 停
        left.dc(0)
        right.dc(0)
    wait(50)
```

</details>

**一步一步看懂程式**
1. **告訴主機輪子怎麼裝**：左右兩顆馬達是面對面裝的，所以左輪要「反過來轉」才會往前。
   - MBC 積木網站：「E 接 樂高 馬達　馬達正轉 逆時針」（F 是順時針）；Pybricks：`Direction.COUNTERCLOCKWISE`。這樣設好之後，兩輪都給正的數字就是往前。
   - 官方 App 的馬達積木沒有這個設定，所以**左輪 E 的數字要加負號**：往前是 E **-50**、F **50**。
2. **一直重複**：讀按鍵。
3. **如果 上 有按** → 兩輪都往前（功率 50%）。
4. **否則如果 下** → 兩輪都往後。
5. **否則如果 左** → 左輪往後、右輪往前 → 原地左轉（功率 30%，轉慢一點比較好控制）。
6. **否則如果 右** → 原地右轉。
7. **都沒按** → 兩輪功率 0，停下來。

| 按住 | MBC 積木網站、Pybricks（E, F） | 官方 App（E, F） |
|---|---|---|
| 上 | 50, 50 | -50, 50 |
| 下 | -50, -50 | 50, -50 |
| 左 | -30, 30 | 30, 30 |
| 右 | 30, -30 | -30, -30 |
| 沒按 | 0, 0 | 0, 0 |

**跑起來應該看到什麼**：按住上，車子往前走；放開，車子停。

**常見問題**
- **按上卻往後退？** → 馬達的裝法跟樂高標準小車相反：MBC 積木網站把 E、F 的「正轉」方向對調；Pybricks 左輪改成 `Direction.CLOCKWISE`、右輪改成 `Motor(Port.F, Direction.COUNTERCLOCKWISE)`；官方 App 把所有數字的正負號都對調。
- **按上卻原地打轉？** → 只有一邊的方向反了：只改那一顆馬達的正負號（或正轉方向）。
- **手把沒電或走太遠斷線了？** → 接收器超過 0.3 秒收不到手把，就會把按鍵全部放開、搖桿回到中間，車子會自己停下來。

---

## 範例 5：雙搖桿坦克開車

**這個範例會做什麼**：像開坦克一樣：**左搖桿管左輪、右搖桿管右輪**。兩支都往前推＝前進、都往後拉＝後退、一前一後＝原地轉彎。推越多跑越快，放開就停。

**要準備什麼**：同範例 4。

**怎麼接**：接收器插 **C 孔**；**左輪接 E、右輪接 F**。

**下載程式**

| 樂高官方 App | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex05_tank_drive.llsp3](/examples/ps4/spike_beginner/official/ex05_tank_drive.llsp3) | [ex05_tank_drive.abp](/examples/ps4/spike_beginner/mbcblocks/ex05_tank_drive.abp) | [ex05_tank_drive.py](/examples/ps4/spike_beginner/pybricks/ex05_tank_drive.py)＋[函式庫](/examples/ps4/spike_beginner/pybricks/MBC_gamepad_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的積木</summary>

![範例 5 官方 App 積木](/images/sensors/ps4/spike-beginner/ex05-official.webp)

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 5 MBC 積木網站積木](/images/sensors/ps4/spike-beginner/ex05-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 掌控者PS4 範例 5：雙搖桿坦克開車（左搖桿管左輪、右搖桿管右輪）
# 接線：掌控者PS4 接收器 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Direction, Port
from pybricks.pupdevices import Motor
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)                          # 接收器插在 C 埠
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)   # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)                              # 右輪

# 開始前的保護：等兩支搖桿都在中間（4~6）才開始，車子才不會一開機就衝出去
while True:
    pad.update()
    if 4 <= pad.stick('LY') <= 6 and 4 <= pad.stick('RY') <= 6:
        break
    wait(20)
hub.light.on(Color.GREEN)                          # 綠燈 = 可以開了

while True:
    pad.update()                                   # 先讀一次手把
    ly = pad.stick('LY')                           # 左搖桿上下 1~9
    ry = pad.stick('RY')                           # 右搖桿上下 1~9
    # 死區：搖桿在中間附近（4~6）就當成 0，放開手車子就停
    if 4 <= ly <= 6:
        left_power = 0
    else:
        left_power = (ly - 5) * 25                 # 7 → 50%、9 → 100%、3 → -50%、1 → -100%
    if 4 <= ry <= 6:
        right_power = 0
    else:
        right_power = (ry - 5) * 25
    left.dc(left_power)
    right.dc(right_power)
    wait(20)
```

</details>

**一步一步看懂程式**
1. **開始前的保護**：先「等到」兩支搖桿都在中間（4 ～ 6）才開始，然後亮**綠燈**。這樣一開機搖桿剛好被碰到時，車子不會突然衝出去。
2. **一直重複**：讀左搖桿上下（Y）和右搖桿上下（Y）。官方 App 是「原始紅色」和「原始綠色」**除以 10 的餘數**。
3. **死區**：搖桿在 **4 ～ 6** 就把功率設成 **0**。手放開時搖桿不一定剛好回到 5，有了死區車子才會乖乖停住。
4. **否則**：功率＝**(搖桿 − 5) × 25**。推到 7 是 50%、推到底 9 是 100%；拉到 3 是 -50%、拉到底 1 是 -100%。
5. **把功率給馬達**：左輪 E 用左搖桿的功率、右輪 F 用右搖桿的功率。官方 App 的左輪要 **× -1**（原因同範例 4）。

**跑起來應該看到什麼**
- 程式一開始：放開搖桿就亮綠燈，車子不動。
- 兩支搖桿一起往前推：前進，推越多越快。
- 左搖桿往前、右搖桿往後：原地右轉。
- 放開：停車。

**常見問題**
- **按了執行，一直沒亮綠燈？** → 搖桿沒在中間：兩支搖桿都放開，就會亮綠燈開始。
- **推一點點車子不會動？** → 這是死區，推到 7 以上（或拉到 3 以下）才會動。想要更靈敏，可以把「4 ～ 6」改成「5 ～ 5」，但放開時車子可能會慢慢滑。
- **太快不好控制？** → 把「× 25」改成「× 15」（推到底變 60%）。
- **前後顛倒或原地打轉？** → 同範例 4 的常見問題。

---

## 範例 6（進階）：連線狀態與電量

> [!NOTE]
> **這個範例只有 Pybricks 版。** 它用的「連線狀態」和「電量」放在接收器的模式 3，樂高官方 App 和 MBC 積木網站讀不到。接收器韌體要是 **2026-10 以後**的版本，舊韌體讀到的會是別的數字。 程式碼就是範例，不需要截圖。

**這個範例會做什麼**：程式一開始亮**紅燈**，**等到手把連上**才亮綠燈、嗶一聲，並印出接收器種類和韌體版本；之後螢幕一直顯示手把**電量**（0～100，例如 80），手把關機或走太遠斷線就變回紅燈、螢幕顯示「--」。

**要準備什麼**：同範例 1（主機＋接收器＋已配對的 PS4 或 PS5 手把）。

**怎麼接**：接收器插 **C 孔**。

**下載程式**

| Pybricks |
|---|
| [ex06_connected_battery.py](/examples/ps4/spike_beginner/pybricks/ex06_connected_battery.py)＋[函式庫](/examples/ps4/spike_beginner/pybricks/MBC_gamepad_Lib.py) |

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 掌控者PS4 範例 6：連線狀態與電量（等手把連上才開始，螢幕顯示電量）
# 接線：掌控者PS4 接收器 → C 埠
# 只有 Pybricks 版：連線狀態和電量放在接收器的「模式 3」，官方 App 和積木網站讀不到；接收器韌體要 2026-10 以後
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_gamepad_Lib import MBC_GAMEPAD

hub = PrimeHub()
pad = MBC_GAMEPAD(Port.C)                    # 接收器插在 C 埠

hub.light.on(Color.RED)                      # 紅燈 = 手把還沒連上
while not pad.connected():                   # 等到手把連上（True）才往下
    wait(200)
hub.light.on(Color.GREEN)                    # 綠燈 = 連上了
hub.speaker.beep()
print('接收器種類', pad.kind(), ' 韌體', pad.firmware_version())   # 例如 PS4 (1, 0)

while True:
    if pad.connected():
        battery = pad.battery()              # 手把電量 0~100
        hub.display.number(min(battery, 99)) # 螢幕最多只能顯示兩位數
        hub.light.on(Color.GREEN)
        print('已連線  電量', battery, '%')
    else:                                    # 手把關機或離太遠 → 斷線
        hub.display.text('--')
        hub.light.on(Color.RED)
        print('斷線了')
    wait(500)
```

</details>

**一步一步看懂程式**
1. `pad.connected()`：手把有沒有連上接收器（True／False）。開頭用「**等到**連上」取代範例 5 的「等搖桿在中間」，更可靠。
2. `pad.kind()`、`pad.firmware_version()`：接收器是 PS2 還是 PS4 款、韌體幾版（讀感應器的模式 8）。
3. **一直重複**：連著就顯示 `pad.battery()`、綠燈；斷線就紅燈、「--」。螢幕最多顯示兩位數，所以 100 會顯示 99。
4. 這個範例沒有呼叫 `pad.update()`，因為沒有要讀按鍵。要讀按鍵的程式請照範例 1～5 的寫法，`connected()`、`battery()` 放在迴圈外或偶爾呼叫（每次呼叫接收器要切換模式，多花幾十 ms）。

**跑起來應該看到什麼**：手把還沒連：紅燈。按手把 PS 鍵連上的瞬間：綠燈＋嗶，螢幕顯示電量（例如 80）。把手把關掉：紅燈、「--」。

**常見問題**
- **電量數字不會變？** → 手把每隔一段時間才回報一次電量，充電中也不一定馬上更新，是正常的。
- **一直紅燈？** → 手把沒配對（看上面「配對手把」）、或接收器韌體太舊（2026-10 以前）沒有連線狀態，請更新韌體。
- **可以跟範例 5 合併嗎？** → 可以：把範例 5 開頭的「等搖桿在中間」換成「等到 `pad.connected()`」，也可以在電量低於 20 時亮橘燈提醒。

---

## 下一步

- 全部按鍵、搖桿和配對的完整說明：[掌控者 PS4/PS5 產品頁](/sensors/ps4/index.md)
- 想加更多鍵：MBC 積木網站「PS2／PS4遙控」分類有全部 16 個鍵；Pybricks 用 `pad.pressed('OPTIONS')` 這樣換名稱就好。
