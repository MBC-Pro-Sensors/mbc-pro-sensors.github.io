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
