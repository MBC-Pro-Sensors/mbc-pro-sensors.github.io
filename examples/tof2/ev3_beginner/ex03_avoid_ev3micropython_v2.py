#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 03：避障車：看到東西就後退轉彎
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子一直往前開，前面太近就後退一點、原地轉一下，再繼續往前。放在有箱子的地上，它會自己繞開。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # TOF2 雙眼雷射插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dist():
    # 左右兩眼的平均距離：0 ~ 100（1 格＝1.2 公分，100＝前面沒東西）
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果前面距離小於 25（約 30 公分）
    if read_dist() < 25:
        # 先後退
        left_motor.dc(-30)
        right_motor.dc(-30)
        # 退 0.5 秒
        wait(500)
        # 左輪往前、右輪往後：原地向右轉
        left_motor.dc(30)
        right_motor.dc(-30)
        # 轉 0.5 秒
        wait(500)
    else:
        # 前面沒東西：往前開
        left_motor.dc(30)
        right_motor.dc(30)
