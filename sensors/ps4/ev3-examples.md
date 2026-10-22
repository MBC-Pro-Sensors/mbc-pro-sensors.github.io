<!-- 這頁由 95_產品網站\EV3新手範例產生器 自動產生，請改產生器再重產，不要直接改這個檔。 -->
# 🧱 EV3 新手範例：掌控者 PS4

[🔙 回到 掌控者 PS4產品頁](/sensors/ps4/index.md)

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

## 這頁會用到的 10 個數值

| 名稱 | 範圍 | 從哪裡讀 |
|---|---|---|
| **方向鍵（上 1、右 2、下 4、左 8）** | 0 ~ 15 | 模式 1 第 1 個值 |
| **形狀鍵（▲1、●2、✕4、■8）** | 0 ~ 15 | 模式 1 第 3 個值 |
| **肩鍵（L2 1、R2 2、L1 4、R1 8）** | 0 ~ 15 | 模式 1 第 5 個值 |
| **功能鍵（SELECT 1、L3 2、R3 4、START 8）** | 0 ~ 15 | 模式 1 第 7 個值 |
| **左搖桿左右** | 0 ~ 99，50＝中間，往右變大 | 模式 1 第 2 個值 |
| **左搖桿上下** | 0 ~ 99，50＝中間，往上變大 | 模式 1 第 4 個值 |
| **右搖桿左右** | 0 ~ 99，50＝中間，往右變大 | 模式 1 第 6 個值 |
| **右搖桿上下** | 0 ~ 99，50＝中間，往上變大 | 模式 1 第 8 個值 |
| **方向鍵（上 1、右 2、下 4、左 8，同時按會相加）** | 0 ~ 15 | 模式 0 |
| **方向鍵＋形狀鍵＋連線狀態** | 0 ~ 511：方向鍵（0~15）＋ 形狀鍵 × 16 ＋ 已連線 × 256，大於等於 256＝手把已連上 | 模式 3 第 1 個值（只有 Pybricks／EV3 MicroPython／clev3r 讀得到） |

**範例列表：**

1. [按鍵測試器：看看每個按鍵的數字](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E6%8C%89%E9%8D%B5%E6%B8%AC%E8%A9%A6%E5%99%A8%EF%BC%9A%E7%9C%8B%E7%9C%8B%E6%AF%8F%E5%80%8B%E6%8C%89%E9%8D%B5%E7%9A%84%E6%95%B8%E5%AD%97)
2. [方向鍵開車](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E6%96%B9%E5%90%91%E9%8D%B5%E9%96%8B%E8%BB%8A)
3. [左搖桿控制一顆馬達的快慢](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E5%B7%A6%E6%90%96%E6%A1%BF%E6%8E%A7%E5%88%B6%E4%B8%80%E9%A1%86%E9%A6%AC%E9%81%94%E7%9A%84%E5%BF%AB%E6%85%A2)
4. [雙搖桿坦克車](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E9%9B%99%E6%90%96%E6%A1%BF%E5%9D%A6%E5%85%8B%E8%BB%8A)
5. [✕ 夾、● 放：控制夾爪](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E2%9C%95-%E5%A4%BE%E3%80%81%E2%97%8F-%E6%94%BE%EF%BC%9A%E6%8E%A7%E5%88%B6%E5%A4%BE%E7%88%AA)
6. [手把連上才出發](#%E7%AF%84%E4%BE%8B-6%EF%BC%9A%E6%89%8B%E6%8A%8A%E9%80%A3%E4%B8%8A%E6%89%8D%E5%87%BA%E7%99%BC)（僅 Pybricks／EV3 MicroPython／clev3r）

---

## 範例 1：按鍵測試器：看看每個按鍵的數字

**這個範例會做什麼：** 模式 1 一次送出 8 個數字：4 組按鍵和 4 個搖桿值。這個範例把它們全部顯示在螢幕上，按按看、推推看，就知道每個按鍵是多少。

**要準備什麼：**

- 掌控者 PS4藍牙接收器（EV3 版）＋手把
- EV3 主機

**怎麼接：** 藍牙接收器插 1 號孔。第一次使用先照產品頁說明配對 PS4/PS5 手把；之後開機按 PS 鍵會自動連線。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/ps4/ev3_beginner/ex01_button_tester.ev3) ｜ [⬇ EV3 Classroom](/examples/ps4/ev3_beginner/ex01_button_tester.lmsp) ｜ [⬇ Pybricks v4](/examples/ps4/ev3_beginner/ex01_button_tester_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/ps4/ev3_beginner/ex01_button_tester_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/ps4/ev3_beginner/ex01_button_tester.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex01_button_tester-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex01_button_tester-ev3g.webp" alt="掌控者 PS4 按鍵測試器：看看每個按鍵的數字（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex01_button_tester-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex01_button_tester-classroom.webp" alt="掌控者 PS4 按鍵測試器：看看每個按鍵的數字（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 01：按鍵測試器：看看每個按鍵的數字
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：掌控者 PS4插 1 號孔
# 這個範例會做什麼：模式 1 一次送出 8 個數字：4 組按鍵和 4 個搖桿值。這個範例把它們全部顯示在螢幕上，按按看、推推看，就知道每個按鍵是多少。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 掌控者 PS4插 1 號孔


def read_dpad1():
    # 方向鍵（上 1、右 2、下 4、左 8）：0 ~ 15
    return sensor.read(1)[0]


def read_shape():
    # 形狀鍵（▲1、●2、✕4、■8）：0 ~ 15
    return sensor.read(1)[2]


def read_shoulder():
    # 肩鍵（L2 1、R2 2、L1 4、R1 8）：0 ~ 15
    return sensor.read(1)[4]


def read_func():
    # 功能鍵（SELECT 1、L3 2、R3 4、START 8）：0 ~ 15
    return sensor.read(1)[6]


def read_lx():
    # 左搖桿左右：0 ~ 99，50＝中間，往右變大
    return sensor.read(1)[1]


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[3]


def read_rx():
    # 右搖桿左右：0 ~ 99，50＝中間，往右變大
    return sensor.read(1)[5]


def read_ry():
    # 右搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[7]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 方向鍵：上 1、右 2、下 4、左 8
    ev3.screen.draw_text(0, 0, "DPAD " + str(read_dpad1()) + "      ", Color.BLACK, Color.WHITE)
    # 形狀鍵：▲1、●2、✕4、■8
    ev3.screen.draw_text(0, 16, "SHAPE " + str(read_shape()) + "      ", Color.BLACK, Color.WHITE)
    # 肩鍵：L2 1、R2 2、L1 4、R1 8
    ev3.screen.draw_text(0, 32, "SHLD " + str(read_shoulder()) + "      ", Color.BLACK, Color.WHITE)
    # 功能鍵：SELECT 1、L3 2、R3 4、START 8
    ev3.screen.draw_text(0, 48, "FUNC " + str(read_func()) + "      ", Color.BLACK, Color.WHITE)
    # 左搖桿左右（50＝中間）
    ev3.screen.draw_text(0, 64, "LX " + str(read_lx()) + "      ", Color.BLACK, Color.WHITE)
    # 左搖桿上下（50＝中間）
    ev3.screen.draw_text(0, 80, "LY " + str(read_ly()) + "      ", Color.BLACK, Color.WHITE)
    # 右搖桿左右
    ev3.screen.draw_text(0, 96, "RX " + str(read_rx()) + "      ", Color.BLACK, Color.WHITE)
    # 右搖桿上下
    ev3.screen.draw_text(0, 112, "RY " + str(read_ry()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 01：按鍵測試器：看看每個按鍵的數字
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS4插 1 號孔
# 這個範例會做什麼：模式 1 一次送出 8 個數字：4 組按鍵和 4 個搖桿值。這個範例把它們全部顯示在螢幕上，按按看、推推看，就知道每個按鍵是多少。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS4插 1 號孔


def read_dpad1():
    # 方向鍵（上 1、右 2、下 4、左 8）：0 ~ 15
    return sensor.read('IR-SEEK')[0]


def read_shape():
    # 形狀鍵（▲1、●2、✕4、■8）：0 ~ 15
    return sensor.read('IR-SEEK')[2]


def read_shoulder():
    # 肩鍵（L2 1、R2 2、L1 4、R1 8）：0 ~ 15
    return sensor.read('IR-SEEK')[4]


def read_func():
    # 功能鍵（SELECT 1、L3 2、R3 4、START 8）：0 ~ 15
    return sensor.read('IR-SEEK')[6]


def read_lx():
    # 左搖桿左右：0 ~ 99，50＝中間，往右變大
    return sensor.read('IR-SEEK')[1]


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read('IR-SEEK')[3]


def read_rx():
    # 右搖桿左右：0 ~ 99，50＝中間，往右變大
    return sensor.read('IR-SEEK')[5]


def read_ry():
    # 右搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read('IR-SEEK')[7]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 方向鍵：上 1、右 2、下 4、左 8
    ev3.screen.draw_text(0, 0, "DPAD " + str(read_dpad1()) + "      ", Color.BLACK, Color.WHITE)
    # 形狀鍵：▲1、●2、✕4、■8
    ev3.screen.draw_text(0, 16, "SHAPE " + str(read_shape()) + "      ", Color.BLACK, Color.WHITE)
    # 肩鍵：L2 1、R2 2、L1 4、R1 8
    ev3.screen.draw_text(0, 32, "SHLD " + str(read_shoulder()) + "      ", Color.BLACK, Color.WHITE)
    # 功能鍵：SELECT 1、L3 2、R3 4、START 8
    ev3.screen.draw_text(0, 48, "FUNC " + str(read_func()) + "      ", Color.BLACK, Color.WHITE)
    # 左搖桿左右（50＝中間）
    ev3.screen.draw_text(0, 64, "LX " + str(read_lx()) + "      ", Color.BLACK, Color.WHITE)
    # 左搖桿上下（50＝中間）
    ev3.screen.draw_text(0, 80, "LY " + str(read_ly()) + "      ", Color.BLACK, Color.WHITE)
    # 右搖桿左右
    ev3.screen.draw_text(0, 96, "RX " + str(read_rx()) + "      ", Color.BLACK, Color.WHITE)
    # 右搖桿上下
    ev3.screen.draw_text(0, 112, "RY " + str(read_ry()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 01：按鍵測試器：看看每個按鍵的數字
' 軟體：clev3r
' 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 1 一次送出 8 個數字：4 組按鍵和 4 個搖桿值。這個範例把它們全部顯示在螢幕上，按按看、推推看，就知道每個按鍵是多少。
' 讀值：dpad1＝方向鍵（上 1、右 2、下 4、左 8）（0 ~ 15）；shape＝形狀鍵（▲1、●2、✕4、■8）（0 ~ 15）；shoulder＝肩鍵（L2 1、R2 2、L1 4、R1 8）（0 ~ 15）；func＝功能鍵（SELECT 1、L3 2、R3 4、START 8）（0 ~ 15）；lx＝左搖桿左右（0 ~ 99，50＝中間，往右變大）；ly＝左搖桿上下（0 ~ 99，50＝中間，往上變大）；rx＝右搖桿左右（0 ~ 99，50＝中間，往右變大）；ry＝右搖桿上下（0 ~ 99，50＝中間，往上變大）
' ============================================================
folder "prjs" "MBC_ps4_ex01"

' 掌控者 PS4插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 方向鍵：上 1、右 2、下 4、左 8
  LCD.Text(1, 0, 0, 1, "DPAD " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 形狀鍵：▲1、●2、✕4、■8
  LCD.Text(1, 0, 16, 1, "SHAPE " + Sensor.ReadRawValue(1, 2) + "      ")
  ' 肩鍵：L2 1、R2 2、L1 4、R1 8
  LCD.Text(1, 0, 32, 1, "SHLD " + Sensor.ReadRawValue(1, 4) + "      ")
  ' 功能鍵：SELECT 1、L3 2、R3 4、START 8
  LCD.Text(1, 0, 48, 1, "FUNC " + Sensor.ReadRawValue(1, 6) + "      ")
  ' 左搖桿左右（50＝中間）
  LCD.Text(1, 0, 64, 1, "LX " + Sensor.ReadRawValue(1, 1) + "      ")
  ' 左搖桿上下（50＝中間）
  LCD.Text(1, 0, 80, 1, "LY " + Sensor.ReadRawValue(1, 3) + "      ")
  ' 右搖桿左右
  LCD.Text(1, 0, 96, 1, "RX " + Sensor.ReadRawValue(1, 5) + "      ")
  ' 右搖桿上下
  LCD.Text(1, 0, 112, 1, "RY " + Sensor.ReadRawValue(1, 7) + "      ")
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 一直重複
   - 方向鍵：上 1、右 2、下 4、左 8
   - 形狀鍵：▲1、●2、✕4、■8
   - 肩鍵：L2 1、R2 2、L1 4、R1 8
   - 功能鍵：SELECT 1、L3 2、R3 4、START 8
   - 左搖桿左右（50＝中間）
   - 左搖桿上下（50＝中間）
   - 右搖桿左右
   - 右搖桿上下
   - 等 0.1 秒

**跑起來應該看到：** 沒按任何鍵時，按鍵都是 0、搖桿都是 50。同時按兩個鍵數字會相加，例如同時按上和右＝3。

**常見問題：**

- **按鍵沒反應？** 先跑範例 1 看數字有沒有變。不會變就是手把沒連上：重新開手把電源，或 EV3 先開機再接接收器。
- **搖桿放開了車子還在慢慢動？** 搖桿中間不一定剛好 50，把程式裡的 × 2 改小，或加一段「數字在 45~55 之間就當成 50」。

---

## 範例 2：方向鍵開車

**這個範例會做什麼：** 用模式 0 只讀方向鍵，上前進、下後退、左右原地轉，放開就停。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 掌控者 PS4藍牙接收器（EV3 版）＋手把

**怎麼接：** 藍牙接收器插 1 號孔。第一次使用先照產品頁說明配對 PS4/PS5 手把；之後開機按 PS 鍵會自動連線。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/ps4/ev3_beginner/ex02_dpad_drive.ev3) ｜ [⬇ EV3 Classroom](/examples/ps4/ev3_beginner/ex02_dpad_drive.lmsp) ｜ [⬇ Pybricks v4](/examples/ps4/ev3_beginner/ex02_dpad_drive_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/ps4/ev3_beginner/ex02_dpad_drive_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/ps4/ev3_beginner/ex02_dpad_drive.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex02_dpad_drive-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex02_dpad_drive-ev3g.webp" alt="掌控者 PS4 方向鍵開車（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex02_dpad_drive-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex02_dpad_drive-classroom.webp" alt="掌控者 PS4 方向鍵開車（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 02：方向鍵開車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用模式 0 只讀方向鍵，上前進、下後退、左右原地轉，放開就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 掌控者 PS4插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dpad():
    # 方向鍵（上 1、右 2、下 4、左 8，同時按會相加）：0 ~ 15
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果方向鍵＝1（上）
    if read_dpad() == 1:
        # 按上：前進
        left_motor.dc(50)
        right_motor.dc(50)
    # 否則，如果方向鍵＝4（下）
    elif read_dpad() == 4:
        # 按下：後退
        left_motor.dc(-50)
        right_motor.dc(-50)
    # 否則，如果方向鍵＝8（左）
    elif read_dpad() == 8:
        # 按左：原地左轉
        left_motor.dc(-30)
        right_motor.dc(30)
    # 否則，如果方向鍵＝2（右）
    elif read_dpad() == 2:
        # 按右：原地右轉
        left_motor.dc(30)
        right_motor.dc(-30)
    else:
        # 都沒按：停車
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 02：方向鍵開車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用模式 0 只讀方向鍵，上前進、下後退、左右原地轉，放開就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS4插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dpad():
    # 方向鍵（上 1、右 2、下 4、左 8，同時按會相加）：0 ~ 15
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果方向鍵＝1（上）
    if read_dpad() == 1:
        # 按上：前進
        left_motor.dc(50)
        right_motor.dc(50)
    # 否則，如果方向鍵＝4（下）
    elif read_dpad() == 4:
        # 按下：後退
        left_motor.dc(-50)
        right_motor.dc(-50)
    # 否則，如果方向鍵＝8（左）
    elif read_dpad() == 8:
        # 按左：原地左轉
        left_motor.dc(-30)
        right_motor.dc(30)
    # 否則，如果方向鍵＝2（右）
    elif read_dpad() == 2:
        # 按右：原地右轉
        left_motor.dc(30)
        right_motor.dc(-30)
    else:
        # 都沒按：停車
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 02：方向鍵開車
' 軟體：clev3r
' 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：用模式 0 只讀方向鍵，上前進、下後退、左右原地轉，放開就停。
' 讀值：dpad＝方向鍵（上 1、右 2、下 4、左 8，同時按會相加）（0 ~ 15）
' ============================================================
folder "prjs" "MBC_ps4_ex02"

' 掌控者 PS4插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複
While "True"
  ' 如果方向鍵＝1（上）
  If Sensor.ReadRawValue(1, 0) = 1 Then
    ' 按上：前進
    Motor.StartPower("B", 50)
    Motor.StartPower("C", 50)
  ' 否則，如果方向鍵＝4（下）
  ElseIf Sensor.ReadRawValue(1, 0) = 4 Then
    ' 按下：後退
    Motor.StartPower("B", -50)
    Motor.StartPower("C", -50)
  ' 否則，如果方向鍵＝8（左）
  ElseIf Sensor.ReadRawValue(1, 0) = 8 Then
    ' 按左：原地左轉
    Motor.StartPower("B", -30)
    Motor.StartPower("C", 30)
  ' 否則，如果方向鍵＝2（右）
  ElseIf Sensor.ReadRawValue(1, 0) = 2 Then
    ' 按右：原地右轉
    Motor.StartPower("B", 30)
    Motor.StartPower("C", -30)
  Else
    ' 都沒按：停車
    Motor.Stop("BC", "True")
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 如果方向鍵＝1（上）
      - 按上：前進
      - 否則：
         - 否則，如果方向鍵＝4（下）
            - 按下：後退
            - 否則：
               - 否則，如果方向鍵＝8（左）
                  - 按左：原地左轉
                  - 否則：
                     - 否則，如果方向鍵＝2（右）
                        - 按右：原地右轉
                        - 否則：
                           - 都沒按：停車

**跑起來應該看到：** 按住方向鍵車子就動，放開就停。

**常見問題：**

- **按鍵沒反應？** 先跑範例 1 看數字有沒有變。不會變就是手把沒連上：重新開手把電源，或 EV3 先開機再接接收器。
- **搖桿放開了車子還在慢慢動？** 搖桿中間不一定剛好 50，把程式裡的 × 2 改小，或加一段「數字在 45~55 之間就當成 50」。

---

## 範例 3：左搖桿控制一顆馬達的快慢

**這個範例會做什麼：** 搖桿的數字是 0~99、中間 50。把它減 50 再乘 2，就變成馬達要的 -100~+100：推越多轉越快，往下推就倒轉。

**要準備什麼：**

- EV3 主機
- 一顆大馬達接 B
- 掌控者 PS4藍牙接收器（EV3 版）＋手把

**怎麼接：** 藍牙接收器插 1 號孔。第一次使用先照產品頁說明配對 PS4/PS5 手把；之後開機按 PS 鍵會自動連線。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/ps4/ev3_beginner/ex03_stick_one_motor.ev3) ｜ [⬇ EV3 Classroom](/examples/ps4/ev3_beginner/ex03_stick_one_motor.lmsp) ｜ [⬇ Pybricks v4](/examples/ps4/ev3_beginner/ex03_stick_one_motor_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/ps4/ev3_beginner/ex03_stick_one_motor_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/ps4/ev3_beginner/ex03_stick_one_motor.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex03_stick_one_motor-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex03_stick_one_motor-ev3g.webp" alt="掌控者 PS4 左搖桿控制一顆馬達的快慢（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex03_stick_one_motor-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex03_stick_one_motor-classroom.webp" alt="掌控者 PS4 左搖桿控制一顆馬達的快慢（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 03：左搖桿控制一顆馬達的快慢
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：掌控者 PS4插 1 號孔；馬達接 B
# 這個範例會做什麼：搖桿的數字是 0~99、中間 50。把它減 50 再乘 2，就變成馬達要的 -100~+100：推越多轉越快，往下推就倒轉。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 掌控者 PS4插 1 號孔
motor_b = Motor(Port.B)             # B 埠馬達


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[3]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 力量 power ＝（左搖桿上下 − 50）× 2
    power = (read_ly() - 50) * 2
    # B 馬達用 power 的力量轉
    motor_b.dc(power)
    # 螢幕顯示現在的力量
    ev3.screen.draw_text(0, 0, "POWER " + str(power) + "      ", Color.BLACK, Color.WHITE)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 03：左搖桿控制一顆馬達的快慢
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS4插 1 號孔；馬達接 B
# 這個範例會做什麼：搖桿的數字是 0~99、中間 50。把它減 50 再乘 2，就變成馬達要的 -100~+100：推越多轉越快，往下推就倒轉。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS4插 1 號孔
motor_b = Motor(Port.B)             # B 埠馬達


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read('IR-SEEK')[3]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 力量 power ＝（左搖桿上下 − 50）× 2
    power = (read_ly() - 50) * 2
    # B 馬達用 power 的力量轉
    motor_b.dc(power)
    # 螢幕顯示現在的力量
    ev3.screen.draw_text(0, 0, "POWER " + str(power) + "      ", Color.BLACK, Color.WHITE)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 03：左搖桿控制一顆馬達的快慢
' 軟體：clev3r
' 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：搖桿的數字是 0~99、中間 50。把它減 50 再乘 2，就變成馬達要的 -100~+100：推越多轉越快，往下推就倒轉。
' 讀值：ly＝左搖桿上下（0 ~ 99，50＝中間，往上變大）
' ============================================================
folder "prjs" "MBC_ps4_ex03"

' 掌控者 PS4插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 一直重複
While "True"
  ' 力量 power ＝（左搖桿上下 − 50）× 2
  power = (Sensor.ReadRawValue(1, 3) - 50) * 2
  ' B 馬達用 power 的力量轉
  Motor.StartPower("B", power)
  ' 螢幕顯示現在的力量
  LCD.Text(1, 0, 0, 1, "POWER " + power + "      ")
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 力量 power ＝（左搖桿上下 − 50）× 2
   - B 馬達用 power 的力量轉
   - 螢幕顯示現在的力量

**跑起來應該看到：** 左搖桿往上推，B 馬達正轉，越推越快；往下推倒轉；放開停止，螢幕顯示 0 左右。

**常見問題：**

- **按鍵沒反應？** 先跑範例 1 看數字有沒有變。不會變就是手把沒連上：重新開手把電源，或 EV3 先開機再接接收器。
- **搖桿放開了車子還在慢慢動？** 搖桿中間不一定剛好 50，把程式裡的 × 2 改小，或加一段「數字在 45~55 之間就當成 50」。

---

## 範例 4：雙搖桿坦克車

**這個範例會做什麼：** 左搖桿管左輪、右搖桿管右輪，跟開坦克一樣：兩個都往前推就前進，一前一後就原地轉。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 掌控者 PS4藍牙接收器（EV3 版）＋手把

**怎麼接：** 藍牙接收器插 1 號孔。第一次使用先照產品頁說明配對 PS4/PS5 手把；之後開機按 PS 鍵會自動連線。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/ps4/ev3_beginner/ex04_tank_drive.ev3) ｜ [⬇ EV3 Classroom](/examples/ps4/ev3_beginner/ex04_tank_drive.lmsp) ｜ [⬇ Pybricks v4](/examples/ps4/ev3_beginner/ex04_tank_drive_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/ps4/ev3_beginner/ex04_tank_drive_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/ps4/ev3_beginner/ex04_tank_drive.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex04_tank_drive-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex04_tank_drive-ev3g.webp" alt="掌控者 PS4 雙搖桿坦克車（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex04_tank_drive-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex04_tank_drive-classroom.webp" alt="掌控者 PS4 雙搖桿坦克車（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 04：雙搖桿坦克車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：左搖桿管左輪、右搖桿管右輪，跟開坦克一樣：兩個都往前推就前進，一前一後就原地轉。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 掌控者 PS4插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[3]


def read_ry():
    # 右搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read(1)[7]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 左輪力量 ＝（左搖桿上下 − 50）× 2
    left = (read_ly() - 50) * 2
    # 右輪力量 ＝（右搖桿上下 − 50）× 2
    right = (read_ry() - 50) * 2
    # 兩輪各自用自己的力量轉
    left_motor.dc(left)
    right_motor.dc(right)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 04：雙搖桿坦克車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：左搖桿管左輪、右搖桿管右輪，跟開坦克一樣：兩個都往前推就前進，一前一後就原地轉。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS4插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_ly():
    # 左搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read('IR-SEEK')[3]


def read_ry():
    # 右搖桿上下：0 ~ 99，50＝中間，往上變大
    return sensor.read('IR-SEEK')[7]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 左輪力量 ＝（左搖桿上下 − 50）× 2
    left = (read_ly() - 50) * 2
    # 右輪力量 ＝（右搖桿上下 − 50）× 2
    right = (read_ry() - 50) * 2
    # 兩輪各自用自己的力量轉
    left_motor.dc(left)
    right_motor.dc(right)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 04：雙搖桿坦克車
' 軟體：clev3r
' 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：左搖桿管左輪、右搖桿管右輪，跟開坦克一樣：兩個都往前推就前進，一前一後就原地轉。
' 讀值：ly＝左搖桿上下（0 ~ 99，50＝中間，往上變大）；ry＝右搖桿上下（0 ~ 99，50＝中間，往上變大）
' ============================================================
folder "prjs" "MBC_ps4_ex04"

' 掌控者 PS4插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 一直重複
While "True"
  ' 左輪力量 ＝（左搖桿上下 − 50）× 2
  left = (Sensor.ReadRawValue(1, 3) - 50) * 2
  ' 右輪力量 ＝（右搖桿上下 − 50）× 2
  right = (Sensor.ReadRawValue(1, 7) - 50) * 2
  ' 兩輪各自用自己的力量轉
  Motor.StartPower("B", left)
  Motor.StartPower("C", right)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 左輪力量 ＝（左搖桿上下 − 50）× 2
   - 右輪力量 ＝（右搖桿上下 − 50）× 2
   - 兩輪各自用自己的力量轉

**跑起來應該看到：** 兩支搖桿一起往前推，車子前進；左推前、右推後，車子原地右轉。

**常見問題：**

- **按鍵沒反應？** 先跑範例 1 看數字有沒有變。不會變就是手把沒連上：重新開手把電源，或 EV3 先開機再接接收器。
- **搖桿放開了車子還在慢慢動？** 搖桿中間不一定剛好 50，把程式裡的 × 2 改小，或加一段「數字在 45~55 之間就當成 50」。

---

## 範例 5：✕ 夾、● 放：控制夾爪

**這個範例會做什麼：** 用形狀鍵控制接在 A 埠的中馬達（夾爪）：按 ✕ 夾緊、按 ● 放開，都不按就停。

**要準備什麼：**

- EV3 主機
- 中馬達接 A（做成夾爪）
- 掌控者 PS4藍牙接收器（EV3 版）＋手把

**怎麼接：** 藍牙接收器插 1 號孔。第一次使用先照產品頁說明配對 PS4/PS5 手把；之後開機按 PS 鍵會自動連線。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/ps4/ev3_beginner/ex05_gripper.ev3) ｜ [⬇ EV3 Classroom](/examples/ps4/ev3_beginner/ex05_gripper.lmsp) ｜ [⬇ Pybricks v4](/examples/ps4/ev3_beginner/ex05_gripper_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/ps4/ev3_beginner/ex05_gripper_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/ps4/ev3_beginner/ex05_gripper.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex05_gripper-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex05_gripper-ev3g.webp" alt="掌控者 PS4 ✕ 夾、● 放：控制夾爪（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/ps4/ev3-beginner/ex05_gripper-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/ps4/ev3-beginner/ex05_gripper-classroom.webp" alt="掌控者 PS4 ✕ 夾、● 放：控制夾爪（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 05：✕ 夾、● 放：控制夾爪
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：掌控者 PS4插 1 號孔；馬達接 A
# 這個範例會做什麼：用形狀鍵控制接在 A 埠的中馬達（夾爪）：按 ✕ 夾緊、按 ● 放開，都不按就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 掌控者 PS4插 1 號孔
motor_a = Motor(Port.A)             # A 埠馬達


def read_shape():
    # 形狀鍵（▲1、●2、✕4、■8）：0 ~ 15
    return sensor.read(1)[2]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果形狀鍵＝4（✕）
    if read_shape() == 4:
        # 按 ✕：夾爪往夾緊的方向轉
        motor_a.dc(40)
    # 否則，如果形狀鍵＝2（●）
    elif read_shape() == 2:
        # 按 ●：夾爪往放開的方向轉
        motor_a.dc(-40)
    else:
        # 都沒按：夾爪停住
        motor_a.brake()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 05：✕ 夾、● 放：控制夾爪
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS4插 1 號孔；馬達接 A
# 這個範例會做什麼：用形狀鍵控制接在 A 埠的中馬達（夾爪）：按 ✕ 夾緊、按 ● 放開，都不按就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS4插 1 號孔
motor_a = Motor(Port.A)             # A 埠馬達


def read_shape():
    # 形狀鍵（▲1、●2、✕4、■8）：0 ~ 15
    return sensor.read('IR-SEEK')[2]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果形狀鍵＝4（✕）
    if read_shape() == 4:
        # 按 ✕：夾爪往夾緊的方向轉
        motor_a.dc(40)
    # 否則，如果形狀鍵＝2（●）
    elif read_shape() == 2:
        # 按 ●：夾爪往放開的方向轉
        motor_a.dc(-40)
    else:
        # 都沒按：夾爪停住
        motor_a.brake()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 05：✕ 夾、● 放：控制夾爪
' 軟體：clev3r
' 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：用形狀鍵控制接在 A 埠的中馬達（夾爪）：按 ✕ 夾緊、按 ● 放開，都不按就停。
' 讀值：shape＝形狀鍵（▲1、●2、✕4、■8）（0 ~ 15）
' ============================================================
folder "prjs" "MBC_ps4_ex05"

' 掌控者 PS4插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 一直重複
While "True"
  ' 如果形狀鍵＝4（✕）
  If Sensor.ReadRawValue(1, 2) = 4 Then
    ' 按 ✕：夾爪往夾緊的方向轉
    Motor.StartPower("A", 40)
  ' 否則，如果形狀鍵＝2（●）
  ElseIf Sensor.ReadRawValue(1, 2) = 2 Then
    ' 按 ●：夾爪往放開的方向轉
    Motor.StartPower("A", -40)
  Else
    ' 都沒按：夾爪停住
    Motor.Stop("A", "True")
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 如果形狀鍵＝4（✕）
      - 按 ✕：夾爪往夾緊的方向轉
      - 否則：
         - 否則，如果形狀鍵＝2（●）
            - 按 ●：夾爪往放開的方向轉
            - 否則：
               - 都沒按：夾爪停住

**跑起來應該看到：** 按住 ✕ 夾爪夾起來、按住 ● 打開，放開按鍵就停。

**常見問題：**

- **夾緊和放開顛倒？** 把 40 和 -40 對調。
- **按鍵沒反應？** 先跑範例 1 看數字有沒有變。不會變就是手把沒連上：重新開手把電源，或 EV3 先開機再接接收器。
- **搖桿放開了車子還在慢慢動？** 搖桿中間不一定剛好 50，把程式裡的 × 2 改小，或加一段「數字在 45~55 之間就當成 50」。

---

## 範例 6：手把連上才出發

> **僅 Pybricks v4／EV3 MicroPython v2.0／clev3r**（EV3-G、Classroom 的積木讀不到這個模式）

**這個範例會做什麼：** 手把還沒連上時搖桿是中間、按鍵是 0，程式分不出「沒按」和「沒連上」。模式 3 的數字加了「已連線」：大於等於 256 就是手把連上了。這個範例等手把連上才嗶一聲出發，之後用方向鍵上（256＋1）前進、下（256＋4）後退，手把斷線就馬上停車。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 掌控者 PS4藍牙接收器（EV3 版，韌體 v4.0 以上）＋手把

**怎麼接：** 藍牙接收器插 1 號孔。先不要開手把，程式跑起來再開，就能看到「等待 → 連上」的過程。

**下載程式（選你用的軟體）：**

[⬇ Pybricks v4](/examples/ps4/ev3_beginner/ex06_wait_connected_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/ps4/ev3_beginner/ex06_wait_connected_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/ps4/ev3_beginner/ex06_wait_connected.bp)

（程式碼即範例，不需截圖。）

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 06：手把連上才出發
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：手把還沒連上時搖桿是中間、按鍵是 0，程式分不出「沒按」和「沒連上」。模式 3 的數字加了「已連線」：大於等於 256 就是手把連上了。這個範例等手把連上才嗶一聲出發，之後用方向鍵上（256＋1）前進、下（256＋4）後退，手把斷線就馬上停車。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 掌控者 PS4插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_status3():
    # 方向鍵＋形狀鍵＋連線狀態：0 ~ 511：方向鍵（0~15）＋ 形狀鍵 × 16 ＋ 已連線 × 256，大於等於 256＝手把已連上
    return sensor.read(3)[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 螢幕顯示 WAIT PAD：等手把
ev3.screen.draw_text(0, 0, "WAIT PAD      ", Color.BLACK, Color.WHITE)
# 等到狀態值大於等於 256（手把已連上）
while not (read_status3() >= 256):
    wait(10)
# 連上了：嗶一聲
ev3.speaker.beep()
# 一直重複
while True:
    # 第 2 行顯示狀態值
    ev3.screen.draw_text(0, 16, "STATUS " + str(read_status3()) + "      ", Color.BLACK, Color.WHITE)
    # 如果狀態值小於 256（手把斷線了）
    if read_status3() < 256:
        # 手把斷線：馬上停車
        left_motor.brake()
        right_motor.brake()
        # 螢幕顯示 PAD LOST
        ev3.screen.draw_text(0, 0, "PAD LOST      ", Color.BLACK, Color.WHITE)
    else:
        # 螢幕顯示 PAD OK
        ev3.screen.draw_text(0, 0, "PAD OK      ", Color.BLACK, Color.WHITE)
        # 如果狀態值＝257（已連上＋方向鍵上）
        if read_status3() == 257:
            # 已連上＋方向鍵上（256＋1）：前進
            left_motor.dc(40)
            right_motor.dc(40)
        # 否則，如果狀態值＝260（已連上＋方向鍵下）
        elif read_status3() == 260:
            # 已連上＋方向鍵下（256＋4）：後退
            left_motor.dc(-40)
            right_motor.dc(-40)
        else:
            # 連上但沒按：停車
            left_motor.brake()
            right_motor.brake()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 06：手把連上才出發
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：手把還沒連上時搖桿是中間、按鍵是 0，程式分不出「沒按」和「沒連上」。模式 3 的數字加了「已連線」：大於等於 256 就是手把連上了。這個範例等手把連上才嗶一聲出發，之後用方向鍵上（256＋1）前進、下（256＋4）後退，手把斷線就馬上停車。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS4插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_status3():
    # 方向鍵＋形狀鍵＋連線狀態：0 ~ 511：方向鍵（0~15）＋ 形狀鍵 × 16 ＋ 已連線 × 256，大於等於 256＝手把已連上
    return sensor.read('IR-REM-A')[0]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 螢幕顯示 WAIT PAD：等手把
ev3.screen.draw_text(0, 0, "WAIT PAD      ", Color.BLACK, Color.WHITE)
# 等到狀態值大於等於 256（手把已連上）
while not (read_status3() >= 256):
    wait(10)
# 連上了：嗶一聲
ev3.speaker.beep()
# 一直重複
while True:
    # 第 2 行顯示狀態值
    ev3.screen.draw_text(0, 16, "STATUS " + str(read_status3()) + "      ", Color.BLACK, Color.WHITE)
    # 如果狀態值小於 256（手把斷線了）
    if read_status3() < 256:
        # 手把斷線：馬上停車
        left_motor.brake()
        right_motor.brake()
        # 螢幕顯示 PAD LOST
        ev3.screen.draw_text(0, 0, "PAD LOST      ", Color.BLACK, Color.WHITE)
    else:
        # 螢幕顯示 PAD OK
        ev3.screen.draw_text(0, 0, "PAD OK      ", Color.BLACK, Color.WHITE)
        # 如果狀態值＝257（已連上＋方向鍵上）
        if read_status3() == 257:
            # 已連上＋方向鍵上（256＋1）：前進
            left_motor.dc(40)
            right_motor.dc(40)
        # 否則，如果狀態值＝260（已連上＋方向鍵下）
        elif read_status3() == 260:
            # 已連上＋方向鍵下（256＋4）：後退
            left_motor.dc(-40)
            right_motor.dc(-40)
        else:
            # 連上但沒按：停車
            left_motor.brake()
            right_motor.brake()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 掌控者 PS4/PS5 藍牙遙控接收器 EV3 新手範例 06：手把連上才出發
' 軟體：clev3r
' 接法：掌控者 PS4插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：手把還沒連上時搖桿是中間、按鍵是 0，程式分不出「沒按」和「沒連上」。模式 3 的數字加了「已連線」：大於等於 256 就是手把連上了。這個範例等手把連上才嗶一聲出發，之後用方向鍵上（256＋1）前進、下（256＋4）後退，手把斷線就馬上停車。
' 讀值：status3＝方向鍵＋形狀鍵＋連線狀態（0 ~ 511：方向鍵（0~15）＋ 形狀鍵 × 16 ＋ 已連線 × 256，大於等於 256＝手把已連上）
' ============================================================
folder "prjs" "MBC_ps4_ex06"

' 掌控者 PS4插 1 號孔，使用模式 3
Sensor.SetMode(1, 3)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 螢幕顯示 WAIT PAD：等手把
LCD.Text(1, 0, 0, 1, "WAIT PAD      ")
' 等到狀態值大於等於 256（手把已連上）
While Sensor.ReadRawValue(1, 0) < 256
  Program.Delay(10)
EndWhile
' 連上了：嗶一聲
Speaker.Tone(50, 1000, 100)
Speaker.Wait()
' 一直重複
While "True"
  ' 第 2 行顯示狀態值
  LCD.Text(1, 0, 16, 1, "STATUS " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 如果狀態值小於 256（手把斷線了）
  If Sensor.ReadRawValue(1, 0) < 256 Then
    ' 手把斷線：馬上停車
    Motor.Stop("BC", "True")
    ' 螢幕顯示 PAD LOST
    LCD.Text(1, 0, 0, 1, "PAD LOST      ")
  Else
    ' 螢幕顯示 PAD OK
    LCD.Text(1, 0, 0, 1, "PAD OK      ")
    ' 如果狀態值＝257（已連上＋方向鍵上）
    If Sensor.ReadRawValue(1, 0) = 257 Then
      ' 已連上＋方向鍵上（256＋1）：前進
      Motor.StartPower("B", 40)
      Motor.StartPower("C", 40)
    ' 否則，如果狀態值＝260（已連上＋方向鍵下）
    ElseIf Sensor.ReadRawValue(1, 0) = 260 Then
      ' 已連上＋方向鍵下（256＋4）：後退
      Motor.StartPower("B", -40)
      Motor.StartPower("C", -40)
    Else
      ' 連上但沒按：停車
      Motor.Stop("BC", "True")
    EndIf
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 螢幕顯示 WAIT PAD：等手把
1. 等到狀態值大於等於 256（手把已連上）
1. 連上了：嗶一聲
1. 一直重複
   - 第 2 行顯示狀態值
   - 如果狀態值小於 256（手把斷線了）
      - 手把斷線：馬上停車
      - 螢幕顯示 PAD LOST
      - 否則：
         - 螢幕顯示 PAD OK
         - 如果狀態值＝257（已連上＋方向鍵上）
            - 已連上＋方向鍵上（256＋1）：前進
            - 否則：
               - 否則，如果狀態值＝260（已連上＋方向鍵下）
                  - 已連上＋方向鍵下（256＋4）：後退
                  - 否則：
                     - 連上但沒按：停車

**跑起來應該看到：** 程式跑起來先顯示 WAIT PAD，數字小於 256；打開手把，EV3 嗶一聲、顯示 PAD OK、數字變成 256。按住方向鍵上車子前進（數字 257）、按下後退（260）；關掉手把，車子馬上停並顯示 PAD LOST。

**常見問題：**

- **按上的時候數字不是 257？** 同時按到形狀鍵會再加（形狀鍵 × 16），例如上＋✕＝256＋1＋64＝321。先放開其他鍵。
- **按鍵沒反應？** 先跑範例 1 看數字有沒有變。不會變就是手把沒連上：重新開手把電源，或 EV3 先開機再接接收器。
- **搖桿放開了車子還在慢慢動？** 搖桿中間不一定剛好 50，把程式裡的 × 2 改小，或加一段「數字在 45~55 之間就當成 50」。
- **數字一直是 0 或不會動？** 感應器韌體要 v4.0（2026-10）以上才有這個模式，舊韌體請先更新。
- **EV3-G／Classroom 怎麼沒有這個範例？** 這個範例用的是新模式（2026-10 韌體 v4 起），EV3-G 和 Classroom 的紅外線積木讀不到，只有 Pybricks、EV3 MicroPython、clev3r 能用。

---

還有問題？歡迎到 [聯絡我們](/contact.md) 留言，附上你用的軟體和程式截圖，我們會幫你看。
