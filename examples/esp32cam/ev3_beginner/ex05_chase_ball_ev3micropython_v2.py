#!/usr/bin/env pybricks-micropython
# ============================================================
# 神攝手 ESP32CAM 視覺感應器（學校老師創意賽版） EV3 新手範例 05：追球：靠近到一定距離就停
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：神攝手（學校版）插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：把範例 4 改成會往前開：一邊對準球一邊前進，球的高度 H 夠大（夠近）就停下來。
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


def read_h():
    # 色塊高度（越近越大）：0 ~ 75，0＝沒找到
    return sensor.read('IR-SEEK')[7]


def read_x():
    # 色塊左右位置：0 ~ 100（左 → 右），沒找到＝0
    return sensor.read('IR-SEEK')[4]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果寬度＝0（沒找到球）
    if read_w() == 0:
        # 看不到球：原地轉圈找
        left_motor.dc(15)
        right_motor.dc(-15)
    # 否則，如果 H 大於 30
    elif read_h() > 30:
        # 球的高度大於 30（夠近了）：停車
        left_motor.brake()
        right_motor.brake()
    else:
        # 轉彎力量 turn ＝（X − 50）× 0.6：球偏右是正數
        turn = (read_x() - 50) * 0.6
        # 左輪＝25＋turn、右輪＝25－turn：邊對準邊前進
        left_motor.dc(25 + turn)
        right_motor.dc(25 - turn)
