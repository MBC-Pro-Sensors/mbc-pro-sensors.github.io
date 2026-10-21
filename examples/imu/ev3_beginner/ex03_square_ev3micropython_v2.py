#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 03：走正方形
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：把「直走 2 秒＋右轉 90 度」重複 4 次，就是一個正方形。
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
# 下面的動作重複 4 次
for i in range(4):
    # 直走
    left_motor.dc(30)
    right_motor.dc(30)
    # 走 2 秒
    wait(2000)
    # 停一下
    left_motor.brake()
    right_motor.brake()
    # 角度歸零
    gyro.reset_angle(0)
    # 左輪往前、右輪往後：原地向右轉（慢一點比較準）
    left_motor.dc(20)
    right_motor.dc(-20)
    # 等到角度的絕對值大於等於 90
    while not (abs(read_angle()) >= 90):
        wait(10)
    # 轉到 90 度：停車
    left_motor.brake()
    right_motor.brake()
# 走完嗶一聲
ev3.speaker.beep()
