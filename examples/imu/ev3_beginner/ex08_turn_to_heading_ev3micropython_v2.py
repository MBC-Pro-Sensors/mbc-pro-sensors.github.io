#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 08：轉到指定方向（絕對角度）
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：程式開始時歸零一次，記住「正前方」是 0 度，之後中間都不再歸零，依序轉到 90 度、180 度、再轉回 0 度。所有方向都以「開始時的正前方」為準，每次轉過頭的一點點誤差不會一次一次累加，比賽最常用這種寫法。
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
# 角度歸零：記住現在的方向是「正前方」0 度（只歸零這一次）
gyro.reset_angle(0)
# 如果現在的角度小於 90
if read_angle() < 90:
    # 還沒到 90 度：原地向右轉（左輪往前、右輪往後）
    left_motor.dc(20)
    right_motor.dc(-20)
    # 等到角度大於等於 90
    while not (read_angle() >= 90):
        wait(10)
else:
    # 超過 90 度：原地向左轉（左輪往後、右輪往前）
    left_motor.dc(-20)
    right_motor.dc(20)
    # 等到角度小於等於 90
    while not (read_angle() <= 90):
        wait(10)
# 到 90 度：停車
left_motor.brake()
right_motor.brake()
# 停 1 秒
wait(1000)
# 如果現在的角度小於 180
if read_angle() < 180:
    # 還沒到 180 度：原地向右轉（左輪往前、右輪往後）
    left_motor.dc(20)
    right_motor.dc(-20)
    # 等到角度大於等於 180
    while not (read_angle() >= 180):
        wait(10)
else:
    # 超過 180 度：原地向左轉（左輪往後、右輪往前）
    left_motor.dc(-20)
    right_motor.dc(20)
    # 等到角度小於等於 180
    while not (read_angle() <= 180):
        wait(10)
# 到 180 度：停車
left_motor.brake()
right_motor.brake()
# 停 1 秒
wait(1000)
# 如果現在的角度小於 0
if read_angle() < 0:
    # 還沒到 0 度：原地向右轉（左輪往前、右輪往後）
    left_motor.dc(20)
    right_motor.dc(-20)
    # 等到角度大於等於 0
    while not (read_angle() >= 0):
        wait(10)
else:
    # 超過 0 度：原地向左轉（左輪往後、右輪往前）
    left_motor.dc(-20)
    right_motor.dc(20)
    # 等到角度小於等於 0
    while not (read_angle() <= 0):
        wait(10)
# 到 0 度：停車
left_motor.brake()
right_motor.brake()
# 停 1 秒
wait(1000)
# 轉完嗶一聲
ev3.speaker.beep()
