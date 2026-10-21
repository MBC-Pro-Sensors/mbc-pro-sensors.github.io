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
