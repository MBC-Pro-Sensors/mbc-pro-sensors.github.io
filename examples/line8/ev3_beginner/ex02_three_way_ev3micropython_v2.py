#!/usr/bin/env pybricks-micropython
# ============================================================
# 循行者 8 路循線感應器 EV3 新手範例 02：三段式循線：偏左、偏右、直走
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：循行者 8 路插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用「如果…就…否則…」讓車子沿著黑線走：線偏左就往左轉，線偏右就往右轉，在中間就直走。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 循行者 8 路插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_pos():
    # 線的位置：-8 ~ +8，0＝線在正中間
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複檢查線的位置
while True:
    # 如果線的位置小於 -2（偏左）
    if read_pos() < (-2):
        # 線偏左：左輪慢、右輪快，車子往左修
        left_motor.dc(10)
        right_motor.dc(40)
    # 否則，如果線的位置大於 2（偏右）
    elif read_pos() > 2:
        # 線偏右：左輪快、右輪慢，車子往右修
        left_motor.dc(40)
        right_motor.dc(10)
    else:
        # 線在中間：兩輪一樣快，直直走
        left_motor.dc(30)
        right_motor.dc(30)
