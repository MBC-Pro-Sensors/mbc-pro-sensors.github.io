#!/usr/bin/env pybricks-micropython
# ============================================================
# TOF2 雙眼雷射測距感應器 EV3 新手範例 02：前方 30 公分停車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：TOF2 雙眼雷射插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子往前開，看到前面 30 公分（感應器數字 25）以內有東西就停下來。
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
# 兩輪用 30 的力量往前開
left_motor.dc(30)
right_motor.dc(30)
# 等到距離小於 25（約 30 公分）
while not (read_dist() < 25):
    wait(10)
# 停車
left_motor.brake()
right_motor.brake()
# 嗶一聲
ev3.speaker.beep()
