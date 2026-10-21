#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 04：看球轉頭
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：車子原地轉，讓球一直保持在畫面中間：球在左邊就往左轉、在右邊就往右轉、在中間就停；看不到球就原地轉圈找。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 神攝手（學校版）插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_w():
    # 色塊寬度：0 ~ 100，0＝沒找到
    return sensor.read('IR-SEEK')[6]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read('IR-SEEK')[4]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果色塊寬度＝0（沒找到球）
    if read_w() == 0:
        # 看不到球（寬度＝0）：原地慢慢轉圈找
        left_motor.dc(15)
        right_motor.dc(-15)
    # 如果 X 小於 40（左邊）
    elif read_x() < 40:
        # 球在左邊：往左轉
        left_motor.dc(-15)
        right_motor.dc(15)
    # 否則，如果 X 大於 60（右邊）
    elif read_x() > 60:
        # 球在右邊：往右轉
        left_motor.dc(15)
        right_motor.dc(-15)
    else:
        # 球在中間：停住
        left_motor.brake()
        right_motor.brake()
