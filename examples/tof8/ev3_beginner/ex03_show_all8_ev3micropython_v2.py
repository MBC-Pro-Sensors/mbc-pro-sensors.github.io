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
