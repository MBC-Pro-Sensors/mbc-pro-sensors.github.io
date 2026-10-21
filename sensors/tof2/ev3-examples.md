<!-- 這頁由 95_產品網站\EV3新手範例產生器 自動產生，請改產生器再重產，不要直接改這個檔。 -->
# 🧱 EV3 新手範例：TOF2 雙眼雷射

[🔙 回到 TOF2 雙眼雷射產品頁](/sensors/tof2/index.md)

這一頁有 5 個範例，給**完全沒寫過程式**的人：每個範例都有現成的程式檔可以下載，打開就能用，下面也有一步一步的白話說明。建議從範例 1 開始照順序做。

## 開始之前

1. 感應器要用 **EV3 版**（RJ12 接頭），插在 EV3 的 **1 號孔**。
2. **EV3 先開機，再插感應器**。數字一直不變時，拔掉感應器重插一次。
3. 會動的範例用樂高教育版基本車：**左輪馬達接 B、右輪馬達接 C**。
4. 感應器在 EV3 上會顯示成「紅外線感應器」，這是正常的，讀數字就是用紅外線感應器的積木。

**每個範例都有 5 種軟體的程式檔，選你用的那一種就好：**

| 軟體 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| **EV3-G 官方軟體** | 學校電腦教室、WRO 比賽隊伍（台灣最常見） | 下載 `.ev3` 檔，在 EV3 官方軟體按「檔案 → 開啟專案」，再按右下角「下載並執行」。 |
| **EV3 Classroom** | 第一次用 EV3、想用跟 SPIKE 一樣的積木 | 下載 `.lmsp` 檔，在 EV3 Classroom 首頁按「開啟」選這個檔，接上 EV3 後按右下角「下載並執行」。 |
| **Pybricks v4** | 想學打字寫程式，用瀏覽器就能寫 | 用 Chrome 打開 [code.pybricks.com](https://code.pybricks.com)，EV3 要先裝 Pybricks 韌體（目前是 Beta 版），把 `.py` 檔拖進編輯區再按執行。 |
| **EV3 MicroPython v2.0** | 已經在用樂高官方 EV3 MicroPython 的人 | 用樂高官方「EV3 MicroPython」microSD 開機卡＋VS Code 擴充套件，把程式內容貼到 `main.py`，按 F5 下載執行。 |
| **clev3r** | 已經在用 clev3r（EV3 Basic）的人 | 在 clev3r 開啟 `.bp` 檔，按「編譯並下載」，在 EV3 的檔案選單找到 `MBC_` 開頭的資料夾執行。 |

## 這頁會用到的 3 個數值

| 名稱 | 範圍 | 從哪裡讀 |
|---|---|---|
| **左右兩眼的平均距離** | 0 ~ 100（1 格＝1.2 公分，100＝前面沒東西） | 模式 0 |
| **左眼距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 1 個值 |
| **右眼距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 2 個值 |

**範例列表：**

1. [量量看前面有多遠](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E9%87%8F%E9%87%8F%E7%9C%8B%E5%89%8D%E9%9D%A2%E6%9C%89%E5%A4%9A%E9%81%A0)
2. [前方 30 公分停車](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E5%89%8D%E6%96%B9-30-%E5%85%AC%E5%88%86%E5%81%9C%E8%BB%8A)
3. [避障車：看到東西就後退轉彎](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E9%81%BF%E9%9A%9C%E8%BB%8A%EF%BC%9A%E7%9C%8B%E5%88%B0%E6%9D%B1%E8%A5%BF%E5%B0%B1%E5%BE%8C%E9%80%80%E8%BD%89%E5%BD%8E)
4. [跟車：保持固定距離](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E8%B7%9F%E8%BB%8A%EF%BC%9A%E4%BF%9D%E6%8C%81%E5%9B%BA%E5%AE%9A%E8%B7%9D%E9%9B%A2)
5. [左眼右眼：閃開兩邊的障礙物](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E5%B7%A6%E7%9C%BC%E5%8F%B3%E7%9C%BC%EF%BC%9A%E9%96%83%E9%96%8B%E5%85%A9%E9%82%8A%E7%9A%84%E9%9A%9C%E7%A4%99%E7%89%A9)

---

## 範例 1：量量看前面有多遠

**這個範例會做什麼：** 用手在感應器前面前後移動，EV3 螢幕會顯示距離。這個範例同時顯示「感應器原始數字」和「換算成公分」。

**要準備什麼：**

- TOF2 雙眼雷射（EV3 版）
- 一個盒子或牆壁當障礙物
- EV3 主機

**怎麼接：** TOF2 雙眼雷射插 1 號孔，雷射朝前。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof2/ev3_beginner/ex01_show_distance.ev3) ｜ [⬇ EV3 Classroom](/examples/tof2/ev3_beginner/ex01_show_distance.lmsp) ｜ [⬇ Pybricks v4](/examples/tof2/ev3_beginner/ex01_show_distance_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof2/ev3_beginner/ex01_show_distance_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof2/ev3_beginner/ex01_show_distance.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex01_show_distance-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex01_show_distance-ev3g.webp" alt="TOF2 雙眼雷射 量量看前面有多遠（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex01_show_distance-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex01_show_distance-classroom.webp" alt="TOF2 雙眼雷射 量量看前面有多遠（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 01：量量看前面有多遠
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔
# 這個範例會做什麼：用手在感應器前面前後移動，EV3 螢幕會顯示距離。這個範例同時顯示「感應器原始數字」和「換算成公分」。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF2 雙眼雷射插 1 號孔


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 先清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示感應器讀到的原始數字（0~100）
    ev3.screen.draw_text(0, 0, "RAW " + str(read_dist()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示換算後的公分：數字 × 1.2
    ev3.screen.draw_text(0, 16, "CM " + str(read_dist() * 1.2) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 01：量量看前面有多遠
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF2 雙眼雷射插 1 號孔
# 這個範例會做什麼：用手在感應器前面前後移動，EV3 螢幕會顯示距離。這個範例同時顯示「感應器原始數字」和「換算成公分」。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF2 雙眼雷射插 1 號孔


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 先清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示感應器讀到的原始數字（0~100）
    ev3.screen.draw_text(0, 0, "RAW " + str(read_dist()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示換算後的公分：數字 × 1.2
    ev3.screen.draw_text(0, 16, "CM " + str(read_dist() * 1.2) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF2 雙眼雷射測距感應器 EV3 新手範例 01：量量看前面有多遠
' 軟體：clev3r
' 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：用手在感應器前面前後移動，EV3 螢幕會顯示距離。這個範例同時顯示「感應器原始數字」和「換算成公分」。
' 讀值：dist＝左右兩眼的平均距離（0 ~ 100（1 格＝1.2 公分，100＝前面沒東西））
' ============================================================
folder "prjs" "MBC_tof2_ex01"

' TOF2 雙眼雷射插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 先清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 第 1 行顯示感應器讀到的原始數字（0~100）
  LCD.Text(1, 0, 0, 1, "RAW " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 第 2 行顯示換算後的公分：數字 × 1.2
  LCD.Text(1, 0, 16, 1, "CM " + Sensor.ReadRawValue(1, 0) * 1.2 + "      ")
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 先清除螢幕
1. 一直重複
   - 第 1 行顯示感應器讀到的原始數字（0~100）
   - 第 2 行顯示換算後的公分：數字 × 1.2
   - 等 0.1 秒

**跑起來應該看到：** 手越靠近，數字越小；拿開手（前面 120 公分內沒東西）時 RAW 會是 100。

**常見問題：**

- **數字跟尺量的不一樣？** EV3 上 1 格＝1.2 公分（例如讀到 25＝30 公分），100 代表前面 120 公分內沒有東西。
- **數字一直是 0 或不變？** EV3 主機先開機再接感應器，或拔掉重插一次；確認雷射前面沒有貼保護膜。

---

## 範例 2：前方 30 公分停車

**這個範例會做什麼：** 車子往前開，看到前面 30 公分（感應器數字 25）以內有東西就停下來。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- TOF2 雙眼雷射（EV3 版）
- 一個盒子或牆壁當障礙物

**怎麼接：** TOF2 雙眼雷射插 1 號孔，裝在車頭、雷射朝正前方，離地約 5 公分。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof2/ev3_beginner/ex02_stop_before_wall.ev3) ｜ [⬇ EV3 Classroom](/examples/tof2/ev3_beginner/ex02_stop_before_wall.lmsp) ｜ [⬇ Pybricks v4](/examples/tof2/ev3_beginner/ex02_stop_before_wall_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof2/ev3_beginner/ex02_stop_before_wall_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof2/ev3_beginner/ex02_stop_before_wall.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex02_stop_before_wall-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex02_stop_before_wall-ev3g.webp" alt="TOF2 雙眼雷射 前方 30 公分停車（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex02_stop_before_wall-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex02_stop_before_wall-classroom.webp" alt="TOF2 雙眼雷射 前方 30 公分停車（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 02：前方 30 公分停車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子往前開，看到前面 30 公分（感應器數字 25）以內有東西就停下來。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 兩輪用 30 的力量往前開
left_motor.dc(30)
right_motor.dc(30)
# 等到距離小於 25（約 30 公分）
while not (read_dist() < 25):
    wait(10)
# 停車
left_motor.brake()
right_motor.brake()
# 嗶一聲
ev3.speaker.beep()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 02：前方 30 公分停車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子往前開，看到前面 30 公分（感應器數字 25）以內有東西就停下來。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 兩輪用 30 的力量往前開
left_motor.dc(30)
right_motor.dc(30)
# 等到距離小於 25（約 30 公分）
while not (read_dist() < 25):
    wait(10)
# 停車
left_motor.brake()
right_motor.brake()
# 嗶一聲
ev3.speaker.beep()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF2 雙眼雷射測距感應器 EV3 新手範例 02：前方 30 公分停車
' 軟體：clev3r
' 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：車子往前開，看到前面 30 公分（感應器數字 25）以內有東西就停下來。
' 讀值：dist＝左右兩眼的平均距離（0 ~ 100（1 格＝1.2 公分，100＝前面沒東西））
' ============================================================
folder "prjs" "MBC_tof2_ex02"

' TOF2 雙眼雷射插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 兩輪用 30 的力量往前開
Motor.StartPower("B", 30)
Motor.StartPower("C", 30)
' 等到距離小於 25（約 30 公分）
While Sensor.ReadRawValue(1, 0) >= 25
  Program.Delay(10)
EndWhile
' 停車
Motor.Stop("BC", "True")
' 嗶一聲
Speaker.Tone(50, 1000, 100)
Speaker.Wait()
```

</details>

**一步一步看懂程式：**

1. 兩輪用 30 的力量往前開
1. 等到距離小於 25（約 30 公分）
1. 停車
1. 嗶一聲

**跑起來應該看到：** 車子往前開，在障礙物前約 30 公分停下來並嗶一聲。

**常見問題：**

- **停得太晚、撞上去？** 車速越快煞車距離越長，把 30 改小，或把 25 改大。
- **數字跟尺量的不一樣？** EV3 上 1 格＝1.2 公分（例如讀到 25＝30 公分），100 代表前面 120 公分內沒有東西。

---

## 範例 3：避障車：看到東西就後退轉彎

**這個範例會做什麼：** 車子一直往前開，前面太近就後退一點、原地轉一下，再繼續往前。放在有箱子的地上，它會自己繞開。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- TOF2 雙眼雷射（EV3 版）
- 一個盒子或牆壁當障礙物

**怎麼接：** TOF2 雙眼雷射插 1 號孔，裝在車頭、雷射朝正前方，離地約 5 公分。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof2/ev3_beginner/ex03_avoid.ev3) ｜ [⬇ EV3 Classroom](/examples/tof2/ev3_beginner/ex03_avoid.lmsp) ｜ [⬇ Pybricks v4](/examples/tof2/ev3_beginner/ex03_avoid_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof2/ev3_beginner/ex03_avoid_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof2/ev3_beginner/ex03_avoid.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex03_avoid-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex03_avoid-ev3g.webp" alt="TOF2 雙眼雷射 避障車：看到東西就後退轉彎（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex03_avoid-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex03_avoid-classroom.webp" alt="TOF2 雙眼雷射 避障車：看到東西就後退轉彎（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 03：避障車：看到東西就後退轉彎
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子一直往前開，前面太近就後退一點、原地轉一下，再繼續往前。放在有箱子的地上，它會自己繞開。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果前面距離小於 25（約 30 公分）
    if read_dist() < 25:
        # 先後退
        left_motor.dc(-30)
        right_motor.dc(-30)
        # 退 0.5 秒
        wait(500)
        # 左輪往前、右輪往後：原地向右轉
        left_motor.dc(30)
        right_motor.dc(-30)
        # 轉 0.5 秒
        wait(500)
    else:
        # 前面沒東西：往前開
        left_motor.dc(30)
        right_motor.dc(30)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 03：避障車：看到東西就後退轉彎
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子一直往前開，前面太近就後退一點、原地轉一下，再繼續往前。放在有箱子的地上，它會自己繞開。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果前面距離小於 25（約 30 公分）
    if read_dist() < 25:
        # 先後退
        left_motor.dc(-30)
        right_motor.dc(-30)
        # 退 0.5 秒
        wait(500)
        # 左輪往前、右輪往後：原地向右轉
        left_motor.dc(30)
        right_motor.dc(-30)
        # 轉 0.5 秒
        wait(500)
    else:
        # 前面沒東西：往前開
        left_motor.dc(30)
        right_motor.dc(30)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF2 雙眼雷射測距感應器 EV3 新手範例 03：避障車：看到東西就後退轉彎
' 軟體：clev3r
' 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：車子一直往前開，前面太近就後退一點、原地轉一下，再繼續往前。放在有箱子的地上，它會自己繞開。
' 讀值：dist＝左右兩眼的平均距離（0 ~ 100（1 格＝1.2 公分，100＝前面沒東西））
' ============================================================
folder "prjs" "MBC_tof2_ex03"

' TOF2 雙眼雷射插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複
While "True"
  ' 如果前面距離小於 25（約 30 公分）
  If Sensor.ReadRawValue(1, 0) < 25 Then
    ' 先後退
    Motor.StartPower("B", -30)
    Motor.StartPower("C", -30)
    ' 退 0.5 秒
    Program.Delay(500)
    ' 左輪往前、右輪往後：原地向右轉
    Motor.StartPower("B", 30)
    Motor.StartPower("C", -30)
    ' 轉 0.5 秒
    Program.Delay(500)
  Else
    ' 前面沒東西：往前開
    Motor.StartPower("B", 30)
    Motor.StartPower("C", 30)
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 如果前面距離小於 25（約 30 公分）
      - 先後退
      - 退 0.5 秒
      - 左輪往前、右輪往後：原地向右轉
      - 轉 0.5 秒
      - 否則：
         - 前面沒東西：往前開

**跑起來應該看到：** 車子遇到障礙物會後退、右轉，然後繼續前進。

**常見問題：**

- **轉的角度不夠，一直撞同一個箱子？** 把轉彎的 0.5 秒改長一點（例如 0.8）。
- **數字跟尺量的不一樣？** EV3 上 1 格＝1.2 公分（例如讀到 25＝30 公分），100 代表前面 120 公分內沒有東西。

---

## 範例 4：跟車：保持固定距離

**這個範例會做什麼：** 拿一本書放在車子前面，往前移車子就跟上、往後推車子就退後，一直保持約 30 公分。用的是「差多少就補多少」的比例控制。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- TOF2 雙眼雷射（EV3 版）
- 一本書或一塊板子

**怎麼接：** TOF2 雙眼雷射插 1 號孔，裝在車頭、雷射朝正前方，離地約 5 公分。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof2/ev3_beginner/ex04_follow.ev3) ｜ [⬇ EV3 Classroom](/examples/tof2/ev3_beginner/ex04_follow.lmsp) ｜ [⬇ Pybricks v4](/examples/tof2/ev3_beginner/ex04_follow_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof2/ev3_beginner/ex04_follow_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof2/ev3_beginner/ex04_follow.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex04_follow-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex04_follow-ev3g.webp" alt="TOF2 雙眼雷射 跟車：保持固定距離（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex04_follow-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex04_follow-classroom.webp" alt="TOF2 雙眼雷射 跟車：保持固定距離（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 04：跟車：保持固定距離
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：拿一本書放在車子前面，往前移車子就跟上、往後推車子就退後，一直保持約 30 公分。用的是「差多少就補多少」的比例控制。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 馬達力量 power ＝（距離 − 25）× 2：太遠是正數往前、太近是負數往後
    power = (read_dist() - 25) * 2
    # 如果 power 大於 50
    if power > 50:
        # 最快只用 50，比較安全
        power = 50
    # 兩輪用 power 一起前進或後退
    left_motor.dc(power)
    right_motor.dc(power)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 04：跟車：保持固定距離
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：拿一本書放在車子前面，往前移車子就跟上、往後推車子就退後，一直保持約 30 公分。用的是「差多少就補多少」的比例控制。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 馬達力量 power ＝（距離 − 25）× 2：太遠是正數往前、太近是負數往後
    power = (read_dist() - 25) * 2
    # 如果 power 大於 50
    if power > 50:
        # 最快只用 50，比較安全
        power = 50
    # 兩輪用 power 一起前進或後退
    left_motor.dc(power)
    right_motor.dc(power)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF2 雙眼雷射測距感應器 EV3 新手範例 04：跟車：保持固定距離
' 軟體：clev3r
' 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：拿一本書放在車子前面，往前移車子就跟上、往後推車子就退後，一直保持約 30 公分。用的是「差多少就補多少」的比例控制。
' 讀值：dist＝左右兩眼的平均距離（0 ~ 100（1 格＝1.2 公分，100＝前面沒東西））
' ============================================================
folder "prjs" "MBC_tof2_ex04"

' TOF2 雙眼雷射插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複
While "True"
  ' 馬達力量 power ＝（距離 − 25）× 2：太遠是正數往前、太近是負數往後
  power = (Sensor.ReadRawValue(1, 0) - 25) * 2
  ' 如果 power 大於 50
  If power > 50 Then
    ' 最快只用 50，比較安全
    power = 50
  EndIf
  ' 兩輪用 power 一起前進或後退
  Motor.StartPower("B", power)
  Motor.StartPower("C", power)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 馬達力量 power ＝（距離 − 25）× 2：太遠是正數往前、太近是負數往後
   - 如果 power 大於 50
      - 最快只用 50，比較安全
   - 兩輪用 power 一起前進或後退

**跑起來應該看到：** 書往前，車子跟上；書往後，車子後退；書不動時車子在約 30 公分處停住。前面完全沒東西時車子會用 50 的力量往前開。

**常見問題：**

- **車子在目標前後來回抖？** 把 × 2 改小（例如 × 1）。
- **數字跟尺量的不一樣？** EV3 上 1 格＝1.2 公分（例如讀到 25＝30 公分），100 代表前面 120 公分內沒有東西。

---

## 範例 5：左眼右眼：閃開兩邊的障礙物

**這個範例會做什麼：** 雙眼雷射可以分別讀左眼和右眼。左邊太近就往右閃，右邊太近就往左閃，都沒東西就直走。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- TOF2 雙眼雷射（EV3 版）
- 一個盒子或牆壁當障礙物

**怎麼接：** TOF2 雙眼雷射插 1 號孔，裝在車頭、雷射朝正前方，離地約 5 公分。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof2/ev3_beginner/ex05_left_right.ev3) ｜ [⬇ EV3 Classroom](/examples/tof2/ev3_beginner/ex05_left_right.lmsp) ｜ [⬇ Pybricks v4](/examples/tof2/ev3_beginner/ex05_left_right_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof2/ev3_beginner/ex05_left_right_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof2/ev3_beginner/ex05_left_right.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex05_left_right-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex05_left_right-ev3g.webp" alt="TOF2 雙眼雷射 左眼右眼：閃開兩邊的障礙物（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof2/ev3-beginner/ex05_left_right-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof2/ev3-beginner/ex05_left_right-classroom.webp" alt="TOF2 雙眼雷射 左眼右眼：閃開兩邊的障礙物（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 05：左眼右眼：閃開兩邊的障礙物
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：雙眼雷射可以分別讀左眼和右眼。左邊太近就往右閃，右邊太近就往左閃，都沒東西就直走。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_left():
    # 左眼距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[0]


def read_right():
    # 右眼距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[1]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 螢幕第 1 行顯示左眼距離
    ev3.screen.draw_text(0, 0, "LEFT " + str(read_left()) + "      ", Color.BLACK, Color.WHITE)
    # 螢幕第 2 行顯示右眼距離
    ev3.screen.draw_text(0, 16, "RIGHT " + str(read_right()) + "      ", Color.BLACK, Color.WHITE)
    # 如果左眼距離小於 25
    if read_left() < 25:
        # 左邊太近：左輪轉、右輪停，車子往右閃
        left_motor.dc(30)
        right_motor.dc(0)
    # 否則，如果右眼距離小於 25
    elif read_right() < 25:
        # 右邊太近：往左閃
        left_motor.dc(0)
        right_motor.dc(30)
    else:
        # 兩邊都沒東西：直走
        left_motor.dc(30)
        right_motor.dc(30)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 05：左眼右眼：閃開兩邊的障礙物
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：雙眼雷射可以分別讀左眼和右眼。左邊太近就往右閃，右邊太近就往左閃，都沒東西就直走。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_left():
    # 左眼距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[0]


def read_right():
    # 右眼距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[1]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 螢幕第 1 行顯示左眼距離
    ev3.screen.draw_text(0, 0, "LEFT " + str(read_left()) + "      ", Color.BLACK, Color.WHITE)
    # 螢幕第 2 行顯示右眼距離
    ev3.screen.draw_text(0, 16, "RIGHT " + str(read_right()) + "      ", Color.BLACK, Color.WHITE)
    # 如果左眼距離小於 25
    if read_left() < 25:
        # 左邊太近：左輪轉、右輪停，車子往右閃
        left_motor.dc(30)
        right_motor.dc(0)
    # 否則，如果右眼距離小於 25
    elif read_right() < 25:
        # 右邊太近：往左閃
        left_motor.dc(0)
        right_motor.dc(30)
    else:
        # 兩邊都沒東西：直走
        left_motor.dc(30)
        right_motor.dc(30)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF2 雙眼雷射測距感應器 EV3 新手範例 05：左眼右眼：閃開兩邊的障礙物
' 軟體：clev3r
' 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：雙眼雷射可以分別讀左眼和右眼。左邊太近就往右閃，右邊太近就往左閃，都沒東西就直走。
' 讀值：left＝左眼距離（0 ~ 100（1 格＝1.2 公分））；right＝右眼距離（0 ~ 100（1 格＝1.2 公分））
' ============================================================
folder "prjs" "MBC_tof2_ex05"

' TOF2 雙眼雷射插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 一直重複
While "True"
  ' 螢幕第 1 行顯示左眼距離
  LCD.Text(1, 0, 0, 1, "LEFT " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 螢幕第 2 行顯示右眼距離
  LCD.Text(1, 0, 16, 1, "RIGHT " + Sensor.ReadRawValue(1, 1) + "      ")
  ' 如果左眼距離小於 25
  If Sensor.ReadRawValue(1, 0) < 25 Then
    ' 左邊太近：左輪轉、右輪停，車子往右閃
    Motor.StartPower("B", 30)
    Motor.StartPower("C", 0)
  ' 否則，如果右眼距離小於 25
  ElseIf Sensor.ReadRawValue(1, 1) < 25 Then
    ' 右邊太近：往左閃
    Motor.StartPower("B", 0)
    Motor.StartPower("C", 30)
  Else
    ' 兩邊都沒東西：直走
    Motor.StartPower("B", 30)
    Motor.StartPower("C", 30)
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 螢幕第 1 行顯示左眼距離
   - 螢幕第 2 行顯示右眼距離
   - 如果左眼距離小於 25
      - 左邊太近：左輪轉、右輪停，車子往右閃
      - 否則：
         - 否則，如果右眼距離小於 25
            - 右邊太近：往左閃
            - 否則：
               - 兩邊都沒東西：直走

**跑起來應該看到：** 用手擋左眼，車子往右轉；擋右眼，往左轉；都不擋就直走。

**常見問題：**

- **左右顛倒？** 感應器可能裝反了（雷射頭朝後或上下顛倒），或把程式裡兩個 tank 的數字對調。
- **數字跟尺量的不一樣？** EV3 上 1 格＝1.2 公分（例如讀到 25＝30 公分），100 代表前面 120 公分內沒有東西。

---

還有問題？歡迎到 [聯絡我們](/contact.md) 留言，附上你用的軟體和程式截圖，我們會幫你看。
