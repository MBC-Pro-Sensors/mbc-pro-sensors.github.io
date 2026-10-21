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
