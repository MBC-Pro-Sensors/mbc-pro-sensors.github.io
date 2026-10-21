<!-- 這頁由 95_產品網站\EV3新手範例產生器 自動產生，請改產生器再重產，不要直接改這個檔。 -->
# 🧱 EV3 新手範例：神攝手（學校版）

[🔙 回到 神攝手（學校版）產品頁](/sensors/esp32cam/index.md)

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

## 這頁會用到的 9 個數值

| 名稱 | 範圍 | 從哪裡讀 |
|---|---|---|
| **顏色（色相）** | 0 ~ 100，-1＝黑白灰 | 模式 1 第 1 個值 |
| **顏色鮮豔度** | 0 ~ 100 | 模式 1 第 2 個值 |
| **亮度** | 0 ~ 100 | 模式 1 第 3 個值 |
| **畫面動了多少** | 0 ~ 100 | 模式 1 第 4 個值 |
| **色塊左右位置** | 0 ~ 100（左 → 右），沒找到＝0 | 模式 1 第 5 個值 |
| **色塊上下位置** | 0 ~ 75（下 → 上），沒找到＝0 | 模式 1 第 6 個值 |
| **色塊寬度** | 0 ~ 100，0＝沒找到 | 模式 1 第 7 個值 |
| **色塊高度（越近越大）** | 0 ~ 75，0＝沒找到 | 模式 1 第 8 個值 |
| **顏色（色相）** | 0 ~ 100（紅 0 或 100、黃約 17、綠約 33、藍約 67），-1＝黑白灰 | 模式 0 |

**範例列表：**

1. [相機看到了什麼？8 個數字一次看](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E7%9B%B8%E6%A9%9F%E7%9C%8B%E5%88%B0%E4%BA%86%E4%BB%80%E9%BA%BC%EF%BC%9F8-%E5%80%8B%E6%95%B8%E5%AD%97%E4%B8%80%E6%AC%A1%E7%9C%8B)
2. [紅綠燈車：看到綠色才走](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E7%B4%85%E7%B6%A0%E7%87%88%E8%BB%8A%EF%BC%9A%E7%9C%8B%E5%88%B0%E7%B6%A0%E8%89%B2%E6%89%8D%E8%B5%B0)
3. [防盜警報：有東西動就叫](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E9%98%B2%E7%9B%9C%E8%AD%A6%E5%A0%B1%EF%BC%9A%E6%9C%89%E6%9D%B1%E8%A5%BF%E5%8B%95%E5%B0%B1%E5%8F%AB)
4. [看球轉頭](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E7%9C%8B%E7%90%83%E8%BD%89%E9%A0%AD)
5. [追球：靠近到一定距離就停](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E8%BF%BD%E7%90%83%EF%BC%9A%E9%9D%A0%E8%BF%91%E5%88%B0%E4%B8%80%E5%AE%9A%E8%B7%9D%E9%9B%A2%E5%B0%B1%E5%81%9C)

---

## 範例 1：相機看到了什麼？8 個數字一次看

**這個範例會做什麼：** 模式 1 一次送出 8 個數字：顏色、鮮豔度、亮度、動態、色塊的 X、Y、寬、高。全部顯示在 EV3 螢幕上，對照相機畫面就能看懂每個數字的意思。

**要準備什麼：**

- 神攝手（學校版）（EV3 版）
- EV3 主機

**怎麼接：** 神攝手（學校版）插 1 號孔。相機設定：側鍵短按進選單 → 左右鍵選「2:TRACK」→ 確認 → 把十字對準球，依序調好位置、容許範圍與最小面積 → 按確認存檔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner/ex01_show_all.ev3) ｜ [⬇ EV3 Classroom](/examples/esp32cam/ev3_beginner/ex01_show_all.lmsp) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner/ex01_show_all_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner/ex01_show_all_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner/ex01_show_all.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex01_show_all-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex01_show_all-ev3g.webp" alt="神攝手（學校版） 相機看到了什麼？8 個數字一次看（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex01_show_all-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex01_show_all-classroom.webp" alt="神攝手（學校版） 相機看到了什麼？8 個數字一次看（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 01：相機看到了什麼？8 個數字一次看
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（學校版）插 1 號孔
# 這個範例會做什麼：模式 1 一次送出 8 個數字：顏色、鮮豔度、亮度、動態、色塊的 X、Y、寬、高。全部顯示在 EV3 螢幕上，對照相機畫面就能看懂每個數字的意思。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（學校版）插 1 號孔


def read_hue1():
    # 顏色（色相）：0 ~ 100，-1＝黑白灰
    return sensor.read(1)[0]


def read_sta():
    # 顏色鮮豔度：0 ~ 100
    return sensor.read(1)[1]


def read_val():
    # 亮度：0 ~ 100
    return sensor.read(1)[2]


def read_motion():
    # 畫面動了多少：0 ~ 100
    return sensor.read(1)[3]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read(1)[4]


def read_y():
    # 色塊上下位置：0 ~ 75（下 → 上），沒找到＝0
    return sensor.read(1)[5]


def read_w():
    # 色塊寬度：0 ~ 100，0＝沒找到
    return sensor.read(1)[6]


def read_h():
    # 色塊高度（越近越大）：0 ~ 75，0＝沒找到
    return sensor.read(1)[7]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 顏色：紅 0 或 100、黃約 17、綠約 33、藍約 67，-1＝黑白灰
    ev3.screen.draw_text(0, 0, "HUE " + str(read_hue1()) + "      ", Color.BLACK, Color.WHITE)
    # 鮮豔度 0~100
    ev3.screen.draw_text(0, 16, "STA " + str(read_sta()) + "      ", Color.BLACK, Color.WHITE)
    # 亮度 0~100
    ev3.screen.draw_text(0, 32, "VAL " + str(read_val()) + "      ", Color.BLACK, Color.WHITE)
    # 畫面動了多少 0~100
    ev3.screen.draw_text(0, 48, "MOTION " + str(read_motion()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊左右位置 0~100
    ev3.screen.draw_text(0, 64, "X " + str(read_x()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊上下位置 0~75
    ev3.screen.draw_text(0, 80, "Y " + str(read_y()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊寬度，0＝沒找到
    ev3.screen.draw_text(0, 96, "W " + str(read_w()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊高度，越近越大
    ev3.screen.draw_text(0, 112, "H " + str(read_h()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 01：相機看到了什麼？8 個數字一次看
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（學校版）插 1 號孔
# 這個範例會做什麼：模式 1 一次送出 8 個數字：顏色、鮮豔度、亮度、動態、色塊的 X、Y、寬、高。全部顯示在 EV3 螢幕上，對照相機畫面就能看懂每個數字的意思。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（學校版）插 1 號孔


def read_hue1():
    # 顏色（色相）：0 ~ 100，-1＝黑白灰
    return sensor.read('IR-SEEK')[0]


def read_sta():
    # 顏色鮮豔度：0 ~ 100
    return sensor.read('IR-SEEK')[1]


def read_val():
    # 亮度：0 ~ 100
    return sensor.read('IR-SEEK')[2]


def read_motion():
    # 畫面動了多少：0 ~ 100
    return sensor.read('IR-SEEK')[3]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read('IR-SEEK')[4]


def read_y():
    # 色塊上下位置：0 ~ 75（下 → 上），沒找到＝0
    return sensor.read('IR-SEEK')[5]


def read_w():
    # 色塊寬度：0 ~ 100，0＝沒找到
    return sensor.read('IR-SEEK')[6]


def read_h():
    # 色塊高度（越近越大）：0 ~ 75，0＝沒找到
    return sensor.read('IR-SEEK')[7]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 顏色：紅 0 或 100、黃約 17、綠約 33、藍約 67，-1＝黑白灰
    ev3.screen.draw_text(0, 0, "HUE " + str(read_hue1()) + "      ", Color.BLACK, Color.WHITE)
    # 鮮豔度 0~100
    ev3.screen.draw_text(0, 16, "STA " + str(read_sta()) + "      ", Color.BLACK, Color.WHITE)
    # 亮度 0~100
    ev3.screen.draw_text(0, 32, "VAL " + str(read_val()) + "      ", Color.BLACK, Color.WHITE)
    # 畫面動了多少 0~100
    ev3.screen.draw_text(0, 48, "MOTION " + str(read_motion()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊左右位置 0~100
    ev3.screen.draw_text(0, 64, "X " + str(read_x()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊上下位置 0~75
    ev3.screen.draw_text(0, 80, "Y " + str(read_y()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊寬度，0＝沒找到
    ev3.screen.draw_text(0, 96, "W " + str(read_w()) + "      ", Color.BLACK, Color.WHITE)
    # 色塊高度，越近越大
    ev3.screen.draw_text(0, 112, "H " + str(read_h()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 01：相機看到了什麼？8 個數字一次看
' 軟體：clev3r
' 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 1 一次送出 8 個數字：顏色、鮮豔度、亮度、動態、色塊的 X、Y、寬、高。全部顯示在 EV3 螢幕上，對照相機畫面就能看懂每個數字的意思。
' 讀值：hue1＝顏色（色相）（0 ~ 100，-1＝黑白灰）；sta＝顏色鮮豔度（0 ~ 100）；val＝亮度（0 ~ 100）；motion＝畫面動了多少（0 ~ 100）；x＝色塊左右位置（0 ~ 100（左 → 右），沒找到＝0）；y＝色塊上下位置（0 ~ 75（下 → 上），沒找到＝0）；w＝色塊寬度（0 ~ 100，0＝沒找到）；h＝色塊高度（越近越大）（0 ~ 75，0＝沒找到）
' ============================================================
folder "prjs" "MBC_esp32cam_ex01"

' 神攝手（學校版）插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 顏色：紅 0 或 100、黃約 17、綠約 33、藍約 67，-1＝黑白灰
  LCD.Text(1, 0, 0, 1, "HUE " + Sensor.ReadRawValue(1, 0) + "      ")
  ' 鮮豔度 0~100
  LCD.Text(1, 0, 16, 1, "STA " + Sensor.ReadRawValue(1, 1) + "      ")
  ' 亮度 0~100
  LCD.Text(1, 0, 32, 1, "VAL " + Sensor.ReadRawValue(1, 2) + "      ")
  ' 畫面動了多少 0~100
  LCD.Text(1, 0, 48, 1, "MOTION " + Sensor.ReadRawValue(1, 3) + "      ")
  ' 色塊左右位置 0~100
  LCD.Text(1, 0, 64, 1, "X " + Sensor.ReadRawValue(1, 4) + "      ")
  ' 色塊上下位置 0~75
  LCD.Text(1, 0, 80, 1, "Y " + Sensor.ReadRawValue(1, 5) + "      ")
  ' 色塊寬度，0＝沒找到
  LCD.Text(1, 0, 96, 1, "W " + Sensor.ReadRawValue(1, 6) + "      ")
  ' 色塊高度，越近越大
  LCD.Text(1, 0, 112, 1, "H " + Sensor.ReadRawValue(1, 7) + "      ")
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 一直重複
   - 顏色：紅 0 或 100、黃約 17、綠約 33、藍約 67，-1＝黑白灰
   - 鮮豔度 0~100
   - 亮度 0~100
   - 畫面動了多少 0~100
   - 色塊左右位置 0~100
   - 色塊上下位置 0~75
   - 色塊寬度，0＝沒找到
   - 色塊高度，越近越大
   - 等 0.1 秒

**跑起來應該看到：** 把球在鏡頭前左右移動，X 跟著變；拿近，W、H 變大；拿走，X、Y、W、H 全部變 0。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **X、Y、W、H 一直是 0？** 還沒設定 2:TRACK，或球的顏色不在容許範圍內。

---

## 範例 2：紅綠燈車：看到綠色才走

**這個範例會做什麼：** 用模式 0 只讀「顏色」。看到綠色就往前開，其他顏色（紅色）就停。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 神攝手（學校版）（EV3 版）
- 紅色、綠色的紙卡各一張

**怎麼接：** 神攝手（學校版）插 1 號孔，裝在車頭、鏡頭朝前。相機設定：側鍵短按進選單 → 左右鍵選「1:COLOR」→ 確認 → 把十字對準要認的顏色，依序調好位置與容許範圍 → 按確認存檔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner/ex02_traffic_light.ev3) ｜ [⬇ EV3 Classroom](/examples/esp32cam/ev3_beginner/ex02_traffic_light.lmsp) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner/ex02_traffic_light_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner/ex02_traffic_light_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner/ex02_traffic_light.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex02_traffic_light-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex02_traffic_light-ev3g.webp" alt="神攝手（學校版） 紅綠燈車：看到綠色才走（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex02_traffic_light-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex02_traffic_light-classroom.webp" alt="神攝手（學校版） 紅綠燈車：看到綠色才走（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 02：紅綠燈車：看到綠色才走
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用模式 0 只讀「顏色」。看到綠色就往前開，其他顏色（紅色）就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（學校版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_hue():
    # 顏色（色相）：0 ~ 100（紅 0 或 100、黃約 17、綠約 33、藍約 67），-1＝黑白灰
    return sensor.read(0)[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果顏色在 25~45 之間（綠色）
    if read_hue() >= 25 and read_hue() <= 45:
        # 是綠色：往前開
        left_motor.dc(30)
        right_motor.dc(30)
    else:
        # 不是綠色：停車
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 02：紅綠燈車：看到綠色才走
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用模式 0 只讀「顏色」。看到綠色就往前開，其他顏色（紅色）就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（學校版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_hue():
    # 顏色（色相）：0 ~ 100（紅 0 或 100、黃約 17、綠約 33、藍約 67），-1＝黑白灰
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果顏色在 25~45 之間（綠色）
    if read_hue() >= 25 and read_hue() <= 45:
        # 是綠色：往前開
        left_motor.dc(30)
        right_motor.dc(30)
    else:
        # 不是綠色：停車
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 02：紅綠燈車：看到綠色才走
' 軟體：clev3r
' 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：用模式 0 只讀「顏色」。看到綠色就往前開，其他顏色（紅色）就停。
' 讀值：hue＝顏色（色相）（0 ~ 100（紅 0 或 100、黃約 17、綠約 33、藍約 67），-1＝黑白灰）
' ============================================================
folder "prjs" "MBC_esp32cam_ex02"

' 神攝手（學校版）插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複
While "True"
  ' 如果顏色在 25~45 之間（綠色）
  If (Sensor.ReadRawValue(1, 0) >= 25) And (Sensor.ReadRawValue(1, 0) <= 45) Then
    ' 是綠色：往前開
    Motor.StartPower("B", 30)
    Motor.StartPower("C", 30)
  Else
    ' 不是綠色：停車
    Motor.Stop("BC", "True")
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 如果顏色在 25~45 之間（綠色）
      - 是綠色：往前開
      - 否則：
         - 不是綠色：停車

**跑起來應該看到：** 鏡頭前放綠色卡，車子往前開；換成紅色卡，車子停下來。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **綠色卡也不走？** 先跑範例 1 看綠色卡實際的 HUE 是多少，再改程式裡的 25 和 45。

---

## 範例 3：防盜警報：有東西動就叫

**這個範例會做什麼：** 模式 1 的第 4 個數字是「畫面動了多少」。相機對著門口，有人經過數字就會變大，超過 20 就嗶嗶叫。這個範例相機不用做任何設定。

**要準備什麼：**

- 神攝手（學校版）（EV3 版）
- EV3 主機

**怎麼接：** 神攝手（學校版）插 1 號孔，鏡頭對著要看守的地方，放穩不要晃。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner/ex03_alarm.ev3) ｜ [⬇ EV3 Classroom](/examples/esp32cam/ev3_beginner/ex03_alarm.lmsp) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner/ex03_alarm_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner/ex03_alarm_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner/ex03_alarm.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex03_alarm-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex03_alarm-ev3g.webp" alt="神攝手（學校版） 防盜警報：有東西動就叫（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex03_alarm-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex03_alarm-classroom.webp" alt="神攝手（學校版） 防盜警報：有東西動就叫（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 03：防盜警報：有東西動就叫
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（學校版）插 1 號孔
# 這個範例會做什麼：模式 1 的第 4 個數字是「畫面動了多少」。相機對著門口，有人經過數字就會變大，超過 20 就嗶嗶叫。這個範例相機不用做任何設定。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（學校版）插 1 號孔


def read_motion():
    # 畫面動了多少：0 ~ 100
    return sensor.read(1)[3]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 螢幕顯示動態值
    ev3.screen.draw_text(0, 0, "MOTION " + str(read_motion()) + "      ", Color.BLACK, Color.WHITE)
    # 如果動態值大於 20
    if read_motion() > 20:
        # 有東西在動：嗶！
        ev3.speaker.beep()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 03：防盜警報：有東西動就叫
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（學校版）插 1 號孔
# 這個範例會做什麼：模式 1 的第 4 個數字是「畫面動了多少」。相機對著門口，有人經過數字就會變大，超過 20 就嗶嗶叫。這個範例相機不用做任何設定。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（學校版）插 1 號孔


def read_motion():
    # 畫面動了多少：0 ~ 100
    return sensor.read('IR-SEEK')[3]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 螢幕顯示動態值
    ev3.screen.draw_text(0, 0, "MOTION " + str(read_motion()) + "      ", Color.BLACK, Color.WHITE)
    # 如果動態值大於 20
    if read_motion() > 20:
        # 有東西在動：嗶！
        ev3.speaker.beep()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 03：防盜警報：有東西動就叫
' 軟體：clev3r
' 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：模式 1 的第 4 個數字是「畫面動了多少」。相機對著門口，有人經過數字就會變大，超過 20 就嗶嗶叫。這個範例相機不用做任何設定。
' 讀值：motion＝畫面動了多少（0 ~ 100）
' ============================================================
folder "prjs" "MBC_esp32cam_ex03"

' 神攝手（學校版）插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 一直重複
While "True"
  ' 螢幕顯示動態值
  LCD.Text(1, 0, 0, 1, "MOTION " + Sensor.ReadRawValue(1, 3) + "      ")
  ' 如果動態值大於 20
  If Sensor.ReadRawValue(1, 3) > 20 Then
    ' 有東西在動：嗶！
    Speaker.Tone(50, 1000, 100)
    Speaker.Wait()
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 螢幕顯示動態值
   - 如果動態值大於 20
      - 有東西在動：嗶！

**跑起來應該看到：** 畫面靜止時不叫；手在鏡頭前揮一下，EV3 就嗶嗶叫。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **沒人也一直叫？** 日光燈閃爍或相機在晃。把 20 改大（例如 35）。

---

## 範例 4：看球轉頭

**這個範例會做什麼：** 車子原地轉，讓球一直保持在畫面中間：球在左邊就往左轉、在右邊就往右轉、在中間就停；看不到球就原地轉圈找。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 神攝手（學校版）（EV3 版）
- 一顆顏色鮮豔的球

**怎麼接：** 神攝手（學校版）插 1 號孔，裝在車頭、鏡頭朝前。相機設定：側鍵短按進選單 → 左右鍵選「2:TRACK」→ 確認 → 把十字對準球，依序調好位置、容許範圍與最小面積 → 按確認存檔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner/ex04_look_at_ball.ev3) ｜ [⬇ EV3 Classroom](/examples/esp32cam/ev3_beginner/ex04_look_at_ball.lmsp) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner/ex04_look_at_ball_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner/ex04_look_at_ball_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner/ex04_look_at_ball.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex04_look_at_ball-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex04_look_at_ball-ev3g.webp" alt="神攝手（學校版） 看球轉頭（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex04_look_at_ball-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex04_look_at_ball-classroom.webp" alt="神攝手（學校版） 看球轉頭（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 04：看球轉頭
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子原地轉，讓球一直保持在畫面中間：球在左邊就往左轉、在右邊就往右轉、在中間就停；看不到球就原地轉圈找。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（學校版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_w():
    # 色塊寬度：0 ~ 100，0＝沒找到
    return sensor.read(1)[6]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read(1)[4]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果色塊寬度＝0（沒找到球）
    if read_w() == 0:
        # 看不到球（寬度＝0）：原地慢慢轉圈找
        left_motor.dc(15)
        right_motor.dc(-15)
    # 如果 X 小於 40（左邊）
    elif read_x() < 40:
        # 球在左邊：往左轉
        left_motor.dc(-15)
        right_motor.dc(15)
    # 否則，如果 X 大於 60（右邊）
    elif read_x() > 60:
        # 球在右邊：往右轉
        left_motor.dc(15)
        right_motor.dc(-15)
    else:
        # 球在中間：停住
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 04：看球轉頭
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子原地轉，讓球一直保持在畫面中間：球在左邊就往左轉、在右邊就往右轉、在中間就停；看不到球就原地轉圈找。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（學校版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_w():
    # 色塊寬度：0 ~ 100，0＝沒找到
    return sensor.read('IR-SEEK')[6]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read('IR-SEEK')[4]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果色塊寬度＝0（沒找到球）
    if read_w() == 0:
        # 看不到球（寬度＝0）：原地慢慢轉圈找
        left_motor.dc(15)
        right_motor.dc(-15)
    # 如果 X 小於 40（左邊）
    elif read_x() < 40:
        # 球在左邊：往左轉
        left_motor.dc(-15)
        right_motor.dc(15)
    # 否則，如果 X 大於 60（右邊）
    elif read_x() > 60:
        # 球在右邊：往右轉
        left_motor.dc(15)
        right_motor.dc(-15)
    else:
        # 球在中間：停住
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 04：看球轉頭
' 軟體：clev3r
' 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：車子原地轉，讓球一直保持在畫面中間：球在左邊就往左轉、在右邊就往右轉、在中間就停；看不到球就原地轉圈找。
' 讀值：w＝色塊寬度（0 ~ 100，0＝沒找到）；x＝色塊左右位置（0 ~ 100（左 → 右），沒找到＝0）
' ============================================================
folder "prjs" "MBC_esp32cam_ex04"

' 神攝手（學校版）插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 一直重複
While "True"
  ' 如果色塊寬度＝0（沒找到球）
  If Sensor.ReadRawValue(1, 6) = 0 Then
    ' 看不到球（寬度＝0）：原地慢慢轉圈找
    Motor.StartPower("B", 15)
    Motor.StartPower("C", -15)
  ' 如果 X 小於 40（左邊）
  ElseIf Sensor.ReadRawValue(1, 4) < 40 Then
    ' 球在左邊：往左轉
    Motor.StartPower("B", -15)
    Motor.StartPower("C", 15)
  ' 否則，如果 X 大於 60（右邊）
  ElseIf Sensor.ReadRawValue(1, 4) > 60 Then
    ' 球在右邊：往右轉
    Motor.StartPower("B", 15)
    Motor.StartPower("C", -15)
  Else
    ' 球在中間：停住
    Motor.Stop("BC", "True")
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 如果色塊寬度＝0（沒找到球）
      - 看不到球（寬度＝0）：原地慢慢轉圈找
      - 否則：
         - 如果 X 小於 40（左邊）
            - 球在左邊：往左轉
            - 否則：
               - 否則，如果 X 大於 60（右邊）
                  - 球在右邊：往右轉
                  - 否則：
                     - 球在中間：停住

**跑起來應該看到：** 拿著球在車子前面左右移動，車頭會跟著球轉。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **左右顛倒？** 把兩個 tank 的 -15 和 15 對調。

---

## 範例 5：追球：靠近到一定距離就停

**這個範例會做什麼：** 把範例 4 改成會往前開：一邊對準球一邊前進，球的高度 H 夠大（夠近）就停下來。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 神攝手（學校版）（EV3 版）
- 一顆顏色鮮豔的球

**怎麼接：** 神攝手（學校版）插 1 號孔，裝在車頭、鏡頭朝前。相機設定：側鍵短按進選單 → 左右鍵選「2:TRACK」→ 確認 → 把十字對準球，依序調好位置、容許範圍與最小面積 → 按確認存檔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner/ex05_chase_ball.ev3) ｜ [⬇ EV3 Classroom](/examples/esp32cam/ev3_beginner/ex05_chase_ball.lmsp) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner/ex05_chase_ball_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner/ex05_chase_ball_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner/ex05_chase_ball.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex05_chase_ball-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex05_chase_ball-ev3g.webp" alt="神攝手（學校版） 追球：靠近到一定距離就停（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>EV3 Classroom 積木圖（Classroom 沒有繁體中文介面，圖是英文版）</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/ex05_chase_ball-classroom.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/ex05_chase_ball-classroom.webp" alt="神攝手（學校版） 追球：靠近到一定距離就停（EV3 Classroom）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 05：追球：靠近到一定距離就停
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：把範例 4 改成會往前開：一邊對準球一邊前進，球的高度 H 夠大（夠近）就停下來。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（學校版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_w():
    # 色塊寬度：0 ~ 100，0＝沒找到
    return sensor.read(1)[6]


def read_h():
    # 色塊高度（越近越大）：0 ~ 75，0＝沒找到
    return sensor.read(1)[7]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read(1)[4]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果寬度＝0（沒找到球）
    if read_w() == 0:
        # 看不到球：原地轉圈找
        left_motor.dc(15)
        right_motor.dc(-15)
    # 否則，如果 H 大於 30
    elif read_h() > 30:
        # 球的高度大於 30（夠近了）：停車
        left_motor.brake()
        right_motor.brake()
    else:
        # 轉彎力量 turn ＝（X − 50）× 0.6：球偏右是正數
        turn = (read_x() - 50) * 0.6
        # 左輪＝25＋turn、右輪＝25－turn：邊對準邊前進
        left_motor.dc(25 + turn)
        right_motor.dc(25 - turn)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 05：追球：靠近到一定距離就停
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：把範例 4 改成會往前開：一邊對準球一邊前進，球的高度 H 夠大（夠近）就停下來。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（學校版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_w():
    # 色塊寬度：0 ~ 100，0＝沒找到
    return sensor.read('IR-SEEK')[6]


def read_h():
    # 色塊高度（越近越大）：0 ~ 75，0＝沒找到
    return sensor.read('IR-SEEK')[7]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read('IR-SEEK')[4]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果寬度＝0（沒找到球）
    if read_w() == 0:
        # 看不到球：原地轉圈找
        left_motor.dc(15)
        right_motor.dc(-15)
    # 否則，如果 H 大於 30
    elif read_h() > 30:
        # 球的高度大於 30（夠近了）：停車
        left_motor.brake()
        right_motor.brake()
    else:
        # 轉彎力量 turn ＝（X − 50）× 0.6：球偏右是正數
        turn = (read_x() - 50) * 0.6
        # 左輪＝25＋turn、右輪＝25－turn：邊對準邊前進
        left_motor.dc(25 + turn)
        right_motor.dc(25 - turn)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 05：追球：靠近到一定距離就停
' 軟體：clev3r
' 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：把範例 4 改成會往前開：一邊對準球一邊前進，球的高度 H 夠大（夠近）就停下來。
' 讀值：w＝色塊寬度（0 ~ 100，0＝沒找到）；h＝色塊高度（越近越大）（0 ~ 75，0＝沒找到）；x＝色塊左右位置（0 ~ 100（左 → 右），沒找到＝0）
' ============================================================
folder "prjs" "MBC_esp32cam_ex05"

' 神攝手（學校版）插 1 號孔，使用模式 1
Sensor.SetMode(1, 1)
LCD.Clear()

' 一直重複
While "True"
  ' 如果寬度＝0（沒找到球）
  If Sensor.ReadRawValue(1, 6) = 0 Then
    ' 看不到球：原地轉圈找
    Motor.StartPower("B", 15)
    Motor.StartPower("C", -15)
  ' 否則，如果 H 大於 30
  ElseIf Sensor.ReadRawValue(1, 7) > 30 Then
    ' 球的高度大於 30（夠近了）：停車
    Motor.Stop("BC", "True")
  Else
    ' 轉彎力量 turn ＝（X − 50）× 0.6：球偏右是正數
    turn = (Sensor.ReadRawValue(1, 4) - 50) * 0.6
    ' 左輪＝25＋turn、右輪＝25－turn：邊對準邊前進
    Motor.StartPower("B", 25 + turn)
    Motor.StartPower("C", 25 - turn)
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 如果寬度＝0（沒找到球）
      - 看不到球：原地轉圈找
      - 否則：
         - 否則，如果 H 大於 30
            - 球的高度大於 30（夠近了）：停車
            - 否則：
               - 轉彎力量 turn ＝（X − 50）× 0.6：球偏右是正數
               - 左輪＝25＋turn、右輪＝25－turn：邊對準邊前進

**跑起來應該看到：** 把球放在地上，車子會轉向球、開過去，在球前面停下來。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **還很遠就停了？** 把 30 改大。

---

還有問題？歡迎到 [聯絡我們](/contact.md) 留言，附上你用的軟體和程式截圖，我們會幫你看。
