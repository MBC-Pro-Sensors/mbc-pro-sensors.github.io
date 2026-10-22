#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF8 八方位雷射測距感應器 EV3 新手範例 06：最近的東西在哪、幾公釐
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF8 八方位雷射插 1 號孔
# 這個範例會做什麼：模式 5 一次給兩個數字：最近的東西有幾公釐、在哪個方向（-7~+7）。比模式 2 的 1.2 公分一格準 12 倍。這個範例把兩個數字顯示在螢幕上，東西靠到 15 公分內就嗶一聲。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF8 八方位雷射插 1 號孔


def read_nearMm():
    # 最近的東西有多遠（公釐）：0 ~ 1200 mm
    return sensor.read('IR-CAL')[0]


def read_nearDir5():
    # 最近的東西在哪個方向：-7 ~ +7（負＝左、正＝右、0 附近＝正前方；±8＝沒有東西）
    return sensor.read('IR-CAL')[1]


# ---------- 程式從這裡開始 ----------
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 第 1 行顯示最近的東西有幾公釐
    ev3.screen.draw_text(0, 0, "NEARMM " + str(read_nearMm()) + "      ", Color.BLACK, Color.WHITE)
    # 第 2 行顯示它在哪個方向（負＝左、正＝右、0＝正前方）
    ev3.screen.draw_text(0, 16, "DIR " + str(read_nearDir5()) + "      ", Color.BLACK, Color.WHITE)
    # 如果最近的東西不到 150 公釐（15 公分）
    if read_nearMm() < 150:
        # 不到 15 公分：嗶一聲
        ev3.speaker.beep()
    # 等 0.1 秒
    wait(100)
