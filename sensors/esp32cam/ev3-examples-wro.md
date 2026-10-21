<!-- 這頁由 95_產品網站\EV3新手範例產生器 自動產生，請改產生器再重產，不要直接改這個檔。 -->
# 🧱 EV3 新手範例：神攝手（WRO 版）

[🔙 回到 神攝手（WRO 版）產品頁](/sensors/esp32cam/index.md)

這一頁有 5 個範例，給**完全沒寫過程式**的人：每個範例都有現成的程式檔可以下載，打開就能用，下面也有一步一步的白話說明。建議從範例 1 開始照順序做。

## 開始之前

1. 感應器要用 **EV3 版**（RJ12 接頭），插在 EV3 的 **1 號孔**。
2. **EV3 先開機，再插感應器**。數字一直不變時，拔掉感應器重插一次。
3. 會動的範例用樂高教育版基本車：**左輪馬達接 B、右輪馬達接 C**。
4. 這一版只能用 EV3-G、clev3r、Pybricks，**EV3 Classroom 讀不到**。
5. 用 EV3-G 的話，先下載 MBC 自訂積木 [MBC_Hue12Sensor.ev3b](/examples/esp32cam/ev3_beginner_wro/MBC_Hue12Sensor.ev3b)，在 EV3-G 選「工具 → 積木匯入」匯入一次（每台電腦只要做一次），範例程式才打得開。

**每個範例都有 4 種軟體的程式檔，選你用的那一種就好：**

| 軟體 | 適合誰 | 怎麼打開範例檔 |
|---|---|---|
| **EV3-G 官方軟體** | 學校電腦教室、WRO 比賽隊伍（台灣最常見） | 下載 `.ev3` 檔，在 EV3 官方軟體按「檔案 → 開啟專案」，再按右下角「下載並執行」。 |
| **Pybricks v4** | 想學打字寫程式，用瀏覽器就能寫 | 用 Chrome 打開 [code.pybricks.com](https://code.pybricks.com)，EV3 要先裝 Pybricks 韌體（目前是 Beta 版），把 `.py` 檔拖進編輯區再按執行。 |
| **EV3 MicroPython v2.0** | 已經在用樂高官方 EV3 MicroPython 的人 | 用樂高官方「EV3 MicroPython」microSD 開機卡＋VS Code 擴充套件，把程式內容貼到 `main.py`，按 F5 下載執行。 |
| **clev3r** | 已經在用 clev3r（EV3 Basic）的人 | 在 clev3r 開啟 `.bp` 檔，按「編譯並下載」，在 EV3 的檔案選單找到 `MBC_` 開頭的資料夾執行。 |

## 這頁會用到的 4 個數值

| 名稱 | 範圍 | 從哪裡讀 |
|---|---|---|
| **色點 S1 的顏色碼** | 0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開 | 色點 S1 |
| **色點 S1 的鮮豔度** | 0 ~ 255（黑色固定 1） | 色點 S1 |
| **色點 S3 的顏色碼** | 0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開 | 色點 S3 |
| **色點 S2 的顏色碼** | 0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開 | 色點 S2 |

**範例列表：**

1. [色點 S1 看到什麼顏色？](#%E7%AF%84%E4%BE%8B-1%EF%BC%9A%E8%89%B2%E9%BB%9E-s1-%E7%9C%8B%E5%88%B0%E4%BB%80%E9%BA%BC%E9%A1%8F%E8%89%B2%EF%BC%9F)
2. [顏色播報：螢幕顯示顏色名稱](#%E7%AF%84%E4%BE%8B-2%EF%BC%9A%E9%A1%8F%E8%89%B2%E6%92%AD%E5%A0%B1%EF%BC%9A%E8%9E%A2%E5%B9%95%E9%A1%AF%E7%A4%BA%E9%A1%8F%E8%89%B2%E5%90%8D%E7%A8%B1)
3. [交通號誌柱：紅停綠走](#%E7%AF%84%E4%BE%8B-3%EF%BC%9A%E4%BA%A4%E9%80%9A%E8%99%9F%E8%AA%8C%E6%9F%B1%EF%BC%9A%E7%B4%85%E5%81%9C%E7%B6%A0%E8%B5%B0)
4. [看到黑線就停車](#%E7%AF%84%E4%BE%8B-4%EF%BC%9A%E7%9C%8B%E5%88%B0%E9%BB%91%E7%B7%9A%E5%B0%B1%E5%81%9C%E8%BB%8A)
5. [左中右三個色點：紅色在哪邊？](#%E7%AF%84%E4%BE%8B-5%EF%BC%9A%E5%B7%A6%E4%B8%AD%E5%8F%B3%E4%B8%89%E5%80%8B%E8%89%B2%E9%BB%9E%EF%BC%9A%E7%B4%85%E8%89%B2%E5%9C%A8%E5%93%AA%E9%82%8A%EF%BC%9F)

---

## 範例 1：色點 S1 看到什麼顏色？

**這個範例會做什麼：** WRO 版有 12 個色點，每個色點回報「那一個位置現在的顏色」。先設定 S1，把它的顏色碼和鮮豔度顯示在螢幕上。

**要準備什麼：**

- 神攝手（WRO 版）（EV3 版）
- EV3 主機
- 各種顏色的積木

**怎麼接：** 神攝手（WRO 版）插 1 號孔，裝在車頭、鏡頭朝前。相機設定：側鍵短按進選單 → 左右鍵選 S1 → 確認 → 移動十字游標到要看的位置（X、Y）→ 調容許範圍 → 確認存檔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner_wro/ex01_show_s1.ev3) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner_wro/ex01_show_s1_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner_wro/ex01_show_s1_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner_wro/ex01_show_s1.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/wro-ex01_show_s1-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/wro-ex01_show_s1-ev3g.webp" alt="神攝手（WRO 版） 色點 S1 看到什麼顏色？（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 01：色點 S1 看到什麼顏色？
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（WRO 版）插 1 號孔
# 這個範例會做什麼：WRO 版有 12 個色點，每個色點回報「那一個位置現在的顏色」。先設定 S1，把它的顏色碼和鮮豔度顯示在螢幕上。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（WRO 版）插 1 號孔


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read(0)
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


def read_sta1():
    # 色點 S1 的鮮豔度：0 ~ 255（黑色固定 1）
    return slot_byte(1)


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行：S1 的顏色碼
    ev3.screen.draw_text(0, 0, "S1 " + str(read_hue1()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行：S1 的鮮豔度
    ev3.screen.draw_text(0, 16, "T1 " + str(read_sta1()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 01：色點 S1 看到什麼顏色？
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（WRO 版）插 1 號孔
# 這個範例會做什麼：WRO 版有 12 個色點，每個色點回報「那一個位置現在的顏色」。先設定 S1，把它的顏色碼和鮮豔度顯示在螢幕上。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（WRO 版）插 1 號孔


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read('HUE12')
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


def read_sta1():
    # 色點 S1 的鮮豔度：0 ~ 255（黑色固定 1）
    return slot_byte(1)


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行：S1 的顏色碼
    ev3.screen.draw_text(0, 0, "S1 " + str(read_hue1()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行：S1 的鮮豔度
    ev3.screen.draw_text(0, 16, "T1 " + str(read_sta1()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 01：色點 S1 看到什麼顏色？
' 軟體：clev3r
' 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：WRO 版有 12 個色點，每個色點回報「那一個位置現在的顏色」。先設定 S1，把它的顏色碼和鮮豔度顯示在螢幕上。
' 讀值：hue1＝色點 S1 的顏色碼（0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開）；sta1＝色點 S1 的鮮豔度（0 ~ 255（黑色固定 1））
' ============================================================
folder "prjs" "MBC_esp32cam_wro_ex01"

' 神攝手（WRO 版）插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 清除螢幕
LCD.Clear()
' 一直重複
While "True"
  ' 第 1 行：S1 的顏色碼
  LCD.Text(1, 0, 0, 1, "S1 " + Math.Remainder(Sensor.ReadRawValue(1, 0), 256) + "      ")
  ' 第 2 行：S1 的鮮豔度
  LCD.Text(1, 0, 16, 1, "T1 " + Math.Remainder(Math.Floor(Sensor.ReadRawValue(1, 0) / 256), 256) + "      ")
  ' 等 0.1 秒
  Program.Delay(100)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 清除螢幕
1. 一直重複
   - 第 1 行：S1 的顏色碼
   - 第 2 行：S1 的鮮豔度
   - 等 0.1 秒

**跑起來應該看到：** 在 S1 的位置放不同顏色的積木，顏色碼會跟著變：紅約 0、黃約 40、綠約 80、藍約 160。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **數字一直是 255？** 那個色點還沒設定（沒啟用）。照上面的相機設定做一次 S1。
- **顏色數字的意思？** 0~239 是彩色：紅 0（或 239）、黃 40、綠 80、青 120、藍 160、紫 200；240~245 白；246~250 黑。
- **相機畫面上下顛倒？** 到選單把 V-Flip 打開。改完方向後，色點的位置要重新設定一次。
- **原本燒的是學校版，改燒 WRO 版後色點怪怪的？** 兩版的設定存在同一個地方，請先在選單做一次 Reset Default，再重新設定色點。

---

## 範例 2：顏色播報：螢幕顯示顏色名稱

**這個範例會做什麼：** 把顏色碼分成幾段，判斷是紅、綠、藍、白、黑，在螢幕上顯示英文名稱並嗶一聲。

**要準備什麼：**

- 神攝手（WRO 版）（EV3 版）
- EV3 主機
- 紅、綠、藍色積木

**怎麼接：** 神攝手（WRO 版）插 1 號孔，裝在車頭、鏡頭朝前。相機設定：側鍵短按進選單 → 左右鍵選 S1 → 確認 → 移動十字游標到要看的位置（X、Y）→ 調容許範圍 → 確認存檔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner_wro/ex02_color_name.ev3) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner_wro/ex02_color_name_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner_wro/ex02_color_name_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner_wro/ex02_color_name.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/wro-ex02_color_name-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/wro-ex02_color_name-ev3g.webp" alt="神攝手（WRO 版） 顏色播報：螢幕顯示顏色名稱（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 02：顏色播報：螢幕顯示顏色名稱
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（WRO 版）插 1 號孔
# 這個範例會做什麼：把顏色碼分成幾段，判斷是紅、綠、藍、白、黑，在螢幕上顯示英文名稱並嗶一聲。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（WRO 版）插 1 號孔


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read(0)
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 把 S1 的顏色碼存進 hue
    hue = read_hue1()
    # 清除螢幕
    ev3.screen.clear()
    # 如果是紅色
    if hue < 15 or (hue > 225 and hue < 240):
        # 0~14 或 226~239：紅色
        ev3.screen.draw_text(0, 0, "RED      ", Color.BLACK, Color.WHITE)
    # 否則，如果是綠色
    elif hue >= 60 and hue <= 100:
        # 60~100：綠色
        ev3.screen.draw_text(0, 0, "GREEN      ", Color.BLACK, Color.WHITE)
    # 否則，如果是藍色
    elif hue >= 140 and hue <= 180:
        # 140~180：藍色
        ev3.screen.draw_text(0, 0, "BLUE      ", Color.BLACK, Color.WHITE)
    # 否則，如果是白色
    elif hue >= 240 and hue <= 245:
        # 240~245：白色
        ev3.screen.draw_text(0, 0, "WHITE      ", Color.BLACK, Color.WHITE)
    # 否則，如果是黑色
    elif hue >= 246 and hue <= 250:
        # 246~250：黑色
        ev3.screen.draw_text(0, 0, "BLACK      ", Color.BLACK, Color.WHITE)
    else:
        # 其他顏色
        ev3.screen.draw_text(0, 0, "OTHER      ", Color.BLACK, Color.WHITE)
    # 等 0.3 秒
    wait(300)
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 02：顏色播報：螢幕顯示顏色名稱
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（WRO 版）插 1 號孔
# 這個範例會做什麼：把顏色碼分成幾段，判斷是紅、綠、藍、白、黑，在螢幕上顯示英文名稱並嗶一聲。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（WRO 版）插 1 號孔


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read('HUE12')
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 把 S1 的顏色碼存進 hue
    hue = read_hue1()
    # 清除螢幕
    ev3.screen.clear()
    # 如果是紅色
    if hue < 15 or (hue > 225 and hue < 240):
        # 0~14 或 226~239：紅色
        ev3.screen.draw_text(0, 0, "RED      ", Color.BLACK, Color.WHITE)
    # 否則，如果是綠色
    elif hue >= 60 and hue <= 100:
        # 60~100：綠色
        ev3.screen.draw_text(0, 0, "GREEN      ", Color.BLACK, Color.WHITE)
    # 否則，如果是藍色
    elif hue >= 140 and hue <= 180:
        # 140~180：藍色
        ev3.screen.draw_text(0, 0, "BLUE      ", Color.BLACK, Color.WHITE)
    # 否則，如果是白色
    elif hue >= 240 and hue <= 245:
        # 240~245：白色
        ev3.screen.draw_text(0, 0, "WHITE      ", Color.BLACK, Color.WHITE)
    # 否則，如果是黑色
    elif hue >= 246 and hue <= 250:
        # 246~250：黑色
        ev3.screen.draw_text(0, 0, "BLACK      ", Color.BLACK, Color.WHITE)
    else:
        # 其他顏色
        ev3.screen.draw_text(0, 0, "OTHER      ", Color.BLACK, Color.WHITE)
    # 等 0.3 秒
    wait(300)
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 02：顏色播報：螢幕顯示顏色名稱
' 軟體：clev3r
' 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：把顏色碼分成幾段，判斷是紅、綠、藍、白、黑，在螢幕上顯示英文名稱並嗶一聲。
' 讀值：hue1＝色點 S1 的顏色碼（0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開）
' ============================================================
folder "prjs" "MBC_esp32cam_wro_ex02"

' 神攝手（WRO 版）插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複
While "True"
  ' 把 S1 的顏色碼存進 hue
  hue = Math.Remainder(Sensor.ReadRawValue(1, 0), 256)
  ' 清除螢幕
  LCD.Clear()
  ' 如果是紅色
  If (hue < 15) Or ((hue > 225) And (hue < 240)) Then
    ' 0~14 或 226~239：紅色
    LCD.Text(1, 0, 0, 1, "RED      ")
  ' 否則，如果是綠色
  ElseIf (hue >= 60) And (hue <= 100) Then
    ' 60~100：綠色
    LCD.Text(1, 0, 0, 1, "GREEN      ")
  ' 否則，如果是藍色
  ElseIf (hue >= 140) And (hue <= 180) Then
    ' 140~180：藍色
    LCD.Text(1, 0, 0, 1, "BLUE      ")
  ' 否則，如果是白色
  ElseIf (hue >= 240) And (hue <= 245) Then
    ' 240~245：白色
    LCD.Text(1, 0, 0, 1, "WHITE      ")
  ' 否則，如果是黑色
  ElseIf (hue >= 246) And (hue <= 250) Then
    ' 246~250：黑色
    LCD.Text(1, 0, 0, 1, "BLACK      ")
  Else
    ' 其他顏色
    LCD.Text(1, 0, 0, 1, "OTHER      ")
  EndIf
  ' 等 0.3 秒
  Program.Delay(300)
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 把 S1 的顏色碼存進 hue
   - 清除螢幕
   - 如果是紅色
      - 0~14 或 226~239：紅色
      - 否則：
         - 否則，如果是綠色
            - 60~100：綠色
            - 否則：
               - 否則，如果是藍色
                  - 140~180：藍色
                  - 否則：
                     - 否則，如果是白色
                        - 240~245：白色
                        - 否則：
                           - 否則，如果是黑色
                              - 246~250：黑色
                              - 否則：
                                 - 其他顏色
   - 等 0.3 秒

**跑起來應該看到：** 把紅、綠、藍積木放到 S1 的位置，螢幕分別顯示 RED、GREEN、BLUE。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **數字一直是 255？** 那個色點還沒設定（沒啟用）。照上面的相機設定做一次 S1。
- **顏色數字的意思？** 0~239 是彩色：紅 0（或 239）、黃 40、綠 80、青 120、藍 160、紫 200；240~245 白；246~250 黑。
- **相機畫面上下顛倒？** 到選單把 V-Flip 打開。改完方向後，色點的位置要重新設定一次。
- **原本燒的是學校版，改燒 WRO 版後色點怪怪的？** 兩版的設定存在同一個地方，請先在選單做一次 Reset Default，再重新設定色點。

---

## 範例 3：交通號誌柱：紅停綠走

**這個範例會做什麼：** WRO 未來工程師常見的紅綠柱子：S1 對準前方，看到紅色就停車、看到綠色就往前開。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 神攝手（WRO 版）（EV3 版）
- 紅色、綠色的柱子或積木

**怎麼接：** 神攝手（WRO 版）插 1 號孔，裝在車頭、鏡頭朝前。相機設定：側鍵短按進選單 → 左右鍵選 S1 → 確認 → 移動十字游標到要看的位置（X、Y）→ 調容許範圍 → 確認存檔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner_wro/ex03_traffic_pillar.ev3) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner_wro/ex03_traffic_pillar_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner_wro/ex03_traffic_pillar_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner_wro/ex03_traffic_pillar.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/wro-ex03_traffic_pillar-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/wro-ex03_traffic_pillar-ev3g.webp" alt="神攝手（WRO 版） 交通號誌柱：紅停綠走（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 03：交通號誌柱：紅停綠走
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：WRO 未來工程師常見的紅綠柱子：S1 對準前方，看到紅色就停車、看到綠色就往前開。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（WRO 版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read(0)
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 把 S1 的顏色碼存進 hue
    hue = read_hue1()
    # 如果是綠色（60~100）
    if hue >= 60 and hue <= 100:
        # 綠色：往前開
        left_motor.dc(30)
        right_motor.dc(30)
    # 否則，如果是紅色
    elif hue < 15 or (hue > 225 and hue < 240):
        # 紅色：停車
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 03：交通號誌柱：紅停綠走
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：WRO 未來工程師常見的紅綠柱子：S1 對準前方，看到紅色就停車、看到綠色就往前開。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（WRO 版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read('HUE12')
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 把 S1 的顏色碼存進 hue
    hue = read_hue1()
    # 如果是綠色（60~100）
    if hue >= 60 and hue <= 100:
        # 綠色：往前開
        left_motor.dc(30)
        right_motor.dc(30)
    # 否則，如果是紅色
    elif hue < 15 or (hue > 225 and hue < 240):
        # 紅色：停車
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 03：交通號誌柱：紅停綠走
' 軟體：clev3r
' 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：WRO 未來工程師常見的紅綠柱子：S1 對準前方，看到紅色就停車、看到綠色就往前開。
' 讀值：hue1＝色點 S1 的顏色碼（0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開）
' ============================================================
folder "prjs" "MBC_esp32cam_wro_ex03"

' 神攝手（WRO 版）插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複
While "True"
  ' 把 S1 的顏色碼存進 hue
  hue = Math.Remainder(Sensor.ReadRawValue(1, 0), 256)
  ' 如果是綠色（60~100）
  If (hue >= 60) And (hue <= 100) Then
    ' 綠色：往前開
    Motor.StartPower("B", 30)
    Motor.StartPower("C", 30)
  ' 否則，如果是紅色
  ElseIf (hue < 15) Or ((hue > 225) And (hue < 240)) Then
    ' 紅色：停車
    Motor.Stop("BC", "True")
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 把 S1 的顏色碼存進 hue
   - 如果是綠色（60~100）
      - 綠色：往前開
      - 否則：
         - 否則，如果是紅色
            - 紅色：停車

**跑起來應該看到：** 前面放綠色柱子，車子往前開；換成紅色，車子停下來。其他顏色會維持原本的動作。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **數字一直是 255？** 那個色點還沒設定（沒啟用）。照上面的相機設定做一次 S1。
- **顏色數字的意思？** 0~239 是彩色：紅 0（或 239）、黃 40、綠 80、青 120、藍 160、紫 200；240~245 白；246~250 黑。
- **相機畫面上下顛倒？** 到選單把 V-Flip 打開。改完方向後，色點的位置要重新設定一次。
- **原本燒的是學校版，改燒 WRO 版後色點怪怪的？** 兩版的設定存在同一個地方，請先在選單做一次 Reset Default，再重新設定色點。

---

## 範例 4：看到黑線就停車

**這個範例會做什麼：** 把 S1 設在畫面下方對著地面，車子往前開，S1 看到黑色（246~250）就停。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 神攝手（WRO 版）（EV3 版）
- 白色地面上貼一條黑膠帶

**怎麼接：** 神攝手（WRO 版）插 1 號孔，裝在車頭、鏡頭朝前。相機設定：側鍵短按進選單 → 左右鍵選 S1 → 確認 → 移動十字游標到畫面下方（對著地面）（X、Y）→ 調容許範圍 → 確認存檔。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner_wro/ex04_stop_at_black.ev3) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner_wro/ex04_stop_at_black_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner_wro/ex04_stop_at_black_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner_wro/ex04_stop_at_black.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/wro-ex04_stop_at_black-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/wro-ex04_stop_at_black-ev3g.webp" alt="神攝手（WRO 版） 看到黑線就停車（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 04：看到黑線就停車
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：把 S1 設在畫面下方對著地面，車子往前開，S1 看到黑色（246~250）就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（WRO 版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read(0)
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


# ---------- 程式從這裡開始 ----------
# 往前開
left_motor.dc(25)
right_motor.dc(25)
# 等到 S1 的顏色碼在 246~250（黑色）
while not (read_hue1() >= 246 and read_hue1() <= 250):
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
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 04：看到黑線就停車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：把 S1 設在畫面下方對著地面，車子往前開，S1 看到黑色（246~250）就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（WRO 版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read('HUE12')
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


# ---------- 程式從這裡開始 ----------
# 往前開
left_motor.dc(25)
right_motor.dc(25)
# 等到 S1 的顏色碼在 246~250（黑色）
while not (read_hue1() >= 246 and read_hue1() <= 250):
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
' 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 04：看到黑線就停車
' 軟體：clev3r
' 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：把 S1 設在畫面下方對著地面，車子往前開，S1 看到黑色（246~250）就停。
' 讀值：hue1＝色點 S1 的顏色碼（0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開）
' ============================================================
folder "prjs" "MBC_esp32cam_wro_ex04"

' 神攝手（WRO 版）插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 往前開
Motor.StartPower("B", 25)
Motor.StartPower("C", 25)
' 等到 S1 的顏色碼在 246~250（黑色）
While (Math.Remainder(Sensor.ReadRawValue(1, 0), 256) < 246) Or (Math.Remainder(Sensor.ReadRawValue(1, 0), 256) > 250)
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

1. 往前開
1. 等到 S1 的顏色碼在 246~250（黑色）
1. 停車
1. 嗶一聲

**跑起來應該看到：** 車子往前開，壓到黑線時停下來並嗶一聲。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **數字一直是 255？** 那個色點還沒設定（沒啟用）。照上面的相機設定做一次 S1。
- **顏色數字的意思？** 0~239 是彩色：紅 0（或 239）、黃 40、綠 80、青 120、藍 160、紫 200；240~245 白；246~250 黑。
- **相機畫面上下顛倒？** 到選單把 V-Flip 打開。改完方向後，色點的位置要重新設定一次。
- **原本燒的是學校版，改燒 WRO 版後色點怪怪的？** 兩版的設定存在同一個地方，請先在選單做一次 Reset Default，再重新設定色點。

---

## 範例 5：左中右三個色點：紅色在哪邊？

**這個範例會做什麼：** 設定 S1（左）、S2（中）、S3（右）三個色點，看紅色出現在哪一個，車子就往那邊轉。

**要準備什麼：**

- EV3 主機＋教育版基本車（左輪接 B、右輪接 C）
- 神攝手（WRO 版）（EV3 版）
- 一個紅色積木

**怎麼接：** 神攝手（WRO 版）插 1 號孔，裝在車頭、鏡頭朝前。相機設定：把 S1 設在畫面左邊、S2 中間、S3 右邊，三個點高度一樣。

**下載程式（選你用的軟體）：**

[⬇ EV3-G 官方軟體](/examples/esp32cam/ev3_beginner_wro/ex05_three_spots.ev3) ｜ [⬇ Pybricks v4](/examples/esp32cam/ev3_beginner_wro/ex05_three_spots_pybricks_v4.py) ｜ [⬇ EV3 MicroPython v2.0](/examples/esp32cam/ev3_beginner_wro/ex05_three_spots_ev3micropython_v2.py) ｜ [⬇ clev3r](/examples/esp32cam/ev3_beginner_wro/ex05_three_spots.bp)

<details><summary>EV3-G 官方軟體 積木圖</summary>

<a href="/images/sensors/esp32cam/ev3-beginner/wro-ex05_three_spots-ev3g.webp" target="_blank" rel="noopener"><img src="/images/sensors/esp32cam/ev3-beginner/wro-ex05_three_spots-ev3g.webp" alt="神攝手（WRO 版） 左中右三個色點：紅色在哪邊？（EV3-G 官方軟體）" loading="lazy" style="max-width:100%;" /></a>

（圖太小看不清楚？點一下圖片就會開原尺寸。）

</details>

<details><summary>Pybricks v4 程式碼</summary>

```python
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 05：左中右三個色點：紅色在哪邊？
# 軟體：Pybricks v4（用 Chrome 打開 https://code.pybricks.com ，EV3 版目前是 Beta）
# 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：設定 S1（左）、S2（中）、S3（右）三個色點，看紅色出現在哪一個，車子就往那邊轉。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import LUMPDevice
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = LUMPDevice(Port.S1)       # 神攝手（WRO 版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read(0)
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


def read_hue3():
    # 色點 S3 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(4)


def read_hue2():
    # 色點 S2 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(2)


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果 S1 是紅色
    if read_hue1() < 15 or (read_hue1() > 225 and read_hue1() < 240):
        # 紅色在左邊（S1）：往左轉
        left_motor.dc(-15)
        right_motor.dc(15)
    # 否則，如果 S3 是紅色
    elif read_hue3() < 15 or (read_hue3() > 225 and read_hue3() < 240):
        # 紅色在右邊（S3）：往右轉
        left_motor.dc(15)
        right_motor.dc(-15)
    # 否則，如果 S2 是紅色
    elif read_hue2() < 15 or (read_hue2() > 225 and read_hue2() < 240):
        # 紅色在中間（S2）：往前開
        left_motor.dc(25)
        right_motor.dc(25)
    else:
        # 沒看到紅色：停住
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>EV3 MicroPython v2.0 程式碼</summary>

```python
#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 05：左中右三個色點：紅色在哪邊？
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：設定 S1（左）、S2（中）、S3（右）三個色點，看紅色出現在哪一個，車子就往那邊轉。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（WRO 版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def slot_byte(n):
    # 神攝手 WRO 版每 3 個 byte 打包成一個數字，這裡把第 n 個 byte 拆出來
    data = sensor.read('HUE12')
    return (data[n // 3] >> (8 * (n % 3))) & 255


def read_hue1():
    # 色點 S1 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(0)


def read_hue3():
    # 色點 S3 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(4)


def read_hue2():
    # 色點 S2 的顏色碼：0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開
    return slot_byte(2)


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果 S1 是紅色
    if read_hue1() < 15 or (read_hue1() > 225 and read_hue1() < 240):
        # 紅色在左邊（S1）：往左轉
        left_motor.dc(-15)
        right_motor.dc(15)
    # 否則，如果 S3 是紅色
    elif read_hue3() < 15 or (read_hue3() > 225 and read_hue3() < 240):
        # 紅色在右邊（S3）：往右轉
        left_motor.dc(15)
        right_motor.dc(-15)
    # 否則，如果 S2 是紅色
    elif read_hue2() < 15 or (read_hue2() > 225 and read_hue2() < 240):
        # 紅色在中間（S2）：往前開
        left_motor.dc(25)
        right_motor.dc(25)
    else:
        # 沒看到紅色：停住
        left_motor.brake()
        right_motor.brake()
```

</details>

<details><summary>clev3r 程式碼</summary>

```vb
' ============================================================
' 神攝手 ESP32CAM 視覺感應器（WRO 2026 版 HUE12） EV3 新手範例 05：左中右三個色點：紅色在哪邊？
' 軟體：clev3r
' 接法：神攝手（WRO 版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
' 這個範例會做什麼：設定 S1（左）、S2（中）、S3（右）三個色點，看紅色出現在哪一個，車子就往那邊轉。
' 讀值：hue1＝色點 S1 的顏色碼（0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開）；hue3＝色點 S3 的顏色碼（0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開）；hue2＝色點 S2 的顏色碼（0 ~ 239 彩色（紅 0/239、黃 40、綠 80、青 120、藍 160、紫 200）；240~245 白；246~250 黑；255＝色點沒開）
' ============================================================
folder "prjs" "MBC_esp32cam_wro_ex05"

' 神攝手（WRO 版）插 1 號孔，使用模式 0
Sensor.SetMode(1, 0)
LCD.Clear()

' 一直重複
While "True"
  ' 如果 S1 是紅色
  If (Math.Remainder(Sensor.ReadRawValue(1, 0), 256) < 15) Or ((Math.Remainder(Sensor.ReadRawValue(1, 0), 256) > 225) And (Math.Remainder(Sensor.ReadRawValue(1, 0), 256) < 240)) Then
    ' 紅色在左邊（S1）：往左轉
    Motor.StartPower("B", -15)
    Motor.StartPower("C", 15)
  ' 否則，如果 S3 是紅色
  ElseIf (Math.Remainder(Math.Floor(Sensor.ReadRawValue(1, 1) / 256), 256) < 15) Or ((Math.Remainder(Math.Floor(Sensor.ReadRawValue(1, 1) / 256), 256) > 225) And (Math.Remainder(Math.Floor(Sensor.ReadRawValue(1, 1) / 256), 256) < 240)) Then
    ' 紅色在右邊（S3）：往右轉
    Motor.StartPower("B", 15)
    Motor.StartPower("C", -15)
  ' 否則，如果 S2 是紅色
  ElseIf (Math.Remainder(Math.Floor(Sensor.ReadRawValue(1, 0) / 65536), 256) < 15) Or ((Math.Remainder(Math.Floor(Sensor.ReadRawValue(1, 0) / 65536), 256) > 225) And (Math.Remainder(Math.Floor(Sensor.ReadRawValue(1, 0) / 65536), 256) < 240)) Then
    ' 紅色在中間（S2）：往前開
    Motor.StartPower("B", 25)
    Motor.StartPower("C", 25)
  Else
    ' 沒看到紅色：停住
    Motor.Stop("BC", "True")
  EndIf
EndWhile
```

</details>

**一步一步看懂程式：**

1. 一直重複
   - 如果 S1 是紅色
      - 紅色在左邊（S1）：往左轉
      - 否則：
         - 否則，如果 S3 是紅色
            - 紅色在右邊（S3）：往右轉
            - 否則：
               - 否則，如果 S2 是紅色
                  - 紅色在中間（S2）：往前開
                  - 否則：
                     - 沒看到紅色：停住

**跑起來應該看到：** 紅色積木放在車子左前方，車子往左轉；放右前方往右轉；正前方就往前開。

**常見問題：**

- **相機螢幕一直停在「LEGO Sync...」？** 相機要接上「已經開機」的 EV3 才會啟動。EV3 先開機，再插相機的線。
- **數字一直是 255？** 那個色點還沒設定（沒啟用）。照上面的相機設定做一次 S1。
- **顏色數字的意思？** 0~239 是彩色：紅 0（或 239）、黃 40、綠 80、青 120、藍 160、紫 200；240~245 白；246~250 黑。
- **相機畫面上下顛倒？** 到選單把 V-Flip 打開。改完方向後，色點的位置要重新設定一次。
- **原本燒的是學校版，改燒 WRO 版後色點怪怪的？** 兩版的設定存在同一個地方，請先在選單做一次 Reset Default，再重新設定色點。

---

還有問題？歡迎到 [聯絡我們](/contact.md) 留言，附上你用的軟體和程式截圖，我們會幫你看。
