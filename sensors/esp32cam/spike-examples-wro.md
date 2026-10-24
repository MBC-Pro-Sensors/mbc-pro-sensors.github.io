<!-- AI 注意：在此專案的 Markdown 檔案中，直接插入的 HTML 程式碼請勿使用任何縮排，否則會被 Markdown 解析器誤認為程式碼區塊（變成文本視窗）。 -->
<!-- 本頁由 scratchpad/products/esp32cam_wro 產生（build.py 從 page_wro.tpl.md 填入官方 App 程式）；Pybricks 程式碼來自 examples/esp32cam/spike_beginner_wro/pybricks/*.py，改程式請改產生器再重建 -->
<!-- moved: /sensors/esp32cam/spike/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/esp32cam/spike/index.md">神攝手 ESP32CAM › SPIKE</a></p>

# 🧱 SPIKE 新手範例：神攝手 WRO 版（HUE12）

[🔙 回到神攝手產品頁](/sensors/esp32cam/index.md)　｜　手上是**學校老師創意賽版**？請看 [學校版新手範例](/sensors/esp32cam/spike-examples.md)

WRO 版（HUE12）一次看 **12 個點（12 格）** 的顏色：你在畫面上指定 12 個位置，它就告訴你每個位置現在是什麼顏色。這一頁是寫給**完全沒寫過程式**的人，5 個範例從「看到一個數字」開始，一步一步做到「看物塊顏色決定從哪邊繞過」。每個範例都有 3 種寫法：

| 寫法 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| 🟨 **樂高官方 SPIKE App**（**Python**） | 學校、只能用官方 App 的人 | 下載 `.llsp3` → 打開 SPIKE App → 左上「檔案」→「開啟」→ 選檔案（打開後是 Python 程式） |
| 🟦 **MBC 積木網站**（中文積木） | 想用積木寫 HUE12 的人（**最推薦新手**） | 打開 [MBC SPIKE 積木網站](https://mbc-spike-blocks.mbcrobot.workers.dev) → 上方選單「檔案 → 開啟」（或按 Ctrl+O）→ 選下載的 `.abp` |
| 🐍 **Pybricks**（Python 文字程式） | 想學打字寫程式的人 | 打開 [code.pybricks.com](https://code.pybricks.com) → 左邊檔案區的「Import a file（匯入檔案）」→ 範例檔和 `MBC_hue12_Lib.py` **兩個都要匯入** → 點開範例 → 按 ▶ 執行 |

> [!IMPORTANT]
> - **樂高官方 App 沒有 HUE12 可以用的積木**（HUE12 不是樂高的感應器），所以官方 App 的範例是 **Python 程式**（教育版 SPIKE 3 App 的 Python）。想用積木的話，請用 MBC 積木網站。
> - **MBC 積木網站與 Pybricks 要先把主機換成 Pybricks 韌體 v4.0.0b5 以上**。MBC 積木網站上方選單「SPIKE 主機 → 安裝／更新 Pybricks 韌體」燒的是 v4.1.0，可以直接用（約 3 分鐘；之後也能用 SPIKE App 換回樂高原廠韌體）。官方 App 不用換。

## 開始之前：在感應器上設定 12 格的位置

每一格是畫面上的**一個點**。你要先用感應器機身的按鍵告訴它「第 1 格看哪裡、第 2 格看哪裡……」。設定會記在感應器裡，重新開機也還在。

**先做這 2 件事**
1. **先把感應器插上主機、主機開機**，感應器螢幕出現鏡頭畫面、上方顯示 `RUN MODE` 才能設定。
2. 按鍵：機身有 **左、確認（中）、右** 三顆，加上一顆**側鍵**。側鍵短按＝進選單／回上一層。

**設定第 1 格（其他格一樣）**
1. `RUN MODE` 畫面**短按側鍵** → 上方出現 `SET MODE S1 [..]`（`[ON]`＝這格已經設定過）。用 **左／右** 可以換成 S2～S12。
2. 按 **確認** → `SET X`：用 **左／右** 把紅色直線移到要看的位置（按住會快速移動）。
3. 按 **確認** → `SET Y`：用 **左／右** 把綠色橫線移到要看的位置。
4. 按 **確認** → `TOL [Live]`：畫面上跟這一點同色的地方會變綠色，用 **左／右** 調到只有你的物塊變綠。
5. 按 **確認** 存好、回到 `RUN MODE`。畫面上這一點會出現一個小十字。
6. 不要用的格子：在選單選到那一格，**側鍵長按 1 秒**就清掉。

> [!TIP]
> - 範例 1、2、4、5 只用**第 1 格**：請把第 1 格設在**車子正前方、物塊會出現的位置**。
> - 範例 3 會檢查全部 12 格，多設幾格（例如排成一排）會比較好玩。
> - **在選單裡的時候，12 格全部會變成 255（沒設定）**，程式會以為什麼都沒看到。設定完一定要回到 `RUN MODE` 再跑程式。
> - 畫面上下顛倒：選單切到最後的 `SET MODE CAM` → 按確認 → 用左右找到 `V-Flip`，按確認切成 ON。補光燈是同一個選單的 `Flash LED`，很亮、積木會反光，先不要開。

## 這頁會用到的數值

每一格都有 2 個數字：

| 名稱 | 範圍 | 意思 |
|---|---|---|
| **色相** | 0 ～ 255 | **0～239＝彩色**（紅 0 附近和 230～239、黃約 40、綠約 80、青約 120、藍約 160、洋紅約 200）；**240～245＝白**；**246～250＝黑**；**255＝這一格沒設定** |
| **飽和度** | 0 ～ 255 | 顏色多鮮豔，越大越鮮豔 |

> [!TIP]
> 紅色在色相的**頭和尾**（0 附近、230～239 也是紅），所以判斷紅色要寫成「色相 < 10 **或** 色相在 230～239」。240 以上不是紅，是白、黑或沒設定。

Pybricks 的函式庫還會直接給你**顏色名稱**：`Red`（紅）、`Org`（橘）、`Yel`（黃）、`Lim`（黃綠）、`Grn`（綠）、`Mnt`（薄荷綠）、`Cyn`（青）、`Azr`（天藍）、`Blu`（藍）、`Prp`（紫）、`Mag`（洋紅）、`Pnk`（粉紅）、`Wht`（白）、`Blk`（黑）、`NONE`（沒設定）。

---

## 範例 1：讀第 1 格的色相

**這個範例會做什麼**：主機螢幕一直顯示第 1 格看到的色相；電腦畫面也會印出色相和飽和度。

**要準備什麼**
- SPIKE 主機 1 台
- HUE12 1 個（已設定第 1 格）
- 紅、綠、藍、白、黑色的積木各一塊

**怎麼接**：HUE12 插 **C 孔**。

**下載程式**

| 樂高官方 App（Python） | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex01_show_slot1.llsp3](/examples/esp32cam/spike_beginner_wro/official/ex01_show_slot1.llsp3) | [ex01_show_slot1.abp](/examples/esp32cam/spike_beginner_wro/mbcblocks/ex01_show_slot1.abp) | [ex01_show_slot1.py](/examples/esp32cam/spike_beginner_wro/pybricks/ex01_show_slot1.py)＋[函式庫](/examples/esp32cam/spike_beginner_wro/pybricks/MBC_hue12_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的 Python 程式</summary>

```python
# ===== 讀 HUE12 的小工具（跟官方範例 Hue12_demo_For_Spike_v3.4.3 的讀法一樣，照抄就好、不用改）=====
import device
import utime
from hub import port


class HUE12:
    def __init__(self, port_number):
        self.port = port_number
        self.slots = [(255, 0)] * 12          # 12 格的（色相, 飽和度）；255 = 這一格沒設定

    def update(self):
        """讀一次最新資料；讀不到回傳 False"""
        raw = device.data(self.port)          # 感應器送來 8 個整數
        if not raw or len(raw) < 8:
            return False
        b = []
        for val in raw[:8]:
            if val < 0:
                val += 2 ** 32
            b += [val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF]   # 每個整數拆成 3 個數字
        for i in range(12):
            self.slots[i] = (b[i * 2], b[i * 2 + 1])
        return True

    def hue(self, slot):
        """第 slot 格（1~12）的色相：0~239 彩色、240~245 白、246~250 黑、255 沒設定"""
        return self.slots[slot - 1][0]

    def sat(self, slot):
        """第 slot 格的飽和度 0~255（越大顏色越鮮豔）"""
        return self.slots[slot - 1][1]
# ===== 小工具到這裡結束，下面才是範例 =====

from hub import light_matrix

cam = HUE12(port.C)                 # HUE12 插在 C 孔

while True:
    cam.update()                    # 讀一次最新資料
    hue = cam.hue(1)                # 第 1 格的色相
    print(hue, cam.sat(1))          # 電腦畫面印出 色相、飽和度
    light_matrix.write(str(hue))    # 主機螢幕顯示色相
    utime.sleep_ms(500)             # 等 0.5 秒
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 1 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex01-wro-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 WRO 版（HUE12）範例 1：讀第 1 格的色相（顯示在主機螢幕）
# 接線：HUE12 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

hub = PrimeHub()
cam = HUE12Sensor(Port.C)               # HUE12 插在 C 孔

while True:
    cam.update()                        # 讀一次最新資料
    name, hue, sat = cam.get_slot(1)    # 第 1 格：顏色名稱、色相、飽和度
    print(name, hue, sat)               # 電腦畫面印出，例如 Red 5 200
    hub.display.text(str(hue))          # 主機螢幕一個字一個字顯示色相
    wait(500)
```

</details>

**一步一步看懂程式**
1. **讀資料**：官方 App 和 Pybricks 先 `cam.update()`（讀一次最新的 12 格），再拿第 1 格；MBC 積木網站直接用「神攝手HUE12 C 第 1 格色相」積木。
2. **顯示**：主機螢幕一個字一個字顯示色相，電腦畫面也印出來。
3. **等 0.5 秒**再讀下一次。
4. 官方 App 程式最上面那一大段是「讀 HUE12 的小工具」（官方 App 不能匯入別的檔案，所以每支程式都要帶著它），**照抄就好，不用改**。

**跑起來應該看到什麼**：紅色積木 0～9 或 230～239；綠色 80 左右；白紙 240～245；黑色 246～250；這一格沒設定是 255。

**常見問題**
- **一直是 255？** → 第 1 格沒設定，或感應器還在選單裡（回到 `RUN MODE`）。
- **Pybricks 一直顯示 255、印出 `Connected to ID:` 後面不是 95？** → 主機的 Pybricks 韌體太舊，請更新到 v4.0.0b5 以上。

---

## 範例 2：第 1 格是不是紅色

**這個範例會做什麼**：第 1 格看到紅色 → 主機中間的燈亮**紅色**並嗶一聲；其他顏色 → 關燈。

**要準備什麼**：同範例 1。　**怎麼接**：HUE12 插 **C 孔**。

**下載程式**

| 樂高官方 App（Python） | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex02_is_red.llsp3](/examples/esp32cam/spike_beginner_wro/official/ex02_is_red.llsp3) | [ex02_is_red.abp](/examples/esp32cam/spike_beginner_wro/mbcblocks/ex02_is_red.abp) | [ex02_is_red.py](/examples/esp32cam/spike_beginner_wro/pybricks/ex02_is_red.py)＋[函式庫](/examples/esp32cam/spike_beginner_wro/pybricks/MBC_hue12_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的 Python 程式</summary>

```python
# ===== 讀 HUE12 的小工具（跟官方範例 Hue12_demo_For_Spike_v3.4.3 的讀法一樣，照抄就好、不用改）=====
import device
import utime
from hub import port


class HUE12:
    def __init__(self, port_number):
        self.port = port_number
        self.slots = [(255, 0)] * 12          # 12 格的（色相, 飽和度）；255 = 這一格沒設定

    def update(self):
        """讀一次最新資料；讀不到回傳 False"""
        raw = device.data(self.port)          # 感應器送來 8 個整數
        if not raw or len(raw) < 8:
            return False
        b = []
        for val in raw[:8]:
            if val < 0:
                val += 2 ** 32
            b += [val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF]   # 每個整數拆成 3 個數字
        for i in range(12):
            self.slots[i] = (b[i * 2], b[i * 2 + 1])
        return True

    def hue(self, slot):
        """第 slot 格（1~12）的色相：0~239 彩色、240~245 白、246~250 黑、255 沒設定"""
        return self.slots[slot - 1][0]

    def sat(self, slot):
        """第 slot 格的飽和度 0~255（越大顏色越鮮豔）"""
        return self.slots[slot - 1][1]
# ===== 小工具到這裡結束，下面才是範例 =====

import color
from hub import light, sound

cam = HUE12(port.C)

while True:
    cam.update()
    hue = cam.hue(1)
    if hue < 10 or (hue >= 230 and hue < 240):   # 紅色在色相的兩端：0~9 和 230~239
        light.color(light.POWER, color.RED)        # 中間按鈕燈變紅
        sound.beep(880, 100, 100)                  # 嗶一聲（880 Hz、0.1 秒、音量 100）
    else:
        light.color(light.POWER, color.BLACK)      # 黑色 = 關燈
    utime.sleep_ms(200)
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 2 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex02-wro-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 WRO 版（HUE12）範例 2：第 1 格是紅色就亮紅燈、嗶一聲
# 接線：HUE12 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Color, Port
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

hub = PrimeHub()
cam = HUE12Sensor(Port.C)

while True:
    cam.update()
    name, hue, sat = cam.get_slot(1)
    if hue < 10 or 230 <= hue < 240:      # 紅色在色相的兩端：0~9 和 230~239（240 以上是白、黑、沒設定）
        hub.light.on(Color.RED)
        hub.speaker.beep(880, 100)
    else:
        hub.light.off()
    wait(200)
```

</details>

**一步一步看懂程式**
1. 讀第 1 格的色相。
2. **如果 色相 < 10，或 色相在 230～239 之間** → 紅色：亮紅燈、嗶一聲。
3. **否則** → 關燈。
4. 等 0.2 秒再檢查。

**跑起來應該看到什麼**：紅色積木放到第 1 格的位置 → 紅燈＋嗶；換成其他顏色 → 燈熄掉。

**常見問題**
- **橘色也會嗶？** → 橘色約 10～30，把「10」改小一點（例如 6）。
- **Pybricks 有沒有更簡單的寫法？** → 有：`if name == "Red":`，函式庫會幫你判斷（範圍跟上面一樣）。

---

## 範例 3：12 格裡有幾格看到顏色

**這個範例會做什麼**：一格一格檢查 12 格，數出有幾格看到**彩色**（不算白、黑、沒設定），把數字顯示在主機螢幕上。

**要準備什麼**：同範例 1，**多設定幾格**（例如第 1～4 格排成一排），再放幾塊彩色積木。

**怎麼接**：HUE12 插 **C 孔**。

**下載程式**

| 樂高官方 App（Python） | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex03_count_colors.llsp3](/examples/esp32cam/spike_beginner_wro/official/ex03_count_colors.llsp3) | [ex03_count_colors.abp](/examples/esp32cam/spike_beginner_wro/mbcblocks/ex03_count_colors.abp) | [ex03_count_colors.py](/examples/esp32cam/spike_beginner_wro/pybricks/ex03_count_colors.py)＋[函式庫](/examples/esp32cam/spike_beginner_wro/pybricks/MBC_hue12_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的 Python 程式</summary>

```python
# ===== 讀 HUE12 的小工具（跟官方範例 Hue12_demo_For_Spike_v3.4.3 的讀法一樣，照抄就好、不用改）=====
import device
import utime
from hub import port


class HUE12:
    def __init__(self, port_number):
        self.port = port_number
        self.slots = [(255, 0)] * 12          # 12 格的（色相, 飽和度）；255 = 這一格沒設定

    def update(self):
        """讀一次最新資料；讀不到回傳 False"""
        raw = device.data(self.port)          # 感應器送來 8 個整數
        if not raw or len(raw) < 8:
            return False
        b = []
        for val in raw[:8]:
            if val < 0:
                val += 2 ** 32
            b += [val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF]   # 每個整數拆成 3 個數字
        for i in range(12):
            self.slots[i] = (b[i * 2], b[i * 2 + 1])
        return True

    def hue(self, slot):
        """第 slot 格（1~12）的色相：0~239 彩色、240~245 白、246~250 黑、255 沒設定"""
        return self.slots[slot - 1][0]

    def sat(self, slot):
        """第 slot 格的飽和度 0~255（越大顏色越鮮豔）"""
        return self.slots[slot - 1][1]
# ===== 小工具到這裡結束，下面才是範例 =====

from hub import light_matrix

cam = HUE12(port.C)

while True:
    cam.update()
    count = 0                           # 先歸零
    for slot in range(1, 13):           # 第 1 格到第 12 格，一格一格檢查
        if cam.hue(slot) < 240:         # 小於 240 = 彩色（240 以上是白、黑、沒設定）
            count = count + 1
    print(count)
    light_matrix.write(str(count))      # 主機螢幕顯示有幾格是彩色
    utime.sleep_ms(500)
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 3 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex03-wro-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 WRO 版（HUE12）範例 3：12 格裡有幾格看到顏色（彩色）
# 接線：HUE12 → C 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Port
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

hub = PrimeHub()
cam = HUE12Sensor(Port.C)

while True:
    cam.update()
    count = 0                                   # 先歸零
    for slot in range(1, 13):                   # 第 1 格到第 12 格，一格一格檢查
        name, hue, sat = cam.get_slot(slot)
        if hue < 240:                           # 小於 240 = 彩色（240 以上是白、黑、沒設定）
            count = count + 1
    print(count)
    hub.display.number(count)                   # 主機螢幕顯示有幾格是彩色（0~12）
    wait(500)
```

</details>

**一步一步看懂程式**
1. **「彩色格數」先歸零**。
2. **重複 12 次**，每次檢查一格：官方 App 和 Pybricks 用 `for slot in range(1, 13)`（slot 會依序是 1、2、……12）；MBC 積木網站用變數「第幾格」從 1 開始，每做完一次加 1。
3. **如果 這一格的色相 < 240**（彩色）→ 彩色格數加 1。
4. 12 格都看完 → 把彩色格數顯示在螢幕上。

**跑起來應該看到什麼**：4 格裡放 2 塊彩色積木 → 顯示 2；再放一塊 → 3。白紙、黑色積木不算。

**常見問題**
- **數字比我放的積木還多？** → 有些格子看到的是彩色的背景（例如場地的彩色線）；把那幾格清掉（選單裡側鍵長按 1 秒）。
- **為什麼用 240 當分界？** → 240 以上是白（240～245）、黑（246～250）和沒設定（255），都不是彩色。

---

## 範例 4：依顏色轉向

**這個範例會做什麼**：第 1 格看到**紅色** → 原地**右轉**；看到**綠色** → 原地**左轉**；其他 → 停。

**要準備什麼**
- 範例 1 的東西，再加上兩輪小車（兩顆馬達當輪子），HUE12 裝在車頭、鏡頭朝前
- 紅色、綠色的物塊

**怎麼接**：HUE12 插 **C 孔**；**左輪接 E、右輪接 F**。

**下載程式**

| 樂高官方 App（Python） | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex04_turn_by_color.llsp3](/examples/esp32cam/spike_beginner_wro/official/ex04_turn_by_color.llsp3) | [ex04_turn_by_color.abp](/examples/esp32cam/spike_beginner_wro/mbcblocks/ex04_turn_by_color.abp) | [ex04_turn_by_color.py](/examples/esp32cam/spike_beginner_wro/pybricks/ex04_turn_by_color.py)＋[函式庫](/examples/esp32cam/spike_beginner_wro/pybricks/MBC_hue12_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的 Python 程式</summary>

```python
# ===== 讀 HUE12 的小工具（跟官方範例 Hue12_demo_For_Spike_v3.4.3 的讀法一樣，照抄就好、不用改）=====
import device
import utime
from hub import port


class HUE12:
    def __init__(self, port_number):
        self.port = port_number
        self.slots = [(255, 0)] * 12          # 12 格的（色相, 飽和度）；255 = 這一格沒設定

    def update(self):
        """讀一次最新資料；讀不到回傳 False"""
        raw = device.data(self.port)          # 感應器送來 8 個整數
        if not raw or len(raw) < 8:
            return False
        b = []
        for val in raw[:8]:
            if val < 0:
                val += 2 ** 32
            b += [val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF]   # 每個整數拆成 3 個數字
        for i in range(12):
            self.slots[i] = (b[i * 2], b[i * 2 + 1])
        return True

    def hue(self, slot):
        """第 slot 格（1~12）的色相：0~239 彩色、240~245 白、246~250 黑、255 沒設定"""
        return self.slots[slot - 1][0]

    def sat(self, slot):
        """第 slot 格的飽和度 0~255（越大顏色越鮮豔）"""
        return self.slots[slot - 1][1]
# ===== 小工具到這裡結束，下面才是範例 =====

import motor_pair

cam = HUE12(port.C)
motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)    # 左輪 E、右輪 F 配成一組

while True:
    cam.update()
    hue = cam.hue(1)
    if hue < 10 or (hue >= 230 and hue < 240):        # 紅色 → 原地右轉
        motor_pair.move(motor_pair.PAIR_1, 100, velocity=200)
    elif hue >= 60 and hue <= 110:                    # 綠色 → 原地左轉
        motor_pair.move(motor_pair.PAIR_1, -100, velocity=200)
    else:                                             # 其他 → 停
        motor_pair.stop(motor_pair.PAIR_1)
    utime.sleep_ms(50)
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 4 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex04-wro-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 WRO 版（HUE12）範例 4：依顏色轉向（紅色右轉、綠色左轉、其他停）
# 接線：HUE12 → C 埠；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

cam = HUE12Sensor(Port.C)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)       # 左輪：反過來裝，所以設成逆時針才是往前
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    cam.update()
    name, hue, sat = cam.get_slot(1)
    if hue < 10 or 230 <= hue < 240:      # 紅色 → 原地右轉（每秒 60 度）
        robot.drive(0, 60)
    elif 60 <= hue <= 110:                # 綠色 → 原地左轉
        robot.drive(0, -60)
    else:                                 # 其他顏色、白、黑、沒設定 → 停
        robot.stop()
    wait(50)
```

</details>

**一步一步看懂程式**
1. **告訴主機輪子接在哪裡**：官方 App 是 `motor_pair.pair(…, port.E, port.F)`；MBC 積木網站和 Pybricks 是「移動底盤 左輪 E、右輪 F」。
2. 讀第 1 格的色相。
3. **如果 紅色** → 原地右轉（官方 App：steering 100、輪子每秒 200 度；另外兩種：車身每秒轉 60 度）。
4. **否則如果 綠色**（色相 60～110）→ 原地左轉。
5. **否則** → 停。

**跑起來應該看到什麼**：紅色物塊放到車頭前 → 車子往右轉，轉到看不到紅色就停；綠色物塊 → 往左轉。

**常見問題**
- **綠色沒反應？** → 先用範例 1 看你的綠色是多少，把「60～110」改成包含它的範圍。
- **轉的方向反了？** → 馬達裝法跟樂高標準小車相反。官方 App：把 100 和 -100 對調；MBC 積木網站：「移動底盤」積木裡左右輪的「正轉」方向都換成另一個；Pybricks：左輪改成 `Direction.CLOCKWISE`、右輪改成 `Motor(Port.F, Direction.COUNTERCLOCKWISE)`。

---

## 範例 5：看物塊顏色，決定從哪邊繞過（WRO 常見用法）

**這個範例會做什麼**：車子慢慢往前走。前面出現**紅色物塊 → 從右邊繞過**；**綠色物塊 → 從左邊繞過**；繞完繼續往前。WRO 很多題目都會用「看顏色決定走哪一邊」（例如未來工程師組的紅綠交通號誌柱），這是最基本的寫法。

**要準備什麼**：同範例 4，場地上放幾個紅色、綠色的物塊（間隔 40 公分以上）。

**怎麼接**：HUE12 插 **C 孔**（第 1 格設在車頭正前方、約 15 公分遠的地面高度）；**左輪接 E、右輪接 F**。

**下載程式**

| 樂高官方 App（Python） | MBC 積木網站 | Pybricks |
|---|---|---|
| [ex05_wro_bypass.llsp3](/examples/esp32cam/spike_beginner_wro/official/ex05_wro_bypass.llsp3) | [ex05_wro_bypass.abp](/examples/esp32cam/spike_beginner_wro/mbcblocks/ex05_wro_bypass.abp) | [ex05_wro_bypass.py](/examples/esp32cam/spike_beginner_wro/pybricks/ex05_wro_bypass.py)＋[函式庫](/examples/esp32cam/spike_beginner_wro/pybricks/MBC_hue12_Lib.py) |

<details>
<summary>🟨 樂高官方 App 的 Python 程式</summary>

```python
# ===== 讀 HUE12 的小工具（跟官方範例 Hue12_demo_For_Spike_v3.4.3 的讀法一樣，照抄就好、不用改）=====
import device
import utime
from hub import port


class HUE12:
    def __init__(self, port_number):
        self.port = port_number
        self.slots = [(255, 0)] * 12          # 12 格的（色相, 飽和度）；255 = 這一格沒設定

    def update(self):
        """讀一次最新資料；讀不到回傳 False"""
        raw = device.data(self.port)          # 感應器送來 8 個整數
        if not raw or len(raw) < 8:
            return False
        b = []
        for val in raw[:8]:
            if val < 0:
                val += 2 ** 32
            b += [val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF]   # 每個整數拆成 3 個數字
        for i in range(12):
            self.slots[i] = (b[i * 2], b[i * 2 + 1])
        return True

    def hue(self, slot):
        """第 slot 格（1~12）的色相：0~239 彩色、240~245 白、246~250 黑、255 沒設定"""
        return self.slots[slot - 1][0]

    def sat(self, slot):
        """第 slot 格的飽和度 0~255（越大顏色越鮮豔）"""
        return self.slots[slot - 1][1]
# ===== 小工具到這裡結束，下面才是範例 =====

import motor_pair
from hub import light_matrix

cam = HUE12(port.C)
motor_pair.pair(motor_pair.PAIR_1, port.E, port.F)


def go(steering, ms):
    """用 steering（-100 左轉 ~ 0 直走 ~ 100 右轉）走 ms 毫秒，然後停"""
    motor_pair.move(motor_pair.PAIR_1, steering, velocity=200)
    utime.sleep_ms(ms)
    motor_pair.stop(motor_pair.PAIR_1)


while True:
    cam.update()
    hue = cam.hue(1)
    if hue < 10 or (hue >= 230 and hue < 240):    # 紅色物塊 → 從右邊繞過
        light_matrix.write("R")
        go(100, 450)      # 右轉約 45 度
        go(0, 1500)       # 直走約 15 公分
        go(-100, 450)     # 左轉回來
    elif hue >= 60 and hue <= 110:                # 綠色物塊 → 從左邊繞過
        light_matrix.write("G")
        go(-100, 450)
        go(0, 1500)
        go(100, 450)
    else:                                         # 沒看到物塊 → 慢慢往前
        motor_pair.move(motor_pair.PAIR_1, 0, velocity=150)
    utime.sleep_ms(20)
```

</details>

<details>
<summary>🟦 MBC 積木網站的積木</summary>

![範例 5 MBC 積木網站積木](/images/sensors/esp32cam/spike-beginner/ex05-wro-mbcblocks.webp)

</details>

<details>
<summary>🐍 Pybricks 程式碼</summary>

```python
# 神攝手 WRO 版（HUE12）範例 5：看前面物塊的顏色，決定從哪一邊繞過
#   紅色 → 從右邊繞、綠色 → 從左邊繞、沒看到 → 直走
# 接線：HUE12 → C 埠（第 1 格對準車子正前方）；左輪馬達 → E 埠；右輪馬達 → F 埠
from pybricks.hubs import PrimeHub
from pybricks.parameters import Direction, Port
from pybricks.pupdevices import Motor
from pybricks.robotics import DriveBase
from pybricks.tools import wait
from MBC_hue12_Lib import HUE12Sensor

hub = PrimeHub()
cam = HUE12Sensor(Port.C)
left = Motor(Port.E, Direction.COUNTERCLOCKWISE)
right = Motor(Port.F)
robot = DriveBase(left, right, wheel_diameter=56, axle_track=112)

while True:
    cam.update()
    name, hue, sat = cam.get_slot(1)
    if hue < 10 or 230 <= hue < 240:      # 紅色物塊 → 從右邊繞過
        hub.display.char("R")
        robot.turn(45)                    # 右轉 45 度（正數右轉）
        robot.straight(150)               # 直走 150 mm
        robot.turn(-45)                   # 左轉 45 度，車頭轉回原來的方向
    elif 60 <= hue <= 110:                # 綠色物塊 → 從左邊繞過
        hub.display.char("G")
        robot.turn(-45)
        robot.straight(150)
        robot.turn(45)
    else:                                 # 沒看到物塊 → 每秒 100 mm 往前
        robot.drive(100, 0)
    wait(20)
```

</details>

**一步一步看懂程式**
1. 讀第 1 格的色相。
2. **如果 紅色** → 螢幕顯示 R → **右轉 45 度 → 直走 15 公分 → 左轉 45 度**（車頭轉回原來的方向）。
3. **否則如果 綠色** → 螢幕顯示 G → 左右反過來做一次。
4. **否則**（沒看到物塊）→ 慢慢直走。
5. MBC 積木網站和 Pybricks 用「轉幾度」「走幾 mm」，會自己算好；官方 App 這裡用「走幾毫秒」（`go(100, 450)`＝右轉 0.45 秒），所以角度要自己試著調。

**跑起來應該看到什麼**：車子往前走，遇到紅色物塊往右閃開、遇到綠色物塊往左閃開，繞過去後繼續走。

**常見問題**
- **還沒繞過去就轉回來、撞到物塊？** → 把直走的距離調長（150 → 200；官方 App 1500 → 2000）。
- **官方 App 轉的角度不對？** → 改 `go(100, 450)` 和 `go(-100, 450)` 的 450：數字越大轉越多。電池電量不同也會差一點，所以比賽建議用 Pybricks 或 MBC 積木網站。
- **一直繞同一個物塊？** → 繞完後鏡頭又看到同一塊。把第 1 格的位置設低一點（靠近車頭），或把轉的角度加大。

---

## 要注意的事（請先看）

- **官方 App 的 Python 程式只做過語法檢查**：我們用電腦確認程式沒有打錯字，也用模擬的資料跑過邏輯，**但還沒有在真的 SPIKE App＋主機上逐支跑過**。讀感應器的方法（`device.data`）跟 MBC 提供的官方範例 `Hue12_demo_For_Spike_v3.4.3` 一樣；燈號、聲音、馬達的寫法如果在你的 App 版本跑出錯誤，請跟我們說。
- **在選單裡 12 格都會是 255**：程式執行中不要按感應器的側鍵。
- 每一格看的是「那一點附近同顏色的一整塊」的平均顏色。那一點如果剛好在物塊邊緣，顏色會不穩，把點移到物塊中間。
- 換了場地或燈光，顏色會變：重新檢查每一格（必要時重新設定 `TOL`）。

## 下一步

- 想同時看左、中、右三個位置：把第 1、2、3 格設在畫面左、中、右，照範例 3 的方法一格一格判斷。
- 神攝手的完整說明：[產品頁](/sensors/esp32cam/index.md)
