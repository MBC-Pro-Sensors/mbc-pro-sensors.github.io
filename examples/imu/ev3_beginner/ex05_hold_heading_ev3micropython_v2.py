#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 05：不倒的指南針：自己轉回原來的方向
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子停在原地。你用手把它轉走，它會自己原地轉回原來的方向。這就是比賽中「轉回正前方」的基本做法。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 角度歸零：記住現在的方向
gyro.reset_angle(0)
# 一直重複
while True:
    # 修正力量 fix ＝ 角度 × 1：轉走越多，轉回來越用力
    fix = read_angle() * 1
    # 左輪＝－fix、右輪＝＋fix：原地往反方向轉回去
    left_motor.dc(0 - fix)
    right_motor.dc(fix)
