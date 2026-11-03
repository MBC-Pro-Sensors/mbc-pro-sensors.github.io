#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 09：從現在的方向再轉 90 度（相對角度）
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：程式開始時歸零一次，然後重複 4 次：先看「現在」的角度，目標就是現在再加 90 度，右轉到目標就停。每次都以「現在的方向」為準，寫法很直覺；但每次轉過頭的幾度會留到下一次，跟範例 8 的「絕對角度」比一比就知道差在哪。
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
# 角度歸零（這時候不要動機器人）
gyro.reset_angle(0)
# 下面的動作重複 4 次
for i in range(4):
    # 目標 target ＝ 現在的角度 ＋ 90：以「現在」為準再轉 90 度
    target = read_angle() + 90
    # 左輪往前、右輪往後：原地向右轉
    left_motor.dc(20)
    right_motor.dc(-20)
    # 等到角度大於等於 target
    while not (read_angle() >= target):
        wait(10)
    # 轉到了：停車
    left_motor.brake()
    right_motor.brake()
    # 停 0.5 秒
    wait(500)
# 轉完 4 次嗶一聲
ev3.speaker.beep()
