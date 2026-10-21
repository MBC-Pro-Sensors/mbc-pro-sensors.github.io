<!-- 這頁由 95_產品網站\EV3新手範例產生器 自動產生，請改產生器再重產，不要直接改這個檔。 -->
# 🧱 EV3 新手範例：TOF8 八方位雷射

[🔙 回到 TOF8 八方位雷射產品頁](/sensors/tof8/index.md)

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

## 這頁會用到的 12 個數值

| 名稱 | 範圍 | 從哪裡讀 |
|---|---|---|
| **正前方（第 4、5 顆平均）距離** | 0 ~ 100（1 格＝1.2 公分，100＝前面沒東西） | 模式 0 |
| **第 1 顆雷射的距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 1 個值 |
| **第 2 顆雷射的距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 2 個值 |
| **第 3 顆雷射的距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 3 個值 |
| **第 4 顆雷射的距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 4 個值 |
| **第 5 顆雷射的距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 5 個值 |
| **第 6 顆雷射的距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 6 個值 |
| **第 7 顆雷射的距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 7 個值 |
| **第 8 顆雷射的距離** | 0 ~ 100（1 格＝1.2 公分） | 模式 1 第 8 個值 |
| **最近的東西有多遠** | 0 ~ 100 | 模式 2 第 2 個值 |
| **最近的東西在哪個方向** | -7 ~ +7（負＝左、正＝右、0 附近＝正前方；±8＝沒有東西） | 模式 2 第 1 個值 |
| **最空曠的方向** | -7 ~ +7（負＝左、正＝右） | 模式 2 第 3 個值 |

**範例列表：**

1. [量量看正前方有多遠](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E9%87%8F%E9%87%8F%E7%9C%8B%E6%AD%A3%E5%89%8D%E6%96%B9%E6%9C%89%E5%A4%9A%E9%81%A0)
2. [前方 30 公分停車](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E5%89%8D%E6%96%B9-30-%E5%85%AC%E5%88%86%E5%81%9C%E8%BB%8A)
3. [一次看 8 個方向的距離](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E4%B8%80%E6%AC%A1%E7%9C%8B-8-%E5%80%8B%E6%96%B9%E5%90%91%E7%9A%84%E8%B7%9D%E9%9B%A2)
4. [相撲追擊：轉向最近的東西衝過去](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E7%9B%B8%E6%92%B2%E8%BF%BD%E6%93%8A%EF%BC%9A%E8%BD%89%E5%90%91%E6%9C%80%E8%BF%91%E7%9A%84%E6%9D%B1%E8%A5%BF%E8%A1%9D%E9%81%8E%E5%8E%BB)
5. [往最空曠的方向走](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E5%BE%80%E6%9C%80%E7%A9%BA%E6%9B%A0%E7%9A%84%E6%96%B9%E5%90%91%E8%B5%B0)

---

## 範例 1：量量看正前方有多遠

**這個範例會做什麼：** 八方位雷射的模式 0 會給你正前方（中間兩顆）的平均距離，最簡單好用。

**要準備什麼：**

- TOF8 八方位雷射（EV3 版）
- EV3 主機

**怎麼接：** TOF8 八方位雷射插 1 號孔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof8/ev3_beginner/ex01_show_front.ev3) ｜ [⬇ EV3 Classroom](/examples/tof8/ev3_beginner/ex01_show_front.lmsp) ｜ [⬇ Pybricks v4](/examples/tof8/ev3_beginner/ex01_show_front_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof8/ev3_beginner/ex01_show_front_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof8/ev3_beginner/ex01_show_front.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex01_show_front-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex01_show_front-ev3g.webp" alt="TOF8 八方位雷射 量量看正前方有多遠（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex01_show_front-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex01_show_front-classroom.webp" alt="TOF8 八方位雷射 量量看正前方有多遠（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 01：量量看正前方有多遠
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF8 八方位雷射插 1 號孔
# 這個範例會做什麼：八方位雷射的模式 0 會給你正前方（中間兩顆）的平均距離，最簡單好用。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF8 八方位雷射插 1 號孔


def read_front():
    # 正前方（第 4、5 顆平均）距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示正前方距離（0~100）
    ev3.screen.draw_text(0, 0, "FRONT " + str(read_front()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行換算成公分：× 1.2
    ev3.screen.draw_text(0, 16, "CM " + str(read_front() * 1.2) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 01：量量看正前方有多遠
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF8 八方位雷射插 1 號孔
# 這個範例會做什麼：八方位雷射的模式 0 會給你正前方（中間兩顆）的平均距離，最簡單好用。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF8 八方位雷射插 1 號孔


def read_front():
    # 正前方（第 4、5 顆平均）距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示正前方距離（0~100）
    ev3.screen.draw_text(0, 0, "FRONT " + str(read_front()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行換算成公分：× 1.2
    ev3.screen.draw_text(0, 16, "CM " + str(read_front() * 1.2) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF8 八方位雷射測距感應器 EV3 新手範例 01：量量看正前方有多遠
' 軟體：clev3r
' 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：八方位雷射的模式 0 會給你正前方（中間兩顆）的平均距離，最簡單好用。
' 讀值：front＝正前方（第 4、5 顆平均）距離（0 ~ 100（1 格＝1.2 公分，100＝前面沒東西））
' ============================================================
folder "prjs" "MBC_tof8_ex01"

' TOF8 八方位雷射插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 第 1 行顯示正前方距離（0~100）
  LCD.Text(1, 0, 0, 1, "FRONT " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 第 2 行換算成公分：× 1.2
  LCD.Text(1, 0, 16, 1, "CM " + Sensor.ReadRawValue(1, 0) * 1.2 + "      ")
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 一直重複
   - 第 1 行顯示正前方距離（0~100）
   - 第 2 行換算成公分：× 1.2
   - 等 0.1 秒

**跑起來應該看到：** 手在正前方移動，數字跟著變；前面沒東西時是 100。

**常見問題：**

- **數字跟尺量的不一樣？** EV3 上 1 格＝1.2 公分（例如讀到 25＝30 公分），100 代表前面 120 公分內沒有東西。
- **數字一直是 0 或不變？** EV3 主機先開機再接感應器，或拔掉重插一次；確認雷射前面沒有貼保護膜。

---

## 範例 2：前方 30 公分停車

**這個範例會做什麼：** 車子往前開，正前方 30 公分（數字 25）以內有東西就停下來。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- TOF8 八方位雷射（EV3 版）
- 幾個盒子當障礙物

**怎麼接：** TOF8 八方位雷射插 1 號孔，裝在車頭、第 4、5 顆雷射朝正前方，離地約 5 公分。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof8/ev3_beginner/ex02_stop_before_wall.ev3) ｜ [⬇ EV3 Classroom](/examples/tof8/ev3_beginner/ex02_stop_before_wall.lmsp) ｜ [⬇ Pybricks v4](/examples/tof8/ev3_beginner/ex02_stop_before_wall_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof8/ev3_beginner/ex02_stop_before_wall_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof8/ev3_beginner/ex02_stop_before_wall.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex02_stop_before_wall-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex02_stop_before_wall-ev3g.webp" alt="TOF8 八方位雷射 前方 30 公分停車（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex02_stop_before_wall-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex02_stop_before_wall-classroom.webp" alt="TOF8 八方位雷射 前方 30 公分停車（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 02：前方 30 公分停車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子往前開，正前方 30 公分（數字 25）以內有東西就停下來。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF8 八方位雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_front():
    # 正前方（第 4、5 顆平均）距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 兩輪往前開
left_motor.dc(30)
right_motor.dc(30)
# 等到正前方距離小於 25
while not (read_front() < 25):
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
# TOF8 八方位雷射測距感應器 EV3 新手範例 02：前方 30 公分停車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子往前開，正前方 30 公分（數字 25）以內有東西就停下來。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF8 八方位雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_front():
    # 正前方（第 4、5 顆平均）距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 兩輪往前開
left_motor.dc(30)
right_motor.dc(30)
# 等到正前方距離小於 25
while not (read_front() < 25):
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
' TOF8 八方位雷射測距感應器 EV3 新手範例 02：前方 30 公分停車
' 軟體：clev3r
' 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：車子往前開，正前方 30 公分（數字 25）以內有東西就停下來。
' 讀值：front＝正前方（第 4、5 顆平均）距離（0 ~ 100（1 格＝1.2 公分，100＝前面沒東西））
' ============================================================
folder "prjs" "MBC_tof8_ex02"

' TOF8 八方位雷射插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 兩輪往前開
Motor.StartPower("B", 30)
Motor.StartPower("C", 30)
' 等到正前方距離小於 25
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

1. 兩輪往前開
1. 等到正前方距離小於 25
1. 停車
1. 嗶一聲

**跑起來應該看到：** 車子在障礙物前約 30 公分停下來並嗶一聲。

**常見問題：**

- **數字跟尺量的不一樣？** EV3 上 1 格＝1.2 公分（例如讀到 25＝30 公分），100 代表前面 120 公分內沒有東西。

---

## 範例 3：一次看 8 個方向的距離

**這個範例會做什麼：** 模式 1 會一次送出 8 顆雷射各自的距離。這個範例把 8 個數字都顯示在螢幕上，拿手在四周晃一晃，就知道每顆雷射對應哪個方向。

**要準備什麼：**

- TOF8 八方位雷射（EV3 版）
- EV3 主機

**怎麼接：** TOF8 八方位雷射插 1 號孔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof8/ev3_beginner/ex03_show_all8.ev3) ｜ [⬇ EV3 Classroom](/examples/tof8/ev3_beginner/ex03_show_all8.lmsp) ｜ [⬇ Pybricks v4](/examples/tof8/ev3_beginner/ex03_show_all8_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof8/ev3_beginner/ex03_show_all8_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof8/ev3_beginner/ex03_show_all8.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex03_show_all8-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex03_show_all8-ev3g.webp" alt="TOF8 八方位雷射 一次看 8 個方向的距離（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex03_show_all8-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex03_show_all8-classroom.webp" alt="TOF8 八方位雷射 一次看 8 個方向的距離（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 03：一次看 8 個方向的距離
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF8 八方位雷射插 1 號孔
# 這個範例會做什麼：模式 1 會一次送出 8 顆雷射各自的距離。這個範例把 8 個數字都顯示在螢幕上，拿手在四周晃一晃，就知道每顆雷射對應哪個方向。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF8 八方位雷射插 1 號孔


def read_d1():
    # 第 1 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[0]


def read_d2():
    # 第 2 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[1]


def read_d3():
    # 第 3 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[2]


def read_d4():
    # 第 4 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[3]


def read_d5():
    # 第 5 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[4]


def read_d6():
    # 第 6 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[5]


def read_d7():
    # 第 7 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[6]


def read_d8():
    # 第 8 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read(1)[7]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示第 1 顆雷射的距離
    ev3.screen.draw_text(0, 0, "D1 " + str(read_d1()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示第 2 顆雷射的距離
    ev3.screen.draw_text(0, 16, "D2 " + str(read_d2()) + "      ", Color.BLACK, Color.WHITE)
    # 第 3 行顯示第 3 顆雷射的距離
    ev3.screen.draw_text(0, 32, "D3 " + str(read_d3()) + "      ", Color.BLACK, Color.WHITE)
    # 第 4 行顯示第 4 顆雷射的距離
    ev3.screen.draw_text(0, 48, "D4 " + str(read_d4()) + "      ", Color.BLACK, Color.WHITE)
    # 第 5 行顯示第 5 顆雷射的距離
    ev3.screen.draw_text(0, 64, "D5 " + str(read_d5()) + "      ", Color.BLACK, Color.WHITE)
    # 第 6 行顯示第 6 顆雷射的距離
    ev3.screen.draw_text(0, 80, "D6 " + str(read_d6()) + "      ", Color.BLACK, Color.WHITE)
    # 第 7 行顯示第 7 顆雷射的距離
    ev3.screen.draw_text(0, 96, "D7 " + str(read_d7()) + "      ", Color.BLACK, Color.WHITE)
    # 第 8 行顯示第 8 顆雷射的距離
    ev3.screen.draw_text(0, 112, "D8 " + str(read_d8()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 03：一次看 8 個方向的距離
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF8 八方位雷射插 1 號孔
# 這個範例會做什麼：模式 1 會一次送出 8 顆雷射各自的距離。這個範例把 8 個數字都顯示在螢幕上，拿手在四周晃一晃，就知道每顆雷射對應哪個方向。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF8 八方位雷射插 1 號孔


def read_d1():
    # 第 1 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[0]


def read_d2():
    # 第 2 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[1]


def read_d3():
    # 第 3 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[2]


def read_d4():
    # 第 4 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[3]


def read_d5():
    # 第 5 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[4]


def read_d6():
    # 第 6 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[5]


def read_d7():
    # 第 7 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[6]


def read_d8():
    # 第 8 顆雷射的距離：0 ~ 100（1 格＝1.2 公分）
    return sensor.read('IR-SEEK')[7]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示第 1 顆雷射的距離
    ev3.screen.draw_text(0, 0, "D1 " + str(read_d1()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示第 2 顆雷射的距離
    ev3.screen.draw_text(0, 16, "D2 " + str(read_d2()) + "      ", Color.BLACK, Color.WHITE)
    # 第 3 行顯示第 3 顆雷射的距離
    ev3.screen.draw_text(0, 32, "D3 " + str(read_d3()) + "      ", Color.BLACK, Color.WHITE)
    # 第 4 行顯示第 4 顆雷射的距離
    ev3.screen.draw_text(0, 48, "D4 " + str(read_d4()) + "      ", Color.BLACK, Color.WHITE)
    # 第 5 行顯示第 5 顆雷射的距離
    ev3.screen.draw_text(0, 64, "D5 " + str(read_d5()) + "      ", Color.BLACK, Color.WHITE)
    # 第 6 行顯示第 6 顆雷射的距離
    ev3.screen.draw_text(0, 80, "D6 " + str(read_d6()) + "      ", Color.BLACK, Color.WHITE)
    # 第 7 行顯示第 7 顆雷射的距離
    ev3.screen.draw_text(0, 96, "D7 " + str(read_d7()) + "      ", Color.BLACK, Color.WHITE)
    # 第 8 行顯示第 8 顆雷射的距離
    ev3.screen.draw_text(0, 112, "D8 " + str(read_d8()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF8 八方位雷射測距感應器 EV3 新手範例 03：一次看 8 個方向的距離
' 軟體：clev3r
' 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 1 會一次送出 8 顆雷射各自的距離。這個範例把 8 個數字都顯示在螢幕上，拿手在四周晃一晃，就知道每顆雷射對應哪個方向。
' 讀值：d1＝第 1 顆雷射的距離（0 ~ 100（1 格＝1.2 公分））；d2＝第 2 顆雷射的距離（0 ~ 100（1 格＝1.2 公分））；d3＝第 3 顆雷射的距離（0 ~ 100（1 格＝1.2 公分））；d4＝第 4 顆雷射的距離（0 ~ 100（1 格＝1.2 公分））；d5＝第 5 顆雷射的距離（0 ~ 100（1 格＝1.2 公分））；d6＝第 6 顆雷射的距離（0 ~ 100（1 格＝1.2 公分））；d7＝第 7 顆雷射的距離（0 ~ 100（1 格＝1.2 公分））；d8＝第 8 顆雷射的距離（0 ~ 100（1 格＝1.2 公分））
' ============================================================
folder "prjs" "MBC_tof8_ex03"

' TOF8 八方位雷射插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 第 1 行顯示第 1 顆雷射的距離
  LCD.Text(1, 0, 0, 1, "D1 " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 第 2 行顯示第 2 顆雷射的距離
  LCD.Text(1, 0, 16, 1, "D2 " + Sensor.ReadRawValue(1, 1) + "      ")
  ' 第 3 行顯示第 3 顆雷射的距離
  LCD.Text(1, 0, 32, 1, "D3 " + Sensor.ReadRawValue(1, 2) + "      ")
  ' 第 4 行顯示第 4 顆雷射的距離
  LCD.Text(1, 0, 48, 1, "D4 " + Sensor.ReadRawValue(1, 3) + "      ")
  ' 第 5 行顯示第 5 顆雷射的距離
  LCD.Text(1, 0, 64, 1, "D5 " + Sensor.ReadRawValue(1, 4) + "      ")
  ' 第 6 行顯示第 6 顆雷射的距離
  LCD.Text(1, 0, 80, 1, "D6 " + Sensor.ReadRawValue(1, 5) + "      ")
  ' 第 7 行顯示第 7 顆雷射的距離
  LCD.Text(1, 0, 96, 1, "D7 " + Sensor.ReadRawValue(1, 6) + "      ")
  ' 第 8 行顯示第 8 顆雷射的距離
  LCD.Text(1, 0, 112, 1, "D8 " + Sensor.ReadRawValue(1, 7) + "      ")
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 一直重複
   - 第 1 行顯示第 1 顆雷射的距離
   - 第 2 行顯示第 2 顆雷射的距離
   - 第 3 行顯示第 3 顆雷射的距離
   - 第 4 行顯示第 4 顆雷射的距離
   - 第 5 行顯示第 5 顆雷射的距離
   - 第 6 行顯示第 6 顆雷射的距離
   - 第 7 行顯示第 7 顆雷射的距離
   - 第 8 行顯示第 8 顆雷射的距離
   - 等 0.1 秒

**跑起來應該看到：** 8 行數字分別對應 8 顆雷射，手靠近哪一顆，那一行數字就變小。

**常見問題：**

- **數字跟尺量的不一樣？** EV3 上 1 格＝1.2 公分（例如讀到 25＝30 公分），100 代表前面 120 公分內沒有東西。
- **第 1 顆在左邊還是右邊？** 請用手實際試一次並記下來，之後寫程式會用到。

---

## 範例 4：相撲追擊：轉向最近的東西衝過去

**這個範例會做什麼：** 模式 2 會直接告訴你「最近的東西在哪個方向、有多遠」，不用自己比 8 個數字。車子轉向最近的東西衝過去，看不到東西時原地轉圈找。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- TOF8 八方位雷射（EV3 版）
- 一個罐子當對手

**怎麼接：** TOF8 八方位雷射插 1 號孔，裝在車頭、第 4、5 顆雷射朝正前方，離地約 5 公分。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof8/ev3_beginner/ex04_sumo.ev3) ｜ [⬇ EV3 Classroom](/examples/tof8/ev3_beginner/ex04_sumo.lmsp) ｜ [⬇ Pybricks v4](/examples/tof8/ev3_beginner/ex04_sumo_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof8/ev3_beginner/ex04_sumo_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof8/ev3_beginner/ex04_sumo.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex04_sumo-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex04_sumo-ev3g.webp" alt="TOF8 八方位雷射 相撲追擊：轉向最近的東西衝過去（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex04_sumo-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex04_sumo-classroom.webp" alt="TOF8 八方位雷射 相撲追擊：轉向最近的東西衝過去（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 04：相撲追擊：轉向最近的東西衝過去
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 2 會直接告訴你「最近的東西在哪個方向、有多遠」，不用自己比 8 個數字。車子轉向最近的東西衝過去，看不到東西時原地轉圈找。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF8 八方位雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_nearDist():
    # 最近的東西有多遠：0 ~ 100
    return sensor.read(2)[1]


def read_nearDir():
    # 最近的東西在哪個方向：-7 ~ +7（負＝左、正＝右、0 附近＝正前方；±8＝沒有東西）
    return sensor.read(2)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果最近的東西在 60 以內（約 72 公分）
    if read_nearDist() < 60:
        # 如果方向小於 -1（偏左）
        if read_nearDir() < (-1):
            # 東西在左邊：原地往左轉
            left_motor.dc(-20)
            right_motor.dc(20)
        # 否則，如果方向大於 1（偏右）
        elif read_nearDir() > 1:
            # 東西在右邊：原地往右轉
            left_motor.dc(20)
            right_motor.dc(-20)
        else:
            # 東西在正前方：全力衝過去
            left_motor.dc(60)
            right_motor.dc(60)
    else:
        # 附近沒東西：原地轉圈找對手
        left_motor.dc(25)
        right_motor.dc(-25)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 04：相撲追擊：轉向最近的東西衝過去
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 2 會直接告訴你「最近的東西在哪個方向、有多遠」，不用自己比 8 個數字。車子轉向最近的東西衝過去，看不到東西時原地轉圈找。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF8 八方位雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_nearDist():
    # 最近的東西有多遠：0 ~ 100
    return sensor.read('IR-REMOTE')[1]


def read_nearDir():
    # 最近的東西在哪個方向：-7 ~ +7（負＝左、正＝右、0 附近＝正前方；±8＝沒有東西）
    return sensor.read('IR-REMOTE')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果最近的東西在 60 以內（約 72 公分）
    if read_nearDist() < 60:
        # 如果方向小於 -1（偏左）
        if read_nearDir() < (-1):
            # 東西在左邊：原地往左轉
            left_motor.dc(-20)
            right_motor.dc(20)
        # 否則，如果方向大於 1（偏右）
        elif read_nearDir() > 1:
            # 東西在右邊：原地往右轉
            left_motor.dc(20)
            right_motor.dc(-20)
        else:
            # 東西在正前方：全力衝過去
            left_motor.dc(60)
            right_motor.dc(60)
    else:
        # 附近沒東西：原地轉圈找對手
        left_motor.dc(25)
        right_motor.dc(-25)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF8 八方位雷射測距感應器 EV3 新手範例 04：相撲追擊：轉向最近的東西衝過去
' 軟體：clev3r
' 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 2 會直接告訴你「最近的東西在哪個方向、有多遠」，不用自己比 8 個數字。車子轉向最近的東西衝過去，看不到東西時原地轉圈找。
' 讀值：nearDist＝最近的東西有多遠（0 ~ 100）；nearDir＝最近的東西在哪個方向（-7 ~ +7（負＝左、正＝右、0 附近＝正前方；±8＝沒有東西））
' ============================================================
folder "prjs" "MBC_tof8_ex04"

' TOF8 八方位雷射插 1 號孔，使用模式 2
Sensor.SetMode(1, 2)
LCD.Clear()

' 一直重複
While "True"
  ' 如果最近的東西在 60 以內（約 72 公分）
  If Sensor.ReadRawValue(1, 1) < 60 Then
    ' 如果方向小於 -1（偏左）
    If Sensor.ReadRawValue(1, 0) < (-1) Then
      ' 東西在左邊：原地往左轉
      Motor.StartPower("B", -20)
      Motor.StartPower("C", 20)
    ' 否則，如果方向大於 1（偏右）
    ElseIf Sensor.ReadRawValue(1, 0) > 1 Then
      ' 東西在右邊：原地往右轉
      Motor.StartPower("B", 20)
      Motor.StartPower("C", -20)
    Else
      ' 東西在正前方：全力衝過去
      Motor.StartPower("B", 60)
      Motor.StartPower("C", 60)
    EndIf
  Else
    ' 附近沒東西：原地轉圈找對手
    Motor.StartPower("B", 25)
    Motor.StartPower("C", -25)
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 如果最近的東西在 60 以內（約 72 公分）
      - 如果方向小於 -1（偏左）
         - 東西在左邊：原地往左轉
         - 否則：
            - 否則，如果方向大於 1（偏右）
               - 東西在右邊：原地往右轉
               - 否則：
                  - 東西在正前方：全力衝過去
      - 否則：
         - 附近沒東西：原地轉圈找對手

**跑起來應該看到：** 把罐子放在車子周圍，車子會轉向罐子並衝過去推它。

**常見問題：**

- **方向數字是 8 或 -8？** ±8 代表那個方向 120 公分內沒有東西，不是「最左」「最右」。
- **轉過頭來回擺？** 把轉向的 20 改小，或把 -1 和 1 改成 -2 和 2。

---

## 範例 5：往最空曠的方向走

**這個範例會做什麼：** 模式 2 也會告訴你「最空曠的方向」。車子用比例控制一直往最空的地方開，在箱子陣裡會自己找出口。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- TOF8 八方位雷射（EV3 版）
- 幾個盒子當障礙物

**怎麼接：** TOF8 八方位雷射插 1 號孔，裝在車頭、第 4、5 顆雷射朝正前方，離地約 5 公分。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/tof8/ev3_beginner/ex05_go_open.ev3) ｜ [⬇ EV3 Classroom](/examples/tof8/ev3_beginner/ex05_go_open.lmsp) ｜ [⬇ Pybricks v4](/examples/tof8/ev3_beginner/ex05_go_open_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/tof8/ev3_beginner/ex05_go_open_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/tof8/ev3_beginner/ex05_go_open.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex05_go_open-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex05_go_open-ev3g.webp" alt="TOF8 八方位雷射 往最空曠的方向走（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/tof8/ev3-beginner/ex05_go_open-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/tof8/ev3-beginner/ex05_go_open-classroom.webp" alt="TOF8 八方位雷射 往最空曠的方向走（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 05：往最空曠的方向走
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 2 也會告訴你「最空曠的方向」。車子用比例控制一直往最空的地方開，在箱子陣裡會自己找出口。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # TOF8 八方位雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_farDir():
    # 最空曠的方向：-7 ~ +7（負＝左、正＝右）
    return sensor.read(2)[2]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 轉彎力量 turn ＝ 最空方向 × 5（負數往左、正數往右）
    turn = read_farDir() * 5
    # 左輪＝25＋turn、右輪＝25－turn
    left_motor.dc(25 + turn)
    right_motor.dc(25 - turn)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 05：往最空曠的方向走
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 2 也會告訴你「最空曠的方向」。車子用比例控制一直往最空的地方開，在箱子陣裡會自己找出口。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF8 八方位雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_farDir():
    # 最空曠的方向：-7 ~ +7（負＝左、正＝右）
    return sensor.read('IR-REMOTE')[2]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 轉彎力量 turn ＝ 最空方向 × 5（負數往左、正數往右）
    turn = read_farDir() * 5
    # 左輪＝25＋turn、右輪＝25－turn
    left_motor.dc(25 + turn)
    right_motor.dc(25 - turn)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' TOF8 八方位雷射測距感應器 EV3 新手範例 05：往最空曠的方向走
' 軟體：clev3r
' 接法：TOF8 八方位雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 2 也會告訴你「最空曠的方向」。車子用比例控制一直往最空的地方開，在箱子陣裡會自己找出口。
' 讀值：farDir＝最空曠的方向（-7 ~ +7（負＝左、正＝右））
' ============================================================
folder "prjs" "MBC_tof8_ex05"

' TOF8 八方位雷射插 1 號孔，使用模式 2
Sensor.SetMode(1, 2)
LCD.Clear()

' 一直重複
While "True"
  ' 轉彎力量 turn ＝ 最空方向 × 5（負數往左、正數往右）
  turn = Sensor.ReadRawValue(1, 2) * 5
  ' 左輪＝25＋turn、右輪＝25－turn
  Motor.StartPower("B", 25 + turn)
  Motor.StartPower("C", 25 - turn)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 轉彎力量 turn ＝ 最空方向 × 5（負數往左、正數往右）
   - 左輪＝25＋turn、右輪＝25－turn

**跑起來應該看到：** 車子會避開箱子，往空的地方前進。

**常見問題：**

- **方向數字是 8 或 -8？** ±8 代表那個方向 120 公分內沒有東西，不是「最左」「最右」。
- **一直往牆壁開？** 方向的正負可能跟車子相反，把程式裡的「＋turn」和「－turn」對調。

---

還有問題？歡迎到 [聯絡我們](/contact.md) 留言，附上你用的軟體和程式截圖，我們會幫你看。
