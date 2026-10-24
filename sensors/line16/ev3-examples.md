<!-- 這頁由 95_產品網站\EV3新手範例產生器 自動產生，請改產生器再重產，不要直接改這個檔。 -->
<!-- moved: /sensors/line16/ev3/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/line16/ev3/index.md">循行者 16 路 › EV3</a></p>

# 🧱 EV3 新手範例：循行者 16 路

[🔙 回到 循行者 16 路產品頁](/sensors/line16/index.md)

這一頁有 6 個範例，給**完全沒寫過程式**的人：每個範例都有現成的程式檔可以下載，打開就能用，下面也有一步一步的白話說明。建議從範例 1 開始照順序做。

## 開始之前

1. 感應器要用 **EV3 版**（RJ12 接頭），插在 EV3 的 **1 號孔**。
2. **EV3 先開機，再插感應器**。數字一直不變時，拔掉感應器重插一次。
3. 會動的範例用樂高教育版基本車：**左輪馬達接 B、右輪馬達接 C**。
4. 感應器在 EV3 上會顯示成「紅外線感應器」，這是正常的，讀數字就是用紅外線感應器的積木。

**每個範例都有 5 種軟體的程式檔，選你用的那一種就好：**（標「僅 Pybricks／EV3 MicroPython／clev3r」的範例只有這 3 種，因為 EV3-G、Classroom 的積木讀不到那個模式）

| 軟體 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| **EV3-G 官方軟體** | 學校電腦教室、WRO 比賽隊伍（台灣最常見） | 下載 `.ev3` 檔，在 EV3 官方軟體按「檔案 → 開啟專案」，再按右下角「下載並執行」。 |
| **EV3 Classroom** | 第一次用 EV3、想用跟 SPIKE 一樣的積木 | 下載 `.lmsp` 檔，在 EV3 Classroom 首頁按「開啟」選這個檔，接上 EV3 後按右下角「下載並執行」。 |
| **Pybricks v4** | 想學打字寫程式，用瀏覽器就能寫 | 用 Chrome 打開 [code.pybricks.com](https://code.pybricks.com)，EV3 要先裝 Pybricks 韌體（目前是 Beta 版），把 `.py` 檔拖進編輯區再按執行。 |
| **EV3 MicroPython v2.0** | 已經在用樂高官方 EV3 MicroPython 的人 | 用樂高官方「EV3 MicroPython」microSD 開機卡＋VS Code 擴充套件，把程式內容貼到 `main.py`，按 F5 下載執行。 |
| **clev3r** | 已經在用 clev3r（EV3 Basic）的人 | 在 clev3r 開啟 `.bp` 檔，按「編譯並下載」，在 EV3 的檔案選單找到 `MBC_` 開頭的資料夾執行。 |

## 開始之前：先校準（只要做一次）

循行者要先「認識」你場地上的黑色和白色，數值才會準。校準完會記在感應器裡，重新開機也還在。

1. 感應器插好、EV3 開機（感應器的燈亮起來）。
2. **按一下**感應器上的校準按鈕就放開（不要一直按著，按著不放會一直重新校準）。
3. 綠色校準燈亮起後，**5 秒內**把感應器在黑線和白色地面之間來回掃幾次，16 個感應點都要掃過黑和白。
4. 綠燈熄滅＝校準完成。

**怎麼知道校準好了？** 跑範例 4 的程式前，先用範例 1 看線位置：線在正中間時是 0，往左移變負數、往右移變正數。

## 這頁會用到的 4 個數值

| 名稱 | 範圍 | 從哪裡讀 |
|---|---|---|
| **線的位置** | -16 ~ +16，0＝線在正中間 | 模式 0 |
| **線的寬度（壓到黑線的格數）** | 0 ~ 16，0＝看不到線 | 模式 2 第 2 個值 |
| **線的位置（細）** | -100 ~ +100，0＝線在正中間 | 模式 2 第 1 個值 |
| **壓線位元圖** | 0 ~ 65535：bit0＝第 1 路（最左）… bit15＝第 16 路，1＝那一路壓到線 | 模式 3 第 1 個值（只有 Pybricks／EV3 MicroPython／clev3r 讀得到） |

> [!TIP]
> 線不見的時候，「線位置」會**停在最後一次看到的值**，不會變回 0。所以判斷「有沒有線」一律看**線寬是不是 0**。

**範例列表：**

1. [看看線在哪裡](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E7%9C%8B%E7%9C%8B%E7%B7%9A%E5%9C%A8%E5%93%AA%E8%A3%A1)
2. [三段式循線：偏左、偏右、直走](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E4%B8%89%E6%AE%B5%E5%BC%8F%E5%BE%AA%E7%B7%9A%EF%BC%9A%E5%81%8F%E5%B7%A6%E3%80%81%E5%81%8F%E5%8F%B3%E3%80%81%E7%9B%B4%E8%B5%B0)
3. [比例循線：偏越多、轉越多](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E6%AF%94%E4%BE%8B%E5%BE%AA%E7%B7%9A%EF%BC%9A%E5%81%8F%E8%B6%8A%E5%A4%9A%E3%80%81%E8%BD%89%E8%B6%8A%E5%A4%9A)
4. [碰到十字路口就停車](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E7%A2%B0%E5%88%B0%E5%8D%81%E5%AD%97%E8%B7%AF%E5%8F%A3%E5%B0%B1%E5%81%9C%E8%BB%8A)
5. [數路口：第 3 個路口停車](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E6%95%B8%E8%B7%AF%E5%8F%A3%EF%BC%9A%E7%AC%AC-3-%E5%80%8B%E8%B7%AF%E5%8F%A3%E5%81%9C%E8%BB%8A)
6. [壓線位元圖：在螢幕上畫一把 LED 尺](#%E7%AF%84%E4%BE%8B-6%EF%BC%9A%E5%A3%93%E7%B7%9A%E4%BD%8D%E5%85%83%E5%9C%96%EF%BC%9A%E5%9C%A8%E8%9E%A2%E5%B9%95%E4%B8%8A%E7%95%AB%E4%B8%80%E6%8A%8A-led-%E5%B0%BA)（僅 Pybricks／EV3 MicroPython／clev3r）

---

## 範例 1：看看線在哪裡

**這個範例會做什麼：** 把感應器放在黑線上，EV3 螢幕會顯示一個數字告訴你線在哪裡。這是所有循線程式的第一步：先學會「讀數字」。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 循行者 16 路（EV3 版，RJ12 接頭）
- 白底黑線的場地（黑線寬約 2 公分）

**怎麼接：** 循行者 16 路插 1 號孔，裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/line16/ev3_beginner/ex01_show_position.ev3) ｜ [⬇ EV3 Classroom](/examples/line16/ev3_beginner/ex01_show_position.lmsp) ｜ [⬇ Pybricks v4](/examples/line16/ev3_beginner/ex01_show_position_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/line16/ev3_beginner/ex01_show_position_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/line16/ev3_beginner/ex01_show_position.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/line16/ev3-beginner/ex01_show_position-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex01_show_position-ev3g.webp" alt="循行者 16 路 看看線在哪裡（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/line16/ev3-beginner/ex01_show_position-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex01_show_position-classroom.webp" alt="循行者 16 路 看看線在哪裡（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 01：看看線在哪裡
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：循行者 16 路插 1 號孔
# 這個範例會做什麼：把感應器放在黑線上，EV3 螢幕會顯示一個數字告訴你線在哪裡。這是所有循線程式的第一步：先學會「讀數字」。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 循行者 16 路插 1 號孔


def read_pos():
    # 線的位置：-16 ~ +16，0＝線在正中間
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 先把螢幕清乾淨
ev3.screen.clear()
# 下面的動作一直重複
while True:
    # 讀取線的位置，顯示在螢幕第 1 行
    ev3.screen.draw_text(0, 0, "POS " + str(read_pos()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒再讀下一次，數字才不會閃太快
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 01：看看線在哪裡
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 16 路插 1 號孔
# 這個範例會做什麼：把感應器放在黑線上，EV3 螢幕會顯示一個數字告訴你線在哪裡。這是所有循線程式的第一步：先學會「讀數字」。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 16 路插 1 號孔


def read_pos():
    # 線的位置：-16 ~ +16，0＝線在正中間
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 先把螢幕清乾淨
ev3.screen.clear()
# 下面的動作一直重複
while True:
    # 讀取線的位置，顯示在螢幕第 1 行
    ev3.screen.draw_text(0, 0, "POS " + str(read_pos()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒再讀下一次，數字才不會閃太快
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 循行者 16 路循線感應器 EV3 新手範例 01：看看線在哪裡
' 軟體：clev3r
' 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：把感應器放在黑線上，EV3 螢幕會顯示一個數字告訴你線在哪裡。這是所有循線程式的第一步：先學會「讀數字」。
' 讀值：pos＝線的位置（-16 ~ +16，0＝線在正中間）
' ============================================================
folder "prjs" "MBC_line16_ex01"

' 循行者 16 路插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 先把螢幕清乾淨
LCD.Clear()
' 下面的動作一直重複
While "True"
  ' 讀取線的位置，顯示在螢幕第 1 行
  LCD.Text(1, 0, 0, 1, "POS " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 等 0.1 秒再讀下一次，數字才不會閃太快
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 先把螢幕清乾淨
1. 下面的動作一直重複
   - 讀取線的位置，顯示在螢幕第 1 行
   - 等 0.1 秒再讀下一次，數字才不會閃太快

**跑起來應該看到：** 把黑線放在感應器正中間，螢幕顯示 0；線往左移數字變成負數，往右移變成正數，最邊邊是 ±16（左右是指站在車子後面、面向車頭時的左右）。

**常見問題：**

- **線不見了，數字卻停在最後的值？** 這是正常的：線跑出感應器範圍時，位置會停在最後看到的地方。要判斷「線不見了」請用範例 4 的線寬（線寬＝0）。
- **數字一直不變？** 先看感應器有沒有亮燈、線有沒有插緊。EV3 主機要先開機再接感應器，或拔掉重插一次。
- **車子往反方向修正、越修越歪？** 把程式裡的「＋」和「－」對調（左右輪的修正互換），或確認左輪是不是接在 B、右輪接在 C。

---

## 範例 2：三段式循線：偏左、偏右、直走

**這個範例會做什麼：** 用「如果…就…否則…」讓車子沿著黑線走：線偏左就往左轉，線偏右就往右轉，在中間就直走。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 循行者 16 路（EV3 版，RJ12 接頭）
- 白底黑線的場地（黑線寬約 2 公分）

**怎麼接：** 循行者 16 路插 1 號孔，裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/line16/ev3_beginner/ex02_three_way.ev3) ｜ [⬇ EV3 Classroom](/examples/line16/ev3_beginner/ex02_three_way.lmsp) ｜ [⬇ Pybricks v4](/examples/line16/ev3_beginner/ex02_three_way_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/line16/ev3_beginner/ex02_three_way_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/line16/ev3_beginner/ex02_three_way.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/line16/ev3-beginner/ex02_three_way-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex02_three_way-ev3g.webp" alt="循行者 16 路 三段式循線：偏左、偏右、直走（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/line16/ev3-beginner/ex02_three_way-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex02_three_way-classroom.webp" alt="循行者 16 路 三段式循線：偏左、偏右、直走（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 02：三段式循線：偏左、偏右、直走
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用「如果…就…否則…」讓車子沿著黑線走：線偏左就往左轉，線偏右就往右轉，在中間就直走。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos():
    # 線的位置：-16 ~ +16，0＝線在正中間
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複檢查線的位置
while True:
    # 如果線的位置小於 -4（偏左）
    if read_pos() < (-4):
        # 線偏左：左輪慢、右輪快，車子往左修
        left_motor.dc(10)
        right_motor.dc(40)
    # 否則，如果線的位置大於 4（偏右）
    elif read_pos() > 4:
        # 線偏右：左輪快、右輪慢，車子往右修
        left_motor.dc(40)
        right_motor.dc(10)
    else:
        # 線在中間：兩輪一樣快，直直走
        left_motor.dc(30)
        right_motor.dc(30)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 02：三段式循線：偏左、偏右、直走
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用「如果…就…否則…」讓車子沿著黑線走：線偏左就往左轉，線偏右就往右轉，在中間就直走。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos():
    # 線的位置：-16 ~ +16，0＝線在正中間
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複檢查線的位置
while True:
    # 如果線的位置小於 -4（偏左）
    if read_pos() < (-4):
        # 線偏左：左輪慢、右輪快，車子往左修
        left_motor.dc(10)
        right_motor.dc(40)
    # 否則，如果線的位置大於 4（偏右）
    elif read_pos() > 4:
        # 線偏右：左輪快、右輪慢，車子往右修
        left_motor.dc(40)
        right_motor.dc(10)
    else:
        # 線在中間：兩輪一樣快，直直走
        left_motor.dc(30)
        right_motor.dc(30)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 循行者 16 路循線感應器 EV3 新手範例 02：三段式循線：偏左、偏右、直走
' 軟體：clev3r
' 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：用「如果…就…否則…」讓車子沿著黑線走：線偏左就往左轉，線偏右就往右轉，在中間就直走。
' 讀值：pos＝線的位置（-16 ~ +16，0＝線在正中間）
' ============================================================
folder "prjs" "MBC_line16_ex02"

' 循行者 16 路插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複檢查線的位置
While "True"
  ' 如果線的位置小於 -4（偏左）
  If Sensor.ReadRawValue(1, 0) < (-4) Then
    ' 線偏左：左輪慢、右輪快，車子往左修
    Motor.StartPower("B", 10)
    Motor.StartPower("C", 40)
  ' 否則，如果線的位置大於 4（偏右）
  ElseIf Sensor.ReadRawValue(1, 0) > 4 Then
    ' 線偏右：左輪快、右輪慢，車子往右修
    Motor.StartPower("B", 40)
    Motor.StartPower("C", 10)
  Else
    ' 線在中間：兩輪一樣快，直直走
    Motor.StartPower("B", 30)
    Motor.StartPower("C", 30)
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複檢查線的位置
   - 如果線的位置小於 -4（偏左）
      - 線偏左：左輪慢、右輪快，車子往左修
      - 否則：
         - 否則，如果線的位置大於 4（偏右）
            - 線偏右：左輪快、右輪慢，車子往右修
            - 否則：
               - 線在中間：兩輪一樣快，直直走

**跑起來應該看到：** 車子會左右搖擺地沿著黑線前進。彎道太急時可能衝出去，把 40 改小、10 改成 0 試試看。

**常見問題：**

- **數字一直不變？** 先看感應器有沒有亮燈、線有沒有插緊。EV3 主機要先開機再接感應器，或拔掉重插一次。
- **車子往反方向修正、越修越歪？** 把程式裡的「＋」和「－」對調（左右輪的修正互換），或確認左輪是不是接在 B、右輪接在 C。

---

## 範例 3：比例循線：偏越多、轉越多

**這個範例會做什麼：** 三段式循線會一直搖擺。這個範例把「線的位置 × 一個數字」當成轉彎的力量：偏一點就轉一點、偏很多就轉很多，車子會走得更順。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 循行者 16 路（EV3 版，RJ12 接頭）
- 白底黑線的場地（黑線寬約 2 公分）

**怎麼接：** 循行者 16 路插 1 號孔，裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/line16/ev3_beginner/ex03_proportional.ev3) ｜ [⬇ EV3 Classroom](/examples/line16/ev3_beginner/ex03_proportional.lmsp) ｜ [⬇ Pybricks v4](/examples/line16/ev3_beginner/ex03_proportional_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/line16/ev3_beginner/ex03_proportional_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/line16/ev3_beginner/ex03_proportional.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/line16/ev3-beginner/ex03_proportional-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex03_proportional-ev3g.webp" alt="循行者 16 路 比例循線：偏越多、轉越多（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/line16/ev3-beginner/ex03_proportional-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex03_proportional-classroom.webp" alt="循行者 16 路 比例循線：偏越多、轉越多（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 03：比例循線：偏越多、轉越多
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：三段式循線會一直搖擺。這個範例把「線的位置 × 一個數字」當成轉彎的力量：偏一點就轉一點、偏很多就轉很多，車子會走得更順。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos():
    # 線的位置：-16 ~ +16，0＝線在正中間
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 轉彎力量 turn ＝ 線的位置 × 2
    turn = read_pos() * 2
    # 左輪＝30＋turn、右輪＝30－turn：線偏哪邊，車子就往哪邊修
    left_motor.dc(30 + turn)
    right_motor.dc(30 - turn)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 03：比例循線：偏越多、轉越多
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：三段式循線會一直搖擺。這個範例把「線的位置 × 一個數字」當成轉彎的力量：偏一點就轉一點、偏很多就轉很多，車子會走得更順。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos():
    # 線的位置：-16 ~ +16，0＝線在正中間
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 轉彎力量 turn ＝ 線的位置 × 2
    turn = read_pos() * 2
    # 左輪＝30＋turn、右輪＝30－turn：線偏哪邊，車子就往哪邊修
    left_motor.dc(30 + turn)
    right_motor.dc(30 - turn)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 循行者 16 路循線感應器 EV3 新手範例 03：比例循線：偏越多、轉越多
' 軟體：clev3r
' 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：三段式循線會一直搖擺。這個範例把「線的位置 × 一個數字」當成轉彎的力量：偏一點就轉一點、偏很多就轉很多，車子會走得更順。
' 讀值：pos＝線的位置（-16 ~ +16，0＝線在正中間）
' ============================================================
folder "prjs" "MBC_line16_ex03"

' 循行者 16 路插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複
While "True"
  ' 轉彎力量 turn ＝ 線的位置 × 2
  turn = Sensor.ReadRawValue(1, 0) * 2
  ' 左輪＝30＋turn、右輪＝30－turn：線偏哪邊，車子就往哪邊修
  Motor.StartPower("B", 30 + turn)
  Motor.StartPower("C", 30 - turn)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 轉彎力量 turn ＝ 線的位置 × 2
   - 左輪＝30＋turn、右輪＝30－turn：線偏哪邊，車子就往哪邊修

**跑起來應該看到：** 車子比範例 2 更平順地沿著黑線走。數字 2 叫做「比例係數」：太大會左右甩，太小會轉不過彎，可以自己調整。

**常見問題：**

- **直線上也一直左右甩？** 把 2 改小一點（例如 1）。
- **數字一直不變？** 先看感應器有沒有亮燈、線有沒有插緊。EV3 主機要先開機再接感應器，或拔掉重插一次。
- **車子往反方向修正、越修越歪？** 把程式裡的「＋」和「－」對調（左右輪的修正互換），或確認左輪是不是接在 B、右輪接在 C。

---

## 範例 4：碰到十字路口就停車

**這個範例會做什麼：** 感應器還會告訴你「壓到黑線的格數」（線寬）。平常只有 2~3 格，到十字路口會一下子變很多。這個範例邊循線邊看線寬，大於等於 10 就停車並嗶一聲。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 循行者 16 路（EV3 版，RJ12 接頭）
- 有十字路口的黑線場地

**怎麼接：** 循行者 16 路插 1 號孔，裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/line16/ev3_beginner/ex04_stop_at_cross.ev3) ｜ [⬇ EV3 Classroom](/examples/line16/ev3_beginner/ex04_stop_at_cross.lmsp) ｜ [⬇ Pybricks v4](/examples/line16/ev3_beginner/ex04_stop_at_cross_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/line16/ev3_beginner/ex04_stop_at_cross_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/line16/ev3_beginner/ex04_stop_at_cross.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/line16/ev3-beginner/ex04_stop_at_cross-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex04_stop_at_cross-ev3g.webp" alt="循行者 16 路 碰到十字路口就停車（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/line16/ev3-beginner/ex04_stop_at_cross-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex04_stop_at_cross-classroom.webp" alt="循行者 16 路 碰到十字路口就停車（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 04：碰到十字路口就停車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：感應器還會告訴你「壓到黑線的格數」（線寬）。平常只有 2~3 格，到十字路口會一下子變很多。這個範例邊循線邊看線寬，大於等於 10 就停車並嗶一聲。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_width():
    # 線的寬度（壓到黑線的格數）：0 ~ 16，0＝看不到線
    return sensor.read(2)[1]


def read_pos100():
    # 線的位置（細）：-100 ~ +100，0＝線在正中間
    return sensor.read(2)[0]


# ---------- 程式從這裡開始 ----------
# 一直循線，直到線寬大於等於 10（到路口了）
while not (read_width() >= 10):
    # 轉彎力量 turn ＝ 線的位置（-100~100）× 0.3
    turn = read_pos100() * 0.3
    # 一樣用比例循線往前走，速度放慢到 25
    left_motor.dc(25 + turn)
    right_motor.dc(25 - turn)
# 停車
left_motor.brake()
right_motor.brake()
# 嗶一聲告訴你到了
ev3.speaker.beep()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 04：碰到十字路口就停車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：感應器還會告訴你「壓到黑線的格數」（線寬）。平常只有 2~3 格，到十字路口會一下子變很多。這個範例邊循線邊看線寬，大於等於 10 就停車並嗶一聲。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_width():
    # 線的寬度（壓到黑線的格數）：0 ~ 16，0＝看不到線
    return sensor.read('IR-REMOTE')[1]


def read_pos100():
    # 線的位置（細）：-100 ~ +100，0＝線在正中間
    return sensor.read('IR-REMOTE')[0]


# ---------- 程式從這裡開始 ----------
# 一直循線，直到線寬大於等於 10（到路口了）
while not (read_width() >= 10):
    # 轉彎力量 turn ＝ 線的位置（-100~100）× 0.3
    turn = read_pos100() * 0.3
    # 一樣用比例循線往前走，速度放慢到 25
    left_motor.dc(25 + turn)
    right_motor.dc(25 - turn)
# 停車
left_motor.brake()
right_motor.brake()
# 嗶一聲告訴你到了
ev3.speaker.beep()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 循行者 16 路循線感應器 EV3 新手範例 04：碰到十字路口就停車
' 軟體：clev3r
' 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：感應器還會告訴你「壓到黑線的格數」（線寬）。平常只有 2~3 格，到十字路口會一下子變很多。這個範例邊循線邊看線寬，大於等於 10 就停車並嗶一聲。
' 讀值：width＝線的寬度（壓到黑線的格數）（0 ~ 16，0＝看不到線）；pos100＝線的位置（細）（-100 ~ +100，0＝線在正中間）
' ============================================================
folder "prjs" "MBC_line16_ex04"

' 循行者 16 路插 1 號孔，使用模式 2
Sensor.SetMode(1, 2)
LCD.Clear()

' 一直循線，直到線寬大於等於 10（到路口了）
While Sensor.ReadRawValue(1, 1) < 10
  ' 轉彎力量 turn ＝ 線的位置（-100~100）× 0.3
  turn = Sensor.ReadRawValue(1, 0) * 0.3
  ' 一樣用比例循線往前走，速度放慢到 25
  Motor.StartPower("B", 25 + turn)
  Motor.StartPower("C", 25 - turn)
EndWhile
' 停車
Motor.Stop("BC", "True")
' 嗶一聲告訴你到了
Speaker.Tone(50, 1000, 100)
Speaker.Wait()
```

</details>

**一步一步看懂程式：**

1. 一直循線，直到線寬大於等於 10（到路口了）
   - 轉彎力量 turn ＝ 線的位置（-100~100）× 0.3
   - 一樣用比例循線往前走，速度放慢到 25
1. 停車
1. 嗶一聲告訴你到了

**跑起來應該看到：** 車子循線前進，壓到十字路口時停下來並嗶一聲。

**常見問題：**

- **還沒到路口就停了？** 彎道也可能讓線寬變大，把 10 改大一點。
- **到路口不停？** 用範例 1 的方法把線寬顯示出來，看路口時實際是多少，再改程式裡的 10。
- **數字一直不變？** 先看感應器有沒有亮燈、線有沒有插緊。EV3 主機要先開機再接感應器，或拔掉重插一次。
- **車子往反方向修正、越修越歪？** 把程式裡的「＋」和「－」對調（左右輪的修正互換），或確認左輪是不是接在 B、右輪接在 C。

---

## 範例 5：數路口：第 3 個路口停車

**這個範例會做什麼：** 比賽常常要「走到第幾個路口」。這個範例用一個變數 count 數路口，用另一個變數 onCross 記住「現在是不是正壓在路口上」，避免同一個路口被數很多次。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 循行者 16 路（EV3 版，RJ12 接頭）
- 有 3 個以上十字路口的黑線場地

**怎麼接：** 循行者 16 路插 1 號孔，裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/line16/ev3_beginner/ex05_count_cross.ev3) ｜ [⬇ EV3 Classroom](/examples/line16/ev3_beginner/ex05_count_cross.lmsp) ｜ [⬇ Pybricks v4](/examples/line16/ev3_beginner/ex05_count_cross_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/line16/ev3_beginner/ex05_count_cross_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/line16/ev3_beginner/ex05_count_cross.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/line16/ev3-beginner/ex05_count_cross-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex05_count_cross-ev3g.webp" alt="循行者 16 路 數路口：第 3 個路口停車（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/line16/ev3-beginner/ex05_count_cross-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/line16/ev3-beginner/ex05_count_cross-classroom.webp" alt="循行者 16 路 數路口：第 3 個路口停車（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 05：數路口：第 3 個路口停車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：比賽常常要「走到第幾個路口」。這個範例用一個變數 count 數路口，用另一個變數 onCross 記住「現在是不是正壓在路口上」，避免同一個路口被數很多次。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos100():
    # 線的位置（細）：-100 ~ +100，0＝線在正中間
    return sensor.read(2)[0]


def read_width():
    # 線的寬度（壓到黑線的格數）：0 ~ 16，0＝看不到線
    return sensor.read(2)[1]


# ---------- 程式從這裡開始 ----------
# 路口數 count 從 0 開始
count = 0
# onCross＝0 代表「現在不在路口上」
onCross = 0
# 一直循線，直到數滿 3 個路口
while not (count >= 3):
    # 比例循線：turn ＝ 線的位置 × 0.3
    turn = read_pos100() * 0.3
    # 左輪＝25＋turn、右輪＝25－turn
    left_motor.dc(25 + turn)
    right_motor.dc(25 - turn)
    # 如果線寬大於等於 10（壓在路口上）
    if read_width() >= 10:
        # 而且剛剛還不在路口上
        if onCross == 0:
            # 剛壓到新路口：count 加 1
            count = count + 1
            # 記住「正在路口上」，同一個路口不要再數
            onCross = 1
            # 嗶一聲
            ev3.speaker.beep()
    else:
        # 離開路口了：onCross 改回 0，準備數下一個
        onCross = 0
    # 螢幕顯示已經數到第幾個路口
    ev3.screen.draw_text(0, 0, "COUNT " + str(count) + "      ", Color.BLACK, Color.WHITE)
# 停車
left_motor.brake()
right_motor.brake()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 05：數路口：第 3 個路口停車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：比賽常常要「走到第幾個路口」。這個範例用一個變數 count 數路口，用另一個變數 onCross 記住「現在是不是正壓在路口上」，避免同一個路口被數很多次。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 16 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos100():
    # 線的位置（細）：-100 ~ +100，0＝線在正中間
    return sensor.read('IR-REMOTE')[0]


def read_width():
    # 線的寬度（壓到黑線的格數）：0 ~ 16，0＝看不到線
    return sensor.read('IR-REMOTE')[1]


# ---------- 程式從這裡開始 ----------
# 路口數 count 從 0 開始
count = 0
# onCross＝0 代表「現在不在路口上」
onCross = 0
# 一直循線，直到數滿 3 個路口
while not (count >= 3):
    # 比例循線：turn ＝ 線的位置 × 0.3
    turn = read_pos100() * 0.3
    # 左輪＝25＋turn、右輪＝25－turn
    left_motor.dc(25 + turn)
    right_motor.dc(25 - turn)
    # 如果線寬大於等於 10（壓在路口上）
    if read_width() >= 10:
        # 而且剛剛還不在路口上
        if onCross == 0:
            # 剛壓到新路口：count 加 1
            count = count + 1
            # 記住「正在路口上」，同一個路口不要再數
            onCross = 1
            # 嗶一聲
            ev3.speaker.beep()
    else:
        # 離開路口了：onCross 改回 0，準備數下一個
        onCross = 0
    # 螢幕顯示已經數到第幾個路口
    ev3.screen.draw_text(0, 0, "COUNT " + str(count) + "      ", Color.BLACK, Color.WHITE)
# 停車
left_motor.brake()
right_motor.brake()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 循行者 16 路循線感應器 EV3 新手範例 05：數路口：第 3 個路口停車
' 軟體：clev3r
' 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：比賽常常要「走到第幾個路口」。這個範例用一個變數 count 數路口，用另一個變數 onCross 記住「現在是不是正壓在路口上」，避免同一個路口被數很多次。
' 讀值：pos100＝線的位置（細）（-100 ~ +100，0＝線在正中間）；width＝線的寬度（壓到黑線的格數）（0 ~ 16，0＝看不到線）
' ============================================================
folder "prjs" "MBC_line16_ex05"

' 循行者 16 路插 1 號孔，使用模式 2
Sensor.SetMode(1, 2)
LCD.Clear()

' 路口數 count 從 0 開始
count = 0
' onCross＝0 代表「現在不在路口上」
onCross = 0
' 一直循線，直到數滿 3 個路口
While count < 3
  ' 比例循線：turn ＝ 線的位置 × 0.3
  turn = Sensor.ReadRawValue(1, 0) * 0.3
  ' 左輪＝25＋turn、右輪＝25－turn
  Motor.StartPower("B", 25 + turn)
  Motor.StartPower("C", 25 - turn)
  ' 如果線寬大於等於 10（壓在路口上）
  If Sensor.ReadRawValue(1, 1) >= 10 Then
    ' 而且剛剛還不在路口上
    If onCross = 0 Then
      ' 剛壓到新路口：count 加 1
      count = count + 1
      ' 記住「正在路口上」，同一個路口不要再數
      onCross = 1
      ' 嗶一聲
      Speaker.Tone(50, 1000, 100)
      Speaker.Wait()
    EndIf
  Else
    ' 離開路口了：onCross 改回 0，準備數下一個
    onCross = 0
  EndIf
  ' 螢幕顯示已經數到第幾個路口
  LCD.Text(1, 0, 0, 1, "COUNT " + count + "      ")
EndWhile
' 停車
Motor.Stop("BC", "True")
```

</details>

**一步一步看懂程式：**

1. 路口數 count 從 0 開始
1. onCross＝0 代表「現在不在路口上」
1. 一直循線，直到數滿 3 個路口
   - 比例循線：turn ＝ 線的位置 × 0.3
   - 左輪＝25＋turn、右輪＝25－turn
   - 如果線寬大於等於 10（壓在路口上）
      - 而且剛剛還不在路口上
         - 剛壓到新路口：count 加 1
         - 記住「正在路口上」，同一個路口不要再數
         - 嗶一聲
      - 否則：
         - 離開路口了：onCross 改回 0，準備數下一個
   - 螢幕顯示已經數到第幾個路口
1. 停車

**跑起來應該看到：** 每經過一個路口嗶一聲、螢幕數字加 1，到第 3 個路口停車。把 3 改成別的數字就能停在不同路口。

**常見問題：**

- **一個路口數成兩個？** 路口的黑線有破損或反光時會發生。把車速 25 調慢，或把線寬門檻調大。
- **數字一直不變？** 先看感應器有沒有亮燈、線有沒有插緊。EV3 主機要先開機再接感應器，或拔掉重插一次。
- **車子往反方向修正、越修越歪？** 把程式裡的「＋」和「－」對調（左右輪的修正互換），或確認左輪是不是接在 B、右輪接在 C。

---

## 範例 6：壓線位元圖：在螢幕上畫一把 LED 尺

> **僅 Pybricks v4／EV3 MicroPython v2.0／clev3r**（EV3-G、Classroom 的積木讀不到這個模式）

**這個範例會做什麼：** 模式 3 送出「壓線位元圖」：16 個位元，每一路壓到線就是 1。這個範例把它畫成一把尺（# ＝壓到線、. ＝沒壓到），跟感應器上的 LED 一樣，一眼就看出線在哪幾路、有沒有分岔。

**要準備什麼：**

- 循行者 16 路（EV3 版，韌體 v4.0 以上）
- EV3 主機
- 白底黑線的場地

**怎麼接：** 循行者 16 路插 1 號孔，裝在車頭下方、離地約 1 公分，感應器正對地面。

**下載程式（選你用的軟體）：**

[⬇ Pybricks v4](/examples/line16/ev3_beginner/ex06_bitmap_ruler_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/line16/ev3_beginner/ex06_bitmap_ruler_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/line16/ev3_beginner/ex06_bitmap_ruler.bp)

（程式碼即範例，不需截圖。）

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 06：壓線位元圖：在螢幕上畫一把 LED 尺
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：循行者 16 路插 1 號孔
# 這個範例會做什麼：模式 3 送出「壓線位元圖」：16 個位元，每一路壓到線就是 1。這個範例把它畫成一把尺（# ＝壓到線、. ＝沒壓到），跟感應器上的 LED 一樣，一眼就看出線在哪幾路、有沒有分岔。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 循行者 16 路插 1 號孔


def read_bitmap():
    # 壓線位元圖：0 ~ 65535：bit0＝第 1 路（最左）… bit15＝第 16 路，1＝那一路壓到線
    return sensor.read(3)[0] & 0xFFFF


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行畫尺：16 格，最左是第 1 路，# ＝壓到線、. ＝沒壓到
    bits_value = read_bitmap()
    bits_text = ""
    for b in range(16):
        if (bits_value // (2 ** b)) % 2 == 1:   # 第 b 個位元是 1
            bits_text = bits_text + "#"
        else:
            bits_text = bits_text + "."
    ev3.screen.draw_text(0, 0, "L " + bits_text + "  ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示位元圖的數字（第 1 路＝1、第 2 路＝2、第 3 路＝4…相加）
    ev3.screen.draw_text(0, 16, "BITS " + str(read_bitmap()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 16 路循線感應器 EV3 新手範例 06：壓線位元圖：在螢幕上畫一把 LED 尺
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 16 路插 1 號孔
# 這個範例會做什麼：模式 3 送出「壓線位元圖」：16 個位元，每一路壓到線就是 1。這個範例把它畫成一把尺（# ＝壓到線、. ＝沒壓到），跟感應器上的 LED 一樣，一眼就看出線在哪幾路、有沒有分岔。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 16 路插 1 號孔


def read_bitmap():
    # 壓線位元圖：0 ~ 65535：bit0＝第 1 路（最左）… bit15＝第 16 路，1＝那一路壓到線
    return sensor.read('IR-REM-A')[0] & 0xFFFF


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行畫尺：16 格，最左是第 1 路，# ＝壓到線、. ＝沒壓到
    bits_value = read_bitmap()
    bits_text = ""
    for b in range(16):
        if (bits_value // (2 ** b)) % 2 == 1:   # 第 b 個位元是 1
            bits_text = bits_text + "#"
        else:
            bits_text = bits_text + "."
    ev3.screen.draw_text(0, 0, "L " + bits_text + "  ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示位元圖的數字（第 1 路＝1、第 2 路＝2、第 3 路＝4…相加）
    ev3.screen.draw_text(0, 16, "BITS " + str(read_bitmap()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 循行者 16 路循線感應器 EV3 新手範例 06：壓線位元圖：在螢幕上畫一把 LED 尺
' 軟體：clev3r
' 接法：循行者 16 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 3 送出「壓線位元圖」：16 個位元，每一路壓到線就是 1。這個範例把它畫成一把尺（# ＝壓到線、. ＝沒壓到），跟感應器上的 LED 一樣，一眼就看出線在哪幾路、有沒有分岔。
' 讀值：bitmap＝壓線位元圖（0 ~ 65535：bit0＝第 1 路（最左）… bit15＝第 16 路，1＝那一路壓到線）
' ============================================================
folder "prjs" "MBC_line16_ex06"

' 循行者 16 路插 1 號孔，使用模式 3
Sensor.SetMode(1, 3)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 第 1 行畫尺：16 格，最左是第 1 路，# ＝壓到線、. ＝沒壓到
  bitsValue = Math.Remainder(Sensor.ReadRawValue(1, 0) + 65536, 65536)
  bitsText = ""
  For b = 0 To 15
    If Math.Remainder(Math.Floor(bitsValue / Math.Power(2, b)), 2) = 1 Then
      bitsText = Text.Append(bitsText, "#")
    Else
      bitsText = Text.Append(bitsText, ".")
    EndIf
  EndFor
  LCD.Text(1, 0, 0, 1, "L " + bitsText + "  ")
  ' 第 2 行顯示位元圖的數字（第 1 路＝1、第 2 路＝2、第 3 路＝4…相加）
  LCD.Text(1, 0, 16, 1, "BITS " + Math.Remainder(Sensor.ReadRawValue(1, 0) + 65536, 65536) + "      ")
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 一直重複
   - 第 1 行畫尺：16 格，最左是第 1 路，# ＝壓到線、. ＝沒壓到
   - 第 2 行顯示位元圖的數字（第 1 路＝1、第 2 路＝2、第 3 路＝4…相加）
   - 等 0.1 秒

**跑起來應該看到：** 把感應器放在黑線上，尺上有 2~3 個 # 跟著線移動；壓到十字路口 # 變成一整排；線不見時全部是 .。數字＝壓到線的那幾路的 1、2、4、8… 加起來，例如第 1、2 路壓線＝3。

**常見問題：**

- **尺跟感應器的 LED 左右相反？** 尺是站在車子後面看的（最左＝第 1 路）；感應器的 LED 從上面看可能相反。
- **數字一直是 0 或不會動？** 感應器韌體要 v4.0（2026-10）以上才有這個模式，舊韌體請先更新。
- **EV3-G／Classroom 怎麼沒有這個範例？** 這個範例用的是新模式（2026-10 韌體 v4 起），EV3-G 和 Classroom 的紅外線積木讀不到，只有 Pybricks、EV3 MicroPython、clev3r 能用。
- **數字一直不變？** 先看感應器有沒有亮燈、線有沒有插緊。EV3 主機要先開機再接感應器，或拔掉重插一次。

---

還有問題？歡迎到 [聯絡我們](/contact.md) 留言，附上你用的軟體和程式截圖，我們會幫你看。
