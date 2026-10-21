#!/usr/bin/env pybricks-micropython
# ============================================================
# IMU 陀螺儀感應器 EV3 新手範例 01：看看機器人轉了幾度
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：IMU 陀螺儀插 1 號孔
# 這個範例會做什麼：程式開始時角度歸零，然後用手轉動機器人，螢幕會顯示轉了幾度。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import GyroSensor
from pybricks.parameters import Port, Color
from pybricks.tools import wait

ev3 = EV3Brick()
gyro = GyroSensor(Port.S1)        # IMU 陀螺儀插 1 號孔


def read_angle():
    # 機器人轉了幾度：連續累加（轉一圈＝360，可超過 360）
    return gyro.angle()


# ---------- 程式從這裡開始 ----------
# 先把角度歸零（這時候不要動機器人）
gyro.reset_angle(0)
# 清除螢幕
ev3.screen.clear()
# 一直重複
while True:
    # 螢幕第 1 行顯示目前角度
    ev3.screen.draw_text(0, 0, "ANGLE " + str(read_angle()) + "      ", Color.BLACK, Color.WHITE)
    # 等 0.1 秒
    wait(100)
