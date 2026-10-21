#!/usr/bin/env pybricks-micropython
# ============================================================
# 掌控者 PS2 2.4G 遙控接收器 EV3 新手範例 02：方向鍵開車
# 軟體：樂高官方 EV3 MicroPython v2.0（microSD 開機卡＋VS Code「LEGO MINDSTORMS EV3 MicroPython」擴充套件）
# 接法：掌控者 PS2插 1 號孔；左輪馬達接 B、右輪馬達接 C
# 這個範例會做什麼：用模式 0 只讀方向鍵，上前進、下後退、左右原地轉，放開就停。
# ============================================================

from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor
from pybricks.iodevices import Ev3devSensor
from pybricks.parameters import Port
from pybricks.tools import wait

ev3 = EV3Brick()
sensor = Ev3devSensor(Port.S1)     # 掌控者 PS2插 1 號孔
left_motor = Motor(Port.B)        # 左輪
right_motor = Motor(Port.C)       # 右輪


def read_dpad():
    # 方向鍵（上 1、右 2、下 4、左 8，同時按會相加）：0 ~ 15
    return sensor.read('IR-PROX')[0]


# ---------- 程式從這裡開始 ----------
# 一直重複
while True:
    # 如果方向鍵＝1（上）
    if read_dpad() == 1:
        # 按上：前進
        left_motor.dc(50)
        right_motor.dc(50)
    # 否則，如果方向鍵＝4（下）
    elif read_dpad() == 4:
        # 按下：後退
        left_motor.dc(-50)
        right_motor.dc(-50)
    # 否則，如果方向鍵＝8（左）
    elif read_dpad() == 8:
        # 按左：原地左轉
        left_motor.dc(-30)
        right_motor.dc(30)
    # 否則，如果方向鍵＝2（右）
    elif read_dpad() == 2:
        # 按右：原地右轉
        left_motor.dc(30)
        right_motor.dc(-30)
    else:
        # 都沒按：停車
        left_motor.brake()
        right_motor.brake()
