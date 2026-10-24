<!-- 這頁由 95_產品網站\EV3新手範例產生器 自動產生，請改產生器再重產，不要直接改這個檔。 -->
<!-- moved: /sensors/imu/ev3/index.md -->
<p class="moved-note">這頁內容已整合到新的說明頁：<a href="/sensors/imu/ev3/index.md">陀螺儀 IMU 九軸 › EV3</a></p>

# 🧱 EV3 新手範例：IMU 陀螺儀

[🔙 回到 IMU 陀螺儀產品頁](/sensors/imu/index.md)

這一頁有 7 個範例，給**完全沒寫過程式**的人：每個範例都有現成的程式檔可以下載，打開就能用，下面也有一步一步的白話說明。建議從範例 1 開始照順序做。

## 開始之前

1. 感應器要用 **EV3 版**（RJ12 接頭），插在 EV3 的 **1 號孔**。
2. **EV3 先開機，再插感應器**。數字一直不變時，拔掉感應器重插一次。
3. 會動的範例用樂高教育版基本車：**左輪馬達接 B、右輪馬達接 C**。
4. 感應器在 EV3 上會顯示成「陀螺儀」，直接用原廠陀螺儀的積木。

**每個範例都有 5 種軟體的程式檔，選你用的那一種就好：**（標「僅 Pybricks／EV3 MicroPython／clev3r」的範例只有這 3 種，因為 EV3-G、Classroom 的積木讀不到那個模式）

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
| **機器人轉了幾度** | 連續累加（轉一圈＝360，可超過 360） | 陀螺儀「角度」 |
| **俯仰角（車頭上抬或下壓幾度）** | -90 ~ +90 度，平放＝0（板子絲印 X 軸） | 陀螺儀模式 6（只有 Pybricks／EV3 MicroPython／clev3r 讀得到） |
| **轉向的角速度（每秒幾度 × 10）** | ±3000 左右，正負跟角度同方向；100＝每秒 10 度 | 陀螺儀模式 2（只有 Pybricks／EV3 MicroPython／clev3r 讀得到） |

**範例列表：**

1. [看看機器人轉了幾度](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E7%9C%8B%E7%9C%8B%E6%A9%9F%E5%99%A8%E4%BA%BA%E8%BD%89%E4%BA%86%E5%B9%BE%E5%BA%A6)
2. [原地右轉 90 度](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E5%8E%9F%E5%9C%B0%E5%8F%B3%E8%BD%89-90-%E5%BA%A6)
3. [走正方形](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E8%B5%B0%E6%AD%A3%E6%96%B9%E5%BD%A2)
4. [走直線自動修正](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E8%B5%B0%E7%9B%B4%E7%B7%9A%E8%87%AA%E5%8B%95%E4%BF%AE%E6%AD%A3)
5. [不倒的指南針：自己轉回原來的方向](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E4%B8%8D%E5%80%92%E7%9A%84%E6%8C%87%E5%8D%97%E9%87%9D%EF%BC%9A%E8%87%AA%E5%B7%B1%E8%BD%89%E5%9B%9E%E5%8E%9F%E4%BE%86%E7%9A%84%E6%96%B9%E5%90%91)
6. [俯仰角：上坡就減速](#%E7%AF%84%E4%BE%8B-6%EF%BC%9A%E4%BF%AF%E4%BB%B0%E8%A7%92%EF%BC%9A%E4%B8%8A%E5%9D%A1%E5%B0%B1%E6%B8%9B%E9%80%9F)（僅 Pybricks／EV3 MicroPython／clev3r）
7. [轉太快就警告](#%E7%AF%84%E4%BE%8B-7%EF%BC%9A%E8%BD%89%E5%A4%AA%E5%BF%AB%E5%B0%B1%E8%AD%A6%E5%91%8A)（僅 Pybricks／EV3 MicroPython／clev3r）

---

## 範例 1：看看機器人轉了幾度

**這個範例會做什麼：** 程式開始時角度歸零，然後用手轉動機器人，螢幕會顯示轉了幾度。

**要準備什麼：**

- IMU 陀螺儀（EV3 版）
- EV3 主機

**怎麼接：** IMU 陀螺儀插 1 號孔，平放固定在車身上（不要晃動）。程式開始時車子要靜止不動。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/imu/ev3_beginner/ex01_show_angle.ev3) ｜ [⬇ EV3 Classroom](/examples/imu/ev3_beginner/ex01_show_angle.lmsp) ｜ [⬇ Pybricks v4](/examples/imu/ev3_beginner/ex01_show_angle_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/imu/ev3_beginner/ex01_show_angle_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/imu/ev3_beginner/ex01_show_angle.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/imu/ev3-beginner/ex01_show_angle-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex01_show_angle-ev3g.webp" alt="IMU 陀螺儀 看看機器人轉了幾度（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/imu/ev3-beginner/ex01_show_angle-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex01_show_angle-classroom.webp" alt="IMU 陀螺儀 看看機器人轉了幾度（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 01：看看機器人轉了幾度
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔
# 這個範例會做什麼：程式開始時角度歸零，然後用手轉動機器人，螢幕會顯示轉了幾度。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import GyroSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 先把角度歸零（這時候不要動機器人）
gyro.reset_angle(0)
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 螢幕第 1 行顯示目前角度
    ev3.screen.draw_text(0, 0, "ANGLE " + str(read_angle()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 01：看看機器人轉了幾度
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔
# 這個範例會做什麼：程式開始時角度歸零，然後用手轉動機器人，螢幕會顯示轉了幾度。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import GyroSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 先把角度歸零（這時候不要動機器人）
gyro.reset_angle(0)
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 螢幕第 1 行顯示目前角度
    ev3.screen.draw_text(0, 0, "ANGLE " + str(read_angle()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' IMU 陀螺儀感應器 EV3 新手範例 01：看看機器人轉了幾度
' 軟體：clev3r
' 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：程式開始時角度歸零，然後用手轉動機器人，螢幕會顯示轉了幾度。
' 讀值：angle＝機器人轉了幾度（連續累加（轉一圈＝360，可超過 360））
' ============================================================
folder "prjs" "MBC_imu_ex01"

' IMU 陀螺儀插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
' 陀螺儀歸零的指令（送 0x11 給感應器）
gyroReset[0] = 17
LCD.Clear()

' 先把角度歸零（這時候不要動機器人）
Sensor.SendUARTData(1, 1, gyroReset)
Program.Delay(100)
' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 螢幕第 1 行顯示目前角度
  LCD.Text(1, 0, 0, 1, "ANGLE " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 先把角度歸零（這時候不要動機器人）
1. 清除螢幕
1. 一直重複
   - 螢幕第 1 行顯示目前角度
   - 等 0.1 秒

**跑起來應該看到：** 轉一圈約 360；往反方向轉會變成負數。轉回原來的方向，數字會回到 0 附近。

**常見問題：**

- **車子沒動，角度卻一直慢慢變？** 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **角度變化方向跟範例說的相反？** 感應器可能裝反了（上下顛倒）。範例 2、3 用「絕對值」所以不受影響；範例 4、5 請把程式裡的「＋」「－」對調。

---

## 範例 2：原地右轉 90 度

**這個範例會做什麼：** 靠陀螺儀轉彎，比「轉幾秒」準很多：一直轉，直到角度到 90 度才停。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- IMU 陀螺儀（EV3 版）

**怎麼接：** IMU 陀螺儀插 1 號孔，平放固定在車身上（不要晃動）。程式開始時車子要靜止不動。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/imu/ev3_beginner/ex02_turn_90.ev3) ｜ [⬇ EV3 Classroom](/examples/imu/ev3_beginner/ex02_turn_90.lmsp) ｜ [⬇ Pybricks v4](/examples/imu/ev3_beginner/ex02_turn_90_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/imu/ev3_beginner/ex02_turn_90_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/imu/ev3_beginner/ex02_turn_90.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/imu/ev3-beginner/ex02_turn_90-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex02_turn_90-ev3g.webp" alt="IMU 陀螺儀 原地右轉 90 度（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/imu/ev3-beginner/ex02_turn_90-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex02_turn_90-classroom.webp" alt="IMU 陀螺儀 原地右轉 90 度（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 02：原地右轉 90 度
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：靠陀螺儀轉彎，比「轉幾秒」準很多：一直轉，直到角度到 90 度才停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 角度歸零
gyro.reset_angle(0)
# 左輪往前、右輪往後：原地向右轉（慢一點比較準）
left_motor.dc(20)
right_motor.dc(-20)
# 等到角度的絕對值大於等於 90
while not (abs(read_angle()) >= 90):
    wait(10)
# 轉到 90 度：停車
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
# IMU 陀螺儀感應器 EV3 新手範例 02：原地右轉 90 度
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：靠陀螺儀轉彎，比「轉幾秒」準很多：一直轉，直到角度到 90 度才停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 角度歸零
gyro.reset_angle(0)
# 左輪往前、右輪往後：原地向右轉（慢一點比較準）
left_motor.dc(20)
right_motor.dc(-20)
# 等到角度的絕對值大於等於 90
while not (abs(read_angle()) >= 90):
    wait(10)
# 轉到 90 度：停車
left_motor.brake()
right_motor.brake()
# 嗶一聲
ev3.speaker.beep()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' IMU 陀螺儀感應器 EV3 新手範例 02：原地右轉 90 度
' 軟體：clev3r
' 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：靠陀螺儀轉彎，比「轉幾秒」準很多：一直轉，直到角度到 90 度才停。
' 讀值：angle＝機器人轉了幾度（連續累加（轉一圈＝360，可超過 360））
' ============================================================
folder "prjs" "MBC_imu_ex02"

' IMU 陀螺儀插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
' 陀螺儀歸零的指令（送 0x11 給感應器）
gyroReset[0] = 17
LCD.Clear()

' 角度歸零
Sensor.SendUARTData(1, 1, gyroReset)
Program.Delay(100)
' 左輪往前、右輪往後：原地向右轉（慢一點比較準）
Motor.StartPower("B", 20)
Motor.StartPower("C", -20)
' 等到角度的絕對值大於等於 90
While Math.Abs(Sensor.ReadRawValue(1, 0)) < 90
  Program.Delay(10)
EndWhile
' 轉到 90 度：停車
Motor.Stop("BC", "True")
' 嗶一聲
Speaker.Tone(50, 1000, 100)
Speaker.Wait()
```

</details>

**一步一步看懂程式：**

1. 角度歸零
1. 左輪往前、右輪往後：原地向右轉（慢一點比較準）
1. 等到角度的絕對值大於等於 90
1. 轉到 90 度：停車
1. 嗶一聲

**跑起來應該看到：** 車子原地右轉約 90 度後停下來並嗶一聲。

**常見問題：**

- **每次都轉過頭幾度？** 車子停下來需要一點時間。把 90 改成 85 試試看，或把轉速 20 再調慢。
- **車子沒動，角度卻一直慢慢變？** 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **角度變化方向跟範例說的相反？** 感應器可能裝反了（上下顛倒）。範例 2、3 用「絕對值」所以不受影響；範例 4、5 請把程式裡的「＋」「－」對調。

---

## 範例 3：走正方形

**這個範例會做什麼：** 把「直走 2 秒＋右轉 90 度」重複 4 次，就是一個正方形。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- IMU 陀螺儀（EV3 版）

**怎麼接：** IMU 陀螺儀插 1 號孔，平放固定在車身上（不要晃動）。程式開始時車子要靜止不動。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/imu/ev3_beginner/ex03_square.ev3) ｜ [⬇ EV3 Classroom](/examples/imu/ev3_beginner/ex03_square.lmsp) ｜ [⬇ Pybricks v4](/examples/imu/ev3_beginner/ex03_square_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/imu/ev3_beginner/ex03_square_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/imu/ev3_beginner/ex03_square.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/imu/ev3-beginner/ex03_square-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex03_square-ev3g.webp" alt="IMU 陀螺儀 走正方形（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/imu/ev3-beginner/ex03_square-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex03_square-classroom.webp" alt="IMU 陀螺儀 走正方形（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 03：走正方形
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：把「直走 2 秒＋右轉 90 度」重複 4 次，就是一個正方形。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 下面的動作重複 4 次
for i in range(4):
    # 直走
    left_motor.dc(30)
    right_motor.dc(30)
    # 走 2 秒
    wait(2000)
    # 停一下
    left_motor.brake()
    right_motor.brake()
    # 角度歸零
    gyro.reset_angle(0)
    # 左輪往前、右輪往後：原地向右轉（慢一點比較準）
    left_motor.dc(20)
    right_motor.dc(-20)
    # 等到角度的絕對值大於等於 90
    while not (abs(read_angle()) >= 90):
        wait(10)
    # 轉到 90 度：停車
    left_motor.brake()
    right_motor.brake()
# 走完嗶一聲
ev3.speaker.beep()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 03：走正方形
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：把「直走 2 秒＋右轉 90 度」重複 4 次，就是一個正方形。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 下面的動作重複 4 次
for i in range(4):
    # 直走
    left_motor.dc(30)
    right_motor.dc(30)
    # 走 2 秒
    wait(2000)
    # 停一下
    left_motor.brake()
    right_motor.brake()
    # 角度歸零
    gyro.reset_angle(0)
    # 左輪往前、右輪往後：原地向右轉（慢一點比較準）
    left_motor.dc(20)
    right_motor.dc(-20)
    # 等到角度的絕對值大於等於 90
    while not (abs(read_angle()) >= 90):
        wait(10)
    # 轉到 90 度：停車
    left_motor.brake()
    right_motor.brake()
# 走完嗶一聲
ev3.speaker.beep()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' IMU 陀螺儀感應器 EV3 新手範例 03：走正方形
' 軟體：clev3r
' 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：把「直走 2 秒＋右轉 90 度」重複 4 次，就是一個正方形。
' 讀值：angle＝機器人轉了幾度（連續累加（轉一圈＝360，可超過 360））
' ============================================================
folder "prjs" "MBC_imu_ex03"

' IMU 陀螺儀插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
' 陀螺儀歸零的指令（送 0x11 給感應器）
gyroReset[0] = 17
LCD.Clear()

' 下面的動作重複 4 次
For i = 1 To 4
  ' 直走
  Motor.StartPower("B", 30)
  Motor.StartPower("C", 30)
  ' 走 2 秒
  Program.Delay(2000)
  ' 停一下
  Motor.Stop("BC", "True")
  ' 角度歸零
  Sensor.SendUARTData(1, 1, gyroReset)
  Program.Delay(100)
  ' 左輪往前、右輪往後：原地向右轉（慢一點比較準）
  Motor.StartPower("B", 20)
  Motor.StartPower("C", -20)
  ' 等到角度的絕對值大於等於 90
  While Math.Abs(Sensor.ReadRawValue(1, 0)) < 90
    Program.Delay(10)
  EndWhile
  ' 轉到 90 度：停車
  Motor.Stop("BC", "True")
EndFor
' 走完嗶一聲
Speaker.Tone(50, 1000, 100)
Speaker.Wait()
```

</details>

**一步一步看懂程式：**

1. 下面的動作重複 4 次
   - 直走
   - 走 2 秒
   - 停一下
   - 角度歸零
   - 左輪往前、右輪往後：原地向右轉（慢一點比較準）
   - 等到角度的絕對值大於等於 90
   - 轉到 90 度：停車
1. 走完嗶一聲

**跑起來應該看到：** 車子走出一個正方形，最後回到出發點附近。

**常見問題：**

- **車子沒動，角度卻一直慢慢變？** 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **角度變化方向跟範例說的相反？** 感應器可能裝反了（上下顛倒）。範例 2、3 用「絕對值」所以不受影響；範例 4、5 請把程式裡的「＋」「－」對調。

---

## 範例 4：走直線自動修正

**這個範例會做什麼：** 兩個輪子不可能完全一樣快，車子直走久了會歪。用陀螺儀看歪了幾度，就往反方向補回來，車子就能走得很直。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- IMU 陀螺儀（EV3 版）

**怎麼接：** IMU 陀螺儀插 1 號孔，平放固定在車身上（不要晃動）。程式開始時車子要靜止不動。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/imu/ev3_beginner/ex04_drive_straight.ev3) ｜ [⬇ EV3 Classroom](/examples/imu/ev3_beginner/ex04_drive_straight.lmsp) ｜ [⬇ Pybricks v4](/examples/imu/ev3_beginner/ex04_drive_straight_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/imu/ev3_beginner/ex04_drive_straight_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/imu/ev3_beginner/ex04_drive_straight.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/imu/ev3-beginner/ex04_drive_straight-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex04_drive_straight-ev3g.webp" alt="IMU 陀螺儀 走直線自動修正（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/imu/ev3-beginner/ex04_drive_straight-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex04_drive_straight-classroom.webp" alt="IMU 陀螺儀 走直線自動修正（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 04：走直線自動修正
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：兩個輪子不可能完全一樣快，車子直走久了會歪。用陀螺儀看歪了幾度，就往反方向補回來，車子就能走得很直。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 角度歸零：現在的方向就是「正前方」
gyro.reset_angle(0)
# 一直重複
while True:
    # 修正力量 fix ＝ 角度 × 2
    fix = read_angle() * 2
    # 往右歪（角度變大）：左輪減速、右輪加速，把車頭拉回來
    left_motor.dc(30 - fix)
    right_motor.dc(30 + fix)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 04：走直線自動修正
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：兩個輪子不可能完全一樣快，車子直走久了會歪。用陀螺儀看歪了幾度，就往反方向補回來，車子就能走得很直。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 角度歸零：現在的方向就是「正前方」
gyro.reset_angle(0)
# 一直重複
while True:
    # 修正力量 fix ＝ 角度 × 2
    fix = read_angle() * 2
    # 往右歪（角度變大）：左輪減速、右輪加速，把車頭拉回來
    left_motor.dc(30 - fix)
    right_motor.dc(30 + fix)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' IMU 陀螺儀感應器 EV3 新手範例 04：走直線自動修正
' 軟體：clev3r
' 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：兩個輪子不可能完全一樣快，車子直走久了會歪。用陀螺儀看歪了幾度，就往反方向補回來，車子就能走得很直。
' 讀值：angle＝機器人轉了幾度（連續累加（轉一圈＝360，可超過 360））
' ============================================================
folder "prjs" "MBC_imu_ex04"

' IMU 陀螺儀插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
' 陀螺儀歸零的指令（送 0x11 給感應器）
gyroReset[0] = 17
LCD.Clear()

' 角度歸零：現在的方向就是「正前方」
Sensor.SendUARTData(1, 1, gyroReset)
Program.Delay(100)
' 一直重複
While "True"
  ' 修正力量 fix ＝ 角度 × 2
  fix = Sensor.ReadRawValue(1, 0) * 2
  ' 往右歪（角度變大）：左輪減速、右輪加速，把車頭拉回來
  Motor.StartPower("B", 30 - fix)
  Motor.StartPower("C", 30 + fix)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 角度歸零：現在的方向就是「正前方」
1. 一直重複
   - 修正力量 fix ＝ 角度 × 2
   - 往右歪（角度變大）：左輪減速、右輪加速，把車頭拉回來

**跑起來應該看到：** 車子會走直線；用手把車頭推歪，它會自己轉回原來的方向繼續直走。

**常見問題：**

- **越走越歪、甚至原地打轉？** 修正方向相反了：把「30－fix」和「30＋fix」對調。
- **車子沒動，角度卻一直慢慢變？** 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **角度變化方向跟範例說的相反？** 感應器可能裝反了（上下顛倒）。範例 2、3 用「絕對值」所以不受影響；範例 4、5 請把程式裡的「＋」「－」對調。

---

## 範例 5：不倒的指南針：自己轉回原來的方向

**這個範例會做什麼：** 車子停在原地。你用手把它轉走，它會自己原地轉回原來的方向。這就是比賽中「轉回正前方」的基本做法。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- IMU 陀螺儀（EV3 版）

**怎麼接：** IMU 陀螺儀插 1 號孔，平放固定在車身上（不要晃動）。程式開始時車子要靜止不動。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/imu/ev3_beginner/ex05_hold_heading.ev3) ｜ [⬇ EV3 Classroom](/examples/imu/ev3_beginner/ex05_hold_heading.lmsp) ｜ [⬇ Pybricks v4](/examples/imu/ev3_beginner/ex05_hold_heading_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/imu/ev3_beginner/ex05_hold_heading_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/imu/ev3_beginner/ex05_hold_heading.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/imu/ev3-beginner/ex05_hold_heading-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex05_hold_heading-ev3g.webp" alt="IMU 陀螺儀 不倒的指南針：自己轉回原來的方向（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/imu/ev3-beginner/ex05_hold_heading-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/imu/ev3-beginner/ex05_hold_heading-classroom.webp" alt="IMU 陀螺儀 不倒的指南針：自己轉回原來的方向（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 05：不倒的指南針：自己轉回原來的方向
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子停在原地。你用手把它轉走，它會自己原地轉回原來的方向。這就是比賽中「轉回正前方」的基本做法。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 角度歸零：記住現在的方向
gyro.reset_angle(0)
# 一直重複
while True:
    # 修正力量 fix ＝ 角度 × 1：轉走越多，轉回來越用力
    fix = read_angle() * 1
    # 左輪＝－fix、右輪＝＋fix：原地往反方向轉回去
    left_motor.dc(0 - fix)
    right_motor.dc(fix)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 05：不倒的指南針：自己轉回原來的方向
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子停在原地。你用手把它轉走，它會自己原地轉回原來的方向。這就是比賽中「轉回正前方」的基本做法。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 角度歸零：記住現在的方向
gyro.reset_angle(0)
# 一直重複
while True:
    # 修正力量 fix ＝ 角度 × 1：轉走越多，轉回來越用力
    fix = read_angle() * 1
    # 左輪＝－fix、右輪＝＋fix：原地往反方向轉回去
    left_motor.dc(0 - fix)
    right_motor.dc(fix)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' IMU 陀螺儀感應器 EV3 新手範例 05：不倒的指南針：自己轉回原來的方向
' 軟體：clev3r
' 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：車子停在原地。你用手把它轉走，它會自己原地轉回原來的方向。這就是比賽中「轉回正前方」的基本做法。
' 讀值：angle＝機器人轉了幾度（連續累加（轉一圈＝360，可超過 360））
' ============================================================
folder "prjs" "MBC_imu_ex05"

' IMU 陀螺儀插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
' 陀螺儀歸零的指令（送 0x11 給感應器）
gyroReset[0] = 17
LCD.Clear()

' 角度歸零：記住現在的方向
Sensor.SendUARTData(1, 1, gyroReset)
Program.Delay(100)
' 一直重複
While "True"
  ' 修正力量 fix ＝ 角度 × 1：轉走越多，轉回來越用力
  fix = Sensor.ReadRawValue(1, 0) * 1
  ' 左輪＝－fix、右輪＝＋fix：原地往反方向轉回去
  Motor.StartPower("B", 0 - fix)
  Motor.StartPower("C", fix)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 角度歸零：記住現在的方向
1. 一直重複
   - 修正力量 fix ＝ 角度 × 1：轉走越多，轉回來越用力
   - 左輪＝－fix、右輪＝＋fix：原地往反方向轉回去

**跑起來應該看到：** 用手把車頭轉走，放手後它會自己轉回原來的方向並停住。

**常見問題：**

- **一直打轉停不下來？** 方向相反了：把「－fix」和「＋fix」對調。
- **在原位附近抖動？** 把 × 1 改小（例如 × 0.6）。
- **車子沒動，角度卻一直慢慢變？** 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **角度變化方向跟範例說的相反？** 感應器可能裝反了（上下顛倒）。範例 2、3 用「絕對值」所以不受影響；範例 4、5 請把程式裡的「＋」「－」對調。

---

## 範例 6：俯仰角：上坡就減速

> **僅 Pybricks v4／EV3 MicroPython v2.0／clev3r**（EV3-G、Classroom 的積木讀不到這個模式）

**這個範例會做什麼：** 模式 6 是「俯仰角」：車頭上抬或下壓幾度，平放是 0。車子在平地用 50 的速度跑，一上坡（或下坡）俯仰角超過 8 度就放慢到 20，比較不會翻車或衝太快。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- IMU 陀螺儀（EV3 版，韌體 v4.0 以上）
- 一塊斜板或書本堆成的小坡

**怎麼接：** IMU 陀螺儀插 1 號孔，平放固定在車身上，板子絲印的 X 軸要跟車子前後方向平行。

**下載程式（選你用的軟體）：**

[⬇ Pybricks v4](/examples/imu/ev3_beginner/ex06_tilt_slow_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/imu/ev3_beginner/ex06_tilt_slow_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/imu/ev3_beginner/ex06_tilt_slow.bp)

（程式碼即範例，不需截圖。）

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 06：俯仰角：上坡就減速
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 6 是「俯仰角」：車頭上抬或下壓幾度，平放是 0。車子在平地用 50 的速度跑，一上坡（或下坡）俯仰角超過 8 度就放慢到 20，比較不會翻車或衝太快。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # IMU 陀螺儀插 1 號孔（當一般 UART 裝置讀原始值）
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_tilt():
    # 俯仰角（車頭上抬或下壓幾度）：-90 ~ +90 度，平放＝0（板子絲印 X 軸）
    return sensor.read(6)[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示俯仰角（度）
    ev3.screen.draw_text(0, 0, "TILT " + str(read_tilt()) + "      ", Color.BLACK, Color.WHITE)
    # 如果俯仰角的絕對值大於 8 度
    if abs(read_tilt()) > 8:
        # 在坡上（俯仰角超過 8 度）：放慢到 20
        left_motor.dc(20)
        right_motor.dc(20)
        # 第 2 行顯示 SLOPE
        ev3.screen.draw_text(0, 16, "SLOPE      ", Color.BLACK, Color.WHITE)
    else:
        # 平地：用 50 往前開
        left_motor.dc(50)
        right_motor.dc(50)
        # 第 2 行顯示 FLAT
        ev3.screen.draw_text(0, 16, "FLAT      ", Color.BLACK, Color.WHITE)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 06：俯仰角：上坡就減速
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：模式 6 是「俯仰角」：車頭上抬或下壓幾度，平放是 0。車子在平地用 50 的速度跑，一上坡（或下坡）俯仰角超過 8 度就放慢到 20，比較不會翻車或衝太快。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # IMU 陀螺儀插 1 號孔（當一般 UART 裝置讀原始值）
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_tilt():
    # 俯仰角（車頭上抬或下壓幾度）：-90 ~ +90 度，平放＝0（板子絲印 X 軸）
    return sensor.read('TILT-ANG')[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示俯仰角（度）
    ev3.screen.draw_text(0, 0, "TILT " + str(read_tilt()) + "      ", Color.BLACK, Color.WHITE)
    # 如果俯仰角的絕對值大於 8 度
    if abs(read_tilt()) > 8:
        # 在坡上（俯仰角超過 8 度）：放慢到 20
        left_motor.dc(20)
        right_motor.dc(20)
        # 第 2 行顯示 SLOPE
        ev3.screen.draw_text(0, 16, "SLOPE      ", Color.BLACK, Color.WHITE)
    else:
        # 平地：用 50 往前開
        left_motor.dc(50)
        right_motor.dc(50)
        # 第 2 行顯示 FLAT
        ev3.screen.draw_text(0, 16, "FLAT      ", Color.BLACK, Color.WHITE)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' IMU 陀螺儀感應器 EV3 新手範例 06：俯仰角：上坡就減速
' 軟體：clev3r
' 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 6 是「俯仰角」：車頭上抬或下壓幾度，平放是 0。車子在平地用 50 的速度跑，一上坡（或下坡）俯仰角超過 8 度就放慢到 20，比較不會翻車或衝太快。
' 讀值：tilt＝俯仰角（車頭上抬或下壓幾度）（-90 ~ +90 度，平放＝0（板子絲印 X 軸））
' ============================================================
folder "prjs" "MBC_imu_ex06"

' IMU 陀螺儀插 1 號孔，使用模式 6
Sensor.SetMode(1, 6)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 第 1 行顯示俯仰角（度）
  LCD.Text(1, 0, 0, 1, "TILT " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 如果俯仰角的絕對值大於 8 度
  If Math.Abs(Sensor.ReadRawValue(1, 0)) > 8 Then
    ' 在坡上（俯仰角超過 8 度）：放慢到 20
    Motor.StartPower("B", 20)
    Motor.StartPower("C", 20)
    ' 第 2 行顯示 SLOPE
    LCD.Text(1, 0, 16, 1, "SLOPE      ")
  Else
    ' 平地：用 50 往前開
    Motor.StartPower("B", 50)
    Motor.StartPower("C", 50)
    ' 第 2 行顯示 FLAT
    LCD.Text(1, 0, 16, 1, "FLAT      ")
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 一直重複
   - 第 1 行顯示俯仰角（度）
   - 如果俯仰角的絕對值大於 8 度
      - 在坡上（俯仰角超過 8 度）：放慢到 20
      - 第 2 行顯示 SLOPE
      - 否則：
         - 平地：用 50 往前開
         - 第 2 行顯示 FLAT

**跑起來應該看到：** 平地上車子快跑、螢幕顯示 FLAT 和 0 附近的角度；開上斜板時角度變大、顯示 SLOPE 並明顯變慢；回到平地又加速。

**常見問題：**

- **平地上角度就不是 0？** 感應器沒有平放，或裝的方向轉了 90 度（俯仰角看的是板子絲印的 X 軸）。
- **上坡角度是負的？** 正常，正負跟安裝方向有關；這個範例用絕對值，所以上坡下坡都會減速。
- **車子沒動，角度卻一直慢慢變？** 開機或歸零的那一下車子要完全靜止。把車子放好，拔掉感應器重插一次再試。
- **數字一直是 0 或不會動？** 感應器韌體要 v4.0（2026-10）以上才有這個模式，舊韌體請先更新。
- **EV3-G／Classroom 怎麼沒有這個範例？** 這個範例用的是新模式（2026-10 韌體 v4 起），EV3-G 和 Classroom 的紅外線積木讀不到，只有 Pybricks、EV3 MicroPython、clev3r 能用。

---

## 範例 7：轉太快就警告

> **僅 Pybricks v4／EV3 MicroPython v2.0／clev3r**（EV3-G、Classroom 的積木讀不到這個模式）

**這個範例會做什麼：** 模式 2 是「轉向角速度」：現在每秒轉幾度（×10）。角度告訴你轉了多少，角速度告訴你轉得多快。這個範例把角速度顯示出來，轉太快（超過每秒 90 度，也就是 900）就嗶一聲，可以拿來檢查機器人轉彎會不會甩尾。

**要準備什麼：**

- IMU 陀螺儀（EV3 版，韌體 v4.0 以上）
- EV3 主機

**怎麼接：** IMU 陀螺儀插 1 號孔，平放固定在車身上。

**下載程式（選你用的軟體）：**

[⬇ Pybricks v4](/examples/imu/ev3_beginner/ex07_rate_warn_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/imu/ev3_beginner/ex07_rate_warn_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/imu/ev3_beginner/ex07_rate_warn.bp)

（程式碼即範例，不需截圖。）

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 07：轉太快就警告
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：IMU 陀螺儀插 1 號孔
# 這個範例會做什麼：模式 2 是「轉向角速度」：現在每秒轉幾度（×10）。角度告訴你轉了多少，角速度告訴你轉得多快。這個範例把角速度顯示出來，轉太快（超過每秒 90 度，也就是 900）就嗶一聲，可以拿來檢查機器人轉彎會不會甩尾。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # IMU 陀螺儀插 1 號孔（當一般 UART 裝置讀原始值）


def read_rate():
    # 轉向的角速度（每秒幾度 × 10）：±3000 左右，正負跟角度同方向；100＝每秒 10 度
    return sensor.read(2)[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示角速度（每秒幾度 × 10）
    ev3.screen.draw_text(0, 0, "RATE " + str(read_rate()) + "      ", Color.BLACK, Color.WHITE)
    # 如果角速度的絕對值大於 900（每秒 90 度）
    if abs(read_rate()) > 900:
        # 轉太快（超過每秒 90 度）：嗶一聲
        ev3.speaker.beep()
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 07：轉太快就警告
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔
# 這個範例會做什麼：模式 2 是「轉向角速度」：現在每秒轉幾度（×10）。角度告訴你轉了多少，角速度告訴你轉得多快。這個範例把角速度顯示出來，轉太快（超過每秒 90 度，也就是 900）就嗶一聲，可以拿來檢查機器人轉彎會不會甩尾。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # IMU 陀螺儀插 1 號孔（當一般 UART 裝置讀原始值）


def read_rate():
    # 轉向的角速度（每秒幾度 × 10）：±3000 左右，正負跟角度同方向；100＝每秒 10 度
    return sensor.read('GYRO-FAS')[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示角速度（每秒幾度 × 10）
    ev3.screen.draw_text(0, 0, "RATE " + str(read_rate()) + "      ", Color.BLACK, Color.WHITE)
    # 如果角速度的絕對值大於 900（每秒 90 度）
    if abs(read_rate()) > 900:
        # 轉太快（超過每秒 90 度）：嗶一聲
        ev3.speaker.beep()
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' IMU 陀螺儀感應器 EV3 新手範例 07：轉太快就警告
' 軟體：clev3r
' 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 2 是「轉向角速度」：現在每秒轉幾度（×10）。角度告訴你轉了多少，角速度告訴你轉得多快。這個範例把角速度顯示出來，轉太快（超過每秒 90 度，也就是 900）就嗶一聲，可以拿來檢查機器人轉彎會不會甩尾。
' 讀值：rate＝轉向的角速度（每秒幾度 × 10）（±3000 左右，正負跟角度同方向；100＝每秒 10 度）
' ============================================================
folder "prjs" "MBC_imu_ex07"

' IMU 陀螺儀插 1 號孔，使用模式 2
Sensor.SetMode(1, 2)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 第 1 行顯示角速度（每秒幾度 × 10）
  LCD.Text(1, 0, 0, 1, "RATE " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 如果角速度的絕對值大於 900（每秒 90 度）
  If Math.Abs(Sensor.ReadRawValue(1, 0)) > 900 Then
    ' 轉太快（超過每秒 90 度）：嗶一聲
    Speaker.Tone(50, 1000, 100)
    Speaker.Wait()
  EndIf
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 一直重複
   - 第 1 行顯示角速度（每秒幾度 × 10）
   - 如果角速度的絕對值大於 900（每秒 90 度）
      - 轉太快（超過每秒 90 度）：嗶一聲
   - 等 0.1 秒

**跑起來應該看到：** 機器人不動時數字在 0 附近；用手慢慢轉它，數字幾百；快速一甩，數字超過 900 並嗶一聲。往另一邊轉數字是負的。

**常見問題：**

- **不動也一直嗶？** 把 900 改大，或確認感應器有固定好、沒有在晃。
- **數字一直是 0 或不會動？** 感應器韌體要 v4.0（2026-10）以上才有這個模式，舊韌體請先更新。
- **EV3-G／Classroom 怎麼沒有這個範例？** 這個範例用的是新模式（2026-10 韌體 v4 起），EV3-G 和 Classroom 的紅外線積木讀不到，只有 Pybricks、EV3 MicroPython、clev3r 能用。

---

還有問題？歡迎到 [聯絡我們](/contact.md) 留言，附上你用的軟體和程式截圖，我們會幫你看。
